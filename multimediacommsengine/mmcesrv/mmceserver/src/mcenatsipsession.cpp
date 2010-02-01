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



#include <sipprofile.h>
#include <sipinvitedialogassoc.h>
#include <sdpdocument.h>
#include "mcenatsipsession.h"
#include "mcenatsipstatemachine.h"
#include "mcemediamanager.h"
#include "mcenatpluginmanager.h"
#include "mcecssession.h"
#include "mcesipconnection.h"
#include "mceclient.pan"
#include "mcesrvlogs.h"
#include "mcecomsession.h"

#define PANIC( aPanic ) User::Panic( KMceServerPanic, aPanic )
#define DEBUG_PANIC( aPanic ) __ASSERT_DEBUG( EFalse, PANIC( aPanic ) )

// ======== MEMBER FUNCTIONS ========
// -----------------------------------------------------------------------------
// CMceNatSipSession::NewL
// -----------------------------------------------------------------------------
//
CMceNatSipSession* CMceNatSipSession::NewL( CMceCsSession& aClientSession, 
                                            CMceSipConnection& aSIPConnection,
								            CMceSipStateMachine& aStateMachine,
								            CSIPProfile& aProfile,
								            const CDesC8Array* aParams )
	{
    CMceNatSipSession* self = CMceNatSipSession::NewLC( aClientSession, 
                                                        aSIPConnection, 
                                                        aStateMachine,
                                                        aProfile,
                                                        aParams );
    CleanupStack::Pop(self);
    return self;
	}

// -----------------------------------------------------------------------------
// CMceNatSipSession::NewL
// -----------------------------------------------------------------------------
//
CMceNatSipSession* CMceNatSipSession::NewL( CMceCsSession& aClientSession, 
                                            CMceSipConnection& aSIPConnection,
								            CMceSipStateMachine& aStateMachine,
								            const CDesC8Array* aParams )
	{
    CMceNatSipSession* self = CMceNatSipSession::NewLC( aClientSession, 
                                                        aSIPConnection, 
                                                        aStateMachine,
                                                        aParams );
    CleanupStack::Pop(self);
    return self;
	}


// -----------------------------------------------------------------------------
// CMceNatSipSession::NewLC
// -----------------------------------------------------------------------------
//
CMceNatSipSession* CMceNatSipSession::NewLC( CMceCsSession& aClientSession, 
                                             CMceSipConnection& aSIPConnection,
 								             CMceSipStateMachine& aStateMachine,
 								             CSIPProfile& aProfile,
 								             const CDesC8Array* aParams )
	{
    CMceNatSipSession* self = new (ELeave) CMceNatSipSession( aClientSession, 
                                                              aSIPConnection, 
                                                              aStateMachine,
                                                              aProfile );
    CleanupStack::PushL(self);
    self->ConstructL( aClientSession, aSIPConnection, aParams );
    return self;
	}

// -----------------------------------------------------------------------------
// CMceNatSipSession::NewLC
// -----------------------------------------------------------------------------
//
CMceNatSipSession* CMceNatSipSession::NewLC( CMceCsSession& aClientSession, 
                                             CMceSipConnection& aSIPConnection,
 								             CMceSipStateMachine& aStateMachine,
 								             const CDesC8Array* aParams )
	{
    CMceNatSipSession* self = new (ELeave) CMceNatSipSession( aClientSession, 
                                                              aSIPConnection, 
                                                              aStateMachine );
    CleanupStack::PushL(self);
    self->ConstructL( aClientSession, aSIPConnection, aParams );
    return self;
	}

// -----------------------------------------------------------------------------
// CMceNatSipSession::CMceNatSipSession
// -----------------------------------------------------------------------------
//
CMceNatSipSession::CMceNatSipSession( CMceCsSession& aClientSession, 
								      CMceSipConnection& aSIPConnection,
								      CMceSipStateMachine& aStateMachine,
								      CSIPProfile& aProfile )
    : CMceSipSession( aClientSession, aSIPConnection, aStateMachine, aProfile )
	{
	}

// -----------------------------------------------------------------------------
// CMceNatSipSession::CMceNatSipSession
// -----------------------------------------------------------------------------
//
CMceNatSipSession::CMceNatSipSession( CMceCsSession& aClientSession, 
								      CMceSipConnection& aSIPConnection,
								      CMceSipStateMachine& aStateMachine )
    : CMceSipSession( aClientSession, aSIPConnection, aStateMachine )
	{
	}

