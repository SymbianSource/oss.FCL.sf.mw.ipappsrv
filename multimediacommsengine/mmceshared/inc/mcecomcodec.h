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




#ifndef CMCECOMCODEC_H
#define CMCECOMCODEC_H

//  INCLUDES
#include <e32base.h>
#include "mcedefs.h"
#include "mceclientserver.h"
#include "mcecomfactory.h"
#include "mcecomserializable.h"


typedef TBuf8<KMceMaxSdpNameLength> TMceComCodecName;

#ifdef MCE_COMMON_SERVER_SIDE
#include "mcemediadefs.h"

class CMccCodecInformation;
class TMceMccComEvent;
class CMceSrvStream;

#endif//MCE_COMMON_SERVER_SIDE
    
class TMceComFactory;
class TMceComAudioCodecFactory;

//  CONSTANTS
const TBool KMceCodecFmtpReset = ETrue;
const TBool KMceCodecFmtpAppend = EFalse;
_LIT8( KMceFmtpSeparator, "; ");

const TUint8 KMinDynPayloadType( 96 );
const TUint8 KMaxDynPayloadType( 127 );

// MACROS

class CMceComCodec : public CBase,
                     public MMceComSerializable

    {


public:  // Constructors and destructor


    /**
    * C++ constructor
    */
    CMceComCodec();

    /**
    * C++ destructor
    */
    ~CMceComCodec();
    
    /**
    * Second phase constructor
    */
    void ConstructL( CMceComCodec& aCodec );
    
    /**
    * ID 
    * @return ID
    */
	TMceMediaId Id() const;

	/**
    * Parent object
    * @return Parent object
    */
	CMceComMediaStream* Stream() const;
    
    /**
    * Set codec specific fmtp line
    * @param aReset if ETrue reset fmtp, EFalse append
    */
    virtual void SetFmtpAttributeL( const TDesC8& aFmtpAttribute, 
                                    TBool aReset = KMceCodecFmtpReset );
                                                                        
    /**
    * Sets keep alive data
    * @param keep alive data
    */
    void SetKeepAliveDataL( const TDesC8& aKeepAliveData );
    
    /**
    * Sets bitrate used with codec for encoding.
    * @param aBitrate bitrate value for encoding
    * @return KErrNotSupported if codec doesn't support bitrate
    *         value issued
    */
    virtual TInt SetBitrate( TUint aBitrate );
    
    /**
    * Sets bitrates allowed with codec.
    * @param aBitrates allowed bitrate values
    * @return KErrNotSupported if codec doesn't support bitrate
    *         values issued
    */
    virtual TInt SetAllowedBitrates( TUint aBitrates );

    /**
    * Sets the codec specific mode.
    * @param aCodecMode mode of the codec
    * @return KErrNotSupported if codec doesnt' support codec mode
    *         value issued
    */
    virtual TInt SetCodecMode( TUint aCodecMode );
    
    /**
    * Sets the payload type.
    * @param aPayloadType type identifier of the payload
    * @return KErrNotSupported if codec doesn't support payload type issued
    */
    virtual TInt SetPayloadType( TUint8 aPayloadType );
    
    /**
    * Returns max bit rate
    * @return max bit rate
    */
    virtual TUint GetMaxBitRate();
    
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


public: //new (virtual) functions
    
    /**
    * Updates this object
    * @param aUpdate object updates this
    */
    virtual void UpdateL( CMceComCodec& aUpdate );
    
    /**
    * Return ETrue, if this codec decodes rtpmap value
    * @param aRtpmapValue rtp map
    * @param aFmtpValue fmtp
    * @param aIsStrictMatch, EFalse if matching is less strict
    * @return ETrue, if this codec decodes rtpmap value
    */
    virtual TBool Decodes( const TDesC8& aRtpmapValue, 
                           const TDesC8& aFmtpValue,
                           TBool aIsStrictMatch = ETrue );

    /**
    * Initializes 
    * @param aParent the parent
    */
    virtual void InitializeL( CMceComMediaStream& aParent );
    
    /**
    * Traversal event handler
    * @param aEvent the event
    * @return status, if event was consumed or not or object needs update
    */
    virtual TInt EventReceivedL( TMceEvent& aEvent );
    
    /**
    * Resolve allowed bitrates based on given bitrate value.
    * @param aBitrate
    * @param aAllowedBitrates, on return contains allowed bitrates
    * @return error code
    */
    virtual TInt ResolveAllowedBitrates( TUint aBitrate, TUint& aAllowedBitrates );
    
    /**
    * Sets codec enabled/disabled.
    * @param aIsEnabled
    */
    virtual void SetEnabled( TBool aIsEnabled );
    
    /**
    * Check if codec can send.
    * @return ETrue if can send
    */
    virtual TBool SendSupported();

public: //new functions

    /**
    * Gets enabled state.
    * @return ETrue if enabled, EFalse if disabled
    */
    TBool IsEnabled() const; 

    /**
    * Factory
    * @return factory
    */
    TMceComFactory BaseFactory();
    
    /**
    * Gets current fmtp attribute
    * @return fmtp attribute
    */   
    TPtrC8 FmtpAttr() const;
    
#ifdef MCE_COMMON_SERVER_SIDE

public: //new functions

    /**
    * Synchronizes itself with another codec, if possible
    * @param aCodec codec to be synchornized with
    * @param aRoleAnswerer is answerer
    */
    virtual void SynchronizeL( CMceComCodec& aCodec, TBool aRoleAnswerer );

    /**
    * Assigns mcc codec
    * @param aMccCodec mcc codec
    * @return assigned codecs
    */
    virtual CMceComCodec& operator=( CMccCodecInformation& aMccCodec ) = 0;
    
    /**
    * Decodes to mcc codec
    * @param aMccCodec mcc codec
    */
    void MccDecodeL( CMccCodecInformation& aMccCodec );

    /**
    * Populates mcc codec
    * @param aMccCodec mcc codec
    * @param aStream stream
    * @param aRole role
    */
    void MccPopulateL( CMccCodecInformation& aMccCodec, 
                       CMceSrvStream& aStream,
                       TMceNegotiationRole aRole );

    /**
    * Validates codec values based on mcc codec
    * @param aMccCodec mcc codec
    * @param aStream stream
    * @param aRole role
    * @return Other codec which is affected because of values or
    *   NULL if none is affected
    */
    CMceComCodec* MccValidateL( CMccCodecInformation& aMccCodec,
                                CMceSrvStream& aStream,
                                TMceNegotiationRole aRole );

    /**
    * Adjust codec values based on mcc codec
    * @param aMccCodec mcc codec
    * @param aStream stream
    */
    void MccAdjustL( CMccCodecInformation& aMccCodec, 
                     CMceSrvStream& aStream );
    
    /**
    * Checks, if signalling is required
    * @param aStream stream
    * @param aMccCurentCodec current mcc codec
    * @param aMccUpdateCodec to be used mcc codec
    */
    TInt MccRequireSignalling( 
                        const CMceSrvStream& aStream,
                        const CMccCodecInformation& aMccCurentCodec, 
                        const CMccCodecInformation& aMccUpdateCodec ) const;
   
    /**
    * Sets state of codec based on event
    * @param aEvent event
    */
    void SetState( TMceMccComEvent& aEvent );
    
    /**
    * Set default fmtp line
    */                                    
    void SetDefaultFmtpAttributeL();

    void Merge( CMceComCodec& aMergeWith );
    
    /**
    * Used for storing fmtp proposal but the value is not set as official
    * fmtp of the codec. Pre-check for fmtp validity
    * @param aFmtpProposal
    * @return ETrue if fmtp is valid
    */
    virtual TBool SetFmtpProposalL( const TDesC8& aFmtpProposal );
 
    /**
    * Set fmtp
    * @param aReset if ETrue reset fmtp, EFalse append
    */
    virtual void SetFmtpFromIncomingSdpL( const TDesC8& aFmtpAttribute, 
                                          TBool aRoleAnswerer,
                                          TBool aReset = KMceCodecFmtpReset );
    
    /**
    * Create fmtp for outgoing sdp
    * @return fmtp, ownership is transferred
    */
    virtual HBufC8* FtmpForOutgoingSdpLC();

protected://abstract methods


    /**
    * Decodes to mcc codec
    * @param aMccCodec mcc codec
    */
    virtual void DoMccDecodeL( CMccCodecInformation& aMccCodec ) = 0;

    /**
    * Encodes from mcc codec
    * @param aMccCodec mcc codec
    * @param aStream stream
    */
    virtual void DoMccEncodeL( CMccCodecInformation& aMccCodec,
                               CMceSrvStream& aStream ) = 0;
    
    /**
    * Validates codec values based on mcc codec
    * @param aMccCodec mcc codec
    * @param aStream stream
    * @param aRole role
    * @return Other codec which is affected because of values or
    *   NULL if none is affected
    */
    virtual CMceComCodec* DoMccValidateL( CMccCodecInformation& aMccCodec,
                                          CMceSrvStream& aStream,
                                          TMceNegotiationRole aRole );
                               
    /**
    * Adjust codec values based on mcc codec
    * @param aMccCodec mcc codec
    * @param aStream stream
    */
    virtual void DoMccAdjustL( CMccCodecInformation& aMccCodec, 
                               CMceSrvStream& aStream ) = 0;
                             
    /**
    * Checks, if signalling is required
    * @param aStream stream
    * @param aMccCurentCodec current mcc codec
    * @param aMccUpdateCodec to be used mcc codec
    */
    virtual TInt DoMccRequireSignalling( 
                                const CMceSrvStream& aStream,
                                const CMccCodecInformation& aMccCurentCodec, 
                                const CMccCodecInformation& aMccUpdateCodec ) const = 0;
  
    /**
    * Set default fmtp line
    */                                    
    virtual void DoSetDefaultFmtpAttributeL() = 0;
        
    /**
    * Encodes common values from mcc codec
    * @param aMccCodec mcc codec
    * @param aStream stream
    */
    void EncodeCodecCommonL( CMccCodecInformation& aMccCodec );
    
    /**
    * Decodes common values to mcc codec
    * @param aMccCodec mcc codec
    */
    void DecodeCodecCommonL( CMccCodecInformation& aMccCodec );

    
private:

    /**
    * Encodes from mcc codec
    * @param aMccCodec mcc codec
    * @param aStream stream
    */
    void MccEncodeL( CMccCodecInformation& aMccCodec, CMceSrvStream& aStream );
    
    /**
    * Updates codec based on mcc codec
    * @param aMccCodec mcc codec
    */
    void UpdateL( CMccCodecInformation& aMccCodec );
    

#endif//MCE_COMMON_SERVER_SIDE

protected: //new functions

    void ConstructL( TMceComCodecName aSdpName );
    

public:// data
    
    /**
     * type of session
     */
    TMceMediaId iID;
	
	/**
     * Fmtp attribute
     */
    HBufC8* iFmtpAttr;

	/**
    * Sdp name used in SDP messages
    */
	TMceComCodecName iSdpName;		

	/**
    * Payload type
    */
	TUint8 iPayloadType;

    /**
     * Is enabled
     */
    TBool iIsEnabled;
    
   	/**
    * Stream's bit rate
    */
	TUint iBitrate;

	/**
    * Stream's allowed bit rates
    */
	TUint32 iAllowedBitrates;

	/**
    * Codec mode ( e.g. PCMU, PCMA, Bandwidth efficient, octet aligned )
    */
	TUint32 iCodecMode;		
	
	/**
	* Codec FourCC
	*/
	TUint32 iFourCC;

	/**
    * Codecs frame size
    */
	TUint iFrameSize;

	/**
    * Codecs state of the codec
    */
	TInt iState;
    
	/**
    * MMF priority of the codec
    */
	TInt iPriority;

	/**
    * MMF priority preference of the codec
    */
	TInt iPriorityPreference;
	
	/**
    * Keep alive timer
    */
    TUint8 iKeepAliveTimer;
    
	/**
    * Keep Alive Payload Type
    */
    TUint8 iKeepAlivePayloadType;
    
	/**
    * Keep Alive Data
    */
    HBufC8* iKeepAliveData;
   
    /**
    *
    */
    TBool iIsNegotiated;
    
    TInt iCodecSdpIndex;
    
    TInt iPreference;
    
    /**
     * Fmtp proposal
     */
    HBufC8* iFmtpProposal;
    
protected: // NOT owned data
	
	
    /**
     * stream
     */
	CMceComMediaStream* iStream;
    

public: //inner classes

    class TIterator
        {

    public:
        
        enum TFilter
            {
            EFilterNone = 0,
            EFilterIsNegotiated,
            EFilterAssociatedLocal
            };
        enum TStreamStateMatchType
            {
            EExactMatch,
            EOtherSdpName
            };
            
    public:
        
            TIterator( const RPointerArray<CMceComCodec>& aCodecs, 
                       TFilter aFilter =  EFilterNone,
                       CMceComCodec* aCodec = NULL );
            
            TBool IsEof();
            TBool Next( CMceComCodec*& aCandidate,
                        TIterator::TStreamStateMatchType aMatchType = EExactMatch );
            
            void Reset();
            TInt Count();
            
            void SetFilter( TFilter aFilter, CMceComCodec* aCodec = NULL );
            
    private:
        
            TFilter iFilter;
            const RPointerArray<CMceComCodec>& iCodecs;
            TInt iCurrentIndex;
            CMceComCodec* iCodec;
    
        };
    
    #ifdef EUNIT_TEST
    	friend class UT_CMceComCodec;
	#endif
    };



#endif//CMCECOMCODEC_H

// End of File