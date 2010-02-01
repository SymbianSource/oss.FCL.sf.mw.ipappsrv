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




#ifndef CMCEVIDEOSDPCODEC_H
#define CMCEVIDEOSDPCODEC_H

#include "mcemediasdpcodec.h"

class CMceComVideoCodec;
class CMceComVideoStream;

#define MCE_SDP_STRING_VIDEOL()\
        SdpCodecStringPool::StringPoolL().StringF( \
        SdpCodecStringConstants::EMediaVideo,\
        SdpCodecStringPool::StringTableL() )




/**
 * 
 *
 * @lib 
 */
class CMceVideoSdpCodec : public CMceMediaSdpCodec
    {

    public://constructor and destructor
    
        /**
        * Constructor
        */
        static CMceVideoSdpCodec* NewL();
        
        /**
        * Destructor
        */
        ~CMceVideoSdpCodec();
        

    public: //from CMceSdpCodec
        
        /**
        * Returns ETrue, if media is encoded by this codec
        * @return ETrue, if media is encoded by this codec
        */
        TBool Encodes( const CMceComMediaStream& aStream ) const;
        
        
    public: //from CMceMediaSdpCodec
    

        /**
        * Returns codecs
        * @param aStream media stream
        * @param return codecs
        * @leave if stream type does not macth codecs
        */
        const RPointerArray<CMceComCodec>& CodecsL( CMceComMediaStream& aStream ) const;

        /**
        * Encodes rtpmap -attribute
        * @param aCodec codec encoded to rtpmap
        * @leave system wide error
        */
        CSdpFmtAttributeField* EncodeRtpmapAttributeLC( CMceComCodec& aCodec ) const;
        
        /**
        * Encodes media related attributes
        * @param aCodec the codec
        * @param aMediaLine media line
        * @param aRtpmap rtpmap attribute
        * @leave system wide error
        */
        void EncodeMediaAttributesL( CMceComCodec& aCodec, 
                                     CSdpMediaField& aMediaLine, 
                                     CSdpFmtAttributeField& aRtpmap ) const;

        /**
        * Decodes media related attributes
        * @param aMediaLine media line
        * @param aCodec the codec
        * @leave system wide error
        */
        
		void DecodeMediaAttributesL( CSdpMediaField& aMediaLine,
                                     CMceComCodec& aCodec,
                                     CSdpFmtAttributeField& aRtpmap ) const;		                                     

        /**
        * Creates media stream based on sdp type
        * @param aType the sdp type sendrecv, sendonly or receive only
        * @return media stream
        * @leave system wide error
        */
        CMceComMediaStream* CreateStreamLC( TInt aType ) const;
        
        /**
        * Updates media based on received offer
        * @param aStream media stream
        * @param aDirection the direction 
        * @leave system wide error
        */
        void UpdateStreamL( CMceComMediaStream& aStream, TInt aDirection ) const;
        
        /**
        * Creates codec based on rtpmap line
        * @param aRtpmap rtpmap line
        * @return codec or NULL, if not supported
        * @leave system wide error
        */
        CMceComCodec* CreateCodecLC( CSdpFmtAttributeField& aRtpmap ) const;
        
        /**
        * Creates codec based on payload type
        * @param aPayload payload
        * @param aMediaLine media line
        * @return codec or NULL, if not supported
        * @leave system wide error
        */
        CMceComCodec* CreateCodecLC( TUint aPayload, CSdpMediaField& aMediaLine ) const;
        
        /**
        * Decodes session level media attributes
        * @param aStream media stream
        * @param aSdpDocument sdp document
        * @return codec or NULL, if not supported
        * @leave system wide error
        */
        void DecodeSessionMediaAttributesL( CMceComMediaStream& aStream, 
                                            CSdpDocument& aSdpDocument ) const;
        
    private:

    
        /**
        * C++ Constructor
        * @param aMedia media
        */
        CMceVideoSdpCodec( RStringF aMedia );
        
        /**
        * 2nd phase constructor
        */
        void ConstructL();
        

        /**
        * Encodes framerate attribute
        * @param aCodec the codec
        * @param aMediaLine media line
        * @param aRtpmap rtpmap attribute
        * @leave system wide error
        */
        void EncodeFrameRateAttributeL( CMceComVideoCodec& aCodec,
                                        CSdpMediaField& aMedia,
                                        CSdpFmtAttributeField& aFmtpline ) const;
    
        /**
        * Encodes framesize attribute
        * @param aCodec the codec
        * @param aMediaLine media line
        * @param aRtpmap rtpmap attribute
        * @leave system wide error
        */
        void EncodeFrameSizeAttributeL( CMceComVideoCodec& aCodec,
                                        CSdpMediaField& aMedia,
                                        CSdpFmtAttributeField& aFmtpline ) const;


        /**
        * Creates uplink video stream with camera and rtp sink
        * @return uplink video stream
        */
        CMceComVideoStream* CreateUplinkStreamL() const;
        
        /**
        * Creates downlink video stream with rtp source and display
        * @return downlink video stream
        */
        CMceComVideoStream* CreateDownlinkStreamL() const;
        
    
    
    private:
    
        RStringF iFrameSizeAttr;
        
    
    };



#endif //CMCEVIDEOSDPCODEC_H
