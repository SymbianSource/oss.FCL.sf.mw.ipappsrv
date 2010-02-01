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




#ifndef __UT_CNATFWTURNCONNECTIONHANDLER_H__
#define __UT_CNATFWTURNCONNECTIONHANDLER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "mnatfwpluginobserver.h"
#include "mncmconnectionmultiplexerobserver.h"

#include <in_sock.h>
#include "natfwconnectivityapidefs.h"
#include "natfwturnstreamdata.h"

//  FORWARD DECLARATIONS
class CNATFWTurnConnectionHandler;
class CNcmConnectionMultiplexer;
class CTestSettings;

#include <e32def.h>
#include <e32base.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
NONSHARABLE_CLASS( UT_CNATFWTurnConnectionHandler ) : 
    public CEUnitTestSuiteClass,
    public MNATFWPluginObserver,
    public MNcmConnectionMultiplexerObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWTurnConnectionHandler* NewL();
        static UT_CNATFWTurnConnectionHandler* NewLC();
        /**
         * Destructor
         */
        ~UT_CNATFWTurnConnectionHandler();

    private:    // Constructors and destructors

        UT_CNATFWTurnConnectionHandler();
        void ConstructL();

    public:     // From observer interface

        // From MNcmConnectionMultiplexerObserver
        
        void Notify( TUint aSessionId, TUint aStreamId, 
            TNotifyType aType, TInt aError );

        void IcmpError( TUint aSessionId,
                        TUint aStreamId,
                        const TInetAddr& aAddress );

        // From MNATFWPluginObserver
    
	    void Error( const CNATFWPluginApi& aPlugin, TUint aStreamId, TInt aErrorCode );
	    
	    void Notify( const CNATFWPluginApi& aPlugin,
	                 TUint aStreamId,
	                 TNATFWPluginEvent aEvent,
	                 TInt aErrCode );

	    void NewCandidatePairFound( const CNATFWPluginApi& aPlugin,
	        CNATFWCandidatePair* aCandidatePair );

        void NewLocalCandidateFound( const CNATFWPluginApi& aPlugin, 
            CNATFWCandidate* aLocalCandidate );
        

    private:    // New methods

         void SetupL();
         
         void SetupAL();
        
         void Teardown();
         
         
         void UT_CNATFWTurnConnectionHandler_ConnectServerLL();
         
         
         void UT_CNATFWTurnConnectionHandler_TryNextServerLL();
         
         
         void UT_CNATFWTurnConnectionHandler_ConnectionByIdL();
         

         void UT_CNATFWTurnConnectionHandler_FetchCandidateLL();
        
        
         void UT_CNATFWTurnConnectionHandler_StartTurnRefreshL();
         
         
         void UT_CNATFWTurnConnectionHandler_CreateTURNBindingLL();
         
         
         void UT_CNATFWTurnConnectionHandler_SetReceivingStateLL();
         
         
         void UT_CNATFWTurnConnectionHandler_SetSendingStateLL();
         
         
         void UT_CNATFWTurnConnectionHandler_GetConnectionIdLL();
         
         
         void UT_CNATFWTurnConnectionHandler_IsRequestOrIndicationLL();
         
         
         void UT_CNATFWTurnConnectionHandler_IsTurnResponseLL();
         
         
         void UT_CNATFWTurnConnectionHandler_STUNClientInitCompletedL();
        
        
         void UT_CNATFWTurnConnectionHandler_STUNBindingEventOccurredLL();
        
        
         void UT_CNATFWTurnConnectionHandler_STUNBindingErrorOccurredL();
        
        
         void UT_CNATFWTurnConnectionHandler_IncomingMessageLL();
         
         
         void UT_CNATFWTurnConnectionHandler_OutgoingMessageLL();
         
         
         void UT_CNATFWTurnConnectionHandler_IncomingMessageNotifyL();
         
         
         void UT_CNATFWTurnConnectionHandler_OutgoingMessageNotifyL();
        
        
         void UT_CNATFWTurnConnectionHandler_ConnectionNotifyL();
         
         
         void UT_CNATFWTurnConnectionHandler_BindingRefreshLL();
         
         
         void UT_CNATFWTurnConnectionHandler_TimerTriggeredLL();
         
         
         void UT_CNATFWTurnConnectionHandler_DeleteStreamL();
         
         void StartActiveScheduler();
         
         void StopActiveScheduler();


    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;

        CNATFWTurnConnectionHandler* iConnHandler;
        
        CNATFWPluginApi* iTurnPlugin;

        CNcmConnectionMultiplexer* iConnMux;
        
        TBool iAlloc;
        
        TBool iAsyncTestCase;
        
        TStreamData iTestStreamData;
        
        CTestSettings* iSettings;
        
        TUint iSessionId;
        
        TUint iStreamId;
        
        TBool iActiveSchedulerStarted;
    
    public: 
        
        static CActiveSchedulerWait iActiveSchedulerWait;
    };

#endif      //  __UT_CNATFWTURNCONNECTIONHANDLER_H__

// End of file
