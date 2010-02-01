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





#ifndef CMESDPCODEC_H
#define CMESDPCODEC_H

//  INCLUDES
#include <e32base.h>
#include <stringpool.h>
#include <sdpattributefield.h>
#include <sdpbandwidthfield.h>
#include "mcesip.h"
#include "mcemmlogs.h"

// FORWARD DECLARATIONS
class CSdpDocument;
class CSdpMediaField;
class CMceComSession;
class CMceComMediaStream;
class CSdpMediaField;
class CSdpFmtAttributeField;



#define SDP_STRING( stringIndex )\
    iStringPool.StringF( stringIndex, *iStringTable )



/**
*  Class for encoding and decoding media lines and creating server streams
*  
*/
class CMceSdpCodec : public CBase
    {

    public:
        
        /**
        * destructor
        */
        ~CMceSdpCodec();

        /**
        * returns media(line) which is supported by this SDP codec
        * @return media(line) which is supported by this SDP codec
        */
        RStringF Media();
        
        /**
        * returns ETrue, if media is decoded by this codec
        * @return ETrue, if media is decoded by this codec
        */
        virtual TBool Decodes( const CSdpMediaField& aMedia ) const;

        /**
        * Adds capability
        * @param aCapability capability
        */
        void AddCapabilityL( const TDesC8& aCapability );

        /**
        * Adds capability payload type
        * @param aPayloadType capability payload type
        */
        void AddCapabilityPayloadTypeL( const TUint aPayloadType );
        
        /**
        * Returns ETrue, if capability is supported
        * @param aCapability capability
        * @return ETrue, if capability is supported
        */
        TBool IsSupported( const TDesC8& aCapability ) const;
        
        /**
        * Returns ETrue, if payload is supported
        * @param aSdpName SDP name
        * @param aPayload payload of codec
        * @return ETrue, if payload is supported
        */
        TBool IsPayloadTypeSupported( const TDesC8& aCapability, const TUint aPayloadType ) const;

public: //abstract interface 

        /**
        * Returns ETrue, if media is encoded by this codec
        * @return ETrue, if media is encoded by this codec
        */
        virtual TBool Encodes( const CMceComMediaStream& aStream ) const = 0;
        
        /**
        * Encodes a media line. 
        * @param aStream media stream
        * @param aMediaLine media line
        * @param aSdpDocument sdp document
        * @leave system wide error
        */ 
        virtual void EncodeMediaOfferL( CMceComMediaStream& aStream,
                                        CSdpMediaField& aMediaLine, 
                                        CSdpDocument& aSdpDocument ) = 0;
        
        /**
        * Decodes the media answer.
        * @param aStream media stream
        * @param aSdpDocument sdp document
        * @return warning if any 
        * @leave system wide error
        */
        virtual TMceSipWarningCode DecodeMediaAnswerL( CSdpMediaField& aMediaLine, 
                                         CMceComMediaStream& aStream,
                                         CSdpDocument& aSdpDocument ) = 0;
        
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
        virtual TMceSipWarningCode DecodeMediaOfferL( CSdpMediaField& aMediaLine,
                                                      CMceComMediaStream*& aStream,
                                                      CMceComSession& aSession,
                                                      CSdpDocument& aSdpDocument ) = 0;
                                                      
        /**
        * Decodes media update by updating media stream
        * @param aMediaLine media line
        * @param aStream stream to be updated
        * @param aSdpDocument sdp document
        * @return warning if any 
        * @leave system wide error
        */
        virtual TMceSipWarningCode DecodeMediaUpdateL( CSdpMediaField& aMediaLine, 
                                                       CMceComMediaStream& aStream,
                                                       CSdpDocument& aSdpDocument ) = 0;
                                                      

        /**
        * Creates a encoded media line based on the offer received earlier.
        * @param aStream media stream
        * @param aMediaLine media line
        * @param aSdpDocument sdp document
        * @leave system wide error
        */
        virtual void EncodeMediaAnswerL( CMceComMediaStream& aStream,
                                          CSdpMediaField& aMediaLine,
                                          CSdpDocument& aSdpDocument ) = 0;
                                          
        /**
        * Prepares media line before offer/answer will be decoded
        * @param aMediaLine media line
        * @param aStream media stream
        * @leave system wide error
        */
        virtual void PrepareForDecodeL( CSdpMediaField& aMediaLine,
                                        CMceComMediaStream* aStream ) = 0;
                                      
        /**
        * Clean attributes
        * @param aMedia Media line
        */
        virtual void CleanAttributes( CSdpMediaField& aMedia ) = 0;
        

public: //interface cont
                                                     

        /**
        * Prepares media line before offer/re-offer will be encoded
        * @param aStream media stream
        * @param aMediaLine media line
        * @leave system wide error
        */
        virtual void PrepareForEncodeL( CMceComMediaStream& aStream,
                                        CSdpMediaField& aMediaLine );



    public: //common static like API
                                                     
        /**
        * Encodes rtpmap field
        * @param aPayloadType payload type
        * @param aEncodingName encoding name
        * @param aClockRate clock rate
        * @param aEncodingParam optional encoding paramteter
        * @return rtpmap
        * @leave system wide error
        */
        CSdpFmtAttributeField* EncodeRtpMapFieldsL( const TUint8 aPayloadType,
                                                    const TDesC8& aEncodingName,
                                                    const TUint aClockRate,
                                                    const TDesC8& aEncodingParam ) const;

        /**
        * Gets the direction attribute
        * @param aMedia media line
        * @param aSessionAttributes session attributes
        * @return Direction attribute.
        */
        TInt Direction( CSdpMediaField& aMedia, 
                        RPointerArray<CSdpAttributeField>& aSessionAttributes ) const;
                        

        
        /**
        * Finds attribute from media line
        * @param aAttribute the attribute as string pool index
        * @param aMediaLine sdp document
        * @return attribute or NULL
        */
        CSdpAttributeField* FindAttribute( TInt aAttribute, 
                                           CSdpMediaField& aMediaLine ) const;
                                              
        /**
        * Converts Descriptor into TUint
        * @param aConv descriptor
        * @return converted value.
        */                                          
        static TUint ConvertDesToUintL( const TDesC8& aConv );
        
        /**
        * Converts Descriptor into TInt
        * @param aConv descriptor
        * @return converted value.
        */                                          
        static TInt ConvertDesToIntL( const TDesC8& aConv );

        /**
        * Converts TReal to HBuf
        * @param aConv descriptor
        * @return converted value.
        */                                          
        static HBufC8* ConvertTRealToDesLC( const TReal& aConv );
        
        /**
        * Converts Descriptor into TReal
        * @param aConv descriptor
        * @return converted value.
        */                                          
        static TReal ConvertDesToRealL( const TDesC8& aConv );
                                

        /**
        * Encodes media sdp fields client is willing to insert
        * @param aClientSdp sdp fields
        * @param aContainer media line / sdp document
        */
        template <class T>
        static void EncodeClientSdpFieldsL( const CDesC8Array& aClientSdp,
                                            T& aContainer )
            {
            MCEMM_DEBUG("CMceSdpCodec::EncodeClientSdpFieldsL, Entry" );

           for ( TInt i = 0; i < aClientSdp.Count(); i++ )
                {
                MCEMM_DEBUG_SVALUE("adding", aClientSdp.MdcaPoint( i ) );
                TPtrC8 sdpFieldLine = aClientSdp.MdcaPoint( i );
                if ( sdpFieldLine.Find( KMceSdpAttributeLineMarker ) == 0 )
                    {
                    CSdpAttributeField* attribute = 
                        CSdpAttributeField::DecodeLC( sdpFieldLine );
                    aContainer.AttributeFields().AppendL( attribute );
                    CleanupStack::Pop( attribute );            
                    }
                else if ( sdpFieldLine.Find( KMceSdpBandwidthLineMarker ) == 0 )
                    {
                    CSdpBandwidthField* bandwidth =
                        CSdpBandwidthField::DecodeLC( sdpFieldLine );
                    aContainer.BandwidthFields().AppendL( bandwidth );
                    CleanupStack::Pop( bandwidth );            
                    }
                else
                    {
                    User::Leave( KErrNotSupported );
                    }
                }
            MCEMM_DEBUG("CMceSdpCodec::EncodeClientSdpFieldsL, Exit" );
            }
                                            
         
    protected:

    
        CMceSdpCodec( RStringF aMedia );
        void ConstructL();

    protected://data
    
        RStringPool iStringPool;
        const TStringTable* iStringTable;

    private:

        RStringF iMedia;
        CDesC8Array* iCapabilities;
        RArray<TUint> iCapabilitiesPT;
        
    
    friend class UT_CMceSdpCodec;
    friend class UT_CMceMediaManager;       
    friend class UT_CMceSdpSession;


    };

#endif      // CMESDPCODEC_H   
            
// End of File
