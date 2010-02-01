/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include "sipsubscribedialogassoc.h"
#include "sipdialog.h"
#include "sipeventheader.h"
#include "sipfromheader.h"
#include "siptoheader.h"
#include "sipcontactheader.h"
#include "sipclienttransaction.h"
#include "siprefresh.h"
#include "sipmessageelements.h"
#include "sipstrings.h"
#include "sipstrconsts.h"


EXPORT_C CSIPSubscribeDialogAssoc::~CSIPSubscribeDialogAssoc()
	{
	delete iEvent;
	delete iRefresh;
	}


CSIPSubscribeDialogAssoc::CSIPSubscribeDialogAssoc()
	{
	iType = SIPStrings::StringF( SipStrConsts::ESubscribe );
	}


EXPORT_C CSIPSubscribeDialogAssoc* CSIPSubscribeDialogAssoc::NewL(
    CSIPEventHeader* aEvent)
	{
	CSIPSubscribeDialogAssoc* self = new(ELeave)CSIPSubscribeDialogAssoc;
	CleanupStack::PushL(self);
	self->CSIPDialogAssocBase::ConstructL();
	CleanupStack::Pop(self);
	self->iEvent = aEvent;
	return self;
	}
	
	
EXPORT_C CSIPSubscribeDialogAssoc* CSIPSubscribeDialogAssoc::NewL(
    CSIPConnection& /*aConnection*/,                                    
    CUri8* aRemoteUri,
    const MSIPRegistrationContext& /*aContext*/,
    CSIPEventHeader* aEvent,
    CSIPFromHeader* aFrom,
    CSIPToHeader* aTo,
    CSIPContactHeader* aContact)
    {
	CSIPSubscribeDialogAssoc* self = new(ELeave)CSIPSubscribeDialogAssoc;
	CleanupStack::PushL(self);
	self->CSIPDialogAssocBase::ConstructL();
	CleanupStack::Pop(self);
	self->iEvent = aEvent;
	delete aRemoteUri;
	delete aFrom;
	delete aTo,
	delete aContact;
	return self;    
    }
	
	
EXPORT_C CSIPClientTransaction* CSIPSubscribeDialogAssoc::SendSubscribeL(
    CSIPMessageElements* aElements,
    CSIPRefresh* aRefresh)
    {
    CSIPClientTransaction* clientTransaction = CSIPClientTransaction::NewL();
    delete aElements;
    if ( iRefresh )
        delete iRefresh;
    iRefresh = aRefresh;
    return clientTransaction;
    }
    
            			   
EXPORT_C CSIPClientTransaction* CSIPSubscribeDialogAssoc::UpdateL(
    CSIPMessageElements* aElements)
    {
    CSIPClientTransaction* clientTransaction = CSIPClientTransaction::NewL();
    delete aElements;
    return clientTransaction;    
    }
        

EXPORT_C CSIPClientTransaction* CSIPSubscribeDialogAssoc::SendUnsubscribeL(
    CSIPMessageElements* aElements)
    {
    CSIPClientTransaction* clientTransaction = CSIPClientTransaction::NewL();
    delete aElements;
    return clientTransaction;    
    }

		
EXPORT_C const CSIPRefresh* CSIPSubscribeDialogAssoc::SIPRefresh() const
    {
    return iRefresh;
    }
    
    	
EXPORT_C const CSIPEventHeader& CSIPSubscribeDialogAssoc::Event() const
    {
    return *iEvent;
    } 