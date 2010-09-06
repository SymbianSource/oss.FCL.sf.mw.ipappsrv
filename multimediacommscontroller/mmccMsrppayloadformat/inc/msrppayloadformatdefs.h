/*
* Copyright (c) 2004-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides a comfort noise generator class
*
*/


#ifndef MSRPPAYLOADFORMATDEFS_H
#define MSRPPAYLOADFORMATDEFS_H

//  INCLUDES
#include <e32base.h>

#ifdef _DEBUG
    #include <e32svr.h>
    #define DEBUG_MSRP_WRITE
#endif

// CONSTANTS

// MACROS
#ifdef DEBUG_MSRP_WRITE

    #define DP_MSRP_WRITE(a) { _LIT( KName, a); RDebug::Print(KName); }
    #define DP_MSRP_WRITE2(a, b) \
    	{ _LIT(KName, a); RDebug::Print(KName, b); }
    #define DP_MSRP_WRITE3(a, b, c) \
 		{ _LIT(KName, a);  RDebug::Print(KName, b, c); } 
 		
 	#define DP_MSRP_WRITE_INT(a) \
 	{ RDebug::Print(_L("Value - VAL: %d"), a);  }
 	 		
#else
    #define DP_MSRP_WRITE(a);
    #define DP_MSRP_WRITE2(a, b);
    #define DP_MSRP_WRITE3(a, b, c);
    #define DP_MSRP_WRITE_INT(a);
#endif

#endif      // MSRPPAYLOADFORMATDEFS_H
            
// End of File
