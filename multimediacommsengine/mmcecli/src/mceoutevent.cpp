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
#include "mceoutevent.h"
#include "mcesession.h"
#include "mcerefer.h"
#include "mcemanager.h"
#include "mceclientserver.h"
#include "mceitcsender.h"
#include "mceserial.h"
#include "mceclilogs.h"

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceOutEvent::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceOutEvent* CMceOutEvent::NewL(
					        CMceManager& aManager,
					        CSIPProfile& aProfile,
					        const TDesC8& aRecipient,
							const TDesC8& aEventHeader,
		    				TUint32 aRefreshInterval,
					        HBufC8* aOriginator ) 
    {
    MCECLI_DEBUG("CMceOutEvent::NewL, Entry");
    MCECLI_DEBUG_SVALUE("recipient", aRecipient );
    MCECLI_DEBUG_SVALUE("event header", aEventHeader );
    
    TUint32 profileId(0);
    if ( aProfile.GetParameter( KSIPProfileId, profileId ) != KErrNone )
        {
        User::Leave(KErrNotFound);
        }

    CMceOutEvent* self = new (ELeave) CMceOutEvent( aManager, 
                                                    profileId,
                                                    KMceNotAssigned,
                                                    aRefreshInterval );
    CleanupStack::PushL( self );
    self->ConstructL( aEventHeader, 
                      aRecipient, 
                      aOriginator, 
                      KMceDlgTypeSubscribe,
                      NULL );
    CleanupStack::Pop( self );
    MCECLI_DEBUG("CMceOutEvent::NewL, Exit");
    return self;  
    }
    
// -----------------------------------------------------------------------------
// CMceOutEvent::NewL
// -----------------------------------------------------------------------------
//    
EXPORT_C CMceOutEvent* CMceOutEvent::NewL(
					  CMceEvent& aEvent,
					  const TDesC8& aEventHeader,
		    		  TUint32 aRefreshInterval )
    {
    MCECLI_DEBUG("CMceOutEvent::NewL, Entry");
    MCECLI_DEBUG_DVALUE("event", aEvent.Id() );
    MCECLI_DEBUG_SVALUE("event header", aEventHeader );
    
    CMceOutEvent* self = new (ELeave) CMceOutEvent( aEvent.Manager(), 
                                                    aEvent.ProfileId(),
                                                    aEvent.DialogId(),
                                                    aRefreshInterval );
    CleanupStack::PushL( self );
    
    self->ConstructL( aEventHeader, 
                      aEvent.Recipient(), 
                      aEvent.Originator(),
                      KMceDlgTypeSubscribe );
    CleanupStack::Pop( self );
    MCECLI_DEBUG("CMceOutEvent::NewL, Exit");
    return self;  
    }

// -----------------------------------------------------------------------------
// CMceOutEvent::NewL
// -----------------------------------------------------------------------------
//        
EXPORT_C CMceOutEvent* CMceOutEvent::NewL(
					  CMceSession& aSession,
					  const TDesC8& aEventHeader,
		    		  TUint32 aRefreshInterval )
    {
    MCECLI_DEBUG("CMceOutEvent::NewL, Entry");
    MCECLI_DEBUG_DVALUE("session", aSession.Id() );
    MCECLI_DEBUG_SVALUE("event header", aEventHeader );
    
    CMceOutEvent* self = new (ELeave) CMceOutEvent( aSession.Manager(), 
                                                    aSession.Profile(),
                                                    aSession.DialogId(),
                                                    aRefreshInterval );
    CleanupStack::PushL( self );
    self->ConstructL( aEventHeader, 
                      aSession.Recipient(), 
                      aSession.Originator(),
                      KMceDlgTypeSubscribe );
    CleanupStack::Pop( self );
    
    MCECLI_DEBUG("CMceOutEvent::NewL, Exit");
    
    return self; 
    }