// -----------------------------------------------------------------------------
// CMceNatSipSession::ConstructL
// -----------------------------------------------------------------------------
//
void CMceNatSipSession::ConstructL( CMceCsSession& aClientSession,
                                    CMceSipConnection& aSIPConnection,
                                    const CDesC8Array* aParams )
	{
    MCESRV_DEBUG( "*** CMceNatSipSession::ConstructL, Entry" )

	CMceSipSession::ConstructL( aClientSession ); // base class

    TRAPD( error, TryNatSessionL( aSIPConnection, aParams ) );

    if ( KErrNone != error )
        {
        NatErrorOccurred( error );
        }
    else
    	{
    	iNatSession = ETrue;
    	}
    MCESRV_DEBUG( "*** CMceNatSipSession::ConstructL, Exit" )  
	}

// -----------------------------------------------------------------------------
// CMceNatSipSession::TryNatSessionL
// -----------------------------------------------------------------------------
//
void CMceNatSipSession::TryNatSessionL( CMceSipConnection& aSIPConnection,
                                        const CDesC8Array* aParams )
    {
    iNatStateMachine = CMceNatSipStateMachine::NewL();
    
    TUint32 iapId = aSIPConnection.AccessPointId();
    
    const TDesC8* userAor = NULL;
    TPtrC8 originator;
    if ( ProfileConfigured() )
        {
        MCESRV_DEBUG( "*** CMceNatSipSession::TryNatSessionL, using profile" )
        
        CSIPProfile& profile = Profile();
        User::LeaveIfError( profile.GetParameter( KSIPUserAor, userAor ) );
        }
    else
        {
        MCESRV_DEBUG( "*** CMceNatSipSession::TryNatSessionL, profile not used" )
        
        __ASSERT_ALWAYS( aParams && 
                         aParams->MdcaCount() > KMceArrayIndexOriginator, 
                         User::Leave( KErrArgument ) );
        originator.Set( aParams->MdcaPoint( KMceArrayIndexOriginator ) );
        userAor = &originator;
        }
    
    TUriParser8 uri8Parser;
    User::LeaveIfError( uri8Parser.Parse( *userAor ) );
    CUri8* cUri8 = CUri8::NewLC( uri8Parser );
    const TDesC8& domain = cUri8->Uri().Extract( EUriHost );
    
    CMceNatPluginManager& manager = Manager().MediaManager().NatPluginManager();
    iSessionId = manager.NewSessionL( *this, iapId, domain );
    
    CleanupStack::PopAndDestroy( cUri8 );
    
    MCESRV_DEBUG( "*** CMceNatSipSession::TryNatSessionL, Exit" )
    }

// -----------------------------------------------------------------------------
// CMceNatSipSession::~CMceNatSipSession
// -----------------------------------------------------------------------------
//
CMceNatSipSession::~CMceNatSipSession()
	{
	MCESRV_DEBUG( "*** CMceNatSipSession::~CMceNatSipSession, Entry" )
	
    delete iNatStateMachine;
    
    CMceNatPluginManager& manager = Manager().MediaManager().NatPluginManager();
    TRAP_IGNORE( manager.CloseSessionL( iSessionId ) )
    
    MCESRV_DEBUG( "*** CMceNatSipSession::~CMceNatSipSession, Exit" )
  	}

// -----------------------------------------------------------------------------
// CMceNatSipSession::UpdateMediaL
// -----------------------------------------------------------------------------
//
TMceReturnStatus CMceNatSipSession::UpdateMediaL()
    {
    MCESRV_DEBUG( "*** CMceNatSipSession::UpdateMediaL, Entry" )
    
    PrepareNatSessionUpdate();
    
    TMceReturnStatus status = CMceSipSession::UpdateMediaL();
    if ( KMceReady == status )
        {
        TMceNatStateTransitionEvent event( NULL, *this, EMceNatUpdated, *this,
        								   iSipEvent );
        
        if ( !iNatDisabled && iNatStateMachine->Accept( event ) )
            {
            MCESRV_DEBUG( "*** CMceNatSipSession::UpdateMediaL, processing" )
            
            iEntryPoint = ENatEntryUpdated;
            event.ParamStatus() = KMceReady;
            
            TRAPD( error, iNatStateMachine->ProcessL( event ) )
            
            if ( error != KErrNone )
                {
                iEntryPoint = ENatEntryNone;
                NatErrorOccurred( error );
                }
            else
                {
                status = event.ParamStatus();
                }
            }
        }
    
    MCESRV_DEBUG( "*** CMceNatSipSession::UpdateMediaL, Exit" )
    return status;
    }

