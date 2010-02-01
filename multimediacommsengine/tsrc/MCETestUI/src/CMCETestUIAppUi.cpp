/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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


#include "CMCETestUIAppUi.h"
#include "CMCETestUIDoc.h"
#include "CMCETestUIAppView.h"
#include "CMCETestUISessionView.h"
#include "CMCETestUIStreamView.h"
#include "createpocsessiondialog.h"
#include "createvoipsessiondialog.h"
#include "create2wayvideosessiondialog.h"
#include "createsessiondialog.h"
#include "createeventdialog.h"
#include "createreferdialog.h"
#include "createNotifyDialog.h"
#include "createsessionEstablishValuesdialog.h"
#include "createMceTestValueSettingsDialog.h"
#include "createMceTestSettingsDialog.h"
#include "createStartDTMFDialog.h"
#include "createSendDTMFDialog.h"
#include "createSendDTMFSequenceDialog.h"
#include "createCryptoHandlingDialog.h"
#include "createAudioCodecDialog.h"
#include "mcetestuiactivitymanager.h"
#include <MCETestUI.rsg>

#include <aknappui.h>
#include <aknpopup.h>
#include <AknQueryDialog.h>
#include <AknNoteWrappers.h>

#include "CMCETestUIEngine.h"

const TInt KBufLength = 100;
const TInt KMCETestUIBacklightTimeOut = 4;    // seconds


CMCETestUIAppUi::CMCETestUIAppUi( CMCETestUIEngine& aEngine )
    :iEngine(aEngine)
    {
    iEngine.SetUserInterface( this );
    }

void CMCETestUIAppUi::ConstructL()
    {
	           
    BaseConstructL(EAknEnableSkin);
  
		
	iMainView = CMCETestUIAppView::NewL( iEngine );
    AddViewL( iMainView ); // transfer ownership

    iSessionView = CMCETestUISessionView::NewL( iEngine );
    AddViewL( iSessionView );
    
    iStreamView = CMCETestUIStreamView::NewL(iEngine);
    AddViewL(iStreamView);
 
    SetDefaultViewL( *iMainView ); 
    iEnableVideo = EFalse;
    
    iActivityManager = CMCETestUIActivityManager::NewL( KMCETestUIBacklightTimeOut );
    iActivityManager->Reset();

    }


CMCETestUIAppUi::~CMCETestUIAppUi()
	{
    delete iActivityManager;
	}


// -----------------------------------------------------------------------------
// CMCETestUIAppUi::EngineStateChanged
// -----------------------------------------------------------------------------
//
void CMCETestUIAppUi::EngineStateChangedL()
    {
    iMainView->EngineStateChangedL();
    iSessionView->EngineStateChangedL();
    iStreamView->EngineStateChangedL();
    }


// -----------------------------------------------------------------------------
// CMCETestUIAppUi::NotifyUser
// -----------------------------------------------------------------------------
//
void CMCETestUIAppUi::NotifyUser( const TDesC& aMessage )
    {
   // User::InfoPrint( aMessage );
    CAknInformationNote* note = new (ELeave) CAknInformationNote( EFalse );
    note->SetTimeout( CAknNoteDialog::EShortTimeout  );
    note->ExecuteLD( aMessage );
    }
    
void CMCETestUIAppUi::NotifyUserWaitL( const TDesC& aMessage )
{
    CAknConfirmationNote* note = new (ELeave) CAknConfirmationNote( ETrue );
    note->SetTimeout( CAknNoteDialog::ENoTimeout  );
    note->ExecuteLD( aMessage );
}

// -----------------------------------------------------------------------------
// CMCETestUIAppUi::AskQuestion
// -----------------------------------------------------------------------------
//
TInt CMCETestUIAppUi::AskQuestionL( 
        const CMCETestUIQuestionSingleSelection& aQuestion )
    {
    
    // Generate list
    CAknSinglePopupMenuStyleListBox* listBox = 
                    new (ELeave) CAknSinglePopupMenuStyleListBox;   
    CleanupStack::PushL( listBox );    

    CAknPopupList* popup = CAknPopupList::NewL( 
                    listBox, 
                    R_AVKON_SOFTKEYS_MENU_LIST,
                    AknPopupLayouts::EPopupSNotePopupWindow );
    CleanupStack::PushL( popup );
    popup->SetTitleL( aQuestion.Caption() );

	listBox->ConstructL( popup, 
		EAknListBoxSelectionList | EAknListBoxLoopScrolling);
	
	listBox->CreateScrollBarFrameL( ETrue );
	listBox->ScrollBarFrame()->SetScrollBarVisibilityL(
		CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto );
	
    listBox->Model()->SetItemTextArray( &(aQuestion.Choices()) );	        
	listBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );
	        
	CleanupStack::Pop( popup ); 
	TInt selectedIndex = -1;             
    if ( popup->ExecuteLD() )
        {
        selectedIndex = listBox->CurrentItemIndex();
        }

    CleanupStack::PopAndDestroy( listBox );    
    return selectedIndex;
  
    }

