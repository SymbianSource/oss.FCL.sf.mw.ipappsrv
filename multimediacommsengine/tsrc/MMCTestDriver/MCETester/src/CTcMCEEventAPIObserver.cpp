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



#include "CTcMCEEventAPIObserver.h"
#include "CTcMCEContext.h"

#include "MCEConstants.h"
#include "CTcMCEReceiveQueue.h"
#include "CTcMCEReceived.h"

#include <MCEEvent.h>
#include <MCERefer.h>
#include <MceInEvent.h>
#include <MceInRefer.h>


CTcMCEEventAPIObserver* CTcMCEEventAPIObserver::NewL( 
			CTcMCEContext& aContext )
	{
	CTcMCEEventAPIObserver* self = 
			new (ELeave) CTcMCEEventAPIObserver( aContext );
			
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	
	return self;
	}

CTcMCEEventAPIObserver::CTcMCEEventAPIObserver(
			CTcMCEContext& aContext):
	iContext(aContext)
	{
	}

void CTcMCEEventAPIObserver::ConstructL()
	{
	iEventQueue = CTcMCEReceiveQueue::NewL( iContext );
	}

CTcMCEEventAPIObserver::~CTcMCEEventAPIObserver()
	{
	delete iEventQueue;
	}


CTcMCEReceived& CTcMCEEventAPIObserver::ReceivedEventItemL( TInt aTimeout )
	{
	return iEventQueue->ReceivedItemL( aTimeout );	
	}

// -- from MMceEventObserver ---------------------------------------------------
    
void CTcMCEEventAPIObserver::EventStateChanged(
			CMceEvent& aEvent,
			TMceTransactionDataContainer* aContainer )
	{
	TRAPD( err, {
		CTcMCEReceived* received = new(ELeave) CTcMCEReceived;
		received->SetEvent( &aEvent );
		received->SetTransactionData( aContainer );
		received->SetEventState( aEvent.State() );
		iEventQueue->QueueReceivedL( received );	
		} )	
	__ASSERT_ALWAYS( !err, User::Panic( KMceErrOOMInObserver, err ));
	}

void CTcMCEEventAPIObserver::NotifyReceived(
			CMceEvent& aEvent,
			TMceTransactionDataContainer* aContainer )
	{
	TRAPD( err, {
		CTcMCEReceived* received = new(ELeave) CTcMCEReceived;
		received->SetEvent( &aEvent );
		received->SetTransactionData( aContainer );

		iEventQueue->QueueReceivedL( received );	
		} )	
	__ASSERT_ALWAYS( !err, User::Panic( KMceErrOOMInObserver, err ));
	}		

void CTcMCEEventAPIObserver::EventConnectionStateChanged(
			CMceEvent& aEvent,
			TBool /*aActive*/ )
	{
	TRAPD( err, {
		CTcMCEReceived* received = new(ELeave) CTcMCEReceived;
		received->SetEvent( &aEvent );
		// aActive should be stored

		iEventQueue->QueueReceivedL( received );	
		} )	
	__ASSERT_ALWAYS( !err, User::Panic( KMceErrOOMInObserver, err ));
	}		

void CTcMCEEventAPIObserver::Failed(CMceEvent& aEvent, TInt aError)
	{
	TRAPD( err, {
		CTcMCEReceived* received = new(ELeave) CTcMCEReceived;
		received->SetEvent( &aEvent );
		received->SetError( aError );

		iEventQueue->QueueReceivedL( received );	
		} )	
	__ASSERT_ALWAYS( !err, User::Panic( KMceErrOOMInObserver, err ));

	}
	
// -- from MMceInEventObserver -------------------------------------------------

void CTcMCEEventAPIObserver::IncomingEvent(
			CMceInEvent* aEvent,
			TMceTransactionDataContainer* aContainer )
	{
	TRAPD( err, {	
		CTcMCEReceived* received = new(ELeave) CTcMCEReceived;
		received->SetInEvent( aEvent );
		received->SetTransactionData( aContainer );

		iEventQueue->QueueReceivedL( received );	
		} )	
	__ASSERT_ALWAYS( !err, User::Panic( KMceErrOOMInObserver, err ));
	}

		
// -- from MMceReferObserver ---------------------------------------------------

void CTcMCEEventAPIObserver::ReferStateChanged(
        			CMceRefer& aRefer,
    				TMceTransactionDataContainer* aContainer )
    {
	TRAPD( err, {
		CTcMCEReceived* received = new(ELeave) CTcMCEReceived;
		received->SetRefer( &aRefer );
		received->SetTransactionData( aContainer );
		received->SetReferState( aRefer.State() );
		iEventQueue->QueueReceivedL( received );	
		} )	
	__ASSERT_ALWAYS( !err, User::Panic( KMceErrOOMInObserver, err ));    
    }

void CTcMCEEventAPIObserver::ReferConnectionStateChanged(
			CMceRefer& aRefer,
			TBool /*aActive*/ )
	{
	TRAPD( err, {
		CTcMCEReceived* received = new(ELeave) CTcMCEReceived;
		received->SetRefer( &aRefer );
		// aActive should be stored

		iEventQueue->QueueReceivedL( received );	
		} )	
	__ASSERT_ALWAYS( !err, User::Panic( KMceErrOOMInObserver, err ));
	}	

void CTcMCEEventAPIObserver::Failed( CMceRefer& aRefer, TInt aError )
	{
	TRAPD( err, {
		CTcMCEReceived* received = new(ELeave) CTcMCEReceived;
		received->SetRefer( &aRefer );
		received->SetError( aError );

		iEventQueue->QueueReceivedL( received );	
		} )	
	__ASSERT_ALWAYS( !err, User::Panic( KMceErrOOMInObserver, err ));

	}
    
void CTcMCEEventAPIObserver::IncomingRefer(
					CMceInRefer* aRefer,
					const TDesC8& aReferTo,
    				TMceTransactionDataContainer* aContainer )
	{
	TRAPD( err, {
		CTcMCEReceived* received = new(ELeave) CTcMCEReceived;
		received->SetInRefer( aRefer ); 
		received->SetReferTo( aReferTo );
		received->SetTransactionData( aContainer );
		
		iEventQueue->QueueReceivedL( received );	
		} )	
	__ASSERT_ALWAYS( !err, User::Panic( KMceErrOOMInObserver, err ));
	}


    	
