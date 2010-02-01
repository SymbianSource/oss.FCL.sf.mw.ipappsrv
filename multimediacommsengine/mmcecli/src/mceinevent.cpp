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




#include "mceinevent.h"
#include "mceitcsender.h"
#include "mceserial.h"
#include "mceclilogs.h"

// ============================ MEMBER FUNCTIONS ===============================



// -----------------------------------------------------------------------------
// CMceInEvent::~CMceInEvent
// -----------------------------------------------------------------------------
//
EXPORT_C CMceInEvent::~CMceInEvent()
    {
    }

// -----------------------------------------------------------------------------
// CMceInEvent::AcceptEventL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceInEvent::AcceptL()
    {
    MCECLI_DEBUG("CMceInEvent::AcceptL, Entry");
    TUint state = State();
    MCECLI_DEBUG_EVENTSTATE( "state", state );
    
    User::LeaveIfError( State() == EIdle ? KErrNone : KErrNotReady );
	TMceIds ids;
	PrepareForITC( ids );
	ids.iState = State();
    iSender->SendL( ids, EMceItcAcceptEvent );
    //update the state
	iState = static_cast<TState>( ids.iState );
	
	state = State();
    MCECLI_DEBUG_EVENTSTATE( "CMceEvent::AcceptL, after ITC, state", state );
    MCECLI_DEBUG("CMceInEvent::AcceptL, Exit");
    }

// -----------------------------------------------------------------------------
// CMceInEvent::RejectEventL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceInEvent::RejectL()
    {
    MCECLI_DEBUG("CMceInEvent::RejectL, Entry");
    TUint state = State();
    MCECLI_DEBUG_EVENTSTATE( "state", state );
    
    User::LeaveIfError( State() == EIdle ? KErrNone : KErrNotReady );
	TMceIds ids;
	PrepareForITC( ids );
	ids.iState = State();
    iSender->SendL( ids, EMceItcRejectEvent );
    //update the state
	iState = static_cast<TState>( ids.iState );
		
	state = State();
    MCECLI_DEBUG_EVENTSTATE( "CMceEvent::RejectL, after ITC, state", state );
    MCECLI_DEBUG("CMceInEvent::RejectL, Exit");
    }


// -----------------------------------------------------------------------------
// CMceInEvent::RespondEventL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceInEvent::RespondL( 
                        const TDesC8& aReason,
			            TUint32 aCode,
			            CDesC8Array* aHeaders,
					    HBufC8* aContentType,
					    HBufC8* aContent ) 
    {
    MCECLI_DEBUG("CMceInEvent::RespondL, Entry");
    TUint state = State();
    MCECLI_DEBUG_EVENTSTATE( "state", state );
    MCECLI_DEBUG_SVALUE( "reason", aReason );
    MCECLI_DEBUG_DVALUE( "code", aCode );
    
    
    User::LeaveIfError( State() == EIdle ? KErrNone : KErrNotReady );
    User::LeaveIfError( 
        ( aContentType && aContent ) || !( aContentType || aContent ) ? 
        KErrNone : KErrArgument );
    
    //copy reason
    HBufC8* reason = aReason.AllocLC();
	//create reply
	CMceMsgSIPReply* reply = new (ELeave) CMceMsgSIPReply( *reason, 
	                                                        aCode, 
	                                                        *aHeaders, 
	                                                        *aContentType );
    CleanupStack::PushL( reply );
    
	TMceIds ids;
	PrepareForITC( ids );
	ids.iState = State();
	
	iSender->SendL( ids, EMceItcRespondEvent , *reply, aContent );    
	
	//update the state
	iState = static_cast<TState>( ids.iState );

    //cleanup    
    CleanupStack::PopAndDestroy( reply );
    CleanupStack::PopAndDestroy( reason );
    delete aHeaders;
    delete aContentType;
    
    state = State();
    MCECLI_DEBUG_EVENTSTATE( "CMceEvent::RespondL, after ITC, state", state );
    MCECLI_DEBUG("CMceInEvent::RespondL, Exit");
    
    }

