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
#include "mccanysource.h"
#include "mmccinterfacedef.h"
#include "mccanysourcesinklogs.h"
#include "mccinternalevents.h"
#include "mccinternaldef.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccAnySource::NewSourceL
// -----------------------------------------------------------------------------
//
MDataSource* CMccAnySource::NewSourceL( TUid /*aImplementationUid*/, 
                                         const TDesC8& /*aInitData*/ )
    {
    __ANYSOURCESINK_CONTROLL( "CMccAnySource::NewSourceL" )
    
    CMccAnySource* self = new ( ELeave ) CMccAnySource();
    return static_cast<MDataSource*>( self );
    }

// -----------------------------------------------------------------------------
// CMccAnySource::ConstructSourceL
// -----------------------------------------------------------------------------
//
void CMccAnySource::ConstructSourceL( const TDesC8& /*aInitData*/ )
    {    	
    __ANYSOURCESINK_CONTROLL( "CMccAnySource::ConstructSourceL" )
    }
	    
// -----------------------------------------------------------------------------
// Constructor.
// -----------------------------------------------------------------------------
//
CMccAnySource::CMccAnySource() : CMccDataSource( KMccAnySourceUid )
    {
    }
        
// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
//
CMccAnySource::~CMccAnySource()
    {   
    __ANYSOURCESINK_CONTROLL( "CMccAnySource::~CMccAnySource" )
    }

	
// -----------------------------------------------------------------------------
// CMccAnySource::SourcePrimeL()
// -----------------------------------------------------------------------------
//
void CMccAnySource::SourcePrimeL()
	{
    __ANYSOURCESINK_CONTROLL( "CMccAnySource::SourcePrimeL" )
    
    SendStreamEventToClient( KMccStreamPrepared );
	}
	    
// -----------------------------------------------------------------------------
// CMccAnySource::PlayL()
// -----------------------------------------------------------------------------
//
void CMccAnySource::SourcePlayL()
    {
    __ANYSOURCESINK_CONTROLL( "CMccAnySource::SourcePlayL" )
    
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
// CMccAnySource::PauseL()
// -----------------------------------------------------------------------------
//
void CMccAnySource::SourcePauseL()
    {
    __ANYSOURCESINK_CONTROLL( "CMccAnySource::SourcePauseL" )
    
    iPaused = ETrue;
    
    SendStreamEventToClient( KMccStreamPaused );
	}

// -----------------------------------------------------------------------------
// CMccAnySource::StopL()
// -----------------------------------------------------------------------------
//
void CMccAnySource::SourceStopL()
    {
    __ANYSOURCESINK_CONTROLL( "CMccAnySource::SourceStopL" )
    
    SendStreamEventToClient( KMccStreamStopped );
    }

// -----------------------------------------------------------------------------
// CMccAnySource::SourceDataTypeCode()
// -----------------------------------------------------------------------------
//
TFourCC CMccAnySource::SourceDataTypeCode( TMediaId /*aMediaId*/ )
	{
	__ANYSOURCESINK_CONTROLL( "CMccAnySource::SourceDataTypeCode" )
	
	return iFourCC;
	}
	
// -----------------------------------------------------------------------------
// CMccAnySource::SetSourceDataTypeCode()
// -----------------------------------------------------------------------------
//
TInt CMccAnySource::SetSourceDataTypeCode( TFourCC aCodec, 
                            TMediaId /*aMediaId*/ )
	{
	__ANYSOURCESINK_CONTROLL( "CMccAnySource::SetSourceDataTypeCode" )
	
	iFourCC = aCodec;
	return KErrNone;
	}	

// -----------------------------------------------------------------------------
// CMccAnySource::BufferEmptiedL()
// -----------------------------------------------------------------------------
//
void CMccAnySource::BufferEmptiedL( CMMFBuffer* /*aBuffer*/ )
	{
    __ANYSOURCESINK_CONTROLL( "CMccAnySource::BufferEmptiedL" )
    
   	User::Leave( KErrNotSupported );
	}
	
// -----------------------------------------------------------------------------
// CMccAnySource::CanCreateSourceBuffer()
// -----------------------------------------------------------------------------
//
TBool CMccAnySource::CanCreateSourceBuffer()
	{
	__ANYSOURCESINK_CONTROLL( "CMccAnySource::CanCreateSourceBuffer" )

    return EFalse;
	}	

// -----------------------------------------------------------------------------
// CMccAnySource::CreateSourceBufferL()
// -----------------------------------------------------------------------------
//
CMMFBuffer* CMccAnySource::CreateSourceBufferL( 
	TMediaId /*aMediaId*/, 
    TBool& /*aReference*/ )
	{
	__ANYSOURCESINK_CONTROLL( "CMccAnySource::CreateSourceBufferL" )

   	User::Leave( KErrNotSupported );
	
	return NULL;	
	}
	
// -----------------------------------------------------------------------------
// CMccAnySource::SourceThreadLogon()
// -----------------------------------------------------------------------------
//
TInt CMccAnySource::SourceThreadLogon( MAsyncEventHandler& aEventHandler )
	{
	__ANYSOURCESINK_CONTROLL( "CMccAnySource::SourceThreadLogon" )
	
	iAsyncEventHandler = &aEventHandler;
	
    return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CMccAnySource::SourceThreadLogoff()
// -----------------------------------------------------------------------------
//
void CMccAnySource::SourceThreadLogoff()
	{
    __ANYSOURCESINK_CONTROLL( "CMccAnySource::SourceThreadLogoff" )
    
    iAsyncEventHandler = NULL;
	}

// -----------------------------------------------------------------------------
// CMccAnySource::FillBufferL()
// -----------------------------------------------------------------------------
//
void CMccAnySource::FillBufferL( 
	CMMFBuffer* /*aBuffer*/,
    MDataSink* /*aConsumer*/,
    TMediaId /*aMediaId*/ )
	{
	__ANYSOURCESINK_CONTROLL( "CMccAnySource::FillBufferL" )
	User::Leave( KErrNotSupported );
	}	
                  
// -----------------------------------------------------------------------------
// CMccAnySource::NegotiateSourceL()
// -----------------------------------------------------------------------------
//
void CMccAnySource::NegotiateSourceL( MDataSink& /*aDataSink*/ )
	{
    __ANYSOURCESINK_CONTROLL( "CMccAnySource::NegotiateSourceL" )
	}

// -----------------------------------------------------------------------------
// CMccAnySource::SendStreamEventToClient
// -----------------------------------------------------------------------------
//	
void CMccAnySource::SendStreamEventToClient( 
    TMccEventType aEventType, 
    TInt aError )
	{
	__ANYSOURCESINK_CONTROLL( "CMccAnySource::SendStreamEventToClient" )
	
	if ( iAsyncEventHandler )
	    {
        TMccEvent event( 0, 
                         0, 
                         0, 
                         MCC_ENDPOINT_ID( static_cast<MDataSource*>( this ) ), 
                         KMccEventCategoryStream, 
                         aEventType, 
                         aError, 
                         KNullDesC8 );
                     
        TMccInternalEvent internalEvent( KMccAnySourceUid, 
		                                 EMccInternalEventNone,
		                                 event );
		                         
		iAsyncEventHandler->SendEventToClient( internalEvent );
	    }
	}

// End of file

