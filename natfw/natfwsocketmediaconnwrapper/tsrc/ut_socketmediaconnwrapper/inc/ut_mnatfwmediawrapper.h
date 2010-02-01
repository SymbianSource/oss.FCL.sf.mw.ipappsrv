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




#ifndef __UT_MNATFWMEDIAWRAPPERIF_H__
#define __UT_MNATFWMEDIAWRAPPERIF_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "cncmlocaladdressresolver.h"
#include "natfwsocketmediaconnwrapper.h"
#include "natfwmediawrapperobserver.h"
#include "mnsmcwtcpconnectionobserver.h"


//  FORWARD DECLARATIONS
class MNATFWMediaWrapper;
class MNATFWSocketMediaConnWrapper;

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif


enum TMessage
    {
    EEmpty = 0,
    EConnectingCompleted,
    ESendingCompleted,
    EReceivedFrom
    };

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
NONSHARABLE_CLASS( UT_MNATFWMediaWrapper )
     :  public CEUnitTestSuiteClass,
        public MNATFWMediaWrapperObserver,
        public MNsmcwTcpConnectionObserver
    {

    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_MNATFWMediaWrapper* NewL();
        static UT_MNATFWMediaWrapper* NewLC();
        /**
         * Destructor
         */
        ~UT_MNATFWMediaWrapper();

    private:    // Constructors and destructors

        UT_MNATFWMediaWrapper();
        
        void ConstructL();

    public:     // From MNATFWMediaWrapperObserver   
            
        void SendingCompleted( const TInt aError );
        
        TInt ReceivedFrom( const TUint aStreamId, TDes8& aStreamPortion );
        
        void GetNewFreePort( TUint& aPort );
        
// From MNATFWSocketMediaConnWrapper

        void ConnectingCompleted( const TInt aError );
        

    private:    // New methods

         void SetupL();
         
         
         void Setup2L();
        
        
         void Teardown();
        
        
         void UT_MNATFWMediaWrapper_SetSendingStateLL();
         
         
         void UT_MNATFWMediaWrapper_SetSendingStateL_TcpL();
         
         
         void UT_MNATFWMediaWrapper_SetReceivingStateLL();
        
         
         void UT_MNATFWMediaWrapper_SetReceivingStateL_TcpL();
         
        
         void UT_MNATFWMediaWrapper_SendLL();
         
         
         void UT_MNATFWMediaWrapper_SendL_TcpL();
         
         
         /*void UT_MNATFWMediaWrapper_OpenTcpConnectionLL();
        
        
         void UT_MNATFWMediaWrapper_CloseTcpConnectionSetupL();*/
         
        
         void UT_MNATFWMediaWrapper_StreamIdL();
        
        
         void UT_MNATFWMediaWrapper_RegisterMediaWrapperObserverL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;
        
       // UT_CWrapperActiveTester *iWrapperTester;
        CActiveSchedulerWait *iWaitScheduler;
        CNATFWSocketMediaConnWrapper *iMediaConnWrapper;
        MNATFWMediaWrapper *iMediaWrapper;
        MNATFWSocketMediaConnWrapper *iSocketMediaConnWrapper;
        RSocketServ iSocketSrv;
        
        TMessage iMessage;
        TInt iError;
        TUint iStreamId;
        TUint iLength;
        TBuf8<40> iReceiveBuffer;
        TBuf8<128> iData;
        TSockAddr iAddress;
        TRequestStatus iStatus;
        TInetAddr iWrapperLocalAddr;
        TInetAddr iSenderAddr;
        RConnection iConn; 
        RSocket iSocket;
                
    };

#endif      //  __UT_MNATFWMEDIAWRAPPERIF_H__

// End of file
