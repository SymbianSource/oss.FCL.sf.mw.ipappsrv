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
* Description:    Encapsulates DTMF Event Payload field values.
*
*/




#ifndef TDTMFEVENTPAYLOADINFO_H
#define TDTMFEVENTPAYLOADINFO_H

// INCLUDES
#include <e32base.h>

/**
*  Class to encapsulate event payload related information.
*
*  @lib mmccdtmfplformat.dll
*  @since Series 60 3.2
*/
class TDTMFEventPayloadInfo
    {
    public:  // Constructors and destructor

        /**
         * C++ default constructor.
         */
        TDTMFEventPayloadInfo();

    public: // New functions
        
        /**
         * Return Event Field value of the payload.
         *
         * @since    Series 60 3.2
         * @return   TChar representing event (0-9, *, #, A, B, C, D)
         */
        inline TChar Event() const;
        
        /**
         * Set Event Field value to the payload.
         *
         * @since    Series 60 3.2
         * @param    aEvent  Event Field value to set
         */
        inline void SetEvent( const TChar& aEvent );
        
        /**
        * Return End Bit value of the payload.
        *
        * @since    Series 60 3.2
        * @return   Boolean indicating whether end bit is set
        */
        inline TBool EndBit() const;
        
        /**
         * Set End Bit value to the payload.
         *
         * @since    Series 60 3.2
         * @param    aEndBit     End Bit value to set
         */
        inline void SetEndBit( TBool aEndBit );
        
        /**
         * Get Volume field of the payload.
         *
         * @since    Series 60 3.2
         * @return   Volume Field value
         */
        inline TUint Volume() const;
        
        /**
         * Set Volume Field to the payload.
         *
         * @since    Series 60 3.2
         * @param    aVolume     Volume Field value to set
         */
        inline void SetVolume( TUint aVolume );
        
        /**
         * Get Duration Field of the payload.
         *
         * @since    Series 60 3.2
         * @return   Duration Field value
         */
        inline TUint Duration() const;
        
        /**
         * Set Duration Field to the payload.
         *
         * @since    Series 60 3.2
         * @param    aDuration       Duration Field value to set
         */
        inline void SetDuration( TUint aDuration );

        /**
         * Get TimeStamp.
         *
         * @since    Series 60 3.2
         * @return   TimeStamp indicating where to place this
         *           event at a time line
         */
        inline TTimeIntervalMicroSeconds32 TimeStamp() const;
        
        /**
         * Set TimeStamp, which syncronizes this event with audio stream.
         *
         * @since    Series 60 3.2
         * @param    aTimeStamp     Indicates where to place this event
         *           at a time line
         */  
        inline void SetTimeStamp( 
            const TTimeIntervalMicroSeconds32& aTimeStamp );

    private: // Data
        
        /**
         * Event Field of Payload
         */        
        TChar iEvent;
        
        /**
         * End bit of Payload
         */
        TBool iEndBit;
        
        /**
         * Volume Field of Payload
         */
        TUint iVolume;
        
        /**
         * Duration Field of Payload
         */
        TUint iDuration;
        
        /**
         * Time stamp of the RTP packet which was carrying this payload
         */
        TTimeIntervalMicroSeconds32 iTimeStamp;
    };

#include    "dtmfeventpayloadinfo.inl"

#endif      // TDTMFEVENTPAYLOADINFO_H
            
// End of File
