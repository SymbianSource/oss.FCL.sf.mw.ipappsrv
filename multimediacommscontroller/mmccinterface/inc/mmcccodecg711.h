/*
* Copyright (c) 2004-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Class is used for configuring G711 codec.
*
*/




#ifndef __MCC_CODEC_G711_H__
#define __MCC_CODEC_G711_H__

//  INCLUDES
#include "mmcccodecinformation.h"

// CONSTANTS

// G.711 Codec possible bitrates
const TUint KG711Bitrate64 = 64000;
const TInt KG711DefaultHwFrameTime( 20 );
const TUint8 KG711PayloadTypePCMU( 0 );
const TUint8 KG711PayloadTypePCMA( 8 );

// CLASS DECLARATION
/**
*  Container class for the codec information.
*
*  @lib mmccinterface.dll 
*/
NONSHARABLE_CLASS( CMCCCodecG711 ) : public CMccCodecInformation
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CMCCCodecG711* NewL( );
        
        /**
        * Destructor.
        */
        virtual ~CMCCCodecG711();
        
    public:    // From CMCCCodecInformation

        /**
        * From CMCCCodecInformation
        */
        CMccCodecInformation* CloneDefaultsL();
        
        /**
        * From CMCCCodecInformation
        */
        CMccCodecInformation* CloneDetailedL();
        
        /**
        * From CMccCodecInformation
        */
        TBool RequireSignalling( const CMccCodecInformation& aCandidate ) const;  
        
// sets
    
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
        * @return     KErrNotSupported if codec doesn't support bitrate value issued; otherwise KErrNone.
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
        * From MccCodecInformation. Sets the packet time.
        * @since    Series 60 3.0
        * @param aPTime PacketTime.
        * @return One of the standard system-wide error codes.
        */
        TInt SetPTime( TUint aPTime );
        
        /**
        * From MccCodecInformation. Sets max packet time.
        * @since    Series 60 3.0
        * @param aMaxPTime Maximum PacketTime
        * @return One of the standard system-wide error codes.
        */
        TInt SetMaxPTime( TUint aMaxPTime );

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
        TInt SetPayloadType( TUint8 aType );
        
        /**
        * From MccCodecInformation. Sets the codec mode.
        * @since         Series 60 3.0
        * @param aMode    Codec mode ( e.g. ULAW/ALAW, Bandwidth efficient / octet aligned )
        * @return        KErrNotSupported if codec doesnt' support codec mode value issued; otherwise KErrNone.
        */
        TInt SetCodecMode( TCodecMode aCodecMode );
        
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
        CMCCCodecG711();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( );

    };


#endif      //__MCC_CODEC_G711_H__

// End of File
