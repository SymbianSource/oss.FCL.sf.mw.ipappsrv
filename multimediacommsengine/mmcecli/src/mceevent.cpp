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




#include "mceevent.h"
#include "mcemanager.h"
#include "mcerefer.h"
#include "mcetransactiondatacontainer.h"
#include "mceeventobserver.h"
#include "mceitcsender.h"
#include "mceeventreceiver.h"
#include "mceevents.h"
#include "mceserial.h"
#include "mce.h"
#include "mceclilogs.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceEvent::ConstructL
// -----------------------------------------------------------------------------
//
void CMceEvent::ConstructL( HBufC8* aEventHeader, 
                            HBufC8* aRecipient,
	                        HBufC8* aOriginator,
	                        TUint32 aDialogType,
	                        CDesC8Array* aParams )
    {
    MCECLI_DEBUG("CMceEvent::ConstructL, Entry");
    
    User::LeaveIfError( aEventHeader && aRecipient ? KErrNone : KErrArgument );
    
    HBufC8* originator = 0;
    if ( aOriginator )
        {
        originator = aOriginator;
        }
    else
        {
        originator = KNullDesC8().AllocLC();
        }
        
    CDesC8Array* params = 0;    
    if ( aParams )
        {
        params = aParams;
        }
    else
        {
        params = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );  
        CleanupStack::PushL( params );  
        }
	
	if ( params->Count() == 0 )
	    {
	    params->AppendL( *aEventHeader );
        if ( iDialogId == KMceNotAssigned )
            {
            params->AppendL( *aRecipient );
            params->AppendL( *originator );
            }
        }
	
    iId = iManager.RegisterEventL( this, *params, iDialogId, aDialogType );

    if ( !aParams )
        {
        CleanupStack::PopAndDestroy( params );
        }
        
    //get server    
    RMce& server = iManager.ServerSession();
    
    //create sender
    iSender = CMceItcSender::NewL( server  );
    //create receiver
    iReceiver = new (ELeave) CMceEventReceiver( server, *this );
    
    // Handle ownership change of input params when leave cannot occur anymore 
    
    if ( !aOriginator )
        {
        CleanupStack::Pop( originator );
        }     
    iOriginator = originator; 
    
    delete aParams;
    iEventHeader = aEventHeader;
    iRecipient = aRecipient;
    MCECLI_DEBUG("CMceEvent::ConstructL, Exit");
    }


// -----------------------------------------------------------------------------
// CMceEvent::~CMceEvent
// -----------------------------------------------------------------------------
//
CMceEvent::~CMceEvent()
    {
    MCECLI_DEBUG("CMceEvent::~CMceEvent, Entry");
    delete iReceiver;
    iReceiver = NULL;
    
	iManager.UnregisterEvent( this );    
    
    delete iSender;    
    delete iEventHeader;
    delete iRecipient;
    delete iOriginator;
    MCECLI_DEBUG("CMceEvent::~CMceEvent, Exit");
    }

// -----------------------------------------------------------------------------
// CMceEvent::State
// -----------------------------------------------------------------------------
//
EXPORT_C CMceEvent::TState CMceEvent::State() const
    {
    return iState;
    }

// -----------------------------------------------------------------------------
// CMceEvent::Recipient
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CMceEvent::Recipient() const
    {
    return *iRecipient;
    }

// -----------------------------------------------------------------------------
// CMceEvent::Originator
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CMceEvent::Originator() const
    {
    return *iOriginator;
    }

// -----------------------------------------------------------------------------
// CMceEvent::EventHeader
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CMceEvent::EventHeader() const
    {
    return *iEventHeader;
    }

// -----------------------------------------------------------------------------
// CMceEvent::TerminateL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceEvent::TerminateL( CDesC8Array* aHeaders,
            						 HBufC8* aContentType,
            						 HBufC8* aContent ) 
    {
    MCECLI_DEBUG("CMceEvent::TerminateL, Entry");
    TUint state = State();
    MCECLI_DEBUG_EVENTSTATE( "state", state );
    User::LeaveIfError( State() == EActive ? KErrNone : KErrNotReady );
    User::LeaveIfError( 
        ( aContentType && aContent ) || !( aContentType || aContent ) ? 
        KErrNone : KErrArgument );
    
    CMceRefer* refer = iManager.FindRefer( *this );
    if ( refer && !refer->IsInRefer() )
        {
        // Terminating not supported for event attached to out refer
        User::Leave( KErrNotSupported ); 
        }
    
	
	//This way both the terminating NOTIFY for CMceInEvent and 
	//the un-SUBSCRIBE for CMceOutEvent can contain 
	// At this moment, only MT terminate in Refer cases is able 
	// to terminate not in MO
	TMceIds ids;
	PrepareForITC( ids );
	ids.iState = State();
	

		
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
	

	iSender->SendL( ids, EMceItcTerminateEvent, *notify, aContent );

	CleanupStack::Pop(); // notify->OwnershipRollbackPushLC()
    
    //cleanup    
    CleanupStack::PopAndDestroy( notify );
    //update the state
	iState = static_cast<TState>( ids.iState );
	
	state = State();
    MCECLI_DEBUG_EVENTSTATE( "CMceEvent::TerminateL, after ITC, state", state );
    MCECLI_DEBUG("CMceEvent::TerminateL, Exit");
   
    }

