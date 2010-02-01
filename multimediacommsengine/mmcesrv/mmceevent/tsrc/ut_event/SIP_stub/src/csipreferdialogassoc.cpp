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
* Description:  
*
*/




#include <uri8.h>
#include "sip.h"
#include "siperr.h"
#include "sipreferdialogassoc.h"
//#include "SipDialogAssocImplementation.h"
//#include "SipDialogImplementation.h"
//#include "SipConnectionImplementation.h"
#include "sipservertransaction.h"
//#include "sipclientconnection.h"
#include "sipregistrationcontext.h"
#include "sipmessageelements.h"
#include "siptoheader.h"
#include "sipfromheader.h"
#include "sipcontactheader.h"
#include "siprefertoheader.h"
//#include "sipassert.h"
#include "sipstrings.h"
#include "sipstrconsts.h"
#include "sipclienttransaction.h"
//#include "MessageHeaderCleanup.h"

#ifdef CPPUNIT_TEST

#include "TestCleanupStack.h"

#undef EXPORT_C
#define EXPORT_C

#endif


// -----------------------------------------------------------------------------
// CSIPReferDialogAssoc::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPReferDialogAssoc*
CSIPReferDialogAssoc::NewL()
    {
    CSIPReferDialogAssoc* self = new (ELeave) CSIPReferDialogAssoc();
    CleanupStack::PushL(self);
	self->CSIPDialogAssocBase::ConstructL();
    CleanupStack::Pop(self);
    self->iReferTo = NULL;
    return self;
    }

// -----------------------------------------------------------------------------
// CSIPReferDialogAssoc::NewLC
// -----------------------------------------------------------------------------
//
/*EXPORT_C CSIPReferDialogAssoc*
CSIPReferDialogAssoc::NewLC(CSIPDialog& aDialog, CSIPReferToHeader* aReferTo)
    {
    __ASSERT_ALWAYS(aReferTo, User::Leave(KErrArgument));

    CSIPReferDialogAssoc* self = new (ELeave) CSIPReferDialogAssoc();
    CleanupStack::PushL(self);
    self->CSIPDialogAssocBase::ConstructL(
    							SIPStrings::StringF(SipStrConsts::ERefer),
    							aDialog);
    self->iReferTo = aReferTo;
    return self;
    }
*/
// -----------------------------------------------------------------------------
// CSIPReferDialogAssoc::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPReferDialogAssoc*
CSIPReferDialogAssoc::NewL(CSIPConnection& /*aConnection*/,
		                   CSIPFromHeader* aFrom,
						   CUri8* aRemoteUri,
						   CSIPReferToHeader* aReferTo,
		                   CSIPToHeader* aTo,
		                   CSIPContactHeader* aContact)
    {    
    CSIPReferDialogAssoc* self = new (ELeave) CSIPReferDialogAssoc();
    CleanupStack::PushL(self);
	self->CSIPDialogAssocBase::ConstructL();
    CleanupStack::Pop(self);
    self->iReferTo = aReferTo;
    delete aRemoteUri;
	delete aFrom;
	delete aTo,
	delete aContact;
    return self;
    }

// -----------------------------------------------------------------------------
// CSIPReferDialogAssoc::NewLC
// -----------------------------------------------------------------------------
//
/*EXPORT_C CSIPReferDialogAssoc*
CSIPReferDialogAssoc::NewLC(CSIPConnection& aConnection,
		                    CSIPFromHeader* aFrom,
						    CUri8* aRemoteUri,
						    CSIPReferToHeader* aReferTo,
		                    CSIPToHeader* aTo,
		                    CSIPContactHeader* aContact)
    {
    __ASSERT_ALWAYS(aFrom, User::Leave(KErrArgument));
    return CSIPReferDialogAssoc::NewLC(aConnection,
									   aRemoteUri,
									   aReferTo,
									   aFrom,
                                	   aTo,
			                    	   aContact,
			                    	   NULL);
    }
*/
// -----------------------------------------------------------------------------
// CSIPReferDialogAssoc::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPReferDialogAssoc*
CSIPReferDialogAssoc::NewL(CSIPConnection& /*aConnection*/,                                    
		                   CUri8* aRemoteUri,
		                   const MSIPRegistrationContext& /*aContext*/,
		                   CSIPReferToHeader* aReferTo,
		                   CSIPFromHeader* aFrom,
			               CSIPToHeader* aTo,
			               CSIPContactHeader* aContact)
    {
    CSIPReferDialogAssoc* self = new (ELeave) CSIPReferDialogAssoc();
    CleanupStack::PushL(self);
	self->CSIPDialogAssocBase::ConstructL();
    CleanupStack::Pop(self);
    self->iReferTo = aReferTo;
    delete aRemoteUri;
	delete aFrom;
	delete aTo,
	delete aContact;
    return self;
    }

