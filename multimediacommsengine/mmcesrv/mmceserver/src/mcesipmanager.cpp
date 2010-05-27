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
* Description:  
*
*/




#include <badesca.h>
#include <sip.h>
#include <sipprofile.h>
#include <sipprofileregistry.h>
#include <siphttpdigest.h>
#include <sipservertransaction.h>
#include <siprequestelements.h>
#include <siptoheader.h>
#include <sipaddress.h>
#include <sipmessageelements.h>
#include <sipinvitedialogassoc.h>
#include <sipnotifydialogassoc.h>
#include <sipeventheader.h>
#include <sipexpiresheader.h>
#include <sipmanagedprofile.h>

#include "mcesipstatemachine.h"
#include "mcesipmanager.h"
#include "mceservercore.h"
#include "mcecssession.h"
#include "mcecssessionimplementation.h"
#include "mcesipsession.h"
#include "mcesipconnection.h"
#include "mcemediamanager.h"
#include "mcenatpluginmanager.h"
#include "mcenatsipsession.h"
#include "mceclientserver.h"
#include "mcesrvlogs.h"
#include "mceclient.pan"
#include "mcesipevent.h"
#include "mceclientresolver.h"
#include "mcetimermanager.h"
#include "cleanupresetanddestroy.h"


#define REJECT( transaction )\
        MCESRV_DEBUG("REJECTED");\
        MceSip::DiscardRequest( transaction );\
        return

#define REJECT_IF_ERROR( error, transaction )\
    if ( error != KErrNone )\
        {\
        REJECT( transaction );\
        }

// -----------------------------------------------------------------------------
// CMceSipManager::NewL
// -----------------------------------------------------------------------------
//
CMceSipManager* CMceSipManager::NewL (CMceServerCore& aServerCore )
	{
    CMceSipManager* self = CMceSipManager::NewLC( aServerCore );
    CleanupStack::Pop(self);
    return self;
	}

// -----------------------------------------------------------------------------
// CMceSipManager::NewLC
// -----------------------------------------------------------------------------
//
CMceSipManager* CMceSipManager::NewLC ( CMceServerCore& aServerCore )
	{
    CMceSipManager* self = new (ELeave) CMceSipManager(aServerCore);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
	}

// -----------------------------------------------------------------------------
// CMceSipManager::CMceSipManager
// -----------------------------------------------------------------------------
//
CMceSipManager::CMceSipManager (CMceServerCore& aServerCore)
: iServerCore (aServerCore),
  iNextDialogId( 0 )
	{
	}

// -----------------------------------------------------------------------------
// CMceSipManager::ConstructL
// -----------------------------------------------------------------------------
//
void CMceSipManager::ConstructL ()
	{
    MCESRV_DEBUG("CMceSipManager::ConstructL, Entry")
	
    User::LeaveIfError( iSocketServ.Connect() );	
	
	iSIP = CSIP::NewL( KServerUid3, *this );
	iProfileRegistry = CSIPProfileRegistry::NewL( *iSIP, *this );
                       
	iMediaManager = CMceMediaManager::NewL( *this );
	iStateMachine = CMceSipStateMachine::NewL();

	iClientResolver = CMceClientResolver::NewL( *this );

	iOrphanSubSessions = CMceCsSessionImplementation::NewL( *this, NULL );
	
	iTimerManager = CMceTimerManager::NewL();
	
    MCESRV_DEBUG("CMceSipManager::ConstructL, Exit")
	}

// -----------------------------------------------------------------------------
// CMceSipManager::~CMceSipManager
// -----------------------------------------------------------------------------
//
CMceSipManager::~CMceSipManager()
	{
    MCESRV_DEBUG("CMceSipManager::~CMceSipManager, Entry")
	
    iSIPClientConnections.ResetAndDestroy();
    iSIPClientConnections.Close();
    iClients.Reset();
    iClients.Close();

    if ( iOrphanSubSessions )
        {
        iOrphanSubSessions->RemoveAllSubSessions( EFalse );    
        }
    delete iOrphanSubSessions;

    iProfiles.ResetAndDestroy();
    iProfiles.Close();

    delete iMediaManager;
    delete iStateMachine;
    delete iProfileRegistry;
	delete iClientResolver;
	delete iTimerManager;
    delete iSIP;
    
    iSocketServ.Close();
    
    MCESRV_DEBUG("CMceSipManager::~CMceSipManager, Exit")
	}

// -----------------------------------------------------------------------------
// CMceSipManager::NextDialogId
// -----------------------------------------------------------------------------
//
TUint32 CMceSipManager::NextDialogId()
    {
    iNextDialogId++;
    return iNextDialogId;
    }

