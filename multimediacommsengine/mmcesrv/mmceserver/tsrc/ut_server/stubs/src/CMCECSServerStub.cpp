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





#include "mceclientserver.h"
#include "mcecsserver.h"
#include "mcecssession.h"
#include "mcecsplatsecpolicy.h"


CMceCsServer* CMceCsServer::NewL (CMceServerCore& aServerCore)
	{
    CMceCsServer* self = CMceCsServer::NewLC(aServerCore);
    CleanupStack::Pop(self);
    return self;
	}


CMceCsServer* CMceCsServer::NewLC (CMceServerCore& aServerCore)
	{
	CMceCsServer* self = new(ELeave)CMceCsServer(EPriorityHigh,aServerCore);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
	}


void CMceCsServer::ConstructL ()
	{
	}


CMceCsServer::CMceCsServer (TInt aPriority, CMceServerCore& aServerCore)
 : CPolicyServer(aPriority, TMceCSPlatSecPolicy),
   iServerCore(aServerCore)
	{
	}


CMceCsServer::~CMceCsServer ()
	{
	}


CSession2* CMceCsServer::NewSessionL (const TVersion& /*aVersion*/,
                                      const RMessage2& /*aMessage*/) const
	{
	return CMceCsSession::NewL( iServerCore );
	}



TInt CMceCsServer::RunError(TInt aError)
	{
	return aError;
	}
