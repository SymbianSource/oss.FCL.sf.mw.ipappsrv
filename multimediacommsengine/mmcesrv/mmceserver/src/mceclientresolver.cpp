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





#include <sipclientdiscovery.h>
#include <sipservertransaction.h>
#include <siprequestelements.h>
#include <sipmessageelements.h>
#include <sipresponseelements.h>
#include <sipcontenttypeheader.h>
#include <sipstrings.h>
#include <sipstrconsts.h>
#include <siptoheader.h>
#include <sipfromheader.h>
#include <sipcseqheader.h>

#include "mceclientresolver.h"
#include "mcesipconnection.h"
#include "mceclientserver.h"
#include "cleanupresetanddestroy.h"



// -----------------------------------------------------------------------------
// CMceClientResolver::CMceRoutingRequest::CMceRoutingRequest
// -----------------------------------------------------------------------------
//
CMceClientResolver::CMceRoutingRequest::CMceRoutingRequest( TUint32 aResolverRequestId,
                                                            CMceSipConnection& aConnection, 
                                                            CSIPServerTransaction* aRequest )
 : iResolverRequestId(aResolverRequestId),
   iRequest(aRequest),
   iConnection(aConnection)
    {    
    }


// -----------------------------------------------------------------------------
// CMceClientResolver::CMceRoutingRequest::~CMceRoutingRequest
// -----------------------------------------------------------------------------
//
CMceClientResolver::CMceRoutingRequest::~CMceRoutingRequest()
    {
    delete iRequest;
    }
    
// -----------------------------------------------------------------------------
// CMceClientResolver::CMceRoutingRequest::LinkOffset
// -----------------------------------------------------------------------------
//
TInt CMceClientResolver::CMceRoutingRequest::LinkOffset ()
	{
	return _FOFF(CMceClientResolver::CMceRoutingRequest,iLink);
	}    

    
// -----------------------------------------------------------------------------
// CMceClientResolver::CMceRoutingRequest::ResolverRequestId
// -----------------------------------------------------------------------------
//
TUint32 CMceClientResolver::CMceRoutingRequest::ResolverRequestId() const
	{
	return iResolverRequestId;		
	}
    
// -----------------------------------------------------------------------------
// CMceClientResolver::CMceRoutingRequest::Request
// -----------------------------------------------------------------------------
// 
CSIPServerTransaction* CMceClientResolver::CMceRoutingRequest::Request()
	{
	CSIPServerTransaction* req = iRequest;
	iRequest = NULL;
	return req;
	}
	
	
// -----------------------------------------------------------------------------
// CMceClientResolver::CMceRoutingRequest::Connection
// -----------------------------------------------------------------------------
// 	
CMceSipConnection& CMceClientResolver::CMceRoutingRequest::Connection() const
	{
	return iConnection;
	}



