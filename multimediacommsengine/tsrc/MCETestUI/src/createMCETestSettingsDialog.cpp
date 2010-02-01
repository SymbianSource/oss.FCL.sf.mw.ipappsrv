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
#include "createMceTestSettingsDialog.h"

// CONSTANTS

const TInt KBufLength = 100;

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CCreateMceSettingsDialog::CCreateMceSettingsDialog( )
// .
// ----------------------------------------------------------------------------
//
CCreateMceSettingsDialog::CCreateMceSettingsDialog(
							CMCETestUIEngine& aEngine):
							iEngine(aEngine) 
    
    {
    
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CCreateMceSettingsDialog::PreLayoutDynInitL()
// .
// ----------------------------------------------------------------------------
//
void CCreateMceSettingsDialog::PreLayoutDynInitL()
    {
     CAknForm::PreLayoutDynInitL();
    CAknPopupFieldText* popupFieldText = 
        static_cast <CAknPopupFieldText*> (Control(EMceSettingsFCSession));
    if(iEngine.IsFCSession())
    	{
    	popupFieldText->SetCurrentValueIndex(1);
    	}
    else
    	{
    	popupFieldText->SetCurrentValueIndex(0);
    	}
    
    CAknPopupFieldText* popupFieldText10 = 
        static_cast <CAknPopupFieldText*> (Control(EMceSettingsAudioPriority));
    if( iEngine.iPriorityType == 1 )
    	{
    	popupFieldText10->SetCurrentValueIndex(0);
    	}
    else if( iEngine.iPriorityType == 2 )
        {
        popupFieldText10->SetCurrentValueIndex(1);
        }
    else
    	{
    	popupFieldText10->SetCurrentValueIndex(2);
    	}
    	
   	CAknPopupFieldText* popupFieldText11 = 
        static_cast <CAknPopupFieldText*> (Control(EMceSettingsMultiCodecSupport));
    if(iEngine.iMultiCodecSupport)
    	{
    	popupFieldText11->SetCurrentValueIndex(1);
    	}
    else
    	{
    	popupFieldText11->SetCurrentValueIndex(0);
    	}
    CAknPopupFieldText* popupFieldText7 = 
        static_cast <CAknPopupFieldText*> (Control(EMceSettingsVoipSession));
    if(iEngine.iVoIPFlag)
    	{
    	popupFieldText7->SetCurrentValueIndex(1);
    	}
    else
    	{
    	popupFieldText7->SetCurrentValueIndex(0);
    	}
     CAknPopupFieldText* popupFieldText1 = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialogSessionRtcp));
    if(iEngine.iRtcpFlag)
    	{
    	popupFieldText1->SetCurrentValueIndex(1);
    	}
    else
    	{
    	popupFieldText1->SetCurrentValueIndex(0);
    	}
    	
    CAknPopupFieldText* popupFieldText2 = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialogSessionAccContactKoppa));
    if(iEngine.iKoppaFlag)
    	{
    	popupFieldText2->SetCurrentValueIndex(1);
    	}
    else
    	{
    	popupFieldText2->SetCurrentValueIndex(0);
    	}
    
    CAknPopupFieldText* popupFieldText3 = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialogBundle));
    
    if(iEngine.iBundle)
    	{
    	popupFieldText3->SetCurrentValueIndex(1);
    	}
    else
    	{
    	popupFieldText3->SetCurrentValueIndex(0);
    	}
    	
    CAknPopupFieldText* popupFieldText4 = 
        static_cast <CAknPopupFieldText*> (Control(EMceSettingsSDP));
    
    if(iEngine.iSdp)
    	{
    	popupFieldText4->SetCurrentValueIndex(1);
    	}
    else
    	{
    	popupFieldText4->SetCurrentValueIndex(0);
    	}
    	
    
    CAknPopupFieldText* popupFieldText5 = 
        static_cast <CAknPopupFieldText*> (Control(EMceSettings180Precondition));
    
    if(iEngine.i180PreCondition)
    	{
    	popupFieldText5->SetCurrentValueIndex(1);
    	}
    else
    	{
    	popupFieldText5->SetCurrentValueIndex(0);
    	}
    	
    CAknPopupFieldText* popupFieldText8 = 
        static_cast <CAknPopupFieldText*> (Control(EMceSettingsHoldWithAddress));
    
    if( iEngine.iHoldWithAddress )
    	{
    	popupFieldText8->SetCurrentValueIndex(1);
    	}
    else
    	{
    	popupFieldText8->SetCurrentValueIndex(0);
    	}
    		
    
    CAknPopupFieldText* popupFieldText6 = 
        static_cast <CAknPopupFieldText*> (Control(EMceSettingsCodemeniconAuto));
    
    if(iEngine.iAutoResponse)
    	{
    	popupFieldText6->SetCurrentValueIndex(1);
    	}
    else
    	{
    	popupFieldText6->SetCurrentValueIndex(0);
    	}
    CAknPopupFieldText* popupFieldText9 = 
        static_cast <CAknPopupFieldText*> (Control(EMceSettingsSecureSessionAVP));
    
    if(iEngine.iSecureSessionAVP)
    	{
    	popupFieldText9->SetCurrentValueIndex(1);
    	}
    else
    	{
    	popupFieldText9->SetCurrentValueIndex(0);
    	}
    	
    CAknPopupFieldText* popupFieldText12 = 
        static_cast <CAknPopupFieldText*> (Control(EMceSecPreconditions));	
    	
    if(iEngine.iSecPreconditions)
    	{
    	popupFieldText12->SetCurrentValueIndex(1);
    	}
    else
    	{
    	popupFieldText12->SetCurrentValueIndex(0);
    	}	
    
     popupFieldText = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialog2WayVideoViewFinder));
        
    if(iEngine.iInSessionSettings.videoSinkDisplay == TSessionParams::EMceCameraPrimary )
    	{
        popupFieldText->SetCurrentValueIndex(0);
    	}
    else
        {
        popupFieldText->SetCurrentValueIndex(1);
        }
    
    popupFieldText = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialog2WayVideoCamera));
    
    if(iEngine.iInSessionSettings.iCameraSetting == TSessionParams::EMceViewFinderDisabled )
    	{
        popupFieldText->SetCurrentValueIndex(0);
    	}
    else
        {
        popupFieldText->SetCurrentValueIndex(1);
        }
    	
    }

