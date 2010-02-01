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
#include "ut_cstunbindingimplementation.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <digia/eunit/EUnitDecorators.h>
#include <commdbconnpref.h>
#include <e32debug.h>


//  INTERNAL INCLUDES
#include "natfwstunbinding.h"
#include "cstunbindingimplementation.h"
#include "mstunbindingobserver.h"
#include "natfwstunclientdefs.h"
#include "natfwunsafunknownattributesattribute.h"
#include "natfwunsafbindingrequest.h"
#include "natfwunsafbindingresponse.h"
#include "natfwunsafbindingerrorresponse.h"
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
#include "natfwunittestmacros.h"

_LIT8( KServerAddress, "10.32.194.251" );
_LIT8( KUsername, "userAA" );
_LIT8( KPassword,"salasana");
_LIT8(KRealmVal,"\"myrealm\"");
_LIT8(KRealmVal2,"\"myREALMI\"");
_LIT8(KNonceVal,"mynonce");

const TDesC8& dataDesc(KRealmVal);
const TDesC8& dataDesc2(KRealmVal2);
const TDesC8& dataDesc3(KNonceVal);

const TUint KNoFingerprint = EFalse; 
const TUint KDefaultRtoValue = 0;

// CONSTRUCTION
ut_cstunbindingimplementation* ut_cstunbindingimplementation::NewL()
    {
    ut_cstunbindingimplementation* self = ut_cstunbindingimplementation::NewLC();
    CleanupStack::Pop();

    return self;
    }

