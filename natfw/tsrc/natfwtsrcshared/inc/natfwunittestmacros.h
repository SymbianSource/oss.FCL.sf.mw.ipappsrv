/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef __NATFWUNITTESTMACROS_H__
#define __NATFWUNITTESTMACROS_H__

#include <digia/eunit/eunitmacros.h>

#define NATFW_EUNIT_ASSERT_EQUALS( func, val ) \
{\
TInt natfwUnitTestRetVal = func;\
if ( natfwUnitTestRetVal != KErrNoMemory ){\
	EUNIT_ASSERT_EQUALS( natfwUnitTestRetVal, val );\
	}\
else{\
	User::Leave( KErrNoMemory );\
	}\
}

#define NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE( func, val ) \
{\
TRAPD( natfwUnitTestRetVal, func );\
if ( val != KErrNoMemory && natfwUnitTestRetVal == KErrNoMemory ){\
	User::Leave( KErrNoMemory );\
	}\
else{\
	EUNIT_ASSERT_EQUALS( natfwUnitTestRetVal, val );\
	}\
}

#define NATFW_EUNIT_ASSERT_NO_LEAVE( func ) NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE( func, KErrNone )


#endif