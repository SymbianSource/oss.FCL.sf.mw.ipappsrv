/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
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




#include <uri8.h>
#include "sip.h"
#include "siperr.h"
#include "sipsubscribedialogassoc.h"
#include "sipdialog.h"
#include "sipclienttransaction.h"
#include "sipregistrationcontext.h"
#include "siprefresh.h"
#include "siptoheader.h"
#include "sipfromheader.h"
#include "sipexpiresheader.h"
#include "sipcontactheader.h"
#include "sipeventheader.h"
#include "sipmessageelements.h"
#include "sipstrings.h"
#include "sipstrconsts.h"
#include "cmcetls.h"

#ifdef CPPUNIT_TEST

#include "TestCleanupStack.h"

#undef EXPORT_C
#define EXPORT_C

#endif


// -----------------------------------------------------------------------------
// CSIPSubscribeDialogAssoc::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPSubscribeDialogAssoc*
CSIPSubscribeDialogAssoc::NewL(CSIPDialog& aDialog, CSIPEventHeader* aEvent)
    {
    CSIPSubscribeDialogAssoc* self =
    	CSIPSubscribeDialogAssoc::NewLC(aDialog, aEvent);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CSIPSubscribeDialogAssoc::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPSubscribeDialogAssoc*
CSIPSubscribeDialogAssoc::NewLC(CSIPDialog& /*aDialog*/, CSIPEventHeader* aEvent)
    {
    __ASSERT_ALWAYS(aEvent, User::Leave(KErrArgument));

    CSIPSubscribeDialogAssoc* self = new (ELeave) CSIPSubscribeDialogAssoc();
    CleanupStack::PushL(self);
    CSIPDialog* dlg = CSIPDialog::NewL();
    CleanupStack::PushL( dlg );
    self->ConstructL( SIPStrings::StringF(SipStrConsts::ESubscribe), dlg );
    CleanupStack::Pop( dlg );
    self->iEvent = aEvent;
    return self;
    }

// -----------------------------------------------------------------------------
// CSIPSubscribeDialogAssoc::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPSubscribeDialogAssoc*
CSIPSubscribeDialogAssoc::NewL(CSIPConnection& aConnection,
						       CSIPFromHeader* aFrom,
						       CUri8* aRemoteUri,
						       CSIPEventHeader* aEvent,        
						       CSIPToHeader* aTo,        
						       CSIPContactHeader* aContact)
    {    
    CSIPSubscribeDialogAssoc* self =
        CSIPSubscribeDialogAssoc::NewLC(aConnection,
                                        aFrom,
                                        aRemoteUri,
                                        aEvent,
                                        aTo,                                        
                                        aContact);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CSIPSubscribeDialogAssoc::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPSubscribeDialogAssoc*
CSIPSubscribeDialogAssoc::NewLC(CSIPConnection& aConnection,
                                CSIPFromHeader* aFrom,
                                CUri8* aRemoteUri,
						        CSIPEventHeader* aEvent,
						        CSIPToHeader* aTo,        
						        CSIPContactHeader* aContact)
    {
    __ASSERT_ALWAYS(aFrom, User::Leave(KErrArgument));

	return CSIPSubscribeDialogAssoc::NewLC(aConnection,
										   aRemoteUri,
										   aEvent,
                                		   aFrom,
                                		   aTo,
			                    		   aContact,
			                    		   NULL);
    }

// -----------------------------------------------------------------------------
// CSIPSubscribeDialogAssoc::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPSubscribeDialogAssoc*
CSIPSubscribeDialogAssoc::NewL(CSIPConnection& aConnection,
							   CUri8* aRemoteUri,                               
                               const MSIPRegistrationContext& aContext,
                               CSIPEventHeader* aEvent,
                               CSIPFromHeader* aFrom,
                               CSIPToHeader* aTo,
	                           CSIPContactHeader* aContact)
    {
    CSIPSubscribeDialogAssoc* self =
        CSIPSubscribeDialogAssoc::NewLC(aConnection,
        								aRemoteUri,
                                        aContext,
                                        aEvent,
                                        aFrom,
                                        aTo,                                
                                        aContact);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CSIPSubscribeDialogAssoc::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPSubscribeDialogAssoc*
CSIPSubscribeDialogAssoc::NewLC(CSIPConnection& aConnection,
								CUri8* aRemoteUri,			                    
			                    const MSIPRegistrationContext& aContext,
			                    CSIPEventHeader* aEvent,
                                CSIPFromHeader* aFrom,
                                CSIPToHeader* aTo,
			                    CSIPContactHeader* aContact)
    {    
    __ASSERT_ALWAYS(aContext.IsContextActive(),
                    User::Leave(KErrSIPInvalidRegistrationState));

	return CSIPSubscribeDialogAssoc::NewLC(aConnection,
										   aRemoteUri,
										   aEvent,
                                		   aFrom,
                                		   aTo,
			                    		   aContact,
			                    		   &aContext);
    }

// -----------------------------------------------------------------------------
// CSIPSubscribeDialogAssoc::NewLC
// -----------------------------------------------------------------------------
//
CSIPSubscribeDialogAssoc*
CSIPSubscribeDialogAssoc::NewLC(CSIPConnection& aConnection,
								CUri8* aRemoteUri,
								CSIPEventHeader* aEvent,
                                CSIPFromHeader* aFrom,
                                CSIPToHeader* aTo,
			                    CSIPContactHeader* aContact,
			                    const MSIPRegistrationContext* aContext)
    {
    __ASSERT_ALWAYS(aRemoteUri && aEvent, User::Leave(KErrArgument));	

	CSIPSubscribeDialogAssoc* self = new( ELeave ) CSIPSubscribeDialogAssoc();
   	CleanupStack::PushL(self);
    CSIPDialog* dlg = CSIPDialog::NewL( *aRemoteUri, *aFrom, *aTo );
    CleanupStack::PushL( dlg );
    self->ConstructL( SIPStrings::StringF( SipStrConsts::ESubscribe ), dlg );
    CleanupStack::Pop( dlg );
    self->iConnection = &aConnection;
    self->iRemoteUri = aRemoteUri;
    self->iFrom = aFrom;
    self->iTo = aTo;
    self->iContact = aContact;
    self->iContext = aContext;
    self->iEvent = aEvent;
    self->iRefresh = NULL;
    return self;
    }

// -----------------------------------------------------------------------------
// CSIPSubscribeDialogAssoc::CSIPSubscribeDialogAssoc
// -----------------------------------------------------------------------------
//
CSIPSubscribeDialogAssoc::CSIPSubscribeDialogAssoc()
    {
    }

// -----------------------------------------------------------------------------
// CSIPSubscribeDialogAssoc::~CSIPSubscribeDialogAssoc
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPSubscribeDialogAssoc::~CSIPSubscribeDialogAssoc()
    {
    delete iRefresh;
    delete iEvent;
    delete iRemoteUri;
    delete iFrom;
    delete iTo;
    delete iContact;
    }

// -----------------------------------------------------------------------------
// CSIPSubscribeDialogAssoc::SendSubscribeL
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPClientTransaction*
CSIPSubscribeDialogAssoc::SendSubscribeL(CSIPMessageElements* aElements,
										 CSIPRefresh* aSIPRefresh)
    {
    
    CSIPClientTransaction* trx = CSIPClientTransaction::NewL( SIPStrings::StringF( SipStrConsts::ESubscribe ) );
    
    iRefresh = aSIPRefresh;
    if ( CMCETls::Storage() )
        {
        CMCETls::Storage()->SIPSends( SIPStrings::StringF( SipStrConsts::ESubscribe ),
                                      aElements );
        }
    else
        {
        delete aElements;
        }
    return trx;
    }

// -----------------------------------------------------------------------------
// CSIPSubscribeDialogAssoc::UpdateL
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPClientTransaction*
CSIPSubscribeDialogAssoc::UpdateL(CSIPMessageElements* aElements)
	{
	
    CSIPClientTransaction* trx = CSIPClientTransaction::NewL( SIPStrings::StringF( SipStrConsts::ESubscribe ) );
	
	if ( CMCETls::Storage() )
        {
        CMCETls::Storage()->SIPSends( SIPStrings::StringF( SipStrConsts::ESubscribe ),
                                      aElements );
        }
    else
        {
        delete aElements;
        }
    return trx;
    }

// -----------------------------------------------------------------------------
// CSIPSubscribeDialogAssoc::SendUnsubscribeL
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPClientTransaction* 
CSIPSubscribeDialogAssoc::SendUnsubscribeL(CSIPMessageElements* aElements)
    {

    CSIPClientTransaction* trx = CSIPClientTransaction::NewL( SIPStrings::StringF( SipStrConsts::ESubscribe ) );
    
    if ( CMCETls::Storage() )
        {
        CMCETls::Storage()->SIPSends( SIPStrings::StringF( SipStrConsts::ESubscribe ),
                                      aElements );
        }
    else
        {
        delete aElements;
        }
    return trx;
    }

// -----------------------------------------------------------------------------
// CSIPSubscribeDialogAssoc::Event
// -----------------------------------------------------------------------------
//
EXPORT_C const CSIPEventHeader& CSIPSubscribeDialogAssoc::Event() const
	{
	return *iEvent;
	}


// -----------------------------------------------------------------------------
// CSIPSubscribeDialogAssoc::SIPRefresh
// -----------------------------------------------------------------------------
//
EXPORT_C const CSIPRefresh* CSIPSubscribeDialogAssoc::SIPRefresh() const
    {
    return iRefresh;
    }

