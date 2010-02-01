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
* Description:    Class is used for configuring AMR codec.
*
*/




#ifndef __MCC_CODEC_AMR_H__
#define __MCC_CODEC_AMR_H__

//  INCLUDES
#include "mmcccodecinformation.h"

// CONSTANTS
const TUint KDefaultModeChangePeriod = 1;

// AMR-NB Codec possible bitrates
const TInt KAmrNbBitrate475( 4750 );
const TInt KAmrNbBitrate515( 5150 );
const TInt KAmrNbBitrate590( 5900 );
const TInt KAmrNbBitrate670( 6700 );
const TInt KAmrNbBitrate740( 7400 );
const TInt KAmrNbBitrate795( 7950 );
const TInt KAmrNbBitrate102( 10200 );
const TInt KAmrNbBitrate122( 12200 );


const TUint KMccAllowedAmrNbBitrate475     = 0x0001;
const TUint KMccAllowedAmrNbBitrate515     = 0x0002;
const TUint KMccAllowedAmrNbBitrate590     = 0x0004;
const TUint KMccAllowedAmrNbBitrate670     = 0x0008;
const TUint KMccAllowedAmrNbBitrate740     = 0x0010;
const TUint KMccAllowedAmrNbBitrate795     = 0x0020;
const TUint KMccAllowedAmrNbBitrate102     = 0x0040;
const TUint KMccAllowedAmrNbBitrate122     = 0x0080;
const TUint KMccAllowedAmrNbBitrateAll     = 0x00FF;

// Allowed bitrates bitfield can contain additional information
const TUint KMccAllowedAmrAdditionalInfo   = 0x0F00;

// If this bit is not present, mode-change-period=1
const TUint KMccAllowedModeChangePeriod2   = 0x0100;

// If this bit is not present, mode-change-neighbor=0
const TUint KMccAllowedModeChangeNeighbor1 = 0x0200;

const TUint KAmrNbSamplingFreq( 8000 );

const TInt KAMRDefaultHwFrameTime( 20 );
const TInt KAMRDefaultFrameSize( 32 );
const TInt KAMRDefaultPTime( 20 );
const TInt KAMRDefaultMaxPTime( 400 );

const TUint KModTen( 10 );

const TInt KAMRMode0( 0 );
const TInt KAMRMode1( 1 );
const TInt KAMRMode2( 2 );
const TInt KAMRMode3( 3 );
const TInt KAMRMode4( 4 );
const TInt KAMRMode5( 5 );
const TInt KAMRMode6( 6 );
const TInt KAMRMode7( 7 );

// Max five frames is used with AMR FEC redundancy
const TInt  KMaxAmrFecRedCount = 5;

_LIT8( KMatchModeSet, "*mode-set*");
_LIT8( KTxtModeSet, "mode-set=");
_LIT8( KMatchOctetAlign, "*octet-align*");
_LIT8( KMatchModeChangePeriod, "*mode-change-period*");
_LIT8( KMatchModeChangeNeighbor, "*mode-change-neighbor*");
_LIT8( KCrc,"*crc*" );
_LIT8( KRobustSorting,"*robust-sorting*" );
_LIT8( KInterleaving,"*interleaving*" );
_LIT8( KMaxRed,"*max-red*" );

