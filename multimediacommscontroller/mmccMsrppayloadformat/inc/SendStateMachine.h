/*
* Copyright (c) 2004-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides a comfort noise generator class
*
*/


#ifndef SENDSTATEMACHINE_H
#define SENDSTATEMACHINE_H

//  INCLUDES
#include <e32base.h>
#include <e32std.h>
#include <mmfformat.h>

// DATA TYPES
// Send states.

// CLASS DECLARATION


class MSendStateObserver 
    {
    public:
    
        virtual void TimerExpiredL() = 0;

    };
    
/**
*  Sendstate machine
*/
class CSendStateMachine : public CBase
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CSendStateMachine* NewL( MSendStateObserver* aClient );

        /**
        * Destructor.
        */
        virtual ~CSendStateMachine() ;


    public: // New functions

	     // Starts the timer.
	    void Start  ();
	 
	    // Cancels the timer.
	    void Cancel ();
	    
	    void SetTimeOut(TTimeIntervalMicroSeconds32 aValue);

    private: // Functions from base classes
    
    	static TInt IdleCallBackL (TAny* aPtr);

    private:
   
        /**
        * C++ default constructor.
        */
        CSendStateMachine( MSendStateObserver* aClient );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private: // Data
        
        // Client who uses this state machine
        MSendStateObserver* iClient;
        
        CPeriodic*  iPeriodic; 
        
        TTimeIntervalMicroSeconds32 iTimeOutValue;
        
        
    };

#endif  //