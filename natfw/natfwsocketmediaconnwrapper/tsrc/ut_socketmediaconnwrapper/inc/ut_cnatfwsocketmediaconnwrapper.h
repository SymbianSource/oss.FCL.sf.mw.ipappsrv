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




#ifndef __UT_CNATFWSOCKETMEDIACONNWRAPPER_H__
#define __UT_CNATFWSOCKETMEDIACONNWRAPPER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "natfwsocketmediaconnwrapper.h"
#include "natfwmediawrapperobserver.h"

//  FORWARD DECLARATIONS
class CNATFWSocketMediaConnWrapper;

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
NONSHARABLE_CLASS( ut_cnatfwsocketmediaconnwrapper )
     : public CEUnitTestSuiteClass,
        public MNATFWMediaWrapperObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static ut_cnatfwsocketmediaconnwrapper* NewL();
        static ut_cnatfwsocketmediaconnwrapper* NewLC();
        /**
         * Destructor
         */
        ~ut_cnatfwsocketmediaconnwrapper();
        
        
    public:     // From MNATFWMediaWrapperObserver   
            
        void SendingCompleted( const TInt aError );
        
        TInt ReceivedFrom( const TUint aStreamId, TDes8& aStreamPortion );
        
        void GetNewFreePort( TUint& aPort );
    

    private:    // Constructors and destructors

        ut_cnatfwsocketmediaconnwrapper();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void ut_cnatfwsocketmediaconnwrapper_NewLL();
        
        
         void ut_cnatfwsocketmediaconnwrapper_StreamIdL();
        
        
         void ut_cnatfwsocketmediaconnwrapper_OutgoingAddrL();
        
        
         void ut_cnatfwsocketmediaconnwrapper_SetIncomingAddrLL();
        
        
         void ut_cnatfwsocketmediaconnwrapper_ConnectLL();
        
        
         void ut_cnatfwsocketmediaconnwrapper_StartListeningLL();
        
        
         void ut_cnatfwsocketmediaconnwrapper_SendLL();
        
        
         void ut_cnatfwsocketmediaconnwrapper_DeactivateSendingL();
        
        
         void ut_cnatfwsocketmediaconnwrapper_DeactivateReceivingL();
        
        
         void ut_cnatfwsocketmediaconnwrapper_RegisterMediaWrapperObserverL();
        
        
         void ut_cnatfwsocketmediaconnwrapper_CloseL();
        
        
         void ut_cnatfwsocketmediaconnwrapper_SendingCompletedL();
        
        
         void ut_cnatfwsocketmediaconnwrapper_DeliverBufferL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        CNATFWSocketMediaConnWrapper *iConnWrapper;
        RSocketServ iSocketServer;
        TInt iError;
        TUint iStreamId;
    };

#endif      //  __UT_CNATFWSOCKETMEDIACONNWRAPPER_H__

// End of file
