/*
* Copyright (c) 2002-2003 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Default data (headers) for a certain SIP request.
*
*/




// INCLUDE FILES
#include "mcesipdefaultdata.h"
#include "mcesip.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CMceSipDefaultData::CMceSipDefaultData()
    {
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CMceSipDefaultData::ConstructL( 
                 const TDesC8& aMethod, 
                 CDesC8Array* aHeaders,
                 RPointerArray< CMceSipDefaultData >& aDataArray )
    {
    User::LeaveIfError ( iMethodIndex = MceSip::SIPStringIndex( aMethod ) );
    aDataArray.AppendL( this );
    iHeaders = aHeaders;
    
    }

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CMceSipDefaultData* CMceSipDefaultData::NewL( 
                  const TDesC8& aMethod,
                  CDesC8Array* aHeaders,
                  RPointerArray< CMceSipDefaultData >& aDataArray )
    {
    __ASSERT_ALWAYS( aHeaders, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aHeaders->MdcaCount() > 0, User::Leave( KErrArgument ) );
    
    CMceSipDefaultData* self = new ( ELeave ) CMceSipDefaultData();
    CleanupStack::PushL( self );
    self->ConstructL( aMethod, aHeaders, aDataArray );
    CleanupStack::Pop( self ); 
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CMceSipDefaultData::~CMceSipDefaultData()
    {
    delete iHeaders;
    }

// ---------------------------------------------------------------------------
// CMceSipDefaultData::Match
// ---------------------------------------------------------------------------
//
TBool CMceSipDefaultData::Match( const TDesC8& aMethod )
    {
    return iMethodIndex == MceSip::SIPStringIndex( aMethod );
    }

// ---------------------------------------------------------------------------
// CMceSipDefaultData::Match
// ---------------------------------------------------------------------------
//
TBool CMceSipDefaultData::Match( TInt aMethodInd )
    {
    return iMethodIndex == aMethodInd;
    }

// ---------------------------------------------------------------------------
// CMceSipDefaultData::SetHeaders
// ---------------------------------------------------------------------------
//
void CMceSipDefaultData::SetHeaders( CDesC8Array* aHeaders )
    {
    delete iHeaders;
    iHeaders = aHeaders;  
    __TEST_INVARIANT;
    }

// ---------------------------------------------------------------------------
// CMceSipDefaultData::DefaultHeaders
// ---------------------------------------------------------------------------
//
CDesC8Array& CMceSipDefaultData::DefaultHeaders()
    {
    return *iHeaders;
    }

// -----------------------------------------------------------------------------
// CMceSipDefaultData::__DbgTestInvariant
// Test invariant
// -----------------------------------------------------------------------------
//	
void CMceSipDefaultData::__DbgTestInvariant() const
	{
	if ( !( iHeaders && iHeaders->MdcaCount() > 0 ) )
	    {
	    User::Invariant();
	    }
	}


//  End of File

