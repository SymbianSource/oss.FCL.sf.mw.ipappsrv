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
* Description:    Connection multiplexer connection abstraction.
*
*/



#include <commdbconnpref.h>

#include "cncmconnection.h"
#include "cncmtcpsender.h"
#include "cncmudpsender.h"
#include "cncmtcpreceiver.h"
#include "cncmudpreceiver.h"
#include "cncmconnectionobserverhandler.h"
#include "ncmconnectionmultiplexerlogs.h"
#include "mncmmultiplexerconnectionobserver.h"
#include "ncmconnectionmultiplexerassert.h"


// ---------------------------------------------------------------------------
// CNcmConnection::CNcmConnection
// ---------------------------------------------------------------------------
//
CNcmConnection::CNcmConnection( TUint aSessionId, TUint aStreamId,
    TUint aConnectionId, const TInetAddr& aLocalAddr,
    MNcmMultiplexerConnectionObserver& aObserver,
    CNcmConnectionObserverHandler& aConnectionObserverHandler,
    TInt aQos, RSocketServ& aSocketServ, RConnection& aConnection,
    TUint aProtocol, TBool aReuseAddress ) :
    CActive( EPriorityStandard ), iSessionId( aSessionId ),
    iStreamId( aStreamId ), iConnectionId( aConnectionId ),
    iLocalAddr( aLocalAddr ), iObserver( aObserver ),
    iConnectionObserverHandler( aConnectionObserverHandler ),
    iIpTOS( aQos ), iProtocol( aProtocol ), iReuseAddress( aReuseAddress ),
    iSocketServ( aSocketServ ), iConnection( aConnection ), 
    iDestinationAddress( KInetAddrNone, 0 ),
    iReceiverState( EStreamingStatePassive ),
    iSenderState( EStreamingStatePassive ),
    iTcpConnectionState( ETcpConnectionUnspecifiedState )
    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// CNcmConnection::NewL
// ---------------------------------------------------------------------------
//
CNcmConnection* CNcmConnection::NewL(TUint aSessionId, TUint aStreamId,
    TUint aConnectionId, const TInetAddr& aLocalAddr,
    MNcmMultiplexerConnectionObserver& aObserver,
    CNcmConnectionObserverHandler& aConnectionObserverHandler,TInt aQos,
    RSocketServ& aSocketServ, RConnection& aConnection,
    TUint aProtocol, TBool aReuseAddress )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnection::NewL" )
    
    CNcmConnection* self =
        CNcmConnection::NewLC( aSessionId, aStreamId, aConnectionId,
            aLocalAddr, aObserver, aConnectionObserverHandler, aQos,
            aSocketServ, aConnection, aProtocol, aReuseAddress );
            
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNcmConnection::NewLC
// ---------------------------------------------------------------------------
//
CNcmConnection* CNcmConnection::NewLC( TUint aSessionId, TUint aStreamId,
    TUint aConnectionId, const TInetAddr& aLocalAddr,
    MNcmMultiplexerConnectionObserver& aObserver,
    CNcmConnectionObserverHandler& aConnectionObserverHandler, TInt aQos,
    RSocketServ& aSocketServ, RConnection& aConnection, TUint aProtocol,
    TBool aReuseAddress )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnection::NewLC" )
    
    CNcmConnection* self =
        new( ELeave ) CNcmConnection( aSessionId, aStreamId, aConnectionId,
            aLocalAddr, aObserver, aConnectionObserverHandler, aQos,
            aSocketServ, aConnection, aProtocol, aReuseAddress );
            
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CNcmConnection::ConstructL
// ---------------------------------------------------------------------------
//
void CNcmConnection::ConstructL()
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnection::ConstructL" )
       
/*    switch( iProtocol )
        {
        case KProtocolInetTcp:
            {
            InitializeTcpL();                                                  
            break;
            }
        case KProtocolInetUdp:
            {
            InitializeUdpL();
            break;
            }
        default:
            {
            __CONNECTIONMULTIPLEXER_ASSERT_L( EFalse, KErrNotSupported );
            }
        }   

    __CONNECTIONMULTIPLEXER_ADDRLOG(
        "CNcmConnection::ConstructL - EXIT - LOCAL ADDRESS: ", iLocalAddr )*/
    }


// ---------------------------------------------------------------------------
// CNcmConnection::~CNcmConnection
// ---------------------------------------------------------------------------
//
CNcmConnection::~CNcmConnection()
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnection::~CNcmConnection" )
    
