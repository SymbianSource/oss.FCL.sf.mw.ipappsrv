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
#include "notifyDialog.h"
#include <AknPopupFieldText.h>

// CONSTANTS
const TInt KBufLength = 100;


// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CSessionEstablishValuesDialog::CSessionEstablishValuesDialog( CRtpEngine::TSessionType* )
// .
// ----------------------------------------------------------------------------
//
CNotifyDialog::CNotifyDialog( TSessionEstablishValues* aParams ) :
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
    
    CEikNumberEditor* timeOut = static_cast<CEikNumberEditor*>(
                                  Control( ESessionTimeOut ) );
    TInt num = 36000;
    timeOut->SetNumber(num); 
     TBuf<KBufLength> defaultValue;
  
    iParams->headers.Copy(KDefaultAcceptContactValue);
 	CEikEdwin* headers = static_cast<CEikEdwin*>(
                               Control( ESessionHeaders ) );
    defaultValue.Copy(iParams->headers);
    headers->SetTextL(&defaultValue);
    headers->SetCursorPosL(defaultValue.Length(),EFalse);
    
      
    /*
    iParams->contentType.Copy(KContentType);
 	CEikEdwin* contentType = static_cast<CEikEdwin*>(
                               Control( EEstablishContentType ) );
    defaultValue.Copy(iParams->contentType);
    contentType->SetTextL(&defaultValue);
    contentType->SetCursorPosL(defaultValue.Length(),EFalse);
    
    iParams->content.Copy(KNone);
 	CEikEdwin* content = static_cast<CEikEdwin*>(
                               Control( EEstablishContent ) );
    defaultValue.Copy(iParams->content);
    content->SetTextL(&defaultValue);
    content->SetCursorPosL(defaultValue.Length(),EFalse);
    
    iParams->contentHeaders.Copy(KNone);
 	CEikEdwin* contentHeaders = static_cast<CEikEdwin*>(
                               Control( EEstablishContentHeaders ) );
    defaultValue.Copy(iParams->contentHeaders);
    contentHeaders->SetTextL(&defaultValue);
    contentHeaders->SetCursorPosL(defaultValue.Length(),EFalse);
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
   CEikNumberEditor* timeOut = static_cast<CEikNumberEditor*>(
                                  Control( ESessionTimeOut ) );
    iParams->timeOut = timeOut->Number();
    
   TBuf<KBufLength> buffer; 
    
    CEikEdwin* headers = static_cast<CEikEdwin*>(
                               Control( ESessionHeaders ) );
    headers->GetText(buffer);
    iParams->headers = buffer;                             
 
   
 
 /*
	CEikEdwin* contentType = static_cast<CEikEdwin*>(
                               Control( EEstablishContentType ) );
    contentType->GetText(buffer);
    iParams->contentType = buffer;   
    
    CEikEdwin* content = static_cast<CEikEdwin*>(
                               Control( EEstablishContent ) );
    content->GetText(buffer);
    iParams->content = buffer;   
    
    CEikEdwin* contentHeaders = static_cast<CEikEdwin*>(
                               Control( EEstablishContentHeaders ) );
    contentHeaders->GetText(buffer);
    iParams->contentHeaders = buffer;   
    
    */
        
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
