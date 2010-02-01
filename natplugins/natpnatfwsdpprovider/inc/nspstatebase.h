/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Base class description for all session states.
*
*/

#ifndef NSPSTATEBASE_H
#define NSPSTATEBASE_H

#include <e32base.h>

typedef TInt TNSPStateIndex;
const TNSPStateIndex KStateIndexInitializing = 0;
const TNSPStateIndex KStateIndexIdle = 1;
const TNSPStateIndex KStateIndexLocalCand = 2;
const TNSPStateIndex KStateIndexNegotiating = 3;
const TNSPStateIndex KStateIndexResolving = 4;
const TNSPStateIndex KStateIndexActivating = 5;
const TNSPStateIndex KStateIndexConnected = 6;
const TNSPStateIndex KStateIndexDeActivating = 7;

class TNSPStateMachineEvent;

/**
 *  Base class for all states.
 *
 *  Defines a set of methods, which are used by state machine.
 *  Each state has own definitions for these methods, and
 *  therefore different behavior.
 *
 *  @lib natfwsdpprovider.dll
 *  @since S60 3.2
 */
class CNSPStateBase : public CBase
    {
public: // Constructors and destructor
 
    /**
     * Destructor.
     */
    virtual ~CNSPStateBase();


protected: // Constructors and destructor

    /**
     * Constructor.
     */
    CNSPStateBase();


public: // New functions

    /**
     * Called to check if individual state accepts event, i.e.
     * whether to execute DoApplyL.
     *
     * @since       S60 3.2
     * @param       aEvent          State machine event.
     * @return      ETrue if transition is consumed, EFalse otherwise.
     */
    TBool AcceptL( TNSPStateMachineEvent& aEvent );
    
    /**
     * Called when the state is entered.
     * 
     * @since       S60 3.2
     * @param       aEvent          State machine event.
     */
    void StateEntryL( TNSPStateMachineEvent& aEvent );
    
    /**
     * Called when state entry to next state will leave, and return
     * into 'this' state must be done.
     *
     * @since       S60 3.2
     * @param       aEvent          State machine event.
     */
    void Reverse( TNSPStateMachineEvent& aEvent );
    
    /**
     * Current state is applied with this method.
     * 
     * @since       S60 3.2
     * @param       aEvent          State machine event.
     */
    void ApplyL( TNSPStateMachineEvent& aEvent );
    
    /**
     * Current state is exited with this method.
     * 
     * @since       S60 3.2
     * @param       aEvent          State machine event.
     */
    void ExitL( TNSPStateMachineEvent& aEvent );


protected:  // New functions

    /**
     * Called to check if individual state accepts event, i.e.
     * whether to execute DoApplyL.
     *
     * @since       S60 3.2
     * @param       aEvent          State machine event.
     * @return      ETrue if event is accepted, EFalse otherwise.
     */
    virtual TBool DoAcceptL( TNSPStateMachineEvent& aEvent ) = 0;
    
    /**
     * Called when the state is entered.
     *
     * @since       S60 3.2
     * @param       aEvent          State machine event.
     */
    virtual void DoStateEntryL( TNSPStateMachineEvent& aEvent ) = 0;
    
    /**
     * Called when state entry to next state will leave, and return
     * into 'this' state must be done. Inverse of DoExitL - method.
     *
     * @since       S60 3.2
     * @param       aEvent          State machine event.
     */
    virtual void DoReverse( TNSPStateMachineEvent& aEvent ) = 0;
    
    /**
     * Current state is applied with this method.
     *
     * @since       S60 3.2
     * @param       aEvent          State machine event.
     */
    virtual void DoApplyL( TNSPStateMachineEvent& aEvent ) = 0;
    
    /**
     * Current state is exited with this method.
     *
     * @since       S60 3.2
     * @param       aEvent          State machine event.
     */
    virtual void DoExitL( TNSPStateMachineEvent& aEvent ) = 0;
    
    };

#endif // NSPSTATEBASE_H

// end of file