/*    Cancel();
    delete iSender;   
    delete iReceiver;

    iSocket.Close();
    iListenSocket.Close();
    delete iMessage;*/
    }


// ---------------------------------------------------------------------------
// CNcmConnection::OpenTcpConnectionL
// ---------------------------------------------------------------------------
//
void CNcmConnection::OpenTcpConnectionL( TNATFWTcpConnectionSetup /*aConfig*/,
    const TInetAddr& /*aDestAddr*/ )
    {
/*    __CONNECTIONMULTIPLEXER_ASSERT_L(
        KProtocolInetTcp == iProtocol, KErrArgument );
    __CONNECTIONMULTIPLEXER_ASSERT_L(
        ETcpConnectionUnspecifiedState == iTcpConnectionState, KErrInUse );
    __CONNECTIONMULTIPLEXER_ADDRLOG(
        "CNcmConnection::OpenTcpConnectionL - DEST ADDRESS: ", aDestAddr )
    __CONNECTIONMULTIPLEXER_ADDRLOG(
        "CNcmConnection::OpenTcpConnectionL - LOCAL ADDRESS: ", iLocalAddr )
            
    iConnSetupConf = aConfig;    
    iDestinationAddress = aDestAddr;
    
    if ( ETcpSetupActive == aConfig )
        {
        User::LeaveIfError( iSocket.Open( iSocketServ, KAfInet, 
            KSockStream, KProtocolInetTcp, iConnection ) );
                      
        User::LeaveIfError(
            iSocket.SetOpt( KSoIpTOS, KSolInetIp, iIpTOS ) );
        
        // Listener socket has same local address.                                
        User::LeaveIfError(
            iSocket.SetOpt( KSoReuseAddr, KProtocolInetIp, 1 ) );

        User::LeaveIfError( iSocket.Bind( iLocalAddr ) );
        
        iSocket.Connect( iDestinationAddress, iStatus );
        }
    else if ( ETcpSetupPassive == aConfig )
        {
        User::LeaveIfError( iSocket.Open( iSocketServ ) );
        
        if ( !iIsListening )
            {
            User::LeaveIfError( iListenSocket.Listen( 1 ) );
            iIsListening = ETrue;
            }
           
        iListenSocket.Accept( iSocket, iStatus );   
        }
    else
        {
        __CONNECTIONMULTIPLEXER(
            "CNcmConnection::OpenTcpConnectionL - Default branch" )
        __CONNECTIONMULTIPLEXER_ASSERT_L( EFalse, KErrNotSupported );
        }
        
    iTcpConnectionState = ETcpConnectionConnectingState;    
    SetActive();*/
    }  


// ---------------------------------------------------------------------------
// CNcmConnection::CloseTcpConnectionL
// ---------------------------------------------------------------------------
//
void CNcmConnection::CloseTcpConnectionL()
    {
/*    __CONNECTIONMULTIPLEXER( "CNcmConnection::CloseTcpConnectionL" )
    __CONNECTIONMULTIPLEXER_ASSERT_L(
        KProtocolInetTcp == iProtocol, KErrArgument );
    __CONNECTIONMULTIPLEXER_ASSERT_L(
        ETcpConnectionUnspecifiedState != iTcpConnectionState, KErrNotReady );
   
    // Cancel all outstanding operations
    iSender->Cancel();
    iReceiver->Cancel();
    Cancel();
    
    // Close the socket
    iListenSocket.CancelAll(); 
    iSocket.Close();

    iSenderState = EStreamingStatePassive;
    iReceiverState = EStreamingStatePassive; 

    if (  ETcpConnectionConnectingState == iTcpConnectionState )
        {
        iObserver.ConnectionNotify( iStreamId, iConnectionId,
            EMultiplexerTcpSetupCompleted, KErrCancel );
        }
        
    iTcpConnectionState = ETcpConnectionUnspecifiedState;*/
    }


// ---------------------------------------------------------------------------
// CNcmConnection::SetReceivingStateL
// ---------------------------------------------------------------------------
//
void CNcmConnection::SetReceivingStateL(
    TNATFWStreamingState /*aState*/ )
    {
/*    switch( aState )
        {
        case EStreamingStateActive:
            this->ActivateReceivingL();
            break;
        case EStreamingStatePassive:           
            this->DeactivateReceivingL();
            break;
        default:
            {
            __CONNECTIONMULTIPLEXER_FAILURE_L( KErrNotSupported );
            }
        }*/
    }
 
  
