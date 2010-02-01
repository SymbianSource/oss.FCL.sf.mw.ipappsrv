/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  
*
*/




#ifndef __TIMEOUTTIMER_H__
#define __TIMEOUTTIMER_H__

// INCLUDE FILES
#include <e32base.h>

// CLASS DECLARATIONS

/**
* A class for notifying of a timeout event.
*/
class MTimeoutNotifier
    {
public:

    /*
    * Called when the timer raises an event.
    */
    virtual void TimerExpired() = 0;
    };

/**
* A class that raises timeout events.
*/
class CTimeOutTimer : public CTimer
    {
public:
    
    /**
    * Standard Symbian two-phase construction
    * @param aPriority Priority of the active object.
    * @param aNotify Timer observer.
    */
    static CTimeOutTimer* NewL( const TInt aPriority,
                                MTimeoutNotifier& aNotify );
    /**
    * Standard Symbian two-phase construction
    * @param aPriority Priority of the active object.
    * @param aNotify A handle to the class to be notified of a timeout event.
    */
    static CTimeOutTimer* NewLC( const TInt aPriority,
                                 MTimeoutNotifier& aNotify );
    /**
    * Destructor
    */
    virtual ~CTimeOutTimer();

protected: // from CTimer

    /**
    * Active object post-request handling.
    */
    void RunL();
    
    /**
    * Handles a leave occurring in the request completion event handler RunL()
    */
    TInt RunError( TInt aError );

private:

    /**
    * Constructor.
    * @param aPriority Priority of the active object.
    * @param aNotify A handle to the class to be notified of a timeout event.
    */
    CTimeOutTimer( const TInt aPriority,
                   MTimeoutNotifier& aNotify);

    /**
    * Standard Symbian second-phase construction.
    */
    void ConstructL();

private:

    // Handle to the class to be notified of a timeout event.
    MTimeoutNotifier& iNotify;
    };

#endif

// End of file