// -----------------------------------------------------------------------------
// CMceEvent::AssociatedSession
// -----------------------------------------------------------------------------
//
EXPORT_C CMceSession* CMceEvent::AssociatedSession() const
    {
    return iManager.FindAssociatedSession( iDialogId );
    }
    
// -----------------------------------------------------------------------------
// CMceEvent::AssociatedEvents
// -----------------------------------------------------------------------------
//
EXPORT_C RPointerArray< CMceEvent >* CMceEvent::AssociatedEventsL() const
    {
    return NULL;
    }
    
// -----------------------------------------------------------------------------
// CMceEvent::AssociatedRefers
// -----------------------------------------------------------------------------
//
EXPORT_C RPointerArray< CMceRefer >* CMceEvent::AssociatedRefersL() const
    {
    return NULL;
    }
    
// -----------------------------------------------------------------------------
// CMceEvent::CMceEvent
// -----------------------------------------------------------------------------
//
CMceEvent::CMceEvent( CMceManager& aManager,                      
                      TUint32 aProfileId,
                      TUint32 aDialogId,
                      TUint32 aRefreshInterval )
    : iManager( aManager ), 
	  iProfileId( aProfileId ),
      iId ( KMceNotAssigned ),
      iDialogId( aDialogId ),
      iRefreshInterval( aRefreshInterval )
    {
    }

// -----------------------------------------------------------------------------
// CMceEvent::Id
// -----------------------------------------------------------------------------
//
TUint32 CMceEvent::Id() const
    {
    return iId;
    }

// -----------------------------------------------------------------------------
// CMceEvent::DialogId
// -----------------------------------------------------------------------------
//
TUint32 CMceEvent::DialogId() const
    {
    return iDialogId;
    }


// -----------------------------------------------------------------------------
// CMceEvent::ITCSender
// -----------------------------------------------------------------------------
//
CMceItcSender& CMceEvent::ITCSender()
	{
	return *iSender;
	}

// -----------------------------------------------------------------------------
// CMceEvent::Manager
// -----------------------------------------------------------------------------
//
CMceManager& CMceEvent::Manager()
    {
    return iManager;
    }

// -----------------------------------------------------------------------------
// CMceEvent::ProfileId
// -----------------------------------------------------------------------------
//
TUint32 CMceEvent::ProfileId() const
	{
	return iProfileId;
	}

// -----------------------------------------------------------------------------
// CMceEvent::PrepareForITC
// -----------------------------------------------------------------------------
//
void CMceEvent::PrepareForITC( TMceIds& aId )
    {
	aId.iAppUID = Manager().AppUid().iUid;
    aId.iManagerType = KMceCSSIPEvent;
	aId.iSessionID = Id();
	aId.iProfileID = iProfileId;
    }

// -----------------------------------------------------------------------------
// CMceEvent::EventReceivedL
// -----------------------------------------------------------------------------
//
void CMceEvent::EventReceivedL( TMceIds& aId  )
    {
    TMceEvent event( aId, NULL, NULL ); 
    EventReceivedL( event );
    }


// -----------------------------------------------------------------------------
// CMceEvent::EventReceivedL
// -----------------------------------------------------------------------------
//
void CMceEvent::EventReceivedL( TMceIds& aId, CMceMsgBase& aMessage )
    {
    TMceEvent event( aId, &aMessage, NULL ); 
    EventReceivedL( event );
    }

// -----------------------------------------------------------------------------
// CMceEvent::EventReceivedL
// -----------------------------------------------------------------------------
//
void CMceEvent::EventReceivedL( TMceIds& aId, 
                                CMceMsgBase& aMessage, 
                                HBufC8* aContent )
    {
    TMceEvent event( aId, &aMessage, aContent ); 
    EventReceivedL( event );
    }