// CLASS DECLARATION
/**
*  Container class for the codec information.
*
*  @lib MCCInterface.dll
*  @since Series 60 3.0
*/
NONSHARABLE_CLASS( CMccCodecAMR ) : public CMccCodecInformation
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CMccCodecAMR* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CMccCodecAMR();
        
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
        
    private:

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
    
    
    /**
    *   Set functions
    */          
    public: 
               
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
        * Sets the allowed bitrates for the codec.
        * @since Series 60 3.0
        * @param aBitrateMask bitrate mask.
        * @return KErrNotSupported if codec doesnt' support the bitrate mask
        *     value issued; otherwise KErrNone.
        */
        TInt SetAllowedBitrates( TUint aBitrateMask );
        
        /**
        * Set number of channels. Range between 1 to 8. Refer to RFC3551 4.1
        * for interpretation of channels value. Default value is 1.
        *
        * @since    Series 60 3.0
        * @return   One of the standard system-wide error codes.
        */
        TInt SetChannels( TInt aNumOfChannels );


        /**
         * From CMccCodecInformation.
         * Sets the redundancy level. Setting is effective only if some
         * redundancy algorithm is set with SetAlgo().
         *
         * @param   aRedCount   The level of redundancy
         * @return  One of the standard system-wide error codes.
         */
        TInt SetRedCount( TUint aRedCount );
        
        /**
         * Sets the upper limit on the usage of redundancy in milliseconds.
         * @since    Series 60 3.2
         * @param    aMaxRed   maximum redundancy
         * @return   One of the standard system-wide error codes.
         */
        TInt SetMaxRed( TInt aMaxRed ); 
    
    /**
    *  New virtual functions
    */  
    protected: 
    
        /**
        * Sets bitrate used with codec.
        * @since Series 60 3.0
        * @param aBitrateMask bitrate mask, sets the largest bitrate
        * @return KErrNotSupported if codec doesn't support bitrate value
        *     issued; otherwise KErrNone.
        */
        virtual TInt SetBitrateFromBitrateMask( TUint aBitrateMask ); 
        
        /**
        * From CMccCodecInformation. Gets the mode set of a bitrate.
        * @since     Series 60 3.0
        * @param     aBitrate     The bitrate value
        * @return     mode set value
        */
        virtual TInt GetBitrateModeSet( TUint aBitrate );
        
        /**
        * From CMccCodecInformation. Gets the bitrate mask from the mode set .
        * @since     Series 60 3.0
        * @param     aMode The mode set
        * @param    aBitrateMask reference to bitrateMask
        * @return KErrNotSupported if codec doesnt' support the bitrate mask
        *     value issued; otherwise KErrNone.
        */
        virtual TInt GetBitrateMaskFromMode( TUint aMode, TUint& aBitrateMask  );

    
        /**
        * Gets the allowed bitrates for the codec.
        * @since  Series 60 3.0
        * @param  aBitrateMask RArray<TUint> reference 
        * @return KErrNotSupported if codec doesnt' support the function
        *         otherwise KErrNone.
        */
        virtual TInt GetAllowedBitratesArrayL( RArray<TUint>& aBitratesArray );
    
        /**
        * Sets the bitrate from the mode set .
        * @since     Series 60 3.0
        * @param     aBitrate     The mode set
        * @return    mode set value
        */
        virtual void SetBitrateModeSet( TUint aModeSet );

    
    public:
              
        /**
        * Get number of channels.
        * @since    Series 60 3.0
        * @return   One of the standard system-wide error codes.
        */
        TInt GetChannels( TInt& aNumOfChannels  ) const; 
        
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
       
        /**
        * Creates the fmtp string from internal values
        * @since Series 60 3.0
        * @return void
        */
        void CreateFmtpAttrListL();
      
        /**
        * Creates the fmtp string from internal values and the given bitrates
        * @since Series 60 3.0
        * @return void
        */
        void CreateFmtpAttrListL( const RArray<TUint>& aBitrates );
   
    
    protected: // New functions for derived classes
    
            
        /**
        * C++ default constructor.
        */
        CMccCodecAMR();
        
        /**
        * Parses the fmtp attribute and updates the right param  
        * @since Series 60 3.0
        * @param aFmtp Fmtp attribute
        * @return ETrue if any params were updated
        */
        TBool ParseFmtpSegmentL( const TDesC8& aFmtp );
        
        /**
        * Parses the fmtp attribute and updates the octet-align value
        * @since Series 60 3.0
        * @param aFmtp Fmtp attribute
        * @return ETrue if any params were updated
        */
        TBool ParseFmtpOctetAlignL( const TDesC8& aFmtpOctetAlign );
        
        /**
        * Parses the fmtp attribute and updates the mode-change-period value
        * @since Series 60 3.0
        * @param aFmtp Fmtp attribute
        * @return ETrue if any params were updated
        */
        TBool ParseFmtpModeChangePeriodL( const TDesC8& aFmtpModeChangePeriod );
        
        /**
        * Parses the fmtp attribute and updates the mode-change-neighbor value
        * @since Series 60 3.0
        * @param aFmtp Fmtp attribute
        * @return ETrue if any params were updated
        */
        TBool ParseFmtpModeChangeNeighborL( const TDesC8& aFmtpModeChangeNeighbor );
        
        /**
        * Parses the fmtp attribute and updates the crc value
        * @since Series 60 3.0
        * @param aFmtp Fmtp attribute
        * @return ETrue if any params were updated
        */
        TBool ParseFmtpCrcL( const TDesC8& aFmtpCrc );

        /**
        * Parses the fmtp attribute and updates the robust sorting value
        * @since Series 60 3.0
        * @param aFmtp Fmtp attribute
        * @return ETrue if any params were updated
        */
        TBool ParseFmtpRobustSortingL( const TDesC8& aFmtpRobustSorting );
        
    
        /**
        * Creates the octet-align value of the FMTP
        * @since Series 60 3.0
        * @param aBuf The buffer to modify
        * @return ETrue if any params were updated
        */
        TBool CreateFmtpOctetAlign( TDes8& aBuf );
   
        /**
        * Creates the mode-change-period value of the FMTP
        * @since Series 60 3.0
        * @param aBuf The buffer to modify
        * @return ETrue if any params were updated
        */
        TBool CreateFmtpModeChangePeriod( TDes8& aBuf );
        
        /**
        * Creates the mode-change-neigbor value of the FMTP
        * @since Series 60 3.0
        * @param aBuf The buffer to modify
        * @return ETrue if any params were updated
        */
        TBool CreateFmtpModeChangeNeighbor( TDes8& aBuf );
        
        /**
         * Creates the max-red value of the FMTP
         * @since Series 60 3.2
         * @param aBuf The buffer to modify
         * @return ETrue if any params were updated
         */
        TBool CreateFmtpMaxRed( TDes8& aBuf );
        
        /**
         * Parses the fmtp attribute and updates the max red value
         * @since Series 60 3.2
         * @param aFmtpMaxRed Fmtp attribute
         * @return ETrue if any params were updated
         */
        TBool ParseFmtpMaxRedL( const TDesC8& aFmtpMaxRed );
        
        /**
        * Parses the fmtp attribute and updates the mode-set value  
        * @since Series 60 3.0
        * @param aFmtp Fmtp attribute
        * @return ETrue if any params were updated
        */
        TBool ParseFmtpModeSetL( const TDesC8& aFmtpModeSet );
        
        /**
        * Creates the mode-set value of the FMTP according to the bitrates
        * @since Series 60 3.0
        * @param aBuf The buffer to modify
        * @param aBitrates Supported bitrates of the codec
        * @return ETrue if the buffer was modified
        */
        TBool CreateFmtpModeSet( TDes8& aBuf,
                                         const RArray<TUint>& aBitrates );
                
        /**
        * Parses the fmtp attribute and updates the interleaving value  
        * @since Series 60 3.0
        * @param aFmtp Fmtp attribute
        * @return ETrue if any params were updated
        */
        TBool ParseFmtpInterleavingL( const TDesC8& aFmtpInterleaving );
        
        /**
        * Specifies the number of frame blocks,
        * that is the interval at which codec mode changes are allowed
        * @since    Series 60 3.0
        * @return   One of the standard system-wide error codes.
        */
        TInt SetModeChangePeriod( const TInt aPeriod );
        
        /**
        * Get specified number of frame blocks
        * @since    Series 60 3.0
        * @return   One of the standard system-wide error codes.
        */
        TInt ModeChangePeriod( ) const;
        
        /**
        * Set boolean variable, which indicates change between two modes.
        * With default value 1 changes are only allowed to the 
        * neighboring mode.
        * @since    Series 60 3.0
        * @return   One of the standard system-wide error codes.
        */
        TInt SetModeChangeNeighbor( const TBool aNeighbor );
        
        /**
        * Get boolean variable, which indicates change between two modes.
        * @since    Series 60 3.0
        * @return   Return iNeighbor
        */
        TBool ModeChangeNeighbor( ) const;
        
        /**
        * Set boolean type frame CRC value. With value 1 CRC shall be
        * included in the payload, otherwise not. 1 also implies that
        * octet-alignet operation shall be used for the session.
        * @since    Series 60 3.0
        * @return   One of the standard system-wide error codes.
        */
        TInt SetCrc( const TBool aCrc );
        
        /**
        * Get boolean type CRC value.
        * @since    Series 60 3.0
        * @return   iCrc
        */
        TBool GetCrc( ) const;
        
        /**
        * Set robust sorting on/off with boolean value. With value 1
        * the payload SHALL employ robust sorting. Implies automatically
        * that octet-aligned operation SHALL be used for the session.
        * @since    Series 60 3.0
        * @return   One of the standard system-wide error codes.
        */     
        TInt SetRobustSorting( const TBool aRobust );
        
        /**
        * Get boolean type robust sorting value
        * @since    Series 60 3.0
        * @return   iRobust
        */
        TBool GetRobustSorting( ) const;
        
        /**
        * Set interleaving as unsigned integer. Number range is 0-63.
        * Indicates that frame-block level interleaving SHALL be
        * used for the session and it's value defines the max number of
        * frame-blocks allowed in interleaving group. Implies automatically
        * that octet-aligned operation SHALL be used.
        * @since    Series 60 3.0
        * @return   One of the standard system-wide error codes.
        */
        TInt SetInterleaving( const TInt aFrameBlockCount );

        /**
        * Get Interleavin value.
        * @since    Series 60 3.0
        * @return   iFrameBlockCount
        */
        TInt GetInterleaving( ) const;  
        
        void SetBitrateMask( TUint aBitrateMask, TBool aKeepAdditionalInfo = EFalse ); 
        
        void SetBitrateMaskAdditionalInfo( TUint aAdditionalInfo ); 
    
    protected:  // Data
    
        TUint iDefaultBitrateMask;
        
        TUint iDefaultBitrate;
        
        TUint iAdditionalInfoMask;
        
        TInt iMaxModeSetVal;
    
    private:    // Data
    
        /**
         * Mode-set exist in FMTP
         */
        TBool iModesExist;
        
        /**
         * Octet-align exist in FMTP
         */
        TBool iOctetAlignExist;
        
        /**
         * Mode-change-period exist in FMTP
         */
        TBool iModeChangePeriodExist;
        
        /**
         * Mode-change-neighbor exist in FMTP
         */
        TBool iModeChangeNeighborExist;
        
        /**
         * Max-red exist in FMTP
         */
        TBool iMaxRedExist;    
        
private:

    #ifdef TEST_EUNIT
        friend class UT_CMccCodecAMR;
        friend class UT_CMccCodecAmrWb;
    #endif
      
    };


#endif      // __MCC_CODEC_AMR_H__

// End of File
