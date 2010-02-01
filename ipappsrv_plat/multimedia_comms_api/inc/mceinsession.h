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




#ifndef CMCEINSESSION_H
#define CMCEINSESSION_H

//  INCLUDES
#include <e32std.h>
#include <mcesession.h>

// CLASS DECLARATION

/**
* Class for mobile terminated MCE sessions.
*
* CMceInSession represents incoming invitation to join in a session.
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
class CMceInSession : public CMceSession
	{

	public: // Constructors and destructor

	    /**
	    * Destructor 
	    */
		IMPORT_C ~CMceInSession();

	public: // from CMceSession

		virtual TBool
			StreamInitializeCondition( CMceMediaStream& aMediaStream ) const;

	public: // Functions

	    /**
	    * Indicates to the server side, that 180(Ringing) response for the 
	    * incoming INVITE request can be sent. Does not establish session.
	    * @pre State() == CMceSession::EProceeding
	    * @param aHeaders SIP headers to be added. Ownership is
		*		 transferred.
		* @param aContentType Type of content. Ownership
		*		 is transferred.
	    * @param aContent Content to be added in body. Ownership is 
		*		 transferred.

	    */
		IMPORT_C void RingL( CDesC8Array* aHeaders = 0,
					 		 HBufC8* aContentType = 0,
					 		 HBufC8* aContent = 0 );

        /**
	    * Sends respond to an incoming session.
	    * @pre State() == CMceSession::EProceeding && ( 100 <= aCode <= 200 )
	    * @param aReason, reason phrase
	    * @param aCode, reason code
	    * @param aHeaders, SIP headers to be added into respond. Ownership is
		*		 transferred.
		* @param aContentType, a type of content. Ownership
		*		 is transferred.
	    * @param aContent, content to be added in body. Ownership is 
		*		 transferred.
	    */	    	    	    
	    IMPORT_C void RespondL( const TDesC8& aReason,
               	                TUint32 aCode,
            	                CDesC8Array* aHeaders = 0,
                    		    HBufC8* aContentType = 0,
                    		    HBufC8* aContent = 0 );
		
		
	    /**
	    * Answers to an incoming session. Indicates to server side that the 
	    * 200(OK) response for the incoming INVITE has to be sent.
	    * @pre State() == CMceSession::EProceeding
	    * @post State() == CMceSession::EAnswering	    
	    */
		IMPORT_C void AcceptL();

        /**
	    * Answers to an incoming session. Indicates to server side that the 
	    * given response for the incoming INVITE has to be sent.
	    * @pre State() == CMceSession::EProceeding && ( 200 <= aCode <= 300 )
	    * @post State() == CMceSession::EAnswering	    
	    * @param aReason Reason phrase
	    * @param aCode Reason code
	    * @param aHeaders SIP headers to be added. Ownership is
		*		 transferred.
		* @param aContentType Type of content. Ownership
		*		 is transferred.
	    * @param aContent Content to be added in body. Ownership is 
		*		 transferred.
	    */
		IMPORT_C void AcceptL(  const TDesC8& aReason,
	           			        TUint32 aCode,
	           			        CDesC8Array* aHeaders = 0,
					 		    HBufC8* aContentType = 0,
					 		    HBufC8* aContent = 0 );

	    /**
	    * Rejects the incoming session. Indicates to server side that 
	    * 603(Decline) response has to be sent.
	    * @pre State() == CMceSession::EIncoming ||
	    *      State() == CMceSession::EProceeding
	    * @post State() == CMceSession::ETerminated	    
	    */
	    IMPORT_C void RejectL();
        
        /**
	    * Rejects the incoming session. Indicates to server side that 
	    * 603(Decline) response has to be sent.
	    * @pre ( State() == CMceSession::EIncoming ||
	    *        State() == CMceSession::EProceeding ) &&
	    *        ( 300 <= aCode < 700 )
	    * @post State() == CMceSession::ETerminated	    
	    * @param aReason Reason phrase
	    * @param aCode Reason code
	    * @param aHeaders SIP headers to be added. Ownership is
		*		 transferred.
		* @param aContentType Type of content. Ownership
		*		 is transferred.
	    * @param aContent Content to be added in body. Ownership is 
		*		 transferred.
	    */
	    IMPORT_C void RejectL(  const TDesC8& aReason,
	           			        TUint32 aCode,
	           			        CDesC8Array* aHeaders = 0,
					 		    HBufC8* aContentType = 0,
					 		    HBufC8* aContent = 0 );

	public: // serialization

	    /**
	    * Two-phased constructor.
	    * @param aReadStream  Read stream used.
	    * @param aManager Reference to session manager.
	    * @param aProfileId Id of the sip profile used for the session.
	    * @param aUpdateSession Boolean defining whether new session
	    *        is created becuase of updating
	    */
		static CMceInSession* NewL( RReadStream& aReadStream,
									CMceManager& aManager,
									TUint32 aProfileId,
									TBool aUpdateSession = EFalse );
									  
	    /**
	    * Two-phased constructor.
	    * For temporay use only.
	    */
		static CMceInSession* NewL();

	    /**
	     * C++ default constructor.
	     * @param aManager Reference to session manager.
	     * @param aProfileId Id of the sip profile used for the session.
	     */
		CMceInSession( CMceManager* aManager, TUint32 aProfileId );
		
	   
		
	protected://methods

	 	/**
	    * Two-phased constructor.
	    * @param aSerCtx Context containing read stream	    
	    * @param aUpdateSession Boolean defining whether new session
	    *        is created becuase of updating
	    */
		void InitializeInSessionL(
			MMceComSerializationContext& aSerCtx,
			TBool aUpdateSession );
		
	protected://methods

	    /**
	     * second-phase constructor
	     * @param aOriginator The originator
	     * @param aRecipient The recipient
	     */
	    void ConstructL( const TDesC8& aOriginator, const TDesC8& aRecipient );
		
		};


	#endif 