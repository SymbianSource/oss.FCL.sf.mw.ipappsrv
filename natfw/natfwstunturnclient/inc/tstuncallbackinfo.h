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
* Description:    
*
*/




#ifndef T_STUNCALLBACKINFO_H
#define T_STUNCALLBACKINFO_H

// INCLUDES
#include <e32base.h>
#include "natfwstunclientobserver.h"

// FORWARD DECLARATIONS
class CSTUNClient;
class CBinding;

//CLASS DECLARATION

/**
 * This class contains the information to execute a single callback function of
 * MSTUNClientObserver.
 */
class TSTUNCallbackInfo
    {
    public: // Enumerations

        // One entry for each function of MSTUNClientObserver, including all the
        // different TSTUNBindingEvent values.
        enum TFunction
            {
            EInitCompleted,
            EEventAddressResolvedL,
            EEventCredentialsRejected,
            EErrorOccurred,
            EEventTCPConnectOk,
            EEventActiveDestinationSet
            };

    public: // Constructors and destructor

        /**
         * Constructor     
         * @param aFunction Identifies the function of MSTUNClientObserver to
         *                     be called
         * @param aBinding STUN binding
         * @param aErrorCode Error reason
         * @param aClient STUN client
         */
        TSTUNCallbackInfo( TSTUNCallbackInfo::TFunction aFunction,
                           const CBinding* aBinding,
                           TInt aErrorCode = KErrNone,
                           const CSTUNClient* aClient = NULL );

        /**
         * Checks that the selected callback function has required parameters
         * defined and that no unrelevant parameters have been specified.
         */
        TBool Validate() const;
        
        /**
         * Calls a selected function of aObserver with the given parameters.
         * The function to call and the parameters were specified in the
         * constructor.
         * @param aObserver Observer implementing the callback functions
         */
        void Execute( MSTUNClientObserver& aObserver ) const;

    private: // Constructors, for internal use

        TSTUNCallbackInfo();

        TSTUNCallbackInfo( const TSTUNCallbackInfo& aCallbackInfo );

    private: // New functions, for internal use

        void PassBindingEvent(
            MSTUNClientObserver& aObserver,
            MSTUNClientObserver::TSTUNBindingEvent aEvent ) const;

    public: // Data

        TSglQueLink    iLink;

        // Selected callback function
        TSTUNCallbackInfo::TFunction iFunction;

        // Parameters to the selected callback function, not owned.
        const CBinding* iBinding;
        TInt iErrorCode;
        const CSTUNClient* iClient;

    private: // For testing purposes

#ifdef TEST_EUNIT
        friend class CSTUNCallbackInfoTest;
#endif
    };

#endif // end of T_STUNCALLBACKINFO_H

// End of File
