/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:    DTMF RTP payload decoder for named telephone events
*                and tones.
*
*/




// INCLUDES
#include <mmf/server/mmfdatabuffer.h>
#include "dtmfpayloaddecoder.h"
#include "streamformatter.h"

// CONSTANTS
const TInt KBitIndex = 8;
const TInt KReservedBits = 4;
const TUint KMaxEventInteger = 9;

#ifdef VOIP_TRACE_ENABLED
#include <voip_trace.h>
#endif

// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// CDTMFPayloadDecoder::CDTMFPayloadDecoder
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CDTMFPayloadDecoder::CDTMFPayloadDecoder()
    {
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadDecoder::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CDTMFPayloadDecoder* CDTMFPayloadDecoder::NewL()
    {
    DP_DTMF_DECODE( _L("CDTMFPayloadDecoder::NewL") );
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_DTMF_PLF_DEC_NEWL );
    #endif
    
    CDTMFPayloadDecoder* self = new( ELeave ) CDTMFPayloadDecoder;
    return self;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadDecoder::~CDTMFPayloadDecoder
// Destructor.
// ---------------------------------------------------------------------------
//
CDTMFPayloadDecoder::~CDTMFPayloadDecoder()
    {
    DP_DTMF_DECODE( _L("CDTMFPayloadDecoder::~CDTMFPayloadDecoder") );
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_DTMF_PLF_DEC_DESTRUCTOR );
    #endif
    
    iEventArray.Reset();
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadDecoder::DecodeEventPayload
// Decodes one DTMF digit or mutually exclusive events from Event Payload
// Format packet.
// ---------------------------------------------------------------------------
//
/*
    0                   1                   2                   3 
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
   |     event     |E|R| volume    |          duration             | 
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
*/
TInt CDTMFPayloadDecoder::DecodeEventPayload( CMMFBuffer* aSourceBuffer,
    RArray<TDTMFEventPayloadInfo>& aEventInfos )
    {
    DP_DTMF_DECODE( _L("CDTMFPayloadDecoder::DecodeEventPayload") );
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x %x", MCC_TRACE, MCC_DTMF_PLF_DEC_DECEVENTPAYLOAD,
        aSourceBuffer );
    #endif
    
    if ( EDTMFPayloadFormatRedEvents == iPayloadFormat )
        {
        return DecodeRedundantEventPayload( aSourceBuffer, aEventInfos );
        }
        
    if ( aSourceBuffer )
        {
        TDes8& dataDes = 
            static_cast<CMMFDataBuffer*>( aSourceBuffer )->Data();
        const TUint8* seekPtr = dataDes.Ptr();
        TInt events = dataDes.Length( ) / KEventBlockLengthInBytes;
        
        TStreamDecoder streamDecoder;
        
        streamDecoder.Initialize( const_cast<TUint8*>( seekPtr ), 0, 0 );
        
        while ( events-- )
            {
            // Decode Event Field
            TInt eventCode = streamDecoder.Decode( KEventFieldBits );
            TChar charRepresentation;
            if ( KErrNone == ConvertToChar( eventCode, charRepresentation ) )
                {
                iDecodedEventInfo.SetEvent( charRepresentation );
                
                // Decode end bit for final packet
                iDecodedEventInfo.SetEndBit( streamDecoder.Decode( 1 ) );
                
                // Decode reserved bit
                streamDecoder.Decode( 1 );
                
                // Decode volume field.
                // Volume field must be ignored if event 
                // is not DTMF digit (RFC 2833)
                if ( KEventCodeForD > eventCode )
                    {
                    iDecodedEventInfo.SetVolume( 
                                        streamDecoder.Decode(KVolFieldBits) );
                    }
                else
                    {
                    streamDecoder.Decode( KVolFieldBits );
                    iDecodedEventInfo.SetVolume( 0 );
                    }
                
                // Decode duration field
                iDecodedEventInfo.SetDuration( 
                                streamDecoder.Decode(KDurationFieldBits) );
                
                // Time stamp from RTP header
                iDecodedEventInfo.SetTimeStamp( iCurTimeStamp );
                
                aEventInfos.Append( iDecodedEventInfo );
                        
                DP_DTMF_DECODE5(
                    _L( "Decoded event: %d, EndBit: %d, Volume: %d, Duration: %d" ),
                                 static_cast<TUint>( iDecodedEventInfo.Event() ), 
                                 iDecodedEventInfo.EndBit(), 
                                 iDecodedEventInfo.Volume(), 
                                 iDecodedEventInfo.Duration() );
                }
            else
                {
                DP_DTMF_DECODE( _L( "CDTMFPayloadDecoder::DecodeEventPayload - Not supported event! ") );
                }                
            }
        
        return KErrNone;    
        }
    else
        {
        return KErrArgument;
        }
    }    

