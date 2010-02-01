/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
#include "TCmdSendRegister.h"
#include "CTcMCEContext.h"
#include "CTcMCEProfileContainer.h"
#include <sipprofile.h>

void TCmdSendRegister::ExecuteL()
	{	
	// ---------- Setup ------------------------------------
	
	CTcMCEProfileContainer& profileContainer = SelectProfileL();	
	
	// ---------- Execution --------------------------------
	
	TInt profileId = ExtractProfileIdL();
	
	CSIPProfile* profile = NULL;
	if (profileId == 0) 
		{
		profile = &(profileContainer.GetDefaultProfileL());
		}
	else
		{
		profile = &(profileContainer.GetProfileByIdL(profileId)); 
		}
	    
	// Fetch real profileId	
	TUint32 longProfileId = 0;
	profile->GetParameter( KSIPProfileId, longProfileId );
	
	// SendRegister, wait until succeeds
	profileId = longProfileId;
	profile = &(profileContainer.RegisterProfileL(profileId));

	// ---------- Response creation -----------------------
 
	AddIdResponseL(KProfileObj, *profile);
	AddIntegerResponseL(KResponseProfileId, profileId);	

	}
	
TBool TCmdSendRegister::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("SendRegister") );
	}

TTcCommandBase* TCmdSendRegister::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendRegister( aContext );
	}