// -----------------------------------------------------------------------------
// CMceNatSipSession::ReserveL
// -----------------------------------------------------------------------------
//
TMceReturnStatus CMceNatSipSession::ReserveL()
    {
    MCESRV_DEBUG("CMceNatSipSession::ReserveL, Entry")

    iReserveStatus = CMceSipSession::ReserveL();
    if ( iReserveStatus == KMceReady )
    	{
    	TMceReturnStatus async = 0;
    	iReserveStatus =
    		GetResourceReservationStatus() == async ? KMceAsync : KMceReady;
    	}
    
    return iReserveStatus;
    }

// -----------------------------------------------------------------------------
// CMceNatSipSession::Updated
// -----------------------------------------------------------------------------
//
void CMceNatSipSession::Updated( CMceComSession& aSession )
    {
    MCESRV_DEBUG( "*** CMceNatSipSession::Updated, Entry" )
    
    TMceNatStateTransitionEvent event(
            NULL, *this, EMceNatUpdated, *this, iSipEvent );
    
    if ( !iNatDisabled &&
         iNatStateMachine->Accept( event ) )
        {
        MCESRV_DEBUG( "*** CMceNatSipSession::Updated, processing" )

        iEntryPoint = ENatEntryUpdated;
        event.ParamStatus() = KMceAsync;
        
        TRAPD( error, iNatStateMachine->ProcessL( event ) )
        
        if ( KErrNone != error )
            {
            iEntryPoint = ENatEntryNone;
            CMceSipSession::Updated( aSession );
            NatErrorOccurred( error );
            }
        }
    else
        {
        MCESRV_DEBUG( "*** CMceNatSipSession::Updated, passing forward" )
        CMceSipSession::Updated( aSession );
        }
		
    MCESRV_DEBUG("*** CMceNatSipSession::Updated, Exit" )
    }

// -----------------------------------------------------------------------------
// CMceNatSipSession::Offer
// -----------------------------------------------------------------------------
//
CSdpDocument* CMceNatSipSession::Offer()
    {
    if ( KMceNatPrevOfferSdp == iOfferedSdp )
        {
        MCESRV_DEBUG("*** CMceNatSipSession::Offer, iPrevOffer" )
        return iPrevOffer;        
        }
    else
        {
        return CMceSipSession::Offer();
        }
    }

// -----------------------------------------------------------------------------
// CMceNatSipSession::NatSession
// -----------------------------------------------------------------------------
//
TBool CMceNatSipSession::NatSession() const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CMceNatSipSession::NatState
// -----------------------------------------------------------------------------
//
TInt CMceNatSipSession::NatState() const
    {
    return iNatStateMachine->State();
    }

// -----------------------------------------------------------------------------
// CMceNatSipSession::IsNatDisabled
// -----------------------------------------------------------------------------
//
TBool CMceNatSipSession::IsNatDisabled() const
	{
	return iNatDisabled;
	}

// -----------------------------------------------------------------------------
// CMceNatSipSession::Initialized
// -----------------------------------------------------------------------------
//
void CMceNatSipSession::Initialized( TUint aSessionId )
    {
    MCESRV_DEBUG( "*** CMceNatSipSession::Initialized, Entry" )
    
    __ASSERT_DEBUG( aSessionId == iSessionId, PANIC( KErrArgument ) );
    
    TMceNatStateTransitionEvent event( 
            NULL, *this, EMceNatInitialized, *this, iSipEvent );
    
    if ( !iNatDisabled &&
         aSessionId == iSessionId &&
         iNatStateMachine->Accept( event ) )
        {
        MCESRV_DEBUG( "*** CMceNatSipSession::Initialized, processing" )
        
        TRAPD( error, iNatStateMachine->ProcessL( event ) )
            
        if ( KErrNone != error )
            {
            NatErrorOccurred( error );
            }        
        }
    else
        {
        DEBUG_PANIC( KErrNotReady );
        }
    
    MCESRV_DEBUG( "*** CMceNatSipSession::Initialized, Entry" )
    }

