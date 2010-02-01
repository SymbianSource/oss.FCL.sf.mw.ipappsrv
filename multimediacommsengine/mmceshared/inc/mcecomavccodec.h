/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef MCECOMAVCCODEC_H
#define MCECOMAVCCODEC_H

//  INCLUDES
#include <mceavccodec.h>
#include "mcecomvideocodec.h"
#include "mcedefs.h"

const TReal KMceAvcDefaultFrameRate = 15;
const TInt  KMceAvcDefaultFrameSize = 10000;
const TInt  KMceAvcDefaultAllowedBitrate = KMceAvcCodecProfileIdBaseline | 
	                                       KMceAvcCodecProfileIopConstraintSet | 
	                                       KMceAvcBitrateLevel1;

const TInt KMceAvcAllowedBitrateRecommendedHigh = KMceAvcCodecProfileIdBaseline | 
	                                              KMceAvcCodecProfileIopConstraintSet | 
	                                              KMceAvcBitrateLevel1b;
	                                   
const TInt KMceAvcLevel1BitrateBps = 64000;
const TInt KMceAvcLevel1bBitrateBps = 128000;
const TInt KMceAvcLevel11BitrateBps = 192000;
const TInt KMceAvcLevel12BitrateBps = 384000;
const TInt KMceAvcLevel13BitrateBps = 768000;
const TInt KMceAvcLevel2BitrateBps = 2000000;

const TUint KMceAvcDefaultFrameHeight = 176;
const TUint KMceAvcDefaultFrameWidth = 144;


// CLASS DECLARATION
/**
*  Container class for the codec information.
*
*  @lib 
*/
class CMceComAvcCodec : public CMceComVideoCodec
    {
public:  // Constructors and destructor

    /**
    * Two-phased constructor.
    * @param aSdpName sdp name
    */
    static CMceComAvcCodec* NewL( TBuf8<KMceMaxSdpNameLength> aSdpName );
    
    /**
    * Two-phased constructor.
    * @param aSdpName sdp name
    */
    static CMceComAvcCodec* NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName );
    
    /**
    * Destructor.
    */
    ~CMceComAvcCodec();

public: // from CMceComCodec

    /**
    * Sets bitrate used with codec for encoding.
    * @param aBitrate bitrate value for encoding
    * @return KErrNotSupported if codec doesn't support bitrate
    *         value issued
    */
    TInt SetBitrate( TUint aBitrate );
    
    /**
    * Sets bitrates allowed with codec.
    * @param aBitrates allowed bitrate values
    * @return KErrNotSupported if codec doesn't support bitrate
    *         values issued
    */
    TInt SetAllowedBitrates( TUint aBitrates );
    
    /**
    * Returns max bit rate
    * @return max bit rate
    */
    TUint GetMaxBitRate();
    
    /**
    * Sets maximum bitrate of the codec.
    * @param aMaxBitrate value of maximum bitrate used for encoding
    * @return KErrNotSupported if codec doesn't support bitrate
    *         values issued
    */
    TInt SetMaxBitrate( TUint aMaxBitrate );
    
    /**
    * Resolve allowed bitrates based on given bitrate value.
    * @param aBitrate
    * @param aAllowedBitrates, on return contains allowed bitrates
    * @return error code
    */
    TInt ResolveAllowedBitrates( TUint aBitrate, TUint& aAllowedBitrates );
    
    /**
    * Return ETrue, if this codec decodes rtpmap value
    * @param aRtpmapValue rtp map
    * @param aFmtpValue fmtp
    * @param aIsStrictMatch, EFalse if matching is less strict
    * @return ETrue, if this codec decodes rtpmap value
    */
    TBool Decodes( const TDesC8& aRtpmapValue, 
                   const TDesC8& aFmtpValue,
                   TBool aIsStrictMatch = ETrue );

#ifdef MCE_COMMON_SERVER_SIDE

    /**
    * Used for storing fmtp proposal but the value is not set as official
    * fmtp of the codec. Pre-check for fmtp validity
    * @param aFmtpProposal
    * @return ETrue if fmtp is valid
    */
    TBool SetFmtpProposalL( const TDesC8& aFmtpProposal );

    /**
    * Set fmtp
    * @param aReset if ETrue reset fmtp, EFalse append
    */
    void SetFmtpFromIncomingSdpL( const TDesC8& aFmtpAttribute, 
                                  TBool aRoleAnswerer,
                                  TBool aReset = KMceCodecFmtpReset );
    
    /**
    * Create fmtp for outgoing sdp
    * @return fmtp, ownership is transferred
    */
    HBufC8* FtmpForOutgoingSdpLC();

private:
    
    /**
    * Removed defined parameter from input parameter and returns it
    * @return stripped parameter if found, ownership is transferred
    */
    HBufC8* StripFmtpParameterL( TPtr8& aInputFmtp, const TDesC8& aStrippedParameter );
    
    /**
    * Adds parameter into fmtp. Also separators are added if needed.
    * @return fmtp containing input fmtp and addded parameter, ownership is transferred
    */
    HBufC8* AddFmtpParameterL( const TDesC8& aInputFmtp, 
                               const TDesC8& aAddedParameter, 
                               const TDesC8& aAddedParameterVal );
            
#endif
    
public: // serialization etc

    /**
    * Clones
    * @return clone
    */
    CMceComVideoCodec* CloneL();

private:

    /**
    * C++ default constructor.
    */
    CMceComAvcCodec();

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName);

    /**
     * second-phase copy constructor
     */
    void ConstructL( CMceComAvcCodec& aCodec );
    
    /**
    * Profile-level-id value from fmtp
    */
    TPtrC8 ParseProfileLevelId( const TDesC8& aFmtpValue );
    
    /**
    * Packetization-mode value from fmtp
    */
    TPtrC8 ParsePacketizationMode( const TDesC8& aFmtpValue );
    
    /**
    * Profile-level value part from fmtp
    */
    TPtrC8 ParseProfileLevelIdValPart( const TDesC8& aFmtpValue,
                                       TInt aValPartOffset,
                                       TInt aValPartLen,
                                       const TDesC8& aDefaultVal );
    
    /**
    * Replace profile level value part of fmtp attribute.
    */
    TInt ReplaceProfileLevelIdValuePart( TDes8& aModifiedFmtp, 
                                         const TDesC8& aReplacement,
                                         TInt aValPartOffset );
    
    /**
    * Get value part of certain fmtp attribute.
    */
    TPtrC8 GetFmtpAttributeValuePart( const TDesC8& aFmtpValue,
                                      const TDesC8& aFmtpAttributeName, 
                                      TInt aValLen, 
                                      const TDesC8& aDefaultVal );

 public:
 
    TUint8 iPacketizationMode;
    
    TUint8 iAllowedPacketizationModes;   
            
    };


#endif      // MCECOMAVCCODEC_H

// End of File