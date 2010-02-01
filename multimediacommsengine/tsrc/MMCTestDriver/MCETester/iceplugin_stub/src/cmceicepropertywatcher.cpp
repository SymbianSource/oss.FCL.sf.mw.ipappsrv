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
* Description:    Monitors a property
*
*/




#include "mcetesterpskeys.h"
#include "cmceicepropertywatcher.h"
#include <Uri8.h>
#include <utf.h>
#include <e32debug.h>

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CMceIcePropertyWatcher::NewL
// ---------------------------------------------------------------------------
//
CMceIcePropertyWatcher*
CMceIcePropertyWatcher::NewL( TUid aCategory, TUint aKey )
    {
	CMceIcePropertyWatcher* self =
		new ( ELeave ) CMceIcePropertyWatcher( aCategory, aKey );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
    }

// ---------------------------------------------------------------------------
// CMceIcePropertyWatcher::~CMceIcePropertyWatcher
// ---------------------------------------------------------------------------
//
CMceIcePropertyWatcher::~CMceIcePropertyWatcher()
    {
    Cancel();
    delete iValue;
    }

// ---------------------------------------------------------------------------
// CMceIcePropertyWatcher::DoCancel
// ---------------------------------------------------------------------------
//
void CMceIcePropertyWatcher::DoCancel()
	{
	iProperty.Cancel();
	}

// ---------------------------------------------------------------------------
// CMceIcePropertyWatcher::RunL
// ---------------------------------------------------------------------------
//		
void CMceIcePropertyWatcher::RunL()
	{
	RDebug::Print( _L( "ICEPlugin Dummy: PropertyWatcher::RunL" ) );

	if ( iStatus == KErrNone )
		{
		ListenForChanges();
		GetPropertyValueL();
		}
	}

// ---------------------------------------------------------------------------
// CMceIcePropertyWatcher::RunError
// ---------------------------------------------------------------------------
//
TInt CMceIcePropertyWatcher::RunError( TInt aError )
	{
	RDebug::Print( _L( "ICEPlugin Dummy: PropertyWatcher::RunError aError=%d" ),
				   aError );
	return KErrNone;
	}

// ---------------------------------------------------------------------------
// CMceIcePropertyWatcher::PropertyValue
// ---------------------------------------------------------------------------
//
TInt CMceIcePropertyWatcher::PropertyValue( TInetAddr& aValue ) const
	{
	if ( iPropertyExists )
		{
		aValue = iAddress;
		return KErrNone;
		}
	return KErrNotFound;
	}

// ---------------------------------------------------------------------------
// CMceIcePropertyWatcher::CMceIcePropertyWatcher
// ---------------------------------------------------------------------------
//
CMceIcePropertyWatcher::CMceIcePropertyWatcher( TUid aCategory, TUint aKey ) :
	CActive( EPriorityStandard ),
	iCategory( aCategory ),
	iKey( aKey )
    {
    CActiveScheduler::Add( this );
	}

// ---------------------------------------------------------------------------
// CMceIcePropertyWatcher::ConstructL
// ---------------------------------------------------------------------------
//
void CMceIcePropertyWatcher::ConstructL()
    {
    iValue = HBufC8::NewL( KPropertyTextValueMaxLength );

    GetPropertyValueL();

    User::LeaveIfError( iProperty.Attach( iCategory, iKey ) );
    ListenForChanges();
	}

// ---------------------------------------------------------------------------
// CMceIcePropertyWatcher::ListenForChanges
// ---------------------------------------------------------------------------
//
void CMceIcePropertyWatcher::ListenForChanges()
	{
	RDebug::Print( _L( "ICEPlugin Dummy: PropertyWatcher::ListenForChanges" ) );

	iProperty.Subscribe( iStatus );
	SetActive();
	}

// ---------------------------------------------------------------------------
// CMceIcePropertyWatcher::GetPropertyValueL
// ---------------------------------------------------------------------------
//
void CMceIcePropertyWatcher::GetPropertyValueL()
	{
	TPtr8 ptr( iValue->Des() );
	TInt error = RProperty::Get( iCategory, iKey, ptr );
	switch ( error )
		{
		case KErrNone:
			ParsePropertyL();
			RDebug::Print( _L( "ICEPlugin Dummy: got property value" ) );
			iPropertyExists = ETrue;
			break;

		case KErrNotFound: // Property has not been defined, no problem.
			iPropertyExists = EFalse;
			RDebug::Print( _L( "ICEPlugin Dummy: property does not exist" ) );
			break;

		default:
			RDebug::Print( _L( "ICEPlugin Dummy: RProperty::Get() error=%d" ),
						   error );
			User::Leave( error );
    	}
	}

// ---------------------------------------------------------------------------
// CMceIcePropertyWatcher::ParsePropertyL
// IPv4 address should be in format "1.2.3.4:5000".
// IPv6 address should be in format "[1:2:3::4:5]:6000".
// ---------------------------------------------------------------------------
//
void CMceIcePropertyWatcher::ParsePropertyL()
	{
	// Add prefix
    _LIT8( KSIPScheme, "sip:" );
	HBufC8* uriBuf = HBufC8::NewLC( KSIPScheme().Length() + iValue->Length() );
	TPtr8 ptrToUri = uriBuf->Des();
	ptrToUri.Append( KSIPScheme );
	ptrToUri.Append( *iValue );

	TUriParser8 parser;
    parser.Parse( *uriBuf );
    // Don't delete uriBuf yet, parser uses it
	const TDesC8& host8 = parser.Extract( EUriHost );

	// Convert to 16-bit descriptor
	HBufC* host16 = HBufC::NewLC( host8.Size() );
	TPtr ptrToHost = host16->Des();
	User::LeaveIfError(
		CnvUtfConverter::ConvertToUnicodeFromUtf8( ptrToHost, host8 ) );

	RDebug::Print( _L( "ICEPlugin Dummy: parse host=%S" ), &ptrToHost );
	User::LeaveIfError( iAddress.Input( *host16 ) );

	if ( parser.IsPresent( EUriPort ) )
		{
        const TDesC8& port = parser.Extract( EUriPort );
        TInt portValue( 0 );
        TLex8 lex( port );
        lex.Val( portValue );
        RDebug::Print( _L( "ICEPlugin Dummy: parse port=%d" ), portValue );
		iAddress.SetPort( portValue );	
		}
	CleanupStack::PopAndDestroy( host16 );
	CleanupStack::PopAndDestroy( uriBuf );
	}

// ---------------------------------------------------------------------------
// CMceIcePropertyWatcher::Value
// ---------------------------------------------------------------------------
//
const TDesC8& CMceIcePropertyWatcher::Value() const
	{
	return *iValue;
	}
			
// end of file
