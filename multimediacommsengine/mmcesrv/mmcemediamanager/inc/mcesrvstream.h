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




#ifndef CMCESRVSTREAM_H
#define CMCESRVSTREAM_H

#include <e32base.h>
#include <mmccinterfacedef.h>
#include <in_sock.h>
#include "mcemediadefs.h"
#include "mcecommediastream.h"
#include "mcesrv.h"

class CMceComAudioStream;
class CMceComVideoStream;
class CMceSrvSource;
class CMceSrvSink;
class CMceSrvCodec;
class CMceComEndpoint;
class CMceComMediaSource;
class CMceComMediaSink;
class CMceComCodec;
class CMceMediaManager;
class TMceMccComEvent;
class CMccCodecInformation;
class CMccInterface;
class CMceDtmfHandler;
class CMceSrvEndpoint;


const TBool KMceSrvStreamCallback = ETrue;
const TBool KMceSrvStreamNoCallback = EFalse;
const TBool KMceSrvStreamDefaultSequence = EFalse;
const TBool KMceSrvStreamAlternativeSequence = ETrue;
const TBool KMceSrvStreamForce = ETrue;
const TBool KMceSrvStreamNoForce = EFalse;
const TBool KMceSrvStreamSync = ETrue;
const TBool KMceSrvStreamNoSync = EFalse;



/**
 * 
 *
 * @lib 
 */
