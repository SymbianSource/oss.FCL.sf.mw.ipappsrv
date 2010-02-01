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
* Description:  
*
*/




#include "sip.h"
#include "siperr.h"
#include "sipnotifydialogassoc.h"
#include "sipservertransaction.h"
#include "siprequestelements.h"
#include "sipmessageelements.h"
#include "sipeventheader.h"
#include "sipsubscriptionstateheader.h"
#include "sipstrings.h"
#include "sipstrconsts.h"
#include "sipclienttransaction.h"
#include "sipdialog.h"
#include "cmcetls.h"

#ifdef CPPUNIT_TEST

#include "TestCleanupStack.h"

#undef EXPORT_C
#define EXPORT_C

#endif


// -----------------------------------------------------------------------------
// CSIPNotifyDialogAssoc::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPNotifyDialogAssoc*
CSIPNotifyDialogAssoc::NewL(CSIPServerTransaction& aTransaction,
			     			CSIPEventHeader* aEvent,
				 			CSIPSubscriptionStateHeader* aState)
    {
    CSIPNotifyDialogAssoc* self =
    	CSIPNotifyDialogAssoc::NewLC(aTransaction, aEvent, aState);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CSIPNotifyDialogAssoc::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPNotifyDialogAssoc*
CSIPNotifyDialogAssoc::NewLC(CSIPServerTransaction& aTransaction,
			      			 CSIPEventHeader* aEvent,
				  			 CSIPSubscriptionStateHeader* aState)
    {    
    __ASSERT_ALWAYS(
    	aTransaction.Type() == SIPStrings::StringF(SipStrConsts::ESubscribe) ||
    	aTransaction.Type() == SIPStrings::StringF(SipStrConsts::ERefer),
        User::Leave(KErrArgument));

    __ASSERT_ALWAYS(aEvent && aState, User::Leave(KErrArgument));		

    CSIPNotifyDialogAssoc* self = new (ELeave) CSIPNotifyDialogAssoc();
    CleanupStack::PushL(self);
    const CSIPRequestElements* req = aTransaction.RequestElements();
    CSIPDialog* dlg = CSIPDialog::NewL( req->RemoteUri(), *req->FromHeader(), *req->ToHeader() );
    CleanupStack::PushL( dlg );
    self->ConstructL( SIPStrings::StringF( SipStrConsts::ENotify ), dlg, aTransaction );
    CleanupStack::Pop( dlg );
    
	self->iEvent = aEvent;
	self->iSubscriptionState = aState;

    return self;
    }

// -----------------------------------------------------------------------------
// CSIPNotifyDialogAssoc::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPNotifyDialogAssoc*
CSIPNotifyDialogAssoc::NewL(CSIPServerTransaction& aTransaction,
                            const MSIPRegistrationContext& /*aContext*/,
			     			CSIPEventHeader* aEvent,
				 			CSIPSubscriptionStateHeader* aState)
    {
    return CSIPNotifyDialogAssoc::NewL(aTransaction,aEvent,aState);
    }

// -----------------------------------------------------------------------------
// CSIPNotifyDialogAssoc::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPNotifyDialogAssoc*
CSIPNotifyDialogAssoc::NewLC(CSIPServerTransaction& aTransaction,
                             const MSIPRegistrationContext& /*aContext*/,
			      			 CSIPEventHeader* aEvent,
				  			 CSIPSubscriptionStateHeader* aState)
    {    
    return CSIPNotifyDialogAssoc::NewLC(aTransaction,aEvent,aState);
    }

// -----------------------------------------------------------------------------
// CSIPNotifyDialogAssoc::CSIPNotifyDialogAssoc
// -----------------------------------------------------------------------------
//
CSIPNotifyDialogAssoc::CSIPNotifyDialogAssoc()
    {
    }

// -----------------------------------------------------------------------------
// CSIPNotifyDialogAssoc::~CSIPNotifyDialogAssoc
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPNotifyDialogAssoc::~CSIPNotifyDialogAssoc()
    {
    delete iEvent;
    delete iSubscriptionState;
    }

// -----------------------------------------------------------------------------
// CSIPNotifyDialogAssoc::Event
// -----------------------------------------------------------------------------
//
EXPORT_C const CSIPEventHeader& CSIPNotifyDialogAssoc::Event() const
	{
	return *iEvent;
	}

// -----------------------------------------------------------------------------
// CSIPNotifyDialogAssoc::SubscriptionState
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPSubscriptionStateHeader& CSIPNotifyDialogAssoc::SubscriptionState()
	{
	return *iSubscriptionState;
	}

// -----------------------------------------------------------------------------
// CSIPNotifyDialogAssoc::SubscriptionState
// -----------------------------------------------------------------------------
//
EXPORT_C const CSIPSubscriptionStateHeader&
CSIPNotifyDialogAssoc::SubscriptionState() const
	{
	return *iSubscriptionState;
	}

// -----------------------------------------------------------------------------
// CSIPNotifyDialogAssoc::SendNotifyL
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPClientTransaction*
CSIPNotifyDialogAssoc::SendNotifyL(CSIPMessageElements* aElements)
    {
    
    CSIPClientTransaction* trx = CSIPClientTransaction::NewL( SIPStrings::StringF( SipStrConsts::ENotify ) );
    
    if ( CMCETls::Storage() )
        {
        CMCETls::Storage()->SIPSends( SIPStrings::StringF( SipStrConsts::ENotify ),
                                      aElements );
        }
    else
        {
        delete aElements;
        }
        
    return trx;
    
    }