// ---------------------------------------------------------------------------
// CNcmConnection::SetSendingStateL
// ---------------------------------------------------------------------------
//
void CNcmConnection::SetSendingStateL(
    const TInetAddr& /*aDestAddr*/, TNATFWStreamingState /*aState*/ )
    { 
/*    switch( aState )
        {
        case EStreamingStateActive:
            this->ActivateSendingL( aDestAddr );
            break;
        case EStreamingStatePassive:      
            this->DeactivateSendingL();
            break;
        default:
            __CONNECTIONMULTIPLEXER_FAILURE_L( KErrNotSupported );
        }*/
    }


// ---------------------------------------------------------------------------
// CNcmConnection::CheckSenderValidityToSending
// ---------------------------------------------------------------------------
//
TBool CNcmConnection::CheckSenderValidityToSending(
    const TInetAddr& /*aDestinationAddr*/ )
    {
/*    if ( iIsGenericDestination && KProtocolInetUdp == iProtocol )
        {
        static_cast<CNcmUdpSender*>( iSender )->SetDestination(
                aDestinationAddr );
        }
    */
    TBool validity( EFalse );
/*    if ( EStreamingStateActive == iSenderState 
        && iDestinationAddress.CmpAddr( aDestinationAddr ) )
        {
        __CONNECTIONMULTIPLEXER(
            "CNcmConnection::CheckSenderValidityToSending TRUE" )
        
        validity = ETrue;
        }
    else
        {
        __CONNECTIONMULTIPLEXER(
            "CNcmConnection::CheckSenderValidityToSending FALSE" )
        }*/
    
    return validity;
    }

    
// ---------------------------------------------------------------------------
// CNcmConnection::DestinationAddress
// ---------------------------------------------------------------------------
//
const TInetAddr& CNcmConnection::DestinationAddress() const
    {
    return iDestinationAddress;
    }


// ---------------------------------------------------------------------------
// CNcmConnection::CancelMessageSendL
// ---------------------------------------------------------------------------
//
void CNcmConnection::CancelMessageSendL(
    const MNcmSenderObserver* /*aSenderObserver*/ )
    {
//    iSender->CancelMessageSendL( aSenderObserver );
    }


// ---------------------------------------------------------------------------
// CNcmConnection::IsGenericDestination
// ---------------------------------------------------------------------------
//
TBool CNcmConnection::IsGenericDestination() const
    {
    __CONNECTIONMULTIPLEXER_INT1(
        "CNcmConnection::IsGenericDestination", iIsGenericDestination )
    
    return iIsGenericDestination;
    }


// ---------------------------------------------------------------------------
// CNcmConnection::SetAcceptedFromAddressL
// ---------------------------------------------------------------------------
//
void CNcmConnection::SetAcceptedFromAddressL( const TInetAddr& /*aAddress*/ )
    {
/*    __CONNECTIONMULTIPLEXER( "CNcmConnection::SetAcceptedFromAddressL" )
    __CONNECTIONMULTIPLEXER_ASSERT_L(
        KProtocolInetUdp == iProtocol, KErrArgument );

    if ( !IsActive() )
        {       
        __CONNECTIONMULTIPLEXER_ADDRLOG(
            "CNcmConnection::SetAcceptedFromAddressL - iAcceptedFromAddr: ", iAcceptedFromAddr )
            
        iAcceptedFromAddr = aAddress;
        iSender->Cancel();  
        iReceiver->Cancel();
                   
        if ( KAFUnspec != aAddress.Address() )
            {
            iSocket.Connect( iAcceptedFromAddr, iStatus );
            SetActive(); 
            }
        else
            {
            iReuseAddress = ETrue;
            iSocket.Close();
            this->InitializeUdpL();
            
            if ( iReceiverState == EStreamingStateActive )
                {
                iReceiver->Activate();
                } 
            }        
        }
    else
        {
        __CONNECTIONMULTIPLEXER( "CNcmConnection::SetAcceptedFromAddressL - State not valid" ) 
        }*/
    }


// ---------------------------------------------------------------------------
// CNcmConnection::Send
// ---------------------------------------------------------------------------
//
void CNcmConnection::Send( const TDesC8& /*aMessage*/,
    const TInetAddr& /*aDestinationAddress*/,
    MNcmSenderObserver* /*aSenderObserver*/ )
    {
/*    __CONNECTIONMULTIPLEXER( "CNcmConnection::Send" )
    
    // Notify source observer from message
    this->OutgoingMessageAvailable( aMessage, aSenderObserver,
        aDestinationAddress );*/
    }


