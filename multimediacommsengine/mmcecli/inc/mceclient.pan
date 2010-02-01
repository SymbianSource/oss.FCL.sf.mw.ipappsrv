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
* Description:  
*
*/




#ifndef MCECLIENT_PAN
#define MCECLIENT_PAN

#include <e32std.h>

/**
 * MCE Client panics
 */

/**
 * Constant literal description
 */

_LIT(KMceClientPanic, "MceClient");
_LIT(KMceServerPanic, "MceServer");


/** MCE application panic codes */
enum TMceClientPanics 
    {
	/** The user has not successfully connected to sip C/S-server */
    EMceClientNotConnected,
	/** The user has not cancelled an outstanding receive before closing */
    EMceReceiveNotCancelled
    // add further panics here
    };

#endif // MCECLIENT_PAN
