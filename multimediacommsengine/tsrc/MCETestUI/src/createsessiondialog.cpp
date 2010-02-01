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
#include "createSessionDialog.h"
#include <AknControl.h>
#include <AknListQueryControl.h>

#include <aknpopupheadingpane.h>
#include <aknmessagequerycontrol.h>
#include <AknPopupFieldText.h> 

#include "mcetestuiengineconstants.h"

// CONSTANTS
const TInt KBufLength = 300;


// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CCreateSessionDialog::CCreateSessionDialog( CRtpEngine::TSessionType* )
// .
// ----------------------------------------------------------------------------
//
CCreateSessionDialog::CCreateSessionDialog( TSessionParams* aParams ) :
    iParams( aParams )
    {
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CCreateSessionDialog::PreLayoutDynInitL()
// .
// ----------------------------------------------------------------------------
//
void CCreateSessionDialog::PreLayoutDynInitL()
    {
    
    CAknForm::PreLayoutDynInitL();
    CEikEdwin* edwinRecipentAddress = static_cast<CEikEdwin*>(
                               Control( ESessionDialogRecipentAddress ) );
      
    TBuf<KBufLength> defaultAddress;
    defaultAddress.Copy(iParams->iRecipentAddress);
    edwinRecipentAddress->SetTextL(&defaultAddress);
   
        
    CAknPopupFieldText* popupFieldText = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialogSessionType));
        
    CAknPopupFieldText* popupFieldText2 = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialogVideoSessionType));
        
    CAknPopupFieldText* popupFieldText10 = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialogSecureSession));
        
    CAknPopupFieldText* popupFieldText3 = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialogSourceType));
        
    TBuf<KBufLength> sourceFileName;
    sourceFileName.Copy(_L("c:\\testSource.3gp"));
    CEikEdwin* sourceFile = static_cast<CEikEdwin*>(
                                 Control( ESessionDialogSourceFilePath ) );
   	sourceFile->SetTextL(&sourceFileName);
   	 
    CAknPopupFieldText* popupFieldText4 = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialogAudioSinkType));
        
/*     
    CAknListQueryControl* listQueryControl =
  		  static_cast <CAknListQueryControl*> (Control(ESessionDialogVideoSinkType));
  		  
 */ 
 	  
    TBuf<KBufLength> sinkFileName;
    sinkFileName.Copy(_L("c:\\testSink.3gp"));
 	CEikEdwin* sinkFile = static_cast<CEikEdwin*>(
                                 Control( ESessionDialogSinkFilePath ) );
   	sinkFile->SetTextL(&sinkFileName);
   	
   	CAknPopupFieldText* popupFieldText5 = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialogVideoSinkDisplay));
        
    CAknPopupFieldText* popupFieldText6 = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialogVideoSinkFile));
        
    CAknPopupFieldText* popupFieldText7 = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialogVideoSinkRtp));
 	
 	CAknPopupFieldText* popupFieldText8 = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialogPrecondition));
    
  /*  CAknPopupFieldText* popupFieldText9 = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialogBundle));*/
         	
    CEikNumberEditor* serviceType = static_cast<CEikNumberEditor*>(
                                  Control( ESessionDialogServiceType ) );
    serviceType->SetNumber(0);   
    
 	iParams->originator.Copy(KNone);
    CEikEdwin* OriginaterAddress = static_cast<CEikEdwin*>(
                               Control( ESessionOriginaterAddress ) );
    defaultAddress.Copy(iParams->originator);
    OriginaterAddress->SetTextL(&defaultAddress);
    }

// ----------------------------------------------------------------------------
// CCreateSessionDialog::OkToExitL( TInt )
// .
// ----------------------------------------------------------------------------
//

TBool CCreateSessionDialog::OkToExitL( TInt aKey )
    {
    if ( aKey == EEikCmdCanceled )
        {
        // Cancel pressed. Just exit.
        return ETrue;
        }
    CEikEdwin* edwinRecipentAddress = static_cast<CEikEdwin*>(
                               Control( ESessionDialogRecipentAddress ) );
    TBuf<KBufLength> buffer;  
    edwinRecipentAddress->GetText(buffer);
    iParams->iRecipentAddress = buffer;   
    CAknPopupFieldText* popupFieldText = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialogSessionType));
   
    iParams->index = popupFieldText->CurrentValueIndex();
    
    CAknPopupFieldText* popupFieldText2 = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialogVideoSessionType));
   
    iParams->videoIndex = popupFieldText2->CurrentValueIndex();
    
    // for secure session 
    CAknPopupFieldText* popupFieldText10 = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialogSecureSession));
    if(popupFieldText10->CurrentValueIndex())
    	{
    	iParams->iSecureSession = ETrue;
    	}
     else
     	{
     	iParams->iSecureSession = EFalse;
     	}
    
    CAknPopupFieldText* popupFieldText3 = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialogSourceType));
   
    iParams->sourceType = popupFieldText3->CurrentValueIndex();
    
    CEikEdwin* sourceFile = static_cast<CEikEdwin*>(
                                 Control( ESessionDialogSourceFilePath ) );
    sourceFile->GetText(buffer);
    iParams->sourceFile = buffer;
    
    CAknPopupFieldText* popupFieldText4 = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialogAudioSinkType));
   
    iParams->audioSinkType = popupFieldText4->CurrentValueIndex();
    
 /*   CAknPopupFieldText* popupFieldText5 = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialogVideoSinkType));
   
    iParams->videoSinkType = popupFieldText5->CurrentValueIndex();
   */ 
   
/*   	CAknListQueryControl* listQueryControl =
  		  static_cast <CAknListQueryControl*> (Control(ESessionDialogVideoSinkType));
  */		  
  
    CEikEdwin* sinkFile = static_cast<CEikEdwin*>(
                                 Control( ESessionDialogSinkFilePath ) );
    sinkFile->GetText(buffer);
    iParams->sinkFile = buffer;
    
    CAknPopupFieldText* popupFieldText5 = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialogVideoSinkDisplay));
    iParams->videoSinkDisplay = popupFieldText5->CurrentValueIndex();    
    
    CAknPopupFieldText* popupFieldText6 = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialogVideoSinkFile));
    iParams->videoSinkFile = popupFieldText6->CurrentValueIndex();
        
    CAknPopupFieldText* popupFieldText7 = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialogVideoSinkRtp));
 	iParams->videoSinkRtp = popupFieldText7->CurrentValueIndex();
    
    CAknPopupFieldText* popupFieldText8 = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialogPrecondition));
 	iParams->iNoPrecondtion = popupFieldText8->CurrentValueIndex();
    
   /* CAknPopupFieldText* popupFieldText9 = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialogBundle));
 	iParams->iBundle = popupFieldText9->CurrentValueIndex();*/
 
 	CEikNumberEditor* serviceType = static_cast<CEikNumberEditor*>(
                              Control( ESessionDialogServiceType ) );
    iParams->iServiceType = serviceType->Number();
    
 	
    CEikEdwin* OriginaterAddress = static_cast<CEikEdwin*>(
                                 Control( ESessionOriginaterAddress ) );
    OriginaterAddress->GetText(buffer);
    iParams->originator = buffer;
   
     return ETrue;
    }

// ----------------------------------------------------------------------------
// CCreatePoCSessionDialog::~CCreatePoCSessionDialog()
// .
// ----------------------------------------------------------------------------
//
CCreateSessionDialog::~CCreateSessionDialog()
    {
    return;
    } 

// End of File
