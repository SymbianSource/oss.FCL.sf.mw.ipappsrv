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
#include "CreatePoCSessionDialog.h"

// CONSTANTS
const TInt KBufLength = 300;


// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CCreateSessionDialog::CCreateSessionDialog( CRtpEngine::TSessionType* )
// .
// ----------------------------------------------------------------------------
//
CCreatePoCSessionDialog::CCreatePoCSessionDialog( TSessionParams* aParams ) :
    iParams( aParams )
    {
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CCreateSessionDialog::PreLayoutDynInitL()
// .
// ----------------------------------------------------------------------------
//
void CCreatePoCSessionDialog::PreLayoutDynInitL()
    {
    
   CEikEdwin* edwinPoCAddress = static_cast<CEikEdwin*>(
                               Control( ESessionPoCDialogPoCAddress ) );
      
    TBuf<KBufLength> defaultAddress;
    defaultAddress.Copy(iParams->iRecipentAddress);
    edwinPoCAddress->SetTextL(&defaultAddress);
    edwinPoCAddress->SetCursorPosL(defaultAddress.Length(),EFalse);
 //   edwinPoCAddress->SetInputCapabilitiesL(TCoeInputCapabilities::EAllText);
 
 	CAknPopupFieldText* popupFieldText = 
        static_cast <CAknPopupFieldText*> (Control(EPoCSessionDialogSessionType));
        
 //	iParams->originator.Copy(KNone);
    CEikEdwin* OriginaterAddress = static_cast<CEikEdwin*>(
                               Control( ESessionPoCOriginaterAddress ) );
    defaultAddress.Copy(iParams->originator);
    OriginaterAddress->SetTextL(&defaultAddress);
    OriginaterAddress->SetCursorPosL(defaultAddress.Length(),EFalse);
    
    
 
    CAknForm::PreLayoutDynInitL();
    }

// ----------------------------------------------------------------------------
// CCreateSessionDialog::OkToExitL( TInt )
// .
// ----------------------------------------------------------------------------
//

TBool CCreatePoCSessionDialog::OkToExitL( TInt aKey )
    {
    if ( aKey == EEikCmdCanceled )
        {
        // Cancel pressed. Just exit.
        return ETrue;
        }
    CEikEdwin* edwinPoCAddress = static_cast<CEikEdwin*>(
                               Control( ESessionPoCDialogPoCAddress ) );
    TBuf<KBufLength> buffer;  
    edwinPoCAddress->GetText(buffer);
    iParams->iRecipentAddress  = buffer;  
    
    CAknPopupFieldText* popupFieldText = 
        static_cast <CAknPopupFieldText*> (Control(EPoCSessionDialogSessionType));  
    if(popupFieldText->CurrentValueIndex() == 0) 
    	{
    	iParams->iPoCAdhocSessionType = ETrue;
    	}
    else
    	{
    	iParams->iPoCAdhocSessionType = EFalse;
    	}	
    
    CEikEdwin* OriginaterAddress = static_cast<CEikEdwin*>(
                                 Control( ESessionPoCOriginaterAddress ) );
    OriginaterAddress->GetText(buffer);
    iParams->originator = buffer;                           

     return ETrue;
    }

// ----------------------------------------------------------------------------
// CCreatePoCSessionDialog::~CCreatePoCSessionDialog()
// .
// ----------------------------------------------------------------------------
//
CCreatePoCSessionDialog::~CCreatePoCSessionDialog()
    {
    return;
    } 

// End of File
