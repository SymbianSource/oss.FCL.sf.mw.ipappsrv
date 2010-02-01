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




#ifndef CSIPCLIENTDISCOVERY_H
#define CSIPCLIENTDISCOVERY_H

//  INCLUDES
#include <e32base.h>
#include <stringpool.h>
#include "_sipcodecdefs.h"

// FORWARD DECLARATIONS
class MSIPClientDiscoveryObserver;
class CSIPClientDiscoveryImpl;
class CSIPHeaderBase;
class CSIPContentTypeHeader;

// CLASS DECLARATION

/**
* Class for managing connected client's channels.
* This class provides services for Symbian OS servers to determine
* client's channel to which the SIP request must be routed.
*  @lib siprsvclient.lib
*/

class CSIPClientDiscovery : public CBase
	{
    public:  // Constructors and destructor
    /**
	* Creates an object representing the session with the server.
	* @param aObserver an observer for receiving events about 
    *        resolved client's communication channel UIDs
	* @param aSelf the UID of the server itself 
    *        that will handle multiple simultaneous clients
	*/
	IMPORT_C static CSIPClientDiscovery* NewL (
	                            MSIPClientDiscoveryObserver& aObserver,
	                            TUid aSelf);

	/**
	* Creates an object representing the session with the server
    * and pushes it to CleanupStack.
	* @param aObserver an observer for receiving events about 
    *        resolved client's communication channel UIDs
    * @param aSelf the UID of the server itself that will handle
    *        multiple simultaneous clients
	*/
	IMPORT_C static CSIPClientDiscovery* NewLC(
	                            MSIPClientDiscoveryObserver& aObserver,
	                            TUid aSelf);

	/**
	* Destructor.
	*/
	IMPORT_C ~CSIPClientDiscovery();

    public: // New functions
	/**
	* Registers handled client
	* @param aChannel the communication channel UID of the connected client
	*/
	IMPORT_C void RegisterL(TUid aChannel);

	/**
	* Deregisters handled client
	* @param aChannel the communication channel UID of the connected client
	* @return KErrNone if successful, KErrNotFound if not found
	*/
	IMPORT_C TInt Deregister(TUid aChannel);

	/**
	* Request to provide the communication channel for the SIP request
	* received outside the SIP dialog
	* @param aRequest the method of the SIP request
    * @param aRequestUri the request-URI of the SIP request as text
    * @param aHeaders all the headers in the SIP request
    * @param aContent SIP request body if present. 
    *        If not present a zero-length descriptor.
    * @param aContentType the content-type of the SIP request body if present.
    *        If not present a NULL pointer. The ownership is not transferred.
	* @return a request ID
	*/
	IMPORT_C TUint32 ChannelL(RStringF aRequestMethod,
                            const TDesC8& aRequestUri,
                            const RPointerArray<CSIPHeaderBase>& aHeaders,
                            const TDesC8& aContent,
                            const CSIPContentTypeHeader* aContentType=0);

	/**
	* Request to provide the communication channel for the SIP request
	* received outside the SIP dialog
	* @param aResolver a specialized ECOM resolver to be applied for resolving
	*		 target client 
	* @param aRequest the method of the SIP request
    * @param aRequestUri the request-URI of the SIP request as text
    * @param aHeaders all the headers in the SIP request
    * @param aContent SIP request body if present. 
    *        If not present a zero-length descriptor.
    * @param aContentType the content-type of the SIP request body if present.
    *        If not present a NULL pointer. The ownership is not transferred.
    * @return a request ID
	*/
	IMPORT_C TUint32 ChannelL(TUid aResolver,
                            RStringF aRequestMethod,
                            const TDesC8& aRequestUri,
                            const RPointerArray<CSIPHeaderBase>& aHeaders,
                            const TDesC8& aContent,
                            const CSIPContentTypeHeader* aContentType=0);
		
	/**
	* Cancels a particular outstanding request
	* @param aRequestId a request ID to be canceled
	*/
	IMPORT_C void Cancel(TUint32 aRequestId);
		
	/**
	* Cancels all outstanding requests
	*/
	IMPORT_C void CancelAll();
	
	
	static TInt ChannelResolved( TAny* aThisInstance );
		
private:
	void ConstructL (MSIPClientDiscoveryObserver& aObserver, TUid aSelf);
	
public:
	
	//data
    MSIPClientDiscoveryObserver* iObserver;
    TUid iChannel;
    TUint32 iRequestId;
    
    

	};

#endif // CSIPCLIENTDISCOVERY_H

// End of File
