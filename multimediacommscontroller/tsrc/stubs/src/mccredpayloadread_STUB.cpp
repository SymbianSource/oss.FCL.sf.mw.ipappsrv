/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Implements format decode interface for redundancy plugin.
*
*/





// INCLUDE FILES
#include "rtpheader.h"
#include "mccredpayloadread.h"
#include "mccreddecoder.h"
#include "mccredpayloadformatdefs.h"
#include "formatstatemachine.h"

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccRedPayloadRead::CMccRedPayloadRead
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccRedPayloadRead::CMccRedPayloadRead()
    {
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadRead::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMccRedPayloadRead::ConstructL( MDataSource* aSource )
    {
    DP_RED_DECODE( "CMccRedPayloadRead_STUB::ConstructL" )
    //__ASSERT_ALWAYS( aSource, User::Leave( KErrArgument ) );
    
    iClip = aSource;
    /*iRedDecoder = CMccRedDecoder::NewL();*/
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadRead::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMccRedPayloadRead* CMccRedPayloadRead::NewL( MDataSource* aSource )
    {
    DP_RED_DECODE( "CMccRedPayloadRead_STUB::NewL" )/*
    __ASSERT_ALWAYS( aSource, User::Leave( KErrArgument ) );*/
    
    CMccRedPayloadRead* self = new( ELeave ) CMccRedPayloadRead;
    
    CleanupStack::PushL( self );
    self->ConstructL( aSource );
    CleanupStack::Pop( self );

    return self;
    }

    
// Destructor
CMccRedPayloadRead::~CMccRedPayloadRead()
    {
    DP_RED_DECODE( "CMccRedPayloadRead_STUB::~CMccRedPayloadRead" )

    /*delete iRedDecoder;
    if ( iSourceBufOwnership )
        {
        delete iSourceBuffer;
        }
    else
        {
        iSourceBuffer = NULL;
        }*/
    
    iDataSink = NULL;
    iClip = NULL;
    iSinkBuffer = NULL;
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadRead::ConfigurePayloadFormatL
// Configure payload decoding parameters.
// -----------------------------------------------------------------------------
//
void CMccRedPayloadRead::ConfigurePayloadFormatL( const TDesC8& /*aConfigParams*/ )
    {
    DP_RED_DECODE( "CMccRedPayloadRead_STUB::ConfigurePayloadFormatL" )/*
    __ASSERT_ALWAYS( aConfigParams.Size() == sizeof( TMccRedPayloadReadConfig ),
        User::Leave( KErrArgument ) );
    
    TMccRedPayloadReadPckg cPckg;
    cPckg.Copy( aConfigParams );
    iPLConfig = cPckg();

    __ASSERT_ALWAYS( iPLConfig.iRedBlockCount <= KMaxRedCount, 
        User::Leave( KErrArgument ) );
    
    if ( iSourceBuffer && iSourceBufOwnership )
        {
        delete iSourceBuffer;
        iSourceBuffer = NULL;
        }
    
    // Extra variables for clarity
    TInt redPayloadSize
        = iPLConfig.iMaxPayloadSize * ( 1 + iPLConfig.iRedBlockCount );
    TInt redHeadersSize 
        = iPLConfig.iRedBlockCount * KRedHeaderSize + KFinalHeaderSize;
    
    iSourceBuffer = CreateClipBufferL( redPayloadSize + redHeadersSize, 
        iSourceBufOwnership );
    iRedDecoder->InitializeL( iPLConfig.iRedBlockCount,
                              iPLConfig.iMaxPayloadSize, 
                              iPLConfig.iNumOfEncodings );
                              
    RArray<TUint> encPTs;
    CleanupClosePushL( encPTs );

    TInt maxNumOfEncodings = iPLConfig.iEncPayloadTypes.Count();
    for ( TInt i = 0; i < maxNumOfEncodings; i++ )
        {
        if ( KPayloadNotDefined != iPLConfig.iEncPayloadTypes[i] )
            {
            encPTs.AppendL( iPLConfig.iEncPayloadTypes[i] );
            }
        }

    User::LeaveIfError( iRedDecoder->SetPayloadTypes( encPTs ) );
    CleanupStack::PopAndDestroy( &encPTs );  */                            
    }
    
// -----------------------------------------------------------------------------
// CMccRedPayloadRead::DataBufferFilledL
// Redundancy format is decoded from parameter buffer and result is passed
// to the data sink of redundancy payload plugin.
// -----------------------------------------------------------------------------
//
void CMccRedPayloadRead::DataBufferFilledL( CMMFBuffer* /*aBuffer*/, 
                                        const TRtpRecvHeader& /*aRecvHeader*/ )
    {
    DP_RED_DECODE( "CMccRedPayloadRead_STUB::BufferFilledL" )
    
    /*__ASSERT_ALWAYS( aBuffer, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( KUidMmfDataBuffer == aBuffer->Type(),
        User::Leave( KErrNotSupported ) );
    __ASSERT_ALWAYS( iDataSink, User::Leave( KErrNotReady ) );
    
    if ( aRecvHeader.iPayloadType == iPLConfig.iRedPayloadType )
        {
        iRedDecoder->SetEncodingBlockL( EMccRTPPayload, 
            static_cast<CMMFDataBuffer*>( aBuffer )->Data() );
        
        TInt numOfBlocks = User::LeaveIfError( 
            iRedDecoder->DecodePayload( aRecvHeader.iTimestamp ) );
        
        if ( numOfBlocks )
            {
            iRedDecoder->GetEncodingBlockL( 
                EMccPrimaryEncoding, iSinkBuffer->Data() );
            }
        }
    else
        {
        iSinkBuffer->Data().Copy( static_cast<CMMFDataBuffer*>( aBuffer )->Data() );
        }        
    
    DP_RED_DECODE( "CMccRedPayloadRead::BufferFilledL - Buffer decoded" )
        
    // Payload format read is data sink for redundancy plugin
    static_cast<CPayloadFormatRead*>( iDataSink )
        ->BufferFilledL( iSinkBuffer, aRecvHeader );*/
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadRead::Streams
// NOT USED. Pure virtual method implementation from CMMFormatDecode needed.
// -----------------------------------------------------------------------------
//
TUint CMccRedPayloadRead::Streams( TUid /*aMediaType*/ ) const
    {
    DP_RED_DECODE( "CMccRedPayloadRead_STUB::Streams" )
        
    /*if ( KUidMediaTypeAudio == aMediaType )
        {
        return 1;
        }
    else
        {
        return 0;
        }*/
    return KErrNone;    
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadRead::FrameTimeInterval
// NOT USED. Pure virtual method implementation from CMMFormatDecode needed.
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CMccRedPayloadRead::FrameTimeInterval( 
        TMediaId /*aMediaType*/ ) const
    {
    DP_RED_DECODE( "CMccRedPayloadRead_STUB::FrameTimeInterval" )
        
    //return TTimeIntervalMicroSeconds( TInt64( 0 ) );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadRead::Duration
// NOT USED. Pure virtual method implementation from CMMFormatDecode needed.
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds   CMccRedPayloadRead::Duration( 
        TMediaId /*aMediaType*/ ) const
    {
    DP_RED_DECODE( "CMccRedPayloadRead_STUB::Duration" )

    //return TTimeIntervalMicroSeconds( TInt64( 0 ) );
    return KErrNone;
    }
        
// -----------------------------------------------------------------------------
// CMccRedPayloadRead::SourceDataTypeCode
// Returns the current datatype FourCC code.
// -----------------------------------------------------------------------------
//
TFourCC CMccRedPayloadRead::SourceDataTypeCode( TMediaId /*aMediaId*/ )
    {
    DP_RED_DECODE( "CMccRedPayloadRead_STUB::SourceDataTypeCode" )
        
    /*if ( KUidMediaTypeAudio == aMediaId.iMediaType )
        {
        return iFourCC;
        }
    else
        {
        return TFourCC(); //defaults to 'NULL' fourCC
        }*/
    return TFourCC();   
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadRead::SetSourceDataTypeCode
// Sets source datatype fourCC code.
// -----------------------------------------------------------------------------
//
TInt CMccRedPayloadRead::SetSourceDataTypeCode( TFourCC /*aSourceFourCC*/,
                                                TMediaId /*aMediaId*/ )
    {
    DP_RED_DECODE( "CMccRedPayloadRead_STUB::SetSourceDataTypeCode" )
    
    /*if ( KUidMediaTypeAudio != aMediaId.iMediaType )
        {
        return KErrNotSupported;
        }
    
    iFourCC = aSourceFourCC;*/
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CMccRedPayloadRead::CanCreateSourceBuffer
// 
// -----------------------------------------------------------------------------
//
TBool CMccRedPayloadRead::CanCreateSourceBuffer()
    {
    DP_RED_DECODE( "CMccRedPayloadRead_STUB::CanCreateSourceBuffer" )

    return EFalse;
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadRead::CreateSourceBufferL
// 
// -----------------------------------------------------------------------------
//
CMMFBuffer* CMccRedPayloadRead::CreateSourceBufferL( TMediaId /*aMediaId*/, 
                                                     TBool& /*aReference*/ )
    {
    DP_RED_DECODE( "CMccRedPayloadRead_STUB::CreateSourceBufferL" )
    /*__ASSERT_ALWAYS( KUidMediaTypeAudio == aMediaId.iMediaType,
        User::Leave( KErrNotSupported ) );
    __ASSERT_ALWAYS( iSuggestedBufSize, User::Leave( KErrNotReady ) );
    
    aReference = EFalse;
    
    CMMFDataBuffer* buf = CMMFDataBuffer::NewL( iSuggestedBufSize );
    iSuggestedBufSize = 0;
    return buf;*/
    return NULL;
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadRead::FillBufferL
// 
// -----------------------------------------------------------------------------
//
void CMccRedPayloadRead::FillBufferL( CMMFBuffer* /*aBuffer*/, 
                                      MDataSink* /*aConsumer*/, 
                                      TMediaId /*aMediaId*/ )
    {
    DP_RED_DECODE( "CMccRedPayloadRead_STUB::FillBufferL" )

    /*__ASSERT_ALWAYS( NULL != aBuffer, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( NULL != aConsumer, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( KUidMediaTypeAudio == aMediaId.iMediaType,
        User::Leave( KErrNotSupported ) );
    __ASSERT_ALWAYS( KUidMmfDataBuffer == aBuffer->Type(),
        User::Leave( KErrNotSupported ) );
    
    iDataSink = aConsumer;
    iSinkBuffer = static_cast<CMMFDataBuffer*>( aBuffer );
    
    iClip->FillBufferL( iSourceBuffer, this, aMediaId );*/
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadRead::SourceThreadLogon
// Passes state transition to the data source of redundancy payload plugin.
// -----------------------------------------------------------------------------
//
TInt CMccRedPayloadRead::SourceThreadLogon( MAsyncEventHandler& /*aEventHandler*/ )
    {
    DP_RED_DECODE( "CMccRedPayloadRead_STUB::SourceThreadLogon" )
        
    //return iClip->SourceThreadLogon( aEventHandler );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadRead::SourceThreadLogoff
// Passes state transition to the data source of redundancy payload plugin.
// -----------------------------------------------------------------------------
//
void CMccRedPayloadRead::SourceThreadLogoff()
    {
    DP_RED_DECODE( "CMccRedPayloadRead_STUB::SourceThreadLogoff" )
        
    //iClip->SourceThreadLogoff();
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadRead::SourcePrimeL
// Passes state transition to the data source of redundancy payload plugin.
// -----------------------------------------------------------------------------
//
void CMccRedPayloadRead::SourcePrimeL()
    {
    DP_RED_DECODE( "CMccRedPayloadRead_STUB::SourcePrimeL" )
        
    //iClip->SourcePrimeL();
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadRead::SourcePlayL
// Passes state transition to the data source of redundancy payload plugin.
// -----------------------------------------------------------------------------
//
void CMccRedPayloadRead::SourcePlayL()
    {
    DP_RED_DECODE( "CMccRedPayloadRead_STUB::SourcePlayL" )
        
    //iClip->SourcePlayL();
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadRead::SourcePauseL
// Passes state transition to the data source of redundancy payload plugin.
// -----------------------------------------------------------------------------
//
void CMccRedPayloadRead::SourcePauseL()
    {
    DP_RED_DECODE( "CMccRedPayloadRead_STUB::SourcePauseL" )
        
    //iClip->SourcePauseL();
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadRead::SourceStopL
// Passes state transition to the data source of redundancy payload plugin.
// -----------------------------------------------------------------------------
//
void CMccRedPayloadRead::SourceStopL()
    {
    DP_RED_DECODE( "CMccRedPayloadRead_STUB::SourceStopL" )
        
    //iClip->SourceStopL();
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadRead::NegotiateSourceL
// Negotiate Source.
// -----------------------------------------------------------------------------
//
void CMccRedPayloadRead::NegotiateSourceL( MDataSink& /*aDataSink*/ )
    {
    DP_RED_DECODE( "CMccRedPayloadRead_STUB::NegotiateSourceL" )
        
    //iDataSink = &aDataSink;
    ///iClip->NegotiateSourceL( *this );
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadRead::SuggestSourceBufferSize
// 
// -----------------------------------------------------------------------------
//
void CMccRedPayloadRead::SuggestSourceBufferSize( TUint /*aSuggestedBufferSize*/ )
    {
    //iSuggestedBufSize = aSuggestedBufferSize;
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadRead::SinkDataTypeCode
// Returns the current datatype FourCC code.
// -----------------------------------------------------------------------------
//
TFourCC CMccRedPayloadRead::SinkDataTypeCode( TMediaId /*aMediaId*/ )
    {
    DP_RED_DECODE( "CMccRedPayloadRead_STUB::SinkDataTypeCode" )
        
    /*if ( KUidMediaTypeAudio == aMediaId.iMediaType )
        {
        if ( iDataSink )
            {
            return iDataSink->SinkDataTypeCode( aMediaId );
            }
        else
            {
            return TFourCC();
            }
        }
    else
        {
        return TFourCC(); //defaults to 'NULL' fourCC
        }*/
    return TFourCC();
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadRead::SetPayloadTypes
// Set payload types to accept as encodings.
// -----------------------------------------------------------------------------
//
TInt CMccRedPayloadRead::SetPayloadTypes( RArray<TUint>& /*aPayloadTypes*/ )
    {
    DP_RED_DECODE( "CMccRedPayloadRead_STUB::SetPayloadTypes" )
    
    /*if ( aPayloadTypes.Count() )
        {
        iPLConfig.iRedPayloadType = static_cast<TUint8>( aPayloadTypes[0] );
        aPayloadTypes.Remove( 0 );
        
        return iRedDecoder->SetPayloadTypes( aPayloadTypes );
        }
    else
        {
        return KErrArgument;
        }*/
    return KErrNone;    
    }
    
// -----------------------------------------------------------------------------
// CMccRedPayloadRead::CreateClipBufferL
// Creates buffer needed in data transfer with format readers clip.
// -----------------------------------------------------------------------------
//
CMMFDataBuffer* CMccRedPayloadRead::CreateClipBufferL( 
        TUint /*aSize*/, TBool& /*aIsOwnBuffer*/ )
    {
    DP_RED_DECODE( "CMccRedPayloadRead_STUB::CreateClipBufferL" )
    CMMFDataBuffer* buffer( NULL );
    
    /*if ( iClip->CanCreateSourceBuffer() )
        {
        static_cast<CMMFFormatDecode*>( iClip )->SuggestSourceBufferSize( aSize );
        
        TBool reference( EFalse );
        CMMFBuffer* sourceBuf 
            = iClip->CreateSourceBufferL( KUidMediaTypeAudio, reference );
        TBool isSupportedBuf 
            = CMMFBuffer::IsSupportedDataBuffer( sourceBuf->Type() );
        TBool isOwnBuffer = reference ? EFalse : ETrue;
        
        if ( !isSupportedBuf )
            {
            if ( isOwnBuffer )
                {
                delete sourceBuf;
                }
            
            User::Leave( KErrNotSupported );
            }
        
        aIsOwnBuffer = isOwnBuffer;
        buffer = static_cast<CMMFDataBuffer*>( sourceBuf );
        }
    else
        {
        aIsOwnBuffer = ETrue;
        buffer = CMMFDataBuffer::NewL( aSize );
        }*/
    
    return buffer;
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================


//  End of File  
