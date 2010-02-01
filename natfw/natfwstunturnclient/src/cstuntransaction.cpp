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

const TInt KInitCompareValue = 1;

// Divide factor from uSecs to mSecs
const TInt KDivideFactor = 1000;

const TInt KWaitingTime = 1600; // rfc3489bis-06#section-7.1

const TInt KMaxTCPWaitingTime = 7900; // rfc3489bis-06#section-7.1

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
    __ASSERT_ALWAYS( !aAddress.IsUnspecified(), User::Leave( KErrArgument ) );

    CSTUNTransaction* self =
        new ( ELeave ) CSTUNTransaction( aRequest,
                                         aAddress,
                                         aSharedSecret,
                                         aTimer,
                                         aRetransmitInterval,
                                         aObserver,
                                         aTransportProtocol );
    CleanupStack::PushL( self );
    self->ConstructL( aSocket, aProtocol );
    CleanupStack::Pop( self );
    return self;
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
    __ASSERT_ALWAYS( !aAddress.IsUnspecified(), User::Leave( KErrArgument ) );

    CSTUNTransaction* self =
        new ( ELeave ) CSTUNTransaction( aRequest,
                                         aAddress,
                                         aSharedSecret,
                                         aStreamId,
                                         aConnectionId,
                                         aTimer,
                                         aRetransmitInterval,
                                         aObserver,
                                         aDestAddr,
                                         aTransportProtocol,
                                         aUseFingerprint );
    CleanupStack::PushL( self );
    self->ConstructL( aProtocol, aMux );
    CleanupStack::Pop( self );
    return self;
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
    iRetransmitCompareValue( KInitCompareValue )
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
    iRetransmitCompareValue( KInitCompareValue ),
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
    __STUNTURNCLIENT( "CSTUNTransaction::ConstructL with sender" )
    CheckProtocolL( aProtocol );
    iSender = CNATFWUNSAFUdpSender::NewL( aSocket, *this );
    SendRequestL();
    }

// ---------------------------------------------------------------------------
// CSTUNTransaction::ConstructL
// ---------------------------------------------------------------------------
//
void CSTUNTransaction::ConstructL( const TDesC8& aProtocol, 
    MNcmConnectionMultiplexer& aMux  )
    {
    __STUNTURNCLIENT( "CSTUNTransaction::ConstructL with mediaconsender" )
    CheckProtocolL( aProtocol );
    iMediaConSender = CNATFWUNSAFMediaConnSender::NewL( aMux );
    SendRequestL();
    }
      
// ---------------------------------------------------------------------------
// CSTUNTransaction::~CSTUNTransaction
// ---------------------------------------------------------------------------
//
CSTUNTransaction::~CSTUNTransaction()
    {
    __STUNTURNCLIENT( "CSTUNTransaction::~CSTUNTransaction IN" )
    
    if ( iMediaConSender )
        {
        iMediaConSender->Cancel( iStreamId, iSubstreamId, this );
        }
    
    delete iMediaConSender;
    delete iSender;
    delete iUnknownAttr;
    delete iProtocol;
    
    __STUNTURNCLIENT( "CSTUNTransaction::~CSTUNTransaction OUT" )
    }

// ---------------------------------------------------------------------------
// CSTUNTransaction::NATFWUNSAFUdpMessageSentL
// ---------------------------------------------------------------------------
//
void CSTUNTransaction::UNSAFUdpMessageSentL()
    {
    __STUNTURNCLIENT( "CSTUNTransaction::UNSAFUdpMessageSentL" )
    __TEST_INVARIANT;
    
    if ( !iTerminated )
        {
        ++iSendCount;
        
        if ( iRetransmit )
            {
            if ( ETcpProtocol == iTransportProtocol )
                {
                StartTimer( KMaxTCPWaitingTime );
                }
            else
                {                       
                iSendTime.HomeTime();
                StartTimer( RetransmitInterval() );
                }
            }
        }

    __TEST_INVARIANT;
    }

