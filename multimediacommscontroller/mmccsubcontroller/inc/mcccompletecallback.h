/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef MCCCOMPLETECALLBACK_H
#define MCCCOMPLETECALLBACK_H

//  INCLUDES
#include <e32base.h>
#include <mmf/common/mmfcontrollerframework.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  DESCRIPTION
*
*  @lib Mccsubhtreads.dll
*  @since Series 60 3.0
*/
#ifdef EKA2
NONSHARABLE_CLASS( CCompleteCallback ) : public CActive
#else
class CCompleteCallback : public CActive
#endif
    {
    public: // Constructors and destructor
        /**
        * Constructor
        */
        CCompleteCallback( MAsyncEventHandler& aEventhandler, 
                TBool aWaitForSink );

        /**
        * Destructor
        */
        virtual ~CCompleteCallback();

    public: // New functions

        /**
        * Signals the datapath when datapath is completed
        * @since
        * @param [input] Completion error code
        * @return void
        */
        void SignalDataPathComplete( TInt aDataPathError );

    private:    // New functions

        /**
        * Activates and returns the current status of CCompleteCallback
        * @since
        * @param None
        * @return TRequestStatus Status of this request
        */
        TRequestStatus& ActiveStatus();

    public: // Functions from baseclasses
 
    protected:  // Functions from baseclasses
        
        /**
        * From CActive
        */
        void RunL();

        /**
        * From CActive
        */
        void DoCancel();
        
        /**
        * From CActive
        */        
        TInt RunError( TInt aError );

    private:
        
        // Eventhandler
        MAsyncEventHandler& iEventhandler;

        TBool iDataPathComplete;
        TBool iSinkComplete;
        TBool iWaitForSink;

        TInt iSinkError;
        TInt iDataPathError;
    };

#endif      // MCCCOMPLETECALLBACK_H
            
// End of File
