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





#ifndef CMCEMEDIASDPCODEC_H
#define CMCEMEDIASDPCODEC_H

//  INCLUDES
#include "mcesdpcodec.h"
#include "mcecomcodec.h"

const TInt KMceMaxPTValue = 127;
const TInt KMceMaxPayloadTypes = 20;

const TBool KMceMediaCodecUpdate = ETrue;
const TBool KMceMediaCodecNoUpdate = EFalse;

/**
*  Class for encoding and decoding media lines and creating server streams
*  
*/
class CMceMediaSdpCodec : public CMceSdpCodec
    {

    public:
    
        /**
        * destructor
        */
        ~CMceMediaSdpCodec();
        
    public: //from CMceSdpCodec
    
    
        /**
        * Encodes a media line. 
        * @param aStream media stream
        * @param aMediaLine media line
        * @param aSdpDocument sdp document
        * @leave system wide error
        */ 
        void EncodeMediaOfferL( CMceComMediaStream& aStream,
                                CSdpMediaField& aMediaLine, 
                                CSdpDocument& aSdpDocument );
        
        /**
        * Decodes the media answer.
        * @param aStream media stream
        * @param aSdpDocument sdp document
        * @return warning if any 
        * @leave system wide error
        */
        TMceSipWarningCode DecodeMediaAnswerL( CSdpMediaField& aMediaLine, 
                                 CMceComMediaStream& aStream,
                                 CSdpDocument& aSdpDocument );
        
        /**
        * Decodes media offer and creates media stream. The stream is appended
        * into the media session, if aStream is NULL
        * @param aMediaLine media line
        * @param aStream stream added to session
        * @param aSession media session
        * @param aSdpDocument sdp document
        * @param aStream stream to updated, if any
        * @return warning if any 
        * @leave system wide error
        */
        TMceSipWarningCode DecodeMediaOfferL( CSdpMediaField& aMediaLine,
                                              CMceComMediaStream*& aStream,
                                              CMceComSession& aSession,
                                              CSdpDocument& aSdpDocument );

        /**
        * Decodes media update by updating media stream
        * @param aMediaLine media line
        * @param aStream stream to be updated
        * @param aSdpDocument sdp document
        * @return warning if any 
        * @leave system wide error
        */
        TMceSipWarningCode DecodeMediaUpdateL( CSdpMediaField& aMediaLine, 
                                               CMceComMediaStream& aStream,
                                               CSdpDocument& aSdpDocument );
                                              

        /**
        * Creates a encoded media line based on the offer received earlier.
        * @param aStream media stream
        * @param aMediaLine media line
        * @param aSdpDocument sdp document
        * @leave system wide error
        */
        void EncodeMediaAnswerL( CMceComMediaStream& aStream,
                                  CSdpMediaField& aMediaLine,
                                  CSdpDocument& aSdpDocument );
                                      
        /**
        * Clean attributes
        * @param aMediaLine media line
        */
        void CleanAttributes( CSdpMediaField& aMediaLine );
        
        
        /**
        * Prepares media line before offer/answer will be decoded
        * @param aMediaLine media line
        * @param aStream media stream
        * @leave system wide error
        */
        void PrepareForDecodeL( CSdpMediaField& aMediaLine,
                                CMceComMediaStream* aStream );


        
    protected://abstract interface
    
        
        /**
        * Returns codecs
        * @param aStream media stream
        * @param return codecs
        * @leave if stream type does not macth codecs
        */
        virtual const RPointerArray<CMceComCodec>& CodecsL( 
                                        CMceComMediaStream& aStream ) const = 0;
        /**
        * Encodes rtpmap -attribute
        * @param aCodec codec encoded to rtpmap
        * @leave system wide error
        */
        virtual CSdpFmtAttributeField* EncodeRtpmapAttributeLC( 
                                        CMceComCodec& aCodec ) const = 0;
        
        /**
        * Encodes media related attributes
        * @param aCodec the codec
        * @param aMediaLine media line
        * @param aRtpmap rtpmap attribute
        * @leave system wide error
        */
        virtual void EncodeMediaAttributesL( CMceComCodec& aCodec, 
                                             CSdpMediaField& aMediaLine, 
                                             CSdpFmtAttributeField& aRtpmap ) const = 0;

        /**
        * Decodes media related attributes
        * @param aMediaLine media line
        * @param aCodec the codec
        * @leave system wide error
        */
		virtual void DecodeMediaAttributesL( CSdpMediaField& aMediaLine, 
                                             CMceComCodec& aCodec,
                                             CSdpFmtAttributeField& aRtpmap ) const = 0;
		                                             

        /**
        * Creates media stream based on sdp type
        * @param aType the sdp type sendrecv, sendonly or receive only
        * @return media stream
        * @leave system wide error
        */
        virtual CMceComMediaStream* CreateStreamLC( TInt aType ) const = 0;
        
        /**
        * Updates media based on received offer
        * @param aStream media stream
        * @param aDirection the direction 
        * @leave system wide error
        */
        virtual void UpdateStreamL( CMceComMediaStream& aStream, TInt aDirection ) const = 0;
        
        /**
        * Creates codec based on rtpmap line
        * @param aRtpmap rtpmap line
        * @return codec or NULL, if not supported
        * @leave system wide error
        */
        virtual CMceComCodec* CreateCodecLC( CSdpFmtAttributeField& aRtpmap ) const = 0;

        /**
        * Creates codec based on payload type
        * @param aPayload payload
        * @param aMediaLine media line
        * @return codec or NULL, if not supported
        * @leave system wide error
        */
        virtual CMceComCodec* CreateCodecLC( TUint aPayload, CSdpMediaField& aMediaLine ) const = 0;
                
        /**
        * Decodes session level media attributes
        * @param aStream media stream
        * @param aSdpDocument sdp document
        * @return codec or NULL, if not supported
        * @leave system wide error
        */
        virtual void DecodeSessionMediaAttributesL( CMceComMediaStream& aStream, 
                                                    CSdpDocument& aSdpDocument ) const = 0;
        
    protected:
    

        /**
        * validates sdp offer. Called before offer will be decoded
        * @param aMediaLine media line
        * @param aSdpDocument sdp document
        * @return warning
        */
        virtual TMceSipWarningCode ValidateSdpL( CSdpMediaField& aMediaLine,
                                                CSdpDocument& aSdpDocument );

        /**
        * Gets the type of session ( sendrecv, sendonly or receiveonly )
        * @param aMediaLine media line
        * @param aSessionAttributes all session attributes
        * @return type
        */
        TInt Type( CSdpMediaField& aMediaLine,
                    RPointerArray<CSdpAttributeField>& aSessionAttributes ) const;
                                

        /**
        * Encodes direction attribute
        * @param aStream media stream
        * @param aMediaLine media line
        * @param aSdpDocument sdp document
        * @param aRole negotiation role
        */
        void EncodeDirectionL( CMceComMediaStream& aStream,
                               CSdpMediaField& aMediaLine,
                               CSdpDocument& aSdpDocument,
                               TMceNegotiationRole aRole ) const;
        

        /**
        * Encodes preconditions
        * @param aStream media stream
        * @param aMedia media line
        * @param aRole negotiation role
        * @leave system wide error
        */
        void EncodePreconditionsL( CMceComMediaStream& aStream,
                                   CSdpMediaField& aMediaLine,
                                   TMceNegotiationRole aRole ) const;
    
        /**
        * Decodes preconditions
        * @param aMedia media line
        * @param aStream media stream
        * @param aRole negotiation role
        * @return warning if any 
        * @leave system wide error
        */
        TMceSipWarningCode DecodePreconditionsL( CSdpMediaField& aMediaLine,
                                   CMceComMediaStream& aStream,
                                   TMceNegotiationRole aRole ) const;
        

        /**
        * Decodes rmtpmap lines in answer
        * @param aMediaLine Media line
        * @param aStream stream
        * @param aRole role of negotiation
        * @return number of lines decoded
        */
        TInt DecodeRtpmapLinesL( CSdpMediaField& aMediaLine,
                                 CMceComMediaStream& aStream,
                                 TMceNegotiationRole aRole ) const;
                                     

        /**
        * Decodes rmtpmap lines in offer, and adds codes to stream.
        * @param aMediaLine Media line
        * @param aStream stream
        * @return number of lines decoded
        */
        TInt DecodeRtpmapLinesL( CSdpMediaField& aMediaLine,
                                 CMceComMediaStream& aStream ) const;

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
        * Decodes rtpmap line. 
        * @param aRtpMaptLine rtpmap line
        * @param aMediaLine media line
        * @param aStream stream
        * @param aFmtpValue
        * @return decoded codec if codec was supported
        */
        CMceComCodec* DecodeRtpmapLineL( CSdpFmtAttributeField& aRtpMaptLine,
                                         CSdpMediaField& aMediaLine,
                                         CMceComMediaStream& aStream,
                                         const TDesC8& aFmtpValue = KNullDesC8 ) const;
 
        /**
        * Decodes fmtp lines. 
        * @pre codecs and rtpmaps are in sync 
        * @param aMediaLine Media line
        * @param aCodecs codecs
        * @param aRole the role
        */
        void DecodeFmtpLinesL( CSdpMediaField& aMediaLine,
                               CMceComCodec::TIterator& aCodecs,
                               TMceNegotiationRole aRole ) const;
                               

        /**
        * Creates fmtp attribute and appends it into media line
        * @param aCodec the codec
        * @param aMediaLine Media line
        * @return None.
        */
        void EncodeFmtpAttributeL( CMceComCodec& aCodec,
                                   CSdpMediaField& aMediaLine ) const;
                                   

        /**
        * Encodes secure session
        * @param aStream media stream
        * @param aMediaLine Media line
        * @param aRole the role
        */
        void EncodeSecureSessionL( CMceComMediaStream& aStream, 
                                   CSdpMediaField& aMediaLine,
                                   TMceNegotiationRole aRole );
                                   
                                  
        /**
        * Decodes secure session
        * @param aMediaLine Media line
        * @param aStream media stream
        * @param aRole the role
        * @param aUpdate is it update (role EMceRoleAnswerer )
        */
        void DecodeSecureSessionL( CSdpMediaField& aMediaLine,
                                  CMceComMediaStream& aStream, 
                                  TMceNegotiationRole aRole,
                                  TBool aUpdate = KMceMediaCodecNoUpdate );
                                  


        /**
        * Decodes direction and 'old school' hold
        * @param aMediaLine Media line
        * @param aStream media stream
        * @param aSdpDocument sdp document
        * @param aRole the role
        * @param aUpdate is it update 
        */
        void DecodeDirectionL( CSdpMediaField& aMediaLine,
                                   CMceComMediaStream& aStream,
                                   CSdpDocument& aSdpDocument,
                                   TMceNegotiationRole aRole ) const;
                                  
                                  
        /**
        * Update direction and 'old school' hold
        * @param aMediaLine Media line
        * @param aStream media stream
        * @param aSdpDocument sdp document
        */

        void UpdateDirectionL( CSdpMediaField& aMediaLine, 
                                              CMceComMediaStream& aStream,
                                              CSdpDocument& aSdpDocument) const;

        /**
        * Creates fmtlist that can be used as media field fmt list -attribute
        * @param aCodecs codecs
        * @return fmtp list
        */
        HBufC8* CreateFormatListL( CMceComCodec::TIterator& aCodecs ) const;
        
        
       /**
        * decodes the remote RTCP port according to RFC 3605
        * @param aMediaLine Media line
        * @param aStream media stream
        */
        void DecodeRemoteRtcpFieldL( CSdpMediaField& aMediaLine, 
                                     CMceComMediaStream& aStream ) const;  
        
        /**
        * encodes the remote RTCP port according to RFC 3605
        * @param aMediaLine Media line
        * @param aStream media stream
        */
        void EncodelocalRtcpAttrL( CSdpMediaField& aMediaLine, 
                                     CMceComMediaStream& aStream ) const; 
        
        /**
        * Encodes media attributes client is willing to insert
        * @param aStream media stream
        * @param aMediaLine media line
        */
        void EncodeClientAttributesL( CMceComMediaStream& aStream,
                                      CSdpMediaField& aMediaLine ) const;

        /**
        * Decodes media attributes to be sent to client
        * @param aMediaLine Media line
        * @param aStream media stream
        */
        void DecodeClientAttributesL( CSdpMediaField& aMediaLine, 
                                      CMceComMediaStream& aStream ) const;  
                              
        
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
        * Finds codec based on payload type
        * @param aPayloadType payload
        * @param aStream stream containig codecs
        * @return codec
        */
        CMceComCodec* FindCodec( TInt aPayloadType, 
                                 CMceComMediaStream& aStream ) const;
                             
        
        /**
        * Decodes payloads and static payloads if payload wasn't defined
        * as rtpmap. Additionally, codec sdp indexes are updated.
        * @param aMediaLine Media line
        * @param aStream stream
        * @param aRole role of negotiation
        * @return number of lines decoded
        */
        TInt DecodePayloadsL( CSdpMediaField& aMediaLine,
                              CMceComMediaStream& aStream,
                              TMceNegotiationRole aRole ) const;
    
        /**
        * Updates streams codec sdp index values. Can be called after
        * dynamic payload types have been updated.
        * @param aStream stream containig codecs
        * @param aPayloadTypes list of payload types
        */
        void UpdateSdpCodecIndexesL( CMceComMediaStream& aStream,
                                     RArray<TUint>& aPayloadTypes ) const;
    
    
        /**
        * Tries to find matching fmtp line for certain rtpmap line
        * @param aMediaLine Media line
        * @param aRtpMapFormatLine rtpmap format line
        * @return KNullDesC8 if fmtp line is not found
        */
        TPtrC8 GetCorrespondingFmtpLineL( CSdpMediaField& aMediaLine, 
                                          CSdpFmtAttributeField& aRtpMapFormatLine ) const;
    
    protected:
    
    
        /**
        * C++ Constructor
        * @param aMedia media
        */
        CMceMediaSdpCodec( RStringF aMedia );
        


    friend class UT_CMceMediaSdpCodec;
    friend class UT_CMceMediaManager;       
    friend class UT_CMceSdpSession;


    };

#endif      // CMCEMEDIASDPCODEC_H   
            
// End of File