// -----------------------------------------------------------------------------
// CMCETestUIAppUi::AskQuestion
// -----------------------------------------------------------------------------
//
void CMCETestUIAppUi::AskQuestionL( CMCETestUIQuestionDataQuery& aQuestion )
    {
   
  	TBuf<KBufLength> answer;
    answer.Copy( aQuestion.DefaultValue() ); 
    CAknTextQueryDialog* dlg = CAknTextQueryDialog::NewL( answer );
    dlg->SetMaxLength( KBufLength );
    dlg->ExecuteLD( R_TEXT_QUERY_DIALOG, aQuestion.Caption() );
    aQuestion.SetAnswerL( answer );	
         
    }
// -----------------------------------------------------------------------------
// CMCETestUIAppUi::SessionParams
// -----------------------------------------------------------------------------
//
TBool CMCETestUIAppUi::SessionParamsL(TSessionParams &aSessionParams, TSessionType aSessionType)
{
    if( aSessionType == EPoCSession)
        {
        CCreatePoCSessionDialog* dialog = new ( ELeave ) CCreatePoCSessionDialog( &aSessionParams );
        if(dialog->ExecuteLD( R_POC_SESSION_DIALOG ))
        	{
        	return ETrue;
        	}
        }
    else if( aSessionType == EVoIPSession)
        {
        CCreateVoIPSessionDialog* dialog = new ( ELeave ) CCreateVoIPSessionDialog( &aSessionParams );
        if(dialog->ExecuteLD( R_VOIP_SESSION_DIALOG ))
        	{
        	return ETrue;
        	}
        }
    else if ( aSessionType == E2WayVideoSession )
        {
        CCreate2WayVideoSessionDialog* dialog = new ( ELeave ) CCreate2WayVideoSessionDialog( &aSessionParams );
        if(dialog->ExecuteLD( R_2WAYVIDEO_SESSION_DIALOG ))
        	{
        	return ETrue;
        	}
        }
    else if ( aSessionType == ECustomSession)
        {
        CCreateSessionDialog* dialog = new ( ELeave ) CCreateSessionDialog( &aSessionParams );
        if(dialog->ExecuteLD( R_MCE_SESSION_DIALOG ) )
        	{
        	return ETrue;
        	}
        }
	return ETrue ;// this code still needs to be updated as it is wrong behaviour
    }

// -----------------------------------------------------------------------------
// CMCETestUIAppUi::SessionEstablishValues
// -----------------------------------------------------------------------------
//

TBool CMCETestUIAppUi::SessionEstablishValues(TSessionEstablishValues &sessionValues)
	{
	CSessionEstablishValuesDialog *dialog = new (ELeave)CSessionEstablishValuesDialog(&sessionValues);
	if (dialog->ExecuteLD(R_MCE_ESTABLISH_VALUES_DIALOG))
		{
		return ETrue;
		}
	
	return EFalse;
	}

// -----------------------------------------------------------------------------
// CMCETestUIAppUi::SessionEstablishValues
// -----------------------------------------------------------------------------
//

TBool CMCETestUIAppUi::NotifyValues(TNotifyValues &notifyValues)
	{
	CNotifyDialog *dialog = new (ELeave)CNotifyDialog(&notifyValues);
	if (dialog->ExecuteLD(R_MCE_NOTIFY_DIALOG))
		{
		return ETrue;
		}
	
	return EFalse;
	}
	
// -----------------------------------------------------------------------------
// CMCETestUIAppUi::SessionResponseValues
// -----------------------------------------------------------------------------
//

TBool CMCETestUIAppUi::SessionResponseValues(TSessionResponseValues &responseValues)
	{
	CSessionResponseValuesDialog *dialog = new (ELeave)CSessionResponseValuesDialog(&responseValues);
	if (dialog->ExecuteLD(R_MCE_RESPONSE_VALUES_DIALOG))
		{
		return ETrue;
		}
	
	return EFalse;
	}