// ---------------------------------------------------------------------------
// CNcmConnection::SendDirectlyToNetwork
// ---------------------------------------------------------------------------
//
void CNcmConnection::SendDirectlyToNetwork( const TDesC8& /*aMessage*/,
    MNcmSenderObserver* /*aSenderObserver*/ )
    {
/*    __CONNECTIONMULTIPLEXER( "CNcmConnection::SendDirectlyToNetwork" )
        
    if ( EStreamingStateActive != iSenderState )
        {
        iObserver.ConnectionNotify( iStreamId,
                                    iConnectionId, 
                                    EMultiplexerConnectionError,
                                    KErrNotReady );
        }
    else
        {
        iSender->Send( aMessage, aSenderObserver );
        }*/
    }


// ---------------------------------------------------------------------------
// CNcmConnection::ConnectionId
// ---------------------------------------------------------------------------
//
TUint CNcmConnection::ConnectionId() const
    {
    return iConnectionId;
    }


// ---------------------------------------------------------------------------
// CNcmConnection::LocalAddress
// ---------------------------------------------------------------------------
//
TInetAddr& CNcmConnection::LocalAddress()
    {
    return iLocalAddr; 
    }


// ---------------------------------------------------------------------------
// From class MNcmSourceObserver
//
// CNcmConnection::OutgoingMessageAvailable
// ---------------------------------------------------------------------------
//
void CNcmConnection::OutgoingMessageAvailable( const TDesC8& /*aMessage*/,
    MNcmSenderObserver* /*aSenderObserver*/, const TInetAddr& /*aDestAddr*/ )
    {
/*    __CONNECTIONMULTIPLEXER( "CNcmConnection::OutgoingMessageAvailable" )
        
    TBool consumed( EFalse );
    
    // get message
    delete iMessage;
    iMessage = NULL;
    iMessage = aMessage.AllocL();
 
    TRAPD( error, consumed = iConnectionObserverHandler.RevealOutgoingMessageL( iStreamId, iConnectionId, iMessage,
        aDestAddr ) )
        
    // see if observing objects need this message
    if ( KErrNone != error )
        {
        iObserver.ConnectionError( iSessionId,
                                   iStreamId,
                                   iConnectionId,
                                   EMultiplexerConnectionError,
                                   error );        
        }
    else if ( !consumed )
        {
        __CONNECTIONMULTIPLEXER( 
            "CNcmConnection::OutgoingMessageAvailable - lets send message to network" )
                                      
        SendDirectlyToNetwork( *iMessage, aSenderObserver );
        }
    else
        {
        __CONNECTIONMULTIPLEXER( 
            "CNcmConnection::OutgoingMessageAvailable - message consumed" )        
        }*/
    }


// ---------------------------------------------------------------------------
// From class MNcmReceiverSenderObserver
//
// CNcmConnection::MessageAvailable
// ---------------------------------------------------------------------------
//
void CNcmConnection::MessageAvailable()
    {
/*    __CONNECTIONMULTIPLEXER_INT1(
        "CNcmConnection::MessageAvailable - connection ID: ", iConnectionId ) 
        
    TBool consumed( EFalse );
 
     // get message
    delete iMessage;
    iMessage = NULL;
    iMessage = iReceiver->Message().AllocL();
    
    TRAPD( error, consumed = iConnectionObserverHandler.RevealIncomingMessageL(
        iStreamId, iMessage, iLocalAddr, iReceiver->FromAddress() ) )
        
    // see if observing objects need this message
    if ( KErrNone != error )
        {
        iObserver.ConnectionError( iSessionId,
                                   iStreamId,
                                   iConnectionId,
                                   EMultiplexerConnectionError,
                                   error );    
        }
    else if ( !consumed )
        {
        __CONNECTIONMULTIPLEXER( 
            "CNcmMessageHandler::HandleIncomingMessageL - message not consumed" ) 
        }
    else
        {
        __CONNECTIONMULTIPLEXER( 
            "CNcmMessageHandler::HandleIncomingMessageL - message consumed" )       
        }*/
    }


// ---------------------------------------------------------------------------
// From class MNcmReceiverSenderObserver
//
// CNcmConnection::Error
// ---------------------------------------------------------------------------
//
void CNcmConnection::Error( TInt /*aError*/ )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnection::Error" )
    