// -----------------------------------------------------------------------------
// CMceSipManager::RegisterClientL
// -----------------------------------------------------------------------------
//
void CMceSipManager::RegisterClientL( CMceCsSession& aClientSession )
    {
    MCESRV_DEBUG("CMceSipManager::RegisterClientL, Entry")
    MCESRV_DEBUG_DVALUE("UID", aClientSession.Uid().iUid )

	iClientResolver->RegisterClientL( aClientSession.Uid() );
    iClients.AppendL( &aClientSession );

    MCESRV_DEBUG("CMceSipManager::RegisterClientL, Exit")
    }

// -----------------------------------------------------------------------------
// CMceSipManager::UnRegisterClient
// -----------------------------------------------------------------------------
//
void CMceSipManager::UnRegisterClient( CMceCsSession& aClientSession )
    {
    MCESRV_DEBUG("CMceSipManager::UnRegisterClient, Entry")

	iClientResolver->DeregisterClient( aClientSession.Uid() );
    
    TInt index = iClients.Find( &aClientSession );
    
    if ( index >= KErrNone )
        {
        iClients.Remove( index );
        }
    else
        {
        MCESRV_DEBUG("UnRegisterClient: CLIENT NOT FOUND")
        }
        
    MCESRV_DEBUG("CMceSipManager::UnRegisterClient, Exit")
    }
        
// -----------------------------------------------------------------------------
// CMceSipManager::MediaManager
// -----------------------------------------------------------------------------
//
CMceMediaManager& CMceSipManager::MediaManager()
    {
    return *iMediaManager;
    }
        

// -----------------------------------------------------------------------------
// CMceSipManager::TimerManager
// -----------------------------------------------------------------------------
//
CMceTimerManager& CMceSipManager::TimerManager()
    {
    return *iTimerManager;
    }
    
// -----------------------------------------------------------------------------
// CMceSipManager::MediaManager
// -----------------------------------------------------------------------------
//
CSIP& CMceSipManager::SIPClient()
    {
    return *iSIP;
    }

// -----------------------------------------------------------------------------
// CMceSipManager::ProfileL
// -----------------------------------------------------------------------------
//
CSIPProfile& CMceSipManager::ProfileL( TUint32 aProfileId )
    {
    CSIPProfile* profile = NULL;
    TInt status = KErrNotFound;

    TInt index = FindProfileById( iProfiles, aProfileId );
    if ( index != KErrNotFound )
        {
        profile = iProfiles[ index ];
        }
    else
        {
        RPointerArray<CSIPProfile> profiles;
        iProfileRegistry->ProfilesL( profiles );
        
        index = FindProfileById( profiles, aProfileId );
        if ( index != KErrNotFound )
            {
            profile = profiles[ index ];
            status = iProfiles.Append( profile );
            if ( status != KErrNone )
                {
                profile = NULL;
                }
            else
                {
                profiles.Remove( index );
                }
            }
        profiles.ResetAndDestroy();
        profiles.Close();
        }
    if (!profile )
        {
        User::Leave( status );
        }
        
    return *profile;
    }

// -----------------------------------------------------------------------------
// CMceSipManager::ProfileL
// -----------------------------------------------------------------------------
//
CSIPProfile& CMceSipManager::ProfileL( TUint32 /*aIapId*/, 
                                       const CUri8& aRequestUri )
    {
    CSIPProfile* profile = NULL;
    TInt status = KErrNotFound;
    
    // Try to find in existing profiles 
    // Try matching with request URI
    TInt index = FindProfileByRequestUriL( iProfiles, aRequestUri );
    
    if ( index != KErrNotFound )
        {
        // found in existing profiles
        profile = iProfiles[ index ];
        }
    else
        {
        // Get all profiles
        RPointerArray<CSIPProfile> profiles;
        MceCleanupResetAndDestroyPushL( profiles );
        iProfileRegistry->ProfilesL( profiles );
        // Try matching with request URI
        index = FindProfileByRequestUriL( profiles, aRequestUri );
            
        if ( index != KErrNotFound )
            {
            profile = profiles[ index ];
            status = iProfiles.Append( profile );
            if ( status != KErrNone )
                {
                profile = NULL;
                }
            else
                {
                profiles.Remove( index );
                }
            }
        CleanupStack::PopAndDestroy( &profiles );
        }
    
    if (!profile )
        {
        User::Leave( status );
        }
    
    return *profile;
    }