// ---------------------------------------------------------------------------
// CSTUNTransaction::NATFWUNSAFUdpMessageFailure
// ---------------------------------------------------------------------------
//
void CSTUNTransaction::UNSAFUdpMessageFailure( TInt aError )
    {
    __STUNTURNCLIENT_INT1( "\
    CSTUNTransaction::UNSAFUdpMessageFailure, error=", aError )
    __TEST_INVARIANT;
    __STUN_ASSERT_RETURN( aError != KErrNone, KErrArgument );

    if ( !iTerminated )
        {
        Terminate( aError );
        }
    }

// ---------------------------------------------------------------------------
// CSTUNTransaction::MessageSent
// ---------------------------------------------------------------------------
//
void CSTUNTransaction::MessageSent()
    {
    iRequestPending = EFalse;
    
    TRAP_IGNORE( UNSAFUdpMessageSentL() );
    }

// ---------------------------------------------------------------------------
// CSTUNTransaction::MessageSentFailure
// ---------------------------------------------------------------------------
// 
void CSTUNTransaction::MessageSentFailure( TInt aError )
    {
    __STUNTURNCLIENT_INT1( "\
    CSTUNTransaction::MessageSentFailure, error=", aError )
    iRequestPending = EFalse;
    UNSAFUdpMessageFailure( aError );
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::SendRequestL
// -----------------------------------------------------------------------------
//
void CSTUNTransaction::SendRequestL()
    {
    __STUNTURNCLIENT( "CSTUNTransaction::SendRequestL" )
    if ( iMediaConSender )
        {
        __STUNTURNCLIENT( "CSTUNTransaction::SendRequestL with mediaconsender" )
        if ( iDestAddr.IsUnspecified() )
            {
            iMediaConSender->SendL( iStreamId, 
                iSubstreamId, iRequest, iSharedSecret, this, iUseFingerprint );
            iRequestPending = ETrue;
            }
        else
            {
            iMediaConSender->SendL( iStreamId, 
                                    iSubstreamId, iRequest, iSharedSecret, 
                                    iDestAddr, this, iUseFingerprint );
            
            iRequestPending = ETrue;
            }
        }
    else if ( iSender )
        {
        __STUNTURNCLIENT( "CSTUNTransaction::SendRequestL with original sender" )
        iSender->SendL( iAddress, iRequest, iSharedSecret );
        }
    else
        {
        User::Leave( KErrNotReady );
        }
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::RetransmitRequestL
// -----------------------------------------------------------------------------
//
void CSTUNTransaction::RetransmitRequestL()
    {
    __STUNTURNCLIENT_INT1( "\
    CSTUNTransaction::RetransmitRequestL, sendcount= ",iSendCount )
    if ( iSendCount < KMaxRequestSentCount )
        {
        if ( !iDisableSending )
            {
            SendRequestL();
            }
        else
            {
            iSendTime.HomeTime();
            StartTimer( RetransmitInterval() );
            }
        }
    else
        {
        Terminate( KErrTimedOut );
        }
    }

// ---------------------------------------------------------------------------
// CSTUNTransaction::ReceiveL
// Also an ignored response stops retransmission.
// ---------------------------------------------------------------------------
//
void CSTUNTransaction::ReceiveL( CNATFWUNSAFMessage& aResponse,
                                 const TDesC8& aByteStream )
    {
    __TEST_INVARIANT;
    __STUN_ASSERT_L( 
        aResponse.Type() == CNATFWUNSAFMessage::EBindingResponse ||
        aResponse.Type() == CNATFWUNSAFMessage::EBindingErrorResponse ||
        aResponse.Type() == CNATFWUNSAFMessage::EAllocateResponse ||
        aResponse.Type() == CNATFWUNSAFMessage::EAllocateErrorResponse ||
        aResponse.Type() == CNATFWUNSAFMessage::EConnectResponse ||
        aResponse.Type() == CNATFWUNSAFMessage::EConnectErrorResponse ||
        aResponse.Type() ==
            CNATFWUNSAFMessage::ESetActiveDestinationResponse ||
        aResponse.Type() ==
            CNATFWUNSAFMessage::ESetActiveDestinationErrorResponse,
        KErrArgument );
    
    if ( !iTerminated )
        {
        iRetransmit = EFalse;
        StopTimer();
        
        if ( ShouldIgnoreResponse( aResponse ) )
            {
            StartTimer( ComputeWaitDuration() );
            }
        else
            {
            if ( EUdpProtocol == iTransportProtocol )
                {
                MeasureNewRTTSample();
                }
            
            TTransactionCleanup taCleanup( *this );
            CleanupStack::PushL( taCleanup.CleanupItem() );

            TInt status = ProcessResponseL( aResponse, aByteStream );

            CleanupStack::Pop(); //cleanupItem
            Terminate( status );
            }
        }
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::ShouldIgnoreResponse
// 1xx-3xx response stops retransmissions, but doesn't terminate transaction.
// -----------------------------------------------------------------------------
//
TBool
CSTUNTransaction::ShouldIgnoreResponse( const CNATFWUNSAFMessage& aResponse ) const
    {
    __TEST_INVARIANT;

    TInt responseCode = GetResponseCode( aResponse );
    return !aResponse.Validate() ||
           aResponse.HasUnknownMandatoryAttributes() ||
           ( responseCode >= E1XX && responseCode < E4XX );
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::ProcessResponseL
// -----------------------------------------------------------------------------
//
TInt CSTUNTransaction::ProcessResponseL( CNATFWUNSAFMessage& aResponse,
                                         const TDesC8& aByteStream )
    {
    __STUNTURNCLIENT( "CSTUNTransaction::ProcessResponseL" )
    __TEST_INVARIANT;
                     
    TInt status = KErrNone;
    
    // binding response ( stun functionality )
    if ( aResponse.Type() == CNATFWUNSAFMessage::EBindingResponse )
        {
        status = CheckIntegrityL( aResponse, aByteStream );
        if ( KErrNone == status )
            {
            STUNUtils::GetMappedAddress( aResponse, iMappedAddress );
            STUNUtils::GetXorMappedAddressL( aResponse, iXorMappedAddress );
            }
        }
    
    // allocate response ( stun relay functionality )
    else if ( aResponse.Type() == CNATFWUNSAFMessage::EAllocateResponse )
        {
        status = CheckIntegrityL( aResponse, aByteStream );
        if ( KErrNone == status )
            {
            // get server relay address
            STUNUtils::GetRelayAddressL( aResponse, iRelayAddress );
            // get server reflexive address
            STUNUtils::GetXorMappedAddressL( aResponse, iXorMappedAddress );
            }
        }
    
    // Connect Response ( stun relay TCP functionality )
    else if ( aResponse.Type() == 
        CNATFWUNSAFMessage::EConnectResponse )
        {
        status = CheckIntegrityL( aResponse, aByteStream );
        }
    
    // Active Destination Response ( stun relay functionality )
    else if ( aResponse.Type() == 
        CNATFWUNSAFMessage::ESetActiveDestinationResponse )
        {
        __STUNTURNCLIENT( "do msg integrity check for active destination response" )
        status = CheckIntegrityL( aResponse, aByteStream );
        if ( KErrNone != status )
            {
            __STUNTURNCLIENT_INT1( "msg integrity check for active\
            destination response FAILED - BUT IGNORE IT - SERVER MISBEHAVIOR DETECTED", status )
            status = KErrNone; 
            }
            
        if ( KErrNone == status )
            {
            // notify stun relay implementation
            STUNUtils::GetTimerValueL( aResponse, iTimerValue );
            }
        }
        
    else
        {
        status = GetResponseCode( aResponse );
        if ( E4XX > status )
            {
            //Invalid Binding Error Response received
            status = KErrCorrupt;
            }            
        if ( status == E420UnknownAttributes )
            {
            __STUN_ASSERT_L( !iUnknownAttr, KErrAlreadyExists );
            iUnknownAttr = static_cast<CNATFWUNSAFUnknownAttributesAttribute*>(
                aResponse.Attribute(
                    CNATFWUNSAFAttribute::EUnknownAttributes ) );
            aResponse.DetachAttribute( iUnknownAttr );
            }
        }

    __TEST_INVARIANT;
    __STUNTURNCLIENT_INT1( "CSTUNTransaction::ProcessResponseL end, errorcode/status:", status )
    return status;
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
    __TEST_INVARIANT;
    __STUN_ASSERT_L( aResponse.Type() == CNATFWUNSAFMessage::EBindingResponse ||
                     aResponse.Type() == CNATFWUNSAFMessage::EAllocateResponse ||
                     aResponse.Type() == CNATFWUNSAFMessage::EConnectResponse ||
                     aResponse.Type() == 
                        CNATFWUNSAFMessage::ESetActiveDestinationResponse,
                     KErrArgument );

    if ( !iRequest.Attribute( CNATFWUNSAFAttribute::EUsername ) )
        {
        return KErrNone;
        }

    if ( aResponse.HasAttribute( CNATFWUNSAFAttribute::EMessageIntegrity ) )
        {
        if ( static_cast<CNATFWUNSAFMessageIntegrityAttribute*>(
                aResponse.Attribute( CNATFWUNSAFAttribute::EMessageIntegrity ) )->
                    CheckMessageIntegrityL( aByteStream, iSharedSecret ) )
            {
            return KErrNone;
            }

        //If XOR-ONLY was already sent, maybe server doesn't support it.
        if ( CheckForNatOverwriteL( aResponse ) &&
             !iRequest.Attribute( CNATFWUNSAFAttribute::EXorOnly ) )
            {
            return ERetryAfterAddingXorOnly;
            }
        }

    return KErrCorrupt;
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::CheckForNatOverwriteL
// Be compatible with old STUN servers that don't understand XOR-MAPPED-ADDRESS
// or XOR-ONLY. Don't treat XOR-MAPPED-ADDRESS as mandatory in Binding Response.
// -----------------------------------------------------------------------------
//
TBool CSTUNTransaction::CheckForNatOverwriteL( const CNATFWUNSAFMessage& aResponse )
    {
    __TEST_INVARIANT;    
    __STUN_ASSERT_L( aResponse.Type() == CNATFWUNSAFMessage::EBindingResponse ||
                     aResponse.Type() == CNATFWUNSAFMessage::EAllocateResponse,
                     KErrArgument );
    TInetAddr mappedAddr;
    TInetAddr xorMappedAddr;
    return STUNUtils::GetMappedAddress( aResponse, mappedAddr ) &&
           STUNUtils::GetXorMappedAddressL( aResponse, xorMappedAddr ) &&
           !mappedAddr.CmpAddr( xorMappedAddr );
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::GetResponseCode
// -----------------------------------------------------------------------------
//
TInt CSTUNTransaction::
    GetResponseCode( const CNATFWUNSAFMessage& aResponse ) const
    {
    __TEST_INVARIANT;
    if ( aResponse.Type() == CNATFWUNSAFMessage::EBindingErrorResponse ||
         aResponse.Type() == CNATFWUNSAFMessage::EAllocateErrorResponse ||
         aResponse.Type() == CNATFWUNSAFMessage::EConnectErrorResponse ||
         aResponse.Type() == 
                CNATFWUNSAFMessage::ESetActiveDestinationErrorResponse )
        {
        CNATFWUNSAFErrorCodeAttribute* errorCode =
            static_cast<CNATFWUNSAFErrorCodeAttribute*>
            ( aResponse.Attribute( CNATFWUNSAFAttribute::EErrorCode ) );
        
        if ( errorCode )
            {
            return errorCode->ResponseCode();
            }
        }
    return KErrNotFound;
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
    __STUNTURNCLIENT_INT1( "CSTUNTransaction::Terminate, error = ", aError )
    __TEST_INVARIANT;
    
    //Terminate only once
    __STUN_ASSERT_RETURN( !iTerminated, KErrDied );
    NATFWUNSAF_INTLOG( "STUNTransaction terminated, reason:", aError )

    iTerminated = ETrue;

    if ( aError == KErrNone )
        {
        TInetAddr publicAddr = iXorMappedAddress;
        if ( publicAddr.IsUnspecified() )
            {
            publicAddr = iMappedAddress;
            }
        
        if ( publicAddr.IsUnspecified() )
            {
            if ( CNATFWUNSAFMessage::ESetActiveDestinationRequest ==
                        iRequest.Type() )
                {
                TRAP( aError, iObserver.TransactionEventOccurredL( 
                    TSTUNCallbackInfo::EEventActiveDestinationSet ) );
                if ( aError )
                    {
                    __STUNTURNCLIENT_INT1(
                        "TransactionEventOccurredL leaves, reason:", aError )
                    }
                }
            else if ( CNATFWUNSAFMessage::EConnectRequest == iRequest.Type() )
                {
                TRAP( aError, iObserver.TransactionEventOccurredL( 
                    TSTUNCallbackInfo::EEventTCPConnectOk ) );
                if ( aError )
                    {
                    __STUNTURNCLIENT_INT1(
                        "TransactionEventOccurredL leaves, reason:", aError )
                    }
                }
            else
                {
                // Server sent invalid Binding Response
                aError = KErrCorrupt;
                }
            }
        else
            {
            if ( !StunRelayUsed() || iRelayAddress.IsUnspecified() )
                {
                __STUNTURNCLIENT( "\
                CSTUNTransaction::Terminate StunRelay NOT Used." )
                TRAP( aError, iObserver.PublicAddressObtainedL( publicAddr ) );
                }
            else
                {
                __STUNTURNCLIENT( "\
                CSTUNTransaction::Terminate StunRelay Used." )
                TRAP( aError, iObserver.PublicAddressObtainedL( 
                    publicAddr, iRelayAddress ) );
                }
            }
        }
        
    if ( aError )
        {
        CNATFWUNSAFUnknownAttributesAttribute* unknownAttr = iUnknownAttr;
        iUnknownAttr = NULL;
        iObserver.TransactionError( aError, unknownAttr );
        }
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::RetransmitInterval
// -----------------------------------------------------------------------------
//
TInt CSTUNTransaction::RetransmitInterval()
    {
    __STUNTURNCLIENT( "CSTUNTransaction::RetransmitInterval" )
    __TEST_INVARIANT;
    
    if ( 0 == iRetransmitInterval )
        {
        iRetransmitInterval = iInitialRetransmitInterval;
        }
    else if ( KMaxRequestSentCount == iSendCount )
        {
        iRetransmitInterval = KWaitingTime;
        }
    else
        {
        iRetransmitInterval = iRetransmitInterval << 1;
        }

    __TEST_INVARIANT;
    return iRetransmitInterval;
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::ComputeWaitDuration
// -----------------------------------------------------------------------------
//
TInt CSTUNTransaction::ComputeWaitDuration()
    {
    __STUNTURNCLIENT( "CSTUNTransaction::ComputeWaitDuration" )
    __TEST_INVARIANT;

    TInt duration( 0 );
    for ( TInt i = iSendCount; i < KMaxRequestSentCount; ++i )
        {
        duration += RetransmitInterval();
        }

    __TEST_INVARIANT;
    return duration;
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::TimerExpiredL
// -----------------------------------------------------------------------------
//
void CSTUNTransaction::TimerExpiredL()
    {
    __STUNTURNCLIENT( "CSTUNTransaction::TimerExpiredL" )
    __TEST_INVARIANT;

    if ( iRetransmit && EUdpProtocol == iTransportProtocol )
        {
        RetransmitRequestL();
        }
    else
        {
        Terminate( KErrTimedOut );
        }
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::LeaveFromTimerExpired
// -----------------------------------------------------------------------------
//
void CSTUNTransaction::LeaveFromTimerExpired( TInt aError )
    {
    __STUNTURNCLIENT_INT1( "\
    CSTUNTransaction::LeaveFromTimerExpired, error = ", aError )
    __TEST_INVARIANT;
    __STUN_ASSERT_RETURN( aError != KErrNone, KErrArgument );

    Terminate( aError );
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::TimerValue
// -----------------------------------------------------------------------------
//
const TUint32& CSTUNTransaction::TimerValue() const
    {
    return iTimerValue;
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::CancelRetransmission
// -----------------------------------------------------------------------------
//
void CSTUNTransaction::CancelRetransmission()
    {
    iDisableSending = ETrue;
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::CheckProtocolL
// -----------------------------------------------------------------------------
//
void CSTUNTransaction::CheckProtocolL( const TDesC8& aProtocol )
    {
    __STUNTURNCLIENT( "CSTUNTransaction::CheckProtocolL" )
    delete iProtocol;
    iProtocol = NULL;
    
    if ( aProtocol.Compare( KStun ) )
        {
        iProtocol = aProtocol.AllocL();
        }
    else if ( aProtocol.Compare( KStunRelay ) )
        {
        iProtocol = aProtocol.AllocL();
        }
    else
        {
        User::Leave( KErrArgument );
        }
    }
    
// -----------------------------------------------------------------------------
// CSTUNTransaction::StunRelayUsed
// -----------------------------------------------------------------------------
//
TBool CSTUNTransaction::StunRelayUsed()
    {
    __STUNTURNCLIENT( "CSTUNTransaction::StunRelayUsed()" )
    return !iProtocol->Compare( KStunRelay );
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::MeasureNewRTTSample
// -----------------------------------------------------------------------------
//
void CSTUNTransaction::MeasureNewRTTSample()
    {
    TTime curTime;
    curTime.HomeTime();
    
    //Measured RTT ( Round-trip time )
    TTimeIntervalMicroSeconds time = curTime.MicroSecondsFrom( iSendTime );
    // change microsecs to millisecs
    TInt64 rttSample = time.Int64() / TInt64( KDivideFactor );
    
    // rfc3489bis-06#section-7.1
    // As with TCP, the usage of Karn's
    // algorithm is RECOMMENDED.  When applied to STUN, it means that RTT
    // estimates SHOULD NOT be computed from STUN transactions which result
    // in the retransmission of a request.

    TBool retransmitted( EFalse );
    
    if ( iRetransmitCompareValue < iSendCount )
        {
        retransmitted = ETrue;
        rttSample = iRetransmitInterval;
        }
        
    iObserver.NewRTTSampleMeasured( rttSample, retransmitted );
    }

// -----------------------------------------------------------------------------
// CSTUNTransaction::__DbgTestInvariant
// -----------------------------------------------------------------------------
// 
void CSTUNTransaction::__DbgTestInvariant() const
    {
#if defined( _DEBUG )
    if ( /*!iSender ||*/
         iAddress.IsUnspecified() ||
         iSendCount < 0 ||
         iSendCount > KMaxRequestSentCount ||         
         iRetransmitInterval < 0 )
        {
        __STUNTURNCLIENT( "CSTUNTransaction::__DbgTestInvariant()\
        stage#1 panic" )
        User::Invariant();
        }
#endif
    }
