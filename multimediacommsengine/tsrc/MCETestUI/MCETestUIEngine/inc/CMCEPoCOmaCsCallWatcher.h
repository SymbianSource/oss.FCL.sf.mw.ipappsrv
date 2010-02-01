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
* Description:    Publish & Subscribe event observer for PoC
*
*/





#ifndef MCEPOCOMACSCALLWATCHER_H
#define MCEPOOMACCSCALLWATCHER_H

//  INCLUDES
#include <CTSYDomainPSKeys.h>
#include <e32property.h>
#include <e32svr.h>
#include "MCEPoCOmaCsStateObserver.h"


// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
// CLASS DECLARATION
/**
*  Handles PubSub events
*/
class CMCEPoCOmaCSCallWatcher: public CActive
    {
    public:
        /**
        * Creates instance of the CMCEPoCOmaCSCallWatcher class.
        */
        static CMCEPoCOmaCSCallWatcher* NewL( MMcePoCOmaCSStateObserver* aObserver );

        /**
        * Destructor.
        */
        virtual ~CMCEPoCOmaCSCallWatcher();

    private: // constructors
        /**
        * C++ default constructor.
        */
        CMCEPoCOmaCSCallWatcher();

        /**
        * Symbian OS constructor.
        */
        void ConstructL( MMcePoCOmaCSStateObserver* aObserver );

    public:
        /**
        * Starts asynchronic subscribe of PubSub event
        *
        * @return KErrNone if no errors and KErrInUse if already subscribed
        */
        TInt Subscribe();

        /**
        * Stops asynchronic subscribe of PubSub event
        */
        void StopSubscribe();

      
     
    private: // from CActive
    
        /**
        * Translates TPSTelephonyCallState state to TPoCCsState state
        *
        * @param aPSCsCallState State of call from PubSub
        * @return TPoCCsState PubSub CS call state translated to PoC CS call state
        */
        TPoCOmaCsState TranslateToPoCCsState( TPSCTsyCallState aCallState );
        
        /**
        * see CActive::RunL()
        */
        void RunL();

        /**
        * see CActive::Error()
        */
        TInt RunError( TInt aError );
        
        /**
        * see CActive::DoCancel()
        */
        void DoCancel();

    public:
        /**
        * For subscribing CS call events
        *
        * @param aObserver Pointer to caller.
        */
     //   void AddObserverL( const MMcePoCOmaCSStateObserver* aObserver );

        /**
        * For unsubscribing CS call events
        *
        * @param aObserver Pointer to caller.
        */
       // void RemoveObserver( const MMcePoCOmaCSStateObserver* aObserver );

        /**
        * For getting call state
        */
        TPoCOmaCsState CsState();

    private: // data
        /**
        * Handle to Publish & Subscribe
        */
        RProperty                	iProperty;
        /**
        * Array holding pointers to CS status observers
        */
        MMcePoCOmaCSStateObserver* iCsCallNotify;

    private: // friend

    };

#endif      // POCOMACSCALLWATCHER_H   
            
// End of File