// -----------------------------------------------------------------------------
// CMCETestUIAppUi::StartDTMFToneL
// -----------------------------------------------------------------------------
//
TBool CMCETestUIAppUi::StartDTMFToneL( TChar& aChar )
	{
	CStartDTMFDialog *dialog = new (ELeave)CStartDTMFDialog( aChar );
	if (dialog->ExecuteLD( R_MCE_START_DTMF_TONE_DIALOG ))
		{
		return ETrue;
		}
	
	return EFalse;
	}

// -----------------------------------------------------------------------------
// CMCETestUIAppUi::StartDTMFToneL
// -----------------------------------------------------------------------------
//
TBool CMCETestUIAppUi::SendDTMFToneL( TChar& aChar, TInt& aInterval )
	{
	CSendDTMFDialog *dialog = new (ELeave)CSendDTMFDialog( aChar, aInterval );
	if (dialog->ExecuteLD( R_MCE_SEND_DTMF_TONE_DIALOG ))
		{
		return ETrue;
		}
	
	return EFalse;
	}
// -----------------------------------------------------------------------------
// CMCETestUIAppUi::StartDTMFToneL
// -----------------------------------------------------------------------------
//
TBool CMCETestUIAppUi::SendDTMFToneSequenceL( TDes& aSequence )
	{
	CSendDTMFSequenceDialog *dialog = new (ELeave)CSendDTMFSequenceDialog( aSequence );
	if (dialog->ExecuteLD( R_MCE_SEND_DTMF_TONE_SEQUENCE_DIALOG ))
		{
		return ETrue;
		}
	
	return EFalse;
	}
	
// -----------------------------------------------------------------------------
// CMCETestUIAppUi::GetCodecTypeL
// -----------------------------------------------------------------------------
//
TBool CMCETestUIAppUi::GetCodecTypeL( TInt &aCodecIndex)
    {
    aCodecIndex = 1;
  	CAddCodecDialog* dlg = new (ELeave) CAddCodecDialog(aCodecIndex);
	if(dlg->ExecuteLD(R_MCE_ADD_CODEC_DIALOG))
		{
		return ETrue;
		}
	return EFalse;
    }
// -----------------------------------------------------------------------------
// CMCETestUIAppUi::GetIapIdL
// -----------------------------------------------------------------------------
//
TBool CMCETestUIAppUi::GetIapIdL( TInt &aNumber)
    {
    aNumber = 1;
  	CAknNumberQueryDialog* dlg = new (ELeave) CAknNumberQueryDialog(aNumber,CAknQueryDialog::ENoTone);
	if(dlg->ExecuteLD(R_NUMBER_QUERY_DIALOG))
		{
		return ETrue;
		}
	return EFalse;
    }
    
    
 // -----------------------------------------------------------------------------
// CMCETestUIAppUi::GetIapIdL
// -----------------------------------------------------------------------------
//
 void CMCETestUIAppUi::ShowCurrentCryptoContextsL(TBool aCrypto32,TBool aCrypto80)
 	{
 	CCurrentCryptoContextsDialog *dialog = new (ELeave)CCurrentCryptoContextsDialog( aCrypto32,aCrypto80 );
	dialog->ExecuteLD( R_MCE_SECURE_SESSION_DIALOG );
 	}
 	
  // -----------------------------------------------------------------------------
// CMCETestUIAppUi::GetIapIdL
// -----------------------------------------------------------------------------
//
 void CMCETestUIAppUi::ShowSupportedCryptoContextsL(TBool aCrypto32,TBool aCrypto80)
 	{
 	CSupportedCryptoContextsDialog *dialog = new (ELeave)CSupportedCryptoContextsDialog( aCrypto32,aCrypto80 );
	dialog->ExecuteLD( R_MCE_SECURE_SESSION_DIALOG );
 	}
 	
  // -----------------------------------------------------------------------------
// CMCETestUIAppUi::GetIapIdL
// -----------------------------------------------------------------------------
//
 void CMCETestUIAppUi::GetCryptoContextsL(TBool& aCrypto32,TBool& aCrypto80)
 	{
 	CGetCryptoContextsDialog *dialog = new (ELeave)CGetCryptoContextsDialog( aCrypto32,aCrypto80 );
	dialog->ExecuteLD( R_MCE_SECURE_SESSION_DIALOG );
 	}
// -----------------------------------------------------------------------------
// CMCETestUIAppUi::EventParams
// -----------------------------------------------------------------------------
//
TBool CMCETestUIAppUi::EventParamsL(TEventParams& aEventParams)
{
   
    CCreateEventDialog* dialog = new ( ELeave ) CCreateEventDialog( &aEventParams );
    if (dialog->ExecuteLD( R_MCE_EVENT_DIALOG ) )
    	{
    	return ETrue;
    	}
    return ETrue ; // this code still needs to be updated as it is wrong behaviour
}

