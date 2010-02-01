/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Encapsulates DTMF settings.
*
*/





#ifndef __MCC_CODEC_DTMF_H__
#define __MCC_CODEC_DTMF_H__

#include <badesca.h>

//  INCLUDES
#include "mmcccodecinformation.h"

// CONSTANTS
const TInt KDTMFPTime( 50 );

// DTMF possible events number
const TUint KDtmfNumber0( 0 );
const TUint KDtmfNumber1( 1 );
const TUint KDtmfNumber2( 2 );
const TUint KDtmfNumber3( 3 );
const TUint KDtmfNumber4( 4 );
const TUint KDtmfNumber5( 5 );
const TUint KDtmfNumber6( 6 );
const TUint KDtmfNumber7( 7 );
const TUint KDtmfNumber8( 8 );
const TUint KDtmfNumber9( 9 );
const TUint KDtmfNumber10( 10 );
const TUint KDtmfNumber11( 11 );
const TUint KDtmfNumber12( 12 );
const TUint KDtmfNumber13( 13 );
const TUint KDtmfNumber14( 14 );
const TUint KDtmfNumber15( 15 );


const TUint KMccAllowedDtmfNumber0 = 0x0001;
const TUint KMccAllowedDtmfNumber1 = 0x0002;
const TUint KMccAllowedDtmfNumber2 = 0x0004;
const TUint KMccAllowedDtmfNumber3 = 0x0008;
const TUint KMccAllowedDtmfNumber4 = 0x0010;
const TUint KMccAllowedDtmfNumber5 = 0x0020;
const TUint KMccAllowedDtmfNumber6 = 0x0040;
const TUint KMccAllowedDtmfNumber7 = 0x0080;
const TUint KMccAllowedDtmfNumber8 = 0x0100;
const TUint KMccAllowedDtmfNumber9 = 0x0200;
const TUint KMccAllowedDtmfNumber10 = 0x0400;
const TUint KMccAllowedDtmfNumber11 = 0x0800;
const TUint KMccAllowedDtmfNumber12 = 0x1000;
const TUint KMccAllowedDtmfNumber13 = 0x2000;
const TUint KMccAllowedDtmfNumber14 = 0x4000;
const TUint KMccAllowedDtmfNumber15 = 0x8000;
const TUint KMccAllowedDtmfNumberALL = 0xFFFF;


// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Container class for the codec information.
*
*  @lib MCCInterface.dll
*  @since Series 60 3.0
*/
NONSHARABLE_CLASS( CMccCodecDTMF ) : public CMccCodecInformation
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CMccCodecDTMF* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CMccCodecDTMF();

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
        * Sets the allowed bitrates for the codec.
        * @param aBitrateMask bitrate mask.
        * @return KErrNotSupported if codec doesnt' support the bitrate mask
        *     value issued; otherwise KErrNone.
        */
        TInt SetAllowedBitrates( TUint aBitrateMask );
        
        /**
        * From CMccCodecInformation.
        * Parses the fmtp attributes, and updates the internal values
        * except the iFmtpAttr attribute
        * @param aFmtp Fmtp attribute
        * @return True if any param was updated
        */
        TBool ParseFmtpAttrL( const TDesC8& aFmtp );
        
        /**
        * From CMccCodecInformation. Gets the codec fmtp parameters.
        * @return Fmtp parameters of the codec
        */
        TDesC8& GetFmtpL();
        
        /**
        * From CMccCodecInformation.
        */
        void CreateFmtpAttrListL();
        
        
    private:

        /**
        * C++ default constructor.
        */
        CMccCodecDTMF();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
    private:
        
  
        /*
        * store tokens sperated by aSeparator to array.
        *
        * @param aValue, a sting contains comma seperated numbers
        * @param aSeparator, a char
        * @return tokens array, ownership is transferred
        */
        CPtrC8Array* TokenizeL( const TDesC8& aValue,
                                const TChar& aSeparator );
        
        /*
        * add one token, which is taken from aValue 
        * from aTokenStartPos to aTokenEndPos, to token array
        * e.g string value is 0-15,60,70. tokens are 0-15, 60 and 70
        *
        * @param aTokenArray
        * @param aValue, 
        * @param aTokenStartPos, 
        * @param aTokenEndPos 
        * @return None, leave in case of errors
        */
        void AddTokenL ( CPtrC8Array& aTokenArray,
                         const TDesC8& aValue, 
                         TInt aTokenStartPos,
                         TInt aTokenEndPos );
             
        /*
        * parse the tokens arrays, and get DTMF tone numbers
        *
        * @param aTokenArray, a list of numbers
        * @return ETrue, if Tone numbers are updated.
        */           
        TBool EncodeTokensL( MDesC8Array& aTokenArray );
        
        /*
        * parse one token, and get DTMF tone numbers
        *
        * @param aValue, a number or two decimal number 
        * @return ETrue, if Tone numbers are updated.
        */    
        TBool ParseTokenL( const TDesC8& aValue );
        
        /*
        * parse one tone number from one token, and get DTMF tone numbers
        * e.g one token 60
        *
        * @param aValue, token value  
        * @return ETrue, if Tone numbers are updated.
        */  
        TBool ParseToneNumberL( const TDesC8& aValue );
        
       /*
        * parse tone numbers from one token, and get DTMF tone numbers
        * e.g one token is 0-15
        *
        * @param aValueLeft, value from left side of hyphen '-' 
        * @param aValueRight, value from Right side of hyphen '-' 

        * @return ETrue, if Tone numbers are updated.
        */  
        TBool ParseToneNumbersL( const TDesC8& aValueLeft, const TDesC8& aValueRight );
        
        /**
        * add tone digit numbers to from left number to right number.
        * eg. 0-15. aLeftNumber = 0, 15 = aRightNumber
        * leave if error happens.
        * @param aLeftNumber
        * @param aRightNumber
        * @return ETrue if tone numbers are added
        */
        TBool AddToneNumbersL( TUint aLeftNumber, TUint aRightNumber );
        
        void GetAllowedToneNumbersArrayL( RArray<TUint>& aToneNumbers );
        
        void CreateFmtpAttrListL( const RArray<TUint>& aToneNumbers );
        
        TBool AddAllowedBit( TUint aBit );
        
        void FindContinuusIntegers( const RArray<TUint>& aToneNumbers, 
                                    TUint& aPosition );
                        
    private:
        #ifdef TEST_EUNIT
            friend class UT_CMccCodecDTMF;
        #endif

    };

#endif      // __MCC_CODEC_DTMF_H__   
            
// End of File
