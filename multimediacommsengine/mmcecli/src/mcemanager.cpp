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




#include <sipstrings.h>
#include <stringpool.h>
#include "mcemanager.h"
#include "mceinsessionobserver.h"
#include "mcesessionobserver.h"
#include "mceeventobserver.h"
#include "mcereferobserver.h"
#include "mcetransactionobserver.h" 
#include "mceintransactionobserver.h" 
#include "mceinsession.h"
#include "mceaudiocodec.h"
#include "mcevideocodec.h"
#include "mcefilesource.h"
#include "mceevent.h"
#include "mcerefer.h"
#include "mcetransactiondatacontainer.h"
#include "mce.h"
#include "mcemanagerreceiver.h"
#include "mceeventmanagerreceiver.h"
#include "mceitcsender.h"
#include "mceserial.h"
#include "mceevents.h"
#include "mceclilogs.h"
#include "mcecomfilesource.h"

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceManager::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceManager* CMceManager::NewL(
                        const TUid& aAppUid, 
						TMceTransactionDataContainer* aContainer )
    {
     
    CMceManager* self = 
        new (ELeave) CMceManager( aAppUid, aContainer );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    
    }


// -----------------------------------------------------------------------------
// CMceManager::~CMceManager
// -----------------------------------------------------------------------------
//
EXPORT_C CMceManager::~CMceManager()
    {
    MCECLI_DEBUG("CMceManager::~CMceManager, Entry");
    
    if (iContainer)
    	{
    	iContainer->Clear();
    	iContainer = 0;
    	}			
    
    iSessions.Reset();
    iSessions.Close();

    iEvents.Reset();
    iEvents.Close();

    iRefers.Reset();
    iRefers.Close();
    
    iSupportedAudioCodecs.ResetAndDestroy();
    iSupportedVideoCodecs.ResetAndDestroy();
    
    iTranscodingFiles.Reset();
    iTranscodingFiles.Close();
    
    delete iSender;
	delete iEventReceiver;
	delete iSessionReceiver;
	
	if (iServerSession)
        {
        iServerSession->Close();
        }
	delete iServerSession;
	
    MCECLI_DEBUG("CMceManager::~CMceManager, Exit");
	
    }

// -----------------------------------------------------------------------------
// CMceManager::SetInSessionObserver
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceManager::SetInSessionObserver( 
                        MMceInSessionObserver* aInSessionObserver )
    {
    iInSessionObserver = aInSessionObserver;
    }

// -----------------------------------------------------------------------------
// CMceManager::SetInEventObserver
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceManager::SetInEventObserver( 
                        MMceInEventObserver* aInEventObserver )
    {
    iInEventObserver = aInEventObserver;
    }

// -----------------------------------------------------------------------------
// CMceManager::SetInReferObserver
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceManager::SetInReferObserver( 
                        MMceInReferObserver* aInReferObserver )
    {
    iInReferObserver = aInReferObserver;
    }

// -----------------------------------------------------------------------------
// CMceManager::SetSessionObserver
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceManager::SetSessionObserver( 
                        MMceSessionObserver* aSessionObserver )
    {
    iSessionObserver = aSessionObserver;
    }

// -----------------------------------------------------------------------------
// CMceManager::SetEventObserver
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceManager::SetEventObserver( 
                        MMceEventObserver* aEventObserver )
    {
    iEventObserver = aEventObserver;
    }

// -----------------------------------------------------------------------------
// CMceManager::SetReferObserver
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceManager::SetReferObserver( 
                        MMceReferObserver* aReferObserver )
    {
    iReferObserver = aReferObserver;
    }
    

// -----------------------------------------------------------------------------
// CMceManager::SetTransactionObserver 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceManager::SetTransactionObserver( 
                        MMceTransactionObserver* aMceTransactionObserver )
    {
    iMceTransactionObserver = aMceTransactionObserver;
    }
    
    
    
// -----------------------------------------------------------------------------
// CMceManager::SetInTransactionObserver 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceManager::SetInTransactionObserver( 
                        MMceInTransactionObserver* aMceInTransactionObserver )
    {
    iMceInTransactionObserver = aMceInTransactionObserver;
    }    
    
