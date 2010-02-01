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
* Description:  Implementation of the SIP client simulator class
*
*/

#include "sipclientsimulator.h"
#include "sipclientsimulatorconsts.h"
#include <sip.h>
#include <sipconnection.h>
#include <sipservertransaction.h>
#include <sipresponseelements.h>
#include <sipmessageelements.h>
#include <sipinvitedialogassoc.h>
#include <SipMessageElements.h>
#include <sipstrings.h>
#include <SipStrConsts.h>
#include <sipextensionheader.h>

// ----------------------------------------------------------------------------
// CSipClientSimulator::NewL
// ----------------------------------------------------------------------------
//
CSipClientSimulator* CSipClientSimulator::NewL( const TUid& aClientUid )
	{
	CSipClientSimulator* self = 
	    new( ELeave )CSipClientSimulator();
	CleanupStack::PushL( self );
	self->ConstructL( aClientUid );
	CleanupStack::Pop( self );
	return self;
	}

// ----------------------------------------------------------------------------
// CSipClientSimulator::CSipClientSimulator
// ----------------------------------------------------------------------------
//
CSipClientSimulator::CSipClientSimulator()
	{
	}

// ----------------------------------------------------------------------------
// CSipClientSimulator::ConstructL
// ----------------------------------------------------------------------------
//
void CSipClientSimulator::ConstructL( const TUid& aClientUid )
	{
	iSip = CSIP::NewL( aClientUid, *this );
	}

// ----------------------------------------------------------------------------
// CSipClientSimulator::~CSipClientSimulator
// ----------------------------------------------------------------------------
//
CSipClientSimulator::~CSipClientSimulator()
	{
	delete iSipConnection;
	delete iSip;
	}

// ----------------------------------------------------------------------------
// CSipClientSimulator::IncomingRequest
// From MSIPObserver
// ----------------------------------------------------------------------------
//
void CSipClientSimulator::IncomingRequest( 
    TUint32 aIapId,
    CSIPServerTransaction* aTransaction )
	{
    TRAP_IGNORE(SendResponseL(aIapId,*aTransaction))
    delete aTransaction;
	}

// ----------------------------------------------------------------------------
// CSipClientSimulator::TimedOut
// From MSIPObserver
// ----------------------------------------------------------------------------
//
void CSipClientSimulator::TimedOut( 
    CSIPServerTransaction& /*aSIPServerTransaction*/ )
	{
	}

// ----------------------------------------------------------------------------
// CSipClientSimulator::IncomingRequest
// From MSIPConnectionObserver
// ----------------------------------------------------------------------------
//
void CSipClientSimulator::IncomingRequest(
    CSIPServerTransaction* aTransaction )
	{
    TRAP_IGNORE(SendResponseL(0,*aTransaction))
    delete aTransaction;
	}

// ----------------------------------------------------------------------------
// CSipClientSimulator::IncomingRequest
// From MSIPConnectionObserver
// ----------------------------------------------------------------------------
//
void CSipClientSimulator::IncomingRequest(
    CSIPServerTransaction* aTransaction,
    CSIPDialog& /*aDialog*/ )
	{
    TRAP_IGNORE(SendResponseL(0,*aTransaction))
    delete aTransaction;
	}

// ----------------------------------------------------------------------------
// CSipClientSimulator::IncomingResponse
// From MSIPConnectionObserver
// ----------------------------------------------------------------------------
//
void CSipClientSimulator::IncomingResponse(
    CSIPClientTransaction& /*aTransaction*/ )
	{
	}

// ----------------------------------------------------------------------------
// CSipClientSimulator::IncomingResponse
// From MSIPConnectionObserver
// ----------------------------------------------------------------------------
//
void CSipClientSimulator::IncomingResponse(
    CSIPClientTransaction& /*aTransaction*/,
    CSIPDialogAssocBase& /*aDialogAssoc*/ )
	{
	}

// ----------------------------------------------------------------------------
// CSipClientSimulator::IncomingResponse
// From MSIPConnectionObserver
// ----------------------------------------------------------------------------
//
void CSipClientSimulator::IncomingResponse(
    CSIPClientTransaction& /*aTransaction*/,
    CSIPInviteDialogAssoc* aDialogAssoc )
	{
	delete aDialogAssoc;
	}

// ----------------------------------------------------------------------------
// CSipClientSimulator::IncomingResponse
// From MSIPConnectionObserver
// ----------------------------------------------------------------------------
//
void CSipClientSimulator::IncomingResponse(
    CSIPClientTransaction& /*aTransaction*/,
    CSIPRegistrationBinding& /*aRegistration*/ )
	{
	}

