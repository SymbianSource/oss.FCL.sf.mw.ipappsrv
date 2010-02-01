/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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




#include <sipstrconsts.h>
#include "mcesession.h"
#include "mceinsession.h"
#include "mcemanager.h"
#include "mceinsessionobserver.h"
#include "mcesessionobserver.h"
#include "mcestreamobserver.h"
#include "mcetransactionobserver.h"
#include "mceintransactionobserver.h"
#include "mcertpobserver.h"
#include "mcefcmsgobserver.h"
#include "mcetransactiondatacontainer.h"
#include "mcefactory.h"
#include "mceitcsender.h"
#include "mcesessionreceiver.h"
#include "mceclientserver.h"
#include "mceevents.h"
#include "mceserial.h"
#include "mceitcsender.h"
#include "mcesessionreceiver.h"
#include "mce.h"
#include "mcecomsession.h"
#include "mcecommediasink.h"
#include "mcecommediasource.h"
#include "mcestreambundle.h"
#include "mceclilogs.h"

#define _FLAT_DATA static_cast<CMceComSession*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceSession::~CMceSession
// -----------------------------------------------------------------------------
//
CMceSession::~CMceSession()
    {
    MCECLI_DEBUG("CMceSession::~CMceSession, Entry")

    //causes EMceItcClientCancelReceive
    delete iReceiver;
    iReceiver = NULL;
    
    if ( iManager && iFlatData )
        {
        //causes EMceItcDeleteSession
        iManager->UnregisterSession( this );
        }
    
    delete iFlatData;
    delete iSender;
    
    iMediaStreams.ResetAndDestroy();
    iBundles.ResetAndDestroy();
    
    MCECLI_DEBUG("CMceSession::~CMceSession, Exit")
    }

// -----------------------------------------------------------------------------
// CMceSession::State
// -----------------------------------------------------------------------------
//
EXPORT_C CMceSession::TState CMceSession::State() const
    {
    return FLAT_DATA( iState );
    }

// -----------------------------------------------------------------------------
// CMceSession::GetSIPParams
// -----------------------------------------------------------------------------
//
void CMceSession::GetSIPParams( CDesC8Array*& aHeaders, 
                                HBufC8*& aContentType,
                                CDesC8Array*& aContentHeaders )
    {
    CDesC8Array* sipHeaders = FLAT_DATA( iSIPHeaders );
    HBufC8* sipContentType = FLAT_DATA( iSIPContentType );
    CDesC8Array* sipContentHeaders = FLAT_DATA( iSIPContentHeaders );
    FLAT_DATA( iSIPHeaders ) = NULL;
    FLAT_DATA( iSIPContentType ) = NULL;
    FLAT_DATA( iSIPContentHeaders ) = NULL;
    aHeaders = sipHeaders;
    aContentType = sipContentType;
    aContentHeaders = sipContentHeaders;
    }

// -----------------------------------------------------------------------------
// CMceSession::SetSIPParams
// -----------------------------------------------------------------------------
//
void CMceSession::SetSIPParams( CDesC8Array* aHeaders, 
                                HBufC8* aContentType,
                                CDesC8Array* aContentHeaders )
    {
    delete FLAT_DATA( iSIPHeaders );
    FLAT_DATA( iSIPHeaders ) = aHeaders;
    delete FLAT_DATA( iSIPContentType );
    FLAT_DATA( iSIPContentType ) = aContentType;
    delete FLAT_DATA( iSIPContentHeaders );
    FLAT_DATA( iSIPContentHeaders ) = aContentHeaders;
    }

// -----------------------------------------------------------------------------
// CMceSession::SIPParamsCleanup
// -----------------------------------------------------------------------------
//
void CMceSession::SIPParamsCleanup( TAny* aSession )
    {
    if ( !aSession )
        {
        return;
        }
    CMceSession* session = reinterpret_cast< CMceSession* >( aSession );
    session->iFlatData->iSIPHeaders = 0;
    session->iFlatData->iSIPContentType = 0;
    session->iFlatData->iSIPContentHeaders = 0;
    }

// -----------------------------------------------------------------------------
// CMceSession::ConnectionActive
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMceSession::ConnectionActive() const
    {
    return FLAT_DATA( iIsConnectionActive );
    }

// -----------------------------------------------------------------------------
// CMceSession::AddStreamL
// If called from InternalizeL, iManager is NULL and don't initialize stream.
// -----------------------------------------------------------------------------
//
void CMceSession::AddStreamL( CMceMediaStream* aMediaStream )
    {
	__ASSERT_ALWAYS( aMediaStream != NULL, User::Leave( KErrArgument ) );
	__ASSERT_ALWAYS( iMediaStreams.Find( aMediaStream ) == KErrNotFound,
					 User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( !aMediaStream->BoundStream() ||
                     aMediaStream->Binder(), User::Leave( KErrArgument ) );

	aMediaStream->AddedL();

	if ( iManager && StreamInitializeCondition( *aMediaStream ) )
		{
		// Initialize stream to get the supported codecs
    	aMediaStream->InitializeL( iManager, *this );
		}

    DoAddStreamL( aMediaStream );
    }

// -----------------------------------------------------------------------------
// CMceSession::DoAddStreamL
// -----------------------------------------------------------------------------
//
void CMceSession::DoAddStreamL( CMceMediaStream* aMediaStream )
    {
    iMediaStreams.AppendL( aMediaStream );
    }

// -----------------------------------------------------------------------------
// CMceSession::RemoveStreamL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceSession::RemoveStreamL( CMceMediaStream& aMediaStream )
    {
    MCECLI_DEBUG("CMceSession::RemoveStreamL, Entry")
    
    __ASSERT_ALWAYS( State() == CMceSession::EIdle || 
                     State() == CMceSession::EIncoming ||
                     State() == CMceSession::EEstablished,
                     User::Leave( KErrNotReady ) );
    
    TInt index = iMediaStreams.Find( &aMediaStream );
    if ( index >= 0 )
        {
        CMceMediaStream* stream = iMediaStreams[ index ];
        iMediaStreams.Remove( index );
        delete stream;
        }
    
    MCECLI_DEBUG("CMceSession::RemoveStreamL, Exit")
    }

// -----------------------------------------------------------------------------
// CMceSession::InitializeL
// -----------------------------------------------------------------------------
//
void CMceSession::InitializeL( 
    TBool aClientSideMedia, 
    TBool aDiscardUnusedCodecs )
    {
    __ASSERT_ALWAYS( iManager, User::Leave( KErrArgument ) );
    
    for( TInt i=0; i<iMediaStreams.Count();i++)
        {
        if ( aClientSideMedia )
            {
            iMediaStreams[i]->InitializeL( iManager, *this );
            }
        iMediaStreams[i]->InitializeL( *this, aDiscardUnusedCodecs );
        }
    for( TInt i=0; i<iBundles.Count();i++)
        {
        iBundles[i]->InitializeL( *this );
        }
    }

// -----------------------------------------------------------------------------
// CMceSession::UpdateL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceSession::UpdateL(
             TUint32 aTimeout,
             CDesC8Array* aHeaders, 
             HBufC8* aContentType,
             HBufC8* aContent,
             CDesC8Array* aContentHeaders ) 
    {
    MCECLI_DEBUG("CMceSession::UpdateL, Entry")
    TState state = State();
    MCECLI_DEBUG_CLISTATE( "state", state )
    
    User::LeaveIfError( IsZombie() ? KErrArgument : KErrNone  );
    User::LeaveIfError( State() != EIdle ? KErrNone : KErrNotReady );
    User::LeaveIfError( 
        ( aContentType && aContent ) || 
       !( aContentType || aContent || aContentHeaders ) ? 
        KErrNone : KErrArgument );
    
    // Leave if in incoming state givem too small timeout value
    User::LeaveIfError( !( aTimeout > 0 && State() == CMceSession::EIncoming ) ||
        aTimeout <= SessionTimer() ? KErrNone : KErrArgument );
    
    // initialize streams
    InitializeL( EFalse ); 
    
    SetSIPParams( aHeaders, aContentType, aContentHeaders );

    if( !( aTimeout == 0 && State() == CMceSession::EIncoming ) )
        {
        FLAT_DATA( iTimeout ) = 
            ( aTimeout < FLAT_DATA( iMinSE ) ?  FLAT_DATA( iMinSE ) : aTimeout );    
        }
     
	SendUpdateL( aContent );   
	
	MCECLI_DEBUG("CMceSession::UpdateL, Exit")
    }

