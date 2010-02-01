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



#include "CTcMCEReceiveQueue.h"
#include "CTcMCEReceived.h"
#include "CTcMCEContext.h"

CTcMCEReceiveQueue* CTcMCEReceiveQueue::NewL( CTcMCEContext& aContext )
	{
	CTcMCEReceiveQueue* self = new (ELeave) CTcMCEReceiveQueue( aContext );

	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	
	return self;	
	}

CTcMCEReceiveQueue::CTcMCEReceiveQueue( CTcMCEContext& aContext ) :
	iReceiveQueue( 1 ),
	iContext( aContext )
	{
	}

void CTcMCEReceiveQueue::ConstructL()
	{
	iTimer = CDeltaTimer::NewL( CActive::EPriorityStandard );
	TCallBack cb( ReceiveTimeout, this );
	iReceiveTimeout.Set( cb );	
	}

CTcMCEReceiveQueue::~CTcMCEReceiveQueue()
	{
	delete iTimer;
	iReceiveQueue.Reset();
	}


// -- Received queue handling -------------------------------------------------

CTcMCEReceived& CTcMCEReceiveQueue::ReceivedItemL( TInt aTimeout )
	{
	if( iReceiveQueue.Count() == 0 )
		{
		iTimer->Queue( aTimeout * KSecondAsMicros, iReceiveTimeout );
		iActiveWait.Start();
		}

	// Check if the queue is still empty (i.e. timeout occurred)
	if( iReceiveQueue.Count() == 0 )
		{
		User::Leave( KErrTimedOut );
		}

	// Get the first (oldest) item from the array
	TBuf8< KTcMaxObjectName > itemName = iReceiveQueue[ 0 ];

	// ..and remove it from the array
	iReceiveQueue.Delete( 0 );
	iReceiveQueue.Compress();
	
	// Fetch the corresponding object from the registry
	CTcMCEReceived* item = 
		reinterpret_cast<CTcMCEReceived*>
			(iContext.Registry().ObjectPtrL( itemName ));
			
	return *item;
	
	}

void CTcMCEReceiveQueue::QueueReceivedL( CTcMCEReceived* aItem )
	{	
	// Add to registry and fetch name
	iContext.Registry().AddObjectL( reinterpret_cast< CBase* >( aItem ) );	
	TBuf8< KTcMaxObjectName > itemName = 
				iContext.Registry().ObjectNameL( aItem );	
	
	// Add name to queue
	iReceiveQueue.AppendL( itemName );

	if( iActiveWait.IsStarted() )
		{
		iTimer->Remove( iReceiveTimeout );
		iActiveWait.AsyncStop();
		}
		
	}

TInt CTcMCEReceiveQueue::ReceiveTimeout( TAny* aSelf )
	{
	CTcMCEReceiveQueue& self =
				*reinterpret_cast< CTcMCEReceiveQueue* >( aSelf );
	if( self.iActiveWait.IsStarted() )
		{
		self.iActiveWait.AsyncStop();
		}

	return KErrNone;
	}
