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




#include "CTcMCEProfileContainer.h"
#include "CTcMCEContext.h"
#include "MCEConstants.h"
#include "sipprofile.h"
#include "e32base.h"

const TInt KReceiveQueueGranularity( 1 );
const TInt KProfileArrayGranularity( 2 );

CTcMCEProfileContainer* CTcMCEProfileContainer::NewL( CTcMCEContext& aContext,
													  CSIP& aSip )
	{
	CTcMCEProfileContainer* self =
		new( ELeave ) CTcMCEProfileContainer( aContext, aSip );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );

	return self;
	}

CTcMCEProfileContainer::~CTcMCEProfileContainer()
	{
    iProfiles.ResetAndDestroy();
    
	delete iProfileRegistry;
	iReceiveQueue.Reset();
	}

CTcMCEProfileContainer::CTcMCEProfileContainer( CTcMCEContext& aContext,
												CSIP& aSip ) :
    iContext( aContext ),
    iSip( aSip ),
    iReceiveQueue( KReceiveQueueGranularity ),
    iProfiles( KProfileArrayGranularity )
	{
	}

void CTcMCEProfileContainer::ConstructL()
	{
	iProfileRegistry = CSIPProfileRegistry::NewL( iSip, *this ); 
	}



//
// -- MSIPProfileRegistryObserver functions -----------------------------------
//

void CTcMCEProfileContainer::ProfileRegistryEventOccurred(TUint32 aProfileId,
														  TEvent aEvent)
    {	
	TTcProfileReceived received;
	received.iProfileId = aProfileId;
	
	switch (aEvent)
	    {
	    case MSIPProfileRegistryObserver::EProfileCreated:
	        {
	        received.iEvent = ETcProfileCreated;
	        break;
	        }
	    case MSIPProfileRegistryObserver::EProfileUpdated:
	        {
	        received.iEvent = ETcProfileUpdated;
	        break;
	        }
	    case MSIPProfileRegistryObserver::EProfileRegistered:
	    	{
	    	CSIPProfile& profile = GetProfileByIdL(aProfileId);
	    	// Expect to be the right profile
	    	if (iWait.IsStarted()) 
	   			{
	   			//CActiveScheduler::Stop();
	   			iWait.AsyncStop();
	   			}

	    	// The control returns to CTcMCEProfileContainer::RegisterProfileL
	    	break;
	    	}
	    case MSIPProfileRegistryObserver::EProfileDeregistered:
	        {
	        received.iEvent = ETcProfileRegistrationStatusChanged;
	        break;
	        }
	    case MSIPProfileRegistryObserver::EProfileDestroyed:
	        {
	        received.iEvent = ETcProfileDestroyed;
	        break;
	        }
	    default:
	        {
	        received.iEvent = ETcUnknown;
	        break;
	        }
	    }
	QueueProfileReceived( received );
    }

void CTcMCEProfileContainer::ProfileRegistryErrorOccurred(TUint32 aProfileId,
														  TInt aError )
	{
	TTcProfileReceived received;
	received.iProfileId = aProfileId;
	received.iEvent = ETcProfileRegistryErrorOccurred;
	received.iError = aError;
	QueueProfileReceived( received );
	CSIPProfile& profile = GetProfileByIdL(aProfileId);
	if (iWait.IsStarted()) 
	   	{
	   	//CActiveScheduler::Stop();
	   	iWait.AsyncStop();
	   	}
	}

//
// -- MSIPConnectionObserver dummy functions
//

void CTcMCEProfileContainer::IncomingRequest (CSIPServerTransaction* /*aTransaction*/) 
	{
	// NOP
	}
void CTcMCEProfileContainer::IncomingRequest (CSIPServerTransaction* /*aTransaction*/,
			          CSIPDialog& /*aDialog*/) 
	{
	// NOP
	}
void CTcMCEProfileContainer::IncomingResponse (CSIPClientTransaction& /*aTransaction*/) 
	{
	// NOP
	}
void CTcMCEProfileContainer::IncomingResponse (CSIPClientTransaction& /*aTransaction*/,
					   CSIPDialogAssocBase& /*aDialogAssoc*/) 
	{
	// NOP
	}
void CTcMCEProfileContainer::IncomingResponse (CSIPClientTransaction& /*aTransaction*/,
					   CSIPInviteDialogAssoc* /*aDialogAssoc*/) 
	{
	// NOP
	}
void CTcMCEProfileContainer::IncomingResponse (CSIPClientTransaction& /*aTransaction*/,
                       CSIPRegistrationBinding& /*aRegistration*/) 
    {
    // NOP
    }
void CTcMCEProfileContainer::ErrorOccured (TInt /*aError*/,
                   CSIPTransactionBase& /*aTransaction*/) 
    {
    // NOP
    }
void CTcMCEProfileContainer::ErrorOccured (TInt /*aError*/,
		           CSIPClientTransaction& /*aTransaction*/,
	               CSIPRegistrationBinding& /*aRegistration*/) 
	{
	// NOP
	}
