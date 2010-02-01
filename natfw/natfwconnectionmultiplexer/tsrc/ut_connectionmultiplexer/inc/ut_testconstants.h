/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Destination for multiplexer by using sockets
*
*/




#ifndef __UT_CONNECTIONMULTIPLEXER_H__
#define __UT_CONNECTIONMULTIPLEXER_H__

#include "in_sock.h"
#include "natfwconnectivityapidefs.h"


const TUint KStartPort = 5000;
const TUint KEndPort = 5020;
const TUint KPort = 5040;

const TUint KIapId = 10;
const TInt KQos = 46;
 
const TUint KTUintMinValue = 0;
const TUint KTUintMaxValue = 4294967295;

const TUint KStreamIdMin = KTUintMinValue;
const TUint KStreamIdMax = KTUintMaxValue;

const TUint KInterval = 100000; // 100ms

const TUint KConnectionId = 10;
const TUint KStreamId = 5;


_LIT8(KMessage, "Hello world");
/*
_LIT8(KReceiveMessage, "Received_Message" );
_LIT8(KSendMessage,"Send_Message");
_LIT8(KErrorMessage, "Error_message");
_LIT8(KTestPanic,"TEST");
*/
#endif //__UT_CONNECTIONMULTIPLEXER_H__