/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides general definitions for Mcc rtpsourcesink
*
*/




#ifndef MCCRTPDEFS_H
#define MCCRTPDEFS_H

// INCLUDES
#include <e32base.h>

// CONSTANTS
const TUint32 KRtpStandByTimer = 30000000; // 30 seconds 

// MACROS

#ifdef _DEBUG
#include <e32svr.h>
#define TRACE_RTCP_RECEIVER
#define TRACE_RTP_SINK
#define TRACE_RTP_SOURCE
#define TRACE_RTP_INTERFACE
#define TRACE_RTP_CRYPTO_ADAPTER
#endif

#ifdef  TRACE_RTP_CRYPTO_ADAPTER
#define TRACE_RTP_CRYPTO_ADAPTER_PRINT(a) { _LIT( KName, a); RDebug::Print(KName); }
#define TRACE_RTP_CRYPTO_ADAPTER_PRINT2(a, b) { _LIT( KName, a); RDebug::Print(KName, b); }
#else
#define TRACE_RTP_CRYPTO_ADAPTER_PRINT(a)
#define TRACE_RTP_CRYPTO_ADAPTER_PRINT2(a, b)
#endif

#ifdef  TRACE_RTCP_RECEIVER
#define TRACE_RTCP_RECEIVER_PRINT(a) { _LIT( KName, a); RDebug::Print(KName); }
#define TRACE_RTCP_RECEIVER_PRINT2(a, b) { _LIT( KName, a); RDebug::Print(KName, b); }
#define TRACE_RTCP_RECEIVER_PRINT3(a, b, c) { _LIT( KName, a); RDebug::Print(KName, b, c); }
#else
#define TRACE_RTCP_RECEIVER_PRINT(a)
#define TRACE_RTCP_RECEIVER_PRINT2(a, b)
#define TRACE_RTCP_RECEIVER_PRINT3(a, b, c)
#endif

#ifdef  TRACE_RTP_SINK
#define TRACE_RTP_SINK_PRINT(a) { _LIT( KName, a); RDebug::Print(KName); }
#define TRACE_RTP_SINK_PRINT2(a, b) { _LIT( KName, a); RDebug::Print(KName, b); }
#define TRACE_RTP_SINK_PRINT3(a, b, c) { _LIT( KName, a); RDebug::Print(KName, b, c); }
#else
#define TRACE_RTP_SINK_PRINT(a)
#define TRACE_RTP_SINK_PRINT2(a, b)
#define TRACE_RTP_SINK_PRINT3(a, b, c)
#endif

#ifdef  TRACE_RTP_SOURCE
#define TRACE_RTP_SOURCE_PRINT(a) { _LIT( KName, a); RDebug::Print(KName); }
#define TRACE_RTP_SOURCE_PRINT2(a, b) { _LIT( KName, a); RDebug::Print(KName, b); }
#define TRACE_RTP_SOURCE_PRINT3(a, b, c) { _LIT( KName, a); RDebug::Print(KName, b, c); }
#else
#define TRACE_RTP_SOURCE_PRINT(a)
#define TRACE_RTP_SOURCE_PRINT2(a, b)
#define TRACE_RTP_SOURCE_PRINT3(a, b, c)
#endif

#ifdef  TRACE_RTP_INTERFACE
#define TRACE_RTP_INTERFACE_PRINT(a) { _LIT( KName, a); RDebug::Print(KName); }
#define TRACE_RTP_INTERFACE_PRINT2(a, b) { _LIT( KName, a); RDebug::Print(KName, b); }
#define TRACE_RTP_INTERFACE_PRINT3(a, b, c) { _LIT( KName, a); RDebug::Print(KName, b, c); }
#else
#define TRACE_RTP_INTERFACE_PRINT(a)
#define TRACE_RTP_INTERFACE_PRINT2(a, b)
#define TRACE_RTP_INTERFACE_PRINT3(a, b, c)
#endif

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

#endif      // MCCRTPDEFS_H
            
// End of File
