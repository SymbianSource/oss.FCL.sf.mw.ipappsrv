/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef NATTRAVERSALLOG_H
#define NATTRAVERSALLOG_H

#include <e32std.h>

#ifdef _DEBUG
    // Comment the following line to turn off all kind of logging:
    #define USE_NATTRAVERSAL_LOGS
#endif


#ifdef USE_NATTRAVERSAL_LOGS

    #include <in_sock.h>

    class NATTraversalLog
        {
        public:

            inline static void Print(const TDesC16& aStr);

            inline static void Print(const TDesC16& aStr1,
                                     const TDesC8& aStr2);

            inline static void Print(const TDesC16& aStr,
                                     TInt aValue);

            inline static void Print(const TDesC16& aStr,
                                     const TInetAddr& aAddr);
        };

    #include "nattraversallog.inl"

    #define NATTRAVERSAL_LOG( a ) NATTraversalLog::Print( _L( a ) );
    #define NATTRAVERSAL_INTLOG( a, b ) NATTraversalLog::Print( _L( a ), b );
    #define NATTRAVERSAL_STRLOG( a, b ) NATTraversalLog::Print( _L( a ), b );
    #define NATTRAVERSAL_ADDRLOG( a, b ) NATTraversalLog::Print( _L( a ), b );
    
#else // USE_NATTRAVERSAL_LOGS

    #define NATTRAVERSAL_LOG( a )
    #define NATTRAVERSAL_INTLOG( a, b )
    #define NATTRAVERSAL_STRLOG( a, b )
    #define NATTRAVERSAL_ADDRLOG( a, b )

#endif // USE_NATTRAVERSAL_LOGS


#endif // NATTRAVERSALLOG_H


