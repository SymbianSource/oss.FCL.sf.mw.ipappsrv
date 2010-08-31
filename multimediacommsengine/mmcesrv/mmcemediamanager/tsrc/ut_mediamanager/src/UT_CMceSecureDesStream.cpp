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




//  CLASS HEADER
#include "UT_CMceSecureDesStream.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include <e32math.h>
#include <SdpCodecStringConstants.h>
#include <SdpCodecStringPool.h>
#include <sdpdocument.h>
#include <SdpOriginField.h>
#include <SdpConnectionField.h>
#include <SdpMediaField.h>
#include <SdpAttributeField.h>
#include <SdpFmtAttributeField.h>
#include <SdpRtpmapValue.h>
#include "CMccController_stub.h"
#include "MceMediaManagerTestHelper.h"
#include "mcemediamanager.h"
#include "mcemediasdpcodec.h"
#include "mmccinterface_stub.h"
#include "cleanupresetanddestroy.h"
#include "mcesecuredesstream.h"
#include "mcevideosdpcodec.h"

#include "mcemediadefs.h"
#include "mcecomaudiocodec.h"

//  INTERNAL INCLUDES

_LIT8( KSdpName, "audio" );
_LIT8( KMKI1, "1:4" );
_LIT8( KMLT1, "2^20" );

_LIT8( KMKI2, "1:1" );
_LIT8( KMLT2, "2^10" );
_LIT8( KEncodedKey, "WVNfX19zZW1jdGwgKCkgewkyMjA7fQp9CnVubGVz");

_LIT8( KCryptoLine, "AES_CM_128_HMAC_SHA1_80 inline:WVNfX19zZW1jdGwgKCkgewkyMjA7fQp9CnVubGVz|2^20|1:4");

_LIT8( KCryptoLine2, "AES_CM_128_HMAC_SHA1_80 inline:WVNfX19zZW1jdGwgKCkgewkyMjA7fQp9CnVubGVz|1:4");
_LIT8( KCryptoLine3, "1 AES_CM_128_HMAC_SHA1_80 inline:ejRFLWhTT2NmQ0V0WVZUdkV6TjdxYk9Sak5naWdI");

//testing with MKI & MasterKeyLT
_LIT8( KSecAttrValue1, "1 AES_CM_128_HMAC_SHA1_80 inline:WVNfX19zZW1jdGwgKCkgewkyMjA7fQp9CnVubGVz|2^20|1:4 FEC_ORDER=FEC_SRTP");
_LIT8( KSecAttrValue2, "2 AES_CM_128_HMAC_SHA1_80 inline:WVNfX19zZW1jdGwgKCkgewkyMjA7fQp9CnVubGVz|2^20|1:4 FEC_ORDER=FEC_SRTP");
//not supported cryptosuite
_LIT8( KSecAttrValue3, "2 F8_128_HMAC_SHA1_80 inline:WVNfX19zZW1jdGwgKCkgewkyMjA7fQp9CnVubGVz|2^20|1:4 FEC_ORDER=FEC_SRTP");
_LIT8( KSecAttrValue4, "2 F8_128_HMAC_SHA1_80 inline:MTIzNDU2Nzg5QUJDREUwMTIzNDU2Nzg5QUJjZGVm|2^20|1:4;inline:QUJjZGVmMTIzNDU2Nzg5QUJDREUwMTIzNDU2Nzg5|2^20|2:4 FEC_ORDER=FEC_SRTP");
//not valid MKI length 
/* When using the MKI, the MKI length MUST be the same for all keys in a
   given crypto attribute.
*/
_LIT8( KSecAttrValue5, "2 AES_CM_128_HMAC_SHA1_80 inline:WVNfX19zZW1jdGwgKCkgewkyMjA7fQp9CnVubGVz|2^10|1:1;inline:QUJjZGVmMTIzNDU2Nzg5QUJDREUwMTIzNDU2Nzg5bGVz|2^20|2:2 FEC_ORDER=FEC_SRTP");
//Only MasterKeyLifteTime
_LIT8( KSecAttrValue6, "1 AES_CM_128_HMAC_SHA1_80 inline:WVNfX19zZW1jdGwgKCkgewkyMjA7fQp9CnVubGVz|2^20");
//Only MKI
_LIT8( KSecAttrValue7, "1 AES_CM_128_HMAC_SHA1_80 inline:WVNfX19zZW1jdGwgKCkgewkyMjA7fQp9CnVubGVz|1:4");
//Only Session Param
_LIT8( KSecAttrValue8, "1 AES_CM_128_HMAC_SHA1_80 inline:WVNfX19zZW1jdGwgKCkgewkyMjA7fQp9CnVubGVz FEC_ORDER=FEC_SRTP");

_LIT8( KSecAttrNotValidLength, "1 AES_CM_128_HMAC_SHA1_80 inline:WVNfX19zZW1jdGwgKCkgewkyMjA7fQp9CnVubGV");
_LIT8( KSecAttrNoInline, "1 AES_CM_128_HMAC_SHA1_80 WVNfX19zZW1jdGwgKCkgewkyMjA7fQp9CnVubGVzinlinex");
_LIT8( KSecAttrNotValidKey, "1 AES_CM_128_HMAC_SHA1_80 inline:WVNfX19zZW1jdGwgKCkgewkyMjA7fQp9CnVubGVzinlinex");

_LIT8( KSecAttrCryptoNotSupport, "a=crypto:2 F8_128_HMAC_SHA1_80 inline:MTIzNDU2Nzg5QUJDREUwMTIzNDU2Nzg5QUJjZGVm|2^20|1:4");
_LIT8( KSecAttrOnlyKey, "1 AES_CM_128_HMAC_SHA1_80 inline:WVNfX19zZW1jdGwgKCkgewkyMjA7fQp9CnVubGVz");


#define SRVSTREAM( n ) iSession->MccStreams()[ n ]

_LIT8( KMceTestMeidaCryptoAVP, "\
m=audio 5006 RTP/AVP 0 96\r\n\
a=sendrecv\r\n\
a=crypto:1 AES_CM_128_HMAC_SHA1_32 inline:ZVNobHlzUGU2X0ZidG5TNDYtOXhZYWlSaEx3SVhU|2^5\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos none remote sendrecv\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=ptime:20\r\n\
a=maxptime:400\r\n\
a=fmtp:96 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n");

_LIT8( KMceTestMeidaNoCryptoAVP, "\
m=audio 5006 RTP/AVP 0 96\r\n\
a=sendrecv\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos none remote sendrecv\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=ptime:20\r\n\
a=maxptime:400\r\n\
a=fmtp:96 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n");


_LIT8( KMceTestMeidaCryptoSAVPF, "\
m=audio 5006 RTP/SAVPF 0 96\r\n\
a=sendrecv\r\n\
a=crypto:1 AES_CM_128_HMAC_SHA1_32 inline:ZVNobHlzUGU2X0ZidG5TNDYtOXhZYWlSaEx3SVhU|2^5\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos none remote sendrecv\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=ptime:20\r\n\
a=maxptime:400\r\n\
a=fmtp:96 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n");

_LIT8( KMceTestMeidaCryptoWrongProtocol, "\
m=audio 5006 RTP/AVP 0 96\r\n\
a=sendrecv\r\n\
a=crypto:1 AES_CM_128_HMAC_SHA1_32 inline:ZVNobHlzUGU2X0ZidG5TNDYtOXhZYWlSaEx3SVhU\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos none remote sendrecv\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=ptime:20\r\n\
a=maxptime:400\r\n\
a=fmtp:96 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n");