// -----------------------------------------------------------------------------
// CMceNatSipSession::OfferReady
// -----------------------------------------------------------------------------
//
void CMceNatSipSession::OfferReady( TUint aSessionId, CSdpDocument* aOffer )
    {
    MCESRV_DEBUG("*** CMceNatSipSession::OfferReady, Entry" )
    
    __ASSERT_DEBUG( aSessionId == iSessionId, PANIC( KErrArgument ) );
    
    CMceNatPluginManager::WriteSdpToLog( _L("Offer"), aOffer );
    
    TMceNatStateTransitionEvent event(
            aOffer, *this, EMceNatOffer, *this, iSipEvent );
    
    if ( !iNatDisabled &&
         aSessionId == iSessionId &&
         iNatStateMachine->Accept( event ) )
        {
        MCESRV_DEBUG( "*** CMceNatSipSession::OfferReady, processing" )
        
        TRAPD( error, iNatStateMachine->ProcessL( event ) )
        
        if ( KErrNone != error )
            {
            NatErrorOccurred( error );
            }
        }
    else
        {
        SafeDelete( aOffer );
        DEBUG_PANIC( KErrNotReady );
        }
    
    MCESRV_DEBUG("*** CMceNatSipSession::OfferReady, Exit" )
    }

// -----------------------------------------------------------------------------
// CMceNatSipSession::AnswerReady
// -----------------------------------------------------------------------------
//
void CMceNatSipSession::AnswerReady( TUint aSessionId, CSdpDocument* aAnswer )
    {
    MCESRV_DEBUG("*** CMceNatSipSession::AnswerReady, Entry" )
    
    __ASSERT_DEBUG( aSessionId == iSessionId, PANIC( KErrArgument ) );
    
    CMceNatPluginManager::WriteSdpToLog( _L("Answer"), aAnswer );
    
    TMceNatStateTransitionEvent event(
            aAnswer, *this, EMceNatAnswer, *this, iSipEvent );
    
    if ( !iNatDisabled &&
         aSessionId == iSessionId &&
         iNatStateMachine->Accept( event ) )
        {
        MCESRV_DEBUG( "*** CMceNatSipSession::AnswerReady, processing" )
        
        TRAPD( error, iNatStateMachine->ProcessL( event ) );
        
        if ( KErrNone != error )
            {
            NatErrorOccurred( error );
            }
        }
    else
        {
        SafeDelete( aAnswer );
        DEBUG_PANIC( KErrNotReady );
        }
    
    MCESRV_DEBUG("*** CMceNatSipSession::AnswerReady, Exit" )
    }
    
// -----------------------------------------------------------------------------
// CMceNatSipSession::UpdateSdp
// -----------------------------------------------------------------------------
//
void CMceNatSipSession::UpdateSdp( TUint aSessionId, CSdpDocument* aOffer )
    {
    MCESRV_DEBUG( "*** CMceNatSipSession::UpdateSdp, Entry" )

    __ASSERT_DEBUG( aSessionId == iSessionId, PANIC( KErrArgument ) );
    
    TMceNatStateTransitionEvent event(
            aOffer, *this, EMceNatUpdateSdp, *this, iSipEvent );
    
    if ( !iNatDisabled &&
         aSessionId == iSessionId &&
         iNatStateMachine->Accept( event ) )
        {
        MCESRV_DEBUG( "*** CMceNatSipSession::UpdateSdp, processing" )
        
        TRAPD( error, iNatStateMachine->ProcessL( event ) );
        SafeDelete( aOffer );
        if ( KErrNone != error )
            {
            NatErrorOccurred( error );
            }
        }
    else
        {
        SafeDelete( aOffer );
        DEBUG_PANIC( KErrNotReady );
        }
    
    MCESRV_DEBUG( "*** CMceNatSipSession::UpdateSdp, Exit" )
    }
    
