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




//#include "stunassert.h"
#include "natfwstunclient.h"
//#include "cstunclientimplementation.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSTUNClient::NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CSTUNClient* CSTUNClient::NewL( 
    TInt aRetransmitInterval,
    const TDesC8& aServerAddress,
    TUint aServerPort,
    const TDesC8& aServiceName,
    RSocketServ& aSocketServ,
    RConnection& aConnection,
    CDeltaTimer& aTimer,
    MSTUNClientObserver& aObserver,
    TBool aObtainSharedSecret,
    TBool aFailIfNoSRVRecordsFound,
    TBool aIcmpReceiverUsed,
    MNcmConnectionMultiplexer* aMultiplexer,
    TTransportProtocol aTransportProtocol )
    {
    CSTUNClient* self = CSTUNClient::NewLC( aRetransmitInterval,
                                            aServerAddress,
                                            aServerPort,
                                            aServiceName,
                                            aSocketServ,
                                            aConnection,
                                            aTimer,
                                            aObserver,
                                            aObtainSharedSecret,
                                            aFailIfNoSRVRecordsFound,
                                            aIcmpReceiverUsed ,
                                            aMultiplexer,
                                            aTransportProtocol );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CSTUNClient::NewL - overloaded for ICE connectivity checks
// ---------------------------------------------------------------------------
//
EXPORT_C CSTUNClient* CSTUNClient::NewL( 
    TInt aRetransmitInterval,
    CDeltaTimer& aTimer,
    MSTUNClientObserver& aObserver,
    MNcmConnectionMultiplexer* aMultiplexer,
    TTransportProtocol aTransportProtocol )
    {
    CSTUNClient* self = CSTUNClient::NewLC( aRetransmitInterval,
                                            aTimer,
                                            aObserver,
                                            aMultiplexer,
                                            aTransportProtocol );
    CleanupStack::Pop( self );
    return self;
    }
   
// ---------------------------------------------------------------------------
// CSTUNClient::NewLC
// ---------------------------------------------------------------------------
//
EXPORT_C CSTUNClient* CSTUNClient::NewLC(
    TInt /*aRetransmitInterval*/,
    const TDesC8& /*aServerAddress*/,
    TUint /*aServerPort*/,
    const TDesC8& /*aNatProtocol*/,
    RSocketServ& /*aSocketServ*/,
    RConnection& /*aConnection*/,
    CDeltaTimer& /*aTimer*/,
    MSTUNClientObserver& /*aObserver*/,
    TBool /*aObtainSharedSecret*/,
    TBool /*aFailIfNoSRVRecordsFound*/,
    TBool /*aIcmpReceiverUsed*/,
    MNcmConnectionMultiplexer* /*aMultiplexer*/,
    TTransportProtocol /*aTransportProtocol*/ )
    {    
    CSTUNClient* self = new ( ELeave ) CSTUNClient();
    CleanupStack::PushL( self );
/*    self->ConstructL( aRetransmitInterval,
                      aServerAddress,
                      aServerPort,
                      aNatProtocol,
                      aSocketServ,
                      aConnection,
                      aTimer,
                      aObserver,
                      aObtainSharedSecret,
                      aFailIfNoSRVRecordsFound,
                      aIcmpReceiverUsed,
                      aMultiplexer,
                      aTransportProtocol ); */
    return self;
    }

// ---------------------------------------------------------------------------
// CSTUNClient::NewLC - overloaded for ICE connectivity checks
// ---------------------------------------------------------------------------
//
EXPORT_C CSTUNClient* CSTUNClient::NewLC(
    TInt /*aRetransmitInterval*/,
    CDeltaTimer& /*aTimer*/,
    MSTUNClientObserver& /*aObserver*/,
    MNcmConnectionMultiplexer* /*aMultiplexer*/,
    TTransportProtocol /*aTransportProtocol*/ )
    {    
    CSTUNClient* self = new ( ELeave ) CSTUNClient();
    CleanupStack::PushL( self );
/*    self->ConstructL( aRetransmitInterval,
                      aTimer,
                      aObserver,
                      aMultiplexer );*/
    return self;
    }

// ---------------------------------------------------------------------------
// CSTUNClient::CSTUNClient
// ---------------------------------------------------------------------------
//
CSTUNClient::CSTUNClient()
    {    
    }

// ---------------------------------------------------------------------------
// CSTUNClient::CSTUNClient
// Dummy implementation, as copy constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNClient::CSTUNClient( const CSTUNClient& /*aClient*/ )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNClient::ConstructL
// ---------------------------------------------------------------------------
//
/*void CSTUNClient::ConstructL( TInt aRetransmitInterval,
                              const TDesC8& aServerAddress,
                              TUint aServerPort,
                              const TDesC8& aNatProtocol,
                              RSocketServ& aSocketServ,
                              RConnection& aConnection,
                              CDeltaTimer& aTimer,
                              MSTUNClientObserver& aObserver,
                              TBool aObtainSharedSecret,
                              TBool aFailIfNoSRVRecordsFound,
                              TBool aIcmpReceiverUsed,
                              MNcmConnectionMultiplexer* aMultiplexer )
    {    
    iImplementation = CSTUNClientImplementation::NewL( *this,
                                                   aRetransmitInterval,
                                                   aServerAddress,
                                                   aServerPort,
                                                   aNatProtocol,
                                                   aSocketServ,
                                                   aConnection,
                                                   aTimer,
                                                   aObserver,
                                                   aObtainSharedSecret,
                                                   aFailIfNoSRVRecordsFound,
                                                   aIcmpReceiverUsed,
                                                   aMultiplexer );
    }*/

// ---------------------------------------------------------------------------
// CSTUNClient::ConstructL - overloaded for ICE connectivity checks
// ---------------------------------------------------------------------------
//
/*void CSTUNClient::ConstructL( TInt aRetransmitInterval,
                              CDeltaTimer& aTimer,
                              MSTUNClientObserver& aObserver,
                              MNcmConnectionMultiplexer* aMultiplexer )
    {    
    iImplementation = CSTUNClientImplementation::NewL( *this,
                                                       aRetransmitInterval,
                                                       aTimer,
                                                       aObserver,
                                                       aMultiplexer );
    }*/

    
// ---------------------------------------------------------------------------
// CSTUNClient::~CSTUNClient
// ---------------------------------------------------------------------------
//
EXPORT_C CSTUNClient::~CSTUNClient()
    {    
//    delete iImplementation;    
    }

// ---------------------------------------------------------------------------
// CSTUNClient::IsInitialized
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CSTUNClient::IsInitialized() const
    {
//    __TEST_INVARIANT;

//    return iImplementation->IsInitialized();
    return ETrue;
    }

// ---------------------------------------------------------------------------
// CSTUNClient::STUNServerAddrL
// ---------------------------------------------------------------------------
//
EXPORT_C const TInetAddr& CSTUNClient::STUNServerAddrL() const
    {
//    __TEST_INVARIANT;
    return KInetAddrAny;
//    return iImplementation->STUNServerAddrL();
    }

// ---------------------------------------------------------------------------
// CSTUNClient::SetCredentialsL
// ---------------------------------------------------------------------------
//
EXPORT_C void CSTUNClient::SetCredentialsL( const TDesC8& /*aUsername*/,
                                            const TDesC8& /*aPasswd*/ )
    {
    //__TEST_INVARIANT;

//    iImplementation->SetCredentialsL( aUsername, aPasswd );
    }

// ---------------------------------------------------------------------------
// CSTUNClient::SharedSecretObtained
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CSTUNClient::SharedSecretObtained() const
    {
    __TEST_INVARIANT;

//    return iImplementation->SharedSecretObtained();
    return ETrue;
    }

// ---------------------------------------------------------------------------
// CSTUNClient::ObtainTransactionIDL
// ---------------------------------------------------------------------------
//
EXPORT_C void CSTUNClient::ObtainTransactionIDL( TDesC8& /*aTransactionID*/ )
    {
/*    TNATFWUNSAFTransactionID& transactionID =
        static_cast<TNATFWUNSAFTransactionID&>( aTransactionID );*/
//    iImplementation->ObtainTransactionIDL( transactionID );
    }

// ---------------------------------------------------------------------------
// CSTUNClient::Implementation
// ---------------------------------------------------------------------------
//
/*CSTUNClientImplementation& CSTUNClient::Implementation()
    {
    __TEST_INVARIANT;

    return *iImplementation;
    }*/

// -----------------------------------------------------------------------------
// CSTUNClient::__DbgTestInvariant
// -----------------------------------------------------------------------------
//
void CSTUNClient::__DbgTestInvariant() const
    {
    }
