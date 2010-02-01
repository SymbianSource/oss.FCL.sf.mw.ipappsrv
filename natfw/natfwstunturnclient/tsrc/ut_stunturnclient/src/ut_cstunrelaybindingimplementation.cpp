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



//  CLASS HEADER
#include "ut_cstunrelaybindingimplementation.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <digia/eunit/EUnitDecorators.h>
#include <commdbconnpref.h>
#include <e32debug.h>

//  INTERNAL INCLUDES
#include "natfwstunrelaybinding.h"
#include "cstunrelaybindingimplementation.h"
#include "mstunbindingobserver.h"
#include "natfwstunclientdefs.h"
#include "natfwunsafunknownattributesattribute.h"
#include "natfwunsafbindingrequest.h"
#include "natfwunsafbindingresponse.h"
#include "natfwunsafbindingerrorresponse.h"
#include "natfwunsafallocaterequest.h"
#include "natfwunsafallocateerrorresponse.h"
#include "natfwunsafallocateresponse.h"
#include "natfwunsafconnectresponse.h"
#include "natfwunsafconnecterrorresponse.h"
#include "natfwunsafconnectrequest.h"
#include "natfwunsafsetactivedestinationrequest.h"
#include "natfwunsafsetactivedestinationresponse.h"
#include "natfwunsafsetactivedestinationerrorresponse.h"
#include "cstunbindinggetaddress.h"
#include "natfwunsaficecontrolledattribute.h"
#include "natfwbindingimplementationbase.h"
#include "natfwunsafdataindication.h"
#include "natfwunsafdataattribute.h"
#include "natfwunsafremoteaddressattribute.h"
#include "natfwunsafalternateserverattribute.h"
#include "cstunbindinggetsharedsecret.h"
#include "cstunbindinggetaddress.h"
#include "cstunbindingactive.h"
#include "cstunbindingwaittoretry.h"
#include "natfwunsafrealmattribute.h"
#include "natfwunsafnonceattribute.h"
#include "cunsafmessageencoder.h"
#include "cstunclientimplementation.h"
#include "natfwunsaftcprelaypacket.h"
#include "cnatfwunsafencodeditem.h"
#include "natfwunittestmacros.h"

_LIT8( KServerAddress, "10.32.194.251" );
_LIT8( KUsername, "userAA" );
_LIT8( KPassword,"salasana");
_LIT8(KRealmVal,"\"myrealm\"");
_LIT8(KRealmVal2,"\"myREALMI\"");

const TDesC8& dataDesc5(KRealmVal);
const TDesC8& dataDesc6(KRealmVal2);

const TUint KDefaultRtoValue = 0;

// CONSTRUCTION
ut_cstunrelaybindingimplementation* ut_cstunrelaybindingimplementation::NewL()
    {
    ut_cstunrelaybindingimplementation* self = ut_cstunrelaybindingimplementation::NewLC();
    CleanupStack::Pop();

    return self;
    }

