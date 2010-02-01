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
* Description:    Default SIP data container
*
*/




// INCLUDE FILES
#include "mcesipdefaults.h"
#include "mcesipdefaultdata.h"
#include "mcesip.h"
#include "mcedefs.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CMceSipDefaults::CMceSipDefaults( ) 
    {
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CMceSipDefaults::~CMceSipDefaults()
    {
    Clear();
    delete iEmptyHeaders;
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CMceSipDefaults::ConstructL()
    {
    iEmptyHeaders = new( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
    }

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CMceSipDefaults* CMceSipDefaults::NewL()
    {
    CMceSipDefaults* self = new ( ELeave ) CMceSipDefaults();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self ); 
    return self;
    }

// ---------------------------------------------------------------------------
// CMceSipDefaults::StoreDefaultHeadersL
// ---------------------------------------------------------------------------
//
void CMceSipDefaults::StoreDefaultHeadersL( const CDesC8Array& aMethods,
                                            CDesC8Array* aHeaders )
    {
    if ( aMethods.MdcaCount() == 0 )
        {
        // No methods, the headers are cleared.
        Clear();
        delete aHeaders;
        }
    else 
	    {
	    for ( TInt m = 0; m< aMethods.MdcaCount() ; m++ )
	        {
	        CDesC8Array* headers = NULL;
	        TBool pushed = EFalse;
	        if ( m == aMethods.MdcaCount() - 1 )
	            {
	            headers = aHeaders;
	            }
	        else
	            {
	            // make a copy
	            headers = CopyHeadersL( aHeaders );
	            CleanupStack::PushL( headers );
	            pushed = ETrue;
	            }
	        StoreDefaultHeadersL( aMethods[ m ], headers );
	        if( pushed )
	            {
	            CleanupStack::Pop( headers );
	            }
	        }    	
	    }
    }

// ---------------------------------------------------------------------------
// CMceSipDefaults::DefaultHeaders
// ---------------------------------------------------------------------------
//  
CDesC8Array& CMceSipDefaults::DefaultHeaders( const TDesC8& aMethod )
    {
    TInt ind = FindData( aMethod );
    if ( ind != KErrNotFound )
        {
        return iDataArray[ ind ]->DefaultHeaders();
        }
    else
        {
        return *iEmptyHeaders;
        }
    }

// ---------------------------------------------------------------------------
// CMceSipDefaults::DefaultHeaders
// ---------------------------------------------------------------------------
//  
CDesC8Array& CMceSipDefaults::DefaultHeaders( TInt aMethodInd )
    {
    TInt ind = FindData( aMethodInd );
    if ( ind != KErrNotFound )
        {
        return iDataArray[ ind ]->DefaultHeaders();
        }
    else
        {
        return *iEmptyHeaders;
        }
    }

// ---------------------------------------------------------------------------
// CMceSipDefaults::StoreDefaultHeadersL
// ---------------------------------------------------------------------------
//
void CMceSipDefaults::StoreDefaultHeadersL( const TDesC8& aMethod,
                                            CDesC8Array* aHeaders )
    {
    // Find the current data for the method
    TInt ind = FindData( aMethod );
    
    if ( !aHeaders || aHeaders->MdcaCount() == 0 )
        {
        // If the headers are empty, no data will be stored.
        if ( ind != KErrNotFound )
            {
            // Delete data
            delete iDataArray[ ind ];
            iDataArray.Remove( ind );
            }
        delete aHeaders;
        }
    else if ( ind != KErrNotFound )
        {
        // reset headers for data
        iDataArray[ ind ]->SetHeaders( aHeaders );   
        }
    else
        {
        // create new.
        CMceSipDefaultData::NewL( aMethod, aHeaders, iDataArray );
        }
    }
     
// ---------------------------------------------------------------------------
// CMceSipDefaults::FindData
// ---------------------------------------------------------------------------
//
TInt CMceSipDefaults::FindData( const TDesC8& aMethod ) const
    {
    TInt ret = KErrNotFound;
    for ( TInt d = 0; ret == KErrNotFound && d < iDataArray.Count(); d++)
        {
        if ( iDataArray[ d ]->Match( aMethod ) )
            {
            // found
            ret = d;
            }
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// CMceSipDefaults::FindData
// ---------------------------------------------------------------------------
//
TInt CMceSipDefaults::FindData( TInt aMethodInd ) const
    {
    TInt ret = KErrNotFound;
    for ( TInt d = 0; ret == KErrNotFound && d < iDataArray.Count(); d++)
        {
        if ( iDataArray[ d ]->Match( aMethodInd ) )
            {
            // found
            ret = d;
            }
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// CMceSipDefaults::Clear
// ---------------------------------------------------------------------------
//  
void CMceSipDefaults::Clear()
    {
    iDataArray.ResetAndDestroy();
    }


// ---------------------------------------------------------------------------
// CMceSipDefaults::CopyHeadersL
// ---------------------------------------------------------------------------
//  
CDesC8Array* CMceSipDefaults::CopyHeadersL( const CDesC8Array* aHeaders )
    {
    CDesC8Array* copy = NULL;
    if ( aHeaders )
        {
        copy = new( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
        CleanupStack::PushL( copy );
        for ( TInt h = 0; h < aHeaders->MdcaCount(); h++ )
            {
            copy->AppendL( aHeaders->MdcaPoint( h ) );
            }
        CleanupStack::Pop( copy );
        }
    return copy;
    }

//  End of File

