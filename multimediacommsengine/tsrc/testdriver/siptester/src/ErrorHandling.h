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
* Description:  Definition of Panic()
*
*/

#ifndef __ERRORHANDLING_H__
#define __ERRORHANDLING_H__

// INCLUDES
#include <e32std.h>

/**
 * Calls User::Panic() with the specified error code and category "SIPTester"
 *
 * @param aCode Error code
 */
void Panic( TInt aCode );

#endif	// __ERRORHANDLING_H__
