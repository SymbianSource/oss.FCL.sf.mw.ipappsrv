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
#include "CMCETestUISessionView.h"
#include "CMCETestUIAppUi.h"
#include "CMCETestUIEngineOutSession.h"
#include "CMCETestUIENgineInSession.h"

#include <e32def.h>
#include <barsread.h>
#include <AknNoteWrappers.h>
#include <aknviewappui.h>
#include <eikbtgpc.h>
#include <akntitle.h>   // for CAknTitlePane
#include <aknnavi.h>    // for CAknNavigationControlContainer
#include <aknnavide.h>  // for CAknNavigationDecorator

// CONSTANTS


CMCETestUISessionView* CMCETestUISessionView::NewL( CMCETestUIEngine& aEngine )
	{
	CMCETestUISessionView* self = new(ELeave) CMCETestUISessionView( aEngine );
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CMCETestUISessionView::CMCETestUISessionView( CMCETestUIEngine& aEngine )
    : iEngine( aEngine )
	{
	}

//
// Destructor for the view.
//
CMCETestUISessionView::~CMCETestUISessionView()
	{
	if (iContainer)
		{
		AppUi()->RemoveFromViewStack( *this, iContainer);
		delete iContainer;
		iContainer = NULL;
		}
	}

// -----------------------------------------------------------------------------
// CMCETestUISessionView::EngineStateChangedL()
// -----------------------------------------------------------------------------
//
void CMCETestUISessionView::EngineStateChangedL()
    {
    if ( iContainer )
        {
        iContainer->ConstructModelL();
        iContainer->DrawNow();
        
        ConstructStatusPaneL();
        }
    }

// -----------------------------------------------------------------------------
// CMCETestUISessionView::ChangeToMainViewL()
// -----------------------------------------------------------------------------
//
void CMCETestUISessionView::ChangeToMainViewL()
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
TUid CMCETestUISessionView::Id() const 
    {
    return TUid::Uid( EMCETestUISessionViewId );
    }


/**
* From CEikAppUi, takes care of command handling for this view.
*
* @param aCommand command to be handled
*/
void CMCETestUISessionView::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case EAknSoftkeyBack:
            {
            ChangeToMainViewL();
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


// -----------------------------------------------------------------------------
// CMCETestUISessionView::DoActivateL
//
// Called by the framework when the view is activated.  Constructs the 
// container if necessary, setting this view as its MOP parent, and 
// adding it to the control stack.
//
// -----------------------------------------------------------------------------

void CMCETestUISessionView::DoActivateL(
        const TVwsViewId& /*aPrevViewId*/,
        TUid /*aCustomMessageId*/,
        const TDesC8& aCustomMessage )
    {
	if (!iContainer)
		{
		// Solve session to be shown
		TLex8 lex( aCustomMessage );
	    TInt sessionIndex = -1; 
        User::LeaveIfError( lex.Val( sessionIndex ) );
        
        const RPointerArray<CMCETestUIEngineOutSession>& outSessions = 
                    iEngine.OutSessions();               
                    
        const RPointerArray<CMCETestUIEngineInSession>& inSessions = 
                    iEngine.InSessions(); 
        
        if ( sessionIndex < outSessions.Count() )
            {
            iSession = outSessions[sessionIndex];
            iTitlePaneText.Copy(KOutSession);
            iTitlePaneText.Append(KLeftParenthesis);
            iTitlePaneText.AppendNum(iSession->Id());
            iTitlePaneText.Append(KRightParenthesis);
            }
        else
            {
            sessionIndex -= outSessions.Count();
            if ( sessionIndex < inSessions.Count() )
                {  
                iSession = inSessions[sessionIndex];
                iTitlePaneText.Copy(KInSession);
	            iTitlePaneText.Append(KLeftParenthesis);
	            iTitlePaneText.AppendNum(iSession->Id());
	            iTitlePaneText.Append(KRightParenthesis);
                }
            else
                {
                User::Leave( KErrArgument );
                }
            }
		
		iContainer = CMCETestUISessionViewContainer::NewL(
		    AppUi()->ClientRect(), *this, iEngine, *iSession );
		iContainer->SetMopParent(this);
		//iContainer->ConstructL(AppUi()->ClientRect());
		AppUi()->AddToViewStackL( *this, iContainer );
		}
    
         
	Cba()->SetCommandSetL( R_AVKON_SOFTKEYS_OPTIONS_BACK );  
	Cba()->DrawNow();  
    
    ConstructStatusPaneL();    
    			  
    }


// -----------------------------------------------------------------------------
// CMCETestUISessionView::DoDeactivate
//
// Called by the framework when the view is deactivated.  
// Removes the container from the control stack and deletes it.
//
// -----------------------------------------------------------------------------
void CMCETestUISessionView::DoDeactivate()
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
// CMCETestUISessionView::ConstructStatusPaneL
// -----------------------------------------------------------------------------
//
void CMCETestUISessionView::ConstructStatusPaneL()
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
    iNaviLabel = navi->CreateNavigationLabelL( iSession->State() );
    navi->PushL( *iNaviLabel );    
    }
    
// -----------------------------------------------------------------------------
// CMCETestUISessionView::ConstructL
// -----------------------------------------------------------------------------
//
void CMCETestUISessionView::ConstructL()
    {
    BaseConstructL(); 
	}

// -----------------------------------------------------------------------------
// CMCETestUISessionView::ChangeToStreamViewL
// -----------------------------------------------------------------------------
//
void CMCETestUISessionView::ChangeToStreamViewL( TInt aStreamIndex )
    {
    TUid viewId = TUid::Uid( EMCETestUIStreamViewId );
    TUid customMessageId = TUid::Uid( EMCETestUIStreamId );
    
    TBuf8<10> streamIndex; 
    streamIndex.AppendNum( aStreamIndex );
    TRAPD( err, AppUi()->ActivateLocalViewL( viewId, 
                                             customMessageId, 
                                             streamIndex ) );
    if ( err != KErrNone )
        {
        User::InfoPrint( _L("Error") );
        }   
    }
