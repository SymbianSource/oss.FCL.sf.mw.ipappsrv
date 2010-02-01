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




#include "mcecsreceiverbase.h"
#include "mcecsreceivequeue.h"
#include "mcecsserveritc.h"
#include "mcesrvlogs.h"

// -----------------------------------------------------------------------------
// CMceCsReceiverBase::ConstructL
// -----------------------------------------------------------------------------
//
void CMceCsReceiverBase::ConstructL ()
	{
    iReceiveQueue = new (ELeave) CMceCsReceiveQueue;
	}

// -----------------------------------------------------------------------------
// CMceCsReceiverBase::CMceCsReceiverBase
// -----------------------------------------------------------------------------
//
CMceCsReceiverBase::CMceCsReceiverBase (CMceCsServerITC& aItc)
:iITC (aItc), iClientReadyToReceive (EFalse)
	{
	}

// -----------------------------------------------------------------------------
// CMceCsReceiverBase::~CMceCsReceiverBase
// -----------------------------------------------------------------------------
//
CMceCsReceiverBase::~CMceCsReceiverBase()
	{
    delete iReceiveQueue;
	}

// -----------------------------------------------------------------------------
// CMceCsReceiverBase::ClientReadyToReceiveL
// -----------------------------------------------------------------------------
//
void CMceCsReceiverBase::ClientReadyToReceiveL (const RMessage2& aMessage)
	{
    MCESRV_DEBUG("CMceCsReceiverBase::ClientReadyToReceiveL, Entry");
    
    __ASSERT_ALWAYS (!iClientReadyToReceive, User::Leave (KErrAlreadyExists));
    
    MCESRV_DEBUG("storing ids");
    
	iIds = iITC.ReadMCEIdsL (aMessage);

    if (iReceiveQueue->IsEmpty())
		{
        iClientReadyToReceive = ETrue;
        iReceiveRMessage = aMessage;
		}
    else
		{
    	MCESRV_DEBUG("queue not empty") 	
        const TMceCSReceiveItem& item = iReceiveQueue->First();
		TMceMessageBufSizes bufSizes = item.BufSizes();
        iITC.WriteL (aMessage,item.Ids());
        iITC.WriteL (aMessage,bufSizes);
        if (bufSizes.iContentBufSize < 0 && bufSizes.iContextBufSize < 0)
        	{
    		MCESRV_DEBUG("removing stored data");
			iReceiveQueue->RemoveFirst ();    
        	}
        iITC.Complete (aMessage, item.Error());
        iClientReadyToReceive = EFalse;
    	MCESRV_DEBUG("event written");
		}
    MCESRV_DEBUG("CMceCsReceiverBase::ClientReadyToReceiveL, Exit");
	}

// -----------------------------------------------------------------------------
// CMceCsReceiverBase::CancelClientReceiveL
// -----------------------------------------------------------------------------
//
void CMceCsReceiverBase::CancelClientReceiveL ()
	{
    __ASSERT_ALWAYS (iClientReadyToReceive, User::Leave (KErrNotFound));

    iClientReadyToReceive = EFalse;
    iITC.Complete (iReceiveRMessage, KErrCancel);
	}

// -----------------------------------------------------------------------------
// CMceCsReceiverBase::SendToClientL
// -----------------------------------------------------------------------------
//       
void CMceCsReceiverBase::SendToClientL( TMceIds& aId, TInt aError)
	{
    MCESRV_DEBUG("CMceCsReceiverBase::SendToClientL( ids ), Entry");
    MCESRV_DEBUG_DVALUE("error", aError );
	
    MCESRV_DEBUG_IDS("IDS", aId );
	
	TMceCSReceiveItem item(aId, NULL, NULL, aError);
	HandleNewReceiveItemL( item );

    MCESRV_DEBUG("CMceCsReceiverBase::SendToClientL( ids ), Exit");
	
	}

// -----------------------------------------------------------------------------
// CMceCsReceiverBase::SendToClientL
// -----------------------------------------------------------------------------
//       
void CMceCsReceiverBase::SendToClientL( TMceIds& aId, HBufC8* aContext )
	{
    MCESRV_DEBUG("CMceCsReceiverBase::SendToClientL( ids, context), Entry");
	
    __ASSERT_ALWAYS (aContext, User::Leave (KErrArgument));

	TMceCSReceiveItem item(aId, aContext, NULL, KErrNone);
	HandleNewReceiveItemL( item );

    MCESRV_DEBUG("CMceCsReceiverBase::SendToClientL( ids, context), Exit");
	
	}