ut_cstunrelaybindingimplementation* ut_cstunrelaybindingimplementation::NewLC()
    {
    ut_cstunrelaybindingimplementation* self = new( ELeave ) ut_cstunrelaybindingimplementation();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
ut_cstunrelaybindingimplementation::~ut_cstunrelaybindingimplementation()
    {
    }

// Default constructor
ut_cstunrelaybindingimplementation::ut_cstunrelaybindingimplementation()
    {
    }

// Second phase construct
void ut_cstunrelaybindingimplementation::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS
// from mstunclientbserver
void ut_cstunrelaybindingimplementation::STUNClientInitCompleted( const CSTUNClient& /*aClient*/,
                              TInt aCompletionCode )
    {
    RDebug::Print( _L("**NATFW STUNClientInitCompleted, code: %d"), aCompletionCode );
    CActiveScheduler::Stop();
    }


void ut_cstunrelaybindingimplementation::STUNBindingEventOccurredL( TSTUNBindingEvent aEvent, 
                                const CBinding& /*aBinding*/ )
    {
    RDebug::Print( _L("**NATFW STUNBindingEventOccurredL, code: %d"), aEvent );
    CActiveScheduler::Stop();
    }


void ut_cstunrelaybindingimplementation::STUNBindingErrorOccurred( const CBinding& /*aBinding*/, 
                               TInt aError )
    {
    RDebug::Print( _L("**NATFW STUNBindingErrorOccurred, code: %d"), aError );
    CActiveScheduler::Stop();
    }

void ut_cstunrelaybindingimplementation::Notify( TUint /*aSessionId*/, TUint /*aStreamId*/, 
    TNotifyType /*aType*/, TInt /*aError*/ )
    {
    RDebug::Print( _L("**NATFW Notify") );
    }

const CSTUNClient& ut_cstunrelaybindingimplementation::STUNClient() const
    {
    return *iStunclient;
    }

CDeltaTimer& ut_cstunrelaybindingimplementation::TimerProvider()
    {  
    return *iDeltatimer;
    }
        
TInt ut_cstunrelaybindingimplementation::RetransmitInterval() const
    {
    TInt dummy(0);
    return dummy;
    }

TTransportProtocol ut_cstunrelaybindingimplementation::TransportProtocol() const
    {
    iStunclient->Implementation().TransportProtocol();
    }

void ut_cstunrelaybindingimplementation::AddressResolvedL( const CBinding& /*aBinding*/ ) const
    {    
    }

void ut_cstunrelaybindingimplementation::ObtainSharedSecretL( CBinding& /*aBinding*/ )
    {
    TBufC8<16> password( KPassword );
    TBufC8<6> usernameA( KUsername );    
    
    iImpl->CreateBindingRequestL( usernameA, password );
    iImpl->SendBindingRequestL();
    }
    
TBool ut_cstunrelaybindingimplementation::SharedSecretRejectedL( 
    const CBinding& /*aBinding*/, const TDesC8& /*aUsername*/, const TDesC8& /*aPassword*/ )
    {
    TBool dummy( EFalse );
    return dummy;    
    }

TBool ut_cstunrelaybindingimplementation::ObtainServerAddress( TInetAddr& aAddress )
    {
    #ifdef _DEBUG_EUNIT
        TInetAddr addr;
        addr.SetAddress( INET_ADDR( 10,32,194,251 ) );
        aAddress = addr;
        return ETrue;
    #endif

    return EFalse;
    }

void ut_cstunrelaybindingimplementation::ObtainTransactionIDL( TNATFWUNSAFTransactionID& aTransactionID )
    {
    iStunclient->ObtainTransactionIDL( aTransactionID );    
    }
    
void ut_cstunrelaybindingimplementation::AttachBindingL( const CBinding& /*aBinding*/ )
    {
    }

void ut_cstunrelaybindingimplementation::DetachBinding( const CBinding& /*aBinding*/ )
    {
    }
void ut_cstunrelaybindingimplementation::BindingErrorL( const CBinding& /*aBinding*/, TInt aError, TBool /*aIsFatal*/ ) 
    {
    RDebug::Print( _L("**NATFW BindingErrorL, error: %d"), aError );
    }
void ut_cstunrelaybindingimplementation::BindingEventOccurred( const CBinding& /*aBinding*/, 
                           TSTUNCallbackInfo::TFunction /*aEvent*/ )
    {    
    }


const TDesC8& ut_cstunrelaybindingimplementation::UsernameForIndication()
    {
    TBufC8<16> password( KPassword );
    return password;    
    }

const TDesC8& ut_cstunrelaybindingimplementation::PasswordForIndication()
    {
    TBufC8<16> password( KPassword );
    return password;
    }
    
void ut_cstunrelaybindingimplementation::SetupL(  )
    {
    TUint iapID( 6 );
    TInt retransmitInterval( 10 );
    TBufC8<13> serveraddress( KServerAddress );
    TUint serverport( 2000 );
    TBufC8<10> service( KStunRelay );
    TBool obtainsharedsecret( ETrue );
    TBool failifnoSRVrecordfound( EFalse );
    iDeltatimer = CDeltaTimer::NewL( 1 );
    User::LeaveIfError( iSocketServ.Connect() );
    iStreamId = 1;
    iSubstreamId = 1;
    TBool icmpUsed( EFalse );
    
    iMux = CNcmConnectionMultiplexer::NewL( *this );
    
    TCommDbConnPref pref;
    pref.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    pref.SetIapId( iapID );
    
    User::LeaveIfError( iConnection.Open( iSocketServ ) );
    iConnection.Start( pref, iStatus );
    User::WaitForRequest( iStatus );
    User::LeaveIfError( iStatus.Int() );    
    
    iStunclient = CSTUNClient::NewL( retransmitInterval,
                                     serveraddress,
                                     serverport,
                                     service,
                                     iSocketServ,
                                     iConnection,
                                     *iDeltatimer,
                                     *this,
                                     obtainsharedsecret,
                                     failifnoSRVrecordfound,
                                     icmpUsed,
                                     iMux );
                                     
    CActiveScheduler::Start();  
    
    User::LeaveIfError( iSocket.Open( iSocketServ, KAfInet, KSockDatagram,
                                     KProtocolInetUdp ));
    iSocket.SetLocalPort( 5000 );                                 
    iBinding = CSTUNRelayBinding::NewL( *iStunclient, iStreamId, iSubstreamId );
    
    iImpl = CSTUNRelayBindingImplementation::NewL( *iBinding, *this,
                                                   iStreamId, iSubstreamId, iMux );
    }

void ut_cstunrelaybindingimplementation::Teardown(  )
    {
    delete iImpl;
    delete iBinding;    
    delete iStunclient;    
    iSocket.Close();
    iConnection.Stop();
    iConnection.Close();
    iSocketServ.Close();
    delete iServeraddress;
    delete iResponse;
    delete iRespDataIndication;    
    delete iDeltatimer;
    delete iMux;   
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_NewLL(  )
    {
    CSTUNRelayBindingImplementation* impl = 
        CSTUNRelayBindingImplementation::NewL( *iBinding, *this, iSocket );
    delete impl;
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_NewL_1L(  )
    {
    CSTUNRelayBindingImplementation* impl = 
        CSTUNRelayBindingImplementation::NewL( *iBinding, *this, iStreamId, iSubstreamId, iMux );
    delete impl;
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_TimerExpiredLL(  )
    {
    CSTUNBindingGetAddress* getAddress = new ( ELeave ) CSTUNBindingGetAddress();
    CleanupStack::PushL( getAddress );
    CSTUNBindingGetSharedSecret* getSharedSecret =
        new ( ELeave ) CSTUNBindingGetSharedSecret( *getAddress );
    CleanupStack::PushL( getSharedSecret );
    CSTUNBindingWaitToRetry* waitToRetry = new ( ELeave ) CSTUNBindingWaitToRetry( *getSharedSecret );
    CleanupStack::PushL( waitToRetry );
    
    iImpl->ChangeState( *waitToRetry );
    
    TInetAddr addr( KProtocolInetUdp );
    _LIT( KServer, "10.32.194.251" );
    addr.Input( KServer );
    iImpl->iServerAddress = addr;
    iImpl->iRequestType = iImpl->ESendRequest;     
    iImpl->TimerExpiredL();

    CleanupStack::PopAndDestroy( waitToRetry );
    CleanupStack::PopAndDestroy( getSharedSecret );
    CleanupStack::PopAndDestroy( getAddress );
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_LeaveFromTimerExpiredL(  )
    {
    //EUNIT_ASSERT_PANIC( iImpl->LeaveFromTimerExpired( KErrNone), "", KErrArgument );    
    iImpl->LeaveFromTimerExpired( KErrNotFound );
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_PublicAddressObtainedLL(  )
    {
    TInetAddr addr;
    CSTUNBindingGetAddress* getAddress = new ( ELeave ) CSTUNBindingGetAddress();
    iImpl->ChangeState( *getAddress );
    iImpl->PublicAddressObtainedL( addr );
    delete getAddress;
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_PublicAddressObtainedL_1L(  )
    {
    TInetAddr reflexsiveAddr;
    TInetAddr relayAddr;
    
    CSTUNBindingGetAddress* getAddress = new ( ELeave ) CSTUNBindingGetAddress();
    iImpl->ChangeState( *getAddress );
    iImpl->PublicAddressObtainedL( reflexsiveAddr, relayAddr );
    delete getAddress;
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_TransactionErrorL(  )
    {
    EUNIT_ASSERT_PANIC( iImpl->TransactionError( 
        KErrNone, CNATFWUNSAFUnknownAttributesAttribute::NewLC() ), "", KErrArgument );     
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_TransactionEventOccurredLL(  )
    {
    iImpl->TransactionEventOccurredL( TSTUNCallbackInfo::EInitCompleted );
    
    TBufC8<16> password( KPassword );
    TBufC8<6> usernameA( KUsername );
    iImpl->iRequestType = iImpl->ESendRequest;
    iImpl->CreateBindingRequestL( usernameA, password );
        
    TInetAddr addr( KProtocolInetUdp );
    _LIT( KServer, "10.32.194.251" );
    addr.Input( KServer );
    iImpl->iServerAddress = addr; 
    iImpl->SendBindingRequestL();    
    //iImpl->iRequestType = iImpl->ESetActiveDestinationRequest;
    iImpl->Terminate(401);    
    TUint32 timerValue(10);
    iImpl->SetActiveDestinationRequestL( addr, timerValue );
    iImpl->TransactionEventOccurredL( TSTUNCallbackInfo::EInitCompleted );
    
    //iImpl->iRequestType = iImpl->ESendIndicationToNetwork;
    iImpl->TransactionEventOccurredL( TSTUNCallbackInfo::EInitCompleted );    
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_AllocateRequestLL(  )
    {
    iImpl->AllocateRequestL( KDefaultRtoValue );
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_CancelRequestL(  )
    {
    iImpl->CancelRequest();
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_SocketL(  )
    {
    CSTUNRelayBindingImplementation* impl = 
        CSTUNRelayBindingImplementation::NewL( *iBinding, *this, iSocket );       
    RSocket socket = impl->Socket();    
    delete impl;
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_AddressResolvedL(  )
    {
    TBool isResolved = iImpl->AddressResolved();
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_PublicAddrL(  )
    {
    TInetAddr addr = iImpl->PublicAddr();
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_AlternateServerAddrL(  )
    {
    TInetAddr addr = iImpl->AlternateServerAddr();
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_HandleDataLL(  )
    {
    EUNIT_DISABLE_ALLOC_DECORATOR;
    CUnsafMessageEncoder* encoder = CUnsafMessageEncoder::NewLC();

    _LIT8( KPassword,"aaaabbbbccccdddd");
    TBufC8<16> password( KPassword );
    TNATFWUNSAFTransactionID transactionID;
    iStunclient->ObtainTransactionIDL( transactionID );
    
    CNATFWUNSAFDataIndication* indication = CNATFWUNSAFDataIndication::NewLC( transactionID );
    
    CNATFWUNSAFBindingResponse* response = CNATFWUNSAFBindingResponse::NewLC( transactionID );
    HBufC8* respmsg = encoder->EncodeMessageL( *response );
    CleanupStack::PopAndDestroy( response );
    CleanupStack::PushL( respmsg );
    
    TInetAddr remoteAddr;
    TBool consumed = EFalse;
    EUNIT_ENABLE_ALLOC_DECORATOR;
    HBufC8* ptr = iImpl->HandleDataL( *respmsg, consumed, remoteAddr );
    EUNIT_DISABLE_ALLOC_DECORATOR;
    EUNIT_ASSERT( NULL == ptr );
    EUNIT_ASSERT( EFalse == consumed );
    
    CleanupStack::PopAndDestroy( respmsg );
// non-valid stun req inside indication
    
    _LIT8( KDummypacket, "oysescatechnologiesabFIN" );
    TBufC8<24> buffi( KDummypacket );
    indication->AddAttributeL( CNATFWUNSAFDataAttribute::NewLC( buffi ) );
    CleanupStack::Pop(); // CNATFWUNSAFDataAttribute
    indication->AddAttributeL( CNATFWUNSAFRemoteAddressAttribute::NewLC( remoteAddr ) );
    CleanupStack::Pop(); // CNATFWUNSAFRemoteAddressAttribute
    HBufC8* msg = encoder->EncodeMessageL( *indication );
    CleanupStack::PushL( msg );
    EUNIT_ENABLE_ALLOC_DECORATOR;
    ptr = iImpl->HandleDataL( *msg, consumed, remoteAddr );
    
    EUNIT_ASSERT( NULL != ptr );
    EUNIT_ASSERT( EFalse == consumed );
        
    delete ptr;

    CleanupStack::PopAndDestroy( msg );
    CleanupStack::PopAndDestroy( indication );  // test
    CleanupStack::PopAndDestroy( encoder );
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_HandleDataL2L(  )  
    {
    EUNIT_DISABLE_ALLOC_DECORATOR;
    CUnsafMessageEncoder* encoder = CUnsafMessageEncoder::NewLC();
    
    TInetAddr remoteAddr;
    TBool consumed = EFalse;
    _LIT8( KPassword,"aaaabbbbccccdddd");
    TBufC8<16> password( KPassword );
    TNATFWUNSAFTransactionID transactionID;
    iStunclient->ObtainTransactionIDL( transactionID );    
    
    CNATFWUNSAFDataIndication* indication = CNATFWUNSAFDataIndication::NewLC( transactionID );
    
    CNATFWUNSAFBindingResponse* response = CNATFWUNSAFBindingResponse::NewLC( transactionID );
    HBufC8* respmsg = encoder->EncodeMessageL( *response );
    CleanupStack::PopAndDestroy( response );
    CleanupStack::PushL( respmsg );
    
// valid stun request inside indication
    
    indication->DeleteAttribute( CNATFWUNSAFAttribute::EData );
    indication->AddAttributeL( CNATFWUNSAFDataAttribute::NewLC( *respmsg ) );
    CleanupStack::Pop(); // CNATFWUNSAFDataAttribute
    
    CleanupStack::PopAndDestroy( respmsg );
    
    HBufC8* msg = encoder->EncodeMessageL( *indication );
    CleanupStack::PushL( msg );
    EUNIT_ENABLE_ALLOC_DECORATOR;
    HBufC8* ptr = iImpl->HandleDataL( *msg, consumed, remoteAddr );
    EUNIT_DISABLE_ALLOC_DECORATOR;
    EUNIT_ASSERT( NULL != ptr );             
    EUNIT_ASSERT( EFalse == consumed );
    
    delete ptr;

// Tcp framing not valid 
    
    iStunclient->Implementation().iTransportProtocol = ETcpProtocol;
    EUNIT_ENABLE_ALLOC_DECORATOR;
    ptr = iImpl->HandleDataL( *msg, consumed, remoteAddr );

    EUNIT_ASSERT( NULL == ptr );
    EUNIT_ASSERT( EFalse == consumed );
    
    CleanupStack::PopAndDestroy( msg );
    CleanupStack::PopAndDestroy( indication ); 
    CleanupStack::PopAndDestroy( encoder );
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_HandleDataL3L(  )
    {
    EUNIT_DISABLE_ALLOC_DECORATOR;
    CUnsafMessageEncoder* encoder = CUnsafMessageEncoder::NewLC();

    TBool consumed = EFalse;
    _LIT8( KPassword,"aaaabbbbccccdddd");
    TBufC8<16> password( KPassword );
    TNATFWUNSAFTransactionID transactionID;
    iStunclient->ObtainTransactionIDL( transactionID );
    TInetAddr remoteAddr;
    
    CNATFWUNSAFDataIndication* indication = CNATFWUNSAFDataIndication::NewLC( transactionID );
    
    CNATFWUNSAFBindingResponse* response = CNATFWUNSAFBindingResponse::NewLC( transactionID );
    HBufC8* respmsg = encoder->EncodeMessageL( *response );
    CleanupStack::PopAndDestroy( response );
    CleanupStack::PushL( respmsg );
    
    indication->AddAttributeL( CNATFWUNSAFRemoteAddressAttribute::NewLC( remoteAddr ) );  
    CleanupStack::Pop(); // CNATFWUNSAFRemoteAddressAttribute
    indication->AddAttributeL( CNATFWUNSAFDataAttribute::NewLC( *respmsg ) );
    CleanupStack::Pop(); // CNATFWUNSAFDataAttribute
    
    CleanupStack::PopAndDestroy( respmsg );
    HBufC8* msg = encoder->EncodeMessageL( *indication ); 
    CleanupStack::PushL( msg );
    
// TCP framing used
    
    iStunclient->Implementation().iTransportProtocol = ETcpProtocol;
    CNATFWUNSAFTcpRelayPacket* packet = CNATFWUNSAFTcpRelayPacket::NewLC(
        *msg, CNATFWUNSAFTcpRelayPacket::EFrameTypeStun);    
    
    CBufBase* message = packet->EncodeL();
    CleanupStack::PushL( message );
    TInt length = message->Size();
    HBufC8* readBuf = HBufC8::NewLC( length );
    TPtr8 writable( readBuf->Des() );
    message->Read( 0, writable, length );
    
    EUNIT_ENABLE_ALLOC_DECORATOR;
    HBufC8* ptr = iImpl->HandleDataL( *readBuf, consumed, remoteAddr );
    EUNIT_ASSERT( NULL != ptr );
    EUNIT_ASSERT( EFalse == consumed );
    EUNIT_DISABLE_ALLOC_DECORATOR;
    delete ptr;
    CleanupStack::PopAndDestroy( readBuf );
    CleanupStack::PopAndDestroy( message );

    CNATFWUNSAFEncodedItem* encoded = CNATFWUNSAFEncodedItem::NewL(
        *packet, iStreamId, iSubstreamId );
    
    CleanupStack::PopAndDestroy( packet );

    CleanupStack::PushL( encoded );
    
    TBufC8<6> usernameA( KUsername );
    iImpl->iRequestType = iImpl->ESendRequest;
    
    EUNIT_ENABLE_ALLOC_DECORATOR;
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iImpl->CreateBindingRequestL( usernameA, password ) );
    
    _LIT( KServer, "10.32.194.251" );
    TInetAddr addr( KProtocolInetUdp );
    addr.Input( KServer );
    iImpl->iServerAddress = addr;
    
    iImpl->SendBindingRequestL();
    
    TPtr8 data = encoded->Ptr();
    
    ptr = iImpl->HandleDataL( data, consumed, remoteAddr );
    EUNIT_ASSERT( NULL == ptr );
    EUNIT_ASSERT( ETrue == consumed );
    
    delete ptr;

    data.Delete( 0, data.Length());
    
    CleanupStack::PopAndDestroy( encoded );

    CleanupStack::PopAndDestroy( msg );
    CleanupStack::PopAndDestroy( indication );
    CleanupStack::PopAndDestroy( encoder );
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_HandleDataL4L(  )
    {
    CUnsafMessageEncoder* encoder = CUnsafMessageEncoder::NewLC();
    
    TBool consumed = EFalse;
    _LIT8( KPassword,"aaaabbbbccccdddd");
    TBufC8<16> password( KPassword );
    TNATFWUNSAFTransactionID transactionID;
    iStunclient->ObtainTransactionIDL( transactionID );
    TInetAddr remoteAddr;
    iStunclient->Implementation().iTransportProtocol = ETcpProtocol;
    
    CNATFWUNSAFDataIndication* indication = CNATFWUNSAFDataIndication::NewLC( transactionID );
    
    CNATFWUNSAFBindingResponse* response = CNATFWUNSAFBindingResponse::NewLC( transactionID );
    HBufC8* respmsg = encoder->EncodeMessageL( *response );
    CleanupStack::PopAndDestroy( response );
    CleanupStack::PushL( respmsg );
    
    indication->AddAttributeL( CNATFWUNSAFDataAttribute::NewLC( *respmsg ) );
    CleanupStack::Pop(); // CNATFWUNSAFDataAttribute
    
    CleanupStack::PopAndDestroy( respmsg );
    HBufC8* msg = encoder->EncodeMessageL( *indication ); 
    CleanupStack::PushL( msg );
    
    CNATFWUNSAFTcpRelayPacket* packet =
    	CNATFWUNSAFTcpRelayPacket::NewLC( *msg, CNATFWUNSAFTcpRelayPacket::EFrameTypeData);     
    
    CNATFWUNSAFEncodedItem* encoded =
    	CNATFWUNSAFEncodedItem::NewLC( *packet, iStreamId, iSubstreamId );

    TPtr8 data = encoded->Ptr();
    
    HBufC8* ptr = iImpl->HandleDataL( data, consumed, remoteAddr );
    EUNIT_ASSERT( NULL != ptr );
    EUNIT_ASSERT( EFalse == consumed );
    
    delete ptr;
    
    data.Delete( 0, data.Length());
    CleanupStack::PopAndDestroy( encoded );
    CleanupStack::PopAndDestroy( packet );
    
    CleanupStack::PopAndDestroy( msg );
    CleanupStack::PopAndDestroy( indication );
    CleanupStack::PopAndDestroy( encoder );
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_STUNClientL(  )
    {
    const CSTUNClient* client = iImpl->STUNClient();
    delete iStunclient;
    iStunclient = NULL;
    const CSTUNClient* client2 = iImpl->STUNClient();
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_GetServerAddressLL(  )
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE( iImpl->GetServerAddressL() );
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_GetSharedSecretLL(  )
    {
    TInetAddr addr( KProtocolInetUdp );
    _LIT( KServer, "10.32.194.251" );
    addr.Input( KServer );
    iImpl->iServerAddress = addr;
    iImpl->iRequestType = iImpl->ESendRequest;    
    iImpl->GetSharedSecretL();
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_DetachClientL(  )
    {
    iImpl->DetachClient();
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_StoreAddressLL(  )
    {
    TInetAddr addr;
    iImpl->StoreAddressL( addr );
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_StoreAddressL_1L(  )
    {
    TInetAddr reflexiveAddr;
    TInetAddr relayAddr;
    iImpl->StoreAddressL( reflexiveAddr, relayAddr );
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_ChangeStateL(  )
    {
    CSTUNBindingGetAddress* getAddress = new ( ELeave ) CSTUNBindingGetAddress();
    iImpl->ChangeState( *getAddress );
    delete getAddress;
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_TerminateL(  )
    {
    _LIT( KServer, "10.32.194.251" );
        
    iImpl->iPublicAddr.SetFamily( KProtocolInetUdp );
    iImpl->iPublicAddr.Input( KServer );
    iImpl->CancelRequest();
    iImpl->Terminate( KErrNotFound );
    iImpl->Terminate( 1 );
    
    CSTUNBindingGetAddress* getAddress = new ( ELeave ) CSTUNBindingGetAddress();
    CleanupStack::PushL( getAddress );
    CSTUNBindingGetSharedSecret* getSharedSecret =
        new ( ELeave ) CSTUNBindingGetSharedSecret( *getAddress );
    CleanupStack::PushL( getSharedSecret );
    CSTUNBindingActive* active = new ( ELeave ) CSTUNBindingActive( *getSharedSecret );
    CleanupStack::PushL( active );
    iImpl->ChangeState( *active );    
    iImpl->Terminate( 300 );

    CleanupStack::PopAndDestroy( active );
    CleanupStack::PopAndDestroy( getSharedSecret );
    CleanupStack::PopAndDestroy( getAddress );
    
    iImpl->Terminate( 401 );
    iImpl->Terminate( 434 );
    iImpl->Terminate( 436 );
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_IsWaitingSharedSecretL(  )
    {
    iImpl->IsWaitingSharedSecret();
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_SharedSecretObtainedLL(  )
    {
    TBufC8<16> password( KPassword );
    TBufC8<6> usernameA( KUsername );
    EUNIT_ASSERT_PANIC( iImpl->SharedSecretObtainedL( usernameA, password ), "", KErrGeneral );    
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_CreateBindingRequestLL(  )
    {
    TBufC8<16> password( KPassword );
    TBufC8<6> usernameA( KUsername );
    TICEAttributes attributes;
    attributes.iPriority = 36;
    attributes.iUseCandidate = ETrue;
    iImpl->SetICESpecificAttributes( attributes );
    iImpl->iAddXorOnly = ETrue;

    iImpl->iRequestType = iImpl->ESendRequest;
    iImpl->CreateBindingRequestL( usernameA, password );
    
    TBufC8<1> dummy;
    iImpl->CreateBindingRequestL( dummy, password );
    iImpl->iTransactionError = 401;
    NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE( iImpl->CreateBindingRequestL( dummy, password ), KErrAccessDenied );
    NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE( iImpl->CreateBindingRequestL( usernameA, dummy ), KErrAccessDenied );    
    
    iImpl->AllocateRequestL( KDefaultRtoValue );
    iImpl->iRequestType = iImpl->EAllocateRequest;
    iImpl->CreateBindingRequestL( usernameA, password );
    
    TInetAddr addr;
    TUint32 timerValue = 1000000;
    iImpl->SetActiveDestinationRequestL( addr, timerValue );
    iImpl->CreateBindingRequestL( usernameA, password );
    
    iImpl->ConnectRequestL( addr );
    iImpl->CreateBindingRequestL( usernameA, password );
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_SendBindingRequestLL(  )
    {
    TBufC8<16> password( KPassword );
    TBufC8<6> usernameA( KUsername );
    iImpl->iRequestType = iImpl->ESendRequest;
    iImpl->CreateBindingRequestL( usernameA, password );
        
    TInetAddr addr( KProtocolInetUdp );
    _LIT( KServer, "10.32.194.251" );
    addr.Input( KServer );
    iImpl->iServerAddress = addr; 
    iImpl->SendBindingRequestL();
    
    CSTUNRelayBindingImplementation* impl = 
        CSTUNRelayBindingImplementation::NewL( *iBinding, *this, iSocket );
    
    RSocket socket = impl->Socket();
    delete impl;
    
    //iImpl->SendBindingRequestL();    
    
    delete iImpl->iRequest;
    iImpl->iRequest = NULL;
    delete iImpl->iTransaction;
    iImpl->iTransaction = NULL;        
    EUNIT_ASSERT_PANIC( iImpl->SendBindingRequestL(), "", KErrNotFound );
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_HandleTransactionErrorL(  )
    {
    TBufC8<16> password( KPassword );
    TBufC8<6> usernameA( KUsername );
    iImpl->iRequestType = iImpl->ESendRequest;
    iImpl->CreateBindingRequestL( usernameA, password );    
    iImpl->HandleTransactionError( 2 );
    EUNIT_ASSERT( iImpl->HandleTransactionError( KErrTimedOut ) == ETrue );   
    EUNIT_ASSERT( iImpl->HandleTransactionError( 401 ) == EFalse );
    EUNIT_ASSERT( iImpl->HandleTransactionError( 401 ) == EFalse ); // errorcount = 3!
    
    iImpl->HandleTransactionError( 432 );
    iImpl->HandleTransactionError( 420 );
    iImpl->iErrorResponseCount = 0;
    
    EUNIT_ASSERT( iImpl->HandleTransactionError( 430 ) == ETrue );       
    iImpl->HandleTransactionError( 431 );
    iImpl->iErrorResponseCount = 0;
    
    iImpl->HandleTransactionError( 432 );
    EUNIT_ASSERT( iImpl->HandleTransactionError( 433 ) == ETrue );
    iImpl->iErrorResponseCount = 0;
    
    EUNIT_ASSERT( iImpl->HandleTransactionError( 434 ) == ETrue );
    
    delete iImpl->iRequest;
    iImpl->iRequest = NULL;    
    TNATFWUNSAFTransactionID transactionID;
    iImpl->iRequest = CNATFWUNSAFBindingRequest::NewL( transactionID );
    iImpl->iRequest->AddAttributeL( CNATFWUNSAFRealmAttribute::NewLC( dataDesc5 ) );
    CleanupStack::Pop(); //CNATFWUNSAFRealmAttribute 
    iImpl->iRealmFromResponse = dataDesc5.AllocL();
    iImpl->iErrorResponseCount = 0;
    
    EUNIT_ASSERT( iImpl->HandleTransactionError( 436 ) == ETrue );
    
    //nonce
    delete iImpl->iRequest;
    iImpl->iRequest = NULL;
    iImpl->iRequestType = iImpl->ESendRequest;    
    iImpl->CreateBindingRequestL( usernameA, password );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iImpl->HandleTransactionError( 435 ) );
    iImpl->iErrorResponseCount = 0;
    
    delete iImpl->iRequest;
    iImpl->iRequest = NULL;
    iImpl->iRequestType = iImpl->ESendRequest;    
    iImpl->CreateBindingRequestL( usernameA, password );
    EUNIT_ASSERT( iImpl->HandleTransactionError( 435 ) == ETrue );
    iImpl->iErrorResponseCount = 0;

    delete iImpl->iRequest;
    iImpl->iRequest = NULL;
    //iImpl->iError436Count = 3;
    iImpl->iRequestType = iImpl->ESendRequest;
    iImpl->CreateBindingRequestL( usernameA, password );
    iImpl->iRequest->AddAttributeL( CNATFWUNSAFRealmAttribute::NewLC( dataDesc5 ) );
    CleanupStack::Pop(); //CNATFWUNSAFRealmAttribute
    delete iImpl->iRealmFromResponse;
    iImpl->iRealmFromResponse = NULL;    
    iImpl->iRealmFromResponse = dataDesc5.AllocL();
        
    EUNIT_ASSERT( iImpl->HandleTransactionError( 436 ) == ETrue );
    iImpl->iErrorResponseCount = 0;
        
    iImpl->HandleTransactionError( 438 );     
    iImpl->HandleTransactionError( 500 );
    iImpl->iErrorResponseCount = 0;   
    iImpl->HandleTransactionError( 600 );    

    CSTUNBindingGetAddress* getAddress = new ( ELeave ) CSTUNBindingGetAddress();
    CleanupStack::PushL( getAddress );
    CSTUNBindingGetSharedSecret* getSharedSecret =
        new ( ELeave ) CSTUNBindingGetSharedSecret( *getAddress );
    CleanupStack::PushL( getSharedSecret );
        
    CSTUNBindingActive* active = new ( ELeave ) CSTUNBindingActive( *getSharedSecret );
    CleanupStack::PushL( active );
    iImpl->ChangeState( *active );
    TInetAddr alternativeServer;
    EUNIT_ASSERT( iImpl->HandleTransactionError( 300 ) == EFalse ); 
    iImpl->iErrorResponseCount = 0; 
    
    delete iImpl->iRequest;
    iImpl->iRequest = NULL;   
    iImpl->iRequest = CNATFWUNSAFBindingRequest::NewL( transactionID );    
    iImpl->iRequest->AddAttributeL( CNATFWUNSAFAlternateServerAttribute::NewLC( alternativeServer ) );
    CleanupStack::Pop(); //CNATFWUNSAFAlternateServerAttribute
    iImpl->iRequest->AddAttributeL( CNATFWUNSAFRealmAttribute::NewLC( dataDesc5 ) );
    CleanupStack::Pop(); //CNATFWUNSAFRealmAttribute
    
    delete iImpl->iSharedSecret;
    iImpl->iSharedSecret = NULL;      
    EUNIT_ASSERT( iImpl->HandleTransactionError( 401 ) == ETrue );
    
    delete iImpl->iRequest;
    iImpl->iRequest = NULL;
    iImpl->iRequestType = iImpl->ESendRequest;    
    iImpl->CreateBindingRequestL( usernameA, password );
    iImpl->iRequest->AddAttributeL( CNATFWUNSAFAlternateServerAttribute::NewLC( alternativeServer ) );
    CleanupStack::Pop(); //CNATFWUNSAFAlternateServerAttribute
    iImpl->iErrorResponseCount = 0;
    
    EUNIT_ASSERT( iImpl->HandleTransactionError( 436 ) == ETrue );
    
    delete iImpl->iSharedSecret;
    iImpl->iSharedSecret = NULL;
    EUNIT_ASSERT( iImpl->HandleTransactionError( 401 ) == ETrue );    
    
    delete iImpl->iRequest;
    iImpl->iRequest = NULL;
    iImpl->iRequestType = iImpl->ESendRequest;
    iImpl->CreateBindingRequestL( usernameA, password );
    delete iImpl->iSharedSecret;
    iImpl->iSharedSecret = NULL;
    
    iImpl->iErrorResponseCount = 0;            
    EUNIT_ASSERT( iImpl->HandleTransactionError( 430 ) == EFalse );           
    EUNIT_ASSERT( iImpl->HandleTransactionError( 433 ) == EFalse );    
    
    CleanupStack::PopAndDestroy( active );
    CleanupStack::PopAndDestroy( getSharedSecret );
    CleanupStack::PopAndDestroy( getAddress );
    
    iImpl->iErrorResponseCount = 0;
    iImpl->iRequest->AddAttributeL( CNATFWUNSAFAlternateServerAttribute::NewLC( alternativeServer ) );
    CleanupStack::Pop(); //CNATFWUNSAFUsernameAttribute 
    EUNIT_ASSERT( iImpl->HandleTransactionError( 1001 ) == ETrue );   
    EUNIT_ASSERT( iImpl->HandleTransactionError( 300 ) == ETrue );
    
    // TURN SPECIFIC
    iImpl->iErrorResponseCount = 0;
    EUNIT_ASSERT( iImpl->HandleTransactionError( 437 ) == EFalse );
    EUNIT_ASSERT( iImpl->HandleTransactionError( 439 ) == EFalse );
    
    iImpl->iErrorResponseCount = 0;
    EUNIT_ASSERT( iImpl->HandleTransactionError( 442 ) == EFalse );
    EUNIT_ASSERT( iImpl->HandleTransactionError( 443 ) == EFalse );
    
    iImpl->iErrorResponseCount = 0;
    EUNIT_ASSERT( iImpl->HandleTransactionError( 444 ) == EFalse );
    EUNIT_ASSERT( iImpl->HandleTransactionError( 445 ) == EFalse );    
    
    iImpl->iErrorResponseCount = 0;
    EUNIT_ASSERT( iImpl->HandleTransactionError( 446 ) == EFalse );
    EUNIT_ASSERT( iImpl->HandleTransactionError( 486 ) == EFalse );

    iImpl->iErrorResponseCount = 0;
    EUNIT_ASSERT( iImpl->HandleTransactionError( 507 ) == EFalse );
    
    iImpl->iErrorResponseCount = 0;
    EUNIT_ASSERT( iImpl->HandleTransactionError( 400 ) == EFalse );
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_HandleTransactionError_AllocL(  )
    {
    TBufC8<16> password( KPassword );
    TBufC8<6> usernameA( KUsername );
    TInetAddr alternativeServer;
    
    delete iImpl->iRequest;
    iImpl->iRequest = NULL;    
    TNATFWUNSAFTransactionID transactionID;
    iImpl->iRequest = CNATFWUNSAFBindingRequest::NewL( transactionID );
    iImpl->iRequest->AddAttributeL( CNATFWUNSAFRealmAttribute::NewLC( dataDesc5 ) );
    CleanupStack::Pop(); //CNATFWUNSAFRealmAttribute 
    iImpl->iRealmFromResponse = dataDesc5.AllocL();     
    EUNIT_ASSERT( iImpl->HandleTransactionError( 434 ) == ETrue );
    
    delete iImpl->iRealmFromResponse;
    iImpl->iRealmFromResponse = NULL;
    iImpl->iRealmFromResponse = dataDesc5.AllocL();
    iImpl->iErrorResponseCount = 0; 
    EUNIT_ASSERT( iImpl->HandleTransactionError( 434 ) == ETrue );

    delete iImpl->iRequest;
    iImpl->iRequest = NULL;
    iImpl->iRequestType = iImpl->ESendRequest;
    iImpl->CreateBindingRequestL( usernameA, password );
    iImpl->iRequest->AddAttributeL( CNATFWUNSAFAlternateServerAttribute::NewLC( alternativeServer ) );
    CleanupStack::Pop(); //CNATFWUNSAFAlternateServerAttribute 
    EUNIT_ASSERT( iImpl->HandleTransactionError( 401 ) == EFalse );
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_IcmpErrorL(  )
    {
    TInetAddr addr( KProtocolInetUdp );
    _LIT( KServer, "10.32.194.251" );
    addr.Input( KServer );
    
    iImpl->IcmpError( addr );
    
    TBufC8<16> password( KPassword );
    TBufC8<6> usernameA( KUsername );
    iImpl->iRequestType = iImpl->ESendRequest;
    iImpl->CreateBindingRequestL( usernameA, password );

    iImpl->iServerAddress = addr;     
    iImpl->SendBindingRequestL();
    //iImpl->IcmpError( addr );  
    EUNIT_ASSERT_PANIC( iImpl->IcmpError( addr ), "", KErrGeneral );
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_WaitBeforeRetryingL(  )
    {
    iImpl->WaitBeforeRetrying();
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_SetICESpecificAttributesL(  )
    {
    TICEAttributes attributes;
    iImpl->SetICESpecificAttributes( attributes );
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_FetchCandidateLL(  )
    {
    TInetAddr relayAddr;
    
    TInetAddr addr( KProtocolInetUdp );
    _LIT( KAddr, "10.32.194.251" );
    addr.Input( KAddr );
    iImpl->iPublicAddr = addr;
    relayAddr = addr;
    NATFW_EUNIT_ASSERT_NO_LEAVE( relayAddr = iImpl->RelayAddr( ) );   
    }
    
void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_ValidateMsgTypeL()
    {
    CNATFWUNSAFMessage* msg = NULL;
    
    TNATFWUNSAFTransactionID transactionID;
    msg = CNATFWUNSAFBindingRequest::NewL( transactionID );    
    EUNIT_ASSERT_EQUALS( iImpl->ValidateMsgType( msg ), EFalse );
    delete msg;
    msg = NULL;
    
    msg = CNATFWUNSAFBindingResponse::NewL( transactionID );
    EUNIT_ASSERT_EQUALS( iImpl->ValidateMsgType( msg ), ETrue );
    delete msg;
    msg = NULL;
    
    msg = CNATFWUNSAFAllocateResponse::NewL( transactionID );
    EUNIT_ASSERT_EQUALS( iImpl->ValidateMsgType( msg ), ETrue );
    delete msg;
    msg = NULL;
    
    msg = CNATFWUNSAFBindingErrorResponse::NewL( transactionID );
    EUNIT_ASSERT_EQUALS( iImpl->ValidateMsgType( msg ), ETrue );
    delete msg;
    msg = NULL;
    
    msg = CNATFWUNSAFAllocateErrorResponse::NewL( transactionID );
    EUNIT_ASSERT_EQUALS( iImpl->ValidateMsgType( msg ), ETrue );
    delete msg;
    msg = NULL;

    msg = CNATFWUNSAFConnectResponse::NewL( transactionID );
    EUNIT_ASSERT_EQUALS( iImpl->ValidateMsgType( msg ), ETrue );
    delete msg;
    msg = NULL;
    
    msg = CNATFWUNSAFConnectErrorResponse::NewL( transactionID );
    EUNIT_ASSERT_EQUALS( iImpl->ValidateMsgType( msg ), ETrue );
    delete msg;
    msg = NULL;
    
    msg = CNATFWUNSAFSetActiveDestinationResponse::NewL( transactionID );
    EUNIT_ASSERT_EQUALS( iImpl->ValidateMsgType( msg ), ETrue );
    delete msg;
    msg = NULL;  
    
    msg = CNATFWUNSAFSetActiveDestinationErrorResponse::NewL( transactionID );
    EUNIT_ASSERT_EQUALS( iImpl->ValidateMsgType( msg ), ETrue );
    delete msg;
    msg = NULL;          
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_SetActiveDestinationRequestLL(  )
    {
    TInetAddr remoteAddr;
    TUint32 timerValue = 1000000;
    iImpl->SetActiveDestinationRequestL( remoteAddr, timerValue );
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_SendIndicationLL(  )
    {
    TInetAddr addr( KProtocolInetUdp );
    _LIT( KAddr, "10.32.194.251" );
    addr.Input( KAddr );
    _LIT8( KDummypacket, "oysescatechnologiesabFIN" );
    TBufC8<24> buffi( KDummypacket );
    TBufC8<16> password( KPassword );
    TBufC8<6> usernameA( KUsername );
    iImpl->iRequestType = iImpl->ESendRequest;
    iImpl->CreateBindingRequestL( usernameA, password );
    
    iImpl->iServerAddress = addr;   
    iImpl->SendIndicationL( addr, buffi, EFalse );
    
    iStunclient->Implementation().iTransportProtocol = ETcpProtocol;
    iImpl->SendIndicationL( addr, buffi, EFalse );
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_ConnectRequestLL(  )
    {
    TInetAddr remoteAddr;
    iImpl->ConnectRequestL( remoteAddr );
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_StreamIdL(  )
    {
    TUint streamId = iImpl->StreamId();
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_ConnectionIdL(  )
    {
    TUint connectionId = iImpl->ConnectionId();
    }

void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_RealmFromResponseL()
    {
    const HBufC8* response = iImpl->RealmFromResponse();
    delete response;
    }

/*
void ut_cstunrelaybindingimplementation::UT_CSTUNRelayBindingImplementation_ExecuteImmediateRetransmitLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iImpl->ExecuteImmediateRetransmitL() );
    
    TBufC8<16> password( KPassword );
    TBufC8<6> usernameA( KUsername );
    iImpl->iRequestType = iImpl->ESendRequest;
    iImpl->CreateBindingRequestL( usernameA, password );
        
    TInetAddr addr( KProtocolInetUdp );
    _LIT( KServer, "10.32.194.251" );
    addr.Input( KServer );
    iImpl->iServerAddress = addr; 
    iImpl->SendBindingRequestL();
    iImpl->ExecuteImmediateRetransmitL();
    }
*/
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    ut_cstunrelaybindingimplementation,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CSTUNRelayBindingImplementation",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_NewLL, Teardown)

EUNIT_TEST(
    "NewL - test ",
    "CSTUNRelayBindingImplementation",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_NewL_1L, Teardown)

EUNIT_TEST(
    "TimerExpiredL - test ",
    "CSTUNRelayBindingImplementation",
    "TimerExpiredL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_TimerExpiredLL, Teardown)

EUNIT_TEST(
    "LeaveFromTimerExpired - test ",
    "CSTUNRelayBindingImplementation",
    "LeaveFromTimerExpired",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_LeaveFromTimerExpiredL, Teardown)

EUNIT_TEST(
    "PublicAddressObtainedL - test ",
    "CSTUNRelayBindingImplementation",
    "PublicAddressObtainedL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_PublicAddressObtainedLL, Teardown)

EUNIT_TEST(
    "PublicAddressObtainedL - test ",
    "CSTUNRelayBindingImplementation",
    "PublicAddressObtainedL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_PublicAddressObtainedL_1L, Teardown)

EUNIT_TEST(
    "TransactionError - test ",
    "CSTUNRelayBindingImplementation",
    "TransactionError",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_TransactionErrorL, Teardown)

EUNIT_TEST(
    "TransactionEventOccurredL - test ",
    "CSTUNRelayBindingImplementation",
    "TransactionEventOccurredL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_TransactionEventOccurredLL, Teardown)

EUNIT_TEST(
    "AllocateRequestL - test ",
    "CSTUNRelayBindingImplementation",
    "AllocateRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_AllocateRequestLL, Teardown)

EUNIT_TEST(
    "CancelRequest - test ",
    "CSTUNRelayBindingImplementation",
    "CancelRequest",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_CancelRequestL, Teardown)

EUNIT_TEST(
    "Socket - test ",
    "CSTUNRelayBindingImplementation",
    "Socket",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_SocketL, Teardown)

EUNIT_TEST(
    "AddressResolved - test ",
    "CSTUNRelayBindingImplementation",
    "AddressResolved",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_AddressResolvedL, Teardown)

EUNIT_TEST(
    "PublicAddr - test ",
    "CSTUNRelayBindingImplementation",
    "PublicAddr",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_PublicAddrL, Teardown)

EUNIT_TEST(
    "AlternateServerAddr - test ",
    "CSTUNRelayBindingImplementation",
    "AlternateServerAddr",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_AlternateServerAddrL, Teardown)

EUNIT_TEST(
    "HandleDataL - test ",
    "CSTUNRelayBindingImplementation",
    "HandleDataL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_HandleDataLL, Teardown)

EUNIT_TEST(
    "HandleDataL - test2 ",
    "CSTUNRelayBindingImplementation",
    "HandleDataL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_HandleDataL2L, Teardown)

EUNIT_TEST(
    "HandleDataL - test3",
    "CSTUNRelayBindingImplementation",
    "HandleDataL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_HandleDataL3L, Teardown)

EUNIT_TEST(
	"HandleDataL - test4",
	"CSTUNRelayBindingImplementation",
	"HandleDataL",
	"FUNCTIONALITY",
	SetupL, UT_CSTUNRelayBindingImplementation_HandleDataL4L, Teardown)
     
EUNIT_TEST(
    "STUNClient - test ",
    "CSTUNRelayBindingImplementation",
    "STUNClient",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_STUNClientL, Teardown)

EUNIT_TEST(
    "GetServerAddressL - test ",
    "CSTUNRelayBindingImplementation",
    "GetServerAddressL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_GetServerAddressLL, Teardown)

EUNIT_TEST(
    "GetSharedSecretL - test ",
    "CSTUNRelayBindingImplementation",
    "GetSharedSecretL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_GetSharedSecretLL, Teardown)

EUNIT_TEST(
    "DetachClient - test ",
    "CSTUNRelayBindingImplementation",
    "DetachClient",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_DetachClientL, Teardown)

EUNIT_TEST(
    "StoreAddressL - test ",
    "CSTUNRelayBindingImplementation",
    "StoreAddressL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_StoreAddressLL, Teardown)

EUNIT_TEST(
    "StoreAddressL - test ",
    "CSTUNRelayBindingImplementation",
    "StoreAddressL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_StoreAddressL_1L, Teardown)

EUNIT_TEST(
    "ChangeState - test ",
    "CSTUNRelayBindingImplementation",
    "ChangeState",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_ChangeStateL, Teardown)

EUNIT_TEST(
    "Terminate - test ",
    "CSTUNRelayBindingImplementation",
    "Terminate",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_TerminateL, Teardown)

EUNIT_TEST(
    "IsWaitingSharedSecret - test ",
    "CSTUNRelayBindingImplementation",
    "IsWaitingSharedSecret",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_IsWaitingSharedSecretL, Teardown)

EUNIT_TEST(
    "SharedSecretObtainedL - test ",
    "CSTUNRelayBindingImplementation",
    "SharedSecretObtainedL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_SharedSecretObtainedLL, Teardown)

EUNIT_TEST(
    "CreateBindingRequestL - test ",
    "CSTUNRelayBindingImplementation",
    "CreateBindingRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_CreateBindingRequestLL, Teardown)

EUNIT_TEST(
    "SendBindingRequestL - test ",
    "CSTUNRelayBindingImplementation",
    "SendBindingRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_SendBindingRequestLL, Teardown)

EUNIT_TEST(
    "HandleTransactionError - test ",
    "CSTUNRelayBindingImplementation",
    "HandleTransactionError",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_HandleTransactionErrorL, Teardown)

EUNIT_TEST(
    "IcmpError - test ",
    "CSTUNRelayBindingImplementation",
    "IcmpError",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_IcmpErrorL, Teardown)

EUNIT_TEST(
    "WaitBeforeRetrying - test ",
    "CSTUNRelayBindingImplementation",
    "WaitBeforeRetrying",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_WaitBeforeRetryingL, Teardown)

EUNIT_TEST(
    "SetICESpecificAttributes - test ",
    "CSTUNRelayBindingImplementation",
    "SetICESpecificAttributes",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_SetICESpecificAttributesL, Teardown)

EUNIT_TEST(
    "FetchCandidateL - test ",
    "CSTUNRelayBindingImplementation",
    "FetchCandidateL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_FetchCandidateLL, Teardown)

EUNIT_TEST(
    "ValidateMsgType - test ",
    "CSTUNRelayBindingImplementation",
    "ValidateMsgType",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_ValidateMsgTypeL, Teardown)

EUNIT_TEST(
    "SetActiveDestinationRequestL - test ",
    "CSTUNRelayBindingImplementation",
    "SetActiveDestinationRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_SetActiveDestinationRequestLL, Teardown)

EUNIT_TEST(
    "SendIndicationL - test ",
    "CSTUNRelayBindingImplementation",
    "SendIndicationL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_SendIndicationLL, Teardown)

EUNIT_TEST(
    "ConnectRequestL - test ",
    "CSTUNRelayBindingImplementation",
    "ConnectRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_ConnectRequestLL, Teardown)

EUNIT_TEST(
    "StreamId - test ",
    "CSTUNRelayBindingImplementation",
    "StreamId",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_StreamIdL, Teardown)

EUNIT_TEST(
    "ConnectionId - test ",
    "CSTUNRelayBindingImplementation",
    "ConnectionId",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_ConnectionIdL, Teardown)
    
EUNIT_TEST(
    "RealmFromResponse - test ",
    "CSTUNRelayBindingImplementation",
    "RealmFromResponse",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_RealmFromResponseL, Teardown)    

/*EUNIT_TEST(
    "ExecuteImmediateRetransmitL - test ",
    "CSTUNRelayBindingImplementation",
    "ExecuteImmediateRetransmitL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBindingImplementation_ExecuteImmediateRetransmitLL, Teardown)*/


EUNIT_END_TEST_TABLE

//  END OF FILE
