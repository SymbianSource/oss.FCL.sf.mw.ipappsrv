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
#include "CMCETestUIAppView.h"
#include <aknpopup.h>
#include "CMCETestUIAppUi.h"
#include <aknviewappui.h>
#include "CMCETestUIMainViewContainer.h"
#include "CMCETestUIMainViewModel.h"
#include "CMCETestUIEngine.h"
#include "CMCETestUIEngineProfile.h"
#include "TMCETestUIEngineCmdBase.h"

// CONSTANTS

// ================= MEMBER FUNCTIONS ==========================================

CMCETestUIMainViewContainer* CMCETestUIMainViewContainer::NewL(
        const TRect& aRect,
        CMCETestUIAppView& aOwningView,
        CMCETestUIEngine& aEngine )
    {
	CMCETestUIMainViewContainer* self = 
	    new(ELeave) CMCETestUIMainViewContainer( aOwningView, aEngine );
	CleanupStack::PushL(self);
	self->ConstructL( aRect );
	CleanupStack::Pop(self);
	return self;    
    }

CMCETestUIMainViewContainer::CMCETestUIMainViewContainer( 
        CMCETestUIAppView& aOwningView, CMCETestUIEngine& aEngine )
    :iOwningView( aOwningView ),
    iEngine( aEngine ),
    iContextMenuAlreadyShown( EFalse )
    {
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIMainViewContainer::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// -----------------------------------------------------------------------------
//
void CMCETestUIMainViewContainer::ConstructL(const TRect& aRect)
    {
    CreateWindowL();

	iListBox = new (ELeave) CAknDoubleStyleListBox(); 
   
	iListBox->SetContainerWindowL(*this);
	iListBox->ConstructL( this, 
	                      EAknListBoxSelectionList | EAknListBoxLoopScrolling);
	iListBox->SetListBoxObserver(this);
	iListBox->SetMopParent(this);
	
    SetRect(aRect);

    iListBox->SetRect(Rect());

	//Set up the listbox to use a scroll bar
	iListBox->CreateScrollBarFrameL(ETrue);
	iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
	    CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);

    ConstructModelL();
    
    ActivateL(); 

    }

// -----------------------------------------------------------------------------
// CMCETestUIMainViewContainer::~CMCETestUIMainViewContainer()
// Destructor
// Frees reserved resources
// -----------------------------------------------------------------------------
//
CMCETestUIMainViewContainer::~CMCETestUIMainViewContainer()
    {
    delete iListBox;
    }

// -----------------------------------------------------------------------------
// CMCETestUIMainViewContainer::~CMCETestUIMainViewContainer()
// Destructor
// Frees reserved resources
// -----------------------------------------------------------------------------
//
void CMCETestUIMainViewContainer::ConstructModelL()
    {
    iListBoxModel = CMCETestUIMainViewModel::NewL( iEngine );   

    iListBox->Model()->SetItemTextArray( iListBoxModel ); 
    
    iListBox->Reset();      
    }

// -----------------------------------------------------------------------------
// CMCETestUIMainViewContainer::SizeChanged()
// Called by framework when the view size is changed
// -----------------------------------------------------------------------------
//
void CMCETestUIMainViewContainer::SizeChanged()
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
// CMCETestUIMainViewContainer::SizeChangedL()
// Methods that can leave and are needed by SizeChanged
// are collected into this method.
// -----------------------------------------------------------------------------
//
void CMCETestUIMainViewContainer::SizeChangedL()
    {
  
    }


// -----------------------------------------------------------------------------
// CMCETestUIMainViewContainer::CountComponentControls() const
// Note! Output window must be ignored.
// -----------------------------------------------------------------------------
//
TInt CMCETestUIMainViewContainer::CountComponentControls() const
    {
    if ( iListBox ) 
        return 1;
    return 0;
    }


// -----------------------------------------------------------------------------
// CMCETestUIMainViewContainer::ComponentControl()
// -----------------------------------------------------------------------------
//
CCoeControl* CMCETestUIMainViewContainer::ComponentControl(TInt /*aIndex*/) const
    {
    return iListBox;
    }

// -----------------------------------------------------------------------------
// CMCETestUIMainViewContainer::HandleListBoxEventL()
// -----------------------------------------------------------------------------
//
void CMCETestUIMainViewContainer::HandleListBoxEventL( 
                CEikListBox* aListBox, 
                TListBoxEvent aEventType )
    {
    TInt selectedIndex = iListBox->CurrentItemIndex();
    iContextMenuAlreadyShown = EFalse;
    CEikonEnv* eikonEnv = CEikonEnv::Static();
	CMCETestUIAppUi* tempUiPtr = (CMCETestUIAppUi*)eikonEnv->AppUi();
    tempUiPtr->iCurrentSessionViewIndex = KErrNone;
    
    if ( aListBox == iListBox )
        {
        if ( aEventType == EEventEnterKeyPressed || aEventType == EEventItemDoubleClicked )
            {
            if ( iListBoxModel->IsManagerSelected( selectedIndex ) )
                {
                 // Change to manager view 
               
                }
            else if ( iListBoxModel->SelectedProfileIndex(selectedIndex) >= 0 )
                {
                // Change to profile view
               
                }
            else if ( iListBoxModel->SelectedSessionIndex(selectedIndex) >= 0 )
                {
                iOwningView.ChangeToSessionViewL( 
                    iListBoxModel->SelectedSessionIndex( selectedIndex ) );
                tempUiPtr->iCurrentSessionViewIndex = 
                	iListBoxModel->SelectedSessionIndex( selectedIndex );    
                iContextMenuAlreadyShown = ETrue;	
                }
                
            // TODO: Add changes to event and refer views
            }
        }
    }


// -----------------------------------------------------------------------------
// CMCETestUIMainViewContainer::Draw(const TRect& aRect) const
// -----------------------------------------------------------------------------
//
void CMCETestUIMainViewContainer::Draw(const TRect& aRect ) const
{
    CWindowGc& gc = SystemGc();
    gc.Clear( aRect );    
}


// -----------------------------------------------------------------------------
// CMCETestUIMainViewContainer::OfferKeyEventL(const TKeyEvent& aKeyEvent,
//                                    TEventCode aType)
// Handle key event. Only up and down key arrow events are
// consumed in order to enable scrolling in output window.
// -----------------------------------------------------------------------------
//
TKeyResponse CMCETestUIMainViewContainer::OfferKeyEventL(
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
void CMCETestUIMainViewContainer::DoShowMenuL()
    {
    if ( iContextMenuAlreadyShown )
        {
        iContextMenuAlreadyShown = EFalse;
        return;
        }
    else
        {
        // Hide video if it is shown
        CMCETestUIAppUi* appui = static_cast<CMCETestUIAppUi*>( CEikonEnv::Static()->AppUi() );
        if ( appui->IsVideoEnabled() )
            {
            appui->DisableVideo();
            }
        }
        
    // Get command provider
    MMCETestUIEngineCmdProvider& cmdProvider = 
        iListBoxModel->CmdProvider( iListBox->CurrentItemIndex() );
    
    // Get commands    
    const RPointerArray<TMCETestUIEngineCmdBase>& commands = 
        cmdProvider.GetCommandsL();


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
