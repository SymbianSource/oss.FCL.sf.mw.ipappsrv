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




#ifndef CMCECOMSESSION_H
#define CMCECOMSESSION_H

#include <e32base.h> 
#include <e32std.h>  
#include <badesca.h>
#include <in_sock.h>
#include <s32strm.h>

#include "mcesession.h"
#include "mceclientserver.h"
#include "mcecomserializable.h"
#include "mcesecuresession.h"

class CMceComMediaStream;
class TMceComFactory;
class TMceComEvent;
class MMceMediaSessionObserver;
class CMceComStreamBundle;

#ifdef MCE_COMMON_SERVER_SIDE
#include "mcesecuremediasession.h"
#include "mcemediadefs.h"
#include "mcesrv.h"

class TMceMediaState;
class CMceSrvStream;
class CMceSdpSession;
class CMceMediaManager;
class CMceAdoptedSrvStream;
class CMceSecureMediaSession;
class CSdpDocument;
#endif//MCE_COMMON_SERVER_SIDE

const TInt KMceMaxSessionModifiers = 10;
const TUint32 KMceMaxTypeOfServiceValue = 256;
const TUint32 KMceDefaultTypeOfServiceValue = 184;

typedef TFixedArray<TUint, KMceMaxSessionModifiers> TMceComSessionModifiers;

#define MCE_SET_LOCAL_IP_ADDR( session, addr )\
    MCESRV_DEBUG_ADDRVALUE( "local ip", addr );\
    (session)->iLocalIpAddress = addr
    
#define MCE_SET_REMOTE_IP_ADDR( session, addr )\
    MCESRV_DEBUG_ADDRVALUE( "remote ip", addr );\
    (session)->iRemoteIpAddress = addr
    

const TBool KMceDeepMergeNo = EFalse;
const TBool KMceDeepMergeYes = ETrue;

        
/**
 * Server/Client session
 *
 * @lib 
 */
