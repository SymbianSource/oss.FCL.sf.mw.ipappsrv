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
#include "mceinsession.h"
#include "mcemediastream.h"
#include "mceaudiocodec.h"
#include "mcesessionreceiver.h"
#include "mcetransactionobserver.h" 
#include "mce.h"
#include "mceserial.h"
#include "mceclilogs.h"


// -----------------------------------------------------------------------------
// CMceSessionReceiver::CMceSessionReceiver
// -----------------------------------------------------------------------------
//
CMceSessionReceiver::CMceSessionReceiver( RMce& aMce,
                                          CMceSession& aSession )
 : CMceItcReceiverBase( aMce ),
   iSession( &aSession )
	{
	iIdsPckg().iAppUID = aSession.Manager().AppUid().iUid;
	iIdsPckg().iManagerType = KMceCSSIPSession;
	iIdsPckg().iSessionID = aSession.Id();
	
    Init();
	
	}

// -----------------------------------------------------------------------------
// CMceSessionReceiver::~CMceSessionReceiver
// -----------------------------------------------------------------------------
//
CMceSessionReceiver::~CMceSessionReceiver () 
	{
	}

// -----------------------------------------------------------------------------
// CMceSessionReceiver::IncomingEventL
// -----------------------------------------------------------------------------
//
void CMceSessionReceiver::IncomingEventL( TMceItcCallBack aEvent, TMceIds& aIds, 
                                          const TDesC8& aContext )
	{
    MCECLI_DEBUG("CMceSessionReceiver::IncomingEventL, Entry");
    MCECLI_DEBUG_DVALUE("context size", aContext.Length() );
	
	switch ( aEvent )
	    {
	    case EMceItcIncomingUpdate:
    	    {
            MCECLI_DEBUG("CMceSessionReceiver::IncomingEventL, incoming update. \
creating new in-session");
    	    
    		RDesReadStream readStream( aContext );
    	    CleanupClosePushL( readStream );
            
            CMceManager& manager = iSession->Manager();
    		CMceInSession* session = 
    		    CMceInSession::NewL( readStream, manager, aIds.iProfileID, ETrue );

    		CleanupStack::PopAndDestroy();//readStream
    		CleanupStack::PushL( session );
    		
    		iSession->IncomingUpdateL( session, NULL );
    		
    		CleanupStack::Pop( session );
    	    
    	    break;   
    	    }
	    case EMceItcStateChanged:
	    case EMceItcUpdateFailed:
	    case EMceItcResponseReceived: 
	    case EMceItcRequestReceived : 
	        {
            MCECLI_DEBUG("CMceSessionReceiver::IncomingEventL, incoming event. \
passing to session");
	        CMceMsgBase* message = DecodeMessageLC( aIds, aContext );
	        User::LeaveIfError( message ? KErrNone : KErrGeneral );
            message->PushL();

            iSession->EventReceivedL( aIds, *message );
            
        	CleanupStack::PopAndDestroy( message );
	        
	        break;
	        }
	    case EMceItcFCMsgReceived:
	        {
            MCECLI_DEBUG("CMceSessionReceiver::IncomingEventL, incoming FC event. \
passing to FC session");
	
			HBufC8* fcMessage = aContext.AllocL();
	        iSession->FCMessageReceived( fcMessage );
	        
	        break;
	        }
	    //no context expected            
        case EMceItcSRReceived:
        case EMceItcRRReceived:
        case EMceItcInactivityTimeout:
        case EMceItcSessionConnectionStateChanged:
        default:
            {
            MCECLI_DEBUG("CMceSessionReceiver::IncomingEventL, not supported");
            User::Leave( KErrNotSupported );
            }
	    }
	
    MCECLI_DEBUG("CMceSessionReceiver::IncomingEventL, Exit");
	}

	
