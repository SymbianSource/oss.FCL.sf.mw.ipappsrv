/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation.
*
*/

#include "CTcSIPConnectionContainer.h"
#include "TTcSIPReceived.h"
#include "CTcSIPContext.h"
#include "SipConstants.h"

#include "TesterConstants.h"
#include "ErrorHandling.h"

// SIP stack includes
#include <sip.h>
#include <sipconnection.h>
#include <sipdialog.h>
#include <sipdialogassocbase.h>
#include <sipheaderbase.h>
#include <sipcseqheader.h>
#include <sipextensionheader.h>
#include <siprefresh.h>
#include <sipregistrationbinding.h>
#include <sipresponseelements.h>
#include <sipclienttransaction.h>
#include <sipmessageelements.h>

CTcSIPConnectionContainer* CTcSIPConnectionContainer::NewL(
													CTcSIPContext& aContext,
													const TDesC8& aIAPName )
	{
	CTcSIPConnectionContainer* self =
						new( ELeave ) CTcSIPConnectionContainer( aContext,
																 aIAPName );

	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );

	return self;
	}

CTcSIPConnectionContainer::~CTcSIPConnectionContainer()
	{
	delete iSipConnection;
	delete iTimer;
	iReceiveQueue.Reset();
	}

CTcSIPConnectionContainer::CTcSIPConnectionContainer( CTcSIPContext& aContext,
													  const TDesC8& aIAPName )
	: iContext( aContext ), iReceiveQueue( 1 ), iIAPName( aIAPName )
	{
	}

void CTcSIPConnectionContainer::ConstructL()
	{
	iTimer = CDeltaTimer::NewL( CActive::EPriorityStandard );
	TCallBack cb( ReceiveTimeout, this );
	iReceiveTimeout.Set( cb );

	TInt iapId = iContext.IAPIdL( iIAPName );
	iSipConnection = CSIPConnection::NewL( iContext.SIP(), iapId, *this );
	// SIPConnection doesn't call the callback function if there already
	// is a connection (e.g. from ProfileServer)
	if( iSipConnection->State() == CSIPConnection::EInit )
		{
		iActiveWait.Start();
		}
	}

//
// -- CSIPConnection notifiers ------------------------------------------------
//

// Note that adding an object by-reference to the registry does NOT transfer
// ownership. Adding by-pointer does transfer ownership, however.
// Most/all SIP objects should be in the registry at this stage anyway,
// just make sure that's the case.

void CTcSIPConnectionContainer::IncomingRequest(
										CSIPServerTransaction* aTransaction )
	{
	TRAPD( err, {
		iContext.Registry().AddObjectL( aTransaction );
		TTcSIPReceived received;
		received.Set( aTransaction );
		received.SetEvent( TTcSIPReceived::EIncomingRequest );
		QueueReceivedL( received );
		} )
	__ASSERT_ALWAYS( !err, Panic( KSIPErrOOMInNotifier ) );
	}

void CTcSIPConnectionContainer::IncomingRequest(
										CSIPServerTransaction* aTransaction,
										CSIPDialog& aDialog )
	{
	TRAPD( err, {
		iContext.Registry().AddObjectL( aTransaction );
		iContext.Registry().AddObjectL( aDialog );
		TTcSIPReceived received;
		received.Set( aTransaction );
		received.Set( &aDialog );
		received.SetEvent( TTcSIPReceived::EIncomingRequest );
		QueueReceivedL( received );
		} )
	__ASSERT_ALWAYS( !err, Panic( KSIPErrOOMInNotifier ) );
	}

void CTcSIPConnectionContainer::IncomingResponse(
										CSIPClientTransaction& aTransaction )
	{
	TRAPD( err, {
		iContext.Registry().AddObjectL( aTransaction );
		TTcSIPReceived received;
		received.Set( &aTransaction );
		received.SetEvent( TTcSIPReceived::EIncomingResponse );
		QueueReceivedL( received );
		} )
	__ASSERT_ALWAYS( !err, Panic( KSIPErrOOMInNotifier ) );
	}

