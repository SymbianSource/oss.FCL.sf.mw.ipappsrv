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
* Description:    Implements format encode interface for redundancy plugin.
*
*/





// INCLUDE FILES
#include    "rtpheader.h"
#include    "mccredpayloadwrite.h"
#include    "mccredencoder.h"
#include    "mccrtpdatasink.h"
#include    "mccredpayloadformatdefs.h"
#include    "mccdef.h"
#include    "mccinternaldef.h"
#include    "mccrtpmediaclock.h"

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccRedPayloadWrite::CMccRedPayloadWrite
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccRedPayloadWrite::CMccRedPayloadWrite()
    {
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadWrite::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMccRedPayloadWrite::ConstructL( MDataSink* /*aSink*/ )
    {
    /*DP_RED_ENCODE( "CMccRedPayloadWrite::ConstructL" )
    __ASSERT_ALWAYS( aSink, User::Leave( KErrArgument ) );
    
    iClip = aSink;*/
    //iRedEncoder = CMccRedEncoder::NewL();
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadWrite::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMccRedPayloadWrite* CMccRedPayloadWrite::NewL( MDataSink* aSink )
    {
    /*DP_RED_ENCODE( "CMccRedPayloadWrite::NewL" )
    __ASSERT_ALWAYS( aSink, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( KMccRtpSinkUid == aSink->DataSinkType(),
        User::Leave( KErrNotSupported ) );*/
    
    CMccRedPayloadWrite* self = new( ELeave ) CMccRedPayloadWrite;
    
    CleanupStack::PushL( self );
    self->ConstructL( aSink );
    CleanupStack::Pop( self );

    return self;
    }

    
// Destructor
CMccRedPayloadWrite::~CMccRedPayloadWrite()
    {
    /*DP_RED_ENCODE( "CMccRedPayloadWrite::~CMccRedPayloadWrite" )*/
        
    //delete iRedEncoder;
    /*delete iSinkBuffer;
    iDataSource = NULL;
    iBufToEmptyPtr = NULL;*/
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadRead::ConfigurePayloadFormatL
// Configure payload encoding parameters.
// -----------------------------------------------------------------------------
//
void CMccRedPayloadWrite::ConfigurePayloadFormatL( const TDesC8& /*aConfigParams*/,
        CMccRtpMediaClock& /*aClock*/ )
    {
    /*DP_RED_ENCODE( "CMccRedPayloadWrite::ConfigurePayloadFormatL" )
    __ASSERT_ALWAYS( aConfigParams.Size() == sizeof( TMccRedPayloadWriteConfig ),
        User::Leave( KErrArgument ) );
    
    TMccRedPayloadWritePckg cPckg;
    cPckg.Copy( aConfigParams );
    iPLConfig = cPckg();

    __ASSERT_ALWAYS( iPLConfig.iRedBlockCount <= KMaxRedCount, 
        User::Leave( KErrArgument ) );
    
    if ( iSinkBuffer )
        {
        delete iSinkBuffer;
        iSinkBuffer = NULL;
        }
    
    // Extra variables for clarity
    TInt redPayloadSize
        = iPLConfig.iMaxPayloadSize * ( 1 + iPLConfig.iRedBlockCount );
    TInt redHeadersSize 
        = iPLConfig.iRedBlockCount * KRedHeaderSize + KFinalHeaderSize;
    iSinkBuffer = CMMFDataBuffer::NewL( redPayloadSize + redHeadersSize );
                                    
    iRedEncoder->InitializeL( iPLConfig.iRedBlockCount,
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

    User::LeaveIfError( iRedEncoder->SetPayloadTypes( encPTs ) );
    CleanupStack::PopAndDestroy( &encPTs );*/
    }
    
// -----------------------------------------------------------------------------
// CMccRedPayloadWrite::FrameTimeInterval
// NOT USED. Pure virtual method implementation from CMMFormatEncode needed.
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CMccRedPayloadWrite::FrameTimeInterval( 
        TMediaId /*aMediaType*/ ) const
    {
    //DP_RED_ENCODE( "CMccRedPayloadWrite::FrameTimeInterval" )

    return TTimeIntervalMicroSeconds( TInt64( 0 ) );
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadWrite::Duration
// NOT USED. Pure virtual method implementation from CMMFormatEncode needed.
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CMccRedPayloadWrite::Duration( 
        TMediaId /*aMediaType*/ ) const
    {
    //DP_RED_ENCODE( "CMccRedPayloadWrite::Duration" )

    return TTimeIntervalMicroSeconds( TInt64( 0 ) );
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadWrite::CreateSinkBufferL
// -----------------------------------------------------------------------------
//
CMMFBuffer* CMccRedPayloadWrite::CreateSinkBufferL( TMediaId /*aMediaId*/,
                                                    TBool& /*aReference*/ )
    {
    /*DP_RED_ENCODE( "CMccRedPayloadWrite::CreateSinkBufferL" )
    User::Leave( KErrNotSupported );*/
    return NULL;
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadWrite::DataSinkType
//
// -----------------------------------------------------------------------------
//
TUid CMccRedPayloadWrite::DataSinkType() const
    {
    return TUid::Uid( KImplUidRedPayloadFormatEncode );
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadWrite::SinkDataTypeCode
// NOT USED.
// -----------------------------------------------------------------------------
//
TFourCC CMccRedPayloadWrite::SinkDataTypeCode( TMediaId /*aMediaId*/ )
    {
    //DP_RED_ENCODE( "CMccRedPayloadWrite::SinkDataTypeCode" )
        
    return TFourCC();
    }


// -----------------------------------------------------------------------------
// CMccRedPayloadWrite::SetSinkDataTypeCode
// NOT USED.
// -----------------------------------------------------------------------------
//
TInt CMccRedPayloadWrite::SetSinkDataTypeCode( TFourCC /*aSinkFourCC*/,
                                               TMediaId /*aMediaId*/ )
    {
    //DP_RED_ENCODE( "CMccRedPayloadWrite::SetSinkDataTypeCode" )
        
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadWrite::SinkThreadLogon
// Passes state transition to the data sink of redundancy payload plugin.
// -----------------------------------------------------------------------------
//
TInt CMccRedPayloadWrite::SinkThreadLogon( MAsyncEventHandler& /*aEventHandler*/ )
    {
    //DP_RED_ENCODE( "CMccRedPayloadWrite::SinkThreadLogon" )
        
    //iClip->SinkThreadLogon( aEventHandler );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadWrite::SinkThreadLogoff
// Passes state transition to the data sink of redundancy payload plugin.
// -----------------------------------------------------------------------------
//
void CMccRedPayloadWrite::SinkThreadLogoff()
    {
    //DP_RED_ENCODE( "CMccRedPayloadWrite::SinkThreadLogoff" )
        
    //iClip->SinkThreadLogoff();    
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadWrite::SinkPrimeL
// Passes state transition to the data sink of redundancy payload plugin.
// -----------------------------------------------------------------------------
//
void CMccRedPayloadWrite::SinkPrimeL()
    {
    //DP_RED_ENCODE( "CMccRedPayloadWrite::SinkPrimeL" )
        
    //iClip->SinkPrimeL();
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadWrite::SinkPlayL
// Passes state transition to the data sink of redundancy payload plugin.
// -----------------------------------------------------------------------------
//
void CMccRedPayloadWrite::SinkPlayL()
    {
    //DP_RED_ENCODE( "CMccRedPayloadWrite::SinkPlayL" )
        
    //iClip->SinkPlayL();
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadWrite::SinkPauseL
// Passes state transition to the data sink of redundancy payload plugin.
// -----------------------------------------------------------------------------
//
void CMccRedPayloadWrite::SinkPauseL()
    {
    //DP_RED_ENCODE( "CMccRedPayloadWrite::SinkPauseL" )
        
    //iClip->SinkPauseL();
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadWrite::SinkStopL
// Passes state transition to the data sink of redundancy payload plugin.
// -----------------------------------------------------------------------------
//
void CMccRedPayloadWrite::SinkStopL()
    {
    DP_RED_ENCODE( "CMccRedPayloadWrite::SinkStopL" )
        
    //iClip->SinkStopL();
    }
    
// -----------------------------------------------------------------------------
// CMccRedPayloadWrite::EmptyBufferL
// NOT USED.
// Overloaded emptybuffer with additional RTP header parameter used instead.
// -----------------------------------------------------------------------------
//
void CMccRedPayloadWrite::EmptyBufferL( CMMFBuffer* /*aBuffer*/,
                                        MDataSource* /*aSupplier*/, 
                                        TMediaId /*aMediaId*/ )
    {
    //DP_RED_ENCODE( "CMccRedPayloadWrite::EmptyBufferL" )
        
    //User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccRedPayloadWrite::BufferEmptiedL
// Called by data sink of redundancy payload plugin, when payload is emptied.
// -----------------------------------------------------------------------------
//
void CMccRedPayloadWrite::BufferEmptiedL( CMMFBuffer* /*aBuffer*/ )
    {
    //DP_RED_ENCODE( "CMccRedPayloadWrite::BufferEmptiedL" )
    //__ASSERT_ALWAYS( aBuffer, User::Leave( KErrArgument ) );
    //__ASSERT_ALWAYS( iBufToEmptyPtr, User::Leave( KErrNotReady ) );
    
    //iDataSource->BufferEmptiedL( iBufToEmptyPtr );
    }
    
// -----------------------------------------------------------------------------
// CMccRedPayloadWrite::EmptyBufferL
// Encodes contents of source buffer with redundancy encoder and forwards
// result buffer to the data sink of redundancy payload plugin.
// -----------------------------------------------------------------------------
//
void CMccRedPayloadWrite::EmptyBufferL( CMMFBuffer* /*aSourceBuffer*/,
                                        MDataSource* /*aSupplier*/, 
                                        TMediaId /*aMediaId*/,
                                        TRtpSendHeader& /*aSendHeader*/ )
    {
    /*DP_RED_ENCODE( "CMccRedPayloadWrite::EmptyBufferL" )
    __ASSERT_ALWAYS( aSourceBuffer, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( KUidMmfDataBuffer == aSourceBuffer->Type(),
        User::Leave( KErrNotSupported ) );
    __ASSERT_ALWAYS( aSupplier, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( KUidMediaTypeAudio == aMediaId.iMediaType,
        User::Leave( KErrNotSupported ) );
    
    DP_RED_ENCODE( "CMccRedPayloadWrite::EmptyBufferL() - MEDIA TYPE OK" )
    
    iDataSource = aSupplier;
    iBufToEmptyPtr = static_cast<CMMFDataBuffer*>( aSourceBuffer );

    if ( KComfortNoisePT != aSendHeader.iPayloadType )
        {
        // Comfort noise packets are forwarded without encoding
        iRedEncoder->SetEncodingBlockL( 
            EMccPrimaryEncoding, iBufToEmptyPtr->Data() );
        iRedEncoder->EncodePayloadL( aSendHeader.iTimestamp );
        iRedEncoder->GetEncodingBlockL( EMccRTPPayload, iSinkBuffer->Data() );

        DP_RED_ENCODE( "CMccRedPayloadWrite::EmptyBufferL() - Buffer encoded" )
        aSendHeader.iPayloadType = iPLConfig.iRedPayloadType;
        }

    // Deliver the packet
    static_cast<CMccRtpDataSink*>( iClip )
        ->EmptyBufferL( iSinkBuffer, this, aMediaId, aSendHeader );*/
    }
    
// -----------------------------------------------------------------------------
// CMccRedPayloadWrite::SetPayloadTypes
// Set payload types to accept as encodings.
// -----------------------------------------------------------------------------
//
TInt CMccRedPayloadWrite::SetPayloadTypes( RArray<TUint>& /*aPayloadTypes*/ )
    {
    /*DP_RED_ENCODE( "CMccRedPayloadWrite::SetPayloadTypes" )
        
    if ( aPayloadTypes.Count() )
        {
        iPLConfig.iRedPayloadType = aPayloadTypes[0];
        aPayloadTypes.Remove( 0 );
        
        return iRedEncoder->SetPayloadTypes( aPayloadTypes );
        }
    else
        {
        return KErrArgument;
        } */
    return KErrNone;           
    }
    
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
