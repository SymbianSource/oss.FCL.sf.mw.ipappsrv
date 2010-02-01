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




#ifndef __UT_CNATFWICEPLUGIN_H__
#define __UT_CNATFWICEPLUGIN_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "natfwpluginapi.h"
#include "mnatfwpluginobserver.h"

//  INTERNAL INCLUDES
#include "mncmconnectionmultiplexerobserver.h"

//  FORWARD DECLARATIONS
class CIcePlugin;
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
NONSHARABLE_CLASS( UT_CIcePlugin )
    :
    public CEUnitTestSuiteClass,
    public MNATFWPluginObserver,
    public MNcmConnectionMultiplexerObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CIcePlugin* NewL();
        static UT_CIcePlugin* NewLC();
        /**
         * Destructor
         */
        ~UT_CIcePlugin();

    private:    // Constructors and destructors

        UT_CIcePlugin();
        void ConstructL();

    public:     // From observer interface

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

        void Notify( TUint aSessionId, TUint aStreamId,
            TNotifyType aType, TInt aError );
            
        void IcmpError( TUint aSessionId, TUint aStreamId, 
            const TInetAddr& aAddress );
        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CNATFWICEPlugin_ConnectServerLL();

         void UT_CNATFWICEPlugin_FetchCandidatesLL();
         
         void UT_CNATFWICEPlugin_FetchCandidatesLL2();
         
         void UT_CNATFWICEPlugin_SetReceivingStateLL();
        
         void UT_CNATFWICEPlugin_SetSendingStateL();
        
         void UT_CNATFWICEPlugin_GetConnectionIdLL();
         
         void UT_CNATFWICEPlugin_SetOperationModeLL();
        
        
         void UT_CNATFWICEPlugin_SetIdentificationLL();
        
        
         void UT_CNATFWICEPlugin_PerformConnectivityChecksLL();
        
         void UT_CNATFWICEPlugin_UpdateIceProcessing1LL();
         
         void UT_CNATFWICEPlugin_UpdateIceProcessing2LL();
         
         void UT_CNATFWICEPlugin_PluginIdentifierL();
         
         void UT_CNATFWICEPlugin_PluginEventOccuredL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        CNATFWPluginApi* iPlugin;
        CNcmConnectionMultiplexer* iMultiplexer;
        RPointerArray<CNATFWCandidate> iLocalCandidates;
        RPointerArray<CNATFWCandidatePair> iCandPairs;
        
        TNATFWPluginEvent iPluginEvent;
        TUint iEventStreamId;
        TInt iEventErrCode;
        TBool iUseWaitLoop;
    };

#endif      //  __UT_CNATFWICEPLUGIN_H__

// End of file
