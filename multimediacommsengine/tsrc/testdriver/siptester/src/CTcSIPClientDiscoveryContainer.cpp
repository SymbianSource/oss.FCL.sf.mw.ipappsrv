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
* Description:  See class definition below.
*
*/

#include "CTcSIPClientDiscoveryContainer.h"
#include "SipConstants.h"
#include "ErrorHandling.h"

#include <sipclientdiscovery.h>
#include <sipheaderbase.h>

const TInt KReceivedEventsQueueGranularity = 4;

CTcSIPClientDiscoveryContainer* CTcSIPClientDiscoveryContainer::NewL( 
    CTcSIPContext& aContext,
    TUid aUid )
    {
    CTcSIPClientDiscoveryContainer* self = 
        new ( ELeave ) CTcSIPClientDiscoveryContainer( aContext, aUid );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

CTcSIPClientDiscoveryContainer::~CTcSIPClientDiscoveryContainer()
    {
    delete iSipClientDiscovery;
    delete iTimer;
    iReceivedEventsQueue.Reset();
    }


CTcSIPClientDiscoveryContainer::CTcSIPClientDiscoveryContainer( CTcSIPContext& aContext, TUid aUid )
    : iContext( aContext ),
      iReceivedEventsQueue( KReceivedEventsQueueGranularity ),
      iUid( aUid )
    {
    }
    
void CTcSIPClientDiscoveryContainer::ConstructL()
    {
   	iTimer = CDeltaTimer::NewL( CActive::EPriorityStandard );
	TCallBack cb( ReceiveTimeout, this );
	iReceiveTimeout.Set( cb );
	
    iSipClientDiscovery = CSIPClientDiscovery::NewL( *this, iUid );
    }

void CTcSIPClientDiscoveryContainer::ChannelResolvedL( TUid aChannel, TUint32 aRequestId )
    {
    TTcClientDiscoveryEvent event;
    event.iType = ETcClientDiscoveryChannelResolved;
    event.iChannel = aChannel;
    event.iRequestId = aRequestId;
    TRAPD( err, QueueReceivedEventL( event ) );
    __ASSERT_ALWAYS( !err, Panic( KSIPErrOOMInNotifier ) );
    }

void CTcSIPClientDiscoveryContainer::ClientNotFoundL( 
    TUint32 aRequestId,
	TUint /*aStatusCode*/, 
	RStringF /*aReasonPhrase*/, 
	RPointerArray<CSIPHeaderBase> aHeaders,
	HBufC8* aContent )
    {
    // At the moment care only about requestId
    TTcClientDiscoveryEvent event;
    event.iType = ETcClientDiscoveryClientNotFound;
    event.iRequestId = aRequestId;
    TRAPD( err, QueueReceivedEventL( event ) );
    __ASSERT_ALWAYS( !err, Panic( KSIPErrOOMInNotifier ) );
    
    aHeaders.ResetAndDestroy();
    delete aContent;
    }

void CTcSIPClientDiscoveryContainer::ErrorOccurred( TInt aError, TUint32 aRequestId )
    {
    TTcClientDiscoveryEvent event;
    event.iType = ETcClientDiscoveryError;
    event.iRequestId = aRequestId;
    event.iError = aError;
    TRAPD( err, QueueReceivedEventL( event ) );
    __ASSERT_ALWAYS( !err, Panic( KSIPErrOOMInNotifier ) );
    }
	    

TTcClientDiscoveryEvent CTcSIPClientDiscoveryContainer::ReceivedClientDiscoveryEventL( TInt aTimeout )
    {
    if( iReceivedEventsQueue.Count() == 0 )
		{
		// wait for items to arrive
		iTimer->Queue( aTimeout * KSecondAsMicros, iReceiveTimeout );
		iActiveWait.Start();
		}

	// Is the queue still empty ? (i.e. timeout occurred)
	if( iReceivedEventsQueue.Count() == 0 )
		{
		User::Leave( KErrTimedOut );
		}
     
    TTcClientDiscoveryEvent event = iReceivedEventsQueue[ 0 ];
    iReceivedEventsQueue.Delete( 0 );
    iReceivedEventsQueue.Compress();
    
    return event;
    }

void CTcSIPClientDiscoveryContainer::QueueReceivedEventL( TTcClientDiscoveryEvent& aClientDiscoveryEvent )
    {
	TRAPD( err, iReceivedEventsQueue.AppendL( aClientDiscoveryEvent ) );
	__ASSERT_ALWAYS( !err, Panic( KSIPErrOOMInNotifier ) );

	if( iActiveWait.IsStarted() )
		{
		iTimer->Remove( iReceiveTimeout );
		iActiveWait.AsyncStop();
		}
    }
    
TInt CTcSIPClientDiscoveryContainer::ReceiveTimeout( TAny* aSelf )
	{
	CTcSIPClientDiscoveryContainer& self =
				*reinterpret_cast< CTcSIPClientDiscoveryContainer* >( aSelf );
	if( self.iActiveWait.IsStarted() )
		{
		self.iActiveWait.AsyncStop();
		}

	return KErrNone;
	}
