/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
#include "mccanysink.h"
#include "mmccinterfacedef.h"
#include "mccanysourcesinklogs.h"
#include "mccinternalevents.h"
#include "mccinternaldef.h"

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccAnySink::NewSinkL
// -----------------------------------------------------------------------------
//
MDataSink* CMccAnySink::NewSinkL( TUid /*aImplementationUid*/, 
                                   const TDesC8& /*aInitData*/ )
    {
    __ANYSOURCESINK_CONTROLL( "CMccAnySink::NewSinkL" )
    
    CMccAnySink* self = new ( ELeave ) CMccAnySink();
    return static_cast<MDataSink*>( self );
    }

// -----------------------------------------------------------------------------
// CMccAnySink::ConstructSinkL
// -----------------------------------------------------------------------------
//
void CMccAnySink::ConstructSinkL( const TDesC8& /*aInitData*/ )
    {
    __ANYSOURCESINK_CONTROLL( "CMccAnySink::ConstructSinkL" )
    }
    
// -----------------------------------------------------------------------------
// CMccAnySink::CMccAnySink
// -----------------------------------------------------------------------------
//
CMccAnySink::CMccAnySink() : CMccDataSink( KMccAnySinkUid )
    {
    }
        
// -----------------------------------------------------------------------------
// CMccAnySink::~CMccAnySink
// -----------------------------------------------------------------------------
//
CMccAnySink::~CMccAnySink()
    {
    __ANYSOURCESINK_CONTROLL( "CMccAnySink::~CMccAnySink" )
    }

// -----------------------------------------------------------------------------
// CMccAnySink::SinkPrimeL
// -----------------------------------------------------------------------------
//
void CMccAnySink::SinkPrimeL()
	{
	__ANYSOURCESINK_CONTROLL( "CMccAnySink::SinkPrimeL" )

	SendStreamEventToClient( KMccStreamPrepared );		
	}
	
// -----------------------------------------------------------------------------
// CMccAnySink::SinkPlayL()
// -----------------------------------------------------------------------------
//
void CMccAnySink::SinkPlayL()
    {
    __ANYSOURCESINK_CONTROLL( "CMccAnySink::SinkPlayL" )  
    
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
// CMccAnySink::SinkPauseL()
// 
// Pauses streaming by cancelling timers
// -----------------------------------------------------------------------------
//
void CMccAnySink::SinkPauseL()
    {
    __ANYSOURCESINK_CONTROLL( "CMccAnySink::SinkPauseL" )
    
    iPaused = ETrue;
	
	SendStreamEventToClient( KMccStreamPaused );	
	}

// -----------------------------------------------------------------------------
// CMccAnySink::SinkStopL()
// 
// Stops streaming
// -----------------------------------------------------------------------------
//
void CMccAnySink::SinkStopL()
    {
    __ANYSOURCESINK_CONTROLL( "CMccAnySink::SinkStopL" )

	SendStreamEventToClient( KMccStreamStopped );		
    }

// -----------------------------------------------------------------------------
// CMccAnySink::SinkDataTypeCode
// -----------------------------------------------------------------------------
//
TFourCC CMccAnySink::SinkDataTypeCode( TMediaId /*aMediaId*/ )
	{
    __ANYSOURCESINK_CONTROLL( "CMccAnySink::SinkDataTypeCode" )

    return iFourCC;
	}
	
// -----------------------------------------------------------------------------
// CMccAnySink::SetSinkDataTypeCode
// -----------------------------------------------------------------------------
//
TInt CMccAnySink::SetSinkDataTypeCode( TFourCC aCodec, 
                            TMediaId /*aMediaId*/ )
	{
    __ANYSOURCESINK_CONTROLL( "CMccAnySink::SetSinkDataTypeCode" )

    iFourCC = aCodec;

	return KErrNone;
	}	

// -----------------------------------------------------------------------------
// CMccAnySink::BufferEmptiedL
// -----------------------------------------------------------------------------
//
void CMccAnySink::BufferEmptiedL( CMMFBuffer* /*aBuffer*/ )
	{
    __ANYSOURCESINK_CONTROLL( "CMccAnySink::BufferEmptiedL" )
    
    User::Leave( KErrNotSupported );
	}
	
// -----------------------------------------------------------------------------
// CMccAnySink::CanCreateSinkBuffer
// -----------------------------------------------------------------------------
//
TBool CMccAnySink::CanCreateSinkBuffer()
	{
    __ANYSOURCESINK_CONTROLL( "CMccAnySink::CanCreateSinkBuffer" )
    
	return EFalse;
	}	

// -----------------------------------------------------------------------------
// CMccAnySink::CreateSinkBufferL
// -----------------------------------------------------------------------------
//
CMMFBuffer* CMccAnySink::CreateSinkBufferL( 
	TMediaId /*aMediaId*/, 
    TBool& /*aReference*/ )
	{
	__ANYSOURCESINK_CONTROLL( "CMccAnySink::CreateSinkBufferL" )
	
	User::Leave( KErrNotSupported );
		
	return NULL;	
	}
	
// -----------------------------------------------------------------------------
// CMccAnySink::SinkThreadLogon
// -----------------------------------------------------------------------------
//
TInt CMccAnySink::SinkThreadLogon( MAsyncEventHandler& aEventHandler )
	{
	__ANYSOURCESINK_CONTROLL( "CMccAnySink::SinkThreadLogon" )

	iAsyncEventHandler = &aEventHandler;

	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// CMccAnySink::SinkThreadLogoff
// -----------------------------------------------------------------------------
//
void CMccAnySink::SinkThreadLogoff()
	{
	__ANYSOURCESINK_CONTROLL( "CMccAnySink::SinkThreadLogoff" )
    iAsyncEventHandler = NULL;		
	}

// -----------------------------------------------------------------------------
// CMccAnySink::EmptyBufferL
// -----------------------------------------------------------------------------
//
void CMccAnySink::EmptyBufferL( CMMFBuffer* /*aBuffer*/,
                  MDataSource* /*aProvider*/,
                  TMediaId /*aMediaId*/ )
	{
	__ANYSOURCESINK_CONTROLL( "CMccAnySink::EmptyBufferL" )
	
	User::Leave( KErrNotSupported );
	}	

// -----------------------------------------------------------------------------
// CMccAnySink::BufferFilledL
// -----------------------------------------------------------------------------
//
void CMccAnySink::BufferFilledL( CMMFBuffer* /*aBuffer*/ )
	{
	__ANYSOURCESINK_CONTROLL( "CMccAnySink::BufferFilledL" )
	
	User::Leave( KErrNotSupported );
	}	                  
	
// -----------------------------------------------------------------------------
// CMccAnySink::SendStreamEventToClient
// -----------------------------------------------------------------------------
//	
void CMccAnySink::SendStreamEventToClient( 
    const TMccEventType& aEventType, 
    TInt aError )
	{
	__ANYSOURCESINK_CONTROLL( "CMccAnySink::SendStreamEventToClient" )
	
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
                     
        TMccInternalEvent internalEvent( KMccAnySinkUid, 
		                                 EMccInternalEventNone,
		                                 event );
		                         
		iAsyncEventHandler->SendEventToClient( internalEvent );
	    }
	}

// End of file

