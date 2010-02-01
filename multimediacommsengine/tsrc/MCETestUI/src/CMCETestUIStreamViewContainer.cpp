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

// INCLUDE FILES
#include "CMCETestUIAppUi.h"
#include "CMCETestUIStreamViewContainer.h"
#include "CMCETestUIStreamViewModel.h"
#include "CMCETestUIEngine.h"
#include "CMCETestUIEngineProfile.h"
#include "CMCETestUIEngineSession.h"
#include "CMCETestUIEngineAudioStream.h"
#include "CMCETestUIEngineVideoStream.h"
#include "TMCETestUIEngineCmdBase.h"
#include <MCEMediaSink.h>
#include <MCERtpSink.h>
#include <MCEDisplaySink.h>
#include <MCESpeakerSink.h>
#include <MCEMicSource.h>
#include <MCEFileSource.h>
#include <MCEcameraSource.h>
#include <MCEMediaSource.h>
#include "CMCETestUIEngineSink.h"
#include "CMCETestUIEngineSource.h"
#include "CMCETestUIEngineVideoSource.h"
#include "CMCETestUIEngineVideoSink.h"
#include "createAudioCodecdialog.h"
#include "createVideoCodecdialog.h"
#include "createRtpSinkdialog.h"
#include "createSpeakerSinkdialog.h"
#include "createMicSourcedialog.h"
#include "createCameraSourcedialog.h"
#include "createFileSourcedialog.h"
#include "createDisplaySinkDialog.h"
#include <aknview.h>
#include <aknpopup.h>
#include <MCETestUI.rsg>


// CONSTANTS


// ================= MEMBER FUNCTIONS ==========================================

CMCETestUIStreamViewContainer* CMCETestUIStreamViewContainer::NewL(
        const TRect& aRect,
        CAknView& aOwningView,
        CMCETestUIEngine& aEngine,
        CMCETestUIEngineAudioStream& aStream )
    {
	CMCETestUIStreamViewContainer* self = 
	        new(ELeave) CMCETestUIStreamViewContainer( 
	                        aOwningView, 
	                        aEngine, 
	                        aStream );
	CleanupStack::PushL(self);
	self->ConstructL( aRect );
	CleanupStack::Pop(self);
	return self;    
    }

CMCETestUIStreamViewContainer::CMCETestUIStreamViewContainer( 
        CAknView& aOwningView, 
        CMCETestUIEngine& aEngine, 
        CMCETestUIEngineAudioStream& aStream )
    :iOwningView( aOwningView ),
    iEngine( aEngine ),
    iStream( aStream ),
    iContextMenuAlreadyShown( EFalse )
    {
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIStreamViewContainer::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// -----------------------------------------------------------------------------
//
void CMCETestUIStreamViewContainer::ConstructL(const TRect& aRect)
    {
    CreateWindowL();
    //Window().SetShadowDisabled(EFalse);

	iListBox = new (ELeave) CAknDoubleStyleListBox(); 
   
	iListBox->SetContainerWindowL(*this);
	iListBox->ConstructL( this, 
	                      EAknListBoxSelectionList | EAknListBoxLoopScrolling);
	iListBox->SetListBoxObserver(this);
	iListBox->SetMopParent(this);

    iListBoxModel = CMCETestUIStreamViewModel::NewL( iEngine , iStream);   

    iListBox->Model()->SetItemTextArray( iListBoxModel );      
	
    SetRect(aRect);

    iListBox->SetRect(Rect());

	//Set up the listbox to use a scroll bar
	iListBox->CreateScrollBarFrameL(ETrue);
	iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
	    CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);

    ConstructModelL();
    
    ActivateL(); 
   
    //iListBox->HandleItemAdditionL();   

    }

// -----------------------------------------------------------------------------
// CMCETestUISessionViewContainer::~CMCETestUISessionViewContainer()
// Destructor
// Frees reserved resources
// -----------------------------------------------------------------------------
//
CMCETestUIStreamViewContainer::~CMCETestUIStreamViewContainer()
    {
    delete iListBox;
    }