// -----------------------------------------------------------------------------
// CMceOutEvent::NewL
// -----------------------------------------------------------------------------
//    
EXPORT_C CMceOutEvent* CMceOutEvent::NewL(
					  CMceRefer& aRefer,
					  const TDesC8& aEventHeader,
		    		  TUint32 aRefreshInterval )
    {
    MCECLI_DEBUG("CMceOutEvent::NewL, Entry");
    MCECLI_DEBUG_DVALUE("refer", aRefer.Id() );
    MCECLI_DEBUG_SVALUE("event header", aEventHeader );
    
    CMceOutEvent* self = new (ELeave) CMceOutEvent( aRefer.Manager(), 
                                                    aRefer.ProfileId(),
                                                    aRefer.DialogId(),
                                                    aRefreshInterval );
    CleanupStack::PushL( self );
    self->ConstructL( aEventHeader, 
                      aRefer.Recipient(), 
                      aRefer.Originator(),
                      KMceDlgTypeSubscribe );
    CleanupStack::Pop( self );
    MCECLI_DEBUG("CMceOutEvent::NewL, Exit");
    
    return self;  
    }

// -----------------------------------------------------------------------------
// CMceOutEvent::NewL
// -----------------------------------------------------------------------------
//
CMceOutEvent* CMceOutEvent::NewL(
					        CMceManager& aManager,
					        TUint32 aProfileId,
					        const TDesC8& aRecipient,
							const TDesC8& aEventHeader,
		    				TUint32 aRefreshInterval,
					        HBufC8* aOriginator,
					        TUint32 aDialogId,
					        TUint32 aDialogType,
					        CDesC8Array* aParams ) 
    {
    MCECLI_DEBUG("CMceOutEvent::NewL, Entry");
    MCECLI_DEBUG_DVALUE("profile", aProfileId );
    MCECLI_DEBUG_SVALUE("recipient", aRecipient );
    MCECLI_DEBUG_SVALUE("event header", aEventHeader );
    MCECLI_DEBUG_DVALUE("dialog id", aDialogId );
        
    CMceOutEvent* self = new (ELeave) CMceOutEvent( aManager, 
                                                    aProfileId,
                                                    aDialogId,
                                                    aRefreshInterval );
    CleanupStack::PushL( self );
    self->ConstructL( aEventHeader, 
                      aRecipient, 
                      aOriginator, 
                      aDialogType,
                      aParams );
    CleanupStack::Pop( self );
    MCECLI_DEBUG("CMceOutEvent::NewL, Exit");
    return self;  
    }
 
// -----------------------------------------------------------------------------
// CMceOutEvent::~CMceOutEvent
// -----------------------------------------------------------------------------
//
EXPORT_C CMceOutEvent::~CMceOutEvent()
    {
    }

// -----------------------------------------------------------------------------
// CMceOutEvent::SubscribeL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceOutEvent::SubscribeL(
			 CDesC8Array* aHeaders,
			 HBufC8* aContentType,
			 HBufC8* aContent )
    {
    MCECLI_DEBUG("CMceOutEvent::SubscribeL, Entry");
    TUint state = State();
    MCECLI_DEBUG_EVENTSTATE( "state", state );
    
    User::LeaveIfError( State() == EIdle ? KErrNone : KErrNotReady );
    User::LeaveIfError( 
        ( aContentType && aContent ) || !( aContentType || aContent ) ? 
        KErrNone : KErrArgument );
    
	TMceIds ids;
	PrepareForITC( ids );
	ids.iState = State();
    
    CMceMsgSIPEvent* subscribe = NULL;
    // stand alone
    subscribe = new (ELeave) CMceMsgSIPEvent(  iId,
                                   iDialogId,
                                   EMceItcEventTypeSubscribe,
                                   CMceRefer::ENoSuppression, 
                                   iRefreshInterval,
                                   aHeaders,
                                   aContentType );
        
    CleanupStack::PushL( subscribe );
    subscribe->OwnershipRollbackPushLC();
    
    iSender->SendL( ids, EMceItcSubscribe , *subscribe, aContent );
    
    CleanupStack::Pop(); // subscribe->OwnershipRollbackPushLC()
    CleanupStack::PopAndDestroy( subscribe );
    
    iState = static_cast<TState>( ids.iState );
    state = State();
    MCECLI_DEBUG_EVENTSTATE( "CMceEvent::SubscribeL, after ITC, state", state );
    MCECLI_DEBUG("CMceEvent::SubscribeL, Exit");
    }