// -----------------------------------------------------------------------------
// CMceNatSipSession::ErrorOccurred
// -----------------------------------------------------------------------------
//
void CMceNatSipSession::ErrorOccurred( TUint aSessionId, TInt aError )
    {
    MCESRV_DEBUG( "*** CMceNatSipSession::ErrorOccurred, Entry" )
    MCESRV_DEBUG_DVALUE( "*** error", aError )
    
    __ASSERT_DEBUG( aSessionId == iSessionId, PANIC( KErrArgument ) );
    
    TMceNatStateTransitionEvent event(
            NULL, *this, EMceNatError, *this, iSipEvent, aError );
    
    if ( !iNatDisabled &&
         aSessionId == iSessionId &&
         iNatStateMachine->Accept( event ) )
        {
        MCESRV_DEBUG( "*** CMceNatSipSession::ErrorOccurred, processing" )
        
        TRAPD( error, iNatStateMachine->ProcessL( event ) )
        
        if ( KErrNone != error )
            {
            NatErrorOccurred( error );
            }
        }
    
    MCESRV_DEBUG( "*** CMceNatSipSession::ErrorOccurred, Exit" )
    }
  
// -----------------------------------------------------------------------------
// CMceNatSipSession::IcmpErrorOccurred
// -----------------------------------------------------------------------------
//
void CMceNatSipSession::IcmpErrorOccurred( TUint aSessionId, TInt aError )
    {
    MCESRV_DEBUG( "*** CMceNatSipSession::IcmpErrorOccurred, Entry" )
    MCESRV_DEBUG_DVALUE( "*** error", aError )
    
    __ASSERT_DEBUG( aSessionId == iSessionId, PANIC( KErrArgument ) );
       
    TMceNatStateTransitionEvent event(
            NULL, *this, EMceNatICMPError, *this, iSipEvent, aError );
    
    if ( !iNatDisabled &&
         aSessionId == iSessionId &&
         iNatStateMachine->Accept( event ) )
        {
        MCESRV_DEBUG( "*** CMceNatSipSession::IcmpErrorOccurred, processing" )
        
        TRAPD( error, iNatStateMachine->ProcessL( event ) )
        
        if ( KErrNone != error )
            {
            NatErrorOccurred( error );
            }
        }
    
    MCESRV_DEBUG( "*** CMceNatSipSession::IcmpErrorOccurred, Exit" )
    }

// -----------------------------------------------------------------------------
// CMceNatSipSession::CreateOfferL
// -----------------------------------------------------------------------------
//
TInt CMceNatSipSession::CreateOfferL()
    {
    SetMediaQosL();
    
    CSdpDocument* offer = Offer();
    CMceNatPluginManager& manager = Manager().MediaManager().NatPluginManager();
    return manager.CreateOfferL( iSessionId, offer );
    }

// -----------------------------------------------------------------------------
// CMceNatSipSession::ResolveL
// -----------------------------------------------------------------------------
//
TInt CMceNatSipSession::ResolveL()
    {
    SetMediaQosL();
    
    CSdpDocument* offer = PrevOffer();
    CSdpDocument* answer = Offer();
    CMceNatPluginManager& manager = Manager().MediaManager().NatPluginManager();
    return manager.ResolveL( iSessionId, offer, answer );
    }

// -----------------------------------------------------------------------------
// CMceNatSipSession::DecodeAnswerL
// -----------------------------------------------------------------------------
//
TInt CMceNatSipSession::DecodeAnswerL()
    {
    CSdpDocument* answer = Offer();
    CMceNatPluginManager& manager = Manager().MediaManager().NatPluginManager();
    return manager.DecodeAnswerL( iSessionId, answer );
    }

// -----------------------------------------------------------------------------
// CMceNatSipSession::UpdateL
// -----------------------------------------------------------------------------
//
void CMceNatSipSession::UpdateL()
    {
    CSdpDocument* offer = Offer();
    CMceNatPluginManager& manager = Manager().MediaManager().NatPluginManager();
    manager.UpdateL( iSessionId, offer );
    }

// -----------------------------------------------------------------------------
// CMceNatSipSession::UpdateRemoteAddressL
// -----------------------------------------------------------------------------
//
void CMceNatSipSession::UpdateRemoteAddressL()
    {    
    CMceMediaManager& manager = Manager().MediaManager();
    RPointerArray<CMceSrvStream>& streams = ActiveBody().MccStreams();
    
    for ( TInt i = 0; i < streams.Count(); i++ )
        {
        manager.SetRemoteAddressL( *streams[i] );
        }
    }

// -----------------------------------------------------------------------------
// CMceNatSipSession::SetOffered
// -----------------------------------------------------------------------------
//
void CMceNatSipSession::SetOffered( TMceNatOfferedSdp aOffered )
    {
    iOfferedSdp = aOffered;
    }

