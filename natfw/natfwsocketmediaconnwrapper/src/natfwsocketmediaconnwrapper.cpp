/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Connecting different media-engines into nat framework.
*
*/




#include "natfwsocketmediaconnwrapper.h"
#include "natfwsocketmediaconnwrapperlogs.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CNATFWSocketMediaConnWrapper::CNATFWSocketMediaConnWrapper()
// ---------------------------------------------------------------------------
//
CNATFWSocketMediaConnWrapper::CNATFWSocketMediaConnWrapper(  ) 
    {
    }


// ---------------------------------------------------------------------------
// CNATFWSocketMediaConnWrapper::ConstructL( )
// ---------------------------------------------------------------------------
//
void CNATFWSocketMediaConnWrapper::ConstructL( TUint aProtocol,
    TUint aStreamId, TUint aPort )
    {
    __SOCKETMEDIACONNWRAPPER(
        "CNATFWSocketMediaConnWrapper::ConstructL start")
    
    iHandler = CNATFWSocketHandler::NewL( *this, *this, aProtocol, aPort );
    iStreamId = aStreamId;
    }


// ---------------------------------------------------------------------------
// CNATFWSocketMediaConnWrapper::NewL( )
// ---------------------------------------------------------------------------
//
EXPORT_C CNATFWSocketMediaConnWrapper* CNATFWSocketMediaConnWrapper::NewL(
    TUint aPort, TUint aProtocol, TUint aStreamId  )
    {
    __SOCKETMEDIACONNWRAPPER( "CNATFWSocketMediaConnWrapper::NewL" )
    
    CNATFWSocketMediaConnWrapper* self = 
        new (ELeave) CNATFWSocketMediaConnWrapper();
    CleanupStack::PushL( self );
    self->ConstructL( aProtocol, aStreamId, aPort );
    CleanupStack::Pop( self );
    
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWSocketMediaConnWrapper::~CNATFWSocketMediaConnWrapper()
// ---------------------------------------------------------------------------
//
CNATFWSocketMediaConnWrapper::~CNATFWSocketMediaConnWrapper()
    {
    __SOCKETMEDIACONNWRAPPER(
        "CNATFWSocketMediaConnWrapper::~CNATFWSocketMediaConnWrapper")
    
    iMediaObserver = NULL;
    delete iHandler;
    }


// ---------------------------------------------------------------------------
// CNATFWSocketMediaConnWrapper::StreamId()
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CNATFWSocketMediaConnWrapper::StreamId()
    {
    __SOCKETMEDIACONNWRAPPER(
        "CNATFWSocketMediaConnWrapper::StreamId")
    
    return iStreamId;
    }
    
    
// ---------------------------------------------------------------------------
// CNATFWSocketMediaConnWrapper::OpenTcpConnectionL()
// ---------------------------------------------------------------------------
//
void CNATFWSocketMediaConnWrapper::OpenTcpConnectionL( 
    TNATFWTcpConnectionSetup aSetup, MNsmcwTcpConnectionObserver& aObserver )
    {
    iHandler->OpenTcpConnectionL( aSetup, aObserver );
    }


// ---------------------------------------------------------------------------
// CNATFWSocketMediaConnWrapper::CloseTcpConnection()
// ---------------------------------------------------------------------------
//
void CNATFWSocketMediaConnWrapper::CloseTcpConnection()
    {
    iHandler->CloseTcpConnection();
    }


// ---------------------------------------------------------------------------
// CNATFWSocketMediaConnWrapper::OutgoingAddr( )
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWSocketMediaConnWrapper::OutgoingAddr(
        TSockAddr& aSocketToSendForPeer )
    {    
    iHandler->LocalAddress( aSocketToSendForPeer );
    }
    
    
// ---------------------------------------------------------------------------
// CNATFWSocketMediaConnWrapper::SetIncomingAddrL( )
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWSocketMediaConnWrapper::SetIncomingAddrL(
        const TSockAddr& aSocketToReceiveFromPeer )
    {    
    iHandler->SetAddrL( aSocketToReceiveFromPeer );
    }


// ---------------------------------------------------------------------------
// CNATFWSocketMediaConnWrapper::SetReceivingStateL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWSocketMediaConnWrapper::SetReceivingStateL( 
    TNATFWStreamingState aState )
    {    
    if ( EStreamingStateActive == aState )
        {
        iHandler->SetReceivingStateL();
        }
    if ( EStreamingStatePassive == aState )
        {
        iHandler->DeactivateReceiving();
        }
    }


