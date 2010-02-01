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
#include "sipreferdialogassoc.h"
#include "sipdialog.h"
#include "sipclienttransaction.h"
#include "sipmessageelements.h"
#include "siptoheader.h"
#include "sipfromheader.h"
#include "sipcontactheader.h"
#include "siprefertoheader.h"
#include "sipstrings.h"
#include "sipstrconsts.h"
#include "cmcetls.h"

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
CSIPReferDialogAssoc::NewL(CSIPDialog& aDialog, CSIPReferToHeader* aReferTo)
    {
    CSIPReferDialogAssoc* self = CSIPReferDialogAssoc::NewLC(aDialog, aReferTo);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CSIPReferDialogAssoc::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPReferDialogAssoc*
CSIPReferDialogAssoc::NewLC(CSIPDialog& /*aDialog*/, CSIPReferToHeader* aReferTo)
    {
    __ASSERT_ALWAYS(aReferTo, User::Leave(KErrArgument));

    CSIPReferDialogAssoc* self = new (ELeave) CSIPReferDialogAssoc();
    CleanupStack::PushL(self);
    CSIPDialog* dlg = CSIPDialog::NewL();
    CleanupStack::PushL( dlg );
    self->ConstructL( SIPStrings::StringF( SipStrConsts::ERefer ), dlg );
    CleanupStack::Pop( dlg );
    self->iReferTo = aReferTo;
    return self;
    }

// -----------------------------------------------------------------------------
// CSIPReferDialogAssoc::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPReferDialogAssoc*
CSIPReferDialogAssoc::NewL(CSIPConnection& aConnection,
		                   CSIPFromHeader* aFrom,
						   CUri8* aRemoteUri,
						   CSIPReferToHeader* aReferTo,
		                   CSIPToHeader* aTo,
		                   CSIPContactHeader* aContact)
    {    
    CSIPReferDialogAssoc* self = CSIPReferDialogAssoc::NewLC(aConnection,
                                                             aFrom,
                                                             aRemoteUri,
                                                             aReferTo,
                                                             aTo,                                                             
                                                             aContact);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CSIPReferDialogAssoc::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPReferDialogAssoc*
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

// -----------------------------------------------------------------------------
// CSIPReferDialogAssoc::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPReferDialogAssoc*
CSIPReferDialogAssoc::NewL(CSIPConnection& aConnection,                                    
		                   CUri8* aRemoteUri,
		                   const MSIPRegistrationContext& aContext,
		                   CSIPReferToHeader* aReferTo,
		                   CSIPFromHeader* aFrom,
			               CSIPToHeader* aTo,
			               CSIPContactHeader* aContact)
    {
    CSIPReferDialogAssoc* self = CSIPReferDialogAssoc::NewLC(aConnection,
    														 aRemoteUri,                                                             
                                                             aContext,
                                                             aReferTo,                                                             
                                                             aFrom,
                                                             aTo,                                                             
                                                             aContact);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CSIPReferDialogAssoc::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPReferDialogAssoc*
CSIPReferDialogAssoc::NewLC(CSIPConnection& aConnection,                                    
		                    CUri8* aRemoteUri,
		                    const MSIPRegistrationContext& aContext,
		                    CSIPReferToHeader* aReferTo,
		                    CSIPFromHeader* aFrom,
			                CSIPToHeader* aTo,
			                CSIPContactHeader* aContact)
    {    
    return CSIPReferDialogAssoc::NewLC(aConnection,
									   aRemoteUri,
									   aReferTo,
									   aFrom,
                                	   aTo,
			                    	   aContact,
			                    	   &aContext);
    }
 
// -----------------------------------------------------------------------------
// CSIPReferDialogAssoc::NewLC
// -----------------------------------------------------------------------------
//
CSIPReferDialogAssoc*
CSIPReferDialogAssoc::NewLC(CSIPConnection& aConnection,
								CUri8* aRemoteUri,
								CSIPReferToHeader* aReferTo,
								CSIPFromHeader* aFrom,
                                CSIPToHeader* aTo,
			                    CSIPContactHeader* aContact,
			                    const MSIPRegistrationContext* aContext)
    {    
    
    __ASSERT_ALWAYS(aRemoteUri && aReferTo, User::Leave(KErrArgument));	
    
    CSIPReferDialogAssoc* self = new( ELeave ) CSIPReferDialogAssoc();
    CleanupStack::PushL( self );
    CSIPDialog* dlg = CSIPDialog::NewL( *aRemoteUri, *aFrom, *aTo );
    CleanupStack::PushL( dlg );
    self->ConstructL( SIPStrings::StringF( SipStrConsts::ERefer ), dlg );
    CleanupStack::Pop( dlg );
    self->iConnection = &aConnection;
    self->iRemoteUri = aRemoteUri;
    self->iFrom = aFrom;
    self->iTo = aTo;
    self->iContact = aContact;
    self->iContext = aContext;
    self->iReferTo = aReferTo;

    return self;
    }

// -----------------------------------------------------------------------------
// CSIPReferDialogAssoc::CSIPReferDialogAssoc
// -----------------------------------------------------------------------------
//
CSIPReferDialogAssoc::CSIPReferDialogAssoc()    
    {
    }

// -----------------------------------------------------------------------------
// CSIPReferDialogAssoc::~CSIPReferDialogAssoc
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPReferDialogAssoc::~CSIPReferDialogAssoc()
    {
    delete iReferTo;
    delete iRemoteUri;
    delete iFrom;
    delete iTo;
    delete iContact;
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

    CSIPClientTransaction* trx = CSIPClientTransaction::NewL( SIPStrings::StringF( SipStrConsts::ERefer ) );
    
    if ( CMCETls::Storage() )
        {
        CMCETls::Storage()->SIPSends( SIPStrings::StringF( SipStrConsts::ERefer ),
                                      aElements );
        }
    else
        {
        delete aElements;
        }
    return trx;
    
    }

