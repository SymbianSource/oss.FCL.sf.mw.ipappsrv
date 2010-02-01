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




#include <uri8.h>
#include <sipmessageelements.h>
#include "sipinvitedialogassoc.h"
#include "sipclienttransaction.h"
#include "sipdialog.h"
#include "sipstrings.h"
#include "sipstrconsts.h"

#include "sipfromheader.h"
#include "siptoheader.h"
#include "sipcontactheader.h"
#include "cmcetls.h"



EXPORT_C CSIPInviteDialogAssoc* CSIPInviteDialogAssoc::NewL( CSIPDialog& aDialog )
    {
    CSIPInviteDialogAssoc* self = NewLC( aDialog );
    CleanupStack::Pop( self );
    return self;
    
    }
    
    
EXPORT_C CSIPInviteDialogAssoc* CSIPInviteDialogAssoc::NewLC( CSIPDialog& aDialog )
    {
    CSIPInviteDialogAssoc* self = new (ELeave) CSIPInviteDialogAssoc();
    CleanupStack::PushL( self );
    self->ConstructL( SIPStrings::StringF( SipStrConsts::EInvite ), &aDialog );
    return self;
    }

EXPORT_C CSIPInviteDialogAssoc*
            CSIPInviteDialogAssoc::NewL(CSIPServerTransaction& aTransaction )
    {
    CSIPInviteDialogAssoc* self = NewLC( aTransaction );
    CleanupStack::Pop( self );
    return self;
    
    }

EXPORT_C CSIPInviteDialogAssoc*
        CSIPInviteDialogAssoc::NewLC(CSIPServerTransaction& aTransaction)
    {
    
    CSIPInviteDialogAssoc* self = new (ELeave) CSIPInviteDialogAssoc();
    CleanupStack::PushL( self );
    CSIPDialog* dlg = CSIPDialog::NewL();
    CleanupStack::PushL( dlg );
    self->ConstructL( SIPStrings::StringF( SipStrConsts::EInvite ), dlg, aTransaction );
    CleanupStack::Pop( dlg );
    return self;
    
    }

EXPORT_C CSIPInviteDialogAssoc* 
        CSIPInviteDialogAssoc::NewL(CSIPServerTransaction& aTransaction,
                                    const MSIPRegistrationContext& /*aContext*/ )
    {
    CSIPInviteDialogAssoc* self = NewLC( aTransaction );
    CleanupStack::Pop( self );
    return self;
    
    }
    

EXPORT_C CSIPInviteDialogAssoc*
    CSIPInviteDialogAssoc::NewL(CSIPConnection& aConnection,
         CSIPFromHeader* aFrom,
         CUri8* aRemoteUri,
         CSIPToHeader* aTo,
         CSIPContactHeader* aContact)
    {
    CSIPInviteDialogAssoc* self = NewLC( aConnection, aFrom, aRemoteUri, aTo, aContact );
    CleanupStack::Pop( self );
    return self;
    
    }

EXPORT_C CSIPInviteDialogAssoc*
    CSIPInviteDialogAssoc::NewLC(CSIPConnection& aConnection,
          CSIPFromHeader* aFrom,
          CUri8* aRemoteUri,
          CSIPToHeader* aTo,
          CSIPContactHeader* aContact)
    {
    return NewLC( aConnection, aRemoteUri, aFrom, aTo, aContact, NULL );
    
    }

EXPORT_C CSIPInviteDialogAssoc*
    CSIPInviteDialogAssoc::NewL(CSIPConnection& aConnection,                                    
         CUri8* aRemoteUri,
         const MSIPRegistrationContext& aContext,
         CSIPFromHeader* aFrom,
         CSIPToHeader* aTo,
         CSIPContactHeader* aContact)
    {
    
    CSIPInviteDialogAssoc* self = NewLC( aConnection, aRemoteUri, aContext, aFrom, aTo, aContact );
    CleanupStack::Pop( self );
    return self;
    
    }

EXPORT_C CSIPInviteDialogAssoc*
    CSIPInviteDialogAssoc::NewLC(CSIPConnection& aConnection,			                        
	      CUri8* aRemoteUri,
          const MSIPRegistrationContext& aContext,
          CSIPFromHeader* aFrom,
          CSIPToHeader* aTo,
          CSIPContactHeader* aContact)
    {
    return NewLC( aConnection, aRemoteUri, aFrom, aTo, aContact, &aContext );
    }

