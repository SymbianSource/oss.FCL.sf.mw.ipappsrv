/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    State machine for DTMF payload format encoding and sending.
*
*/




#ifndef CDTMFENCSTATEMACHINE_H
#define CDTMFENCSTATEMACHINE_H

// INCLUDES
#include <e32base.h>

// DATA TYPES
/**
 * Eventhandling state
 */
enum TDTMFSendingState
    {
    EStateSendingIdle       = 0,
    EStateEncodeNextDigit   = 1,
    EStateToneOn            = 2,
    EStateToneOff           = 3,
    EStateSendPaused        = 4,
    EStateSendStopped       = 5
    };

// CLASS DECLARATION

/**
 * Callback interface for state machine clients.
 *
 * @lib mmccdtmfplformat.dll
 * @since Series 60 3.2
 */
class MDtmfEncClient
    {
    public:
    
        /**
         * Called when it's time to handle next digit in sequence
         * while sending DTMF string.
         *
         * @since   Series 60 3.2
         */
        virtual void EncodeNextDigitL() = 0;
        
        /**
         * Does actions belonging to tone on state. Actions include
         * DTMF event update packets sending.
         *
         * @since   Series 60 3.2
         * @param   aEntryToState   Indicates is this transition to state
         */        
        virtual void DoToneOnActionsL( TBool aEntryToState ) = 0;
        
        /**
         * Does actions belonging to tone off state.
         *
         * @since   Series 60 3.2
         * @param   aEntryToState   Indicates is this transition to state
         */        
        virtual void DoToneOffActionsL( TBool aEntryToState ) = 0;
    };

/**
 * State machine for dtmf payload format encoding and sending.
 *
 * @lib mmccdtmfplformat.dll
 * @since Series 60 3.2
 */
class CDtmfEncStateMachine : public CActive
    {
    public:  // Constructors and destructor
        
        /**
         * Two-phased constructor.
         */
        static CDtmfEncStateMachine* NewL( MDtmfEncClient& aClient );
        
        /**
         * Destructor.
         */
        virtual ~CDtmfEncStateMachine();

        /**
         * Changes state to the new one if transition is acceptable.
         *
         * @since    Series 60 3.2
         * @param    aNewState  New state to proceed to
         */
        void ChangeStateTo( TDTMFSendingState aNewState );
        
        /**
         * Returns current state.
         *
         * @since    Series 60 3.2
         * @return   Current state
         */
        TDTMFSendingState State() const;
        
        /**
         * Resets the statemachine from any state.
         *
         * @since S60 3.2
         */
        void ResetStateMachine();

    protected: // New functions

        /**
         * From CActive.
         */
        void RunL();

        /**
         * From CActive.
         */
        void DoCancel();

        /**
         * From CActive.
         */
        TInt RunError( TInt aError );

    private:

        /**
         * C++ default constructor.
         */
        CDtmfEncStateMachine( MDtmfEncClient& aClient );

    private: // Data

        /**
         * Client using state machine
         */        
        MDtmfEncClient& iClient;

        /**
         * Current state
         */
        TDTMFSendingState iState;
    };

#endif      // CDTMFENCSTATEMACHINE_H

//  End of File
