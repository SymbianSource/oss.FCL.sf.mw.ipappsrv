/*
* Copyright (c) 2004-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides a comfort noise generator class
*
*/

        
// INCLUDE FILES
#include "mccmsrpsink.h"
#include "mmccinterfacedef.h"
#include "mccmsrpsourcesinklogs.h"
#include "mccinternalevents.h"
#include "mccinternaldef.h"

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccMsrpSink::NewSinkL
// -----------------------------------------------------------------------------
//
MDataSink* CMccMsrpSink::NewSinkL( TUid /*aImplementationUid*/, 
                                   const TDesC8& /*aInitData*/ )
    {
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpSink::NewSinkL" )
    
    CMccMsrpSink* self = new ( ELeave ) CMccMsrpSink();
    return static_cast<MDataSink*>( self );
    }

// -----------------------------------------------------------------------------
// CMccMsrpSink::ConstructSinkL
// -----------------------------------------------------------------------------
//
void CMccMsrpSink::ConstructSinkL( const TDesC8& /*aInitData*/ )
    {
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpSink::ConstructSinkL" )
    iDataSender = CMccMsrpDataSender::NewL();
    }
    
// -----------------------------------------------------------------------------
// CMccMsrpSink::CMccMsrpSink
// -----------------------------------------------------------------------------
//
CMccMsrpSink::CMccMsrpSink() : CMccDataSink( KMccMsrpSinkUid )
    {
    }
        
// -----------------------------------------------------------------------------
// CMccMsrpSink::~CMccMsrpSink
// -----------------------------------------------------------------------------
//
CMccMsrpSink::~CMccMsrpSink()
    {
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpSink::~CMccMsrpSink" )
    delete iDataSender;
    }

// -----------------------------------------------------------------------------
// CMccMsrpSink::SinkPrimeL
// -----------------------------------------------------------------------------
//
void CMccMsrpSink::SinkPrimeL()
	{
	__MSRPSOURCESINK_CONTROLL( "CMccMsrpSink::SinkPrimeL" )

	SendStreamEventToClient( KMccStreamPrepared );		
	}
	
// -----------------------------------------------------------------------------
// CMccMsrpSink::SinkPlayL()
// -----------------------------------------------------------------------------
//
void CMccMsrpSink::SinkPlayL()
    {
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpSink::SinkPlayL" )  
    
	if ( iPaused )
        {
        SendStreamEventToClient( KMccStreamResumed );
        }
    else
        {
    	SendStreamEventToClient( KMccStreamStarted );	
        }
		
    iPaused = EFalse;
    }

// -----------------------------------------------------------------------------
// CMccMsrpSink::SinkPauseL()
// 
// Pauses streaming by cancelling timers
// -----------------------------------------------------------------------------
//
void CMccMsrpSink::SinkPauseL()
    {
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpSink::SinkPauseL" )
    
    iPaused = ETrue;
	
	SendStreamEventToClient( KMccStreamPaused );	
	}

// -----------------------------------------------------------------------------
// CMccMsrpSink::SinkStopL()
// 
// Stops streaming
// -----------------------------------------------------------------------------
//
void CMccMsrpSink::SinkStopL()
    {
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpSink::SinkStopL" )

	SendStreamEventToClient( KMccStreamStopped );		
    }

// -----------------------------------------------------------------------------
// CMccMsrpSink::SinkDataTypeCode
// -----------------------------------------------------------------------------
//
TFourCC CMccMsrpSink::SinkDataTypeCode( TMediaId /*aMediaId*/ )
	{
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpSink::SinkDataTypeCode" )

    return iFourCC;
	}
	
// -----------------------------------------------------------------------------
// CMccMsrpSink::SetSinkDataTypeCode
// -----------------------------------------------------------------------------
//
TInt CMccMsrpSink::SetSinkDataTypeCode( TFourCC aCodec, 
                            TMediaId /*aMediaId*/ )
	{
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpSink::SetSinkDataTypeCode" )

    iFourCC = aCodec;

	return KErrNone;
	}	

// -----------------------------------------------------------------------------
// CMccMsrpSink::BufferEmptiedL
// -----------------------------------------------------------------------------
//
void CMccMsrpSink::BufferEmptiedL( CMMFBuffer* /*aBuffer*/ )
	{
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpSink::BufferEmptiedL" )
    
    User::Leave( KErrNotSupported );
	}
	