ut_cstunbindingimplementation* ut_cstunbindingimplementation::NewLC()
    {
    ut_cstunbindingimplementation* self = new( ELeave ) ut_cstunbindingimplementation();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
ut_cstunbindingimplementation::~ut_cstunbindingimplementation()
    {
    }

// Default constructor
ut_cstunbindingimplementation::ut_cstunbindingimplementation()
    {
    }

// Second phase construct
void ut_cstunbindingimplementation::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS
// from mstunclientbserver
void ut_cstunbindingimplementation::STUNClientInitCompleted( const CSTUNClient& /*aClient*/,
                              TInt aCompletionCode )
    {
    RDebug::Print( _L("**NATFW STUNClientInitCompleted, code: %d"), aCompletionCode );
    CActiveScheduler::Stop();
    }


void ut_cstunbindingimplementation::STUNBindingEventOccurredL( TSTUNBindingEvent aEvent, 
                                const CBinding& /*aBinding*/ )
    {
    RDebug::Print( _L("**NATFW STUNBindingEventOccurredL, code: %d"), aEvent );
    CActiveScheduler::Stop();
    }


void ut_cstunbindingimplementation::STUNBindingErrorOccurred( const CBinding& /*aBinding*/, 
                               TInt aError )
    {
    RDebug::Print( _L("**NATFW STUNBindingErrorOccurred, code: %d"), aError );
    CActiveScheduler::Stop();
    }

void ut_cstunbindingimplementation::Notify( TUint /*aSessionId*/, TUint /*aStreamId*/, 
    TNotifyType /*aType*/, TInt /*aError*/ )
    {
    RDebug::Print( _L("**NATFW Notify") );
    }


const CSTUNClient& ut_cstunbindingimplementation::STUNClient() const
    {
    return *iStunclient;
    }

CDeltaTimer& ut_cstunbindingimplementation::TimerProvider()
    {
    return *iDeltatimer;
    }
        
TInt ut_cstunbindingimplementation::RetransmitInterval() const
    {
    TInt dummy(0);
    return dummy;
    }

TTransportProtocol ut_cstunbindingimplementation::TransportProtocol() const
    {
    
    }

void ut_cstunbindingimplementation::AddressResolvedL( const CBinding& /*aBinding*/ ) const
    {
    
    }

void ut_cstunbindingimplementation::ObtainSharedSecretL( CBinding& /*aBinding*/ )
    {
    TBufC8<16> password( KPassword );
    TBufC8<6> usernameA( KUsername );    
    
    iImpl->CreateBindingRequestL( usernameA, password );
    iImpl->SendBindingRequestL();
    }
    
TBool ut_cstunbindingimplementation::SharedSecretRejectedL( 
    const CBinding& /*aBinding*/, const TDesC8& /*aUsername*/, const TDesC8& /*aPassword*/ )
    {
    TBool dummy( EFalse );
    return dummy;    
    }

TBool ut_cstunbindingimplementation::ObtainServerAddress( TInetAddr& aAddress )
    {
    #ifdef _DEBUG_EUNIT
        TInetAddr addr;
        addr.SetAddress( INET_ADDR( 10,32,194,251 ) );
        aAddress = addr;
        return ETrue;
    #endif

    return EFalse;
    }

void ut_cstunbindingimplementation::ObtainTransactionIDL( TNATFWUNSAFTransactionID& /*aTransactionID*/ )
    {
    
    }
    
void ut_cstunbindingimplementation::AttachBindingL( const CBinding& /*aBinding*/ )
    {
    }

void ut_cstunbindingimplementation::DetachBinding( const CBinding& /*aBinding*/ )
    {
    }
void ut_cstunbindingimplementation::BindingErrorL( const CBinding& /*aBinding*/, TInt aError, TBool /*aIsFatal*/ )
    {
    RDebug::Print( _L("**NATFW BindingErrorL, error: %d"), aError );
    }
void ut_cstunbindingimplementation::BindingEventOccurred( const CBinding& /*aBinding*/, 
                           TSTUNCallbackInfo::TFunction /*aEvent*/ )
    {
    
    }

const TDesC8& ut_cstunbindingimplementation::UsernameForIndication()
    {
    
    }

const TDesC8& ut_cstunbindingimplementation::PasswordForIndication()
    {
    
    }


void ut_cstunbindingimplementation::SetupL(  )
    {
    TUint iapID( 6 );
    TInt retransmitInterval( 10 );
    TBufC8<13> serveraddress( KServerAddress );
    TUint serverport( 2000 );
    TBufC8<4> protocol( KStun );
    TBool obtainsharedsecret( EFalse );
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
                                     protocol,
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
    iBinding = CSTUNBinding::NewL( *iStunclient, iSocket );
    
    iImpl = CSTUNBindingImplementation::NewL( *iBinding, *this, iSocket );
    }

void ut_cstunbindingimplementation::Teardown(  )
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

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_NewLL(  )
    {
    CSTUNBindingImplementation* impl = 
        CSTUNBindingImplementation::NewL( *iBinding, *this, iSocket );
    delete impl;
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_NewL_1L(  )
    {
    CSTUNBindingImplementation* impl = 
        CSTUNBindingImplementation::NewL( *iBinding, *this, iStreamId, iSubstreamId, iMux );
    delete impl;
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_TimerExpiredLL(  )
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
    iImpl->TimerExpiredL();
    
    CleanupStack::PopAndDestroy( waitToRetry );
    CleanupStack::PopAndDestroy( getSharedSecret );
    CleanupStack::PopAndDestroy( getAddress );
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_LeaveFromTimerExpiredL(  )
    {
    //EUNIT_ASSERT_PANIC( iImpl->LeaveFromTimerExpired( KErrNone), "", KErrArgument );    
    iImpl->LeaveFromTimerExpired( KErrNotFound );
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_PublicAddressObtainedLL(  )
    {
    TInetAddr addr;
    CSTUNBindingGetAddress* getAddress = new ( ELeave ) CSTUNBindingGetAddress();
    iImpl->ChangeState( *getAddress );
    iImpl->PublicAddressObtainedL( addr );
    delete getAddress;
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_PublicAddressObtainedL_1L(  )
    {
    TInetAddr reflexsiveAddr;
    TInetAddr relayAddr;
    
    CSTUNBindingGetAddress* getAddress = new ( ELeave ) CSTUNBindingGetAddress();
    iImpl->ChangeState( *getAddress );
    NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE( iImpl->PublicAddressObtainedL( reflexsiveAddr, relayAddr ), KErrNotSupported )
    delete getAddress;
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_TransactionErrorL(  )
    {
    TBufC8<16> password( KPassword );
    TBufC8<6> usernameA( KUsername );      
    iImpl->CreateBindingRequestL( usernameA, password );    
    
    CSTUNBindingGetAddress* getAddress = new ( ELeave ) CSTUNBindingGetAddress();
    CleanupStack::PushL( getAddress );
    iImpl->ChangeState( *getAddress );
    iImpl->TransactionError( 401, CNATFWUNSAFUnknownAttributesAttribute::NewLC() );
    CleanupStack::Pop();
    CleanupStack::PopAndDestroy( getAddress );
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_TransactionEventOccurredLL(  )
    {
    iImpl->TransactionEventOccurredL( TSTUNCallbackInfo::EInitCompleted );
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_SendRequestLL(  )
    {
    iImpl->SendRequestL();
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_SendRequestL_1L(  )
    {
    TInetAddr addr;
    iImpl->SendRequestL( addr, KNoFingerprint, KDefaultRtoValue );
    
    addr.SetFamily( KAfInet );
    iImpl->SendRequestL( addr, KNoFingerprint, 1 );
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_CancelRequestL(  )
    {
    iImpl->CancelRequest();
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_SocketL(  )
    {
    RSocket socket = iImpl->Socket();
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_AddressResolvedL(  )
    {
    TBool isResolved = iImpl->AddressResolved();
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_PublicAddrL(  )
    {
    TInetAddr addr = iImpl->PublicAddr();
    }
    
void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_AlternateServerAddrL()
    {
    TInetAddr addr = iImpl->AlternateServerAddr();
    }
    
void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_HandleDataLL(  )
    {
    _LIT8( KPassword,"aaaabbbbccccdddd");
    TBufC8<16> password( KPassword );
    _LIT8( KTransactionId,"aaaabbbb");
    TNATFWUNSAFTransactionID transactionID;
    transactionID.Copy( KTransactionId );
    
    CNATFWUNSAFBindingRequest* request = CNATFWUNSAFBindingRequest::NewLC( transactionID );
    request->AddAttributeL( CNATFWUNSAFRealmAttribute::NewLC( dataDesc ) );
    CleanupStack::Pop();
    request->AddAttributeL( CNATFWUNSAFNonceAttribute::NewLC( dataDesc3 ) );
    CleanupStack::Pop();
    
    CBufBase* msg = request->EncodeL( password );
    CleanupStack::PushL( msg );        
    TInt length = msg->Size();
    HBufC8* readBuf1 = HBufC8::NewLC( length );
    TPtr8 writable1( readBuf1->Des() );
    msg->Read( 0, writable1 );
    
    TInetAddr remoteAddr;
    TBool consumed = EFalse;
    HBufC8* ptr = iImpl->HandleDataL( *readBuf1, consumed, remoteAddr );
    EUNIT_ASSERT( NULL == ptr );             
    EUNIT_ASSERT( EFalse == consumed );
    
    delete ptr;
    CleanupStack::PopAndDestroy( readBuf1 );
    CleanupStack::PopAndDestroy( msg );
    CleanupStack::PopAndDestroy( request );
    
    CNATFWUNSAFDataIndication* indication = CNATFWUNSAFDataIndication::NewLC( transactionID );
    _LIT8( KDummypacket, "oysescatechnologiesabFIN" );
    TBufC8<24> buffi( KDummypacket );    
    indication->AddAttributeL( CNATFWUNSAFDataAttribute::NewLC( buffi ) );
    CleanupStack::Pop();
    indication->AddAttributeL( CNATFWUNSAFRemoteAddressAttribute::NewLC( remoteAddr ) );
    CleanupStack::Pop();
        
    CBufBase* msg1 = indication->EncodeL( password );    
    CleanupStack::PushL( msg1 );
    length = msg1->Size();
    HBufC8* readBuf2 = HBufC8::NewLC( length );
    TPtr8 writable2( readBuf2->Des() );
    msg1->Read( 0, writable2 );
    ptr = iImpl->HandleDataL( *readBuf2, consumed, remoteAddr );
    EUNIT_ASSERT( NULL != ptr );
    EUNIT_ASSERT( EFalse == consumed );

    delete ptr;
    CleanupStack::PopAndDestroy( readBuf2 );
    CleanupStack::PopAndDestroy( msg1 );
    CleanupStack::PopAndDestroy( indication );
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_STUNClientL(  )
    {
    const CSTUNClient* client = iImpl->STUNClient();
    delete iStunclient;
    iStunclient = NULL;
    const CSTUNClient* client2 = iImpl->STUNClient();
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_GetServerAddressLL(  )
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE( iImpl->GetServerAddressL() );
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_GetSharedSecretLL(  )
    {
    TInetAddr addr( KProtocolInetUdp );
    _LIT( KServer, "10.32.194.251" );
    addr.Input( KServer );
    iImpl->iServerAddress = addr;    
    iImpl->GetSharedSecretL();
    }
    
void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_UsernameL()
    {
    TBufC8<16> password( KPassword );
    TBufC8<6> usernameA( KUsername );
    iImpl->CreateBindingRequestL( usernameA, password );    
    const TDesC8& name = iImpl->Username();
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_DetachClientL(  )
    {
    iImpl->DetachClient();
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_StoreAddressLL(  )
    {
    TInetAddr addr;
    iImpl->StoreAddressL( addr );
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_ChangeStateL(  )
    {
    CSTUNBindingGetAddress* getAddress = new ( ELeave ) CSTUNBindingGetAddress();
    iImpl->ChangeState( *getAddress );
    delete getAddress;
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_TerminateL(  )
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

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_IsWaitingSharedSecretL(  )
    {
    iImpl->IsWaitingSharedSecret();
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_SharedSecretObtainedLL(  )
    {
    TBufC8<16> password( KPassword );
    TBufC8<6> usernameA( KUsername );
    EUNIT_ASSERT_PANIC( iImpl->SharedSecretObtainedL( usernameA, password ), "", KErrGeneral );    
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_CreateBindingRequestLL(  )
    {
    TBufC8<16> password( KPassword );
    TBufC8<6> usernameA( KUsername );
    TICEAttributes attributes;
    attributes.iPriority = 36;
    attributes.iUseCandidate = ETrue;
    iImpl->SetICESpecificAttributes( attributes );
    iImpl->iAddXorOnly = ETrue;

    iImpl->CreateBindingRequestL( usernameA, password );
    TBufC8<1> username;
    iImpl->CreateBindingRequestL( username, password );
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_SendBindingRequestLL(  )
    {
    TBufC8<16> password( KPassword );
    TBufC8<6> usernameA( KUsername );
    iImpl->CreateBindingRequestL( usernameA, password );
        
    TInetAddr addr( KProtocolInetUdp );
    _LIT( KServer, "10.32.194.251" );
    addr.Input( KServer );
    iImpl->iServerAddress = addr; 
    iImpl->SendBindingRequestL();
    
    RSocket socket = iImpl->Socket();
    socket.Close();
    
    //iImpl->SendBindingRequestL();    
    
    delete iImpl->iRequest;
    iImpl->iRequest = NULL;
    delete iImpl->iTransaction;
    iImpl->iTransaction = NULL;        
    EUNIT_ASSERT_PANIC( iImpl->SendBindingRequestL(), "", KErrNotFound );
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_HandleTransactionErrorL(  )
    {    
    TBufC8<16> password( KPassword );
    TBufC8<6> usernameA( KUsername );
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
    iImpl->iRequest->AddAttributeL( CNATFWUNSAFRealmAttribute::NewLC( dataDesc ) );
    CleanupStack::Pop(); //CNATFWUNSAFRealmAttribute 
    iImpl->iRealmFromResponse = dataDesc2.AllocL();
    iImpl->iErrorResponseCount = 0; 

    EUNIT_ASSERT( iImpl->HandleTransactionError( 436 ) == ETrue );
    
    //nonce
    delete iImpl->iRequest;
    iImpl->iRequest = NULL;    
    iImpl->CreateBindingRequestL( usernameA, password );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iImpl->HandleTransactionError( 435 ) );
    iImpl->iErrorResponseCount = 0;
    
    delete iImpl->iRequest;
    iImpl->iRequest = NULL;    
    iImpl->CreateBindingRequestL( usernameA, password );
    EUNIT_ASSERT( iImpl->HandleTransactionError( 435 ) == ETrue );
    iImpl->iErrorResponseCount = 0;
    
    delete iImpl->iRequest;
    iImpl->iRequest = NULL;
    //iImpl->iError436Count = 3;
    iImpl->CreateBindingRequestL( usernameA, password );
    iImpl->iRequest->AddAttributeL( CNATFWUNSAFRealmAttribute::NewLC( dataDesc ) );
    CleanupStack::Pop(); //CNATFWUNSAFRealmAttribute
    delete iImpl->iRealmFromResponse;
    iImpl->iRealmFromResponse = NULL;    
    iImpl->iRealmFromResponse = dataDesc.AllocL();
        
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
    iImpl->iRequest->AddAttributeL( CNATFWUNSAFRealmAttribute::NewLC( dataDesc ) );
    CleanupStack::Pop(); //CNATFWUNSAFRealmAttribute
    
    delete iImpl->iSharedSecret;
    iImpl->iSharedSecret = NULL;      
    EUNIT_ASSERT( iImpl->HandleTransactionError( 401 ) == ETrue );
    
    
    delete iImpl->iRequest;
    iImpl->iRequest = NULL;   
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
    CleanupStack::Pop(); //CNATFWUNSAFAlternateServerAttribute 
    
    EUNIT_ASSERT( iImpl->HandleTransactionError( 1001 ) == ETrue );   
    EUNIT_ASSERT( iImpl->HandleTransactionError( 300 ) == ETrue );
    iImpl->iErrorResponseCount = 0;
    EUNIT_ASSERT( iImpl->HandleTransactionError( 400 ) == EFalse );
    EUNIT_ASSERT( iImpl->HandleTransactionError( 487 ) == EFalse );
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_HandleTransactionError2L(  )
    {
    TBufC8<16> password( KPassword );
    TBufC8<6> usernameA( KUsername );
    TInetAddr alternativeServer;
    
    delete iImpl->iRequest;
    iImpl->iRequest = NULL;    
    TNATFWUNSAFTransactionID transactionID;
    iImpl->iRequest = CNATFWUNSAFBindingRequest::NewL( transactionID );
    iImpl->iRequest->AddAttributeL( CNATFWUNSAFRealmAttribute::NewLC( dataDesc ) );
    CleanupStack::Pop(); //CNATFWUNSAFRealmAttribute 
    iImpl->iRealmFromResponse = dataDesc.AllocL();     
    EUNIT_ASSERT( iImpl->HandleTransactionError( 434 ) == ETrue );
    
    delete iImpl->iRealmFromResponse;
    iImpl->iRealmFromResponse = NULL;
    iImpl->iRealmFromResponse = dataDesc2.AllocL();
    iImpl->iErrorResponseCount = 0; 
    EUNIT_ASSERT( iImpl->HandleTransactionError( 434 ) == ETrue );

    delete iImpl->iRequest;
    iImpl->iRequest = NULL;   
    iImpl->CreateBindingRequestL( usernameA, password );
    iImpl->iRequest->AddAttributeL( CNATFWUNSAFAlternateServerAttribute::NewLC( alternativeServer ) );
    CleanupStack::Pop(); //CNATFWUNSAFAlternateServerAttribute 
    EUNIT_ASSERT( iImpl->HandleTransactionError( 401 ) == EFalse );
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_IcmpErrorL(  )
    {
    TInetAddr addr( KProtocolInetUdp );
    _LIT( KServer, "10.32.194.251" );
    addr.Input( KServer );
    
    iImpl->IcmpError( addr );
    
    TBufC8<16> password( KPassword );
    TBufC8<6> usernameA( KUsername );
    iImpl->CreateBindingRequestL( usernameA, password );

    iImpl->iServerAddress = addr;     
    iImpl->SendBindingRequestL();
    //iImpl->IcmpError( addr );  
    EUNIT_ASSERT_PANIC( iImpl->IcmpError( addr ), "", KErrGeneral );
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_WaitBeforeRetryingL(  )
    {
    iImpl->WaitBeforeRetrying();
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_SetICESpecificAttributesL(  )
    {
    TICEAttributes attributes;
    iImpl->SetICESpecificAttributes( attributes );
    }
/*
void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_ExecuteImmediateRetransmitLL()
    {
    EUNIT_ASSERT_NO_LEAVE( iImpl->ExecuteImmediateRetransmitL() );
    
    TBufC8<16> password( KPassword );
    TBufC8<6> usernameA( KUsername );
    iImpl->CreateBindingRequestL( usernameA, password );
        
    TInetAddr addr( KProtocolInetUdp );
    _LIT( KServer, "10.32.194.251" );
    addr.Input( KServer );
    iImpl->iServerAddress = addr; 
    iImpl->SendBindingRequestL();
    iImpl->ExecuteImmediateRetransmitL();
    }
*/    
void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_ValidateMsgTypeL()
    {
    _LIT8( KTransactionId,"aaaabbbb");
    TNATFWUNSAFTransactionID transactionID;
    transactionID.Copy( KTransactionId );
        
    CNATFWUNSAFBindingRequest* request = CNATFWUNSAFBindingRequest::NewLC( transactionID );
    EUNIT_ASSERT_EQUALS( iImpl->ValidateMsgType( request ), EFalse );
    CleanupStack::PopAndDestroy( request );    
    
    CNATFWUNSAFBindingResponse* response = CNATFWUNSAFBindingResponse::NewLC( transactionID );
    EUNIT_ASSERT_EQUALS( iImpl->ValidateMsgType( response ), ETrue );
    CleanupStack::PopAndDestroy( response ); 
    
    CNATFWUNSAFBindingErrorResponse* errorResp = CNATFWUNSAFBindingErrorResponse::NewLC( transactionID );
    EUNIT_ASSERT_EQUALS( iImpl->ValidateMsgType( errorResp ), ETrue );
    CleanupStack::PopAndDestroy( errorResp );
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_StreamIdL(  )
    {
    TUint streamId = iImpl->StreamId();
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_ConnectionIdL(  )
    {
    TUint connectionId = iImpl->ConnectionId();
    }

void ut_cstunbindingimplementation::UT_CSTUNBindingImplementation_RealmFromResponseL()
    {
    const HBufC8* response = iImpl->RealmFromResponse();
    delete response;
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    ut_cstunbindingimplementation,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CSTUNBindingImplementation",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_NewLL, Teardown)

EUNIT_TEST(
    "NewL - test ",
    "CSTUNBindingImplementation",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_NewL_1L, Teardown)

EUNIT_TEST(
    "TimerExpiredL - test ",
    "CSTUNBindingImplementation",
    "TimerExpiredL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_TimerExpiredLL, Teardown)

EUNIT_TEST(
    "LeaveFromTimerExpired - test ",
    "CSTUNBindingImplementation",
    "LeaveFromTimerExpired",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_LeaveFromTimerExpiredL, Teardown)

EUNIT_TEST(
    "PublicAddressObtainedL - test ",
    "CSTUNBindingImplementation",
    "PublicAddressObtainedL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_PublicAddressObtainedLL, Teardown)

EUNIT_TEST(
    "PublicAddressObtainedL - test ",
    "CSTUNBindingImplementation",
    "PublicAddressObtainedL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_PublicAddressObtainedL_1L, Teardown)

EUNIT_TEST(
    "TransactionError - test ",
    "CSTUNBindingImplementation",
    "TransactionError",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_TransactionErrorL, Teardown)

EUNIT_TEST(
    "TransactionEventOccurredL - test ",
    "CSTUNBindingImplementation",
    "TransactionEventOccurredL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_TransactionEventOccurredLL, Teardown)

EUNIT_TEST(
    "SendRequestL - test ",
    "CSTUNBindingImplementation",
    "SendRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_SendRequestLL, Teardown)

EUNIT_TEST(
    "SendRequestL - test ",
    "CSTUNBindingImplementation",
    "SendRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_SendRequestL_1L, Teardown)

EUNIT_TEST(
    "CancelRequest - test ",
    "CSTUNBindingImplementation",
    "CancelRequest",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_CancelRequestL, Teardown)

EUNIT_TEST(
    "Socket - test ",
    "CSTUNBindingImplementation",
    "Socket",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_SocketL, Teardown)

EUNIT_TEST(
    "AddressResolved - test ",
    "CSTUNBindingImplementation",
    "AddressResolved",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_AddressResolvedL, Teardown)

EUNIT_TEST(
    "PublicAddr - test ",
    "CSTUNBindingImplementation",
    "PublicAddr",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_PublicAddrL, Teardown)

EUNIT_TEST(
    "AlternateServerAddr - test ",
    "CSTUNBindingImplementation",
    "AlternateServerAddr",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_AlternateServerAddrL, Teardown)
    
EUNIT_TEST(
    "HandleDataL - test ",
    "CSTUNBindingImplementation",
    "HandleDataL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_HandleDataLL, Teardown)

EUNIT_TEST(
    "STUNClient - test ",
    "CSTUNBindingImplementation",
    "STUNClient",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_STUNClientL, Teardown)

EUNIT_TEST(
    "GetServerAddressL - test ",
    "CSTUNBindingImplementation",
    "GetServerAddressL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_GetServerAddressLL, Teardown)

EUNIT_TEST(
    "GetSharedSecretL - test ",
    "CSTUNBindingImplementation",
    "GetSharedSecretL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_GetSharedSecretLL, Teardown)

EUNIT_TEST(
    "Username - test ",
    "CSTUNBindingImplementation",
    "Username",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_UsernameL, Teardown)

EUNIT_TEST(
    "DetachClient - test ",
    "CSTUNBindingImplementation",
    "DetachClient",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_DetachClientL, Teardown)

EUNIT_TEST(
    "StoreAddressL - test ",
    "CSTUNBindingImplementation",
    "StoreAddressL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_StoreAddressLL, Teardown)

EUNIT_TEST(
    "ChangeState - test ",
    "CSTUNBindingImplementation",
    "ChangeState",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_ChangeStateL, Teardown)

EUNIT_TEST(
    "Terminate - test ",
    "CSTUNBindingImplementation",
    "Terminate",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_TerminateL, Teardown)

EUNIT_TEST(
    "IsWaitingSharedSecret - test ",
    "CSTUNBindingImplementation",
    "IsWaitingSharedSecret",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_IsWaitingSharedSecretL, Teardown)

EUNIT_TEST(
    "SharedSecretObtainedL - test ",
    "CSTUNBindingImplementation",
    "SharedSecretObtainedL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_SharedSecretObtainedLL, Teardown)

EUNIT_TEST(
    "CreateBindingRequestL - test ",
    "CSTUNBindingImplementation",
    "CreateBindingRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_CreateBindingRequestLL, Teardown)

EUNIT_TEST(
    "SendBindingRequestL - test ",
    "CSTUNBindingImplementation",
    "SendBindingRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_SendBindingRequestLL, Teardown)

EUNIT_TEST(
    "HandleTransactionError - test ",
    "CSTUNBindingImplementation",
    "HandleTransactionError",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_HandleTransactionErrorL, Teardown)

EUNIT_NOT_DECORATED_TEST(
    "HandleTransactionError - test 2 ",
    "CSTUNBindingImplementation",
    "HandleTransactionError",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_HandleTransactionError2L, Teardown)

EUNIT_TEST(
    "IcmpError - test ",
    "CSTUNBindingImplementation",
    "IcmpError",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_IcmpErrorL, Teardown)

EUNIT_TEST(
    "WaitBeforeRetrying - test ",
    "CSTUNBindingImplementation",
    "WaitBeforeRetrying",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_WaitBeforeRetryingL, Teardown)

EUNIT_TEST(
    "SetICESpecificAttributes - test ",
    "CSTUNBindingImplementation",
    "SetICESpecificAttributes",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_SetICESpecificAttributesL, Teardown)

/*EUNIT_TEST(
    "ExecuteImmediateRetransmitL - test ",
    "CSTUNBindingImplementation",
    "ExecuteImmediateRetransmitL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_ExecuteImmediateRetransmitLL, Teardown)*/

EUNIT_TEST(
    "ValidateMsgType - test ",
    "CSTUNBindingImplementation",
    "ValidateMsgType",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_ValidateMsgTypeL, Teardown)

EUNIT_TEST(
    "StreamId - test ",
    "CSTUNBindingImplementation",
    "StreamId",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_StreamIdL, Teardown)

EUNIT_TEST(
    "RealmFromResponse - test ",
    "CSTUNBindingImplementation",
    "RealmFromResponse",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBindingImplementation_RealmFromResponseL, Teardown)    

EUNIT_END_TEST_TABLE

//  END OF FILE
