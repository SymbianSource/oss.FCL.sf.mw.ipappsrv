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
* Description:  Implementation
*
*/

#include "TTcSerialFactory.h"
#include "CTcSerialManager.h"
#include "ErrorHandling.h"

EXPORT_C TTcSerialFactory::TTcSerialFactory( 
    const TDesC& aCSYName, 
    const TDesC& aPortName, 
    TUint aPortSpeed )
	{
	__ASSERT_ALWAYS( aCSYName.Length() <= KMaxCSYNameLen, Panic( KErrArgument ) );
	__ASSERT_ALWAYS( aPortName.Length() <= KMaxPortNameLen, Panic( KErrArgument ) );
	
	iCSYName.Copy( aCSYName );
	iPortName.Copy( aPortName );
	iPortSpeed = aPortSpeed;
	}

EXPORT_C TTcSerialFactory::~TTcSerialFactory()
	{
	}

MTcBearerManager* TTcSerialFactory::CreateManagerL() const
	{
	return CTcSerialManager::NewL( *this );
	}
