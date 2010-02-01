/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  NAT FW SDP Provider test console macros, asserts & logs
*
*/

#ifndef NSPTESTCONSOLEMACROS_H
#define NSPTESTCONSOLEMACROS_H

#define COMPONENT
_LIT( KComponent, "NSP test console" );

#include "nspdefs.h"

const TInt KAssertFailure = -20100;

struct Result
	{
	enum TState{ ECreated = 0, ERunning, EPass, EFail } iState;
	TBuf<255> iDescription;
	};

typedef struct Result TResult;

#define __ASSERT( aStatement ) \
	__ASSERT_ALWAYS( aStatement, User::Leave( KAssertFailure ) )

#define START( aSetupStatus, aCons, aDesc ) \
	if ( KErrNone != aSetupStatus )\
		{\
		NSPLOG_STR( "******************************************" );\
		NSPLOG_STR2( "FAILURE in SetupL, test:", aDesc );\
		NSPLOG_INT(  "with               code:", aSetupStatus );\
		aCons.Printf( _L( "%S - FAILURE in SetupL\n\n" ), &aDesc );\
		NSPLOG_STR( "******************************************" );\
		}\
	else\
		{\
		NSPLOG_STR( "******************************************" );\
		NSPLOG_STR2( "STARTING test:", aDesc );\
		aCons.Printf( _L( "%S - STARTING\n\n" ), &aDesc );\
		NSPLOG_STR( "******************************************" );\
		}

#define END( aCons, aDesc, aState ) \
	NSPLOG_STR( "******************************************" );\
	NSPLOG_STR2( "ENDED    test:", aDesc );\
	NSPLOG_INT(  "with     code:", aState );\
	aCons.Printf( _L( "\n%S - ENDED with %d\n\n" ), &aDesc, aState );\
	NSPLOG_STR( "******************************************" )

#define WAIT( aCons, aText ) aCons.Printf( _L( "Waiting " aText "..\n" ) )

#define COMPLETED_L( aCons, aText, aTimerStatus ) \
	if ( KErrTimedOut == aTimerStatus )\
		{ aCons.Printf( _L("..Timeout " aText "\n" ) ); User::Leave( KErrTimedOut ); }\
	else if ( KErrCancel == aTimerStatus )\
		{ aCons.Printf( _L("..Canceled " aText "\n" ) ); User::Leave( KErrCancel ); }\
	else{ aCons.Printf( _L("..Completed " aText "\n" ) ); }


#endif // NSPTESTCONSOLEMACROS_H
