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
    }


// ---------------------------------------------------------------------------
// CStunIndicationTransmitter::NewL
// ---------------------------------------------------------------------------
//
CStunIndicationTransmitter* CStunIndicationTransmitter::NewL(
    MNcmConnectionMultiplexer& aMux, TUint aStreamId, TUint aConnectionId )
    {
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CStunIndicationTransmitter::~CStunIndicationTransmitter()
    {    
    }


// ---------------------------------------------------------------------------
// CStunIndicationTransmitter::TransmitL
// ---------------------------------------------------------------------------
//
void CStunIndicationTransmitter::TransmitL( CNATFWUNSAFMessage& aIndication )
    {
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
    }
    
// ---------------------------------------------------------------------------
// CStunIndicationTransmitter::TransmitL
// ---------------------------------------------------------------------------
//
void CStunIndicationTransmitter::TransmitL( 
    const CNATFWUNSAFTcpRelayPacket& aMessage )
    {
    }    


