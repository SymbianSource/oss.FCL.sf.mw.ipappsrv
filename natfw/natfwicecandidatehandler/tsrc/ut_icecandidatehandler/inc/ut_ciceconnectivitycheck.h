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




#ifndef __UT_CICECONNECTIVITYCHECK_H__
#define __UT_CICECONNECTIVITYCHECK_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "mnatfwpluginobserver.h"
#include "natfwconnectivityapidefs.h"
#include "mncmconnectionmultiplexerobserver.h"
#include "miceconnchecklistener.h"
#include "ciceconnectivitycheck.h"
#include "miceconnhandlerobserver.h"
#include "micenatplugineventobs.h"

//  FORWARD DECLARATIONS
class CIceConnectivityCheck;
class CNATFWCandidate;
class CIceSessionData;
class CNcmConnectionMultiplexer;
class CIceConnectionHandler;
class CNATFWCandidatePair;
class CNATFWCredentials;
class CIceNatPluginContainer;

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
NONSHARABLE_CLASS( UT_CIceConnectivityCheck )
     : 
     public CEUnitTestSuiteClass,
     public MNcmConnectionMultiplexerObserver,
     public MIceConnCheckListener,
     public MIceConnHandlerObserver,
     public MIceNatPluginEventObs
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CIceConnectivityCheck* NewL();
        static UT_CIceConnectivityCheck* NewLC();
        /**
         * Destructor
         */
        ~UT_CIceConnectivityCheck();

    private:    // Constructors and destructors

        UT_CIceConnectivityCheck();
        void ConstructL();

public:     // From observer interface

    void Notify( TUint aSessionId, TUint aStreamId,
        TNotifyType aType, TInt aError );
        
    void IcmpError( TUint aSessionId, TUint aStreamId, 
        const TInetAddr& aAddress );
    
    void CheckCompletedL( 
        TInt aCompletionCode,
        const CIceConnectivityCheck& aCheck,
        CNATFWCandidatePair* aValidatedPair );

    void NominationCompletedL( 
        TInt aCompletionCode,
        const CIceConnectivityCheck& aCheck,
        const CNATFWCandidatePair& aValidatedPair );
    
    void StreamClosed( TUint aStreamId );

    // from base class MIceNatPluginEventObs

    void PluginEventOccured( const CNATFWPluginApi* aPlugin, 
        TUint aStreamId, TNatPluginEvent aEventCode, 
        TInt aErrorCode, TAny* aEventData, TUint aStreamConnectionId,
        TUint aComponentId, TBool aIPv6After );

private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CICEConnectivityCheck_ComparePrioritiesL();
        
        
         void UT_CICEConnectivityCheck_MatchAddressesL();
        
         void UT_CICEConnectivityCheck_CreateValidatedPairLL();
        
         void UT_CICEConnectivityCheck_PerformConnCheckLL();
         void UT_CICEConnectivityCheck_PerformConnCheckL_TL();
         
         void UT_CICEConnectivityCheck_PerformConnCheckL2L();
         
         void UT_CICEConnectivityCheck_PerformConnCheckL3L();
         
         void UT_CICEConnectivityCheck_PerformConnCheckL4L();
                
         void UT_CICEConnectivityCheck_FoundationL();
        
        
         void UT_CICEConnectivityCheck_ComponentIdL();
        
        
         void UT_CICEConnectivityCheck_StateL();
        
        
         void UT_CICEConnectivityCheck_InitializeCheckL();
        
        
         void UT_CICEConnectivityCheck_ValidatedPairLL();
        
        
         void UT_CICEConnectivityCheck_ConnCheckCompletedLL();
         
         void UT_CICEConnectivityCheck_StreamIdL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        CIceConnectivityCheck* iConnCheck;
        CNATFWCandidate* iLocalCand;
        CNATFWCandidate* iRemoteCand;
	    CIceSessionData* iCandStorage;
	    CNcmConnectionMultiplexer* iMultiplexer;
	    CIceConnectionHandler* iConnHandler;

        CNATFWCredentials* iInboundIdentRtp;
        CNATFWCredentials* iOutboundIdentRtp;
	    
        // Not own
        TInt iCheckResult;
        CNATFWCandidatePair* iValidatedPair;
        
        CIceConnectivityCheck::TIceCheckType iCheckType;
        
        CIceNatPluginContainer* iPluginContainer;
    };

#endif      //  __UT_CICECONNECTIVITYCHECK_H__

// End of file
