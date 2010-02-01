/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Publish&Subscribe keys used by MCE Tester
*
*/



#ifndef MCETESTERPSKEYS_H
#define MCETESTERPSKEYS_H

// INCLUDES
#include <e32std.h>

/**
* UID for Publish&Subscribe keys used by MCE Tester
*/
const TUid KUidMceTesterPSKeys = { 0x102010D5 };

/**
* Defines resource reservation status returned by 
* NAT traversal plug-in stub implementing CNSPPlugin. 
* Parameter value is of type TInt.
*/
const TUint KMceTesterNatPluginResourceReservationStatus = 1;

/**
* Defines the asynchronous delay simulated by the 
* NAT traversal plug-in stub implementing CNSPPlugin. 
* Parameter value is of type TInt.
*/
const TUint KMceTesterNatPluginCallbackDelay 		= 2;

/**
* Defines the public address of the local endpoint.
* Parameter value is of type RProperty::TType::EText and its maximum length is
* KPropertyTextValueMaxLength.
* Example values: "1.2.3.4:5000", "[1:2:3::4:5]:6000".
*/
const TUint KMceTesterNatPluginLocalAddress 		= 3;

/**
* Defines the public address of the remote endpoint.
* Parameter value is of type RProperty::TType::EText and its maximum length is
* KPropertyTextValueMaxLength.
* Example values: "1.2.3.4:5000", "[1:2:3::4:5]:6000".
*/
const TUint KMceTesterNatPluginRemoteAddress 		= 4;

/**
* Defines the asynchronous delay between AnswerReady and OfferReady callbacks,
* simulated by the NAT traversal plug-in stub implementing CNSPPlugin.
* This delay is only used after CNSPPlugin::ResolveL has been called.
* Parameter value is of type TInt.
*/
const TUint KMceTesterNatPluginSecondCallbackDelay 	= 5;

/**
* Defines the error callback that the NAT traversal plug-in stub will call.
* Parameter value is of type RProperty::TType::EText and its maximum length is
* KPropertyTextValueMaxLength.
*
* Value begins with error type, either "icmp" or "error", followed by a space
* and the error code as a number.
* Error type "icmp" indicates an ICMP error, and "error" other error.
* Example values: "icmp -7206", "error 28".
*/
const TUint KMceTesterNatPluginErrorCallback		= 6;


const TInt KPropertyTextValueMaxLength = 60;


#endif // MCETESTERPSKEYS_H

// End of File
