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




#ifndef __UT_CNATFWSOCKETHANDLER_H__
#define __UT_CNATFWSOCKETHANDLER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "natfwsocketreceiverobserver.h"
#include "natfwsocketsenderobserver.h"
#include "natfwmediawrapperobserver.h"
#include "mnsmcwtcpconnectionobserver.h"
 
//  FORWARD DECLARATIONS
class CNATFWSocketHandler;

#include <e32def.h>
#include <es_sock.h>
#include <in_sock.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
NONSHARABLE_CLASS( ut_cnatfwsockethandler )
     : public CEUnitTestSuiteClass, public MNATFWSocketReceiverObserver,
       public MNATFWSocketSenderObserver,
       public MNATFWMediaWrapperObserver,
       public MNsmcwTcpConnectionObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static ut_cnatfwsockethandler* NewL();
        static ut_cnatfwsockethandler* NewLC();
        /**
         * Destructor
         */
        ~ut_cnatfwsockethandler();

    private:    // Constructors and destructors

        ut_cnatfwsockethandler();
        void ConstructL();

    public:     // From observer interface
    
        // MNATFWSocketReceiverObserver
        void DeliverBuffer( TDes8& /*aStreamPortion*/ ){};
        
        // MNsmcwIfObserver
        void ConnectingCompleted( TInt /*aError*/ ){};
        
        void SendingCompleted( TInt /*aError*/ ){};
        
        // MNATFWMediaWrapperObserver
        //void SendingCompleted( const TInt /*aError*/ ){};
        
        TInt ReceivedFrom( TUint /*aStreamId*/, TDes8& /*aStreamPortion*/ ){ return 0;};
        
        void GetNewFreePort( TUint& aPort );
        
        
    private:    // New methods

         void SetupL();
         
         void Setup2L();
        
         void Teardown();        
        
         void ut_cnatfwsockethandler_NewLL();
                
         void ut_cnatfwsockethandler_LocalAddressL();
        
        
         void ut_cnatfwsockethandler_SetAddrLL();
        
        
         void ut_cnatfwsockethandler_SetAddrL2L();
        
        
         void ut_cnatfwsockethandler_ConnectLL();
        
        
         void ut_cnatfwsockethandler_StartListeningLL();
        
        
         void ut_cnatfwsockethandler_SendLL();
        
        
         void ut_cnatfwsockethandler_SetLocalAddressL();
        
        
         void ut_cnatfwsockethandler_DeactivateSendingL();
        
        
         void ut_cnatfwsockethandler_DeactivateReceivingL();
         
         
         void ut_cnatfwsockethandler_SetupTcpConnectionL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;
        
        CNATFWSocketHandler* iHandler;
        RSocketServ iSocketServer;
        TInetAddr iAddr; 

    };

#endif      //  __UT_CNATFWSOCKETHANDLER_H__

// End of file
