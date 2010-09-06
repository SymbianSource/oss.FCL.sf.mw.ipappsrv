/*
* Copyright (c) 2004-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    UL/DL Session abstraction for Mcc Subthread clients
*
*/

// INCLUDE FILES
#include "msrpcallbackmgrobserver.h"
#include "mccsubcontrollerlogs.h"
#include <e32base.h>
#include "mccinternalevents.h"
#include "mmcccodecinformation.h"

CMsrpCallbackMgrObserver* CMsrpCallbackMgrObserver::NewL(MAsyncEventHandler& aEventHandler)
	{
    CMsrpCallbackMgrObserver* self = new(ELeave) CMsrpCallbackMgrObserver(aEventHandler);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
	}


void CMsrpCallbackMgrObserver::ConstructL()
    {
    
    }


CMsrpCallbackMgrObserver::CMsrpCallbackMgrObserver(
    MAsyncEventHandler& aEventHandler) : 
    iEventHandler( aEventHandler )
    {
    iDataObserver = NULL;
    iConnObserver = NULL;
    }

// ============================= LOCAL FUNCTIONS ===============================

void CMsrpCallbackMgrObserver::IncomingMessage( HBufC8* aMessageContent, TInt aStatus )
    {
    __SUBCONTROLLER( "CMsrpCallbackMgrObserver::IncomingMessage" )
    
    if(iDataObserver)
        {
        iDataObserver->IncomingMessage(aMessageContent,aStatus);
        }
    }
    
void CMsrpCallbackMgrObserver::IncomingReport( TInt aStatus, TInt aRangeStart,
                                           TInt aRangeEnd, TInt aTotal )
	{
    __SUBCONTROLLER( "CMsrpCallbackMgrObserver::IncomingReport" )
            
    if(iDataObserver)
        {
        iDataObserver->IncomingReport( aStatus, aRangeStart, aRangeEnd,  aTotal );
        }
	}
    
void CMsrpCallbackMgrObserver::SendResult( TInt aStatus, const TDesC8& aMessageid )
	{
    __SUBCONTROLLER( "CMsrpCallbackMgrObserver::SendResult" )
            
    if(iDataObserver)
        {
        iDataObserver->SendResult(aStatus, aMessageid );
        }
	}
    
void CMsrpCallbackMgrObserver::ListenPortFailure( TInt aStatus )
	{
    __SUBCONTROLLER( "CMsrpCallbackMgrObserver::ListenPortFailure" )
            
    if(iDataObserver)
        {
        iDataObserver->ListenPortFailure(aStatus);
        }
	}
    
void CMsrpCallbackMgrObserver::ConnectStatus( TInt aStatus )
	{
    __SUBCONTROLLER( "CMsrpCallbackMgrObserver::ConnectStatus" )
            
    if(iConnObserver)    
        {
        iConnObserver->ConnectStatus(aStatus);
        }
	}
		
void CMsrpCallbackMgrObserver::RegisterConnectionObserver (MMsrpConnectionObserver& aObserver)
	{
    __SUBCONTROLLER( "CMsrpCallbackMgrObserver::RegisterConnectionObserver" )
	iConnObserver = &aObserver;
	}
		
		
void CMsrpCallbackMgrObserver::RegisterDataObserver (MMsrpDataObserver& aObserver)
	{
    __SUBCONTROLLER( "CMsrpCallbackMgrObserver::RegisterDataObserver" )
	iDataObserver = &aObserver;
	}

//
//
//
void CMsrpCallbackMgrObserver::SendFileNotification(TBool aStatus)
    {
    __SUBCONTROLLER( "CMsrpCallbackMgrObserver::SendFileNotification : Entry" )
    iMccEvent = TMccEvent();
    
    iMccEvent.iEndpointId = KErrNone;
    iMccEvent.iEventCategory = 0;
    iMccEvent.iEventType = KMccFileSendCompleted;
    iMccEvent.iErrorCode = (TInt)aStatus;
    iMccEvent.iEventNumData = 0;
    
    TMccInternalEvent internalEvent( KUidMediaTypeMessage, 
                                             EMccInternalEventNone,
                                             iMccEvent );
    iEventHandler.SendEventToClient(internalEvent);
    __SUBCONTROLLER( "CMsrpCallbackMgrObserver::SendFileNotification : Exit" )
    }

/* Notifies that the entire file has been received successfully */
void CMsrpCallbackMgrObserver::ReceiveFileNotification(TBool status)
    {
    __SUBCONTROLLER( "CMsrpCallbackMgrObserver::ReceiveFileNotification : Entry" )
    iMccEvent = TMccEvent();
    
    iMccEvent.iEndpointId = KErrNone;
    iMccEvent.iEventCategory = 0;
    iMccEvent.iEventType = KMccFileReceiveCompleted;
    iMccEvent.iErrorCode = (TInt)status;
    iMccEvent.iEventNumData = 0;
    
    TMccInternalEvent internalEvent( KUidMediaTypeMessage, 
                                             EMccInternalEventNone,
                                             iMccEvent );
    iEventHandler.SendEventToClient(internalEvent);
    __SUBCONTROLLER( "CMsrpCallbackMgrObserver::ReceiveFileNotification : Exit" )
    }
/**
* File data transfer progress. returns the progress of data transfer
* @param aTransferred number of bytes transferred
* @param aTotal Total amount of bytes to be transferred
*/
void CMsrpCallbackMgrObserver::FileSendProgress( TInt aTransferred, TInt aTotal )
    {
    __SUBCONTROLLER( "CMsrpCallbackMgrObserver::FileSendProgress : Entry" )
    iMccEvent = TMccEvent();
    
    iMccEvent.iEndpointId = KErrNone;
    iMccEvent.iEventCategory = 0;
    iMccEvent.iEventType = KMccFileSendProgressNotification;
    iMccEvent.iErrorCode = (TInt)aTransferred;
    iMccEvent.iEventNumData = aTotal;
    
    TMccInternalEvent internalEvent( KUidMediaTypeMessage, 
                                             EMccInternalEventNone,
                                             iMccEvent );
    iEventHandler.SendEventToClient(internalEvent);
    __SUBCONTROLLER( "CMsrpCallbackMgrObserver::FileSendProgress : Exit" )
    
    }

/**
* File receive progress, number of bytes received from incoming data
* @param aReceived number of bytes received
* @param aTotal Total amount of bytes to be received
*/
void CMsrpCallbackMgrObserver::FileReceiveProgress( TInt aReceived, TInt aTotal )
    {
    __SUBCONTROLLER( "CMsrpCallbackMgrObserver::FileReceiveProgress : Entry" )
    iMccEvent = TMccEvent();
    
    iMccEvent.iEndpointId = KErrNone;
    iMccEvent.iEventCategory = 0;
    iMccEvent.iEventType = KMccFileReceiveProgressNotification;
    iMccEvent.iErrorCode = (TInt)aReceived;
    iMccEvent.iEventNumData = aTotal;
    
    TMccInternalEvent internalEvent( KUidMediaTypeMessage, EMccInternalEventNone, iMccEvent );
    iEventHandler.SendEventToClient(internalEvent);
    __SUBCONTROLLER( "CMsrpCallbackMgrObserver::FileReceiveProgress : Exit" )
    }
