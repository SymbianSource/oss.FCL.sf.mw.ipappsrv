/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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




#include "natfwunsaflog.h"
#include "cnatfwunsafudpsender.h"
#include "natfwunsafbindingrequest.h"
#include "natfwunsaferrorcodeattribute.h"
#include "natfwunsafunknownattributesattribute.h"
#include "natfwunsafmessageintegrityattribute.h"
#include "stunassert.h"
#include "stunutils.h"
#include "mstuntransactionobserver.h"
#include "cstuntransaction.h"
#include "ttransactioncleanup.h"
#include "stunturnclientlogs.h"
#include "tstuncallbackinfo.h"

// ======== MEMBER FUNCTIONS ========


// Divide factor from uSecs to mSecs



// ---------------------------------------------------------------------------
// CSTUNTransaction::NewL
// ---------------------------------------------------------------------------
//
CSTUNTransaction* CSTUNTransaction::NewL( CNATFWUNSAFMessage& aRequest,
                                          const TInetAddr& aAddress,
                                          const TDesC8& aSharedSecret,
                                          RSocket& aSocket,
                                          CDeltaTimer& aTimer,
                                          TInt aRetransmitInterval,
                                          MSTUNTransactionObserver& aObserver,
                                          const TDesC8& aProtocol,
                                          TTransportProtocol aTransportProtocol )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNTransaction::NewL - overloaded
// ---------------------------------------------------------------------------
//
CSTUNTransaction* CSTUNTransaction::NewL( CNATFWUNSAFMessage& aRequest,
                                          const TInetAddr& aAddress,
                                          const TDesC8& aSharedSecret,
                                          TUint aStreamId,
                                          TUint aConnectionId,
                                          CDeltaTimer& aTimer,
                                          TInt aRetransmitInterval,
                                          MSTUNTransactionObserver& aObserver,
                                          const TDesC8& aProtocol,
                                          MNcmConnectionMultiplexer& aMux,
                                          TTransportProtocol aTransportProtocol,
                                          TBool aUseFingerprint,
                                          const TInetAddr& aDestAddr )
    {

    }

// ---------------------------------------------------------------------------
// CSTUNTransaction::CSTUNTransaction
// ---------------------------------------------------------------------------
//
CSTUNTransaction::CSTUNTransaction( CNATFWUNSAFMessage& aRequest,
                                    const TInetAddr& aAddress,
                                    const TDesC8& aSharedSecret,
                                    CDeltaTimer& aTimer,
                                    TInt aRetransmitInterval,
                                    MSTUNTransactionObserver& aObserver,
                                    TTransportProtocol aTransportProtocol ) :
    CSTUNTimerUser( aTimer ),
    iObserver( aObserver ),
    iRequest( aRequest ),
    iAddress( aAddress ),
    iSharedSecret( aSharedSecret ),
    iInitialRetransmitInterval( aRetransmitInterval ),
    iRetransmit( ETrue ),
    iTransportProtocol( aTransportProtocol ),
    iRetransmitCompareValue( 0 )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNTransaction::CSTUNTransaction - overloaded
// ---------------------------------------------------------------------------
//
CSTUNTransaction::CSTUNTransaction( CNATFWUNSAFMessage& aRequest,
                                    const TInetAddr& aAddress,
                                    const TDesC8& aSharedSecret,
                                    TUint aStreamId,
                                    TUint aConnectionId,
                                    CDeltaTimer& aTimer,
                                    TInt aRetransmitInterval,
                                    MSTUNTransactionObserver& aObserver,
                                    const TInetAddr& aDestAddr,
                                    TTransportProtocol aTransportProtocol,
                                    TBool aUseFingerprint ) :
    CSTUNTimerUser( aTimer ),
    iObserver( aObserver ),
    iRequest( aRequest ),
    iAddress( aAddress ),
    iSharedSecret( aSharedSecret ),
    iInitialRetransmitInterval( aRetransmitInterval ),
    iRetransmit( ETrue ),
    iStreamId( aStreamId ),
    iSubstreamId( aConnectionId ),
    iDestAddr( aDestAddr ),
    iTransportProtocol( aTransportProtocol ),
    iRetransmitCompareValue( 0 ),
    iUseFingerprint( aUseFingerprint )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNTransaction::CSTUNTransaction
// Dummy implementation. Default constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNTransaction::CSTUNTransaction() :
    CSTUNTimerUser( *( CDeltaTimer* )0x1 ),
    iObserver( *( MSTUNTransactionObserver* )0x1 ),
    iRequest( *( CNATFWUNSAFMessage* )0x1 ),
    iAddress( *( TInetAddr* )0x1 ),
    iSharedSecret( KNullDesC8 )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNTransaction::CSTUNTransaction
// Dummy implementation, as copy constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNTransaction::CSTUNTransaction( const CSTUNTransaction& aTransaction ) :
    CSTUNTimerUser( *( CDeltaTimer* )0x1 ),
    iObserver( aTransaction.iObserver ),
    iRequest( aTransaction.iRequest ),
    iAddress( aTransaction.iAddress ),
    iSharedSecret( aTransaction.iSharedSecret )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNTransaction::ConstructL
