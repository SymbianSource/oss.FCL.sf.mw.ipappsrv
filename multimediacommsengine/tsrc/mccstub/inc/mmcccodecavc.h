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





#ifndef MMCCCODECAVC_H
#define MMCCCODECAVC_H

//  INCLUDES
#include "mmcccodecinformation.h"

// CONSTANTS
const TInt KAvcPTime( 50 );
const TInt KAvcPayloadType( 98 );
const TReal KAvcFramerate( 15 );
const TUint KAvcFrameWidth( 176 );
const TUint KAvcFrameHeight( 144 );

const TInt KAvcFormatModeSingleNal( 0 );
const TInt KAvcFormatModeNonInterleaved( 1 );
const TInt KAvcFormatModeInterleaved( 2 );

// Allowedbitrates bitfield combines avc profile-level-id information in
// similar manner  as defined in RFC 3984. Most significant byte stores
// profile-id value, seconds significant byte contains profile-iop value and
// last two bytes contain profile-level information.
//
const TUint KMccAvcBitrateLevel1 = 0x0001;
const TUint KMccAvcBitrateLevel1b = 0x0002;
const TUint KMccAvcBitrateLevel1_1 = 0x0004;
const TUint KMccAvcBitrateLevel1_2 = 0x0008;
const TUint KMccAvcBitrateLevel1_3 = 0x0010;
const TUint KMccAvcBitrateLevel2 = 0x0020;

const TUint KMccAvcCodecProfileIopConstraintSet = 0x0100;
const TUint KMccAvcCodecProfileIopConstraintSet1 = 0x0200;
const TUint KMccAvcCodecProfileIopConstraintSet2 = 0x0400;

const TUint KMccAvcCodecProfileIdBaseline = 0x1000;
const TUint KMccAvcCodecProfileIdMain = 0x2000;
const TUint KMccAvcCodecProfileIdExtended = 0x4000; 
const TUint KMccAvcCodecProfileIdHigh = 0x8000; 

const TUint KMccAllowedAvcBitrateAll = 0x00FF;

// Allowed bitrates bitfield can contain additional information
const TUint KMccAllowedAvcAdditionalInfo = 0xFF00;

_LIT8( KMatchProfileLevelId, "*profile-level-id*");
_LIT8( KTxtProfileLevelId, "profile-level-id=");
_LIT8( KMatchSpropParameterSets, "*sprop-parameter-sets*");
_LIT8( KTxtSpropParameterSets, "sprop-parameter-sets=");
_LIT8( KMatchPacketizationMode, "*packetization-mode*");
_LIT8( KTxtPacketizationMode, "packetization-mode=");

const TInt KMccAvcCodecProfileIdBaselineVal = 0x42;

const TInt KMccAvcCodecProfileIopConstraintSetVal = 4;
const TInt KMccAvcCodecProfileIopConstraintSet1Val = 2;
const TInt KMccAvcCodecProfileIopConstraintSet2Val = 1;

