/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides services for SDP Security Descriptions.
*
*/





#ifndef CMCESECUREDESSTREAM_H
#define CMCESECUREDESSTREAM_H

//  INCLUDES
#include <e32base.h>
#include <stringpool.h>
#include <mmccsrtpcryptoparams.h>
//#include <mmccsecureinterface.h>
#include "mcesecuremediasession.h"
#include "mcesecurecryptoinfo.h"
#include "mcesrvstream.h"

// FORWARD DECLARATIONS
class CSdpDocument;
class CSdpMediaField;
class CMceComSession;
class CMceComMediaStream;
class CSdpAttributeField;
class CSdpFmtAttributeField;
class CMccSecureInterface;
class TMccSrtpCryptoParams;
class TMceSecureCryptoInfo;
//  INCLUDES


// CONSTANTS
_LIT8( KInline, "inline:" );
_LIT8( KSeparator, "|" );
_LIT8( KInvolute, "^" );
_LIT8( KColon, ":" );
_LIT8( KSemiColon, ";" );
_LIT8( KAES_SHA1_80, "AES_CM_128_HMAC_SHA1_80" );
_LIT8( KAES_SHA1_32, "AES_CM_128_HMAC_SHA1_32" );
_LIT8( KCrypto, "crypto" );
_LIT8( KCryptoCount, "1" );
_LIT8( KDefaultMasterKeyLifetime, "2^20" );
_LIT8( KDefaultMKI, "1:4 " );
_LIT8( KSpace, " " );
_LIT8( KCryptoCount2, "2" );
_LIT8( KProtocolSAVP, "RTP/SAVP" );
_LIT8( KProtocolSAVPF, "RTP/SAVPF" );
_LIT8( KProtocolAVP, "RTP/AVP" );

const TInt KInlineLength = 7;
const TInt KRandomStringCount = 64;
const TInt KProtocolMaxLength = 20;
const TInt KCryptoLineMaxLength = 120;
const TInt KCryptoAttributeValueMinLength = 69; //when F8_128_HMAC_SHA1_80
const TUint KEncrKeyLengthDefault = 0x80;
const TUint KAuthKeyLengthDefault = 0xa0;
const TUint KDerivRateDefault = 0x00;


const TUint KAuthTagLengthTypeCount = 2;
const TUint KMKILength = 4;

const TUint KSeparatorLen = 1;
const TUint KMkiValueLen = 3;
const TUint KMaxMKILen = 10;
const TUint64 KMasterKeyLifeTime = 0x100000;
const TInt KCryptoAnswerCount = 1;
const TInt KTotalCryptoAnswerCount = 2;
const TInt KCryptoGranularity = 4;

static const TInt KBase64Chars[ 64 ] = 
        {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 
        'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c',  
        'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',  
        'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '+', '/'
        };

static const TInt KRandomChars[ 64 ] = 
        {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 
        'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c',  
        'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',  
        'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '_', '-'
        };

static const TInt KNumbers[ 10 ] = 
        {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
        };

