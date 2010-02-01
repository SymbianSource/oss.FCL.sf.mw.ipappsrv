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


#include "natfwstunclient.h"

_LIT(KSTUNServerAddr, "9.8.7.6");


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSTUNClient::NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CSTUNClient* CSTUNClient::NewL(
    TInt aRetransmitInterval,
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
    MNcmConnectionMultiplexer* aMultiplexer,
    TTransportProtocol aTransportProtocol)
    {
    CSTUNClient* self = CSTUNClient::NewLC( aRetransmitInterval,
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
                                            aTransportProtocol );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CSTUNClient::NewLC
// ---------------------------------------------------------------------------
//
EXPORT_C CSTUNClient* CSTUNClient::NewLC(
    TInt aRetransmitInterval,
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
    MNcmConnectionMultiplexer* aMultiplexer,
    TTransportProtocol aTransportProtocol)
    {
    CSTUNClient* self = new ( ELeave ) CSTUNClient;
    CleanupStack::PushL( self );

    self->ConstructL(aRetransmitInterval,aServerAddress,aServerPort,
                     aNatProtocol,aSocketServ,aConnection,aTimer,aObserver,
                     aObtainSharedSecret,aFailIfNoSRVRecordsFound,
                     aIcmpReceiverUsed, aMultiplexer, aTransportProtocol);
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
// CSTUNClient::ConstructL
// ---------------------------------------------------------------------------
//
void CSTUNClient::ConstructL(TInt /*aRetransmitInterval*/,
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
                         TTransportProtocol /*aTransportProtocol*/)
    {
    TInetAddr* addr = new(ELeave)TInetAddr;
    User::LeaveIfError(addr->Input(KSTUNServerAddr));
    iImplementation = reinterpret_cast<CSTUNClientImplementation*>(addr);
    }

// ---------------------------------------------------------------------------
// CSTUNClient::~CSTUNClient
// ---------------------------------------------------------------------------
//
EXPORT_C CSTUNClient::~CSTUNClient()
    {
    TInetAddr* addr = reinterpret_cast<TInetAddr*>(iImplementation);
    delete addr;
    }



// ---------------------------------------------------------------------------
// CSTUNClient::IsInitialized
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CSTUNClient::IsInitialized() const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CSTUNClient::SharedSecretObtained
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSTUNClient::SharedSecretObtained() const
    {
    return ETrue;
    }

// ---------------------------------------------------------------------------
// CSTUNClient::STUNServerAddrL
// ---------------------------------------------------------------------------
//
EXPORT_C const TInetAddr& CSTUNClient::STUNServerAddrL() const
    {
    return *(reinterpret_cast<TInetAddr*>(iImplementation));
    }


// -----------------------------------------------------------------------------
// CSTUNClient::SetCredentialsL
// -----------------------------------------------------------------------------
//
EXPORT_C void CSTUNClient::SetCredentialsL(const TDesC8& /*aUsername*/,
    const TDesC8& /*aPassword*/)
    {
    }

