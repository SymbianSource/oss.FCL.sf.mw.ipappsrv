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
#include "CreateVoIPSessionDialog.h"
#include "mcetestuiengineconstants.h"

// CONSTANTS
const TInt KBufLength = 300;


// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CCreateVoIPSessionDialog::CCreateVoIPSessionDialog( CRtpEngine::TSessionType* )
// .
// ----------------------------------------------------------------------------
//
CCreateVoIPSessionDialog::CCreateVoIPSessionDialog( TSessionParams* aParams ) :
    iParams( aParams )
    {
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CCreateVoIPSessionDialog::PreLayoutDynInitL()
// .
// ----------------------------------------------------------------------------
//
void CCreateVoIPSessionDialog::PreLayoutDynInitL()
    {
    
   CEikEdwin* edwinVoIPAddress = static_cast<CEikEdwin*>(
                               Control( ESessionVoIPDialogVoIPAddress ) );
      
    TBuf<KBufLength> defaultAddress;
    defaultAddress.Copy(iParams->iRecipentAddress);
    edwinVoIPAddress->SetTextL(&defaultAddress);
    edwinVoIPAddress->SetCursorPosL(defaultAddress.Length(),EFalse);
    
    iParams->originator.Copy(KNone);
    CEikEdwin* OriginaterAddress = static_cast<CEikEdwin*>(
                               Control( ESessionVoIPOriginaterAddress ) );
    defaultAddress.Copy(iParams->originator);
    OriginaterAddress->SetTextL(&defaultAddress);
    CAknForm::PreLayoutDynInitL();
    }

// ----------------------------------------------------------------------------
// CCreateVoIPSessionDialog::OkToExitL( TInt )
// .
// ----------------------------------------------------------------------------
//

TBool CCreateVoIPSessionDialog::OkToExitL( TInt aKey )
    {
    if ( aKey == EEikCmdCanceled )
        {
        // Cancel pressed. Just exit.
        return ETrue;
        }
    CEikEdwin* edwinVoIPAddress = static_cast<CEikEdwin*>(
                               Control( ESessionVoIPDialogVoIPAddress ) );
    TBuf<KBufLength> buffer;  
    edwinVoIPAddress->GetText(buffer);
    iParams->iRecipentAddress  = buffer;  
    
    CEikEdwin* OriginaterAddress = static_cast<CEikEdwin*>(
                                 Control( ESessionVoIPOriginaterAddress ) );
    OriginaterAddress->GetText(buffer);
    iParams->originator = buffer;
    
    return ETrue;
    }

// ----------------------------------------------------------------------------
// CCreateVoIPSessionDialog::~CCreateVoIPSessionDialog()
// .
// ----------------------------------------------------------------------------
//
CCreateVoIPSessionDialog::~CCreateVoIPSessionDialog()
    {
    return;
    } 

// End of File