// -----------------------------------------------------------------------------
// CMceEvent::ErrorOccuredL
// -----------------------------------------------------------------------------
//
void CMceEvent::ErrorOccuredL( TInt aError )
    {
    MCECLI_DEBUG("CMceEvent::ErrorOccuredL, Entry");
    MCECLI_DEBUG_DVALUE("error", aError );
    
    iState = ETerminated;    
    User::LeaveIfError( iManager.EventObserver() ? KErrNone : KErrArgument );
    
    CMceRefer* refer = iManager.FindRefer( *this );

    if ( refer )
        {
        refer->ErrorOccuredL( aError );
        }
    
    iManager.EventObserver()->Failed( *this, aError );
	
	TUint state = State();
    MCECLI_DEBUG_EVENTSTATE( "CMceEvent::ErrorOccuredL, before exit. state", state );
    MCECLI_DEBUG("CMceEvent::ErrorOccuredL, Exit");
    }

// -----------------------------------------------------------------------------
// CMceEvent::EventReceivedL
// -----------------------------------------------------------------------------
//
void CMceEvent::EventReceivedL( TMceEvent& aEvent )
    {
    MCECLI_DEBUG("CMceEvent::EventReceivedL, Entry");
    TUint state = State();
    MCECLI_DEBUG_EVENTSTATE("this: state", state );
    MCECLI_DEBUG_DVALUE("this: id", Id() );
    
    User::LeaveIfError( aEvent.Id().iSessionID == Id() ? 
        KErrNone : KErrGeneral );
    User::LeaveIfError( aEvent.Id().IsSessionId() ? KErrNone : KErrGeneral );
        
    HandleEventL( aEvent );

	state = State();
    MCECLI_DEBUG_EVENTSTATE("CMceEvent::EventReceivedL, before exit. state", state );
    MCECLI_DEBUG("CMceEvent::EventReceivedL, Exit");
    
    }

// -----------------------------------------------------------------------------
// CMceEvent::HandleEventL
// -----------------------------------------------------------------------------
//
void CMceEvent::HandleEventL( TMceEvent& aEvent )
    {
    MCECLI_DEBUG("CMceEvent::EventReceivedL, Entry");
    
    // update state
    iState = static_cast<TState>( aEvent.Id().iState );
    MMceEventObserver* observer = iManager.EventObserver();
    TMceTransactionDataContainer* container = iManager.TransactionContainer();

    CMceMsgBase* msg = static_cast< CMceMsgBase* >( aEvent.Message() );
    // notify observer
    if ( aEvent.MessageType() == EMceItcMsgTypeSIPReply  
        && aEvent.Action() == EMceItcStateChanged )
        {
        MCECLI_DEBUG("CMceEvent::HandleEventL, session state changed" );
        MCECLI_DEBUG("CMceEvent::HandleEventL, caused by sip reply");
        
        CMceMsgSIPReply* replyMsg = static_cast< CMceMsgSIPReply* >( msg );
        
        CMceRefer* refer = iManager.FindRefer( *this );
    
        if ( replyMsg && container && 
            !( refer && !refer->IsInRefer() && 
            refer->State() == CMceRefer::EPending ) )
            {
            container->SetStatusCode( replyMsg->iCode );
            replyMsg->iCode = 0;
            container->SetReasonPhrase( replyMsg->Reason() );
            container->SetHeaders( replyMsg->Headers() );
            container->SetContentType( replyMsg->ContentType() );
            container->SetContent( aEvent.Content() );
            }
           
        if ( observer )
            {
            iManager.EventObserver()->EventStateChanged( *this, container );    
            }
        
        if ( refer && 
            !( iState == EActive && refer->State() == CMceRefer::EAccepted ) )
            {
            refer->EventStateChangedL( aEvent );
            }
        }
    else if ( aEvent.MessageType() == EMceItcMsgTypeEvent 
        && aEvent.Action() == EMceItcNotifyReceived ) 
        {
        MCECLI_DEBUG("CMceEvent::HandleEventL, notify received" );
        if ( observer )
            {
            CMceMsgSIPEvent* eventMsg =  static_cast<CMceMsgSIPEvent*>( msg );
            if ( container)
                {
                container->SetHeaders( eventMsg->SIPHeaders() );
                container->SetContentType( eventMsg->ContentType() );
                container->SetContent( aEvent.Content() );
                }
            iManager.EventObserver()->NotifyReceived( *this, container );    
            }
        }
    else
        {
        MCECLI_DEBUG("CMceEvent::HandleEventL, not supported" );
        User::Leave( KErrNotSupported );
        }
        
    if ( container )
    	{
    	container->Clear();
    	}
    	
    if ( msg )
        {
        msg->Close();
        }
    delete aEvent.Content();

    MCECLI_DEBUG("CMceEvent::HandleEventL, Exit" );
    
    }