void CTcMCEProfileContainer::ErrorOccured (TInt /*aError*/,
				   CSIPTransactionBase& /*aTransaction*/,
				   CSIPDialogAssocBase& /*aDialogAssoc*/) 
	{
	// NOP
	}
void CTcMCEProfileContainer::ErrorOccured (TInt /*aError*/, CSIPRefresh& /*aSIPRefresh*/) 
	{
	// NOP
	}
void CTcMCEProfileContainer::ErrorOccured (TInt /*aError*/,	
										   CSIPRegistrationBinding& /*aRegistration*/) 
	{
	// NOP
	}
void CTcMCEProfileContainer::ErrorOccured (TInt /*aError*/,			
					               CSIPDialogAssocBase& /*aDialogAssoc*/) 
	{
	// NOP
	}
void CTcMCEProfileContainer::InviteCompleted (CSIPClientTransaction& /*aTransaction*/) 
	{
	// NOP
	}
void CTcMCEProfileContainer::InviteCanceled (CSIPServerTransaction& /*aTransaction*/) 
	{
	// NOP
	}
void CTcMCEProfileContainer::ConnectionStateChanged (CSIPConnection::TState /*aState*/) 
	{
	// NOP
	}

//
// -- Public functions --------------------------------------------------------
//

TTcProfileReceived CTcMCEProfileContainer::ReceivedProfileItemL( TInt /*aTimeout*/ )
	{
	// Get the first (oldest) item from the array
    TTcProfileReceived item = iReceiveQueue[ 0 ];

	// ..and remove it from the array
	iReceiveQueue.Delete( 0 );
	iReceiveQueue.Compress();
	return item;
	}

void CTcMCEProfileContainer::AddProfileL( CSIPProfile* aProfile )
    {
    CleanupStack::PushL( aProfile );
    
    TUint32 id( 0 );
	User::LeaveIfError( aProfile->GetParameter( KSIPProfileId, id ) );
	
	// Array may contain multiple instances with same profileId
    CTcSIPProfile* tcprofile =
    	new ( ELeave ) CTcSIPProfile( aProfile, (TInt)id );
    CleanupStack::Pop( aProfile );
    CleanupStack::PushL( tcprofile );
    User::LeaveIfError( iProfiles.Append( tcprofile ) );
    CleanupStack::Pop( tcprofile );
    }

CSIPProfile& CTcMCEProfileContainer::GetDefaultProfileL()
    {
	CSIPProfile* profile =
		static_cast< CSIPProfile* >( ProfileRegistry().DefaultProfileL() );

    AddProfileL( profile );
        
    return *profile;			
    }

CSIPProfile& CTcMCEProfileContainer::GetProfileByIdL( TInt aProfileId )
    {
    CSIPProfile* profile = 0;
    
    // Check whether profile is already stored into our internal array
    TInt count = iProfiles.Count();
    for( TInt i=0; i < count; i++ )
        {
        if( iProfiles[ i ]->ProfileId() == aProfileId )
            {
            return iProfiles[ i ]->Profile();
            }
        
        }
    // Profile wasn't found from internal array, create new instance through
    // registry and store it 
    profile = ProfileRegistry().ProfileL( aProfileId );
    
    if ( !profile )
        {
        User::Leave( KErrNotFound );
        }
        
    AddProfileL( profile );
    
    return *profile;
    }

CSIPProfile& CTcMCEProfileContainer::RegisterProfileL(TInt aProfileId)
    {
    CSIPProfile& profile = GetProfileByIdL(aProfileId);
    ProfileRegistry().EnableL(profile, *this);
	if (!profile.IsContextActive()) 
	{
		 //CActiveScheduler::Start();
		 iWait.Start();
	}
   
    // After calling CActiveScheduler::Stop() the control returns here 
    return profile;
    }


TInt CTcMCEProfileContainer::DisableProfileL(TInt aProfileId)
	{
	CSIPProfile& profile = GetProfileByIdL(aProfileId);
    TInt err=0;
	if (profile.IsContextActive()) 
		{
		err = ProfileRegistry().Disable(profile);
		}
   
    // After calling CActiveScheduler::Stop() the control returns here 
    return err;
	}
//
// -- Internal utility functions ----------------------------------------------
//

void CTcMCEProfileContainer::QueueProfileReceived( TTcProfileReceived& aItem )
	{

	TRAPD( err, iReceiveQueue.AppendL( aItem ) );
	__ASSERT_ALWAYS( !err, User::Panic( KMceErrOOMInObserver, KErrNoMemory ) );
	}

//
// -- Helper classes ----------------------------------------------------------
//


CTcSIPProfile::CTcSIPProfile( CSIPProfile* aProfile, TInt aProfileId )
    {
	iProfile = aProfile;
	iProfileId = aProfileId;
    }

CTcSIPProfile::~CTcSIPProfile()
    {
    delete iProfile;
    }           

CSIPProfile& CTcSIPProfile::Profile()
    {
    return *iProfile;
    }
            
TInt CTcSIPProfile::ProfileId()
	{
    return iProfileId;
    }
