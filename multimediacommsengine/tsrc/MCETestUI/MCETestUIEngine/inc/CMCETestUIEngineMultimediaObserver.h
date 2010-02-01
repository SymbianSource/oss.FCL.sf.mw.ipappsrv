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



#ifndef __CMCETESTUIENGINEMULTIMEDIAOBSERVER_H__
#define __CMCETESTUIENGINEMULTIMEDIAOBSERVER_H__


// INCLUDES

#include <mcesessionObserver.h>
#include <mcetransactionobserver.h> 
#include <mceintransactionobserver.h> 
#include <MceInSessionObserver.h>
#include <MceStreamObserver.h>
#include <MceRtpObserver.h>
#include <MceEventObserver.h>
#include <MceInEventObserver.h>
#include <MceReferObserver.h>
#include <MceInReferObserver.h>
#include <MceFCMsgObserver.h>
#include <MceMediaStream.h>
#include "MCEPoCOmaCsStateObserver.h"
#include "MCEdtmfObserver.h"

// FORWARD DECLARATIONS
class CMCETestUIEngine;
class TMceTransactionDataContainer;	
class CMceMediaStream;
class CMCERtpSource;

// CLASS DEFINITION
/**
 * CMCETestUIEngineMultimediaObserver observes all events defined in 
 * multimedia API and either:
 * 
 * 1. Passes these via CMCETetsUIEngine to user interface.
 * 2. Creates and executes one or more commands.
 * 3. Both. 
 */
