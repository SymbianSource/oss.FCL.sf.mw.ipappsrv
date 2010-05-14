/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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





#ifndef CMCESDPSESSION_H
#define CMCESDPSESSION_H

//  INCLUDES
#include <e32base.h>
#include <e32cmn.h>
#include <e32std.h> 
#include <in_sock.h>
#include <stringpool.h>

#include "mcecommediastream.h"
#include "mcemediadefs.h"
#include "mcesrv.h"
#include "mcesip.h"

// FORWARD DECLARATIONS
class CSdpDocument;
class CMceComSession;
class CMceComAudioStream;
class CSdpMediaField;
class CSdpOriginField;
class CMceMediaManager;
class CMceSdpCodec;
class CSIPDialogAssocBase;

/**
*  Class for storing session specific SDP parameters
*/
class CMceSdpSession : public CBase
    {

    public:  // enums
                        
    enum TOfferType
        {
        EFirst,
        EUpdate,
        ERefresh
        };
        
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CMceSdpSession* NewL( CMceComSession& aSession,
                                     CMceMediaManager& aManager );

        /**
        * Two-phased constructor.
        */
        static CMceSdpSession* NewLC( CMceComSession& aSession,
                                      CMceMediaManager& aManager );

        
        /**
        * Destructor.
        */
        ~CMceSdpSession();
        
    public: 
    
    
        CMceMediaManager& Manager() const;

        /**
        * Createas a SDPDocument based on the streams in iSession. For each stream
        * a media line is created in the sdp document. Fails if one of the streams 
        * is not supported / failed
        * @since
        * @param None.
        * @return CSdpDocument, containing the offer
        */
        CSdpDocument* CreateOfferL( CMceComSession& aSession, TOfferType aType = EFirst );
        
        /**
        * Goes through the answer and removes all the not supported codecs from 
        * iSession streams so that for each stream one codec is left. Otherwise
        * fail.
        * @since
        * @param None.
        * @return KErrNone, if successful, error if answer is not accepted 
        */
        TInt DecodeAnswerL( CSdpDocument& aSdpDocument, CMceComSession& aSession );
        
        /**
        * Decodes the incoming offer. For each media line in the offer, create corresponding
        * stream in aSession. If one of the streams failes, then fail the whole offer.
        * NOTE! if KErrNotSupported is returned, offer is rejected and next time CreateAnswerL()
        * is called it creates a rejected answer.
        * @return KErrNone, if successful, KErrNotSupported if failed
        */
        TInt DecodeOfferL( CSdpDocument& aSdpDocument, CMceComSession& aSession );
        
        /**
        * Creates an answer based on the OFFER received and the application's
        * updated preferences. In successful case SdpDocument will have one selected rtpmap
        * field for each media line and updated port number. And all session level parameters.
        * NOTE! if any of the media lines fail, SdpDocument will have rejected media lines.
        * @since
        * @param None.
        * @return KErrNone, if successful, KErrNotSupported if offer was rejected,        
        */
        CSdpDocument& CreateAnswerL( CMceComSession& aSession, TBool aInvalid = EFalse ); 
        
          

    public: // New functions
        
        /**
        * Set SDP <session name>. Default is -.
        * Leaves with system wide error code if setting fails.
        * @since
        * @param aUserName User name.
        * @return None.
        */
        void SetSessionNameL( const TDesC8& aUserName );
        
        /**
        * Get SDP <session name>
        * @since
        * @param None.
        * @return Session name
        */
        const TDesC8& SessionName() const;
            
        /**
        * Set SDP <username>. Default is MediaManager.
        * Leaves with system wide error code if setting fails.
        * @since
        * @param aUserName User name.
        * @return None.
        */
        void SetUserNameL( const TDesC8& aUserName );
        
        /**
        * Get SDP <username>
        * @since
        * @param None.
        * @return User name.
        */
        const TDesC8& UserName() const;

        /**
        * Stores the remote o= field
        * @param None.
        * @return None.
        */
        void StoreRemoteOriginL();
        
        /**
        * Checks if the SDP is refresh or re-invite
        * @param aSdpDocument, SDP document
        * @return None.
        */
        TBool IsSessionRefresh( CSdpDocument* aSdpDocument = NULL );

        /**
        * Cleans session level parameters
        * @since
        * @param aSessionVersion Session version.
        * @return None.
        */ 
        void CleanSessionParams( CSdpDocument& aDocument ) const;
        
        TBool DetachMedia( CMceComSession& aSession );
        
        void StockMediaSessionL();
        
        CMceSdpSession& ForkL();

        CMceComSession* Consumes( TUint32 aSessionId );
        
        void ContextSwitch( CMceSdpSession* aForkedSession = NULL );

        TBool IsMaster();
        
        void AttachContext( CSIPDialogAssocBase* aContext );

        CSIPDialogAssocBase* Context();

        TBool ContextSwitchRequested();
        void UpdateSecureStreamL( CMceComSession& aSession );
        

public: //update handling
        
        /**
        * Set the current media session
        * @param aSession the current media session
        */
        void SetMediaSession( CMceComSession* aSession );
        
        /**
        * Get the current media session
        * @return the current media session
        */
        CMceComSession* MediaSession();
        
        /**
        * Store backup session for current media session
        * @param aSession the backup media session
        */
        void SetBackup( CMceComSession* aSession );
        
        /**
        * backup session
        * @param None.
        * @return server session
        */
        CMceComSession* Backup();
        
        /**
        * Return indication wheater signalling is required
        * @return reference to value indication wheater signalling is required
        */
        TInt& SignallingRequired();

        /**
        * Return indication wheater network resource reservation is ready
        * @return flag indication wheater network resource reservation is ready
        */
        TBool NeedToNegotiate( CMceComSession& aSession );
        
        TBool ReservationNeeded( CMceComSession& aSession );
        
        TMceReturnStatus ReserveL( CMceComSession& aSession );

        /**
        * prepares session refresh
        * @param aSession the media session
        */ 
        void PrepareSessionRefreshL( CMceComSession& aSession );

        void SdpCleanup( CSdpDocument* aPrevious, CSdpDocument* aReplacement );
        
        
    private: // for internal use only
    
        /**
        * Encodes session level parameters
        * @param aSession the media session
        * @param aDocument the sdp document
        * @param aType the type of offer
        * @return None.
        */ 
        void EncodeSessionParamsL( CMceComSession& aSession, 
                                   CSdpDocument& aDocument, 
                                   TOfferType aType = EFirst );
        
        /**
        * Encodes session level attributes client is willing to insert
        * @param aSession the media session
        * @param aDocument the sdp document
        */
        void EncodeClientAttributesL( CMceComSession& aSession,
                                      CSdpDocument& aDocument ) const;

        /**
        * Decodes session level attributes to be sent to client
        * @param aDocument the sdp document
        * @param aSession the media session
        */
        void DecodeClientAttributesL( CSdpDocument& aDocument, 
                                      CMceComSession& aSession ) const;  
        
        /**
        * Gets a random number for version and session id 
        * @since
        * @param aSessionVersion Session version.
        * @return None.
        */
        TInt64 GetRandomNumber();
        
        TMceSipWarningCode SetRemoteIpAddressL( CMceComSession& aSession,
                                                CSdpDocument& aSdpDocument );
                                                
        
        void PrepareForAnswerEncodeL( CSdpMediaField& aMediaLine );
        
        CSdpMediaField* CreateMediaLineLC( CMceComMediaStream& aStream ) const;
        
        TBool MediaLineLC( CSdpMediaField*& aMediaLine, 
                           CMceSdpCodec*& sdpCodec,
                           CMceComMediaStream* aStream,
                           RPointerArray<CSdpMediaField>& aMediaLines,
                           RPointerArray<CMceComMediaStream>& aStreams );
                           
        CMceComMediaStream* MediaSlotInUse( TInt aMediaLine, 
                                            RPointerArray<CMceComMediaStream>& aStreams );
                           

        
        void DecodeConnectionFieldL( CSdpDocument& aSdpDocument, CMceComSession& aSession );
        void EncodeConnectionFieldL( CMceComSession& aSession, CSdpDocument& aSdpDocument );

        void MatchingStreamsToMLinesL( 
            CSdpDocument& aSdpDocument, CMceComSession& aSession );
            
        void DecodePullModeUpdateMediaLineL( CSdpDocument& aSdpDocument, 
                                             CMceComSession& aSession,
                                             CSdpMediaField& aMediaLine,
                                             TInt aIndex,
                                             CMceSdpCodec& aSdpCodec );   

        /**
        * Stores the remote a= field
        * @param None.
        * @return None.
        */
        void StoreRemoteMediaFieldsL();

        /**
        * Compares media lines 
        * @param aFieldsSrc for media lines being compared 
        * @param aFieldsDst for media lines being compared
        * @return ETrue if media lines are equal, EFalse otherwise
        */
        TBool CompareMediaLines( 
            RPointerArray<CSdpMediaField>& aFieldsSrc,
            RPointerArray<CSdpMediaField>& aFieldsDst ) const;
        
    private:
    
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
        /**
        * C++ default constructor
        */
        CMceSdpSession( CMceComSession& aSession, CMceMediaManager& aManager );  
    
    public:    // Owned Data

        CMceComSession* iInitialSession;

    public:    // Data
        
        CMceMediaManager& iManager;
            
        // User name in SDP origin field
        HBufC8* iUserName;  
        
        // Session name in SDP session name field
        HBufC8* iSessionName;
        
        // Session ID in SDP origin field
        TInt64 iSessionId;

        // Session version in SDP origin field
        TInt64 iSessionVersion; 
        
        // reference to corresponding server stream
        CMceComSession* iSession;
        
        // pointer to backup stream
        CMceComSession* iBackup;
        
        // SDP document, not owned
        CSdpDocument* iSdpDocument;
        
        // the origin field received last from the remote end
        CSdpOriginField* iRemoteOrigin;
        
        // the "a:-" field received last from the remote end
        RPointerArray<CSdpMediaField> iSdpRemoteMediaFields;
        
        TInt iIsSignallingRequired;
        
        RStringPool iStringPool;
        const TStringTable* iStringTable;
        
        // OldSchool Flag Outgoing Session
     	TUint iOOldSchool;
     	
     	// OldSchool Flag for Incomming Session
     	TUint iIOldSchool;
     	
     	TUint iOOldSchoolProceeding;
        
        TUint iOOldSchoolCompleted;

        TBool iIsMaster;
        CSIPDialogAssocBase* iContext;
        CSIPDialogAssocBase* iRequestedContext;
        
        // old local port, used for refresh
        TUint iOldLocalMediaPort;
		
        TBool iStoreRemoteOrigin;
    	//definitions for unit testing
    	MCEMM_UT_DEFINITIONS

public: //inner classes

    class TIterator
        {

    public:
        
        enum TFilter
            {
            EFilterNone = 0,
            EFilterMaster
            };
            
    public:
        
            TIterator( RPointerArray<CMceSdpSession>& aSessions, TFilter aFilter =  EFilterNone );
            
            TBool IsEof();
            TBool Next( CMceSdpSession*& aCandidate );
            
            void Reset();
            TInt Count();
            TInt Remove();
            TInt Current();
            
            
            void SetFilter( TFilter aFilter );
            
    private:
        
            TFilter iFilter;
            RPointerArray<CMceSdpSession>& iSessions;
            TInt iCurrentIndex;
    
        };
        
        

    };

#endif      // CMCESDPPARAMETERS_H   
            
// End of File
