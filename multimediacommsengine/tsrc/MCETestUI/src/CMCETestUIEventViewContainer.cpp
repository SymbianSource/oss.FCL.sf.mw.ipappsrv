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
#include "CMCETestUIEventView.h"
#include "CMCETestUIEventViewContainer.h"
#include "CMCETestUIEventViewModel.h"
#include "CMCETestUIEngine.h"
#include "CMCETestUIEngineProfile.h"
#include "CMCETestUIEngineEvent.h"
#include "TMCETestUIEngineCmdBase.h"
#include <aknviewappui.h>
#include <aknview.h>
#include <aknpopup.h>



// CONSTANTS
_LIT( KTab, "\t" );
_LIT( KLeaveMessage, "Command left with value: " );

// ================= MEMBER FUNCTIONS ==========================================

CMCETestUIEventViewContainer* CMCETestUIEventViewContainer::NewL(
        const TRect& aRect,
        CMCETestUIEventView& aOwningView,
        CMCETestUIEngine& aEngine,
        CMCETestUIEngineEvent& aSession )
    {
	CMCETestUIEventViewContainer* self = 
	        new(ELeave) CMCETestUIEventViewContainer( 
	                        aOwningView, 
	                        aEngine, 
	                        aEvent );
	CleanupStack::PushL(self);
	self->ConstructL( aRect );
	CleanupStack::Pop(self);
	return self;    
    }

CMCETestUIEventViewContainer::CMCETestUIEventViewContainer( 
        CMCETestUIEventView& aOwningView, 
        CMCETestUIEngine& aEngine, 
        CMCETestUIEngineEvent& aEvent )
    :iOwningView( aOwningView ),
    iEngine( aEngine ),
    iEvent( aEvent ),
    iContextMenuAlreadyShown( EFalse )
    {
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIEventViewContainer::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// -----------------------------------------------------------------------------
//
void CMCETestUIEventViewContainer::ConstructL(const TRect& aRect)
    {
    CreateWindowL();
    //Window().SetShadowDisabled(EFalse);

	iListBox = new (ELeave) CAknDoubleStyleListBox(); 
   
	iListBox->SetContainerWindowL(*this);
	iListBox->ConstructL( this, 
	                      EAknListBoxSelectionList | EAknListBoxLoopScrolling);
	iListBox->SetListBoxObserver(this);
	iListBox->SetMopParent(this);

    iListBoxModel = CMCETestUIEventViewModel::NewL( iEngine, iEvent );   

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
// CMCETestUIEventViewContainer::~CMCETestUIEventViewContainer()
// Destructor
// Frees reserved resources
// -----------------------------------------------------------------------------
//
CMCETestUIEventViewContainer::~CMCETestUIEventViewContainer()
    {
    delete iListBox;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEventViewContainer::~CMCETestUISessionViewContainer()
// Destructor
// Frees reserved resources
// -----------------------------------------------------------------------------
//
void CMCETestUIEventViewContainer::ConstructModelL()
    {
    iListBoxModel = CMCETestUIEventViewModel::NewL( iEngine, iEvent );   

    iListBox->Model()->SetItemTextArray( iListBoxModel ); 
    
    iListBox->Reset();      
    }

// -----------------------------------------------------------------------------
// CMCETestUIEventViewContainer::SizeChanged()
// Called by framework when the view size is changed
// -----------------------------------------------------------------------------
//
void CMCETestUIEventViewContainer::SizeChanged()
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
// CMCETestUIEventViewContainer::SizeChangedL()
// Methods that can leave and are needed by SizeChanged
// are collected into this method.
// -----------------------------------------------------------------------------
//
void CMCETestUIEventViewContainer::SizeChangedL()
    {
  
    }


// -----------------------------------------------------------------------------
// CMCETestUIEventViewContainer::CountComponentControls() const
// Note! Output window must be ignored.
// -----------------------------------------------------------------------------
//
TInt CMCETestUIEventViewContainer::CountComponentControls() const
    {
    if ( iListBox ) 
        return 1;
    return 0;
    }


// -----------------------------------------------------------------------------
// CMCETestUIEventViewContainer::ComponentControl()
// -----------------------------------------------------------------------------
//
CCoeControl* CMCETestUIEventViewContainer::ComponentControl(TInt /*aIndex*/) const
    {
    return iListBox;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEventViewContainer::HandleListBoxEventL()
// -----------------------------------------------------------------------------
//
void CMCETestUIEventViewContainer::HandleListBoxEventL( 
                CEikListBox*/* aListBox*/, 
                TListBoxEvent /*aEventType */)
    {
   
    }


// -----------------------------------------------------------------------------
// CMCETestUIEventViewContainer::Draw(const TRect& aRect) const
// -----------------------------------------------------------------------------
//
void CMCETestUIEventViewContainer::Draw(const TRect& aRect ) const
{
    CWindowGc& gc = SystemGc();
    gc.Clear( aRect );    
}


// -----------------------------------------------------------------------------
// CMCETestUIEventViewContainer::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// -----------------------------------------------------------------------------
//
void CMCETestUIEventViewContainer::HandleControlEventL(
        CCoeControl* /*aControl*/,
        TCoeEvent /*aEventType*/)
{
    // TODO: Add your control event handler code here
}


// -----------------------------------------------------------------------------
// CMCETestUIEventViewContainer::OfferKeyEventL(const TKeyEvent& aKeyEvent,
//                                    TEventCode aType)
// Handle key event. Only up and down key arrow events are
// consumed in order to enable scrolling in output window.
// -----------------------------------------------------------------------------
//
TKeyResponse CMCETestUIEventViewContainer::OfferKeyEventL(
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
void CMCETestUIEventViewContainer::DoShowMenuL()
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
