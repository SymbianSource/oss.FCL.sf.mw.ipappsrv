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
* Description:  State machine description.
*
*/

#ifndef NSPSTATEMACHINE_H
#define NSPSTATEMACHINE_H

#include <e32base.h>
#include "nsppluginreturndef.h"
#include "nsputdefinitions.h"

class TNSPStateMachineEvent;
class CNSPStateBase;

/**
 *  State machine that contains all states, and pointer to current state.
 *
 *  Machine which used to implement state design pattern. Machine contains
 *  all states, and current state. When processing completes successfully,
 *  state machine observer is used to notify success. Otherwise leave expected.
 *
 *  @lib natfwsdpprovider.dll
 *  @since S60 3.2
 */
class CNSPStateMachine : public CBase
    {
public: // Constructors and destructor

    /**
     * A two-phase constructor.
     */
    static CNSPStateMachine* NewL();
    
    /**
     * A two-phase constructor.
     */
    static CNSPStateMachine* NewLC();
    
    /**
     * Destructor.
     */
    virtual ~CNSPStateMachine();


private: // Constructors and destructor

    CNSPStateMachine();
    
    void ConstructL();
    
    void InitializeStateArrayL();


public: // New functions

    /**
     * Current state is applied with this method.
     *
     * @since       S60 3.2
     * @param       aEvent          State machine event
     */
    void ProcessL( TNSPStateMachineEvent& aEvent );


private: // New functions
    
    void IsAcceptableStateTransitionL( TInt aStateIndex ) const;
    
    TBool IsAcceptableStateTransition( TInt aStateIndex ) const;


private: // data

    /**
     * Array containing each state object.
     * Own.
     */
    RPointerArray<CNSPStateBase> iStateArray;
    
    /**
     * Pointer to a object which is the 'CURRENT' state
     * Not own.
     */
    CNSPStateBase* iCurrentState;
    
    /**
     * Definitions for unit testing.
     */
    NSP_UT_DEFINITIONS
    
    };

#endif // NSPSTATEMACHINE_H

// end of file
