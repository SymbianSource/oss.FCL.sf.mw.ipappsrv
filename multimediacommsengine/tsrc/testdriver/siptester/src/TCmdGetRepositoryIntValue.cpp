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
* Description:  Implementation
*
*/


#include <centralrepository.h>
#include "CTcSIPContext.h"
#include "TCmdGetRepositoryIntValue.h"
#include "SIPConstants.h"


/**
 * KParamRepositoryUid identifies the Central repository.
 * This parameter is mandatory
 *
 * KParamRepositoryParameterKey identifies the integer type parameter within
 * the selected central repository. This parameter is mandatory.
 *
 * KParamRepositoryParameterValue is the value to be set for the parameter.
 * This parameter is optional. If it is not specified, then the parameter is
 * deleted from the repository.
 *
 *
 * INPUT:
 *   Headers:		-
 *   Parameters:	KParamRepositoryUid,
 *					KParamRepositoryParameterKey,
 * 					KParamRepositoryParameterValue
 *   IDs:			-
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			-
 */
void TCmdGetRepositoryIntValue::ExecuteL()
	{
	// If KParamRepositoryParameterValue is not specified, remove the
	// parameter from repository.
	const TInt KDefaultValue = -984079;
	TInt repositoryUidValue = ExtractIntegerL( KParamRepositoryUid,
								  	   	  	   KDefaultValue,
								  	   	  	   ETrue );
	TUid repositoryUid = TUid::Uid( repositoryUidValue );

	TInt paramKey = ExtractIntegerL( KParamRepositoryParameterKey,
								  	 KDefaultValue,
								  	 ETrue );

	TInt err( KErrNone );
	CRepository* repository = CRepository::NewLC( repositoryUid );
	
	TInt repValue = 0;

	if ( paramKey != KDefaultValue )
		{
		err = repository->Get( paramKey, repValue );
		}
	else
		{
		err = repository->Delete( paramKey );
		if ( err == KErrNotFound )
			{
			// Trying to delete a non-existent parameter is not an error
			err = KErrNone;
			}
		}

	CleanupStack::PopAndDestroy( repository );
	
	AddIntegerResponseL( KParamRepositoryIntValue, repValue );

	User::LeaveIfError( err );
	}

TBool TCmdGetRepositoryIntValue::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("GetRepositoryIntValue") );
	}

TTcCommandBase* TCmdGetRepositoryIntValue::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetRepositoryIntValue( aContext );
	}