// ----------------------------------------------------------------------------
// CCreateMceSettingsDialog::OkToExitL( TInt )
// .
// ----------------------------------------------------------------------------
//

TBool CCreateMceSettingsDialog::OkToExitL( TInt aKey )
    {
    if ( aKey == EEikCmdCanceled )
        {
        // Cancel pressed. Just exit.
        return ETrue;
        }
	CAknPopupFieldText* popupFieldText = 
    					static_cast <CAknPopupFieldText*> (Control(EMceSettingsFCSession));

    if( popupFieldText->CurrentValueIndex() == 0)
    	{
    	iEngine.SetFCSession(EFalse);
    	}
    else
    	{
    	iEngine.SetFCSession(ETrue);
    	}
    	CAknPopupFieldText* popupFieldText7 = 
    					static_cast <CAknPopupFieldText*> (Control(EMceSettingsVoipSession));
    
    CAknPopupFieldText* popupFieldText11 = 
    					static_cast <CAknPopupFieldText*> (Control(EMceSettingsMultiCodecSupport));

    if( popupFieldText11->CurrentValueIndex() == 0)
    	{
    	iEngine.iMultiCodecSupport = EFalse;
    	}
    else
    	{
    	iEngine.iMultiCodecSupport = ETrue;
    	}
    
    CAknPopupFieldText* popupFieldText10 = 
        static_cast <CAknPopupFieldText*> (Control(EMceSettingsAudioPriority));
    if( popupFieldText10->CurrentValueIndex() == 0 )
    	{
    	iEngine.iPriorityType = KPoCPriority;
    	}
    else if( popupFieldText10->CurrentValueIndex() == 1)
        {
        iEngine.iPriorityType = KVoIPPriority;
        }
    else
    	{
    	iEngine.iPriorityType = KVSPriority;
    	}

    if( popupFieldText7->CurrentValueIndex())
    	{
    	iEngine.iVoIPFlag = ETrue;
    	}
    else
    	{
    	iEngine.iVoIPFlag = EFalse;
    	}		
   
    CAknPopupFieldText* popupFieldText1 = 
    static_cast <CAknPopupFieldText*> (Control(ESessionDialogSessionRtcp));  
    if(popupFieldText1->CurrentValueIndex()) 
    	{
    	iEngine.iRtcpFlag = ETrue;
    	}
    else
    	{
    	iEngine.iRtcpFlag = EFalse;
    	}	
    	
    CAknPopupFieldText* popupFieldText2 = 
    static_cast <CAknPopupFieldText*> (Control(ESessionDialogSessionAccContactKoppa));  
    if(popupFieldText2->CurrentValueIndex()) 
    	{
    	iEngine.iKoppaFlag = ETrue;
    	}
    else
    	{
    	iEngine.iKoppaFlag = EFalse;
    	}
    
    CAknPopupFieldText* popupFieldText3 = 
    static_cast <CAknPopupFieldText*> (Control(ESessionDialogBundle));  
    if(popupFieldText3->CurrentValueIndex()) 
    	{
    	iEngine.iBundle = ETrue;
    	}
    else
    	{
    	iEngine.iBundle = EFalse;
    	}
    	
    CAknPopupFieldText* popupFieldText4 = 
    static_cast <CAknPopupFieldText*> (Control(EMceSettingsSDP));  
    if(popupFieldText4->CurrentValueIndex()) 
    	{
    	iEngine.iSdp = ETrue;
    	}
    else
    	{
    	iEngine.iSdp = EFalse;
    	}
    CAknPopupFieldText* popupFieldText5 = 
    static_cast <CAknPopupFieldText*> (Control(EMceSettings180Precondition));  
    if(popupFieldText5->CurrentValueIndex()) 
    	{
    	iEngine.i180PreCondition = ETrue;
    	}
    else
    	{
    	iEngine.i180PreCondition = EFalse;
    	}
    		
    CAknPopupFieldText* popupFieldText8 = 
    static_cast <CAknPopupFieldText*> (Control(EMceSettingsHoldWithAddress));  
    if(popupFieldText8->CurrentValueIndex()) 
    	{
    	iEngine.iHoldWithAddress = ETrue;
    	}
    else
    	{
    	iEngine.iHoldWithAddress = EFalse;
    	}		
    			
    CAknPopupFieldText* popupFieldText6 = 
    static_cast <CAknPopupFieldText*> (Control(EMceSettingsCodemeniconAuto));  
    if(popupFieldText6->CurrentValueIndex()) 
    	{
    	iEngine.iAutoResponse = ETrue;
    	}
    else
    	{
    	iEngine.iAutoResponse = EFalse;
    	}
    
    	
     CAknPopupFieldText* popupFieldText9 = 
        static_cast <CAknPopupFieldText*> (Control(EMceSettingsSecureSessionAVP));
    
    if(popupFieldText9->CurrentValueIndex())
    	{
    	iEngine.iSecureSessionAVP = ETrue;
    	}
    else
    	{
    	iEngine.iSecureSessionAVP = EFalse;
    	}

    CAknPopupFieldText* popupFieldText12 = 
        static_cast <CAknPopupFieldText*> (Control(EMceSecPreconditions));	
    	
    if(popupFieldText12->CurrentValueIndex()) 
    	{
    	iEngine.iSecPreconditions = ETrue;
    	}
    else
    	{
    	iEngine.iSecPreconditions = EFalse;
    	}		
	
	popupFieldText = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialog2WayVideoViewFinder));
    
    if( popupFieldText->CurrentValueIndex() )
    	{
        iEngine.iInSessionSettings.videoSinkDisplay = TSessionParams::EMceViewFinderEnabled;
    	}
    else
        {
        iEngine.iInSessionSettings.videoSinkDisplay = TSessionParams::EMceViewFinderDisabled;
        }
        
	popupFieldText = 
        static_cast <CAknPopupFieldText*> (Control(ESessionDialog2WayVideoCamera));
    
    if( popupFieldText->CurrentValueIndex() )
    	{
        iEngine.iInSessionSettings.iCameraSetting = TSessionParams::EMceCameraSecondary;
    	}
    else
        {
        iEngine.iInSessionSettings.iCameraSetting = TSessionParams::EMceCameraPrimary;
        }
        	
    return ETrue;
    }

// ----------------------------------------------------------------------------
// CCreateMceSettingsDialog::~CCreateMceSettingsDialog()
// .
// ----------------------------------------------------------------------------
//
CCreateMceSettingsDialog::~CCreateMceSettingsDialog()
    {
    return;
    } 

// End of File
