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
#include "create2wayvideosessiondialog.h"

// CONSTANTS
const TInt KBufLength = 300;


// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CCreate2WayVideoSessionDialog::CCreate2WayVideoSessionDialog( CRtpEngine::TSessionType* )
// .
// ----------------------------------------------------------------------------
//
CCreate2WayVideoSessionDialog::CCreate2WayVideoSessionDialog( TSessionParams* aParams ) :
    iParams( aParams )
    {
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CCreate2WayVideoSessionDialog::PreLayoutDynInitL()
// .
// ----------------------------------------------------------------------------
//
void CCreate2WayVideoSessionDialog::PreLayoutDynInitL()
    {
    CEikEdwin* edwinPoCAddress = static_cast<CEikEdwin*>(
                               Control( ESessionDialog2WayVideoRecipientAddress ) );
      
    TBuf<KBufLength> defaultAddress;
    defaultAddress.Copy(iParams->iRecipentAddress);
    edwinPoCAddress->SetTextL(&defaultAddress);
    edwinPoCAddress->SetCursorPosL(defaultAddress.Length(),EFalse);
    
    CAknPopupFieldText* popupFieldText = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialog2WayVideoVideoCodec));
        
    popupFieldText->SetCurrentValueIndex(0);
    
    popupFieldText = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialog2WayVideoAudioCodec));
        
    popupFieldText->SetCurrentValueIndex(0);
    
    popupFieldText = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialog2WayVideoViewFinder));
        
    popupFieldText->SetCurrentValueIndex(0);
    
    popupFieldText = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialog2WayVideoCamera));
        
    popupFieldText->SetCurrentValueIndex(0);
    
    CAknForm::PreLayoutDynInitL();
    }

// ----------------------------------------------------------------------------
// CCreate2WayVideoSessionDialog::OkToExitL( TInt )
// .
// ----------------------------------------------------------------------------
//

TBool CCreate2WayVideoSessionDialog::OkToExitL( TInt aKey )
    {
    if ( aKey == EEikCmdCanceled )
        {
        // Cancel pressed. Just exit.
        return ETrue;
        }
    
    CEikEdwin* edwinRecipientAddress = static_cast<CEikEdwin*>(
                               Control( ESessionDialog2WayVideoRecipientAddress ) );
    TBuf<KBufLength> buffer;  
    edwinRecipientAddress->GetText(buffer);
    iParams->iRecipentAddress  = buffer;  
        
    CAknPopupFieldText* popupFieldText = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialog2WayVideoVideoCodec));
    TInt index = popupFieldText->CurrentValueIndex();
    if(index == 0)
    	{
    	iParams->iVideoCodec = TSessionParams::EMceCodecSingle;
    	}
    else if ( index == 1 )
        {
        iParams->iVideoCodec = TSessionParams::EMceCodecAll;
        }
    else
    	{
    	iParams->iVideoCodec = TSessionParams::EMceCodecMultipleModes;
       	}

    popupFieldText = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialog2WayVideoAudioCodec));
    index = popupFieldText->CurrentValueIndex();
    if(index == 0)
    	{
    	iParams->iAudioCodec = TSessionParams::EMceCodecSingle;
    	}
    else if (index == 1)
    	{
    	iParams->iAudioCodec = TSessionParams::EMceCodecAll;
       	}
     else
        {
        iParams->iAudioCodec = TSessionParams::EMceCodecNone;
        }
    
    popupFieldText = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialog2WayVideoViewFinder));
    index = popupFieldText->CurrentValueIndex();
    if(index == 0)
    	{
    	iParams->videoSinkDisplay = TSessionParams::EMceViewFinderDisabled;
    	}
     else
        {
        iParams->videoSinkDisplay = TSessionParams::EMceViewFinderEnabled;
        }
        
    popupFieldText = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialog2WayVideoCamera));
    index = popupFieldText->CurrentValueIndex();
    if(index == 0)
    	{
    	iParams->iCameraSetting = TSessionParams::EMceCameraPrimary;
    	}
     else
        {
        iParams->iCameraSetting = TSessionParams::EMceCameraSecondary;
        }    
    
    return ETrue;
    }

// ----------------------------------------------------------------------------
// CCreate2WayVideoSessionDialog::~CCreate2WayVideoSessionDialog()
// .
// ----------------------------------------------------------------------------
//
CCreate2WayVideoSessionDialog::~CCreate2WayVideoSessionDialog()
    {
    } 

// End of File
