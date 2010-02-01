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
* Description:    Implementation.
*
*/





// INCLUDES
#include "CMCETestUIEngineProfileContainer.h"
#include "MCETestUIEngineDefaultValues.h"
#include "CMCETestUIEngineProfile.h"
#include "sipprofile.h"
#include "MCETestUIEngineConstants.h"
#include "MCETestUIDebug.h"


// -----------------------------------------------------------------------------
// CMCETestUIEngineProfileContainer::NewL
// -----------------------------------------------------------------------------
//
CMCETestUIEngineProfileContainer* CMCETestUIEngineProfileContainer::NewL(
            CMCETestUIEngine& aEngine, CSIP& aSip )
	{
	CMCETestUIEngineProfileContainer* self =
            new( ELeave ) CMCETestUIEngineProfileContainer( aEngine, aSip );												 

	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );

	return self;
	}

// -----------------------------------------------------------------------------
// CMCETestUIEngineProfileContainer::~CMCETestUIEngineProfileContainer
// -----------------------------------------------------------------------------
//
CMCETestUIEngineProfileContainer::~CMCETestUIEngineProfileContainer()
	{
    iProfiles.ResetAndDestroy();
    iUsedProfiles.ResetAndDestroy();
    
	delete iProfileRegistry;
	}

// -----------------------------------------------------------------------------
// CMCETestUIEngineProfileContainer::CMCETestUIEngineProfileContainer
// -----------------------------------------------------------------------------
//
CMCETestUIEngineProfileContainer::CMCETestUIEngineProfileContainer( 
            CMCETestUIEngine& aEngine, CSIP& aSip )
	: iEngine( aEngine ), 
	iProfiles( 2 ), 
	iSip( aSip )
	{
	}

// -----------------------------------------------------------------------------
// CMCETestUIEngineProfileContainer::ConstructL
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineProfileContainer::ConstructL()
	{
	iProfileRegistry = CSIPProfileRegistry::NewL( iSip, *this ); 

    // Instantiate and store all profiles
    RPointerArray<CSIPProfile> profiles;
    iProfileRegistry->ProfilesL( profiles );

    for ( TInt i = 0; i < profiles.Count(); ++i )
        {
        CMCETestUIEngineProfile* item = 
            CMCETestUIEngineProfile::NewL( profiles[i], iEngine );
        CleanupStack::PushL( item );
        User::LeaveIfError( iProfiles.Append( item ) );
        CleanupStack::Pop( item );
        }
     
    profiles.Reset();
    
    if ( MCETestUIEngineDefaultValues::TakeDefaultProfileIntoUse() )
        {
        TakeProfileIntoUseL( GetDefaultProfileL() );
        }
    
	}


//
// -- MSIPProfileRegistryObserver functions -----------------------------------
//

