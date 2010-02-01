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




#include "mceinsessionobserver.h"
#include "mcemanager.h"
#include "mceinsession.h"
#include "mcetransactiondatacontainer.h"
#include "mcemanagerreceiver.h"
#include "mce.h"
#include "mcecomsession.h"
#include "mceserial.h"
#include "mceclilogs.h"
#include "mcesecureinsession.h"

// -----------------------------------------------------------------------------
// CMceManagerReceiver::CMceManagerReceiver
// -----------------------------------------------------------------------------
//
CMceManagerReceiver::CMceManagerReceiver( RMce& aMce,
                                          CMceManager& aManager )
 : CMceItcReceiverBase ( aMce ),
   iManager( aManager )
	{
	iIdsPckg().iAppUID = aManager.AppUid().iUid;
	iIdsPckg().iManagerType = KMceCSSIPSession;
	
    Init();
	
	}

// -----------------------------------------------------------------------------
// CMceManagerReceiver::~CMceManagerReceiver
// -----------------------------------------------------------------------------
//
CMceManagerReceiver::~CMceManagerReceiver () 
	{
	}

// -----------------------------------------------------------------------------
// CMceManagerReceiver::IncomingEventL
// -----------------------------------------------------------------------------
//
void CMceManagerReceiver::IncomingEventL( TMceItcCallBack aEvent, 
                                          TMceIds& aIds, 
                                          const TDesC8& aContext )
	{

    MCECLI_DEBUG("CMceManagerReceiver::IncomingEventL, Entry");
    MCECLI_DEBUG_DVALUE("context size", aContext.Length() );
	
	__ASSERT_ALWAYS( aEvent == EMceItcObjectAdded , 
	    User::Leave( KErrNotSupported ) );
	__ASSERT_ALWAYS( aIds.iMsgType == EMceItcMsgTypeSession , 
	    User::Leave( KErrNotSupported ) );
    
    HandleIncomingSessionL( aContext, NULL, aIds );
    
    MCECLI_DEBUG("CMceManagerReceiver::IncomingEventL, Exit");
    
	}

	
// -----------------------------------------------------------------------------
// CMceManagerReceiver::IncomingEventL
// -----------------------------------------------------------------------------
//
void CMceManagerReceiver::IncomingEventL( TMceItcCallBack aEvent, 
                                          TMceIds& aIds, 
                                          const TDesC8& aContext, 
                                          HBufC8* aContent )
	{
    MCECLI_DEBUG("CMceManagerReceiver::IncomingEventL, Entry");
    MCECLI_DEBUG_DVALUE("context size", aContext.Length() );
    MCECLI_DEBUG_DVALUE("content size", aContent ? aContent->Length() : 0 );

	__ASSERT_ALWAYS( aEvent == EMceItcObjectAdded , 
	    User::Leave( KErrNotSupported ) );
	__ASSERT_ALWAYS( aIds.iMsgType == EMceItcMsgTypeSession , 
	    User::Leave( KErrNotSupported ) );
    
    HandleIncomingSessionL( aContext, aContent, aIds );

    MCECLI_DEBUG("CMceManagerReceiver::IncomingEventL, Exit");
    
	}
	
// -----------------------------------------------------------------------------
// CMceManagerReceiver::IncomingEventL
// -----------------------------------------------------------------------------
//
void CMceManagerReceiver::IncomingEventL( TMceItcCallBack /*aEvent*/, 
                                          TMceIds& aIds )
	{
    MCECLI_DEBUG("CMceManagerReceiver::IncomingEventL, Entry");
	iManager.HandleIncomingEventL( aIds );
    MCECLI_DEBUG("CMceManagerReceiver::IncomingEventL, Exit");
	}


// -----------------------------------------------------------------------------
// CMceManagerReceiver::ErrorOccuredL
// -----------------------------------------------------------------------------
//
void CMceManagerReceiver::ErrorOccuredL (TMceIds& /*aIds*/, TInt aError )
	{
    MCECLI_DEBUG_DVALUE("CMceManagerReceiver::ErrorOccuredL, error", aError );
	User::Leave( aError );
	}


// -----------------------------------------------------------------------------
// CMceManagerReceiver::HandleIncomingSessionL
// -----------------------------------------------------------------------------
//
void CMceManagerReceiver::HandleIncomingSessionL ( const TDesC8& aContext, 
                                                   HBufC8* aContent,
                                                   TMceIds& aIds )
	{
    MCECLI_DEBUG("CMceManagerReceiver::HandleIncomingSessionL, Entry");
	
	if ( iManager.InSessionObserver() )
		{	
	    CMceInSession* session = NULL;
	    
		RDesReadStream readStream( aContext );
	    CleanupClosePushL( readStream );

        MCECLI_DEBUG("CMceManagerReceiver::HandleIncomingSessionL, creating new session");
		
		if (aIds.iSecureFlag)
			{
			session = CMceSecureInSession::NewL( readStream, iManager, aIds.iProfileID );
			}
		else
			{
			session = CMceInSession::NewL( readStream, iManager, aIds.iProfileID );
			}
		

		CleanupStack::PopAndDestroy();//readStream
		CleanupStack::PushL( session );
		CDesC8Array* headers = NULL;
	    HBufC8* contentType = NULL;
	    CDesC8Array* contentHeaders = NULL;
	    session->GetSIPParams( headers, contentType, contentHeaders );
	    
		TInt pushs = 0;
		
		if ( headers )
			{
			CleanupStack::PushL( headers );++pushs;
			}
		if ( contentType )
			{
			CleanupStack::PushL( contentType );++pushs;
			}
        if ( contentHeaders )
			{
			CleanupStack::PushL( contentHeaders );++pushs;
			}
		
		
	    TMceTransactionDataContainer* container = 
	        iManager.TransactionContainer();
	    
		if ( container )
			{
            container->SetHeaders( headers );
            container->SetContentType( contentType );
            container->SetContent( aContent );
            container->SetContentHeaders( contentHeaders );
			iManager.InSessionObserver()->IncomingSession( session, 
			                                               container );
			CleanupStack::Pop( pushs );
			}
	    else
	        {
			CleanupStack::PopAndDestroy( pushs );
			iManager.InSessionObserver()->IncomingSession( session, 
			                                               container );
	        }
	        
		CleanupStack::Pop( session );
		
	    if (container)
	    	{
	    	container->Clear();
	    	}						
		}
	else
		{
		delete aContent;	
		}

    MCECLI_DEBUG("CMceManagerReceiver::HandleIncomingSessionL, Exit");

	}
	
