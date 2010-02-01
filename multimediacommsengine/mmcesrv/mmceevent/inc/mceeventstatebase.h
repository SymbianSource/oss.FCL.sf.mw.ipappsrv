/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Abstract base class for event states.
*
*/





#ifndef CMCEEVENTSTATEBASE_H
#define CMCEEVENTSTATEBASE_H

// INCLUDES
#include <e32base.h>
#include "mcecomevent.h"

// FORWARD DECLARATIONS
class CMceComEvent;

// CLASS DECLARATION

/**
*  Abstract base class for event states. Virtual methods are implemented
*  in concrete states,
*
*  @lib 
*  @since
*/
class CMceEventStateBase : public CBase 
    {
    public:  // Constructors and destructor
        
        /**
        * Destructor.
        */
        ~CMceEventStateBase();

    public: // New functions
      
        /**
        * Handle client initiated actions. Leaves on error.
        * @param aEvent, a reference to CMceComEvent
        * @return None
        */
        void ProcessStateL( CMceComEvent& aEvent );

        /**
        * Handle request received from the SIP stack. Leaves on error.
        * @param aEvent, a reference to CMceComEvent
        * @return None
        */
        void ProcessReqRecvStateL( CMceComEvent& aEvent );
        
        /**
        * Handle response or error received from the SIP stack. 
        * Leaves on error.
        * @param aEvent, a reference to CMceComEvent
        * @return None
        */
        void ProcessRespRecvStateL( CMceComEvent& aEvent );
 
    protected:  // New functions
        
        /**
        * Handle client initiated actions. Leaves on error.
        * Implemented in concrete states.
        * @param aEvent, a reference to CMceComEvent
        * @return None
        */
        virtual void HandleL( CMceComEvent& aEvent ) = 0;
        
        /**
        * Handle request received from the SIP stack. Leaves on error.
        * Implemented in concrete states.
        * @param aEvent, a reference to CMceComEvent
        */
        virtual void HandleReceiveRequestL( CMceComEvent& aEvent ) = 0;
        
        /**
        * Handle response or error received from the SIP stack. 
        * Leaves on error. Implemented in concrete states.
        * @param aEvent, a reference to CMceComEvent
        */
        virtual void HandleResponseReceivedL( CMceComEvent& aEvent ) = 0;

        /**
        * C++ default constructor.
        */
        CMceEventStateBase();
        
    private://EUNIT
    
    	friend class UT_CMceEventStateBase;
        
    };

#endif      // CMCEEVENTSTATEBASE_H
            
// End of File
