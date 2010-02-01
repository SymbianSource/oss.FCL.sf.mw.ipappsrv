/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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






#ifndef MCESECURECRYPTOINFO_H
#define MCESECURECRYPTOINFO_H

#include <e32std.h>
#include "mceclientserver.h"
#include <mmccsrtpcryptoparams.h>

_LIT( KUnEcryptedSrtp,"UNENCRYPTED_SRTP" );
_LIT( KUnEcryptedSrtcp,"UNENCRYPTED_SRTCP" );
_LIT( KUnAuthenticcatedSrtp,"UNAUTHENTICATED_SRTP" );
_LIT( KKeyDrivationRate,"KDR=");
_LIT( KFecOrder,"FEC_ORDER=");
_LIT( KFecType,"FEC_SRTP");
_LIT( KFecKey,"FEC_Key=");
_LIT( KWSH,"WSH=");

//2^31 -1 for SRTCP to use as default for both rtp/rtcp
/*lint -e435 */

const TUint64 	KDefalutMaterKeysLifeTime = 0x7FFFFFFF;
const TUint 	KAuthTagLength80 = 80;
const TUint 	KAuthTagLength32 = 32;
const TUint 	KSaltLength = 0xe ;
const TUint 	KMasterKeyLength = 0x10;
const TUint 	KMasterKeyAndSaltLength = 0x1e;
const TUint 	KEncodedStringLength = 0x28;
const TUint 	KSessionExtLen = 0x64;
const TUint16 	KMKIDesCLen = 0x101;
const TUint64 	KWSHLen = 0x100000000;
const TUint	  	KDRLen = 0x2;		 
const TUint64  	KMLTLen = 0x1000000000000;
const TUint 	KCryptoSuiteLen = 0x18;
typedef TBool TSrtpSessionParam;

/*
 * Cryptography Information
 *
 */
