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




#ifndef CMCECSSESSION_H
#define CMCECSSESSION_H

#include "mcesrv.h"
#include "mceclientserver.h"

#define MCE_SESSION_ID( session ) reinterpret_cast<TUint32>( session )
#define ALLOW_NOT_SUPPORTED_ITC


//  FORWARD DECLARATION
class CMceServerCore;
class CMceCsSessionReceiver;
class CMceCsSubSessionReceiver;
class CMceCsSubSession;
class CMceCsServerITC;
class TMceIds;
class CMceCsSessionImplementation;


// CLASS DECLARATION

/**
* MCE CS session. This class implements Symbian
* Client-Server session
*
*  @lib 
*  @since
*/
class CMceCsSession : public CSession2
	{
	
public: // Constructors and destructor

	/**
	* Two-phased constructor.
	* @param aServerCore reference to server core.
	* @return CS session
	*/
	static CMceCsSession* NewL( CMceServerCore& aServerCore );

	/**
	* Two-phased constructor.
	* @param aServerCore, reference to server core.
	* @return CS session
	*/
	static CMceCsSession* NewLC( CMceServerCore& aServerCore );

	/**
	* Destructor.
	*/
	~CMceCsSession();

public: // CSession2

	/**
	* Provides services of MCE server
	* @param aMessage, message from the client.
	*/
	void ServiceL( const RMessage2& aMessage );

public: // New

	/**
	* Gets reference to server core
	* @return reference to server core
	*/
	CMceServerCore& ServerCore();

	/**
	* Gets UID of the client
	* @return client UID
	*/
	TUid Uid();

	/**
	* Creates subsession receiver
	* @return subsession receiver
	*/
    CMceCsSubSessionReceiver* CreateSubSessionReceiverL();
    
	/**
	* Returns implementation 
	* @return implementation
	*/
    CMceCsSessionImplementation& Implementation();
    
	/**
	* Gets pointer to receiver based on iManagerType of ids
    * @param aIds set of ids defining ITC callback context
	* @return session receiver
	*/
	CMceCsSessionReceiver* ReceiverL( TMceIds& aIds );
    
    
private:

	CMceCsSession (CMceServerCore& aServerCore);
	void ConstructL();
	CMceCsServerITC& ITC();
	void DoServiceL (TInt aFunction, const RMessage2& aMessage);
	void ClientReadyToReceiveL(TMceIds& aIds, const RMessage2& aMessage);
	void CancelClientReceiveL( TMceIds& aIds );
	void SupportedCodecsL(TInt aFunction, const RMessage2& aMessage);
	void SetClientUidL(TMceIds& aIds, const RMessage2& aMessage);
	void CreateSubSessionL(TMceIds& aIds, const RMessage2& aMessage);
	void DeleteSubSessionL(TMceIds& aIds, const RMessage2& aMessage);
	void ReceiveL (TMceIds& aIds, const RMessage2& aMessage);
	void CallSubSessionL(TMceIds& aIds, TInt aFunction, const RMessage2& aMessage);
    void StoreDefaultHeadersL( const RMessage2& aMessage );		
    void DefaultHeadersL( const RMessage2& aMessage );
    void MediaServiceL( TMceIds& aIds, TMceItcFunctions aFunction, const RMessage2& aMessage );
    
    		
    
private: //Owned data

    //client's UID
	TUid  iClientUid;
	//server ITC
	CMceCsServerITC* iITC;
	//session receivers (one per client)
	RPointerArray<CMceCsSessionReceiver> iReceivers;
    //session implementation
    CMceCsSessionImplementation* iImpl;

private: //NOT owned data

    //MCE server core
	CMceServerCore& iServerCore;
    
    
	//definitions for unit testing
	MCESRV_UT_DEFINITIONS
	
	};

#endif

// End of File
