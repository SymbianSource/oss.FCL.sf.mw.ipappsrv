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




//  CLASS HEADER
#include "ut_cstuntransaction.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <digia/eunit/EUnitDecorators.h>


//  INTERNAL INCLUDES
#include "cstuntransaction.h"
#include "natfwunsafbindingrequest.h"
#include "natfwunsafbindingresponse.h"
#include "natfwunsafsharedsecreterrorresponse.h"
#include "natfwunsafbindingerrorresponse.h"
#include "natfwunsaferrorcodeattribute.h"
#include "natfwunsafusernameattribute.h"
#include "natfwunsafmessagefactory.h"
#include "natfwunsafmappedaddressattribute.h"
#include "natfwunsafxormappedaddressattribute.h"
#include "natfwunsafallocateresponse.h"
#include "natfwunsafconnectresponse.h"
#include "natfwunsafsetactivedestinationresponse.h"
#include "natfwunsafrelayaddressattribute.h"
#include "natfwunsafconnectrequest.h"
#include "natfwunsafsetactivedestinationrequest.h"
#include "natfwunsafallocaterequest.h"
#include "natfwunsafallocateerrorresponse.h"
#include "natfwunsafconnecterrorresponse.h"
#include "natfwunsafsetactivedestinationerrorresponse.h"
#include "natfwunittestmacros.h"

_LIT( KServerAddressi, "10.32.194.251" );
_LIT8( KPasswordi,"salasana");
_LIT8( KErr, "ERSKA" );

const TBufC8<8> passwordi( KPasswordi );

TNATFWUNSAFTransactionID transactionIDi;

// CONSTRUCTION
ut_cstuntransaction* ut_cstuntransaction::NewL()
    {
    ut_cstuntransaction* self = ut_cstuntransaction::NewLC();
    CleanupStack::Pop();

    return self;
    }

