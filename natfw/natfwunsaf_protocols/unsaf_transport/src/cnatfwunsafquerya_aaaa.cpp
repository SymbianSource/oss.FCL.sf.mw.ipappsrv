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



#include <utf.h>
#include "cnatfwunsafquerya_aaaa.h"
#include "mnatfwunsafhostresolver.h"

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryA_AAAA::CNATFWUNSAFQueryA_AAAA
// ----------------------------------------------------------------------------
//
CNATFWUNSAFQueryA_AAAA::CNATFWUNSAFQueryA_AAAA()
:CNATFWUNSAFQueryBase()
    {
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryA_AAAA::ConstructL
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFQueryA_AAAA::ConstructL( const TDesC8& aTarget )
    {
    SetTargetL( aTarget );
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryA_AAAA::NewL
// ----------------------------------------------------------------------------
//
CNATFWUNSAFQueryA_AAAA* CNATFWUNSAFQueryA_AAAA::NewL( const TDesC8& aTarget )
    {
    CNATFWUNSAFQueryA_AAAA* self = new ( ELeave ) CNATFWUNSAFQueryA_AAAA();
    CleanupStack::PushL( self );
    self->ConstructL( aTarget );
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryA_AAAA::~CNATFWUNSAFQueryA_AAAA
// ----------------------------------------------------------------------------
//
CNATFWUNSAFQueryA_AAAA::~CNATFWUNSAFQueryA_AAAA()
    {
    delete iTarget;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryA_AAAA::SetTargetL
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFQueryA_AAAA::SetTargetL( const TDesC8& aTarget )
    {
    __ASSERT_ALWAYS ( aTarget.Length() > 0, User::Leave( KErrArgument ) );
    HBufC* temp = HBufC::NewL( aTarget.Length() );
    TPtr targetPtr( temp->Des() );
    CleanupStack::PushL( temp );
    User::LeaveIfError(
        CnvUtfConverter::ConvertToUnicodeFromUtf8( targetPtr, aTarget ) );
    delete iTarget;
    iTarget = temp;
    CleanupStack::Pop( temp );
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryA_AAAA::QueryResultBuf
// ----------------------------------------------------------------------------
//
TInetAddr CNATFWUNSAFQueryA_AAAA::QueryResultBuf()
    {
    return iResult = TInetAddr::Cast( iNResult().iAddr );
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryA_AAAA::Query
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFQueryA_AAAA::Query( MNATFWUNSAFHostResolver& aResolver )
    {
    aResolver.Resolver().GetByName( *iTarget,
                                     iNResult,
                                     aResolver.RequestStatus() );
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryA_AAAA::QueryNext
// ----------------------------------------------------------------------------
//
TBool CNATFWUNSAFQueryA_AAAA::QueryNext( MNATFWUNSAFHostResolver& aResolver )
    {
    TBool result = EFalse;
    if( aResolver.Resolver().Next( iNResult ) == KErrNone    )
        {
        result = ETrue;
        }
    return result;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryA_AAAA::HandleQueryResultL
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFQueryA_AAAA::HandleQueryResultL(
    MNATFWUNSAFHostResolver& /*aResolver*/ )
    {
    __ASSERT_ALWAYS ( EFalse, User::Leave( KErrNotSupported ) );
    }


