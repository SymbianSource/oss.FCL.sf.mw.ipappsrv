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
* Description:    Defines debug assert macroes for Connection Multiplexer
*                module
*
*/




#ifndef NCMCONNECTIONMULTIPLEXERASSERT_H
#define NCMCONNECTIONMULTIPLEXERASSERT_H

#include <e32std.h>

#if defined( _DEBUG ) && !defined( TEST_EUNIT )

//Panic is used in debug builds, so the exact location where the assertion
//failed is caught early in testing.
#define __CONNECTIONMULTIPLEXER_PANIC( i )    \
User::Panic( KNullDesC, ( i ) )

#define __CONNECTIONMULTIPLEXER_ASSERT_L( c, i ) \
__ASSERT_ALWAYS( c, __CONNECTIONMULTIPLEXER_PANIC( i ) )

#define __CONNECTIONMULTIPLEXER_ASSERT_RETURN( c, i ) \
__ASSERT_ALWAYS( c, __CONNECTIONMULTIPLEXER_PANIC( i ) )

#define __CONNECTIONMULTIPLEXER_ASSERT_RETURN_VALUE( c, i ) \
__ASSERT_ALWAYS( c, __CONNECTIONMULTIPLEXER_PANIC( i ) )

#define __CONNECTIONMULTIPLEXER_FAILURE_L( i )    \
__CONNECTIONMULTIPLEXER_PANIC( i )
#define __CONNECTIONMULTIPLEXER_FAILURE_RETURN( i ) \
__CONNECTIONMULTIPLEXER_PANIC( i )

#else

//For release builds, leave is used instead of panic.
#define __CONNECTIONMULTIPLEXER_ASSERT_L( c, i ) \
__ASSERT_ALWAYS( c, User::Leave( i ) )
#define __CONNECTIONMULTIPLEXER_ASSERT_RETURN( c, i ) \
if ( !( c ) ) return
#define __CONNECTIONMULTIPLEXER_ASSERT_RETURN_VALUE( c, i ) \
if ( !( c ) ) return ( i )

#define __CONNECTIONMULTIPLEXER_FAILURE_L( i ) \
User::Leave( i )
#define __CONNECTIONMULTIPLEXER_FAILURE_RETURN( i ) return

#endif


#endif // NCMCONNECTIONMULTIPLEXERASSERT_H
