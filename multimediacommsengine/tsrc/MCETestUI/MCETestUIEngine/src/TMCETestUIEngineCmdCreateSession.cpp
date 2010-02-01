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

#include "TMCETestUIEngineCmdCreateSession.h"
#include "CMCETestUIEngineOutSession.h"
#include "CMCETestUIQuestionSingleSelection.h"
#include "CMCETestUIEngineProfile.h"


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateSession::Caption
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdCreateSession::TMCETestUIEngineCmdCreateSession(
		            CMCETestUIEngine& aEngine )
    : TMCETestUIEngineCmdBase( aEngine ),
      iProfile( NULL )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateSession::TMCETestUIEngineCmdCreateSession
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdCreateSession::TMCETestUIEngineCmdCreateSession(
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineProfile& aProfile )
    : TMCETestUIEngineCmdBase( aEngine ),
      iProfile( &aProfile )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateSession::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdCreateSession::ExecuteL()
	{
	iEngine.iOutSessionWithProfile = ETrue;        
    iEngine.iOutSessionWithAPList = EFalse;
    iEngine.iOutSessionWithAPNumber = EFalse;
	TSessionType sessionType = ECustomSession;	
	        
	TDirection direction = ENotSpesified;
	        
	TVideoDirection videoDirection = EVideoNotSpesified;

    // Solve used profile
    CMCETestUIEngineProfile* usedProfile = iProfile;
    
    if ( !usedProfile )
        {
        const RPointerArray<CMCETestUIEngineProfile>& profiles = 
                        iEngine.UsedProfiles();

        RPointerArray<CMCETestUIEngineProfile> registeredProfiles;
        CleanupClosePushL( registeredProfiles );    
        for ( TInt i = 0; i < profiles.Count(); ++i )
            {
            if ( profiles[i]->IsRegistered() )
                {
                registeredProfiles.AppendL( profiles[i] );
                }
            }

        if ( registeredProfiles.Count() == 1 )
            {
            usedProfile = registeredProfiles[ 0 ];
            }
        else
            {         
        	CMCETestUIQuestionSingleSelection* profileQuestion = 
    	            CMCETestUIQuestionSingleSelection::NewLC();        
    	    profileQuestion->SetCaptionL( KUserQuestionSelectUsedProfile );   
            
     
            
            for ( TInt i = 0; i < registeredProfiles.Count(); ++i )
                {      
                const TDesC8& providerName = registeredProfiles[i]->ProviderName();    
                HBufC* providerName16 = HBufC16::NewLC( providerName.Length() );
                TPtr providerNamePtr = providerName16->Des();
                providerNamePtr.Copy( providerName );
                
                profileQuestion->AddChoiceL( *providerName16 );
                
                CleanupStack::PopAndDestroy( providerName16 );
                providerName16 = NULL;  
                }
            
            TInt index = iEngine.AskQuestionL( *profileQuestion );
                
            CleanupStack::PopAndDestroy( profileQuestion ); 
            
            if ( index >= 0 && index < registeredProfiles.Count() )
                {
                usedProfile = registeredProfiles[ index ];
                }
                
            }
        CleanupStack::PopAndDestroy(); // registeredProfiles
        }
    
    if ( usedProfile )
        {
	    iEngine.CreateOutSessionL( *usedProfile, direction,videoDirection,sessionType  );
        }
    else
        {
        iEngine.NotifyUser( KUserNotificationSessionCannotCreate );
        }
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateSession::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdCreateSession::Caption() const
	{
	return KCommandCaptionCreateCustomSession;
	}


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateSessionUsingAPList::Caption
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdCreateSessionUsingAPList::TMCETestUIEngineCmdCreateSessionUsingAPList(
		            CMCETestUIEngine& aEngine )
    : TMCETestUIEngineCmdBase( aEngine )
    {
    }


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateSessionUsingAPList::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdCreateSessionUsingAPList::ExecuteL()
	{

	iEngine.iOutSessionWithProfile = EFalse;        
    iEngine.iOutSessionWithAPList = ETrue;
    iEngine.iOutSessionWithAPNumber = EFalse;
	TSessionType sessionType = ECustomSession;	
	        
	TDirection direction = ENotSpesified;
	        
	TVideoDirection videoDirection = EVideoNotSpesified;
	
//	TUint iapId;
 	iEngine.GetIapIdFromAPList();
    iEngine.CreateOutSessionL( iEngine.iIapId, direction,videoDirection,sessionType  );
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateSessionUsingAPList::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdCreateSessionUsingAPList::Caption() const
	{
	return KCommandCaptionCreateCustomSessionAPList;
	}



// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateSessionUsingAPNumber::TMCETestUIEngineCmdCreateSessionUsingAPNumber
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdCreateSessionUsingAPNumber::TMCETestUIEngineCmdCreateSessionUsingAPNumber(
		            CMCETestUIEngine& aEngine)
    : TMCETestUIEngineCmdBase( aEngine )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateSessionUsingAPNumber::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdCreateSessionUsingAPNumber::ExecuteL()
	{
	iEngine.iOutSessionWithProfile = EFalse;        
    iEngine.iOutSessionWithAPList = EFalse;
    iEngine.iOutSessionWithAPNumber = ETrue;
	TSessionType sessionType = ECustomSession;	
	        
	TDirection direction = ENotSpesified;
	        
	TVideoDirection videoDirection = EVideoNotSpesified;

    TInt iapId;
    iEngine.GetIapIdL(iapId);
    iEngine.CreateOutSessionL( iapId, direction,videoDirection,sessionType  );
       
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateSessionUsingAPNumber::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdCreateSessionUsingAPNumber::Caption() const
	{
	return KCommandCaptionCreateCustomSessionAPNumber;
	}


// End of File
