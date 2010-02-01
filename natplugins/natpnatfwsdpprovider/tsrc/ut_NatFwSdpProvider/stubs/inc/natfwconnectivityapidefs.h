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
* Description:  General definitions for NAT connectivity framework
*
*/


#ifndef NATCONNECTIVITYFW_HRH
#define NATCONNECTIVITYFW_HRH

/** Defines in which direction TCP connection will be established. */
enum TNATFWTcpConnectionSetup
    {
    /** Uninitialized or some other error. */
    ETcpSetupUnknown            = 0,
    
    /** The local endpoint will initiate an outgoing connection. */
    ETcpSetupActive             = 1,
    
    /** The local endpoint will accept an incoming connection. */
    ETcpSetupPassive            = 2
    };

/** Defines streaming state for the NATFW stream. */
enum TNATFWStreamingState
    {
    /** Uninitialized or some other error. */
    EStreamingStateUnknown      = 0,
    
    /** Streaming enabled. */
    EStreamingStateActive       = 1,
    
    /** Streaming disabled. */
    EStreamingStatePassive      = 2
    };

/** Defines possible roles for ICE. */
enum TNATFWIceRole
    {
    /** Uninitialized or some other error. */
    EIceRoleUnknown             = 0,
    
    /** Local endpoint will act as a controlling agent. */
    EIceRoleControlling         = 1,
    
    /** Local endpoint will act as a controlled agent. */
    EIceRoleControlled          = 2
    };

#endif // NATCONNECTIVITYFW_HRH
