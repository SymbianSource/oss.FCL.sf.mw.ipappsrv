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




#include "mcerefer.h"
#include "mcemanager.h"
#include "mcereferobserver.h"
#include "mceoutevent.h"
#include "mcetransactiondatacontainer.h"
#include "mceitcsender.h"
#include "mcereferreceiver.h"
#include "mceevents.h"
#include "mceserial.h"
#include "mce.h"
#include "mceclilogs.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceRefer::ConstructL
// -----------------------------------------------------------------------------
//
void CMceRefer::ConstructL( HBufC8* aRecipient,
            	            HBufC8* aReferTo,
            	            HBufC8* aOriginator,
            	            CDesC8Array* aParams )
    {
    MCECLI_DEBUG("CMceRefer::ConstructL, Entry");
        
    User::LeaveIfError( aRecipient && aReferTo ? KErrNone : KErrArgument );
    if ( ReferEvent() )
        {
        iDialogId = ReferEvent()->DialogId();            
        }

    if ( aParams )
        {
        iId = iManager.RegisterReferL( this, *aParams, iDialogId );
        }
    else
        {
        CDesC8Array* params = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );    
        CleanupStack::PushL( params );
        iId = iManager.RegisterReferL( this, *params, iDialogId );
        CleanupStack::PopAndDestroy( params );
        }

    //get server    
    RMce& server = iManager.ServerSession();
    
    //create sender
    iSender = CMceItcSender::NewL( server  );
    //create receiver
    if ( !ReferEvent() )
        {
        iReceiver = new (ELeave) CMceReferReceiver( server, *this );    
        }
    
    // Handle ownership change of input params when leave cannot occur anymore  
    
    if ( aOriginator )
        {
        iOriginator = aOriginator;
        }
    else
        {
        iOriginator = KNullDesC8().AllocL();
        } 
    delete aParams;
    iReferTo = aReferTo;
    iRecipient = aRecipient;
    
    MCECLI_DEBUG("CMceRefer::ConstructL, Exit");
    
    }

// -----------------------------------------------------------------------------
// CMceRefer::~CMceRefer
// -----------------------------------------------------------------------------
//
CMceRefer::~CMceRefer()
    {
    MCECLI_DEBUG("CMceRefer::~CMceRefer, Entry");
    
    if ( iOwnedEvent )
        {
        delete iOwnedEvent;
        iOwnedEvent = NULL;
        }
        
    if( iReceiver )
        {
        delete iReceiver;    
        }
    
    iManager.UnregisterRefer( this );
    
    delete iSender;    
    delete iReferTo;
    delete iRecipient;
    delete iOriginator;
    MCECLI_DEBUG("CMceRefer::~CMceRefer, Exit");
    }

// -----------------------------------------------------------------------------
// CMceRefer::State
// -----------------------------------------------------------------------------
//
EXPORT_C CMceRefer::TState CMceRefer::State() const
    {
    return iState;
    }

// -----------------------------------------------------------------------------
// CMceRefer::Type
// -----------------------------------------------------------------------------
//
EXPORT_C CMceRefer::TType CMceRefer::Type() const
    {
    return iType;
    }

// -----------------------------------------------------------------------------
// CMceRefer::Recipient
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CMceRefer::Recipient() const
    {
    return *iRecipient;
    }

// -----------------------------------------------------------------------------
// CMceRefer::Originator
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CMceRefer::Originator() const
    {
    return *iOriginator;
    }

// -----------------------------------------------------------------------------
// CMceRefer::ReferTo
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CMceRefer::ReferTo() const
    {
    return *iReferTo;
    }

// -----------------------------------------------------------------------------
// CMceRefer::AssociatedSession
// -----------------------------------------------------------------------------
//
EXPORT_C CMceSession* CMceRefer::AssociatedSession() const
    {
    return iManager.FindAssociatedSession( iDialogId );
    }
    
// -----------------------------------------------------------------------------
// CMceRefer::AssociatedEvents
// -----------------------------------------------------------------------------
//
EXPORT_C RPointerArray< CMceEvent >* CMceRefer::AssociatedEventsL() const
    {
    return NULL;
    }
    
// -----------------------------------------------------------------------------
// CMceRefer::AssociatedRefers
// -----------------------------------------------------------------------------
//
EXPORT_C RPointerArray< CMceRefer >* CMceRefer::AssociatedRefersL() const
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// CMceRefer::CMceRefer
// -----------------------------------------------------------------------------
//
CMceRefer::CMceRefer( CMceManager& aManager, 
                      TUint32 aProfileId,
                      TUint32 aDialogId,
                      TType aType )
    : iManager( aManager ), 
      iProfileId( aProfileId ), 
      iId ( KMceNotAssigned ),
      iDialogId( aDialogId ),
      iType ( aType )
    {
    }

