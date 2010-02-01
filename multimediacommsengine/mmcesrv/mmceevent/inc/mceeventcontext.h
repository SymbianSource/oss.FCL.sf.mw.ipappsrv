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
* Description:    Base class for event contexts. Contains methods for
*                creating the state array and controlling current state.
*
*/





#ifndef CMCEEVENTCONTEXT_H
#define CMCEEVENTCONTEXT_H

// INCLUDES
#include <e32base.h>
#include "mcecomeventconstants.h"

// FORWARD DECLARATIONS
class CMceComEvent;
class CMceEventStateBase;

// CLASS DECLARATION

/**
*  Base class for event contexts.
*
*  @lib 
*  @since
*/
class CMceEventContext : public CBase 
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        
        static CMceEventContext* NewL( CMceComEvent& aEvent );
        
        /**
        * Destructor.
        */
        ~CMceEventContext();

    public: // New functions
        
        /**
        * Sets the current state for the context.
        * @since Series 60 3.0
        * @param aEvent The event owning this context.
        * @param aNewEventState Index of the new state in the state array.
        * @return KErrNone if successfull, system wide error code otherwise.
        */
        void SetCurrentStateL( CMceComEvent& aEvent, 
            				   TMceEventStateIndex aNewEventState );

        /**
        * Returns the current state of the context
        * @since Series 60 3.0
        * @param None.
        * @return TMceEventStateIndex the current state, index to it.
        */
        TMceEventStateIndex CurrentState();
        
        /**
        * Applies the current state of the context for the client actions. 
        * This causes a state transition to next state of the state machine. 
        * The current state sets the next state. Leaves on error
        * @since Series 60 3.0
        * @param aEvent The event this context belongs to.
        * @return None
        */
        void ProcessEventL( CMceComEvent& aEvent );
		
	    /**
        * Applies the current state of the context for the received request. 
        * This causes a state transition to next state of the state machine. 
        * The current state sets the next state. Leaves on error.
        * @since Series 60 3.0
        * @param aEvent The event this context belongs to.
        * @return None
        */
        void ProcessRequestReceivedEventL(CMceComEvent& aEvent );
        
        /**
        * Applies the current state of the context for the incoming response.
        * This causes a state transition to next state of the state machine. 
        * The current state sets the next state. Leaves on error
        * @since Series 60 3.0
        * @param aEvent The event this context belongs to.
        * @return None
        */
        void ProcessIncomingResponseEventL( CMceComEvent& aEvent );
                
    public: // Functions from base classes
        
    protected:  // New functions
        
        /**
        * Creates the states belonging to this context
        * @since Series 60 3.0
        * @param None
        * @return None
        */
        void InitializeStateArrayL( CMceComEvent& aEvent );

        /**
        * Checks if a state transition to a given state is accepted.
        * @since Series 60 3.0
        * @param TMceEventStateIndex aStateIndex
        * @return TBool, boolean value, is the transition valid.
        */
        TBool IsStateTransitionAccepted( TMceEventStateIndex aNewState );
         
        /**
        * C++ default constructor.
        */
        CMceEventContext();  
          
        /**
        * By default Symbian 2nd phase constructor is private. 
        * Now it is protected.
        */
        void ConstructL( CMceComEvent& aEvent );  
        
        TBool IsMoStateTransitionAccepted( TMceEventStateIndex aNewState );
        
        TBool IsMtStateTransitionAccepted( TMceEventStateIndex aNewState );
    
    private:
    
    	void InitialSubscribeCommonStatesL(); 
    	
    	void InitialReferCommonStatesL();  

    protected:  // Data

        // State array
        RPointerArray< CMceEventStateBase >* iStates;
       
    private:    // Data

        // Current state
        CMceEventStateBase* iCurrentState;
        
        // For testing
        friend class CMceSipEvent;
        friend class UT_CMceEventContext;
        friend class UT_CMceEventReferMtEstablishedState;
       
    };

#endif      // CMCEEVENTCONTEXT_H
            
// End of File
