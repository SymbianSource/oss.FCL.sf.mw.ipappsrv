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

#include "TMCETestUIEngineCmdCreateEvent.h"
#include "CMCETestUIEngineOutEvent.h"
#include "CMCETestUIQuestionSingleSelection.h"
#include "CMCETestUIEngineProfile.h"
#include "CMCETestUIEngineRefer.h"
#include <MceRefer.h>


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateEvent::Caption
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdCreateEvent::TMCETestUIEngineCmdCreateEvent(
		            CMCETestUIEngine& aEngine )
    : TMCETestUIEngineCmdBase( aEngine ),
      iProfile( NULL )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateEvent::TMCETestUIEngineCmdCreateEvent
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdCreateEvent::TMCETestUIEngineCmdCreateEvent(
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineProfile& aProfile )
    : TMCETestUIEngineCmdBase( aEngine ),
      iProfile( &aProfile )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateEvent::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdCreateEvent::ExecuteL()
	{

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
	    iEngine.CreateOutEventL( *usedProfile );
        }
    else
        {
        iEngine.NotifyUser( KUserNotificationEventCannotCreate );
        }
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateEvent::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdCreateEvent::Caption() const
	{
	return KCommandCaptionCreateEvent;
	}



// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateRefer::Caption
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdCreateRefer::TMCETestUIEngineCmdCreateRefer(
		            CMCETestUIEngine& aEngine )
    : TMCETestUIEngineCmdBase( aEngine ),
      iProfile( NULL )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateRefer::TMCETestUIEngineCmdCreateRefer
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdCreateRefer::TMCETestUIEngineCmdCreateRefer(
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineProfile& aProfile )
    : TMCETestUIEngineCmdBase( aEngine ),
      iProfile( &aProfile )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateRefer::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdCreateRefer::ExecuteL()
	{

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
	    iEngine.CreateOutReferL( *usedProfile );
        }
    else
        {
        iEngine.NotifyUser( KUserNotificationReferCannotCreate );
        }
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateRefer::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdCreateRefer::Caption() const
	{
	return KCommandCaptionCreateRefer;
	}


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateRefer::TMCETestUIEngineCmdCreateReferSession
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdCreateReferSession::TMCETestUIEngineCmdCreateReferSession(
		            CMCETestUIEngine& aEngine )
    : TMCETestUIEngineCmdBase( aEngine ),
      iSession( NULL )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateReferSession::TMCETestUIEngineCmdCreateReferSession
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdCreateReferSession::TMCETestUIEngineCmdCreateReferSession(
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSession& aSession )
    : TMCETestUIEngineCmdBase( aEngine ),
      iSession( &aSession )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateReferSession::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdCreateReferSession::ExecuteL()
	{
    iEngine.CreateOutReferL( *iSession );
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateReferSession::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdCreateReferSession::Caption() const
	{
	return KCommandCaptionCreateRefer;
	}
// End of File

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateReferEvent::TMCETestUIEngineCmdCreateReferEvent
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdCreateReferEvent::TMCETestUIEngineCmdCreateReferEvent(
		            CMCETestUIEngine& aEngine )
    : TMCETestUIEngineCmdBase( aEngine ),
      iEvent( NULL )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateReferEvent::TMCETestUIEngineCmdCreateReferEvent
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdCreateReferEvent::TMCETestUIEngineCmdCreateReferEvent(
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineEvent& aEvent )
    : TMCETestUIEngineCmdBase( aEngine ),
      iEvent( &aEvent )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateReferEvent::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdCreateReferEvent::ExecuteL()
	{
    iEngine.CreateOutReferL( *iEvent );
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateReferEvent::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdCreateReferEvent::Caption() const
	{
	return KCommandCaptionCreateRefer;
	}
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateSessionFromRefer::TMCETestUIEngineCmdCreateSessionFromRefer
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdCreateSessionFromRefer::TMCETestUIEngineCmdCreateSessionFromRefer(
		            CMCETestUIEngine& aEngine )
    : TMCETestUIEngineCmdBase( aEngine ),
      iRefer( NULL )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateSessionFromRefer::TMCETestUIEngineCmdCreateSessionFromRefer
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdCreateSessionFromRefer::TMCETestUIEngineCmdCreateSessionFromRefer(
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineRefer& aRefer )
    : TMCETestUIEngineCmdBase( aEngine ),
      iRefer( &aRefer )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateSessionFromRefer::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C  void TMCETestUIEngineCmdCreateSessionFromRefer::ExecuteL()
	{
    iEngine.CreateOutSessionL( iRefer->Refer());
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateSessionFromRefer::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdCreateSessionFromRefer::Caption() const
	{
	return KCommandCaptionCreateSessionFromRefer;
	}
// End of File
	
