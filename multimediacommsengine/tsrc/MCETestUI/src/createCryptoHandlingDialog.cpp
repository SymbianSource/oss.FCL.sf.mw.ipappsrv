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
#include "createCryptoHandlingDialog.h"
#include "CMCETestUIEngineAudioStream.h"
#include "CMCETestUIEngineSink.h"
#include <MCEAudioStream.h>
#include <MCERtpSink.h>
#include <MCEMediaSink.h>
#include <MCESpeakerSink.h>
// CONSTANTS

const TInt KBufLength = 100;

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CCurrentCryptoContextsDialog::CCurrentCryptoContextsDialog( )
// .
// ----------------------------------------------------------------------------
//
CCurrentCryptoContextsDialog::CCurrentCryptoContextsDialog( TBool aCrypto32,TBool aCrypto80)
    {
    iCrypto32 = aCrypto32;
    iCrypto80 = aCrypto80;
    }

// ----------------------------------------------------------------------------
// CCurrentCryptoContextsDialog::PreLayoutDynInitL()
// .
// ----------------------------------------------------------------------------
//
void CCurrentCryptoContextsDialog::PreLayoutDynInitL()
    {
    CAknPopupFieldText* popupFieldText = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialogSecureSessionType));
        
    if( (iCrypto32 ) &&  ( !iCrypto80))
    	{
    	popupFieldText->SetCurrentValueIndex(0);
    	}
    else if( (!iCrypto32 ) &&  (iCrypto80))
    	{
    	popupFieldText->SetCurrentValueIndex(1);
    	}
    else if( (iCrypto32 ) &&  (iCrypto80))
    	{
    	popupFieldText->SetCurrentValueIndex(2);
    	}
    }

// ----------------------------------------------------------------------------
// CStartDTMFDialog::OkToExitL( TInt )
// .
// ----------------------------------------------------------------------------
//

TBool CCurrentCryptoContextsDialog::OkToExitL( TInt aKey )
    {
    if ( aKey == EEikCmdCanceled )
        {
        // Cancel pressed. Just exit.
        return ETrue;
        }
     return ETrue;
    }

// ----------------------------------------------------------------------------
// CCurrentCryptoContextsDialog::~CCurrentCryptoContextsDialog()
// .
// ----------------------------------------------------------------------------
//
CCurrentCryptoContextsDialog::~CCurrentCryptoContextsDialog()
    {
    return;
    } 
    
// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CSupportedCryptoContextsDialog::CSupportedCryptoContextsDialog( )
// .
// ----------------------------------------------------------------------------
//
CSupportedCryptoContextsDialog::CSupportedCryptoContextsDialog( TBool aCrypto32,TBool aCrypto80)
    {
    iCrypto32 = aCrypto32;
    iCrypto80 = aCrypto80;
    }

// ----------------------------------------------------------------------------
// CSupportedCryptoContextsDialog::PreLayoutDynInitL()
// .
// ----------------------------------------------------------------------------
//
void CSupportedCryptoContextsDialog::PreLayoutDynInitL()
    {
    CAknPopupFieldText* popupFieldText = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialogSecureSessionType));
        
    if( (iCrypto32 ) &&  ( !iCrypto80))
    	{
    	popupFieldText->SetCurrentValueIndex(0);
    	}
    else if( (!iCrypto32 ) &&  (iCrypto80))
    	{
    	popupFieldText->SetCurrentValueIndex(1);
    	}
    else if( (iCrypto32 ) &&  (iCrypto80))
    	{
    	popupFieldText->SetCurrentValueIndex(2);
    	}
    }

// ----------------------------------------------------------------------------
// CSupportedCryptoContextsDialog::OkToExitL( TInt )
// .
// ----------------------------------------------------------------------------
//

TBool CSupportedCryptoContextsDialog::OkToExitL( TInt aKey )
    {
    if ( aKey == EEikCmdCanceled )
        {
        // Cancel pressed. Just exit.
        return ETrue;
        }
     return ETrue;
    }

// ----------------------------------------------------------------------------
// CSupportedCryptoContextsDialog::~CSupportedCryptoContextsDialog()
// .
// ----------------------------------------------------------------------------
//
CSupportedCryptoContextsDialog::~CSupportedCryptoContextsDialog()
    {
    return;
    } 
// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CGetCryptoContextsDialog::CGetCryptoContextsDialog( )
// .
// ----------------------------------------------------------------------------
//
CGetCryptoContextsDialog::CGetCryptoContextsDialog( TBool& aCrypto32,TBool& aCrypto80):
										iCrypto32(aCrypto32),iCrypto80(aCrypto80)
											
    {
    }

// ----------------------------------------------------------------------------
// CGetCryptoContextsDialog::PreLayoutDynInitL()
// .
// ----------------------------------------------------------------------------
//
void CGetCryptoContextsDialog::PreLayoutDynInitL()
    {
    CAknPopupFieldText* popupFieldText = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialogSecureSessionType));
        
    popupFieldText->SetCurrentValueIndex(2);
    
    }

// ----------------------------------------------------------------------------
// CGetCryptoContextsDialog::OkToExitL( TInt )
// .
// ----------------------------------------------------------------------------
//

TBool CGetCryptoContextsDialog::OkToExitL( TInt aKey )
    {
    if ( aKey == EEikCmdCanceled )
        {
        // Cancel pressed. Just exit.
        return ETrue;
        }
    CAknPopupFieldText* popupFieldText = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialogSecureSessionType));
    TInt index = popupFieldText->CurrentValueIndex();
    if(index == 0)
    	{
    	iCrypto32 = ETrue;
    	}
    else if(index == 1)
    	{
    	iCrypto80 = ETrue;
       	}
    else if(index == 2)
    	{
    	iCrypto32 = ETrue;
    	iCrypto80 = ETrue;
    	}
     return ETrue;
    }

// ----------------------------------------------------------------------------
// CGetCryptoContextsDialog::~CGetCryptoContextsDialog()
// .
// ----------------------------------------------------------------------------
//
CGetCryptoContextsDialog::~CGetCryptoContextsDialog()
    {
    return;
    } 

// End of File