// ---------------------------------------------------------------------------
// CDTMFPayloadDecoder::DecodeRedundantEventPayload
// Decodes events from Event Payload Format where is used Multi-Event
// Redundancy.
// ---------------------------------------------------------------------------
//
/*
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
   |F|   block PT  |     timestamp offset      |   block length    | 
   |1|     97      |            11200          |         4         | 
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
   |F|   block PT  |     timestamp offset      |   block length    | 
   |1|     97      |   11200 - 6400 = 4800     |         4         | 
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
   |F|   Block PT  | 
   |0|     97      | 
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
   |     digit     |E R| volume    |          duration             | 
   |       9       |1 0|     7     |             1600              |  
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
   |     digit     |E R| volume    |          duration             | 
   |       1       |1 0|    10     |             2000              |  
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
   |     digit     |E R| volume    |          duration             | 
   |       1       |0 0|    20     |              400              |  
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
   */
TInt CDTMFPayloadDecoder::DecodeRedundantEventPayload( 
    CMMFBuffer* aSourceBuffer,
    RArray<TDTMFEventPayloadInfo>& aEventInfos )
    {
    DP_DTMF_DECODE( _L("CDTMFPayloadDecoder::DecodeRedundantEventPayload") );
    
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x %x", MCC_TRACE, MCC_DTMF_PLF_DEC_DECREDEVPL, aSourceBuffer );
    #endif
    
    if ( aSourceBuffer )
        {
        TDes8& dataDes = 
            static_cast<CMMFDataBuffer*>( aSourceBuffer )->Data();
        const TUint8* seekPtr = dataDes.Ptr();
        
        TStreamDecoder streamDecoder;
        
        streamDecoder.Initialize( const_cast<TUint8*>( seekPtr ), 0, 0 );
        
        aEventInfos.Reset();
        
        // Count events in a payload from the redundancy headers
        TInt eventCount( 0 );
        TBool FBitSet( ETrue );
        RArray< TTimeIntervalMicroSeconds32 > timeStamps;
        
        while ( FBitSet )
            {
            eventCount++;
            
            FBitSet = static_cast<TBool>( streamDecoder.Decode( 1 ) );
            streamDecoder.Decode( KBlockPTBits );
            
            if ( FBitSet )
                {
                TUint offset = streamDecoder.Decode( KTimeStampOffsetBits );
                timeStamps.Append( TTimeIntervalMicroSeconds32( 
                                            iCurTimeStamp.Int() - offset ) );
                }
            else
                {
                timeStamps.Append( TTimeIntervalMicroSeconds32( 
                                                    iCurTimeStamp.Int() ) );
                }                

            // Jump to the beginning of next redundancy header
            streamDecoder.Initialize( const_cast<TUint8*>( seekPtr ), 
                                eventCount * KEventBlockLengthInBytes, 0 );
            }

        // Count index of first real event.
        // Last redundancy header consists of F-bit and PT only.
        streamDecoder.Initialize( const_cast<TUint8*>( seekPtr ), 
                                  eventCount * KEventBlockLengthInBytes - 
                                  KEventBlockLengthInBytes, KBitIndex );

        // Decode all events from payload to the event info array.
        for ( TInt i = 0; i < eventCount; i++ )
            {
            // Decode Event Field
            TInt eventCode = streamDecoder.Decode( KEventFieldBits );
            TChar charRepresentation;
            
            if ( KErrNone == ConvertToChar( eventCode, charRepresentation ) )
                {
                iDecodedEventInfo.SetEvent( charRepresentation );
                
                // Decode end bit for final packet
                iDecodedEventInfo.SetEndBit( streamDecoder.Decode( 1 ) );
                
                // Decode reserved bit
                streamDecoder.Decode( 1 );
                
                // Decode volume field.
                iDecodedEventInfo.SetVolume( streamDecoder.Decode( 
                                                            KVolFieldBits ) );
                
                // Decode duration field
                iDecodedEventInfo.SetDuration( streamDecoder.Decode( 
                                                    KDurationFieldBits ) );
                
                // Set time stamp
                iDecodedEventInfo.SetTimeStamp( timeStamps[i] );
                
                aEventInfos.Append( iDecodedEventInfo );
                }
            else
                {
                DP_DTMF_DECODE( _L( "CDTMFPayloadDecoder::DecodeRedEventPayload" ) );
                DP_DTMF_DECODE( _L( "- Not supported event! ") );
                }                
            }

        timeStamps.Reset();

        // Delete already played events from the beginning of aEventInfos
        // array based on time stamp
        TInt latestRecordedEventInd( iEventArray.Count() - 1 );
        
        if ( 0 <= latestRecordedEventInd )
            {
            while ( aEventInfos.Count() && aEventInfos[0].TimeStamp() 
                <= iEventArray[ latestRecordedEventInd ].TimeStamp() )
                {
                // Redundant event has earlier time stamp than latest played
                // event we must delete this.
                aEventInfos.Remove( 0 );
                }
            }

        // Copy count of accepted events to the end of iEventArray and delete
        // same count from the beginning of array. Decoder must keep copies of
        // EventInfos, because PayloadFormatRead may make Reset() for a
        // aEventInfos Array.
        for ( TInt j = 0; j < aEventInfos.Count(); j++ )
            {
            iEventArray.Append( aEventInfos[ j ] );
            if ( KDTMFDefaultRedundancyCount < iEventArray.Count() )
                {
                iEventArray.Remove( 0 );
                }
            }

        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadDecoder::DecodeTonePayload
// Decodes tones from Tone Payload Format packet.
// Currently only DTMF tones are decoded (first tone).
// Format of line event tones are unclear. Are multiple parts containing
// line events encoded in one RTP packet or one part per RTP packet.
// No proper support at DevSound / AudioServer to play out tone payload.
// ---------------------------------------------------------------------------
//
/*
     0                   1                   2                   3 
     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
    |    modulation   |T|  volume   |          duration             | 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
    |R R R R|       frequency       |R R R R|       frequency       | 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
    |R R R R|       frequency       |R R R R|       frequency       | 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
        ...... 
    
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
    |R R R R|       frequency       |R R R R|      frequency        | 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
*/    
TInt CDTMFPayloadDecoder::DecodeTonePayload( CMMFBuffer* aSourceBuffer,
    RArray<TDTMFTonePayloadInfo>& aToneInfos )
    {
    DP_DTMF_DECODE( _L( "CDTMFPayloadDecoder::DecodeTonePayload" ) );
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x %x", MCC_TRACE, MCC_DTMF_PLF_DEC_DECTONEPL, aSourceBuffer );
    #endif
    
    if ( aSourceBuffer )
        {
        TDes8& dataDes = 
            static_cast<CMMFDataBuffer*>( aSourceBuffer )->Data();
        const TUint8* seekPtr = dataDes.Ptr();
        
        TStreamDecoder streamDecoder;
        
        streamDecoder.Initialize( const_cast<TUint8*>( seekPtr ), 0, 0 );
        
        // Encode modulation field
        iDecodedToneInfo.SetModulation( streamDecoder.Decode( 
                                                        KModulationBits ) );
        
        // Encode T bit
        iDecodedToneInfo.SetTBit( streamDecoder.Decode( 1 ) );
        
        // Encode Volume Field
        iDecodedToneInfo.SetVolume( streamDecoder.Decode( KVolFieldBits ) );
        
        // Encode Duration Field
        iDecodedToneInfo.SetDuration( streamDecoder.Decode( 
                                                    KDurationFieldBits ) );
        
        // Encode four R(eserved) bits
        streamDecoder.Decode( KReservedBits );
        
        // Encode first Frequency Field
        iDecodedToneInfo.SetLowFrequency( streamDecoder.Decode( 
                                                        KFrequencyBits ) );
        
        // Encode four R(eserved) bits
        streamDecoder.Decode( KReservedBits );
        
        // Encode second Frequency Field
        iDecodedToneInfo.SetHighFrequency( streamDecoder.Decode( 
                                                        KFrequencyBits ) );
        
        aToneInfos.Append( iDecodedToneInfo );
        
        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadDecoder::PayloadFormat
// Returns payload format in use.
// ---------------------------------------------------------------------------
//
TDTMFPayloadFormat CDTMFPayloadDecoder::PayloadFormat( ) const
    {
    DP_DTMF_DECODE2( _L( "CDTMFPayloadDecoder::PayloadFormat - Format: %d" ),
        iPayloadFormat );
    
    return iPayloadFormat;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadDecoder::SetPayloadFormat
// Sets payload format in use.
// ---------------------------------------------------------------------------
//    
TInt CDTMFPayloadDecoder::SetPayloadFormat( 
    TDTMFPayloadFormat aPayloadFormat )
    {
    DP_DTMF_DECODE2( _L( "CDTMFPayloadDecoder::SetPayloadFormat - Format: %d" ),
        aPayloadFormat );
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x %d", MCC_TRACE, MCC_DTMF_PLF_DEC_SETPLF, aPayloadFormat );
    #endif
    
    if ( EDTMFPayloadFormatEvent != aPayloadFormat
         && EDTMFPayloadFormatTone != aPayloadFormat
         && EDTMFPayloadFormatRedEvents != aPayloadFormat )
        {
        return KErrNotSupported;
        }
    else
        {
        iPayloadFormat = aPayloadFormat;
        }
        
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadDecoder::SetCurTimeStamp
// Sets the time stamp of a RTP packet containing payload which is decoded 
// next.
// ---------------------------------------------------------------------------
//      
void CDTMFPayloadDecoder::SetCurTimeStamp( 
    const TTimeIntervalMicroSeconds32& aTimeStamp )
    {
    DP_DTMF_DECODE2( _L( "CDTMFPayloadDecoder::SetCurTimeStamp - TimeStamp: %d" ),
        aTimeStamp.Int() );
        
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x %d", MCC_TRACE, MCC_DTMF_PLF_DEC_SETCURTS, aTimeStamp.Int() );
    #endif
    
    iCurTimeStamp = aTimeStamp;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadDecoder::ConvertToChar
// Converts integer presentation used in a payload format to the character.
// ---------------------------------------------------------------------------
//
TInt CDTMFPayloadDecoder::ConvertToChar( TUint aEvent, TChar& aChar ) const
    {
    TInt conversionResult( KErrNone );

    if ( aEvent <= KMaxEventInteger )
        {
        aChar = TChar( aEvent + '0' );
        }
    else
        {
        switch ( aEvent )
            {
            case KEventCodeForAsterisk:
                aChar = '*';
                break;
            case KEventCodeForHashMark:
                aChar = '#';
                break;
            case KEventCodeForA:
                aChar = 'a';
                break;
            case KEventCodeForB:
                aChar = 'b';
                break;
            case KEventCodeForC:
                aChar = 'c';
                break;
            case KEventCodeForD:
                aChar = 'd';
                break;
            default:
                conversionResult = KErrNotSupported;
                break;
            }
        }    

    return conversionResult;
    }

//  End of File  
