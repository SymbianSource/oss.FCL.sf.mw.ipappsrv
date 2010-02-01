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




#ifndef CMCESECUREOUTSESSION_H
#define CMCESECUREOUTSESSION_H

//  INCLUDES
#include <e32std.h> 
#include <mceoutsession.h>
#include <mcesecuresession.h>

// FORWARD DECLARATIONS
class CSIPProfile;
class CMceMediaStream;
class CMceManager;
class CMceEvent;
class CMceRefer;

// CLASS DECLARATION

/**
* Class for mobile originated secure MCE sessions.
*
* CMceSecureOutSession represents outgoing invitation to remote terminal to join in a session.
*
* The characteristics of the session are defined by user of this class by attaching
* media streams into the session. Theses charactereistics are turned by MCE server into SDP
* and attached into sent invitation request.
*
* After the session has been succesfylly established, it can be later updated and
* must be finally terminated.
*
* If for any reason instance of this class is deleted while in established state,
* session is automatically terminated by MCE server.
*
*  @lib mceclient.lib
*/
class CMceSecureOutSession : public CMceOutSession, public MMceSecureSession
	{

public: // Constructors and destructor

	/**
	* Two-phased constructor. Should be used when session
	* is established into new logical connection to remote terminal.
	* @param aManager, reference to session manager.
   	* @param aProfile, sip profile used for the session.
	* @param aRecipient, recipient of the session invitation.
	* @param aOriginator, originator of the session invitation.
	*/
	IMPORT_C static CMceSecureOutSession* NewL( CMceManager& aManager,
						    CSIPProfile& aProfile,
						    const TDesC8& aRecipient,
						    HBufC8* aOriginator = 0 );

	/**
	* Two-phased constructor. Should be used when session is associated
	* to existing event. This guarantees that session is established with
	* same remote terminal with the event.
	* @param aEvent, reference to existing event that is associated
	*        with this session.
	*/
	IMPORT_C static CMceSecureOutSession* NewL( CMceEvent& aEvent );
									  
	/**
	* Two-phased constructor. Should be used when session is associated
	* to existing refer. This guarantees that session is established with
	* same remote terminal with the refer.
	* @param aRefer, reference to existing refer that is associated
	*        with this session.
	*/
	IMPORT_C static CMceSecureOutSession* NewL( CMceRefer& aRefer );
	    
	/**
	* Destructor.
	*/
	IMPORT_C ~CMceSecureOutSession();

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
	* For temporary use only.
	*/
	static CMceSecureOutSession* NewL();
									  
private://methods

	/**
	* C++ default constructor.
	* @param aManager, reference to session manager.
	* @param aProfileId, id of the sip profile used for the session.
	*/
	CMceSecureOutSession( CMceManager* aManager, TUint32 aProfileId );

	/**
	* second-phase constructor
	* @param aRecipient, the recipient
	* @param aOriginator, the originator
	*/
	void ConstructL( const TDesC8& aRecipient, HBufC8* aOriginator  );

        /**
	* second-phase constructor
	* @param aRecipient, the recipient
	* @param aOriginator, the originator
	* @param aDialoId, the dialog ID of existing dialog
	*/
	void ConstructL( const TDesC8& aRecipient, 
	                 const TDesC8& aOriginator,
	                 TUint32 aDialogId );

private: //data

	RArray<TMceCryptoContext> iTemp;


	};

#endif
