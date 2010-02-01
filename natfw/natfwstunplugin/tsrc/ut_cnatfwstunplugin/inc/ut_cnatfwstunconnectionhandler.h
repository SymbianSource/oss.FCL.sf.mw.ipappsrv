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




#ifndef __UT_CNATFWSTUNCONNECTIONHANDLER_H__
#define __UT_CNATFWSTUNCONNECTIONHANDLER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "mnatfwpluginobserver.h"
#include "mncmconnectionmultiplexerobserver.h"

//  FORWARD DECLARATIONS
class CNATFWStunConnectionHandler;
class CNcmConnectionMultiplexer;
class CTestSettings;

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
NONSHARABLE_CLASS( UT_CNATFWStunConnectionHandler ) : 
    public CEUnitTestSuiteClass,
    public MNATFWPluginObserver,
    public MNcmConnectionMultiplexerObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWStunConnectionHandler* NewL();
        static UT_CNATFWStunConnectionHandler* NewLC();
        /**
         * Destructor
         */
        ~UT_CNATFWStunConnectionHandler();

    private:    // Constructors and destructors

        UT_CNATFWStunConnectionHandler();
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
        
         void Teardown();
         
         
         void UT_CNATFWStunConnectionHandler_ConnectServerLL();
         
         
         void UT_CNATFWStunConnectionHandler_TryNextServerLL();
         
         
         void UT_CNATFWStunConnectionHandler_ConnectionByIdL();
         

         void UT_CNATFWStunConnectionHandler_FetchCandidateLL();
         
         
         void UT_CNATFWStunConnectionHandler_StartStunRefreshL();
         
         
         void UT_CNATFWStunConnectionHandler_CreateSTUNBindingLL();
         
         
         void UT_CNATFWStunConnectionHandler_SetReceivingStateLL();
         
         
         void UT_CNATFWStunConnectionHandler_SetSendingStateLL();


         void UT_CNATFWStunConnectionHandler_GetConnectionIdLL();
         
         
         void UT_CNATFWStunConnectionHandler_STUNClientInitCompletedL();
         
         
         void UT_CNATFWStunConnectionHandler_STUNClientInitCompleted_AllocL();
         
         
         void UT_CNATFWStunConnectionHandler_STUNBindingEventOccurredLL();
        
        
         void UT_CNATFWStunConnectionHandler_STUNBindingErrorOccurredL();
        
         
         void UT_CNATFWStunConnectionHandler_STUNBindingErrorOccurred_AllocL();
         
        
         void UT_CNATFWStunConnectionHandler_IncomingMessageLL();
        
        
         void UT_CNATFWStunConnectionHandler_ConnectionNotifyL();
         
         
         void UT_CNATFWStunConnectionHandler_BindingRefreshLL();
         
         
         void UT_CNATFWStunConnectionHandler_DeleteStreamL();


    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;

        CNATFWStunConnectionHandler* iConnHandler;
        
        CNATFWPluginApi* iStunPlugin;

        CNcmConnectionMultiplexer* iConnMux;
        
        CTestSettings* iSettings;
        
        TBool iAsyncTestCase;

        TUint iSessionId;
        
        TUint iStreamId;
        
        CNATFWCandidate* iLocalCandidate;
    };

#endif      //  __UT_CNATFWSTUNCONNECTIONHANDLER_H__

// End of file
