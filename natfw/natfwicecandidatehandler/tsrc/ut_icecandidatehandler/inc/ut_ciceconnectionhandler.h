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




#ifndef __UT_CNATFWCONNECTIONHANDLER_H__
#define __UT_CNATFWCONNECTIONHANDLER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "mnatfwpluginobserver.h"

//  INTERNAL INCLUDES
#include "mncmconnectionmultiplexerobserver.h"
#include "ciceplugin.h"
#include "mncmincomingconnectionobserver.h"
#include "cncmconnectionmultiplexer_stub.h"
#include "miceconncheckobserver.h"
#include "natfwstunplugin.h"
#include "natfwcredentials.h"
#include "micenatplugineventobs.h"

//  FORWARD DECLARATIONS
class CIceConnectionHandler;
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
NONSHARABLE_CLASS( UT_CIceConnectionHandler )
     : public CEUnitTestSuiteClass,
       public MNcmConnectionMultiplexerObserver,
       public MNcmIncomingConnectionObserver,
       public MIceConnCheckObserver,
       public MIceConnHandlerObserver,
       public MIceNatPluginEventObs
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CIceConnectionHandler* NewL();
        static UT_CIceConnectionHandler* NewLC();
        /**
         * Destructor
         */
        ~UT_CIceConnectionHandler();

    private:    // Constructors and destructors

        UT_CIceConnectionHandler();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
            
         // from MNcmConnectionMultiplexerObserver
         void Notify( TUint aSessionId, TUint aStreamId,
            MNcmConnectionMultiplexerObserver::TNotifyType aType, TInt aError );

         void IcmpError( TUint aSessionId, TUint aStreamId, 
            const TInetAddr& aAddress );
         
         void IncomingMessageL( TUint aStreamId, const TDesC8& aMessage,
            const TInetAddr& aLocalAddr, const TInetAddr& aFromAddr,
            const TInetAddr& aPeerRemoteAddr, TBool& aConsumed );
         
         void ConnCheckCompletedL(
            TInt aCompletionCode,
            const TInetAddr& aFromAddr,
            const TInetAddr& aDestAddr,
            const TInetAddr& aMappedAddr );   
            
         void StreamClosed( TUint aStreamId );

         // from base class MIceNatPluginEventObs
         void PluginEventOccured( const CNATFWPluginApi* aPlugin, 
            TUint aStreamId, TNatPluginEvent aEventCode, 
            TInt aErrorCode, TAny* aEventData, TUint aStreamConnectionId,
            TUint aComponentId, TBool aIPv6After );         
        
         void UT_CNATFWConnectionHandler_SetSendingStatusLL();
        
        
         void UT_CNATFWConnectionHandler_CreateConnectionLL();
        
        
         void UT_CNATFWConnectionHandler_RemoveConnectionL();
        
        
         void UT_CNATFWConnectionHandler_PerformConnCheckLL();
         
         void UT_CNATFWConnectionHandler_CancelCheckL();
                 
         void UT_CNATFWConnectionHandler_IncomingMessageLL();
         
         void UT_CNATFWConnectionHandler_STUNClientInitCompletedL();
         
         void UT_CNATFWConnectionHandler_ConnectionNotifyL();
         
         void UT_CNATFWConnectionHandler_PluginEventOccuredL();
         
         void UT_CNATFWConnectionHandler_STUNBindingEventOccurredLL();
         
         void UT_CNATFWConnectionHandler_STUNBindingErrorOccurredL();

         void WaitForEvent( MIceNatPluginEventObs::TNatPluginEvent aEvent );
        
         void CreateCandidatePairsL();
         
    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;
        
        CNcmConnectionMultiplexer* iMultiplexer;
        TUint iSessionId;
        TUint iStreamId;
        TUint iConId;
        CIceConnectionHandler* iConnectionHandler;
        CIceSessionData* iCandStorage;
        CNATFWCredentials* iIdentification;
        TBool iDontStop;
        
        TInt iEventErrCode;
        TUint iEventStreamId;
        MIceNatPluginEventObs::TNatPluginEvent iEventToWait;
        
        CIceNatPluginContainer* iPluginContainer;
        TBool iCheckCompletedCalled;
        RPointerArray<CNATFWCandidate> iLocalCandidates;
        RPointerArray<CNATFWCandidatePair> iPairs;
    };

#endif      //  __UT_CNATFWCONNECTIONHANDLER_H__

// End of file