ut_cstuntransaction* ut_cstuntransaction::NewLC()
    {
    ut_cstuntransaction* self = new( ELeave ) ut_cstuntransaction();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
ut_cstuntransaction::~ut_cstuntransaction()
    {
    }

// Default constructor
ut_cstuntransaction::ut_cstuntransaction()
    {
    }

// Second phase construct
void ut_cstuntransaction::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void ut_cstuntransaction::Notify( TUint /*aSessionId*/, TUint /*aStreamId*/, 
    TNotifyType /*aType*/, TInt /*aError*/ )
    {
    RDebug::Print( _L("**NATFW Notify") );
    }
    
void ut_cstuntransaction::IcmpError( TUint /*aSessionId*/, TUint /*aStreamId*/, const TInetAddr& /*aAddress*/ )
    {
    RDebug::Print( _L("**NATFW IcmpError") );
    }


void ut_cstuntransaction::SetupL(  )
    {
    TInetAddr destAddr( KProtocolInetUdp );
    destAddr.Input(_L("10.10.10.10"));
    destAddr.SetPort( 250 );
      
    iTransportProtocol = EUdpProtocol;
    TBufC8<4> service( KStun );
    iStreamId = 1;
    iSubstreamId = 1;
    iInterval = 1;        
    iServerAddr.Input( KServerAddressi );
    
    iDeltatimer = CDeltaTimer::NewL( 1 ); 
    iRequest = CNATFWUNSAFBindingRequest::NewL( transactionIDi );
    iMux = CNcmConnectionMultiplexer::NewL( *this );    
    
    iCSTUNTransaction = CSTUNTransaction::NewL( *iRequest, iServerAddr, passwordi, 
        iStreamId, iSubstreamId, *iDeltatimer, iInterval, *this, service, *iMux, 
        iTransportProtocol, EFalse, destAddr );
    }
    

void ut_cstuntransaction::Teardown(  )
    {
    delete iCSTUNTransaction;
    delete iDeltatimer;
    delete iRequest;
    delete iMux; 
    }

// Encodes messages
HBufC8* ut_cstuntransaction::EncodeMessageL( 
    const CNATFWUNSAFMessage& aRequest, const TBool aUseSharedSecret,
    const TBool aAddFingerPrint )
    {
    CBufBase* message = NULL;       
         
    if ( !aUseSharedSecret )
        {
        message = aRequest.EncodeL(); 
        }
    else if ( !aAddFingerPrint )
        {
        message = aRequest.EncodeL( KPasswordi );
        }
    else
        {
        // ETrue indicates that fingerprint attribute will be added
        message = aRequest.EncodeL( KPasswordi, ETrue );
        }
        
    CleanupStack::PushL( message );
    TInt length = message->Size();
    HBufC8* readBuf = HBufC8::NewLC( length );
    TPtr8 writable( readBuf->Des() );
    message->Read( 0, writable, length );

    CleanupStack::Pop( readBuf );
    CleanupStack::PopAndDestroy( message );
    
    return readBuf;    
    }
    
void ut_cstuntransaction::ut_cstuntransaction_NewL()
    {
    RSocket socket;
    RSocketServ srv;
    
    User::LeaveIfError( srv.Connect() );
    CleanupClosePushL( srv );
    User::LeaveIfError( socket.Open( srv ) );
    CleanupClosePushL( socket );
    
    TBufC8<4> service( KStun );
    
    CSTUNTransaction* transaction = CSTUNTransaction::NewL( *iRequest, iServerAddr, passwordi, 
        socket, *iDeltatimer, iInterval, *this, service, iTransportProtocol );    
    delete transaction;
    transaction= NULL;
    
    TInetAddr addr;
 	NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE( transaction = CSTUNTransaction::NewL( *iRequest, addr,
 		passwordi, socket, *iDeltatimer, iInterval, *this, service, iTransportProtocol ), KErrArgument );   
    
    delete transaction;
    transaction= NULL;
    
 	NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE( transaction = CSTUNTransaction::NewL( *iRequest, addr, 
 		passwordi, iStreamId, iSubstreamId, *iDeltatimer, iInterval, *this, service, *iMux, iTransportProtocol ), KErrArgument );   
        
    CleanupStack::PopAndDestroy( &socket );
    CleanupStack::PopAndDestroy( &srv );
    }
    

void ut_cstuntransaction::ut_cstuntransaction__CSTUNTransactionL(  )
    {
    /*CSTUNTransaction* dummy = new ( ELeave ) CSTUNTransaction( *iCSTUNTransaction );
    delete iCSTUNTransaction; 
    //dummy->StopTimer();
    dummy->~CSTUNTransaction( );
    */    
    delete iCSTUNTransaction->iMediaConSender;
    iCSTUNTransaction->iMediaConSender = NULL;
    iCSTUNTransaction->~CSTUNTransaction( );
    
    /*CSTUNTransaction* dummy = new ( ELeave ) CSTUNTransaction( );
    //dummy->~CSTUNTransaction( );
    delete dummy;*/
    }
    
void ut_cstuntransaction::ut_cstuntransaction_TimerExpiredLL(  )
    {
    iCSTUNTransaction->TimerExpiredL( );
    
    CNATFWUNSAFBindingResponse* response = CNATFWUNSAFBindingResponse::NewLC( transactionIDi );   
    HBufC8* message = EncodeMessageL( *response );
    CleanupStack::PushL( message );
    
    iCSTUNTransaction->iTransportProtocol = ETcpProtocol;
    iCSTUNTransaction->TimerExpiredL( );    
    
    iCSTUNTransaction->iTerminated = EFalse;        
    iCSTUNTransaction->ReceiveL( *response, *message  );
    
    CleanupStack::PopAndDestroy( message );
    CleanupStack::PopAndDestroy( response );
    
    iCSTUNTransaction->iTransportProtocol = EUdpProtocol;
    iCSTUNTransaction->TimerExpiredL( );    
    }
    
void ut_cstuntransaction::ut_cstuntransaction_LeaveFromTimerExpiredL(  )
    {
    iCSTUNTransaction->LeaveFromTimerExpired( KErrTimedOut );
    }
    
void ut_cstuntransaction::ut_cstuntransaction_UNSAFUdpMessageSentLL(  )
    {
    iCSTUNTransaction->UNSAFUdpMessageSentL( );
    
    iCSTUNTransaction->StopTimer();
    iCSTUNTransaction->iTransportProtocol = ETcpProtocol;
    iCSTUNTransaction->UNSAFUdpMessageSentL( );
    
    iCSTUNTransaction->StopTimer();
    iCSTUNTransaction->iRetransmit = EFalse;
    iCSTUNTransaction->UNSAFUdpMessageSentL( );
    
    iCSTUNTransaction->StopTimer();
    iCSTUNTransaction->Terminate( KErrNone );
    iCSTUNTransaction->UNSAFUdpMessageSentL( );    
    }
    
void ut_cstuntransaction::ut_cstuntransaction_UNSAFUdpMessageFailureL(  )
    {
    iCSTUNTransaction->UNSAFUdpMessageFailure( KErrArgument );
    
    iCSTUNTransaction->iTerminated = ETrue;
    iCSTUNTransaction->UNSAFUdpMessageFailure( KErrArgument );
    }
    
void ut_cstuntransaction::ut_cstuntransaction_MessageSentL(  )
    {
    iCSTUNTransaction->MessageSent( );
    }
    
void ut_cstuntransaction::ut_cstuntransaction_MessageSentFailureL(  )
    {
    iCSTUNTransaction->MessageSentFailure( KErrTimedOut );
    }
    
void ut_cstuntransaction::ut_cstuntransaction_SendRequestLL(  )
    {
    iCSTUNTransaction->SendRequestL( );
    
    TInetAddr addr;
    iCSTUNTransaction->iDestAddr = addr;
    iCSTUNTransaction->SendRequestL( );
       
    RSocket socket;
    RSocketServ srv;
    User::LeaveIfError( srv.Connect() );
    CleanupClosePushL( srv );
    User::LeaveIfError( socket.Open( srv ) ); 
    CleanupClosePushL( socket );
    TBufC8<4> service( KStun );
    
    CSTUNTransaction* transaction = CSTUNTransaction::NewL( *iRequest, iServerAddr, passwordi, 
        socket, *iDeltatimer, iInterval, *this, service, iTransportProtocol );
                
    delete transaction;
    CleanupStack::PopAndDestroy( &socket );
    CleanupStack::PopAndDestroy( &srv );
            
    delete iCSTUNTransaction->iMediaConSender;
    iCSTUNTransaction->iMediaConSender = NULL;
    
    NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE( iCSTUNTransaction->SendRequestL( ), KErrNotReady );
    }
    
/*void ut_cstuntransaction::ut_cstuntransaction_RequestSentL(  )
    {
    iCSTUNTransaction->RequestSent( ETrue );
    }*/
    
void ut_cstuntransaction::ut_cstuntransaction_RetransmitRequestLL(  )
    {
    iCSTUNTransaction->RetransmitRequestL( );
    
    iCSTUNTransaction->iDisableSending = ETrue;
    iCSTUNTransaction->RetransmitRequestL( );
    
    iCSTUNTransaction->iSendCount = 7;
    iCSTUNTransaction->RetransmitRequestL( );    
    }
    
void ut_cstuntransaction::ut_cstuntransaction_ReceiveLL(  )
    {
    CNATFWUNSAFBindingResponse* response = CNATFWUNSAFBindingResponse::NewLC( transactionIDi );
    
    HBufC8* message = EncodeMessageL( *response );
    CleanupStack::PushL( message );
    iCSTUNTransaction->Terminate( KErrNone );    
    iCSTUNTransaction->ReceiveL( *response, *message  );
    CleanupStack::PopAndDestroy( message );
    CleanupStack::PopAndDestroy( response );
    iCSTUNTransaction->iTerminated = EFalse;
    
    CNATFWUNSAFBindingErrorResponse* errorresponse =
        CNATFWUNSAFBindingErrorResponse::NewLC( transactionIDi );
    errorresponse->AddAttributeL( CNATFWUNSAFErrorCodeAttribute::NewLC( 401, KErr ) );   
    CleanupStack::Pop(); // CNATFWUNSAFErrorCodeAttribute
    
    message = EncodeMessageL( *errorresponse );
    CleanupStack::PushL( message );
    iCSTUNTransaction->ReceiveL( *errorresponse, *message  );
    iCSTUNTransaction->iTerminated = EFalse;
    iCSTUNTransaction->iTransportProtocol = ETcpProtocol;
    iCSTUNTransaction->ReceiveL( *errorresponse, *message  );
    CleanupStack::PopAndDestroy( message );
    CleanupStack::PopAndDestroy( errorresponse ); 

    iCSTUNTransaction->iTerminated = EFalse;
    iCSTUNTransaction->iTransportProtocol = EUdpProtocol;
    
    CNATFWUNSAFAllocateResponse* allocateResponse = CNATFWUNSAFAllocateResponse::NewLC( transactionIDi );   
    message = EncodeMessageL( *allocateResponse );
    CleanupStack::PushL( message );
    iCSTUNTransaction->ReceiveL( *allocateResponse, *message  );
    CleanupStack::PopAndDestroy( message );   
    CleanupStack::PopAndDestroy( allocateResponse );
    
    CNATFWUNSAFConnectResponse* connectResponse = CNATFWUNSAFConnectResponse::NewLC( transactionIDi );   
    message = EncodeMessageL( *connectResponse );
    CleanupStack::PushL( message );
    iCSTUNTransaction->ReceiveL( *connectResponse, *message  );
    CleanupStack::PopAndDestroy( message );   
    CleanupStack::PopAndDestroy( connectResponse );
    
    CNATFWUNSAFSetActiveDestinationResponse* activeResponse = CNATFWUNSAFSetActiveDestinationResponse::NewLC( transactionIDi );   
    message = EncodeMessageL( *activeResponse );
    CleanupStack::PushL( message );
    iCSTUNTransaction->ReceiveL( *activeResponse, *message  );
    CleanupStack::PopAndDestroy( message );    
    CleanupStack::PopAndDestroy( activeResponse );
    
    CNATFWUNSAFBindingErrorResponse* bindingerrorresponse = CNATFWUNSAFBindingErrorResponse::NewLC( transactionIDi );
    message = EncodeMessageL( *bindingerrorresponse );
    CleanupStack::PushL( message );
    iCSTUNTransaction->ReceiveL( *bindingerrorresponse, *message  );
    CleanupStack::PopAndDestroy( message );
    CleanupStack::PopAndDestroy( bindingerrorresponse ); 
    
    CNATFWUNSAFAllocateErrorResponse* allocateErrorResponse = CNATFWUNSAFAllocateErrorResponse::NewLC( transactionIDi );
    message = EncodeMessageL( *allocateErrorResponse );
    CleanupStack::PushL( message );
    iCSTUNTransaction->ReceiveL( *allocateErrorResponse, *message  );
    CleanupStack::PopAndDestroy( message );
    CleanupStack::PopAndDestroy( allocateErrorResponse );
    
    CNATFWUNSAFConnectErrorResponse* connectErrorResponse =  CNATFWUNSAFConnectErrorResponse::NewLC( transactionIDi );
    message = EncodeMessageL( *connectErrorResponse );
    CleanupStack::PushL( message );
    iCSTUNTransaction->ReceiveL( *connectErrorResponse, *message  );
    CleanupStack::PopAndDestroy( message );
    CleanupStack::PopAndDestroy( connectErrorResponse );
    
    CNATFWUNSAFSetActiveDestinationErrorResponse* activeErrorResponse = CNATFWUNSAFSetActiveDestinationErrorResponse::NewLC( transactionIDi );
    message = EncodeMessageL( *activeErrorResponse );
    CleanupStack::PushL( message );
    iCSTUNTransaction->ReceiveL( *activeErrorResponse, *message  );
    CleanupStack::PopAndDestroy( message );
    CleanupStack::PopAndDestroy( activeErrorResponse );        
    }
    
/*void ut_cstuntransaction::ut_cstuntransaction_HasReceivedResponseL(  )
    {
    iCSTUNTransaction->HasReceivedResponse( );
     EUNIT_ASSERT_DESC( EFalse, "Generated assert, replace with real");
    }*/
    
void ut_cstuntransaction::ut_cstuntransaction_TerminateL(  )
    {
    transactionIDi.Zero();
    transactionIDi.Append(TUint8(0));
    transactionIDi.Append(TUint8(1));
    transactionIDi.Append(TUint8(2));
    transactionIDi.Append(TUint8(3));
    transactionIDi.Append(TUint8(4));
    transactionIDi.Append(TUint8(5));
    transactionIDi.Append(TUint8(6));
    transactionIDi.Append(TUint8(7));
    transactionIDi.Append(TUint8(8));
    transactionIDi.Append(TUint8(9));
    transactionIDi.Append(TUint8(0xa));
    transactionIDi.Append(TUint8(0xb));
    
    iCSTUNTransaction->Terminate( 500 );
    
    iCSTUNTransaction->iTerminated = EFalse;
    iCSTUNTransaction->Terminate( KErrNone );
    
    CNATFWUNSAFBindingResponse* response = CNATFWUNSAFBindingResponse::NewLC( transactionIDi );  
    TInetAddr addr;
    response->AddAttributeL( CNATFWUNSAFXorMappedAddressAttribute::NewLC( addr, transactionIDi ) );
    CleanupStack::Pop(); // CNATFWUNSAFXorMappedAddressAttribute
    HBufC8* message = EncodeMessageL( *response );
    CleanupStack::PushL( message );
    iCSTUNTransaction->ProcessResponseL( *response, *message );
    iCSTUNTransaction->iTerminated = EFalse;
    iCSTUNTransaction->Terminate( KErrNone );
    
    CleanupStack::PopAndDestroy( message );
    
    CleanupStack::PopAndDestroy( response );
    
    RSocket socket;
    RSocketServ srv;
    User::LeaveIfError( srv.Connect() );
    CleanupClosePushL( srv );
    User::LeaveIfError( socket.Open( srv ) );
    CleanupClosePushL( socket );
    TBufC8<10> service( KStunRelay );
    TInetAddr destAddr( KProtocolInetUdp );
    destAddr.Input(_L("10.10.10.10"));
    destAddr.SetPort( 250 );       
    
// Allocate request

    CNATFWUNSAFAllocateRequest* allocateReq = CNATFWUNSAFAllocateRequest::NewLC( transactionIDi );
    message = EncodeMessageL( *allocateReq );
    CleanupStack::PushL( message );
    
    CSTUNTransaction* transaction = CSTUNTransaction::NewL( *allocateReq, iServerAddr, passwordi, 
        iStreamId, iSubstreamId, *iDeltatimer, iInterval, *this, service, *iMux, 
        iTransportProtocol, EFalse, destAddr );
    CleanupStack::PopAndDestroy( message );
    CleanupStack::PushL( transaction );
    
    CNATFWUNSAFAllocateResponse* allocateResponse = CNATFWUNSAFAllocateResponse::NewLC( transactionIDi );   
    allocateResponse->AddAttributeL( CNATFWUNSAFRelayAddressAttribute::NewLC( destAddr ) );
    CleanupStack::Pop(); // CNATFWUNSAFRelayAddressAttribute 
    allocateResponse->AddAttributeL( CNATFWUNSAFXorMappedAddressAttribute::NewLC( destAddr, transactionIDi ) );
    CleanupStack::Pop(); // CNATFWUNSAFXorMappedAddressAttribute
    message = EncodeMessageL( *allocateResponse );
    CleanupStack::PushL( message );
    transaction->ProcessResponseL( *allocateResponse, *message );
    CleanupStack::PopAndDestroy( message );

    CleanupStack::PopAndDestroy( allocateResponse );    
            
    transaction->iTerminated = EFalse;
    transaction->Terminate( KErrNone );
    
    CleanupStack::PopAndDestroy( transaction );
    CleanupStack::PopAndDestroy( allocateReq );
    
// Connect request

    CNATFWUNSAFConnectRequest* connReq = CNATFWUNSAFConnectRequest::NewLC( transactionIDi );   
    message = EncodeMessageL( *connReq );
    CleanupStack::PushL( message );
    
    transaction = CSTUNTransaction::NewL( *connReq, iServerAddr, passwordi, 
        iStreamId, iSubstreamId, *iDeltatimer, iInterval, *this, service, *iMux, 
        iTransportProtocol, EFalse, destAddr );
    
    CleanupStack::PopAndDestroy( message );
    CleanupStack::PushL( transaction );
    
    CNATFWUNSAFConnectResponse* connectResponse = CNATFWUNSAFConnectResponse::NewLC( transactionIDi );   
    message = EncodeMessageL( *connectResponse );
    CleanupStack::PushL( message );
    transaction->ProcessResponseL( *connectResponse, *message );
    CleanupStack::PopAndDestroy( message );
    CleanupStack::PopAndDestroy( connectResponse );
           
    transaction->iTerminated = EFalse;
    transaction->Terminate( KErrNone );   
    
    CleanupStack::PopAndDestroy( transaction );    
    CleanupStack::PopAndDestroy( connReq );
    
// Set Active Destionation request

    CNATFWUNSAFSetActiveDestinationRequest* activedestReq = CNATFWUNSAFSetActiveDestinationRequest::NewLC( transactionIDi );   
    message = EncodeMessageL( *activedestReq );
    CleanupStack::PushL( message );
    
    transaction = CSTUNTransaction::NewL( *activedestReq, iServerAddr, passwordi, 
        iStreamId, iSubstreamId, *iDeltatimer, iInterval, *this, service, *iMux, 
        iTransportProtocol, EFalse, destAddr );
    
    CleanupStack::PopAndDestroy( message );
    CleanupStack::PushL( transaction );
    
    CNATFWUNSAFSetActiveDestinationResponse* activeResponse = CNATFWUNSAFSetActiveDestinationResponse::NewLC( transactionIDi );   
    message = EncodeMessageL( *activeResponse );
    CleanupStack::PushL( message );
    transaction->ProcessResponseL( *activeResponse, *message );
    CleanupStack::PopAndDestroy( message );
    CleanupStack::PopAndDestroy( activeResponse );
          
    transaction->iTerminated = EFalse;
    transaction->Terminate( KErrNone );   
    
    CleanupStack::PopAndDestroy( transaction );
    CleanupStack::PopAndDestroy( activedestReq );
                
    CleanupStack::PopAndDestroy( &socket );
    CleanupStack::PopAndDestroy( &srv );
    }
    
void ut_cstuntransaction::ut_cstuntransaction_TimerValueL(  )
    {
    iCSTUNTransaction->TimerValue( );
    }
    
void ut_cstuntransaction::ut_cstuntransaction_CancelRetransmissionL(  )
    {
    iCSTUNTransaction->CancelRetransmission( );
    }
    
void ut_cstuntransaction::ut_cstuntransaction___DbgTestInvariantL(  )
    {
    iCSTUNTransaction->__DbgTestInvariant( );
    }
    
void ut_cstuntransaction::ut_cstuntransaction_RetransmitIntervalL()
    {
    iCSTUNTransaction->RetransmitInterval();
    
    iCSTUNTransaction->iSendCount = 7/*KMaxRequestSentCount*/;
    iCSTUNTransaction->RetransmitInterval();
    }
    
void ut_cstuntransaction::ut_cstuntransaction_ComputeWaitDurationL()
    {
    iCSTUNTransaction->ComputeWaitDuration();
    }
    
void ut_cstuntransaction::ut_cstuntransaction_ShouldIgnoreResponseL()
    {
    CNATFWUNSAFBindingErrorResponse* errorresponse =
        CNATFWUNSAFBindingErrorResponse::NewLC( transactionIDi );
    errorresponse->AddAttributeL( CNATFWUNSAFErrorCodeAttribute::NewLC( 300, KErr ) );
    CleanupStack::Pop(); //CNATFWUNSAFErrorCodeAttribute
    iCSTUNTransaction->ShouldIgnoreResponse( *errorresponse );
    
    //TInetAddr addr;
    //errorresponse->AddAttributeL( CNATFWUNSAFMappedAddressAttribute::NewLC( addr ) );
    errorresponse->UnknownMandatoryAttributeFound();
    iCSTUNTransaction->ShouldIgnoreResponse( *errorresponse );    
    CleanupStack::PopAndDestroy( errorresponse );
    }
    
void ut_cstuntransaction::ut_cstuntransaction_ProcessResponseLL()
    {
    CNATFWUNSAFBindingResponse* response = CNATFWUNSAFBindingResponse::NewLC( transactionIDi );  
    HBufC8* message = EncodeMessageL( *response );        
    iCSTUNTransaction->ProcessResponseL( *response, *message );
    delete message;
    CleanupStack::PopAndDestroy( response );
    
    CNATFWUNSAFAllocateResponse* allocateResponse = CNATFWUNSAFAllocateResponse::NewLC( transactionIDi );   
    message = EncodeMessageL( *allocateResponse );
    iCSTUNTransaction->ProcessResponseL( *allocateResponse, *message );
    delete message;     
    CleanupStack::PopAndDestroy( allocateResponse );
    
    CNATFWUNSAFConnectResponse* connectResponse = CNATFWUNSAFConnectResponse::NewLC( transactionIDi );   
    message = EncodeMessageL( *connectResponse );
    iCSTUNTransaction->ProcessResponseL( *connectResponse, *message );
    delete message;     
    CleanupStack::PopAndDestroy( connectResponse );
    
    CNATFWUNSAFSetActiveDestinationResponse* activeResponse = CNATFWUNSAFSetActiveDestinationResponse::NewLC( transactionIDi );   
    message = EncodeMessageL( *activeResponse );
    iCSTUNTransaction->ProcessResponseL( *activeResponse, *message );
    delete message;     
    CleanupStack::PopAndDestroy( activeResponse );
    
    CNATFWUNSAFBindingRequest* request = CNATFWUNSAFBindingRequest::NewLC( transactionIDi );  
    message = EncodeMessageL( *request );
    iCSTUNTransaction->ProcessResponseL( *request, *message );
    delete message;     
    CleanupStack::PopAndDestroy( request );
    
    CNATFWUNSAFBindingErrorResponse* errorresponse = CNATFWUNSAFBindingErrorResponse::NewLC( transactionIDi );  
    errorresponse->AddAttributeL( CNATFWUNSAFErrorCodeAttribute::NewLC( 420, KErr ) );
    CleanupStack::Pop(); // CNATFWUNSAFErrorCodeAttribute
    
    errorresponse->UnknownMandatoryAttributeFound();
    message = EncodeMessageL( *errorresponse );
    iCSTUNTransaction->ProcessResponseL( *errorresponse, *message );
    delete message;
    CleanupStack::PopAndDestroy( errorresponse );                 
    }
    
void ut_cstuntransaction::ut_cstuntransaction_CheckIntegrityLL()
    {
    CNATFWUNSAFMessageFactory* decoder = CNATFWUNSAFMessageFactory::NewLC();
    CNATFWUNSAFBindingResponse* response = CNATFWUNSAFBindingResponse::NewLC( transactionIDi );
    HBufC8* message = EncodeMessageL( *response );
    CleanupStack::PushL( message );
    
    EUNIT_ASSERT_EQUALS( iCSTUNTransaction->CheckIntegrityL( *response, *message ), KErrNone );
    
    _LIT8( KUsername, "userB:userAA" );
    TBufC8<12> username( KUsername );    
    iCSTUNTransaction->iRequest.AddAttributeL( CNATFWUNSAFUsernameAttribute::NewLC( username ) );
    CleanupStack::Pop(); // CNATFWUNSAFUsernameAttribute
    
    EUNIT_ASSERT_EQUALS( iCSTUNTransaction->CheckIntegrityL( *response, *message ), KErrCorrupt );
    
    CNATFWUNSAFMessage* msg = decoder->DecodeL( *message );
    CleanupStack::PushL( msg );
    EUNIT_ASSERT_EQUALS( iCSTUNTransaction->CheckIntegrityL( *msg, *message ), KErrNone );
    CleanupStack::PopAndDestroy( msg );
    
    CleanupStack::PopAndDestroy( message );
    message = NULL;
    CNATFWUNSAFSetActiveDestinationResponse* activeResponse = 
        CNATFWUNSAFSetActiveDestinationResponse::NewLC( transactionIDi );
    message = EncodeMessageL( *activeResponse );
    CleanupStack::PushL( message );
    
    msg = decoder->DecodeL( *message );
    CleanupStack::PushL( msg );
    
    EUNIT_ASSERT_EQUALS( iCSTUNTransaction->CheckIntegrityL( *msg, *message ), KErrNone );
    
    CleanupStack::PopAndDestroy( msg );
    CleanupStack::PopAndDestroy( message );
    CleanupStack::PopAndDestroy( activeResponse );
    CleanupStack::PopAndDestroy( response );
    CleanupStack::PopAndDestroy( decoder );
    }
        
/*void ut_cstuntransaction::ut_cstuntransaction_AddressDiffersLL()
    {
    iCSTUNTransaction->AddressDiffersL();
    }*/
                    
void ut_cstuntransaction::ut_cstuntransaction_CheckForNatOverwriteLL()
    {  
    CNATFWUNSAFBindingResponse* response = CNATFWUNSAFBindingResponse::NewLC( transactionIDi );
    iCSTUNTransaction->CheckForNatOverwriteL( *response );
    
    TInetAddr addr;
    response->AddAttributeL( CNATFWUNSAFMappedAddressAttribute::NewLC( addr ) );
    CleanupStack::Pop(); // CNATFWUNSAFMappedAddressAttribute
    iCSTUNTransaction->CheckForNatOverwriteL( *response );
    
    response->AddAttributeL( CNATFWUNSAFXorMappedAddressAttribute::NewLC( addr, transactionIDi ) );
    CleanupStack::Pop(); // CNATFWUNSAFXorMappedAddressAttribute   
    iCSTUNTransaction->CheckForNatOverwriteL( *response );
    
    TInetAddr addr2( KProtocolInetTcp );
    addr2.Input( _L( "10.10.10.10" ) );
    addr2.SetPort( 250 );
    
    response->DeleteAttribute( CNATFWUNSAFAttribute::EXorMappedAddress );
    response->AddAttributeL( CNATFWUNSAFXorMappedAddressAttribute::NewLC( addr2, transactionIDi ) );
    CleanupStack::Pop(); // CNATFWUNSAFXorMappedAddressAttribute
    iCSTUNTransaction->CheckForNatOverwriteL( *response );

    CleanupStack::PopAndDestroy( response );
    
    CNATFWUNSAFAllocateResponse* allocateResponse = CNATFWUNSAFAllocateResponse::NewLC( transactionIDi );
    iCSTUNTransaction->CheckForNatOverwriteL( *response );
    CleanupStack::PopAndDestroy( allocateResponse );    
    }    

void ut_cstuntransaction::ut_cstuntransaction_GetResponseCodeL()
    {
    CNATFWUNSAFBindingErrorResponse* response = 
        CNATFWUNSAFBindingErrorResponse::NewL( transactionIDi );
    iCSTUNTransaction->GetResponseCode( *response );
    delete response;
    
    CNATFWUNSAFAllocateErrorResponse* allocateResponse = 
        CNATFWUNSAFAllocateErrorResponse::NewL( transactionIDi );
    iCSTUNTransaction->GetResponseCode( *allocateResponse );
    delete allocateResponse;
    
    CNATFWUNSAFConnectErrorResponse* connectResponse = 
        CNATFWUNSAFConnectErrorResponse::NewL( transactionIDi );
    iCSTUNTransaction->GetResponseCode( *connectResponse );
    delete connectResponse;
    
    CNATFWUNSAFSetActiveDestinationErrorResponse* activeResponse = 
        CNATFWUNSAFSetActiveDestinationErrorResponse::NewL( transactionIDi );
    iCSTUNTransaction->GetResponseCode( *activeResponse );
    delete activeResponse;      
    } 
    
void ut_cstuntransaction::ut_cstuntransaction_CheckProtocolLL()
    {
    TBufC8<4> service( KStun );
    TBufC8<10> service2( KStunRelay );
    iCSTUNTransaction->CheckProtocolL( service );
    iCSTUNTransaction->CheckProtocolL( service2 );
    } 
    
void ut_cstuntransaction::ut_cstuntransaction_StunRelayUsedL()
    {
    iCSTUNTransaction->StunRelayUsed();
    } 
    
void ut_cstuntransaction::ut_cstuntransaction_MeasureNewRTTSampleL()
    {
    iCSTUNTransaction->MeasureNewRTTSample();
    
    iCSTUNTransaction->iSendCount = 3;
    iCSTUNTransaction->MeasureNewRTTSample();
    }         
        

//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    ut_cstuntransaction,
    "Add test suite description here.",
    "UNIT" ) 