// ----------------------------------------------------------------------------
// CSipClientSimulator::ErrorOccured
// From MSIPConnectionObserver
// ----------------------------------------------------------------------------
//
void CSipClientSimulator::ErrorOccured(
    TInt /*aError*/,
    CSIPTransactionBase& /*aTransaction*/ )
	{
	}

// ----------------------------------------------------------------------------
// CSipClientSimulator::ErrorOccured
// From MSIPConnectionObserver
// ----------------------------------------------------------------------------
//
void CSipClientSimulator::ErrorOccured( 
    TInt /*aError*/,
    CSIPClientTransaction& /*aTransaction*/,
    CSIPRegistrationBinding& /*aRegistration*/ )
	{
	}

// ----------------------------------------------------------------------------
// CSipClientSimulator::ErrorOccured
// From MSIPConnectionObserver
// ----------------------------------------------------------------------------
//
void CSipClientSimulator::ErrorOccured( 
    TInt /*aError*/,
    CSIPTransactionBase& /*aTransaction*/,
    CSIPDialogAssocBase& /*aDialogAssoc*/ )
	{
	}

// ----------------------------------------------------------------------------
// CSipClientSimulator::ErrorOccured
// From MSIPConnectionObserver
// ----------------------------------------------------------------------------
//
void CSipClientSimulator::ErrorOccured(
    TInt /*aError*/,
    CSIPRefresh& /*aSIPRefresh*/ )
	{
	}

// ----------------------------------------------------------------------------
// CSipClientSimulator::ErrorOccured
// From MSIPConnectionObserver
// ----------------------------------------------------------------------------
//
void CSipClientSimulator::ErrorOccured(
    TInt /*aError*/,
    CSIPRegistrationBinding& /*aRegistration*/ )
	{
	}

// ----------------------------------------------------------------------------
// CSipClientSimulator::ErrorOccured
// From MSIPConnectionObserver
// ----------------------------------------------------------------------------
//
void CSipClientSimulator::ErrorOccured(
    TInt /*aError*/,
    CSIPDialogAssocBase& /*aDialogAssoc*/)
	{
	}

// ----------------------------------------------------------------------------
// CSipClientSimulator::InviteCompleted
// From MSIPConnectionObserver
// ----------------------------------------------------------------------------
//
void CSipClientSimulator::InviteCompleted(
    CSIPClientTransaction& /*aTransaction*/ )
	{
	}

// ----------------------------------------------------------------------------
// CSipClientSimulator::InviteCanceled
// From MSIPConnectionObserver
// ----------------------------------------------------------------------------
//
void CSipClientSimulator::InviteCanceled( 
    CSIPServerTransaction& /*aTransaction*/ )
	{
	}

// ----------------------------------------------------------------------------
// CSipClientSimulator::ConnectionStateChanged
// From MSIPConnectionObserver
// ----------------------------------------------------------------------------
//
void CSipClientSimulator::ConnectionStateChanged(
    CSIPConnection::TState /*aState*/ )
	{
	}

// ----------------------------------------------------------------------------
// CSipClientSimulator::SendResponseL
// ----------------------------------------------------------------------------
//
void CSipClientSimulator::SendResponseL( 
    TUint32 aIapId,
    CSIPServerTransaction& aTransaction )
    {
    if ( !iSipConnection )
        {
        iSipConnection = CSIPConnection::NewL( *iSip, aIapId, *this );
        }

    if ( aIapId != 0 && iSipConnection->IapId() != aIapId )
        {
        CSIPConnection* connection = 
            CSIPConnection::NewL( *iSip, aIapId, *this );
        delete iSipConnection;
        iSipConnection = connection;
        }
        
    CSIPResponseElements* response = 
        CSIPResponseElements::NewLC(
            200, SIPStrings::StringF( SipStrConsts::EPhraseOk ) );
    
    RPointerArray< CSIPHeaderBase > headers;
    CSIPHeaderBase::PushLC( &headers );
    CSIPExtensionHeader* extensionHeader = 
        CSIPExtensionHeader::NewLC(
            KGeneratedSipHeaderName, KGeneratedSipHeaderValue );
    headers.AppendL( extensionHeader);
    CleanupStack::Pop( extensionHeader );
    response->MessageElements().SetUserHeadersL( headers );  
    CleanupStack::Pop( 1 ); // headers
           
    aTransaction.SendResponseL( response );
           
    CleanupStack::Pop( response );
    }