//
/**
*  Class for encoding and decoding media lines and creating server streams
*  
*/
class CMceSecureDesStream : public CBase
    {

    public:
        
        /**
        * Two-phased constructor.
        * @param aSecureSession secure media session
        * @param aMediaField secure description stream uses
        * @param aSecureInterface
        * @param aMediaStream media stream
        */
        static CMceSecureDesStream* NewL( CMceSecureMediaSession& aSecureSession,
										  CSdpMediaField& aMediaField,
										  CMccSecureInterface& aSecureInterface,
										  CMceComMediaStream& aMediaStream);

        /**
        * Destructor.
        */
        ~CMceSecureDesStream();
        
  

public: 

        /**
        * Encodes a media line to Secure Offer/Answer. 
        * @param aMediaField, one of mediafield in sdp document
        * @param aIsAnswer if it is answer or offer/update types
        * @leave system wide error only if Sdp document is not valid
        */ 
        void EncodeSecureSdpL( 	CSdpMediaField& aMediaField,
								TBool aIsAnswer ) ;
  
        
        /**
        * Decodes media offer/update
        * @param aMediaField, one of mediafield in sdp document
        * @return system KErrNotFound or KErrArgument
        * @leave system wide error only if Sdp document is not valid
        */
        void DecodeSecureSdpL( CSdpMediaField& aMediaField ) ;
         
        
        /**
        * Decodes media answer 
        * @param aMediaField, one of mediafield in sdp document
        * @param aMode Security stream mode based on sdp document offer, 
        *  update, answer, refresh
        * @return system KErrNotFound or KErrArgument
        * @leave system wide error only if Sdp document is not valid
        */
        void DecodeSecureSdpAnswerL( CSdpMediaField& aMediaField );                                              
        
        
        /**
        * Remove Crypto line from sdp document
        * @param aMediaField, one of mediafield in sdp document
        * @return void
        */
        void RemvoeSecureSdp( CSdpMediaField& aMediaField ) ;
        
        
       
        /**
        * Return this Media Field 
        * @return CSdpMediaField for this security stream
        */
        CSdpMediaField& MediaField();
        
        /**
        * Return this related Codec
        * @return CMceComCodec for this security stream
        */
        CMceComMediaStream& MediaStream() const;
        
        
        /**
        * Compare if the context Id is in this secure stream
        * @return ETrue if there is one context Id match 
        */
        TBool CompareContextId( TUint32 aContextId );
        
        
        /**
        * BindCrypto to MCC
        * @param aStream SeverStream
        * @return KErrNone if successful 
        */
        TInt BindCrypto( CMceSrvStream& aStream );
        
        /**
        * Copy Stream CryptoInfo to new Security Description Stream 
        * while updating Offer
        * @param aCopyFrom SecureStream to be copied
        * @return void 
        */
       	void CopyStreamCryptoL( CMceSecureDesStream& aCopyFrom );
       	void ForceUpdateStreamL();
    
   private:     
    	/**
        * Set Crypto Context to MCC after Encode/Decode SDP are done
        * @return void
        */
 		void SetCryptoContextL( );
 		
        
		/**
        * Base64 encoding
        * @since    Series 60 3.0 
        * @param    aData,the data to encode into Base 64
        * @param    aEncoded, encoded result
        * @return   None
        */
        void Base64Encode( const TDesC8& aData, TDes8& aEncoded );
        
        /**
        * Base64 decoding
        * @since    Series 60 3.0 
        * @param    aData,the data to encode into Base 64
        * @param    aDecoded, decoded result
        * @return   None
        */
        void Base64Decode( const TDesC8& aData, TDes8& aDecoded );
        
        /**
        * Validation of security descriptions
        * @since    Series 60 3.0 
        * @param    aData, data to be checked 
        * @return   ETrue, if valid
        */
        TBool ValidateSecurityDescriptions( TDesC8& aData );
        
        /**
        * Generate cryptoline for crypto -attribute
        * @since    Series 60 3.0 
        * @param    aResult is the cryptoline 
        * @param    aCryptoCount =1 or 2....
        * @param	aAnswer if is an Answer or offer/update
        * @return   None
        */
        void GenerateCryptoLineL( TDes8& aResult, 
        						 TInt aCryptoCount, 
        						 TBool aAnswer );
        
        /**
        * Generate cryptoline for crypto -attribute
        * @since    Series 60 3.0 
        * @param    aResult is the cryptoline 
        * @param    aCrypto secure crypto info
        * @return   void
        */
        void GenerateCryptoSuiteLineL( TDes8& aResult, 
									  TMceSecureCryptoInfo& aCrypto);
        /**
        * Generates random string
        * @since    Series 60 3.0 
        * @param    aRandom is the created random string 
        * @return   None
        */
        void RandomString( TDes8& aRandom );
        
        /**
        * Generates random string
        * @since    Series 60 3.0 
        * @param    aCrypto secure crypto info
        * @return   None
        */
        void GenerateRandomKeys(TMceSecureCryptoInfo& aCrypto);
        
        /**
        * Store the exchanged keys
        * @since    Series 60 3.0 
        * @param    aData sdp key inforamtion
        * @return   None
        */
        void StoreKeys(TDesC8& aData);
        
        /**
        * Sets crypto context to MCC 
        * @param aType to set crypto context 
        * @return   KErrNone, if successful
        */
        void CreateCryptoContextL(TMceSecureCryptoInfo& aCrypto);
        
        /**
        * Update crypto context to MCC 
        * @param aType to set crypto context 
        * @return   KErrNone, if successful
        */
        void UpdateCryptoContextL( TMceSecureCryptoInfo& aCrypto );
        
        /**
        * Decode MKI life time information
        * @param aSecDec security inforamtion in SDP
        * @param aCrypto aCryptoContext used in incoming offer
        * @return   KErrNone, if successful
        */
        void DecodeMKLifeTimeL(const TDesC8& aSecDec,
    						TMceSecureCryptoInfo& aCrypto);
        
    	/**
        * Decode MKI value information
        * @param aSecDec security inforamtion in SDP
        * @param aIsAnswer if it is answer type or offer/update type
        * @param aCrypto aCryptoContext used in incoming offer
        * @return   void
        */
        void DecodeMKIValueL(	const TDesC8& aSecDec,
								TBool aIsAnswer,
    							TMceSecureCryptoInfo& aCrypto );
    	
    
    			
		CSdpAttributeField* MediaFieldAttrMatch( 	CSdpMediaField& aMediaField,
													const TDesC8& aMatchString );	
		
		TBool ValidateMediaProfile( CSdpMediaField& aMediaField );	
		
		TInt SetMediaProfile( CSdpMediaField& aMediaField );
		
		void AppendCryptoAttributeL( 	TDesC8& aCryptoLine,
										CSdpMediaField& aMediaField );
		
		void ValidateAnswerByOfferL( const TDesC8& aSecDec);
    	
    	void ValidateOfferByAnswerL( const TDesC8& aSecDec );
    	
    	void SetSecureProtocolL(CSdpMediaField& aMediaField);
    	
    	void SetPreferedCryptoL();
    	
    	void SetClientCryptoL(TInt aCryptoCount);
    	
    	TInt ClientCrytoCount();
    			
		void IfMatchLocalCryptoL(const TDesC8& aSecDec);	
		
		TInt SearchAndSetCrypto(TMceSecureCryptoInfo& aCrypto);		
		
		
		void FormMKIL(TMceSecureCryptoInfo& aCrypto);			
		
		void StoreCryptoInFromOfferL(const TDesC8& aSecDec,
									TMceSecureCryptoInfo& aCrypto);		
		
		TInt CompareCryptosForAnswer();	
		
		void RemoveClientCrypto();		
		
		void SetClientCryptoL(TMceSecureCryptoInfo& aCrypto);
		
		TInt CountCryptoInOffer(CSdpMediaField& aMediaField);	
		
		void SetMultipleClientCryptoL( CArrayFixFlat< TMceSecureCryptoInfo >& aArray );
		
		CMceComSession& Session();
		
		TInt SearchChar( TDesC8& aSearchLine, TChar& aChar,  
										TInt aSearchFrom  );
		void AppendEmptyCryptoL( CArrayFixFlat<TMceSecureCryptoInfo>& aCryptos,
							TInt aCount );
							
		void SetSHA132( TMceSecureCryptoInfo& aCrypto, TInt aTag );
		
		void SetSHA180( TMceSecureCryptoInfo& aCrypto, TInt aTag );
		
		void SetDefaultCryptoL( 
							CArrayFixFlat<TMceSecureCryptoInfo>& aCryptos );
		void SetCryptoByClientL( 
							CArrayFixFlat<TMceSecureCryptoInfo>& aCryptos );														
    
    private:

    
        CMceSecureDesStream( CMceSecureMediaSession& aSecureSession,
							 CMccSecureInterface& aSecureInterface,
        					 CMceComMediaStream& aMediaStream);
        									
        void ConstructL(CSdpMediaField& aMediaField);
        
        
	public: //Data
		
		TBool iCryptoUpdateNeeded;
		TMceSecureCryptoInfo iCryptoOut;
    	TMceSecureCryptoInfo iCryptoIn;
    	TBool iGnoreSdpMsg;
    	
    private://data
    
        RStringPool iStringPool;

        const TStringTable* iStringTable;
  
		CMceSecureMediaSession& iSecureSession;
        
        CArrayFixFlat<TMceSecureCryptoInfo>* iCryptoOuts;
        
        CArrayFixFlat<TMceSecureCryptoInfo>* iCryptoIns;
  
    	CMccSecureInterface& iSecInf;
    
    	CMceComMediaStream& iMediaStream;
    	
    	CSdpMediaField* iMediaField;

    	TBool iIsSAVP;

    	TBool iWaitingBinding;

    	TUint32 iCryptoContextOutId;

    	TUint32 iCryptoContextInId;	

		HBufC8* iMKIBuf;

        TUint iOldLocalMediaPort;

    friend class UT_CMceMediaManager;       
    friend class UT_CMceSecureMediaSession;
    friend class UT_CMceSecureDesStream;


    };

#endif      // CMCESECUREDESSTREAM_H
            
// End of File