// -----------------------------------------------------------------------------
// CMceCsReceiverBase::SendToClientL
// -----------------------------------------------------------------------------
//       
void CMceCsReceiverBase::SendToClientL( TMceIds& aId, HBufC8* aContext, HBufC8* aContent )
	{
    MCESRV_DEBUG("CMceCsReceiverBase::SendToClientL( ids, context, content ), Entry");
	
    __ASSERT_ALWAYS (aContext, User::Leave (KErrArgument));
    __ASSERT_ALWAYS (aContent, User::Leave (KErrArgument));

	TMceCSReceiveItem item(aId, aContext, aContent, KErrNone);
	HandleNewReceiveItemL( item );
	
    MCESRV_DEBUG("CMceCsReceiverBase::SendToClientL( ids, context, content ), Exit");
	
	}

// -----------------------------------------------------------------------------
// CMceCsReceiverBase::Ids
// -----------------------------------------------------------------------------
//       
TMceIds CMceCsReceiverBase::Ids() const
	{
	return iIds;
	}
	
// -----------------------------------------------------------------------------
// CMceCsReceiverBase::ITC
// -----------------------------------------------------------------------------
//       
CMceCsServerITC& CMceCsReceiverBase::ITC()
    {
    return iITC;
    }
	

// -----------------------------------------------------------------------------
// CMceCsReceiverBase::HandleNewReceiveItemL
// -----------------------------------------------------------------------------
//       
void CMceCsReceiverBase::HandleNewReceiveItemL (TMceCSReceiveItem& aItem)
    {
    MCESRV_DEBUG("CMceCsReceiverBase::HandleNewReceiveItemL, Entry");
    
    if ( iClientReadyToReceive )
        {
    	MCESRV_DEBUG("client ready, sending") 	
		TMceMessageBufSizes bufSizes = aItem.BufSizes();
        iITC.WriteL( iReceiveRMessage, aItem.Ids() );
        iITC.WriteL( iReceiveRMessage, bufSizes );
        if ( bufSizes.iContentBufSize >= 0 || bufSizes.iContextBufSize >= 0 )
        	{
    		MCESRV_DEBUG("stored for receive");
        	User::LeaveIfError( iReceiveQueue->AddLast( aItem ) );        	
        	}
        iITC.Complete ( iReceiveRMessage, aItem.Error() );
        iClientReadyToReceive = EFalse;
        }
    else
        {
    	MCESRV_DEBUG("client not ready, storing");
        User::LeaveIfError( iReceiveQueue->AddLast( aItem ) );
        }     
    MCESRV_DEBUG("CMceCsReceiverBase::HandleNewReceiveItemL, Exit");
    }

// -----------------------------------------------------------------------------
// CMceCsReceiverBase::ReceiveL
// -----------------------------------------------------------------------------
//
void CMceCsReceiverBase::ReceiveL (const RMessage2& aMessage)
	{
    MCESRV_DEBUG("CMceCsReceiverBase::ReceiveL, Entry");

    __ASSERT_ALWAYS(!iReceiveQueue->IsEmpty(), User::Leave(KErrNotReady));

    TMceCSReceiveItem& item = iReceiveQueue->First();
	TMceIds ids = iITC.ReadMCEIdsL( aMessage );
    (void)ids;
    if ( item.Content() )
        {
    	MCESRV_DEBUG_DVALUE("writing context. size", 
    	    item.Context() ? item.Context()->Length() : 0 );
    	    
    	iITC.WriteL( aMessage, *(item.Context()), EMceItcArgContext );
    	
    	MCESRV_DEBUG_DVALUE("writing content. size", 
    	    item.Context() ? item.Content()->Length() : 0 );
    	    
    	iITC.WriteL( aMessage, *(item.Content()), EMceItcArgMessageContent );
        }   
	else
		{
    	MCESRV_DEBUG_DVALUE("writing context. size", 
    	    item.Context() ? item.Context()->Length() : 0 );
    	iITC.WriteL( aMessage, *(item.Context()), EMceItcArgContext );
		}
						
    MCESRV_DEBUG("removing stored data");
	iReceiveQueue->RemoveFirst ();
	
    MCESRV_DEBUG("CMceCsReceiverBase::ReceiveL, Exit");
    
	}
