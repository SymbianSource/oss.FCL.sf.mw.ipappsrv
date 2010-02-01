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
* Description:    ECom 8.x compatibility definition
*
*/



#ifndef __ECOMIMPLEMENTATIONPROXY_H__
#define __ECOMIMPLEMENTATIONPROXY_H__

// INCLUDES
#include <ecom/ImplementationProxy.h>

// Disabled PC-Lint warning for "suspicious typecast" caused by Symbian's
// ECom declarations
/*lint -e611 */

// 8.x ECOM compatibility - EABI requires the use of IMPLEMENTATION_PROXY_ENTRY
// (i.e. define IMPLEMENTATION_PROXY_ENTRY locally on pre 8.x systems)
#ifndef IMPLEMENTATION_PROXY_ENTRY
#define IMPLEMENTATION_PROXY_ENTRY( aUid, aFuncPtr ) { { aUid }, aFuncPtr }
#endif

#endif // __ECOMIMPLEMENTATIONPROXY_H__
