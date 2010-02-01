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
* Description:    See class definition below.
*
*/



#ifndef __CTcMCEReceived_H__
#define __CTcMCEReceived_H__

// INCLUDES
#include <e32base.h>
#include <badesca.h>
#include <MCESession.h>
#include <MCEEvent.h>
#include <MCERefer.h>
#include "MCEConstants.h"
#include "MCEMediaStream.h"
// FORWARD DECLARATIONS
class CMceSession;
class CMceInSession;
class CMceEvent;
class CMceInEvent;
class CMceRefer;
class CMceInRefer;
class CMceMediaStream;
class CMceMediaSource;
class CMceMediaSink;
class CMceRtpSource;
class TMceTransactionDataContainer;

// CLASS DEFINITION
/**
 * CTcMCEReceived implements a container class for MCE notification messages.
 * Objects of this class are created and stored in MMCEManagerObserver,
 * MMceSessionObserver, MMceStreamObserver, MMceRtpObserver, MMceFcMsgObserver,
 * MMceEventObserver, MMceReferObserver and MMCESubscribeObserver observer 
 * functions.
 */
class CTcMCEReceived: public CBase
	{
	public:	// Constructors and destructor

		/// Default constructor
		CTcMCEReceived();

		/// Destructor
		~CTcMCEReceived();

	public: // New methods

		
		void SetSession( const CMceSession* aSession );
		inline const CMceSession* Session() const
				{ return iSession; }

		void SetMediaStream( const CMceMediaStream* aMediaStream );
		inline const CMceMediaStream* MediaStream() const
				{ return iMediaStream; }
				
		void SetMediaSource( const CMceMediaSource* aMediaSource );
		inline const CMceMediaSource* MediaSource() const
				{ return iMediaSource; }		
				
		void SetMediaSink( const CMceMediaSink* aMediaSink );
		inline const CMceMediaSink* MediaSink() const
				{ return iMediaSink; }	

		void SetRtpSource( const CMceRtpSource* aRtpSource );
		inline const CMceRtpSource* RtpSource() const
				{ return iRtpSource; }	

		void SetInSession( CMceInSession* aInSession );
		inline CMceInSession* InSession()
				{return iInSession;};
		void SetIncomingUpdateSession(CMceSession& aOrigSession,
									CMceInSession* aInSession );
	
		void SetHeaders( CDesC8Array* aHeaders );
		inline const CDesC8Array* Headers() const
				{return iHeaders;};
		
		void SetContentType( HBufC8* aContentType );
		inline const HBufC8* ContentType() const
				{return iContentType;};
	
					
		void SetBody( HBufC8* aBody);
		inline const HBufC8* Body() const
			{return iBody;};
	
		
		void SetStatusCode( TInt32 aStatusCode );
		inline TInt StatusCode() const
			{return iStatusCode;};
	
		
		void SetReasonPhrase( HBufC8* aReasonPhrase);
		inline const HBufC8* ReasonPhrase() const
				{return iReasonPhrase;};
	
	
		void SetFCMessage( HBufC8* aFCMessage);
		inline const HBufC8* FCMessage() const
				{return iFCMessage;};
	
	
		void SetEvent( const CMceEvent* aEvent );
		inline const CMceEvent* Event() const
				{return iEvent;};

				
		void SetInEvent( CMceInEvent* aInEvent );	
		inline const CMceInEvent* InEvent() const
				{return iInEvent;};


        void SetRefer( CMceRefer* aRefer );
        inline const CMceRefer* Refer() const
                {return iRefer;};

        
        void SetInRefer( CMceInRefer* aInRefer );
        inline const CMceInRefer* InRefer() const
                {return iInRefer;};                           
        

		void SetReferTo( const TDesC8& aReferTo );
		inline const TDesC8& ReferTo()
				{return iReferTo;};
		
		void SetSessionState( CMceSession::TState aState );
		inline const CMceSession::TState SessionState()
				{return iSessionState;};

		void SetEventState( CMceEvent::TState aState );
		inline const CMceEvent::TState EventState()
				{return iEventState;};	
		

		void SetReferState(CMceRefer::TState aState );
		inline const CMceRefer::TState ReferState()
				{return iReferState;};	

		void SetStreamState(CMceMediaStream::TState aState );
		inline const CMceMediaStream::TState StreamState()
				{return iStreamState;};	
				
				
		/**
		 * Set error code
		 *
		 * @param aError System-wide or SIP specific error code
		 */

		void SetError( TInt aError );
		inline TInt Error()
				{return iError;};

		/**
		* Set type or received callback
		*/
		inline void SetType(TTcMceCallbackType aCallbackType)
			{iCallbackType = aCallbackType;}
			
		inline TTcMceCallbackType Type()
			{return iCallbackType;}

		void SetMethodL( const TDesC8& aMethod );
		inline const HBufC8* Method() const
			{
			return iMethod;
			}

		void SetTransactionId( TMceTransactionId aTransactionId );
		inline const TMceTransactionId TransactionId() const
			{
			return iTransactionId;
			}

    public: // helper function
    
        void SetTransactionData( TMceTransactionDataContainer* aContainer );

	private: // Owned data

		/// MCE Incoming session pointer. Owned.
		CMceInSession* iInSession;

		/// Headers of statechanged or incoming session. Owned.
        CDesC8Array* iHeaders; 
        
        /// Content type of incoming session. Owned.
        HBufC8* iContentType;
        
        /// Body of statechanged or incoming session. Owned.
        HBufC8* iBody;

        /// Reasonphrase of statechanged session. Owned.
        HBufC8* iReasonPhrase;

        /// Floor control message. Owned.
        HBufC8* iFCMessage;

		/// Incoming event pointer. Owned.
		CMceInEvent* iInEvent;
		
		/// Incoming refer pointer. Owned.
		CMceInRefer* iInRefer;

		/// Method of incoming SIP request. Owned.
        HBufC8* iMethod;

	private: // Referenced data

		/// MCE Session pointer. Not owned.
		const CMceSession* iSession;
		
		/// MCE Media Stream. Not owned.
		const CMceMediaStream* iMediaStream; // Still missing from GetMessage
		
		/// MCE Media Source. Not owned.
        const CMceMediaSource* iMediaSource; // Still missing from GetMessage
       
        /// MCE Media sink. Not owned.
        const CMceMediaSink* iMediaSink; // Still missing from GetMessage
        	
        /// MCE Rtp Source. Not owned.
		const CMceRtpSource* iRtpSource; // Still missing from GetMessage
        
    	/// Status code of the statechanged session.
        TInt32 iStatusCode;
        		
		/// Event pointer. Not owned.
		const CMceEvent* iEvent;

        /// Refer pointer. Not owned.
        const CMceRefer* iRefer;
	
		/// Refer-to-header
		TDesC8 iReferTo;
		
		/// System-wide or SIP specific error code
		TInt iError;
		
		// Type of the received callback
		TTcMceCallbackType iCallbackType;

		// Transaction Id of the received SIP message
		TMceTransactionId iTransactionId;
		
		CMceSession::TState iSessionState;
		CMceEvent::TState iEventState;
		CMceRefer::TState iReferState;
		CMceMediaStream::TState iStreamState;

	};

#endif // __CTcMCEReceived_H__
