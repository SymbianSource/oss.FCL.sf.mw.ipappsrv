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
#include "createsessionEstablishValuesdialog.h"
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
CSessionEstablishValuesDialog::CSessionEstablishValuesDialog( TSessionEstablishValues* aParams ) :
    iParams( aParams )
    {
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CSessionEstablishValuesDialog::PreLayoutDynInitL()
// .
// ----------------------------------------------------------------------------
//
void CSessionEstablishValuesDialog::PreLayoutDynInitL()
    {
    
    CEikNumberEditor* timeOut = static_cast<CEikNumberEditor*>(
                                  Control( ESessionTimeOut ) );
    TInt num = 36000;
    timeOut->SetNumber(num); 
    
    CEikNumberEditor* serviceType = static_cast<CEikNumberEditor*>(
                                  Control( ESessionServiceType ) );
    serviceType->SetNumber(0); 
    
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
// CCreateSessionDialog::OkToExitL( TInt )
// .
// ----------------------------------------------------------------------------
//

TBool CSessionEstablishValuesDialog::OkToExitL( TInt aKey )
    {
    if ( aKey == EEikCmdCanceled )
        {
        // Cancel pressed. Just exit.
        return ETrue;
        }
   CEikNumberEditor* timeOut = static_cast<CEikNumberEditor*>(
                                  Control( ESessionTimeOut ) );
   iParams->timeOut = timeOut->Number();
   
   CEikNumberEditor* serviceType = static_cast<CEikNumberEditor*>(
                                  Control( ESessionServiceType ) );
   iParams->iServiceType = serviceType->Number();
    
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
// CSessionEstablishValuesDialog::~CCreatePoCSessionDialog()
// .
// ----------------------------------------------------------------------------
//
CSessionEstablishValuesDialog::~CSessionEstablishValuesDialog()
    {
    return;
    } 
// ----------------------------------------------------------------------------
// CSessionResponseValuesDialog::CSessionResponseValuesDialog( CRtpEngine::TSessionType* )
// .
// ----------------------------------------------------------------------------
//
CSessionResponseValuesDialog::CSessionResponseValuesDialog( TSessionResponseValues* aParams ) :
    iParams( aParams )
    {
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CSessionResponseValuesDialog::PreLayoutDynInitL()
// .
// ----------------------------------------------------------------------------
//
void CSessionResponseValuesDialog::PreLayoutDynInitL()
    {
      
    CAknForm::PreLayoutDynInitL();
    CEikNumberEditor* responsecode = static_cast<CEikNumberEditor*>(
                                  Control( ESessionResponseCode ) );
    TInt num = 182;
    responsecode->SetNumber(num); 
     TBuf<KBufLength> defaultValue;
  
    iParams->reason.Copy(KDefaultAcceptContactValue);
 	CEikEdwin* responsereason = static_cast<CEikEdwin*>(
                               Control( ESessionResponseReason ) );
    defaultValue.Copy(iParams->reason);
    responsereason->SetTextL(&defaultValue);
    responsereason->SetCursorPosL(defaultValue.Length(),EFalse);
  
    }

// ----------------------------------------------------------------------------
// CSessionResponseValuesDialog::OkToExitL( TInt )
// .
// ----------------------------------------------------------------------------
//
TBool CSessionResponseValuesDialog::OkToExitL( TInt aKey )
    {
    if ( aKey == EEikCmdCanceled )
        {
        // Cancel pressed. Just exit.
        return ETrue;
        }
   CEikNumberEditor* responsecode = static_cast<CEikNumberEditor*>(
                                  Control( ESessionResponseCode ) );
    iParams->code = responsecode->Number();
    
   TBuf<KBufLength> buffer; 
    
    CEikEdwin* responsereason = static_cast<CEikEdwin*>(
                               Control( ESessionResponseReason ) );
    responsereason->GetText(buffer);
    iParams->reason = buffer;                             

    return ETrue;
    }

// ----------------------------------------------------------------------------
// CSessionResponseValuesDialog::~CSessionResponseValuesDialog()
// .
// ----------------------------------------------------------------------------
//
CSessionResponseValuesDialog::~CSessionResponseValuesDialog()
    {
    return;
    } 

// End of File