// -----------------------------------------------------------------------------
// CMceSession::SendUpdateL
// -----------------------------------------------------------------------------
//
void CMceSession::SendUpdateL( HBufC8* aContent )				            
    {
    MCECLI_DEBUG("CMceSession::SendUpdateL, Entry")
    
    TCleanupItem sipParamsCleanup( SIPParamsCleanup, this );
    CleanupStack::PushL( sipParamsCleanup );
    
    TMceIds ids;
	PrepareForITC( ids );
	ids.iState = State();

	CMceMsgObject<CMceSession>* msg = 
	    new (ELeave) CMceMsgObject<CMceSession>( *this, EMceItcMsgTypeSession );
    CleanupStack::PushL( msg );

    // Don't pass aContent's ownership. Delete it when this function can't leave
	// anymore
    iSender->SendL( ids, EMceItcUpdate, *msg, aContent, EFalse );
        
    CleanupStack::PopAndDestroy( msg );
    
	FLAT_DATA( iState ) = static_cast<TState>( ids.iState );
	TState state = State();
    MCECLI_DEBUG_CLISTATE( "CMceSession::SendUpdateL, after ITC, state", state )
    
    if ( ids.iStatus != KErrNone )
        {
        MCECLI_DEBUG("CMceSession::SendUpdateL, update failed. rollback")
        
    	TMceIds ids2;
        SynchronizeL( ids2 );
        state = State();
        MCECLI_DEBUG_CLISTATE( "CMceSession::SendUpdateL, after ITC, state", state )
        
        CleanupStack::Pop(); // sipParamsCleanup
        SetSIPParams( NULL, NULL, NULL );
        
        MMceSessionObserver* observer = iManager->SessionObserver();
        TMceTransactionDataContainer* container = iManager->TransactionContainer();
        if ( observer && container )
            {
            container->SetStatusCode( ids2.iStatus );
            observer->UpdateFailed( *this, container );
            container->Clear();
            }

        }
    else
        {
        CleanupStack::Pop(); // sipParamsCleanup
        SetSIPParams( NULL, NULL, NULL );
        }

	delete aContent;
  
    MCECLI_DEBUG("CMceSession::SendUpdateL, Exit")
    }

// -----------------------------------------------------------------------------
// CMceSession::SynchronizeL
// -----------------------------------------------------------------------------
//
void CMceSession::SynchronizeL( TMceIds& aIds )
    {
    MCECLI_DEBUG("CMceSession::SynchronizeL, Entry")
    
    PrepareForITC( aIds );
    aIds.iState = State();

    HBufC8* encodedSrvSession = iSender->ReadStringL( aIds, EMceItcGetSession );
    CleanupStack::PushL( encodedSrvSession );

    CMceMsgObject<CMceSession>* srvSession = 
            new (ELeave) CMceMsgObject<CMceSession>();
    CleanupStack::PushL( srvSession );
    srvSession->DecodeL( *encodedSrvSession );

    srvSession->PushL();
    UpdateL( *srvSession->iObject );
    CleanupStack::PopAndDestroy( srvSession );
    CleanupStack::PopAndDestroy( encodedSrvSession );
    
    MCECLI_DEBUG("CMceSession::SynchronizeL, Exit")
    }

// -----------------------------------------------------------------------------
// CMceSession::TerminateL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceSession::TerminateL( CDesC8Array* aHeaders,
					                   HBufC8* aContentType,
					                   HBufC8* aContent ) 
    {
    MCECLI_DEBUG("CMceSession::TerminateL, Entry")
    TState state = State();
    MCECLI_DEBUG_CLISTATE( "state", state )

    User::LeaveIfError( IsZombie() ? KErrArgument : KErrNone  );
    User::LeaveIfError( State() != EIdle ? KErrNone : KErrNotReady );
    User::LeaveIfError( 
        ( aContentType && aContent ) || !( aContentType || aContent ) ? 
        KErrNone : KErrArgument );
    
    
	TMceIds ids;
	PrepareForITC( ids );
	ids.iState = State();

    CMceMsgSIPData* sipData = new (ELeave) CMceMsgSIPData( SipStrConsts::EBye,
                                                           *aHeaders, *aContentType );
    CleanupStack::PushL( sipData );
    
    sipData->OwnershipRollbackPushLC();
    
    iSender->SendL( ids, EMceItcTerminateSession, *sipData, aContent );
    
    CleanupStack::Pop(); // sipData->OwnershipRollbackPushLC()
    
    sipData->Close();
    CleanupStack::PopAndDestroy( sipData );
    
   	FLAT_DATA( iState ) = static_cast<TState>( ids.iState );
   	
   	state = State();
    MCECLI_DEBUG_CLISTATE( "CMceSession::TerminateL, after ITC, state", state )
    MCECLI_DEBUG("CMceSession::TerminateL, Exit")
    }

