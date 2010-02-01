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



#include "CTcMCEFloorControlObserver.h"
#include "CTcMCEContext.h"

#include "MCEConstants.h"

#include "CTcMCEReceived.h"
#include "CTcMCEReceiveQueue.h"



CTcMCEFloorControlObserver* CTcMCEFloorControlObserver::NewL( 
			CTcMCEContext& aContext )
	{
	CTcMCEFloorControlObserver* self = 
			new (ELeave) CTcMCEFloorControlObserver( aContext );
			
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	
	return self;
	}

CTcMCEFloorControlObserver::CTcMCEFloorControlObserver(
			CTcMCEContext& aContext):
	iContext(aContext)
	{
	}

void CTcMCEFloorControlObserver::ConstructL()
	{
	iFCQueue = CTcMCEReceiveQueue::NewL( iContext );
	}

CTcMCEFloorControlObserver::~CTcMCEFloorControlObserver()
	{
	delete iFCQueue;
	}



CTcMCEReceived& CTcMCEFloorControlObserver::ReceivedFCItemL( TInt aTimeout )
	{
	return iFCQueue->ReceivedItemL( aTimeout );	
	}

// -- From MMceFcMsgObserver ---------------------------------------------------

void CTcMCEFloorControlObserver::FCMsgReceived(
			CMceSession& aSession, 
			HBufC8* aMessage)
	{
	TRAPD( err, {	
		
		CTcMCEReceived* received = new(ELeave) CTcMCEReceived;
		received->SetSession( &aSession ); 
		received->SetFCMessage( aMessage );		

		iFCQueue->QueueReceivedL( received );	
		} )	
	__ASSERT_ALWAYS( !err, User::Panic( KMceErrOOMInObserver, err ));
	}

void CTcMCEFloorControlObserver::FCMsgErrorOccurred( 
			CMceSession& aSession, 
			TInt aError )
	{
	TRAPD( err, {
		CTcMCEReceived* received = new(ELeave) CTcMCEReceived;
		received->SetSession( &aSession ); 
		received->SetError( aError );		

		iFCQueue->QueueReceivedL( received );
		} )	
	__ASSERT_ALWAYS( !err, User::Panic( KMceErrOOMInObserver, err ));	
	}	
