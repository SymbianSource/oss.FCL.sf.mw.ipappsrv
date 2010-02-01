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
* Description:  Implementation.
*
*/


#include "CTcSIPProfileContainer.h"
#include "TTcSIPReceived.h"
#include "CTcSIPContext.h"
#include "SipConstants.h"
#include "sipprofile.h"
#include "sipmanagedprofile.h"

#include "tclog.h"
#include "TesterConstants.h"
#include "ErrorHandling.h"

CTcSIPProfileContainer* CTcSIPProfileContainer::NewL(
													CTcSIPContext& aContext,
													CSIP& aSip )
	{
	CTcSIPProfileContainer* self =
						new( ELeave ) CTcSIPProfileContainer( aContext,
															  aSip );

	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );

	return self;
	}

CTcSIPProfileContainer::~CTcSIPProfileContainer()
	{
	TcLog::Write( _L8("CTcSIPContext::~CTcSIPContext() start\n") );

    iProfiles.ResetAndDestroy();
    iManagedProfiles.ResetAndDestroy();
    
	delete iProfileRegistry;
	delete iManagedProfileRegistry;
	delete iTimer;
	iReceiveQueue.Reset();

    TcLog::Write( _L8("CTcSIPContext::~CTcSIPContext() end\n") );
	}

CTcSIPProfileContainer::CTcSIPProfileContainer( CTcSIPContext& aContext, CSIP& aSip )
	: iContext( aContext ), iSip( aSip ), iReceiveQueue( 1 ), iProfiles( 2 ), iManagedProfiles( 2 )
	{
	}

void CTcSIPProfileContainer::ConstructL()
	{
	iTimer = CDeltaTimer::NewL( CActive::EPriorityStandard );
	TCallBack cb( ReceiveTimeout, this );
	iReceiveTimeout.Set( cb );

    TcLog::Write( _L8("CTcSIPProfileContainer::ConstructL() initializing SIP Profiles client\n") );
	// Could be used only MangedProfile or UserProfile..
    iManagedProfileRegistry = CSIPManagedProfileRegistry::NewL( *this );
	iProfileRegistry = CSIPProfileRegistry::NewL( iSip, *this ); 

    TcLog::Write( _L8("CTcSIPProfileContainer::ConstructL() end\n") );
	}


//
// -- MSIPProfileRegistryObserver functions -----------------------------------
//

void CTcSIPProfileContainer::ProfileRegistryEventOccurred(TUint32 aProfileId, TEvent aEvent)
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
	        received.iEvent = ETcProfileRegistrationStatusChanged;
	        received.iRegStatus = ETcStatusRegistered;
	        break;
	        }
	    case MSIPProfileRegistryObserver::EProfileDeregistered:
	        {
	        received.iEvent = ETcProfileRegistrationStatusChanged;
	        received.iRegStatus = ETcStatusUnregistered;
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

void CTcSIPProfileContainer::ProfileRegistryErrorOccurred(TUint32 aProfileId, TInt aError )
	{
	TTcProfileReceived received;
	received.iProfileId = aProfileId;
	received.iEvent = ETcProfileRegistryErrorOccurred;
	received.iError = aError;
	QueueProfileReceived( received );
	}

//
// -- Public functions --------------------------------------------------------
//

TTcProfileReceived CTcSIPProfileContainer::ReceivedProfileItemL( TInt aTimeout )
	{
	if( iReceiveQueue.Count() == 0 )
		{
		// wait for items to arrive
		iTimer->Queue( aTimeout * KSecondAsMicros, iReceiveTimeout );
		iActiveWait.Start();
		}

	// Is the queue still empty ? (i.e. timeout occurred)
	if( iReceiveQueue.Count() == 0 )
		{
		User::Leave( KErrTimedOut );
		}

	// Get the first (oldest) item from the array
    TTcProfileReceived item = iReceiveQueue[ 0 ];

	// ..and remove it from the array
	iReceiveQueue.Delete( 0 );
	iReceiveQueue.Compress();
	return item;
	}
	
void CTcSIPProfileContainer::AddManagedProfileL( CSIPManagedProfile* aManagedProfile )
    {
    CleanupStack::PushL( aManagedProfile );
    
    TUint32 id( 0 );
	User::LeaveIfError( aManagedProfile->GetParameter( KSIPProfileId, id ) );
	
	// Array may contain multiple instances with same profileId
    CTcSIPManagedProfile* tcprofile = new ( ELeave ) CTcSIPManagedProfile( aManagedProfile, (TInt)id );
    CleanupStack::Pop( aManagedProfile );
    CleanupStack::PushL( tcprofile );
    User::LeaveIfError( iManagedProfiles.Append( tcprofile ) );
    CleanupStack::Pop();
    }


CSIPProfile& CTcSIPProfileContainer::GetProfileByIdL( TInt aProfileId )
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
    // Profile wasn't found from internal array, create new instance through registry 
    // and store it 
    profile = ProfileRegistry().ProfileL( aProfileId );
    
    CleanupStack::PushL( profile );
    CTcSIPProfile* item = new ( ELeave ) CTcSIPProfile( profile, aProfileId );
    CleanupStack::Pop( profile );
    
    CleanupStack::PushL( item );
    User::LeaveIfError( iProfiles.Append( item ) );
    CleanupStack::Pop( item );
    
    return *profile;
    }
    
    
