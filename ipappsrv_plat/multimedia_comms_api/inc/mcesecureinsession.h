/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef CMCESECUREINSESSION_H
#define CMCESECUREINSESSION_H

//  INCLUDES
#include <e32std.h>
#include <mceinsession.h>
#include <mcesecuresession.h>

// CLASS DECLARATION

/**
* Class for mobile terminated secure MCE sessions.
*
* CMceSecureInSession represents incoming invitation to join in a secure session.
*
* The characteristics of the session are defined by remote terminal. Theses charactereistics
* are defined using SDP and are turned by MCE server into structure of media streams attached 
* into the session. The user of this class is then responsible to either reject accept
* the invitation.
*
* Accepting the invitation is executed in two steps. First after receiving the invitation
* the user must complete the media structure. Streams attached to the session are not complete,
* but they must be completed by the user. Those componens defined in SDP are put automatically
* into media structure (like streams and RTP sinks and sources), but other relations must
* be completed by user. When media structure is complete, UpdateL method is called and session
* establishement continues with second step.
*
* After all media resources are succesfully negotiated and reserved, session state is changed
* to Proceeding, and end used can be alarmed about incoming invitation. Typically, at this state
* 180 Ringing response is sent to network. If end user accepts the invitation, 200 OK is
* sent to network and session is established.
*
* After the session has been succesfylly established, it can be later updated and
* must be finally terminated.
*
* This class is never instantiated by user, but the ownership is in any case transferred
* to it. See class MMceInSessionObserver.
*
* If for any reason instance of this class is deleted while in established state,
* session is automatically terminated by MCE server.
*
*  @lib mceclient.lib
*/
class CMceSecureInSession : public CMceInSession, public MMceSecureSession
	{
public: // Constructors and destructor

	/**
	* Destructor 
	*/
	IMPORT_C ~CMceSecureInSession();

public: // Functions

	/**
	* Returns the type of the session.
	* @return The type of the session.
	*/
	virtual TMceSessionType Type() const;

    /*
    * Get the supported crypto contexts of the terminal.
    * @return supported crypto contexts.
    */	    
	virtual const RArray<TMceCryptoContext>& SupportedCryptoContextsL() const;
		             
    /*
    * Get the used crypto contexts in the session.
    * @return used crypto contexts.
    */	    
	virtual const RArray<TMceCryptoContext>& CryptoContexts() const;

    /*
    * Set the used crypto contexts in the session.
	* @pre State() == CMceSession::EIdle ||
	*      State() == CMceSession::EIncoming ||
	*      State() == CMceSession::EEstablished
    * @param aArray Array of crypto contexts to be used in session.
    */	    
	virtual void SetCryptoContextsL( const RArray<TMceCryptoContext>& aArray );

public: // serialization

	/**
	* Two-phased constructor.
	* @param aReadStream, read stream
	* @param aManager, reference to session manager.
	* @param aProfileId, id of the sip profile used for the session.
	* @param aUpdateSession, boolean defining whether new session
	*        is created becuase of updating
	*/
	static CMceSecureInSession* NewL( RReadStream& aReadStream,
				          CMceManager& aManager,
					  TUint32 aProfileId,
					  TBool aUpdateSession = EFalse );
									  
	/**
	* Two-phased constructor.
	* For temporay use only.
	*/
	static CMceSecureInSession* NewL();

	/**
	* C++ default constructor.
	* @param aManager, reference to session manager.
	* @param aProfileId, id of the sip profile used for the session.
	*/
	CMceSecureInSession( CMceManager* aManager, TUint32 aProfileId );
		
	/**
	* Two-phased constructor.
	* @param aSerCtx, context containing read stream	    
	*/
	void InitializeInSessionL( MMceComSerializationContext& aSerCtx,
			           TBool aUpdateSession );
		
private: //methods

	/**
	* second-phase constructor
	* @param aOriginator, the originator
	* @param aRecipient, the recipient
	*/
	void ConstructL( const TDesC8& aOriginator, const TDesC8& aRecipient );
		

private: //data

	RArray<TMceCryptoContext> iTemp;
	
	MCE_UNIT_TEST_DEFS

	};


	#endif 