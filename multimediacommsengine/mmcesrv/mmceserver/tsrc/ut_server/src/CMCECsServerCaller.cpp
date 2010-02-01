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


#include "CMceCsServerCaller.h"
#include "mceserverstarter.h"

#include <f32file.h>


CMceCsServerCaller::CMceCsServerCaller()
 : RSessionBase ()
	{
	}
	
CMceCsServerCaller::~CMceCsServerCaller()
	{
	}

CMceCsServerCaller* CMceCsServerCaller::NewL()
	{	
	CMceCsServerCaller* self = new (ELeave) CMceCsServerCaller();
	CleanupStack::PushL (self);	
	self->ConstructL ();
	CleanupStack::Pop ();  // self
	return self;
	}
void CMceCsServerCaller::ConstructL()
	{
	}

	
#ifndef EKA2
// Starting point of the DLL (Removed in EKA2) 
GLDEF_C TInt E32Dll(TDllReason)
	{
	return(KErrNone);
	}
#endif

void CMceCsServerCaller::ConnectL()
	{
	    TVersion version (KMceServerMajorVersionNumber,
                      KMceServerMinorVersionNumber,
                      KMceServerBuildVersionNumber);
			
		User::LeaveIfError(MceServerStarter::Start());
		User::LeaveIfError(CreateSession (KMceServerName,version));
	}

TInt CMceCsServerCaller::Send(TMceItcFunctions aFunction)
	{
	TIpcArgs aArgs;
		return SendReceive (aFunction, aArgs);
	}