// -----------------------------------------------------------------------------
// CMceSession::Streams
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<CMceMediaStream>& CMceSession::Streams() const
    {
    return iMediaStreams;
    }

// -----------------------------------------------------------------------------
// CMceSession::Recipient
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CMceSession::Recipient() const
    {
    return *FLAT_DATA( iRecipient );
    }

// -----------------------------------------------------------------------------
// CMceSession::Originator
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CMceSession::Originator() const
    {
    return *FLAT_DATA( iOriginator );
    }

// -----------------------------------------------------------------------------
// CMceSession::Originator
// -----------------------------------------------------------------------------
//
EXPORT_C TUint32 CMceSession::SessionTimer() const
    {
    return FLAT_DATA( iTimeout );
    }

// -----------------------------------------------------------------------------
// CMceSession::SetSessionSDPLinesL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceSession::SetSessionSDPLinesL( CDesC8Array* aSessionSDPLines )
    {
    MCECLI_DEBUG("CMceSession::SetSessionSDPLinesL, Entry")
    
    User::LeaveIfError( State() == CMceSession::EIdle ||
		                State() == CMceSession::EIncoming ||
		                State() == CMceSession::EEstablished ? 
		                KErrNone : KErrNotReady );
    CDesC8Array* lines = ( aSessionSDPLines ? aSessionSDPLines :
        new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity ) );	                
    delete FLAT_DATA( iLocalSessionSDPLines );
    FLAT_DATA( iLocalSessionSDPLines ) = lines;
    MCECLI_DEBUG("CMceSession::SetSessionSDPLinesL, Exit")
    }
		
// -----------------------------------------------------------------------------
// CMceSession::SessionSDPLinesL
// -----------------------------------------------------------------------------
//
EXPORT_C MDesC8Array* CMceSession::SessionSDPLinesL()
    {
    CDesC8Array*  lines = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( lines );
    for ( TInt i=0; i < FLAT_DATA( iRemoteSessionSDPLines )->Count(); i++ )
        {
        lines->AppendL( FLAT_DATA( iRemoteSessionSDPLines )->MdcaPoint( i ) );
        }
    CleanupStack::Pop( lines );
    return lines;
    }
        
// -----------------------------------------------------------------------------
// CMceRefer::AssociatedEvents
// -----------------------------------------------------------------------------
//
EXPORT_C RPointerArray< CMceEvent >* CMceSession::AssociatedEventsL() const
    {
    return NULL;
    }
    
// -----------------------------------------------------------------------------
// CMceRefer::AssociatedRefers
// -----------------------------------------------------------------------------
//
EXPORT_C RPointerArray< CMceRefer >* CMceSession::AssociatedRefersL() const
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// CMceSession::GetModifierL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceSession::GetModifierL( TMceSessionModifier aModifier,
                            TUint& aValue ) const
    {
    __ASSERT_ALWAYS( aModifier < KMceMaxSessionModifiers, 
                     User::Leave( KErrArgument ) );
    TUint value = _FLAT_DATA->Modifier( aModifier );
    aValue = value;
    }
    
// -----------------------------------------------------------------------------
// CMceSession::SetModifierL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceSession::SetModifierL( TMceSessionModifier aModifier, 
                            TUint aValue )        
    {
    __ASSERT_ALWAYS( aModifier < KMceMaxSessionModifiers, 
                     User::Leave( KErrArgument ) );
    _FLAT_DATA->Modifier( aModifier ) = aValue;
    if ( aModifier == KMceSecPreconditions && aValue == KMcePreconditionsE2ESupported )
    	{
    	_FLAT_DATA->Modifier( KMcePreconditions ) = KMcePreconditionsNotUsed;
    	}
    
    if ( aModifier == KMcePreconditions && aValue == KMcePreconditionsSupported &&
    	_FLAT_DATA->Modifier( KMceSecPreconditions ) != KMcePreconditionsE2ESupported )
    	{
    	_FLAT_DATA->Modifier( KMceSecPreconditions ) = KMcePreconditionsNotUsed;
    	}	
    	
    }
    
    
// -----------------------------------------------------------------------------
// CMceSession::AddBundleL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceSession::AddBundleL( CMceStreamBundle* aBundle )
    {
    
    aBundle->AddedL();
    iBundles.AppendL( aBundle );

    
    }

// -----------------------------------------------------------------------------
// CMceSession::RemoveBundleL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceSession::RemoveBundleL( CMceStreamBundle& aBundle )
    {
    TInt index = iBundles.Find( &aBundle );
    User::LeaveIfError( index );
    iBundles.Remove( index );
    
    }

// -----------------------------------------------------------------------------
// CMceSession::Bundles
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray< CMceStreamBundle >& CMceSession::Bundles() const
    {
    return iBundles;
    }
    
// -----------------------------------------------------------------------------
// CMceSession::Profile
// -----------------------------------------------------------------------------
//
EXPORT_C TUint32 CMceSession::Profile() const
	{
	return iProfileId;
    }


// -----------------------------------------------------------------------------
// CMceSession::SetServiceTypeL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceSession::SetServiceTypeL( TUint32 aServiceType )
	{
	MCECLI_DEBUG("CMceSession::SetServiceTypeL, Entry")
    
    User::LeaveIfError( State() == CMceSession::EIdle ||
		                State() == CMceSession::EIncoming ? 
		                KErrNone : KErrNotReady );
	User::LeaveIfError ( aServiceType >= KMceMaxTypeOfServiceValue ?
						 KErrArgument : KErrNone );	   
	
	FLAT_DATA( iServiceType ) = aServiceType ;
						              
	MCECLI_DEBUG("CMceSession::SetServiceTypeL, Exit")
	}
	
// -----------------------------------------------------------------------------
// CMceSession::ControlPathSecurityLevel()
// -----------------------------------------------------------------------------
//
EXPORT_C CMceSession::TControlPathSecurityLevel
CMceSession::ControlPathSecurityLevel() const 
	{
	if ( FLAT_DATA( iSipContactAddrSecure ) != CMceSession::EControlPathUnsecure &&
		 FLAT_DATA( iSipContactAddrSecure ) != CMceSession::EControlPathSecure )
		{
		FLAT_DATA( iSipContactAddrSecure ) = CMceSession::EControlPathSecurityUnknown;
		}
							
	return FLAT_DATA( iSipContactAddrSecure );
	}
	