EUNIT_TEST(
    "New - test0",
    "CSTUNTransaction",
    "New - test0",
    "FUNCTIONALITY",
    SetupL, ut_cstuntransaction_NewL, Teardown)

EUNIT_TEST(
    "~CSTUNTransaction - test0",
    "CSTUNTransaction",
    "~CSTUNTransaction - test0",
    "FUNCTIONALITY",
    SetupL, ut_cstuntransaction__CSTUNTransactionL, Teardown)
    
EUNIT_TEST(
    "TimerExpiredL - test1",
    "CSTUNTransaction",
    "TimerExpiredL - test1",
    "FUNCTIONALITY",
    SetupL, ut_cstuntransaction_TimerExpiredLL, Teardown)
    
EUNIT_TEST(
    "LeaveFromTimerExpired - test2",
    "CSTUNTransaction",
    "LeaveFromTimerExpired - test2",
    "FUNCTIONALITY",
    SetupL, ut_cstuntransaction_LeaveFromTimerExpiredL, Teardown)
    
EUNIT_TEST(
    "UNSAFUdpMessageSentL - test3",
    "CSTUNTransaction",
    "UNSAFUdpMessageSentL - test3",
    "FUNCTIONALITY",
    SetupL, ut_cstuntransaction_UNSAFUdpMessageSentLL, Teardown)
    