/*    iObserver.ConnectionNotify( iStreamId,
                                iConnectionId, 
                                EMultiplexerConnectionError,
                                aError );*/
    }


// ---------------------------------------------------------------------------
// From class CActive
//
// CNcmConnection::RunL
// ---------------------------------------------------------------------------
//
void CNcmConnection::RunL()
    {
/*    __CONNECTIONMULTIPLEXER_INT1( "CNcmConnection::RunL", iStatus.Int() )
    
    if ( KProtocolInetTcp == iProtocol )
        {
        __CONNECTIONMULTIPLEXER_ASSERT_L(
            ETcpConnectionConnectingState == iTcpConnectionState,
                KErrNotReady );
        
        iTcpConnectionState = ( KErrNone == iStatus.Int() ) ?
            ETcpConnectionConnectedState : ETcpConnectionUnspecifiedState;
            
        iObserver.ConnectionNotify( iStreamId, iConnectionId, 
            EMultiplexerTcpSetupCompleted, iStatus.Int() );        
        }
    else 
        {
        if ( iReceiverState == EStreamingStateActive )
            {
            iReceiver->Activate();
            }   
        }*/
    }


// ---------------------------------------------------------------------------
// From class CActive
//
// DoCancel
// ---------------------------------------------------------------------------
//
void CNcmConnection::DoCancel()
    {
/*    __CONNECTIONMULTIPLEXER( "CNcmConnection::DoCancel" )

    if ( KProtocolInetTcp == iProtocol )
        {
        iListenSocket.CancelAll(); 
        }
       
    iSocket.CancelAll();*/
    }

