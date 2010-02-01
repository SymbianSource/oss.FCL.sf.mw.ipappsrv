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
#include "mcerefer.h"
#include "mcereferreceiver.h"
#include "mce.h"
#include "mceserial.h"
#include "mceclilogs.h"

// -----------------------------------------------------------------------------
// CMceReferReceiver::CMceReferReceiver
// -----------------------------------------------------------------------------
//
CMceReferReceiver::CMceReferReceiver( RMce& aMce,
                                          CMceRefer& aRefer )
 : CMceItcReceiverBase( aMce ),
   iRefer( aRefer )
	{
	iIdsPckg().iAppUID = aRefer.Manager().AppUid().iUid;
	iIdsPckg().iManagerType = KMceCSSIPEvent;
	iIdsPckg().iSessionID = aRefer.Id();
	
    Init();
	
	}

// -----------------------------------------------------------------------------
// CMceReferReceiver::~CMceReferReceiver
// -----------------------------------------------------------------------------
//
CMceReferReceiver::~CMceReferReceiver () 
	{
	}

// -----------------------------------------------------------------------------
// CMceReferReceiver::IncomingEventL
// -----------------------------------------------------------------------------
//
void CMceReferReceiver::IncomingEventL( TMceItcCallBack aEvent, TMceIds& aIds, 
                                          const TDesC8& aContext )
	{
    MCECLI_DEBUG("CMceReferReceiver::IncomingEventL, Entry");
    MCECLI_DEBUG_DVALUE("context size", aContext.Length() );
	
	switch ( aEvent )
	    {
	    case EMceItcStateChanged:
	        {
            MCECLI_DEBUG("CMceReferReceiver::IncomingEventL, incoming event. \
passing to refer");
        CMceMsgBase* message = DecodeMessageLC( aIds, aContext );
	        User::LeaveIfError( message ? KErrNone : KErrGeneral );
            message->PushL();

            iRefer.EventReceivedL( aIds, *message );
            
            message->Pop();
        	CleanupStack::PopAndDestroy( message );
	        
	        break;
	        }
        default:
            {
            MCECLI_DEBUG("CMceReferReceiver::IncomingEventL, not supported");
            User::Leave( KErrNotSupported );
            }
	    }
	
    MCECLI_DEBUG("CMceReferReceiver::IncomingEventL, Exit");
	}

	
// -----------------------------------------------------------------------------
// CMceReferReceiver::IncomingEventL
// -----------------------------------------------------------------------------
//
void CMceReferReceiver::IncomingEventL( TMceItcCallBack aEvent, TMceIds& aIds, 
                                          const TDesC8& aContext, 
                                          HBufC8* aContent )
	{
    MCECLI_DEBUG("CMceReferReceiver::IncomingEventL, Entry");
    MCECLI_DEBUG_DVALUE("context size", aContext.Length() );
    MCECLI_DEBUG_DVALUE("content size", aContent ? aContent->Length() : 0 );
    
	switch ( aEvent )
	    {
	    case EMceItcStateChanged:
	        {
            MCECLI_DEBUG("CMceReferReceiver::IncomingEventL, incoming event. \
passing to refer");
	        CMceMsgBase* message = DecodeMessageLC( aIds, aContext );
	        User::LeaveIfError( message ? KErrNone : KErrGeneral );
            message->PushL();

            iRefer.EventReceivedL( aIds, *message, aContent );
            
            message->Pop();
        	CleanupStack::PopAndDestroy( message );
	        break;
	        }
	   default:
            {
            MCECLI_DEBUG("CMceReferReceiver::IncomingEventL, not supported");
            User::Leave( KErrNotSupported );
            }
	    }
	    
    MCECLI_DEBUG("CMceReferReceiver::IncomingEventL, Exit");
	}

	
// -----------------------------------------------------------------------------
// CMceReferReceiver::IncomingEventL
// -----------------------------------------------------------------------------
//
void CMceReferReceiver::IncomingEventL( TMceItcCallBack /*aEvent*/, TMceIds& aIds )
	{
    MCECLI_DEBUG("CMceReferReceiver::IncomingEventL, Entry");
		
    iRefer.EventReceivedL( aIds );
    
    MCECLI_DEBUG("CMceReferReceiver::IncomingEventL, Exit");
	}

// -----------------------------------------------------------------------------
// CMceReferReceiver::ErrorOccuredL
// -----------------------------------------------------------------------------
//
void CMceReferReceiver::ErrorOccuredL (TMceIds& /*aIds*/, TInt aError )
	{
    MCECLI_DEBUG("CMceReferReceiver::IncomingEventL, Entry");
    iRefer.ErrorOccuredL( aError );
    MCECLI_DEBUG("CMceReferReceiver::ErrorOccuredL, Exit");
	}

// -----------------------------------------------------------------------------
// CMceReferReceiver::DecodeMessageL
// -----------------------------------------------------------------------------
//
CMceMsgBase* CMceReferReceiver::DecodeMessageLC( TMceIds& aIds, 
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
    
    
    
    
    
    

