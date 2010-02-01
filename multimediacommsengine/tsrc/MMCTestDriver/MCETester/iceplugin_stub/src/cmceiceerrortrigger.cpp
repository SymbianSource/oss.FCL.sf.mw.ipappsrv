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
* Description:    Generates error callbacks to client
*
*/




#include "cmceiceerrortrigger.h"
#include "nspsessionobserver.h"
#include <e32debug.h>

_LIT8( KIcmp, "icmp" );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CMceIceErrorTrigger::NewL
// ---------------------------------------------------------------------------
//
CMceIceErrorTrigger* CMceIceErrorTrigger::NewL( TUid aCategory, TUint aKey )
    {
	CMceIceErrorTrigger* self =
		new ( ELeave ) CMceIceErrorTrigger( aCategory, aKey );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
    }

// ---------------------------------------------------------------------------
// CMceIceErrorTrigger::~CMceIceErrorTrigger
// ---------------------------------------------------------------------------
//
CMceIceErrorTrigger::~CMceIceErrorTrigger()
    {
    }

// ---------------------------------------------------------------------------
// CMceIceErrorTrigger::CMceIceErrorTrigger
// ---------------------------------------------------------------------------
//
CMceIceErrorTrigger::CMceIceErrorTrigger( TUid aCategory, TUint aKey ) :
	CMceIcePropertyWatcher( aCategory, aKey )
    {
	}

// ---------------------------------------------------------------------------
// CMceIceErrorTrigger::ConstructL
// ---------------------------------------------------------------------------
//
void CMceIceErrorTrigger::ConstructL()
    {
    CMceIcePropertyWatcher::ConstructL();
	}

// ---------------------------------------------------------------------------
// CMceIceErrorTrigger::ParsePropertyL
// If observer has not yet been set don't make the callback.
// ---------------------------------------------------------------------------
//
void CMceIceErrorTrigger::ParsePropertyL()
	{
	if ( iObserver )
		{
		TInt errorCode = ParseErrorCode();
		if ( Value().Left( KIcmp().Length() ).CompareF( KIcmp ) == 0 )
			{
			RDebug::Print(
				_L( "ICEPlugin Dummy: IcmpErrorOccurred id=%d err=%d" ),
				iSessionId, errorCode );
			iObserver->IcmpErrorOccurred( iSessionId, errorCode );
			}
		else
			{
			RDebug::Print( _L( "ICEPlugin Dummy: ErrorOccurred id=%d err=%d" ),
						   iSessionId, errorCode );
			iObserver->ErrorOccurred( iSessionId, errorCode );
			}
		}
	else
		{
		RDebug::Print(
			_L( "ICEPlugin Dummy: ErrorTrigger no observer" ) );
		}
	}

// ---------------------------------------------------------------------------
// CMceIceErrorTrigger::SetObserver
// ---------------------------------------------------------------------------
//
void CMceIceErrorTrigger::SetObserver( MNSPSessionObserver& aObserver,
									   TInt aSessionId )
	{
	iObserver = &aObserver;
	iSessionId = aSessionId;
	}

// ---------------------------------------------------------------------------
// CMceIceErrorTrigger::ParseErrorCode
// ---------------------------------------------------------------------------
//
TInt CMceIceErrorTrigger::ParseErrorCode() const
	{
	TInt errorCode( 0 );
	TLex8 lex( Value() );

	TChar ch = lex.Peek();
	while ( !ch.IsDigit() && ch != '-' && !ch.Eos() )
		{
		lex.Get();
		ch = lex.Peek();
		}

	if ( ch.Eos() )
		{
		errorCode = KErrNotFound;
		}
	else
		{
		lex.Val( errorCode );
		}
	RDebug::Print( _L("ICEPlugin Dummy: ParseErrorCode=%d" ), errorCode );
	return errorCode;
	}

// end of file
