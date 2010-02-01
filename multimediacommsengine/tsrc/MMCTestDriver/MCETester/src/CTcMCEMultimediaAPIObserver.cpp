/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Implementation.
*
*/




#include "CTcMCEMultimediaAPIObserver.h"
#include "CTcMCEContext.h"

#include "MCEConstants.h"

#include "CTcMCEReceived.h"

#include <MceInSession.h>
#include <MCEAudioStream.h>
#include <MCESpeakerSink.h>
#include <MceMediaStream.h>
#include <MceRtpSource.h>
#include <MCEMicSource.h>



CTcMCEMultimediaAPIObserver* CTcMCEMultimediaAPIObserver::NewL( 
			CTcMCEContext& aContext )
	{
	CTcMCEMultimediaAPIObserver* self = 
			new (ELeave) CTcMCEMultimediaAPIObserver( aContext );
			
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	
	return self;
	}

CTcMCEMultimediaAPIObserver::CTcMCEMultimediaAPIObserver(
			CTcMCEContext& aContext):
	iContext(aContext)
	{
	}

void CTcMCEMultimediaAPIObserver::ConstructL()
	{
	iSessionQueue = CTcMCEReceiveQueue::NewL( iContext );
	iUpdatedSessionQueue = CTcMCEReceiveQueue::NewL( iContext );
	iStreamQueue = CTcMCEReceiveQueue::NewL( iContext );
	iRTPQueue = CTcMCEReceiveQueue::NewL( iContext );
	}

CTcMCEMultimediaAPIObserver::~CTcMCEMultimediaAPIObserver()
	{
	delete iSessionQueue;
	delete iUpdatedSessionQueue;
	delete iStreamQueue;
	delete iRTPQueue;
	}


// -- Queue getters ------------------------------------------------------------

CTcMCEReceived& CTcMCEMultimediaAPIObserver::ReceivedSessionItemL
		( TInt aTimeout )
	{
	return iSessionQueue->ReceivedItemL( aTimeout );	
	}

CTcMCEReceived& CTcMCEMultimediaAPIObserver::ReceivedUpdatedSessionItemL
		( TInt aTimeout )
	{
	return iUpdatedSessionQueue->ReceivedItemL( aTimeout );	
	}


CTcMCEReceived& CTcMCEMultimediaAPIObserver::ReceivedStreamItemL
		( TInt aTimeout )
	{
	return iStreamQueue->ReceivedItemL( aTimeout );	
	}

CTcMCEReceived& CTcMCEMultimediaAPIObserver::ReceivedRTPItemL( TInt aTimeout )
	{
	return iRTPQueue->ReceivedItemL( aTimeout );	
	}
		

// -- From MMceInSessionObserver--------------------------------------------------

void CTcMCEMultimediaAPIObserver::IncomingSession(
			CMceInSession* aSession,
            TMceTransactionDataContainer* aContainer )
	{
	TRAPD( err, {	

		CTcMCEReceived* received = new(ELeave) CTcMCEReceived;
				
		received->SetInSession( aSession ); 
        received->SetTransactionData( aContainer );
    
		iSessionQueue->QueueReceivedL( received );	
	 
	    } )	
	__ASSERT_ALWAYS( !err, User::Panic( KMceErrOOMInObserver, err ));		
	}

void CTcMCEMultimediaAPIObserver::IncomingUpdate(
					CMceSession& aOrigSession, 
					CMceInSession* aUpdatedSession,
    				TMceTransactionDataContainer* aContainer ) 
	{
	TRAPD( err, {	

		CTcMCEReceived* received = new(ELeave) CTcMCEReceived;
		//Can ask from incoming session if the session state is updated		
		received->SetIncomingUpdateSession( aOrigSession, aUpdatedSession ); 
        received->SetTransactionData( aContainer );
    
		iUpdatedSessionQueue->QueueReceivedL( received );	
	 
	    } )	
	__ASSERT_ALWAYS( !err, User::Panic( KMceErrOOMInObserver, err ));
	}


// -- From MMceSessionObserver -------------------------------------------------
	
void CTcMCEMultimediaAPIObserver::SessionStateChanged(
            CMceSession& aSession,
            TMceTransactionDataContainer* aContainer )
	{
	TRAPD( err, {		
		// Add a message to message queue	
		CTcMCEReceived* received = new(ELeave) CTcMCEReceived;
		received->SetSession( &aSession );
		received->SetSessionState( aSession.State() );
        received->SetTransactionData( aContainer );

		iSessionQueue->QueueReceivedL( received );		
		} )	
	__ASSERT_ALWAYS( !err, User::Panic( KMceErrOOMInObserver, err ));
	}
		