// -----------------------------------------------------------------------------
// CMceSipManager::Cleanup
// -----------------------------------------------------------------------------
//
void CMceSipManager::Cleanup()
    {
    MCESRV_DEBUG("CMceSipManager::Cleanup, Entry")
    
    for ( int i = 0; i < iOrphanSubSessions->SubSessions().Count(); i++ )
        {
        CMceCsSubSession* session = iOrphanSubSessions->SubSessions()[i];
        if ( session->CanDispose() ||
             session->Terminate() )
            {
            MCESRV_DEBUG_DVALUE("Cleanup: orphan removed. ID", 
                MCE_SESSION_ID( session ) )
            iOrphanSubSessions->SubSessions().Remove( i );
            delete session;
            i--;
            }
        }
    
    // If number of orphan subsessions and number of CMceCsSession 
    // object's count are 0, stop MCE server after time out.
    iServerCore.InactiveServerCleanup();
    
    MCESRV_DEBUG("CMceSipManager::Cleanup, Exit")
    }

// -----------------------------------------------------------------------------
// CMceSipManager::ConnectionL
// -----------------------------------------------------------------------------
//
CMceSipConnection& CMceSipManager::ConnectionL( CSIPProfile& aProfile )
    {
	MCESRV_DEBUG("CMceSipManager::ConnectionL, Entry")
	TUint32 iapId = 0;
	aProfile.GetParameter( KSIPAccessPointId, iapId );

    CMceSipConnection* connection = NULL;
    
    TInt index = 0;
    while ( !connection && index < iSIPClientConnections.Count() )
        {
        if ( iSIPClientConnections[ index ]->AccessPointId() == iapId )
            {
            connection = iSIPClientConnections[ index ];
            }
        index++;            
        }
    	    
    if (!connection )
        {
        MCESRV_DEBUG("CMceSipManager::ConnectionL, no connection created")
        
        MCESRV_DEBUG("CMceSipManager::ConnectionL, Create MceSipConnection")
		connection = 
		    CMceSipConnection::NewLC( *this, aProfile, *iProfileRegistry );
        iSIPClientConnections.AppendL( connection );
        CleanupStack::Pop( connection );
        }
        
    if ( !iProfileRegistry->IsEnabled( aProfile ) )
    	{
    	iProfileRegistry->EnableL( aProfile, *connection );
        }
    	
    __ASSERT_ALWAYS( aProfile.IsContextActive(), 
                     User::Leave( KErrCouldNotConnect ) ); 

    MCESRV_DEBUG("CMceSipManager::ConnectionL, Exit")
    return *connection;
    }

// -----------------------------------------------------------------------------
// CMceSipManager::ConnectionL
// -----------------------------------------------------------------------------
//
CMceSipConnection& CMceSipManager::ConnectionL( TUint32 aIAPId )
    {
    CMceSipConnection* connection = NULL;

    TInt index = 0;
    while ( !connection && index < iSIPClientConnections.Count() )
        {
        if ( iSIPClientConnections[ index ]->AccessPointId() == aIAPId )
            {
            connection = iSIPClientConnections[ index ];
            }
        index++;            
        }

    if ( !connection )
        {
        connection = CMceSipConnection::NewLC( *this, aIAPId );
        iSIPClientConnections.AppendL( connection );
        CleanupStack::Pop( connection );
        }
        
    return *connection;
    }

