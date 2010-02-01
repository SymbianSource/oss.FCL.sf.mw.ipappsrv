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
#include "TCmdSetSecurityParams.h"

//#include <sipsecurity.h>

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	AuthenticationMechanism, SecurityParams*
 *   IDs:			-
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			-
 */
void TCmdSetSecurityParams::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get mandatory AuthenticationMechanism
	TPtrC8 mechanism = ExtractTextL( KParamAuthenticationMechanism );

	// Create a string array for the parameter values
	CDesC8ArrayFlat* secParams =
							new( ELeave ) CDesC8ArrayFlat( KTcGranularity );
	CleanupStack::PushL( secParams );

	// Get optional security parameters structure
	CTcStructure* structure = FindStructureL( KParamSecurityParams, EFalse );
	if( structure )
		{
		TInt count = structure->Count();
		for( TInt i = 0; i < count; i++ )
			{
			// Each item is read from the structure
			const CTcNameValue& tv = structure->Item( i );
			// ..and appended to the array formatted as "<name><tab><value>"
			TBuf8< KMaxSecurityParamLength > entry;
			entry.Append( tv.Name() );
			entry.Append( KTab );
			entry.Append( tv.Value() );
			secParams->AppendL( entry );
			}
		}

	// -- Execution -----------------------------------------------------------

//	CSIPSecurity* security = CSIPSecurity::NewLC( iContext.SIP() );
//	security->SetParamsL( mechanism, *secParams );
//	CleanupStack::PopAndDestroy( security );
	CleanupStack::PopAndDestroy( secParams );

	// -- Response creation ---------------------------------------------------
	}

TBool TCmdSetSecurityParams::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("SetSecurityParams") );
	}

TTcCommandBase* TCmdSetSecurityParams::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSetSecurityParams( aContext );
	}