// -----------------------------------------------------------------------------
// CMceSession::ServiceType
// -----------------------------------------------------------------------------
//
EXPORT_C TUint32 CMceSession::ServiceType() const
	{
	return FLAT_DATA( iServiceType );
	}

// -----------------------------------------------------------------------------
// CMceSession::Type
// -----------------------------------------------------------------------------
//
TMceSessionType CMceSession::Type() const
	{
	return KMceSessionNormal;
	}
    
// -----------------------------------------------------------------------------
// CMceSession::CMceSession
// -----------------------------------------------------------------------------
//
CMceSession::CMceSession( CMceManager* aManager, TUint32 aProfileId )
 : iManager( aManager ),
   iProfileId( aProfileId )
    {
    }

// -----------------------------------------------------------------------------
// CMceSession::SerializationId
// -----------------------------------------------------------------------------
//
TUint64 CMceSession::SerializationId() const
    {
    return FLAT_DATA( SerializationId() );
    }

// -----------------------------------------------------------------------------
// CMceSession::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceSession::InternalizeFlatL( RReadStream& aReadStream )
    {
    _FLAT_DATA->InternalizeFlatL( aReadStream );
    }

// -----------------------------------------------------------------------------
// CMceSession::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceSession::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    _FLAT_DATA->ExternalizeFlatL( aWriteStream );
    }

// -----------------------------------------------------------------------------
// CMceSession::InternalizeL
// -----------------------------------------------------------------------------
//
void CMceSession::InternalizeL( MMceComSerializationContext& aSerCtx )
    {
    TMceSessionSerializer<CMceSession> serial( *this, _FLAT_DATA );
    serial.InternalizeL( aSerCtx );
    }

// -----------------------------------------------------------------------------
// CMceSession::ExternalizeL
// -----------------------------------------------------------------------------
//
void CMceSession::ExternalizeL( MMceComSerializationContext& aSerCtx )
    {
    TMceSessionSerializer<CMceSession> serial( *this, _FLAT_DATA );
    serial.ExternalizeL( aSerCtx );
    }


// -----------------------------------------------------------------------------
// CMceSession::BaseFactory
// -----------------------------------------------------------------------------
//
TMceFactory CMceSession::BaseFactory()
    {
    return TMceFactory();
    }

// -----------------------------------------------------------------------------
// CMceSession::Factory
// -----------------------------------------------------------------------------
//
TMceFactory CMceSession::Factory()
    {
    return TMceFactory();
    }


// -----------------------------------------------------------------------------
// CMceSession::Id
// -----------------------------------------------------------------------------
//
TUint32 CMceSession::Id() const
    {
    return FLAT_DATA( iID );
    }


// -----------------------------------------------------------------------------
// CMceSession::DialogId
// -----------------------------------------------------------------------------
//
TUint32 CMceSession::DialogId() const
    {
    return FLAT_DATA( iDialogId );
    }

// -----------------------------------------------------------------------------
// CMceSession::ITCSender
// -----------------------------------------------------------------------------
//
CMceItcSender& CMceSession::ITCSender() const
	{
	return *iSender;
	}

// -----------------------------------------------------------------------------
// CMceSession::Manager
// -----------------------------------------------------------------------------
//
CMceManager& CMceSession::Manager() const
    {
    return *iManager;
    }

// -----------------------------------------------------------------------------
// CMceSession::SetFCObserver
// -----------------------------------------------------------------------------
//
void CMceSession::SetFCObserver( MMceFcMsgObserver* aPtr)
	{
	iFCObserver = aPtr;
	}

// -----------------------------------------------------------------------------
// CMceSession::ConstructL
// -----------------------------------------------------------------------------
//
void CMceSession::ConstructL( TBool aUpdateSession )
    {
    MCECLI_DEBUG("CMceSession::ConstructL, Entry")
    
    User::LeaveIfNull( iFlatData );
    
    if ( iManager && !aUpdateSession )
        {
		TBuf8<64> iapId;    
    	iapId.AppendNum( FLAT_DATA( iIapId ) );
        
    	CDesC8ArrayFlat* params = 
    	    new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
    	CleanupStack::PushL( params );
    	params->AppendL( Originator() );
    	params->AppendL( Recipient() );
		params->AppendL( iapId );    	
        
        FLAT_DATA( iID ) = iManager->RegisterSessionL( this, 
                                                       *params, 
                                                       FLAT_DATA( iDialogId ),
                                                       FLAT_DATA( iIsConnectionActive ) );
        
        CleanupStack::PopAndDestroy( params );
        
        //get server    
        RMce& server = iManager->ServerSession();
        
        //create sender
        iSender = CMceItcSender::NewL( server  );
        //create receiver
        iReceiver = new (ELeave) CMceSessionReceiver( server, *this );
        }
        
    MCECLI_DEBUG("CMceSession::ConstructL, Exit")
    }

// -----------------------------------------------------------------------------
// CMceSession::PrepareForITC
// -----------------------------------------------------------------------------
//
void CMceSession::PrepareForITC( TMceIds& aId )
    {
    
	aId.iAppUID    = Manager().AppUid().iUid;
	aId.iSessionID = Id();
	aId.iManagerType = KMceCSSIPSession;
	aId.iProfileID = iProfileId;
    
    }


// -----------------------------------------------------------------------------
// CMceSession::EventReceivedL
// -----------------------------------------------------------------------------
//
void CMceSession::EventReceivedL( TMceIds& aId )
    {
    TMceEvent event( aId, NULL, NULL ); 
    EventReceivedL( event );
    }


// -----------------------------------------------------------------------------
// CMceSession::EventReceivedL
// -----------------------------------------------------------------------------
//
void CMceSession::EventReceivedL( TMceIds& aId, CMceMsgBase& aMessage )
    {
    TMceEvent event( aId, &aMessage, NULL ); 
    EventReceivedL( event );
    }


// -----------------------------------------------------------------------------
// CMceSession::IsZombie
// -----------------------------------------------------------------------------
//
TBool CMceSession::IsZombie()
    {
    return !iReceiver && !iSender;
    }