// -----------------------------------------------------------------------------
// CMceInEvent::NotifyL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceInEvent::NotifyL(
					 CDesC8Array* aHeaders, 
					 HBufC8* aContentType,
					 HBufC8* aContent ) 
    {
    MCECLI_DEBUG("CMceInEvent::NotifyL, Entry");
    TUint state = State();
    MCECLI_DEBUG_EVENTSTATE( "state", state );
    
    User::LeaveIfError( State() == EActive ? KErrNone : KErrNotReady );
    User::LeaveIfError( 
        ( aContentType && aContent ) || !( aContentType || aContent ) ? 
        KErrNone : KErrArgument );
    
    //create notify message
	CMceMsgSIPEvent* notify = new (ELeave) CMceMsgSIPEvent( 
	                                                iId,
	                                                iDialogId,
	                                                EMceItcEventTypeEither,
	                                                CMceRefer::ENoSuppression, 
	                                                ( TUint32 ) KErrNotFound, 
	                                                aHeaders, 
	                                                aContentType );
    CleanupStack::PushL( notify );
    
    notify->OwnershipRollbackPushLC();
    
	TMceIds ids;
	PrepareForITC( ids );
	ids.iState = State();
	
	iSender->SendL( ids, EMceItcNotify, *notify, aContent );
	
	CleanupStack::Pop(); // notify->OwnershipRollbackPushLC()
	
    //update the state
	iState = static_cast<TState>( ids.iState );

    //cleanup    
    CleanupStack::PopAndDestroy( notify );
    
    state = State();
    MCECLI_DEBUG_EVENTSTATE( "CMceEvent::NotifyL, after ITC, state", state );
    MCECLI_DEBUG("CMceInEvent::NotifyL, Exit");
    }


// -----------------------------------------------------------------------------
// CMceInEvent::NewL
// -----------------------------------------------------------------------------
//
CMceInEvent* CMceInEvent::NewL( CMceManager& aManager,
								TUint32 aProfileId,
                                CMceMsgSIPEvent& aEventMsg )
    {    
    MCECLI_DEBUG("CMceInEvent::NewL, Entry");
    MCECLI_DEBUG_DVALUE("profile", aProfileId );
    
    CMceInEvent* self = new (ELeave) CMceInEvent( aManager, 
                                                  aProfileId, 
                                                  aEventMsg.Id(),
                                                  aEventMsg.DialogId(),
                                                  aEventMsg.RefreshInterval() );
    CleanupStack::PushL( self );
    
    HBufC8* eventHeader = aEventMsg.EventHeader();
    CleanupStack::PushL( eventHeader );
    HBufC8* recipient = aEventMsg.Recipient();
    CleanupStack::PushL( recipient );
    HBufC8* originator = aEventMsg.Originator();
    CleanupStack::PushL( originator );
    self->ConstructL( eventHeader, 
                      recipient, 
                      originator,
                      KMceDlgTypeSubscribe  );

    CleanupStack::Pop( originator );
    CleanupStack::Pop( recipient );
    CleanupStack::Pop( eventHeader );
    CleanupStack::Pop( self );
    MCECLI_DEBUG("CMceInEvent::NewL, Exit");
    return self;
    
    }


// -----------------------------------------------------------------------------
// CMceInEvent::CMceInEvent
// -----------------------------------------------------------------------------
//
CMceInEvent::CMceInEvent( CMceManager& aManager, 
                          TUint32 aProfileId,
                          TUint32 aId,
                          TUint32 aDialogId,
                          TUint32 aRefreshInterval )
    : CMceEvent( aManager, aProfileId, aDialogId, aRefreshInterval )
    {
    iId = aId;
    }

    
// -----------------------------------------------------------------------------
// CMceInEvent::ConstructL
// -----------------------------------------------------------------------------
//
void CMceInEvent::ConstructL( HBufC8* aEventHeader, 
                              HBufC8* aRecipient, 
                              HBufC8* aOriginator,
                              TUint32 aDialogType )
    {
    CMceEvent::ConstructL( aEventHeader, 
                           aRecipient, 
                           aOriginator, 
                           aDialogType,
                           NULL );    
    }
    

// -----------------------------------------------------------------------------
// CMceInEvent::IsInEvent
// -----------------------------------------------------------------------------
//
TBool CMceInEvent::IsInEvent() const
	{
	return ETrue;
	}