// -----------------------------------------------------------------------------
// CMceOutEvent::CMceOutEvent
// -----------------------------------------------------------------------------
//
CMceOutEvent::CMceOutEvent( CMceManager& aManager, 
                            TUint32 aProfileId,
                            TUint32 aDialogId,
                            TUint32 aRefreshInterval )
    : CMceEvent( aManager, aProfileId, aDialogId, aRefreshInterval )
    {
    }

// -----------------------------------------------------------------------------
// CMceOutEvent::ConstructL
// -----------------------------------------------------------------------------
//
void CMceOutEvent::ConstructL( const TDesC8& aEventHeader, 
                               const TDesC8& aRecipient, 
                               HBufC8* aOriginator,
                               TUint32 aDialogType,
                               CDesC8Array* aParams )
    {
    HBufC8* eventHeader = aEventHeader.AllocLC();
    HBufC8* recipient = aRecipient.AllocLC();
 
    CMceEvent::ConstructL( eventHeader, 
                           recipient, 
                           aOriginator,
                           aDialogType,
                           aParams );    
    
    CleanupStack::Pop( recipient );
    CleanupStack::Pop( eventHeader );
    }

// -----------------------------------------------------------------------------
// CMceOutEvent::ConstructL
// -----------------------------------------------------------------------------
//
void CMceOutEvent::ConstructL( const TDesC8& aEventHeader, 
                               const TDesC8& aRecipient, 
                               const TDesC8& aOriginator,
                               TUint32 aDialogType )
    {
    HBufC8* eventHeader = aEventHeader.AllocLC();
    HBufC8* recipient = aRecipient.AllocLC();
    HBufC8* originator = aOriginator.AllocLC();
    
    CMceEvent::ConstructL( eventHeader, 
                           recipient, 
                           originator,
                           aDialogType,
                           NULL );
                           
    CleanupStack::Pop( originator ); 
    CleanupStack::Pop( recipient );
    CleanupStack::Pop( eventHeader ); 
    }
    
// -----------------------------------------------------------------------------
// CMceOutEvent::UpdateL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceOutEvent::UpdateL( TUint32 aRefreshInterval,
                                     CDesC8Array* aHeaders,
			                         HBufC8* aContentType,
				                     HBufC8* aContent )
    {
    MCECLI_DEBUG("CMceOutEvent::UpdateL, Entry");
    TUint state = State();
    MCECLI_DEBUG_EVENTSTATE( "state", state );
    
    User::LeaveIfError( aRefreshInterval > 0 ? KErrNone : KErrArgument );
    User::LeaveIfError( State() == EActive ? KErrNone : KErrNotReady );
    User::LeaveIfError( 
        ( aContentType && aContent ) || !( aContentType || aContent ) ? 
        KErrNone : KErrArgument );
    
    CMceRefer* refer = iManager.FindRefer( *this );
    if ( refer )
        {
        // Updating not supported for event attached to refer.
        User::Leave( KErrNotSupported ); 
        }
    
    iRefreshInterval = aRefreshInterval;
    
	TMceIds ids; 
	PrepareForITC( ids );
	ids.iState = State();
	
	CMceMsgSIPEvent* update = 
	    new (ELeave) CMceMsgSIPEvent(  iId,
	                                   iDialogId,
	                                   EMceItcEventTypeSubscribe,
	                                   CMceRefer::ENoSuppression, 
                                       iRefreshInterval,
                                       aHeaders,
                                       aContentType );
    CleanupStack::PushL( update );
    update->OwnershipRollbackPushLC();
    
    iSender->SendL( ids, EMceItcSubscribeUpdate, *update, aContent );    
    
    CleanupStack::Pop(); // update->OwnershipRollbackPushLC()
    CleanupStack::PopAndDestroy( update );
    
	iState = static_cast<TState>( ids.iState );

	state = State();
    MCECLI_DEBUG_EVENTSTATE( "CMceEvent::UpdateL, after ITC, state", state );
    MCECLI_DEBUG("CMceEvent::SubscribeL, Exit");
	
    }

// -----------------------------------------------------------------------------
// CMceOutEvent::IsInEvent
// -----------------------------------------------------------------------------
//
TBool CMceOutEvent::IsInEvent() const
	{
	return EFalse;
	}
