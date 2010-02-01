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




#ifndef __UT_CSTUNCLIENT_H__
#define __UT_CSTUNCLIENT_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES

#include "cncmconnectionmultiplexer.h"
#include "mncmconnectionmultiplexerobserver.h"
#include "natfwstunclientobserver.h"

//  FORWARD DECLARATIONS
class CSTUNClient;

#include <e32def.h>
#include <es_sock.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
NONSHARABLE_CLASS( ut_cstunclient )
     : public CEUnitTestSuiteClass, public MSTUNClientObserver, 
       public MNcmConnectionMultiplexerObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static ut_cstunclient* NewL();
        static ut_cstunclient* NewLC();
        /**
         * Destructor
         */
        ~ut_cstunclient();

    private:    // Constructors and destructors

        ut_cstunclient();
        void ConstructL();

    public:     // From mstunclientobserver

        void STUNClientInitCompleted( const CSTUNClient& aClient,
                                      TInt aCompletionCode );   
    

        void STUNBindingEventOccurredL( TSTUNBindingEvent aEvent, 
                                        const CBinding& aBinding );


        void STUNBindingErrorOccurred( const CBinding& aBinding, 
                                       TInt aError );

    public:     // From MNcmConnectionMultiplexerObserver
    
        void Notify( TUint aSessionId, TUint aStreamId, TNotifyType aType, TInt aError );
        void IcmpError( TUint aSessionId, TUint aStreamId, const TInetAddr& aAddress );
        
    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CSTUNClient_NewLL();
        
        
         void UT_CSTUNClient_NewL_1L();
         
         
         void UT_CSTUNClient_NewL_2L();
        
        
         void UT_CSTUNClient_NewLCL();
        
        
         void UT_CSTUNClient_NewLC_1L();
        
        
         void UT_CSTUNClient_IsInitializedL();
        
        
         void UT_CSTUNClient_STUNServerAddrLL();
        
        
         void UT_CSTUNClient_SetCredentialsLL();
        
        
         void UT_CSTUNClient_SharedSecretObtainedL();
        
        
         //void UT_CSTUNClient_SetSTUNServerAddrLL();
        
        
         void UT_CSTUNClient_ObtainTransactionIDLL();
        
        
         void UT_CSTUNClient_ImplementationL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;
        
        RSocketServ iSocketServ;
        RConnection iConnection;
        CDeltaTimer* iDeltatimer;
        CSTUNClient* iStunclient;
        TUint iStreamId;
        TUint iSubstreamId;
        HBufC8* iServeraddress;
        HBufC8* iResponse;
        HBufC8* iRespDataIndication;
        CNcmConnectionMultiplexer* iMux;
        RSocket iSocket;
        TRequestStatus iStatus; 
        TBool iCallbackCalled;

    };

#endif      //  __UT_CSTUNCLIENT_H__

// End of file
