/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  DM Tree module
*
*/


// ------------------------------------------------------------------------------------------------
// Includes
// ------------------------------------------------------------------------------------------------

#include <utf.h>
#include <nsmldmiapmatcher.h>
#include <smldmadapter.h>
#include <e32debug.h>

#include "UT_StateQuery.h"

_LIT8( KPrefConRef, "AP/APId001" );

// ------------------------------------------------------------------------------------------------
// CNSmlDMIAPMatcher* CNSmlDMIAPMatcher::NewL( MSmlDmCallback* aDmCallback )
// ------------------------------------------------------------------------------------------------
EXPORT_C CNSmlDMIAPMatcher* CNSmlDMIAPMatcher::NewL( 
	MSmlDmCallback* aDmCallback )
    {
    CNSmlDMIAPMatcher* self = CNSmlDMIAPMatcher::NewLC( aDmCallback );	
    CleanupStack::Pop();
    return self;
    }

// ------------------------------------------------------------------------------------------------
// CNSmlDMIAPMatcher* CNSmlDMIAPMatcher::NewLC( MSmlDmCallback* aDmCallback )
// ------------------------------------------------------------------------------------------------
EXPORT_C CNSmlDMIAPMatcher* CNSmlDMIAPMatcher::NewLC(
	MSmlDmCallback* aDmCallback )
    {
    CNSmlDMIAPMatcher* self = new( ELeave ) CNSmlDMIAPMatcher;
    CleanupStack::PushL( self );
    self->ConstructL( aDmCallback );
    return self;
    }

// ------------------------------------------------------------------------------------------------
// void CNSmlDMIAPMatcher::ConstructL( MSmlDmCallback* aDmCallback )
// ------------------------------------------------------------------------------------------------
void CNSmlDMIAPMatcher::ConstructL( MSmlDmCallback* aDmCallback )
    {
    iCallback = aDmCallback;
    }

// ------------------------------------------------------------------------------------------------
// CNSmlDMIAPMatcher::~CNSmlDMIAPMatcher()
// ------------------------------------------------------------------------------------------------
CNSmlDMIAPMatcher::~CNSmlDMIAPMatcher()
    {
    }

// ------------------------------------------------------------------------------------------------
// TInt CNSmlDMIAPMatcher::IAPIdFromURIL( const TDesC8& aURI )
// ------------------------------------------------------------------------------------------------
EXPORT_C TInt CNSmlDMIAPMatcher::IAPIdFromURIL( const TDesC8& aURI )
    {
    if ( KPrefConRef() == aURI )
        {
        return 2;
        }
    else
        {
        return -1;
        }
//    TStates* states = static_cast< TStates* > ( Dll::Tls() );
//    return states->Err();
    
    }

// ------------------------------------------------------------------------------------------------
// HBufC8* CNSmlDMIAPMatcher::URIFromIAPIdL( TInt aIAPId )
// ------------------------------------------------------------------------------------------------
EXPORT_C HBufC8* CNSmlDMIAPMatcher::URIFromIAPIdL( TInt aIAPId )
    {
//    TStates* states = static_cast< TStates* > ( Dll::Tls() );
//    HBufC8* temp = HBufC8::NewL( 5 );
//    TPtr8 ptr( temp->Des() );
//    ptr.Copy( _L( "aa" ) );
//    TInt err( states->Err() );
//    if ( err )
//        {
//        delete temp;
//        temp = NULL;
//        }
    HBufC8* temp = NULL;

    if ( 2 == aIAPId )
        {
        temp = HBufC8::NewL( 10 );
        TPtr8 ptr( temp->Des() );
        ptr.Copy( KPrefConRef() );
        }
    
    return temp;
    }

// ------------------------------------------------------------------------------------------------
// TInt CNSmlDMIAPMatcher::DesToInt(const TDesC& aLuid)
// ------------------------------------------------------------------------------------------------
TInt CNSmlDMIAPMatcher::DesToInt(const TDesC8& /*aLuid*/)
	{
	return 0;
	}

// ------------------------------------------------------------------------------------------------
// TPtrC8 CNSmlDMIAPMatcher::RemoveDotSlash(const TDesC8& aURI) const
// ------------------------------------------------------------------------------------------------
TPtrC8 CNSmlDMIAPMatcher::RemoveDotSlash(const TDesC8& /*aURI*/) const
	{
	return KNullDesC8();
	}

// ------------------------------------------------------------------------------------------------
// CNSmlDMIAPMatcher::TPtrC8 LastURISeg(const TDesC8& aURI)
// Returns only the last uri segemnt
// ------------------------------------------------------------------------------------------------
TPtrC8 CNSmlDMIAPMatcher::LastURISeg(const TDesC8& /*aURI*/) const
	{
	return KNullDesC8();
	}

// End of file