void CTcSIPConnectionContainer::IncomingResponse(
										CSIPClientTransaction& aTransaction,
										CSIPDialogAssocBase& aDialogAssoc )
	{
	TRAPD( err, {
		iContext.Registry().AddObjectL( aTransaction );
		iContext.Registry().AddObjectL( aDialogAssoc );
		TTcSIPReceived received;
		received.Set( &aTransaction );
		received.Set( &aDialogAssoc );
		received.SetEvent( TTcSIPReceived::EIncomingResponse );
		QueueReceivedL( received );
		} )
	__ASSERT_ALWAYS( !err, Panic( KSIPErrOOMInNotifier ) );
	}

void CTcSIPConnectionContainer::IncomingResponse(
										CSIPClientTransaction& aTransaction,
										CSIPInviteDialogAssoc* aDialogAssoc )
	{
	TRAPD( err, {
		iContext.Registry().AddObjectL( aTransaction );
		iContext.Registry().AddObjectL( aDialogAssoc );
		TTcSIPReceived received;
		received.Set( &aTransaction );
		received.Set( aDialogAssoc );
		received.SetEvent( TTcSIPReceived::EIncomingResponse );
		QueueReceivedL( received );
		} )
	__ASSERT_ALWAYS( !err, Panic( KSIPErrOOMInNotifier ) );
	}

void CTcSIPConnectionContainer::IncomingResponse(
										CSIPClientTransaction& aTransaction,
										CSIPRegistrationBinding& aRegistration )
	{
	TRAPD( err, {
		iContext.Registry().AddObjectL( aTransaction );
		iContext.Registry().AddObjectL( aRegistration );
		TTcSIPReceived received;
		received.Set( &aTransaction );
		received.Set( &aRegistration );
		received.SetEvent( TTcSIPReceived::EIncomingResponse );
		QueueReceivedL( received );
		} )
	__ASSERT_ALWAYS( !err, Panic( KSIPErrOOMInNotifier ) );
	}

void CTcSIPConnectionContainer::ErrorOccured(
										TInt aError,
										CSIPTransactionBase& aTransaction )
	{
	TRAPD( err, {
		iContext.Registry().AddObjectL( aTransaction );
		TTcSIPReceived received;
		received.Set( &aTransaction );
		received.SetError( aError );
		received.SetEvent( TTcSIPReceived::EErrorOccured );
		QueueReceivedL( received );
		} )
	__ASSERT_ALWAYS( !err, Panic( KSIPErrOOMInNotifier ) );
	}

void CTcSIPConnectionContainer::ErrorOccured( TInt aError,
										CSIPClientTransaction& aTransaction,
										CSIPRegistrationBinding& aRegistration )
	{
	TRAPD( err, {
		iContext.Registry().AddObjectL( aTransaction );
		iContext.Registry().AddObjectL( aRegistration );
		TTcSIPReceived received;
		received.Set( &aTransaction );
		received.Set( &aRegistration );
		received.SetError( aError );
		received.SetEvent( TTcSIPReceived::EErrorOccured );
		QueueReceivedL( received );
		} )
	__ASSERT_ALWAYS( !err, Panic( KSIPErrOOMInNotifier ) );
	}

void CTcSIPConnectionContainer::ErrorOccured( TInt aError,
										CSIPTransactionBase& aTransaction,
										CSIPDialogAssocBase& aDialogAssoc )
	{
	TRAPD( err, {
		iContext.Registry().AddObjectL( aTransaction );
		iContext.Registry().AddObjectL( aDialogAssoc );
		TTcSIPReceived received;
		received.Set( &aTransaction );
		received.Set( &aDialogAssoc );
		received.SetError( aError );
		received.SetEvent( TTcSIPReceived::EErrorOccured );
		QueueReceivedL( received );
		} )
	__ASSERT_ALWAYS( !err, Panic( KSIPErrOOMInNotifier ) );
	}

void CTcSIPConnectionContainer::ErrorOccured(
										TInt aError,
										CSIPRefresh& aSIPRefresh )
	{
	TRAPD( err, {
		iContext.Registry().AddObjectL( aSIPRefresh );
		TTcSIPReceived received;
		received.Set( &aSIPRefresh );
		received.SetError( aError );
		received.SetEvent( TTcSIPReceived::EErrorOccured );
		QueueReceivedL( received );
		} )
	__ASSERT_ALWAYS( !err, Panic( KSIPErrOOMInNotifier ) );
	}

