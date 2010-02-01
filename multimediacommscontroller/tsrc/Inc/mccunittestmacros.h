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



#ifndef __MCCUNITTESTMACROS_H__
#define __MCCUNITTESTMACROS_H__

#include <digia/eunit/eunitmacros.h>

#define MCC_EUNIT_ASSERT_EQUALS( func, val ) \
{\
TInt mccUnitTestRetVal = func;\
if ( mccUnitTestRetVal != KErrNoMemory ){\
	EUNIT_ASSERT_EQUALS( mccUnitTestRetVal, val );\
	}\
else{\
	User::Leave( KErrNoMemory );\
	}\
}

#define MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( func, val ) \
{\
TRAPD( mccUnitTestRetVal, func );\
if ( val != KErrNoMemory && mccUnitTestRetVal == KErrNoMemory ){\
	User::Leave( KErrNoMemory );\
	}\
else{\
	EUNIT_ASSERT_EQUALS( mccUnitTestRetVal, val );\
	}\
}

#define MCC_EUNIT_ASSERT_NO_LEAVE( func ) MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( func, KErrNone )


#endif