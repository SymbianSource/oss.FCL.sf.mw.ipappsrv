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
#include "sipregistrationbinding.h"
#include "sipconnection.h"
#include "sipmessageelements.h"
#include "siprefresh.h"
#include "siprouteheader.h"
#include "sipclienttransaction.h"
#include "siptoheader.h"
#include "sipfromheader.h"
#include "sipcontactheader.h"
#include "sipexpiresheader.h"
#include "sipaddress.h"
#include "uricontainer.h"
#include "sipuri.h"

EXPORT_C CSIPRegistrationBinding::~CSIPRegistrationBinding()
	{
	delete iE;
	delete iP;
	delete iContact;
	delete iRefresh;
	}


CSIPRegistrationBinding::CSIPRegistrationBinding()
	{
	}


void CSIPRegistrationBinding::ConstructL(CSIPConnection& /*aConnection*/,
        			                     CSIPToHeader* aAor,
        			                     CSIPContactHeader* aContact,                        
        			                     CSIPRefresh* aRefresh,
        			                     CSIPRouteHeader* aOutboundProxy,
        			                     CUri8* aRemoteUri,
        			                     CSIPFromHeader* aFrom)
	{
	TUriParser8 parser;
    User::LeaveIfError(parser.Parse(_L8("sip:host1")));  
    CUri8* uri8 = CUri8::NewL(parser);
    CleanupStack::PushL(uri8);
	CSIPAddress* addr = CSIPAddress::NewL(uri8);
	CleanupStack::Pop(1); // uri8
	CleanupStack::PushL(addr);
	iContact = CSIPContactHeader::NewL(addr);
	CleanupStack::Pop(addr);//addr
	HBufC8* tmp = HBufC8::NewL(100); // simulate leaving in low memory condition
	delete tmp;
	iLeave = EFalse;
	iP = aOutboundProxy;
	delete aAor;
	delete aContact;
	iRefresh = aRefresh;
	delete aRemoteUri;
	delete aFrom;
	}


EXPORT_C CSIPRegistrationBinding* CSIPRegistrationBinding::NewL(CSIPConnection& aConnection,
								CSIPToHeader* aAor,
								CSIPContactHeader* aContact,
								CSIPRefresh* aRefresh,
								CSIPRouteHeader* aOutboundProxy,
								CUri8* aRemoteUri,
                                CSIPFromHeader* aFrom)
	{
	CSIPRegistrationBinding* self = new(ELeave)CSIPRegistrationBinding;
	CleanupStack::PushL(self);
	self->ConstructL(aConnection,aAor,aContact,aRefresh,aOutboundProxy,aRemoteUri,aFrom);
	CleanupStack::Pop(self);
	return self;
	}


EXPORT_C const CSIPRouteHeader* CSIPRegistrationBinding::OutboundProxy() const
	{
	return iP;
	}


EXPORT_C CSIPClientTransaction* CSIPRegistrationBinding::RegisterL(CSIPMessageElements* aSIPMessageElements)
	{	
	if (iLeave)
		User::Leave(-12345);

	CSIPClientTransaction* tx = CSIPClientTransaction::NewL();

	if (iE)
		delete iE;
	iE = aSIPMessageElements;

	return tx;
	}


EXPORT_C CSIPClientTransaction* CSIPRegistrationBinding::DeregisterL(CSIPMessageElements* aSIPMessageElements)
	{
	if (iLeave)
		User::Leave(-1234);

	CSIPClientTransaction* tx = CSIPClientTransaction::NewL();

	if (iE)
		delete iE;
	iE = aSIPMessageElements;

	return tx;
	}

EXPORT_C CSIPClientTransaction* CSIPRegistrationBinding::UpdateL(TUint /*aExpirationValue*/, 
									CSIPMessageElements* aElements)
	{	
	
	CSIPClientTransaction* tx = CSIPClientTransaction::NewL();
	delete aElements;
	aElements = NULL;

	return tx;
	}

EXPORT_C CSIPContactHeader& CSIPRegistrationBinding::ContactHeader()
	{
   
	return *iContact;
	}


EXPORT_C TBool CSIPRegistrationBinding::IsContextActive() const
	{
	return iContext;
	}


EXPORT_C TUint32 CSIPRegistrationBinding::ContextId() const
	{
	return 22;
	}


EXPORT_C void CSIPRegistrationBinding::SetContext(TBool aValue)
	{
	iContext = aValue;
	}


EXPORT_C void CSIPRegistrationBinding::SetOutboundProxyL(CSIPRouteHeader* aOutboundProxy)
	{
	delete iP;
	iP = aOutboundProxy;
	}


EXPORT_C void CSIPRegistrationBinding::SetLeave(TBool aValue)
	{
	iLeave = aValue;
	}


EXPORT_C CSIPRefresh* CSIPRegistrationBinding::SIPRefresh()
	{
	return iRefresh;
	}


EXPORT_C TBool CSIPRegistrationBinding::operator==(const CSIPRegistrationBinding& aRegistration) const
	{
	return (this == &aRegistration);
	}