/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Transmit indications to sender
*
*/




#include "cstunindicationtransmitter.h"
#include "stunturnclientlogs.h"


// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// CStunIndicationTransmitter::CStunIndicationTransmitter
// ---------------------------------------------------------------------------
//
CStunIndicationTransmitter::CStunIndicationTransmitter( 
    MNcmConnectionMultiplexer& aMux, TUint aStreamId, TUint aConnectionId ) :
    iMux( aMux ),
    iStreamId( aStreamId ),
    iConnectionId( aConnectionId )    
    {   
    }


// ---------------------------------------------------------------------------
// CStunIndicationTransmitter::ConstructL
// ---------------------------------------------------------------------------
//
void CStunIndicationTransmitter::ConstructL()
    {
    iSender = CNATFWUNSAFMediaConnSender::NewL( iMux );
    iTcpRelaySender = CNATFWUNSAFTcpRelayPacketSender::NewL( iMux );
    }


// ---------------------------------------------------------------------------
// CStunIndicationTransmitter::NewL
// ---------------------------------------------------------------------------
//
CStunIndicationTransmitter* CStunIndicationTransmitter::NewL(
    MNcmConnectionMultiplexer& aMux, TUint aStreamId, TUint aConnectionId )
    {
    CStunIndicationTransmitter* self = new ( ELeave ) 
        CStunIndicationTransmitter ( aMux, aStreamId, aConnectionId );
    CleanupStack::PushL( self );
    self->ConstructL( );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CStunIndicationTransmitter::~CStunIndicationTransmitter()
    {
    __STUNTURNCLIENT( 
        "CStunIndicationTransmitter::~CStunIndicationTransmitter" )
    delete iSender;
    delete iTcpRelaySender;
    }


// ---------------------------------------------------------------------------
// CStunIndicationTransmitter::TransmitL
// ---------------------------------------------------------------------------
//
void CStunIndicationTransmitter::TransmitL( CNATFWUNSAFMessage& aIndication )
    {
    __STUNTURNCLIENT( "CStunIndicationTransmitter::TransmitL" )
    
    iSender->SendL( iStreamId, iConnectionId, aIndication, KNullDesC8(),
        NULL, EFalse );
    }


// ---------------------------------------------------------------------------
// CStunIndicationTransmitter::TransmitL
// ---------------------------------------------------------------------------
//
void CStunIndicationTransmitter::TransmitL( CNATFWUNSAFMessage& aIndication,
                                            const TInetAddr& aAddress,
                                            const TDesC8& aSharedSecret,
                                            TBool aAddFingerprint )
    {
    __STUNTURNCLIENT( "CStunIndicationTransmitter::TransmitL" )

    iSender->SendL( iStreamId, iConnectionId, aIndication, aSharedSecret,
        aAddress, NULL, aAddFingerprint );
    }
    
// ---------------------------------------------------------------------------
// CStunIndicationTransmitter::TransmitL
// ---------------------------------------------------------------------------
//
void CStunIndicationTransmitter::TransmitL( 
    const CNATFWUNSAFTcpRelayPacket& aMessage )
    {
    __STUNTURNCLIENT( "CStunIndicationTransmitter::TransmitL - TCP" )
    
    iTcpRelaySender->SendL( iStreamId, iConnectionId, aMessage );
    }    


