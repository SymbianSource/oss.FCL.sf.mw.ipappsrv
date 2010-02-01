/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    DTMF RTP payload encoder for named telephone events
*                and tones.
*
*/




// INCLUDES
#include <mmf/server/mmfdatabuffer.h>
#include "dtmfpayloadformatdefs.h"
#include "dtmfpayloadencoder.h"
#include "dtmfeventpayloadinfo.h"
#include "dtmftonepayloadinfo.h"
#include "streamformatter.h"

// CONSTANTS
const TInt KEventLengthInBytes = 8;
const TInt KByteIndex = 2;
const TInt KReservedBits = 4;

#ifdef VOIP_TRACE_ENABLED
#include <voip_trace.h>
#endif

// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// CDTMFPayloadEncoder::CDTMFPayloadEncoder
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CDTMFPayloadEncoder::CDTMFPayloadEncoder()
    {
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadEncoder::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CDTMFPayloadEncoder* CDTMFPayloadEncoder::NewL()
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_DTMF_PLF_ENC_NEWL );
    #endif

    CDTMFPayloadEncoder* self = new( ELeave ) CDTMFPayloadEncoder;
    return self;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadEncoder::~CDTMFPayloadEncoder
// Destructor.
// ---------------------------------------------------------------------------
//
CDTMFPayloadEncoder::~CDTMFPayloadEncoder()
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_DTMF_PLF_ENC_DESTRUCTOR );
    #endif
    iEventArray.Reset();
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadEncoder::EncodeEvent
// Forms payload for named events.
// ---------------------------------------------------------------------------
//
/*
    0                   1                   2                   3 
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
   |     event     |E|R| volume    |          duration             | 
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
*/  
TInt CDTMFPayloadEncoder::EncodeEvent( 
    const TDTMFEventPayloadInfo& aEventInfo,
    CMMFBuffer* aOutputBuffer )
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_DTMF_PLF_ENC_ENCEV );
    #endif
    if ( EDTMFPayloadFormatRedEvents == iPayloadFormat )
        {
        return EncodeRedundantEventPayload( aEventInfo, aOutputBuffer );
        }

    if ( aOutputBuffer )
        {
        // Possible events are: 0-9, * (10), # (11), A-D (12-15)
        TInt eventAsInteger = EventAsInteger( aEventInfo.Event() );

        TStreamEncoder streamEncoder;
        TDes8& dataDes = 
            static_cast<CMMFDataBuffer*>( aOutputBuffer )->Data();
        const TUint8* seekPtr = dataDes.Ptr();
         // 32 bits
        dataDes.SetLength( KEventBlockLengthInBytes );
        
        streamEncoder.Initialize( const_cast<TUint8*>( seekPtr ), 0, 0 );
        
        // Encode Event Field
        streamEncoder.Encode( eventAsInteger, KEventFieldBits );
        
        // Encode end bit for final packet.
        streamEncoder.Encode( aEventInfo.EndBit(), 1 );
        
        // Encode reserved bit
        streamEncoder.Encode( 0, 1 );
        
        // Encode volume field. Volume must be encoded only if it is relevant
        // to the event. Otherwise it must be set to zero.
        streamEncoder.Encode( aEventInfo.Volume(), KVolFieldBits );
        
        // Encode duration field
        streamEncoder.Encode( aEventInfo.Duration(), KDurationFieldBits );
        
        // Update event array for redundant events encoding
        iEventArray.Append( aEventInfo );
        if ( KDTMFDefaultRedundancyCount < iEventArray.Count() )
            {
            iEventArray.Remove( 0 );
            }
        
        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadEncoder::EncodeRedundantEventPayload
// Encodes events to Event Payload Format using Multi-Event Redundancy.
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
TInt CDTMFPayloadEncoder::EncodeRedundantEventPayload(
    const TDTMFEventPayloadInfo& aEventInfo, CMMFBuffer* aOutputBuffer )
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_DTMF_PLF_ENC_ENCREDEVPL );
    #endif
    if ( aOutputBuffer )
        {
        TStreamEncoder streamEncoder;
        TDes8& dataDes = 
            static_cast<CMMFDataBuffer*>( aOutputBuffer )->Data();
        const TUint8* seekPtr = dataDes.Ptr();
        
        // Count length of packet. Event consists of 4 byte header and 4 byte
        // payload. Length of the redundancy header of last event is one byte.
        TInt length( iEventArray.Count() * KEventLengthInBytes - 3 );
        if ( length > 0 && length < dataDes.MaxSize() )
            {
            dataDes.SetLength( length );    
            }

        streamEncoder.Initialize( const_cast<TUint8*>( seekPtr ), 0, 0 );
        
        // Update event array
        iEventArray.Append( aEventInfo );
        if ( KDTMFDefaultRedundancyCount < iEventArray.Count() )
            {
            iEventArray.Remove( 0 );
            }
            
        // Encode redundancy headers
        TInt eventCount( iEventArray.Count() );
        for ( TInt i = 0; i < eventCount; i++ )
            {
            if ( i != eventCount - 1 )
                {
                // F-bit
                streamEncoder.Encode( 1, 1 );
                streamEncoder.Encode( iPayloadType, KBlockPTBits );
                
                // Encode Timestamp offset
                TUint offset( iEventArray[eventCount - 1].TimeStamp().Int()
                                         - iEventArray[i].TimeStamp().Int() );
                streamEncoder.Encode( offset, KTimeStampOffsetBits );
                
                streamEncoder.Encode( KEventBlockLengthInBytes, 
                                                        KBlockLengthBits );
                }
            else
                {
                // Last redundancy header
                // F-bit
                streamEncoder.Encode( 0, 1 );
                streamEncoder.Encode( iPayloadType, KBlockPTBits );
                }
            }

        // Encode payload blocks
        for ( TInt index = 0; index < eventCount; index++ )
            {
            // Possible events are: 0-9, * (10), # (11), A-D (12-15)
            TInt eventAsInteger = 
                                EventAsInteger( iEventArray[index].Event() );

            // Encode digit field
            streamEncoder.Encode( eventAsInteger, KEventFieldBits );

            // Encode E-bit
            if ( index != eventCount - 1 )
                {
                streamEncoder.Encode( 1, 1 );
                }
            else
                {
                // Last event
                streamEncoder.Encode( 0, 1 );
                }

            // Encode Reserved bit
            streamEncoder.Encode( 0, 1 );

            // Encode Volume Field
            streamEncoder.Encode( iEventArray[index].Volume(), 
                                                            KVolFieldBits );
            
            // Encode Duration Field
            streamEncoder.Encode( iEventArray[index].Duration(), 
                                                        KDurationFieldBits );
            }

        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadEncoder::EncodeTone
// Forms tone payload for DTMF digit.
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
TInt CDTMFPayloadEncoder::EncodeTone( const TDTMFTonePayloadInfo& aToneInfo,
    CMMFDataBuffer* aOutputBuffer ) const
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_DTMF_PLF_ENC_ENCTONE );
    #endif
    if ( aOutputBuffer )
        {
        TInt toneAsInteger = EventAsInteger( aToneInfo.Tone() );
        if ( toneAsInteger < 0 || toneAsInteger >= KDtmfGroupFrequencyCount )
            {
            return KErrArgument;
            }
        TUint lowGroupFrequency( KDTMFLowGroupFrequencies[ toneAsInteger ] );
        TUint highGroupFrequency( 
                                KDTMFHighGroupFrequencies[ toneAsInteger ] );
        
        TStreamEncoder streamEncoder;
        TDes8& dataDes = 
            STATIC_CAST( CMMFDataBuffer*, aOutputBuffer )->Data();
        const TUint8* seekPtr = dataDes.Ptr();
        // 2 * 32 bits
        dataDes.SetLength( KEventLengthInBytes );
        
        streamEncoder.Initialize( const_cast<TUint8*>( seekPtr ), 0, 0 );
        
        // Encode modulation field
        streamEncoder.Encode( aToneInfo.Modulation(), KModulationBits );
        
        // Encode T bit
        streamEncoder.Encode( 0, 1 );
        
        // Encode Volume Field
        streamEncoder.Encode( aToneInfo.Volume(), KVolFieldBits );
        
        // Encode Duration Field
        streamEncoder.Encode( aToneInfo.Duration(), KDurationFieldBits );
        
        // Encode four R(eserved) bits
        streamEncoder.Encode( 0, KReservedBits );
        
        // Encode first Frequency Field
        streamEncoder.Encode( lowGroupFrequency, KFrequencyBits );
        
        // Encode four R(eserved) bits
        streamEncoder.Encode( 0, KReservedBits );
        
        // Encode second Frequency Field
        streamEncoder.Encode( highGroupFrequency, KFrequencyBits );
        
        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadEncoder::UpdateEventPayload
// Updates previously created event payload according to parameters.
// ---------------------------------------------------------------------------
//
TInt CDTMFPayloadEncoder::UpdateEventPayload( const TBool aFinalPacket,
    const TUint aDuration,
    CMMFDataBuffer* aOutputBuffer ) const
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_DTMF_PLF_ENC_UPDEVPL );
    #endif
    if ( aOutputBuffer )
        {
        TStreamEncoder streamEncoder;
        TDes8& dataDes = STATIC_CAST( CMMFDataBuffer*, 
                                                    aOutputBuffer )->Data();
        const TUint8* seekPtr = dataDes.Ptr();
        
        // End bit starts from the beginning of second byte
        streamEncoder.Initialize( const_cast<TUint8*>( seekPtr ), 1, 0 );
        
        // Encode End Bit
        if ( aFinalPacket )
            {
            streamEncoder.Encode( 1, 1 );    
            }
            
        // Duration Field starts from third byte
        streamEncoder.SetBitIndex( 0 );
        streamEncoder.SetByteIndex( KByteIndex );
        
        // Encode Duration Field with new value
        streamEncoder.Encode( aDuration, KDurationFieldBits );
        
        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
    }


