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
* Description:  Localcand state description.
*
*/

#ifndef NSPSTATELOCALCAND_H
#define NSPSTATELOCALCAND_H

#include "nspstatebase.h"

/**
 *  State where local candidates are waited from NAT FW.
 *
 *  Media stream components are waiting for local candidates from NAT FW, 
 *  each component must have at least one local candidate until state machine
 *  can go forward.
 *
 *  @lib natfwsdpprovider.dll
 *  @since S60 3.2
 */
class CNSPStateLocalCand : public CNSPStateBase
    {
public: // Constructors and destructor

    /**
     * A two-phase constructor.
     */
    static CNSPStateLocalCand* NewL();
    
    /**
     * A two-phase constructor.
     */
    static CNSPStateLocalCand* NewLC();
    
    /**
     * Destructor.
     */
    virtual ~CNSPStateLocalCand();


private: // Constructors and destructor

    CNSPStateLocalCand();


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
    
    void ApplyUpdateL( TNSPStateMachineEvent& aEvent ) const;
    
    };

#endif // NSPSTATELOCALCAND_H

// end of file