_LIT8( KMceTestMeidaCryptoMultiple, "\
m=audio 5006 RTP/SAVPF 0 96\r\n\
a=sendrecv\r\n\
a=crypto:1 AES_CM_128_HMAC_SHA1_32 inline:ZVNobHlzUGU2X0ZidG5TNDYtOXhZYWlSaEx3SVhU\r\n\
a=crypto:2 AES_CM_128_HMAC_SHA1_32 inline:ZVNobHlzUGU2X0ZidG5TNDYtOXhZYWlSaEx3SVhU\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos none remote sendrecv\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=ptime:20\r\n\
a=maxptime:400\r\n\
a=fmtp:96 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n");

_LIT8( KMceTestMeidaCryptoMultiple2, "\
m=audio 5006 RTP/SAVPF 0 96\r\n\
a=sendrecv\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos none remote sendrecv\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=ptime:20\r\n\
a=maxptime:400\r\n\
a=fmtp:96 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n\
a=crypto:1 AES_CM_128_HMAC_SHA1_80 inline:ZVNobHlzUGU2X0ZidG5TNDYtOXhZYWlSaEx3SVhU\r\n\
a=crypto:2 AES_CM_128_HMAC_SHA1_32 inline:hmEnUUhipdXlKqsom7YbO/3Zg61x39DV4oUiXq1O\r\n\
a=crypto:3 F8_128_HMAC_SHA1_80 inline:vJeiM7l17DrIkbCH8QLR8y8ZymcGVWnn/J/Pugrs\r\n");

_LIT8( KMceTestMeidaNoCrypto, "\
m=audio 5006 RTP/SAVPF 0 96\r\n\
a=sendrecv\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos none remote sendrecv\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=ptime:20\r\n\
a=maxptime:400\r\n\
a=fmtp:96 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n");