void CTcSIPConnectionContainer::ErrorOccured(
										TInt aError,
										CSIPRegistrationBinding& aRegistration )
	{
	TRAPD( err, {
		iContext.Registry().AddObjectL( aRegistration );
		TTcSIPReceived received;
		received.Set( &aRegistration );
		received.SetError( aError );
		received.SetEvent( TTcSIPReceived::EErrorOccured );
		QueueReceivedL( received );
		} )
	__ASSERT_ALWAYS( !err, Panic( KSIPErrOOMInNotifier ) );
	}

void CTcSIPConnectionContainer::ErrorOccured(
										TInt aError,
										CSIPDialogAssocBase& aDialogAssoc )

	{
	TRAPD( err, {
		iContext.Registry().AddObjectL( aDialogAssoc );
		TTcSIPReceived received;
		received.Set( &aDialogAssoc );
		received.SetError( aError );
		received.SetEvent( TTcSIPReceived::EErrorOccured );
		QueueReceivedL( received );
		} )
	__ASSERT_ALWAYS( !err, Panic( KSIPErrOOMInNotifier ) );
	}

void CTcSIPConnectionContainer::InviteCompleted(
										CSIPClientTransaction& aTransaction )
	{
	TRAPD( err, {
		iContext.Registry().AddObjectL( aTransaction );
		TTcSIPReceived received;
		received.Set( &aTransaction );
		received.SetEvent( TTcSIPReceived::EInviteCompleted );
		QueueReceivedL( received );
		} )
	__ASSERT_ALWAYS( !err, Panic( KSIPErrOOMInNotifier ) );
	}

void CTcSIPConnectionContainer::InviteCanceled( 
										CSIPServerTransaction& aTransaction )
	{
	TRAPD( err, {
		iContext.Registry().AddObjectL( &aTransaction );
		TTcSIPReceived received;
		received.Set( &aTransaction );
		received.SetError( KSIPErrInviteCanceled );
		received.SetEvent( TTcSIPReceived::EInviteCanceled );
		QueueReceivedL( received );
		} )
	__ASSERT_ALWAYS( !err, Panic( KSIPErrOOMInNotifier ) );
	}

void CTcSIPConnectionContainer::ConnectionStateChanged(
										CSIPConnection::TState aState )
	{
	iConnectionState = aState;
	if( iActiveWait.IsStarted() )
		{
		iActiveWait.AsyncStop();
		}
	}

//
// -- Public functions --------------------------------------------------------
//

TTcSIPReceived CTcSIPConnectionContainer::ReceivedItemL( TInt aTimeout )
	{
	if( iReceiveQueue.Count() == 0 )
		{
		// wait for items to arrive
		iTimer->Queue( aTimeout * KSecondAsMicros, iReceiveTimeout );
		iActiveWait.Start();
		}

	// Is the queue still empty ? (i.e. timeout occurred)
	if( iReceiveQueue.Count() == 0 )
		{
		User::Leave( KErrTimedOut );
		}

	// Get the first (oldest) item from the array
	TTcSIPReceived item = iReceiveQueue[ 0 ];

	// ..and remove it from the array
	iReceiveQueue.Delete( 0 );
	iReceiveQueue.Compress();
	return item;
	}

//
// -- Internal utility functions ----------------------------------------------
//

void CTcSIPConnectionContainer::QueueReceivedL( TTcSIPReceived& aItem )
	{
	iReceiveQueue.AppendL( aItem );
	if( iActiveWait.IsStarted() )
		{
		iTimer->Remove( iReceiveTimeout );
		iActiveWait.AsyncStop();
		}
	}

TInt CTcSIPConnectionContainer::ReceiveTimeout( TAny* aSelf )
	{
	CTcSIPConnectionContainer& self =
				*reinterpret_cast< CTcSIPConnectionContainer* >( aSelf );
	if( self.iActiveWait.IsStarted() )
		{
		self.iActiveWait.AsyncStop();
		}

	return KErrNone;
	}
