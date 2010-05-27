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
* Description:    
*
*/




#ifndef WSSTUBSHELPER_H
#define WSSTUBSHELPER_H

//  INCLUDES
#include <w32std.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*
*/
class TWsStubsHelper
    {
public:
    static CGraphicsContext::TDrawMode CurrentDrawMode();   
    static TInt NumWindowsCreated();
    static void Reset();

    };

#endif      // WSSTUBSHELPER_H   
            
// End of File