// -----------------------------------------------------------------------------
// CMceSipManager::CreateSubSessionL
// -----------------------------------------------------------------------------
//
CMceCsSubSession* CMceSipManager::CreateSubSessionL( CMceCsSession& aSession,
                                                     TMceIds& aIds,
                                                     CDesC8Array& aParams )
    {
    MCESRV_DEBUG("CMceSipManager::CreateSubSessionL( by client ), Entry")
    
    Cleanup();
    
    TUint32 profileId = aIds.iProfileID;
    TMceCsSessionType sessionType = (TMceCsSessionType)aIds.iManagerType;
    TMceDialogType dialogType = (TMceDialogType)aIds.iDialogType;
    TUint32 dialogId = aIds.iDialogID;

    CMceSipConnection* connection = NULL;
    if ( profileId )
	    {
	    CSIPProfile& profile = ProfileL( profileId );
	    
	    connection = &ConnectionL( profile );   	
	    }
	else
		{
		TInt32 iapId = 0;
		TLex8 accessPoint( aParams.MdcaPoint( KMceArrayIndexIAPId ) );
		accessPoint.Val( iapId );
	    connection = &ConnectionL( iapId );		
		}
		
    __ASSERT_ALWAYS( connection, User::Leave( KErrCouldNotConnect ) );    	

    CMceCsSubSession* subSession = NULL;
    
    aIds.iSpare1 = ( connection->State() == CSIPConnection::EActive );
    
    switch ( sessionType )
        {
        case KMceCSSIPSession:
            {
            if ( iMediaManager->NatPluginManager().NatEnabledL() )
                {
                if( profileId )
                	{
                	MCESRV_DEBUG("CMceSipManager::CreateSubSessionL, NAT SIP session with profile")
                	subSession = CMceNatSipSession::NewLC( aSession,
                                                       *connection,
                                                       *iStateMachine,
                                                       ProfileL( profileId ),
                                                       &aParams );
                	}
                else
                	{
                	MCESRV_DEBUG("CMceSipManager::CreateSubSessionL, NAT SIP session without profile")
                	subSession = CMceNatSipSession::NewLC( aSession,
                                                       *connection,
                                                       *iStateMachine,
                                                       &aParams );
                	}
                
                }
            else
                {
                if( profileId )
                	{
                    MCESRV_DEBUG("CMceSipManager::CreateSubSessionL, SIP session with profile")
                    subSession = CMceSipSession::NewLC( aSession,
                                                    *connection,
                                                    *iStateMachine,
                                                    ProfileL( profileId ) );
                	}
                else
                	{
                	MCESRV_DEBUG("CMceSipManager::CreateSubSessionL, SIP session without profile")
                	subSession = CMceSipSession::NewLC( aSession,
                                                       *connection,
                                                       *iStateMachine );
                	}
                }
            break;
            }
        case KMceCSSIPEvent:
            {
            MCESRV_DEBUG("CreateSubSessionL: creating SIP event")
            subSession = CMceSipEvent::NewLC( aSession, 
            								  *connection, 
            								  ProfileL( profileId ) );
            break;
            }
        default:
            {
            MCESRV_DEBUG("CreateSubSessionL: NOT SUPPORTED")
            User::Leave( KErrNotSupported );
            }
        }
        
    const CMceCsSubSession* sharedSession = NULL;
    CSIPDialogAssocBase* dialog = NULL;
    
    if ( dialogId != KMceNotAssigned )
        {
        MCESRV_DEBUG_DVALUE( 
            "CreateSubSessionL: find subsession with dialogId", dialogId )
        sharedSession = aSession.Implementation().FindSubSession( dialogId );
        }

    if ( sharedSession )
        {
        MCESRV_DEBUG_DVALUE( 
            "CreateSubSessionL: creating dialog assoc to existing dialog. ID",
            dialogId )
        
        dialog = subSession->CreateOutgoingDialogL( 
                                        dialogType, 
                                        aParams, 
                                        sharedSession->Dialog()->Dialog() );
        subSession->SetDialog( CMceCsSubSession::EOutSession, 
                               dialog, 
                               sharedSession->DialogId() );
        }
    else
        {
        MCESRV_DEBUG( "CreateSubSessionL: creating new dialog assoc" )
        dialog = subSession->CreateOutgoingDialogL( dialogType, aParams );
        subSession->SetDialog( CMceCsSubSession::EOutSession, 
                               dialog, 
                               NextDialogId() );
        }

    connection->AttachL( *subSession );
    CleanupStack::Pop( subSession );
    
    MCESRV_DEBUG( 
        "CMceSipManager: CMceSipManager::CreateSubSessionL( by client ), Exit")
            
    return subSession;
    }

// -----------------------------------------------------------------------------
// CMceSipManager::CreateSubSessionL
// -----------------------------------------------------------------------------
//
void CMceSipManager::CreateSubSessionL( CSIPServerTransaction* aTransaction,
                                        CMceSipConnection& aConnection )
    {
    MCESRV_DEBUG("CMceSipManager::CreateSubSessionL, Entry")
    
    User::LeaveIfError( Consumes( *aTransaction ) ?
                        KErrNone :
                        KErrNotSupported );
                        
    
    iClientResolver->RegisterIncomingRequestL( aTransaction, aConnection );
    
    MCESRV_DEBUG("CMceSipManager::CreateSubSessionL, Exit")
    }