// -----------------------------------------------------------------------------
// CMceSession::IncomingUpdateL
// -----------------------------------------------------------------------------
//
void CMceSession::IncomingUpdateL( CMceInSession* aInSession, HBufC8* aContent )
    {
    MCECLI_DEBUG("CMceSession::IncomingUpdateL, Entry")
    TState state = State();
    MCECLI_DEBUG_CLISTATE("state", state )
    state = aInSession->State();
    MCECLI_DEBUG_CLISTATE("update state", state )
    
    MMceInSessionObserver* observer = iManager->InSessionObserver();
    if ( !observer )
        {
        MCECLI_DEBUG("CMceSession::IncomingUpdateL, no observer. reject")
        SendITCEventL( EMceItcRejectSession );
        delete aInSession; 
        return;           
        }
    
    CDesC8Array* headers = NULL;
	HBufC8* contentType = NULL;
	CDesC8Array* contentHeaders = NULL;
	aInSession->GetSIPParams( headers, contentType, contentHeaders );
	    
	TInt pushs = 0;
	
	if ( headers )
		{
		CleanupStack::PushL( headers );++pushs;
		}
		
	if ( contentType )
		{
		CleanupStack::PushL( contentType );++pushs;
		}
    
    if ( contentHeaders )
		{
		CleanupStack::PushL( contentHeaders );++pushs;
		}
	
    TMceTransactionDataContainer* container = iManager->TransactionContainer();
	    
	if ( container )
	    {
        container->SetHeaders( headers );
        container->SetContentType( contentType );
        container->SetContent( aContent );  
        container->SetContentHeaders( contentHeaders );              			
        CleanupStack::Pop( pushs );
	    }
    else
	    {
		CleanupStack::PopAndDestroy( pushs );
        }

    iManager->IncomingSessionUpdateL( *aInSession, *this );

    MCECLI_DEBUG("CMceSession::IncomingUpdateL, this session is now zombie.")
    
    observer->IncomingUpdate( *this, aInSession, container );
   	
   	if ( container )
	    {
    	container->Clear();
    	}

    
    MCECLI_DEBUG("CMceSession::IncomingUpdateL, Exit")
    
    }


// -----------------------------------------------------------------------------
// CMceSession::SetZombie
// -----------------------------------------------------------------------------
//
void CMceSession::SetZombie( CMceInSession& aInSession )
    {
    aInSession.iManager = iManager;
    iManager = NULL;
    
    aInSession.iProfileId = iProfileId;
    
    aInSession.iSender = iSender;
    iSender = NULL;
    
    aInSession.iReceiver = iReceiver;
    iReceiver->ReplaceSession( aInSession );
    iReceiver = NULL;
    
    aInSession.iFCObserver = iFCObserver;
    iFCObserver = NULL;
    
    }
    

// -----------------------------------------------------------------------------
// CMceSession::EventReceivedL
// -----------------------------------------------------------------------------
//
void CMceSession::EventReceivedL( TMceIds& aId, 
                                  CMceMsgBase& aMessage, 
                                  HBufC8* aContent )
    {
    TMceEvent event( aId, &aMessage, aContent ); 
    EventReceivedL( event );
    }

// -----------------------------------------------------------------------------
// CMceSession::ErrorOccuredL
// -----------------------------------------------------------------------------
//
void CMceSession::ErrorOccuredL( TInt aError )
    {
    MCECLI_DEBUG("CMceSession::ErrorOccuredL, Entry")
    MCECLI_DEBUG_DVALUE("error", aError )
    
    User::LeaveIfError( iManager->SessionObserver() ? KErrNone : KErrArgument );
    FLAT_DATA( iState ) = CMceSession::ETerminated;

    
    iManager->SessionObserver()->Failed( *this, aError );
    
    TState state = State();
    MCECLI_DEBUG_CLISTATE( "CMceSession::ErrorOccuredL, before exit. state", state )
    MCECLI_DEBUG("CMceSession::ErrorOccuredL, Exit")
    }

// -----------------------------------------------------------------------------
// CMceSession::FCMessageReceived
// -----------------------------------------------------------------------------
//
void CMceSession::FCMessageReceived( HBufC8* aMessage )
    {
    MCECLI_DEBUG("CMceSession::FCMessageReceived, Entry")
    
    if ( iFCObserver )
        {
        iFCObserver->FCMsgReceived( *this, aMessage );
        }
    else
        {
        delete aMessage;
        }
    MCECLI_DEBUG("CMceSession::FCMessageReceived, Exit")
    }

// -----------------------------------------------------------------------------
// CMceSession::FCErrorOccurred
// -----------------------------------------------------------------------------
//
void CMceSession::FCErrorOccurred( TInt aError )
    {
    MCECLI_DEBUG("CMceSession::FCErrorOccurred, Entry")
    MCECLI_DEBUG_DVALUE("error", aError )
    
    if ( iFCObserver )
        {
        iFCObserver->FCMsgErrorOccurred( *this, aError );
        }
        
    MCECLI_DEBUG("CMceSession::FCErrorOccurred, Exit")
    }

// -----------------------------------------------------------------------------
// CMceSession::EventReceivedL
// -----------------------------------------------------------------------------
//
void CMceSession::EventReceivedL( TMceEvent& aEvent )
    {
    MCECLI_DEBUG( "CMceSession::EventReceivedL, Entry" )
    TState state = State();
    MCECLI_DEBUG_CLISTATE( "this: state", state )
    MCECLI_DEBUG_DVALUE( "this: id", Id() )
    
    User::LeaveIfError( 
        aEvent.Id().iSessionID == Id() ? KErrNone : KErrGeneral );
        
    if ( aEvent.Id().IsSessionId() )
        {
        MCECLI_DEBUG("CMceSession::EventReceivedL, for session")
        HandleEvent( aEvent );
        }
    else
        {
        MCECLI_DEBUG( "CMceSession::EventReceivedL, for media" )
        TInt j = 0;
        TInt status = KMceEventNotConsumed;
        while ( status != KMceEventConsumed && j < iMediaStreams.Count() )
            {
            CMceMediaStream* stream = iMediaStreams[j]; 
            status = stream->EventReceivedL( aEvent );
            if ( status == KMceEventUpdate )
                {
                MCECLI_DEBUG( "CMceSession::EventReceivedL, \
update media stream by replacing old version with new one" )
                CMceMsgObject<CMceMediaStream>* updateMsg = 
                    static_cast<CMceMsgObject<CMceMediaStream>*>( aEvent.Message() );
                CMceMediaStream* updateStream = updateMsg->Object();
                CleanupStack::PushL( updateStream );
                iMediaStreams.Remove( j );
                delete stream;
                updateStream->InitializeL( *this, EFalse );
                AddStreamL( updateStream );
                CleanupStack::Pop( updateStream );              
                updateStream->Updated();
                status = KMceEventConsumed;
                }
            j++;                
            }
        
        } 
    MCECLI_DEBUG( "CMceSession::EventReceivedL, Exit" )
    }

