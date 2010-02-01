/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    implementation
*
*/




//  INCLUDE FILES
#include "sipprofile.h"

_LIT8( KSIPProfileExtenstionParameter, "%u");
const TUint KProfileBufferSize = 25;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSIPProfile::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSIPProfile* CSIPProfile::NewL()
	{
    CSIPProfile* self = new(ELeave)CSIPProfile();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
	}

// -----------------------------------------------------------------------------
// CSIPProfile::~CSIPProfile
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPProfile::~CSIPProfile()
	{
	delete iAORs;
    delete iContactParams;
	}

// -----------------------------------------------------------------------------
// CSIPProfile::GetParameter
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CSIPProfile::GetParameter(TUint32 /*aParam*/, TDesC8 const *& aVal ) const
	{
	if ( !iError )
	    {
	    const TDesC8& val= KSIPProfileDefAOR;
        aVal = &val;
	    }
	return iError;
	}	

// -----------------------------------------------------------------------------
// CSIPProfile::GetParameter
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CSIPProfile::GetParameter(TUint32 /*aParam*/, TUint32& aVal) const
	{
	if ( !iError )
	    {
	    aVal = 1;
	    }
	
	return iError;
	}	

// -----------------------------------------------------------------------------
// CSIPProfile::GetParameter
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CSIPProfile::GetParameter(TUint32 /*aParam*/, TBool& aVal) const
	{
	if ( !iError )
	    {
	    TBool val = ETrue;
	    aVal = ETrue;
	    }
	return iError;
	}

// -----------------------------------------------------------------------------
// CSIPProfile::GetParameter
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CSIPProfile::GetParameter(TUint32 aParam, 
                                        MDesC8Array const *& aVal ) const
	{
	TInt ret = KErrNone;
	switch ( aParam )
	    {
	    case KSIPRegisteredAors:
	        {
	        if ( !iAorError )
	            {
	            aVal = iAORs;
	            }
	        ret = iAorError;
	        break;
	        }
	    case KSIPContactHeaderParams:
	        {
	        if ( !iParamError )
	            {
	            aVal = iContactParams;
	            }
	        ret = iParamError;
	        break;
	        }
	    default:
	        {
	        ret = KErrNotFound;
	        break;
	        }
	    }
	return ret;
	}
	
// -----------------------------------------------------------------------------
// CSIPProfile::CSIPProfile
// -----------------------------------------------------------------------------
//	
CSIPProfile::CSIPProfile():
	iEnabled(EFalse)
	{
	}

// -----------------------------------------------------------------------------
// CSIPProfile::ConstructL
// -----------------------------------------------------------------------------
//	
void CSIPProfile::ConstructL()
	{
    iAORs = new( ELeave ) CDesC8ArrayFlat( 1 );
    iAORs->AppendL( KSIPProfileDefAOR );
    iContactParams = new( ELeave ) CDesC8ArrayFlat( 1 );
    iContactParams->AppendL( KFeatureTag );
    iContactParams->AppendL( KParamValue );
	}

// -----------------------------------------------------------------------------
// CSIPProfile::IsContextActive
// -----------------------------------------------------------------------------
//	
TBool CSIPProfile::IsContextActive() const
	{
	return ETrue;
	}
	
// -----------------------------------------------------------------------------
// CSIPProfile::ContextId
// -----------------------------------------------------------------------------
//	
TUint32 CSIPProfile::ContextId() const
	{
	return 1;
	}
