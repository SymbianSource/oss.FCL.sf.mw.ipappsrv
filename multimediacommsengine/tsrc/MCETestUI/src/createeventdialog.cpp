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
#include "createEventDialog.h"

#include "mcetestuiengineconstants.h"
#include "cmcetestuiengineoutrefer.h"
#include "cmcetestuiengineoutevent.h"

// CONSTANTS
const TInt KBufLength = 100;

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CCreateEventDialog::CCreateEventDialog( CRtpEngine::TSessionType* )
// .
// ----------------------------------------------------------------------------
//
CCreateEventDialog::CCreateEventDialog( TEventParams* aParams ) :
    iParams( aParams )
    {
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CCreateEventDialog::PreLayoutDynInitL()
// .
// ----------------------------------------------------------------------------
//
void CCreateEventDialog::PreLayoutDynInitL()
    {
    
   CEikEdwin* edwinRecipentAddress = static_cast<CEikEdwin*>(
                               Control( EEventRecipentAddress ) );
      
    TBuf<KBufLength> defaultAddress;
    defaultAddress.Copy(iParams->iRecipentAddress);
    edwinRecipentAddress->SetTextL(&defaultAddress);	
    edwinRecipentAddress->SetCursorPosL(defaultAddress.Length(),EFalse);
    
    iParams->iEventHeader.Copy(KEventHeader);
    CEikEdwin* edwinEventHeader = static_cast<CEikEdwin*>(
                               Control( EEventHeader ) );
    defaultAddress.Copy(iParams->iEventHeader);
    edwinEventHeader->SetTextL(&defaultAddress);
    edwinEventHeader->SetCursorPosL(defaultAddress.Length(),EFalse);
    
    iParams->originator.Copy(KNone);
    CEikEdwin* OriginaterAddress = static_cast<CEikEdwin*>(
                               Control( EEventOriginaterAddress ) );
    defaultAddress.Copy(iParams->originator);
    OriginaterAddress->SetTextL(&defaultAddress);
    OriginaterAddress->SetCursorPosL(defaultAddress.Length(),EFalse);
 
 	CEikNumberEditor* refreshInterval = static_cast<CEikNumberEditor*>(
                                  Control( EEventRefreshInterval ) );
    refreshInterval->SetNumber(3600);  
    CAknForm::PreLayoutDynInitL();
    }

// ----------------------------------------------------------------------------
// CCreateEventDialog::OkToExitL( TInt )
// .
// ----------------------------------------------------------------------------
//

TBool CCreateEventDialog::OkToExitL( TInt aKey )
    {
    if ( aKey == EEikCmdCanceled )
        {
        // Cancel pressed. Just exit.
        return ETrue;
        }
    CEikEdwin* edwinRecipentAddress = static_cast<CEikEdwin*>(
                               Control( EEventRecipentAddress ) );
    TBuf<KBufLength> buffer;  
    edwinRecipentAddress->GetText(buffer);
    iParams->iRecipentAddress = buffer;                             

    CEikEdwin* edwinEventHeader = static_cast<CEikEdwin*>(
                               Control( EEventHeader ) );
    edwinEventHeader->GetText(buffer);
    iParams->iEventHeader = buffer;
    
    CEikEdwin* OriginaterAddress = static_cast<CEikEdwin*>(
                                 Control( EEventOriginaterAddress ) );
    OriginaterAddress->GetText(buffer);
    iParams->originator = buffer;
    
    
 	CEikNumberEditor* refreshInterval = static_cast<CEikNumberEditor*>(
                                  Control( EEventRefreshInterval ) );
    iParams->refreshInterval = refreshInterval->Number();
    return ETrue;
    }

// ----------------------------------------------------------------------------
// CCreateEventDialog::~CCreateEventDialog()
// .
// ----------------------------------------------------------------------------
//
CCreateEventDialog::~CCreateEventDialog()
    {
    return;
    } 

// End of File