// ---------------------------------------------------------------------------
// CDTMFPayloadEncoder::UpdateTonePayload
// Updates previously created tone payload according to parameters.
// ---------------------------------------------------------------------------
//     
TInt CDTMFPayloadEncoder::UpdateTonePayload( const TUint aToneDuration,
    CMMFDataBuffer* aOutputBuffer ) const
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x", MCC_TRACE, MCC_DTMF_PLF_ENC_UPDTONEPL );
    #endif

    if ( aOutputBuffer )
        {
        TStreamEncoder streamEncoder;
        TDes8& dataDes = STATIC_CAST( CMMFDataBuffer*, 
                                      aOutputBuffer )->Data();
        const TUint8* seekPtr = dataDes.Ptr();

        // Duration Field starts from third byte
        streamEncoder.Initialize( const_cast<TUint8*>( seekPtr ), 3, 0 );
        
        // Encode Duration Field with a new value
        streamEncoder.Encode( aToneDuration, KDurationFieldBits );

        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadEncoder::PayloadFormat
// Returns payload format in use.
// ---------------------------------------------------------------------------
//
TDTMFPayloadFormat CDTMFPayloadEncoder::PayloadFormat( ) const
    {
    return iPayloadFormat;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadEncoder::SetPayloadFormat
// Sets payload format in use.
// ---------------------------------------------------------------------------
//
TInt CDTMFPayloadEncoder::SetPayloadFormat( 
    TDTMFPayloadFormat aPayloadFormat )
    {
    #ifdef VOIP_TRACE_ENABLED
        VoipTrace( "%x %x %d", MCC_TRACE, MCC_DTMF_PLF_ENC_SETPLF, aPayloadFormat );
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
// CDTMFPayloadEncoder::PayloadType
// Returns payload type in use.
// ---------------------------------------------------------------------------
//
TUint8 CDTMFPayloadEncoder::PayloadType( ) const
    {
    return iPayloadType;
    }

// ---------------------------------------------------------------------------
// CDTMFPayloadEncoder::SetPayloadType
// Sets payload type in use.
// ---------------------------------------------------------------------------
//    
void CDTMFPayloadEncoder::SetPayloadType( TUint8 aPayloadType )
    {
    iPayloadType = aPayloadType;
    }
     
// ---------------------------------------------------------------------------
// CDTMFPayloadEncoder::EventAsInteger
// Converts DTMF digit / Tone represented as TChar to the integer encoding
// used in payload.
// ---------------------------------------------------------------------------
//
TInt CDTMFPayloadEncoder::EventAsInteger( const TChar& aEvent ) const
    {
    TInt returnValue( KErrNotFound );

    // Conversion because of PC_LINT warning #1561
    TChar argument( aEvent );
    // For PC_LINT error   #1023
    TUint nullChar( '0' );

    if ( '0' <= aEvent && '9' >= aEvent )
        {
        returnValue = argument - nullChar;
        }
    else
        {
        switch ( aEvent )
            {
            case '*':
                returnValue = KEventCodeForAsterisk;
                break;
            case '#':
                returnValue = KEventCodeForHashMark;
                break;
            case 'a':
            case 'A':
                returnValue = KEventCodeForA;
                break;
            case 'b':
            case 'B':
                returnValue = KEventCodeForB;
                break;
            case 'c':                
            case 'C':
                returnValue = KEventCodeForC;
                break;
            case 'd':
            case 'D':
                returnValue = KEventCodeForD;
                break;
            default:
                break;
            }
        }

    return returnValue;
    }

//  End of File
