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




#ifndef CMCECLIENRESOLVER_H
#define CMCECLIENRESOLVER_H

#include <e32base.h>
#include <sipclientdiscoveryobserver.h>

#include "mcesrv.h"


class CSIPClientDiscovery;
class CSIPServerTransaction;
class CMceSipConnection;
class CSIPRequestElements;
class CSIPHeaderBase;

/**
*  Client resolver observer
*
*  @lib 
*/
class MMceClientResolverObserver
    {
public:

    /**
    *  Called when client discovery subsystem has resolved
    *  the client the transaction should go to
    *
    *  @param aClientUid Client's uid
    *  @param aConnection the connection used
    *  @param aRequest the transaction
    */
    virtual void IncomingResolvedRequest( TUid aClientUid,
                                          CMceSipConnection& aConnection,
                                          CSIPServerTransaction* aRequest ) = 0;
                                    
    
    };



/**
*  Client resolver class
*
*  @lib 
*/
class CMceClientResolver : public CBase,
                           public MSIPClientDiscoveryObserver
    {
    
    
    /**
    *  Routing request class
    *
    *  @lib 
    */
    class CMceRoutingRequest : public CBase
        {
        
    public: // Constructors and destructor

        /**
        *  Constructor
        *
        *  @param aResolverRequestId request id given by the client discovery
        *  @param aConnection the connection 
        *  @param aRequest the request whose information has been given to CD
        */
        CMceRoutingRequest( TUint32 aResolverRequestId,
                            CMceSipConnection& aConnection,
                            CSIPServerTransaction* aRequest );
 
        /**
        *  Destructor
        */
        ~CMceRoutingRequest ();
        
    public: // New functions

        /**
        *  Link offset
        */
    	static TInt LinkOffset();
    	
        /**
        *  Returns request id given by the client discovery
        *  @return request id given by the client discovery
        */
        TUint32 ResolverRequestId() const;
        
        /**
        *  Returns request. Ownership is transferred
        *  @return the request
        */
        CSIPServerTransaction* Request();

        /**
        *  Returns connection.
        *  @return the connection
        */
        CMceSipConnection& Connection() const;
        
    private: // Data
        
       	TSglQueLink iLink;    
        TUint32 iResolverRequestId;
        CSIPServerTransaction* iRequest;
        CMceSipConnection& iConnection;

    	//definitions for unit testing
    	MCESRV_UT_DEFINITIONS
        
        };

    
public: // Contstructors and destructor

    /**
    *  Constructor
    *
    *  @param aRequestHandler who is handling the resolved request
    */
    static CMceClientResolver* NewL( MMceClientResolverObserver& aRequestHandler );

    /**
    *  Constructor
    *
    *  @param aRequestHandler who is handling the resolved request
    */
    static CMceClientResolver* NewLC( MMceClientResolverObserver& aRequestHandler );

    /**
    *  Destructor
    */
    ~CMceClientResolver ();
    
public: // New functions

    /**
    *  Registers client
    *
    * @param aClientUid the client's uid
    */
    void RegisterClientL( TUid aClientUid );

    /**
    *  De-registers client
    *
    * @param aClientUid the client's uid
    */
    void DeregisterClient( TUid aClientUid );
    
    /**
    *  Starts client resolving process for a transaction
    *
    * @param aRequest the transaction. Ownership is transferred
    * @param aConnection the connection 
    */
    void RegisterIncomingRequestL( CSIPServerTransaction* aRequest,
                                   CMceSipConnection& aConnection );
    

public: // from MSIPClientDiscoveryObserver

	/**
	* Channel UID was resolved for the SIP request
	* @param aChannel a channel UID to which the SIP request
	*		 must be routed
	* @param aRequest a request ID
    */
	void ChannelResolvedL(TUid aChannel, TUint32 aRequestId);

	/**
	* The client channel UID was not found.
    * The implementation of this callback should generate 
    * a SIP response with the provided elements and send it to network.
	* @param aRequest a request ID
	* @param aStatusCode contains a SIP response status code
    *        for the SIP response that should be generated.
	* @param aReasonPhrase contains a SIP response reason phrase
    *        for the SIP response that should be generated.
	* @param aHeaders contains well-formed optional SIP headers 
    *        that include additional information why the client 
    *        could not be found. For example Accept-headers that describe
    *        the content-types currently supported. 
    *        The ownership is transferred.
    * @param aContent the body of the response. NULL if not present.
    *        The ownership is transferred.
	*/
	void ClientNotFoundL(TUint32 aRequestId,
							     TUint aStatusCode, 
							     RStringF aReasonPhrase, 
							     RPointerArray<CSIPHeaderBase> aHeaders,
							     HBufC8* aContent);

	/**
	* Error occurred during resolution of the client's channel UID.
    * This callback will also be called if one of the callbacks 
    * ChannelResolvedL or ClientNotFoundL leaves using the leave code.
	* @param aError error code. 
	*/
	void ErrorOccurred(TInt aError, TUint32 aRequestId);



private: // Second phase constructors

    /**
    *  Constructor
    *
    *  @param aRequestHandler who is handling the resolved request
    */
    CMceClientResolver ( MMceClientResolverObserver& aRequestHandler );
    
    /**
    *  Second phase constructor
    *
    */
    void ConstructL ();
    
    CMceClientResolver::CMceRoutingRequest* Find( TUint32 aRequestId );
    void RemoveAndDelete( CMceClientResolver::CMceRoutingRequest* aRequest );
    
    void AllHeadersL( const CSIPRequestElements& aRequestElements, 
                      RPointerArray<CSIPHeaderBase>& aAllHeaders );
    

private: // Data

    CSIPClientDiscovery* iDiscovery;
	MMceClientResolverObserver& iRequestHandler;
    TSglQue<CMceClientResolver::CMceRoutingRequest> iList;

	//definitions for unit testing
	MCESRV_UT_DEFINITIONS
    
    };

#endif // CMCECLIENRESOLVER_H

// End of File
