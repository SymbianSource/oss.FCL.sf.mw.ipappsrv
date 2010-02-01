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
#include "CMCETestUISessionView.h"
#include "CMCETestUISessionViewContainer.h"
#include "CMCETestUISessionViewModel.h"
#include "CMCETestUIEngine.h"
#include "CMCETestUIEngineProfile.h"
#include "CMCETestUIEngineSession.h"
#include "TMCETestUIEngineCmdBase.h"
#include <aknviewappui.h>
#include <aknview.h>
#include <aknpopup.h>



// CONSTANTS


// ================= MEMBER FUNCTIONS ==========================================

CMCETestUISessionViewContainer* CMCETestUISessionViewContainer::NewL(
        const TRect& aRect,
        CMCETestUISessionView& aOwningView,
        CMCETestUIEngine& aEngine,
        CMCETestUIEngineSession& aSession )
    {
	CMCETestUISessionViewContainer* self = 
	        new(ELeave) CMCETestUISessionViewContainer( 
	                        aOwningView, 
	                        aEngine, 
	                        aSession );
	CleanupStack::PushL(self);
	self->ConstructL( aRect );
	CleanupStack::Pop(self);
	return self;    
    }

CMCETestUISessionViewContainer::CMCETestUISessionViewContainer( 
        CMCETestUISessionView& aOwningView, 
        CMCETestUIEngine& aEngine, 
        CMCETestUIEngineSession& aSession )
    :iOwningView( aOwningView ),
    iEngine( aEngine ),
    iSession( aSession ),
    iContextMenuAlreadyShown( EFalse )
    {
    }
    
// -----------------------------------------------------------------------------
// CMCETestUISessionViewContainer::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// -----------------------------------------------------------------------------
//
void CMCETestUISessionViewContainer::ConstructL(const TRect& aRect)
    {
    CreateWindowL();
    //Window().SetShadowDisabled(EFalse);

	iListBox = new (ELeave) CAknDoubleStyleListBox(); 
   
	iListBox->SetContainerWindowL(*this);
	iListBox->ConstructL( this, 
	                      EAknListBoxSelectionList | EAknListBoxLoopScrolling);
	iListBox->SetListBoxObserver(this);
	iListBox->SetMopParent(this);

    iListBoxModel = CMCETestUISessionViewModel::NewL( iEngine, iSession );   

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
CMCETestUISessionViewContainer::~CMCETestUISessionViewContainer()
    {
    delete iListBox;
    }

// -----------------------------------------------------------------------------
// CMCETestUISessionViewContainer::~CMCETestUISessionViewContainer()
// Destructor
// Frees reserved resources
// -----------------------------------------------------------------------------
//
void CMCETestUISessionViewContainer::ConstructModelL()
    {
    iListBoxModel = CMCETestUISessionViewModel::NewL( iEngine, iSession );   

    iListBox->Model()->SetItemTextArray( iListBoxModel ); 
    
    iListBox->Reset();      
    }

// -----------------------------------------------------------------------------
// CMCETestUISessionViewContainer::SizeChanged()
// Called by framework when the view size is changed
// -----------------------------------------------------------------------------
//
void CMCETestUISessionViewContainer::SizeChanged()
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
void CMCETestUISessionViewContainer::SizeChangedL()
    {
  
    }


// -----------------------------------------------------------------------------
// CMCETestUISessionViewContainer::CountComponentControls() const
// Note! Output window must be ignored.
// -----------------------------------------------------------------------------
//
TInt CMCETestUISessionViewContainer::CountComponentControls() const
    {
    if ( iListBox ) 
        return 1;
    return 0;
    }


// -----------------------------------------------------------------------------
// CMCETestUISessionViewContainer::ComponentControl()
// -----------------------------------------------------------------------------
//
CCoeControl* CMCETestUISessionViewContainer::ComponentControl(TInt /*aIndex*/) const
    {
    return iListBox;
    }

// -----------------------------------------------------------------------------
// CMCETestUISessionViewContainer::HandleListBoxEventL()
// -----------------------------------------------------------------------------
//
void CMCETestUISessionViewContainer::HandleListBoxEventL( 
                CEikListBox* aListBox, 
                TListBoxEvent aEventType )
    {
    TInt selectedIndex = iListBox->CurrentItemIndex();
    CEikonEnv* eikonEnv = CEikonEnv::Static();
	CMCETestUIAppUi* tempUiPtr = (CMCETestUIAppUi*)eikonEnv->AppUi();
    
    tempUiPtr->iCurrentStreamViewIndex = KErrNone;
    if ( aListBox == iListBox )
        {
        if ( aEventType == EEventEnterKeyPressed || aEventType == EEventItemDoubleClicked )
            {
            
            iOwningView.ChangeToStreamViewL(selectedIndex);
            tempUiPtr->iCurrentStreamViewIndex = selectedIndex;
            // TODO: Add changes to event and refer views
            }
        }
    }


// -----------------------------------------------------------------------------
// CMCETestUISessionViewContainer::Draw(const TRect& aRect) const
// -----------------------------------------------------------------------------
//
void CMCETestUISessionViewContainer::Draw(const TRect& aRect ) const
{
    CWindowGc& gc = SystemGc();
    gc.Clear( aRect );    
}


// -----------------------------------------------------------------------------
// CMCETestUISessionViewContainer::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// -----------------------------------------------------------------------------
//
void CMCETestUISessionViewContainer::HandleControlEventL(
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
TKeyResponse CMCETestUISessionViewContainer::OfferKeyEventL(
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
void CMCETestUISessionViewContainer::DoShowMenuL()
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