// -----------------------------------------------------------------------------
// CMCETestUIStreamViewContainer::~CMCETestUIStreamViewContainer()
// Destructor
// Frees reserved resources
// -----------------------------------------------------------------------------
//
void CMCETestUIStreamViewContainer::ConstructModelL()
    {
    iListBoxModel = CMCETestUIStreamViewModel::NewL( iEngine, iStream );   

    iListBox->Model()->SetItemTextArray( iListBoxModel ); 
    
    iListBox->Reset();      
    }

// -----------------------------------------------------------------------------
// CMCETestUIStreamViewContainer::SizeChanged()
// Called by framework when the view size is changed
// -----------------------------------------------------------------------------
//
void CMCETestUIStreamViewContainer::SizeChanged()
    {
    TInt error;
    
    // TRAP methods that can leave.
    TRAP(error, SizeChangedL());
    if(error != KErrNone)
        {
        User::Panic( KNullDesC16, 0 );
        }   
    }

// -----------------------------------------------------------------------------
// CMCETestUISessionViewContainer::SizeChangedL()
// Methods that can leave and are needed by SizeChanged
// are collected into this method.
// -----------------------------------------------------------------------------
//
void CMCETestUIStreamViewContainer::SizeChangedL()
    {
  
    }


// -----------------------------------------------------------------------------
// CMCETestUIStreamViewContainer::CountComponentControls() const
// Note! Output window must be ignored.
// -----------------------------------------------------------------------------
//
TInt CMCETestUIStreamViewContainer::CountComponentControls() const
    {
    if ( iListBox ) 
        return 1;
    return 0;
    }


// -----------------------------------------------------------------------------
// CMCETestUIStreamViewContainer::ComponentControl()
// -----------------------------------------------------------------------------
//
CCoeControl* CMCETestUIStreamViewContainer::ComponentControl(TInt /*aIndex*/) const
    {
    return iListBox;
    }

// -----------------------------------------------------------------------------
// CMCETestUIMainViewContainer::HandleListBoxEventL()
// -----------------------------------------------------------------------------
//
void CMCETestUIStreamViewContainer::HandleListBoxEventL( 
                CEikListBox* aListBox, 
                TListBoxEvent aEventType )
    {
    TInt selectedIndex = iListBox->CurrentItemIndex();
    iContextMenuAlreadyShown = EFalse;
    
    if ( aListBox == iListBox )
        {
        if ( aEventType == EEventEnterKeyPressed || aEventType == EEventItemDoubleClicked )
            {
            if ( iListBoxModel->IsAudioSourceSelected( selectedIndex ) )
                {
                // Change to manager view 
                const RPointerArray<CMCETestUIEngineSource>& sources = 
            										iStream.SourcesL();
            	if(sources[selectedIndex]->Type() == KMceMicSource)
            		{
            		CCreateMicSourceDialog* dialog = new ( ELeave )
                					 CCreateMicSourceDialog(iStream, selectedIndex );
    				if (dialog->ExecuteLD( R_MCE_STREAM_MICSOURCE_DIALOG ) == EAknSoftkeyOk)
	    				{
	    				iContextMenuAlreadyShown = ETrue;
	    				}
            		}
                 else if(sources[selectedIndex]->Type() == KMceFileSource)
            		{
            		CCreateAudioFileSourceDialog* dialog = new ( ELeave )
                					 CCreateAudioFileSourceDialog(iStream, selectedIndex );
    				if (dialog->ExecuteLD( R_MCE_STREAM_FILESOURCE_AUDIO_DIALOG ) == EAknSoftkeyOk)
	    				{
	    				iContextMenuAlreadyShown = ETrue;
	    				}
            		}           	
                }
            else if ( iListBoxModel->SelectedAudioSinkIndex(selectedIndex) >= 0 )
                {
                const RPointerArray<CMCETestUIEngineSink>& sinks = 
            										iStream.SinksL();
            	if(sinks[iListBoxModel->SelectedAudioSinkIndex(selectedIndex)]->Type() == KMceSpeakerSink)
                	{
                	CCreateSpeakerSinkDialog* dialog = new ( ELeave )
                					 CCreateSpeakerSinkDialog(iStream, iListBoxModel->SelectedAudioSinkIndex(selectedIndex) );
    				if (dialog->ExecuteLD( R_MCE_STREAM_SPEAKERSINK_DIALOG ) == EAknSoftkeyOk)
	    				{
	    				iContextMenuAlreadyShown = ETrue;
	    				}
                	}
          /*      else if(sinks[iListBoxModel->SelectedAudioSinkIndex(selectedIndex)]->Type() == KMCERTPSink)
                	{
                	CCreateRtpSinkDialog* dialog = new ( ELeave )
                					 CCreateRtpSinkDialog(iStream, iListBoxModel->SelectedAudioSinkIndex(selectedIndex) );
    				if (dialog->ExecuteLD( R_MCE_STREAM_RTPSINK_DIALOG ) == EAknSoftkeyOk)
	    				{
	    				iContextMenuAlreadyShown = ETrue;
	    				}
                	}
            */    
                }
            else if ( iListBoxModel->SelectedAudioCodecIndex(selectedIndex) >= 0 )
                {
                CCreateAudioCodecDialog* dialog = new ( ELeave )
                					 CCreateAudioCodecDialog(iStream, iListBoxModel->SelectedAudioCodecIndex(selectedIndex) );
    			if (dialog->ExecuteLD( R_MCE_STREAM_AUDIOCODEC_DIALOG ) == EAknSoftkeyOk)
    				{
    				iContextMenuAlreadyShown = ETrue;
    				}
                }
                
            // TODO: Add changes to event and refer views
            }
        }
   }