class CMceSrvStream: public CBase
    {

public: // enums

    /**
    * Current state in MCC 
    */
    enum TState
        {
        EAdopted = 1,
        ECreated,
        ECreatingLink,
        ELinkCreated,
        EPreparing,
        EPrepared,
        EStarting,
        EPending,
        EStopped,
        EInactive,
        EStarted,
        EPausing,
        EPaused
        };

        
public: // Constructors & Destructor


    /**
    * Decodes stream to array of server streams.
    * This is factory method
    * @return array of server streams
    */
    static void DecodeL( RPointerArray<CMceSrvStream>& aStreams,
                         CMceComMediaStream& aStream,
                         CMceMediaManager& aManager );
    
    
    /**
    * Creates srv stream.
    */
    static CMceSrvStream* NewL( CMceMediaManager& aManager,
                                CMceComMediaStream& aData,
                                CMceComMediaSource& aSource,
                                CMceComMediaSink& aSink,
                                CMceComCodec& aCodec );
                                
    /**
    * Destructor.
    */
     ~CMceSrvStream();

    
public: // static API

    static CMceSrvStream* EventReceived( 
                        RPointerArray<CMceSrvStream>& aStreams,
                        TMceMccComEvent& aEvent,
                        TBool aConsumeOnlyOnce = EFalse );
                                        

public: // virtual functions


    /**
    * Gets session id
    * @return session id
    */
    virtual TUint32 SessionId() const;
    
    /**
    * Gets link id
    * @return link id
    */
    virtual TUint32 LinkId() const;

    /**
    * Sets link id
    * @param aLinkId link id
    */
    virtual void SetLinkId( TUint32 aLinkId );
    
    /**
    * Gets IAP id
    * @return IAP id
    */
    virtual TUint32 IapId() const;
    
    /**
    * Gets local media port of stream
    * @return local media port of stream
    */
    virtual TUint LocalMediaPort() const;

    
    /**
    * Handles event from MCC
    * @return status of operation
    */
    virtual TInt EventReceived( TMceMccComEvent& aEvent, TBool aCallback = KMceSrvStreamCallback );

    /**
    * Cleans up unused sinks/sources from mcc
    */
    virtual void Cleanup();

    /**
    * Checks, if this is equal to another stream
    * @param aStream an another stream
    * @return ETrue, if this is equal
    */
    virtual TBool IsEqual( const CMceSrvStream& aStream ) const;

    /**
    * Merges with stream from cloned session
    */
    virtual void Merge( CMceSrvStream& aMergeWith, TBool aDeepMerge = EFalse );

    /**
    * UnMerges with stream from cloned session
    */
    void UnMergeL( CMceSrvStream& aUnMergeFrom );

    /**
    * Is Prepared by MCC
    * @return ETrue, if so
    */
    virtual TBool IsMccPrepared() const;

    /**
    * Returns state of stream, which depends on states of
    * sink and source
    * @return the state
    */
    virtual TState State() const;

    /**
    * Invalidates the stream
    */
    virtual void Invalidate();
    
    /**
    * Is adopted
    */
    virtual TBool IsAdopted() const;
    
public: // Data access etc

    /**
    * Sets local media port of stream
    * @param aPort local media
    */
    void SetLocalMediaPort( TUint aPort );

    /**
    * Gets id
    * @return id
    */
    TUint32& Id();

    /**
    * Gets stream type
    * @return stream type
    */
    CMceComMediaStream::TStreamType StreamType() const;
    
    /**
    * Gets direction
    * @return direction
    */
    CMceComMediaStream::TStreamType Direction() const;
    
    /**
    * Is Merged
    * @return ETrue, if merged
    */
    TBool IsMerged() const;
    
    /**
    * Gets the original com stream
    * @return the original com stream
    */
    CMceComMediaStream& Data() const;

    /**
    * Gets remote ip address
    * @return  remote ip address
    */
    TInetAddr RemoteIpAddress() const;

    /**
    * Gets codec of the mcc stream. 
    * @return codec of the mcc stream. 
    */
    CMceComCodec& Codec() const;

    /**
    * Gets source of the mcc stream.
    * @return source of the mcc stream.
    */
    CMceSrvSource& Source() const;

    /**
    * Gets sink of the mcc stream. 
    * @return sink of the mcc stream. 
    */
    CMceSrvSink& Sink() const;
    

    /**
    * Gets mcc stream type
    * @return mcc stream type
    */
    TInt MccStreamType() const;

    /**
    * Gets mcc link type
    * @return mcc link
    */
    TInt LinkType() const;

    /**
    * Checks, if received event matches
    * @param aEvent an event
    * @return ETrue, if there is a match
    */
    TBool Consumes( const TMceMccComEvent& aEvent ) const;
    
    /**
    * Returns dtmf handler for the stream.
    * @return dtmf handler
    */
    CMceDtmfHandler& DtmfHandlerL( CMccInterface& aMccInterface );
    
    /**
    * Updates sink/source's "is enabled" status.
    * @param aSinkAffected if sink status is changed
    * @param aSourceAffected if source status is changed
    * @param aIsEnabled
    */
    void UpdateEndpointStates( TBool aSinkAffected, 
                               TBool aSourceAffected, 
                               TBool aIsEnabled );
    
    /**
    * Checks whether stream is associated with certain endpoint.
    * @param aEndpoint search term
    * @param aUseProxyMatch
    */
    TBool EndpointMatch( const CMceComEndpoint& aEndpoint, 
                         TBool aUseProxyMatch );
    
public: // new 


    /**
    * Hook to allow stream to have alternative create link sequence
    * @return EFalse to indicate that normal link creation can 
    * take place. ETrue indicates that link creation is not needed
    */
    TBool CreateLinkL();
    
    /**
    * Hook to allow stream to have alternative preparation
    * @return EFalse to indicate that normal preparation can 
    * take place. ETrue indicates that stream has been prepared 
    * by itself
    */
    TBool PrepareL();

    /**
    * Hook to allow stream to have alternative starting
    * @return EFalse to indicate that normal starting can 
    * take place. ETrue indicates that stream has been started 
    * by itself
    */
    TBool StartL();

    /**
    * Hook to allow stream to have alternative updating
    * @return EFalse to indicate that normal updating can 
    * take place. ETrue indicates that stream has been updated 
    * by itself
    */
    TBool SynchronizeL();

    /**
    * Hook to allow stream to have alternative stopping
    * @return EFalse to indicate that normal stopping can 
    * take place. ETrue indicates that stream has been stopped 
    * by itself
    */
    TBool Stop();

    /**
    * Hook to allow stream to have alternative validation
    * @return EFalse to indicate that normal validation can 
    * take place. ETrue indicates that stream has been validated 
    * by itself
    */
    TBool Validate();
    
    /**
    * Called as a part of session unmerge
    */
    void UnMerge();
    
    /**
    * Checks if update requires signalling
    * @param aCurrent current stream, if NULL this is a new stream
    * @param aMccCurentCodec current codec, if NULL this is a new stream
    * @param aMccUpdateCodec codec based on update (this)
    * @return indication of signaling required
    */
    TInt RequireSignalling( CMceSrvStream& aCurrent,
                             CMccCodecInformation& aMccCurentCodec, 
                             CMccCodecInformation& aMccUpdateCodec ) const;
                             
    /**
    * Called when new stream has beed detected in update
    * @param aUpdateStreams new streams from update
    * @return indication of signaling required
    */
    TInt RequireSignalling( RPointerArray<CMceSrvStream>& aUpdateStreams ) const;
    
    
    /**
    * Called after prepare is called for this
    * stream
    */
    void MccPrepareCalledL();
    
    
protected://CMcc

    /**
     * C++ default constructor.
     */
    CMceSrvStream();
    

    /**
     * C++ default constructor.
     */
    CMceSrvStream( CMceMediaManager* aManager,
                   CMceComMediaStream* aData,
                   CMceComCodec* aCodec );
                   
    void ConstructL( CMceComMediaSource& aSource,
                     CMceComMediaSink& aSink );


private: // 


    static void DecodeAudioL( RPointerArray<CMceSrvStream>& aStreams,
                              CMceComAudioStream& aAudio,
                              CMceMediaManager& aManager );
                              
    static void DecodeVideoL( RPointerArray<CMceSrvStream>& aStreams,
                              CMceComVideoStream& aVideo,
                              CMceMediaManager& aManager );
                              
    TBool UseDefaultStartupSequence();
    
    TBool StreamStateChangeEventReceived( TMceMccComEvent& aEvent );
    
    void DoEndpointDisable( CMceSrvEndpoint& aEndpoint );
                                      
protected://not owned data

    CMceMediaManager* iManager;

protected: // owned data
    
    CMceSrvSource* iSource;
    CMceSrvSink* iSink;
    CMceComMediaStream* iData;
    CMceComCodec* iCodec;

    TBool iMerged;
    
    CMceDtmfHandler* iDtmfHandler;
    
protected: // NOT owned data

    TUint32 iID;

    friend class CMceAdoptedSrvStream;
    
	//definitions for unit testing
	MCEMM_UT_DEFINITIONS
    
    };



#endif //CMCESRVSTREAM_H