class CMCETestUIEngineMultimediaObserver
	: public CBase,
	  public MMceSessionObserver,
	  public MMceTransactionObserver, 
	  public MMceInTransactionObserver, 
	  public MMceInSessionObserver,
	  public MMceStreamObserver,
      public MMceRtpObserver,
      public MMceEventObserver,
      public MMceInEventObserver,
      public MMceReferObserver,
      public MMceInReferObserver,
      public MMceFcMsgObserver,  
      public MMcePoCOmaCSStateObserver,
      public MMceDtmfObserver   
	{
	
	public:	// Constructors and destructor

		/**
		 * Static constructor.
		 */
		static CMCETestUIEngineMultimediaObserver* NewL( CMCETestUIEngine& aEngine );

    	/**
    	* Destructor.
    	*/
		~CMCETestUIEngineMultimediaObserver();

	private:

		/**
		 * C++ first phase constructor.
		 */
		CMCETestUIEngineMultimediaObserver( CMCETestUIEngine& aEngine );

        /**
        * C++ default constructor. Not implemented.
        */
		CMCETestUIEngineMultimediaObserver();

       	/**
        * Symbian second-phase constructor
        */
		void ConstructL();

      	/**
        * Utility Function for Debugging, Prints info contained in container to console.
        */
		
		void LogContainer( TMceTransactionDataContainer* aContainer,
			TMceTransactionId aTransactionId ) const;
			
			
    public: // from MMCEInSessionObserver

        void IncomingSession(
	                CMceInSession* aSession,
    				TMceTransactionDataContainer* aContainer );

        void IncomingUpdate(
					CMceSession& aOrigSession, 
					CMceInSession* aUpdatedSession,
    				TMceTransactionDataContainer* aContainer );

	public: // fron MMCESessionObserver
	
		void SessionStateChanged(
                    CMceSession& aSession,
                    TMceTransactionDataContainer* aContainer );
		
		void SessionConnectionStateChanged(
                    CMceSession& aSession,
                    TBool aActive);
                    
        void Failed( CMceSession& aSession, TInt aError );
        
        void UpdateFailed(                     
                    CMceSession& aSession,
                    TMceTransactionDataContainer* aContainer );
    
    
    public: // fron MMCETransactionObserver 
	
		void TransactionResponse(
	    			CMceSession& aSession,
	    			TMceTransactionId aTransactionId,
	    			TMceTransactionDataContainer* aContainer );
	    
	  	void TransactionFailed( 
                    CMceSession& aSession,
	    			TMceTransactionId aTransactionId, 
	    			TInt aError );
	    			
	    			
	    			
	public: // fron MMCEInTransactionObserver 
	
	
		void IncomingRequest(
        			const TDesC8& aMethod,
					CMceSession& aSession,
	    			TMceTransactionId aTransactionId,
	    			TMceTransactionDataContainer* aContainer );
				    			
	    			
	public: // from MMCEStreamObserver
 
 	   	void StreamStateChanged( 
                   CMceMediaStream& aStream);

		void StreamStateChanged( 
					CMceMediaStream& aStream, 
					CMceMediaSink& aSink);
    
  	  	void StreamStateChanged(
                    CMceMediaStream& aStream,
                    CMceMediaSource& aSource);       	
        
	public: // from MMCERtpObserver

        void SRReceived(
				    CMceSession& aStream,
					CMceMediaStream& aSource);

        void RRReceived(
					CMceSession& aStream,
					CMceMediaStream& aSource);     
		
        void InactivityTimeout(
					CMceMediaStream& aStream,
					CMceRtpSource& aSource);
					 
				void Activity(
					CMceMediaStream& aStream,
					CMceRtpSource& aSource) ;
					
				void SsrcAdded(
					CMceMediaStream& aStream,
					CMceRtpSource& aSource,
					TUint aSsrc ) ;
	    
				void SsrcRemoved(
					CMceMediaStream& aStream,
					CMceRtpSource& aSource,
					TUint aSsrc );


					
	public: // from MMCEInEventObserver
	
	    void IncomingEvent(
					CMceInEvent* aEvent,
    				TMceTransactionDataContainer* aContainer );
    
    				
    public: // from MMCEEventObserver
    
  		void EventStateChanged(
        			CMceEvent& aEvent,
    				TMceTransactionDataContainer* aContainer );

		void NotifyReceived(
					CMceEvent& aEvent,
    				TMceTransactionDataContainer* aContainer );
					
					
	    void EventConnectionStateChanged(
	                CMceEvent& aEvent,
	                TBool aActive ) ;
        
        void Failed(CMceEvent& aEvent, TInt aError );
        
    public: // from MMCEReferObserver    
    	
    	void ReferStateChanged(
        			CMceRefer& aRefer,
    				TMceTransactionDataContainer* aContainer ) ;
    				
        void ReferConnectionStateChanged(
	                CMceRefer& aRefer,
	                TBool aActive );
	                
	    void Failed(CMceRefer& aRefer, TInt aError );      
	    
	public: // from MMCEInReferObserver       
	
		void IncomingRefer(
					CMceInRefer* aRefer,
					const TDesC8& aReferTo,
    				TMceTransactionDataContainer* aContainer );
    				
   	public: // from MMCEFCMsgObserver
   	
   	   void FCMsgReceived(CMceSession& aSession,
    					HBufC8* aMessage);

       void FCMsgErrorOccurred(CMceSession& aSession,
    					TInt aError);   				

    public: // from MMcePoCOmaCSStateObserver
    
        void CsStatusChangedL( TPoCOmaCsState aStatus );
       
    public: // from MMcedtmfObserver
    
    void  DtmfToneReceived( CMceSession& aSession,
                               CMceAudioStream& aStream,
                               const TChar& aTone );
    
	void DtmfEventReceived( CMceSession& aSession,
	                                CMceAudioStream& aStream,
	                                CMceMediaSource& aSource,
	                                TMceDtmfEvent aEvent );
	   
	void  DtmfErrorOccured( CMceSession& aSession,
		                               CMceAudioStream& aStream,
		                               CMceMediaSource& aSource,
		                               TInt aError );
	    

	private: // Not owned data

		CMCETestUIEngine& iEngine;
		   

	};

#endif // __CMCETESTUIENGINEMULTIMEDIAOBSERVER_H__
