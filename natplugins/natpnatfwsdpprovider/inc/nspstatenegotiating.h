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
* Description:  Negotiating state description.
*
*/

#ifndef NSPSTATENEGOTIATING_H
#define NSPSTATENEGOTIATING_H

#include "nspstatebase.h"

/**
 *  State where second round of Sdp O/A is interpret.
 *
 *  Response to initial O/A is interpred here, and Ice specific utilities 
 *  are applied. Also remote candidates are extracted from remote Sdp, 
 *  for later use.(next state)
 *
 *  @lib natfwsdpprovider.dll
 *  @since S60 3.2
 */
class CNSPStateNegotiating : public CNSPStateBase
    {
public: // Constructors and destructor

    /**
     * A two-phase constructor.
     */
    static CNSPStateNegotiating* NewL();
    
    /**
     * A two-phase constructor.
     */
    static CNSPStateNegotiating* NewLC();
    
    /**
     * Destructor.
     */
    virtual ~CNSPStateNegotiating();


private: // Constructors and destructor

    CNSPStateNegotiating();


protected: // From CNSPStateBase

    /**
     * Called to check if individual state accepts event, i.e.
     * whether to execute DoApplyL.
     *
     * @since       S60 3.2
     * @param       aEvent          State machine event.
     * @return      ETrue if event is accepted, EFalse otherwise.
     */
    TBool DoAcceptL( TNSPStateMachineEvent& aEvent );
    
    /**
     * Called when the state is entered.
     * 
     * @since       S60 3.2
     * @param       aEvent          State machine event.
     */
    void DoStateEntryL( TNSPStateMachineEvent& aEvent );
    
    /**
     * Called when state entry to next state will leave, and return
     * into 'this' state must be done.
     * 
     * @since       S60 3.2
     * @param       aEvent          State machine event.
     */
    void DoReverse( TNSPStateMachineEvent& aEvent );
    
    /**
     * Current state is applied with this method.
     * 
     * @since       S60 3.2
     * @param       aEvent          State machine event.
     */
    void DoApplyL( TNSPStateMachineEvent& aEvent );
    
    /**
     * Current state is exited with this method.
     * 
     * @since       S60 3.2
     * @param       aEvent          State machine event.
     */
    void DoExitL( TNSPStateMachineEvent& aEvent );


private:

	void ControlMediaL( TNSPStateMachineEvent& aEvent ) const;
	
    void ApplyNatL( TNSPStateMachineEvent& aEvent ) const;
    
    void ApplyDecodeAnswerL( TNSPStateMachineEvent& aEvent ) const;
    
    };

#endif // NSPSTATENEGOTIATING_H

// end of file