EUNIT_TEST(
    "UNSAFUdpMessageFailure - test4",
    "CSTUNTransaction",
    "UNSAFUdpMessageFailure - test4",
    "FUNCTIONALITY",
    SetupL, ut_cstuntransaction_UNSAFUdpMessageFailureL, Teardown)
    
EUNIT_TEST(
    "MessageSent - test5",
    "CSTUNTransaction",
    "MessageSent - test5",
    "FUNCTIONALITY",
    SetupL, ut_cstuntransaction_MessageSentL, Teardown)
    
EUNIT_TEST(
    "MessageSentFailure - test6",
    "CSTUNTransaction",
    "MessageSentFailure - test6",
    "FUNCTIONALITY",
    SetupL, ut_cstuntransaction_MessageSentFailureL, Teardown)
    
EUNIT_TEST(
    "SendRequestL - test7",
    "CSTUNTransaction",
    "SendRequestL - test7",
    "FUNCTIONALITY",
    SetupL, ut_cstuntransaction_SendRequestLL, Teardown)

/*EUNIT_TEST(
    "RequestSent - test8",
    "CSTUNTransaction",
    "RequestSent - test8",
    "FUNCTIONALITY",
    SetupL, ut_cstuntransaction_RequestSentL, Teardown)*/

EUNIT_TEST(
    "RetransmitRequestL - test9",
    "CSTUNTransaction",
    "RetransmitRequestL - test9",
    "FUNCTIONALITY",
    SetupL, ut_cstuntransaction_RetransmitRequestLL, Teardown)
    
