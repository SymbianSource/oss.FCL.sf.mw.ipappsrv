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
#include "TCmdSendDeRegister.h"
#include "CTcMCEContext.h"
#include "CTcMCEProfileContainer.h"
#include <sipprofile.h>

void TCmdSendDeRegister::ExecuteL()
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
	//Here add unregister
	TInt err = profileContainer.DisableProfileL(profileId);

	// ---------- Response creation -----------------------
 	if(err==0)
 		{
		AddIdResponseL(KProfileObj, *profile);
		AddIntegerResponseL(KResponseProfileId, profileId);	
 		}
 	else
 		{
 		AddIdResponseL(KProfileObj, *profile);
		AddIntegerResponseL(KResponseProfileId, profileId);	
		AddIntegerResponseL(KResponseError, err);	
 		}

	}
	
TBool TCmdSendDeRegister::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("SendDeRegister") );
	}

TTcCommandBase* TCmdSendDeRegister::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendDeRegister( aContext );
	}
