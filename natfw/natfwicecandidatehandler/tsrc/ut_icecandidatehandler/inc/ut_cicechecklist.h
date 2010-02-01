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




#ifndef __UT_CICECHECKLIST_H__
#define __UT_CICECHECKLIST_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "mnatfwpluginobserver.h"
#include "mncmconnectionmultiplexerobserver.h"
#include "micechecklistobserver.h"
#include "miceconnhandlerobserver.h"
#include "micenatplugineventobs.h"

//  FORWARD DECLARATIONS
class CIceCheckList;
class CIceSessionData;
class CNcmConnectionMultiplexer;
class CIceConnectionHandler;
class CNATFWCandidate;
class CNATFWCandidatePair;
class CIceCheckPrioritizer;
class CNATFWCredentials;
class CIceNatPluginContainer;
class CNATFWPluginApi;

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
NONSHARABLE_CLASS( UT_CIceCheckList )
    :
    public CEUnitTestSuiteClass,
    public MIceChecklistObserver,
    public MNcmConnectionMultiplexerObserver,
    public MIceConnHandlerObserver,
    public MIceNatPluginEventObs
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CIceCheckList* NewL();
        static UT_CIceCheckList* NewLC();
        /**
         * Destructor
         */
        ~UT_CIceCheckList();

    private:    // Constructors and destructors

        UT_CIceCheckList();
        void ConstructL();

public:     // From observer interface

    // from base class MICEChecklistObserver
    void ChecklistCompletedL( 
        const CIceCheckList& aChecklist, 
        const RPointerArray<CNATFWCandidatePair>& aSelectedPairs );

    void ChecklistCompletedL( 
        const CIceCheckList& aChecklist,
        const RArray<TUint>& aFailedComps,
        TInt aErrCode );

    void ComponentsHaveValidPairsL( 
        const CIceCheckList& aChecklist, const TDesC8& aFoundation );

    void RoleChangeNeeded( TNATFWIceRole aDesiredRole );
    
    // from base class MNcmConnectionMultiplexerObserver
    void Notify( TUint aSessionId, TUint aStreamId,
        TNotifyType aType, TInt aError );
        
    void IcmpError( TUint aSessionId, TUint aStreamId, 
        const TInetAddr& aAddress );
        
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
        
         void UT_CICECheckList_NewLL();
        
         void UT_CICECheckList_TimerFiredL();
        
        
         void UT_CICECheckList_StateL();
        
        
         void UT_CICECheckList_StreamCollectionIdL();
        
        
         void UT_CICECheckList_InitializeCheckListLL();
        
        
         void UT_CICECheckList_InitializeCheckListL_1L();
        
        
         void UT_CICECheckList_StartPerformChecksLL();
        
         void UT_CICECheckList_StartPerformChecksL_2L();
         
         void UT_CICECheckList_RestartCheckListLL();
        
        
         void UT_CICECheckList_UpdateCheckListL_1L();
         
         void UT_CICECheckList_UpdateCheckListL_2L();
         
         void UT_CICECheckList_UpdateCheckListL_3L();
         
         void UT_CICECheckList_UpdateCheckListL_4L();
         
         void UT_CICECheckList_UpdateCheckListL_5L();
         
         void UT_CICECheckList_UpdateCheckListL_6L();
         
         void UT_CICECheckList_UpdateCheckListL_7L();
        
        
         void UT_CICECheckList_CheckCompletedLL();
        
        
         void UT_CICECheckList_STUNRequestReceivedLL();
         
         void UT_CICECheckList_STUNRequestReceivedL_1L();
         
         void UT_CICECheckList_STUNRequestReceivedL_2L();
         
         void UT_CICECheckList_STUNRequestReceivedL3L();
         
         void UT_CICECheckList_STUNRequestReceivedL4L();
         
         void UT_CICECheckList_RecomputePairPrioritiesL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
		
        CIceCheckPrioritizer* iCheckPrioritizer;
		CIceCheckList* iCheckList;
	    CIceSessionData* iCandStorage;
	    CNcmConnectionMultiplexer* iMultiplexer;
	    CIceConnectionHandler* iConnHandler;
	    
        CNATFWCandidate* iLocalCandRtp;
        CNATFWCandidate* iRemoteCandRtp;
        CNATFWCandidate* iLocalCandRtcp;
        CNATFWCandidate* iRemoteCandRtcp;
        
        RPointerArray<CNATFWCandidatePair> iSelectedPairs;
        
        TBool iCheckForLastCompSucceed;
        TBool iCheckListCompleted;
        CNATFWCredentials* iInboundIdentRtp;
        CNATFWCredentials* iOutboundIdentRtp;
        CNATFWCredentials* iInboundIdentRtcp;
        CNATFWCredentials* iOutboundIdentRtcp;
        
        CIceNatPluginContainer* iPluginContainer;
        
        TNATFWIceRole iRole;
    };

#endif      //  __UT_CICECHECKLIST_H__

// End of file