// -----------------------------------------------------------------------------
// CMceManager::SetMediaObserver
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceManager::SetMediaObserver( 
                        MMceStreamObserver* aMediaObserver )
    {
    iMediaObserver = aMediaObserver;
    }


// -----------------------------------------------------------------------------
// CMceManager::SetRtpObserver
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceManager::SetRtpObserver( 
                        MMceRtpObserver* aRtpObserver )
    {
    iRtpObserver = aRtpObserver;
    }

// -----------------------------------------------------------------------------
// CMceManager::SetRtpObserver
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceManager::SetDtmfObserver(
			 MMceDtmfObserver* aDtmfObserver )
	{
	iDtmfObserver = aDtmfObserver;	
	}

// -----------------------------------------------------------------------------
// CMceManager::SupportedAudioCodecs
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<const CMceAudioCodec>&
CMceManager::SupportedAudioCodecs() const
    {
    return iSupportedAudioCodecs;
    }

// -----------------------------------------------------------------------------
// CMceManager::SupportedVideoCodecs
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<const CMceVideoCodec>&
CMceManager::SupportedVideoCodecs() const
    {
    return iSupportedVideoCodecs;
    }
    
// -----------------------------------------------------------------------------
// CMceManager::Sessions
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<CMceSession>& CMceManager::Sessions() const
    {
    return iSessions;
    }

// -----------------------------------------------------------------------------
// CMceManager::Events
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<CMceEvent>& CMceManager::Events() const
    {
    return iEvents;
    }

// -----------------------------------------------------------------------------
// CMceManager::Refers
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<CMceRefer>& CMceManager::Refers() const
    {
    return iRefers;
    }