// -----------------------------------------------------------------------------
// CMCETestUIStreamViewContainer::Draw(const TRect& aRect) const
// -----------------------------------------------------------------------------
//
void CMCETestUIStreamViewContainer::Draw(const TRect& aRect ) const
{
    CWindowGc& gc = SystemGc();
    gc.Clear( aRect );    
}


// -----------------------------------------------------------------------------
// CMCETestUISessionViewContainer::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// -----------------------------------------------------------------------------
//
void CMCETestUIStreamViewContainer::HandleControlEventL(
        CCoeControl* /*aControl*/,
        TCoeEvent /*aEventType*/)
{
    // TODO: Add your control event handler code here
}


// -----------------------------------------------------------------------------
// CMCETestUISessionViewContainer::OfferKeyEventL(const TKeyEvent& aKeyEvent,
//                                    TEventCode aType)
// Handle key event. Only up and down key arrow events are
// consumed in order to enable scrolling in output window.
// -----------------------------------------------------------------------------
//
TKeyResponse CMCETestUIStreamViewContainer::OfferKeyEventL(
        const TKeyEvent& aKeyEvent,
        TEventCode aType)
    {    
    if ( aType == EEventKeyUp )
        {
        
        if ( aKeyEvent.iCode == 0 && aKeyEvent.iScanCode == 164 ) // EAknSoftkeyOptions?
            {
            DoShowMenuL();

            return EKeyWasConsumed;
            }
        }
    return iListBox->OfferKeyEventL( aKeyEvent, aType );
   
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMCETestUIStreamViewContainer::DoShowMenuL()
    {
    if ( iContextMenuAlreadyShown )
        {
        iContextMenuAlreadyShown = EFalse;
        return;
        }
        
    // Get command provider
    MMCETestUIEngineCmdProvider& cmdProvider = 
        iListBoxModel->CmdProvider( iListBox->CurrentItemIndex() );
    
    // Get commands    
    const RPointerArray<TMCETestUIEngineCmdBase>& commands = 
        cmdProvider.GetCommandsL();
        
    if(commands.Count() == 0)
        {
        return;    
        }
      

    // Generate list
    CAknSinglePopupMenuStyleListBox* listBox = 
            new (ELeave) CAknSinglePopupMenuStyleListBox;   
    CleanupStack::PushL( listBox );
    
    CAknPopupList* popup = CAknPopupList::NewL( 
            listBox, 
            R_AVKON_SOFTKEYS_MENU_LIST,
            AknPopupLayouts::EPopupSNotePopupWindow );
    CleanupStack::PushL( popup );
    popup->SetTitleL( _L("Available commands") );
    
    listBox->ConstructL( popup, 
        EAknListBoxSelectionList | EAknListBoxLoopScrolling);
    
    listBox->CreateScrollBarFrameL( ETrue );
    listBox->ScrollBarFrame()->SetScrollBarVisibilityL(
        CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto );
    
    CDesCArray* array = new (ELeave) CDesCArrayFlat( 1 );
    CleanupStack::PushL( array );
    
    for ( TInt i = 0; i < commands.Count(); ++i )
        {
        TInt itemLength = commands[i]->Caption().Length() + 
                          KTab().Length();
        HBufC* item = HBufC::NewLC( itemLength );
        TPtr itemPtr = item->Des();
        itemPtr.Append( commands[i]->Caption() );
        itemPtr.Append( KTab );

        array->AppendL( *item );

        CleanupStack::PopAndDestroy( item );
        item = NULL;
        }
           
    CleanupStack::Pop( array );
    listBox->Model()->SetItemTextArray( array );            
    
    CleanupStack::Pop( popup ); 
    TInt selectedCommand = -1;             
    if ( popup->ExecuteLD() )
        {
        iContextMenuAlreadyShown = ETrue;
        selectedCommand = listBox->CurrentItemIndex();
        }

    CleanupStack::PopAndDestroy( listBox );

    if ( selectedCommand != -1 )
        {
        // This will not work...
        TRAPD( err, commands[ selectedCommand ]->ExecuteL() );
        if ( err != KErrNone )
            {
            CEikonEnv* eikonEnv = CEikonEnv::Static();
            CMCETestUIAppUi* tempUiPtr = (CMCETestUIAppUi*)eikonEnv->AppUi();
            TInt errorLength = KLeaveMessage().Length() + 10;
            HBufC* error = HBufC::NewL( errorLength );
            TPtr errorPtr = error->Des();
            errorPtr.Append( KLeaveMessage );
            errorPtr.AppendNum( err );
           // User::InfoPrint( *error );
            tempUiPtr->NotifyUser(*error);
            delete error;
            }
                          
        }
    }

// ================= MEMBER FUNCTIONS ==========================================

CMCETestUIVideoStreamViewContainer* CMCETestUIVideoStreamViewContainer::NewL(
        const TRect& aRect,
        CAknView& aOwningView,
        CMCETestUIEngine& aEngine,
        CMCETestUIEngineVideoStream& aStream )
    {
	CMCETestUIVideoStreamViewContainer* self = 
	        new(ELeave) CMCETestUIVideoStreamViewContainer( 
	                        aOwningView, 
	                        aEngine, 
	                        aStream );
	CleanupStack::PushL(self);
	self->ConstructL( aRect );
	CleanupStack::Pop(self);
	return self;    
    }

CMCETestUIVideoStreamViewContainer::CMCETestUIVideoStreamViewContainer( 
        CAknView& aOwningView, 
        CMCETestUIEngine& aEngine, 
        CMCETestUIEngineVideoStream& aStream )
    :iOwningView( aOwningView ),
    iEngine( aEngine ),
    iStream( aStream ),
    iContextMenuAlreadyShown( EFalse )
    {
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIVideoStreamViewContainer::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// -----------------------------------------------------------------------------
//
void CMCETestUIVideoStreamViewContainer::ConstructL(const TRect& aRect)
    {
    CreateWindowL();
    //Window().SetShadowDisabled(EFalse);

	iListBox = new (ELeave) CAknDoubleStyleListBox(); 
   
	iListBox->SetContainerWindowL(*this);
	iListBox->ConstructL( this, 
	                      EAknListBoxSelectionList | EAknListBoxLoopScrolling);
	iListBox->SetListBoxObserver(this);
	iListBox->SetMopParent(this);

    iListBoxModel = CMCETestUIVideoStreamViewModel::NewL( iEngine , iStream);   

    iListBox->Model()->SetItemTextArray( iListBoxModel );      
	
    SetRect(aRect);

    iListBox->SetRect(Rect());

	//Set up the listbox to use a scroll bar
	iListBox->CreateScrollBarFrameL(ETrue);
	iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
	    CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);

    ConstructModelL();
    
    ActivateL(); 
   
    //iListBox->HandleItemAdditionL();   
    }

// -----------------------------------------------------------------------------
// CMCETestUIVideoStreamViewContainer::~CMCETestUIVideoStreamViewContainer()
// Destructor
// Frees reserved resources
// -----------------------------------------------------------------------------
//
CMCETestUIVideoStreamViewContainer::~CMCETestUIVideoStreamViewContainer()
    { 
    delete iListBox;
    }

// -----------------------------------------------------------------------------
// CMCETestUIVideoStreamViewContainer::~CMCETestUIStreamViewContainer()
// Destructor
// Frees reserved resources
// -----------------------------------------------------------------------------
//
void CMCETestUIVideoStreamViewContainer::ConstructModelL()
    {
    iListBoxModel = CMCETestUIVideoStreamViewModel::NewL( iEngine, iStream );   

    iListBox->Model()->SetItemTextArray( iListBoxModel ); 
    
    iListBox->Reset();      
    }

// -----------------------------------------------------------------------------
// CMCETestUIVideoStreamViewContainer::SizeChanged()
// Called by framework when the view size is changed
// -----------------------------------------------------------------------------
//
void CMCETestUIVideoStreamViewContainer::SizeChanged()
    {
    TInt error;
    
    // TRAP methods that can leave.
    TRAP(error, SizeChangedL());
    if(error != KErrNone)
        {
        User::Panic( KNullDesC16, 0 );
        }   
    }

// -----------------------------------------------------------------------------
// CMCETestUISessionViewContainer::SizeChangedL()
// Methods that can leave and are needed by SizeChanged
// are collected into this method.
// -----------------------------------------------------------------------------
//
void CMCETestUIVideoStreamViewContainer::SizeChangedL()
    {
  
    }


// -----------------------------------------------------------------------------
// CMCETestUIVideoStreamViewContainer::CountComponentControls() const
// Note! Output window must be ignored.
// -----------------------------------------------------------------------------
//
TInt CMCETestUIVideoStreamViewContainer::CountComponentControls() const
    {
    if ( iListBox ) 
        return 1;
    return 0;
    }


// -----------------------------------------------------------------------------
// CMCETestUIVideoStreamViewContainer::ComponentControl()
// -----------------------------------------------------------------------------
//
CCoeControl* CMCETestUIVideoStreamViewContainer::ComponentControl(TInt /*aIndex*/) const
    {
    return iListBox;
    }

// -----------------------------------------------------------------------------
// CMCETestUIVideoStreamViewContainer::HandleListBoxEventL()
// -----------------------------------------------------------------------------
//
void CMCETestUIVideoStreamViewContainer::HandleListBoxEventL( 
                CEikListBox* aListBox, 
                TListBoxEvent aEventType )
    {
    TInt selectedIndex = iListBox->CurrentItemIndex();
    iContextMenuAlreadyShown = EFalse;
    
    if ( aListBox == iListBox )
        {
        if ( aEventType == EEventEnterKeyPressed || aEventType == EEventItemDoubleClicked )
            {
            if ( iListBoxModel->IsVideoSourceSelected( selectedIndex ) )
                {
                // Change to manager view 
                   const RPointerArray<CMCETestUIEngineVideoSource>& sources = 
            										iStream.SourcesL();
            	if(sources[selectedIndex]->Type() == KMceCameraSource)
            		{
            		CCreateCameraSourceDialog* dialog = new ( ELeave )
                					 CCreateCameraSourceDialog(iStream, selectedIndex );
    				if (dialog->ExecuteLD( R_MCE_STREAM_CAMERASOURCE_DIALOG ) == EAknSoftkeyOk)
	    				{
	    				iContextMenuAlreadyShown = ETrue;
	    				}
            		}
            	else if(sources[selectedIndex]->Type() == KMceFileSource)
            		{
            		CCreateVideoFileSourceDialog* dialog = new ( ELeave )
                					 CCreateVideoFileSourceDialog(iStream, selectedIndex );
    				if (dialog->ExecuteLD( R_MCE_STREAM_FILESOURCE_VIDEO_DIALOG ) == EAknSoftkeyOk)
	    				{
	    				iContextMenuAlreadyShown = ETrue;
	    				}
            		}    
                }
            else if ( iListBoxModel->SelectedVideoSinkIndex(selectedIndex) >= 0 )
                {
                const RPointerArray<CMCETestUIEngineVideoSink>& sinks = 
            										iStream.SinksL();
            	if(sinks[iListBoxModel->SelectedVideoSinkIndex(selectedIndex)]->Type() == KMceSpeakerSink)
                	{
                /*	CCreateSpeakerSinkDialog* dialog = new ( ELeave )
                					 CCreateSpeakerSinkDialog(iStream, iListBoxModel->SelectedAudioSinkIndex(selectedIndex) );
    				if (dialog->ExecuteLD( R_MCE_STREAM_SPEAKERSINK_DIALOG ) == EAknSoftkeyOk)
	    				{
	    				iContextMenuAlreadyShown = ETrue;
	    				}*/
                	}
                else if((sinks[iListBoxModel->SelectedVideoSinkIndex(selectedIndex)]->Type() == KMceDisplaySink))
                	{
                	CCreateDisplaySinkDialog* dialog = new ( ELeave )
                					 CCreateDisplaySinkDialog(iStream, iListBoxModel->SelectedVideoSinkIndex(selectedIndex) );
    				if (dialog->ExecuteLD( R_MCE_STREAM_DISPLAYSINK_VIDEO_DIALOG ) == EAknSoftkeyOk)
	    				{
	    				iContextMenuAlreadyShown = ETrue;
	    				}
                	}
          
                }
            else if ( iListBoxModel->SelectedVideoCodecIndex(selectedIndex) >= 0 )
                {
                CCreateVideoCodecDialog* dialog = new ( ELeave )
                					 CCreateVideoCodecDialog(iStream, iListBoxModel->SelectedVideoCodecIndex(selectedIndex) );
    			if (dialog->ExecuteLD( R_MCE_STREAM_VIDEOCODEC_DIALOG ) == EAknSoftkeyOk)
    				{
    				iContextMenuAlreadyShown = ETrue;
    				}
                }
                
            // TODO: Add changes to event and refer views
            }
        }
   }


// -----------------------------------------------------------------------------
// CMCETestUIVideoStreamViewContainer::Draw(const TRect& aRect) const
// -----------------------------------------------------------------------------
//
void CMCETestUIVideoStreamViewContainer::Draw(const TRect& aRect ) const
{
    CWindowGc& gc = SystemGc();
    gc.Clear( aRect );    
}


// -----------------------------------------------------------------------------
// CMCETestUISessionViewContainer::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// -----------------------------------------------------------------------------
//
void CMCETestUIVideoStreamViewContainer::HandleControlEventL(
        CCoeControl* /*aControl*/,
        TCoeEvent /*aEventType*/)
{
    // TODO: Add your control event handler code here
}


// -----------------------------------------------------------------------------
// CMCETestUIVideoStreamViewContainer::OfferKeyEventL(const TKeyEvent& aKeyEvent,
//                                    TEventCode aType)
// Handle key event. Only up and down key arrow events are
// consumed in order to enable scrolling in output window.
// -----------------------------------------------------------------------------
//
TKeyResponse CMCETestUIVideoStreamViewContainer::OfferKeyEventL(
        const TKeyEvent& aKeyEvent,
        TEventCode aType)
    {    
    if ( aType == EEventKeyUp )
        {
        
        if ( aKeyEvent.iCode == 0 && aKeyEvent.iScanCode == 164 ) // EAknSoftkeyOptions?
            {
            DoShowMenuL();
            
            return EKeyWasConsumed;
            }
        }
    return iListBox->OfferKeyEventL( aKeyEvent, aType );   
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMCETestUIVideoStreamViewContainer::DoShowMenuL()
    {
    if ( iContextMenuAlreadyShown )
        {
        iContextMenuAlreadyShown = EFalse;
        return;
        }
        
    // Get command provider
    MMCETestUIEngineCmdProvider& cmdProvider = 
        iListBoxModel->CmdProvider( iListBox->CurrentItemIndex() );
    
    // Get commands    
    const RPointerArray<TMCETestUIEngineCmdBase>& commands = 
        cmdProvider.GetCommandsL();
        
    if(commands.Count() == 0)
        {
        return;    
        }
      
    
    // Generate list
    CAknSinglePopupMenuStyleListBox* listBox = 
            new (ELeave) CAknSinglePopupMenuStyleListBox;   
    CleanupStack::PushL( listBox );
    
    CAknPopupList* popup = CAknPopupList::NewL( 
            listBox, 
            R_AVKON_SOFTKEYS_MENU_LIST,
            AknPopupLayouts::EPopupSNotePopupWindow );
    CleanupStack::PushL( popup );
    popup->SetTitleL( _L("Available commands") );
    
    listBox->ConstructL( popup, 
        EAknListBoxSelectionList | EAknListBoxLoopScrolling);
    
    listBox->CreateScrollBarFrameL( ETrue );
    listBox->ScrollBarFrame()->SetScrollBarVisibilityL(
        CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto );
    
    CDesCArray* array = new (ELeave) CDesCArrayFlat( 1 );
    CleanupStack::PushL( array );
    
    for ( TInt i = 0; i < commands.Count(); ++i )
        {
        TInt itemLength = commands[i]->Caption().Length() + 
                          KTab().Length();
        HBufC* item = HBufC::NewLC( itemLength );
        TPtr itemPtr = item->Des();
        itemPtr.Append( commands[i]->Caption() );
        itemPtr.Append( KTab );
    
        array->AppendL( *item );
    
        CleanupStack::PopAndDestroy( item );
        item = NULL;
        }
           
    CleanupStack::Pop( array );
    listBox->Model()->SetItemTextArray( array );            
    
    CleanupStack::Pop( popup ); 
    TInt selectedCommand = -1;             
    if ( popup->ExecuteLD() )
        {
        iContextMenuAlreadyShown = ETrue;
        selectedCommand = listBox->CurrentItemIndex();
        }
    
    CleanupStack::PopAndDestroy( listBox );
    
    if ( selectedCommand != -1 )
        {
        // This will not work...
        TRAPD( err, commands[ selectedCommand ]->ExecuteL() );
        if ( err != KErrNone )
            {
            CEikonEnv* eikonEnv = CEikonEnv::Static();
            CMCETestUIAppUi* tempUiPtr = (CMCETestUIAppUi*)eikonEnv->AppUi();
            TInt errorLength = KLeaveMessage().Length() + 10;
            HBufC* error = HBufC::NewL( errorLength );
            TPtr errorPtr = error->Des();
            errorPtr.Append( KLeaveMessage );
            errorPtr.AppendNum( err );
           // User::InfoPrint( *error );
            tempUiPtr->NotifyUser(*error);
            delete error;
            }
                          
        }
    }

// End of File  
