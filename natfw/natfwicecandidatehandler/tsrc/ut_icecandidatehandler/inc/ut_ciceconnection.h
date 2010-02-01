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




#ifndef __UT_CICECONNECTION_H__
#define __UT_CICECONNECTION_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "mncmconnectionmultiplexerobserver.h"
#include "miceconnectionobserver.h"

//  FORWARD DECLARATIONS
class CNcmConnectionMultiplexer;
class CIceConnection;

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
NONSHARABLE_CLASS( UT_CIceConnection )
        :
        public CEUnitTestSuiteClass, 
        public MNcmConnectionMultiplexerObserver, 
        public MIceConnectionObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CIceConnection* NewL();
        static UT_CIceConnection* NewLC();
        /**
         * Destructor
         */
        ~UT_CIceConnection();

    private:    // Constructors and destructors

        UT_CIceConnection();
        void ConstructL();


    public:
    // From MNcmConnectionMultiplexerObserver
        void Notify( TUint aSessionId, TUint aStreamId,
            MNcmConnectionMultiplexerObserver::TNotifyType aType, TInt aError );
        
    // From MIceConnectionObserver
        
        void ConnectionNotify( CIceConnection& aConnection,
            MIceConnectionObserver::TNotifyType aType, TInt aError );
        
    private:    // New methods

         void SetupL();
        
         void Teardown();

         void UT_CIceConnection_StreamIdL();

         void UT_CIceConnection_StateL();
         
         void UT_CIceConnection_TransportProtocolL();
            
         void UT_CIceConnection_LocalAddressLL();

         void UT_CIceConnection_SetReceivingStateLL();

         void UT_CIceConnection_SetSendingStateLL();
         
         void UT_CIceConnection_BindWithCandidateLL();
         
         void UT_CIceConnection_IsBoundToCandidateL();


    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CIceConnection* iConnection;
        
        CNcmConnectionMultiplexer* iMultiplexer;
        
        TUint iSessionId;
        TUint iStreamId;
        
        MIceConnectionObserver::TNotifyType iLatestNotifyType;
        TInt iLatestError;
    };

#endif      //  __UT_CICECONNECTION_H__

// End of file
