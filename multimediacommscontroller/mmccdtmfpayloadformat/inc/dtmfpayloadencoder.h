/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Formatter class for encoding of RTP payload format for named
*                telephone events and tones.
*
*/




#ifndef CDTMFPAYLOADENCODER_H
#define CDTMFPAYLOADENCODER_H

//  INCLUDES
#include <e32base.h>
#include "dtmfpayloadformatdefs.h"

// FORWARD DECLARATIONS
class CMMFBuffer;
class CMMFDataBuffer;
class TDTMFEventPayloadInfo;
class TDTMFTonePayloadInfo;


// CLASS DECLARATION

/**
 * Provides functionality to encode Tone and Event Payload according
 * to RFC 2833.
 *
 *  @lib mmccdtmfplformat.dll
 *  @since Series 60 3.2
 */
class CDTMFPayloadEncoder : public CBase
    {
    public: // Constructors and destructor
        
        /**
         * Two-phased constructor.
         */
        static CDTMFPayloadEncoder* NewL();
        
        /**
         * Destructor.
         */
        virtual ~CDTMFPayloadEncoder();

    public: // New functions
        
        /**
         * Encodes DTMF digit into Payload Format for Named Events.
         *
         * @since    Series 60 3.2
         * @param    aEventInfo      Object containing values to encode
         * @param    aOutputBuffer   Buffer where to place encoded payload
         * @return   System wide error code
         */
        TInt EncodeEvent( const TDTMFEventPayloadInfo& aEventInfo,
                          CMMFBuffer* aOutputBuffer );
        
        /**
         * Encodes DTMF tone to Tone Payload Format packet.
         *
         * @since    Series 60 3.2
         * @param    aToneInfo       Object containing values to encode
         * @param    aOutputBuffer   Buffer where to place encoded payload
         * @return   System wide error code
         */
        TInt EncodeTone( const TDTMFTonePayloadInfo& aToneInfo,
                         CMMFDataBuffer* aOutputBuffer ) const;

        /**
         * Forms update Event Payload packet.
         *
         * @since    Series 60 3.2
         * @param    aFinalPacket    Indicates whether this is final packet
         * @param    aDuration       Updated duration of the event
         * @param    aOutputBuffer   Buffer where to place encoded payload
         * @return   System wide error code
         */
        TInt UpdateEventPayload( const TBool aFinalPacket,
                                 const TUint aDuration,
                                 CMMFDataBuffer* aOutputBuffer ) const;

        /**
         * Updates field(s) of previously created Tone Payload packet.
         *
         * @since    Series 60 3.2
         * @param    aToneDuration   Updated duration of the tone
         * @param    aOutputBuffer   Buffer where to place encoded payload
         * @return   System wide error code
         */
        TInt UpdateTonePayload( const TUint aToneDuration,
                                CMMFDataBuffer* aOutputBuffer ) const;

        /**
         * Returns type of the payload format.
         *
         * @since    Series 60 3.2
         * @return   Type of the payload
         */
        TDTMFPayloadFormat PayloadFormat() const;

        /**
         * Set payload format used.
         *
         * @since    Series 60 3.2
         * @param    aPayloadFormat  Payload format to use.
         * @return   System wide error code
         */
        TInt SetPayloadFormat( TDTMFPayloadFormat aPayloadFormat );
        
        /**
         * Get payload type used within redundancy headers.
         *
         * @since    Series 60 3.2
         * @return   Payload type
         */
        TUint8 PayloadType() const;
        
        /**
        * Set payload type used within redundancy headers.
        *
        * @since    Series 60 3.2
        * @param    aPayloadType    Payload type
        * @return   void
        */        
        void SetPayloadType( TUint8 aPayloadType );

    private:

        /**
         * C++ default constructor.
         */
        CDTMFPayloadEncoder();

        /**
         * Converts DTMF digit / Tone represented as a TChar to the
         * integer encoding used in payload.
         *
         * @since    Series 60 3.2
         * @param    aEvent     DTMF digit to convert
         * @return   Integer presenting digit
         */
        TInt EventAsInteger( const TChar& aEvent ) const;
        
        /**
         * Encodes DTMF digit into Payload Format for Named Events using
         * redundancy.
         *
         * @since    Series 60 3.2
         * @param    aEventInfo      Object containing values to encode
         * @param    aOutputBuffer   Buffer where to place encoded payload
         * @return   System wide error code
         */
        TInt EncodeRedundantEventPayload( 
            const TDTMFEventPayloadInfo& aEventInfo,
            CMMFBuffer* aOutputBuffer );        

    private: // Data

         /**
          * Payload format used in encoding
          */ 
         TDTMFPayloadFormat iPayloadFormat;

         /**
          * Payload type, needed for redundancy headers
          */         
         TUint8 iPayloadType;

         /**
          * Array used to for redundant events encoding
          */         
         RArray<TDTMFEventPayloadInfo> iEventArray;
    
    private:
        #ifdef TEST_EUNIT
            friend class UT_CDTMFPayloadEncoder;
        #endif
    };

#endif      // CDTMFPAYLOADENCODER_H

// End of File
