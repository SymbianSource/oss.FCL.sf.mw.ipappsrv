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
* Description:    Implementation
*
*/



#include "MCEConstants.h"
#include "TCmdSetObserver.h"
#include "CTcMCEContext.h"
#include "CTcMCEMultimediaAPIObserver.h"
#include "CTcMCEEventAPIObserver.h"

// -----------------------------------------------------------------------------
// TCmdSetObserver::ExecuteL
// -----------------------------------------------------------------------------
//
void TCmdSetObserver::ExecuteL()
	{

	// ---------- Setup --------------------------------------------------------

	TInt observerId = ExtractIntegerL( KParamObserverId );
	// ETrue if observer is to be set, EFalse if it is to be cleared
	TBool setObserver = ExtractBooleanL( KParamSetObserver );

	// ---------- Execution ----------------------------------------------------

	CTcMCEMultimediaAPIObserver* mmObserver( NULL );
	CTcMCEEventAPIObserver* eventObserver( NULL );
	if ( setObserver )
		{
		mmObserver = &iContext.MultimediaObserver();
		eventObserver = &iContext.EventObserver();
		}

	CMceManager& mceManager = iContext.MCEManager();
	switch ( observerId )
		{
		case KSessionObserver:
			mceManager.SetSessionObserver( mmObserver );
			break;
			
		case KInSessionObserver:
			mceManager.SetInSessionObserver( mmObserver );
			break;

		case KMediaObserver:
			mceManager.SetMediaObserver( mmObserver );
			break;

		case KRtpObserver:
			mceManager.SetRtpObserver( mmObserver );
			break;
			
		case KTransactionObserver:
			mceManager.SetTransactionObserver( mmObserver );
			break;

		case KInTransactionObserver:
			mceManager.SetInTransactionObserver( mmObserver );
			break;
			
		case KEventObserver:
			mceManager.SetEventObserver( eventObserver );
			break;
			
		case KInEventObserver:
			mceManager.SetInEventObserver( eventObserver );
			break;
			
		case KReferObserver:
			mceManager.SetReferObserver( eventObserver );
			break;

		case KInReferObserver:
			mceManager.SetInReferObserver( eventObserver );
			break;

		default:
			User::Leave( KErrArgument );
		}

	// ---------- Response creation --------------------------------------------
    }

// -----------------------------------------------------------------------------
// TCmdSetObserver::Match
// -----------------------------------------------------------------------------
//
TBool TCmdSetObserver::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8( "SetObserver" ) );
	}

// -----------------------------------------------------------------------------
// TCmdSetObserver::CreateL
// -----------------------------------------------------------------------------
//
TTcCommandBase* TCmdSetObserver::CreateL( MTcTestContext& aContext )
	{
	return new ( ELeave ) TCmdSetObserver( aContext );
	}