// CONSTRUCTION
UT_CMceSecureDesStream* UT_CMceSecureDesStream::NewL()
    {
    UT_CMceSecureDesStream* self = UT_CMceSecureDesStream::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMceSecureDesStream* UT_CMceSecureDesStream::NewLC()
    {
    UT_CMceSecureDesStream* self = new( ELeave ) UT_CMceSecureDesStream();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceSecureDesStream::~UT_CMceSecureDesStream()
    {
    }

// Default constructor
UT_CMceSecureDesStream::UT_CMceSecureDesStream()
    {
    }

// Second phase construct
void UT_CMceSecureDesStream::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceSecureDesStream::SetupL(  )
    {
    //Set up the basic
    iSession = MceMediaManagerTestHelper::CreateMultimediaOutSessionL();
    RPointerArray<CMceComMediaStream>& streams = iSession->Streams();
    
    TInt count=streams.Count();
    if (count>0)
    	{
    	iStream1=streams[0];
    	}
    if (count>1)
    	{
    	iStream2=streams[1];
    	}
    
    iMcc = CMccControllerStub::Stub();
    
    iManager = CMceMediaManager::NewL( *this );
    
    RPointerArray<CMceComAudioCodec> audioCapabilities;
    RPointerArray<CMceComVideoCodec> videoCapabilities;
    MceCleanupResetAndDestroyPushL( audioCapabilities );
    MceCleanupResetAndDestroyPushL( videoCapabilities );
    iManager->GetCapabilitiesL( audioCapabilities );
    iManager->GetCapabilitiesL( videoCapabilities );
    CleanupStack::PopAndDestroy( 2 );//capas
    
    
    
    __ASSERT_ALWAYS( iManager->iSdpCodecs.Count() >= 2, User::Leave( KErrNotFound ) );
    iAudioSdpCodec = static_cast<CMceMediaSdpCodec*>( iManager->iSdpCodecs[ 0 ] );
    iVideoSdpCodec = static_cast<CMceMediaSdpCodec*>( iManager->iSdpCodecs[ 1 ] );

    iAudio = SdpCodecStringPool::StringPoolL().StringF( 
    											SdpCodecStringConstants::EMediaAudio, 
												SdpCodecStringPool::StringTableL());
												
    iVideo = SdpCodecStringPool::StringPoolL().StringF( 
    											SdpCodecStringConstants::EMediaVideo, 
												SdpCodecStringPool::StringTableL());
												
	iProtocol = SdpCodecStringPool::StringPoolL().StringF( 
												SdpCodecStringConstants::EProtocolRtpAvp, 
												SdpCodecStringPool::StringTableL() );
    
 // Create media line: m= This should correspond to the OFFER received!
    
	
	iMedia = CSdpMediaField::NewLC ( iVideo, 8000, iProtocol, _L8("96") );
	
	// Note the sendrcv, this should create bound stream as well since it's two way.
	RStringF attribute = SdpCodecStringPool::StringPoolL().StringF
    			( SdpCodecStringConstants::EAttributeSendrecv, SdpCodecStringPool::StringTableL() );
	CSdpAttributeField* mediaAttr = CSdpAttributeField::NewLC(attribute, _L8(""));
	iMedia->AttributeFields().AppendL( mediaAttr );
	attribute.Close();
    CleanupStack::Pop( mediaAttr );
    
    CleanupStack::Pop( iMedia );
    TUint payloadType = 96;
	// Get the encoding name
	TPtrC8 codecName = _L8("HMMH");	
	TUint clockRate = 90000;		

	// create rtpmap field for current codec
	CSdpFmtAttributeField* rtpmapAttribute = 
	    iVideoSdpCodec->EncodeRtpMapFieldsL( payloadType, 
											 codecName, 
										     clockRate,
										     KMceSdpOptionalEncodingParam );
	CleanupStack::PushL( rtpmapAttribute );
	iMedia->FormatAttributeFields().AppendL( rtpmapAttribute );
	CleanupStack::Pop( rtpmapAttribute );

	
	iOffer = CSdpDocument::NewL();
	CleanupStack::PushL( iOffer );
	iOffer->MediaFields().AppendL( iMedia );
   
    
     // Set session name: s=
    iOffer->SetSessionNameL( _L8("") );

    const TUint32 KInetAddr = INET_ADDR(10,10,10,10);
	TInetAddr localIpAddress;
	localIpAddress.SetAddress(KInetAddr);
	
    CSdpOriginField* originField = CSdpOriginField::NewL( _L8("username"),
                                                          55555,
                                                          55555,
                                                          localIpAddress );
    iOffer->SetOriginField( originField );
    
    // Create connection field: c=
    CSdpConnectionField* connectionField = CSdpConnectionField::NewL( localIpAddress );
    iOffer->SetConnectionField( connectionField );
    
    CleanupStack::Pop( iOffer );
    
    iInterface = CMccInterface::NewL( *iManager );
    
    iSecureInterface= CMccSecureInterface::NewL( *iInterface );
    
    iSecureSession = 
        CMceSecureMediaSession::NewL( *iSession, *iManager, *iSecureInterface );
    
    iSecureStream = 
        CMceSecureDesStream::NewL( *iSecureSession, *iMedia, 
                                   *iSecureInterface, *iStream1 );
	iSecureStream->AppendEmptyCryptoL( *iSecureStream->iCryptoOuts, KTotalCryptoAnswerCount );
    }

void UT_CMceSecureDesStream::Teardown(  )
    {
    delete iSecureStream;
    iAudio.Close();
    iVideo.Close();
    iProtocol.Close();
    delete iSecureSession;
    delete iSecureInterface;
    delete iInterface;
    delete iOffer;
	delete iManager;
	SdpCodecStringPool::Close();
	CMccControllerStub::Close();
	iSession->iClientCryptoSuites.Reset();
	delete iSession;	
    }

void UT_CMceSecureDesStream::UT_CMceSecureDesStream_NewLL(  )
    {  
    CMceSecureDesStream* iSecureStream= CMceSecureDesStream::NewL(*iSecureSession,
    															*iMedia,
    															*iSecureInterface,
    															*iStream1);
    
    delete iSecureStream;
    
    CMceSecureDesStream* secureInStream= CMceSecureDesStream::NewL(*iSecureSession,
    															*iMedia,
    															*iSecureInterface,
    															*iStream2);
    
    
    delete secureInStream;
    }

void UT_CMceSecureDesStream::UT_CMceSecureDesStream_EncodeSecureSdpLL(  )
    {    
	/*=============Test case 1 ====================================================*/
    //Test case 1 client crypto count = NULL
    CSdpMediaField* media = CSdpMediaField::NewLC ( iVideo, 8000, iProtocol, _L8("96") );	
	// Note the sendrcv, this should create bound stream as well since it's two way.
	RStringF attribute = SdpCodecStringPool::StringPoolL().StringF
    			( SdpCodecStringConstants::EAttributeSendrecv, SdpCodecStringPool::StringTableL() );
	CSdpAttributeField* mediaAttr = CSdpAttributeField::NewLC(attribute, _L8(""));
	
	media->AttributeFields().AppendL( mediaAttr );
    CleanupStack::Pop( mediaAttr );
    TUint payloadType = 96;
	// Get the encoding name
	TPtrC8 codecName = _L8("HMMH");	
	TUint clockRate = 90000;		
		
	// create rtpmap field for current codec
	CSdpFmtAttributeField* rtpmapAttribute = 
	    iVideoSdpCodec->EncodeRtpMapFieldsL( payloadType, 
											 codecName, 
										     clockRate,
										     KMceSdpOptionalEncodingParam );
	CleanupStack::PushL( rtpmapAttribute );
	media->FormatAttributeFields().AppendL( rtpmapAttribute );
	CleanupStack::Pop( rtpmapAttribute );
	
	iSecureSession->iCryptoContextUpdate = ETrue;
    iSecureStream->EncodeSecureSdpL(*media, EFalse);
    EUNIT_ASSERT( !iSecureStream->Session().iIsSecureSession);
    CleanupStack::PopAndDestroy(media);
    	
    /*==============Test case 2 ========================================================*/
    //Test case 2 if meadia type==audio
    iSession->iClientCryptoSuites.AppendL( EAES_CM_128_HMAC_SHA1_32 );
    iSession->iClientCryptoSuites.AppendL( EAES_CM_128_HMAC_SHA1_80 );
    media = CSdpMediaField::NewLC( iAudio, 8000, iProtocol, _L8("96") );	
	// Note the sendrcv, this should create bound stream as well since it's two way.
	mediaAttr = CSdpAttributeField::NewLC(attribute, _L8(""));
	media->AttributeFields().AppendL( mediaAttr );
    CleanupStack::Pop( mediaAttr );
    
	// create rtpmap field for current codec
	rtpmapAttribute = 
	    iAudioSdpCodec->EncodeRtpMapFieldsL( payloadType, 
											 codecName, 
										     clockRate,
										     KMceSdpOptionalEncodingParam );
	CleanupStack::PushL( rtpmapAttribute);
	media->FormatAttributeFields().AppendL( rtpmapAttribute);
	CleanupStack::Pop( rtpmapAttribute );
	iSecureStream->Session().iIsSecureSession = ETrue;
	iSecureSession->iCryptoContextUpdate = ETrue;
    iSecureStream->EncodeSecureSdpL(*media, EFalse);
   	EUNIT_ASSERT( iSecureStream->Session().iIsSecureSession );
   	
    /*==============Test case 3 ========================================================*/
    //Test case 3
    //state ==ERefresh or Eupdate
    iSecureSession->iCryptoContextUpdate = EFalse;
    iSecureSession->iKeyNeedUpdated = EFalse;
   	
   	iSecureStream->iCryptoOut = iSecureStream->iCryptoOuts->At( 0 );
   	TMceSecureCryptoInfo oldCryptoNegotiated = iSecureStream->iCryptoOut;
   	
   	//Encode the SecureSDP and Ensure that crypto is reused not created again
    iSecureStream->EncodeSecureSdpL( *media, EFalse );
    
    EUNIT_ASSERT( oldCryptoNegotiated.iEncodedKey == 
    							iSecureStream->iCryptoOuts->At( 0 ).iEncodedKey );    

	/*==============Test case 3a ========================================================*/
	// Test Case 3a 
	// Ensure that keys get change once keyUpdate is Required in offer/answer

	iSecureSession->iCryptoContextUpdate = ETrue;
    iSecureSession->iKeyNeedUpdated = ETrue;
   	   	
   	//Encode the SecureSDP and Ensure that crypto is recreated
    iSecureStream->EncodeSecureSdpL( *media, EFalse );
    
    EUNIT_ASSERT( oldCryptoNegotiated.iEncodedKey != 
    							iSecureStream->iCryptoOuts->At( 0 ).iEncodedKey );    

    							        
    /*=================================================================================*/   
    
    // Reset everything for the rest of the test cases
    iSession->iClientCryptoSuites.Reset();
    iSession->iClientCryptoSuites.AppendL( EAES_CM_128_HMAC_SHA1_32 );  
    CleanupStack::PopAndDestroy( media );
    media = CSdpMediaField::NewLC( iAudio, 8000, iProtocol, _L8("96") );
    delete iSecureStream;
    iSecureStream = NULL;
    iSecureStream = 
        CMceSecureDesStream::NewL( *iSecureSession, *iMedia, 
                                   *iSecureInterface, *iStream1 );
    iSecureStream->GenerateRandomKeys( iSecureStream->iCryptoIn );   
    iSecureStream->GenerateRandomKeys( iSecureStream->iCryptoOut );
   
    // Answer; 
    // iSecureSession.iCryptoContextUpdate == EFalse
    media->AttributeFields().ResetAndDestroy();
    iSecureSession->iKeyNeedUpdated = EFalse; 
    iSecureSession->iCryptoContextUpdate = EFalse;
    iSecureStream->EncodeSecureSdpL( *media, ETrue );
    EUNIT_ASSERT_EQUALS( 1, media->AttributeFields().Count() )  
    
    // Answer; 
    // iSecureSession.iCryptoContextUpdate == ETrue
    media->AttributeFields().ResetAndDestroy();
    iSecureSession->iKeyNeedUpdated = EFalse;
    iSecureSession->iCryptoContextUpdate = ETrue;   
    iSecureStream->EncodeSecureSdpL( *media, ETrue );
    EUNIT_ASSERT( !iSecureSession->iCryptoContextUpdate )
    EUNIT_ASSERT_EQUALS( 1, media->AttributeFields().Count() )

    // Offer; 
    // iSecureSession.iKeyNeedUpdated == ETrue
    // iSecureSession.iCryptoContextUpdate == ETrue
    media->AttributeFields().ResetAndDestroy();
    iSecureSession->iKeyNeedUpdated = ETrue;
    iSecureSession->iCryptoContextUpdate = ETrue;
    iSecureStream->EncodeSecureSdpL( *media, EFalse );
    EUNIT_ASSERT( !iSecureSession->iCryptoContextUpdate )
    EUNIT_ASSERT_EQUALS( 1, media->AttributeFields().Count() )
      
    // Offer;
    // iSecureSession.iKeyNeedUpdated == EFalse
    // iSecureSession.iCryptoContextUpdate == EFalse
    media->AttributeFields().ResetAndDestroy();
    iSecureSession->iKeyNeedUpdated = EFalse;
    iSecureSession->iCryptoContextUpdate = EFalse; 
    iSecureStream->EncodeSecureSdpL( *media, EFalse );
    EUNIT_ASSERT_EQUALS( 1, media->AttributeFields().Count() )      
  
    // Offer;
    // iSecureSession.iKeyNeedUpdated == ETrue
    // iSecureSession.iCryptoContextUpdate == EFalse
    media->AttributeFields().ResetAndDestroy();
    iSecureSession->iKeyNeedUpdated = ETrue;
    iSecureSession->iCryptoContextUpdate = EFalse; 
    iSecureStream->EncodeSecureSdpL( *media, EFalse );
    EUNIT_ASSERT_EQUALS( 1, media->AttributeFields().Count() )
        
    CleanupStack::PopAndDestroy( media );
    }

void UT_CMceSecureDesStream::UT_CMceSecureDesStream_DecodeSecureSdpLL(  )
    {															
	/*=============Test case 1 ====================================================*/
    //Test case 1 happy cases
    CSdpMediaField* media = CSdpMediaField::NewLC ( iVideo, 8000, iProtocol, _L8("96") );	
	// Note the sendrcv, this should create bound stream as well since it's two way.
	RStringF attribute = SdpCodecStringPool::StringPoolL().StringF
    			( SdpCodecStringConstants::EAttributeSendrecv, SdpCodecStringPool::StringTableL() );
	CSdpAttributeField* mediaAttr = CSdpAttributeField::NewLC(attribute, _L8(""));
	
	 media->AttributeFields().AppendL( mediaAttr );
    CleanupStack::Pop( mediaAttr );
    TUint payloadType = 96;
	// Get the encoding name
	TPtrC8 codecName = _L8("HMMH");	
	TUint clockRate = 90000;		
		
	// create rtpmap field for current codec
	CSdpFmtAttributeField* rtpmapAttribute = 
	    iVideoSdpCodec->EncodeRtpMapFieldsL( payloadType, 
											 codecName, 
										     clockRate,
										     KMceSdpOptionalEncodingParam );
	CleanupStack::PushL( rtpmapAttribute );
	media->FormatAttributeFields().AppendL( rtpmapAttribute );
	CleanupStack::Pop( rtpmapAttribute );
																
    iSecureStream->DecodeSecureSdpL( *media );
    EUNIT_ASSERT( iSecureStream->Session().iIsSecureSession );
    
    CleanupStack::PopAndDestroy(media);

    /*===========Test case 2=============================================*/	
    
    //Test case 2 if meadia no crypto
    
    media = CSdpMediaField::NewLC( iAudio, 8000, iProtocol, _L8("96") );	
	
	mediaAttr = CSdpAttributeField::NewLC(attribute, _L8(""));
	
	 media->AttributeFields().AppendL( mediaAttr );
    CleanupStack::Pop( mediaAttr );
    rtpmapAttribute = 
	    iAudioSdpCodec->EncodeRtpMapFieldsL( payloadType, 
											 codecName, 
										     clockRate,
										     KMceSdpOptionalEncodingParam );
	CleanupStack::PushL( rtpmapAttribute);
	media->FormatAttributeFields().AppendL( rtpmapAttribute);
	CleanupStack::Pop( rtpmapAttribute );
																
    iSecureStream->DecodeSecureSdpL( *media);
    EUNIT_ASSERT( iSecureStream->Session().iIsSecureSession );
    /*===========Test case 3 ==================================================*/
    //state ==ERefresh or Eupdate Real Decode
    if ( !iSession->iClientCryptoSuites.Count() )
    	{
        iSession->iClientCryptoSuites.AppendL(EAES_CM_128_HMAC_SHA1_32);
        iSession->iClientCryptoSuites.AppendL(EAES_CM_128_HMAC_SHA1_80);
    	}
    iSecureSession->iCryptoContextUpdate = ETrue;	
    iSecureStream->EncodeSecureSdpL(*media, EFalse );
    EUNIT_ASSERT( iSecureStream->Session().iIsSecureSession );
    iSecureStream->DecodeSecureSdpL( *media );
    EUNIT_ASSERT( iSecureStream->Session().iIsSecureSession );
    iSecureStream->RemvoeSecureSdp(*media);
    
    iSecureSession->iCryptoContextUpdate = ETrue;
    iSecureStream->EncodeSecureSdpL( *media, EFalse );
    EUNIT_ASSERT( iSecureStream->Session().iIsSecureSession );
    iSecureStream->DecodeSecureSdpL( *media );
    EUNIT_ASSERT( iSecureStream->Session().iIsSecureSession );
    /*==========================================================================*/ 
    CleanupStack::PopAndDestroy(media);
    
    /*===========Test case 4 ==================================================*/
    
    CSdpMediaField* mediaAnswer1 = CSdpMediaField::DecodeL( KMceTestMeidaCryptoSAVPF );
    CleanupStack::PushL( mediaAnswer1 );
    iSecureStream->DecodeSecureSdpL( *mediaAnswer1 );
    EUNIT_ASSERT( iSecureStream->Session().iIsSecureSession );
    CleanupStack::PopAndDestroy(mediaAnswer1);
    
    /*===========Test case 5 ==================================================*/
    
    CSdpMediaField* mediaAnswer2 = CSdpMediaField::DecodeL( KMceTestMeidaCryptoWrongProtocol );
    CleanupStack::PushL( mediaAnswer2 );
    iSecureStream->DecodeSecureSdpL(*mediaAnswer2 );
    EUNIT_ASSERT( iSecureStream->Session().iIsSecureSession );
    EUNIT_ASSERT( !iSecureStream->iGnoreSdpMsg )
    CleanupStack::PopAndDestroy(mediaAnswer2);
    
    /*===========Test case 6 ==================================================*/
    
    CSdpMediaField* mediaAnswer3 = CSdpMediaField::DecodeL( KMceTestMeidaCryptoMultiple2 );
    CleanupStack::PushL( mediaAnswer3 );
    iSecureStream->DecodeSecureSdpL(*mediaAnswer3 );
    EUNIT_ASSERT( iSecureStream->Session().iIsSecureSession );
    EUNIT_ASSERT( !iSecureStream->iGnoreSdpMsg )
    EUNIT_ASSERT( iSecureStream->iCryptoIns->Count() == 2 );
    CleanupStack::PopAndDestroy(mediaAnswer3);
    }

void UT_CMceSecureDesStream::UT_CMceSecureDesStream_DecodeSecureSdpAnswerLL(  )
    {
	/*===========Test case 1 ==================================================*/
    //state ==ERefresh or Eupdate Real Decode
    if ( !iSession->iClientCryptoSuites.Count() )
    	{
    	User::LeaveIfError ( iSession->iClientCryptoSuites.Append(EAES_CM_128_HMAC_SHA1_32));
    	User::LeaveIfError ( iSession->iClientCryptoSuites.Append(EAES_CM_128_HMAC_SHA1_80));
    	}
    CSdpMediaField* mediaOffer = CSdpMediaField::DecodeL( KMceTestMeidaNoCrypto );
    CleanupStack::PushL( mediaOffer );	
    iSecureStream->Session().iIsSecureSession = ETrue;	
    iSecureSession->iCryptoContextUpdate = ETrue;
    iSecureStream->EncodeSecureSdpL( *mediaOffer, EFalse );
	EUNIT_ASSERT( iSecureStream->Session().iIsSecureSession );
    CSdpMediaField* mediaAnswer1 = CSdpMediaField::DecodeL( KMceTestMeidaCryptoSAVPF );
    CleanupStack::PushL( mediaAnswer1 );
    iSecureStream->DecodeSecureSdpAnswerL( *mediaAnswer1 );
    EUNIT_ASSERT( iSecureStream->Session().iIsSecureSession );
    CleanupStack::PopAndDestroy(mediaAnswer1);
    CleanupStack::PopAndDestroy(mediaOffer);
    
    /*===========Test case 2 ==================================================*/
    
    mediaOffer = CSdpMediaField::DecodeL( KMceTestMeidaNoCrypto );
    CleanupStack::PushL( mediaOffer );	
    iSecureSession->iCryptoContextUpdate = ETrue;
    iSecureStream->EncodeSecureSdpL(*mediaOffer, EFalse);
	EUNIT_ASSERT( iSecureStream->Session().iIsSecureSession );
    CSdpMediaField* mediaAnswer2 = CSdpMediaField::DecodeL( KMceTestMeidaCryptoWrongProtocol );
    CleanupStack::PushL( mediaAnswer2 );
    //iCryptoOut crypto is not set
    iSecureStream->DecodeSecureSdpAnswerL(*mediaAnswer2);
    EUNIT_ASSERT( !iSecureStream->Session().iIsSecureSession );
    EUNIT_ASSERT( iSecureStream->iCryptoIn.iMKLifeTime == 32 );
    EUNIT_ASSERT( iSecureStream->iCryptoOut.iMKLifeTime == KDefalutMaterKeysLifeTime );
    CleanupStack::PopAndDestroy(mediaAnswer2);
    CleanupStack::PopAndDestroy(mediaOffer);
    
    /*===========Test case 3 ==================================================*/
    
    mediaOffer = CSdpMediaField::DecodeL( KMceTestMeidaNoCrypto );
    CleanupStack::PushL( mediaOffer );	
    iSecureSession->iCryptoContextUpdate = ETrue;
    iSecureStream->EncodeSecureSdpL(*mediaOffer, EFalse );
    EUNIT_ASSERT( !iSecureStream->Session().iIsSecureSession );
	
    CSdpMediaField* mediaAnswer3 = CSdpMediaField::DecodeL( KMceTestMeidaCryptoMultiple );
    CleanupStack::PushL( mediaAnswer3 );
    EUNIT_ASSERT_SPECIFIC_LEAVE	( iSecureStream->DecodeSecureSdpAnswerL(*mediaAnswer3), KErrArgument );
    CleanupStack::PopAndDestroy(mediaAnswer3);
    CleanupStack::PopAndDestroy(mediaOffer);
    }

void UT_CMceSecureDesStream::UT_CMceSecureDesStream_RemvoeSecureSdpLL(  )
    {
    /*=============Test case 1 ====================================================*/
    //Test case 1 check if media type!=audio
    CSdpMediaField* media = CSdpMediaField::NewLC ( iVideo, 8000, iProtocol, _L8("96") );	
	// Note the sendrcv, this should create bound stream as well since it's two way.
	RStringF attribute = SdpCodecStringPool::StringPoolL().StringF
    			( SdpCodecStringConstants::EAttributeSendrecv, SdpCodecStringPool::StringTableL() );
	CSdpAttributeField* mediaAttr = CSdpAttributeField::NewLC(attribute, _L8(""));
	
	 media->AttributeFields().AppendL( mediaAttr );
    CleanupStack::Pop( mediaAttr );
    TUint payloadType = 96;
	// Get the encoding name
	TPtrC8 codecName = _L8("HMMH");	
	TUint clockRate = 90000;		
		
	// create rtpmap field for current codec
	CSdpFmtAttributeField* rtpmapAttribute = 
	    iVideoSdpCodec->EncodeRtpMapFieldsL( payloadType, 
											 codecName, 
										     clockRate,
										     KMceSdpOptionalEncodingParam );
	CleanupStack::PushL( rtpmapAttribute );
	media->FormatAttributeFields().AppendL( rtpmapAttribute );
	CleanupStack::Pop( rtpmapAttribute );
																
    iSecureStream->RemvoeSecureSdp( *media );
    CleanupStack::Pop(media);
    delete media; 
    media=NULL; 
    
    /*===========Test case 2=============================================*/	
    
    //Test case 2 if meadia type==audio but no crypto
    
    media= CSdpMediaField::NewLC( iAudio, 8000, iProtocol, _L8("96") );	
	
	mediaAttr = CSdpAttributeField::NewLC(attribute, _L8(""));
	
	 media->AttributeFields().AppendL( mediaAttr );
    CleanupStack::Pop( mediaAttr );
    rtpmapAttribute = 
	    iAudioSdpCodec->EncodeRtpMapFieldsL( payloadType, 
											 codecName, 
										     clockRate,
										     KMceSdpOptionalEncodingParam );
	CleanupStack::PushL( rtpmapAttribute);
	media->FormatAttributeFields().AppendL( rtpmapAttribute);
	CleanupStack::Pop( rtpmapAttribute );
	
	iSecureStream->RemvoeSecureSdp(*media);
 	
 	/*===========Test case 3 ==================================================*/
    //Remove Sdp with crypto
   	if ( !iSession->iClientCryptoSuites.Count() )
    	{
    	User::LeaveIfError ( iSession->iClientCryptoSuites.Append(EAES_CM_128_HMAC_SHA1_32));
    	User::LeaveIfError ( iSession->iClientCryptoSuites.Append(EAES_CM_128_HMAC_SHA1_80));
    	}
   	iSecureStream->Session().iIsSecureSession = ETrue;
   	iSecureSession->iCryptoContextUpdate = ETrue;
   	iSecureStream->EncodeSecureSdpL( *media, EFalse );
    EUNIT_ASSERT( iSecureStream->Session().iIsSecureSession );
    iSecureStream->RemvoeSecureSdp( *media );														
    
    /*=======================================================================*/
    CleanupStack::PopAndDestroy(media);
    }

void UT_CMceSecureDesStream::UT_CMceSecureDesStream_SetCryptoContextL(  )
    {
    /*=============Test case 1 ====================================================*/
    //key has not yet created
    iSecureStream->SetCryptoContextL();
    EUNIT_ASSERT( iSecureStream->iGnoreSdpMsg );
    
     /*=============Test case 2 ====================================================*/
    //state is EFist but key is not created
    
    iSecureStream->SetCryptoContextL();											
    EUNIT_ASSERT( iSecureStream->iGnoreSdpMsg );
    
    /*=============Test case 3 ====================================================*/
    //state is EUpdate but key is not created	
    iSecureStream->iCryptoUpdateNeeded = ETrue;	
    iSecureStream->SetCryptoContextL();	
    EUNIT_ASSERT( iSecureStream->iGnoreSdpMsg );	
    										
    /*=============Test case 4 ====================================================*/
    //key created and the state is EFirst
    CSdpMediaField* media= CSdpMediaField::NewLC( iAudio, 8000, iProtocol, _L8("96") );	
	
	RStringF attribute = SdpCodecStringPool::StringPoolL().StringF
    			( SdpCodecStringConstants::EAttributeSendrecv, SdpCodecStringPool::StringTableL() );
	CSdpAttributeField* mediaAttr = CSdpAttributeField::NewLC(attribute, _L8(""));
	
	 media->AttributeFields().AppendL( mediaAttr );
    CleanupStack::Pop( mediaAttr );TUint payloadType = 96;
	// Get the encoding name
	TPtrC8 codecName = _L8("HMMH");	
	TUint clockRate = 90000;		
		
	// create rtpmap field for current codec
	CSdpFmtAttributeField* rtpmapAttribute = 
	    iAudioSdpCodec->EncodeRtpMapFieldsL( payloadType, 
											 codecName, 
										     clockRate,
										     KMceSdpOptionalEncodingParam );
	CleanupStack::PushL( rtpmapAttribute);
	media->FormatAttributeFields().AppendL( rtpmapAttribute);
	CleanupStack::Pop( rtpmapAttribute );
	if ( !iSession->iClientCryptoSuites.Count() )
    	{
    	User::LeaveIfError ( iSession->iClientCryptoSuites.Append(EAES_CM_128_HMAC_SHA1_32));
    	User::LeaveIfError ( iSession->iClientCryptoSuites.Append(EAES_CM_128_HMAC_SHA1_80));
    	}
	
	iSecureStream->Session().iIsSecureSession = ETrue;
	iSecureSession->iCryptoContextUpdate = ETrue;
	iSecureStream->EncodeSecureSdpL( *media, EFalse );
    EUNIT_ASSERT( iSecureStream->Session().iIsSecureSession );
    iSecureStream->SetCryptoContextL();
    EUNIT_ASSERT( iSecureStream->iGnoreSdpMsg );	
    
    /*=============Test case 5====================================================*/
    //change state to EUpdate
    iSecureStream->iCryptoUpdateNeeded  = ETrue;
    iSecureStream->SetCryptoContextL();
    EUNIT_ASSERT( iSecureStream->iGnoreSdpMsg );		
    /*==========================================================================*/ 
    CleanupStack::PopAndDestroy(media);														
    }

void UT_CMceSecureDesStream::UT_CMceSecureDesStream_RemoveCryptoContextL(  )
    {										
    /*=============Test case 1 ====================================================*/
    //key created and the state is EFirst
    CSdpMediaField* media= CSdpMediaField::NewLC( iAudio, 8000, iProtocol, _L8("96") );	
	
	RStringF attribute = SdpCodecStringPool::StringPoolL().StringF
    			( SdpCodecStringConstants::EAttributeSendrecv, SdpCodecStringPool::StringTableL() );
	CSdpAttributeField* mediaAttr = CSdpAttributeField::NewLC(attribute, _L8(""));
	
	 media->AttributeFields().AppendL( mediaAttr );
    CleanupStack::Pop( mediaAttr );TUint payloadType = 96;
	// Get the encoding name
	TPtrC8 codecName = _L8("HMMH");	
	TUint clockRate = 90000;		
		
	// create rtpmap field for current codec
	CSdpFmtAttributeField* rtpmapAttribute = 
	    iAudioSdpCodec->EncodeRtpMapFieldsL( payloadType, 
											 codecName, 
										     clockRate,
										     KMceSdpOptionalEncodingParam );
	CleanupStack::PushL( rtpmapAttribute);
	media->FormatAttributeFields().AppendL( rtpmapAttribute);
	CleanupStack::Pop( rtpmapAttribute );
	if ( !iSession->iClientCryptoSuites.Count() )
    	{
    	User::LeaveIfError ( iSession->iClientCryptoSuites.Append(EAES_CM_128_HMAC_SHA1_32));
    	User::LeaveIfError ( iSession->iClientCryptoSuites.Append(EAES_CM_128_HMAC_SHA1_80));
    	}
    iSecureStream->Session().iIsSecureSession = ETrue;	
    iSecureSession->iCryptoContextUpdate = ETrue;
	iSecureStream->EncodeSecureSdpL( *media, EFalse );
	EUNIT_ASSERT( iSecureStream->Session().iIsSecureSession );
    iSecureStream->SetCryptoContextL();
    EUNIT_ASSERT( iSecureStream->iGnoreSdpMsg );	
    /*==========================================================================*/ 
    CleanupStack::PopAndDestroy(media);
    }

void UT_CMceSecureDesStream::UT_CMceSecureDesStream_MediaStreamL(  )
    {    
    EUNIT_ASSERT( iSecureStream->MediaStream().Direction() == iStream1->Direction() )													
    }

void UT_CMceSecureDesStream::UT_CMceSecureDesStream_MediaFieldL(  )
    {
    EUNIT_ASSERT( iSecureStream->iMediaField != NULL )			
    }

void UT_CMceSecureDesStream::UT_CMceSecureDesStream_ValidateOfferByAnswerLL(  )  
	{
    /*=============Test case 1 ====================================================*/
    iSecureStream->ValidateOfferByAnswerL( KSecAttrValue1 );
    EUNIT_ASSERT( !iSecureStream->iGnoreSdpMsg );	
    EUNIT_ASSERT( iSecureStream->iCryptoIns->At(0).iMKI == KMKI1 );
    TUint64 mkiLiftTime = 1048576;//2^20
    EUNIT_ASSERT( iSecureStream->iCryptoIns->At(0).iMKLifeTime == mkiLiftTime );
    
    /*=============Test case 2 ====================================================*/
    iSecureStream->ValidateOfferByAnswerL(KSecAttrValue5 );
    EUNIT_ASSERT( !iSecureStream->iGnoreSdpMsg );
    EUNIT_ASSERT( iSecureStream->iCryptoIns->At(1).iMKI==KMKI2 );
    TReal result = 0;
    Math::Pow( result, 2, 10 );
    EUNIT_ASSERT( iSecureStream->iCryptoIns->At(1).iMKLifeTime == (TUint64 ) result);
    
    /*=============Test case 3 ====================================================*/
    TRAPD( error, iSecureStream->ValidateOfferByAnswerL(KSecAttrNotValidLength) );
	if ( error == KErrNoMemory )
        {
    	User::Leave( KErrNoMemory );
        }
	EUNIT_ASSERT( error == KErrArgument );
    
    //EUNIT_ASSERT_SPECIFIC_LEAVE( iSecureStream->ValidateOfferByAnswerL(KSecAttrNotValidLength), KErrArgument );
    
    /*=============Test case 4 ====================================================*/
    TRAP( error, iSecureStream->ValidateOfferByAnswerL(KSecAttrNoInline) );
	if ( error == KErrNoMemory )
        {
    	User::Leave( KErrNoMemory );
        }
	EUNIT_ASSERT( error == KErrArgument );
	//EUNIT_ASSERT_SPECIFIC_LEAVE( iSecureStream->ValidateOfferByAnswerL( KSecAttrNoInline ), KErrArgument );
    
    /*=============Test case 5 ====================================================*/
    TRAP( error, iSecureStream->ValidateOfferByAnswerL(KSecAttrNotValidKey) );
	if ( error == KErrNoMemory )
        {
    	User::Leave( KErrNoMemory );
        }
	EUNIT_ASSERT( error == KErrArgument );
    //EUNIT_ASSERT_SPECIFIC_LEAVE( iSecureStream->ValidateOfferByAnswerL( KSecAttrNotValidKey ), KErrArgument );
    
    /*=============Test case 6 ====================================================*/
    iSecureStream->ValidateOfferByAnswerL( KSecAttrCryptoNotSupport );
    EUNIT_ASSERT( iSecureStream->iGnoreSdpMsg );	
    /*=============Test case 7 ====================================================*/
    iSecureStream->iGnoreSdpMsg = EFalse;
    iSecureStream->ValidateOfferByAnswerL( KSecAttrOnlyKey );
    EUNIT_ASSERT( !iSecureStream->iGnoreSdpMsg );
	}
	
void UT_CMceSecureDesStream::UT_CMceSecureDesStream_ValidateAnswerByOfferLL(  )  
	{
    /*=============Test case 1 ====================================================*/
    //no iCryptoOuts found
   	iSecureStream->ValidateAnswerByOfferL( KSecAttrValue1 );
    EUNIT_ASSERT( iSecureStream->iGnoreSdpMsg );
    /*=============Test case 2 ====================================================*/
    iSecureStream->SetPreferedCryptoL();
    iSecureStream->iCryptoOuts->At(0).iMKIUsed=ETrue;
    iSecureStream->iCryptoOuts->At(1).iMKIUsed=ETrue;
    iSecureStream->ValidateAnswerByOfferL( KSecAttrValue1 );
    EUNIT_ASSERT( !iSecureStream->iGnoreSdpMsg );
    EUNIT_ASSERT(iSecureStream->iCryptoIn.iMKIUsed==ETrue);
    EUNIT_ASSERT(iSecureStream->iCryptoIn.iMKI==KMKI1);
    TUint64 mkiLiftTime = 1048576;//2^20
    EUNIT_ASSERT(iSecureStream->iCryptoIn.iMKLifeTime == mkiLiftTime);
    
    /*=============Test case 3 ====================================================*/
    iSecureStream->ValidateAnswerByOfferL( KSecAttrValue5 );
    EUNIT_ASSERT( !iSecureStream->iGnoreSdpMsg );
    EUNIT_ASSERT(iSecureStream->iCryptoIn.iMKI==KMKI2);
    TReal result = 0;
    Math::Pow(result, 2, 10);
    EUNIT_ASSERT( iSecureStream->iCryptoIn.iMKLifeTime  == (TUint64 ) result);
    
        /*=============Test case 3 ====================================================*/
    EUNIT_ASSERT_SPECIFIC_LEAVE( iSecureStream->ValidateAnswerByOfferL( KSecAttrNotValidLength ), KErrArgument );
    
    /*=============Test case 4 ====================================================*/
    EUNIT_ASSERT_SPECIFIC_LEAVE( iSecureStream->ValidateAnswerByOfferL( KSecAttrNoInline ), KErrArgument );
    
    /*=============Test case 5 ====================================================*/
    EUNIT_ASSERT_SPECIFIC_LEAVE( iSecureStream->ValidateAnswerByOfferL( KSecAttrNotValidKey ), KErrArgument );
    
    /*=============Test case 6 ====================================================*/
    iSecureStream->ValidateAnswerByOfferL( KSecAttrCryptoNotSupport );
    EUNIT_ASSERT( iSecureStream->iGnoreSdpMsg );
    /*=============Test case 7 ====================================================*/
    iSecureStream->iGnoreSdpMsg = EFalse;
    iSecureStream->ValidateAnswerByOfferL( KSecAttrOnlyKey );
    EUNIT_ASSERT( !iSecureStream->iGnoreSdpMsg );
    /*==========================================================================*/ 					
	}	

void UT_CMceSecureDesStream::UT_CMceSecureDesStream_FormMKILL(  )  
	{
    iSecureStream->SetPreferedCryptoL();
    iSecureStream->iCryptoOuts->At(0).iMKIUsed=ETrue;
    iSecureStream->iCryptoOuts->At(1).iMKIUsed=ETrue;
    //iSecureStream->iCryptoOuts[0].iMKIValue = 1;
	//iSecureStream->iCryptoOuts[0].iMKILength = 4;
	//iSecureStream->iCryptoOuts[1].iMKIValue = 1;
	//iSecureStream->iCryptoOuts[1].iMKILength = 4;
    iSecureStream->ValidateAnswerByOfferL( KSecAttrValue1 );
    EUNIT_ASSERT( !iSecureStream->iGnoreSdpMsg );
    EUNIT_ASSERT( iSecureStream->iCryptoIn.iMKIUsed==ETrue);
    EUNIT_ASSERT( iSecureStream->iCryptoIn.iMKI==KMKI1);
    TUint64 mkiLiftTime = 1048576;//2^20
    EUNIT_ASSERT( iSecureStream->iCryptoIn.iMKLifeTime == mkiLiftTime);
    iSecureStream->CreateCryptoContextL( iSecureStream->iCryptoIn);				
	}	

void UT_CMceSecureDesStream::UT_CMceSecureDesStream_GenerateCryptoSuiteLineL()
	{
	TMceSecureCryptoInfo crypto;
	TBuf8< KCryptoLineMaxLength > cryptoLine;
    /*=============Test case 1 ====================================================*/
    
	crypto.iEncAlgms = ESrtpNullAlg;
	iSecureStream->GenerateCryptoSuiteLineL( cryptoLine, crypto );
	EUNIT_ASSERT( iSecureStream->iGnoreSdpMsg );
    cryptoLine.Zero();
    iSecureStream->iGnoreSdpMsg = EFalse;
    /*=============Test case 2 ====================================================*/
    crypto.iTagLen = KAuthTagLength80;
    crypto.iEncAlgms = ESrtpEncAES_CM;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iSecureStream->GenerateCryptoSuiteLineL( cryptoLine, crypto ), KErrArgument);
    cryptoLine.Zero();
    /*=============Test case 3 ====================================================*/
    crypto.iTagLen = KAuthTagLength80;
    crypto.iEncodedKey = KEncodedKey;
    crypto.iMKIUsed = ETrue;
    crypto.iMKI = KMKI1 ;
    cryptoLine.Copy( KNullDesC8);
    iSecureStream->GenerateCryptoSuiteLineL( cryptoLine, crypto );
    EUNIT_ASSERT ( cryptoLine.CompareF( KCryptoLine2 )==0 );
    EUNIT_ASSERT( !iSecureStream->iGnoreSdpMsg );
    cryptoLine.Zero();
    crypto.CryptoDestruct();
	}

void UT_CMceSecureDesStream::UT_CMceSecureDesStream_CopyStreamCryptoL()
    {
    CMceSecureDesStream* copy = CMceSecureDesStream::NewL(*iSecureSession,
                                                iSecureStream->MediaField(),
                                                *iSecureInterface,
                                                *iStream1);
	CleanupStack::PushL( copy);
	copy->CopyStreamCryptoL(*iSecureStream);
	CleanupStack::Pop( copy );
	EUNIT_ASSERT( copy->iCryptoOuts->Count()==KTotalCryptoAnswerCount );
	
	delete copy;
	}

void UT_CMceSecureDesStream::UT_CMceSecureDesStream_GenerateCryptoLineL()
	{
	TInt cryptoCount = 1;
	TBuf8< KCryptoLineMaxLength > cryptoLine;

    /*=============Test case 1 ====================================================*/
    

	iSecureStream->GenerateCryptoLineL( cryptoLine, cryptoCount, EFalse );
    cryptoLine.Zero();
    
    /*=============Test case 2 ====================================================*/
    
	EUNIT_ASSERT_SPECIFIC_LEAVE ( iSecureStream->GenerateCryptoLineL( cryptoLine, cryptoCount, ETrue ), KErrArgument );
    cryptoLine.Zero();
    
     /*=============Test case 3====================================================*/
    
    iSecureStream->iCryptoOuts->At( 0 ).iTagLen = KAuthTagLength32;
	iSecureStream->iCryptoOuts->At( 0 ).iEncAlgms = ESrtpEncAES_CM;
	iSecureStream->iCryptoOuts->At( 0 ).iAuthAlgms = ESrtpAuthHMAC_SHA1;
	iSecureStream->iCryptoOuts->At( 0 ).iCryptoSuite = KAES_SHA1_32;
	TMceSecureCryptoInfo cryptoIn;
	iSecureStream->iCryptoIns->AppendL( cryptoIn );
    iSecureStream->iCryptoIns->At( 0 ).Copy( iSecureStream->iCryptoOuts->At( 0 ) );
    iSecureStream->GenerateCryptoLineL( cryptoLine, cryptoCount, ETrue );
    
    
	cryptoLine.Zero();		
    /*=============Test case 4 ====================================================*/
    iSecureStream->GenerateCryptoLineL( cryptoLine, cryptoCount, EFalse );
    cryptoLine.Zero();
    
    /*=============Test case for iKeyNeedUpdated is true===========================*/
    iSecureStream->iCryptoOuts->At( 0 ).iTagLen = KAuthTagLength32;
    iSecureStream->iCryptoOuts->At( 0 ).iEncAlgms = ESrtpEncAES_CM;
    iSecureStream->iCryptoOuts->At( 0 ).iAuthAlgms = ESrtpAuthHMAC_SHA1;
    iSecureStream->iCryptoOuts->At( 0 ).iCryptoSuite = KAES_SHA1_32;
    TMceSecureCryptoInfo cryptoIn1;
    iSecureStream->iCryptoIns->AppendL(cryptoIn1);
    iSecureStream->iCryptoIns->At( 0 ).Copy(iSecureStream->iCryptoOuts->At( 0) );
    iSecureSession->iKeyNeedUpdated = ETrue;
    iSecureStream->GenerateCryptoLineL(cryptoLine, cryptoCount, ETrue);
    cryptoLine.Zero();

    /*=============Test case for iKeyNeedUpdated is false===========================*/
    iSecureSession->iKeyNeedUpdated = EFalse;
    iSecureStream->GenerateCryptoLineL(cryptoLine, cryptoCount, ETrue);
    cryptoLine.Zero();
    
    /*=============Test case 5====================================================*/
    
    cryptoLine.Copy( KNullDesC8);
    iSecureStream->GenerateCryptoLineL( cryptoLine, cryptoCount, EFalse  );
    EUNIT_ASSERT ( cryptoLine.Length() == KCryptoLine3().Length() );
    cryptoLine.Zero();
	}
	
void UT_CMceSecureDesStream::UT_CMceSecureDesStream_SetMediaProfileL()
	{
	/*=============Test case 1 ====================================================*/
    CSdpMediaField* mediaAnswercrytoAVP = CSdpMediaField::DecodeL( KMceTestMeidaCryptoAVP );
    CleanupStack::PushL( mediaAnswercrytoAVP );
    iSecureStream->SetMediaProfile( *mediaAnswercrytoAVP );
    EUNIT_ASSERT( KMceSecurePlainAVP == iSecureStream->Session().Modifier( KMceSecureSession ) );
    CleanupStack::PopAndDestroy(mediaAnswercrytoAVP);
    
    /*=============Test case 2 ====================================================*/
    CSdpMediaField* mediaAnswerNoCryptoAVP = CSdpMediaField::DecodeL( KMceTestMeidaNoCryptoAVP );
    CleanupStack::PushL( mediaAnswerNoCryptoAVP );
    iSecureStream->SetMediaProfile( *mediaAnswerNoCryptoAVP );
    EUNIT_ASSERT( KMceSecurePlainAVP == iSecureStream->Session().Modifier( KMceSecureSession ) );
    CleanupStack::PopAndDestroy(mediaAnswerNoCryptoAVP);
    
    /*=============Test case 3 ====================================================*/
    CSdpMediaField* mediaAnswerCryptoSAVP = CSdpMediaField::DecodeL( KMceTestMeidaCryptoSAVPF );
    CleanupStack::PushL( mediaAnswerCryptoSAVP );
    iSecureStream->SetMediaProfile( *mediaAnswerCryptoSAVP );
    EUNIT_ASSERT( KMceSecureNormal == iSecureStream->Session().Modifier( KMceSecureSession ) );
    CleanupStack::PopAndDestroy(mediaAnswerCryptoSAVP);
	}
	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMceSecureDesStream,
    "CMceSecureDesStream",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMceSecureDesStream",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSecureDesStream_NewLL, Teardown)

EUNIT_TEST(
    "EncodeSecureSdpL - test ",
    "CMceSecureDesStream",
    "EncodeSecureSdpL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSecureDesStream_EncodeSecureSdpLL, Teardown)

EUNIT_TEST(
    "DecodeSecureSdpL - test ",
    "CMceSecureDesStream",
    "DecodeSecureSdpL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSecureDesStream_DecodeSecureSdpLL, Teardown)

EUNIT_TEST(
    "DecodeSecureSdpAnswerL - test ",
    "CMceSecureDesStream",
    "DecodeSecureSdpAnswerL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSecureDesStream_DecodeSecureSdpAnswerLL, Teardown)
    
    
EUNIT_TEST(
    "RemvoeSecureSdpL - test ",
    "CMceSecureDesStream",
    "RemvoeSecureSdpL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSecureDesStream_RemvoeSecureSdpLL, Teardown)

EUNIT_TEST(
    "SetCryptoContext - test ",
    "CMceSecureDesStream",
    "SetCryptoContext",
    "FUNCTIONALITY",
    SetupL, UT_CMceSecureDesStream_SetCryptoContextL, Teardown)

EUNIT_TEST(
    "RemoveCryptoContext - test ",
    "CMceSecureDesStream",
    "RemoveCryptoContext",
    "FUNCTIONALITY",
    SetupL, UT_CMceSecureDesStream_RemoveCryptoContextL, Teardown)


EUNIT_TEST(
    "MediaStream - test ",
    "CMceSecureDesStream",
    "MediaStream",
    "FUNCTIONALITY",
    SetupL, UT_CMceSecureDesStream_MediaStreamL, Teardown)

EUNIT_TEST(
    "MediaField - test ",
    "CMceSecureDesStream",
    "MediaField",
    "FUNCTIONALITY",
    SetupL, UT_CMceSecureDesStream_MediaFieldL, Teardown)

EUNIT_TEST(
    "ValidateOfferByAnswerL - test ",
    "CMceSecureDesStream",
    "ValidateOfferByAnswer",
    "FUNCTIONALITY",
    SetupL, UT_CMceSecureDesStream_ValidateOfferByAnswerLL, Teardown)
    
EUNIT_TEST(
    "ValidateAnswerByOfferL - test ",
    "CMceSecureDesStream",
    "ValidateAnswerByOffer",
    "FUNCTIONALITY",
    SetupL, UT_CMceSecureDesStream_ValidateAnswerByOfferLL, Teardown)
    
EUNIT_TEST(
    "FormMKILL - test ",
    "CMceSecureDesStream",
    "FormMKILL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSecureDesStream_FormMKILL, Teardown) 
    
EUNIT_TEST(
    "GenerateCryptoSuiteLine - test ",
    "CMceSecureDesStream",
    "GenerateCryptoSuiteLine",
    "FUNCTIONALITY",
    SetupL, UT_CMceSecureDesStream_GenerateCryptoSuiteLineL, Teardown)     

EUNIT_TEST(
    "GenerateCryptoLineL - test ",
    "CMceSecureDesStream",
    "CryptoLineL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSecureDesStream_GenerateCryptoLineL, Teardown)     

EUNIT_TEST(
    "SetMediaProfileL - test ",
    "CMceSecureDesStream",
    "SetMediaProfileL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSecureDesStream_SetMediaProfileL, Teardown)
	
EUNIT_TEST(
    "CopyStreamCryptoL - test ",
    "CMceSecureDesStream",
    "CopyStreamCryptoL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSecureDesStream_CopyStreamCryptoL, Teardown)     

EUNIT_END_TEST_TABLE

//  END OF FILE