EUNIT_TEST(
    "ReceiveL - test10",
    "CSTUNTransaction",
    "ReceiveL - test10",
    "FUNCTIONALITY",
    SetupL, ut_cstuntransaction_ReceiveLL, Teardown)

/*EUNIT_TEST(
    "HasReceivedResponse - test11",
    "CSTUNTransaction",
    "HasReceivedResponse - test11",
    "FUNCTIONALITY",
    SetupL, ut_cstuntransaction_HasReceivedResponseL, Teardown)*/ 

EUNIT_TEST(
    "Terminate - test12",
    "CSTUNTransaction",
    "Terminate - test12",
    "FUNCTIONALITY",
    SetupL, ut_cstuntransaction_TerminateL, Teardown)

EUNIT_TEST(
    "TimerValue - test13",
    "CSTUNTransaction",
    "TimerValue - test13",
    "FUNCTIONALITY",
    SetupL, ut_cstuntransaction_TimerValueL, Teardown)
    
EUNIT_TEST(
    "CancelRetransmission - test14",
    "CSTUNTransaction",
    "CancelRetransmission - test14",
    "FUNCTIONALITY",
    SetupL, ut_cstuntransaction_CancelRetransmissionL, Teardown)
    
EUNIT_TEST(
    "__DbgTestInvariant - test28",
    "CSTUNTransaction",
    "__DbgTestInvariant - test28",
    "FUNCTIONALITY",
    SetupL, ut_cstuntransaction___DbgTestInvariantL, Teardown) 
   
