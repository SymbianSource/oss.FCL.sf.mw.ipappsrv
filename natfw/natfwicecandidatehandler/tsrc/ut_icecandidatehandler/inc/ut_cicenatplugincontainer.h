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




#ifndef __UT_CICENATPLUGINCONTAINER_H__
#define __UT_CICENATPLUGINCONTAINER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "mnatfwpluginobserver.h"

//  INTERNAL INCLUDES
#include "mncmconnectionmultiplexerobserver.h"
#include "cicenatplugincontainer.h"

//  FORWARD DECLARATIONS
class CIceNatPluginContainer;
class CIceSessionData;
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
NONSHARABLE_CLASS( UT_CIceNatPluginContainer )
     :  public CEUnitTestSuiteClass, 
        public MNcmConnectionMultiplexerObserver,
        public MIceNatPluginEventObs
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CIceNatPluginContainer* NewL();
        static UT_CIceNatPluginContainer* NewLC();
        /**
         * Destructor
         */
        ~UT_CIceNatPluginContainer();

    private:    // Constructors and destructors

        UT_CIceNatPluginContainer();
        void ConstructL();

    public:     // From observer interface
        
        // From MNcmConnectionMultiplexerObserver
        void Notify( TUint aSessionId, TUint aStreamId,
            TNotifyType aType, TInt aError );
            
        void IcmpError( TUint aSessionId, TUint aStreamId,
            const TInetAddr& aAddress );
        
        // From MIceNatPluginEventObs
        void PluginEventOccured( const CNATFWPluginApi* aPlugin, 
            TUint aStreamId, TNatPluginEvent aEventCode, 
            TInt aErrorCode, TAny* aEventData, TUint aStreamConnectionId = 0,
            TUint aComponentId = 0, TBool aIPv6After = EFalse );
            
    private:    // New methods

         void SetupL();
         
         void Teardown();
        
         void UT_CIceNatPluginContainer_ConnectPluginsLL();

         void UT_CIceNatPluginContainer_CountL();
        
        
         void UT_CIceNatPluginContainer_RegObserverForEventLL();
        
        
         void UT_CIceNatPluginContainer_UnRegObserverForEventL();
        
        
         void UT_CIceNatPluginContainer_UnregObserverL();
        
        
         void UT_CIceNatPluginContainer_ReleasePluginsL();
        
        
         void UT_CIceNatPluginContainer_PluginByIdentifierL();
        
        
         void UT_CIceNatPluginContainer_PluginByTypeL();
       
        
         void UT_CIceNatPluginContainer_ErrorL();
        
        
         void UT_CIceNatPluginContainer_NotifyL();
        
        
         void UT_CIceNatPluginContainer_NewCandidatePairFoundL();
        
        
         void UT_CIceNatPluginContainer_NewLocalCandidateFoundL();
        
         void ClearTestData();
         
         void WaitForEvent( MIceNatPluginEventObs::TNatPluginEvent aEvent );
         
    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        CIceNatPluginContainer* iPluginContainer;
        CIceSessionData* iSessionData;
        CNcmConnectionMultiplexer* iMultiplexer;
        
        TUint iStreamId;
        TInt iErrorCode;
        TNatPluginEvent iPluginEvent;
        CNATFWCandidate* iCandidate;
        CNATFWCandidatePair* iCandidatePair;
        TInt iNumOfCallbacks;
        MIceNatPluginEventObs::TNatPluginEvent iEventToWait;
    };

#endif      //  __UT_CICENATPLUGINCONTAINER_H__

// End of file
