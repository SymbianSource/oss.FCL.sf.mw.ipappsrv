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
* Description:
*
*/

#ifndef UT_NSPTLS_H
#define UT_NSPTLS_H

/**
 * Thread storage for NSP unit testing, storage for global data.
 * 
 */
class TNSPStorage
    {
public: // Enumerations
	
	/* NATFW actions */
	enum TNATFWAction
		{
		ENone = 0,
		ERegisterObserver,
		EUnRegisterObserver,
		ECreateSession,
		ELoadPlugin,
		ECreateStream,
		ECreateWrapper,
		EFetchCandidate,
		EFetchCandidates,
		ESetRole,
		ESetCredentials,
		EPerformConnectivityChecks,
		EUpdateIcePeers,
		EUpdateIceRemoteCands,
		ESetReceivingState,
		ESetSendingState,
		ECloseStream,
		ECloseSession
		};
    
public: // Constructors and destructors
	
	TNSPStorage();
    ~TNSPStorage();
    
public: // Data
	
	TInt iOOMService;
	TInt iLeaveService;
	TNATFWAction iLastAction;
	TUint iSessionId;
	TUint iStreamId;
	
    };

/**
 * TLS Initializer class, a static class.
 * 
 */
class NSPTls
    {
public:
	
	static void OpenL();
	static void Close();
	static TNSPStorage* Storage();
	
    };



#endif // UT_NSPTLS_H

// End of file
