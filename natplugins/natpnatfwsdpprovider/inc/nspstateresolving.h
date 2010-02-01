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
* Description:  Resolving state description.
*
*/

#ifndef NSPSTATERESOLVING_H
#define NSPSTATERESOLVING_H

#include "nspstatebase.h"

/**
 *  State where Candidate pairs are waited from NAT FW.
 *
 *  Each media stream component should receive one candidate pair,
 *  which are used to modify Sdp incase of Ice.
 *  This is Ice specific state.
 *
 *  @lib natfwsdpprovider.dll
 *  @since S60 3.2
 */
class CNSPStateResolving : public CNSPStateBase
    {
public: // Constructors and destructor

    /**
     * A two-phase constructor.
     */
    static CNSPStateResolving* NewL();
    
    /**
     * A two-phase constructor.
     */
    static CNSPStateResolving* NewLC();
    
    /**
     * Destructor.
     */
    virtual ~CNSPStateResolving();


private: // Constructors and destructor

    CNSPStateResolving();


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


private: // New functions

	void ControlMediaL( TNSPStateMachineEvent& aEvent ) const;
	
    void ApplyUpdateL( TNSPStateMachineEvent& aEvent ) const;
    
    };

#endif // NSPSTATERESOLVING_H

// end of file
