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



#ifndef __CTcMCEMultimediaAPIObserver_H__
#define __CTcMCEMultimediaAPIObserver_H__


// INCLUDES

#include <MCESessionObserver.h>
#include <MceInSessionObserver.h>
#include <MceStreamObserver.h>
#include <MceRtpObserver.h>
#include <mcetransactionobserver.h>
#include <mceintransactionobserver.h>
#include "CTcMCEReceiveQueue.h"


// FORWARD DECLARATIONS
class CTcMCEContext;
class CTcMCEReceived;
class TMceTransactionDataContainer;	

// CLASS DEFINITION
/**
 * CTcMCEMultimediaAPIObserver observes all events defined in 
 * multimedia API.
 */

class CTcMCEMultimediaAPIObserver
	: public CBase,
	  public MMceSessionObserver,
	  public MMceInSessionObserver,
	  public MMceStreamObserver,
      public MMceRtpObserver,
      public MMceTransactionObserver,
      public MMceInTransactionObserver
	{
	
	public:	// Constructors and destructor

		/**
		 * Static constructor.
		 *
		 * @param aContext Reference to test context object.
		 * @return An initialized instance of this class.
		 */
		static CTcMCEMultimediaAPIObserver* NewL( CTcMCEContext& aContext );

		/// Destructor
		~CTcMCEMultimediaAPIObserver();

	private:

		/**
		 * Constructor.
		 *
		 * @param aContext Reference to test context object.
		 */
		CTcMCEMultimediaAPIObserver( CTcMCEContext& aContext );

		/// Default constructor. Not implemented.
		CTcMCEMultimediaAPIObserver();

		/// 2nd phase constructor
		void ConstructL();

	public: // new functions
	
		/**
		 * Returns the first item off the session receive queue (FIFO). 
		 * If no items are present, the function will wait for aTimeout seconds
		 * and then try fetching again.
		 *
		 * @param aTimeOut Time in seconds waited for items to arrive.
		 */
		CTcMCEReceived& ReceivedSessionItemL( TInt aTimeout );
		
		CTcMCEReceived& ReceivedUpdatedSessionItemL( TInt aTimeout );

		/**
		 * Returns the first item off the Stream receive queue (FIFO). 
		 * If no items are present, the function will wait for aTimeout seconds
		 * and then try fetching again.
		 *
		 * @param aTimeOut Time in seconds waited for items to arrive.
		 */
		CTcMCEReceived& ReceivedStreamItemL( TInt aTimeout );

		/**
		 * Returns the first item off the RTP receive queue (FIFO). 
		 * If no items are present, the function will wait for aTimeout seconds
		 * and then try fetching again.
		 *
		 * @param aTimeOut Time in seconds waited for items to arrive.
		 */
		CTcMCEReceived& ReceivedRTPItemL( TInt aTimeout );	

		
    public: // from MMceInSessionObserver

        void IncomingSession(
	                CMceInSession* aSession,
    				TMceTransactionDataContainer* aContainer );
    	void IncomingUpdate(
					CMceSession& aOrigSession, 
					CMceInSession* aUpdatedSession,
    				TMceTransactionDataContainer* aContainer ) ;

	public: // fron MMceSessionObserver
	
		void SessionStateChanged(
                    CMceSession& aSession,
                    TMceTransactionDataContainer* aContainer );
		
		void SessionConnectionStateChanged(
                    CMceSession& aSession,
                    TBool aActive);
                    
        void Failed( CMceSession& aSession, TInt aError );
        
		void UpdateFailed(
	    			CMceSession& aSession,
	    			TMceTransactionDataContainer* aContainer ) ;
	    			
	public: // from MMceStreamObserver
 
 	   	void StreamStateChanged( 
                   CMceMediaStream& aStream);

		void StreamStateChanged( 
					CMceMediaStream& aStream, 
					CMceMediaSink& aSink);
    
  	  	void StreamStateChanged(
                    CMceMediaStream& aStream,
                    CMceMediaSource& aSource);       	
        
	public: // from MMceRtpObserver

        void SRReceived(
        			CMceSession& aSession,
				    CMceMediaStream& aStream);

        void RRReceived(
        			CMceSession& aSession,
				    CMceMediaStream& aStream);
		
        void InactivityTimeout(
					CMceMediaStream& aStream,
					CMceRtpSource& aSource); 
					
        void SsrcAdded( 
        			CMceMediaStream& aStream,
        			CMceRtpSource& aSource,
        			TUint aSsrc );

        void SsrcRemoved( 
        			CMceMediaStream& aStream,
        			CMceRtpSource& aSource,
        			TUint aSsrc );

	public: // from MMceTransactionObserver

		void TransactionResponse(
	    			CMceSession& aSession,
	    			TMceTransactionId aTransactionId,
	    			TMceTransactionDataContainer* aContainer );

		void TransactionFailed( 
                    CMceSession& aSession,
	    			TMceTransactionId aTransactionId, 
	    			TInt aError );

    public: // from MMceInTransactionObserver

    	void IncomingRequest(
        			const TDesC8& aMethod,
					CMceSession& aSession,
	    			TMceTransactionId aTransactionId,
	    			TMceTransactionDataContainer* aContainer );

	private: // data

		/// Reference to the test context. Not owned.
		CTcMCEContext& iContext;
		
		CTcMCEReceiveQueue* iSessionQueue;
		CTcMCEReceiveQueue* iUpdatedSessionQueue;
		CTcMCEReceiveQueue* iStreamQueue;
		CTcMCEReceiveQueue* iRTPQueue;

	};

#endif // __CTcMCEMultimediaAPIObserver_H__
