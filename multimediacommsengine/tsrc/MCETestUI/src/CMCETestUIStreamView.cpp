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
#include "CMCETestUIStreamView.h"

#include <e32def.h>
#include <barsread.h>
#include <AknNoteWrappers.h>
#include <aknviewappui.h>
#include <eikbtgpc.h>
#include <akntitle.h>   // for CAknTitlePane
#include <aknnavi.h>    // for CAknNavigationControlContainer
#include <aknnavide.h>  // for CAknNavigationDecorator
#include "CMCETestUIAppUi.h"
#include "CMCETestUiEngineAudioStream.h"
#include "CMCETestUiEngineVideoStream.h"
#include "CMCETestUiEngineOutSession.h"
#include "CMCETestUiEngineInSession.h"
#include <aknviewappui.h>

// CONSTANTS


CMCETestUIStreamView* CMCETestUIStreamView::NewL( CMCETestUIEngine& aEngine )
	{
	CMCETestUIStreamView* self = new(ELeave) CMCETestUIStreamView( aEngine );
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CMCETestUIStreamView::CMCETestUIStreamView( CMCETestUIEngine& aEngine )
    : iEngine( aEngine)
	{
	}

//
// Destructor for the view.
//
CMCETestUIStreamView::~CMCETestUIStreamView()
	{
	if (iContainer)
		{
		AppUi()->RemoveFromViewStack( *this, iContainer);
		delete iContainer;
		iContainer = NULL;
		}
	if (iVideoContainer)
		{
		AppUi()->RemoveFromViewStack( *this, iVideoContainer);
		delete iVideoContainer;
		iVideoContainer = NULL;
		}
	}

// -----------------------------------------------------------------------------
// CMCETestUISessionView::EngineStateChangedL()
// -----------------------------------------------------------------------------
//
void CMCETestUIStreamView::EngineStateChangedL()
    {
    if ( iContainer )
        {
        iContainer->ConstructModelL();
        iContainer->DrawNow();
    //    ConstructStatusPaneL();
        }
     if ( iVideoContainer )
        {
        iVideoContainer->ConstructModelL();
        iVideoContainer->DrawNow();
    //    ConstructStatusPaneL();
        }
    }

// -----------------------------------------------------------------------------
// CMCETestUISessionView::ChangeToMainViewL()
// -----------------------------------------------------------------------------
//
void CMCETestUIStreamView::ChangeToSessionViewL()
    {
    TUid viewId = TUid::Uid( EMCETestUISessionViewId );
    TUid customMessageId = TUid::Uid( EMCETestUISessionViewId );
    
    TBuf8<10> streamIndex; 
    TInt aStreamIndex = 0;
    streamIndex.AppendNum( aStreamIndex );
    
    TRAPD( err, AppUi()->ActivateLocalViewL( viewId,customMessageId,streamIndex ) );
    if ( err != KErrNone )
        {
        User::InfoPrint( _L("Error") );
        }
        
    }

/**
* Called by the framework
* @return The Uid for this view
*/
TUid CMCETestUIStreamView::Id() const 
    {
    return TUid::Uid( EMCETestUIStreamViewId );
    }


/**
* From CEikAppUi, takes care of command handling for this view.
*
* @param aCommand command to be handled
*/
void CMCETestUIStreamView::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case EAknSoftkeyBack:
            {
            ChangeToSessionViewL();
            break;
            }
        case EAknSoftkeyOptions:
            {
            if ( iContainer ){
                iContainer->DoShowMenuL();
                }
            if ( iVideoContainer ){
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

void CMCETestUIStreamView::DoActivateL(
        const TVwsViewId& /*aPrevViewId*/,
        TUid /*aCustomMessageId*/,
        const TDesC8& /*aCustomMessage */)
    {
    if (!iContainer)
		{
		
		// Solve session to be shown
		CEikonEnv* eikonEnv = CEikonEnv::Static();
		CMCETestUIAppUi* tempUiPtr = (CMCETestUIAppUi*)eikonEnv->AppUi();
		TInt sessionIndex = tempUiPtr->iCurrentSessionViewIndex ;
		const RPointerArray<CMCETestUIEngineOutSession>& outSessions = 
                    iEngine.OutSessions();               
                    
        const RPointerArray<CMCETestUIEngineInSession>& inSessions = 
                    iEngine.InSessions(); 
        
        if ( sessionIndex < outSessions.Count() )
            {
            iSession = outSessions[sessionIndex];
            }
        else
            {
            sessionIndex -= outSessions.Count();
            if ( sessionIndex < inSessions.Count() )
                {  
                iSession = inSessions[sessionIndex];
                }
            else
                {
                User::Leave( KErrArgument );
                }
            }
		
		// Solve stream to be shown
	 
    	TInt streamIndex = tempUiPtr->iCurrentStreamViewIndex;             
       
        
        const RPointerArray<CMCETestUIEngineAudioStream>& audioStreams = 
                    iSession->AudioStreamsL();               
                    
        const RPointerArray<CMCETestUIEngineVideoStream>& videoStreams = 
                    iSession->VideoStreamsL();  
                    
        if ( streamIndex < audioStreams.Count() )
            {
            iStream = audioStreams[streamIndex];
            iContainer = CMCETestUIStreamViewContainer::NewL(
		    AppUi()->ClientRect(), *this, iEngine, *iStream );
			iContainer->SetMopParent(this);
		//iContainer->ConstructL(AppUi()->ClientRect());
			AppUi()->AddToViewStackL( *this, iContainer );
            }
        else
            {
            streamIndex -= audioStreams.Count();
            if ( streamIndex < videoStreams.Count() )
                {  
                iVideoStream = videoStreams[streamIndex];
                iVideoContainer = CMCETestUIVideoStreamViewContainer::NewL(
		    	AppUi()->ClientRect(), *this, iEngine, *iVideoStream );
				iVideoContainer->SetMopParent(this);
				//iContainer->ConstructL(AppUi()->ClientRect());
				AppUi()->AddToViewStackL( *this, iVideoContainer );
                }
            else
                {
                User::Leave( KErrArgument );
                }
            }
		

      /*  else
            {
            streamIndex -= audioStreams.Count();
            iStream = audioStreams[streamIndex];
            }
		*/    
	/*	iContainer = CMCETestUIStreamViewContainer::NewL(
		    AppUi()->ClientRect(), *this, iEngine, *iStream );
		iContainer->SetMopParent(this);
		//iContainer->ConstructL(AppUi()->ClientRect());
		AppUi()->AddToViewStackL( *this, iContainer );
	*/	}
    
         
	Cba()->SetCommandSetL( R_AVKON_SOFTKEYS_OPTIONS_BACK );  
	Cba()->DrawNow();  
    
  //  ConstructStatusPaneL();    
    
    }


// -----------------------------------------------------------------------------
// CMCETestUIStreamView::DoDeactivate
//
// Called by the framework when the view is deactivated.  
// Removes the container from the control stack and deletes it.
//
// -----------------------------------------------------------------------------
void CMCETestUIStreamView::DoDeactivate()
    {
	if (iContainer )
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
	if (iVideoContainer )
		{
		// TODO: This should not leave
        CAknNavigationControlContainer* navi = 
            static_cast<CAknNavigationControlContainer*>
                (StatusPane()->ControlL( TUid::Uid( EEikStatusPaneUidNavi ) ));
        navi->Pop( iNaviLabel );
        delete iNaviLabel;
        iNaviLabel = NULL;	
        	
		AppUi()->RemoveFromViewStack( *this, iVideoContainer );
		delete iVideoContainer;
		iVideoContainer = NULL;
		}	
    }

// -----------------------------------------------------------------------------
// CMCETestUIStreamView::ConstructStatusPaneL
// -----------------------------------------------------------------------------
//
void CMCETestUIStreamView::ConstructStatusPaneL()
    {
    CAknTitlePane* titlePane = static_cast<CAknTitlePane*>
        (StatusPane()->ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ));
    titlePane->SetTextL( KStream );
    
    CAknNavigationControlContainer* navi = 
            static_cast<CAknNavigationControlContainer*>
                (StatusPane()->ControlL( TUid::Uid( EEikStatusPaneUidNavi ) ));
    navi->Pop( iNaviLabel );
    delete iNaviLabel;
    iNaviLabel = NULL;
    iNaviLabel = navi->CreateNavigationLabelL( iStream->State() );
    navi->PushL( *iNaviLabel );    
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIStreamView::ConstructL
// -----------------------------------------------------------------------------
//
void CMCETestUIStreamView::ConstructL()
    {
    BaseConstructL(); 
	}