class TMceSecureCryptoInfo
    {
public:

	inline TMceSecureCryptoInfo()
		: iTag( KMceNotAssigned ),
		  iTagLen( KMceNotAssigned ),	
		  iCryptoContextId( KMceNotAssigned ),
		  iMKIValue( KMceNotAssigned ),
		  iMKILength( KMceNotAssigned ),
		  iMKLifeTime(0),
		  iMKIUsed(EFalse),
		  iIfBinded(EFalse),
		  iIfCryptoContextIdSet( EFalse ),
		  iKeysCreated( EFalse ),
		  iEncAlgms( ESrtpEncAES_CM ),
		  iAuthAlgms( ESrtpAuthHMAC_SHA1 ),
          iSetMasterKey( KNullDesC8 ),
          iSetSaltKey( KNullDesC8 ),
          iEncodedKey( KNullDesC8 ),
          iKeyParamCount(0),
          iIfFEC( EFalse ),
          iIfUnEncSRTP(EFalse),
		  iIfUnEncSRTCP(EFalse),
		  iIfUnAuthSRTP(EFalse),
		  iIfSrtpSessionExt(EFalse),
		  iCryptoSuite( KNullDesC8 ),
		  iMKI( KNullDesC8 )
		   {}



	inline TMceSecureCryptoInfo( const TMceSecureCryptoInfo& aInfo )
		: iTag(aInfo.iTag),
		  iTagLen(aInfo.iTagLen),
		  iCryptoContextId( aInfo.iCryptoContextId ),
		  iMKIValue(aInfo.iMKIValue),
		  iMKILength(aInfo.iMKILength),
		  iMKLifeTime(aInfo.iMKLifeTime),
		  iMKIUsed(aInfo.iMKIUsed),
		  iIfBinded(aInfo.iIfBinded),
		  iIfCryptoContextIdSet( aInfo.iIfCryptoContextIdSet),
		  iKeysCreated(aInfo.iKeysCreated),
		  iEncAlgms(aInfo.iEncAlgms),
		  iAuthAlgms(aInfo.iAuthAlgms),
          iSetMasterKey(aInfo.iSetMasterKey),
          iSetSaltKey(aInfo.iSetSaltKey),
          iEncodedKey(aInfo.iEncodedKey),
          iKeyParamCount(aInfo.iKeyParamCount),
          iIfFEC( aInfo.iIfFEC ),
          iIfUnEncSRTP(aInfo.iIfUnEncSRTP),
		  iIfUnEncSRTCP(aInfo.iIfUnEncSRTCP),
		  iIfUnAuthSRTP(aInfo.iIfUnAuthSRTP),
		  iIfSrtpSessionExt( aInfo.iIfSrtpSessionExt ),
		  iCryptoSuite( aInfo.iCryptoSuite ),
		  iMKI( aInfo.iMKI ) 
		  {
		  if(aInfo.iKeyParamCount)
			  {
			  iMasterKeys.Reset();
			  iSaltKeys.Reset();
			  iEncodedKeys.Reset();
			  TInt count =aInfo.iMasterKeys.Count();
			  for (int index=0; index<count; index++)
			  	{
			  	iMasterKeys.Append(aInfo.iMasterKeys[index]);
			  	}
			  count =aInfo.iSaltKeys.Count();	
			  for (int index=0; index<count; index++)
			  	{
			  	iSaltKeys.Append(aInfo.iSaltKeys[index]);
			  	}	
			  count =aInfo.iEncodedKeys.Count();	
			  for (int index=0; index<count; index++)
			  	{
			  	iEncodedKeys.Append(aInfo.iEncodedKeys[index]);
			  	}	
			  }
		  }
	
	inline void Copy( const TMceSecureCryptoInfo& aInfo )
	    {
	    iTag = aInfo.iTag;
	    iTagLen = aInfo.iTagLen;
		iCryptoContextId=aInfo.iCryptoContextId;
		iMKIValue = aInfo.iMKIValue;
		iMKILength = aInfo.iMKILength;
		iMKLifeTime=aInfo.iMKLifeTime;
		iMKIUsed=aInfo.iMKIUsed;
		iIfBinded=aInfo.iIfBinded;
		iIfCryptoContextIdSet=aInfo.iIfCryptoContextIdSet;
		iKeysCreated = aInfo.iKeysCreated;
		iEncAlgms = aInfo.iEncAlgms;
		iAuthAlgms = aInfo.iAuthAlgms;
	    iSetMasterKey = aInfo.iSetMasterKey;
	    iSetSaltKey = aInfo.iSetSaltKey;
	    iEncodedKey = aInfo.iEncodedKey;
	      
	    iKeyParamCount = aInfo.iKeyParamCount;
	    iIfFEC = aInfo.iIfFEC;
	    iIfUnEncSRTP = aInfo.iIfUnEncSRTP;
		iIfUnEncSRTCP = aInfo.iIfUnEncSRTCP;
		iIfUnAuthSRTP = aInfo.iIfUnAuthSRTP;
		iIfSrtpSessionExt = aInfo.iIfSrtpSessionExt;
		iMKI = aInfo.iMKI;
		if( aInfo.iKeyParamCount )
		  {
		  iMasterKeys.Reset();
		  iSaltKeys.Reset();
		  iEncodedKeys.Reset();
		  TInt count = aInfo.iMasterKeys.Count();
		  for (int index=0; index<count; index++)
		  	{
		  	iMasterKeys.Append(aInfo.iMasterKeys[index]);
		  	}
		  count = aInfo.iSaltKeys.Count();	
		  for (int index=0; index<count; index++)
		  	{
		  	iSaltKeys.Append(aInfo.iSaltKeys[index]);
		  	}	
		  count = aInfo.iEncodedKeys.Count();	
		  for (int index=0; index<count; index++ )
		  	{
		  	iEncodedKeys.Append( aInfo.iEncodedKeys[index] );
		  	}	
		  }
	
		iCryptoSuite = aInfo.iCryptoSuite;
        }
 
    inline TBool IsContextId(TUint32 aContextId) const
        {
        return (
            iIfCryptoContextIdSet &&
            iCryptoContextId == aContextId );
        }
    
    inline void ResetKeys() 
        {
        iSetMasterKey = KNullDesC8;
        iSetSaltKey = KNullDesC8;
        iEncodedKey = KNullDesC8;
        }    

    inline void CryptoDestruct() 
        {
        iSetMasterKey.Des().Zero();
        iSetSaltKey.Des().Zero();
        iEncodedKey.Des().Zero();
        iMKI.Des().Zero();
        iMasterKeys.Close();
        iSaltKeys.Close();
        iEncodedKeys.Close();
        }    
            
public://data

    TUint32     					iTag;     //crypto Tag
    TUint							iTagLen; //authentication tag len
    TUint32     					iCryptoContextId;//1
    TUint32							iKeyParamCount; //2//how many inline under one attribute
    TUint32							iMKIValue; //3
    TUint32							iMKILength;//4
    TUint64 						iMKLifeTime;
    TBool 							iMKIUsed; 	//5
    TBool 							iIfBinded; //6
    TBool							iIfCryptoContextIdSet;
    TBool							iKeysCreated;
    TMccSRTPEncAlg 					iEncAlgms;
    TMccSRTPAuthAlg 				iAuthAlgms;
    TBufC8< KMasterKeyLength > 		iSetMasterKey;
    TBufC8< KSaltLength> 			iSetSaltKey;
    TBufC8< KEncodedStringLength> 	iEncodedKey;
    
    RArray <TDesC8> 				iMasterKeys; //if there is more than one inline key method
    RArray <TDesC8> 				iSaltKeys;
    RArray <TDesC8> 				iEncodedKeys;
    
    TSrtpSessionParam				iIfFEC;/* = EFalse*/
	TSrtpSessionParam				iIfUnEncSRTP;
	TSrtpSessionParam 				iIfUnEncSRTCP;
	TSrtpSessionParam				iIfUnAuthSRTP;
	TSrtpSessionParam				iIfSrtpSessionExt;/* = EFlase*/
	TBufC8< KCryptoSuiteLen > 		iCryptoSuite;
	TBufC8<	KMKIDesCLen >			iMKI;
	};

#endif // MCESECURECRYPTOINFO_H
// End of File