void CTcMCEMultimediaAPIObserver::SessionConnectionStateChanged(
            CMceSession& aSession,
            TBool /*aActive*/) 
	{
	TRAPD( err, {	
		CTcMCEReceived* received = new(ELeave) CTcMCEReceived;
		received->SetSession( &aSession );
		// aActive can be asked from session
		// iSessionQueue->QueueReceivedL( received ); NOT YET	
		} )	
	__ASSERT_ALWAYS( !err, User::Panic( KMceErrOOMInObserver, err ));	
	}
                    
void CTcMCEMultimediaAPIObserver::Failed( CMceSession& aSession, TInt aError )
	{
 	TRAPD( err, {	   
	    // Add an error to message queue	
		CTcMCEReceived* received = new(ELeave) CTcMCEReceived;
		received->SetSession( &aSession );
	    received->SetError( aError );	

		iSessionQueue->QueueReceivedL( received );	
		} )	
	__ASSERT_ALWAYS( !err, User::Panic( KMceErrOOMInObserver, err ));		
    }

void CTcMCEMultimediaAPIObserver::UpdateFailed(
	    			CMceSession& aSession,
	    			TMceTransactionDataContainer* aContainer ) 
	{
	TRAPD( err, {		
		// Add a message to message queue	
		CTcMCEReceived* received = new(ELeave) CTcMCEReceived;
		received->SetSession( &aSession );
		received->SetSessionState( aSession.State() );
        received->SetTransactionData( aContainer );

		iSessionQueue->QueueReceivedL( received );		
		} )	
	__ASSERT_ALWAYS( !err, User::Panic( KMceErrOOMInObserver, err ));	
	}

// -- from MMceStreamObserver --------------------------------------------------

void CTcMCEMultimediaAPIObserver::StreamStateChanged( CMceMediaStream& aStream )
	{
 	TRAPD( err, {	    
		CTcMCEReceived* received = new(ELeave) CTcMCEReceived;
		received->SetMediaStream( &aStream );
		received->SetStreamState( aStream.State() );

		iStreamQueue->QueueReceivedL( received );
		} )
	__ASSERT_ALWAYS( !err, User::Panic( KMceErrOOMInObserver, err ));
	}
	
void CTcMCEMultimediaAPIObserver::StreamStateChanged( 
			CMceMediaStream& aStream, 
			CMceMediaSink& aSink )
    {
	TRAPD( err, {	   	
		CTcMCEReceived* received = new(ELeave) CTcMCEReceived;
		received->SetMediaStream( &aStream );
		received->SetMediaSink( &aSink );
		received->SetStreamState( aStream.State() );
		
		iStreamQueue->QueueReceivedL( received );	 
		} )	
	__ASSERT_ALWAYS( !err, User::Panic( KMceErrOOMInObserver, err ));
	}
	
void CTcMCEMultimediaAPIObserver::StreamStateChanged( 
			CMceMediaStream& aStream, 
			CMceMediaSource& aSource)
	{
	TRAPD( err, {	   	
		CTcMCEReceived* received = new(ELeave) CTcMCEReceived;
		received->SetMediaStream( &aStream );
		received->SetMediaSource( &aSource );
		received->SetStreamState( aStream.State() );
		
		iStreamQueue->QueueReceivedL( received );	 
		} )	
	__ASSERT_ALWAYS( !err, User::Panic( KMceErrOOMInObserver, err ));
	}


// -- from MMceRtpObserver -----------------------------------------------------

void CTcMCEMultimediaAPIObserver::SRReceived( 
			CMceSession& aSession,
			CMceMediaStream& aStream )
	{
	TRAPD( err, {	   
		CTcMCEReceived* received = new(ELeave) CTcMCEReceived;
		received->SetMediaStream( &aStream );
		if ( aStream.Source()->Type() == KMceRTPSource )
			{
			CMceRtpSource* source = static_cast<CMceRtpSource*>( aStream.Source() );
			received->SetRtpSource( source );			
			}
		received->SetSession( &aSession );
		iRTPQueue->QueueReceivedL( received );	 
		} )	
	__ASSERT_ALWAYS( !err, User::Panic( KMceErrOOMInObserver, err ));
	}