// -----------------------------------------------------------------------------
// CMceSipManager::IncomingResolvedRequest
// -----------------------------------------------------------------------------
//
void CMceSipManager::IncomingResolvedRequest( TUid aClientUid,
                                              CMceSipConnection& aConnection,
                                              CSIPServerTransaction* aRequest )
    {
    MCESRV_DEBUG("CMceSipManager::IncomingResolvedRequest, Entry")
    MCESRV_DEBUG_SVALUE("request", MceSip::Method( *aRequest ).DesC() )

    CMceCsSession* client = NULL;
    TInt i = 0;
    while ( !client && i < iClients.Count() )
        {
        client = iClients[ i++ ];
        if ( client->Uid() != aClientUid )
            {
            client = NULL;
            }
        }

    TInt error = KErrNone;
    if ( client )
        {
        TRAP( error, CreateSubSessionL( aRequest, *client, aConnection ) );
        }
    else
        {
        MCESRV_DEBUG("CLIENT NOT FOUND")
        }

    if ( !client || error != KErrNone )
        {
        MCESRV_DEBUG("IncomingResolvedRequest: discarding request")
        MCESRV_DEBUG_DVALUE( "ERROR:", error )
        MceSip::DiscardRequest( aRequest, error );
        }
    
    MCESRV_DEBUG("CMceSipManager::IncomingResolvedRequest, Exit")
    }

// -----------------------------------------------------------------------------
// CMceSipManager::CreateSubSessionL
// -----------------------------------------------------------------------------
//
void CMceSipManager::CreateSubSessionL( CSIPServerTransaction* aTransaction,
                                        CMceCsSession& aClient,
                                        CMceSipConnection& aConnection,
                                        CMceCsSubSession* aAssocSession )
    {
    MCESRV_DEBUG("CMceSipManager::CreateSubSessionL( from network ), Entry")
    MCESRV_DEBUG_SVALUE("request", MceSip::Method( *aTransaction ).DesC() )
    
    __ASSERT_ALWAYS( Consumes( *aTransaction ), User::Leave( KErrArgument ) );
    
    CMceCsSubSession* session = NULL;
    CSIPProfile *profile = NULL;
    TUint32 profileId = 0;

    if ( aAssocSession != NULL )
    	{
    	User::LeaveIfError(
    	    aAssocSession->Profile().GetParameter( KSIPProfileId, profileId ) );
    	}
    else
    	{
    	const TDesC8* remoteContact =
    	    &aTransaction->RequestElements()->RemoteUri().Uri().UriDes();
    			
		TInt len = remoteContact->Length();
		HBufC8* editRemoteContact = HBufC8::NewLC( len );
		TPtr8 desERC = editRemoteContact->Des();
		desERC.Copy( *remoteContact );
		TInt cut = desERC.Find( _L8( ":" ) );
		
		if ( cut != KErrNotFound )
			{
			desERC.Delete( 0, ++cut );
			len -= cut;
			cut = desERC.Find( _L8( "@" ) );
			desERC.Delete( cut, len );
			
			RPointerArray<CSIPProfile> profiles;
			iProfileRegistry->ProfilesL( profiles );

			TInt error( KErrNone );
        	const TDesC8* localContact = NULL;
			TBool found( EFalse );
	    	for ( TInt i = 0; !found && i < profiles.Count(); i++ )
	    		{
	    		profile = profiles[ i ];
	    		error = profile->GetParameter( KSIPContactHeaderUser, localContact );
	    		if ( error == KErrNone )
	    			{
	    			if ( desERC.Compare( *localContact ) == 0 )
	    				{
	    				User::LeaveIfError( 
	    				    profile->GetParameter( KSIPProfileId, profileId ) );
	    				found = ETrue;
	    				}
	    			}
	    		}
	    	profiles.ResetAndDestroy();
        	profiles.Close();
			}
		CleanupStack::PopAndDestroy( editRemoteContact );
    	}
    profile = &ProfileL( profileId );


    RStringF method = MceSip::Method( *aTransaction );
    
    if ( method == SIPStrings::StringF( SipStrConsts::EInvite ) )
        {
        if ( iMediaManager->NatPluginManager().NatEnabledL() )
            {
            MCESRV_DEBUG("CMceSipManager::CreateSubSessionL, NAT SIP session")
            session = CMceNatSipSession::NewLC( aClient,
                                                aConnection,
                                                *iStateMachine,
                                                ProfileL( profileId ) );
            }
        else
            {
            MCESRV_DEBUG("CMceSipManager::CreateSubSessionL, SIP session")
            // we can give to MceSipSession...
            session = CMceSipSession::NewLC( aClient,
                                             aConnection,
                                             *iStateMachine,
                                             *profile );
            }
        }
    else
        {
        MCESRV_DEBUG("IncomingResolvedRequest: creating SIP event")
        session = CMceSipEvent::NewLC( aClient, aConnection, *profile );
        }
        
    CSIPDialogAssocBase* dialog = 
        session->CreateIncomingDialogL( *aTransaction );
        
    TUint32 dialogId = aAssocSession ? 
        aAssocSession->DialogId() : NextDialogId();
        
    session->SetDialog( CMceCsSubSession::EInSession, dialog, dialogId );
    
    aConnection.AttachL( *session );

    session->InitializeIncomingDialogL( aTransaction );
    
    //check if session failed and is in terminated state already
    if ( !session->CanDispose() )
        {
        TInt error = aClient.Implementation().SubSessions().Append( session );
        if ( error != KErrNone )
            {
            MCESRV_DEBUG("CMceSipManager::CreateSubSessionL: Appending failed")
            session->PopRequest();
            User::Leave( error );
            }
        CleanupStack::Pop( session );
        }
    else
        {
        MCESRV_DEBUG("IncomingResolvedRequest: Initialization FAILED")
        CleanupStack::PopAndDestroy( session );
        }
        
    MCESRV_DEBUG("CMceSipManager::CreateSubSessionL( from network ), Exit")
    }

