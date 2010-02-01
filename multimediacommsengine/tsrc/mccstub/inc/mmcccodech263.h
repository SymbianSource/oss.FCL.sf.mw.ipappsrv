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





#ifndef MMCCCODECH263_H
#define MMCCCODECH263_H

//  INCLUDES
#include "mmcccodecinformation.h"

// CONSTANTS
const TInt KH263PTime( 50 );
const TInt KH263PayloadType( 97 );
const TInt KH2631998PayloadType( 99 );
const TReal KH263Framerate( 12 );
const TUint KH263FrameWidth( 176 );
const TUint KH263FrameHeight( 144 );

const TInt KH263FormatModeNew( 20 );
const TInt KH263FormatModeOld( 21 );

const TInt KH263Level10 = 10;
const TInt KH263Level10Bitrate = 64000;
const TReal KH263Level10Framerate = 12;
const TInt KH263Level45 = 45;
const TInt KH263Level45Bitrate = 128000;
const TReal KH263Level45Framerate = 12;

_LIT8(KH263FmtpLine, "profile=0; level=%d");
_LIT8( KH263Profile, "*profile*");
_LIT8( KH263Level, "*level*");

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Container class for the codec information.
*
*/
NONSHARABLE_CLASS( CMccCodecH263 ) : public CMccCodecInformation
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CMccCodecH263* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CMccCodecH263();

    public: // New functions

    public: // Functions from base classes

    public: // From CMccCodecInformation
    
        /**
        * From CMccCodecInformation
        */
        TBool RequireSignalling( const CMccCodecInformation& aCandidate ) const;     
        
        /**
        * From CMccCodecInformation
        */
        CMccCodecInformation* CloneDefaultsL();
        
        /**
        * From CMccCodecInformation
        */
        CMccCodecInformation* CloneDetailedL();
        
        /**
        * From CMccCodecInformation. Enable / Disable Voice Activity Detection.
        * @since    Series 60 3.0
        * @param    aEnableVAD    Boolean indicating whether to enable Voice Activity Detection
        * @return   KErrNotSupported if codec doesn't support VAD; otherwise KErrNone.
        */
        TInt EnableVAD( TBool aEnableVAD );
     
        /**
        * From CMccCodecInformation. Sets bitrate used with codec.
        * @since    Series 60 3.0
        * @param    aBitrate     New bitrate value
        * @return   KErrNotSupported if codec doesn't support bitrate value issued; otherwise KErrNone.
        */
        TInt SetBitrate( TUint aBitrate );
        
        TInt SetMaxBitrate( TUint aMaxBitrate );
        
        /**
        * From CMccCodecInformation. Sets the sampling frequency.
        * @since    Series 60 3.0
        * @param    aSamplingFreq   Sampling frequency to be used
        * @return   KErrNotSupported if codec doesn't support sampling frequency issued; otherwise KErrNone.
        */
        TInt SetSamplingFreq( TUint32 aSamplingFreq );

        /**
        * From CMccCodecInformation. Sets the packet rate.
        * @since    Series 60 3.0
        * @param    aPacketrate        New packetrate
        * @return   KErrNotSupported if codec doesn't support packetrate value issued; otherwise KErrNone.
        */
        TInt SetPTime( TUint aPTime );
        
        /**
        * From CMccCodecInformation. Sets the packet rate.
        * @since    Series 60 3.0
        * @param    aPacketrate        New packetrate
        * @return   KErrNotSupported if codec doesn't support packetrate value issued; otherwise KErrNone.
        */
        TInt SetMaxPTime( TUint aMaxPTime );

        /**
        * From CMccCodecInformation. Sets the sdp name.
        * @since    Series 60 3.0
        * @param aRtpId - [input] RTP session ID
        * @return One of the standard system-wide error codes.
        */
        TInt SetSdpName( const TDesC8& aSdpName );
        
        /**
        * From CMccCodecInformation. Sets the payload type.
        * @since    Series 60 3.0
        * @param    aRtpId - [input] RTP session ID
        * @return   One of the standard system-wide error codes.
        */
        TInt SetPayloadType( TUint8 aPayloadType );

        /**
        * From CMccCodecInformation. Sets the codec mode.
        * @since    Series 60 3.0
        * @param    aMode    Codec mode ( e.g. ULAW/ALAW, Bandwidth efficient / octet aligned )
        * @return   KErrNotSupported if codec doesnt' support codec mode value issued; otherwise KErrNone.
        */
        TInt SetCodecMode( TCodecMode aCodecMode );
        
        /**
        * From CMccCodecInformation. Gets the codec fmtp parameters.
        * @return Fmtp parameters of the codec
        */
        TDesC8& GetFmtpL();
        
        void SetFmtpAttrL( const TDesC8& aFmtp, TBool parseFmtp );
        
        /**
        * Sets the allowed bitrates for the codec.
        * @since Series 60 3.0
        * @param aBitrateMask bitrate mask.
        * @return KErrNotSupported if codec doesnt' support the bitrate mask
        *     value issued; otherwise KErrNone.
        */
        TInt SetAllowedBitrates( TUint aBitrateMask );
        
        /**
        * Set encoder/decoder configuration key
        * @param aConfigKey, config key as Base64 encoded
        */    
        void SetConfigKeyL( const TDesC8& aConfigKey );

        
    private:

        /**
        * C++ default constructor.
        */
        CMccCodecH263();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        TBool ParseFmtpAttrL( const TDesC8& aFmtp );

        TBool ParseFmtpSegmentL( const TDesC8& aSeg );
        
        TBool ParseProfileL( const TDesC8& aFmtp );
        
        TBool ParseLevelL( const TDesC8& aFmtp );

        void SetMaxBitrateFromBitrateMask( TUint aBitrateMask );
        
        void SetBitrateMaskFromMaxBitrate( TUint aMaxBitrate );
        
        void SetLevelBasedBitrate( TUint aBitrate );
        
    private:
    
        TBool iBitrateSet;
        
        #ifdef TEST_EUNIT
            friend class UT_CMccCodecH263;
        #endif
    };

#endif      // __MCC_CODEC_DTMF_H__   
            
// End of File
