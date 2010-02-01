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




#ifndef CMCECOMMEDIASTREAM_H
#define CMCECOMMEDIASTREAM_H

#include <e32std.h>  	
#include <e32base.h>
#include <s32strm.h>
#include <in_sock.h>
#include "mcemediastream.h"
#include "mcedefs.h"
#include "mceclientserver.h"
#include "mcecomserializable.h"


#ifdef MCE_COMMON_SERVER_SIDE

#include <mmccinterfacedef.h>
#include <mmccsecureinterface.h>

class TMceMccComEvent;
class CMceMediaManager;

#endif//MCE_COMMON_SERVER_SIDE


class TMceComFactory;
class TMceComMediaStreamFactory;
class TMceComEvent;
class CMceComSession;
class TMceMccComEvent;
class CMceComCodec;
class CMceComMediaSource;
class CMceComMediaSink;
class MMceComSerializationContext;
class TMcePreconditions;
class CSdpMediaField;

#define IS_SENDSTREAM( stream )\
    ( ( stream )->iStreamType == CMceComMediaStream::ESendStream ||\
      ( stream )->iStreamType == CMceComMediaStream::ESendOnlyStream )

#define IS_RECEIVESTREAM( stream )\
    ( ( stream )->iStreamType == CMceComMediaStream::EReceiveStream ||\
      ( stream )->iStreamType == CMceComMediaStream::EReceiveOnlyStream )


const TBool KMceNoEndpointAdjustment = EFalse;
const TBool KMceEndpointAdjustment = ETrue;
const TBool KMceRoleAnswerer = ETrue;
const TBool KMceRoleOfferer = EFalse;


/**
 * 
 *
 * @lib 
 */
