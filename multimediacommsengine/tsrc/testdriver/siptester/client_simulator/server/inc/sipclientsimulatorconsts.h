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
* Description:  Common definitions for the client and server
*
*/

#ifndef SIPCLIENTSIMULATORCONSTS_H
#define SIPCLIENTSIMULATORCONSTS_H

// INCLUDES
#include <e32std.h>

/**
* Unique name for the server used as:
*/
_LIT( KSipClientSimulatorName, "sipclientsimulatorsrv" );

const TUid KSipClientSimulatorServerUid = { 0x101F5D37 };

/**
 * The server version. A version must be specifyed when creating
 * a session with the server. It is used to verify that the
 * client dll is compatible with the server. 
 * Doesn't matter much in this case.
 */
const TUint KSipClientSimulatorMajorVersion = 1;
const TUint KSipClientSimulatorMinorVersion = 0;
const TUint KSipClientSimulatorBuildVersion = 0;

/// Opcodes used in message passing between client and server.
/// Used to identify functions in the server sessions.
enum TSipClientSimulatorIpcFunctions
	{
    ESipClientSimulatorSetClientMatchType,
    ESipClientSimulatorGetClientMatchType,
    ESipClientSimulatorConnectClient,
    ESipClientSimulatorDisconnectClient,
    ESipClientSimulatorSetClientContentTypes,
    ESipClientSimulatorGetClientContentTypes,
    ESipClientSimulatorSetClientSdpMedias,
    ESipClientSimulatorGetClientSdpMedias
	};

/// IPC function arguments
enum TSipClientSimulatorIpcArgs
	{
    ESipClientSimulatorIpcArgClientMatchType = 0, // Type: TInt
    ESipClientSimulatorIpcArgClientUid = 1, // Type: TInt
    ESipClientSimulatorIpcArgContentTypes = 3, //Type: RPointerArray<CSIPContentTypeHeader>
	ESipClientSimulatorIpcArgSdpMediaFields = 3 //Type: RPointerArray<CSdpMediaField>
	};

/// The name and the value of the SIP-header 
/// that is added to all the generated SIP responses
_LIT8(KGeneratedSipHeaderName, "SIP-Client-Simulator");
_LIT8(KGeneratedSipHeaderValue, "Generated header");

#endif // SIPCLIENTSIMULATORCONSTS_H
