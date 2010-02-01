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

#ifndef SESSIONVIEWCONTAINER_H
#define SESSIONVIEWCONTAINER_H

// INCLUDES

#include <aknlists.h>
#include <coecntrl.h>
#include <eiklbo.h>  // MListBoxObserver

//#include "UINotify.h"


// FORWARD DECLARATIONS
class CAknView;
class CMCETestUIEngine;
class CMCETestUIEngineSession;
class CMCETestUISessionViewModel;
class CMCETestUISessionView;


// CLASS DECLARATION

/**
*  CMCETestUISessionViewContainer  container control class.
*  
*/
class CMCETestUISessionViewContainer : public CCoeControl, MCoeControlObserver,
										public MEikListBoxObserver
{
public: // Constructors and destructor
    
    static CMCETestUISessionViewContainer* NewL( 
        const TRect& aRect, 
        CMCETestUISessionView& aOwningView,
        CMCETestUIEngine& aEngine,
        CMCETestUIEngineSession& aSession );
    
    /**
    * Destructor.
    */
    ~CMCETestUISessionViewContainer();
    
public: // New functions

    void ConstructModelL();
        
private: // Constructors 

    CMCETestUISessionViewContainer( CMCETestUISessionView& aOwningView, 
                                 CMCETestUIEngine& aEngine,
                                 CMCETestUIEngineSession& aSession );

    /**
    * @param aRect Frame rectangle for container.
    */
    void ConstructL(const TRect& aRect);
        
private: // Functions from base classes
    
    /**
    * From CoeControl. Calls SizeChangedL with TRAP.
    */
    void SizeChanged();  
    
    /**
    * Handle change of control size
    */
    void SizeChangedL();
    
    /**
    * From CoeControl.
    */
    TInt CountComponentControls() const;
    
    /**
    * From CCoeControl.
    */
    CCoeControl* ComponentControl(TInt aIndex) const;
    
    /**
    * Event handling section, e.g Listbox events.
    */
    void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);
    
    void HandleListBoxEventL(CEikListBox* aListBox, TListBoxEvent aEventType);

    /**
    * From CCoeControl,Draw.
    */
    void Draw(const TRect& aRect) const;
    
    /**
    * Key event handler. Handles up and down arrow keys, so that
    * output window can be scrolled.
    */
    TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);

    
public: // convenience functions

    void DoShowMenuL();


    
private: //data

	CAknDoubleStyleListBox* iListBox; // owned
	CMCETestUISessionViewModel* iListBoxModel; // ownership is given to iListBox

    CMCETestUISessionView& iOwningView; 
    CMCETestUIEngine& iEngine;
    CMCETestUIEngineSession& iSession;
    
    TBool iContextMenuAlreadyShown;

    };
    
#endif
    
    // End of File
