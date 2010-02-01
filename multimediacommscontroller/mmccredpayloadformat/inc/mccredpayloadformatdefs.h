/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides Red payload format spesific definitions
*
*/



#ifndef MCCREDPAYLOADFORMATDEFS_H
#define MCCREDPAYLOADFORMATDEFS_H

//  INCLUDES
#include <e32base.h>
#ifdef _DEBUG 
    #include <e32svr.h>
    #define _DEBUG_RED_DECODE
    #define _DEBUG_RED_ENCODE 
#endif

// CONSTANTS
const TInt KMaxRedCount             = 1;
const TInt KRedHeaderSize           = 4;
const TInt KFinalHeaderSize         = 1;
const TInt KDefRedCount             = 1;

const TInt KPTFieldBits             = 7;
const TInt KTSOffsetBits            = 14;
const TInt KBlockLengthBits         = 10;
const TInt KFBitLength              = 1;

// MACROS
#ifdef _DEBUG_RED_DECODE
    #define DP_RED_DECODE(a) { _LIT(KName, a); RDebug::Print(KName); }
    #define DP_RED_DECODE2(a, b) { _LIT(KName, a); RDebug::Print(KName, b); }
    #define DP_RED_DECODE3(a, b, c) \
        { _LIT(KName, a); RDebug::Print(KName, b, c); }
    #define DP_RED_DECODE4(a, b, c, d) \
        { _LIT(KName, a); RDebug::Print(KName, b, c, d); }
    #define DP_RED_DECODE5(a, b, c, d, e) \
        { _LIT(KName, a); RDebug::Print(KName, b, c, d, e); }
    #define DP_RED_DECODE6(a, b, c, d, e, f) \
        { _LIT(KName, a); RDebug::Print(KName, b, c, d, e, f); }
#else
    #define DP_RED_DECODE(a);
    #define DP_RED_DECODE2(a, b);
    #define DP_RED_DECODE3(a, b, c);
    #define DP_RED_DECODE4(a, b, c, d);
    #define DP_RED_DECODE5(a, b, c, d, e);
    #define DP_RED_DECODE6(a, b, c, d, e, f);
#endif

#ifdef _DEBUG_RED_ENCODE
    #define DP_RED_ENCODE(a) { _LIT(KName, a); RDebug::Print(KName); }
    #define DP_RED_ENCODE2(a, b) { _LIT(KName, a); RDebug::Print(KName, b); }
    #define DP_RED_ENCODE3(a, b, c) \
        { _LIT(KName, a); RDebug::Print(KName, b, c); }
    #define DP_RED_ENCODE4(a, b, c, d) \
        { _LIT(KName, a); RDebug::Print(KName, b, c, d); }
    #define DP_RED_ENCODE5(a, b, c, d, e) \
        { _LIT(KName, a); RDebug::Print(KName, b, c, d, e); }
    #define DP_RED_ENCODE6(a, b, c, d, e, f) \
        { _LIT(KName, a); RDebug::Print(KName, b, c, d, e, f); }  
#else
    #define DP_RED_ENCODE(a);
    #define DP_RED_ENCODE2(a, b);
    #define DP_RED_ENCODE3(a, b, c);
    #define DP_RED_ENCODE4(a, b, c, d);
    #define DP_RED_ENCODE5(a, b, c, d, e);
    #define DP_RED_ENCODE6(a, b, c, d, e, f);
#endif

#endif      // MCCREDPAYLOADFORMATDEFS_H
            
// End of File