// ----------------------------------------------------------------------------
// CMceClientResolver::NewL
// ----------------------------------------------------------------------------
//
CMceClientResolver* 
CMceClientResolver::NewL( MMceClientResolverObserver& aRequestHandler )
    {
    CMceClientResolver* self = CMceClientResolver::NewLC( aRequestHandler );
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CMceClientResolver::NewLC
// ----------------------------------------------------------------------------
//
CMceClientResolver* 
CMceClientResolver::NewLC( MMceClientResolverObserver& aRequestHandler )
    {
	CMceClientResolver* self = 
		new(ELeave)CMceClientResolver( aRequestHandler );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ----------------------------------------------------------------------------
// CMceClientResolver::ConstructL
// ----------------------------------------------------------------------------
//
void CMceClientResolver::ConstructL ()
    {
    iDiscovery = CSIPClientDiscovery::NewL( *this, KServerUid3 );
    }

// ----------------------------------------------------------------------------
// CMceClientResolver::CMceClientResolver
// ----------------------------------------------------------------------------
//
CMceClientResolver::CMceClientResolver( MMceClientResolverObserver& aRequestHandler )
: iRequestHandler( aRequestHandler ),
  iList( CMceClientResolver::CMceRoutingRequest::LinkOffset() )
    {
    }

// ----------------------------------------------------------------------------
// CMceClientResolver::~CMceClientResolver
// ----------------------------------------------------------------------------
//
CMceClientResolver::~CMceClientResolver ()
    {
	TSglQueIter<CMceClientResolver::CMceRoutingRequest> iter( iList );
	for (CMceClientResolver::CMceRoutingRequest* request = iter++; request; request = iter++)
		{
		iList.Remove( *request );
		delete request;
    	}
    	
    delete iDiscovery;
    
    }


// ----------------------------------------------------------------------------
// CMceClientResolver::RegisterClientL
// ----------------------------------------------------------------------------
//
void CMceClientResolver::RegisterClientL( TUid aClientUid )
    {
    iDiscovery->RegisterL( aClientUid );
    }

// ----------------------------------------------------------------------------
// CMceClientResolver::RegisterClientL
// ----------------------------------------------------------------------------
//
void CMceClientResolver::DeregisterClient( TUid aClientUid )
    {
    iDiscovery->Deregister( aClientUid );
    }


// ----------------------------------------------------------------------------
// CMceClientResolver::RegisterClientL
// ----------------------------------------------------------------------------
//
void CMceClientResolver::RegisterIncomingRequestL( CSIPServerTransaction* aRequest,
                                                   CMceSipConnection& aConnection )
    {
	const CSIPRequestElements* requestElements =
	    aRequest->RequestElements();
	    
	__ASSERT_ALWAYS( requestElements!=NULL, User::Leave( KErrNotFound ) );

    const CSIPMessageElements& messageElements = 
        requestElements->MessageElements();
    
    // Array does not own added headers
    RPointerArray<CSIPHeaderBase> allHeaders;
    CleanupClosePushL( allHeaders );
    AllHeadersL( *requestElements, allHeaders );
	TUint32 requestId = 
	    iDiscovery->ChannelL( requestElements->Method(),
                              requestElements->RemoteUri().Uri().UriDes(),
                              allHeaders,
                              messageElements.Content(),
                              messageElements.ContentType() );
    CleanupStack::PopAndDestroy( &allHeaders );
    
    CMceClientResolver::CMceRoutingRequest* request =
     new (ELeave) CMceClientResolver::CMceRoutingRequest(requestId, 
                                                         aConnection,
                                                         aRequest);
  	iList.AddLast( *request );
                                     
                                           
    }



// -----------------------------------------------------------------------------
// CMceClientResolver::ChannelResolvedL
// -----------------------------------------------------------------------------
//
void CMceClientResolver::ChannelResolvedL(TUid aChannel, TUint32 aRequestId)
    {
    CSIPServerTransaction* incomingRequest = NULL;
    CMceSipConnection* connection = NULL;
    
    CMceClientResolver::CMceRoutingRequest* request = Find( aRequestId );
    User::LeaveIfNull( request );
    
	incomingRequest = request->Request();
	connection = &request->Connection();
    RemoveAndDelete( request );        

    iRequestHandler.IncomingResolvedRequest( aChannel, *connection, 
                                             incomingRequest );
    
    }


// -----------------------------------------------------------------------------
// CMceClientResolver::ClientNotFoundL
// -----------------------------------------------------------------------------
//
void CMceClientResolver::ClientNotFoundL(TUint32 aRequestId,
        						     TUint aStatusCode, 
        						     RStringF aReasonPhrase, 
        						     RPointerArray<CSIPHeaderBase> aHeaders,
        						     HBufC8* aContent)
    {
    CSIPServerTransaction* incomingRequest = NULL;

    CMceClientResolver::CMceRoutingRequest* request = Find( aRequestId );
    User::LeaveIfNull( request );
    
	incomingRequest = request->Request();
    RemoveAndDelete( request );

    CleanupStack::PushL( incomingRequest );
    
    CSIPResponseElements* response = 
        CSIPResponseElements::NewLC( aStatusCode, aReasonPhrase );
        
    if ( aContent )
        {
        CSIPContentTypeHeader* contenttype = NULL;
        
    	for (TInt i=aHeaders.Count()-1; i >= 0; i--)
    		{
    		CSIPHeaderBase* header = aHeaders[i];
    		if (header->Name() ==  SIPStrings::StringF( SipStrConsts::EContentTypeHeader ) )
    			{
    			contenttype = static_cast<CSIPContentTypeHeader*>(header);
    			aHeaders.Remove(i);
    			}
    		}
        CleanupStack::PushL( contenttype );
        
        response->MessageElements().SetContentL( aContent, contenttype );
        CleanupStack::Pop( contenttype );
        }
        
    response->MessageElements().SetUserHeadersL( aHeaders );
    
    incomingRequest->SendResponseL( response );
	aHeaders.ResetAndDestroy();
	CleanupStack::Pop( response );
	CleanupStack::PopAndDestroy( incomingRequest );
    
    
    }

// -----------------------------------------------------------------------------
// CMceClientResolver::ErrorOccurred
// -----------------------------------------------------------------------------
//
void CMceClientResolver::ErrorOccurred(TInt /*aError*/, TUint32 aRequestId)
    {
    CMceClientResolver::CMceRoutingRequest* request = Find( aRequestId );
    RemoveAndDelete( request );
    }


	
	
// ----------------------------------------------------------------------------
// CMceClientResolver::Find
// ----------------------------------------------------------------------------
//	
CMceClientResolver::CMceRoutingRequest* CMceClientResolver::Find(TUint32 aRequestId)
	{
    CMceClientResolver::CMceRoutingRequest* routingRequest = NULL;
	TSglQueIter<CMceClientResolver::CMceRoutingRequest> iter(iList);
	CMceClientResolver::CMceRoutingRequest* listItem = NULL; 
	for (listItem = iter++; listItem && !routingRequest; listItem = iter++)
		{
        if (listItem->ResolverRequestId() == aRequestId )
            {
            routingRequest = listItem;
            }
		}
	return routingRequest;	
	}

// ----------------------------------------------------------------------------
// CMceClientResolver::RemoveAndDelete
// ----------------------------------------------------------------------------
//	
void CMceClientResolver::RemoveAndDelete( CMceClientResolver::CMceRoutingRequest* aRequest )
	{
	if ( aRequest )
		{
		iList.Remove(*aRequest);
		delete aRequest;		
		}		
	}

// ----------------------------------------------------------------------------
// CMceClientResolver::AllHeadersL
// ----------------------------------------------------------------------------
//
void CMceClientResolver::AllHeadersL( 
    const CSIPRequestElements& aRequestElements, 
    RPointerArray<CSIPHeaderBase>& aAllHeaders )
    {
    const CSIPToHeader* toHeader = aRequestElements.ToHeader();
    if ( toHeader )
        {
        aAllHeaders.AppendL( toHeader );
        }
    const CSIPFromHeader* fromHeader = aRequestElements.FromHeader();
    if ( fromHeader )
        {
        aAllHeaders.AppendL( fromHeader );
        }
    const CSIPCSeqHeader* cseqHeader = aRequestElements.CSeqHeader();
    if( cseqHeader )
        {
        aAllHeaders.AppendL( cseqHeader );
        }
    
    const CSIPMessageElements& messageElements = aRequestElements.MessageElements();
    
    const RPointerArray<CSIPHeaderBase>& userHeaders = 
        messageElements.UserHeaders();
    for ( TInt i = 0; i < userHeaders.Count(); i++ )
        {
        aAllHeaders.AppendL( userHeaders[ i ] );
        }
    }