// -----------------------------------------------------------------------------
// CMccMsrpSink::CanCreateSinkBuffer
// -----------------------------------------------------------------------------
//
TBool CMccMsrpSink::CanCreateSinkBuffer()
	{
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpSink::CanCreateSinkBuffer" )
    
	return EFalse;
	}	

// -----------------------------------------------------------------------------
// CMccMsrpSink::CreateSinkBufferL
// -----------------------------------------------------------------------------
//
CMMFBuffer* CMccMsrpSink::CreateSinkBufferL( 
	TMediaId /*aMediaId*/, 
    TBool& /*aReference*/ )
	{
	__MSRPSOURCESINK_CONTROLL( "CMccMsrpSink::CreateSinkBufferL" )
	
	User::Leave( KErrNotSupported );
		
	return NULL;	
	}
	
// -----------------------------------------------------------------------------
// CMccMsrpSink::SinkThreadLogon
// -----------------------------------------------------------------------------
//
TInt CMccMsrpSink::SinkThreadLogon( MAsyncEventHandler& aEventHandler )
	{
	__MSRPSOURCESINK_CONTROLL( "CMccMsrpSink::SinkThreadLogon" )

	iAsyncEventHandler = &aEventHandler;

	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// CMccMsrpSink::SinkThreadLogoff
// -----------------------------------------------------------------------------
//
void CMccMsrpSink::SinkThreadLogoff()
	{
	__MSRPSOURCESINK_CONTROLL( "CMccMsrpSink::SinkThreadLogoff" )
    iAsyncEventHandler = NULL;		
	}

// -----------------------------------------------------------------------------
// CMccMsrpSink::EmptyBufferL
// -----------------------------------------------------------------------------
//
void CMccMsrpSink::EmptyBufferL( CMMFBuffer* aBuffer,
                  MDataSource* aProvider,
                  TMediaId /*aMediaId*/ )
	{
	__MSRPSOURCESINK_CONTROLL( "CMccMsrpSink::EmptyBufferL" )
    __ASSERT_ALWAYS( aBuffer, User::Leave( KErrArgument ) );
	__ASSERT_ALWAYS( aProvider, User::Leave( KErrArgument ) );
	
    CMMFDataBuffer* dataBuffer = static_cast<CMMFDataBuffer*>( aBuffer );
	const TDesC8& data = dataBuffer->Data();	
    iDataSender->SendL( data, iMsrpSession );
    aProvider->BufferEmptiedL( aBuffer ); 
	}	

// -----------------------------------------------------------------------------
// CMccMsrpSink::BufferFilledL
// -----------------------------------------------------------------------------
//
void CMccMsrpSink::BufferFilledL( CMMFBuffer* /*aBuffer*/ )
	{
	__MSRPSOURCESINK_CONTROLL( "CMccMsrpSink::BufferFilledL" )
	
	User::Leave( KErrNotSupported );
	}	


// -----------------------------------------------------------------------------
// CMccMsrpSink::BufferFilledL
// -----------------------------------------------------------------------------
//
void CMccMsrpSink::SetSessionParamsL( const TMccMsrpSessionParams& aParams )
    {
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpSink::SetSessionParamsL IN" )
    
    User::LeaveIfNull( aParams.iMsrp );
    User::LeaveIfNull( aParams.iMsrpSession );
    
    iMsrp = aParams.iMsrp;
    iMsrpSession = aParams.iMsrpSession;
    
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpSink::SetSessionParamsL OUT" )
    }
	
// -----------------------------------------------------------------------------
// CMccMsrpSink::SendStreamEventToClient
// -----------------------------------------------------------------------------
//	
void CMccMsrpSink::SendStreamEventToClient( 
    const TMccEventType& aEventType, 
    TInt aError )
	{
	__MSRPSOURCESINK_CONTROLL( "CMccMsrpSink::SendStreamEventToClient" )
	
	if ( iAsyncEventHandler )
	    {
        TMccEvent event( 0, 
                         0, 
                         0, 
                         MCC_ENDPOINT_ID( static_cast<MDataSink*>( this ) ), 
                         KMccEventCategoryStream, 
                         aEventType, 
                         aError, 
                         KNullDesC8 );
                     
        TMccInternalEvent internalEvent( KMccMsrpSinkUid, 
		                                 EMccInternalEventNone,
		                                 event );
		                         
		iAsyncEventHandler->SendEventToClient( internalEvent );
	    }
	}

// End of file