// -----------------------------------------------------------------------------
// CMceSession::Updated
// -----------------------------------------------------------------------------
//
void CMceSession::Updated()
    {
    MMceSessionObserver* observer = iManager->SessionObserver();
    TMceTransactionDataContainer* container = iManager->TransactionContainer();
    
    if ( observer )
        {
        observer->SessionStateChanged( *this , container);
        }
    
    }


// -----------------------------------------------------------------------------
// CMceSession::SetContainer
// -----------------------------------------------------------------------------
//
TBool CMceSession::SetContainer( CMceMsgSIPReply& aReply, HBufC8* aContent )
    {
    if ( !&aReply ) 
        {
        delete aContent;
        return ETrue;
        }
    TBool contentDeleted( EFalse );    
    TMceTransactionDataContainer* container = iManager->TransactionContainer();
    
    if ( container )
        {        
        container->SetStatusCode( aReply.iCode );
        container->SetReasonPhrase( aReply.Reason() );
        container->SetHeaders( aReply.Headers() );
        container->SetContentType( aReply.ContentType() );
        container->SetContent( aContent );
        }
    else
        {
        aReply.Close();
        delete aContent;
        contentDeleted = ETrue;
        }
    return contentDeleted;
    }
    
    
// -----------------------------------------------------------------------------
// CMceSession::SetContainer
// -----------------------------------------------------------------------------
//
TBool CMceSession::SetContainer( CMceMsgSIPData& aRequest, HBufC8* aContent )
    {
    if ( !&aRequest ) 
        {
        delete aContent;
        return ETrue;
        }
    TBool contentDeleted( EFalse );    
    TMceTransactionDataContainer* container = iManager->TransactionContainer();
    
    if ( container )
        {
        container->SetHeaders( aRequest.Headers() );
        container->SetContentType( aRequest.ContentType() );
        container->SetContent( aContent );
        }
    else
        {
        aRequest.Close();
        delete aContent;
        contentDeleted = ETrue;
        }
    return contentDeleted;
    }


// -----------------------------------------------------------------------------
// CMceSession::SetContainer
// -----------------------------------------------------------------------------
//
void CMceSession::SetContainer( CMceMsgArray<TMceFactory>& aMsgArray, 
                                HBufC8* aContent )
    {
    
    if ( !&aMsgArray  ) 
        {
        delete aContent;
        return;
        }


    for( TInt i = 0; i < aMsgArray.iArray.Count(); i++ )
        {
        CMceMsgBase* msg = aMsgArray.iArray[ i ];
        
        if ( msg->Type() == EMceItcMsgTypeSIPReply )
            {
            if ( SetContainer( static_cast<CMceMsgSIPReply&>( *msg ), aContent ) )
                {
                // Content was deleted
                aContent = NULL;
                }
            }
        else if ( msg->Type() == EMceItcMsgTypeSIPData )
            {
            if ( SetContainer( static_cast<CMceMsgSIPData&>( *msg ), aContent ) )
                {
                // Content was deleted
                aContent = NULL;
                }
            }
        else if ( msg->Type() == EMceItcMsgTypeSession )
            {
            CMceSession* session = 
                static_cast<CMceMsgObject<CMceSession>*>( msg )->Object();
            TRAPD( error, UpdateL( *session ) );
            if ( error != KErrNone )
                {
                //TBD
                }
            delete session;
            }
        else
            {
            }
        }   
    }
    
// -----------------------------------------------------------------------------
// CMceSession::HandleEvent
// -----------------------------------------------------------------------------
//
void CMceSession::HandleEvent( TMceEvent& aEvent )
    {
    MCECLI_DEBUG("CMceSession::HandleEvent, Entry")
    
    if ( aEvent.Action() == EMceItcSessionConnectionStateChanged )
        {
        MCECLI_DEBUG("CMceSession::HandleEvent, connection state changed")
        FLAT_DATA( iIsConnectionActive ) = 
            static_cast<TBool>( aEvent.ActionData() );
        iManager->SessionObserver()->SessionConnectionStateChanged( 
                                        *this, 
                                        FLAT_DATA( iIsConnectionActive ) );
        return;
        }
            
    if ( aEvent.Action() == EMceItcStateChanged || 
         aEvent.Action() == EMceItcUpdateFailed )
        {
        TMceTransactionDataContainer* container = iManager->TransactionContainer();
		MMceSessionObserver* observer = iManager->SessionObserver();
		HBufC8* content = aEvent.Content();
        
        FLAT_DATA( iState ) = 
            static_cast<CMceSession::TState>( aEvent.ActionData() );
        
        MCECLI_DEBUG("CMceSession::HandleEvent, session state changed" )
        
        if ( aEvent.MessageType() == EMceItcMsgTypeSIPReply )
            {
            MCECLI_DEBUG("CMceSession::HandleEvent, caused by sip reply")
            CMceMsgSIPReply* reply = 
                static_cast<CMceMsgSIPReply*>( aEvent.Message() );
            if ( observer )
                {
                SetContainer( *reply, content );
                if ( aEvent.Action() == EMceItcStateChanged )
                    {
                    observer->SessionStateChanged( *this, container );
                    }
                else
                    {
                    MCECLI_DEBUG("CMceSession::HandleEvent, update failed")
                    observer->UpdateFailed( *this, container );
                    }
                
			    if ( container )
			    	{
			    	container->Clear();
			    	}
                }
            else
                {
                if ( reply )
                    {
                    reply->Close();
                    }
                delete content;
                }
            }
        else if ( aEvent.MessageType() == EMceItcMsgTypeSIPData )
            {
            MCECLI_DEBUG("CMceSession::HandleEvent, caused by sip request")
            CMceMsgSIPData* request = 
                static_cast<CMceMsgSIPData*>( aEvent.Message() );
            if ( observer )
                {
                SetContainer( *request, content );
                if ( aEvent.Action() == EMceItcStateChanged )
                    {
                    observer->SessionStateChanged( *this, container );
                    }
                else
                    {
                    MCECLI_DEBUG("CMceSession::HandleEvent, update failed")
                    observer->UpdateFailed( *this, container );
                    }
                
			    if ( container )
			    	{
			    	container->Clear();
			    	}
                }
            else
                {
                if ( request )
                    {
                    request->Close();
                    }
                delete content;
                }
            }
        else if ( aEvent.MessageType() == EMceItcMsgTypeMessageArray )
            {
            MCECLI_DEBUG("CMceSession::HandleEvent, caused by sip request/reply")
            
            CMceMsgArray<TMceFactory>* msgArray = 
                        static_cast<CMceMsgArray<TMceFactory>*>( aEvent.Message() );
            if ( observer )
                {
                if ( aEvent.Action() == EMceItcStateChanged )
                    {
                    SetContainer( *msgArray, content );
                    observer->SessionStateChanged( *this, container );
                    }
				else
				  	{
				  	if ( aEvent.Id().iStatus != KMceErrOldSchool )
				  		{
	                    SetContainer( *msgArray, content );
				  		}
                    UpdateFailed( aEvent, container );
				  	}
                
			    if ( container )
			    	{
			    	container->Clear();
			    	}
                }
            else
                {
                if ( msgArray )
                    {
                    msgArray->Close();
                    }
                delete content;
                }
            }
        else 
            {
            if ( aEvent.MessageType() == EMceItcMsgTypeSession )
                {
                MCECLI_DEBUG("CMceSession::HandleEvent, caused by session update")
                
                CMceMsgObject<CMceSession>* updateMsg = 
                    static_cast<CMceMsgObject<CMceSession>*>( aEvent.Message() );
                CMceSession* session = updateMsg->Object();
                TRAPD( error, UpdateL( *session ) );
                if ( error != KErrNone )
                    {
                    //TBD
                    }
                delete session;
                }
            if ( aEvent.Action() == EMceItcUpdateFailed && observer )
                {
                observer->UpdateFailed( *this, container );
                }
            else
                {
                Updated();    
                }
            }
        MCECLI_DEBUG("CMceSession::HandleEvent, Exit")
        return;
        }

	// Handling of Request / Responses Received within a Session

	if ( aEvent.Action() == EMceItcRequestReceived )     
	
		{
		TRAPD( error, HandleEventRequestReceivedL (aEvent));
		if ( error != KErrNone )
          	{
            //TBD
            }
		}

	if ( aEvent.Action() == EMceItcResponseReceived )     
		{
		TRAPD( error, HandleEventResponseReceivedL (aEvent));
		if ( error != KErrNone )
          	{
            //TBD
            }

		}
	
	MCECLI_DEBUG("CMceSession::HandleEvent, Exit")
    return;
    }

