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
* Description:  
*
*/

#include <e32base.h>
#include <in_sock.h>
#include <es_sock.h>
#include "nspdefs.h"
#include "s_natfwsocketmediaconnwrapper.h"

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// CNSPWrapper::CNSPWrapper
// ---------------------------------------------------------------------------
// 
CNSPWrapper::CNSPWrapper( TUint aStreamId )
    : iOutgoingAddress( KInetAddrLoop, aStreamId )
    {
    }


// ---------------------------------------------------------------------------
// CNSPWrapper::ConstructL
// ---------------------------------------------------------------------------
// 
void CNSPWrapper::ConstructL()
    {
    }


// ---------------------------------------------------------------------------
// CNSPWrapper::~CNSPWrapper
// ---------------------------------------------------------------------------
// 
CNSPWrapper::~CNSPWrapper()
    {
    }


// ---------------------------------------------------------------------------
// CNSPWrapper::NewL
// ---------------------------------------------------------------------------
// 
CNSPWrapper* CNSPWrapper::NewL( TUint aStreamId )
    {
    CNSPWrapper* self = new (ELeave) CNSPWrapper( aStreamId );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPWrapper::OutgoingAddr
// ---------------------------------------------------------------------------
// 
void CNSPWrapper::OutgoingAddr( TSockAddr& aSocketToSendForPeer )
    {
    aSocketToSendForPeer = iOutgoingAddress;
    
    TBuf<40> output;
    iOutgoingAddress.Output( output );
    NSPLOG_STR2( "(STUB)CNSPWrapper::OutgoingAddr, address:", output )
    NSPLOG_UINT( "(STUB)CNSPWrapper::OutgoingAddr, port   :", aSocketToSendForPeer.Port() )
    }


// ---------------------------------------------------------------------------
// CNSPWrapper::SetIncomingAddrL
// ---------------------------------------------------------------------------
// 
void CNSPWrapper::SetIncomingAddrL( const TSockAddr& aSocketToReceiveFromPeer )
    {
    iIncomingAddress = aSocketToReceiveFromPeer;
    
    TBuf<40> output;
    iIncomingAddress.Output( output );
    NSPLOG_STR2( "(STUB)CNSPWrapper::SetIncomingAddrL, address:", output )
    NSPLOG_UINT( "(STUB)CNSPWrapper::SetIncomingAddrL, port   :", iIncomingAddress.Port() )
    }


// ---------------------------------------------------------------------------
// CNSPWrapper::StreamId
// ---------------------------------------------------------------------------
// 
TUint CNSPWrapper::StreamId()
    {
    return iOutgoingAddress.Port();
    }


// ---------------------------------------------------------------------------
// CNSPWrapper::SetReceivingStateL
// ---------------------------------------------------------------------------
// 
void CNSPWrapper::SetReceivingStateL( TNATFWStreamingState aState )
    {
    NSPLOG_UINT( "(STUB)CNSPWrapper::SetReceivingStateL, aState:", (TUint)aState )
    }


// ---------------------------------------------------------------------------
// CNSPWrapper::SetSendingStateL
// ---------------------------------------------------------------------------
// 
void CNSPWrapper::SetSendingStateL( TNATFWStreamingState aState )
    {
    NSPLOG_UINT( "(STUB)CNSPWrapper::SetSendingStateL, aState:", (TUint)aState )
    }


// end of file
