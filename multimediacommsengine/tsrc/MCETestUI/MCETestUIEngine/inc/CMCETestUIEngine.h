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

#ifndef CMCETESTUIENGINE_H
#define CMCETESTUIENGINE_H

// SYSTEM INCLUDES
#include <e32base.h>
#include <es_sock.h>
#include <CommDbConnPref.h>
#include <mcetransactiondatacontainer.h>
// USER INCLUDES
#include "MMCETestUIEngineCmdProvider.h"
#include "CMCEPoCOmaCSCallWatcher.h"

// FORWARD DECLARATIONS

class CSIP;
class CMCETestUIEngineMultimediaObserver;
class CMCETestUIEngineProfile;
class CMCETestUIEngineProfileContainer;
class CMCETestUIEngineOutSession;
class CMCETestUIEngineInSession;
class CMCETestUIEngineSipObserver;
class MMCETestUIUserInterface;
class CMceManager;
class CMceFcMsgExchange;
class CMceOutSession;
class CMceInSession;
class CMCETestUIQuestionSingleSelection;
class CMCETestUIQuestionDataQuery;
class TSessionParams;
class CMCETestUIEngineOutEvent;
class CMCETestUIEngineInEvent;
class CMceOutEvent;
class CMceInEvent;
class TEventParams;
class CMCETestUIEngineCodec;
class TSessionEstablishValues;
class TSessionResponseValues;
class TNotifyValues;
class CMceOutRefer;
class CMceInRefer;
class TReferParams;
class CMCEPoCOmaCSCallWatcher;
class CMceRefer;
class CMceTestUIEngineRefer;
class CMCETestUIEngineSession;
class CMCETestUIEngineEvent;
class CMCETestUIEngineOutRefer;
class CMCETestUIEngineInRefer;
class CMCETestUIEngineRefer;
class CMCETestUIEngineVideoCodec;
class CMceMediaStream;
class CMceAudioCodec;
class CMceSession;
class CMceVideoCodec;

const TUint KPoCPriority = 1;
const TUint KVoIPPriority = 2;
const TUint KVSPriority = 3;

// CLASS DECLARATIONS

/**
* 
* CMCETestUIEngine provides wrapper for CMCEManager. 
* Class is not the traditional wrapper in a sense that it would 
* conform to interface provided by CMCEManager. Instead class
* implements interface MMCETestUIEngineCmdProvider to provide
* same kind of functionality in controlled way on different levels 
* of abstraction.
* 
* Class also has container for all available and used profiles,
* observer for Multimedia API and all existing sessions. 
*
* TODO: Support for Events and Refers.
* TODO: Support for Event API and FC API.
* (TODO: Possibly add support for observing RTP traffic)
* 
*/


/**
* Enumerations for sessions
*/
enum TDirection 
    {
    ESendRecv = 0,
    ERecvOnly,
    ESendOnly,
    ENotSpesified,
    };        
enum TVideoDirection 
    {
    EVideoSendRecv = 0,
    EVideoRecvOnly,
    EVideoSendOnly,
    EVideoNotSpesified,
    };


enum TSessionType 
    {
    ECustomSession = 0,
    EPoCSession,
    EVoIPSession,
    EVSSession,
    E2WayVideoSession
    }; 
                         
class TSessionParams
		{
		public:
		
		enum KMceTestUiCodecSetting
            {
            EMceCodecSingle,
            EMceCodecAll,
            EMceCodecNone,
            EMceCodecMultipleModes
            };
    
         enum KMceTestUiCameraSetting
            {
            EMceCameraPrimary,
            EMceCameraSecondary
            };
         
         enum KMceTestUiViewFinderSetting
            {
            EMceViewFinderDisabled,
            EMceViewFinderEnabled
            };

		 TBuf<300>	iRecipentAddress;
		 TBool iSuppressRTCP;
		 TBuf<100> originator;
		 TInt index;
		 TInt videoIndex;
		 TBool iPoCAdhocSessionType;
		 TInt sourceType;
		 TBuf<100> sourceFile;
		 TBuf<100> sinkFile;
		 TInt audioSinkType;
		 TInt videoSinkType;
		 TInt videoSinkDisplay;
		 TInt videoSinkFile;
		 TInt videoSinkRtp;
    	 TInt iNoPrecondtion;
	     TInt iBundle;
	     TBool iHoldWithAddress;
	     TBool iSecureSession;
	     TInt iServiceType;
	     KMceTestUiCodecSetting iVideoCodec;
	     KMceTestUiCodecSetting iAudioCodec;
         TInt iCameraSetting;
		 
		};
		
 
