/*
* Copyright (c) 2004-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides G711 payload format spesific definitions.
*
*/





#ifndef G711PAYLOADFORMATDEFS_H
#define G711PAYLOADFORMATDEFS_H

//  INCLUDES
#include <e32base.h>

#ifdef _DEBUG 
    #include <e32svr.h>
    #define _DEBUG_G711_WRITE
    #define _DEBUG_G711_READ
#endif

// CONSTANTS
const TInt KG711MaxBitrate( 64000 );
const TInt KDefaultSampleRate = 8000;
const TInt KDefaultSampleRateInkHz = 8;

// MACROS
#ifdef _DEBUG_G711_WRITE
    #define DP_G711_WRITE(a) { _LIT( KName, a); RDebug::Print(KName); }
    #define DP_G711_WRITE2(a, b) \
    	{ _LIT(KName, a); RDebug::Print(KName, b); }
    #define DP_G711_WRITE3(a, b, c) \
 		{ _LIT(KName, a);  RDebug::Print(KName, b, c); }    
#else
    #define DP_G711_WRITE(a);
    #define DP_G711_WRITE2(a, b);
    #define DP_G711_WRITE3(a, b, c);
#endif

#ifdef _DEBUG_G711_READ
    #define DP_G711_READ(a) \
    	{ _LIT( KName, a); RDebug::Print(KName); }
  	#define DP_G711_READ2(a, b) \
  		{ _LIT(KName, a);  RDebug::Print(KName, b); }
    #define DP_G711_READ3(a, b, c) \
    	{ _LIT(KName, a);  RDebug::Print(KName, b, c); }
#else
    #define DP_G711_READ(a);
    #define DP_G711_READ2(a, b);
    #define DP_G711_READ3(a, b, c);
#endif

#endif      // G711PAYLOADFORMATDEFS_H
            
// End of File
