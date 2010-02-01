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
* Description:    Encapsulates Tone Payload field values.
*
*/




#ifndef TDTMFTONEPAYLOADINFO_H
#define TDTMFTONEPAYLOADINFO_H

//  INCLUDES
#include <e32base.h>
#include <e32std.h>


// CLASS DECLARATION

/**
 *  Class to encapsulate tone payload related information.
 *
 *  @lib mmccdtmfplformat.dll
 *  @since Series 60 3.2
 */
class TDTMFTonePayloadInfo
    {
    public:  // Constructors and destructor
        
        /**
         * C++ default constructor.
         */
        TDTMFTonePayloadInfo();

    public: // New functions
        
        /**
         * Return character presentation of a tone.
         *
         * @since    Series 60 3.2
         * @return   Tone as a character
         */
        inline TChar Tone() const;

        /**
         * Set tone value.
         *
         * @since    Series 60 3.2
         * @param    aTone   Tone presented as a TChar
         */
        inline void SetTone( const TChar& aTone );
                
        /**
         * Return modulation field value of the payload.
         *
         * @since    Series 60 3.2
         * @return   Modulation value
         */
        inline TUint Modulation() const;
        
        /**
         * Set modulation field value to the payload.
         *
         * @since    Series 60 3.2
         * @param    aModulation  Modulation value to set
         */
        inline void SetModulation( TUint aModulation );
        
        /**
         * Return T-bit of the payload.
         *
         * @since    Series 60 3.2
         * @return   Boolean indicating whether T-bit is set
         */
        inline TBool TBit() const;
        
        /**
         * Set T-bit value to the payload.
         *
         * @since    Series 60 3.2
         * @param    aTBit   Value to set
         */
        inline void SetTBit( TBool aTBit );
        
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
         * @param    aVolume Volume Field value to set
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
         * @param    aDuration   Duration Field value to set
         */
        inline void SetDuration( TUint aDuration );
        
        /**
         * Get first frequency of the tone in payload.
         *
         * @since    Series 60 3.2
         * @return   First frequency value
         */
        inline TUint LowFrequency() const;
        
        /**
         * Set first frequency of the tone to the tone payload.
         *
         * @since    Series 60 3.2
         * @param    aLowFrequency   Frequency value to set
         */
        inline void SetLowFrequency( TUint aLowFrequency );
        
        /**
         * Get second frequency of the tone in payload.
         *
         * @since    Series 60 3.2
         * @return   Second frequency value
         */
        inline TUint HighFrequency() const;
        
        /**
         * Set second frequency of the tone to the tone payload.
         *
         * @since    Series 60 3.2
         * @param    aHighFrequency  Frequency value to set
         */
        inline void SetHighFrequency( TUint aHighFrequency );
        
        /**
         * Get tone on period used in tone playback.
         *
         * @since    Series 60 3.2
         * @return   The period over which the tone will be played
         */
        inline TTimeIntervalMicroSeconds& ToneOnPeriod();
        
        /**
         * Set tone on period used in tone playback.
         *
         * @since    Series 60 3.2
         * @param    aToneOnPeriod   Tone on period
         */
        inline void SetToneOnPeriod( 
            const TTimeIntervalMicroSeconds& aToneOnPeriod );
        
        /**
         * Set tone off period used in tone playback.
         *
         * @since    Series 60 3.2
         * @return   The period over which no tone will be played
         */
        inline TTimeIntervalMicroSeconds& ToneOffPeriod();
        
        /**
         * Set tone off period used in tone playback.
         *
         * @since    Series 60 3.2
         * @param    aToneOffPeriod  Tone off period
         */
        inline void SetToneOffPeriod( 
            const TTimeIntervalMicroSeconds& aToneOffPeriod );
        
        /**
         * Get pause length used with tone playback operation.
         *
         * @since    Series 60 3.2
         * @return   The period over which the tone playing will be paused
         */
        inline TTimeIntervalMicroSeconds& PauseLength( );

        /**
         * Set pause length used with tone playback operation.
         *
         * @since    Series 60 3.2
         * @param    aPauseLength    Pause length
         */
        inline void SetPauseLength( 
            const TTimeIntervalMicroSeconds& aPauseLength );

    private: // Data
        
        /**
         * Tone as a character
         */        
        TChar iTone;

        /**
         * Modulation Field of Payload
         */        
        TChar iModulation;

        /**
         * T bit of Payload
         */        
        TBool iTBit;

        /**
         * Volume Field of Payload
         */        
        TUint iVolume;
        
        /**
         * Duration Field of Payload
         */        
        TUint iDuration;
        
        /**
         * Low Frequency Field of Payload
         */        
        TUint iLowFrequency;
        
        /**
         * High Frequecny Field of Payload
         */        
        TUint iHighFrequency;
        
        /**
         * Needed for tone playback operation
         */        
        TTimeIntervalMicroSeconds iToneOnPeriod;
        
        /**
         * Needed for tone playback operation
         */        
        TTimeIntervalMicroSeconds iToneOffPeriod;
        
        /**
         * Needed for tone playback operation
         */        
        TTimeIntervalMicroSeconds iPauseLength;
    };

#include    "dtmftonepayloadinfo.inl"

#endif      // TDTMFTONEPAYLOADINFO_H

// End of File
