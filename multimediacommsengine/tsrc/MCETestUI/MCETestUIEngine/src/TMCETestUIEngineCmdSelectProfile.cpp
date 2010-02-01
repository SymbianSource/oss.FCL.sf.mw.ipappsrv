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
* Description:    Implementation.
*
*/




// INCLUDES

#include "TMCETestUIEngineCmdSelectProfile.h"
#include "CMCETestUIEngineProfileContainer.h"
#include "CMCETestUIQuestionSingleSelection.h"
#include "CMCETestUIEngineProfile.h"


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdSelectProfile::TMCETestUIEngineCmdSelectProfile
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdSelectProfile::TMCETestUIEngineCmdSelectProfile( 
        CMCETestUIEngine& aEngine )
    :TMCETestUIEngineCmdBase( aEngine )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdSelectProfile::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdSelectProfile::ExecuteL()
	{

	CMCETestUIEngineProfileContainer& profileContainer = 
	        iEngine.ProfileContainer();
	
	const RPointerArray<CMCETestUIEngineProfile>& profiles = 
	        profileContainer.AvailableProfiles();
	        
	CMCETestUIQuestionSingleSelection* question = 
	        CMCETestUIQuestionSingleSelection::NewLC();

    question->SetCaptionL( KUserQuestionSelectProfile );
   
    for ( TInt i = 0; i < profiles.Count(); ++i )
        {
        const TDesC8& providerName = profiles[i]->ProviderName();    
        HBufC* providerName16 = HBufC16::NewLC( providerName.Length() );
        TPtr providerNamePtr = providerName16->Des();
        providerNamePtr.Copy( providerName );
        
        question->AddChoiceL( *providerName16 );
        
        CleanupStack::PopAndDestroy( providerName16 );
        providerName16 = NULL;
        }
 
    TInt selectedProfile = iEngine.AskQuestionL( *question );
	        	
	CleanupStack::PopAndDestroy( question );
    
    if ( selectedProfile >= 0 && selectedProfile < profiles.Count() )
        {
	    profileContainer.TakeProfileIntoUseL( *(profiles[selectedProfile]) );
        }
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdSelectProfile::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdSelectProfile::Caption() const
	{
	return KCommandCaptionSelectProfile;
	}




// End of File
