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
* Description:    Provides asynchronous eventhandler for MMF/Mcc events
*
*/



#ifndef MCCTESTEVENTHANDLER_H
#define MCCTESTEVENTHANDLER_H

//  INCLUDES
#include <e32base.h>
#include <mmf/common/mmfcontrollerframework.h>
#include "rtpapi.h"
#include "mmccevents.h"
#include "mccinternaldef.h"

class MMccEventHandler;

// CONSTANTS

// MACROS

// DATA TYPES
/**
* Eventhandling state
*/
enum TMccEventHandlerState
    {
    EStateIdle = 0,
    EStateHandleMmfEvent,
    EStateHandleSinkEvent,
    EStateHandleSourceEvent,
    EStateHandleJitterEvent,
    EStateHandleSourceInactivityEvent,
    EStateHandleSourceActivityEvent,
    EStateCompleteClientReq,
    EStateHandleDtmfSig,
    EStateHandleAmrEvent
    };

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
* Mcc Asynchronous eventhandler for async MMF/Mcc event handling
*
* @lib Mccsubthreads.lib
* @since Series 60 3.0
*/
NONSHARABLE_CLASS( CMccTestEventHandler ):  
    public CBase, public MAsyncEventHandler, public MMccEventHandler, public MRtpErrNotify
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CMccTestEventHandler* NewL();

        /**
        * Destructor.
        */
        virtual ~CMccTestEventHandler();


    public: // Functions from base classes
    
        /**
        * From MAsyncEventHandler
        */
        
        TInt SendEventToClient( const TMMFEvent& aEvent );
        
    public: // Functions from base classes
    
    	TInt SendMccEventToClient( TMccEvent& aEvent );
        	    
	    void StateChange( TInt aState, TUint32 aLink );

	    void ErrorOccured( TInt aError, TUint32 aSessionId, TUint32 aLink,
	                       TUint32 aStreamId, TUint32 aEndpointId );  
    
    public: // From MRtpErrNotify
    
        void ErrorNotify( TInt aErrCode );
            
    private:    // New functions
        
        /**
        * Changes the state of the eventhandler
        * @since Series 60 3.0
        * @param aState New state to take
        * @return void
        */
        void ChangeState( const TMccEventHandlerState& aState );
        
        
        /**
        * Completes the client request if active if a client needs to be 
        * informed about subthread events.
        * @since Series 60 3.0
        * @param None
        * @return void
        */
        void CompleteClientRequest();
        
        /**
        * Handles a AMR payload formatter CMR event
        * @since Series 60 3.0
        * @param None
        * @return void
        */
        void HandleAmrEvent();

    private:    // Constructors and destructor

        /**
        * C++ default constructor.
        */
        CMccTestEventHandler();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
    public:
    
        TMccEvent iLastEvent;

        
    };

#endif      // MCCTESTEVENTHANDLER_H
            
// End of File
