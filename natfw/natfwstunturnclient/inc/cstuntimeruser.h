/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef C_STUNTIMERUSER_H
#define C_STUNTIMERUSER_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CDeltaTimer;

// CLASS DECLARATION
/*
 * This class stores basic timer information needed for using CDeltaTimer.
 */
class CSTUNTimerUser : public CBase
    {
    public: // Constructor and destructor

        /**
         * Constructor.
         *
         * @param aDeltaTimer CDeltaTimer to provide timer services
         * @return value New CSTUNTimerUser object. Ownership is transferred.
         */
         CSTUNTimerUser( CDeltaTimer& aDeltaTimer );

        /**     
         * Frees the resources of CSTUNTimerUser. If a timer is currently
         * running, it is stopped.
         */
        ~CSTUNTimerUser();

    public: // New pure virtual functions

        /**
        * Handles the timer expiration event.
        */
        virtual void TimerExpiredL() = 0;

        /**
        * Called if leave occurred from TimerExpiredL.
        * @pre aError != KErrNone
        * @param aError Leave value from TimerExpiredL
        */
        virtual void LeaveFromTimerExpired( TInt aError ) = 0;

    protected: // New functions

        /**
        * Start a timer. Only one timer can be running at a time. 
        * @param aDuration Timer duration in milliseconds
        */
        void StartTimer( TInt aDuration );

        /**
        * Stop the timer.
        */
        void StopTimer();

        /**
        * This function is called by CDeltaTimer when a timer entry has expired.
        * @param aPtr Parameter that was specified when the timer was started.
        */
        static TInt TimerExpired( TAny *aPtr );
        
        /**
        * Obtain the CDeltaTimer instance.
        * @return CDeltaTimer
        */
        CDeltaTimer& DeltaTimer();
        
        /**
         * Obtain information is timer running or not
         *
         * @since   s60 3.2
         * @return  ETrue if running, else EFalse
         */
        TBool IsRunning() const;

    private:

        CSTUNTimerUser();

        CSTUNTimerUser( const CSTUNTimerUser& aTimerUser );

    private: // New functions, for internal use

        /**
        * Converts a duration of milliseconds to microseconds and returns a
        * TTimeIntervalMicroSeconds32 object, which can be passed to
        * CDeltaTimer.
        * @pre aMilliseconds <= KMaxTInt32 / 1000
        * @param aMilliseconds Duration in milliseconds
        * @return TTimeIntervalMicroSeconds32 Duration converted to microseconds
        * @panic KErrOverflow In debug build, panics if aMilliseconds has too
        *                      large value. In release build, the duration is
        *                      decreased to a maximum value, if it is too large.
        */
        TTimeIntervalMicroSeconds32 TimerDuration( TInt aMilliseconds );

    protected: // Data
        
        CDeltaTimer& iDeltaTimer;
    
    private: // Data

        //Identifies the running timer so that it can be canceled.
        TDeltaTimerEntry iTimerEntry;
        
        //ETrue if a timer is currently running
        TBool iIsRunning;

    private: // For testing purposes

#ifdef TEST_EUNIT
        friend class CSTUNClientImplementationTest;
        friend class CSTUNSharedSecretTest;
        friend class CSTUNBindingImplementationTest;
        friend class CSTUNTransactionTest;
#endif
    };

#endif // end of C_STUNTIMERUSER_H

// End of File
