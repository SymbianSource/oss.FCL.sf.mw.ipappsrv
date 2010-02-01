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

#include "TTcIdentifier.h"

EXPORT_C TTcIdentifier::TTcIdentifier()
	: iContainer( KNullDesC8 ), iFunction( KNullDesC8 )
	{
	}

EXPORT_C TTcIdentifier::TTcIdentifier( const TDesC8& aFunction )
	: iContainer( KNullDesC8 ), iFunction( aFunction )
	{
	}

EXPORT_C TTcIdentifier::TTcIdentifier( const TDesC8& aContainer,
									   const TDesC8& aFunction )
	: iContainer( aContainer ), iFunction( aFunction )
	{
	}

EXPORT_C TTcIdentifier::~TTcIdentifier()
	{
	}
