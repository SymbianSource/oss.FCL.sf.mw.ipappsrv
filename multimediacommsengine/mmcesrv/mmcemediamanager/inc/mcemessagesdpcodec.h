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




#ifndef CMCEMESSAGESDPCODEC_H
#define CMCEMESSAGESDPCODEC_H

#include "mcemediasdpcodec.h"
#include "mcecommediastream.h"
#include "mcecommsrpsource.h"
#include "mcecommsrpsink.h"
#include <sdpmediafield.h>

class CMceComMessageStream;
//class CMceComAudioCodec;


#define MCE_SDP_STRING_MESSAGEL()\
        SdpCodecStringPool::StringPoolL().OpenFStringL(KMceMediaMessage)

struct TParsedHeader
    {
    TBuf8<256> iNameValue;
    TBuf8<50> iTypeValue;
    TPtrC8 iHash;
    TInt64  iSize;
    TBuf8<32> iFileTransferId;
    };

/**
 * 
 *
 * @lib 
 */
class CMceMessageSdpCodec: public CMceMediaSdpCodec
    {

    public://constructor and destructor
    
        /**
        * Constructor
        */
        static CMceMessageSdpCodec* NewL();
        
        /**
        * Destructor
        */
        ~CMceMessageSdpCodec();
        

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
        * Encodes message media related attributes
        * @param aStream for media stream
        * @param aMediaLine media line        
        * @leave system wide error
        */
        void EncodeMessageMediaAttributesL( CMceComMessageStream& aStream, 
                CSdpMediaField& aMediaLine);

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
        CMceMessageSdpCodec( RStringF aMedia );

        /**
        * Creates uplink audio stream with mic and rtp sink
        * @return uplink audio stream
        */
        CMceComMessageStream* CreateUplinkStreamL() const;
        
        /**
        * Creates downlink audio stream with rtp source and speaker
        * @return downlink audio stream
        */
        CMceComMessageStream* CreateDownlinkStreamL() const;
        
        /**
         * Creates fmtlist that can be used as media field fmt list -attribute
         * @param aCodecs codecs
         * @return fmtp list
         */
        HBufC8* CreateFormatListL( CMceComCodec::TIterator& aCodecs ) const;
        
        /**
         * Decodes media line's format list to payload type array
         * @param aMediaLine Media line
         * @param aPayloadTypes payload types as an array
         */
        void DecodeFormatListL( CSdpMediaField& aMedia, 
                        RArray<TUint>& aPayloadTypes ) const;
        
        /**
        * Decodes media line's format list to payload type array
        * @param aMediaLine Media line
        * @param aPayloadTypes payload types as an array
        * @return  system wide error
        */
        TInt DecodeFormatList( CSdpMediaField& aMedia, 
                               RArray<TUint>& aPayloadTypes ) const;
        
        /**
         * Decodes static payloads if payload wasn't defined as rtpmap
         * @param aMediaLine Media line
         * @param aStream stream
         * @param aPayloadTypes payload types as an array
         * @param aRole role of negotiation
         * @return number of lines decoded
         */
        TInt DecodeStaticPayloadsL( CSdpMediaField& aMediaLine,
                CMceComMediaStream& aStream,
                RArray<TUint>& aPayloadTypes,
                TMceNegotiationRole aRole ) const;
        
        /**
         * If the path attribute is set for the message streams then this funtion
         * extracts the host and port informaiotn and sets accordinlgy to the 
         * media stream
         */
        void SetHostAddrFromPathAttrL(const TUriC8& aMsrpUri, 
                CSdpMediaField& aMediaLine);
        
        /**
         * Set MSRP path SDP attribute  to the media line
         */
        void SetPathAttributeL(CMceComMessageStream& aMsgStream, 
                CMceComMsrpSource* msrpSource, 
                CMceComMsrpSink* msrpSink,
                CSdpMediaField& aMediaLine);
        
        /**
         * Set MSRP SDP AcceptTypes attribte to the media line 
         */
        void SetAcceptTypesAttributeL(CMceComMsrpSource* msrpSource, CMceComMsrpSink* msrpSink, 
                     CSdpMediaField& aMediaLine);
        
        /**
         * Set MSRP SDP AcceptWrappedTypes attribte to the media line
         */
        void SetAcceptWrappedAttributeL(CMceComMsrpSource* msrpSource, CMceComMsrpSink* msrpSink,
                     CSdpMediaField& aMediaLine);
        
        /*
         * Set file share attribure
         */
        void SetFileShareAttributeL(CMceComMsrpSource* msrpSource, CMceComMsrpSink* msrpSink,
                     CSdpMediaField& aMediaLine);
        
        /*
         * Parsing remotesdplines
         */
        TInt ParseSomeHeader(const TDesC8& aHeaderToParse, TParsedHeader& aParsedHeader) const;
        
        /**
         * Creates File-Selector attributes and appends it to media line
         * 
         */
        
        void EncodeFileAttributeL( CMceComMediaStream& aStream,
                                   CSdpMediaField& aMediaLine ) const;
        
        /**
         * Sets the MSRP connection usage for the MSRP based m-lines
         */
        void SetConnectionAttributeL(CMceComMessageStream& aStream,
                CSdpMediaField& aMediaLine);
        
        void DecodeClientAttributesL( CSdpMediaField& aMediaLine, 
                                      CMceComMediaStream& aStream ) const; 
        
    private:        
        RStringF iFileSelectorStr;
        RStringF iFileTransferidStr;    
        RStringF iFileDispositionStr;
        RStringF iAcceptWrappedTypesStr;
        RStringF iPathStr;
        RStringF iAcceptTypesStr;
        RStringF iNetType;
        RStringF iAddressType;
        RStringF iConnectionStr;
    };



#endif //CMceMessageSdpCodec_H

