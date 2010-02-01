/*
* Copyright (c) 2004-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Class is used for configuring G729 codec.
*
*/




#ifndef __MCC_CODEC_ILBC_H__
#define __MCC_CODEC_ILBC_H__

//  INCLUDES
#include "mmcccodecinformation.h"

// CONSTANTS
const TUint KIlbcMaxPTime30 = 180;

// iLBC Codec possible bitrates
const TInt KIlbcBitrate20 = 15200;
const TInt KIlbcBitrate30 = 13330;

// Other iLBC related constants
const TInt KIlbcSampleRate( 8000 );
const TInt KIlbcPTime( 30 );
const TInt KIlbcHwFrameTime( 30 );
const TInt KIlbcMaxPTime( 180 );
const TInt KIlbcPTime20( 20 );
const TInt KIlbcPTime30( 30 );

// CLASS DECLARATION

/**
 *  Container class for the iLBC codec information.
 *
 *  @lib mmccinterface.dll
 */
NONSHARABLE_CLASS( CMCCCodecILBC ): public CMccCodecInformation
    {
    public:  // Constructors and destructor
   
        /**
        * Two-phased constructor.
        */
        static CMCCCodecILBC* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CMCCCodecILBC();
        
    public:    // From CMccCodecInformation

        /**
        * From CMccCodecInformation
        */
        CMccCodecInformation* CloneDefaultsL();
        
        /**
        * From CMccCodecInformation
        */
        CMccCodecInformation* CloneDetailedL();
        
        /**
        * From CMccCodecInformation
        */
        TBool RequireSignalling( const CMccCodecInformation& aCandidate ) const; 
        
        /**
        * From MccCodecInformation. Enable / Disable Voice Activity Detection.
        * @since    Series 60 3.0
        * @param    aEnableVAD    Boolean indicating whether to enable Voice Activity Detection
        * @return   KErrNotSupported if codec doesn't support VAD; otherwise KErrNone.
        */
        TInt EnableVAD( TBool aEnableVAD );
               
        /**
        * From MccCodecInformation. Sets bitrate used with codec.
        * @since     Series 60 3.0
        * @param     aBitrate     New bitrate value
        * @return    KErrNotSupported if codec doesn't support bitrate value issued; otherwise KErrNone.
        */
        TInt SetBitrate( TUint aBitrate );
            
        /**
        * From MccCodecInformation. Sets the sampling frequency.
        * @since    Series 60 3.0
        * @param    aSamplingFreq   Sampling frequency to be used
        * @return   KErrNotSupported if codec doesn't support sampling frequency issued; otherwise KErrNone.
        */
        TInt SetSamplingFreq( TUint32 aSamplingFreq );

        /**
        * From MccCodecInformation. Sets the sdp name.
        * @since Series 60 3.0
        * @param aRtpId - [input] RTP session ID
        * @return One of the standard system-wide error codes.
        */
        TInt SetSdpName( const TDesC8& aSdpName );
        
        /**
        * From MccCodecInformation. Sets the payload type.
        * @since Series 60 3.0
        * @param aRtpId - [input] RTP session ID
        * @return One of the standard system-wide error codes.
        */
        TInt SetPayloadType( TUint8 aPayloadType );
        
        /**
        * From MccCodecInformation. Sets the codec mode.
        * @since         Series 60 3.0
        * @param aMode   Codec mode ( e.g. ULAW/ALAW, Bandwidth efficient / octet aligned )
        * @return        KErrNotSupported if codec doesnt' support codec mode value issued; otherwise KErrNone.
        */
        TInt SetCodecMode( TCodecMode aCodecMode );

        /**
        * Creates the fmtp string from internal values
        * @since Series 60 3.0
        * @return void
        */
        void CreateFmtpAttrListL();
        
        /**
        * Parses the fmtp attributes, and updates the internal values
        * except the iFmtpAttr attribute
        * @since Series 60 3.0
        * @param aFmtp Fmtp attribute
        * @return True if any param was updated
        */
        TBool ParseFmtpAttrL( const TDesC8& aFmtp );
               
        /**
        * Get the fmtp string
        * @since Series 60 3.0
        * @return Fmtp.
        */
        TDesC8& GetFmtpL( );
        
        /**
        * From MccCodecInformation. Sets max packet time.
        * @since    Series 60 3.0
        * @param aMaxPTime Maximum PacketTime
        * @return One of the standard system-wide error codes.
        */
        TInt SetMaxPTime( TUint aMaxPTime );
        
        /**
        * From MccCodecInformation. Sets the packet time.
        * @since    Series 60 3.0
        * @param aPTime PacketTime.
        * @return One of the standard system-wide error codes.
        */
        TInt SetPTime( TUint aPTime );

        /**
        * Enable / Disable Comfort noise generation.
        * @param aComfortNoisePT Payloadtype value indicating what
        *        payload type should be used for comfort noise.
        *        KPayloadTypeUndefined can be used to turn comfor noise
        *        generation off.
        * @return One of the standard system-wide error codes.
        */
        TInt SetComfortNoiseGeneration( TUint8 aComfortNoisePT );
        
        /**
        * Set Allowed BitRate
        * @since    Series 60 3.0
        * @param    aBitRateMask
        *                       
        * @return   Etrue, if Mask is valid
        */

        TInt SetAllowedBitrates( TUint aBitrateMask );
                   
    private:
        
        /**
        * C++ default constructor.
        */
        CMCCCodecILBC();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
        /**
        * Adjust codec.
        */
        void AdjustCodec();

    public:     // Data

    protected:  // Data

    private:    // Data

    public:     // Friend classes

    protected:  // Friend classes

    private:    // Friend classes

    };


#endif      //__MCC_CODEC_ILBC_H__

// End of File
