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




#ifndef __UT_CNCMCONNECTION_H__
#define __UT_CNCMCONNECTION_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "cncmconnection.h"
#include "mncmmultiplexerconnectionobserver.h"

//  FORWARD DECLARATIONS


//  CLASS DEFINITION
/**
 * TODO Auto-generated EUnit test suite
 *
 */
NONSHARABLE_CLASS( UT_CNcmConnection )
	: public CEUnitTestSuiteClass,
	  public MNcmMultiplexerConnectionObserver
	  
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNcmConnection* NewL();
        static UT_CNcmConnection* NewLC();
        /**
         * Destructor
         */
        ~UT_CNcmConnection();

    private:    // Constructors and destructors

        
        UT_CNcmConnection();
        void ConstructL();

    private:    // New methods

        void SetupUdpL();

        void SetupTcpL();

        void Teardown();
         
        void UT_CNcmConnection_NewLL(); 
        
        void UT_CNcmConnection_TCP_NewLL();  
         
        void UT_CNcmConnection_SetReceivingStateLL();
        
        void UT_CNcmConnection_SetSendingStateLL();
        
        void UT_CNcmConnection_OpenTcpConnectionLL();
        
        void UT_CNcmConnection_CloseTcpConnectionLL();        
         
        void UT_CNcmConnection_TCP_SetReceivingSendingStateLL();
         
        void UT_CNcmConnection_SendL();
                      
        void UT_CNcmConnection_TCP_SendL();   
        
        void UT_CNcmConnection_CheckSenderValidityToSendingL();


// From MNcmMultiplexerConnectionObserver    
    void ConnectionError( TUint aSessionId, TUint aStreamId,
        TUint aConnectionId,
        TMultiplexerConnectionNotifyType aNotifyType, TInt aError );
    
    void ConnectionNotify( TUint aStreamId, TUint aConnectionId,
        TMultiplexerConnectionNotifyType aType, TInt aError );
  
    void GetNewFreePort( TUint aStreamId, TUint aConnectionId,
        TUint& aPort );
        
    private:    // Data
		
        CNcmConnection* iConnection;
        CNcmConnectionObserverHandler* iObserverHandler;
        
        TInetAddr iAddress;
        TInetAddr iTestAddress;
        
        RSocketServ iSocketSrv;
        RConnection iConn;
        RSocket iSocket;

        RSocketServ iTestSocketSrv;
        RConnection iTestConnection;
        RSocket iTestSocket;
                
        TBool iActive;
        
        EUNIT_DECLARE_TEST_TABLE; 

    };

#endif      //  __UT_CNCMCONNECTION_H__

// End of file
