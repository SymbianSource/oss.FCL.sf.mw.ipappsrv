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




#include "mcemanager.h"
#include "mceevent.h"
#include "mceeventreceiver.h"
#include "mce.h"
#include "mceserial.h"
#include "mceevents.h"
#include "mceclilogs.h"

// -----------------------------------------------------------------------------
// CMceEventReceiver::CMceEventReceiver
// -----------------------------------------------------------------------------
//
CMceEventReceiver::CMceEventReceiver( RMce& aMce,
                                          CMceEvent& aEvent )
 : CMceItcReceiverBase( aMce ),
   iEvent( aEvent )
	{
	iIdsPckg().iAppUID = aEvent.Manager().AppUid().iUid;
	iIdsPckg().iManagerType = KMceCSSIPEvent;
	iIdsPckg().iSessionID = aEvent.Id();
	
    Init();
    
	}

// -----------------------------------------------------------------------------
// CMceEventReceiver::~CMceEventReceiver
// -----------------------------------------------------------------------------
//
CMceEventReceiver::~CMceEventReceiver () 
	{
	}

// -----------------------------------------------------------------------------
// CMceEventReceiver::IncomingEventL
// -----------------------------------------------------------------------------
//
void CMceEventReceiver::IncomingEventL( TMceItcCallBack aEvent, TMceIds& aIds, 
                                          const TDesC8& aContext )
	{
    MCECLI_DEBUG("CMceEventReceiver::IncomingEventL, Entry");
    MCECLI_DEBUG_DVALUE("context size", aContext.Length() );
    
	switch ( aEvent )
	    {
	    case EMceItcStateChanged:
	    case EMceItcNotifyReceived:
	        {
            MCECLI_DEBUG("CMceEventReceiver::IncomingEventL, incoming event. \
passing to event");
	        
	        CMceMsgBase* message = DecodeMessageLC( aIds, aContext );
	        User::LeaveIfError( message ? KErrNone : KErrGeneral );
            message->PushL();

            iEvent.EventReceivedL( aIds, *message );
            
            message->Pop();
        	CleanupStack::PopAndDestroy( message );
	        
	        break;
	        }
        default:
            {
            MCECLI_DEBUG("CMceEventReceiver::IncomingEventL, not supported");
            User::Leave( KErrNotSupported );
            }
	    }
	
    MCECLI_DEBUG("CMceEventReceiver::IncomingEventL, Exit");
	}

	
// -----------------------------------------------------------------------------
// CMceEventReceiver::IncomingEventL
// -----------------------------------------------------------------------------
//
void CMceEventReceiver::IncomingEventL( TMceItcCallBack aEvent, TMceIds& aIds, 
                                          const TDesC8& aContext, 
                                          HBufC8* aContent )
	{
    MCECLI_DEBUG("CMceEventReceiver::IncomingEventL, Entry");
    MCECLI_DEBUG_DVALUE("context size", aContext.Length() );
    MCECLI_DEBUG_DVALUE("content size", aContent ? aContent->Length() : 0 );
	
	switch ( aEvent )
	    {
	    case EMceItcStateChanged:
	    case EMceItcNotifyReceived:
	        {
            MCECLI_DEBUG("CMceEventReceiver::IncomingEventL, incoming event. \
passing to event");
	        CMceMsgBase* message = DecodeMessageLC( aIds, aContext );
	        User::LeaveIfError( message ? KErrNone : KErrGeneral );
            message->PushL();

            iEvent.EventReceivedL( aIds, *message, aContent );
            
            message->Pop();
        	CleanupStack::PopAndDestroy( message );
	        break;
	        }
	   default:
            {
            MCECLI_DEBUG("CMceEventReceiver::IncomingEventL, not supported");
            User::Leave( KErrNotSupported );
            }
	    }
	    
    MCECLI_DEBUG("CMceEventReceiver::IncomingEventL, Exit");
	}

	
// -----------------------------------------------------------------------------
// CMceEventReceiver::IncomingEventL
// -----------------------------------------------------------------------------
//
void CMceEventReceiver::IncomingEventL( TMceItcCallBack /*aEvent*/, TMceIds& aIds )
	{
    MCECLI_DEBUG("CMceEventReceiver::IncomingEventL, Entry");
		
    iEvent.EventReceivedL( aIds );
    
    MCECLI_DEBUG("CMceEventReceiver::IncomingEventL, Exit");
    
	}

// -----------------------------------------------------------------------------
// CMceEventReceiver::ErrorOccuredL
// -----------------------------------------------------------------------------
//
void CMceEventReceiver::ErrorOccuredL (TMceIds& /*aIds*/, TInt aError )
	{
    MCECLI_DEBUG("CMceEventReceiver::ErrorOccuredL, Entry");
    iEvent.ErrorOccuredL( aError );
    MCECLI_DEBUG("CMceEventReceiver::ErrorOccuredL, Exit");
	}

// -----------------------------------------------------------------------------
// CMceEventReceiver::DecodeMessageL
// -----------------------------------------------------------------------------
//
CMceMsgBase* CMceEventReceiver::DecodeMessageLC( TMceIds& aIds, 
                                                   const TDesC8& aContext )
    {
    CMceMsgBase* message = NULL;
    switch ( aIds.iMsgType )
        {
        case EMceItcMsgTypeSIPReply:
            {
            message = new (ELeave) CMceMsgSIPReply();
        	break;
            }
        case EMceItcMsgTypeEvent:
            {
            message = new (ELeave) CMceMsgSIPEvent();
            break;
            }
	    default:
	        {
	        }
        }
        
    if ( message )
        {
    	CleanupStack::PushL( message );
        message->DecodeL( aContext );
        }

    return message;            
    }
    
    
    
    
    
    

