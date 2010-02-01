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
#include "CTcSIPConnectionContainer.h"
#include "TCmdGetProfileStatus.h"
#include "CTcSIPProfileContainer.h"

#include <sipmanagedprofile.h>
#include <sipmanagedprofileregistry.h>
#include <sipprofile.h>
#include <sipprofiletypeinfo.h>

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	Timeout*
 *   IDs:			RegistryId
 *
 * OUTPUT:
 *	 Parameters:	ProfileErrorCode, ProfileState
 *	 IDs:			ProfileId
 */
void TCmdGetProfileStatus::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------
	// Get selected registry
    CTcSIPProfileContainer& container = SelectProfileL();

	// Get an item off the receive queue (waits until timeout if none is present)
	TInt timeout = ExtractIntegerL( KParamTimeout, KDefaultReceiveTimeout, EFalse );
	TTcProfileReceived item = container.ReceivedProfileItemL( timeout );

	// -- Execution -----------------------------------------------------------


	// -- Response creation ---------------------------------------------------

	AddIntegerResponseL( KParamProfileId, item.iProfileId );
	AddIntegerResponseL( KParamProfileErrorCode, item.iError );

	TPtrC8 state;
	switch( item.iEvent )
		{
		case ETcProfileCreated:
			{
			state.Set( KProfileCreated );
			break;
			}
		case ETcProfileUpdated:
			{
			state.Set( KProfileUpdated );
			break;
			}
		case ETcProfileRegistrationStatusChanged:
			{
			state.Set( KProfileRegistrationStatusChanged );
			break;
			}
		case ETcProfileDestroyed:
			{
			state.Set( KProfileDestroyed );
			break;
			}
		case ETcProfileRegistryErrorOccurred:
			{
			state.Set( KProfileRegistryErrorOccurred );
			break;
			}
		default:
			{
			state.Set( KUnknown );
			break;
			}
		}
	AddTextResponseL( KParamProfileState, state );
	
	TPtrC8 regStatus;
	switch( item.iRegStatus )
		{
		case ETcStatusUnregistered:
			{
			regStatus.Set( KProfileUnregistered );
			break;
			}
		case ETcStatusRegistered:
			{
			regStatus.Set( KProfileRegistered );
			break;
			}
		default:
		    {
		    regStatus.Set( KUnknown );
		    break;
		    }
		}
	AddTextResponseL( KParamProfileRegStatus, regStatus );
	}


TBool TCmdGetProfileStatus::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("GetProfileStatus") );
	}

TTcCommandBase* TCmdGetProfileStatus::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetProfileStatus( aContext );
	}

