/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation
*
*/

#include "CTcSIPContext.h"
#include "SIPConstants.h"
#include "TCmdGetProfiles.h"
#include "Cleanupresetanddestroy.h"
#include "CTcSIPProfileContainer.h"

#include <sipmanagedprofile.h>
#include <sipmanagedprofileregistry.h>
#include <sipprofile.h>
#include <sipprofiletypeinfo.h>
#include "tclog.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	ProfileType*, ProfileAOR*
 *   IDs:			RegistryId
 *
 * OUTPUT:
 *	 Parameters:	-
 *	 IDs:			ProfileIdList
 */
void TCmdGetProfiles::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

    // Get selected registry
    CTcSIPProfileContainer& container = SelectProfileL();

	// Type
	TcLog::WriteFormat( _L8("-- Get optional structure \"ProfileType\"") );
	CTcStructure* structure = FindStructureL( KParamProfileType, EFalse );
	TSIPProfileTypeInfo typeInfo;
	if( structure )
		{
		TcLog::Write( KLogOk );
		TcLog::WriteFormat( _L8("-- Get mandatory structure item \"ProfileName\"") );
		typeInfo.iSIPProfileName = structure->ItemL( KParamProfileName )->Value();
		TcLog::Write( KLogOk );
		TcLog::WriteFormat( _L8("-- Get mandatory structure item \"Class\"") );
		typeInfo.iSIPProfileClass = ProfileTypeClassL( structure );
		TcLog::Write( KLogOk );
		}
	else
		{
		TcLog::Write( KLogNotFound );
		}

	// Get profile AOR, or KNullDesC8
	TPtrC8 aor = ExtractTextL( KParamProfileAOR, EFalse );

	// -- Execution -----------------------------------------------------------

	RPointerArray< CSIPProfile > profileArray;
	CleanupResetAndDestroyPushL( profileArray );

	// See if Type was given
	if( structure )
		{
		container.ManagedProfileRegistry().ProfilesL( typeInfo, profileArray );
		}
	else if( aor != KNullDesC8 )
		{
		container.ManagedProfileRegistry().ProfilesL( aor, profileArray );
		}
	else
		{
		User::Leave( KTcErrMandatoryParameterNotFound );
		}

	// -- Response creation ---------------------------------------------------

    // Crete response only if some profiles exist
	TInt count = profileArray.Count();
	if ( count > 0 )
	    {
    	CDesC8ArrayFlat* array = new ( ELeave ) CDesC8ArrayFlat( count );
    	CleanupStack::PushL( array );

    	for( TInt i = 0; i < count; i++ )
    		{
    		// Register object
    		CSIPProfile* profile = profileArray[ i ];
    		// Create a name out of the profile id
    		TBuf8< KTcMaxIntConversion > idName;
    		
    		TUint32 profileId( 0 );
    		User::LeaveIfError(
    		        profile->GetParameter( KSIPProfileId, profileId ) );
    		idName.AppendNum( (TInt)profileId );
    		// ..and add it to the ProfileIdList array
    		array->AppendL( idName );
    		}

    	AddArrayResponseL( KParamProfileProfileIdist, *array );
    	
    	CleanupStack::PopAndDestroy(); // array
	    }
	CleanupStack::PopAndDestroy(); // profileArray
	}


TBool TCmdGetProfiles::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("GetProfiles") );
	}

TTcCommandBase* TCmdGetProfiles::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetProfiles( aContext );
	}
