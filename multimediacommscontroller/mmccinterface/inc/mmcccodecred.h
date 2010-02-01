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
* Description:    Class is used for configuring red codec.
*
*/




#ifndef __MCC_CODEC_RED_H__
#define __MCC_CODEC_RED_H__

//  INCLUDES
#include <e32cmn.h>
#include "rtpdef.h"
#include "mmcccodecinformation.h"

// CONSTANTS
const TText8 KCharSpace( ' ' );
const TText8 KCharSlash( '/' );

const TInt KRedSampleRate = 8000;
const TInt KRedPTime = 20;
const TInt KRedMaxPTime = 200;

// Red Codec possible bitrates
const TUint KRedSamplingFreq = 8000;

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
*  Container class for the codec information.
*
*  @lib mmccinterface.dll
*  
*/
NONSHARABLE_CLASS( CMccCodecRed ) : public CMccCodecInformation
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CMccCodecRed* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CMccCodecRed();
        
    public: // New functions
    
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
        * @since     Series 60 3.0
        * @param     aBitrate     New bitrate value
        * @return    KErrNotSupported if codec doesn't support bitrate value issued; otherwise KErrNone.
        */
        TInt SetBitrate( TUint aBitrate );
        
        /**
        * From CMccCodecInformation. Sets the sampling frequency.
        * @since    Series 60 3.0
        * @param    aSamplingFreq   Sampling frequency to be used
        * @return   KErrNotSupported if codec doesn't support sampling frequency issued; otherwise KErrNone.
        */
        TInt SetSamplingFreq( TUint32 aSamplingFreq );

        /**
        * From CMccCodecInformation. Sets the packet rate.
        * @since     Series 60 3.0
        * @param     aPacketrate        New packetrate
        * @return    KErrNotSupported if codec doesn't support packetrate value issued; otherwise KErrNone.
        */
        TInt SetPTime( TUint aPTime );
        
        /**
        * From CMccCodecInformation. Sets the packet rate.
        * @since     Series 60 3.0
        * @param     aPacketrate        New packetrate
        * @return    KErrNotSupported if codec doesn't support packetrate value issued; otherwise KErrNone.
        */
        TInt SetMaxPTime( TUint aMaxPTime );
        
        /**
        * From CMccCodecInformation. Sets the sdp name.
        * @since Series 60 3.0
        * @param aRtpId - [input] RTP session ID
        * @return One of the standard system-wide error codes.
        */
        TInt SetSdpName( const TDesC8& aSdpName );
        
        /**
        * From CMccCodecInformation. Sets the payload type.
        * @since Series 60 3.0
        * @param aRtpId - [input] RTP session ID
        * @return One of the standard system-wide error codes.
        */
        TInt SetPayloadType( TUint8 aPayloadType );

        /**
        * From CMccCodecInformation. Sets the codec mode.
        * @since         Series 60 3.0
        * @param aMode   Codec mode ( e.g. ULAW/ALAW, Bandwidth efficient / octet aligned )
        * @return        KErrNotSupported if codec doesnt' support codec mode value issued; otherwise KErrNone.
        */
        TInt SetCodecMode( TCodecMode aCodecMode );
        
        /**
        * Parses the fmtp attribute and sets the parsed codecs   
        * @since         Series60 3.0
        * @param aFmtp   Fmtp attribute
        * @return        One of the standard system-wide error codes.
        */
        TBool ParseFmtpAttrL( const TDesC8& aFmtp );    
        
        /**
        * Set Allowed BitRate
        * @since    Series 60 3.0
        * @param    aBitRateMask
        *                       
        * @return   Etrue, if Mask is valid
        */
        TInt SetAllowedBitrates( TUint aBitrateMask );
          
        /**
        * Sets the payload types used in redundancy 
        * @since        Series60 3.0
        * @param        Reference to the array containing the codecs
        * @return       None       
        */
        void SetRedPayloadsL( RArray<TUint>& aRedPayloads );
        
        /**
        * Fills array with red payload types.
        * @since        Series60 3.0
        * @param        aRedPayloads   
        */
        void RedPayloadsL( RArray<TUint>& aRedPayloads ) const;
   
   public: // New methods
       
        const RArray<TUint>& RedPayloads() const;
           
    private:
        /**
        * C++ default constructor.
        */
        CMccCodecRed();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
        /** 
        * Array contains the payloadtypes used in the Redundancy payload
        * Used only for redundancy payload format type 
        */ 
        RArray< TUint > iRedPayloads;        
    private:
        #ifdef TEST_EUNIT
            friend class UT_CMccCodecRed;
        #endif     
    };


#endif      // __MCC_CODEC_RED_H__

// End of File