// -----------------------------------------------------------------------------
// CMceNatSipSession::SdpCleanup
// -----------------------------------------------------------------------------
//
void CMceNatSipSession::SdpCleanup( CSdpDocument* aPrev, CSdpDocument* aRep )
    {
    CMceSipSession::SdpCleanup( aPrev, aRep );
    }

// -----------------------------------------------------------------------------
// CMceNatSipSession::Continue
// -----------------------------------------------------------------------------
//
void CMceNatSipSession::Continue() 
    {
    switch( iEntryPoint )
        {
        case ENatEntryUpdated:
            {
            CMceSipSession::Updated( ActiveBody() );
            break;
            }
        
        case ENatEntryHandleSIPEvent:
            {
            CMceSipSession::HandleSIPEvent( iSipEvent, Dialog()->Dialog() );
            break;
            }
        
        default:
            {
            MCESRV_DEBUG( "*** CMceNatSipSession::Continue, default!" )
            DEBUG_PANIC( KErrTotalLossOfPrecision );
            }
        }
    }

// -----------------------------------------------------------------------------
// CMceNatSipSession::StateChanged
// -----------------------------------------------------------------------------
//
void CMceNatSipSession::StateChangedL( TMceNatStateTransitionEvent& aEvent,
                                       TUint aNewState )
    {
    iNatStateMachine->NewStateL( aEvent, aNewState );
    }

// -----------------------------------------------------------------------------
// CMceNatSipSession::HandleSIPEvent
// -----------------------------------------------------------------------------
//
void CMceNatSipSession::HandleSIPEvent( TMceSipEventCode aEventCode, CSIPDialog& aDialog )
    {
    MCESRV_DEBUG( "*** CMceNatSipSession::HandleSIPEvent, Entry" )
    
    TMceNatStateTransitionEvent event(
            NULL, *this, EMceNatHandleSIP, *this, aEventCode );
    
    if ( !iNatDisabled && iNatStateMachine->Accept( event ) )
        {
        MCESRV_DEBUG( "*** CMceNatSipSession::HandleSIPEvent, processing" )

        iEntryPoint = ENatEntryHandleSIPEvent;
        iSipEvent = aEventCode;
        event.ParamStatus() = KMceAsync;
        
        TRAPD( error, iNatStateMachine->ProcessL( event ) );
        
        if ( KErrNone != error )
            {
	        iEntryPoint = ENatEntryNone;
            CMceSipSession::HandleSIPEvent( aEventCode, aDialog );
            NatErrorOccurred( error );
            }
        }
    else
        {
        MCESRV_DEBUG( "*** CMceNatSipSession::HandleSIPEvent, passing forward" )
        CMceSipSession::HandleSIPEvent( aEventCode, aDialog );        
        }
    	
    MCESRV_DEBUG( "*** CMceNatSipSession::HandleSIPEvent, Exit" )
    }

// -----------------------------------------------------------------------------
// CMceNatSipSession::SafeDelete
// -----------------------------------------------------------------------------
//
void CMceNatSipSession::SafeDelete( CSdpDocument* aDocument )
    {
    if ( iOffer != aDocument && iPrevOffer != aDocument )
        {
        delete aDocument;
        }
    }

// -----------------------------------------------------------------------------
// CMceNatSipSession::NatErrorOccurred
// -----------------------------------------------------------------------------
//
void CMceNatSipSession::NatErrorOccurred( TInt /*aError*/ )
    {
    MCESRV_DEBUG( "*** CMceNatSipSession::NatErrorOccurred, Entry")

    // Local handling
    if ( iNatStateMachine )
    	{
    	// If error occurs during creation of NatStateMachine, iNatStateMachine
    	// is NULL.
    	iNatStateMachine->ErrorOccurred();	
    	}
    
    iNatDisabled = ETrue;
    // Free resources reserved by NAT Session
    CMceNatPluginManager& manager = Manager().MediaManager().NatPluginManager();
    TRAP_IGNORE( manager.CloseSessionL( iSessionId ) );
    PrepareNatSessionUpdate();
    }