// -----------------------------------------------------------------------------
// CMCETestUIEngineProfileContainer::ProfileRegistryEventOccurred
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineProfileContainer::ProfileRegistryEventOccurred(
        TUint32 /*aProfileId*/, TEvent aEvent)
    {	
    
    TPtrC aMessage; 
    MCE_TESTUI_DEBUG3( "CMCETestUIEngineProfileContainer::ProfileRegistryEventOccurred", aEvent );
	switch (aEvent)
	    {
	    case MSIPProfileRegistryObserver::EProfileUpdated:
	        {
	        aMessage.Set( KUserNotificationProfileUpdated );
	        break;
	        }
	    case MSIPProfileRegistryObserver::EProfileRegistered:
	        {
            aMessage.Set( KUserNotificationProfileRegistered );
	        break;
	        }
	    case MSIPProfileRegistryObserver::EProfileDeregistered:
	        {
            aMessage.Set( KUserNotificationProfileDeregistered );
	        break;
	        }
	    /*
	    case MSIPProfileRegistryObserver::EProfileCreated:
	        {
	        break;
	        }
	    case MSIPProfileRegistryObserver::EProfileDestroyed:
	        {
	        break;
	        }
	    */
	    default:
	        {
	        aMessage.Set( KUserNotificationProfileUnknownEvent );
	        break;
	        }
	    }
	    
    iEngine.NotifyUser( aMessage );
    iEngine.EngineStateChangedL();

    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineProfileContainer::ProfileRegistryErrorOccurred
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineProfileContainer::ProfileRegistryErrorOccurred(
        TUint32 /*aProfileId*/, TInt /*aError*/ )
	{
	// Not implemented yet
	}

//
// -- MSIPConnectionObserver dummy functions
//

void CMCETestUIEngineProfileContainer::IncomingRequest (CSIPServerTransaction* /*aTransaction*/) 
	{
	// NOP
	}
void CMCETestUIEngineProfileContainer::IncomingRequest (CSIPServerTransaction* /*aTransaction*/,
			          CSIPDialog& /*aDialog*/) 
	{
	// NOP
	}
void CMCETestUIEngineProfileContainer::IncomingResponse (CSIPClientTransaction& /*aTransaction*/) 
	{
	// NOP
	}
void CMCETestUIEngineProfileContainer::IncomingResponse (CSIPClientTransaction& /*aTransaction*/,
					   CSIPDialogAssocBase& /*aDialogAssoc*/) 
	{
	// NOP
	}
void CMCETestUIEngineProfileContainer::IncomingResponse (CSIPClientTransaction& /*aTransaction*/,
					   CSIPInviteDialogAssoc* /*aDialogAssoc*/) 
	{
	// NOP
	}
void CMCETestUIEngineProfileContainer::IncomingResponse (CSIPClientTransaction& /*aTransaction*/,
                       CSIPRegistrationBinding& /*aRegistration*/) 
    {
    // NOP
    }
void CMCETestUIEngineProfileContainer::ErrorOccured (TInt /*aError*/,
                   CSIPTransactionBase& /*aTransaction*/) 
    {
    // NOP
    }
void CMCETestUIEngineProfileContainer::ErrorOccured (TInt /*aError*/,
		           CSIPClientTransaction& /*aTransaction*/,
	               CSIPRegistrationBinding& /*aRegistration*/) 
	{
	// NOP
	}
void CMCETestUIEngineProfileContainer::ErrorOccured (TInt /*aError*/,
				   CSIPTransactionBase& /*aTransaction*/,
				   CSIPDialogAssocBase& /*aDialogAssoc*/) 
	{
	// NOP
	}
void CMCETestUIEngineProfileContainer::ErrorOccured (TInt /*aError*/, CSIPRefresh& /*aSIPRefresh*/) 
	{
	// NOP
	}
void CMCETestUIEngineProfileContainer::ErrorOccured (TInt /*aError*/,	
										   CSIPRegistrationBinding& /*aRegistration*/) 
	{
	// NOP
	}
void CMCETestUIEngineProfileContainer::ErrorOccured (TInt /*aError*/,			
					               CSIPDialogAssocBase& /*aDialogAssoc*/) 
	{
	// NOP
	}
void CMCETestUIEngineProfileContainer::InviteCompleted (CSIPClientTransaction& /*aTransaction*/) 
	{
	// NOP
	}
void CMCETestUIEngineProfileContainer::InviteCanceled (CSIPServerTransaction& /*aTransaction*/) 
	{
	// NOP
	}
void CMCETestUIEngineProfileContainer::ConnectionStateChanged (CSIPConnection::TState /*aState*/) 
	{
	// NOP
	}

//
// -- Public functions --------------------------------------------------------
//
 

// -----------------------------------------------------------------------------
// CMCETestUIEngineProfileContainer::AvailableProfiles
// -----------------------------------------------------------------------------
// 
const RPointerArray<CMCETestUIEngineProfile>& 
        CMCETestUIEngineProfileContainer::AvailableProfiles()
    {
    return iProfiles;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineProfileContainer::UsedProfiles
// -----------------------------------------------------------------------------
//
const RPointerArray<CMCETestUIEngineProfile>& 
        CMCETestUIEngineProfileContainer::UsedProfiles()
    {
    return iUsedProfiles;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineProfileContainer::IsProfileInUse
// -----------------------------------------------------------------------------
//
TBool CMCETestUIEngineProfileContainer::IsProfileInUse( 
        CMCETestUIEngineProfile& aProfile )
    {
    for ( TInt i = 0; i < iUsedProfiles.Count(); ++i )
        {
        if ( iUsedProfiles[i] == &aProfile )
            {
            return ETrue;
            }
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineProfileContainer::TakeProfileIntoUseL
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineProfileContainer::TakeProfileIntoUseL( 
        CMCETestUIEngineProfile& aProfile )
    {
    // for every available profile
    for ( TInt i = 0; i < iProfiles.Count(); ++i )
        {
        // check if it is the one to be taken into use
        if ( iProfiles[i] == &aProfile )
            {
            // check that this is not allready in use
            if ( !IsProfileInUse( aProfile ) )
                {
                iUsedProfiles.AppendL( iProfiles[i] );
                iProfiles.Remove( i );
                iProfiles.Compress();
                
                if ( MCETestUIEngineDefaultValues::RegisterUsedProfiles() )
                    {
                    iProfileRegistry->EnableL( aProfile.Profile(), *this );
                    }
                iEngine.EngineStateChangedL();
                }
            }
        }
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineProfileContainer::GetDefaultProfileL
// -----------------------------------------------------------------------------
//
CMCETestUIEngineProfile& CMCETestUIEngineProfileContainer::GetDefaultProfileL()
    {
    for ( TInt i = 0; i < iProfiles.Count(); ++i )
        {
        if ( iProfiles[i]->IsDefault() )
            {
            return *(iProfiles[i]);
            }
        }
        
    for ( TInt i = 0; i < iUsedProfiles.Count(); ++i )
        {
        if ( iUsedProfiles[i]->IsDefault() )
            {
            return *(iUsedProfiles[i]);
            }
        }
        	
    User::Leave( KErrNotFound );
    return *(CMCETestUIEngineProfile::NewL( NULL, iEngine )); // To keep compiler happy;
    }


// End of file







