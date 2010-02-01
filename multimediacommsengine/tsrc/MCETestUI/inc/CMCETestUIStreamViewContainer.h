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

#ifndef STREAMVIEWCONTAINER_H
#define STREAMVIEWCONTAINER_H

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
class CMCETestUIEngineAudioStream;
class CMCETestUIEngineVideoStream;
class CMCETestUIStreamViewModel;
class CMCETestUIVideoStreamViewModel;


// CLASS DECLARATION

/**
*  CMCETestUISessionViewContainer  container control class.
*  
*/
class CMCETestUIStreamViewContainer : 
    public CCoeControl, public MCoeControlObserver, public MEikListBoxObserver
{
public: // Constructors and destructor
    
    static CMCETestUIStreamViewContainer* NewL( 
        const TRect& aRect, 
        CAknView& aOwningView,
        CMCETestUIEngine& aEngine,
        CMCETestUIEngineAudioStream& aStream );
    
    /**
    * Destructor.
    */
    ~CMCETestUIStreamViewContainer();
    
public: // New functions

    void ConstructModelL();
        
private: // Constructors 

    CMCETestUIStreamViewContainer( CAknView& aOwningView, 
                                 CMCETestUIEngine& aEngine,
                                 CMCETestUIEngineAudioStream& aStream );

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
	CMCETestUIStreamViewModel* iListBoxModel; // ownership is given to iListBox

    CAknView& iOwningView; 
    CMCETestUIEngine& iEngine;
    CMCETestUIEngineAudioStream& iStream;
    
    TBool iContextMenuAlreadyShown;

    };
 
 
/**
*  CMCETestUISessionViewContainer  container control class.
*  
*/
class CMCETestUIVideoStreamViewContainer : 
    public CCoeControl, public MCoeControlObserver,	public MEikListBoxObserver
{
public: // Constructors and destructor
    
    static CMCETestUIVideoStreamViewContainer* NewL( 
        const TRect& aRect, 
        CAknView& aOwningView,
        CMCETestUIEngine& aEngine,
        CMCETestUIEngineVideoStream& aStream );
    
    /**
    * Destructor.
    */
    ~CMCETestUIVideoStreamViewContainer();
    
public: // New functions

    void ConstructModelL();
        
private: // Constructors 

    CMCETestUIVideoStreamViewContainer( CAknView& aOwningView, 
                                 CMCETestUIEngine& aEngine,
                                 CMCETestUIEngineVideoStream& aStream );

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
	CMCETestUIVideoStreamViewModel* iListBoxModel; // ownership is given to iListBox

    CAknView& iOwningView; 
    CMCETestUIEngine& iEngine;
    CMCETestUIEngineVideoStream& iStream;
    
    TBool iContextMenuAlreadyShown;

    };    
#endif
    
    // End of File
