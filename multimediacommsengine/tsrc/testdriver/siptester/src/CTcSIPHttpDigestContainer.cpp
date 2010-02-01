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
* Description:  See class definition below.
*
*/

#include "CTcSIPHttpDigestContainer.h"
#include "CTcSIPContext.h"
#include "CTcSIPConnectionContainer.h"
#include "SipConstants.h"
#include "ErrorHandling.h"
#include "SIPRefresh.h"

const TInt KReceivedChallengesQueueGranularity = 4;

CTcSIPHttpDigestContainer* CTcSIPHttpDigestContainer::NewL( 
    CTcSIPContext& aContext,
    CSIP& aSip )
    {
    CTcSIPHttpDigestContainer* self = 
        new ( ELeave ) CTcSIPHttpDigestContainer( aContext );
    CleanupStack::PushL( self );
    self->ConstructL( aSip );
    CleanupStack::Pop( self );
    return self;
    }

CTcSIPHttpDigestContainer::~CTcSIPHttpDigestContainer()
    {
    delete iSipHttpDigest;
    delete iTimer;
    iReceivedChallengesQueue.ResetAndDestroy();
    }


CTcSIPHttpDigestContainer::CTcSIPHttpDigestContainer( CTcSIPContext& aContext )
    : iContext( aContext ),
      iReceivedChallengesQueue( KReceivedChallengesQueueGranularity )
    {
    }
    
void CTcSIPHttpDigestContainer::ConstructL( CSIP& aSip )
    {
   	iTimer = CDeltaTimer::NewL( CActive::EPriorityStandard );
	TCallBack cb( ReceiveTimeout, this );
	iReceiveTimeout.Set( cb );
	
    iSipHttpDigest = CSIPHttpDigest::NewL( 
        aSip, static_cast< MSIPHttpDigestChallengeObserver& >( *this ) );
    }

void CTcSIPHttpDigestContainer::ChallengeReceived( const TDesC8& aRealm )
    {
    TRAPD( err, QueueReceivedChallengeL( aRealm ) );
    __ASSERT_ALWAYS( !err, Panic( KSIPErrOOMInNotifier ) );
    }

void CTcSIPHttpDigestContainer::ChallengeReceived( 
    const CSIPClientTransaction& aTransaction )
    {
    iContext.Connection().IncomingResponse( 
        const_cast< CSIPClientTransaction& >( aTransaction ) );
    }
   
void CTcSIPHttpDigestContainer::ChallengeReceived( 
    const CSIPRefresh& aRefresh )
    {
	TRAPD( err, {
		iContext.Registry().AddObjectL( aRefresh );
		TTcSIPReceived received;
		received.Set( &aRefresh );
		received.SetEvent( TTcSIPReceived::EIncomingResponse );
		iContext.Connection().QueueReceivedL( received );
		} )

	__ASSERT_ALWAYS( !err, Panic( KSIPErrOOMInNotifier ) );
    }

HBufC8* CTcSIPHttpDigestContainer::ReceivedChallengeL( TInt aTimeout )
    {
    if( iReceivedChallengesQueue.Count() == 0 )
		{
		// wait for items to arrive
		iTimer->Queue( aTimeout * KSecondAsMicros, iReceiveTimeout );
		iActiveWait.Start();
		}

	// Is the queue still empty ? (i.e. timeout occurred)
	if( iReceivedChallengesQueue.Count() == 0 )
		{
		User::Leave( KErrTimedOut );
		}
     
    HBufC8* challenge = iReceivedChallengesQueue[ 0 ];
    iReceivedChallengesQueue.Remove( 0 );
    iReceivedChallengesQueue.Compress();
    
    return challenge;
    }

void CTcSIPHttpDigestContainer::QueueReceivedChallengeL( const TDesC8& aRealm )
    {
    HBufC8* realm = aRealm.AllocLC();
    User::LeaveIfError( iReceivedChallengesQueue.Append( realm ) );
    CleanupStack::Pop(); //realm
    
    if( iActiveWait.IsStarted() )
		{
		iTimer->Remove( iReceiveTimeout );
		iActiveWait.AsyncStop();
		}
    }
    
TInt CTcSIPHttpDigestContainer::ReceiveTimeout( TAny* aSelf )
	{
	CTcSIPHttpDigestContainer& self =
				*reinterpret_cast< CTcSIPHttpDigestContainer* >( aSelf );
	if( self.iActiveWait.IsStarted() )
		{
		self.iActiveWait.AsyncStop();
		}

	return KErrNone;
	}