// -----------------------------------------------------------------------------
// CMceSession::HandleEventRequestReceivedL
// -----------------------------------------------------------------------------
//
void CMceSession::HandleEventRequestReceivedL( TMceEvent& aEvent )
	{
	MCECLI_DEBUG("CMceSession::HandleEventRequestReceivedL, Entry ")
	
	TMceTransactionDataContainer* container = iManager->TransactionContainer();
    HBufC8* content = aEvent.Content();

	MMceInTransactionObserver* observer = iManager->InTransactionObserver();
					
	if ( aEvent.MessageType() == EMceItcMsgTypeSIPRequest )
		{
	                      
		if ( observer )
		    {
		    if ( aEvent.Action() == EMceItcRequestReceived )
		        {
		        TMceTransactionId TransactonID;
		        TransactonID = aEvent.Id().iTransactionID;
		        
		        CMceMsgSIPRequest* msgRequest = 
		    	static_cast<CMceMsgSIPRequest*>( aEvent.Message() );         
		    	
		    	SetContainer( *msgRequest, content );
		                                                           
		        observer->IncomingRequest( *msgRequest->RequestMethod(), 
		        							*this, TransactonID, container );
		        }
		                   
		    if ( container )
		    	{
		    	container->Clear();
		    	}
		    }
    
		else
		    {
		    // Reject the Transaction with 501 Error Response 
		    // Saying Its not implemented.
		    const TUint KMceSipNotImplemented = 501;
		    _LIT8( KNotImplemented ,"Not Implemented"); 
		    
		    HBufC8* reason = KNotImplemented().AllocLC();
			SendResponseL( aEvent.Id().iTransactionID,
			      			*reason,
		          			KMceSipNotImplemented,	
			      			NULL,
			      			NULL,
			      			NULL);
		    
		    CleanupStack::PopAndDestroy( reason );
		    
		    delete content;
		    }
		}

	MCECLI_DEBUG("CMceSession::HandleEventRequestReceivedL, Exit ")
	}
    
// -----------------------------------------------------------------------------
// CMceSession::HandleEventResponseReceivedL
// -----------------------------------------------------------------------------
//
void CMceSession::HandleEventResponseReceivedL( TMceEvent& aEvent )
	{
	MCECLI_DEBUG("CMceSession::HandleEventResponseReceivedL, Entry" )
	TMceTransactionDataContainer* container = iManager->TransactionContainer();
    HBufC8* content = aEvent.Content();
		
	if ( aEvent.MessageType() == EMceItcMsgTypeSIPReply )
		{
		MMceTransactionObserver* observer = iManager->TransactionObserver();
			
		CMceMsgSIPReply* msgReply = 
	    			static_cast<CMceMsgSIPReply*>( aEvent.Message() );         			                
	    
		if ( observer )
	    	{
	       	SetContainer( *msgReply, content );
	        TMceTransactionId TransactonID;
	        TransactonID = aEvent.Id().iTransactionID;
	        observer->TransactionResponse(*this, TransactonID, container );
	       
	    	if ( container )
	    		{
	    		container->Clear();
	    		} 
	    	}
		else 
			{
	        if ( msgReply )
	            {
	            msgReply->Close();
	            }
	        delete content;
			}
		}
		
	MCECLI_DEBUG("CMceSession::HandleEventResponseReceivedL, Exit" )
	}

// -----------------------------------------------------------------------------
// CMceSession::UpdateFailed
// -----------------------------------------------------------------------------
//
void CMceSession::UpdateFailed( TMceEvent& aEvent, 
								TMceTransactionDataContainer* aContainer )
    {
    MMceSessionObserver* observer = iManager->SessionObserver();
   	HBufC8* content = aEvent.Content();
   	    	
    if ( aEvent.Id().iStatus == KMceErrOldSchool )
    	{
    	TRAPD( leaveErr, SendUpdateL( content ) );
		if ( leaveErr == KMceErrOldSchool )
			{
			observer->UpdateFailed( *this, aContainer );				
			}
    	}
	else
		{
		observer->UpdateFailed( *this, aContainer );	
		}
    }
    
