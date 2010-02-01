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
#include "sipnotifydialogassoc.h"
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
#include <sipsubscriptionstateheader.h>
_LIT8( KSubscrStateActive, "active" );

EXPORT_C CSIPNotifyDialogAssoc::~CSIPNotifyDialogAssoc()
	{
	if( iEvent )
	    delete iEvent;
	if (iSubscriptionState )
	delete iSubscriptionState;
	}


CSIPNotifyDialogAssoc::CSIPNotifyDialogAssoc()
	{
	iType = SIPStrings::StringF( SipStrConsts::ENotify );
	}


EXPORT_C CSIPNotifyDialogAssoc* CSIPNotifyDialogAssoc::NewL()
	{
	CSIPNotifyDialogAssoc* self = new(ELeave)CSIPNotifyDialogAssoc;
	CleanupStack::PushL(self);
	self->CSIPDialogAssocBase::ConstructL();
	self->iEvent=NULL;
	self->iSubscriptionState=CSIPSubscriptionStateHeader::DecodeL( KSubscrStateActive );
	CleanupStack::Pop(self);
	return self;
	}
	
	
EXPORT_C CSIPClientTransaction* CSIPNotifyDialogAssoc::SendNotifyL(
    CSIPMessageElements* aElements)
    {
    CSIPClientTransaction* clientTransaction = CSIPClientTransaction::NewL();
    delete aElements;
    return clientTransaction;
    }
    	
EXPORT_C const CSIPEventHeader& CSIPNotifyDialogAssoc::Event() const
    {
    return *iEvent;
    } 

EXPORT_C CSIPSubscriptionStateHeader& CSIPNotifyDialogAssoc::SubscriptionState()
	{
	return *iSubscriptionState;	
	}