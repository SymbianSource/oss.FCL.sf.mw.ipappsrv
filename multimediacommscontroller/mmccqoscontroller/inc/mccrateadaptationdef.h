/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Mcc Internal definitions
*
*/




#ifndef MCCRATEADAPTATIONDEF_H
#define MCCRATEADAPTATIONDEF_H

// INCLUDES
#include <utf.h>
#include <e32debug.h>
#include "rtpdef.h"

// CONSTANTS
const TInt KRoundTripTimeLimit = 4000;

const TReal KAdaptation = 0.2;      //rate control
const TReal KAdaptationMore = 0.3;  //rate control

const TInt KMccShortTermCalculation = 5;
const TInt KMccLongTermCalculation = 15;

#define RATECONTROL
//#define FRAMERATECONTROL //enables framerate change in RateControl
#ifdef FRAMERATECONTROL
const TInt KIPULFrameRate10Limit    = 50000;
const TInt KIPULFrameRate5Limit     = 32000;
#endif //FRAMERATECONTROL

// rate control thresholds
class TThresholdValues
    {
public:
    TInt iFractionLostHigh;
    TInt iFractionLostLow;
    TInt iBufferCountHigh;
    TInt iBufferCountLow;
    TInt iBandwidthHigh;
    TInt iBandwidthLow;
    };

const TInt KMccFirstIcmpError       = 1;

// Cisco IOS default for ICMP report generation is once per 500ms. Threshold
// parameters are adjusted according to that default because better statistics
// is not available. Rate-limits for ICMP generation varies according to
// the vendor and media errors cannot be detected too reliably by observing 
// ICMPs.
const TInt KMccIcmpErrorThreshold   = 9;
const TInt64 KMccIcmpErrorTimeWindow= 10000000; // 10 seconds

// DATA TYPES

#endif
