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
#include "UT_cmcesdpcodec.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include "mcemediamanager.h"

#define RESET()\
    iServer->Reset()

#include <sdpcodecstringpool.h>
#include <sdpcodecstringconstants.h>
#include <sdpmediafield.h>
#include <sdpfmtattributefield.h>
#include <sdprtpmapvalue.h>
#include <sdpattributefield.h>
#include "mceaudiosdpcodec.h"
#include "mcevideosdpcodec.h"
#include "cleanupresetanddestroy.h"
#include "mmcccodecinformation.h"


//  INTERNAL INCLUDES

// CONSTRUCTION
UT_CMceSdpCodec* UT_CMceSdpCodec::NewL()
    {
    UT_CMceSdpCodec* self = UT_CMceSdpCodec::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceSdpCodec* UT_CMceSdpCodec::NewLC()
    {
    UT_CMceSdpCodec* self = new( ELeave ) UT_CMceSdpCodec();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceSdpCodec::~UT_CMceSdpCodec()
    {
    }

// Default constructor
UT_CMceSdpCodec::UT_CMceSdpCodec()
    {
    }

// Second phase construct
void UT_CMceSdpCodec::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_CMceSdpCodec::SetupL()
    {
    iServer = new (ELeave) CMceServerStub();
    iManager = CMceMediaManager::NewL( *iServer );
    iSession = MceMediaManagerTestHelper::CreateMultimediaOutSessionL();
    iSdpCodec = CMceAudioSdpCodec::NewL();
    
    } 

void UT_CMceSdpCodec::Teardown()
    {
	CMceComSession::Delete( iSession, *iManager );
	delete iManager;
	iManager = NULL;
	delete iServer;
	iServer = NULL;
	delete iSdpCodec;
	iSdpCodec = NULL;
    }


void UT_CMceSdpCodec::UT_CMceSdpCodec_MediaL()
    {
    RStringF audio = MCE_SDP_STRING_AUDIOL();
    EUNIT_ASSERT( iSdpCodec->iMedia == audio );
    
    RStringF media = iSdpCodec->Media();
    
    EUNIT_ASSERT( media == audio );
    
    }

void UT_CMceSdpCodec::UT_CMceSdpCodec_DecodesL()
    {
    RStringF video = MCE_SDP_STRING_VIDEOL();
    RStringF audio = MCE_SDP_STRING_AUDIOL();
    RStringF protocol = SDP_STRINGL( SdpCodecStringConstants::EProtocolRtpAvp );

    CSdpMediaField* audioLine = 
        CSdpMediaField::NewLC( audio, 5005, protocol, _L8("0") );

    CSdpMediaField* videoLine = 
        CSdpMediaField::NewLC( video, 5005, protocol, _L8("0") );
        
    EUNIT_ASSERT( iSdpCodec->Decodes( *audioLine ) );
    EUNIT_ASSERT( !iSdpCodec->Decodes( *videoLine ) );

    CleanupStack::PopAndDestroy( videoLine );
    CleanupStack::PopAndDestroy( audioLine );
    
    }

void UT_CMceSdpCodec::UT_CMceSdpCodec_AddCapabilityLL()
    {

    EUNIT_ASSERT( iSdpCodec->iCapabilities->Count() == 0 );
    
    iSdpCodec->AddCapabilityL( KMceSDPNameAMR );
    EUNIT_ASSERT( iSdpCodec->iCapabilities->Count() == 1 );
    iSdpCodec->AddCapabilityL( KMceSDPNamePCMU );
    EUNIT_ASSERT( iSdpCodec->iCapabilities->Count() == 2 );
    
    }

void UT_CMceSdpCodec::UT_CMceSdpCodec_IsSupportedL()
    {
    EUNIT_ASSERT( iSdpCodec->iCapabilities->Count() == 0 );
    EUNIT_ASSERT( !iSdpCodec->IsSupported( KMceSDPNameAMR ) );
    
    iSdpCodec->AddCapabilityL( KMceSDPNameAMR );
    EUNIT_ASSERT( iSdpCodec->iCapabilities->Count() == 1 );
    iSdpCodec->AddCapabilityL( KMceSDPNamePCMU );
    EUNIT_ASSERT( iSdpCodec->iCapabilities->Count() == 2 );

    EUNIT_ASSERT( iSdpCodec->IsSupported( KMceSDPNameAMR ) );
    EUNIT_ASSERT( iSdpCodec->IsSupported( KMceSDPNamePCMU ) );
    EUNIT_ASSERT( !iSdpCodec->IsSupported( KMceSDPNamePCMA ) );
    
    }

void UT_CMceSdpCodec::UT_CMceSdpCodec_PrepareForEncodeLL()
    {
    RStringF audio = MCE_SDP_STRING_AUDIOL();
    RStringF protocol = SDP_STRINGL( SdpCodecStringConstants::EProtocolRtpAvp );
    
    CSdpMediaField* audioLine = 
        CSdpMediaField::NewLC( audio, 5005, protocol, _L8("0") );

    RStringF rtpmapStr = SDP_STRINGL( SdpCodecStringConstants::EAttributeRtpmap );
    MCE_DEFINE_DECSTR( payload, 96 );
    
    TSdpRtpmapValue rtpmapValue = TSdpRtpmapValue::DecodeL( _L8("AMR/8000") );
    HBufC8* rtpmapBuf = rtpmapValue.EncodeL();
    CleanupStack::PushL( rtpmapBuf );
    CSdpFmtAttributeField* rtpmapAttribute = 
        CSdpFmtAttributeField::NewL( rtpmapStr, payload, *rtpmapBuf );
    CleanupStack::PopAndDestroy( rtpmapBuf ); 
    CleanupStack::PushL( rtpmapAttribute ); 

    audioLine->FormatAttributeFields().AppendL( rtpmapAttribute );
    CleanupStack::Pop( rtpmapAttribute ); 

    EUNIT_ASSERT( audioLine->FormatAttributeFields().Count() == 1 );
        
    iSdpCodec->PrepareForEncodeL( *iSession->Streams()[0], *audioLine );

    EUNIT_ASSERT( audioLine->FormatAttributeFields().Count() == 0 );
    
    RStringF empty = SdpCodecStringPool::StringPoolL().OpenFStringL( KNullDesC8 );
    CleanupClosePushL( empty );

    audioLine->SetMediaL( empty );
    EUNIT_ASSERT_LEAVE( iSdpCodec->PrepareForEncodeL( *iSession->Streams()[0], *audioLine  ) );
    
    CleanupStack::PopAndDestroy(); //empty
    CleanupStack::PopAndDestroy( audioLine );
    }
    

void UT_CMceSdpCodec::UT_CMceSdpCodec_EncodeRtpMapFieldsLL()
    {
    CSdpFmtAttributeField* rtpmap = NULL;
                                                    
    rtpmap = iSdpCodec->EncodeRtpMapFieldsL( 96, _L8("AMR"), 8000, KNullDesC8 );
    EUNIT_ASSERT( rtpmap );
    EUNIT_ASSERT( rtpmap->Value() == _L8("AMR/8000") );
    
    CleanupStack::PushL( rtpmap );

    CleanupStack::PopAndDestroy( rtpmap );
    
    }

void UT_CMceSdpCodec::UT_CMceSdpCodec_DirectionL()
    {
    
    _LIT8( KAttrValue, "120");
    
    RStringF audio = MCE_SDP_STRING_AUDIOL();
    RStringF protocol = SDP_STRINGL( SdpCodecStringConstants::EProtocolRtpAvp );
    CSdpAttributeField* direction = NULL;
    RPointerArray<CSdpAttributeField> sessionAttributes;
    MceCleanupResetAndDestroyPushL( sessionAttributes );

    CSdpMediaField* audioLine = 
        CSdpMediaField::NewLC( audio, 5005, protocol, _L8("0") );

    EUNIT_ASSERT( iSdpCodec->Direction( *audioLine, sessionAttributes ) == KErrNotFound );
    
    for( TInt i = 0; i < 10; i++ )
        {
        
        if ( i == 0 || i == 5 )
            {
            direction = CSdpAttributeField::NewLC( SDP_STRINGL( SdpCodecStringConstants::EAttributeSendonly ), KNullDesC8 );
            }
        else if ( i == 1 || i == 6 )
            {
            direction = CSdpAttributeField::NewLC( SDP_STRINGL( SdpCodecStringConstants::EAttributeRecvonly ), KNullDesC8 );
            }
        else if ( i == 2 || i == 7 )
            {
            direction = CSdpAttributeField::NewLC( SDP_STRINGL( SdpCodecStringConstants::EAttributeInactive ), KNullDesC8 );
            }
        else if ( i == 3 || i == 8 )
            {
            direction = CSdpAttributeField::NewLC( SDP_STRINGL( SdpCodecStringConstants::EAttributeSendrecv ), KNullDesC8 );
            }
        else if ( i == 4 || i == 9 )
            {
            //wrong
            direction = CSdpAttributeField::NewLC( SDP_STRINGL( SdpCodecStringConstants::EAttributePtime ), KAttrValue );
            }
            
        if ( i < 5 )
            {
            audioLine->AttributeFields().AppendL( direction );
            CleanupStack::Pop( direction );
            }
        else
            {
            sessionAttributes.AppendL( direction );
            CleanupStack::Pop( direction );
            }
        
        if ( i == 0 || i == 5 )
            {
            EUNIT_ASSERT( iSdpCodec->Direction( *audioLine, sessionAttributes ) == SdpCodecStringConstants::EAttributeSendonly );
            }
        else if ( i == 1 || i == 6 )
            {
            EUNIT_ASSERT( iSdpCodec->Direction( *audioLine, sessionAttributes ) == SdpCodecStringConstants::EAttributeRecvonly );
            }
        else if ( i == 2 || i == 7 )
            {
            EUNIT_ASSERT( iSdpCodec->Direction( *audioLine, sessionAttributes ) == SdpCodecStringConstants::EAttributeInactive );
            }
        else if ( i == 3 || i == 8 )
            {
            EUNIT_ASSERT( iSdpCodec->Direction( *audioLine, sessionAttributes ) == SdpCodecStringConstants::EAttributeSendrecv );
            }
        else if ( i == 4 || i == 9 )
            {
            EUNIT_ASSERT( iSdpCodec->Direction( *audioLine, sessionAttributes ) == KErrNotFound );
            }
        
        audioLine->AttributeFields().ResetAndDestroy();
        sessionAttributes.ResetAndDestroy();
        
        }
        
    CleanupStack::PopAndDestroy( audioLine );
    CleanupStack::PopAndDestroy();//sessionAttributes
    
    }

void UT_CMceSdpCodec::UT_CMceSdpCodec_ConvertDesToUintLL()
    {
    EUNIT_ASSERT( CMceSdpCodec::ConvertDesToUintL( _L8("10") ) == 10 );
    EUNIT_ASSERT_LEAVE( CMceSdpCodec::ConvertDesToUintL( _L8("Yada, Yada") ) );
    }

void UT_CMceSdpCodec::UT_CMceSdpCodec_ConvertDesToIntLL()
    {
    EUNIT_ASSERT( CMceSdpCodec::ConvertDesToIntL( _L8("10") ) == 10 );
    EUNIT_ASSERT_LEAVE( CMceSdpCodec::ConvertDesToIntL( _L8("Yada, Yada") ) );
    }

void UT_CMceSdpCodec::UT_CMceSdpCodec_ConvertTRealToDesLCL()
    {
    TReal real = 10.2304;
    HBufC8* realDes = NULL;
    realDes = CMceSdpCodec::ConvertTRealToDesLC( real );
    EUNIT_ASSERT( *realDes == _L8("10.2304") );
    CleanupStack::PopAndDestroy( realDes );
    
    }

void UT_CMceSdpCodec::UT_CMceSdpCodec_ConvertDesToRealLL()
    {
    EUNIT_ASSERT( CMceSdpCodec::ConvertDesToRealL( _L8("10.2304") ) == 10.2304 );
    EUNIT_ASSERT_LEAVE( CMceSdpCodec::ConvertDesToRealL( _L8("Yada, Yada") ) );
    }

void UT_CMceSdpCodec::UT_CMceSdpCodec_IsPayloadTypeSupportedL()
    {
	EUNIT_ASSERT(iSdpCodec->IsPayloadTypeSupported(KMceSDPNameAMR, KDefaultAmrNbPT)==EFalse);
    iSdpCodec->AddCapabilityL(KMceSDPNameAMR);
    iSdpCodec->AddCapabilityPayloadTypeL(KDefaultAmrNbPT);
    EUNIT_ASSERT(iSdpCodec->IsPayloadTypeSupported(KMceSDPNameAMR, KDefaultAmrNbPT)==ETrue);
    }

void UT_CMceSdpCodec::UT_CMceSdpCodec_AddCapabilityPayloadTypeLL()
    {
    iSdpCodec->AddCapabilityPayloadTypeL(KDefaultAmrNbPT);
    EUNIT_ASSERT(iSdpCodec->iCapabilitiesPT.Count() > 0 );
    }
	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceSdpCodec,
    "CMceSdpCodec tests",
    "UNIT" )

EUNIT_TEST (
    "Media test",
    "CMceSdpCodec",
    "Media",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpCodec_MediaL, Teardown)

EUNIT_TEST (
    "Decodes test",
    "CMceSdpCodec",
    "Decodes",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpCodec_DecodesL, Teardown)

EUNIT_TEST (
    "AddCapabilityL test",
    "CMceSdpCodec",
    "AddCapabilityL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpCodec_AddCapabilityLL, Teardown)

EUNIT_TEST (
    "IsSupported test",
    "CMceSdpCodec",
    "IsSupported",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpCodec_IsSupportedL, Teardown)

EUNIT_TEST (
    "PrepareForEncodeL test",
    "CMceSdpCodec",
    "PrepareForEncodeL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpCodec_PrepareForEncodeLL, Teardown)

EUNIT_TEST (
    "EncodeRtpMapFieldsL test",
    "CMceSdpCodec",
    "EncodeRtpMapFieldsL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpCodec_EncodeRtpMapFieldsLL, Teardown)

EUNIT_TEST (
    "Direction test",
    "CMceSdpCodec",
    "Direction",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpCodec_DirectionL, Teardown)

EUNIT_TEST (
    "ConvertDesToUintL test",
    "CMceSdpCodec",
    "ConvertDesToUintL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpCodec_ConvertDesToUintLL, Teardown)

EUNIT_TEST (
    "ConvertDesToIntL test",
    "CMceSdpCodec",
    "ConvertDesToIntL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpCodec_ConvertDesToIntLL, Teardown)

EUNIT_TEST (
    "ConvertTRealToDesLC test",
    "CMceSdpCodec",
    "ConvertTRealToDesLC",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpCodec_ConvertTRealToDesLCL, Teardown)

EUNIT_TEST (
    "ConvertDesToRealL test",
    "CMceSdpCodec",
    "ConvertDesToRealL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpCodec_ConvertDesToRealLL, Teardown)
    
EUNIT_TEST (
    "IsPayloadTypeSupported test",
    "CMceSdpCodec",
    "IsPayloadTypeSupported",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpCodec_IsPayloadTypeSupportedL, Teardown)
    
EUNIT_TEST (
    "AddCapabilityPayloadType test",
    "CMceSdpCodec",
    "AddCapabilityPayloadType",
    "FUNCTIONALITY",
    SetupL, UT_CMceSdpCodec_AddCapabilityPayloadTypeLL, Teardown)

EUNIT_END_TEST_TABLE



//  END OF FILE