class CMCETestUIEngine: public CBase,
                         public MMCETestUIEngineCmdProvider
    {
        
    public: // Constructors and destructors


        /**
        * Two-phased constructor.
        */
        IMPORT_C static CMCETestUIEngine* NewL();

    	/**
    	* Destructor.
    	*/
    	IMPORT_C virtual ~CMCETestUIEngine();

    public: // From MMCETestUICommandProvider

        IMPORT_C const RPointerArray<TMCETestUIEngineCmdBase>& 
                    GetCommandsL();
	
    public: // New functions

	    /**
	    * Set observer for user interface events. Ownership is not
	    * transferred.
	    *
	    * @param aUserInterface, observer for user interface events.
	    */
        IMPORT_C void SetUserInterface( 
                            MMCETestUIUserInterface* aUserInterface );

	    /**
	    * Returns available profiles still not in use.
	    *
	    * @return Array of available profiles still not in use
	    */        
        IMPORT_C const RPointerArray<CMCETestUIEngineProfile>& 
                            AvailableProfiles();

	    /**
	    * Returns profiles allready in use.
	    *
	    * @return Array of available profiles allready in use.
	    */        
        IMPORT_C const RPointerArray<CMCETestUIEngineProfile>& 
                            UsedProfiles();

	    /**
	    * Deletes session.
	    *
	    * @param aSession Session to be deleted
	    */     
        IMPORT_C void DeleteSession( CMCETestUIEngineSession& aSession);
        
       

	    /**
	    * Creates new outgoing session wrapper and stores it.
	    *
	    * @return reference to newly created outsession wrapper.
	    */                
        IMPORT_C CMCETestUIEngineOutSession& CreateOutSessionL(
            CMCETestUIEngineProfile& aProfile,
            TDirection aDirection = ENotSpesified,
            TVideoDirection aVideoDirection =EVideoNotSpesified,    
            TSessionType aSessionType = ECustomSession );
        
        /**
	    * Creates new outgoing session wrapper and stores it.
	    *
	    * @return reference to newly created outsession wrapper.
	    */                
        IMPORT_C CMCETestUIEngineOutSession& CreateOutSessionL(
            TUint32 aIAPId,
            TDirection aDirection = ENotSpesified,
            TVideoDirection aVideoDirection = EVideoNotSpesified,    
            TSessionType aSessionType = ECustomSession );
        /**
	    * Creates new outgoing session wrapper and stores it.
	    *
	    * @return reference to newly created outsession wrapper.
	    */             
	     IMPORT_C CMCETestUIEngineOutSession& CreateOutSessionL(
				CMceRefer& aRefer
				);
	    /**
	    * Creates new incoming session wrapper and stores it.
	    *
	    * @return reference to newly created insession wrapper.
	    */      
        IMPORT_C CMCETestUIEngineInSession& CreateInSessionL(
            CMceInSession* aInSession );

	    /**
	    * Returns all existing outsessions. Ownership is not transferred.
	    *
	    * @return reference to array of existing outsessions
	    */      
        IMPORT_C const RPointerArray<CMCETestUIEngineOutSession>& OutSessions();

	    /**
	    * Returns all existing insessions. Ownership is not transferred.
	    *
	    * @return reference to array of existing insessions
	    */      
        IMPORT_C const RPointerArray<CMCETestUIEngineInSession>& InSessions();
        
         /**
	    * Returns all existing insessions. Ownership is not transferred.
	    *
	    * @return reference to array of existing insessions
	    */  
        IMPORT_C void IncomingUpdateL(CMceSession& aOrigSession, 
					CMceInSession* aUpdatedSession);
        /**
	    * Creates new outgoing event wrapper and stores it.
	    *
	    * @return reference to newly created outsession wrapper.
	    */                
        IMPORT_C CMCETestUIEngineOutEvent& CreateOutEventL(
            CMCETestUIEngineProfile& aProfile);
        
        /**
	    * Creates new outgoing event wrapper and stores it.
	    *
	    * @return reference to newly created outsession wrapper.
	    */                
        IMPORT_C CMCETestUIEngineOutEvent& CreateOutEventL(
            CMceOutEvent* aOutEvent);    
         /**
	    * Creates new incoming event wrapper and stores it.
	    *
	    * @return reference to newly created insession wrapper.
	    */      
        IMPORT_C CMCETestUIEngineInEvent& CreateInEventL(
            CMceInEvent* aInEvent );
            
        /**
	    * Returns all existing outevents. Ownership is not transferred.
	    *
	    * @return reference to array of existing outevents
	    */      
        IMPORT_C const RPointerArray<CMCETestUIEngineOutEvent>& OutEvents();

	    /**
	    * Returns all existing inevents. Ownership is not transferred.
	    *
	    * @return reference to array of existing inevents
	    */      
        IMPORT_C const RPointerArray<CMCETestUIEngineInEvent>& InEvents();
        
         /**
	    * Deletes event.
	    *
	    * @param aEvent event to be deleted
	    */     
        IMPORT_C void DeleteEvent( CMCETestUIEngineEvent& aEvent);


		/**
	    * Creates new outgoing event wrapper and stores it.
	    *
	    * @return reference to newly created outsession wrapper.
	    */                
        IMPORT_C CMCETestUIEngineOutRefer& CreateOutReferL(
            CMCETestUIEngineProfile& aProfile);
            
        IMPORT_C CMCETestUIEngineOutRefer& CreateOutReferL(
            CMCETestUIEngineSession& aSession);
            
        IMPORT_C CMCETestUIEngineOutRefer& CreateOutReferL(
            CMCETestUIEngineEvent& aEvent);
            
         /**
	    * Creates new incoming event wrapper and stores it.
	    *
	    * @return reference to newly created insession wrapper.
	    */      
        IMPORT_C CMCETestUIEngineInRefer& CreateInReferL(
            CMceInRefer* aInRefer );
            
        /**
	    * Returns all existing outevents. Ownership is not transferred.
	    *
	    * @return reference to array of existing outevents
	    */      
        IMPORT_C const RPointerArray<CMCETestUIEngineOutRefer>& OutRefers();

	    /**
	    * Returns all existing inevents. Ownership is not transferred.
	    *
	    * @return reference to array of existing inevents
	    */      
        IMPORT_C const RPointerArray<CMCETestUIEngineInRefer>& InRefers();
        
         /**
	    * Deletes event.
	    *
	    * @param aEvent event to be deleted
	    */     
        IMPORT_C void DeleteRefer( CMCETestUIEngineRefer& aRefer);


        /**
	    * Deletes audiocodec.
	    *
	    * @param aAudioCodec audiocodec to be deleted
	    */     
        IMPORT_C void DeleteAudioCodec( CMCETestUIEngineCodec& aAudioCodec);
        
        IMPORT_C void DeleteVideoCodec( CMCETestUIEngineVideoCodec& aVideoCodec);
        
        /**
	    * responds if FCsession is on or off, default is off.
	    *
	    * */
        
        IMPORT_C TBool IsFCSession();
        
        /**
	    * sets fcsession on or off, default is off.
	    *
	    * */
        
        IMPORT_C void SetFCSession(TBool aFCSession);
  
  
        IMPORT_C void CreateMceSettingsL();
        
        IMPORT_C void CreateMceValueSettingsL();
        
        IMPORT_C TBool StartDTMFToneL( TChar& aChar );
        
        IMPORT_C TBool SendDTMFToneL( TChar& aChar, TInt& aInterval );
        
        IMPORT_C TBool SendDTMFToneSequenceL( TDes& aSequence  );
        
        IMPORT_C TBool GetIapIdL( TInt& aIapId  );
        
        IMPORT_C TBool GetCodecTypeL( TInt& aCodecIndex  );
        
        IMPORT_C void ShowCurrentCryptoContextsL(TBool aCrypto32,TBool aCrypto80);
        
        IMPORT_C void ShowSupportedCryptoContextsL(TBool aCrypto32,TBool aCrypto80);
        
        IMPORT_C void GetCryptoContextsL(TBool& aCrypto32,TBool& aCrypto80);
        
        IMPORT_C TRect GetRect();
        
  	
  	  //  IMPORT_C TInt StatusCode();to be modified later
  	    //IMPORT_C HBuf)
    public: // New functions, internal (=not imported)
	    
	  //   TMCETransactionDataContainer iDataContainer;	
	    CMCETestUIEngineProfileContainer& ProfileContainer();

	    /**
	    * Gets MCEManager
	    * @return reference to MCEManager
	    */        
        CMceManager& MCEManager();
        
        
        CMCETestUIEngineMultimediaObserver& MMObserver();
	    /**
	    * Informs user interface that internal state of engine has changed. 
	    */       
        void EngineStateChangedL();
        
       
	    /**
	    * Informs user interface that a notification should be shown to user. 
	    * @param aMessage notification to be shown
	    */               
        void NotifyUser( const TDesC& aMessage );
        
	    /**
	    * Notifies user. The note has to be dismissed by the user.
	    * @param aMessage notification to be shown to user
	    */   
	      void NotifyUserWaitL ( const TDesC& aMessage );

        /////////////////////////////////////// 
        //
        // TODO: Functions to inform and warn user 
        //
        ////////////////////////////////////////

        /**
        * Asks user interface to select one of multiple choices.
        * 
        * @param aQuestion data structure holding the question and choices.
        * @return index of chosen choice. Negative if canceled or otherwise not
        *         answered. 
        */         
        TInt AskQuestionL( const CMCETestUIQuestionSingleSelection& aQuestion );        

        /**
        * Asks user interface to return single textual value
        * 
        * @param aQuestion data structure holding the question and 
        *                  default value.
        * @return given value
        */       
        void AskQuestionL( CMCETestUIQuestionDataQuery& aQuestion );
        
        TBool SessionParamsL(TSessionParams &aSessionParams, TSessionType aSessionType);
        
        TBool SessionEstablishValues(TSessionEstablishValues &sessionValues);
        
        TBool SessionResponseValues(TSessionResponseValues &responseValues);
        
        TBool NotifyValues(TNotifyValues &notifyValues);
        
        TBool IsVideoEnabled();
        
        void EnableVideo(CMCETestUIEngineSession& iSession);
        
      	void EnableReceivingVideo(CMCETestUIEngineSession& iSession);
        
        void DisableVideo(CMCETestUIEngineSession& iSession);
        
        void DisableReceivingVideo(CMCETestUIEngineSession& iSession);
        
        void UpdateWithPriorityCodecValues(CMceInSession* aSession);
        
        void UpdateWithPocPriorityCodecValues(CMceInSession* aSession);
        void SetPriorities();       
        
        void UpdatePoCValues(CMceInSession* aSession);
        
        void UpdateInactivitytimeValues(CMceInSession* aSession);
        
        void UpdateRtpSink(CMceInSession* aSession);
        
        void BundleStreamsL(CMceInSession* aSession);
        
        void UpdateIncomingSessionVoIPDataL(CMceInSession* aSession);
        
        void UpdateIncomingSessionVoIPDataAfterAnswerL(CMceInSession& aSession);
        
        void CsStatusChangedL( TPoCOmaCsState aStatus );
        
        void DoHoldUnholdL( TBool aHold );
        
        void DoEnableDisableL( CMceSession& aSession, TBool aHold  );
        
        void NotifyStreamStateChangeL( CMceMediaStream& aStream ); 
        
        void AddSendRecvDefaultAudioStreamsL(CMceInSession* aSession);
        
        TInt PrepareConnection( TCommDbConnPref& aPrefs, TInt aIapId );
        
        void GetIapIdFromAPList();
        
        /**
        * Get next available Id for new outsession 
        *
        * @return Next available Id.
        */
        TInt NextOutSessionId();

        /**
        * Get next available Id for new insession 
        *
        * @return Next available Id.
        */
        TInt NextInSessionId();
        
        
        CMceFcMsgExchange& FloorControl();
    //    void SetStatusCode(TInt aStatusCode);

       
       /* enum TSessionType 
            {
            ECustomSession = 0,
            EPoCSession,
            EVoIPSession,
            EVSSession,
            };   
            
         TSessionType*/     
         
        TBool EventParamsL(TEventParams &aEventParams);
        
        /**
        * Get next available Id for new outevent 
        *
        * @return Next available Id.
        */
        TInt NextOutEventId();

        /**
        * Get next available Id for new inevent
        *
        * @return Next available Id.
        */
        TInt NextInEventId();
        
        // get parameters from the user
        TBool ReferParamsL(TReferParams &aReferParams);
        
        TBool ReferToParamsL(TReferParams &aReferParams);
        /**
        * Get next available Id for new outrefer
        *
        * @return Next available Id.
        */
        TInt NextOutReferId();

        /**
        * Set MMF priority for audio codecs
        */
        TInt NextInReferId();
        
        void SetMMFPriorityForCodecsL( const RPointerArray<CMceAudioCodec>& aCodecs, 
                                       TUint aAudioPriority, 
                                       TUint aAudioPreference );

        /**
        * Set MMF priority for video codecs
        */
        void SetMMFPriorityForCodecsL( const RPointerArray<CMceVideoCodec>& aCodecs, 
                                       TUint aPriority, 
                                       TUint aPreference );
    
		/**
        * Add Transaction ID's to an Array
        *
        * @param aId Transaction ID to be added
        */
  		void AddTransactionIdL( TMceTransactionId aId );


		/**
        * Returns Array of Transaction ID's
        *
        * @returns Array of Transaction ID's
        */
  		const RArray< TMceTransactionId >& TransactionIds() const;
  		

		/**
        * Remvoes INFO TransactionID from the Array
        *
        * @param aId Transaction ID to be delted
        */
  		void RemoveTransactionId( TMceTransactionId aId );
  		
  		void GetJitterBufferValues( CMceMediaStream* aMediaStream, 
  		                            TInt& aJitbufSize, 
  		                            TInt& aJitbufThreshold, 
  		                            TInt& aJitbufTimeout,
  		                            TBool aSessionTypeKnownToBeVideo );

        TBool IsSendRecvVideo( CMceSession* aSession );
         
    private:  // New functions
	    
        /**
        * C++ default constructor.
        */
        CMCETestUIEngine();	

    	/**
        * Symbian second-phase constructor
        */
        void ConstructL();	    
        
    private:  // Owned data

		/// SIP stack observer, owned.
		CMCETestUIEngineSipObserver* iSipObserver;

		/// SIP stack, owned.
		CSIP* iSip;

        /// Container for all available and used profiles. Owned.
        CMCETestUIEngineProfileContainer* iProfiles;

		// MCE Manager, owned
		CMceManager* iManager;

        // Container for Multimedia API observers, owned. 
        CMCETestUIEngineMultimediaObserver* iMMObserver;

		// Floor controller, owned. Not used yet.
		CMceFcMsgExchange* iFC;	

        // All existing OutSessions, owned
        RPointerArray<CMCETestUIEngineOutSession> iOutSessions;

        

        // Commands applicaple to engine
        RPointerArray<TMCETestUIEngineCmdBase> iCommands;        

        // Data container meant to be used in observers
        TMceTransactionDataContainer iDataContainer;	

		 // All existing OutEvents, owned
        RPointerArray<CMCETestUIEngineOutEvent> iOutEvents;

        // All existing InEvents, owned
        RPointerArray<CMCETestUIEngineInEvent> iInEvents;
        
        // All existing OutRefers, owned
        RPointerArray<CMCETestUIEngineOutRefer> iOutRefers;

        // All existing InRefers, owned
        RPointerArray<CMCETestUIEngineInRefer> iInRefers;
        
        // call watcher from PoC
        CMCEPoCOmaCSCallWatcher* iCallWatcher;


        // Counter for created CMCETestUIEngineOutSessions.
        // To simulate static counter in class CMCETestUIEngineOutSession.
        TInt iNextOutSessionId;
        
        // Counter for created CMCETestUIEngineOutSessions.
        // To simulate static counter in class CMCETestUIEngineOutSession.
        TInt iNextInSessionId;
        
     //   TInt iStatusCode;
     
     // Counter for created CMCETestUIEngineOutEvents.
        // To simulate static counter in class CMCETestUIEngineOutEvent.
        TInt iNextOutEventId;
        
        // Counter for created CMCETestUIEngineOutEevnts.
        // To simulate static counter in class CMCETestUIEngineOutEvent.
        TInt iNextInEventId;
        
     // Counter for created CMCETestUIEngineOutRefers.
        // To simulate static counter in class CMCETestUIEngineOutEvent.
        TInt iNextOutReferId;
        
        // Counter for created CMCETestUIEngineOutRefers.
        // To simulate static counter in class CMCETestUIEngineOutEvent.
        TInt iNextInReferId;   
        

        TBool iFCSession ;

        RArray< TMceTransactionId > iTransactionIds;
        
    public:
    	TBool iRtcpFlag;
        
        TInt iSRCount;
        
        TInt iRRCount;
        
        TBool iKoppaFlag;
        
        TBool iBundle;
        
        TBool iSdp;
        
        TBool i180PreCondition;
        
        TBool iSecureSessionAVP;
        
        TBool iSecPreconditions;
        TBool iHoldWithAddress;
        
        TBool iAutoResponse;
        
        TBool iVoIPFlag;
        
        TBool iMultiCodecSupport;
        
       
        TInt iPriorityType;
        
        TInt iTranscodeCount;
        
        TInt iPriorityUplink;
        TInt iPriorityDownlink;
        TInt iPrefUplink;
        TInt iPrefDownlink;
        
        TInt iJitterBufSize;
        TInt iJitterBufTreshold;
        TInt iJitterBufTimeout;
        
        TUint iIapId;
        
        TBool iOutSessionWithProfile;        
        TBool iOutSessionWithAPList;
        TBool iOutSessionWithAPNumber;
        
        RSocketServ iSocketServ; 
        RConnection iConn;  
        // All existing InSessions, owned
        RPointerArray<CMCETestUIEngineInSession> iInSessions;
        
        TSessionParams iInSessionSettings; 
        
         private: // Not owned data
    
        // Pointer to user interface. Not owned.
        // All user interface related calls are delegated to iUserInterface.
        MMCETestUIUserInterface* iUserInterface;
        
    };

class TSessionEstablishValues
		{
		public:
		 TInt timeOut;
		 TBuf<100>	headers;
		 TInt iBundle;
		// TBuf<100> contentType;
		// TBuf<100> content;
		// TBuf<100> contentHeaders;
		TInt iServiceType;
		};
class TSessionResponseValues
		{
		public:
		 TInt code;
		 TBuf<100>	reason;
		// TInt iBundle;
		// TBuf<100> contentType;
		// TBuf<100> content;
		// TBuf<100> contentHeaders;
		};
		
class TNotifyValues
		{
		public:
		 TBuf<100>	headers;
		 //TBuf<100>	contentType;
		 //TBuf<100>	content;
		};		
					 
#endif