// -----------------------------------------------------------------------------
// CSIPReferDialogAssoc::NewLC
// -----------------------------------------------------------------------------
//
/*EXPORT_C CSIPReferDialogAssoc*
CSIPReferDialogAssoc::NewLC(CSIPConnection& aConnection,                                    
		                    CUri8* aRemoteUri,
		                    const MSIPRegistrationContext& aContext,
		                    CSIPReferToHeader* aReferTo,
		                    CSIPFromHeader* aFrom,
			                CSIPToHeader* aTo,
			                CSIPContactHeader* aContact)
    {    
    __ASSERT_ALWAYS(aContext.IsContextActive(),
                    User::Leave(KErrSIPInvalidRegistrationState));
	return CSIPReferDialogAssoc::NewLC(aConnection,
									   aRemoteUri,
									   aReferTo,
									   aFrom,
                                	   aTo,
			                    	   aContact,
			                    	   &aContext);
    }*/
 
// -----------------------------------------------------------------------------
// CSIPReferDialogAssoc::NewLC
// -----------------------------------------------------------------------------
//
/*CSIPReferDialogAssoc*
CSIPReferDialogAssoc::NewLC(CSIPConnection& aConnection,
								CUri8* aRemoteUri,
								CSIPReferToHeader* aReferTo,
								CSIPFromHeader* aFrom,
                                CSIPToHeader* aTo,
			                    CSIPContactHeader* aContact,
			                    const MSIPRegistrationContext* aContext)
    {    
    __ASSERT_ALWAYS(aRemoteUri && aReferTo, User::Leave(KErrArgument));	

	CSIPDialogImplementation* dialogImpl =
			CSIPDialogImplementation::NewLC(aConnection.Implementation(),
											aContext);
	CSIPReferDialogAssoc* self =
    	CSIPReferDialogAssoc::NewL(dialogImpl->Dialog(), aReferTo);
    //Don't take ownership of aReferTo yet, as leave can occur
    self->iReferTo = NULL;

    CleanupStack::Pop(dialogImpl);
    CleanupStack::PushL(self);

	dialogImpl->SetHeadersL(aFrom, aTo, aRemoteUri, aContact);
    self->iReferTo = aReferTo;

    return self;
    }
*/
// -----------------------------------------------------------------------------
// CSIPReferDialogAssoc::CSIPReferDialogAssoc
// -----------------------------------------------------------------------------
//
CSIPReferDialogAssoc::CSIPReferDialogAssoc()    
    {
    iType = SIPStrings::StringF( SipStrConsts::ERefer );
    }

// -----------------------------------------------------------------------------
// CSIPReferDialogAssoc::~CSIPReferDialogAssoc
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPReferDialogAssoc::~CSIPReferDialogAssoc()
    {
    if ( iReferTo )
        delete iReferTo;
    }

// -----------------------------------------------------------------------------
// CSIPReferDialogAssoc::ReferTo
// -----------------------------------------------------------------------------
//
EXPORT_C const CSIPReferToHeader& CSIPReferDialogAssoc::ReferTo() const
	{
	return *iReferTo;
	}

// -----------------------------------------------------------------------------
// CSIPReferDialogAssoc::SendReferL
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPClientTransaction*
CSIPReferDialogAssoc::SendReferL(CSIPMessageElements* aElements)
    {     
    CSIPClientTransaction* clientTransaction = CSIPClientTransaction::NewL();
    delete aElements;
    return clientTransaction;
    }

// -----------------------------------------------------------------------------
// CSIPReferDialogAssoc::DoSendReferL
// -----------------------------------------------------------------------------
//
CSIPClientTransaction*
CSIPReferDialogAssoc::DoSendReferL(CSIPMessageElements* aElements,
								   TBool /*aWithinDialog*/)
    {
    CSIPClientTransaction* clientTransaction = CSIPClientTransaction::NewL();
    delete aElements;
    return clientTransaction;
    }