// -----------------------------------------------------------------------------
// CMceSessionReceiver::IncomingEventL
// -----------------------------------------------------------------------------
//
void CMceSessionReceiver::IncomingEventL( TMceItcCallBack aEvent, TMceIds& aIds, 
                                          const TDesC8& aContext, 
                                          HBufC8* aContent )
	{
    MCECLI_DEBUG("CMceSessionReceiver::IncomingEventL, Entry");
    MCECLI_DEBUG_DVALUE("context size", aContext.Length() );
    MCECLI_DEBUG_DVALUE("content size", aContent ? aContent->Length() : 0 );

	switch ( aEvent )
	    {
	    case EMceItcIncomingUpdate:
    	    {
            MCECLI_DEBUG("CMceSessionReceiver::IncomingEventL, incoming update. \
creating new in-session");
    	    
    		RDesReadStream readStream( aContext );
    	    CleanupClosePushL( readStream );

            CMceManager& manager = iSession->Manager();
    		CMceInSession* session = 
    		    CMceInSession::NewL( readStream, manager, aIds.iProfileID, ETrue );

    		CleanupStack::PopAndDestroy();//readStream
    		CleanupStack::PushL( session );
    		
    		iSession->IncomingUpdateL( session, aContent );
    		
    		CleanupStack::Pop( session );
    	    
    	    break;   
    	    }
	    case EMceItcStateChanged:
	    case EMceItcUpdateFailed:
	    case EMceItcResponseReceived: 
	    case EMceItcRequestReceived:
	        {
            MCECLI_DEBUG("CMceSessionReceiver::IncomingEventL, incoming event. \
passing to session");
	        
	        CMceMsgBase* message = DecodeMessageLC( aIds, aContext );
	        User::LeaveIfError( message ? KErrNone : KErrGeneral );
            message->PushL();

            iSession->EventReceivedL( aIds, *message, aContent );

        	CleanupStack::PopAndDestroy( message );
	        break;
	        }
	    //no content expected
        case EMceItcSRReceived:
        case EMceItcRRReceived:
        case EMceItcInactivityTimeout:
        case EMceItcSessionConnectionStateChanged:
        default:
            {
            MCECLI_DEBUG("CMceSessionReceiver::IncomingEventL, not supported");
            User::Leave( KErrNotSupported );
            }
	    }

    MCECLI_DEBUG("CMceSessionReceiver::IncomingEventL, Exit");
	    
	}

	
// -----------------------------------------------------------------------------
// CMceSessionReceiver::IncomingEventL
// -----------------------------------------------------------------------------
//
void CMceSessionReceiver::IncomingEventL( TMceItcCallBack aEvent, TMceIds& aIds )
	{
    MCECLI_DEBUG("CMceSessionReceiver::IncomingEventL, Entry");
	
	if ( aEvent == EMceItcFCErrorReceived )
	    {
        MCECLI_DEBUG("CMceSessionReceiver::IncomingEventL, FC error occured");
	    iSession->FCErrorOccurred( aIds.iStatus );
	    }
	else
	    {
        MCECLI_DEBUG("CMceSessionReceiver::IncomingEventL, incoming event. \
passing to session");
	    iSession->EventReceivedL( aIds );    
	    }
    }


// -----------------------------------------------------------------------------
// CMceSessionReceiver::ErrorOccuredL
// -----------------------------------------------------------------------------
//
void CMceSessionReceiver::ErrorOccuredL (TMceIds& aIds, TInt aError )
	{
    MCECLI_DEBUG("CMceSessionReceiver::ErrorOccuredL, Entry");
    MCECLI_DEBUG_DVALUE("error", aError );
    
    if ( aIds.iTransactionID != KMceNotAssigned )

    	{
    	MCECLI_DEBUG("Handling Extension Requests Error");	

        MMceTransactionObserver* observer = iSession->Manager().TransactionObserver();
        
        if ( observer )
            {
            observer->TransactionFailed( *iSession, aIds.iTransactionID, aError );
            }
    	}
    
    else
    	{
    	iSession->ErrorOccuredL( aError );		
    	}
    
    
    
    MCECLI_DEBUG("CMceSessionReceiver::ErrorOccuredL, Exit");
    
	}




// -----------------------------------------------------------------------------
// CMceSessionReceiver::DecodeMessageL
// -----------------------------------------------------------------------------
//
CMceMsgBase* CMceSessionReceiver::DecodeMessageLC( TMceIds& aIds, 
                                                   const TDesC8& aContext )
    {
    
	RDesReadStream readStream( aContext );
    CleanupClosePushL( readStream );
    
    CMceMsgBase* message = TMceFactory().CreateL( (TMceItcDataType)aIds.iMsgType, 
                                                  readStream );
	CleanupStack::PopAndDestroy(); // readStream
	
	CleanupStack::PushL( message );

    return message;
            
    }

// -----------------------------------------------------------------------------
// CMceSessionReceiver::ReplaceSession
// -----------------------------------------------------------------------------
//
void CMceSessionReceiver::ReplaceSession( CMceSession& aNewSession )
    {
    iSession = &aNewSession;    
    }