void CTcMCEMultimediaAPIObserver::RRReceived( 
			CMceSession& aSession,
			CMceMediaStream& aStream )
	{
	TRAPD( err, {	   
		CTcMCEReceived* received = new(ELeave) CTcMCEReceived;
		received->SetMediaStream( &aStream );
		if ( aStream.Source()->Type() == KMceRTPSource )
			{
			CMceRtpSource* source = static_cast<CMceRtpSource*>( aStream.Source() );
			received->SetRtpSource( source );			
			}
		received->SetSession( &aSession );
		iRTPQueue->QueueReceivedL( received );	 
		} )	
	__ASSERT_ALWAYS( !err, User::Panic( KMceErrOOMInObserver, err ));
	}
		
void CTcMCEMultimediaAPIObserver::InactivityTimeout( 
			CMceMediaStream& aStream,
			CMceRtpSource& aSource )
	{
	TRAPD( err, {	   
		CTcMCEReceived* received = new(ELeave) CTcMCEReceived;
		received->SetMediaStream( &aStream );
		received->SetRtpSource( &aSource );
		received->SetType(KTcMceInactivityTimeout);
		iRTPQueue->QueueReceivedL( received );	 
		} )	
	__ASSERT_ALWAYS( !err, User::Panic( KMceErrOOMInObserver, err ));
	}

void CTcMCEMultimediaAPIObserver::SsrcAdded( 
			CMceMediaStream& /*aStream*/,
			CMceRtpSource& /*aSource*/,
			TUint /*aSsrc*/ )
	{
	}

void CTcMCEMultimediaAPIObserver::SsrcRemoved( 
			CMceMediaStream& /*aStream*/,
			CMceRtpSource& /*aSource*/,
			TUint /*aSsrc*/ )
	{
	}


// -- from MMceTransactionObserver ---------------------------------------------

void CTcMCEMultimediaAPIObserver::TransactionResponse(
	CMceSession& aSession,
	TMceTransactionId aTransactionId,
	TMceTransactionDataContainer* aContainer )
	{
	TRAPD( err, {
		CTcMCEReceived* received = new ( ELeave ) CTcMCEReceived;
		received->SetSession( &aSession );
		received->SetTransactionId( aTransactionId );
        received->SetTransactionData( aContainer );

        // "received" must not be in CleanupStack when calling QueueReceivedL,
        // which calls CTcRegistry::AddObjectL (see its comments).

    	// GetMessage command reads this queue.
		iSessionQueue->QueueReceivedL( received );
	    } )	
	__ASSERT_ALWAYS( !err, User::Panic( KMceErrOOMInObserver, err ) );
	}

void CTcMCEMultimediaAPIObserver::TransactionFailed(
    CMceSession& aSession,
	TMceTransactionId aTransactionId, 
	TInt aError )
	{
	TRAPD( err, {	   
	    // Add an error to message queue	
		CTcMCEReceived* received = new ( ELeave ) CTcMCEReceived;
		received->SetSession( &aSession );
		received->SetTransactionId( aTransactionId );
	    received->SetError( aError );	

		// "received" must not be in CleanupStack when calling QueueReceivedL,
        // which calls CTcRegistry::AddObjectL (see its comments).
		iSessionQueue->QueueReceivedL( received );
		} )	
	__ASSERT_ALWAYS( !err, User::Panic( KMceErrOOMInObserver, err ) );
	}


// -- from MMceInTransactionObserver -------------------------------------------

void CTcMCEMultimediaAPIObserver::IncomingRequest(
	const TDesC8& aMethod,
	CMceSession& aSession,
	TMceTransactionId aTransactionId,
	TMceTransactionDataContainer* aContainer )
	{
	TRAPD( err, {
		CTcMCEReceived* received = new ( ELeave ) CTcMCEReceived;
		received->SetSession( &aSession );

		CleanupStack::PushL( received );
		received->SetMethodL( aMethod );
		CleanupStack::Pop( received );

		received->SetTransactionId( aTransactionId );
        received->SetTransactionData( aContainer );

    	// "received" must not be in CleanupStack when calling QueueReceivedL,
        // which calls CTcRegistry::AddObjectL (see its comments).
		iSessionQueue->QueueReceivedL( received );
	    } )	
	__ASSERT_ALWAYS( !err, User::Panic( KMceErrOOMInObserver, err ) );
	}
