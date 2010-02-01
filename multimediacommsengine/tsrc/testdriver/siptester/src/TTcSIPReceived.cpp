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

#include <sipinvitedialogassoc.h>
#include <sipsubscribedialogassoc.h>
#include <sipnotifydialogassoc.h>
#include <sipreferdialogassoc.h>

#include <sipstrings.h>
#include "SipStrConsts.h"
#include "TTcSIPReceived.h"

TTcSIPReceived::TTcSIPReceived()
	: iClientTransaction( NULL ),
	  iDialog( NULL ),
	  iInviteDialogAssoc( NULL ),
      iNotifyDialogAssoc( NULL ),
	  iSubscribeDialogAssoc( NULL ),
      iReferDialogAssoc( NULL ),
	  iRefresh( NULL ),
	  iRegistration( NULL ),
	  iServerTransaction( NULL ),
	  iTransactionBase( NULL ),
	  iIapId( 0 ),
	  iError( KErrNone ),
	  iEvent( TTcSIPReceived::ENone )
	{
	}

TTcSIPReceived::~TTcSIPReceived()
	{
	}

void TTcSIPReceived::Set( const CSIPClientTransaction* aTransaction )
	{
	iClientTransaction = aTransaction;
	}

void TTcSIPReceived::Set( const CSIPDialog* aDialog )
	{
	iDialog = aDialog;
	}

void TTcSIPReceived::Set( const CSIPDialogAssocBase* aDialogAssoc )
	{
	if( aDialogAssoc->Type() == SIPStrings::StringF( SipStrConsts::EInvite ) )
		{
		iInviteDialogAssoc = static_cast< const CSIPInviteDialogAssoc* >( aDialogAssoc );
		}
	if( aDialogAssoc->Type() == SIPStrings::StringF( SipStrConsts::ESubscribe ) )
		{
		iSubscribeDialogAssoc = static_cast< const CSIPSubscribeDialogAssoc* >( aDialogAssoc );
		}
	if( aDialogAssoc->Type() == SIPStrings::StringF( SipStrConsts::ERefer ) )
		{
		iReferDialogAssoc = static_cast< const CSIPReferDialogAssoc* >( aDialogAssoc );
		}
	if( aDialogAssoc->Type() == SIPStrings::StringF( SipStrConsts::ENotify ) )
		{
		iNotifyDialogAssoc = static_cast< const CSIPNotifyDialogAssoc* >( aDialogAssoc );
		}
	}

void TTcSIPReceived::Set( const CSIPInviteDialogAssoc* aDialogAssoc )
	{
	iInviteDialogAssoc = aDialogAssoc;
	}

void TTcSIPReceived::Set( const CSIPNotifyDialogAssoc* aDialogAssoc )
	{
	iNotifyDialogAssoc = aDialogAssoc;
	}

void TTcSIPReceived::Set( const CSIPReferDialogAssoc* aDialogAssoc )
	{
	iReferDialogAssoc = aDialogAssoc;
	}

void TTcSIPReceived::Set( const CSIPRefresh* aRefresh )
	{
	iRefresh = aRefresh;
	}

void TTcSIPReceived::Set( const CSIPRegistrationBinding* aRegistration )
	{
	iRegistration = aRegistration;
	}

void TTcSIPReceived::Set( const CSIPServerTransaction* aTransaction )
	{
	iServerTransaction = aTransaction;
	}

void TTcSIPReceived::Set( const CSIPTransactionBase* aTransactionBase )
	{
	iTransactionBase = aTransactionBase;
	}

void TTcSIPReceived::Set( TInt aIapId )
	{
	iIapId = aIapId;
	}

void TTcSIPReceived::SetError( TInt aError )
	{
	iError = aError;
	}
	
void TTcSIPReceived::SetEvent( TTcSIPEvent aEvent )
    {
    iEvent = aEvent;
    }