// -----------------------------------------------------------------------------
// CMceRefer::Id
// -----------------------------------------------------------------------------
//
TUint32 CMceRefer::Id() const
    {
    return iId;
    }

// -----------------------------------------------------------------------------
// CMceRefer::DialogId
// -----------------------------------------------------------------------------
//
TUint32 CMceRefer::DialogId() const
    {
    return iDialogId;
    }


// -----------------------------------------------------------------------------
// CMceRefer::ITCSender
// -----------------------------------------------------------------------------
//
CMceItcSender& CMceRefer::ITCSender() const
	{
	if ( ReferEvent() )
	    {
	    return ReferEvent()->ITCSender();
	    }
	else
	    {
	    return *iSender;
	    }
	}

// -----------------------------------------------------------------------------
// CMceRefer::Manager
// -----------------------------------------------------------------------------
//
CMceManager& CMceRefer::Manager() const
    {
    return iManager;
    }

// -----------------------------------------------------------------------------
// CMceRefer::ReferEvent
// -----------------------------------------------------------------------------
//
CMceEvent* CMceRefer::ReferEvent() const
    {
    if ( iOwnedEvent )
        {
        return iOwnedEvent;
        }
    else
        {
        return iEvent;
        }
    }


// -----------------------------------------------------------------------------
// CMceRefer::DetachReferEvent
// -----------------------------------------------------------------------------
//
void CMceRefer::DetachReferEvent()
    {
    iEvent = NULL;
    iOwnedEvent = NULL;
    }

// -----------------------------------------------------------------------------
// CMceRefer::ResolveStateL
// -----------------------------------------------------------------------------
//
CMceRefer::TState CMceRefer::ResolveStateL()
    {
    User::LeaveIfError( ReferEvent() ? KErrNone : KErrArgument );
    CMceEvent::TState eventState = ReferEvent()->State();
    CMceRefer::TState newState = iState;
    switch( eventState )
        {
        case CMceEvent::EIdle:
            {
            newState = CMceRefer::EIdle;
            break;
            }
        case CMceEvent::EPending:
            {
            newState = CMceRefer::EPending;
            break;
            }
        case CMceEvent::EActive:
            {
            newState = CMceRefer::EAccepted;
            break;
            }
        case CMceEvent::ETerminated:
            {
            newState = CMceRefer::ETerminated;
            break;
            }
        default:
            {
            User::Leave( KErrArgument );
            break;
            }
        }
    return newState;
    }

// -----------------------------------------------------------------------------
// CMceRefer::ProfileId
// -----------------------------------------------------------------------------
//
TUint32 CMceRefer::ProfileId() const
    {
    return iProfileId;
    }
    
// -----------------------------------------------------------------------------
// CMceRefer::PrepareForITC
// -----------------------------------------------------------------------------
//
void CMceRefer::PrepareForITC( TMceIds& aId )
    {
	aId.iAppUID = Manager().AppUid().iUid;
    aId.iManagerType = KMceCSSIPEvent;
	aId.iSessionID = Id();
	aId.iProfileID = iProfileId;    
    }

// -----------------------------------------------------------------------------
// CMceRefer::EventReceivedL
// -----------------------------------------------------------------------------
//
void CMceRefer::EventReceivedL( TMceIds& aId  )
    {
    TMceEvent event( aId, NULL, NULL ); 
    EventReceivedL( event );
    }


// -----------------------------------------------------------------------------
// CMceRefer::EventReceivedL
// -----------------------------------------------------------------------------
//
void CMceRefer::EventReceivedL( TMceIds& aId, CMceMsgBase& aMessage )
    {
    TMceEvent event( aId, &aMessage, NULL ); 
    EventReceivedL( event );
    }

// -----------------------------------------------------------------------------
// CMceRefer::EventReceivedL
// -----------------------------------------------------------------------------
//
void CMceRefer::EventReceivedL( TMceIds& aId, 
                                CMceMsgBase& aMessage, 
                                HBufC8* aContent )
    {
    TMceEvent event( aId, &aMessage, aContent ); 
    EventReceivedL( event );
    }

// -----------------------------------------------------------------------------
// CMceRefer::ErrorOccuredL
// -----------------------------------------------------------------------------
//
void CMceRefer::ErrorOccuredL( TInt aError )
    {
    MCECLI_DEBUG("CMceRefer::ErrorOccuredL, Entry");
    MCECLI_DEBUG_DVALUE("error", aError );
    
    User::LeaveIfError( iManager.ReferObserver() ? KErrNone : KErrArgument );
    iManager.ReferObserver()->Failed( *this, aError );
	TState state = State();
    MCECLI_DEBUG_EVENTSTATE( "CMceRefer::ErrorOccuredL, before exit. state", state );
    MCECLI_DEBUG("CMceEvent::ErrorOccuredL, Exit");
    
    }

