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



#include "sipdialogassocbase.h"
#include "sipclienttransaction.h"
#include "sipdialog.h"

CSIPDialogAssocBase::CSIPDialogAssocBase()
	{
	}


void CSIPDialogAssocBase::ConstructL()
    {
    iDialog = CSIPDialog::NewL();
    }


CSIPDialogAssocBase::~CSIPDialogAssocBase()
	{
	delete iDialog;
	}


EXPORT_C const CSIPDialog& CSIPDialogAssocBase::Dialog() const
    {
    return *iDialog;
    }


EXPORT_C CSIPDialog& CSIPDialogAssocBase::Dialog()
    {
    return *iDialog;
    }
    
EXPORT_C RStringF CSIPDialogAssocBase::Type() const
    {
    return iType;
    }


EXPORT_C CSIPClientTransaction* CSIPDialogAssocBase::SendNonTargetRefreshRequestL(
								RStringF /*aMethod*/,
								CSIPMessageElements* /*aElements*/)
	{
	return CSIPClientTransaction::NewL();
	}


CSIPRefresh* CSIPDialogAssocBase::FindRefresh(TUint32 /*aRefreshId*/)
    {
    return NULL;
    }


void CSIPDialogAssocBase::DeletingRefresh(CSIPRefresh& /*aRefresh*/,
										  TUint32 /*aRefreshId*/)
	{	
	}

	