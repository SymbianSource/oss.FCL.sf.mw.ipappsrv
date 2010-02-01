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
* Description:    Class is used for configuring mcc srtp crypto params
*
*/




#ifndef MMCCSRTPCRYPTOPARAMS_H
#define MMCCSRTPCRYPTOPARAMS_H

// INCLUDES
#include <e32base.h>


// CONSTANTS
// The default bit length of the HMAC-SHA1 authentication tag
const TUint KMccSRTPAuthTagDefaultLength = 80;

// default SRTP key derivation rate: 0
const TUint16 KMccSRTPDefaultKeyDerivationRate = 0x00;

// The default length of the SRTP prefix
const TUint KMccSRTPPrefixLength = 0;

const TInt KMccReplayWindowSize = 64;

// default SRTP ROC Transmission rate: 1
const TUint16 KMccSRTPDefaultROCTransRate = 0x01;

// The 80 bit length of the HMAC-SHA1 authentication tag (n_tag)
const TUint KMccSRTPAuthTagLength80 = 80;

// The 32 bit length of the HMAC-SHA1 authentication tag (n_tag)
// Also recommending in RCCm3 as 4 octects
const TUint KMccSRTPAuthTagLength32 = 32;

//recommending to use in RCCm1 or RCCm2 as 14 octets
const TUint KMccSRTPAuthTagLength112 = 112;

/**
 * Encyption algorithms.
 */
enum TMccSRTPEncAlg
    {
    ESrtpNullAlg = 0,
    ESrtpEncAES_CM
    };

/**
 * Authentication algorithms.
 * With RCCm1, RCCm2 and RCCm3, ROC will be synchronized if SEQ%R==0
 */
enum TMccSRTPAuthAlg
    {
    ESrtpAuthNull = 0,
    ESrtpAuthHMAC_SHA1,
    ESrtpAuthRCCm1,
    ESrtpAuthRCCm2,
    ESrtpAuthRCCm3
    };

/**
 * The TMccSrtpCryptoParams captures the MCC SRTP crypto params
 * into one class
 *
 * @lib N/A
 * @since Series 60 3.2
 */    
class TMccSrtpCryptoParams
	{

public:
 
    //IF any of the following param changes it will cause the caculation changes.
    inline TMccSrtpCryptoParams::TMccSrtpCryptoParams() : 
      	iSrtpEncAlg( ESrtpEncAES_CM ), 
       	iSrtcpEncAlg( ESrtpEncAES_CM ), 
        iSrtpAuthAlg( ESrtpAuthHMAC_SHA1 ), 
        iSrtcpAuthAlg(ESrtpAuthHMAC_SHA1 ), 
        iSrtpAuthTagLen( KMccSRTPAuthTagDefaultLength ),
        iSrtcpAuthTagLen(KMccSRTPAuthTagDefaultLength ),
        iSrtpKeyDervRate( KMccSRTPDefaultKeyDerivationRate ),
        iSrtcpKeyDervRate( KMccSRTPDefaultKeyDerivationRate ),
        iPrefixLen( KMccSRTPPrefixLength ), 
        iSrtpReplayProtection( ETrue ), 
        iSrtcpReplayProtection( ETrue),
        iROCTransRate( KMccSRTPDefaultROCTransRate ), 
        iMasterKeysLifeTime ( 0 ),
        iReplayWindowSizeHint ( KMccReplayWindowSize ),
        iIsRCCm3Sync( EFalse )                 
    	{
    	};

    TMccSRTPEncAlg iSrtpEncAlg;     //encoding algorithm
    TMccSRTPEncAlg iSrtcpEncAlg;                
    TMccSRTPAuthAlg iSrtpAuthAlg;   // authentication algorithm
    TMccSRTPAuthAlg iSrtcpAuthAlg; 
    TUint iSrtpAuthTagLen;
    TUint iSrtcpAuthTagLen;             
    TUint iSrtpKeyDervRate;         //The key derivation rate
    TUint iSrtcpKeyDervRate;    
    TUint iPrefixLen;               // prefix length	 must be 0
    TBool iSrtpReplayProtection;    // replay protection on/off
    TBool iSrtcpReplayProtection;   
    TUint16 iROCTransRate;          // Integrity transform parameter
    TUint64 iMasterKeysLifeTime;    //Set both for SRTP/SRTCP otherwise use default
    TInt iReplayWindowSizeHint;     //Set both for SRTP/SRTCP otherwise use default
    TBool iIsRCCm3Sync;             // using in RCCm3 only defined in RFC srtp-rcc04 Section 4	
	};

		
typedef TPckgBuf<TMccSrtpCryptoParams> TMccSrtpCryptoParamsPckg;

#endif /* MMCCSRTPCRYPTOPARAMS_H */
            
// End of File
