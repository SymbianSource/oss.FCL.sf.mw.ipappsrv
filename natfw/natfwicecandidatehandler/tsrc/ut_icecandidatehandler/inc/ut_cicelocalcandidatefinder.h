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




#ifndef __UT_CNATFWLOCALCANDIDATEFINDER_H__
#define __UT_CNATFWLOCALCANDIDATEFINDER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "mnatfwpluginobserver.h"

//  INTERNAL INCLUDES
#include "mncmconnectionmultiplexerobserver.h"
#include "micenatplugineventobs.h"

//  FORWARD DECLARATIONS
class CIceLocalCandidateFinder;
class CNcmConnectionMultiplexer;
class CIceSessionData;
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
NONSHARABLE_CLASS( UT_CIceLocalCandidateFinder )
        : 
        public CEUnitTestSuiteClass,
        public MNcmConnectionMultiplexerObserver,
        public MIceNatPluginEventObs
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CIceLocalCandidateFinder* NewL();
        static UT_CIceLocalCandidateFinder* NewLC();
        /**
         * Destructor
         */
        ~UT_CIceLocalCandidateFinder();

    private:    // Constructors and destructors

        UT_CIceLocalCandidateFinder();
        void ConstructL();

    public:     // From observer interface

         // from MNATFWConnectionMultiplexerObserver
         void Notify( TUint aSessionId, TUint aStreamId,
            TNotifyType aType, TInt aError );

         void IcmpError( TUint aSessionId, TUint aStreamId, 
            const TInetAddr& aAddress );
         
         HBufC8* IncomingMessageL( TUint aStreamId, const TDesC8& aMessage,
            const TInetAddr& aLocalAddr, const TInetAddr& aFromAddr,
            TInetAddr& aPeerRemoteAddr, TBool& aConsumed );

         // from base class MIceNatPluginEventObs
         void PluginEventOccured( const CNATFWPluginApi* aPlugin, 
            TUint aStreamId, TNatPluginEvent aEventCode, 
            TInt aErrorCode, TAny* aEventData, TUint aStreamConnectionId,
            TUint aComponentId, TBool aIPv6After );         
    
    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CNATFWLocalCandidateFinder_ResolveNextCandidateL();
        
         void UT_CNATFWLocalCandidateFinder_FetchCandidatesLL();
        
         void UT_CNATFWLocalCandidateFinder_PluginEventOccuredL();
        
         void WaitForEvent( MIceNatPluginEventObs::TNatPluginEvent aEvent );

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;
        
        CIceLocalCandidateFinder* iFinder;
        CIceSessionData* iCandStorage;
        CNcmConnectionMultiplexer* iMultiplexer;
        TUint iSessionId;
        TUint iStreamId;

        TInt iEventErrCode;
        TInt iEventStreamId;
        MIceNatPluginEventObs::TNatPluginEvent iEventToWait;
        CIceNatPluginContainer* iPluginContainer;
    };

#endif      //  __UT_CNATFWLOCALCANDIDATEFINDER_H__

// End of file