// -----------------------------------------------------------------------------
// CMceSipManager::SocketServ
// -----------------------------------------------------------------------------
//
RSocketServ& CMceSipManager::SocketServ()
    {
    return iSocketServ;
    }

// -----------------------------------------------------------------------------
// CMceSipManager::Consumes
// -----------------------------------------------------------------------------
//
TBool CMceSipManager::Consumes( CSIPServerTransaction& aTransaction )
    {
    RStringF method = MceSip::Method( aTransaction );
    
    return method == SIPStrings::StringF( SipStrConsts::EInvite ) ||
           method == SIPStrings::StringF( SipStrConsts::ESubscribe ) ||
           method == SIPStrings::StringF( SipStrConsts::ERefer );
    }
	
// -----------------------------------------------------------------------------
// CMceSipManager::AddOrphanSubSessionL
// -----------------------------------------------------------------------------
//
void CMceSipManager::AddOrphanSubSessionL( CMceCsSubSession* aSubSession )
    {
    MCESRV_DEBUG("CMceSipManager::AddOrphanSubSessionL, Entry")
    MCESRV_DEBUG_DVALUE("dialogid", aSubSession->DialogId() )
    
    aSubSession->SetClient( *iOrphanSubSessions );
    aSubSession->SetSessionReceiver( NULL );
    iOrphanSubSessions->SubSessions().AppendL( aSubSession );
    
    MCESRV_DEBUG("CMceSipManager::AddOrphanSubSessionL, Exit")
    }

// -----------------------------------------------------------------------------
// CMceSipManager::OrphanSubSessionCount
// -----------------------------------------------------------------------------
//
TInt CMceSipManager::OrphanSubSessionCount()
    {
    return iOrphanSubSessions->SubSessions().Count();
    }

// -----------------------------------------------------------------------------
// CMceSipManager::UnRegisterSubSession
// -----------------------------------------------------------------------------
//
void CMceSipManager::UnRegisterSubSession( CMceCsSubSession& aSubSession )
    {
    MCESRV_DEBUG("CMceSipManager::UnRegisterSubSession, Entry")

    CSIPProfile* profile = NULL;
    TBool profileUsed( EFalse );
    TInt err = KErrNone;
    TInt index = iSIPClientConnections.Find( &aSubSession.SIPConnection() );
 
    if ( index != KErrNotFound )
        {
        if ( aSubSession.SIPConnection().Detach( aSubSession ) == 0 )
            {
            // If no SubSessions related to the CMceSipConnection any more,
            // delete the CMceSipConnection instance.
            MCESRV_DEBUG("CMceSipManager::UnRegisterSubSession, remove connection")
            CMceSipConnection* connection = iSIPClientConnections[ index ];
            iSIPClientConnections.Remove( index );
            delete connection;
			connection = NULL;
            }
        if ( aSubSession.ProfileConfigured() )
        	{
            MCESRV_DEBUG("CMceSipManager::UnRegisterSubSession, profile configured")
            TUint32 profileId = 0;
            aSubSession.Profile().GetParameter( KSIPProfileId, profileId );

        	TRAP( err, profile = &ProfileL( profileId ) );

        	if ( err == KErrNone )
        		{
        		for ( TInt i = 0; i < iSIPClientConnections.Count() && !profileUsed; i++ )
            		{
            		profileUsed = iSIPClientConnections[ i ]->IsProfileUsedInSession(
            														*profile );
            		}
        		}
            }

        // Delete the unused profile after un-register a SubSession.
        if ( !profileUsed )
            {
            MCESRV_DEBUG("CMceSipManager::UnRegisterSubSession, profile not used anymore")
            
			// No need to loop downwards, as just one profile is removed
            for ( TInt j = 0; j < iProfiles.Count() && profile; j++ )
                {
                if ( iProfiles[ j ] == profile )
                    {
                    MCESRV_DEBUG("CMceSipManager::UnRegisterSubSession, deleting profile")
                    iProfiles.Remove( j );
                    delete profile;
                    profile = NULL;
                    }
                }
            }
        }
    MCESRV_DEBUG("CMceSipManager::UnRegisterSubSession, Exit")
    }