// -----------------------------------------------------------------------------
// CMceManager::SetDefaultHeadersL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceManager::SetDefaultHeadersL( CDesC8Array* aMethods, 
                                               CDesC8Array* aHeaders )
    {
    MCECLI_DEBUG("CMceManager::SetDefaultHeadersL, Entry");
    
    TMceIds ids;
    PrepareForITC( ids, KMceCSSIPSession );
    
    TMceFactory factory;
    CMceMsgArray< TMceFactory >* msgArray = 
        new (ELeave) CMceMsgArray< TMceFactory >( factory );
    CleanupStack::PushL( msgArray ); 

    CMceMsgTextArray* methodsMsg = NULL;
    if ( aMethods )
        {
        methodsMsg = new ( ELeave ) CMceMsgTextArray( *aMethods );   
        }
    else
        {
        CDesC8Array* empty = new( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
        CleanupStack::PushL( empty );
        methodsMsg = new ( ELeave ) CMceMsgTextArray( *empty );
        }
    
    CleanupStack::PushL( methodsMsg ); 
    msgArray->iArray.AppendL( methodsMsg ); 
    CleanupStack::Pop( methodsMsg );
    
    CMceMsgTextArray* hdrsMsg = NULL;
    if ( aHeaders )
        {
        hdrsMsg = new ( ELeave ) CMceMsgTextArray( *aHeaders );   
        }
    else
        {
        CDesC8Array* empty = new( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
        CleanupStack::PushL( empty );
        hdrsMsg = new ( ELeave ) CMceMsgTextArray( *empty );
        }
     
    CleanupStack::PushL( hdrsMsg );
    msgArray->iArray.AppendL( hdrsMsg );
    CleanupStack::Pop( hdrsMsg );
    
    iSender->SendL( ids, EMceItcSetDefaultHeaders, *msgArray );  
    
    if ( !aHeaders )
        {
        CleanupStack::PopAndDestroy(); // empty
        }
    if ( !aMethods )
        {
        CleanupStack::PopAndDestroy(); // empty
        }

    CleanupStack::PopAndDestroy( msgArray );
    
    delete aMethods;
    delete aHeaders;
    
    MCECLI_DEBUG("CMceManager::SetDefaultHeadersL, Exit");
    
    }

// -----------------------------------------------------------------------------
// CMceManager::DefaultHeadersL
// -----------------------------------------------------------------------------
//
EXPORT_C CDesC8Array* CMceManager::DefaultHeadersL( const TDesC8& aMethod ) const
    {
    MCECLI_DEBUG("CMceManager::DefaultHeadersL, Entry");
    MCECLI_DEBUG_SVALUE("method", aMethod );
    
    CDesC8Array* headers = NULL;
    
    TMceIds ids;
    PrepareForITC( ids, KMceCSSIPSession );
    	
 	headers = iSender->ReadArrayL( ids, EMceItcGetDefaultHeaders, aMethod );
 	
    MCECLI_DEBUG("CMceManager::DefaultHeadersL, Exit");
    
 	return headers;
 	
    }

// -----------------------------------------------------------------------------
// CMceManager::TranscodeL
// -----------------------------------------------------------------------------
//
TUint32 CMceManager::TranscodeL( CMceFileSource& aFile )
    {
    MCECLI_DEBUG("CMceManager::TranscodeL, Entry");
    MCECLI_DEBUG_SVALUE("file", aFile.FileInfo().iFileName );
    
    TMceIds ids;
    PrepareForITC( ids, KMceCSSIPSession );
    
    TMceItcFunctions itcFunc = EMceItcTranscode;
    CMceMsgBase* msg = NULL;
    if ( aFile.iStream && aFile.iStream->Session() )
        {
        msg = new (ELeave) CMceMsgObject<CMceSession>( 
                *aFile.iStream->Session(), EMceItcMsgTypeSession );
        
        itcFunc = EMceItcTranscodeWithSession;
        }
    else
        {
        msg = new (ELeave) CMceMsgObject<CMceMediaSource>( aFile, EMceItcMsgTypeSource );
        }
    
        
    CleanupStack::PushL( msg );
    
    iSender->SendL( ids, itcFunc, *msg );
    
    CleanupStack::PopAndDestroy( msg );
        
    iTranscodingFiles.AppendL( &aFile );

    MCECLI_DEBUG("CMceManager::TranscodeL, Exit");
    
    return ids.Get( KMceIdsIndexTCId );
    
    }

// -----------------------------------------------------------------------------
// CMceManager::CancelTranscodeL
// -----------------------------------------------------------------------------
//
void CMceManager::CancelTranscodeL( const CMceFileSource& aFile, TUint32 aSessionId )
    {
    MCECLI_DEBUG("CMceManager::CancelTranscodeL, Entry");
    MCECLI_DEBUG_DVALUE("session id", aSessionId );
    
    TInt index = iTranscodingFiles.Find( &aFile );
    User::LeaveIfError( index );
    
    TMceIds ids;
    PrepareForITC( ids, KMceCSSIPSession );

    TMceItcArgTUint32 sessionId( aSessionId );
    iSender->WriteL( ids, EMceItcCancelTranscode, sessionId );
        
    TranscodeCompleted( aFile );
    
    MCECLI_DEBUG("CMceManager::CancelTranscodeL, Exit");
    
    }

// -----------------------------------------------------------------------------
// CMceManager::TranscodeCompleted
// -----------------------------------------------------------------------------
//
void CMceManager::TranscodeCompleted( const CMceFileSource& aFile )
    {
    TInt index = iTranscodingFiles.Find( &aFile );
    if ( index >= 0 )
        {
        iTranscodingFiles.Remove( index );
        }
    }



// -----------------------------------------------------------------------------
// CMceManager::HandleIncomingEventL
// -----------------------------------------------------------------------------
//
void CMceManager::HandleIncomingEventL( TMceIds& aIds )
    {
    MCECLI_DEBUG("CMceManager::HandleIncomingEventL, Entry");
    
    CMceFileSource* session = NULL;
    TInt index = 0;
    TInt status = KMceEventNotConsumed;
    
    while( status == KMceEventNotConsumed && index < iTranscodingFiles.Count() )
        {
        TMceEvent event( aIds, NULL, NULL );
        session = iTranscodingFiles[index];
        status = session->EventReceivedL( event );
        status = status == KMceEventNotOwner ? KMceEventNotConsumed : status;
        index++;
        }
    MCECLI_DEBUG("CMceManager::HandleIncomingEventL, Exit");        
    }

// -----------------------------------------------------------------------------
// CMceManager::FindAssociatedSession
// -----------------------------------------------------------------------------
//	
CMceSession* CMceManager::FindAssociatedSession( TUint32 aDialogId ) const
    {
    for ( TInt i = 0; i < iSessions.Count(); i++ )
        {
        if ( aDialogId == iSessions[ i ]->DialogId() )
            {
            return iSessions[ i ];
            }
        }
    return NULL;
    }
// -----------------------------------------------------------------------------
// CMceManager::InSessionObserver
// -----------------------------------------------------------------------------
//
MMceInSessionObserver* CMceManager::InSessionObserver() const
    {
    return iInSessionObserver;
    }
	    
// -----------------------------------------------------------------------------
// CMceManager::InEventObserver
// -----------------------------------------------------------------------------
//
MMceInEventObserver* CMceManager::InEventObserver() const
    {
    return iInEventObserver;
    }
	    
// -----------------------------------------------------------------------------
// CMceManager::InReferObserver
// -----------------------------------------------------------------------------
//
MMceInReferObserver* CMceManager::InReferObserver() const
    {
    return iInReferObserver;
    }

// -----------------------------------------------------------------------------
// CMceManager::SessionObserver
// -----------------------------------------------------------------------------
//
MMceSessionObserver* CMceManager::SessionObserver() const
    {
    return iSessionObserver;
    }

// -----------------------------------------------------------------------------
// CMceManager::MediaObserver
// -----------------------------------------------------------------------------
//
MMceStreamObserver* CMceManager::MediaObserver() const
    {
    return iMediaObserver;
    }

// -----------------------------------------------------------------------------
// CMceManager::RtpObserver
// -----------------------------------------------------------------------------
//
MMceRtpObserver* CMceManager::RtpObserver() const
    {
    return iRtpObserver;
    }

// -----------------------------------------------------------------------------
// CMceEventManager::EventObserver
// -----------------------------------------------------------------------------
//
MMceEventObserver* CMceManager::EventObserver() const
    {
    return iEventObserver;
    }

// -----------------------------------------------------------------------------
// CMceEventManager::ReferObserver
// -----------------------------------------------------------------------------
//
MMceReferObserver* CMceManager::ReferObserver() const
    { 
    return iReferObserver;
    }
    
// -----------------------------------------------------------------------------
// CMceManager::TransactionObserver
// -----------------------------------------------------------------------------
//
MMceTransactionObserver* CMceManager::TransactionObserver() const
    { 
    return iMceTransactionObserver;
    }
    
    
// -----------------------------------------------------------------------------
// CMceManager::TransactionObserver
// -----------------------------------------------------------------------------
//
MMceInTransactionObserver* CMceManager::InTransactionObserver() const
    { 
    return iMceInTransactionObserver;
    }
    
        

// -----------------------------------------------------------------------------
// CMceEventManager::DtmfObserver
// -----------------------------------------------------------------------------
//    
MMceDtmfObserver* CMceManager::DtmfObserver() const
    {
    return iDtmfObserver;
    }
    
// -----------------------------------------------------------------------------
// CMceManager::ServerSession
// -----------------------------------------------------------------------------
//
RMce& CMceManager::ServerSession() const
    {
    return *iServerSession;
    }

// -----------------------------------------------------------------------------
// CMceManager::AppUid
// -----------------------------------------------------------------------------
//
TUid CMceManager::AppUid() const
    {
    return iAppUid;
    }

// -----------------------------------------------------------------------------
// CMceManager::TransactionContainer
// -----------------------------------------------------------------------------
//
TMceTransactionDataContainer* CMceManager::TransactionContainer() const
	{
	return iContainer;
	}

// -----------------------------------------------------------------------------
// CMceManager::PrepareForITC
// -----------------------------------------------------------------------------
//
void CMceManager::PrepareForITC( TMceIds& aId, TUint aRole ) const
    {
    
	aId.iAppUID    = AppUid().iUid;
	aId.iManagerType = static_cast<TMceSessionType>( aRole );
    
    }

// -----------------------------------------------------------------------------
// CMceManager::RegisterSessionL
// -----------------------------------------------------------------------------
//
TUint32 CMceManager::RegisterSessionL( CMceSession* aSession, 
                                       CDesC8Array& aParams,
                                       TUint32& aDialogId,
                                       TBool& aConnectionState )
    {
    MCECLI_DEBUG("CMceManager::RegisterSessionL, Entry");
    
    TUint32 sessionID =  aSession->Id();
    if ( sessionID == KMceNotAssigned &&
    	 aSession->iFlatData->iType == CMceComSession::EOutSession )
        {        
    	TMceIds ids;
        PrepareForITC( ids, KMceCSSIPSession );
    	ids.iProfileID  = aSession->Profile();
    	ids.iDialogID = aDialogId;
    	ids.iDialogType = KMceDlgTypeInvite;

        CMceMsgTextArray* msgParams = new (ELeave) CMceMsgTextArray( aParams );
        CleanupStack::PushL( msgParams );
        
        iSender->SendL( ids, EMceItcCreateSession, *msgParams );
        CleanupStack::PopAndDestroy( msgParams );
        sessionID = ids.iSessionID;
        aDialogId = ids.iDialogID;
		aConnectionState = ids.iSpare1;
        }
    else
        {
        //NOP
        MCECLI_DEBUG("CMceManager::RegisterSessionL, Allready registered");
        }
        
    AddSessionL( aSession );
    
    MCECLI_DEBUG_DVALUE("CMceManager::RegisterSessionL, Exit. session id", 
                        sessionID );
                        
    return sessionID;
    
    }

// -----------------------------------------------------------------------------
// CMceManager::UnregisterSession
// -----------------------------------------------------------------------------
//
void CMceManager::UnregisterSession( CMceSession* aSession )
    {
    MCECLI_DEBUG("CMceManager::UnregisterSession, Entry");
    MCECLI_DEBUG_DVALUE("session id", aSession->Id() );
    MCECLI_DEBUG_DVALUE("zombie", aSession->IsZombie() );

    if ( !aSession->IsZombie() )
        {
    	TMceIds ids;
        PrepareForITC( ids, KMceCSSIPSession );
    	
    	TUint32 sessionID = aSession->Id();

        TRAP_IGNORE( iSender->SendL( ids, EMceItcDeleteSession, sessionID ) );

        }
        
    RemoveSession( aSession );
    
    MCECLI_DEBUG("CMceManager::UnregisterSession, Exit");
    }
  

// -----------------------------------------------------------------------------
// CMceManager::RemoveSession
// -----------------------------------------------------------------------------
//
void CMceManager::RemoveSession( CMceSession* aSession )
    {
    MCECLI_DEBUG("CMceManager::RemoveSession, Entry");
    TInt index = iSessions.Find( aSession );

    if ( index >= 0 )
        {
        iSessions.Remove( index );
        MCECLI_DEBUG("CMceManager::RemoveSession, session removed");
        }
    
    MCECLI_DEBUG_DVALUE("CMceManager::RemoveSession, Exit. \
sessions", iSessions.Count() );
    }

// -----------------------------------------------------------------------------
// CMceManager::IncomingSessionUpdateL
// -----------------------------------------------------------------------------
//
void CMceManager::IncomingSessionUpdateL( CMceInSession& aNewSession, 
                                          CMceSession& aOldSession )
    {
    MCECLI_DEBUG("CMceManager::IncomingSessionUpdateL, Entry");
    MCECLI_DEBUG_DVALUE("old session id", aNewSession.Id() );
    MCECLI_DEBUG_DVALUE("new session id", aOldSession.Id() );
    
    AddSessionL( &aNewSession );
    RemoveSession( &aOldSession );
    aOldSession.SetZombie( aNewSession );
    
    MCECLI_DEBUG("CMceManager::IncomingSessionUpdateL, Exit");
    
    }
  
// -----------------------------------------------------------------------------
// CMceManager::FindRefer
// -----------------------------------------------------------------------------
//
CMceRefer* CMceManager::FindRefer( const CMceEvent& aEvent )
    {
    CMceRefer* refer = NULL;
    for ( TInt ind = 0; ind < iRefers.Count() && !refer ; ind++ )
        {
        refer = iRefers[ ind ];
        if ( refer->ReferEvent() && 
             refer->ReferEvent()->Id() == aEvent.Id() )
            {
            // found 
            }
        else
            {
            refer = NULL;
            }
        }
    return refer;
    }

// -----------------------------------------------------------------------------
// CMceManager::RegisterEventL
// -----------------------------------------------------------------------------
//
TUint32 CMceManager::RegisterEventL( CMceEvent* aEvent,
                                     CDesC8Array& aParams,
                                     TUint32& aDialogId,
                                     TUint32 aDialogType )
    {
    MCECLI_DEBUG("CMceManager::RegisterEventL, Entry");
    
	TUint32 sessionID =  aEvent->Id();
    if ( sessionID == KMceNotAssigned && !aEvent->IsInEvent() )
        {
    	TMceIds ids;
        PrepareForITC( ids, KMceCSSIPEvent );
    	ids.iProfileID  = aEvent->ProfileId();
    	ids.iDialogID = aDialogId;
    	ids.iDialogType = aDialogType;
    	
        CMceMsgTextArray* msgParams = new (ELeave) CMceMsgTextArray( aParams );
        CleanupStack::PushL( msgParams );
        
        iSender->SendL( ids, EMceItcCreateSession, *msgParams );
        CleanupStack::PopAndDestroy( msgParams );
        sessionID = ids.iSessionID;
        aDialogId = ids.iDialogID;
        }
    else
        {
        //NOP
        MCECLI_DEBUG("CMceManager::RegisterEventL, Allready registered");
        }
        
    AddEventL( aEvent );
    
    MCECLI_DEBUG_DVALUE("CMceManager::RegisterEventL, Exit. session id", 
                        sessionID );
    
    return sessionID;
    }
    
// -----------------------------------------------------------------------------
// CMceManager::RegisterReferL
// -----------------------------------------------------------------------------
//
TUint32 CMceManager::RegisterReferL( CMceRefer* aRefer,
                                     CDesC8Array& aParams,
                                     TUint32& aDialogId )
    {
    MCECLI_DEBUG("CMceManager::RegisterReferL, Entry");
    
	TUint32 sessionID = KMceNotAssigned;
	if( aRefer->ReferEvent() )
	    {
	    sessionID = aRefer->ReferEvent()->Id();
	    }
	else
	    {
	    sessionID = aRefer->Id();
	    }
	    
    if ( sessionID == KMceNotAssigned && !aRefer->ReferEvent() )
        {
    	TMceIds ids;
        PrepareForITC( ids, KMceCSSIPEvent );
    	ids.iProfileID = aRefer->ProfileId();
    	ids.iDialogID = aDialogId;
    	ids.iDialogType = KMceDlgTypeRefer;
    	
        CMceMsgTextArray* msgParams = new (ELeave) CMceMsgTextArray( aParams );
        CleanupStack::PushL( msgParams );
        
        iSender->SendL( ids, EMceItcCreateSession, *msgParams );
        CleanupStack::PopAndDestroy( msgParams );
        
        sessionID = ids.iSessionID;
        aDialogId = ids.iDialogID;
        
        }
    else
        {
        //NOP
        MCECLI_DEBUG("CMceManager::RegisterReferL, Allready registered");
        }
        
    AddReferL( aRefer );
    
    MCECLI_DEBUG_DVALUE("CMceManager::RegisterReferL, Exit. session id", 
                        sessionID );
    
    return sessionID;
    }

// -----------------------------------------------------------------------------
// CMceManager::UnregisterEvent 
// -----------------------------------------------------------------------------
//
void CMceManager::UnregisterEvent( CMceEvent* aEvent )
    {
    
    // Delete the Session if necessary. 
    
    MCECLI_DEBUG("CMceManager::UnregisterEvent, Entry");
    if (aEvent->State() != CMceEvent::ETerminated)
    	{
    	TRAP_IGNORE( aEvent->TerminateL() )	
    	}      
    else
    	{
	   	CMceRefer* refer = FindRefer( *aEvent );
	    if ( refer )
	        {
	        refer->DetachReferEvent();
	        }
	        
		TMceIds ids;
	    PrepareForITC( ids, KMceCSSIPEvent );
		
		TUint32 sessionID = aEvent->Id();

	    MCECLI_DEBUG_DVALUE("session id", sessionID );

	    TRAP_IGNORE( iSender->SendL( ids, EMceItcDeleteSession, sessionID ) )
	    
	    TInt index = iEvents.Find( aEvent);

	    if ( index >= 0 )
	        {
	        iEvents.Remove( index );
	        MCECLI_DEBUG("CMceManager::UnregisterEvent, event removed");
	        }
	        
	    MCECLI_DEBUG_DVALUE("CMceManager::UnregisterEvent, Exit.\
		events", iEvents.Count() );
    	}
            
    }

// -----------------------------------------------------------------------------
// CMceManager::UnregisterRefer
// -----------------------------------------------------------------------------
//
void CMceManager::UnregisterRefer( CMceRefer* aRefer )
    {   
    MCECLI_DEBUG("CMceManager::UnregisterRefer, Entry");
	TMceIds ids;
    PrepareForITC( ids, KMceCSSIPEvent );
	
	TUint32 sessionID = aRefer->Id();
    MCECLI_DEBUG_DVALUE("session id", sessionID );

    if ( !aRefer->ReferEvent() )
        {
        // if event does no exist, delete the subsession.
        TRAP_IGNORE( iSender->SendL( ids, EMceItcDeleteSession, sessionID ) )
        }
        
    TInt index = iRefers.Find( aRefer);

    if ( index >= 0 )
        {
        iRefers.Remove( index );
        MCECLI_DEBUG("CMceManager::UnregisterRefer, refer removed");
        }
        
    MCECLI_DEBUG_DVALUE("CMceManager::UnregisterRefer, Exit.\
refers", iRefers.Count() );

    }
    
// -----------------------------------------------------------------------------
// CMceManager::AddSessionL
// -----------------------------------------------------------------------------
//
void CMceManager::AddSessionL( CMceSession* aSession )
    {
    MCECLI_DEBUG("CMceManager::AddSessionL, Entry");
    __ASSERT_ALWAYS( iSessions.Find( aSession ) < 0, User::Leave( KErrArgument ) );
    
    iSessions.AppendL( aSession );

    MCECLI_DEBUG_DVALUE("CMceManager::AddSessionL, Exit.\
sessions", iSessions.Count() );
    
    }

// -----------------------------------------------------------------------------
// CMceManager::AddEventL
// -----------------------------------------------------------------------------
//
void CMceManager::AddEventL( CMceEvent* aEvent )
    {
    MCECLI_DEBUG("CMceManager::AddEventL, Entry");
    __ASSERT_ALWAYS( iEvents.Find( aEvent ) < 0, User::Leave( KErrArgument ) );
    
    iEvents.AppendL( aEvent );

    MCECLI_DEBUG_DVALUE("CMceManager::AddEventL, Exit.\
events", iEvents.Count() );
    
    }


// -----------------------------------------------------------------------------
// CMceManager::AddReferL
// -----------------------------------------------------------------------------
//
void CMceManager::AddReferL( CMceRefer* aRefer )
    {
    MCECLI_DEBUG("CMceManager::AddReferL, Entry");
    __ASSERT_ALWAYS( iRefers.Find( aRefer ) < 0, User::Leave( KErrArgument ) );
    
    iRefers.AppendL( aRefer );

    MCECLI_DEBUG_DVALUE("CMceManager::AddReferL, Exit.\
refers", iRefers.Count() );
    
    }
    
// -----------------------------------------------------------------------------
// CMceManager::CMceManager
// -----------------------------------------------------------------------------
//
CMceManager::CMceManager( const TUid& aAppUid, 
                        TMceTransactionDataContainer* aContainer )
    :iContainer( aContainer ),
     iNextId( 0 ),
     iAppUid( aAppUid )
    {
    
    }


// -----------------------------------------------------------------------------
// CMceManager::ConstructL
// -----------------------------------------------------------------------------
//
void CMceManager::ConstructL()	
    {
    MCECLI_DEBUG("CMceManager::ConstructL, Entry");
    
	iServerSession = new (ELeave) RMce;
    User::LeaveIfError( iServerSession->Connect() );
        
	iSessionReceiver = new (ELeave) CMceManagerReceiver( *iServerSession, 
	                                              *this );

	iEventReceiver = new (ELeave) CMceEventManagerReceiver( *iServerSession, 
	                                              *this );
	                                               
    iSender = CMceItcSender::NewL( *iServerSession );
    
    ReceiveSupportedAudioCodecsL();
    ReceiveSupportedVideoCodecsL();
    	                           
    User::LeaveIfError( iServerSession->SendClientUid( iAppUid ) );

    MCECLI_DEBUG("CMceManager::ConstructL, Exit");
    
    }


// -----------------------------------------------------------------------------
// CMceManager::ReceiveSupportedAudioCodecsL
// -----------------------------------------------------------------------------
//
void CMceManager::ReceiveSupportedAudioCodecsL()	
    {
    MCECLI_DEBUG("CMceManager::ReceiveSupportedAudioCodecsL, Entry");
    
	TMceIds ids;
    PrepareForITC( ids, KMceCSSIPSession );
    	
 	CDesC8Array* serializedCodecs = 
 	    iSender->ReadArrayL( ids, EMceItcGetSupportedAudioCodecs );
    CleanupStack::PushL( serializedCodecs );
     	    
    for ( TInt i = 0; i < serializedCodecs->MdcaCount(); i++ )
        {
        TPtrC8 serializedCodec = serializedCodecs->MdcaPoint( i );
        CMceMsgObject<CMceAudioCodec>* audioCodecMsg = 
                                new (ELeave) CMceMsgObject<CMceAudioCodec>();
        CleanupStack::PushL( audioCodecMsg );
        audioCodecMsg->DecodeL( serializedCodec );
        audioCodecMsg->PushL();
        
        iSupportedAudioCodecs.AppendL( audioCodecMsg->iObject );
        audioCodecMsg->Pop();
        CleanupStack::PopAndDestroy( audioCodecMsg );
        }

    CleanupStack::PopAndDestroy( serializedCodecs );
    
    MCECLI_DEBUG("CMceManager::ReceiveSupportedAudioCodecsL, Exit");
    }

// -----------------------------------------------------------------------------
// CMceManager::ReceiveSupportedVideoCodecsL
// -----------------------------------------------------------------------------
//
void CMceManager::ReceiveSupportedVideoCodecsL()	
    {
    MCECLI_DEBUG("CMceManager::ReceiveSupportedVideoCodecsL, Entry");
    
	TMceIds ids;
    PrepareForITC( ids, KMceCSSIPSession );
    	
 	CDesC8Array* serializedCodecs = 
 	    iSender->ReadArrayL( ids, EMceItcGetSupportedVideoCodecs );
    CleanupStack::PushL( serializedCodecs );
     	    
    for ( TInt i = 0; i < serializedCodecs->MdcaCount(); i++ )
        {
        TPtrC8 serializedCodec = serializedCodecs->MdcaPoint( i );
        CMceMsgObject<CMceVideoCodec>* videoCodecMsg = 
                                new (ELeave) CMceMsgObject<CMceVideoCodec>();
        CleanupStack::PushL( videoCodecMsg );
        videoCodecMsg->DecodeL( serializedCodec );
        videoCodecMsg->PushL();
        
        iSupportedVideoCodecs.AppendL( videoCodecMsg->iObject );
        videoCodecMsg->Pop();
        CleanupStack::PopAndDestroy( videoCodecMsg );
        }

    CleanupStack::PopAndDestroy( serializedCodecs );
    
    MCECLI_DEBUG("CMceManager::ReceiveSupportedVideoCodecsL, Exit");
    }
    
    
// -----------------------------------------------------------------------------
// CMceManager::NextMediaId
// -----------------------------------------------------------------------------
//
TMceMediaId CMceManager::NextMediaId()	
    {
    iNextId++;
    
    return TMceMediaId( iAppUid, iNextId );
        
    }