EUNIT_TEST(
    "RetransmitInterval - test28",
    "CSTUNTransaction",
    "RetransmitInterval - test28",
    "FUNCTIONALITY",
    SetupL, ut_cstuntransaction_RetransmitIntervalL, Teardown)

EUNIT_TEST(
    "ComputeWaitDuration - test28",
    "CSTUNTransaction",
    "ComputeWaitDuration - test28",
    "FUNCTIONALITY",
    SetupL, ut_cstuntransaction_ComputeWaitDurationL, Teardown)

EUNIT_TEST(
    "ShouldIgnoreResponse - test28",
    "CSTUNTransaction",
    "ShouldIgnoreResponse - test28",
    "FUNCTIONALITY",
    SetupL, ut_cstuntransaction_ShouldIgnoreResponseL, Teardown)

EUNIT_TEST(
    "ProcessResponseL - test28",
    "CSTUNTransaction",
    "ProcessResponseL - test28",
    "FUNCTIONALITY",
    SetupL, ut_cstuntransaction_ProcessResponseLL, Teardown)

EUNIT_TEST(
    "CheckIntegrityL - test28",
    "CSTUNTransaction",
    "CheckIntegrityL - test28",
    "FUNCTIONALITY",
    SetupL, ut_cstuntransaction_CheckIntegrityLL, Teardown)
 
