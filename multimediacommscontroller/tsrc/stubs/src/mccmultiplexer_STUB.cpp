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
* Description:   
*
*/




// INCLUDE FILES
#include "mccmultiplexer.h"
#include "mccsinkitem.h"
#include "mccinternaldef.h"
#include "mccinternalevents.h"
#include "mccmultiplexerlogs.h"
#include "mccinternalevents.h"
#include "mccrtpdatasource.h"

// MACROS

// LOCAL FUNCTION PROTOTYPES

// LOCAL CONSTANTS

// ============================= LOCAL FUNCTIONS ===============================
        
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccMultiplexer::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMccMultiplexer* CMccMultiplexer::NewL( MDataSource* aSource )
    {
	CMccMultiplexer* self = new ( ELeave ) CMccMultiplexer;
	CleanupStack::PushL( self );
    self->ConstructL( aSource );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::~CMccMultiplexer
// -----------------------------------------------------------------------------
//
EXPORT_C CMccMultiplexer::~CMccMultiplexer()
    {
    }
  
// -----------------------------------------------------------------------------
// CMccMultiplexer::MultiplexerSourceThreadLogon
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMccMultiplexer::MultiplexerSourceThreadLogon( 
    MAsyncEventHandler& aEventHandler, 
    MDataSink& /*aSink*/, 
    TBool /*aSinkIsDecoder*/, 
    TUid /*aMediaType*/,
    TBool /*aPassAllBuffersSink*/ )
    {
    return iSource->SourceThreadLogon( aEventHandler );
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::SourceThreadLogoff
// -----------------------------------------------------------------------------
//       
EXPORT_C TInt CMccMultiplexer::MultiplexerSourceThreadLogoff( MDataSink* /*aSink*/ )
    {
    iSource->SourceThreadLogoff();
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::SetCurrentUserL
// -----------------------------------------------------------------------------
//    
EXPORT_C void CMccMultiplexer::SetCurrentUserL( MDataSink& /*aSink*/ )
    {
    }
    
// -----------------------------------------------------------------------------
// CMccMultiplexer::RegClientForPayloadsL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccMultiplexer::RegClientForPayloadsL( MDataSink& /*aConsumer*/, 
        const RArray<TUint>& aPayloadTypes )
    {
    __MULTIPLEXER( "CMccMultiplexer::RegClientForPayloadsL" )
    __ASSERT_ALWAYS( iSource, User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS( KMccRtpSourceUid == iSource->DataSourceType(),
        User::Leave( KErrNotSupported ) );
    
    //FindSinkItemL( &aConsumer )->RegisterPayloadTypesL( aPayloadTypes );
    static_cast<CMccRtpDataSource*>( iSource )
        ->RegisterPayloadTypesL( aPayloadTypes );
    }
    
// -----------------------------------------------------------------------------
// CMccMultiplexer::Streams
// -----------------------------------------------------------------------------
//
TUint CMccMultiplexer::Streams( TUid /*aMediaType*/ ) const
    {
    return 1;
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::FrameTimeInterval
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CMccMultiplexer::FrameTimeInterval( 
    TMediaId /*aMediaId*/ ) const
    {
    return TTimeIntervalMicroSeconds( 0 );
    }  

// -----------------------------------------------------------------------------
// CMccMultiplexer::DataSourceType
// -----------------------------------------------------------------------------
//
TUid CMccMultiplexer::DataSourceType() const
    {
    return KMccMultiplexerUid;
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::FillBufferL
// -----------------------------------------------------------------------------
//
void CMccMultiplexer::FillBufferL( 
    CMMFBuffer* /*aBuffer*/, 
    MDataSink* /*aConsumer*/, 
    TMediaId /*aMediaId*/ )
    {
    }  
        
// -----------------------------------------------------------------------------
// CMccMultiplexer::SourceDataTypeCode
// -----------------------------------------------------------------------------
//
TFourCC CMccMultiplexer::SourceDataTypeCode( 
	TMediaId /*aMediaId*/ )
    {
    return TFourCC();
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::SetSourceDataTypeCode
// -----------------------------------------------------------------------------
//
TInt CMccMultiplexer::SetSourceDataTypeCode( 
    TFourCC /*aSourceFourCC*/, 
    TMediaId /*aMediaId*/ )
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::SourceThreadLogon
// -----------------------------------------------------------------------------
//
TInt CMccMultiplexer::SourceThreadLogon( 
    MAsyncEventHandler& /*aEventHandler*/ )
    {
    return KErrNotSupported;
    }
    
// -----------------------------------------------------------------------------
// CMccMultiplexer::NegotiateSourceL
// -----------------------------------------------------------------------------
//
void CMccMultiplexer::NegotiateSourceL( MDataSink& aDataSink )
    {
    if ( KUidMmfAudioInput == iSource->DataSourceType() )
        {
        // Audio input need settings from real sink.
        // This is just quick workaround, better way would be to route
        // those requests through multiplexer to the sink
        
        __MULTIPLEXER( "CMccMultiplexer::NegotiateSourceL, negotiate (use sink)" )
        
        iSource->NegotiateSourceL( aDataSink );
        }
    else
        {
        __MULTIPLEXER( "CMccMultiplexer::NegotiateSourceL, negotiate (normal)" )
        
        iSource->NegotiateSourceL( *this );
        }
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::SourceThreadLogoff
// -----------------------------------------------------------------------------
//
void CMccMultiplexer::SourceThreadLogoff()
    {
    // NOP
    }
    
// -----------------------------------------------------------------------------
// CMccMultiplexer::SourcePrimeL
// -----------------------------------------------------------------------------
//
void CMccMultiplexer::SourcePrimeL()
    {
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::SourcePlayL
// -----------------------------------------------------------------------------
//
void CMccMultiplexer::SourcePlayL()
    {
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::SourcePauseL
// -----------------------------------------------------------------------------
//
void CMccMultiplexer::SourcePauseL()
    {
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::SourceStopL
// -----------------------------------------------------------------------------
//
void CMccMultiplexer::SourceStopL()
    {
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::DataBufferFilledL
// -----------------------------------------------------------------------------
//
void CMccMultiplexer::DataBufferFilledL( 
    CMMFBuffer* /*aBuffer*/, 
    const TRtpRecvHeader& /*aRtpHeader*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::ConfigurePayloadFormatL
// -----------------------------------------------------------------------------
//
void CMccMultiplexer::ConfigurePayloadFormatL( const TDesC8& /*aConfigParams*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::BufferFilledL
// -----------------------------------------------------------------------------
//
void CMccMultiplexer::BufferFilledL( CMMFBuffer* /*aBuffer*/ )
	{
	}
	
// -----------------------------------------------------------------------------
// CMccMultiplexer::Duration
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CMccMultiplexer::Duration( 
	TMediaId /*aMediaType*/ ) const
    {
    return TTimeIntervalMicroSeconds( 0 );
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::CanCreateSourceBuffer
// -----------------------------------------------------------------------------
//    
TBool CMccMultiplexer::CanCreateSourceBuffer()
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::CreateSourceBufferL
// -----------------------------------------------------------------------------
//
CMMFBuffer* CMccMultiplexer::CreateSourceBufferL( 
    TMediaId /*aMediaId*/, 
    TBool& /*aReference*/ )
    {
    User::Leave( KErrNotSupported );
    return 0;
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::SuggestSourceBufferSize
// -----------------------------------------------------------------------------
//
void CMccMultiplexer::SuggestSourceBufferSize( TUint aSuggestedBufferSize )
    {
    __MULTIPLEXER_INT1( "CMccMultiplexer::SuggestSourceBufferSize, SIZE: ",
         aSuggestedBufferSize )
    
//    iSuggestedBufSize = aSuggestedBufferSize;
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::SendEventToClient
// -----------------------------------------------------------------------------
//
TInt CMccMultiplexer::SendEventToClient( const TMMFEvent& /*aEvent*/ )
    {
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CMccMultiplexer::CMccMultiplexer
// -----------------------------------------------------------------------------
//
CMccMultiplexer::CMccMultiplexer()
    {
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::ConstructL
// -----------------------------------------------------------------------------
//
void CMccMultiplexer::ConstructL( MDataSource* aSource )
    {
    __ASSERT_ALWAYS( aSource, User::Leave( KErrArgument ) );
    
    iSource = aSource;
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::FindSinkItemL
// -----------------------------------------------------------------------------
//
CMccSinkItem* CMccMultiplexer::FindSinkItemL( MDataSink* aSink )
    {
    TInt index = FindSinkItemIndex( aSink );
    __ASSERT_ALWAYS( index != KErrNotFound, User::Leave( KErrNotFound ) );
    
    return iSinkItems[ index ];
    }
    
// -----------------------------------------------------------------------------
// CMccMultiplexer::FindSinkItemIndex
// -----------------------------------------------------------------------------
//
TInt CMccMultiplexer::FindSinkItemIndex( MDataSink* /*aSink*/ )
    {
    /*for ( TInt i = 0; i < iSinkItems.Count(); i++ )
        {
        if ( iSinkItems[ i ]->Match( aSink ) )
            {
            return i;
            }
        }*/
    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::IsPrimarySink
// -----------------------------------------------------------------------------
//    
TBool CMccMultiplexer::IsPrimarySink( MDataSink* /*aSink*/ )
    {
    return ETrue;
    }
    
// -----------------------------------------------------------------------------
// CMccMultiplexer::DoBufferFilledL
// -----------------------------------------------------------------------------
//
void CMccMultiplexer::DoBufferFilledL( 
    CMMFBuffer* /*aBuffer*/, 
    const TRtpRecvHeader* /*aRtpHeader*/ )
    {
    }   

// -----------------------------------------------------------------------------
// CMccMultiplexer::AddSinkL
// -----------------------------------------------------------------------------
//  
void CMccMultiplexer::AddSinkL( 
    MDataSink* /*aSink*/, 
    TBool /*aSinkIsDecoder*/,
    TUid /*aMediaType*/,
    TBool /*aPassAllBuffersSink*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::SetMultiplexerState
// -----------------------------------------------------------------------------
//    
TBool CMccMultiplexer::SetMultiplexerState( TMccMultiplexerState /*aState*/ )
    {
    return EFalse;
    }
    
// -----------------------------------------------------------------------------
// CMccMultiplexer::SendStreamEventToClient
// -----------------------------------------------------------------------------
//     
void CMccMultiplexer::SendStreamEventToClient( TMccEventType& /*aEventType*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::UpdateSinkModes
// -----------------------------------------------------------------------------
//
void CMccMultiplexer::UpdateSinkModes()
    {
    }
    
// -----------------------------------------------------------------------------
// CMccMultiplexer::PassAllRequestsToSource
// -----------------------------------------------------------------------------
//   
TBool CMccMultiplexer::PassAllRequestsToSource( CMccSinkItem& /*aItem*/ ) const
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::IsSupportedMediaType
// -----------------------------------------------------------------------------
//    
TBool CMccMultiplexer::IsSupportedMediaType( TUid aMediaType ) const
    {
    return ( aMediaType == KUidMediaTypeVideo || 
             aMediaType == KUidMediaTypeAudio );
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::SendEventToAllClients
// -----------------------------------------------------------------------------
// 
void CMccMultiplexer::SendEventToAllClients( const TMMFEvent& /*aEvent*/ )
    {
    /*for ( TInt i = 0; i < iSinkItems.Count(); i++ )
        {
        iSinkItems[ i ]->SendEventToClient( aEvent );
        }*/
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// E32Dll.
// DLL Entry point
// Returns: KErrNone
// -----------------------------------------------------------------------------
//
#ifndef EKA2
EXPORT_C TInt E32Dll( TDllReason )
    {
    return KErrNone;
    }
#endif


//  End of File 