// -----------------------------------------------------------------------------
// CMCETestUIAppUi::ReferParamsL
// -----------------------------------------------------------------------------
//
TBool CMCETestUIAppUi::ReferParamsL(TReferParams&  aReferParams)
{
   
    CCreateReferDialog* dialog = new ( ELeave ) CCreateReferDialog( &aReferParams );
    if (dialog->ExecuteLD( R_MCE_REFER_DIALOG ) )
    	{
    	return ETrue;
    	}
    return ETrue ; // this code still needs to be updated as it is wrong behaviour
}

// -----------------------------------------------------------------------------
// CMCETestUIAppUi::ReferToParamsL
// -----------------------------------------------------------------------------
//
TBool CMCETestUIAppUi::ReferToParamsL(TReferParams&  aReferParams)
{
   
    CCreateReferToDialog* dialog = new ( ELeave ) CCreateReferToDialog( &aReferParams );
    if (dialog->ExecuteLD( R_MCE_REFERTO_DIALOG ) )
    	{
    	return ETrue;
    	}
    return ETrue ; // this code still needs to be updated as it is wrong behaviour
}

// -----------------------------------------------------------------------------
// CMCETestUIEngine::CreateMceSettingsL
// -----------------------------------------------------------------------------
//

void CMCETestUIAppUi::CreateMceSettingsL()
{
	CCreateMceSettingsDialog* dialog = new(ELeave)CCreateMceSettingsDialog(iEngine);
	dialog->ExecuteLD(R_MCE_MCESETTINGS_DIALOG );
	 
}

// -----------------------------------------------------------------------------
// CMCETestUIEngine::CreateMceValueSettingsL
// -----------------------------------------------------------------------------
//

void CMCETestUIAppUi::CreateMceValueSettingsL()
    {
	CCreateMceValueSettingsDialog* dialog = new(ELeave)CCreateMceValueSettingsDialog(iEngine);
	dialog->ExecuteLD(R_MCE_VALUE_SETTINGS_DIALOG );
	 
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::HandleCommandL
// -----------------------------------------------------------------------------
//
void CMCETestUIAppUi::HandleCommandL(TInt aCommand)
	{
	switch (aCommand)
		{	
        case EAknSoftkeyExit:
		case EEikCmdExit:
		    {    
			Exit();
			break;
		    }
		case EAknSoftkeyBack:
		    {
		    iSessionView->ChangeToMainViewL();
		    break;
		    }	
		case EAknSoftkeyOptions:
            {
            if ( iView )
                {
                iView->HandleCommandL(aCommand);
                }
            break;
            }
		default:
	        break;

		}
	}

// -----------------------------------------------------------------------------
// CMCETestUIAppUi::ProcessCommandL
// -----------------------------------------------------------------------------
//
void CMCETestUIAppUi::ProcessCommandL(TInt aCommand)
    {
    if ( aCommand == EAknSoftkeyOptions)
        {
        if ( iView )
            {
            iView->HandleCommandL(aCommand);
            }
        return;
        }
    CAknViewAppUi::ProcessCommandL(aCommand);
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::ChangeToMainViewL
// -----------------------------------------------------------------------------
//	
void CMCETestUIAppUi::ChangeToMainViewL()
    {
    iSessionView->ChangeToMainViewL();
    }
// -----------------------------------------------------------------------------
// CMCETestUIEngine::IsVideoEnabled
// -----------------------------------------------------------------------------
//	
TBool CMCETestUIAppUi::IsVideoEnabled()
    {
    return iEnableVideo;
    }    

// -----------------------------------------------------------------------------
// CMCETestUIEngine::EnableVideo
// -----------------------------------------------------------------------------
//	
void CMCETestUIAppUi::EnableVideo()
    {
    iMainView->iContainer->MakeVisible( EFalse );
//    iMainView->MakeVisible( EFalse );
    iEnableVideo = ETrue;
    }    

// -----------------------------------------------------------------------------
// CMCETestUIEngine::DisableVideo
// -----------------------------------------------------------------------------
//	
void CMCETestUIAppUi::DisableVideo()
    {
    iMainView->iContainer->MakeVisible( ETrue );						    
//  	iMainView->MakeVisible( ETrue );						   
            		 
    iEnableVideo = EFalse;
    }    
// -----------------------------------------------------------------------------
// CMCETestUIEngine::DisableVideo
// -----------------------------------------------------------------------------
//	
TRect CMCETestUIAppUi::GetRect()
    {
   	return ClientRect();
    }    