/*
// ---------------------------------------------------------------------------
// CNcmConnection::InitializeUdpL
// ---------------------------------------------------------------------------
//
void CNcmConnection::InitializeUdpL()
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnection::InitializeUdpL" )
    delete iSender;
    iSender = NULL;
    delete iReceiver; 
    iReceiver = NULL;
         
    User::LeaveIfError( iSocket.Open( iSocketServ, KAfInet,
        KSockDatagram, KProtocolInetUdp, iConnection ) );

    User::LeaveIfError( iSocket.SetOpt( KSoIpTOS, KSolInetIp, iIpTOS ) );
    
    TUint port( iLocalAddr.Port() );   
    // Checks port validity   
    if ( !iReuseAddress )
        {
        TBool requiredPortOpen( EFalse );
        TInt error( KErrNone );
        
        while ( !requiredPortOpen )
            {
            error = iSocket.SetLocalPort( port );

            if ( KErrNone != error )
                {
                iObserver.GetNewFreePort(
                    iStreamId, iConnectionId, port );
                    
                if ( 0 == port )
                    {
                    User::Leave( KErrGeneral );
                    }
                }
            else
                {
                requiredPortOpen = ETrue; 
                }   
            }
        iLocalAddr.SetPort( port );       
        }
    else
        {                          
        User::LeaveIfError(
            iSocket.SetOpt( KSoReuseAddr, KProtocolInetIp, 1 ) );
            
        User::LeaveIfError( iSocket.SetLocalPort( iLocalAddr.Port() ) );
        }

    iSender = CNcmUdpSender::NewL( iSocket, *this );
    iReceiver = CNcmUdpReceiver::NewL( iSocket, *this );
    }

        
// ---------------------------------------------------------------------------
// CNcmConnection::InitializeTcpL
// ---------------------------------------------------------------------------
//
void CNcmConnection::InitializeTcpL()
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnection::InitializeTcpL" )
        
    // Open listen socket and check port validity
    User::LeaveIfError( iListenSocket.Open( iSocketServ, KAfInet, 
        KSockStream, KProtocolInetTcp, iConnection ) );

    TUint port( iLocalAddr.Port() );   
    // Checks port validity   
    if ( !iReuseAddress )
        {         
        TBool requiredPortOpen( EFalse );
        TInt error( KErrNone );
        
        while ( !requiredPortOpen )
            {
            error = iListenSocket.Bind( iLocalAddr );

            if ( KErrNone != error )
                {
                iObserver.GetNewFreePort(
                    iStreamId, iConnectionId, port );
                                                
                if ( 0 == port )
                    {
                    User::Leave( KErrGeneral );
                    }
                    
                iLocalAddr.SetPort( port );
                }
            else
                {
                requiredPortOpen = ETrue; 
                }   
            }       
        }
    else
        {                          
        User::LeaveIfError(
            iListenSocket.SetOpt( KSoReuseAddr, KProtocolInetIp, 1 ) );
            
        User::LeaveIfError( iListenSocket.Bind( iLocalAddr ) );
        }    
           
    iSender = CNcmTcpSender::NewL( iSocket, *this );    
    iReceiver = CNcmTcpReceiver::NewL( iSocket, *this );
    }


// ---------------------------------------------------------------------------
// CNcmConnection::ActivateReceivingL
// ---------------------------------------------------------------------------
//
void CNcmConnection::ActivateReceivingL()
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnection::ActivateReceivingL - Entry" )

    if ( KProtocolInetUdp == iProtocol )
        {
        iReceiverState = EStreamingStateActive;
        iReceiver->Activate();
        }
    else if ( KProtocolInetTcp == iProtocol )
        {
        __ASSERT_ALWAYS( ETcpConnectionConnectedState == iTcpConnectionState,
            User::Leave( KErrNotReady ) );

        iReceiverState = EStreamingStateActive;
        iReceiver->Activate();
        }
    else
        {
        __CONNECTIONMULTIPLEXER_FAILURE_L( KErrNotSupported );
        }
          
    iObserver.ConnectionNotify( iStreamId, iConnectionId,
        EMultiplexerReceivingActivated, KErrNone );
  
    __CONNECTIONMULTIPLEXER_ADDRLOG(
        "CNcmConnection::ActivateReceivingL - EXIT - LOCAL ADDRESS: ", iLocalAddr )  
    }


// ---------------------------------------------------------------------------
// CNcmConnection::DeactivateReceivingL
// ---------------------------------------------------------------------------
//
void CNcmConnection::DeactivateReceivingL()
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnection::DeactivateReceivingL" )
    __CONNECTIONMULTIPLEXER_ASSERT_L( 
        EStreamingStatePassive != iReceiverState, KErrNotReady );
    
    iReceiver->Deactivate();
    iReceiverState = EStreamingStatePassive;
        
    iObserver.ConnectionNotify( iStreamId, iConnectionId,
        EMultiplexerReceivingDeactivated, KErrNone );     
    }
    
    
// ---------------------------------------------------------------------------
// CNcmConnection::ActivateSendingL
// ---------------------------------------------------------------------------
//
void CNcmConnection::ActivateSendingL( const TInetAddr& aDestAddr )
    {
    __CONNECTIONMULTIPLEXER_ADDRLOG(
        "CNcmConnection::ActivateSendingL - DEST ADDRESS: ", 
        aDestAddr )
    __CONNECTIONMULTIPLEXER_INT1(
        "CNcmConnection::ActivateSendingL - Connection ID: ",
        iConnectionId )    
    
    iDestinationAddress = aDestAddr;
    
    // Check if this destination can be used to send data to the arbitrary
    // remote address.
    aDestAddr.IsUnspecified() ? 
        iIsGenericDestination = ETrue : iIsGenericDestination = EFalse;
    
    if ( KProtocolInetUdp == iProtocol )
        {            
        static_cast<CNcmUdpSender*>( iSender )->SetDestination(
                iDestinationAddress );
        
        iSender->Activate();
        iSenderState = EStreamingStateActive;               
        }
    else if ( KProtocolInetTcp == iProtocol )
        {  
        __ASSERT_ALWAYS( ETcpConnectionConnectedState == iTcpConnectionState,
            User::Leave( KErrNotSupported ) );
        
        iSenderState = EStreamingStateActive;             
        iSender->Activate();
        }
    else
        {
        __CONNECTIONMULTIPLEXER_FAILURE_L( KErrNotSupported );        
        }
    
    iObserver.ConnectionNotify( iStreamId, iConnectionId,
        EMultiplexerSendingActivated, KErrNone );
    }


// ---------------------------------------------------------------------------
// CNcmConnection::DeactivateSendingL
// ---------------------------------------------------------------------------
//
void CNcmConnection::DeactivateSendingL()
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnection::DeactivateSendingL" )
    __CONNECTIONMULTIPLEXER_ASSERT_L(
        EStreamingStatePassive != iSenderState, KErrNotReady );

    iSender->Deactivate();
    iSenderState = EStreamingStatePassive;     
    iObserver.ConnectionNotify( iStreamId, iConnectionId,
            EMultiplexerSendingDeactivated, KErrNone );      
    }*/
