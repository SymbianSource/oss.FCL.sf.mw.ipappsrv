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
* Description:  Implementation
*
*/

#include "TTcTcpFactory.h"
#include "CTcTcpManager.h"

EXPORT_C TTcTcpFactory::TTcTcpFactory( TInt aPort, TInt aIAPId )
	: iPort( aPort ), iIAPId( aIAPId )
	{
	}

EXPORT_C TTcTcpFactory::~TTcTcpFactory()
	{
	}

MTcBearerManager* TTcTcpFactory::CreateManagerL() const
	{
	return CTcTcpManager::NewL( *this );
	}