EXPORT_C CSIPInviteDialogAssoc::~CSIPInviteDialogAssoc()
    {
    delete iRemoteUri;
    delete iFrom;
    delete iTo;
    delete iContact;
    }
			                                  
EXPORT_C CSIPClientTransaction*
    CSIPInviteDialogAssoc::SendInviteL( CSIPMessageElements* aElements )
    {
    
    CSIPClientTransaction* trx = CSIPClientTransaction::NewL( SIPStrings::StringF( SipStrConsts::EInvite ) );
    
    if ( CMCETls::Storage() )
        {
        CMCETls::Storage()->SIPSends( SIPStrings::StringF( SipStrConsts::EInvite ),
                                      aElements );
        }
    else
        {
        delete aElements;
        }
        
    return trx;
      
    }

EXPORT_C CSIPClientTransaction*
    CSIPInviteDialogAssoc::SendPrackL(CSIPMessageElements* aElements )
    {
    
    CSIPClientTransaction* trx = CSIPClientTransaction::NewL( SIPStrings::StringF( SipStrConsts::EPrack ) );
    
    if ( CMCETls::Storage() )
        {
        CMCETls::Storage()->SIPSends( SIPStrings::StringF( SipStrConsts::EPrack ),
                                      aElements );
        }
    else
        {
        delete aElements;
        }
        
    return trx;
    }

EXPORT_C CSIPClientTransaction*
    CSIPInviteDialogAssoc::SendUpdateL(CSIPMessageElements* aElements )
    {
    CSIPClientTransaction* trx = CSIPClientTransaction::NewL( SIPStrings::StringF( SipStrConsts::EUpdate ) );
    
    if ( CMCETls::Storage() )
        {
        CMCETls::Storage()->SIPSends( SIPStrings::StringF( SipStrConsts::EUpdate ),
                                      aElements );
        }
    else
        {
        delete aElements;
        }
        
    return trx;
    }

EXPORT_C void 
    CSIPInviteDialogAssoc::SendAckL(const CSIPClientTransaction& /*aTransaction*/,
	                                CSIPMessageElements* aElements )
    {
    if ( CMCETls::Storage() )
        {
        CMCETls::Storage()->SIPSends( SIPStrings::StringF( SipStrConsts::EAck ),
                                      aElements );
        }
    else
        {
        delete aElements;
        }
    
    }

EXPORT_C CSIPClientTransaction*
    CSIPInviteDialogAssoc::SendByeL(CSIPMessageElements* aElements )
    {
    CSIPClientTransaction* trx = CSIPClientTransaction::NewL( SIPStrings::StringF( SipStrConsts::EBye ) );
    
    if ( CMCETls::Storage() )
        {
        CMCETls::Storage()->SIPSends( SIPStrings::StringF( SipStrConsts::EBye ),
                                      aElements );
        }
    else
        {
        delete aElements;
        }
        
    return trx;
        
    }


CSIPInviteDialogAssoc* CSIPInviteDialogAssoc::NewLC(CSIPConnection& aConnection,
            								 CUri8* aRemoteUri,
            	                             CSIPFromHeader* aFrom,
            	                             CSIPToHeader* aTo,
            		                         CSIPContactHeader* aContact,
            		                         const MSIPRegistrationContext* aContext)
    {
    CSIPInviteDialogAssoc* self = new (ELeave) CSIPInviteDialogAssoc();
    CleanupStack::PushL( self );
    CSIPDialog* dlg = CSIPDialog::NewL( *aRemoteUri, *aFrom, *aTo );
    CleanupStack::PushL( dlg );
    self->ConstructL( SIPStrings::StringF( SipStrConsts::EInvite ), dlg );
    CleanupStack::Pop( dlg );
    self->iConnection = &aConnection;
    self->iRemoteUri = aRemoteUri;
    self->iFrom = aFrom;
    self->iTo = aTo;
    self->iContact = aContact;
    self->iContext = aContext;
    return self;
    }
            		                         
   	   	
CSIPInviteDialogAssoc::CSIPInviteDialogAssoc()
    {
    
    }
	