class CMceComMediaStream: public CBase,
                          public MMceComSerializable

	{


public: // Destructor

    enum TStreamType
        {
        ELocalStream,
        ESendOnlyStream,
        EReceiveOnlyStream,
        ESendStream,
        EReceiveStream
        };
    
public: // Destructor


    /**
    * Destructor.
    */
	~CMceComMediaStream();
	
public: // Source and sink functions

    /**
    * Adds source to the stream.
    * @param aSource source to be added to the stream.
    */
	void SetSourceL( CMceComMediaSource* aSource );

    /**
    * Adds sink to the stream.
    * @param aSink sink to be added to the stream.
    */
	void AddSinkL( CMceComMediaSink* aSink );

    /**
    * Deletes sink
    * @param aIndex index of sink in the array
    */
    void DeleteSink( TInt aIndex );

    /**
    * Removes codec from stream
    * @param aCodec the codec
    */
    virtual void RemoveCodecL( CMceComCodec* aCodec ) = 0;
    
	/**
	* Clears list of codecs and deletes each codec in it.
	*/
    virtual void DestroyCodecs() = 0;

    /**
    * Searches for codec matching with input codec instance or sdp name
    * @param aCodec the search term
    * @return matching codec or NULL if not found
    */
    virtual CMceComCodec* FindCodecL( CMceComCodec& aCodec ) = 0;

    /**
    * Adds codec to the stream.
    * @param aCodec codec to be added to the stream.
	*	     Ownership is transferred.
    */
	virtual void AddCodecL( CMceComCodec* aCodec ) = 0;

    /**
    * Gets source of the stream. Returns null is source not set.
    * @return source of the stream. Ownership is not transferred.
    */
	CMceComMediaSource* Source();

    /**
    * Gets array of sinks of the stream.
    * @return array of sinks.
    */
	RPointerArray<CMceComMediaSink>& Sinks();
	
	virtual void ReorderCodecs() = 0;


public: // Binding opposite way stream

    /**
    * Binds opposite direction stream to this stream.
    * @param aStream another stream to be bind to the stream.
    */
	 void BindL( CMceComMediaStream* aStream );

    /**
    * Gets opposite direction stream of this stream.
    * @return opposite direction stream.
    */
	 CMceComMediaStream& BoundStreamL();

    /**
    * Checks if opposite direction stream is set.
    * @return ETrue if opposite direction stream is set.
    */
	 TBool BoundStream() const;

    /**
    * Checks if this is binder (biding owner)
    * @return ETrue if this is binder (biding owner)
    */
	TBool Binder() const;
	
    /**
    * Gives 'main' stream
    * @return 'main' stream
    */
	CMceComMediaStream& operator()();

    /**
    * Gives 'main' stream
    * @return 'main' stream
    */
	const CMceComMediaStream& operator()() const;
	
    /**
    * Gives send stream
    * @return send stream
    */
    CMceComMediaStream* SendStream();
    
    /**
    * Gives receive stream
    * @return receive stream
    */
    CMceComMediaStream* ReceiveStream();

    /**
    * Gives offer stream
    * @return offer stream
    */
    CMceComMediaStream* OfferStream();

    /**
    * Gives answer stream
    * @return answer stream
    */
    CMceComMediaStream* AnswerStream();
    
    /**
    * Gives sdp index
    * @return sdp index
    */
    TInt& SdpIndex();
    
    
public: //from MMceComSerializable

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
    

public: //serialization helpers

    /**
    * Factory
    * @return factory
    */
    TMceComFactory BaseFactory();

    /**
    * Factory
    * @return factory
    */
    static TMceComMediaStreamFactory Factory();
    
    /**
    * Adds source, during serialization, to the stream.
    * @param aSerCtx serialization context
    */
	void SetSourceL( MMceComSerializationContext& aSerCtx );

    /**
    * Adds sink, during serialization, to the stream.
    * @param aSerCtx serialization context
    */
	void AddSinkL( MMceComSerializationContext& aSerCtx );

    
public:// new Functions

    /**
    * ID 
    * @return ID
    */
	TMceMediaId Id() const;

    /**
    * Parent object
    * @return Parent object
    */
	CMceComSession* Session() const;


    /**
    * Initializes 
    * @param aParent the parent
    */
    virtual void InitializeL( CMceComSession& aParent );
    
    /**
    * Traversal event handler
    * @param aEvent the event
    * @return status, if event was consumed or not or object needs update
    */
    virtual TInt EventReceivedL( TMceComEvent& aEvent );

    /**
    * Updates 
    * @param aStream new stream
    */
    virtual void UpdateL( CMceComMediaStream& aStream );

    /**
    * Clones whole stream object
    * @return cloned stream object
    */
    virtual CMceComMediaStream* CloneL() = 0;

    
public:// new Functions

    /**
    * Returns ETrue, if enabled
    * @return ETrue, if enabled
    */
    virtual TBool IsEnabled();
    
    /**
    * Sets remote media port
    * @param aPort remote media port
    */
	void SetRemoteMediaPort( TUint aPort );
	
	/**
    * Sets remote rtcp media address
    * @param aAddr remote rtcp media address
    */
	void SetRemoteRtcpMediaAddrL( TDesC16& aAddr );
	
    /**
    * Sets local media port
    * @param aPort local media port
    */
    void SetLocalMediaPort( TUint aPort );
	
    /**
    * Gets remote media port
    * @return remote media port
    */
	TUint RemoteMediaPort() const;
	
    /**
    * Gets local media port
    * @return local media port
    */
	TUint LocalMediaPort() const;
	
    /**
    * Sets remote media port
    * @param aPort remote media port
    */
	void SetRemoteRtcpMediaPort( TUint aPort );
	
	/**
	* Checks whether remote media port has changed
	* @return ETrue if port has changed, otherwise return EFalse
	*/
	TBool RemoteMediaPortChanged( TUint aNewPort );
	
	
	/**
	* Checks whether remote RTCP address, and port has changed 
	* @return ETrue if port has changed, otherwise return EFalse
	*/
	TBool CMceComMediaStream::RemoteRTCPAddressChanged( TUint aNewPort, TInetAddr aAddr );
    
		
	/**
    * Stores remote port.
    * @param aIgnore, ETrue if remote port comparison should be ignored
    */
	void SetIgnoreRemotePort( TBool aIgnore );
    
    TBool& DowngradedEndpoints();
    
#ifdef MCE_COMMON_SERVER_SIDE

public:// new (virtual) Functions

    /**
    * Called when stream has been decoded
    * @param aDecodedOffer is offer decoded
    */
    void DecodedL( TBool aRoleAnswerer );
    
    /**
    * Synchronizes stream's codecs.
    * @param aRoleAnswerer is answerer
    */
    virtual void SynchronizeL( TBool aRoleAnswerer ) = 0;
    
    /**
    * Update default codec to reflect current negotiation
    */
    virtual void UpdateDefaultCodecL() = 0;

    /**
    * Prepares
    */
    virtual void PrepareL() = 0;

    /**
    * Return mcc stream type
    * @return mcc stream type
    */
    virtual TInt MccStreamType() const = 0;

    /**
    * Return mcc link type
    * @return mcc link type
    */
    virtual TInt MccLinkType() const = 0;

    /**
    * Handles event received from mcc
    * @param aEvent the event from mcc
    */
    virtual void EventReceived( TMceMccComEvent& aEvent );

public:// new Functions
        
    /**
    * Sets state 
    * @param aEvent the event from mcc
    */
    void SetState( TMceMccComEvent& aEvent );
    
    /**
    * Sets link id
    * @param aLinkId the link id
    */
    void SetLinkId( TUint32 aLinkId );
    
    /**
    * Gets direction
    * @return direction
    */
    TInt Direction();
    
    /**
    * Sets direction
    * @param aDirection direction
    * @param aEndpointAdjustment should endpoints follow
    */
    void SetDirection( TInt aDirection, TBool aEndpointAdjustment = KMceEndpointAdjustment );
    
    /**
    * Sets direction. Leaves if not allowed
    * @param aDirection direction
    */
    void SetDirectionL( TInt aDirection );

    /**
    * Sets stream enabled/disabled
    * @param aEnable if ETrue stream and sinks/source 
    * is set enabled
    * @param aEndpointAdjustment if endpoints follow
    */
    void Enable( TBool aEnable, TBool aEndpointAdjustment );
    
    /**
    * Gets SDP stream type
    * @return SDP stream type
    */
    TInt SdpStreamType();
    
    /**
    * Gets or creates preconditions
    * @param aModifier mce preconditon modifier
    * @param aMediaLine media line
    * @return preconditions
    */
    TMcePreconditions* PreconditionsL( TMceSessionModifier aModifier,
    									CSdpMediaField* aMediaLine = NULL );

    /**
    * Gets preconditions
    * @return preconditions
    */
    const RPointerArray<TMcePreconditions>& Preconditions() const ;
    
    /**
    * CopyPrecondition parameters 
    * @param aModifier mce preconditon modifier
    * @return void
    */
    void CopyPreconditionL( TMceSessionModifier aModifier );

    
    /**
    * Merges with stream
    */
    void Merge( CMceComMediaStream& aMergeWith );

#endif//MCE_COMMON_SERVER_SIDE

    /**
    * Gets empty sink
    * @return empty sink
    */
    CMceComMediaSink& EmptySinkL();
    
    /**
    * Gets empty source
    * @return empty source
    */
    CMceComMediaSource& EmptySourceL();
    

    /**
    * Checks whether RTCP should be used.
    * return ETrue, if the source and all the sinks indicate RTCP usage,
    *        otherwise EFalse. 
    */
    TBool UseRtcp() const;

protected:


    /**
     * C++ default constructor.
     * @param type of stream
     */
	CMceComMediaStream( TMceMediaType aType );

    /**
    * second-phase constructor
    */
	void ConstructL();
	
    /**
     * second-phase copy constructor
     */
	void ConstructL( CMceComMediaStream& aStream );
	

private: // methods

    /**
    * one-way bind
    * @param aStream another stream to be bind to the stream.
    */
    void DoBind( CMceComMediaStream* aStream );    
    
    /**
    * Deletes source
    */
	void DeleteSource();
	
	/**
    * Checks whether RTCP should be used.
    * return ETrue, if the source and all the sinks indicate RTCP usage,
    *        otherwise EFalse. 
    */
	TBool DoUseRtcp() const;

public: // Owned serialized Data

    /**
     * ID
     */
    TMceMediaId iID;

    /**
     * type
     */
    TMceMediaType iType;
    
    /**
     * state, enabled or disabled
     */
    TBool iIsEnabled;
    
	/**
    * Local port
    */
	TUint iLocalMediaPort;

	/**
    * Remote port
    */
	TUint iRemoteMediaPort;
    
    /**
    * Remote RTCP IP-address (IPv4 or IPv6)
    */    
    TInetAddr iRemoteRtcpAddress;
    
    /**
    * Remote RTCP port, if 0 then use RTP port + 1
    */
	TUint iRemoteRtcpPort;
	
    /**
     * local sdp media lines
     */
    CDesC8Array* iLocalMediaSDPLines;

    /**
     * remote sdp media lines
     */
    CDesC8Array* iRemoteMediaSDPLines;

    /**
     * stream type
     */
    TStreamType iStreamType;
    
    /**
     * stream state
     */
    CMceMediaStream::TState iState;
    
    /**
    * ignore remote port comparison
    */
    TBool iIgnoreRemotePort;

    /**
     * does this owns the link
     */
    TBool iLinkOwner;

    /**
     * linked stream. Owned if iLinkOwner is ETrue
     */
    CMceComMediaStream* iLinkedStream;
    
    /**
     * source
     */
	CMceComMediaSource* iSource;
	
    /**
     * sinks
     */
	RPointerArray<CMceComMediaSink> iSinks;

    /**
     * sdp index
     */
    TInt iSdpIndex;
    
    /**
     * link id
     */
    TUint32 iLinkId;
    
    /**
     * preconditions
     */
    RPointerArray <TMcePreconditions> iPreconditions;
    
    /**
     * empty source
     */
    CMceComMediaSource* iEmptySource;
    
    /**
     * empty sink
     */
    CMceComMediaSink* iEmptySink;
    
    
protected: // NOT owned data
	
    /**
     * session
     */
	CMceComSession* iSession;

private:

    TBool iDowngradedEnpoints;

#ifdef EUNIT_TEST
	friend class UT_CMceMediaSdpCodec;
    friend class UT_CMceAudioSdpCodec;
    friend class UT_CMceVideoSdpCodec;

#endif    
	};


#endif 