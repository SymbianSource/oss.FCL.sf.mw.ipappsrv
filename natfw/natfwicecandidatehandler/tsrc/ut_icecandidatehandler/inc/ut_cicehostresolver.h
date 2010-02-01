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




#ifndef __UT_CICEHOSTRESOLVER_H__
#define __UT_CICEHOSTRESOLVER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "mnatfwpluginobserver.h"

//  INTERNAL INCLUDES
#include "mncmconnectionmultiplexerobserver.h"

//  FORWARD DECLARATIONS
class CIceHostResolver;
class CNcmConnectionMultiplexer;

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
NONSHARABLE_CLASS( UT_CIceHostResolver )
        :
        public CEUnitTestSuiteClass,
        public MNATFWPluginObserver,
        public MNcmConnectionMultiplexerObserver     
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CIceHostResolver* NewL();
        static UT_CIceHostResolver* NewLC();
        /**
         * Destructor
         */
        ~UT_CIceHostResolver();

    private:    // Constructors and destructors

        UT_CIceHostResolver();
        void ConstructL();

    public:     // From observer interface

        // from MNATFWPluginObserver
        void Error( const CNATFWPluginApi& aPlugin,
            TUint aStreamId, TInt aErrorCode );
        
        void Notify( const CNATFWPluginApi& aPlugin,
            TUint aStreamId, TNATFWPluginEvent aEvent, TInt aErrCode );
        
        void NewCandidatePairFound( 
            const CNATFWPluginApi& aPlugin,
            CNATFWCandidatePair* aCandidatePair );
        
        void NewLocalCandidateFound( 
            const CNATFWPluginApi& aPlugin,
            CNATFWCandidate* aLocalCandidate );
         
         // from MNcmConnectionMultiplexerObserver
         void Notify( TUint aSessionId, TUint aStreamId,
            TNotifyType aType, TInt aError );

         void IcmpError( TUint aSessionId, TUint aStreamId, 
            const TInetAddr& aAddress );
         
         HBufC8* IncomingMessageL( TUint aStreamId, const TDesC8& aMessage,
            const TInetAddr& aLocalAddr, const TInetAddr& aFromAddr,
            TInetAddr& aPeerRemoteAddr, TBool& aConsumed );        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CICEHostResolver_HandleCallBackL();
         
         void UT_CICEHostResolver_LocalAddressL();
        
         void UT_CICEHostResolver_ConnectServerLL();
         
         void UT_CICEHostResolver_FetchCandidateLL();
        
         void UT_CICEHostResolver_SetReceivingStateLL();
         
         void UT_CICEHostResolver_SetSendingStateLL();
         
         void UT_CICEHostResolver_GetConnectionIdLL();
        
         void WaitForEvent( TNATFWPluginEvent aEvent );

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CIceHostResolver* iResolver;
        CNcmConnectionMultiplexer* iMultiplexer;
        TUint iSessionId;
        TUint iStreamId;
        
        TInt iEventErrCode;
        TInt iEventStreamId;
        TNATFWPluginEvent iEventToWait;
        RPointerArray<CNATFWCandidate> iLocalCandidates;
    };

#endif      //  __UT_CICEHOSTRESOLVER_H__

// End of file