// -----------------------------------------------------------------------------
// CMceNatSipSession::PrepareNatSessionUpdate
// -----------------------------------------------------------------------------
//
void CMceNatSipSession::PrepareNatSessionUpdate()
    {
    MCESRV_DEBUG( "*** CMceNatSipSession::PrepareNatSessionUpdate, Entry" )
    
    CMceComSession* activeBody = &ActiveBody();
    if ( activeBody )
        {
	    TBool ignorePortComparison( !iNatDisabled );
	    RPointerArray<CMceComMediaStream>& streams = activeBody->Streams();
	    for ( TInt i = 0; i < streams.Count(); i++ )
	        {
	        CMceComMediaStream* stream = streams[ i ];
	        stream->SetIgnoreRemotePort( ignorePortComparison );
	        if ( stream->BoundStream() )
	            {
	            stream->iLinkedStream->SetIgnoreRemotePort(
	            	ignorePortComparison );
	            }
	        }
        }
    
    MCESRV_DEBUG( "*** CMceNatSipSession::PrepareNatSessionUpdate, Exit" )
    }

// -----------------------------------------------------------------------------
// CMceNatSipSession::SetMediaQosL
// -----------------------------------------------------------------------------
//
void CMceNatSipSession::SetMediaQosL()
    {
    CMceComSession* activeBody = &ActiveBody();
    __ASSERT_ALWAYS( activeBody, User::Leave( KErrTotalLossOfPrecision ) );
    
    CMceNatPluginManager& manager = Manager().MediaManager().NatPluginManager();
    User::LeaveIfError( manager.SetSessionParam( iSessionId, 
        CNSPPlugin::ENSPMediaTypeOfServiceKey,
        activeBody->iServiceType ) );
    }

// -----------------------------------------------------------------------------
// CMceNatSipSession::SetNeedtoReceive 
// -----------------------------------------------------------------------------
//
void CMceNatSipSession::SetNeedtoReceive()
	{
	MCESRV_DEBUG( "*** CMceNatSipSession::SetNeedtoReceive, Entry" )	
	iForceSDPSending = ETrue;
	MCESRV_DEBUG( "*** CMceNatSipSession::SetNeedtoReceive, Exit" )		
	}

// -----------------------------------------------------------------------------
// CMceNatSipSession::GetResourceReservationStatus 
// -----------------------------------------------------------------------------
//
TInt CMceNatSipSession::GetResourceReservationStatus()
	{
	MCESRV_DEBUG( "*** CMceNatSipSession::GetResourceReservationStatus, Entry" )	

	CMceNatPluginManager& manager = Manager().MediaManager().NatPluginManager();
    TUint status = manager.GetSessionParam( iSessionId,
    							CNSPPlugin::ENSPResourseReservationStatusKey );

	MCESRV_DEBUG( "*** CMceNatSipSession::GetResourceReservationStatus, Exit" )		
	return status;
	}

// -----------------------------------------------------------------------------
// CMceNatSipSession::GetReservationStatus 
// -----------------------------------------------------------------------------
//
TMceReturnStatus CMceNatSipSession::GetReservationStatus()
	{
	return iReserveStatus;
	}

// -----------------------------------------------------------------------------
// CMceNatSipSession::SetReservationStatus 
// -----------------------------------------------------------------------------
//
void CMceNatSipSession::SetReservationStatus( TMceReturnStatus aStatus )
	{
	iReserveStatus = aStatus;
	}

// -----------------------------------------------------------------------------
// CMceNatSipSession::ResumeNatUpdated
// -----------------------------------------------------------------------------
//
void CMceNatSipSession::ResumeNatUpdated()
	{
	MCESRV_DEBUG( "*** CMceNatSipSession::ResumeNatUpdated, Entry" )

	if( !iNatDisabled )
        {
		TMceNatStateTransitionEvent event( NULL, *this, EMceNatUpdated, *this,
										   iSipEvent );
        iEntryPoint = ENatEntryUpdated;
        // iComSession was already set in UpdateMediaL() or Updated()
        event.ParamStatus() = KMceAsync;

        TRAPD( error, iNatStateMachine->ProcessL( event ) );
		if ( error != KErrNone )
            {
            iEntryPoint = ENatEntryNone;
            CMceSipSession::Updated( ActiveBody() );
            NatErrorOccurred( error );
            }
		}
    else
    	{
        CMceSipSession::Updated( ActiveBody() );
        }

	MCESRV_DEBUG( "*** CMceNatSipSession::ResumeNatUpdated, Exit" )
	}

// End of File
