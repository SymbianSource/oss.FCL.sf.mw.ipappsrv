/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides DTMF payload format spesific definitions.
*
*/




#ifndef DTMFPAYLOADFORMATDEFS_H
#define DTMFPAYLOADFORMATDEFS_H

//  INCLUDES
#include <e32base.h>

#ifdef _DEBUG 
    #include <e32svr.h>
    #define _DEBUG_DTMF_WRITE
    #define _DEBUG_DTMF_READ
    #define _DEBUG_DTMF_STIMER
    #define _DEBUG_DTMF_DECODE
    #define _DEBUG_DTMF_ENCODE
#endif

// CONSTANTS
// Redundant events handled, RFC 2833 recommendation
const TInt KDTMFDefaultRedundancyCount( 5 );
const TUint KMaxDtmfRedCount( 5 );

// Redundancy header = 4 bytes, DTMF event size = 4 bytes
const TUint KDTMFDefaultPayloadSize( KMaxDtmfRedCount * 4 * 2 + 1 );

// Maximum is -63 dBm0, preferred -8 to -3 RFC 2833bis6 4.3.3
const TUint KDTMFDefaultToneVolume( 4 );
const TUint KDTMFDefaultModulation( 0 );

const TUint KDTMFLowGroupFrequencies[] = 
    {
    941/*0*/, 697, 697, 697, 770/*4*/, 770, 770, 852/*7*/, 852, 852, 941/***/,
    941/*#*/, 697/*A*/, 770/*B*/, 852/*C*/, 941/*D*/
    };
    
const TUint KDTMFHighGroupFrequencies[] =
    {
    1336/*0*/, 1209, 1336, 1477, 1209/*4*/, 1336, 1477, 1209/*7*/, 1336, 1477,
    1209/***/, 1477/*#*/, 1633/*A*/, 1633/*B*/, 1633/*C*/, 1633/*D*/
    };
    
const TInt KDtmfGroupFrequencyCount = 16;

// Local/inband DTMF tone playing parameters
const TUint KDTMFDefToneOnLengthInUs = 300000;
const TUint KDTMFDefToneOffLengthInUs = 300000;
const TUint KDTMFDefPauseLengthInUs = 2000000;
const TUint KDTMFDefUpdateIntervalInUs = 50000;

// Limit for local playback of tone to avoid getting stucked tones
// (RFC2833bis12 2.5.2.2)
const TUint KDTMFMaxTonePlaybackTime = KDTMFDefToneOnLengthInUs * 3;
const TUint KDTMFMaxDurationFieldValue = 0xFFFF;

// Number of bits used for value(s) saving in payload header
const TInt KEventBlockLengthInBytes = 4;
const TInt KEventFieldBits = 8;
const TInt KVolFieldBits = 6;
const TInt KDurationFieldBits = 16;
const TInt KBlockPTBits = 7;
const TInt KModulationBits = 9;
const TInt KFrequencyBits = 12;
const TInt KTimeStampOffsetBits = 14;
const TInt KBlockLengthBits = 10;

// Event codes for DTMFs
const TInt KEventCodeForAsterisk = 10;
const TInt KEventCodeForHashMark = 11;
const TInt KEventCodeForA = 12;
const TInt KEventCodeForB = 13;
const TInt KEventCodeForC = 14;
const TInt KEventCodeForD = 15;

// MACROS
#ifdef _DEBUG_DTMF_WRITE
    #define DP_DTMF_WRITE(a) RDebug::Print(a);
    #define DP_DTMF_WRITE2(a, b) RDebug::Print(a, b);
    #define DP_DTMF_WRITE3(a, b, c) RDebug::Print(a, b, c);
    #define DP_DTMF_WRITE4(a, b, c, d) RDebug::Print(a, b, c, d);
#else
    #define DP_DTMF_WRITE(a);
    #define DP_DTMF_WRITE2(a, b);
    #define DP_DTMF_WRITE3(a, b, c);
    #define DP_DTMF_WRITE4(a, b, c, d);
#endif

#ifdef _DEBUG_DTMF_READ
    #define DP_DTMF_READ(a) RDebug::Print(a);
    #define DP_DTMF_READ2(a, b) RDebug::Print(a, b);
    #define DP_DTMF_READ3(a, b, c) RDebug::Print(a, b, c);
#else
    #define DP_DTMF_READ(a);
    #define DP_DTMF_READ2(a, b);
    #define DP_DTMF_READ3(a, b, c);
#endif

#ifdef _DEBUG_DTMF_STIMER
    #define DP_DTMF_STIMER(a) RDebug::Print(a);
    #define DP_DTMF_STIMER2(a, b) RDebug::Print(a, b);
    #define DP_DTMF_STIMER3(a, b, c) RDebug::Print(a, b, c);
#else
    #define DP_DTMF_STIMER(a);
    #define DP_DTMF_STIMER2(a, b);
    #define DP_DTMF_STIMER3(a, b, c);
#endif

#ifdef _DEBUG_DTMF_DECODE
    #define DP_DTMF_DECODE(a) RDebug::Print(a);
    #define DP_DTMF_DECODE2(a, b) RDebug::Print(a, b);
    #define DP_DTMF_DECODE3(a, b, c) RDebug::Print(a, b, c);
    #define DP_DTMF_DECODE5(a, b, c, d, e) RDebug::Print(a, b, c, d, e);
#else
    #define DP_DTMF_DECODE(a);
    #define DP_DTMF_DECODE2(a, b);
    #define DP_DTMF_DECODE3(a, b, c);
    #define DP_DTMF_DECODE5(a, b, c, d, e);
#endif

#ifdef _DEBUG_DTMF_ENCODE

#endif


// DATA TYPES
enum TDTMFPayloadFormat
    {
    EDTMFPayloadFormatNotDefined,
    EDTMFPayloadFormatEvent,
    EDTMFPayloadFormatTone,
    EDTMFPayloadFormatRedEvents
    };

#endif      // DTMFPAYLOADFORMATDEFS_H
      
// End of File
