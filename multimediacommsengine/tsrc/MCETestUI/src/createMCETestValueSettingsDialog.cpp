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
#include "createMceTestValueSettingsDialog.h"

// CONSTANTS

const TInt KBufLength = 100;

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CCreateMceValueSettingsDialog::CCreateMceValueSettingsDialog( )
// .
// ----------------------------------------------------------------------------
//
CCreateMceValueSettingsDialog::CCreateMceValueSettingsDialog(
							CMCETestUIEngine& aEngine):
							iEngine(aEngine) 
    
    {
    
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CCreateMceValueSettingsDialog::PreLayoutDynInitL()
// .
// ----------------------------------------------------------------------------
//
void CCreateMceValueSettingsDialog::PreLayoutDynInitL()
    {
    CAknForm::PreLayoutDynInitL();

    CEikNumberEditor* jittersize = static_cast<CEikNumberEditor*>(
                                  Control( EMceValueSettingsJitterSize ) );
    jittersize->SetNumber(iEngine.iJitterBufSize); 
    
    CEikNumberEditor* jittertreshold = static_cast<CEikNumberEditor*>(
                                  Control( EMceValueSettingsJitterTreshold ) );
    jittertreshold->SetNumber(iEngine.iJitterBufTreshold);  
    
    CEikNumberEditor* jittertimeout = static_cast<CEikNumberEditor*>(
                                  Control( EMceValueSettingsJitterTimeout ) );
    jittertimeout->SetNumber(iEngine.iJitterBufTimeout/1000);   
	
    }

// ----------------------------------------------------------------------------
// CCreateMceValueSettingsDialog::OkToExitL( TInt )
// .
// ----------------------------------------------------------------------------
//

TBool CCreateMceValueSettingsDialog::OkToExitL( TInt aKey )
    {
    if ( aKey == EEikCmdCanceled )
        {
        // Cancel pressed. Just exit.
        return ETrue;
        }
    
    CEikNumberEditor* jittersize = static_cast<CEikNumberEditor*>(
                                  Control( EMceValueSettingsJitterSize ) );
                                  
    iEngine.iJitterBufSize = jittersize->Number();
    
    CEikNumberEditor* jittertreshold = static_cast<CEikNumberEditor*>(
                                  Control( EMceValueSettingsJitterTreshold ) );
    
    iEngine.iJitterBufTreshold = jittertreshold->Number();
    
    CEikNumberEditor* jittertimeout = static_cast<CEikNumberEditor*>(
                                  Control( EMceValueSettingsJitterTimeout ) );
    
    iEngine.iJitterBufTimeout = jittertimeout->Number()*1000;
     
    return ETrue;
    }

// ----------------------------------------------------------------------------
// CCreateMceValueSettingsDialog::~CCreateMceValueSettingsDialog()
// .
// ----------------------------------------------------------------------------
//
CCreateMceValueSettingsDialog::~CCreateMceValueSettingsDialog()
    {
    return;
    } 

// End of File