/*EUNIT_TEST(
    "AddressDiffersL - test28",
    "CSTUNTransaction",
    "AddressDiffersL - test28",
    "FUNCTIONALITY",
    SetupL, ut_cstuntransaction_AddressDiffersLL, Teardown)*/

EUNIT_TEST(
    "CheckForNatOverwriteL - test28",
    "CSTUNTransaction",
    "CheckForNatOverwriteL - test28",
    "FUNCTIONALITY",
    SetupL, ut_cstuntransaction_CheckForNatOverwriteLL, Teardown)
    
EUNIT_TEST(
    "GetResponseCode - test28",
    "CSTUNTransaction",
    "GetResponseCode - test28",
    "FUNCTIONALITY",
    SetupL, ut_cstuntransaction_GetResponseCodeL, Teardown)
    
EUNIT_TEST(
    "CheckProtocolL - test28",
    "CSTUNTransaction",
    "CheckProtocolL - test28",
    "FUNCTIONALITY",
    SetupL, ut_cstuntransaction_CheckProtocolLL, Teardown)                    

EUNIT_TEST(
    "StunRelayUsed - test28",
    "CSTUNTransaction",
    "StunRelayUsed - test28",
    "FUNCTIONALITY",
    SetupL, ut_cstuntransaction_StunRelayUsedL, Teardown)
    
EUNIT_TEST(
    "MeasureNewRTTSample - test28",
    "CSTUNTransaction",
    "MeasureNewRTTSample - test28",
    "FUNCTIONALITY",
    SetupL, ut_cstuntransaction_MeasureNewRTTSampleL, Teardown)    

EUNIT_END_TEST_TABLE

//  END OF FILE