// -----------------------------------------------------------------------------
// CMceSipManager::FindProfileByRequestUriL
// -----------------------------------------------------------------------------
//                                 
TInt CMceSipManager::FindProfileByRequestUriL( 
    RPointerArray<CSIPProfile>& aProfiles, 
    const CUri8& aRequestUri )
    {
    TInt ret = KErrNotFound;
    
    // User name of request URI 
    const TDesC8& uriUser = aRequestUri.Uri().Extract( EUriUserinfo );
    
    CSIPProfile* profile = NULL;
    TInt profileInd = 0;
    
    while( ret == KErrNotFound && profileInd < aProfiles.Count() )
        {
        // Try to find a profile with matching contact header user part
        profile = aProfiles[ profileInd ];

        const TDesC8* contactUser = NULL;
        // get Contact-header's user-part
        User::LeaveIfError( profile->GetParameter( KSIPContactHeaderUser, contactUser ) );

        if ( uriUser.CompareF( *contactUser ) == 0 )
            {
            ret = profileInd;
            }

        profileInd++;
        }

    return ret;
    }   

// -----------------------------------------------------------------------------
// CMceSipManager::FindProfileById
// -----------------------------------------------------------------------------
//
TInt CMceSipManager::FindProfileById( 
    RPointerArray<CSIPProfile>& aProfiles, 
    TUint32 profileId )
    {
    TInt index = 0;
    while ( index < aProfiles.Count() )
        {
        TUint32 macthId;
        TInt error = aProfiles[ index ]->GetParameter( KSIPProfileId, macthId );
        if ( error == KErrNone && macthId == profileId )
            {
            return index;
            }
        index++;            
        }
        
    return KErrNotFound;
    }
    
// -----------------------------------------------------------------------------
// CMceSipManager::IncomingRequest
// -----------------------------------------------------------------------------
//
void CMceSipManager::IncomingRequest( 
    TUint32 aIapId,
    CSIPServerTransaction* aTransaction )	                 
    {
    MCESRV_DEBUG("CMceSipManager::IncomingRequest, Entry")
    MCESRV_DEBUG_SVALUE("request", MceSip::Method( *aTransaction ).DesC() )
    MCESRV_DEBUG_DVALUE("iapid", aIapId )
    
    CMceSipConnection* connection = NULL;
    CSIPProfile* profile = NULL;
    
    if ( !Consumes( *aTransaction ) )
        {
        REJECT( aTransaction );
        }
        
    const CSIPRequestElements* request = aTransaction->RequestElements();
    const CSIPToHeader* to = request->ToHeader();
    if ( !to )
        {
        MCESRV_DEBUG("IncomingRequest: no recipient: REJECT")
        REJECT( aTransaction );
        }
        
    const CUri8& reqUri = request->RemoteUri();
    
    TRAPD( error1, profile = &ProfileL( aIapId, reqUri ) );
    REJECT_IF_ERROR( error1, aTransaction )
    
    TRAPD( error2, connection = &ConnectionL( *profile ) );
    REJECT_IF_ERROR( error2, aTransaction )

    __ASSERT_ALWAYS( profile->IsContextActive(), 
        User::Leave( KErrCouldNotConnect ) );
    
    TRAPD( error3, CreateSubSessionL( aTransaction, *connection ) );
    REJECT_IF_ERROR( error3, aTransaction )

    MCESRV_DEBUG("CMceSipManager::IncomingRequest, Exit")
    }

// -----------------------------------------------------------------------------
// CMceSipManager::TimedOut
// -----------------------------------------------------------------------------
//
void CMceSipManager::TimedOut( CSIPServerTransaction& /*aTransaction*/ )
    {
    MCESRV_DEBUG("CMceSipManager::TimedOut, Entry")
    MCESRV_DEBUG("-- NOT IMPLEMENTED --")
    MCESRV_DEBUG("CMceSipManager::TimedOut, Exit")
    }

