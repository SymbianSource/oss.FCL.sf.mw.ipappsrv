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


// INCLUDES
#include "createNotifyDialog.h"
#include <AknPopupFieldText.h>

#include "mcetestuiengineconstants.h"

// CONSTANTS
const TInt KBufLength = 100;


// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CSessionEstablishValuesDialog::CSessionEstablishValuesDialog( CRtpEngine::TSessionType* )
// .
// ----------------------------------------------------------------------------
//
CNotifyDialog::CNotifyDialog( TNotifyValues* aParams ) :
    iParams( aParams )
    {
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CSessionEstablishValuesDialog::PreLayoutDynInitL()
// .
// ----------------------------------------------------------------------------
//
void CNotifyDialog::PreLayoutDynInitL()
    {
    TBuf<KBufLength> defaultValue;
    iParams->headers.Copy(KDefaultAcceptContactValue);
 	CEikEdwin* headers = static_cast<CEikEdwin*>(
                               Control( ENotifyHeaders ) );
    defaultValue.Copy(iParams->headers);
    headers->SetTextL(&defaultValue);
    headers->SetCursorPosL(defaultValue.Length(),EFalse);
    
  /*  iParams->contentType.Copy( KNotifyContentType );
 	CEikEdwin* contentType = static_cast<CEikEdwin*>(
                               Control( ENotifyContentType ) );
    defaultValue.Copy(iParams->contentType);
    contentType->SetTextL(&defaultValue);
    contentType->SetCursorPosL(defaultValue.Length(),EFalse);
    
    iParams->content.Copy( KNotifyContent );
 	CEikEdwin* content = static_cast<CEikEdwin*>(
                               Control( ENotifyContent ) );
    defaultValue.Copy(iParams->content);
    content->SetTextL(&defaultValue);
    content->SetCursorPosL(defaultValue.Length(),EFalse);
  */ 
 
   	
    CAknForm::PreLayoutDynInitL();
    }

// ----------------------------------------------------------------------------
// CNotifyDialog::OkToExitL( TInt )
// .
// ----------------------------------------------------------------------------
//

TBool CNotifyDialog::OkToExitL( TInt aKey )
    {
    if ( aKey == EEikCmdCanceled )
        {
        // Cancel pressed. Just exit.
        return ETrue;
        }
    TBuf<KBufLength> buffer; 
    
    CEikEdwin* headers = static_cast<CEikEdwin*>(
                               Control( ENotifyHeaders ) );
    headers->GetText(buffer);
    iParams->headers = buffer;                             

 /*   CEikEdwin* contentType = static_cast<CEikEdwin*>(
                               Control( ENotifyContentType ) );
    contentType->GetText(buffer);
    iParams->contentType = buffer;   
    
    CEikEdwin* content = static_cast<CEikEdwin*>(
                               Control( ENotifyContent ) );
    content->GetText(buffer);
    iParams->content = buffer;  */ 
    

        
    return ETrue;
    }

// ----------------------------------------------------------------------------
// CNotifyDialog::~CNotifyDialog()
// .
// ----------------------------------------------------------------------------
//
CNotifyDialog::~CNotifyDialog()
    {
    return;
    } 