const TInt KMccAvcCodecProfileLevel1Val = 0x0A;
const TInt KMccAvcCodecProfileLevel1_1Val = 0x0B;
const TInt KMccAvcCodecProfileLevel1_2Val = 0x0C;
const TInt KMccAvcCodecProfileLevel1_3Val = 0x0D;
const TInt KMccAvcCodecProfileLevel2Val = 0x14;

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Container class for the codec information.
*
*/
NONSHARABLE_CLASS( CMccCodecAVC ) : public CMccCodecInformation
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CMccCodecAVC* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CMccCodecAVC();

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
        * @param    aEnableVAD    Boolean indicating whether to enable
        *                         Voice Activity Detection
        * @return   KErrNotSupported if codec doesn't support VAD; otherwise KErrNone.
        */
        TInt EnableVAD( TBool aEnableVAD );
     
        /**
        * From CMccCodecInformation. Sets bitrate used with codec.
        * @since    Series 60 3.0
        * @param    aBitrate     New bitrate value
        * @return   KErrNotSupported if codec doesn't support bitrate
        *           value issued; otherwise KErrNone.
        */
        TInt SetBitrate( TUint aBitrate );
        
        /**
        * From CMccCodecInformation. Sets the sampling frequency.
        * @since    Series 60 3.0
        * @param    aSamplingFreq   Sampling frequency to be used
        * @return   KErrNotSupported if codec doesn't support sampling frequency issued;
        *           otherwise KErrNone.
        */
        TInt SetSamplingFreq( TUint32 aSamplingFreq );

        /**
        * From CMccCodecInformation. Sets the packet rate.
        * @since    Series 60 3.0
        * @param    aPacketrate        New packetrate
        * @return   KErrNotSupported if codec doesn't support packetrate value issued;
        *           otherwise KErrNone.
        */
        TInt SetPTime( TUint aPTime );
        
        /**
        * From CMccCodecInformation. Sets the packet rate.
        * @since    Series 60 3.0
        * @param    aPacketrate        New packetrate
        * @return   KErrNotSupported if codec doesn't support packetrate value issued;
        *           otherwise KErrNone.
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
        * @param    aMode    Codec mode
        * @return   KErrNotSupported if codec doesn't 
        *   support codec mode value issued; otherwise KErrNone.
        */
        TInt SetCodecMode( TCodecMode aCodecMode );
        
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
        
        /**
    	* Sets preferred encoding device for uplink stream and preferred decoding
    	* device for downlink stream.
    	* @param aEncodingDecodingDevice
    	*/
    	void SetPreferredEncodingDecodingDevice( 
                                    TUid aEncodingDecodingDevice );
              
    
    /**
    *   FMTP functions
    */ 
    public: //Public FMTP functions
        
        /**
        * Get the fmtp string
        * @since Series 60 3.0
        * @return Fmtp.
        */
        TDesC8& GetFmtpL( ); 
        
        /**
        * Parses the fmtp attributes, and updates the internal values
        * except the iFmtpAttr attribute
        * @since Series 60 3.0
        * @param aFmtp Fmtp attribute
        * @return True if any param was updated
        */
        TBool ParseFmtpAttrL( const TDesC8& aFmtp );      
       
        void SetFmtpAttrL( const TDesC8& aFmtp, TBool parseFmtp );
        
        /**
        * Creates the fmtp string from internal values
        * @since Series 60 3.0
        * @return void
        */
        void CreateFmtpAttrListL();
            
    protected:  // New functions
    
        TBool ParseFmtpSegmentL( const TDesC8& aSeg );
    
        TBool ParseFmtpProfileLevelIdL( const TDesC8& aProfileLevelId );
        
        TBool ParseFmtpSpropParameterSetsL( const TDesC8& aSpropParameterSets );
        
        TBool ParseFmtpPacketizationModeL( const TDesC8& aPacketizationMode );
        
        void SetProfileLevelIdL( TUint aProfileId, 
                                 TUint aProfileIop, 
                                 TUint aProfileLevel );
                                 
        void SetBitrateMask( TUint aBitrateMask, TBool aKeepAdditionalInfo = EFalse ); 
        
        void SetBitrateMaskAdditionalInfo( TUint aAdditionalInfo ); 
                                 
        TInt SetBitrateFromBitrateMask( TUint aBitrateMask );
        
        TInt ProfileLevelIdVals( TUint& aProfileId, 
                                 TUint& aProfileIop, 
                                 TUint& aProfileLevel );

        TBool CreateFmtpProfileLevelId( TDes8& aBuf );
        
        TBool CreateFmtpPacketizationMode( TDes8& aBuf );
        
        TBool CreateFmtpSpropParameterSets( TDes8& aBuf );
        
        TUint DefaultProfileIopVal();
        
        void SetLevelBasedBitrate( TUint aBitrate );
        
    protected:  // Functions from base classes

    private:

        /**
        * C++ default constructor.
        */
        CMccCodecAVC();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    public:     // Data
    
    protected:  // Data

    private:    // Data
    
        TBool iProfileLevelIdParamExists;
        
        TBool iPacketizationModeParamExists;
    
        TBool iBitrateSet;

    public:     // Friend classes

    protected:  // Friend classes
    
    private:    // Friend classes

    #ifdef TEST_EUNIT
        friend class UT_CMccCodecAVC;
    #endif
    };

#endif      // __MCC_CODEC_DTMF_H__   
            
// End of File