// -----------------------------------------------------------------------------
// CMceSipManager::ProfileRegistryEventOccurred
// If profile is updated, read its data from profile server and replace the
// local copy of profile's data. As CSIPProfile's memory address changes, also
// CMceCsSubSession::iProfile pointers must be updated.
// -----------------------------------------------------------------------------
//
void CMceSipManager::ProfileRegistryEventOccurred( 
    TUint32 aProfileId, 
    MSIPProfileRegistryObserver::TEvent aEvent )
    {
    MCESRV_DEBUG("CMceSipManager::ProfileRegistryEventOccurred(pid,event), Entry")
    MCESRV_DEBUG_DVALUES( "profileId", aProfileId, "event", aEvent )

	if ( aEvent == MSIPProfileRegistryObserver::EProfileUpdated )
		{
		CSIPProfile* profile( NULL );
		TRAPD( err, profile = iProfileRegistry->ProfileL( aProfileId ) );

		if ( profile && err == KErrNone )
			{
			TInt index = FindProfileById( iProfiles, aProfileId );
			if ( index != KErrNotFound )
				{
				CSIPProfile* oldProfile = iProfiles[ index ];
				UpdateProfileToSubSessions( *profile, *oldProfile );
			    delete oldProfile;
			    // Replace pointer in iProfiles array
			    iProfiles[ index ] = profile;
				}
			else
				{
	    		err = iProfiles.Append( profile );
	    		if ( err != KErrNone )
	    			{
	    			delete profile;
	    			}
				}
			}
        MCESRV_DEBUG_DVALUE( "err", err )
		}
	MCESRV_DEBUG("CMceSipManager::ProfileRegistryEventOccurred(pid,event), Exit")
    }

// -----------------------------------------------------------------------------
// CMceSipManager::ProfileRegistryErrorOccurred
// -----------------------------------------------------------------------------
//
void CMceSipManager::ProfileRegistryErrorOccurred( 
    TUint32 /*aProfileId*/, 
    TInt /*aError*/)
    {
    MCESRV_DEBUG("CMceSipManager::ProfileRegistryErrorOccurred(pid,error), Entry")
    MCESRV_DEBUG("-- NOT IMPLEMENTED --")
    MCESRV_DEBUG("CMceSipManager::ProfileRegistryErrorOccurred(pid,error), Exit")
    }
    
// -----------------------------------------------------------------------------
// CMceSipManager::MediaError
// -----------------------------------------------------------------------------
//
void CMceSipManager::MediaError( TInt /*aError*/ )
    {
    MCESRV_DEBUG("CMceSipManager::MediaError, Entry")
    MCESRV_DEBUG("-- NOT IMPLEMENTED --")
    MCESRV_DEBUG("CMceSipManager::MediaError, Exit")
    }

// -----------------------------------------------------------------------------
// CMceSipManager::UpdateProfileToSubSessions
// -----------------------------------------------------------------------------
//
void CMceSipManager::UpdateProfileToSubSessions( CSIPProfile& aNewProfile,
												 CSIPProfile& aOldProfile )
	{
	MCESRV_DEBUG("CMceSipManager::UpdateProfileToSubSessions, Entry")

	TInt count = iClients.Count();
	for ( TInt i = 0; i < count; ++i )
		{
		CMceCsSessionImplementation& session = iClients[ i ]->Implementation();
		RPointerArray<CMceCsSubSession>& subSessions = session.SubSessions();

		TInt subSessionCount = subSessions.Count();
		for ( TInt j = 0; j < subSessionCount; ++j )
			{
			CSIPProfile& subSessionProfile = subSessions[ j ]->Profile();
			if ( &subSessionProfile == &aOldProfile )
				{
				MCESRV_DEBUG_DVALUES("update subsession profile, i", i, "j", j )
				MCESRV_DEBUG_DVALUES("oldProfile", (TInt)&aOldProfile,
									 "newProfile", (TInt)&aNewProfile )
				subSessions[ j ]->SetProfile( aNewProfile );
				}
			}
		}

	// Update orphan subsessions
    RPointerArray<CMceCsSubSession>& orphanSubSessions = iOrphanSubSessions->SubSessions();
    TInt orphanSubSessionCount = orphanSubSessions.Count();
	
    for ( TInt j = 0; j < orphanSubSessionCount; ++j )
        {
        CSIPProfile& subSessionProfile = orphanSubSessions[ j ]->Profile();
        if ( &subSessionProfile == &aOldProfile )
            {
            MCESRV_DEBUG_DVALUE("update orphan subsession profile, j", j )
            MCESRV_DEBUG_DVALUES("oldProfile", (TInt)&aOldProfile,
                                 "newProfile", (TInt)&aNewProfile )
            orphanSubSessions[ j ]->SetProfile( aNewProfile );
            }
        }
	MCESRV_DEBUG("CMceSipManager::UpdateProfileToSubSessions, Exit")
	}