class CMceComSession : public CBase,
                       public MMceComSerializable
                       
	{

public://enums

    enum TType
        {
        EInSession,
        EOutSession
        };
        
    /**
    * Constructor
    * @param aType the type of session
    * @return The instance
    */
    static CMceComSession* NewL();

    /**
    * Constructor
    * @param aType the type of session
    * @return The instance
    */
    static CMceComSession* NewL( TType aType );

    /**
    * Constructor
    * @param aType the type of session
    * @return The instance
    */
    static CMceComSession* NewLC( TType aType );

    /**
    * Constructor
    * @param aSerCtx context for serialization
    * @return The instance
    */
    static CMceComSession* NewL( MMceComSerializationContext& aSerCtx );

    /**
    * Constructor
    * @param aSerCtx context for serialization
    * @return The instance
    */
    static CMceComSession* NewLC( MMceComSerializationContext& aSerCtx );

    /**
    * Destructor.
    */
	~CMceComSession();
	
public: // Functions

    /**
    * ID 
    * @return ID
    */
	TUint32 Id() const;

    /**
    * Adds media stream into session.
    * @param aMediaStream, media stream to be added.
	*		 ownership is transferred.
    * @return The current state of the session
    */
	void AddStreamL( CMceComMediaStream* aMediaStream );


    /**
    * Returns the streams belonging to the session.
    * @return streams of the session.
    */
	RPointerArray<CMceComMediaStream>& Streams();

 	/**
    * Adds media manager observer
    * @param aMediaObserver, media manager observer
    */
	void SetObserver( MMceMediaSessionObserver& aMediaObserver );
	
	/**
	* Adds grouped media streams to the session. Changes SDP
	* of the session. In order to get complete update, whole
	* session must be updated causing sending of re-invite.
    */
	void AddBundleL( CMceComStreamBundle* aBundle );
	
	/**
    * Removes grouped media stream from session. Changes SDP
	* of the session. In order to get complete update, whole
	* session must be updated causing sending of re-invite.
    * @param aBundle bundle to be removed
    */
    void RemoveBundleL( CMceComStreamBundle& aBundle );
	
	/**
    * Gets the grouped media streams of the session.
    * @return bundles of the session
    */
	const RPointerArray< CMceComStreamBundle >& Bundles() const;
	
	/**
    * Returns media observer.
    * @return Media observer
    */
	MMceMediaSessionObserver& MediaObserver();
	
	/**
    * Sets the originator
    * @return 
    */
	void SetOriginatorL( const TDesC8& aOriginator );
	
	/**
    * Sets the recipient
    * @return Media observer
    */
	void SetRecipientL( const TDesC8& aRecipient );
	
	/**
    * Assigner for session modifier
    * @param aModifier modifer key
    * @return assigner for session modifier
    */
    TUint& Modifier( TMceSessionModifier aModifier );
    
    /**
    * Checks whether RTCP should be used.
    * return ETrue, if all the streams indicate RTCP usage,
    *        otherwise EFalse. 
    */
    TBool UseRtcp() const;
    
public: // serialization

    /**
    * Returns serialization id
    * @return serialization id
    */
    TUint64 SerializationId() const;

    /**
    * Internalizes flat data
    * @param aReadStream read stream
    */
    void InternalizeFlatL( RReadStream& aReadStream );

    /**
    * Externalizes flat data
    * @param aWriteStream write stream
    */
    void ExternalizeFlatL( RWriteStream& aWriteStream );

    /**
    * Internalizes
    * @param aSerCtx context for serialization
    */
    void InternalizeL( MMceComSerializationContext& aSerCtx );
    
    /**
    * Externalizes
    * @param aSerCtx context for serialization
    */
    void ExternalizeL( MMceComSerializationContext& aSerCtx );

public://

    
    /**
    * Factory
    * @return factory
    */
    TMceComFactory BaseFactory();

    /**
    * Factory
    * @return factory
    */
    static TMceComFactory Factory();
        
public://event handling && updating && cloning

    /**
    * Initializes 
    * @param aParent the parent
    */
    void InitializeL();

    /**
    * Traversal event handler
    * @param aEvent the event
    * @return status, if event was consumed or not or object needs update
    */
    virtual void EventReceivedL( TMceComEvent& aEvent );
        
    /**
    * Updates whole session object with new one
    * @param aSession new session
    */
    void UpdateL( CMceComSession& aSession );
    
    /**
    * Clones whole session object 
    */
    CMceComSession* CloneL( TBool aPullModeUpdate = EFalse );

    /**
    * Is structure changed during update
    */
    TBool& IsStructureChanged();

    void UpdateFlatL( CMceComSession& aSession );


#ifdef MCE_COMMON_SERVER_SIDE

    static void Delete( CMceComSession*& aThis, CMceMediaManager& aManager );

    CMceSdpSession& SdpSession() const;

    CMceComSession* Backup();
    
    TBool IsBackup();
    
    TBool IsMerged();

    void Merge( CMceComSession& aSession, TBool aDeepMerge = KMceDeepMergeNo );

    void UnMerge();
        
    void UnMergeL( CMceComSession& aSession );
    
    void AttachSDPSessionL( CMceSdpSession& aSdpSession );
    
    void DetachSDPSession();

    TMceMMState SetNegotiationStateL( TMceMediaState* aNewState );
    
    void SetNegotiationState( TMceMediaState* aNewState );
    
    TMceMediaState& NegotiationState() const;

    void PrepareL( CMceMediaManager& aManager );
    
    void PrepareL();

    CMceComSession* CloneAndMergeLC( CMceComSession& aUpdate );
        
    RPointerArray<CMceSrvStream>& MccStreams();
    
    RPointerArray<CMceSrvStream>& AdoptedMccStreams();
    
    void AdoptStreamL( CMceSrvStream& aStream );
  
    CMceSrvStream* RemoveAdoptedStream( CMceSrvStream& aStream );
	
	/**
	* Create Secure Session
	*/
    CMceSecureMediaSession& SecureSessionL();

	CMceSecureMediaSession* SecureSession();

	void CloneSecureSessionL(CMceComSession& aSession);
        
    void DeleteSecureSession();
    
    void SdpCleanup( CSdpDocument* aPrevious, CSdpDocument* aReplacement );
    
    TMceNegotiationAnswerType& AnswerType();
    
#endif//MCE_COMMON_SERVER_SIDE
    
    
private:

    /**
     * C++ default constructor.
     */
	CMceComSession();

    /**
     * C++ constructor.
     * @param aType the type of session
     */
	CMceComSession( TType aType );

    /**
     * second-phase constructor
     */
	void ConstructL();

    /**
     * second-phase copy constructor
     */
	void ConstructL( CMceComSession& aSession, 
	                 TBool aPullModeUpdate = EFalse);

    /**
    * Clone streams
    * @param aSession session
    */
    void CloneStreamsL( CMceComSession& aSession );
    
    /**
    * Clone bundles
    * @param aSession session
    */
    void CloneBudlesL( CMceComSession& aSession );
    
    /**
    * Update streams
    * @param aSession session
    */
    void UpdateStreamsL( CMceComSession& aSession );

    /**
    * Update bundles
    * @param aSession session
    * @param aStreams streams
    */
    void UpdateBundlesL( CMceComSession& aSession,
                         const RPointerArray<CMceComMediaStream>& aStreams );

    

public: // Owned serialized data


    /**
     * type of session
     */
    TUint32 iID;

    /**
     * type of session
     */
    TType iType;


    /**
     * state of session
     */
    CMceSession::TState iState;
    
    /**
     * recipient
     */
    HBufC8* iRecipient;
    
    /**
     * originator
     */
    HBufC8* iOriginator;

    /**
     * is connection active
     */
    TBool iIsConnectionActive;
    
    /**
     * Dialog id
     */
    TUint32 iDialogId;
    
    /**
     * Session modifiers
     */
     TMceComSessionModifiers iSessionModifiers;
     
public: // Owned data

    /**
     * Internet access point used
     */
    TUint32 iIapId;	
    
   	/**
    * Mcc session Id
    */
    TUint32 iMccID;
    
    /**
    * Local host IP-address (IPv4 or IPv6)
    */    
    TInetAddr iLocalIpAddress;
 
    /**
    * Remote host IP-address (IPv4 or IPv6)
    */
    TInetAddr iRemoteIpAddress;

    /**
    * has structure changed during update
    */
    TBool iStructureChanged;
    
    /**
    * Should use local preconditions
    */
    TBool iUseLocalPreconditions;

    /**
    * Should use remote preconditions
    */
    TBool iUseRemotePreconditions;
    
    TBool iFcSignallingRequired;
    
    TBool iRequireSignalling;
    // Setting TMccNetService MediaQoS
    TUint32 iServiceType;
    
    TUint iAnswerType;
    
    TUint iCodecSelection;

    /* Remote use require header with precondtion option*/
    TBool iRemoteSecPreconditionsRequired ;
    
public: // NOT Owned data
    
    /**
    * Media manager observer
    */
    MMceMediaSessionObserver* iMediaObserver;

    /**
    * cloned from
    */
    CMceComSession* iCloneOrigin;
    

public: // Owned invite related data

    /**
     * Session timer interval, in milliseconds
     */
    TUint32 iTimeout;
    
    /**
     * Minimun SE received in 422, in milliseconds
     */
    TUint32 iMinSE;
    
    /**
     * Is this UA performing the refresh requests
     */
    TBool iRefresh;
    
     /**
     * sip headers
     */
    CDesC8Array* iSIPHeaders;
    
     /**
     * content type
     */
    HBufC8* iSIPContentType;
    
     /**
     * content headers
     */
    CDesC8Array* iSIPContentHeaders;
    
    /**
    * content
    * NOT serialized
    */
    HBufC8* iSIPContent;
    
    /**
    * local sdp session lines
    */
    CDesC8Array* iLocalSessionSDPLines;

    /**
    * remote sdp session lines
    */
    CDesC8Array* iRemoteSessionSDPLines;
    
    /**
    * Flag to indicate if the secure session is needed 
    * To create secure link in MCC, this has to be set as True.
    */
    TBool iIsSecureSession;
    
    TBool iPullModeUpdate;
    
     /**
    * Crypto Suite Set by client
    */
    RArray<TMceCryptoContext> iClientCryptoSuites;
    
    /**
    * secure level by the contact header in offer/Answer which is  
    * with SIPSUri or transport parameter is tls.
    */
    CMceSession::TControlPathSecurityLevel iSipContactAddrSecure;

    
private: // Owned data


    /**
     * media streams
     */
	RPointerArray<CMceComMediaStream> iMediaStreams;
	
    /**
     * bundles
     */
	RPointerArray<CMceComStreamBundle> iBundles;

#ifdef MCE_COMMON_SERVER_SIDE


    RPointerArray<CMceSrvStream> iMccStreams;
    
    RPointerArray<CMceSrvStream> iMccAdoptedStreams;

	//current state
    TMceMediaState* iNegotiationState;
    
    CMceSdpSession* iSdpSession;

	
	/**
    * SecureMediaSession
    */
	CMceSecureMediaSession* iSecureMediaSession;

        
	//definitions for unit testing
	MCEMM_UT_DEFINITIONS
    friend class UT_CMceSipSession;
    friend class UT_CMceStateOffering;
    
#endif//MCE_COMMON_SERVER_SIDE


	};


#endif 