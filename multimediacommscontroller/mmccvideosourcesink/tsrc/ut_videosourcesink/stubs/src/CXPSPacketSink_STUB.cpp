/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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



#include <CXPSPacketSink.h>

CXPSPacketSink* CXPSPacketSink::New()
    {
    CXPSPacketSink* self = NULL;
    TRAP_IGNORE( self = new ( ELeave ) CXPSPacketSink )
    return self;
    }

CXPSPacketSink::~CXPSPacketSink()
    {
    }

TInt CXPSPacketSink::Init(const TDesC& ServerName, MXPSPktSinkObserver* pObserver)
    {
    return KErrNone;
    }

TInt CXPSPacketSink::AuthenticateClient(TSecurityPolicy& aSecurityPolicy)
    {
    return KErrNone;
    }

TInt CXPSPacketSink::SetSessionDescription(TDesC8& aSDPDesc, TUint unNumStreams)
    {
    return KErrNone;
    }
    
TInt CXPSPacketSink::Enqueue(TUint unStreamId, const TRtpRecvHeader& aHeaderInfo, const TDesC8& aPayloadData)
    {
    return KErrNone;
    }
    
TInt CXPSPacketSink::StreamEnd(TUint unStreamId)
    {
    return KErrNone;
    }

TInt CXPSPacketSink::ConfigStream(TUint unStreamId, TUint unNumSlots)
    {
    return KErrNone;
    }

TInt CXPSPacketSink::Reset()
    {
    return KErrNone;
    }
    
CXPSPacketSink::CXPSPacketSink()
    {
    }