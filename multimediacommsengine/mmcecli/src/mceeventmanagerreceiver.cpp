/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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




#include "mceineventobserver.h"
#include "mceinreferobserver.h"
#include "mcemanager.h"
#include "mceinevent.h" 
#include "mceinrefer.h"
#include "mcetransactiondatacontainer.h"
#include "mceserial.h"
#include "mceeventmanagerreceiver.h"
#include "mce.h"
#include "mceclilogs.h"

// -----------------------------------------------------------------------------
// CMceEventManagerReceiver::CMceEventManagerReceiver
// -----------------------------------------------------------------------------
//
CMceEventManagerReceiver::CMceEventManagerReceiver( RMce& aMce,
                                                    CMceManager& aManager)
                                          
 : CMceItcReceiverBase ( aMce ),
   iManager( aManager )
	{
	iIdsPckg().iAppUID = aManager.AppUid().iUid;
	iIdsPckg().iManagerType = KMceCSSIPEvent;
	Init();
	}

// -----------------------------------------------------------------------------
// CMceEventManagerReceiver::~CMceEventManagerReceiver
// -----------------------------------------------------------------------------
//
CMceEventManagerReceiver::~CMceEventManagerReceiver () 
	{
	}

// -----------------------------------------------------------------------------
// CMceEventManagerReceiver::IncomingEventL
// -----------------------------------------------------------------------------
//
void CMceEventManagerReceiver::IncomingEventL( TMceItcCallBack aEvent, 
                                               TMceIds& aIds, 
                                               const TDesC8& aContext )
	{
    MCECLI_DEBUG("CMceEventManagerReceiver::IncomingEventL, Entry");
    MCECLI_DEBUG_DVALUE("context size", aContext.Length() );
	
	__ASSERT_ALWAYS( aEvent == EMceItcObjectAdded, 
	    User::Leave( KErrNotSupported ) );
	__ASSERT_ALWAYS( aIds.iMsgType == EMceItcMsgTypeEvent, 
	    User::Leave( KErrNotSupported ) );
    
    HandleIncomingEventL( aContext, NULL, aIds );
	
    MCECLI_DEBUG("CMceEventManagerReceiver::IncomingEventL, Exit");
	}

	
// -----------------------------------------------------------------------------
// CMceEventManagerReceiver::IncomingEventL
// -----------------------------------------------------------------------------
//
void CMceEventManagerReceiver::IncomingEventL( TMceItcCallBack aEvent, 
                                               TMceIds& aIds, 
                                               const TDesC8& aContext, 
                                               HBufC8* aContent )
	{
    MCECLI_DEBUG("CMceManagerReceiver::IncomingEventL, Entry");
    MCECLI_DEBUG_DVALUE("context size", aContext.Length() );
    MCECLI_DEBUG_DVALUE("content size", aContent ? aContent->Length() : 0 );

	__ASSERT_ALWAYS( aEvent == EMceItcObjectAdded, 
	    User::Leave( KErrNotSupported ) );
	__ASSERT_ALWAYS( aIds.iMsgType == EMceItcMsgTypeEvent, 
	    User::Leave( KErrNotSupported ) );
    
    HandleIncomingEventL( aContext, aContent, aIds );

    MCECLI_DEBUG("CMceEventManagerReceiver::IncomingEventL, Exit");
	
	}
	
// -----------------------------------------------------------------------------
// CMceEventManagerReceiver::IncomingEventL
// -----------------------------------------------------------------------------
//
void CMceEventManagerReceiver::IncomingEventL( TMceItcCallBack /*aEvent*/, 
                                               TMceIds& /*aIds*/ )
	{
    MCECLI_DEBUG("CMceManagerReceiver::IncomingEventL, NotSupported!");
	
	User::Leave( KErrNotSupported );
	}


// -----------------------------------------------------------------------------
// CMceEventManagerReceiver::ErrorOccuredL
// -----------------------------------------------------------------------------
//
void CMceEventManagerReceiver::ErrorOccuredL ( TMceIds& /*aIds*/, TInt aError )
	{
    MCECLI_DEBUG_DVALUE("CMceEventManagerReceiver::ErrorOccuredL, error", aError );
	
	User::Leave( aError );
	}