// ---------------------------------------------------------------------------
//
void CSTUNTransaction::ConstructL( RSocket& aSocket, const TDesC8& aProtocol )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNTransaction::ConstructL
// ---------------------------------------------------------------------------
//
void CSTUNTransaction::ConstructL( const TDesC8& aProtocol, 
    MNcmConnectionMultiplexer& aMux  )
    {
    }
      
// ---------------------------------------------------------------------------
// CSTUNTransaction::~CSTUNTransaction
// ---------------------------------------------------------------------------
//
CSTUNTransaction::~CSTUNTransaction()
    {
    }

// ---------------------------------------------------------------------------
// CSTUNTransaction::NATFWUNSAFUdpMessageSentL
// ---------------------------------------------------------------------------
//
void CSTUNTransaction::UNSAFUdpMessageSentL()
    {
    }

// ---------------------------------------------------------------------------
// CSTUNTransaction::NATFWUNSAFUdpMessageFailure
// ---------------------------------------------------------------------------
//
void CSTUNTransaction::UNSAFUdpMessageFailure( TInt aError )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNTransaction::MessageSent
// ---------------------------------------------------------------------------
//
void CSTUNTransaction::MessageSent()
    {
    
    }

// ---------------------------------------------------------------------------
// CSTUNTransaction::MessageSentFailure
// ---------------------------------------------------------------------------
// 
void CSTUNTransaction::MessageSentFailure( TInt aError )
    {
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::SendRequestL
// -----------------------------------------------------------------------------
//
void CSTUNTransaction::SendRequestL()
    {
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::RetransmitRequestL
// -----------------------------------------------------------------------------
//
void CSTUNTransaction::RetransmitRequestL()
    {
    }

// ---------------------------------------------------------------------------
// CSTUNTransaction::ReceiveL
// Also an ignored response stops retransmission.
// ---------------------------------------------------------------------------
//
void CSTUNTransaction::ReceiveL( CNATFWUNSAFMessage& aResponse,
                                 const TDesC8& aByteStream )
    {
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::ShouldIgnoreResponse
// 1xx-3xx response stops retransmissions, but doesn't terminate transaction.
// -----------------------------------------------------------------------------
//
TBool
CSTUNTransaction::ShouldIgnoreResponse( const CNATFWUNSAFMessage& aResponse ) const
    {
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::ProcessResponseL
// -----------------------------------------------------------------------------
//
TInt CSTUNTransaction::ProcessResponseL( CNATFWUNSAFMessage& aResponse,
                                         const TDesC8& aByteStream )
    {
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::CheckIntegrityL
// MESSAGE-INTEGRITY is in the encoded message, not in iRequest. If iRequest has
// a USERNAME, then also a MESSAGE-INTEGRITY was sent in the Binding Request.
// -----------------------------------------------------------------------------
//
TInt CSTUNTransaction::CheckIntegrityL( const CNATFWUNSAFMessage& aResponse,
                                         const TDesC8& aByteStream )
    {
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::CheckForNatOverwriteL
// Be compatible with old STUN servers that don't understand XOR-MAPPED-ADDRESS
// or XOR-ONLY. Don't treat XOR-MAPPED-ADDRESS as mandatory in Binding Response.
// -----------------------------------------------------------------------------
//
TBool CSTUNTransaction::CheckForNatOverwriteL( const CNATFWUNSAFMessage& aResponse )
    {
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::GetResponseCode
// -----------------------------------------------------------------------------
//
TInt CSTUNTransaction::
    GetResponseCode( const CNATFWUNSAFMessage& aResponse ) const
    {
    }

// ---------------------------------------------------------------------------
// CSTUNTransaction::Terminate
// Don't use invariant at the end of Terminate nor elsewhere after calling it,
// as binding deletes this transaction.
// If PublicAddressObtainedL leaves, call TransactionError.
// ---------------------------------------------------------------------------
//
void CSTUNTransaction::Terminate( TInt aError )
    {
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::RetransmitInterval
// -----------------------------------------------------------------------------
//
TInt CSTUNTransaction::RetransmitInterval()
    {
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::ComputeWaitDuration
// -----------------------------------------------------------------------------
//
TInt CSTUNTransaction::ComputeWaitDuration()
    {
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::TimerExpiredL
// -----------------------------------------------------------------------------
//
void CSTUNTransaction::TimerExpiredL()
    {
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::LeaveFromTimerExpired
// -----------------------------------------------------------------------------
//
void CSTUNTransaction::LeaveFromTimerExpired( TInt aError )
    {
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::TimerValue
// -----------------------------------------------------------------------------
//
const TUint32& CSTUNTransaction::TimerValue() const
    {
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::CancelRetransmission
// -----------------------------------------------------------------------------
//
void CSTUNTransaction::CancelRetransmission()
    {
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::CheckProtocolL
// -----------------------------------------------------------------------------
//
void CSTUNTransaction::CheckProtocolL( const TDesC8& aProtocol )
    {
    }
    
// -----------------------------------------------------------------------------
// CSTUNTransaction::StunRelayUsed
// -----------------------------------------------------------------------------
//
TBool CSTUNTransaction::StunRelayUsed()
    {
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::MeasureNewRTTSample
// -----------------------------------------------------------------------------
//
void CSTUNTransaction::MeasureNewRTTSample()
    {
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::__DbgTestInvariant
// -----------------------------------------------------------------------------
// 
void CSTUNTransaction::__DbgTestInvariant() const
    {
    }