// -----------------------------------------------------------------------------
// CMceSession::UpdateL
// -----------------------------------------------------------------------------
//
void CMceSession::UpdateL( CMceSession& aSession )
    {
    MCECLI_DEBUG("CMceSession::UpdateL(received), Entry")
    MCECLI_DEBUG("CMceSession::UpdateL(received), updating session data")
        
    iFlatData->UpdateFlatL( *aSession.iFlatData );
    
    MCECLI_DEBUG("CMceSession::UpdateL(received), \
removing old streams and bundles")
    iMediaStreams.ResetAndDestroy();
    iBundles.ResetAndDestroy();

    while( aSession.iMediaStreams.Count() > 0 )
        {
        CMceMediaStream* stream = aSession.iMediaStreams[0];
        aSession.iMediaStreams.Remove( 0 );
        CleanupStack::PushL( stream );
        CMceSession::AddStreamL( stream );
        CleanupStack::Pop( stream );
        MCECLI_DEBUG("CMceSession::UpdateL(received), added updated stream")
        MCECLI_DEBUG_DVALUE("id", stream->Id().iId )
        MCECLI_DEBUG_DVALUE("enabled", stream->IsEnabled() )
        CMceMediaStream::TState state = stream->State();
        MCECLI_DEBUG_MEDIASTATE("state", state )
        }
    
    while( aSession.iBundles.Count() > 0 )
        {
        CMceStreamBundle* bundle = aSession.iBundles[0];
        aSession.iBundles.Remove( 0 );
        CleanupStack::PushL( bundle );
        AddBundleL( bundle );
        CleanupStack::Pop( bundle );
        MCECLI_DEBUG("CMceSession::UpdateL(received), added updated bundle")
        }
          
    InitializeL();

    MCECLI_DEBUG("CMceSession::UpdateL(received), Exit")
    }

// -----------------------------------------------------------------------------
// CMceSession::SendITCEventL
// -----------------------------------------------------------------------------
//
void CMceSession::SendITCEventL( TUint8 aITCFunction )
    {
    __ASSERT_ALWAYS( iSender, User::Leave( KErrGeneral ) );

	TMceIds ids;
	PrepareForITC( ids );
	ids.iState = State();
    iSender->SendL( ids, static_cast<TMceItcFunctions>( aITCFunction ) );
    //update the state
	FLAT_DATA( iState ) = static_cast<TState>( ids.iState );    
    }

// -----------------------------------------------------------------------------
// CMceSession::SendRequestL
// -----------------------------------------------------------------------------
//
EXPORT_C TMceTransactionId CMceSession::SendRequestL( const TDesC8& aMethod,
													  CDesC8Array* aHeaders, 
				  								 	  HBufC8* aContentType,
													  HBufC8* aContent )
    {
    MCECLI_DEBUG("CMceSession::SendRequestL, Entry")
    TState state = State();
    MCECLI_DEBUG_CLISTATE( "state", state )

    User::LeaveIfError( IsZombie() ? KErrArgument : KErrNone  );

	TCleanupItem sipParamsCleanup( SIPParamsCleanup, this );
    CleanupStack::PushL( sipParamsCleanup );

    TMceIds ids;
	PrepareForITC( ids );
	ids.iState = State();

	HBufC8* method = aMethod.AllocLC();

	CMceMsgSIPRequest* msg =
		new (ELeave) CMceMsgSIPRequest( *method, *aHeaders,	*aContentType );
	CleanupStack::PushL( msg );
	// Don't pass aContent's ownership. Delete it when this function can't leave
	// anymore
    iSender->SendL( ids, EMceItcRequestSend, *msg, aContent, EFalse );    
    CleanupStack::PopAndDestroy( msg );
    CleanupStack::PopAndDestroy( method );

	FLAT_DATA( iState ) = static_cast<TState>( ids.iState );
	        
    if ( ids.iStatus != KErrNone )
        {
        MCECLI_DEBUG("CMceSession::SendInfoRequestL, info failed. rollback")
    	TMceIds ids2;
        SynchronizeL( ids2 );
        state = State();
        MCECLI_DEBUG_CLISTATE(
        	"CMceSession::SendInfoRequestL, after ITC, state", state )
        CleanupStack::Pop(); // sipParamsCleanup
               
        MMceTransactionObserver* observer = iManager->TransactionObserver();
        if ( observer )
            {
            observer->TransactionFailed( *this, ids.iTransactionID, ids.iStatus );
            }
        }
    else
        {
        CleanupStack::Pop(); // sipParamsCleanup
        }

	delete aHeaders;
	delete aContentType;
	delete aContent;

    MCECLI_DEBUG("CMceSession::SendRequestL, Exit")
    return ids.iTransactionID;
    }    

// -----------------------------------------------------------------------------
// CMceSession::SendResponseL 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceSession::SendResponseL( TMceTransactionId aTransactionId,
					      				  const TDesC8& aReason,
				          				  TUint32 aCode,	
					      				  CDesC8Array* aHeaders,
					      				  HBufC8* aContentType,
					      				  HBufC8* aContent )
	{
	MCECLI_DEBUG("CMceSession::SendResponseL, Entry")
    TState state = State();
    MCECLI_DEBUG_CLISTATE( "state", state )

    User::LeaveIfError( State() != CMceSession::EEstablished );
       
    TCleanupItem sipParamsCleanup( SIPParamsCleanup, this );
    CleanupStack::PushL( sipParamsCleanup );

	TMceIds ids;
	PrepareForITC( ids );
	ids.iState = State();
	ids.iTransactionID = aTransactionId;

	HBufC8* reason = aReason.AllocLC();
	CMceMsgSIPReply* msg = new ( ELeave ) CMceMsgSIPReply( aTransactionId, 
														   *reason,
														   aCode,
														   *aHeaders,
														   *aContentType );
    CleanupStack::PushL( msg );
    // Don't pass aContent's ownership. Delete it when this function can't leave
	// anymore
    iSender->SendL( ids, EMceItcReplySend, *msg, aContent, EFalse );
    CleanupStack::PopAndDestroy( msg );
    CleanupStack::PopAndDestroy( reason );

	FLAT_DATA( iState ) = static_cast<TState>( ids.iState );
    MCECLI_DEBUG_CLISTATE( "CMceSession::SendResponseL, after ITC, state", state )
    
    if ( ids.iStatus != KErrNone )
        {
        MCECLI_DEBUG( "CMceSession::SendInfoRequestL, info failed. rollback" )
        
    	TMceIds ids2;
        SynchronizeL( ids2 );
        state = State();
        MCECLI_DEBUG_CLISTATE(
        	"CMceSession::SendInfoRequestL, after ITC, state", state )
        
        CleanupStack::Pop(); // sipParamsCleanup
               
        MMceTransactionObserver* observer = iManager->TransactionObserver();
        if ( observer )
            {
            // Pass the proper Error Event to the Client..
            observer->TransactionFailed( *this, ids.iTransactionID, KErrNone ); 
            }
        }
    else
        {
        CleanupStack::Pop(); // sipParamsCleanup
        }

	delete aHeaders;
	delete aContentType;
	delete aContent;

    MCECLI_DEBUG("CMceSession::SendInfoL, Exit")
	}

// End of File
