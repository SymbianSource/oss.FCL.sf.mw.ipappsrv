/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef __UT_CSTUNSHAREDSECRET_H__
#define __UT_CSTUNSHAREDSECRET_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/EUnitDecorators.h>
#include <es_sock.h>
#include <in_sock.h>


//  INTERNAL INCLUDES
#include "cstunsharedsecret.h"
#include "msharedsecretobserver.h"

//  FORWARD DECLARATIONS


//  CLASS DEFINITION
/**
 * TODO Auto-generated EUnit test suite
 *
 */
NONSHARABLE_CLASS( ut_cstunsharedsecret )
	: public CEUnitTestSuiteClass, public MSharedSecretObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static ut_cstunsharedsecret* NewL();
        static ut_cstunsharedsecret* NewLC();
        /**
         * Destructor
         */
        ~ut_cstunsharedsecret();

    private:    // Constructors and destructors

        ut_cstunsharedsecret();
        void ConstructL();
        
        void SharedSecretObtainedL() {};

        void SharedSecretErrorL( TInt /*aError*/ ) {};

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void ut_cstunsharedsecret_CSTUNSharedSecretL();
        
         void ut_cstunsharedsecret_TimerExpiredLL();
        
         void ut_cstunsharedsecret_LeaveFromTimerExpiredL();
        
         void ut_cstunsharedsecret_TlsConnectedLL();
        
         void ut_cstunsharedsecret_IncomingMessageLL();
        
         void ut_cstunsharedsecret_ErrorOccuredL();
        
         void ut_cstunsharedsecret_UsernameL();
        
         void ut_cstunsharedsecret_PasswordL();
        
         void ut_cstunsharedsecret_ChangeStateL();
        
         void ut_cstunsharedsecret_TerminateL();
        
         void ut_cstunsharedsecret_SendRequestLL();
        
         void ut_cstunsharedsecret_CheckMessageL();
        
         void ut_cstunsharedsecret_ResponseReceivedLL();
        
         void ut_cstunsharedsecret_ErrorResponseReceivedLL();
        
         void ut_cstunsharedsecret___DbgTestInvariantL();
        

    private:    // Data
		
        CSTUNSharedSecret* iCSTUNSharedSecret;
        CTransactionIDGenerator* iIdGenerator;

        RSocketServ iSocketServ;
        RConnection iConnection;
        CDeltaTimer* iDeltatimer;
        TRequestStatus iStatus;
        TInetAddr iServerAddr;
                
        EUNIT_DECLARE_TEST_TABLE; 

    };

#endif      //  __UT_CSTUNSHAREDSECRET_H__

// End of file
