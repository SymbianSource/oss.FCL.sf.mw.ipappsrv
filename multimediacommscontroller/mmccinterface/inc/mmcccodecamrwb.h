/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Class is used for configuring AMR WB codec.
*
*/




#ifndef __MCC_CODEC_AMR_WB_H__
#define __MCC_CODEC_AMR_WB_H__

//  INCLUDES
#include "mmcccodecamr.h"

// CONSTANTS

// AMR-WB Codec possible bitrates
const TInt KAmrWbBitrate660( 6600 );
const TInt KAmrWbBitrate885( 8850 );
const TInt KAmrWbBitrate1265( 12650 );
const TInt KAmrWbBitrate1425( 14250 );
const TInt KAmrWbBitrate1585( 15850 );
const TInt KAmrWbBitrate1825( 18250 );
const TInt KAmrWbBitrate1985( 19850 );
const TInt KAmrWbBitrate2305( 23050 );
const TInt KAmrWbBitrate2385( 23850 );

const TUint KMccAllowedAmrWbBitrate660     = 0x0001;
const TUint KMccAllowedAmrWbBitrate885     = 0x0002;
const TUint KMccAllowedAmrWbBitrate1265    = 0x0004;
const TUint KMccAllowedAmrWbBitrate1425    = 0x0008;
const TUint KMccAllowedAmrWbBitrate1585    = 0x0010;
const TUint KMccAllowedAmrWbBitrate1825    = 0x0020;
const TUint KMccAllowedAmrWbBitrate1985    = 0x0040;
const TUint KMccAllowedAmrWbBitrate2305    = 0x0080;
const TUint KMccAllowedAmrWbBitrate2385    = 0x0100;
const TUint KMccAllowedAmrWbBitrateAll     = 0x0FFF;

// Allowed bitrates bitfield can contain additional information
const TUint KMccAllowedAmrWbAdditionalInfo   = 0xF000;

// If this bit is not present, mode-change-period=1
const TUint KMccAllowedAmrWbModeChangePeriod2   = 0x1000;

// If this bit is not present, mode-change-neighbor=0
const TUint KMccAllowedAmrWbModeChangeNeighbor1 = 0x2000;

const TUint KAmrWbSamplingFreq( 16000 );

const TInt KAMRMode8( 8 );

const TInt KAmrWbDefaultFrameSize( 64 );

// CLASS DECLARATION
/**
*  Container class for the codec information.
*
*  @lib MCCInterface.dll
*  @since Series 60 3.0
*/
NONSHARABLE_CLASS( CMccCodecAmrWb ) : public CMccCodecAMR
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CMccCodecAmrWb* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CMccCodecAmrWb();
        
    public: // New functions
    
    public: // From CMccCodecInformation 

        /**
        * From CMccCodecInformation
        */
        CMccCodecInformation* CloneDefaultsL();
        
        /**
        * From CMccCodecInformation
        */
        CMccCodecInformation* CloneDetailedL();
        
    private:
        /**
        * C++ default constructor.
        */
        CMccCodecAmrWb();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
    
    
    /**
    *   Set functions
    */          
    public: 

        /**
        * From CMccCodecInformation. Sets bitrate used with codec.
        * @since     Series 60 3.0
        * @param     aBitrate     New bitrate value
        * @return    KErrNotSupported if codec doesn't support bitrate value issued; otherwise KErrNone.
        */
        TInt SetBitrate( TUint aBitrate );
        
        /**
        * From CMccCodecInformation. Sets the bitrate from the mode set .
        * @since     Series 60 3.0
        * @param     aBitrate     The mode set
        * @return    mode set value
        */
        void SetBitrateModeSet( TUint aModeSet );
        
        /**
        * From CMccCodecInformation. Sets the sampling frequency.
        * @since    Series 60 3.0
        * @param    aSamplingFreq   Sampling frequency to be used
        * @return   KErrNotSupported if codec doesn't support sampling frequency issued; otherwise KErrNone.
        */
        TInt SetSamplingFreq( TUint32 aSamplingFreq );

        /**
        * From CMccCodecInformation. Sets the sdp name.
        * @since Series 60 3.0
        * @param aRtpId - [input] RTP session ID
        * @return One of the standard system-wide error codes.
        */
        TInt SetSdpName( const TDesC8& aSdpName );
               
        /**
        * Sets the allowed bitrates for the codec.
        * @since Series 60 3.0
        * @param aBitrateMask bitrate mask.
        * @return KErrNotSupported if codec doesnt' support the bitrate mask
        *     value issued; otherwise KErrNone.
        */
        TInt SetAllowedBitrates( TUint aBitrateMask );


    protected: // From CMccCodecAMR 
     
        /**
        * Sets bitrate used with codec.
        * @since Series 60 3.0
        * @param aBitrateMask bitrate mask, sets the largest bitrate
        * @return KErrNotSupported if codec doesn't support bitrate value
        *     issued; otherwise KErrNone.
        */
        TInt SetBitrateFromBitrateMask( TUint aBitrateMask ); 
        
        /**
        * From CMccCodecInformation. Gets the mode set of a bitrate.
        * @since     Series 60 3.0
        * @param     aBitrate     The bitrate value
        * @return     mode set value
        */
        TInt GetBitrateModeSet( TUint aBitrate );
        
        /**
        * From CMccCodecInformation. Gets the bitrate mask from the mode set .
        * @since     Series 60 3.0
        * @param     aMode The mode set
        * @param    aBitrateMask reference to bitrateMask
        * @return KErrNotSupported if codec doesnt' support the bitrate mask
        *     value issued; otherwise KErrNone.
        */
        TInt GetBitrateMaskFromMode( TUint aMode, TUint& aBitrateMask  );

    
        /**
        * Gets the allowed bitrates for the codec.
        * @since  Series 60 3.0
        * @param  aBitrateMask RArray<TUint> reference 
        * @return KErrNotSupported if codec doesnt' support the function
        *         otherwise KErrNone.
        */
        TInt GetAllowedBitratesArrayL( RArray<TUint>& aBitratesArray );
        
    protected:  // New functions
    
    protected:  // Functions from base classes

        
    public:     // Data    
    
    protected:  // Data
    
    private:    // Data  
        
private:

    #ifdef TEST_EUNIT
        friend class UT_CMccCodecAmrWb;
    #endif
      
    };


#endif      // __MCC_CODEC_AMR_WB_H__

// End of File
