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





// INCLUDES
#include "sipclienttransaction.h"
#include "sipdialogassocbase.h"
#include "sipstrings.h"
#include "sipstrconsts.h"

#include "cmcetls.h"

#include "sipdialog.h"


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

	
CSIPDialogAssocBase::~CSIPDialogAssocBase()
    {
    if ( iDialog && iDialog->Detach() == 0 )
        {
        delete iDialog;
        }
    }
		
CSIPDialogAssocBase::CSIPDialogAssocBase()
    {
    }

void CSIPDialogAssocBase::ConstructL(RStringF aType, CSIPDialog* aDialog )
    {
    iType = aType;
    iDialog = aDialog;
    iDialog->Attach();
    }
    
    
EXPORT_C CSIPClientTransaction* 
			CSIPDialogAssocBase::SendNonTargetRefreshRequestL(RStringF /*aMethod*/,
                		                         CSIPMessageElements* aElements)
	{
   	CSIPClientTransaction* trx = 
	   	CSIPClientTransaction::NewL( SIPStrings::StringF( SipStrConsts::EInfo ) );
    
    if ( CMCETls::Storage() )
        {
        CMCETls::Storage()->SIPSends( SIPStrings::StringF( SipStrConsts::EInfo ),
                                      aElements );
        }
    else
        {
        delete aElements;
        }
        
    return trx;
	}

    

void CSIPDialogAssocBase::ConstructL(RStringF aType,
				CSIPDialog* aDialog,
				CSIPServerTransaction& aTransaction)
    {
    iType = aType;
    iDialog = aDialog;
    iDialog->Attach();
    iTransaction = &aTransaction;
    }


