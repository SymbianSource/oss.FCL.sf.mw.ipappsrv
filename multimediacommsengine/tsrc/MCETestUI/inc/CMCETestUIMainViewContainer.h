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

#ifndef MAINVIEWCONTAINER_H
#define MAINVIEWCONTAINER_H

// INCLUDES

#include <aknlists.h>
#include <eiklbo.h>  // MListBoxObserver
#include <coecntrl.h>

//#include "UINotify.h"


// FORWARD DECLARATIONS
class CMCETestUIAppView;
class CMCETestUIEngine;
class CMCETestUIMainViewModel;


// CLASS DECLARATION

/**
*  CMCETestUIMainViewContainer  container control class.
*  
*/
class CMCETestUIMainViewContainer : public CCoeControl, 
                                    public MEikListBoxObserver
{
public: // Constructors and destructor
    
    static CMCETestUIMainViewContainer* NewL( 
        const TRect& aRect, 
        CMCETestUIAppView& aOwningView,
        CMCETestUIEngine& aEngine );
    
    /**
    * Destructor.
    */
    ~CMCETestUIMainViewContainer();
    
public: // New functions

    void ConstructModelL();
        
private: // Constructors 

    CMCETestUIMainViewContainer( CMCETestUIAppView& aOwningView, 
                                 CMCETestUIEngine& aEngine  );

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
    * Listbox event handler.
    */
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
	CMCETestUIMainViewModel* iListBoxModel; // ownership is given to iListBox

    CMCETestUIAppView& iOwningView; 
    CMCETestUIEngine& iEngine;

    TBool iContextMenuAlreadyShown;

    };
    
#endif
    
    // End of File
