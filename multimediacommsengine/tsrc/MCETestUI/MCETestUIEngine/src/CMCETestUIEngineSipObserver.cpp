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


#include "CMCETestUIEngineSipObserver.h"
#include <sip.h>
#include <sipconnection.h>
#include <sipdialog.h>
#include <sipprofile.h>
#include <sipprofileregistry.h>
#include <sipservertransaction.h>
#include <sipclienttransaction.h>
#include <sipdialogassocbase.h>
#include <sipinvitedialogassoc.h>
#include <sipregistrationbinding.h>
#include <siprefresh.h>
#include <sipstrings.h>
#include <sipstrconsts.h>
#include <sipresponseelements.h>
#include <e32err.h>
#include <e32debug.h>
#include <stringpool.h>


const TUint KNotImplemented = 501;
const TUid KUIOptionsUid = { 0x10123F9E };

// -----------------------------------------------------------------------------
// CMCETestUIEngineSipObserver::NewL
// -----------------------------------------------------------------------------
//
CMCETestUIEngineSipObserver* CMCETestUIEngineSipObserver::NewL()
	{
	CMCETestUIEngineSipObserver* self = 
	    new( ELeave ) CMCETestUIEngineSipObserver();

	CleanupStack::PushL( self );
	self->ConstructL();
    CleanupStack::Pop( self );
	return self;
	}

// -----------------------------------------------------------------------------
// CMCETestUIEngineSipObserver::~CMCETestUIEngineSipObserver
// -----------------------------------------------------------------------------
//
CMCETestUIEngineSipObserver::~CMCETestUIEngineSipObserver()
	{	
	}

// -----------------------------------------------------------------------------
// CMCETestUIEngineSipObserver::CMCETestUIEngineSipObserver
// -----------------------------------------------------------------------------
//
CMCETestUIEngineSipObserver::CMCETestUIEngineSipObserver()
	{
	
	}

// -----------------------------------------------------------------------------
// CMCETestUIEngineSipObserver::ConstructL
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineSipObserver::ConstructL( )
	{
	iSip = CSIP::NewL( KUIOptionsUid, *this );
	}

// -----------------------------------------------------------------------------
// CMCETestUIEngineSipObserver::GetSipInstance
// -----------------------------------------------------------------------------
//

CSIP* CMCETestUIEngineSipObserver::GetSipInstance()
	{
	return iSip;
	}
// -- from SIPObserver ---------------------------------------------------------

void CMCETestUIEngineSipObserver::IncomingRequest(
			TUint32 aIapId,
			CSIPServerTransaction* aTransaction)
	{
	CSIPConnection* connection = NULL;
    TRAPD( err, connection = CSIPConnection::NewL( *iSip, aIapId, *this  ) );
    if ( aTransaction->Type() == SIPStrings::StringF( SipStrConsts::EOptions ) )
        {
        RDebug::Print( _L( "SIPObserver-IncomingRequest-Options:Entry" ));
        RStringF phrase = SIPStrings::StringF( SipStrConsts::EPhraseNotImplemented );
        CSIPResponseElements* response = 
            CSIPResponseElements::NewLC( KNotImplemented, phrase );
        aTransaction->SendResponseL( response );
        CleanupStack::Pop( response );
        RDebug::Print( _L( "Response to OPTIONS sent" ));
        }
    delete connection;
	}

void CMCETestUIEngineSipObserver::TimedOut(
            CSIPServerTransaction& /*aTransaction*/)
    {
   	// NOP, dummy
    }   	

//
// -- MSIPConnectionObserver dummy functions
//

void CMCETestUIEngineSipObserver::IncomingRequest (CSIPServerTransaction* /*aTransaction*/) 
	{
	// NOP
	}
void CMCETestUIEngineSipObserver::IncomingRequest (CSIPServerTransaction* /*aTransaction*/,
			          CSIPDialog& /*aDialog*/) 
	{
	// NOP
	}
void CMCETestUIEngineSipObserver::IncomingResponse (CSIPClientTransaction& /*aTransaction*/) 
	{
	// NOP
	}
void CMCETestUIEngineSipObserver::IncomingResponse (CSIPClientTransaction& /*aTransaction*/,
					   CSIPDialogAssocBase& /*aDialogAssoc*/) 
	{
	// NOP
	}
void CMCETestUIEngineSipObserver::IncomingResponse (CSIPClientTransaction& /*aTransaction*/,
					   CSIPInviteDialogAssoc* /*aDialogAssoc*/) 
	{
	// NOP
	}
void CMCETestUIEngineSipObserver::IncomingResponse (CSIPClientTransaction& /*aTransaction*/,
                       CSIPRegistrationBinding& /*aRegistration*/) 
    {
    // NOP
    }
void CMCETestUIEngineSipObserver::ErrorOccured (TInt /*aError*/,
                   CSIPTransactionBase& /*aTransaction*/) 
    {
    // NOP
    }
void CMCETestUIEngineSipObserver::ErrorOccured (TInt /*aError*/,
		           CSIPClientTransaction& /*aTransaction*/,
	               CSIPRegistrationBinding& /*aRegistration*/) 
	{
	// NOP
	}
void CMCETestUIEngineSipObserver::ErrorOccured (TInt /*aError*/,
				   CSIPTransactionBase& /*aTransaction*/,
				   CSIPDialogAssocBase& /*aDialogAssoc*/) 
	{
	// NOP
	}
void CMCETestUIEngineSipObserver::ErrorOccured (TInt /*aError*/, CSIPRefresh& /*aSIPRefresh*/) 
	{
	// NOP
	}
void CMCETestUIEngineSipObserver::ErrorOccured (TInt /*aError*/,	
										   CSIPRegistrationBinding& /*aRegistration*/) 
	{
	// NOP
	}
void CMCETestUIEngineSipObserver::ErrorOccured (TInt /*aError*/,			
					               CSIPDialogAssocBase& /*aDialogAssoc*/) 
	{
	// NOP
	}
void CMCETestUIEngineSipObserver::InviteCompleted (CSIPClientTransaction& /*aTransaction*/) 
	{
	// NOP
	}
void CMCETestUIEngineSipObserver::InviteCanceled (CSIPServerTransaction& /*aTransaction*/) 
	{
	// NOP
	}
void CMCETestUIEngineSipObserver::ConnectionStateChanged (CSIPConnection::TState /*aState*/) 
	{
	// NOP
	}
