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




#ifndef __UT_CNcmConnectionMultiplexer_H__
#define __UT_CNcmConnectionMultiplexer_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <in_sock.h>

//  INTERNAL INCLUDES
#include "mncmconnectionmultiplexerobserver.h"
#include "mncmincomingconnectionobserver.h"
#include "mncmoutgoingconnectionobserver.h"
#include "mncmconnectionobserver.h"
#include "natfwconnectivityapidefs.h"
#include "mncmsenderobserver.h"
#include "mncmmessageobserver.h"


//  FORWARD DECLARATIONS
class CNcmConnectionMultiplexer;
class CTestingTimer;
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
NONSHARABLE_CLASS( UT_CNcmConnectionMultiplexer )
     : public CEUnitTestSuiteClass,
       public MNcmConnectionMultiplexerObserver,
       public MNcmOutgoingConnectionObserver,
       public MNcmIncomingConnectionObserver,
       public MNcmConnectionObserver,
       public MNcmSenderObserver,
       public MNcmMessageObserver
       
    {
public:     // Constructors and destructors

    /**
     * Two phase construction
     */
    static UT_CNcmConnectionMultiplexer* NewL();
    static UT_CNcmConnectionMultiplexer* NewLC();
    /**
     * Destructor
     */
    ~UT_CNcmConnectionMultiplexer();

private:    // Constructors and destructors

    UT_CNcmConnectionMultiplexer();
    void ConstructL();

    void WaitForNotify( TUint aStreamId, TUint aSessionId );

public:     // From observer interface

    /**
     * Called by Connection Multiplexer when actions for stream completes
     *
     * @since S60 v3.2
     * @param aStreamId which is target of notify
     * @param aType of notify
     * @return void
     */
    void Notify(  TUint aSessionId, TUint aStreamId, TNotifyType aType, TInt aError );

    //MNATFWConnectionObserver
    void ConnectionNotify( TUint aStreamId, TUint aConnectionId,
        TConnectionNotifyType aType, TInt aError );

    //MNATFWOutgoingConnectionObserver
    void OutgoingMessageL( TUint aStreamId, TUint aConnectionId,
        const TInetAddr& aDestinationAddress, const TDesC8& aMessage, TBool& aConsumed );

    //MNATFWIncomingConnectionObserver
    void IncomingMessageL( TUint aStreamId, const TDesC8& aMessage,
        const TInetAddr& aLocalAddr, const TInetAddr& aFromAddr,
        const TInetAddr& aPeerRemoteAddr, TBool& aConsumed );
  
    // MNcmMessageObserver  
    HBufC8* IncomingMessageNotify( TUint aStreamId,
        const TDesC8& aMessage, const TInetAddr& aLocalAddr,
        const TInetAddr& aFromAddr, TInetAddr& aPeerRemoteAddr );

    HBufC8* UT_CNcmConnectionMultiplexer::OutgoingMessageNotify( TUint aStreamId,
        TUint aConnectionId, const TInetAddr& aDestinationAddress,
        const TDesC8& aMessage );
          
          
    //MNATFWMultiplexerSenderObserver     
    void MessageSent();

    void MessageSentFailure( TInt aError );
        
private:    // New methods

    void SetupL();
     
    void Setup2L();

    void Teardown();
          
    void Teardown2();
     
    void UT_CNcmConnectionMultiplexer_NewLL();
    
    void UT_CNcmConnectionMultiplexer_CreateSessionLL();
     
    void UT_CNcmConnectionMultiplexer_RemoveSessionLL();
    
    void UT_CNcmConnectionMultiplexer_CreateStreamLL();

    void UT_CNcmConnectionMultiplexer_GetStreamInfoLL();
    
    void UT_CNcmConnectionMultiplexer_CreateConnectionLL();
    
    void UT_CNcmConnectionMultiplexer_CreateConnectionLL2();
     
    void UT_CNcmConnectionMultiplexer_LocalIPAddressLL();
    
    void UT_CNcmConnectionMultiplexer_SetSendingStateLL();

    void UT_CNcmConnectionMultiplexer_SetSendingStateForMediaLL();
            
    void UT_CNcmConnectionMultiplexer_SetReceivingStateLL();
    
    void UT_CNcmConnectionMultiplexer_SetReceivingStateL_TCP();

    void UT_CNcmConnectionMultiplexer_SetReceivingStateForMediaLL();
     
    void UT_CNcmConnectionMultiplexer_RemoveStreamLL();
    
    void UT_CNcmConnectionMultiplexer_RemoveConnectionLL();
     
    void UT_CNcmConnectionMultiplexer_RegisterIncomingConnectionObserverLL();
    
    void UT_CNcmConnectionMultiplexer_RegisterOutgoingConnectionObserverLL();
    
    void UT_CNcmConnectionMultiplexer_UnregisterIncomingConnectionObserverLL();
    
    void UT_CNcmConnectionMultiplexer_UnregisterOutgoingConnectionObserverLL();
       
    void UT_CNcmConnectionMultiplexer_RegisterConnectionObserverLL();
   
    void UT_CNcmConnectionMultiplexer_RegisterMessageObserverLL();
  
    void UT_CNcmConnectionMultiplexer_UnregisterConnectionObserverLL();   
     
    void UT_CNcmConnectionMultiplexer_UnregisterMessageObserverL();
             
    void UT_CNcmConnectionMultiplexer_RegisterMediaWrapperLL();
    
    void UT_CNcmConnectionMultiplexer_GetSessionInfoLL();

    void UT_CNcmConnectionMultiplexer_PortStoreL();
    
    void UT_CNcmConnectionMultiplexer_ActivateStreamReceivingLL();
    
    void UT_CNcmConnectionMultiplexer_ActivateStreamSendingLL();       

    void UT_CNcmConnectionMultiplexer_SendLL();
    
    void UT_CNcmConnectionMultiplexer_SendLL2();
     
    void UT_CNcmConnectionMultiplexer_SendLL3();
     
    void UT_CNcmConnectionMultiplexer_SendLL4();

    void UT_CNcmConnectionMultiplexer_CancelMessageSendL();
     
    void UT_CNcmConnectionMultiplexer_MultiplexerErrorL();
    
    void UT_CNcmConnectionMultiplexer_StreamByIdL();
    
    void UT_CNcmConnectionMultiplexer_ConnectionNotifyL();
       
    void UT_CNcmConnectionMultiplexer_WrapperConnectedL();
    
    void UT_CNcmConnectionMultiplexer_OpenTcpConnectionLL();
    
    void UT_CNcmConnectionMultiplexer_CloseTcpConnectionL();

    void UT_CNcmConnectionMultiplexer_ResolveDestinationAddressLL();

    void UT_CNcmConnectionMultiplexer_SetAcceptedFromAddressLL();
    
     /**
      * For usage with CPeriodic.
      */
     static TInt TimerFired( TAny* aObject );
           
private:    // Data

    EUNIT_DECLARE_TEST_TABLE; 

    CNcmConnectionMultiplexer* iConnMux;

    CActiveSchedulerWait* iScheduler;
    TInt iType;
    TInt iConnId;
    TUint iConnNotifyType;
    TInt iNotifyErr;
    TInt iConnNotifyErr;
    HBufC8* iIncomingMessage;

    // Is active
    TBool iActive;
    
    TUint iSessionId;
    TUint iStreamId;
    TUint iConnectionId;
    TUint iDefaultConnection;
    
    RSocketServ iTestSocketSrv;
    RConnection iTestConnection;
    RSocket iTestSocket;
    TInetAddr iTestAddress;  
    
    CNATFWSocketMediaConnWrapper* iMediaWrapper;   
    CPeriodic* iTimer;
    
    TBool iPacketReceived;
    };

#endif      //  __UT_CNcmConnectionMultiplexer_H__

// End of file
