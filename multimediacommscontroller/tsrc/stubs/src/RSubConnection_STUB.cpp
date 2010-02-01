/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Downlink (playback) datapath
*
*/




// INCLUDE FILES
#include <es_sock.h>

EXPORT_C RSubConnection::RSubConnection()
    {
    }
    
EXPORT_C TInt RSubConnection::Open(RSocketServ& /*aServer*/, TSubConnType /*aSubConnType*/, RConnection& /*aConnection*/)
    {
    return KErrNone;
    }
    
EXPORT_C void RSubConnection::Close()
    {
    }

EXPORT_C void RSubConnection::Start(TRequestStatus& aStatus)
    {
    TRequestStatus* status = &aStatus;
    User::RequestComplete( status, KErrNone );
    }
    
EXPORT_C TInt RSubConnection::Start()
    {
    return KErrNone;
    }
    
EXPORT_C TInt RSubConnection::Stop()
    {
    return KErrNone;
    }

EXPORT_C void RSubConnection::Add(RSocket& /*aSocket*/, TRequestStatus& aStatus)
    {
    TRequestStatus* status = &aStatus;
    User::RequestComplete( status, KErrNone );
    }
    
EXPORT_C void RSubConnection::Remove(RSocket& /*aSocket*/, TRequestStatus& aStatus)
    {
    TRequestStatus* status = &aStatus;
    User::RequestComplete( status, KErrNone );
    }

EXPORT_C TInt RSubConnection::SetParameters(const RSubConParameterBundle& /*aParametersSet*/)
    {
    return KErrNone;
    }
    
EXPORT_C TInt RSubConnection::GetParameters(RSubConParameterBundle& /*aParametersSet*/)
    {
    return KErrNone;
    }

EXPORT_C void RSubConnection::EventNotification(TNotificationEventBuf& /*aEventBuffer*/, TBool /*aGenericEventsOnly*/, TRequestStatus& aStatus)
    {
    TRequestStatus* status = &aStatus;
    User::RequestComplete( status, KErrNone );
    }
    
EXPORT_C void RSubConnection::EventNotification(TNotificationEventBuf& /*aEventBuffer*/, TEventFilter /*aEventFilterList*/[], TUint /*aEventListLength*/, TRequestStatus& aStatus)
    {
    TRequestStatus* status = &aStatus;
    User::RequestComplete( status, KErrNone );
    }
    
EXPORT_C void RSubConnection::CancelEventNotification()
    {
    }

EXPORT_C TInt RSubConnection::Control(TUint /*aOptionLevel*/, TUint /*aOptionName*/, TDes8& /*aOption*/)
    {
    return KErrNone;
    }

EXPORT_C TInt RSubConnection::Open(RSocketServ& /*aServer*/, TSubConnOpen::TSubConnType /*aSubConnType*/, RConnection& /*aConnection*/)
    {
    return KErrNone;
    }
	
//  End of File  
