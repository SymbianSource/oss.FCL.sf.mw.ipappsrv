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




#include <eiklabel.h>
#include <eikbctrl.h>
#include <eiktxlbx.h>
#include <eiktxlbm.h>
#include <barsread.h>
#include <coemain.h>
#include <aknlists.h>
#include <AknNoteWrappers.h>
#include <eikgted.h>
#include <avkon.hrh>
#include "CMCETestUIAppView.h"
//#include "FCAppUi.h"
#include "MCETestUI.hrh"
#include <MCETestUI.rsg>
#include <eikrted.h>
#include <aknviewappui.h>
#include <aknglobalnote.h>
#include <eikbtgpc.h>
#include <akntitle.h>   // for CAknTitlePane

//const TInt KMaxLineLength = 60; // Length of line in list
//const TInt KGranularityOfArray = 6;

// CONSTANTS

_LIT( KMCETestUI, "MCETestUI" );


CMCETestUIAppView* CMCETestUIAppView::NewL( CMCETestUIEngine& aEngine )
	{
	CMCETestUIAppView* self = new(ELeave) CMCETestUIAppView( aEngine );
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CMCETestUIAppView::CMCETestUIAppView( CMCETestUIEngine& aEngine )
    : iEngine( aEngine )
	{
	}

//
// Destructor for the view.
//
CMCETestUIAppView::~CMCETestUIAppView()
	{
	if (iContainer)
		{
		AppUi()->RemoveFromViewStack( *this, iContainer );
		//AppUi()->RemoveFromStack( iContainer );
		delete iContainer;
		iContainer = NULL;
		}
	}

// -----------------------------------------------------------------------------
// CMCETestUIAppView::EngineStateChangedL()
// -----------------------------------------------------------------------------
//
void CMCETestUIAppView::EngineStateChangedL()
    {
    if ( iContainer )
        {
        iContainer->ConstructModelL();
        iContainer->DrawNow();
        }
    }

// -----------------------------------------------------------------------------
// CMCETestUIAppView::ChangeToSessionViewL()
// -----------------------------------------------------------------------------
//
void CMCETestUIAppView::ChangeToSessionViewL( TInt aSessionIndex )
    {
    TUid viewId = TUid::Uid( EMCETestUISessionViewId );
    TUid customMessageId = TUid::Uid( EMCETestUISessionId );
    
    TBuf8<10> sessionIndex; 
    sessionIndex.AppendNum( aSessionIndex );
    TRAPD( err, AppUi()->ActivateLocalViewL( viewId, 
                                             customMessageId, 
                                             sessionIndex ) );
    if ( err != KErrNone )
        {
        User::InfoPrint( _L("Error") );
        }
        
    }

/**
* Called by the framework
* @return The Uid for this view
*/
TUid CMCETestUIAppView::Id() const 
    {
    return TUid::Uid( EMCETestUIAppViewId );
    }


/**
* From CEikAppUi, takes care of command handling for this view.
*
* @param aCommand command to be handled
*/
void CMCETestUIAppView::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case EAknSoftkeyBack:
            {
            AppUi()->HandleCommandL( EEikCmdExit );
            break;
            }
        case EAknSoftkeyOptions:
            {
            if ( iContainer ){
                iContainer->DoShowMenuL();
                }
            break;
            }
        default:
            {
            }
        }
	}

/**
* Called by the framework when the view is activated.  Constructs the 
* container if necessary, setting this view as its MOP parent, and 
* adding it to the control stack.
*/
void CMCETestUIAppView::DoActivateL(
        const TVwsViewId& /*aPrevViewId*/,
        TUid /*aCustomMessageId*/,
        const TDesC8& /*aCustomMessage*/ )
    {
	if (!iContainer)
		{
		iContainer = CMCETestUIMainViewContainer::NewL(
		    AppUi()->ClientRect(), *this, iEngine );
		iContainer->SetMopParent(this);
		//iContainer->ConstructL(AppUi()->ClientRect());
		AppUi()->AddToViewStackL( *this, iContainer );
		//AppUi()->AddToStackL(*this, iContainer);
		}
    Cba()->SetCommandSetL( R_AVKON_SOFTKEYS_OPTIONS_EXIT );	
    Cba()->DrawNow();
     
    CEikStatusPane* statusPane = StatusPane();
    CAknTitlePane* titlePane = static_cast<CAknTitlePane*>
        (StatusPane()->ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ));
    titlePane->SetTextL( KMCETestUI );
        
    }

/**
* Called by the framework when the view is deactivated.  
* Removes the container from the control stack and deletes it.
*/
void CMCETestUIAppView::DoDeactivate()
    {
	if (iContainer)
		{
		AppUi()->RemoveFromViewStack( *this, iContainer );
		//AppUi()->RemoveFromStack(iContainer);
		delete iContainer;
		iContainer = NULL;
		}
    }

//
// Second phase construction.
//
void CMCETestUIAppView::ConstructL()
    {
    BaseConstructL();
	}