// -----------------------------------------------------------------------------
// CMceEventManagerReceiver::HandleIncomingEventL
// -----------------------------------------------------------------------------
//
void CMceEventManagerReceiver::HandleIncomingEventL ( const TDesC8& aContext, 
                                                      HBufC8* aContent,
                                                      TMceIds& aIds )
	{
    MCECLI_DEBUG("CMceEventManagerReceiver::HandleIncomingEventL, Entry");
	     
	// create and decode message
	CMceMsgSIPEvent* eventMsg = new(ELeave) CMceMsgSIPEvent();
	CleanupStack::PushL( eventMsg );
	eventMsg->DecodeL( aContext );
	eventMsg->PushL();
        
    TMceTransactionDataContainer* container = iManager.TransactionContainer();
    
    if ( eventMsg->EventType() == EMceItcEventTypeSubscribe )
        {
        MCECLI_DEBUG("CMceEventManagerReceiver::HandleIncomingEventL, \
creating new in-event");
        
        CMceInEvent* event = CMceInEvent::NewL( iManager, 
                                            aIds.iProfileID, 
                                            *eventMsg );
        if ( iManager.InEventObserver() )
            {
            if ( container )
                {
                container->SetHeaders( eventMsg->SIPHeaders() );
                container->SetContentType( eventMsg->ContentType() );
                container->SetContent( aContent ); 
                eventMsg->Pop();
	            } 
	        else
	            {
	            delete aContent;
	            }
	        iManager.InEventObserver()->IncomingEvent( event, container );
            }
        else
            {
            CleanupStack::PushL( event );
            event->RejectL();
            CleanupStack::PopAndDestroy( event );
            delete aContent;
            }
        }
    else if ( eventMsg->EventType() == EMceItcEventTypeRefer ) 
        {
        CMceInEvent* event = NULL;
        if ( eventMsg->ReferType() == CMceRefer::ENoSuppression )
            {            
            MCECLI_DEBUG("CMceEventManagerReceiver::HandleIncomingEventL, \
no suppression. creating new in-event");
            event = CMceInEvent::NewL( iManager, aIds.iProfileID, *eventMsg );
            }
            
        CleanupStack::PushL( event );
	    CMceMsgSIPEvent* referMsg = new(ELeave) CMceMsgSIPEvent();
	    CleanupStack::PushL( referMsg );
	    referMsg->DecodeL( aContext );
	    referMsg->PushL();
            MCECLI_DEBUG("CMceEventManagerReceiver::HandleIncomingEventL, \
creating new in-refer-event");
	    CMceInRefer* refer = CMceInRefer::NewL( iManager, 
                                                aIds.iProfileID, 
                                                *referMsg,
                                                event );
	    
	    CleanupStack::Pop( referMsg );
	    CleanupStack::Pop( event ); // Ownership was transferred to refer
	    CleanupStack::PushL( referMsg );
	    
	    if ( iManager.InReferObserver() )
            {
            if ( container )
                {
                container->SetHeaders( referMsg->SIPHeaders() );
                container->SetContentType( referMsg->ContentType() );
                container->SetContent( aContent );
                referMsg->Pop();                                    
	            } 
	        else
	            {
	            delete aContent;
	            }
            iManager.InReferObserver()->IncomingRefer( 
                                       refer,
					                   refer->ReferTo(),
					                   container ); 
		    CleanupStack::PopAndDestroy( referMsg ); 
            }
        else
            {
            CleanupStack::PushL( refer );
            refer->RejectL();
            CleanupStack::PopAndDestroy( refer );
            CleanupStack::PopAndDestroy( referMsg );
            delete aContent;
            }   
        }
    else
        {
        MCECLI_DEBUG("CMceEventManagerReceiver::HandleIncomingEventL, \
not supported");
        User::Leave( KErrNotSupported );
        }
    
    CleanupStack::PopAndDestroy( eventMsg );
	
    if ( container )
    	{
    	container->Clear();
    	}
	}


