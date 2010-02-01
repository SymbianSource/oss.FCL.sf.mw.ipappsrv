/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    The security policy for MCE Server
*
*/




#ifndef TMCECSPLATSECPOLICY_H
#define TMCECSPLATSECPOLICY_H

/**
 * Number of different IPC ranges 
 */
const TUint KMceCSPlatSecRangeCount = 13;


/**
 * IPC ranges segmented into groups based on their capability requirements
 */
const TInt TMceCSPlatSecRanges[KMceCSPlatSecRangeCount] =
	{
	0,  // IPC message 0
	1,	// IPC message 1 - 14
	15, // IPC message 15 - 20
	21, // IPC message 21
	22, // IPC message 22 - 24
	25, // IPC message 25 - 32
	33, // IPC message 33
	34,	// IPC message 34 - 35
	36, // IPC message 36
	37, // IPC message 37 - 41
	42, // IPC message 42 - 49
    50, // IPC messages 50 - 150
	150 // Not Supported 150 -> 	
	};

/**
 * IPC segments and their capability requirements
 */
const TUint8 TMceCSPlatSecElementsIndex[KMceCSPlatSecRangeCount] =
	{
	CPolicyServer::EAlwaysPass,// Security policy for IPC 0
	0,						   // Security policy for IPC message 1 - 14 (NetworkServices)
	CPolicyServer::EAlwaysPass,// Security policy for IPC message 15 - 20 (None)
	0,						   // Security policy for IPC message 21 (NetworkServices)
	CPolicyServer::EAlwaysPass,// Security policy for IPC message 22 - 24 (None)
	0,						   // Security policy for IPC message 25 - 32 (NetworkServices)
	1, 						   // Security policy for IPC message 33 (WriteDeviceData)
	CPolicyServer::EAlwaysPass,// Security policy for IPC message 34 - 35 (None)	
	1,						   // Security policy for IPC message 36 (WriteDeviceData)
	CPolicyServer::EAlwaysPass,// Security policy for IPC message 37 - 41 (None)
	2,                         // Security policy for IPC message 42 - 49 (MultimediaDD)
	CPolicyServer::EAlwaysPass,// Security policy for IPC message 50 - 150 (None) 
	CPolicyServer::ENotSupported // Not Supported  ->
	};
	
/**
 * Capability sets and the action performed in case of failure
 */	
const CPolicyServer::TPolicyElement TMceCSPlatSecElements[3] =
	{
	// Requires the user to have NetworkServices capability, otherwise the message is 
	// completed with KErrPermissionDenied
	{ _INIT_SECURITY_POLICY_C1(ECapabilityNetworkServices), CPolicyServer::EFailClient },
	
	// Requires the user to have WriteDeviceData capability, otherwise the message is 
	// completed with KErrPermissionDenied
	{ _INIT_SECURITY_POLICY_C1(ECapabilityWriteDeviceData), CPolicyServer::EFailClient },
	
	
	// Requires the user to have MultimediaDD capability, otherwise the message is 
	// completed with KErrPermissionDenied
	{ _INIT_SECURITY_POLICY_C1(ECapabilityMultimediaDD), CPolicyServer::EFailClient }	
	
	};
	
/**
 * Platform security policy that MCE Server uses. 
 */	
const CPolicyServer::TPolicy TMceCSPlatSecPolicy =
	{
	CPolicyServer::EAlwaysPass, 	// Specifies all connect attempts should pass
	KMceCSPlatSecRangeCount,		// Number of ITC ranges
	TMceCSPlatSecRanges,			// ITC Ranges
	TMceCSPlatSecElementsIndex,		// Policy to ITC mapping array
	TMceCSPlatSecElements			// Policies used in MCE Server
	};
						
#endif//TMCECSPLATSECPOLICY_H