CSIPManagedProfile& CTcSIPProfileContainer::GetManagedProfileByIdL( TInt aManagedProfileId )
    {
    CSIPManagedProfile* profile = 0;
    
    // Check whether profile is already stored into our internal array
    for( TInt i = 0; i < iManagedProfiles.Count(); i++ )
        {
        if( iManagedProfiles[ i ]->ProfileId() == aManagedProfileId )
            {
            return iManagedProfiles[ i ]->Profile();
            }
        
        }
    // Profile wasn't found from internal array, create new instance through registry 
    // and store it 
    profile = static_cast< CSIPManagedProfile* >( ManagedProfileRegistry().ProfileL( aManagedProfileId ) );
    
    CleanupStack::PushL( profile );
    CTcSIPManagedProfile* item = new ( ELeave ) CTcSIPManagedProfile( profile, aManagedProfileId );
    CleanupStack::Pop( profile );
    
    CleanupStack::PushL( item );
    User::LeaveIfError( iManagedProfiles.Append( item ) );
    CleanupStack::Pop( item );
    
    return *profile;
    }
 
CSIPManagedProfile& CTcSIPProfileContainer::GetDefaultProfileL()
    {
	CSIPManagedProfile* profile = static_cast< CSIPManagedProfile* >( ManagedProfileRegistry().DefaultProfileL() );
				
    AddManagedProfileL( profile );
        
    return *profile;			
    }

void CTcSIPProfileContainer::DeleteProfileById( TInt aProfileId )
    {
    CTcSIPProfile* tcprofile = 0;
    for( TInt i = 0; i < iProfiles.Count(); i++ )
        {
        tcprofile = iProfiles[ i ];
        
        if( tcprofile->ProfileId() == aProfileId )
            {
            iProfiles.Remove( i );
            delete tcprofile;
            return;
            }
        }
    }
    
void CTcSIPProfileContainer::DeleteManagedProfileById( TInt aManagedProfileId )
    {
    CTcSIPManagedProfile* tcprofile = 0;
    for( TInt i = 0; i < iManagedProfiles.Count(); i++ )
        {
        tcprofile = iManagedProfiles[ i ];
        if( tcprofile->ProfileId() == aManagedProfileId )
            {
            iManagedProfiles.Remove( i );
            delete tcprofile;
            return;
            }
        }
    }
//
// -- Internal utility functions ----------------------------------------------
//

void CTcSIPProfileContainer::QueueProfileReceived( TTcProfileReceived& aItem )
	{
	TcLog::WriteFormat( _L8("CTcSIPProfileContainer::QueueProfileReceived( ProfileId = %d, Event = %d, Error = %d )"), aItem.iProfileId, aItem.iEvent, aItem.iError );

	TRAPD( err, iReceiveQueue.AppendL( aItem ) );
	__ASSERT_ALWAYS( !err, Panic( KSIPErrOOMInNotifier ) );

	if( iActiveWait.IsStarted() )
		{
		iTimer->Remove( iReceiveTimeout );
		iActiveWait.AsyncStop();
		}
	}


TInt CTcSIPProfileContainer::ReceiveTimeout( TAny* aSelf )
	{
	CTcSIPProfileContainer& self =
				*reinterpret_cast< CTcSIPProfileContainer* >( aSelf );
	if( self.iActiveWait.IsStarted() )
		{
		self.iActiveWait.AsyncStop();
		}

	return KErrNone;
	}


//
// -- Helper classes ------------------------------------------------------------
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
            
	    
CTcSIPManagedProfile::CTcSIPManagedProfile( CSIPManagedProfile* aManagedProfile, TInt aManagedProfileId )
    {
    iManagedProfile = aManagedProfile;
    iManagedProfileId = aManagedProfileId;
    }
    
 CTcSIPManagedProfile::~CTcSIPManagedProfile()
    {
    delete iManagedProfile;
    }
    
CSIPManagedProfile& CTcSIPManagedProfile::Profile()
    {
    return *iManagedProfile;
    }
    
TInt CTcSIPManagedProfile::ProfileId()
    {
    return iManagedProfileId;
    }


