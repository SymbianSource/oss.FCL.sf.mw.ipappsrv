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
* Description:    Formatter class for decoding of RTP payload format for named
*                telephone events and tones.
*
*/




#ifndef CDTMFPAYLOADDECODER_H
#define CDTMFPAYLOADDECODER_H

//  INCLUDES
#include <e32base.h>
#include "dtmfpayloadformatdefs.h"
#include "dtmfeventpayloadinfo.h"
#include "dtmftonepayloadinfo.h"

// FORWARD DECLARATIONS
class CMMFBuffer;
class CMMFDataBuffer;


// CLASS DECLARATION
/**
 * Provides functionality to decode Tone and Event Payload according
 * to RFC 2833. Functionality for decoding redundant event Payload
 * is also served ( RFC 2198 ).
 *
 * @lib mmccdtmfplformat.dll
 * @since Series 60 3.2
 */
class CDTMFPayloadDecoder : public CBase
    {
    public:  // Constructors and destructor
        
        /**
         * Two-phased constructor.
         */
        static CDTMFPayloadDecoder* NewL();
        
        /**
         * Destructor.
         */
        virtual ~CDTMFPayloadDecoder();

    public: // New functions
        
        /**
         * Decodes DTMF string from Payload Format for Named Events.
         *
         * @since    Series 60 3.2
         * @param    aSourceBuffer  Databuffer containing payload
         * @param    aEventInfos    Array where to place decoded information
         * @return   System wide error code
         */
        TInt DecodeEventPayload( CMMFBuffer* aSourceBuffer,
            RArray< TDTMFEventPayloadInfo >& aEventInfos );
        
        /**
         * Decodes DTMF from Tone Payload Format packet.
         *
         * @since    Series 60 3.2
         * @param    aSourceBuffer  Databuffer containing payload
         * @param    aToneInfos     Array where to place decoded information
         * @return   System wide error code
         */
        TInt DecodeTonePayload( CMMFBuffer* aSourceBuffer,
            RArray<TDTMFTonePayloadInfo>& aToneInfos );
        
        /**
         * Returns type of the payload format.
         *
         * @since    Series 60 3.2
         * @return   Type of the payload
         */
        TDTMFPayloadFormat PayloadFormat( ) const;
        
        /**
         * Set payload format used.
         *
         * @since    Series 60 3.2
         * @param    aPayloadFormat  Payload format to set.
         * @return   System wide error code
         */
        TInt SetPayloadFormat( TDTMFPayloadFormat aPayloadFormat );
        
        /**
         * Sets the time stamp of a RTP packet containing payload which is
         * decoded next.
         *
         * @since    Series 60 3.2
         * @param    aTimeStamp  Time stamp
         */
        void SetCurTimeStamp( const TTimeIntervalMicroSeconds32& aTimeStamp );

    private:

        /**
         * C++ default constructor.
         */
        CDTMFPayloadDecoder();

        /**
         * Decodes DTMF from Payload Format for Named Events with
         * redundant events.
         *
         * @since    Series 60 3.2
         * @param    aSourceBuffer  Databuffer containing payload
         * @param    aEventInfos    Array where to place decoded information
         * @return   System wide error code
         */
        TInt DecodeRedundantEventPayload( CMMFBuffer* aSourceBuffer,
            RArray<TDTMFEventPayloadInfo>& aEventInfos );
                                          
        /**
         * Converts integer presentation used in a payload format to the
         * character.
         *
         * @since    Series 60 3.2
         * @param    aEvent  Integer presentation of the event
         * @param    aChar   Result character
         * @return   System wide error code
         */
        TInt ConvertToChar( TUint aEvent, TChar& aChar ) const;

    private: // Data
         
         /**
          * Payload format used for decoding 
          */
         TDTMFPayloadFormat iPayloadFormat;
         
         /**
          * Time stamp of the RTP packet under decoding
          */         
         TTimeIntervalMicroSeconds32 iCurTimeStamp;
         
         /**
          * Array used to recognise redundant events already played out
          */         
         RArray<TDTMFEventPayloadInfo> iEventArray;
         
         /**
          * Used as a temporarily container for the 
          * payload info under decoding
          */         
         TDTMFEventPayloadInfo iDecodedEventInfo;
         
         /**
          * Used as a temporarily container for the
          * payload info under decoding
          */         
         TDTMFTonePayloadInfo iDecodedToneInfo;

    private:
        #ifdef TEST_EUNIT
            friend class UT_CDTMFPayloadDecoder;
        #endif
    
    };

#endif      // CDTMFPAYLOADDECODER_H

// End of File
