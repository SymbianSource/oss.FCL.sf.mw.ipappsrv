/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
#include "TCmdCreateSecureSession.h"
#include "CTcMCEContext.h"

#include <sipprofile.h>
#include "TCLog.h"
#include <MceSession.h>
#include "mcesecureoutsession.h"

class CMceManager;
	
// ----------------------------------------------------------------------------
// TCmdCreateSecureSession::ExecuteL
// ----------------------------------------------------------------------------
//	
void TCmdCreateSecureSession::ExecuteL()
	{
	TcLog::Write( _L8("\nTCmdCreateSecureSession::ExecuteL()") );
	CreateOutSessionL();	
	}

// ----------------------------------------------------------------------------
// TCmdCreateSecureSession::CreateSessionL
// ----------------------------------------------------------------------------
//	
CMceSession* TCmdCreateSecureSession::CreateSessionL( CMceManager& aManager,
                        						CSIPProfile& aProfile,
                        						const TDesC8& aRecipient,
												HBufC8* aOriginator  )
	{
 	return  CMceSecureOutSession::NewL( aManager, aProfile, 
							   	  aRecipient, aOriginator );	
	}

// ----------------------------------------------------------------------------
// TCmdCreateSecureSession::CreateSessionL
// ----------------------------------------------------------------------------
//	
CMceSession* TCmdCreateSecureSession::CreateSessionL( CMceEvent& aEvent )
	{
 	return CMceSecureOutSession::NewL( aEvent );	
	}

// ----------------------------------------------------------------------------
// TCmdCreateSecureSession::CreateSessionL
// ----------------------------------------------------------------------------
//	
CMceSession* TCmdCreateSecureSession::CreateSessionL( CMceRefer& aRefer )
	{
 	 return CMceSecureOutSession::NewL( aRefer );	    
	}

// ----------------------------------------------------------------------------
// TCmdCreateSecureSession::Match
// ----------------------------------------------------------------------------
//	
TBool TCmdCreateSecureSession::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("CreateSecureSession") );
	}

// ----------------------------------------------------------------------------
// TCmdCreateSecureSession::CreateL
// ----------------------------------------------------------------------------
//
TTcCommandBase* TCmdCreateSecureSession::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdCreateSecureSession( aContext );
	}

