/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    MCC EventHandler
*
*/




#ifndef __MCC_EVENT_H__
#define __MCC_EVENT_H__

// INCLUDES
#include <e32base.h>
#include <mmf/common/mmfcontroller.h>
#include "mccdef.h"
#include "mmccevents.h"
#include "mccinternaldef.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class MMccCtrlObserver;

// CLASS DECLARATION

/**
*  CMccEvent receives event notifications from the controller plugin and forwards 
*  them to the client by calling corresponding callback functions of the 
*  MMCCCtrlObserver.
*
*  @lib MCCInterface.lib
*  @since Series 60 3.0
*/
#ifdef EKA2
NONSHARABLE_CLASS( CMccEvent ) : public CActive
#else
class CMccEvent : public CActive
#endif
    {
    #ifdef TEST_EUNIT 
        friend class UT_CMCCInterface;
    #endif
    
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CMccEvent* NewL( RMMFController& aController, 
                                MMccCtrlObserver& aObserver );
        
        /**
        * Destructor.
        */
        virtual ~CMccEvent();

    public: // New functions
        /**
        * Requests for event from Controller plugin and starts event listening loop.
        * @since    Series 60 3.0
        * @return   void
        */
        void RequestEventNotification();

    protected: // From CActive
        /**
        * From CActive Handles an active object’s request completion event.
        * @since Series 60 3.0
        * @return None
        */
        void RunL();
        
        /**
        * From CActive
        */
        TInt RunError( TInt aError );
        
        /**
        * From CActive Implements cancellation of an outstanding request.
        * @since Series 60 3.0
        * @return None
        */
        void DoCancel();
    
    private: // New functions
        
        /**
        * Handles an media event
        * @since Series 60 3.0
        * @param aEvent Media event
        * @return void
        */
        void HandleMccMediaEvent( const TMccEvent& aEvent );
        
        /**
        * Handles state change
        * @since Series 60 3.0
        * @param aEvent Statechange event
        * @return void
        */
        void HandleMccStateChangeEvent( const TMccEvent& aEvent );
        
        /**
        * Handles error event
        * @since Series 60 3.0
        * @param aEvent Error event
        * @return void
        */
        void HandleMccErrorEvent( const TMccEvent& aEvent );
        
        /**
        * Handles inactivity event
        * @since Series 60 3.0
        * @param aEvent Error event
        * @return void
        */
        void HandleMccInactivityEvent( const TMccEvent& aEvent );

        /**
        * Handles activity event
        * @since Series 60 3.0
        * @param aEvent Error event
        * @return void
        */
        void HandleMccActivityEvent( const TMccEvent& aEvent );
		
        /**
         * Handles unknown media event
         *
         * @since Series 60 3.0
         * @param aEvent Event
         * @return void
         */
        void HandleMccUnknownMediaEvent( const TMccEvent& aEvent );
		
    private:

        /**
        * C++ default constructor.
        */
        CMccEvent( RMMFController& aController, 
                   MMccCtrlObserver& aCallback );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();


    private:    // Data
        
        // Handle to the controller plugin for sending custom commands
        RMMFController& iController;

        // Callback object to inform client about events
        MMccCtrlObserver& iEventCallBack;

        // Destination of the custom command
        TMMFMessageDestinationPckg iMessageDest; 

        // Contains information about event after completion of 
        // notification request issued to the controller
        TMccEventPackage iPackage;
    
    private:
        #ifdef TEST_EUNIT
        friend class UT_CMccInterface;
        #endif   
    };

#endif /* __MCC_EVENT_H__ */
            
// End of File
