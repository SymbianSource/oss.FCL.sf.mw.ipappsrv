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
#include "UT_CMceSecureMediaSession.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mcesecuremediasession.h"
#include "MceMediaManagerTestHelper.h"
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

#include "mcemediamanager.h"
#include "mcemediasdpcodec.h"
#include "mmccinterface_stub.h"
#include "cleanupresetanddestroy.h"
#include "mcesecuredesstream.h"
#include "mcevideosdpcodec.h"

#include "mcemediadefs.h"
#include "mcecomaudiocodec.h"
_LIT8( KSdpName, "audio" );


#define SRVSTREAM( n ) iSession->MccStreams()[ n ]

_LIT8( KMceTestMeidaCrypto1, "\
m=audio 5006 RTP/SAVP 0 96\r\n\
a=sendrecv\r\n\
a=crypto:1 AES_CM_128_HMAC_SHA1_32 inline:ZVNobHlzUGU2X0ZidG5TNDYtOXhZYWlSaEx3SVhU|2^20\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos none remote sendrecv\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=ptime:20\r\n\
a=maxptime:400\r\n\
a=fmtp:96 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n");


_LIT8( KMceTestMeidaCrypto2, "\
m=video 5008 RTP/SAVP 0 96\r\n\
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

_LIT8( KMceTestMeidaCrypto3, "\
m=audio 9000 RTP/SAVP 97\r\n\
a=sendrecv\r\n\
a=crypto:1 AES_CM_128_HMAC_SHA1_32 inline:ZVNobHlzUGU2X0ZidG5TNDYtOXhZYWlSaEx3SVhU\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos none remote sendrecv\r\n\
a=rtpmap:97 AMR/9000\r\n\
a=ptime:20\r\n\
a=maxptime:400\r\n\
a=fmtp:96 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n");

_LIT8( KMceTestMeidaCryptoNOK, "\
m=audio 5006 RTP/SAVP 0 96\r\n\
a=sendrecv\r\n\
a=crypto:1 AES_CM_128_HMAC_SHA1_32 inline:ZVNobHlzUGU2X0ZidG5TNDYtOXhZYWlSaEx\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos none remote sendrecv\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=ptime:20\r\n\
a=maxptime:400\r\n\
a=fmtp:96 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n");

// CONSTRUCTION
UT_CMceSecureMediaSession* UT_CMceSecureMediaSession::NewL()
    {
    UT_CMceSecureMediaSession* self = UT_CMceSecureMediaSession::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMceSecureMediaSession* UT_CMceSecureMediaSession::NewLC()
    {
    UT_CMceSecureMediaSession* self = new( ELeave ) UT_CMceSecureMediaSession();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceSecureMediaSession::~UT_CMceSecureMediaSession()
    {
    }

// Default constructor
UT_CMceSecureMediaSession::UT_CMceSecureMediaSession()
    {
    }

// Second phase construct
void UT_CMceSecureMediaSession::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceSecureMediaSession::SetupL(  )
    {
	iSession = MceMediaManagerTestHelper::CreateMultimediaOutSessionL();
	
    
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
    
	
	iMedia = CSdpMediaField::NewLC ( iVideo, 5008, iProtocol, _L8("96") );
	
	// Note the sendrcv, this should create bound stream as well since it's two way.
	RStringF attribute = SdpCodecStringPool::StringPoolL().StringF
    			( SdpCodecStringConstants::EAttributeSendrecv, SdpCodecStringPool::StringTableL() );
	CSdpAttributeField* mediaAttr = CSdpAttributeField::NewLC(attribute, _L8(""));
	iMedia->AttributeFields().AppendL( mediaAttr );
	attribute.Close();
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
    
    User::LeaveIfError (iOffer->IsValid());
     CleanupStack::Pop( iOffer );
     CleanupStack::Pop( iMedia );
    iInterface = CMccInterface::NewL( *iManager );
    iSecureInterface= CMccSecureInterface::NewL(*iInterface);
	
	iSecureSession= CMceSecureMediaSession::NewL( *iSession,
                                     			*iManager,
                                     *iSecureInterface );
                               
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
    }

void UT_CMceSecureMediaSession::Teardown(  )
    {
	iAudio.Close();
    iVideo.Close();
    iProtocol.Close();
    delete iSecureSession;
    delete iSecureInterface;
    delete iInterface;
    delete iOffer;
    
    if (iManager)
    	delete iManager;
	SdpCodecStringPool::Close();
	CMccControllerStub::Close();
	iMcc = NULL;
	if (iSession)
		{
		for (TInt i=(iSession->iClientCryptoSuites.Count()-1);i>=0; i--)
			{
			iSession->iClientCryptoSuites.Reset();	
			}
		
		}
	
	delete iSession;
	
    }

void UT_CMceSecureMediaSession::UT_CMceSecureMediaSession_NewLL(  )
    {
    CMceSecureMediaSession* secureSession= CMceSecureMediaSession::NewL( *iSession,
                                     			*iManager,
                                     *iSecureInterface );
    CleanupStack::PushL(secureSession);
    CleanupStack::PopAndDestroy(secureSession);                                 
    }

void UT_CMceSecureMediaSession::UT_CMceSecureMediaSession_NewLCL(  )
    {
    CMceSecureMediaSession* secureSession= CMceSecureMediaSession::NewLC( *iSession,
                                     			*iManager,
                                     *iSecureInterface );
    
    CleanupStack::PopAndDestroy(secureSession);  
    }


void UT_CMceSecureMediaSession::UT_CMceSecureMediaSession_EncodeSecureDesSdpOfferLL(  )
    {
    TBuf8<KMceMaxSdpNameLength> sdpName;
    sdpName.Append(KSdpName);
    CMceComCodec* codec= CMceComAudioCodec::NewL( sdpName );
    CleanupStack::PushL(codec);
    
    /*==========Test Case  1===================================================*/
    //No Audio line
    iSecureSession->iCryptoContextUpdate = ETrue;
    iSecureSession->EncodeSecureDesSdpOfferL( *iStream1, *iMedia );
   
    /*==========Test Case  2===================================================*/
    //Audio Line state==EFirst
    CSdpMediaField* media = CSdpMediaField::NewLC ( iAudio, 5006, iProtocol, _L8("96") );	
	// Note the sendrcv, this should create bound stream as well since it's two way.
	RStringF attribute = SdpCodecStringPool::StringPoolL().StringF
    			( SdpCodecStringConstants::EAttributeSendrecv, SdpCodecStringPool::StringTableL() );
	CSdpAttributeField* mediaAttr = CSdpAttributeField::NewLC(attribute, _L8(""));
//	CleanupStack::Pop( mediaAttr );
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
    iSecureSession->EncodeSecureDesSdpOfferL( *iStream1, *media );
    
    EUNIT_ASSERT(iSecureSession->iMceSecureDesStreams.Count()!=0);
    
    /*==========Test Case  3===================================================*/
    // found state==ERefresh
    iSecureSession->iCryptoContextUpdate = ETrue;
    iSecureSession->EncodeSecureDesSdpOfferL(*iStream1, *media);	
    
    /*==========Test Case  4===================================================*/
    // state==EUpdate
    CMceSecureDesStream* secstream= iSecureSession->iMceSecureDesStreams[0];
    secstream->iCryptoUpdateNeeded = ETrue;
    iSecureSession->iCryptoContextUpdate = ETrue;
    iSecureSession->EncodeSecureDesSdpOfferL( *iStream1, *media );	
    EUNIT_ASSERT( !secstream->iGnoreSdpMsg );
    CleanupStack::Pop(media);
    delete media; 
    media=NULL; 
    /*==========Test Case  3===================================================*/
    // Not found state==ERefresh create new secure stream
    media= CSdpMediaField::NewLC( iAudio, 9000, iProtocol, _L8("97") );	
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
	/**/
	iSecureSession->iCryptoContextUpdate = ETrue;
  	iSecureSession->EncodeSecureDesSdpOfferL( *iStream1, *media );
	    
    CleanupStack::Pop( media );
    delete media; 
    media = NULL; 
    /*=========================================================================*/
    CleanupStack::PopAndDestroy( codec );
    
    
    }

void UT_CMceSecureMediaSession::UT_CMceSecureMediaSession_DecodeSecureDesSdpAnswerLL(  )
    {
    TBuf8<KMceMaxSdpNameLength> sdpName;
    sdpName.Append(KSdpName);
    CMceComCodec* codec= CMceComAudioCodec::NewL( sdpName );
    CleanupStack::PushL(codec);
    /*==========Test Case  1===================================================*/
    //No Audio line the error was KErrNotFound but becase of we ignore it
    iSecureSession->iCryptoContextUpdate = ETrue;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iSecureSession->DecodeSecureDesSdpAnswerL( *iStream1, *iMedia ), KErrArgument );
    
    /*==========Test Case  2===================================================*/
    //Audio Line state==EAnswer
    CSdpMediaField* media = CSdpMediaField::NewLC ( iAudio, 5006, iProtocol, _L8("96") );	
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
	if ( !iSession->iClientCryptoSuites.Count() )
    	{
    	User::LeaveIfError ( iSession->iClientCryptoSuites.Append(EAES_CM_128_HMAC_SHA1_32));
    	}							
    iSecureSession->iCryptoContextUpdate = ETrue;									
    iSecureSession->EncodeSecureDesSdpOfferL( *iStream1, *media );
    //set up medialine
    CSdpMediaField* mediaAnswer = CSdpMediaField::DecodeL( KMceTestMeidaCrypto1 );
    CleanupStack::PushL( mediaAnswer );
    iSecureSession->DecodeSecureDesSdpAnswerL( *iStream1, *mediaAnswer );
   	CleanupStack::PopAndDestroy(mediaAnswer);
    
    /*==========Test Case 2 ===================================================*/
    //Invalid crypto attribute in answer
    //set up medialine
    
    CSdpMediaField* mediaAnswer2 = CSdpMediaField::DecodeL( KMceTestMeidaCryptoNOK );
    CleanupStack::PushL( mediaAnswer2 );
    TRAPD( err, iSecureSession->DecodeSecureDesSdpAnswerL( *iStream1, *mediaAnswer2 ) );
    EUNIT_ASSERT( KErrArgument == err );
   	CleanupStack::PopAndDestroy(mediaAnswer2);
   	//For test purposes, because of the leave, we must update the crypto suite as
   	//the old stream got removed
    if ( !iSession->iClientCryptoSuites.Count() )
        {
        User::LeaveIfError ( iSession->iClientCryptoSuites.Append(EAES_CM_128_HMAC_SHA1_32));
        }
    iSecureSession->iCryptoContextUpdate = ETrue;    
    iSecureSession->EncodeSecureDesSdpOfferL( *iStream1, *media );
    
    /*==========Test Case 3 ===================================================*/
    //state = EUpdate
    
    //Note that we will never have Update state todecode the answer even if SdpMode
    //is Update, Media Manager should update the Cryptocontext flage if answere contains
    //the wrong crypto key, this will enable sending of Exiting keys pair
    
    iSecureSession->iCryptoContextUpdate = ETrue;
    iSecureSession->EncodeSecureDesSdpOfferL(*iStream2, *iMedia );
    CMceSecureDesStream* secstream= iSecureSession->iMceSecureDesStreams[1];
    secstream->iCryptoUpdateNeeded = ETrue;
    
    //set up medialine
    CSdpMediaField* mediaAnswer3 = CSdpMediaField::DecodeL( KMceTestMeidaCrypto2 );
    CleanupStack::PushL( mediaAnswer3 );
    iSecureSession->DecodeSecureDesSdpAnswerL(*iStream2, *mediaAnswer3 );
    CleanupStack::PopAndDestroy(mediaAnswer3);
    
    EUNIT_ASSERT(iSecureSession->iMceSecureDesStreams.Count()==2);
    
    EUNIT_ASSERT( secstream->iCryptoUpdateNeeded );
   	/*=========================================================================*/
    CleanupStack::Pop( media );
    delete media; 
    media = NULL; 
    
    CleanupStack::PopAndDestroy( codec );
    }

void UT_CMceSecureMediaSession::UT_CMceSecureMediaSession_DecodeSecureDesSdpOfferLL(  )
    {
    /*==========Test Case  1===================================================*/
    
    iSecureSession->DecodeSecureDesSdpOfferL( *iStream1, *iMedia );
    
    /*==========Test Case  2===================================================*/
    //Audio Line state==EOffer
    CSdpMediaField* media = CSdpMediaField::NewLC ( iAudio, 5006, iProtocol, _L8("96") );	
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
    iSecureSession->EncodeSecureDesSdpOfferL( *iStream1, *media );
    iSecureSession->DecodeSecureDesSdpOfferL( *iStream1, *media );
    
    EUNIT_ASSERT( iSecureSession->iMceSecureDesStreams.Count()==1  );
    CMceSecureDesStream* secstream= iSecureSession->iMceSecureDesStreams[0];
    EUNIT_ASSERT( secstream->iCryptoUpdateNeeded == EFalse );
    
    /*==========Test Case  3===================================================*/
    //Invalid crypto attribute in offer
    CSdpMediaField* mediaOffer = CSdpMediaField::DecodeL( KMceTestMeidaCryptoNOK );
    CleanupStack::PushL( mediaOffer );
    TRAPD( err, iSecureSession->DecodeSecureDesSdpOfferL( *iStream1, *mediaOffer ) );
    EUNIT_ASSERT( KErrArgument == err );
   	CleanupStack::PopAndDestroy(mediaOffer);
   	//For test purposes, because of the leave, we must update the crypto suite as
   	//the old stream got removed
    if ( !iSession->iClientCryptoSuites.Count() )
        {
        User::LeaveIfError ( iSession->iClientCryptoSuites.Append(EAES_CM_128_HMAC_SHA1_32));
        }
    iSecureSession->iCryptoContextUpdate = ETrue;    
    iSecureSession->EncodeSecureDesSdpOfferL( *iStream1, *media );

    /*==========Test Case  4===================================================*/
    //state =ERefresh
    iSecureSession->DecodeSecureDesSdpOfferL(*iStream1, *media );
    EUNIT_ASSERT(iSecureSession->iMceSecureDesStreams.Count()== 1);
    
    /*==========Test Case  5===================================================*/
    //state = EUpdate
    secstream= iSecureSession->iMceSecureDesStreams[0];
    secstream->iCryptoUpdateNeeded = EFalse;
    iSecureSession->DecodeSecureDesSdpOfferL( *iStream1, *media );
    EUNIT_ASSERT( iSecureSession->iMceSecureDesStreams.Count()==1 );
    //After update the crypto then the mode becomes refresh mode
    
   	/*=========================================================================*/
    CleanupStack::Pop( media );
    delete media; 
    media = NULL; 
    
   
    }

void UT_CMceSecureMediaSession::UT_CMceSecureMediaSession_EncodeSecureDesSdpAnswerLL(  )
    {
    TBuf8<KMceMaxSdpNameLength> sdpName;
    sdpName.Append(KSdpName);
    CMceComCodec* codec= CMceComAudioCodec::NewL( sdpName );
    CleanupStack::PushL(codec);
    /*==========Test Case  1===================================================*/
    if ( !iSession->iClientCryptoSuites.Count() )
    	{
    	User::LeaveIfError ( iSession->iClientCryptoSuites.Append(EAES_CM_128_HMAC_SHA1_32));
    	}	
//    EUNIT_ASSERT_SPECIFIC_LEAVE( iSecureSession->EncodeSecureDesSdpAnswerL( *iStream1, *iMedia ), KErrArgument );
    TRAPD( error, iSecureSession->EncodeSecureDesSdpAnswerL( *iStream1, *iMedia )  );
	if ( error == KErrNoMemory )
    	{
    	User::Leave( KErrNoMemory );
    	}
	EUNIT_ASSERT( error == KErrArgument );
    
    
    /*==========Test Case  2===================================================*/

    CSdpMediaField* media = CSdpMediaField::NewLC ( iAudio, 5006, iProtocol, _L8("96") );	
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
	
	//setup cryptoOut
	CSdpMediaField* mediaOffer = CSdpMediaField::DecodeL( KMceTestMeidaCrypto1 );
    CleanupStack::PushL( mediaOffer );
    iSecureSession->DecodeSecureDesSdpOfferL( *iStream1, *mediaOffer );
   	iSecureSession->EncodeSecureDesSdpAnswerL( *iStream1, *media );
    CleanupStack::PopAndDestroy(mediaOffer);
   	
    
    EUNIT_ASSERT(iSecureSession->iMceSecureDesStreams.Count()!=0);
    /*==========Test Case  3===================================================*/
    // found state==ERefresh
    iSecureSession->EncodeSecureDesSdpAnswerL(*iStream1, *media);	
    
    /*==========Test Case  4===================================================*/
    // state==EUpdate
    CMceSecureDesStream* secstream= iSecureSession->iMceSecureDesStreams[0];
    secstream->iCryptoUpdateNeeded = EFalse;
    iSecureSession->EncodeSecureDesSdpAnswerL(*iStream1, *media);	
    
    CleanupStack::Pop( media );
    delete media; 
    media = NULL; 

    /*==========Test Case  5===================================================*/
    // Not found state==ERefresh create new secure stream
    media= CSdpMediaField::NewLC( iAudio, 9000, iProtocol, _L8("97") );	
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
	
	//setup cryptoOut
	CSdpMediaField* mediaOffer2 = CSdpMediaField::DecodeL( KMceTestMeidaCrypto3 );
    CleanupStack::PushL( mediaOffer2 );
	iSecureSession->DecodeSecureDesSdpOfferL( *iStream1, *mediaOffer2 );
   	iStream1->SetLocalMediaPort(9000);
   	iSecureSession->EncodeSecureDesSdpAnswerL( *iStream1, *media );
    CleanupStack::PopAndDestroy(mediaOffer2);
   	
    EUNIT_ASSERT(iSecureSession->iMceSecureDesStreams.Count()==3);
    CleanupStack::Pop( media );
    delete media; 
    media = NULL; 
    /*=========================================================================*/
    CleanupStack::PopAndDestroy( codec );
    }

void UT_CMceSecureMediaSession::UT_CMceSecureMediaSession_DecodeSecureDesSdpUpdateLL(  )
    {
    TBuf8<KMceMaxSdpNameLength> sdpName;
    sdpName.Append(KSdpName);
    CMceComCodec* codec= CMceComAudioCodec::NewL( sdpName );
    CleanupStack::PushL(codec);
    /*==========Test Case  1===================================================*/
    //No Audio line
    TInt direction= 0;
    iSecureSession->iCryptoContextUpdate = ETrue;
    iSecureSession->DecodeSecureDesSdpUpdateL( *iStream1, *iMedia );
    
    /*==========Test Case  2===================================================*/
    //Audio Line state==EOffer
    CSdpMediaField* media = CSdpMediaField::NewLC ( iAudio, 5006, iProtocol, _L8("96") );	
	// Note the sendrcv, this should create bound stream as well since it's two way.
	RStringF attribute = SdpCodecStringPool::StringPoolL().StringF
    			( SdpCodecStringConstants::EAttributeSendrecv, SdpCodecStringPool::StringTableL() );
	CSdpAttributeField* mediaAttr = CSdpAttributeField::NewLC(attribute, _L8(""));
	
	 media->AttributeFields().AppendL( mediaAttr );
    CleanupStack::Pop( mediaAttr );
    direction = SdpCodecStringConstants::EAttributeSendrecv;
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
    iSecureSession->EncodeSecureDesSdpOfferL( *iStream1, *media );
    iSecureSession->DecodeSecureDesSdpUpdateL( *iStream1, *media );
    
    EUNIT_ASSERT(iSecureSession->iMceSecureDesStreams.Count()==1);
    CMceSecureDesStream* secstream= iSecureSession->iMceSecureDesStreams[0];
    
    /*==========Test Case  3===================================================*/
    
    direction = SdpCodecStringConstants::EAttributeSendonly;
    iSecureSession->DecodeSecureDesSdpUpdateL( *iStream1, *media );
    EUNIT_ASSERT(iSecureSession->iMceSecureDesStreams.Count()==1);
    
    /*==========Test Case  4===================================================*/
    
    direction = SdpCodecStringConstants::EAttributeRecvonly;
    secstream= iSecureSession->iMceSecureDesStreams[0];
    secstream->iCryptoUpdateNeeded = EFalse;
    iSecureSession->DecodeSecureDesSdpUpdateL( *iStream1, *media );
    EUNIT_ASSERT(iSecureSession->iMceSecureDesStreams.Count()==1);
    //After update the crypto then the mode becomes refresh mode
    EUNIT_ASSERT( secstream->iCryptoUpdateNeeded == EFalse );
   	/*=========================================================================*/
    CleanupStack::Pop( media );
    delete media; 
    media = NULL; 
    
    CleanupStack::PopAndDestroy( codec );
    }



void UT_CMceSecureMediaSession::UT_CMceSecureMediaSession_CleanSecureCryptoInfoLL(  )
    {
    TBuf8<KMceMaxSdpNameLength> sdpName;
    sdpName.Append(KSdpName);
    CMceComCodec* codec= CMceComAudioCodec::NewL( sdpName );
    CleanupStack::PushL(codec);
    /*==========Test Case  1===================================================*/
    //No Audio line
    iSecureSession->iCryptoContextUpdate = ETrue;
    iSecureSession->EncodeSecureDesSdpOfferL( *iStream1, *iMedia );
    iSecureSession->CleanSecureCryptoInfoL( *iMedia );
    /*==========Test Case  2===================================================*/
    //Audio Line state==EFirst
    CSdpMediaField* media = CSdpMediaField::NewLC ( iAudio, 5006, iProtocol, _L8("96") );	
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
    iSecureSession->EncodeSecureDesSdpOfferL( *iStream1, *media );
    
    iSecureSession->CleanSecureCryptoInfoL( *media );
    
    EUNIT_ASSERT(iSecureSession->iMceSecureDesStreams.Count()==2);
   
   /*=========================================================================*/
    CleanupStack::Pop( media );
    delete media; 
    media = NULL; 
    
    CleanupStack::PopAndDestroy(codec);
    }

void UT_CMceSecureMediaSession::UT_CMceSecureMediaSession_BindCryptoL(  )
    {
    TMceReturnStatus status = iManager->UpdateL( *iSession );
    TBuf8<KMceMaxSdpNameLength> sdpName;
    sdpName.Append(KSdpName);
    CMceComCodec* codec= CMceComAudioCodec::NewL( sdpName );
    CleanupStack::PushL(codec);
    /*==========Test Case  1===================================================*/
    //No Audio line
    //CMceSrvStream* srvStream=SRVSTREAM(0);
    iSecureSession->iCryptoContextUpdate = ETrue;
    iSecureSession->EncodeSecureDesSdpOfferL( *iStream1, *iMedia );
    if ( !iSession->iClientCryptoSuites.Count() )
    	{
    	User::LeaveIfError ( iSession->iClientCryptoSuites.Append(EAES_CM_128_HMAC_SHA1_32));
    	}	
    EUNIT_ASSERT(iSecureSession->BindStreamCrypto()==KErrNone);
    /*==========Test Case  2===================================================*/
    //Audio Line state==EFirst
    CSdpMediaField* media = CSdpMediaField::NewLC ( iAudio, 5006, iProtocol, _L8("96") );	
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
    iSecureSession->EncodeSecureDesSdpOfferL( *iStream1, *media );
    
    EUNIT_ASSERT(iSecureSession->BindStreamCrypto() == KErrNone);
   
    
    EUNIT_ASSERT(iSecureSession->iMceSecureDesStreams.Count() == 2);
    /*==========Test Case  3===================================================*/
    iSecureSession->iCryptoContextUpdate = ETrue;
    iSecureSession->EncodeSecureDesSdpOfferL( *iStream1, *media );	
    //decodeanswer
   
	CSdpMediaField* mediaAnswer = CSdpMediaField::DecodeL( KMceTestMeidaCrypto1 );
    CleanupStack::PushL( mediaAnswer );
    iSecureSession->DecodeSecureDesSdpAnswerL( *iStream1, *mediaAnswer );
    CleanupStack::PopAndDestroy(mediaAnswer);
   	
   //end of decode answer
    
    EUNIT_ASSERT(iSecureSession->BindStreamCrypto()==KErrNone);
   
    EUNIT_ASSERT(iSecureSession->iMceSecureDesStreams.Count()==2);
   /*=========================================================================*/
    CleanupStack::Pop( media );
    delete media; 
    media=NULL; 
    
    CleanupStack::PopAndDestroy( codec );
    
	iSession->iClientCryptoSuites.Reset();	
	CMceComSession::Delete( iSession, *iManager );
    }

void UT_CMceSecureMediaSession::UT_CMceSecureMediaSession_ContextNeedUpdatedL(  )
    {
    TMceReturnStatus status = iManager->UpdateL( *iSession );
    TBuf8<KMceMaxSdpNameLength> sdpName;
    sdpName.Append(KSdpName);
    CMceComCodec* codec= CMceComAudioCodec::NewL( sdpName );
    CleanupStack::PushL(codec);
    /*==========Test Case  1===================================================*/
    //No Audio line
    //CMceSrvStream* srvStream=SRVSTREAM(0);
    iSecureSession->iCryptoContextUpdate = ETrue;
    iSecureSession->EncodeSecureDesSdpOfferL( *iStream1, *iMedia );
    iSecureSession->ContextNeedUpdated(0);
    /*==========Test Case  2===================================================*/
    //Audio Line state==EFirst
    CSdpMediaField* media = CSdpMediaField::NewLC ( iAudio, 5006, iProtocol, _L8("96") );	
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
    iSecureSession->EncodeSecureDesSdpOfferL( *iStream1, *media );
    
    
    EUNIT_ASSERT( iSecureSession->BindStreamCrypto()==KErrNone );
   
    iSecureSession->ContextNeedUpdated(0);
    EUNIT_ASSERT(iSecureSession->iMceSecureDesStreams.Count()==2);
    /*==========Test Case  3===================================================*/
    iSecureSession->iCryptoContextUpdate = ETrue;
    iSecureSession->EncodeSecureDesSdpOfferL( *iStream1, *media );	
    iSecureSession->DecodeSecureDesSdpOfferL( *iStream1, *media );
   
    EUNIT_ASSERT(iSecureSession->BindStreamCrypto()==KErrNone);
   
    iSecureSession->ContextNeedUpdated(0);
    EUNIT_ASSERT(iSecureSession->iMceSecureDesStreams.Count()==2);
   /*=========================================================================*/
    CleanupStack::Pop( media );
    delete media; 
    media=NULL; 
    
    CleanupStack::PopAndDestroy( codec );
   
	iSession->iClientCryptoSuites.Reset();	
 	CMceComSession::Delete( iSession, *iManager );
    }


void UT_CMceSecureMediaSession::UT_CMceSecureMediaSession_CopyStreamsLL(  )
    {
    CMceSecureMediaSession* copyToSecSesssion = CMceSecureMediaSession::NewLC( *iSession,
                                     			*iManager,
                                     *iSecureInterface );
    EUNIT_ASSERT(copyToSecSesssion->iMceSecureDesStreams.Count()==0);
                                    
     /*==========Create Streams ===================================================*/
    
    
    TBuf8<KMceMaxSdpNameLength> sdpName;
    sdpName.Append(KSdpName);
    CMceComCodec* codec= CMceComAudioCodec::NewL( sdpName );
    CleanupStack::PushL(codec);
    
   	CSdpMediaField* media = CSdpMediaField::NewLC ( iAudio, 5006, iProtocol, _L8("96") );	

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
	// worked			
	iSecureSession->iCryptoContextUpdate = ETrue;												
    iSecureSession->EncodeSecureDesSdpOfferL( *iStream1, *media );
    
    EUNIT_ASSERT( iSecureSession->iMceSecureDesStreams.Count() != 0);
    
    CleanupStack::Pop( media );
    delete media; 
    media = NULL; 
    CleanupStack::PopAndDestroy( codec );
	
	/*==========Call Copy Streams ===================================================*/
    copyToSecSesssion->CopyStreamsL(*iSecureSession);

	EUNIT_ASSERT( copyToSecSesssion->iMceSecureDesStreams.Count()== 
					iSecureSession->iMceSecureDesStreams.Count() );
   
    CleanupStack::PopAndDestroy( copyToSecSesssion );  
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMceSecureMediaSession,
    "CMceSecureMediaSession",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMceSecureMediaSession",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSecureMediaSession_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CMceSecureMediaSession",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, UT_CMceSecureMediaSession_NewLCL, Teardown)


EUNIT_TEST(
    "EncodeSecureDesSdpOfferL - test ",
    "CMceSecureMediaSession",
    "EncodeSecureDesSdpOfferL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSecureMediaSession_EncodeSecureDesSdpOfferLL, Teardown)

EUNIT_TEST(
    "DecodeSecureDesSdpAnswerL - test ",
    "CMceSecureMediaSession",
    "DecodeSecureDesSdpAnswerL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSecureMediaSession_DecodeSecureDesSdpAnswerLL, Teardown)

EUNIT_TEST(
    "DecodeSecureDesSdpOfferL - test ",
    "CMceSecureMediaSession",
    "DecodeSecureDesSdpOfferL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSecureMediaSession_DecodeSecureDesSdpOfferLL, Teardown)

EUNIT_TEST(
    "EncodeSecureDesSdpAnswerL - test ",
    "CMceSecureMediaSession",
    "EncodeSecureDesSdpAnswerL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSecureMediaSession_EncodeSecureDesSdpAnswerLL, Teardown)

EUNIT_TEST(
    "DecodeSecureDesSdpUpdateL - test ",
    "CMceSecureMediaSession",
    "DecodeSecureDesSdpUpdateL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSecureMediaSession_DecodeSecureDesSdpUpdateLL, Teardown)

EUNIT_TEST(
    "CleanSecureCryptoInfo - test ",
    "CMceSecureMediaSession",
    "CleanSecureCryptoInfoL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSecureMediaSession_CleanSecureCryptoInfoLL, Teardown)

EUNIT_TEST(
    "BindCrypto - test ",
    "CMceSecureMediaSession",
    "BindCrypto",
    "FUNCTIONALITY",
    SetupL, UT_CMceSecureMediaSession_BindCryptoL, Teardown)

EUNIT_TEST(
    "ContextNeedUpdated - test ",
    "CMceSecureMediaSession",
    "ContextNeedUpdated",
    "FUNCTIONALITY",
    SetupL, UT_CMceSecureMediaSession_ContextNeedUpdatedL, Teardown)

EUNIT_TEST(
    "CopyStreamsL - test ",
    "CMceSecureMediaSession",
    "CopyStreamsL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSecureMediaSession_CopyStreamsLL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
