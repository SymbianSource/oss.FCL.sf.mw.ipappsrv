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
#include "createReferDialog.h"
#include "mcetestuiengineconstants.h"
#include "cmcetestuiengineoutrefer.h"
#include "cmcetestuiengineoutevent.h"

// CONSTANTS
const TInt KBufLength = 100;

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CCreateReferDialog::CCreateReferDialog( CRtpEngine::TSessionType* )
// .
// ----------------------------------------------------------------------------
//
CCreateReferDialog::CCreateReferDialog( TReferParams* aParams ) :
    iParams( aParams )
    {
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CCreateReferDialog::PreLayoutDynInitL()
// .
// ----------------------------------------------------------------------------
//
void CCreateReferDialog::PreLayoutDynInitL()
    {
  	CAknForm::PreLayoutDynInitL();  
    CEikEdwin* edwinRecipentAddress = static_cast<CEikEdwin*>(
                               Control( EReferRecipentAddress ) );
      
    TBuf<KBufLength> defaultAddress;
    defaultAddress.Copy(iParams->iRecipentAddress);
    edwinRecipentAddress->SetTextL(&defaultAddress);	
    edwinRecipentAddress->SetCursorPosL(defaultAddress.Length(),EFalse);
    
    
    CEikEdwin* edwinReferTo = static_cast<CEikEdwin*>(
                               Control( EReferTo ) );
    defaultAddress.Copy(iParams->iReferTo);
    edwinReferTo->SetTextL(&defaultAddress);
    edwinReferTo->SetCursorPosL(defaultAddress.Length(),EFalse);
    
    iParams->originator.Copy(KNone);
    CEikEdwin* OriginaterAddress = static_cast<CEikEdwin*>(
                               Control( EReferOriginaterAddress ) );
    defaultAddress.Copy(iParams->originator);
    OriginaterAddress->SetTextL(&defaultAddress);
    OriginaterAddress->SetCursorPosL(defaultAddress.Length(),EFalse);
    }

// ----------------------------------------------------------------------------
// CCreateReferDialog::OkToExitL( TInt )
// .
// ----------------------------------------------------------------------------
//

TBool CCreateReferDialog::OkToExitL( TInt aKey )
    {
    if ( aKey == EEikCmdCanceled )
        {
        // Cancel pressed. Just exit.
        return ETrue;
        }
    CEikEdwin* edwinRecipentAddress = static_cast<CEikEdwin*>(
                               Control( EReferRecipentAddress ) );
    TBuf<KBufLength> buffer;  
    edwinRecipentAddress->GetText(buffer);
    iParams->iRecipentAddress = buffer;                             

    CEikEdwin* edwinReferTo = static_cast<CEikEdwin*>(
                               Control( EReferTo ) );
    edwinReferTo->GetText(buffer);
    iParams->iReferTo = buffer;
    
    CEikEdwin* OriginaterAddress = static_cast<CEikEdwin*>(
                                 Control( EReferOriginaterAddress ) );
    OriginaterAddress->GetText(buffer);
    iParams->originator = buffer;
       
    return ETrue;
    }

// ----------------------------------------------------------------------------
// CCreateReferDialog::~CCreateReferDialog()
// .
// ----------------------------------------------------------------------------
//
CCreateReferDialog::~CCreateReferDialog()
    {
    return;
    } 

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CCreateReferToDialog::CCreateReferToDialog( CRtpEngine::TSessionType* )
// .
// ----------------------------------------------------------------------------
//
CCreateReferToDialog::CCreateReferToDialog( TReferParams* aParams ) :
    iParams( aParams )
    {
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CCreateReferToDialog::PreLayoutDynInitL()
// .
// ----------------------------------------------------------------------------
//
void CCreateReferToDialog::PreLayoutDynInitL()
    {
  	CAknForm::PreLayoutDynInitL();  
    TBuf<KBufLength> defaultAddress;
    CEikEdwin* edwinReferTo = static_cast<CEikEdwin*>(
                               Control( ESendReferTo ) );
    defaultAddress.Copy(iParams->iReferTo);
    edwinReferTo->SetTextL(&defaultAddress);
    edwinReferTo->SetCursorPosL(defaultAddress.Length(),EFalse);
    }

// ----------------------------------------------------------------------------
// CCreateReferToDialog::OkToExitL( TInt )
// .
// ----------------------------------------------------------------------------
//

TBool CCreateReferToDialog::OkToExitL( TInt aKey )
    {
    if ( aKey == EEikCmdCanceled )
        {
        // Cancel pressed. Just exit.
        return ETrue;
        }
   
    TBuf<KBufLength> buffer;  
    CEikEdwin* edwinReferTo = static_cast<CEikEdwin*>(
                               Control( ESendReferTo ) );
    edwinReferTo->GetText(buffer);
    iParams->iReferTo = buffer;
    
    return ETrue;
    }

// ----------------------------------------------------------------------------
// CCreateReferToDialog::~CCreateReferToDialog()
// .
// ----------------------------------------------------------------------------
//
CCreateReferToDialog::~CCreateReferToDialog()
    {
    return;
    } 

// End of File