// ---------------------------------------------------------------------------
// CNATFWSocketMediaConnWrapper::SetSendingStateL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWSocketMediaConnWrapper::SetSendingStateL( 
    TNATFWStreamingState aState )
    {    
    if ( EStreamingStateActive == aState )
        {
        iHandler->SetSendingStateL();
        }
    if ( EStreamingStatePassive == aState )
        {
        iHandler->DeactivateSending();
        }
    }
    
    
// ---------------------------------------------------------------------------
// CNATFWSocketMediaConnWrapper::SetReceivingStateForMuxWrapper( )
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWSocketMediaConnWrapper::SetReceivingStateForMuxWrapper(
    TNATFWStreamingState aState )
    {
    iHandler->SetReceivingStateForMuxWrapper( aState );
    }


// ---------------------------------------------------------------------------
// CNATFWSocketMediaConnWrapper::SendL( )
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWSocketMediaConnWrapper::SendL( 
    const TDesC8& aStreamPortion )
    {
    iHandler->SendL( aStreamPortion );
    }


// ---------------------------------------------------------------------------
// CNATFWSocketMediaConnWrapper::RegisterMediaWrapperObserverL( )
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWSocketMediaConnWrapper::RegisterMediaWrapperObserverL( 
        MNATFWMediaWrapperObserver* aMediaObserver,
        const RSocketServ& aServer, const TInetAddr& aLocalAddr )
    {
    __SOCKETMEDIACONNWRAPPER(
        "CNATFWSocketMediaConnWrapper::RegisterMediaWrapperObserverL" )
    
    iMediaObserver = aMediaObserver;
    iHandler->SetLocalAddress( aServer, aLocalAddr );
    iHandler->SetMediaObserverL( aMediaObserver );
    }


// ---------------------------------------------------------------------------
// CNATFWSocketMediaConnWrapper::Close()
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWSocketMediaConnWrapper::Close()
    {
    __SOCKETMEDIACONNWRAPPER( "CNATFWSocketMediaConnWrapper::Close" )
    
    delete this;
    }

// ---------------------------------------------------------------------------
// CNATFWSocketMediaConnWrapper::GetAddresses()
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWSocketMediaConnWrapper::GetAddresses( TInetAddr& aLocalAddr,
    TInetAddr& aRemoteAddr )
    {
    __SOCKETMEDIACONNWRAPPER( "CNATFWSocketMediaConnWrapper::GetAddressesL" )
    
    iHandler->LocalAddress( aLocalAddr );    
    iHandler->GetRemoteAddress( aRemoteAddr );
    }
    

// ---------------------------------------------------------------------------
// CNATFWSocketMediaConnWrapper::DeliverBuffer( )
// ---------------------------------------------------------------------------
//
void CNATFWSocketMediaConnWrapper::DeliverBuffer( TDes8& aStreamPortion )
    {
    __SOCKETMEDIACONNWRAPPER( "CNATFWSocketMediaConnWrapper::DeliverBuffer" )

    iMediaObserver->ReceivedFrom( iStreamId, aStreamPortion );
    }


// ---------------------------------------------------------------------------
// CNATFWSocketMediaConnWrapper::SendingCompleted( )
// ---------------------------------------------------------------------------
//
void CNATFWSocketMediaConnWrapper::SendingCompleted( TInt aError )
    {
    __SOCKETMEDIACONNWRAPPER(
        "CNATFWSocketMediaConnWrapper::SendingCompleted" )
 
    iMediaObserver->SendingCompleted( aError );
    }
