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




#ifndef __UT_CICECHECKHANDLER_H__
#define __UT_CICECHECKHANDLER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "mnatfwpluginobserver.h"
#include "mncmconnectionmultiplexerobserver.h"
#include "miceconnhandlerobserver.h"
#include "micenatplugineventobs.h"

//  FORWARD DECLARATIONS
class CIceCheckHandler;
class CIceSessionData;
class CNcmConnectionMultiplexer;
class CIceConnectionHandler;
class CIceCheckList;
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
NONSHARABLE_CLASS( UT_CIceCheckHandler )
     : 
     public CEUnitTestSuiteClass,
     public MNcmConnectionMultiplexerObserver,
     public MIceConnHandlerObserver,
     public MIceNatPluginEventObs
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CIceCheckHandler* NewL();
        static UT_CIceCheckHandler* NewLC();
        /**
         * Destructor
         */
        ~UT_CIceCheckHandler();

private:    // Constructors and destructors

        UT_CIceCheckHandler();
        void ConstructL();

public:     // From observer interface

    void Notify( TUint aSessionId, TUint aStreamId,
        TNotifyType aType, TInt aError );
        
    void IcmpError( TUint aSessionId, TUint aStreamId, 
        const TInetAddr& aAddress );
    
    void ChecklistCompleted( 
        const CIceCheckList& aChecklist, TInt aCompletionCode );

    void ComponentsHaveValidPairsL( 
        const CIceCheckList& aChecklist, const TDesC8& aFoundation );

    void StreamClosed( TUint aStreamId );
    
    // from base class MIceNatPluginEventObs

    void PluginEventOccured( const CNATFWPluginApi* aPlugin, 
        TUint aStreamId, TNatPluginEvent aEventCode, 
        TInt aErrorCode, TAny* aEventData, TUint aStreamConnectionId,
        TUint aComponentId, TBool aIPv6After );

private:    // New methods

         void SetupControllingL();
         
         void SetupControlledL();
        
         void Teardown();
        
         void UT_CICECheckHandler_SetIdentificationLL();
         
         void UT_CICECheckHandler_PerformConnectivityChecksLL();
        
         void UT_CICECheckHandler_UpdateIceProcessing1LL();
         
         void UT_CICECheckHandler_UpdateIceProcessing2LL();

         void UT_CICECheckHandler_CleanupCollectionDataL();
         
         void UT_CICECheckHandler_STUNRequestReceivedL_1L();
         
         void UT_CICECheckHandler_STUNRequestReceivedL_2L();
        
         void UT_CICECheckHandler_ChecklistCompletedL();
        
         void UT_CICECheckHandler_ComponentsHaveValidPairsLL();
         
         void UT_CICECheckHandler_HandleNewCollectionsLL();
         
         void UT_CICECheckHandler_HandleIceRestartsLL();
         
         void UT_CICECheckHandler_RoleChangeNeededL();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
		
		CIceCheckHandler* iCheckHandler;
		    
	    CIceSessionData* iCandStorage;
	    CNcmConnectionMultiplexer* iMultiplexer;
	    CIceConnectionHandler* iConnHandler;

        CNATFWCandidate* iLocalCandRtpHost;
        CNATFWCandidate* iLocalCandRtpRflx;
        CNATFWCandidate* iRemoteCandRtpHost;
        CNATFWCandidate* iRemoteCandRtpRflx;
        CNATFWCredentials* iInboundIdent;
        CNATFWCredentials* iOutboundIdent;
        
        TBool iCandidatePairFound;
        TInt iErrorWithStream;
        
        CIceNatPluginContainer* iPluginContainer;
    };

#endif      //  __UT_CICECHECKHANDLER_H__

// End of file
