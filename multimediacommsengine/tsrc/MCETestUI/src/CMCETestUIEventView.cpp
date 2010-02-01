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



#include "MCETestUI.hrh"
#include "CMCETestUIEventView.h"
#include "CMCETestUIAppUi.h"
#include "CMCETestUIEngineOutEvent.h"
#include "CMCETestUIENgineInEvent.h"

#include <e32def.h>
#include <barsread.h>
#include <AknNoteWrappers.h>
#include <aknviewappui.h>
#include <eikbtgpc.h>
#include <akntitle.h>   // for CAknTitlePane
#include <aknnavi.h>    // for CAknNavigationControlContainer
#include <aknnavide.h>  // for CAknNavigationDecorator

// CONSTANTS

_LIT( KEvent, "Event" );
_LIT( KOutEvent, "OutEvent: " );
_LIT( KInEvent, "InEvent: " );
_LIT( KLeftParenthesis, " (" );
_LIT( KRightParenthesis, ")" );

CMCETestUIEventView* CMCETestUIEventView::NewL( CMCETestUIEngine& aEngine )
	{
	CMCETestUIEventView* self = new(ELeave) CMCETestUIEventView( aEngine );
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CMCETestUIEventView::CMCETestUIEventView( CMCETestUIEngine& aEngine )
    : iEngine( aEngine )
	{
	}

//
// Destructor for the view.
//
CMCETestUIEventView::~CMCETestUIEventView()
	{
	if (iContainer)
		{
		AppUi()->RemoveFromViewStack( *this, iContainer);
		delete iContainer;
		iContainer = NULL;
		}
	}

// -----------------------------------------------------------------------------
// CMCETestUIEventView::EngineStateChangedL()
// -----------------------------------------------------------------------------
//
void CMCETestUIEventView::EngineStateChangedL()
    {
    if ( iContainer )
        {
        iContainer->ConstructModelL();
        iContainer->DrawNow();
        
        ConstructStatusPaneL();
        }
    }

// -----------------------------------------------------------------------------
// CMCETestUIEventView::ChangeToMainViewL()
// -----------------------------------------------------------------------------
//
void CMCETestUIEventView::ChangeToMainViewL()
    {
    TUid viewId = TUid::Uid( EMCETestUIAppViewId );
    TRAPD( err, AppUi()->ActivateLocalViewL( viewId ) );
    if ( err != KErrNone )
        {
        User::InfoPrint( _L("Error") );
        }
        
    }

/**
* Called by the framework
* @return The Uid for this view
*/
TUid CMCETestUIEventView::Id() const 
    {
    return TUid::Uid( EMCETestUIEventViewId );
    }


/**
* From CEikAppUi, takes care of command handling for this view.
*
* @param aCommand command to be handled
*/
void CMCETestUIEventView::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case EAknSoftkeyBack:
            {
            ChangeToMainViewL();
            break;
            }
        default:
            {
            }
        }
	}


// -----------------------------------------------------------------------------
// CMCETestUIEventView::DoActivateL
//
// Called by the framework when the view is activated.  Constructs the 
// container if necessary, setting this view as its MOP parent, and 
// adding it to the control stack.
//
// -----------------------------------------------------------------------------

void CMCETestUIEventView::DoActivateL(
        const TVwsViewId& /*aPrevViewId*/,
        TUid /*aCustomMessageId*/,
        const TDesC8& aCustomMessage )
    {
	if (!iContainer)
		{
		// Solve event to be shown
		TLex8 lex( aCustomMessage );
	    TInt eventIndex = -1; 
        User::LeaveIfError( lex.Val( eventIndex ) );
        
        const RPointerArray<CMCETestUIEngineOutEvent>& outEvents = 
                    iEngine.OutEvents();               
                    
        const RPointerArray<CMCETestUIEngineInEvent>& inEvents = 
                    iEngine.InEvents(); 
        
        if ( eventIndex  < outEvents .Count() )
            {
            iEvent = outEvents[eventIndex ];
            }
        else
            {
            eventIndex -= outEvents.Count();
            if ( eventIndex < inEvents.Count() )
                {  
                iEvent = inEvents[eventIndex];
                }
            else
                {
                User::Leave( KErrArgument );
                }
            }
		
		iContainer = CMCETestUIEventViewContainer::NewL(
		    AppUi()->ClientRect(), *this, iEngine, *iEvent );
		iContainer->SetMopParent(this);
		//iContainer->ConstructL(AppUi()->ClientRect());
		AppUi()->AddToViewStackL( *this, iContainer );
		}
    
         
	Cba()->SetCommandSetL( R_AVKON_SOFTKEYS_OPTIONS_BACK );  
	Cba()->DrawNow();  
    
    ConstructStatusPaneL();    
    			  
    }


// -----------------------------------------------------------------------------
// CMCETestUIEventView::DoDeactivate
//
// Called by the framework when the view is deactivated.  
// Removes the container from the control stack and deletes it.
//
// -----------------------------------------------------------------------------
void CMCETestUIEventView::DoDeactivate()
    {
	if (iContainer)
		{
		// TODO: This should not leave
        CAknNavigationControlContainer* navi = 
            static_cast<CAknNavigationControlContainer*>
                (StatusPane()->ControlL( TUid::Uid( EEikStatusPaneUidNavi ) ));
        navi->Pop( iNaviLabel );
        delete iNaviLabel;
        iNaviLabel = NULL;	
        	
		AppUi()->RemoveFromViewStack( *this, iContainer );
		delete iContainer;
		iContainer = NULL;
		}
    }

// -----------------------------------------------------------------------------
// CMCETestUIEventView::ConstructStatusPaneL
// -----------------------------------------------------------------------------
//
void CMCETestUIEventView::ConstructStatusPaneL()
    {
    CAknTitlePane* titlePane = static_cast<CAknTitlePane*>
        (StatusPane()->ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ));
    titlePane->SetTextL( iTitlePaneText );
    
    CAknNavigationControlContainer* navi = 
            static_cast<CAknNavigationControlContainer*>
                (StatusPane()->ControlL( TUid::Uid( EEikStatusPaneUidNavi ) ));
    navi->Pop( iNaviLabel );
    delete iNaviLabel;
    iNaviLabel = NULL;
    iNaviLabel = navi->CreateNavigationLabelL( iEvent->State() );
    navi->PushL( *iNaviLabel );    
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIEventView::ConstructL
// -----------------------------------------------------------------------------
//
void CMCETestUIEventView::ConstructL()
    {
    BaseConstructL(); 
	}