// -----------------------------------------------------------------------------
// CMceRefer::EventReceivedL
// -----------------------------------------------------------------------------
//
void CMceRefer::EventReceivedL( TMceEvent& aEvent )
    {
    MCECLI_DEBUG("CMceRefer::EventReceivedL, Entry");
    TState state = State();
    MCECLI_DEBUG_EVENTSTATE("this: state", state );
    MCECLI_DEBUG_DVALUE("this: id", Id() );
    
    User::LeaveIfError( 
        aEvent.Id().iSessionID == Id() ? KErrNone : KErrGeneral );
    User::LeaveIfError( aEvent.Id().IsSessionId() ? KErrNone : KErrGeneral );
        
    HandleEventL( aEvent );

	state = State();
    MCECLI_DEBUG_EVENTSTATE("CMceRefer::EventReceivedL, before exit. state", state );
    MCECLI_DEBUG("CMceRefer::EventReceivedL, Exit");
    
    }

// -----------------------------------------------------------------------------
// CMceRefer::HandleEventL
// -----------------------------------------------------------------------------
//
void CMceRefer::HandleEventL( TMceEvent& aEvent )
    {
    MCECLI_DEBUG("CMceRefer::HandleEventL, Entry");
    
    // update state
    iState = static_cast<TState>( aEvent.Id().iState );
    // notify observer
    if ( aEvent.MessageType() == EMceItcMsgTypeSIPReply ) 
        {
        if ( aEvent.Action() == EMceItcStateChanged )
            {
            MCECLI_DEBUG("CMceRefer::HandleEventL, session state changed" );
            MCECLI_DEBUG("CMceRefer::HandleEventL, caused by sip reply");
            HandleStateChangedL( aEvent );
            }
        }
    MCECLI_DEBUG("CMceRefer::HandleEventL, Exit");
    }

// -----------------------------------------------------------------------------
// CMceRefer::EventStateChangedL
// -----------------------------------------------------------------------------
//
void CMceRefer::EventStateChangedL( TMceEvent& aEvent )
    {
    MCECLI_DEBUG("CMceRefer::EventStateChangedL, Entry");
    TState state = State();
    MCECLI_DEBUG_EVENTSTATE("this: state", state );
    MCECLI_DEBUG_DVALUE("this: id", Id() );
    
    User::LeaveIfError( ReferEvent() ? KErrNone : KErrArgument );
    User::LeaveIfError( aEvent.Id().iSessionID == ReferEvent()->Id() ?
        KErrNone : KErrArgument );
    // resolve new state
    iState = ResolveStateL();
    HandleStateChangedL( aEvent );
    state = State();
    MCECLI_DEBUG_EVENTSTATE("CMceRefer::EventStateChangedL, before exit. state", state );
    MCECLI_DEBUG("CMceRefer::EventStateChangedL, Exit");
    
    }
// -----------------------------------------------------------------------------
// CMceRefer::HandleStateChangedL
// -----------------------------------------------------------------------------
//
void CMceRefer::HandleStateChangedL( TMceEvent& aEvent )
    {
    User::LeaveIfError( aEvent.Action() == EMceItcStateChanged ?
        KErrNone : KErrArgument );
    User::LeaveIfError( aEvent.MessageType() == EMceItcMsgTypeSIPReply ?
        KErrNone : KErrArgument );
    
    TMceTransactionDataContainer* container = iManager.TransactionContainer();
    
    CMceMsgSIPReply* replyMsg = 
            static_cast< CMceMsgSIPReply* >( aEvent.Message() );
    if ( iManager.ReferObserver() )
    	{   	
        if ( replyMsg && container )
            {
            container->SetStatusCode(replyMsg->iCode);
            replyMsg->iCode = 0;
            container->SetReasonPhrase( replyMsg->Reason() );
            container->SetHeaders( replyMsg->Headers() );
            container->SetContentType( replyMsg->ContentType() );
            container->SetContent( aEvent.Content() );
            } 
        iManager.ReferObserver()->ReferStateChanged( *this, container );
    	}
    	
    if ( container )
    	{
    	container->Clear();
    	}    
    	
    if ( replyMsg )
        {
        replyMsg->Close();
        }
    delete aEvent.Content();	
    }

// -----------------------------------------------------------------------------
// CMceRefer::TransferEventOwnership
// -----------------------------------------------------------------------------
//
CMceEvent* CMceRefer::TransferEventOwnership()
    {
    if ( iOwnedEvent && !iEvent )
        {
        // Transfer the event from owned member to not owned
        iEvent = iOwnedEvent;
        iOwnedEvent = NULL;    
        }
    return iEvent;
    }
