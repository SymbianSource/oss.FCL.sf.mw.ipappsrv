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
#include "mceinrefer.h"
#include "mceitcsender.h"
#include "mceserial.h"
#include "mceclilogs.h"

// ============================ MEMBER FUNCTIONS ===============================



// -----------------------------------------------------------------------------
// CMceInRefer::~CMceInRefer
// -----------------------------------------------------------------------------
//
EXPORT_C CMceInRefer::~CMceInRefer()
    {
    }

// -----------------------------------------------------------------------------
// CMceInRefer::AcceptL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceInEvent* CMceInRefer::AcceptL()
    {
    MCECLI_DEBUG("CMceInRefer::AcceptL, Entry");
    TState state = State();
    MCECLI_DEBUG_EVENTSTATE( "state", state );
    
    User::LeaveIfError( State() == EIdle ? KErrNone : KErrNotReady );
    if ( ReferEvent() )
        {
        static_cast< CMceInEvent* >( ReferEvent() )->AcceptL();
        
        // resolve new state
        iState = ResolveStateL();
        }
    else
        {
        TMceIds ids;
	    PrepareForITC( ids );
	    ids.iState = State();
        iSender->SendL( ids, EMceItcAcceptEvent );
        //update the state
	    iState = static_cast< TState >( ids.iState );
        }
    state = State();
    MCECLI_DEBUG_EVENTSTATE( "CMceInRefer::AcceptL, after ITC, state", state );
    MCECLI_DEBUG("CMceInRefer::AcceptL, Exit");
    
	return static_cast< CMceInEvent* >( TransferEventOwnership() );
    }

// -----------------------------------------------------------------------------
// CMceInRefer::RejectL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceInRefer::RejectL()
    {
    MCECLI_DEBUG("CMceInRefer::RejectL, Entry");
    TState state = State();
    MCECLI_DEBUG_EVENTSTATE( "state", state );
    
    User::LeaveIfError( State() == EIdle ? KErrNone : KErrNotReady );
    if ( ReferEvent() )
        {
        static_cast< CMceInEvent* >( ReferEvent() )->RejectL();
        // resolve new state
        iState = ResolveStateL();
        }
    else
        {
        TMceIds ids;
	    PrepareForITC( ids );
	    ids.iState = State();
        iSender->SendL( ids, EMceItcRejectEvent );
        //update the state
	    iState = static_cast<TState>( ids.iState );	
        }
    state = State();
    MCECLI_DEBUG_EVENTSTATE( "CMceInRefer::RejectL, after ITC, state", state );
    MCECLI_DEBUG("CMceInRefer::RejectL, Exit");
    
	}


// -----------------------------------------------------------------------------
// CMceInRefer::RespondL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceInEvent* CMceInRefer::RespondL( 
                        const TDesC8& aReason,
			            TUint32 aCode,
			            CDesC8Array* aHeaders,
					    HBufC8* aContentType,
					    HBufC8* aContent ) 
    {
    MCECLI_DEBUG("CMceInRefer::RespondL, Entry");
    TState state = State();
    MCECLI_DEBUG_EVENTSTATE( "state", state );
    MCECLI_DEBUG_SVALUE( "reason", aReason );
    MCECLI_DEBUG_DVALUE( "code", aCode );
    
    
    User::LeaveIfError( State() == EIdle ? KErrNone : KErrNotReady );
    User::LeaveIfError( 
        ( aContentType && aContent ) || !( aContentType || aContent ) ? 
        KErrNone : KErrArgument );
    
    if ( ReferEvent() )
        {
        static_cast< CMceInEvent* >( ReferEvent() )-> RespondL( aReason, 
                                                                    aCode, 
                                                                    aHeaders,
                                                                    aContentType,
                                                                    aContent );
        // resolve new state
        iState = ResolveStateL();	    
        }
    else
        {
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
        }
    state = State();
    MCECLI_DEBUG_EVENTSTATE( "CMceInRefer::RespondL, after ITC, state", state );
    MCECLI_DEBUG("CMceInRefer::RespondL, Exit");
    
    return ( iState == EPending ? 
        static_cast< CMceInEvent* >( TransferEventOwnership() ) : NULL );
    }

// -----------------------------------------------------------------------------
// CMceInRefer::NewL
// -----------------------------------------------------------------------------
//
CMceInRefer* CMceInRefer::NewL( CMceManager& aManager,
                                TUint32 aProfileId,
                                CMceMsgSIPEvent& aEventMsg,
                                CMceInEvent* aInEvent )
    {
    MCECLI_DEBUG("CMceInRefer::NewL, Entry");
    MCECLI_DEBUG_DVALUE("profile", aProfileId );
    
    CMceInRefer* self = new (ELeave) CMceInRefer( 
        aManager, 
        aProfileId, 
        aEventMsg.Id(), 
        aEventMsg.DialogId(),
        static_cast< TType > ( aEventMsg.ReferType() ) );
        
    CleanupStack::PushL( self );
    
    HBufC8* referTo = aEventMsg.ReferTo();
    CleanupStack::PushL( referTo );
    HBufC8* recipient = aEventMsg.Recipient();
    CleanupStack::PushL( recipient );
    HBufC8* originator = aEventMsg.Originator();
    CleanupStack::PushL( originator );
    
    self->ConstructL( referTo, 
                      recipient, 
                      originator, 
                      aInEvent );

    CleanupStack::Pop( originator );
    CleanupStack::Pop( recipient );
    CleanupStack::Pop( referTo );
    
    CleanupStack::Pop( self );
    MCECLI_DEBUG("CMceInRefer::NewL, Exit");
    return self;
    }


// -----------------------------------------------------------------------------
// CMceInRefer::CMceInRefer
// -----------------------------------------------------------------------------
//
CMceInRefer::CMceInRefer( CMceManager& aManager, 
                          TUint32 aProfileId,
                          TUint32 aId,
                          TUint32 aDialogId,
                          TType aType )
    : CMceRefer( aManager, aProfileId, aDialogId, aType )
    {
    iId = aId;
    }
    
// -----------------------------------------------------------------------------
// CMceInRefer::ConstructL
// -----------------------------------------------------------------------------
//
void CMceInRefer::ConstructL( HBufC8* aReferTo,
                              HBufC8* aRecipient,
                              HBufC8* aOrginator,
                              CMceInEvent* aInEvent ) 
    {
    iEvent = aInEvent; 
    CMceRefer::ConstructL( aRecipient, aReferTo, aOrginator, NULL );
    iEvent = NULL;
    iOwnedEvent = aInEvent; 
    }

// -----------------------------------------------------------------------------
// CMceInRefer::IsInRefer
// -----------------------------------------------------------------------------
//    
TBool CMceInRefer::IsInRefer() const
    {
    return ETrue;
    }

