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
#include "UT_tmcepreconditions.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sdpcodecstringpool.h>
#include <sdpdocument.h>
#include <sdpmediafield.h>
#include <sdpattributefield.h>
#include "mcemediamanager.h"
#include "mcesdpsession.h"
#include "mcesrvsource.h"
#include "mcesrvsink.h"
#include "mceaudiosdpcodec.h"
#include "sdpcodecstringconstants.h"
#include "mcemediastate.h"

#define RESET()\
    iServer->Reset()
    
#include "mcepreconditions.h"


#define CONSTANT( constant ) iPreconditions->constant
#define DEFINE_QOSLINES()\
    CSdpAttributeField* qos1 = NULL;\
    CSdpAttributeField* qos2 = NULL;\
    CSdpAttributeField* qos3 = NULL;\
    CSdpAttributeField* qos4 = NULL;\
    CSdpAttributeField* qos5 = NULL;\
    CSdpAttributeField* qos6 = NULL
    
#define CREATE_QOS4( media, line1, line2, line3, line4 )\
    qos4 = CSdpAttributeField::DecodeLC( _L8( line4 ) );\
    qos3 = CSdpAttributeField::DecodeLC( _L8( line3 ) );\
    qos2 = CSdpAttributeField::DecodeLC( _L8( line2 ) );\
    qos1 = CSdpAttributeField::DecodeLC( _L8( line1 ) );\
    media->AttributeFields().AppendL( qos1 );\
    CleanupStack::Pop( qos1 );\
    media->AttributeFields().AppendL( qos2 );\
    CleanupStack::Pop( qos2 );\
    media->AttributeFields().AppendL( qos3 );\
    CleanupStack::Pop( qos3 );\
    media->AttributeFields().AppendL( qos4 );\
    CleanupStack::Pop( qos4 );
    
#define CREATE_QOS5( media, line1, line2, line3, line4, line5 )\
    CREATE_QOS4( media, line1, line2, line3, line4 );\
    qos5 = CSdpAttributeField::DecodeLC( _L8( line5 ) );\
    media->AttributeFields().AppendL( qos5 );\
    CleanupStack::Pop( qos5 )

#define CREATE_QOS6( media, line1, line2, line3, line4, line5, line6 )\
    CREATE_QOS5( media, line1, line2, line3, line4, line5 );\
    qos6 = CSdpAttributeField::DecodeLC( _L8( line6 ) );\
    media->AttributeFields().AppendL( qos6 );\
    CleanupStack::Pop( qos6 )
    

#define ASSERT_QOS1( media, ind, attr1, line1 )\
    EUNIT_ASSERT( media->AttributeFields()[ind+0]->Attribute().DesC() == _L8( attr1 ) );\
    EUNIT_ASSERT( media->AttributeFields()[ind+0]->Value() == _L8( line1 ) )

#define ASSERT_QOS2( media, ind, attr1, line1, attr2, line2 )\
    ASSERT_QOS1( media, ind, attr1, line1 );\
    EUNIT_ASSERT( media->AttributeFields()[ind+1]->Attribute().DesC() == _L8( attr2 ) );\
    EUNIT_ASSERT( media->AttributeFields()[ind+1]->Value() == _L8( line2 ) );
    
#define ASSERT_QOS4( media, ind, attr1, line1, attr2, line2, attr3, line3, attr4, line4 )\
    ASSERT_QOS2( media, ind, attr1, line1, attr2, line2 );\
    EUNIT_ASSERT( media->AttributeFields()[ind+2]->Attribute().DesC() == _L8( attr3 ) );\
    EUNIT_ASSERT( media->AttributeFields()[ind+2]->Value() == _L8( line3 ) );\
    EUNIT_ASSERT( media->AttributeFields()[ind+3]->Attribute().DesC() == _L8( attr4 ) );\
    EUNIT_ASSERT( media->AttributeFields()[ind+3]->Value() == _L8( line4 ) )

#define ASSERT_QOS5( media, ind, attr1, line1, attr2, line2, attr3, line3, attr4, line4, attr5, line5 )\
    ASSERT_QOS4( media, ind, attr1, line1, attr2, line2, attr3, line3, attr4, line4 );\
    EUNIT_ASSERT( media->AttributeFields()[ind+4]->Attribute().DesC() == _L8( attr5 ) );\
    EUNIT_ASSERT( media->AttributeFields()[ind+4]->Value() == _L8( line5 ) )

#define ASSERT_QOS6( media, ind, attr1, line1, attr2, line2, attr3, line3, attr4, line4, attr5, line5, attr6, line6 )\
    ASSERT_QOS5( media, ind, attr1, line1, attr2, line2, attr3, line3, attr4, line4, attr5, line5 );\
    EUNIT_ASSERT( media->AttributeFields()[ind+5]->Attribute().DesC() == _L8( attr6 ) );\
    EUNIT_ASSERT( media->AttributeFields()[ind+5]->Value() == _L8( line6 ) )

#define ASSERT_QOS7( media, ind, attr1, line1, attr2, line2, attr3, line3, attr4, line4, attr5, line5, attr6, line6, attr7, line7 )\
    ASSERT_QOS6( media, ind, attr1, line1, attr2, line2, attr3, line3, attr4, line4, attr5, line5, attr6, line6 );\
    EUNIT_ASSERT( media->AttributeFields()[ind+6]->Attribute().DesC() == _L8( attr7 ) );\
    EUNIT_ASSERT( media->AttributeFields()[ind+6]->Value() == _L8( line7 ) )


//  INTERNAL INCLUDES

// CONSTRUCTION
UT_TMceSegmentedPreconditions* UT_TMceSegmentedPreconditions::NewL()
    {
    UT_TMceSegmentedPreconditions* self = UT_TMceSegmentedPreconditions::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_TMceSegmentedPreconditions* UT_TMceSegmentedPreconditions::NewLC()
    {
    UT_TMceSegmentedPreconditions* self = new( ELeave ) UT_TMceSegmentedPreconditions();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_TMceSegmentedPreconditions::~UT_TMceSegmentedPreconditions()
    {
    }

// Default constructor
UT_TMceSegmentedPreconditions::UT_TMceSegmentedPreconditions()
    {
    }

// Second phase construct
void UT_TMceSegmentedPreconditions::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_TMceSegmentedPreconditions::SetupL()
    {
    iServer = new (ELeave) CMceServerStub();
    iManager = CMceMediaManager::NewL( *iServer );
    iSession = MceMediaManagerTestHelper::CreateMultimediaOutSessionL();
    iSession->SetObserver( *iServer );
    iManager->CreateSessionL( *iSession );
    
    iPreconditions = new (ELeave) TMceSegmentedPreconditions( *(iSession->Streams()[0]) );
    iPreconditions->ConstructL();
        
    } 

void UT_TMceSegmentedPreconditions::Teardown()
    {
	delete iSdpDocument;
    
	CMceComSession::Delete( iSession, *iManager );
	delete iManager;
	iManager = NULL;
	delete iServer;
	iServer = NULL;
	delete iPreconditions;
    }


void UT_TMceSegmentedPreconditions::UT_TMceSegmentedPreconditions_DoConstructLL()
    {
    
    EUNIT_ASSERT( iPreconditions->iStream.Direction() == SdpCodecStringConstants::EAttributeSendrecv );
    
    iPreconditions->DoConstructL();
    
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ECreated );
    
    iPreconditions->iStream.Enable( EFalse, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( EFalse, ETrue );
    
    EUNIT_ASSERT( iPreconditions->iStream.Direction() == SdpCodecStringConstants::EAttributeInactive );
    
    iPreconditions->DoConstructL();
    
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ECreated );

    iPreconditions->iStream.Enable( ETrue, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( EFalse, ETrue );
    
    EUNIT_ASSERT( iPreconditions->iStream.Direction() == SdpCodecStringConstants::EAttributeRecvonly );
    
    iPreconditions->DoConstructL();
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ECreated );

    iPreconditions->iStream.Enable( EFalse, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( ETrue, ETrue );
    
    EUNIT_ASSERT( iPreconditions->iStream.Direction() == SdpCodecStringConstants::EAttributeSendonly );
    
    iPreconditions->DoConstructL();
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ECreated );
    
    }

    
void UT_TMceSegmentedPreconditions::UT_TMceSegmentedPreconditions_EncodeLL()
    {

    RStringF audio = MCE_SDP_STRING_AUDIOL();
    RStringF protocol = SDP_STRINGL( SdpCodecStringConstants::EProtocolRtpAvp );
    
    CSdpMediaField* audioLine = 
        CSdpMediaField::NewLC( audio, 5005, protocol, _L8("0") );

    const TInt KCases = 4;
    const TInt KVariants = 2;
    const TInt KSendRecv = 0;
    const TInt KInactive = 1;
    const TInt KSendOnly = 2;
    const TInt KRecvOnly = 3;
    const TInt KNotReserved = 0;
    const TInt KReserved = 1;

//offerer    
    iPreconditions->iStream.Session()->NegotiationState().SetRole( EMceRoleOfferer );

    for ( TInt casendx = 0;casendx < KCases ;casendx++ )
        {
        for (TInt variant = 0;variant < KVariants; variant++)
            {
            audioLine->AttributeFields().ResetAndDestroy();
            iPreconditions->iState = TMcePreconditions::ECreated;

            switch( casendx )
                {
                case KSendRecv:
                    {
                    iPreconditions->iStream.Enable( ETrue, ETrue );
                    iPreconditions->iStream.iLinkedStream->Enable( ETrue, ETrue );
                    EUNIT_ASSERT( iPreconditions->iStream.Direction() == SdpCodecStringConstants::EAttributeSendrecv );
                    break;
                    }
                case KInactive:
                    {
                    iPreconditions->iStream.Enable( EFalse, ETrue );
                    iPreconditions->iStream.iLinkedStream->Enable( EFalse, ETrue );
                    EUNIT_ASSERT( iPreconditions->iStream.Direction() == SdpCodecStringConstants::EAttributeInactive );
                    break;
                    }
                case KSendOnly:
                    {
                    iPreconditions->iStream.Enable( EFalse, ETrue );
                    iPreconditions->iStream.iLinkedStream->Enable( ETrue, ETrue );
                    EUNIT_ASSERT( iPreconditions->iStream.Direction() == SdpCodecStringConstants::EAttributeSendonly );
                    break;
                    }
                case KRecvOnly:
                    {
                    iPreconditions->iStream.Enable( ETrue, ETrue );
                    iPreconditions->iStream.iLinkedStream->Enable( EFalse, ETrue );
                    EUNIT_ASSERT( iPreconditions->iStream.Direction() == SdpCodecStringConstants::EAttributeRecvonly );
                    break;
                    }
                default:
                    {
                    EUNIT_ASSERT( EFalse );
                    break;
                    }
                }
            
                iPreconditions->DoConstructL();
            
                if ( variant == KReserved )
                    {
                    iPreconditions->Reserved();
                    }
                    
                iPreconditions->EncodeL( *audioLine );

                switch( casendx )
                    {
                    case KSendRecv:
                        {
                        if ( variant == KNotReserved )
                            {
                            ASSERT_QOS4( audioLine, 0,
                                         "curr", "qos local none",
                                         "curr", "qos remote none",
                                         "des" , "qos mandatory local sendrecv",
                                         "des" , "qos none remote sendrecv" );
                            }
                        else if ( variant == KReserved )
                            {
                            ASSERT_QOS4( audioLine, 0,
                                         "curr", "qos local sendrecv",
                                         "curr", "qos remote none",
                                         "des" , "qos mandatory local sendrecv",
                                         "des" , "qos none remote sendrecv" );
                            }
                        break;
                        }
                    case KInactive:
                        {
                        if ( variant == KNotReserved )
                            {
                            ASSERT_QOS4( audioLine, 0,
                                         "curr", "qos local none",
                                         "curr", "qos remote none",
                                         "des" , "qos mandatory local sendrecv",
                                         "des" , "qos none remote sendrecv" );
                            }
                        else if ( variant == KReserved )
                            {
                            ASSERT_QOS4( audioLine, 0,
                                         "curr", "qos local sendrecv",
                                         "curr", "qos remote none",
                                         "des" , "qos mandatory local sendrecv",
                                         "des" , "qos none remote sendrecv" );
                            }
                        break;
                        }
                    case KSendOnly:
                        {
                        if ( variant == KNotReserved )
                            {
                            ASSERT_QOS5( audioLine, 0,
                                         "curr", "qos local none",
                                         "curr", "qos remote none",
                                         "des" , "qos mandatory local send",
                                         "des" , "qos none local recv",
                                         "des" , "qos none remote sendrecv" );
                            }
                        else if ( variant == KReserved )
                            {
                            ASSERT_QOS5( audioLine, 0,
                                         "curr", "qos local send",
                                         "curr", "qos remote none",
                                         "des" , "qos mandatory local send",
                                         "des" , "qos none local recv",
                                         "des" , "qos none remote sendrecv" );
                            }
                        break;
                        }
                    case KRecvOnly:
                        {
                        if ( variant == KNotReserved )
                            {
                            ASSERT_QOS5( audioLine, 0,
                                         "curr", "qos local none",
                                         "curr", "qos remote none",
                                         "des" , "qos none local send",
                                         "des" , "qos mandatory local recv",
                                         "des" , "qos none remote sendrecv" );
                            }
                        else if ( variant == KReserved )
                            {
                            ASSERT_QOS5( audioLine, 0,
                                         "curr", "qos local recv",
                                         "curr", "qos remote none",
                                         "des" , "qos none local send",
                                         "des" , "qos mandatory local recv",
                                         "des" , "qos none remote sendrecv" );
                            }
                        break;
                        }
                    default:
                        {
                        EUNIT_ASSERT( EFalse );
                        break;
                        }
                    }

                EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ENegotiating );
            }
        }

//answerer
    iPreconditions->iStream.Session()->NegotiationState().SetRole( EMceRoleAnswerer );

    for ( TInt casendx = 0;casendx < KCases ;casendx++ )
        {

        for ( TInt variant = 0;variant < KVariants; variant++)
            {

            audioLine->AttributeFields().ResetAndDestroy();
            iPreconditions->iState = TMcePreconditions::ECreated;

            switch( casendx )
                {
                case KSendRecv:
                    {
                    iPreconditions->iStream.Enable( ETrue, ETrue );
                    iPreconditions->iStream.iLinkedStream->Enable( ETrue, ETrue );
                    iPreconditions->DoConstructL();
                    iPreconditions->iDesiredRemoteSend = CONSTANT( iKAttributeMandatory );
                    iPreconditions->iDesiredRemoteRecv = CONSTANT( iKAttributeMandatory );
                    
                    if ( variant == KReserved )
                        {
                        iPreconditions->iCurrentRemoteStatus = CONSTANT( iKAttributeSendRecv );
                        }
                    EUNIT_ASSERT( iPreconditions->iStream.Direction() == SdpCodecStringConstants::EAttributeSendrecv );
                    break;
                    }
                case KInactive:
                    {
                    iPreconditions->iStream.Enable( EFalse, ETrue );
                    iPreconditions->iStream.iLinkedStream->Enable( EFalse, ETrue );
                    iPreconditions->DoConstructL();
                    iPreconditions->iDesiredRemoteSend = CONSTANT( iKAttributeMandatory );
                    iPreconditions->iDesiredRemoteRecv = CONSTANT( iKAttributeMandatory );
                    if ( variant == KReserved )
                        {
                        iPreconditions->iCurrentRemoteStatus = CONSTANT( iKAttributeSendRecv );
                        }
                    EUNIT_ASSERT( iPreconditions->iStream.Direction() == SdpCodecStringConstants::EAttributeInactive );
                    break;
                    }
                case KSendOnly:
                    {
                    iPreconditions->iStream.Enable( EFalse, ETrue );
                    iPreconditions->iStream.iLinkedStream->Enable( ETrue, ETrue );
                    iPreconditions->DoConstructL();
                    iPreconditions->iDesiredRemoteSend = CONSTANT( iKAttributeMandatory );
                    iPreconditions->iDesiredRemoteRecv = CONSTANT( iKAttributeNone );
                    if ( variant == KReserved )
                        {
                        iPreconditions->iCurrentRemoteStatus = CONSTANT( iKAttributeSend );
                        }
                    EUNIT_ASSERT( iPreconditions->iStream.Direction() == SdpCodecStringConstants::EAttributeSendonly );
                    break;
                    }
                case KRecvOnly:
                    {
                    iPreconditions->iStream.Enable( ETrue, ETrue );
                    iPreconditions->iStream.iLinkedStream->Enable( EFalse, ETrue );
                    iPreconditions->DoConstructL();
                    iPreconditions->iDesiredRemoteSend = CONSTANT( iKAttributeNone );
                    iPreconditions->iDesiredRemoteRecv = CONSTANT( iKAttributeMandatory );
                    if ( variant == KReserved )
                        {
                        iPreconditions->iCurrentRemoteStatus = CONSTANT( iKAttributeRecv );
                        }
                    EUNIT_ASSERT( iPreconditions->iStream.Direction() == SdpCodecStringConstants::EAttributeRecvonly );
                    break;
                    }
                default:
                    {
                    EUNIT_ASSERT( EFalse );
                    break;
                    }
                }
            
                //local is now always reserved
                iPreconditions->Reserved();
                iPreconditions->EncodeL( *audioLine );

                switch( casendx )
                    {
                    case KSendRecv:
                        {
                        if ( variant == KNotReserved )
                            {
                            //offer:
                            //a=sendrecv
                            //curr: qos local none
                            //curr: qos remote none
                            //des: qos mandatory local sendrecv
                            //des: qos none remote sendrecv
                            ASSERT_QOS5( audioLine, 0,
                                         "curr" , "qos local sendrecv",
                                         "curr" , "qos remote none",
                                         "des"  , "qos mandatory local sendrecv",
                                         "des"  , "qos mandatory remote sendrecv",
                                         "conf" , "qos remote sendrecv" );
                            EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ENegotiating );
                                         
                            }
                        else if ( variant == KReserved )
                            {
                            //offer:
                            //a=sendrecv
                            //curr: qos local sendrecv
                            //curr: qos remote none
                            //des: qos mandatory local sendrecv
                            //des: qos none remote sendrecv
                            ASSERT_QOS4( audioLine, 0,
                                         "curr", "qos local sendrecv",
                                         "curr", "qos remote sendrecv",
                                         "des" , "qos mandatory local sendrecv",
                                         "des" , "qos mandatory remote sendrecv" );
                            EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::EReserved );
                            }
                        break;
                        }
                    case KInactive:
                        {
                        if ( variant == KNotReserved )
                            {
                            //offer:
                            //a=inactive
                            //curr: qos local none
                            //curr: qos remote none
                            //des: qos mandatory local sendrecv
                            //des: qos none remote sendrecv
                            ASSERT_QOS5( audioLine, 0,
                                         "curr" , "qos local sendrecv",
                                         "curr" , "qos remote none",
                                         "des"  , "qos mandatory local sendrecv",
                                         "des"  , "qos mandatory remote sendrecv",
                                         "conf" , "qos remote sendrecv" );
                            EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ENegotiating );
                            }
                        else if ( variant == KReserved )
                            {
                            //offer:
                            //a=inactive
                            //curr: qos local sendrecv
                            //curr: qos remote none
                            //des: qos mandatory local sendrecv
                            //des: qos none remote sendrecv
                            ASSERT_QOS4( audioLine, 0,
                                         "curr", "qos local sendrecv",
                                         "curr", "qos remote sendrecv",
                                         "des" , "qos mandatory local sendrecv",
                                         "des" , "qos mandatory remote sendrecv" );
                            EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::EReserved );
                            }
                        break;
                        }
                    case KSendOnly:
                        {
                        if ( variant == KNotReserved )
                            {
                            //offer:
                            //a=recvonly
                            //curr: qos local none
                            //curr: qos remote none
                            //des: qos none local send
                            //des: qos mandatory local recv
                            //des: qos none remote sendrecv
                            ASSERT_QOS7( audioLine, 0,
                                         "curr" , "qos local send",
                                         "curr" , "qos remote none",
                                         "des"  , "qos mandatory local send",
                                         "des"  , "qos none local recv",
                                         "des"  , "qos mandatory remote send",
                                         "des"  , "qos none remote recv",
                                         "conf" , "qos remote send" );
                            EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ENegotiating );
                            }
                        else if ( variant == KReserved )
                            {
                            //offer:
                            //a=recvonly
                            //curr: qos local recv
                            //curr: qos remote none
                            //des: qos none local send
                            //des: qos mandatory local recv
                            //des: qos none remote sendrecv
                            ASSERT_QOS6( audioLine, 0,
                                         "curr" , "qos local send",
                                         "curr" , "qos remote send",
                                         "des"  , "qos mandatory local send",
                                         "des"  , "qos none local recv",
                                         "des"  , "qos mandatory remote send",
                                         "des"  , "qos none remote recv" );
                            EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::EReserved );
                            }
                        break;
                        }
                    case KRecvOnly:
                        {
                        if ( variant == KNotReserved )
                            {
                            //offer:
                            //a=sendonly
                            //curr: qos local none
                            //curr: qos remote none
                            //des: qos mandatory local send
                            //des: qos none local recv
                            //des: qos none remote sendrecv
                            ASSERT_QOS7( audioLine, 0,
                                         "curr" , "qos local recv",
                                         "curr" , "qos remote none",
                                         "des"  , "qos none local send",
                                         "des"  , "qos mandatory local recv",
                                         "des"  , "qos none remote send",
                                         "des"  , "qos mandatory remote recv",
                                         "conf" , "qos remote recv" );
                            EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ENegotiating );
                            }
                        else if ( variant == KReserved )
                            {
                            //offer:
                            //a=sendonly
                            //curr: qos local send
                            //curr: qos remote none
                            //des: qos mandatory local send
                            //des: qos none local recv
                            //des: qos none remote sendrecv
                            ASSERT_QOS6( audioLine, 0,
                                         "curr" , "qos local recv",
                                         "curr" , "qos remote recv",
                                         "des"  , "qos none local send",
                                         "des"  , "qos mandatory local recv",
                                         "des"  , "qos none remote send",
                                         "des"  , "qos mandatory remote recv" );
                            EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::EReserved );
                            }
                        break;
                        }
                    default:
                        {
                        EUNIT_ASSERT( EFalse );
                        break;
                        }
                    }

            }
        }
    

//special case: answer to inactive sendonly
//a=inactive
//curr: qos local none
//curr: qos remote none
//des: qos mandatory local send
//des: qos none local recv
//des: qos none remote sendrecv
    audioLine->AttributeFields().ResetAndDestroy();
    iPreconditions->iState = TMcePreconditions::ECreated;
    iPreconditions->iStream.Enable( EFalse, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( EFalse, ETrue );
    iPreconditions->DoConstructL();
    iPreconditions->iDesiredRemoteSend = CONSTANT( iKAttributeNone );
    iPreconditions->iDesiredRemoteRecv = CONSTANT( iKAttributeMandatory );
    EUNIT_ASSERT( iPreconditions->iStream.Direction() == SdpCodecStringConstants::EAttributeInactive );
    //local is now always reserved
    iPreconditions->Reserved();
    iPreconditions->EncodeL( *audioLine );

    ASSERT_QOS6( audioLine, 0,
                 "curr" , "qos local sendrecv",
                 "curr" , "qos remote none",
                 "des"  , "qos mandatory local sendrecv",
                 "des"  , "qos none remote send",
                 "des"  , "qos mandatory remote recv",
                 "conf" , "qos remote recv" );
    
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ENegotiating );
    CleanupStack::PopAndDestroy( audioLine );
    
    }
    
    
void UT_TMceSegmentedPreconditions::UT_TMceSegmentedPreconditions_DecodeLL()
    {
    RStringF audio = MCE_SDP_STRING_AUDIOL();
    RStringF protocol = SDP_STRINGL( SdpCodecStringConstants::EProtocolRtpAvp );
    
    CSdpMediaField* audioLine = 
        CSdpMediaField::NewLC( audio, 5005, protocol, _L8("0") );

    iPreconditions->iStream.Session()->NegotiationState().SetRole( EMceRoleAnswerer );

//no qos lines
    iPreconditions->iState = TMcePreconditions::ECreated;
    iPreconditions->iStream.Enable( ETrue, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( ETrue, ETrue );
    iPreconditions->DoConstructL();
    iPreconditions->Reserved();

    iPreconditions->DecodeL( *audioLine );
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::EReserved );

    
    DEFINE_QOSLINES();

//answerer reserved, offerer not reserved
    iPreconditions->iState = TMcePreconditions::ECreated;
    iPreconditions->iStream.Enable( ETrue, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( ETrue, ETrue );
    iPreconditions->DoConstructL();
    iPreconditions->Reserved();
    
    CREATE_QOS4( audioLine,
                 "a=curr:qos local none\r\n",
                 "a=curr:qos remote none\r\n",
                 "a=des:qos mandatory local sendrecv\r\n",
                 "a=des:qos none remote sendrecv\r\n" );
                 
    
    iPreconditions->DecodeL( *audioLine );
    
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ENegotiating );

//answerer not reserved, offerer not reserved
    iPreconditions->iState = TMcePreconditions::ECreated;
    iPreconditions->iStream.Enable( ETrue, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( ETrue, ETrue );
    iPreconditions->DoConstructL();
    
    CREATE_QOS4( audioLine,
                 "a=curr:qos local none\r\n",
                 "a=curr:qos remote none\r\n",
                 "a=des:qos mandatory local sendrecv\r\n",
                 "a=des:qos none remote sendrecv\r\n" );
    
    iPreconditions->DecodeL( *audioLine );
    
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ENegotiating );

//answerer reserved, offerer not reserved
    iPreconditions->iState = TMcePreconditions::ECreated;
    iPreconditions->iStream.Enable( ETrue, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( EFalse, ETrue );
    iPreconditions->DoConstructL();
    iPreconditions->Reserved();
    
    CREATE_QOS5( audioLine,
                 "a=curr:qos local none\r\n",
                 "a=curr:qos remote none\r\n",
                 "a=des:qos mandatory local send\r\n",
                 "a=des:qos none local recv\r\n",
                 "a=des:qos none remote sendrecv\r\n" );
                 
    
    iPreconditions->DecodeL( *audioLine );
    
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeRecv ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ENegotiating );

//answerer not reserved, offerer reserved
    iPreconditions->iState = TMcePreconditions::ECreated;
    iPreconditions->iStream.Enable( ETrue, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( EFalse, ETrue );
    iPreconditions->DoConstructL();
    
    CREATE_QOS5( audioLine,
                 "a=curr:qos local send\r\n",
                 "a=curr:qos remote none\r\n",
                 "a=des:qos mandatory local send\r\n",
                 "a=des:qos none local recv\r\n",
                 "a=des:qos none remote sendrecv\r\n" );
    
    iPreconditions->DecodeL( *audioLine );
    
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeRecv ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ENegotiating );


//answerer reserved, offerer reserved
    audioLine->AttributeFields().ResetAndDestroy();
    iPreconditions->iState = TMcePreconditions::ECreated;
    iPreconditions->iStream.Enable( EFalse, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( ETrue, ETrue );
    iPreconditions->DoConstructL();
    iPreconditions->Reserved();

    CREATE_QOS5( audioLine,
                 "a=curr:qos local recv\r\n",
                 "a=curr:qos remote none\r\n",
                 "a=des:qos none local send\r\n",
                 "a=des:qos mandatory local recv\r\n",
                 "a=des:qos none remote sendrecv\r\n" );

    iPreconditions->DecodeL( *audioLine );

    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeSend ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeSend ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::EReserved );

//answerer reserved, offerer reserved, incorrect qos
    audioLine->AttributeFields().ResetAndDestroy();
    iPreconditions->iState = TMcePreconditions::ECreated;
    iPreconditions->iStream.Enable( ETrue, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( ETrue, ETrue );
    iPreconditions->DoConstructL();
    iPreconditions->Reserved();

    CREATE_QOS6( audioLine,
                 "a=curr:qos local sendrecv\r\n",
                 "a=curr:qos remote send\r\n",//wrong
                 "a=des:qos mandatory local sendrecv\r\n",
                 "a=des:qos mandatory remote recv\r\n",//wrong with trailing tokens
                 "a=conf:qos remote sendrecv\r\n",//not for answerer
                 "a=sec:remote sendrecv\r\n" );//extra

    iPreconditions->DecodeL( *audioLine );

    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::EReserved );


    iPreconditions->iStream.Session()->NegotiationState().SetRole( EMceRoleOfferer );


//offerer not reserved, answerer not reserved
    audioLine->AttributeFields().ResetAndDestroy();
    iPreconditions->iState = TMcePreconditions::ENegotiating;
    iPreconditions->iStream.Enable( ETrue, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( ETrue, ETrue );
    iPreconditions->DoConstructL();
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeNone ) );


    CREATE_QOS5( audioLine,
                 "a=curr:qos local none\r\n",
                 "a=curr:qos remote none\r\n",
                 "a=des:qos mandatory local sendrecv\r\n",
                 "a=des:qos mandatory remote sendrecv\r\n",
                 "a=conf:qos remote sendrecv\r\n");
    
    iPreconditions->DecodeL( *audioLine );
    
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ENegotiating );

//offerer reserved, answerer not reserved
    audioLine->AttributeFields().ResetAndDestroy();
    iPreconditions->iState = TMcePreconditions::ENegotiating;
    iPreconditions->iStream.Enable( ETrue, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( ETrue, ETrue );
    iPreconditions->DoConstructL();
    iPreconditions->Reserved();
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeNone ) );


    CREATE_QOS4( audioLine,
                 "a=curr:qos local none\r\n",
                 "a=curr:qos remote sendrecv\r\n",
                 "a=des:qos mandatory local sendrecv\r\n",
                 "a=des:qos mandatory remote sendrecv\r\n" );
    
    iPreconditions->DecodeL( *audioLine );
    
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ENegotiating );


//offerer not reserved, answerer reserved
    audioLine->AttributeFields().ResetAndDestroy();
    iPreconditions->iState = TMcePreconditions::ENegotiating;
    iPreconditions->iStream.Enable( ETrue, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( ETrue, ETrue );
    iPreconditions->DoConstructL();
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeNone ) );


    CREATE_QOS5( audioLine,
                 "a=curr:qos local sendrecv\r\n",
                 "a=curr:qos remote none\r\n",
                 "a=des:qos mandatory local sendrecv\r\n",
                 "a=des:qos mandatory remote sendrecv\r\n",
                 "a=conf:qos remote sendrecv\r\n");
    
    iPreconditions->DecodeL( *audioLine );
    
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ENegotiating );

//offerer reserved, answerer reserved
    audioLine->AttributeFields().ResetAndDestroy();
    iPreconditions->iState = TMcePreconditions::ENegotiating;
    iPreconditions->iStream.Enable( ETrue, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( ETrue, ETrue );
    iPreconditions->DoConstructL();
    iPreconditions->Reserved();
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeNone ) );


    CREATE_QOS4( audioLine,
                 "a=curr:qos local sendrecv\r\n",
                 "a=curr:qos remote sendrecv\r\n",
                 "a=des:qos mandatory local sendrecv\r\n",
                 "a=des:qos mandatory remote sendrecv\r\n" );
    
    iPreconditions->DecodeL( *audioLine );
    
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::EReserved );

//offerer reserved, answerer reserved + conf
    audioLine->AttributeFields().ResetAndDestroy();
    iPreconditions->iState = TMcePreconditions::ENegotiating;
    iPreconditions->iStream.Enable( ETrue, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( ETrue, ETrue );
    iPreconditions->DoConstructL();
    iPreconditions->Reserved();
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeNone ) );


    CREATE_QOS5( audioLine,
                 "a=curr:qos local sendrecv\r\n",
                 "a=curr:qos remote sendrecv\r\n",
                 "a=des:qos mandatory local sendrecv\r\n",
                 "a=des:qos mandatory remote sendrecv\r\n",
                 "a=conf:qos remote sendrecv\r\n" );
    
    iPreconditions->DecodeL( *audioLine );
    
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ENegotiating );


//offerer: e2e reserved + conf
    audioLine->AttributeFields().ResetAndDestroy();
    iPreconditions->iState = TMcePreconditions::EReserved;
    iPreconditions->iConfirmation = CONSTANT( iKAttributeNone );

    CREATE_QOS5( audioLine,
                 "a=curr:qos local sendrecv\r\n",
                 "a=curr:qos remote sendrecv\r\n",
                 "a=des:qos mandatory local sendrecv\r\n",
                 "a=des:qos mandatory remote sendrecv\r\n",
                 "a=conf:qos remote sendrecv\r\n" );
    
    iPreconditions->DecodeL( *audioLine );
    
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ENegotiating );

//offerer: e2e reserved no conf
    audioLine->AttributeFields().ResetAndDestroy();
    iPreconditions->iState = TMcePreconditions::EReserved;
    iPreconditions->iConfirmation = CONSTANT( iKAttributeNone );

    CREATE_QOS4( audioLine,
                 "a=curr:qos local sendrecv\r\n",
                 "a=curr:qos remote sendrecv\r\n",
                 "a=des:qos mandatory local sendrecv\r\n",
                 "a=des:qos mandatory remote sendrecv\r\n");
    
    iPreconditions->DecodeL( *audioLine );
    
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::EReserved );

    CleanupStack::PopAndDestroy( audioLine );
    
    
    }
    
void UT_TMceSegmentedPreconditions::UT_TMceSegmentedPreconditions_ReservedL()
    {
    EUNIT_ASSERT( iPreconditions->iStream.Direction() == SdpCodecStringConstants::EAttributeSendrecv );
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );

    iPreconditions->Reserved();

    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );
    
    iPreconditions->iStream.Enable( EFalse, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( EFalse, ETrue );
    iPreconditions->DoConstructL();
    
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );

    iPreconditions->Reserved();
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );

    iPreconditions->iStream.Enable( ETrue, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( EFalse, ETrue );
    iPreconditions->DoConstructL();
    
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );

    iPreconditions->Reserved();
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeRecv ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );

    iPreconditions->iStream.Enable( EFalse, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( ETrue, ETrue );
    iPreconditions->DoConstructL();
    
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeNone ) );

    iPreconditions->Reserved();
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeSend ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeNone ) );
    
    }
    
void UT_TMceSegmentedPreconditions::UT_TMceSegmentedPreconditions_CloneLL()
    {

    iPreconditions->iStream.Enable( ETrue, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( EFalse, ETrue );
    iPreconditions->DoConstructL();
    iPreconditions->Reserved();
    
    TMceSegmentedPreconditions* clone = 
        static_cast<TMceSegmentedPreconditions*>( iPreconditions->CloneL( iPreconditions->iStream ) );
    
    CleanupStack::PushL( clone );
    
    EUNIT_ASSERT( clone->iKAttributeCurr == iPreconditions->iKAttributeCurr );
    EUNIT_ASSERT( clone->iKAttributeDes == iPreconditions->iKAttributeDes );
    EUNIT_ASSERT( clone->iKAttributeConf == iPreconditions->iKAttributeConf );
    EUNIT_ASSERT( clone->iKAttributeNone == iPreconditions->iKAttributeNone );
    EUNIT_ASSERT( clone->iKAttributeSend == iPreconditions->iKAttributeSend );
    EUNIT_ASSERT( clone->iKAttributeRecv == iPreconditions->iKAttributeRecv );
    EUNIT_ASSERT( clone->iKAttributeSendRecv == iPreconditions->iKAttributeSendRecv );
    EUNIT_ASSERT( clone->iKAttributeRemote == iPreconditions->iKAttributeRemote );
    EUNIT_ASSERT( clone->iKAttributeLocal == iPreconditions->iKAttributeLocal );
    EUNIT_ASSERT( clone->iKAttributeMandatory == iPreconditions->iKAttributeMandatory );
    EUNIT_ASSERT( clone->iKAttributeOptional == iPreconditions->iKAttributeOptional );
    EUNIT_ASSERT( clone->iKAttributeFailure == iPreconditions->iKAttributeFailure );
    EUNIT_ASSERT( clone->iKAttributeUnknown == iPreconditions->iKAttributeUnknown );
    EUNIT_ASSERT( clone->iCurrentLocalStatus == iPreconditions->iCurrentLocalStatus );
    EUNIT_ASSERT( clone->iCurrentRemoteStatus == iPreconditions->iCurrentRemoteStatus );
    EUNIT_ASSERT( clone->iDesiredLocalSend == iPreconditions->iDesiredLocalSend );
    EUNIT_ASSERT( clone->iDesiredLocalRecv == iPreconditions->iDesiredLocalRecv );
    EUNIT_ASSERT( clone->iDesiredRemoteSend == iPreconditions->iDesiredRemoteSend );
    EUNIT_ASSERT( clone->iDesiredRemoteRecv == iPreconditions->iDesiredRemoteRecv );
    EUNIT_ASSERT( clone->iConfirmation == iPreconditions->iConfirmation );
    EUNIT_ASSERT( clone->iState == iPreconditions->iState );
    
    CleanupStack::PopAndDestroy( clone );
    
    }
    
void UT_TMceSegmentedPreconditions::UT_TMceSegmentedPreconditions_EncodeCurrentStatusLL()
    {
    RStringF audio = MCE_SDP_STRING_AUDIOL();
    RStringF protocol = SDP_STRINGL( SdpCodecStringConstants::EProtocolRtpAvp );
    
    CSdpMediaField* audioLine = 
        CSdpMediaField::NewLC( audio, 5005, protocol, _L8("0") );

    iPreconditions->iStream.Enable( ETrue, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( ETrue, ETrue );
    iPreconditions->Reserved();
    EUNIT_ASSERT( iPreconditions->iStream.Direction() == SdpCodecStringConstants::EAttributeSendrecv );
    
    iPreconditions->EncodeCurrentStatusL( *audioLine, CONSTANT( iKAttributeLocal ) );

    ASSERT_QOS1( audioLine, 0,
                 "curr" , "qos local sendrecv" );
    
    iPreconditions->EncodeCurrentStatusL( *audioLine, CONSTANT( iKAttributeRemote ) );

    ASSERT_QOS1( audioLine, 1,
                 "curr" , "qos remote none" );

    CleanupStack::PopAndDestroy( audioLine );

    }
    
void UT_TMceSegmentedPreconditions::UT_TMceSegmentedPreconditions_EncodeDesiredStatusLL()
    {
    RStringF audio = MCE_SDP_STRING_AUDIOL();
    RStringF protocol = SDP_STRINGL( SdpCodecStringConstants::EProtocolRtpAvp );
    
    CSdpMediaField* audioLine = 
        CSdpMediaField::NewLC( audio, 5005, protocol, _L8("0") );

    iPreconditions->iStream.Enable( ETrue, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( ETrue, ETrue );
    EUNIT_ASSERT( iPreconditions->iStream.Direction() == SdpCodecStringConstants::EAttributeSendrecv );

    audioLine->AttributeFields().ResetAndDestroy();
    iPreconditions->iDesiredLocalSend = CONSTANT( iKAttributeMandatory );
    iPreconditions->iDesiredLocalRecv = CONSTANT( iKAttributeMandatory );
    iPreconditions->iDesiredRemoteSend = CONSTANT( iKAttributeMandatory );
    iPreconditions->iDesiredRemoteRecv = CONSTANT( iKAttributeMandatory );
    
    iPreconditions->EncodeDesiredStatusL( *audioLine, CONSTANT( iKAttributeLocal ) );

    ASSERT_QOS1( audioLine, 0,
                 "des" , "qos mandatory local sendrecv" );

                 
    audioLine->AttributeFields().ResetAndDestroy();
    iPreconditions->iDesiredLocalSend = CONSTANT( iKAttributeNone );
    iPreconditions->iDesiredLocalRecv = CONSTANT( iKAttributeMandatory );

    iPreconditions->EncodeDesiredStatusL( *audioLine, CONSTANT( iKAttributeLocal ) );

    ASSERT_QOS2( audioLine, 0,
                 "des" , "qos none local send",
                 "des" , "qos mandatory local recv" );
                 

    
    audioLine->AttributeFields().ResetAndDestroy();
    
    iPreconditions->EncodeDesiredStatusL( *audioLine, CONSTANT( iKAttributeRemote ) );

    ASSERT_QOS1( audioLine, 0,
                 "des" , "qos mandatory remote sendrecv" );


    audioLine->AttributeFields().ResetAndDestroy();
    iPreconditions->iDesiredRemoteSend = CONSTANT( iKAttributeMandatory );
    iPreconditions->iDesiredRemoteRecv = CONSTANT( iKAttributeNone );

    iPreconditions->EncodeDesiredStatusL( *audioLine, CONSTANT( iKAttributeRemote ) );
    
    ASSERT_QOS2( audioLine, 0,
                 "des" , "qos mandatory remote send",
                 "des" , "qos none remote recv" );

    audioLine->AttributeFields().ResetAndDestroy();
    iPreconditions->iDesiredRemoteSend = CONSTANT( iKAttributeOptional );
    iPreconditions->iDesiredRemoteRecv = CONSTANT( iKAttributeNone );

    iPreconditions->EncodeDesiredStatusL( *audioLine, CONSTANT( iKAttributeRemote ) );
    
    ASSERT_QOS2( audioLine, 0,
                 "des" , "qos optional remote send",
                 "des" , "qos none remote recv" );

    audioLine->AttributeFields().ResetAndDestroy();
    iPreconditions->iDesiredRemoteSend = CONSTANT( iKAttributeFailure );
    iPreconditions->iDesiredRemoteRecv = CONSTANT( iKAttributeNone );

    iPreconditions->EncodeDesiredStatusL( *audioLine, CONSTANT( iKAttributeRemote ) );
    
    ASSERT_QOS2( audioLine, 0,
                 "des" , "qos failure remote send",
                 "des" , "qos none remote recv" );

    audioLine->AttributeFields().ResetAndDestroy();
    iPreconditions->iDesiredRemoteSend = CONSTANT( iKAttributeUnknown );
    iPreconditions->iDesiredRemoteRecv = CONSTANT( iKAttributeNone );

    iPreconditions->EncodeDesiredStatusL( *audioLine, CONSTANT( iKAttributeRemote ) );
    
    ASSERT_QOS2( audioLine, 0,
                 "des" , "qos unknown remote send",
                 "des" , "qos none remote recv" );

    CleanupStack::PopAndDestroy( audioLine );

    }
    
void UT_TMceSegmentedPreconditions::UT_TMceSegmentedPreconditions_EncodeConfirmationLL()
    {
    RStringF audio = MCE_SDP_STRING_AUDIOL();
    RStringF protocol = SDP_STRINGL( SdpCodecStringConstants::EProtocolRtpAvp );
    
    CSdpMediaField* audioLine = 
        CSdpMediaField::NewLC( audio, 5005, protocol, _L8("0") );

    iPreconditions->iStream.Enable( ETrue, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( ETrue, ETrue );
    iPreconditions->Reserved();
    EUNIT_ASSERT( iPreconditions->iStream.Direction() == SdpCodecStringConstants::EAttributeSendrecv );

    iPreconditions->EncodeConfirmationL( *audioLine );

    EUNIT_ASSERT( audioLine->AttributeFields().Count() == 0 );

    iPreconditions->iConfirmation = CONSTANT( iKAttributeRecv );
    iPreconditions->EncodeConfirmationL( *audioLine );

    ASSERT_QOS1( audioLine, 0,
                 "conf" , "qos remote recv" );
    
    CleanupStack::PopAndDestroy( audioLine );
    
    }
    
void UT_TMceSegmentedPreconditions::UT_TMceSegmentedPreconditions_CurrentStausTextLCL()
    {
    
    HBufC8* text = iPreconditions->CurrentStausTextLC( CONSTANT( iKAttributeLocal ).DesC(), 
                                                       CONSTANT( iKAttributeRecv ).DesC() );
                                                       
    EUNIT_ASSERT( *text == _L8("qos local recv") );
    
    CleanupStack::PopAndDestroy( text );
                                                       
    }
    
void UT_TMceSegmentedPreconditions::UT_TMceSegmentedPreconditions_DesiredStausTextLCL()
    {
    
    HBufC8* text = iPreconditions->DesiredStausTextLC( CONSTANT( iKAttributeMandatory ).DesC(), 
                                                       CONSTANT( iKAttributeLocal ).DesC(), 
                                                       CONSTANT( iKAttributeRecv ).DesC() );
                                                       
    EUNIT_ASSERT( *text == _L8("qos mandatory local recv") );
    
    CleanupStack::PopAndDestroy( text );

    }
    
void UT_TMceSegmentedPreconditions::UT_TMceSegmentedPreconditions_DecodeCurrentStatusLL()
    {

    iPreconditions->iCurrentLocalStatus = CONSTANT( iKAttributeNone );
    iPreconditions->iCurrentRemoteStatus = CONSTANT( iKAttributeNone );

    CSdpAttributeField* attribute = CSdpAttributeField::DecodeLC( _L8("a=curr:qos local sendrecv\r\n" ) );
    
    EUNIT_ASSERT( iPreconditions->DecodeCurrentStatusL( *attribute ) == 1 );
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeSendRecv ) );
    
    CleanupStack::PopAndDestroy( attribute );
    iPreconditions->iCurrentLocalStatus = CONSTANT( iKAttributeNone );
    iPreconditions->iCurrentRemoteStatus = CONSTANT( iKAttributeNone );

     attribute = CSdpAttributeField::DecodeLC( _L8("a=curr:qos remote sendrecv\r\n" ) );
    
    EUNIT_ASSERT( iPreconditions->DecodeCurrentStatusL( *attribute ) == 1 );
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );

    CleanupStack::PopAndDestroy( attribute );
    iPreconditions->iCurrentLocalStatus = CONSTANT( iKAttributeNone );
    iPreconditions->iCurrentRemoteStatus = CONSTANT( iKAttributeNone );

    attribute = CSdpAttributeField::DecodeLC( _L8("a=curr:x local sendrecv\r\n" ) );
    
    EUNIT_ASSERT( iPreconditions->DecodeCurrentStatusL( *attribute ) == 0 );
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );

    CleanupStack::PopAndDestroy( attribute );
    
    }
    
void UT_TMceSegmentedPreconditions::UT_TMceSegmentedPreconditions_DecodeDesiredStatusLL()
    {
    iPreconditions->iDesiredLocalSend = CONSTANT( iKAttributeNone );
    iPreconditions->iDesiredLocalRecv = CONSTANT( iKAttributeNone );
    iPreconditions->iDesiredRemoteSend = CONSTANT( iKAttributeNone );
    iPreconditions->iDesiredRemoteRecv = CONSTANT( iKAttributeNone );

    CSdpAttributeField* attribute = CSdpAttributeField::DecodeLC( _L8("a=des:qos mandatory local sendrecv\r\n" ) );
    
    EUNIT_ASSERT( iPreconditions->DecodeDesiredStatusL( *attribute ) == 1 );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeMandatory ) );
    
    CleanupStack::PopAndDestroy( attribute );
    iPreconditions->iDesiredLocalSend = CONSTANT( iKAttributeNone );
    iPreconditions->iDesiredLocalRecv = CONSTANT( iKAttributeNone );
    iPreconditions->iDesiredRemoteSend = CONSTANT( iKAttributeNone );
    iPreconditions->iDesiredRemoteRecv = CONSTANT( iKAttributeNone );

    attribute = CSdpAttributeField::DecodeLC( _L8("a=des:qos mandatory remote sendrecv\r\n" ) );
    
    EUNIT_ASSERT( iPreconditions->DecodeDesiredStatusL( *attribute ) == 1 );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeNone ) );

    CleanupStack::PopAndDestroy( attribute );
    iPreconditions->iDesiredLocalSend = CONSTANT( iKAttributeNone );
    iPreconditions->iDesiredLocalRecv = CONSTANT( iKAttributeNone );
    iPreconditions->iDesiredRemoteSend = CONSTANT( iKAttributeNone );
    iPreconditions->iDesiredRemoteRecv = CONSTANT( iKAttributeNone );

    attribute = CSdpAttributeField::DecodeLC( _L8("a=des:x mandatory local sendrecv\r\n" ) );
    
    EUNIT_ASSERT( iPreconditions->DecodeDesiredStatusL( *attribute ) == 0 );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeNone ) );

    CleanupStack::PopAndDestroy( attribute );

    }
    
void UT_TMceSegmentedPreconditions::UT_TMceSegmentedPreconditions_DecodeConfStatusLL()
    {
    iPreconditions->iConfirmation = CONSTANT( iKAttributeNone );

    CSdpAttributeField* attribute = CSdpAttributeField::DecodeLC( _L8("a=conf:qos remote sendrecv\r\n" ) );
    
    EUNIT_ASSERT( iPreconditions->DecodeConfStatusL( *attribute ) == 1 );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeSendRecv ) );
    
    CleanupStack::PopAndDestroy( attribute );
    iPreconditions->iConfirmation = CONSTANT( iKAttributeNone );

     attribute = CSdpAttributeField::DecodeLC( _L8("a=conf:qos local sendrecv\r\n" ) );
    
    EUNIT_ASSERT( iPreconditions->DecodeConfStatusL( *attribute ) == 0 );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeNone ) );

    CleanupStack::PopAndDestroy( attribute );
    iPreconditions->iConfirmation = CONSTANT( iKAttributeNone );

    attribute = CSdpAttributeField::DecodeLC( _L8("a=conf:x local sendrecv\r\n" ) );
    
    EUNIT_ASSERT( iPreconditions->DecodeConfStatusL( *attribute ) == 0 );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeNone ) );

    CleanupStack::PopAndDestroy( attribute );
    }
    
void UT_TMceSegmentedPreconditions::UT_TMceSegmentedPreconditions_ParseCurrentStatusL()
    {

    TPtrC8 accessNetworkDesc;
    TPtrC8 directionDesc;
    
    EUNIT_ASSERT( iPreconditions->ParseCurrentStatus( _L8("qos remote sendrecv"),
                                                      accessNetworkDesc, directionDesc ) == KErrNone );
    
    EUNIT_ASSERT( accessNetworkDesc == CONSTANT( iKAttributeRemote ).DesC() );
    EUNIT_ASSERT( directionDesc == CONSTANT( iKAttributeSendRecv ).DesC() );

    EUNIT_ASSERT( iPreconditions->ParseCurrentStatus( _L8("qos local send trailing tokens"),
                                                      accessNetworkDesc, directionDesc ) == KErrNone );
    
    EUNIT_ASSERT( accessNetworkDesc == CONSTANT( iKAttributeLocal ).DesC() );
    EUNIT_ASSERT( directionDesc == CONSTANT( iKAttributeSend ).DesC() );


    EUNIT_ASSERT( iPreconditions->ParseCurrentStatus( _L8("x remote sendrecv"),
                                                      accessNetworkDesc, directionDesc ) == KErrNotFound );
    
    
    }
    
void UT_TMceSegmentedPreconditions::UT_TMceSegmentedPreconditions_ParseDesiredStatusL()
    {

    TPtrC8 strengthDesc; 
    TPtrC8 accessNetworkDesc;
    TPtrC8 directionDesc;
    
    
    EUNIT_ASSERT( iPreconditions->ParseDesiredStatus( _L8("qos optional remote sendrecv"),
                                                      strengthDesc, accessNetworkDesc, directionDesc ) == KErrNone );
    
    EUNIT_ASSERT( strengthDesc == CONSTANT( iKAttributeOptional ).DesC() );
    EUNIT_ASSERT( accessNetworkDesc == CONSTANT( iKAttributeRemote ).DesC() );
    EUNIT_ASSERT( directionDesc == CONSTANT( iKAttributeSendRecv ).DesC() );

    EUNIT_ASSERT( iPreconditions->ParseDesiredStatus( _L8("qos mandatory local send trailing tokens"),
                                                      strengthDesc, accessNetworkDesc, directionDesc ) == KErrNone );
    
    EUNIT_ASSERT( strengthDesc == CONSTANT( iKAttributeMandatory ).DesC() );
    EUNIT_ASSERT( accessNetworkDesc == CONSTANT( iKAttributeLocal ).DesC() );
    EUNIT_ASSERT( directionDesc == CONSTANT( iKAttributeSend ).DesC() );


    EUNIT_ASSERT( iPreconditions->ParseDesiredStatus( _L8("x mandatory remote sendrecv"),
                                                      strengthDesc, accessNetworkDesc, directionDesc ) == KErrNotFound );
    
    }
    
void UT_TMceSegmentedPreconditions::UT_TMceSegmentedPreconditions_DecodeMediaDirectionLL()
    {
    
    EUNIT_ASSERT( iPreconditions->DecodeMediaDirectionL( _L8( "send" ) ) == CONSTANT( iKAttributeRecv ) );
    EUNIT_ASSERT( iPreconditions->DecodeMediaDirectionL( _L8( "recv" ) ) == CONSTANT( iKAttributeSend ) );
    EUNIT_ASSERT( iPreconditions->DecodeMediaDirectionL( _L8( "sendrecv" ) ) == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT_LEAVE( iPreconditions->DecodeMediaDirectionL( _L8( "foo" ) ) );
    
    }
    
void UT_TMceSegmentedPreconditions::UT_TMceSegmentedPreconditions_DecodeStrengthLL()
    {
    EUNIT_ASSERT( iPreconditions->DecodeStrengthL( _L8( "mandatory" ) ) == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->DecodeStrengthL( _L8( "optional" ) ) == CONSTANT( iKAttributeOptional ) );
    EUNIT_ASSERT( iPreconditions->DecodeStrengthL( _L8( "none" ) ) == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->DecodeStrengthL( _L8( "failure" ) ) == CONSTANT( iKAttributeFailure ) );
    EUNIT_ASSERT( iPreconditions->DecodeStrengthL( _L8( "unknown" ) ) == CONSTANT( iKAttributeUnknown ) );
    EUNIT_ASSERT_LEAVE( iPreconditions->DecodeStrengthL( _L8( "foo" ) ) );
    }
    
void UT_TMceSegmentedPreconditions::UT_TMceSegmentedPreconditions_SetStateL()
    {
    EUNIT_ASSERT( !iPreconditions->IsMet() );
    
    iPreconditions->SetState();
    EUNIT_ASSERT( !iPreconditions->IsMet() );

    iPreconditions->iCurrentLocalStatus = CONSTANT( iKAttributeSendRecv );

    iPreconditions->SetState();
    EUNIT_ASSERT( !iPreconditions->IsMet() );
    
    iPreconditions->iCurrentRemoteStatus = CONSTANT( iKAttributeSendRecv );
    iPreconditions->SetState();
    EUNIT_ASSERT( iPreconditions->IsMet() );

    iPreconditions->iConfirmation = CONSTANT( iKAttributeSendRecv );
    iPreconditions->SetState();
    EUNIT_ASSERT( !iPreconditions->IsMet() );

    
    }
    
void UT_TMceSegmentedPreconditions::UT_TMceSegmentedPreconditions_SetRemoteStatusL()
    {
    
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    
    iPreconditions->SetRemoteStatus( CONSTANT( iKAttributeSend ) );

    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeSend ) );

    iPreconditions->SetRemoteStatus( CONSTANT( iKAttributeRecv ) );

    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeSend ) );

    iPreconditions->SetRemoteStatus( CONSTANT( iKAttributeSendRecv ) );

    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeSendRecv ) );
    
    }
    
void UT_TMceSegmentedPreconditions::UT_TMceSegmentedPreconditions_DesiredDirectionL()
    {
    iPreconditions->iDesiredLocalSend = CONSTANT( iKAttributeNone );
    iPreconditions->iDesiredLocalRecv = CONSTANT( iKAttributeNone );
    iPreconditions->iDesiredRemoteSend = CONSTANT( iKAttributeNone );
    iPreconditions->iDesiredRemoteRecv = CONSTANT( iKAttributeNone );
    
    EUNIT_ASSERT( iPreconditions->DesiredDirection( CONSTANT( iKAttributeLocal ) ) == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->DesiredDirection( CONSTANT( iKAttributeRemote ) ) == CONSTANT( iKAttributeNone ) );

    iPreconditions->iDesiredLocalSend = CONSTANT( iKAttributeMandatory );
    iPreconditions->iDesiredLocalRecv = CONSTANT( iKAttributeNone );
    iPreconditions->iDesiredRemoteSend = CONSTANT( iKAttributeMandatory );
    iPreconditions->iDesiredRemoteRecv = CONSTANT( iKAttributeNone );
    
    EUNIT_ASSERT( iPreconditions->DesiredDirection( CONSTANT( iKAttributeLocal ) ) == CONSTANT( iKAttributeSend ) );
    EUNIT_ASSERT( iPreconditions->DesiredDirection( CONSTANT( iKAttributeRemote ) ) == CONSTANT( iKAttributeSend ) );

    iPreconditions->iDesiredLocalSend = CONSTANT( iKAttributeNone );
    iPreconditions->iDesiredLocalRecv = CONSTANT( iKAttributeMandatory );
    iPreconditions->iDesiredRemoteSend = CONSTANT( iKAttributeNone );
    iPreconditions->iDesiredRemoteRecv = CONSTANT( iKAttributeMandatory );
    
    EUNIT_ASSERT( iPreconditions->DesiredDirection( CONSTANT( iKAttributeLocal ) ) == CONSTANT( iKAttributeRecv ) );
    EUNIT_ASSERT( iPreconditions->DesiredDirection( CONSTANT( iKAttributeRemote ) ) == CONSTANT( iKAttributeRecv ) );

    iPreconditions->iDesiredLocalSend = CONSTANT( iKAttributeMandatory );
    iPreconditions->iDesiredLocalRecv = CONSTANT( iKAttributeMandatory );
    iPreconditions->iDesiredRemoteSend = CONSTANT( iKAttributeMandatory );
    iPreconditions->iDesiredRemoteRecv = CONSTANT( iKAttributeMandatory );
    
    EUNIT_ASSERT( iPreconditions->DesiredDirection( CONSTANT( iKAttributeLocal ) ) == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->DesiredDirection( CONSTANT( iKAttributeRemote ) ) == CONSTANT( iKAttributeSendRecv ) );

    iPreconditions->iDesiredLocalSend = CONSTANT( iKAttributeMandatory );
    iPreconditions->iDesiredLocalRecv = CONSTANT( iKAttributeMandatory );
    iPreconditions->iDesiredRemoteSend = CONSTANT( iKAttributeOptional );
    iPreconditions->iDesiredRemoteRecv = CONSTANT( iKAttributeOptional );
    
    EUNIT_ASSERT( iPreconditions->DesiredDirection( CONSTANT( iKAttributeLocal ) ) == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->DesiredDirection( CONSTANT( iKAttributeRemote ) ) == CONSTANT( iKAttributeNone ) );
    
    }
    
void UT_TMceSegmentedPreconditions::UT_TMceSegmentedPreconditions_SetDesiredStatusL()
    {

    iPreconditions->SetDesiredStatus( CONSTANT( iKAttributeLocal ), 
                                      CONSTANT( iKAttributeSendRecv ), 
                                      CONSTANT( iKAttributeMandatory ) );
                                      
    
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeNone ) );

    iPreconditions->SetDesiredStatus( CONSTANT( iKAttributeRemote ), 
                                      CONSTANT( iKAttributeSendRecv ), 
                                      CONSTANT( iKAttributeOptional ) );
                                      
    
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeOptional ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeOptional ) );

    iPreconditions->SetDesiredStatus( CONSTANT( iKAttributeLocal ), 
                                      CONSTANT( iKAttributeSend ), 
                                      CONSTANT( iKAttributeNone ) );
                                      
    
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeOptional ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeOptional ) );

    iPreconditions->SetDesiredStatus( CONSTANT( iKAttributeRemote ), 
                                      CONSTANT( iKAttributeSend ), 
                                      CONSTANT( iKAttributeNone ) );
                                      
    
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeOptional ) );

    iPreconditions->SetDesiredStatus( CONSTANT( iKAttributeLocal ), 
                                      CONSTANT( iKAttributeRecv ), 
                                      CONSTANT( iKAttributeOptional ) );
                                      
    
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeOptional ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeOptional ) );

    iPreconditions->SetDesiredStatus( CONSTANT( iKAttributeRemote ), 
                                      CONSTANT( iKAttributeRecv ), 
                                      CONSTANT( iKAttributeFailure ) );
                                      
    
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSend == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalRecv == CONSTANT( iKAttributeOptional ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSend == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteRecv == CONSTANT( iKAttributeFailure ) );
    
    
    }

    

/**
* Test: Decode Ignores the Unknow QoS parameters
*
*/
void UT_TMceSegmentedPreconditions::UT_TMceSegmentedPreconditions_DecodeUnknownQoSLL()
    {
    
    iSdpDocument = CSdpDocument::DecodeL( KMceMMTestSdpUnKnownQoSParameter );
    
    RPointerArray< CSdpMediaField >& mediaLines = iSdpDocument->MediaFields();
    
    for( TInt i = 0;i<mediaLines.Count();i++)
        {
        TUint error = KErrNone;
        iPreconditions->DecodeL( *mediaLines[ i ] );
        }
    }




	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_TMceSegmentedPreconditions,
    "TMceSegmentedPreconditions tests",
    "UNIT" )


EUNIT_TEST(
    "DoConstructL test",
    "TMceSegmentedPreconditions",
    "DoConstructL",
    "FUNCTIONALITY",
    SetupL, UT_TMceSegmentedPreconditions_DoConstructLL, Teardown )

EUNIT_TEST(
    "EncodeL test",
    "TMceSegmentedPreconditions",
    "EncodeL",
    "FUNCTIONALITY",
    SetupL, UT_TMceSegmentedPreconditions_EncodeLL, Teardown )

EUNIT_TEST(
    "DecodeL test",
    "TMceSegmentedPreconditions",
    "DecodeL",
    "FUNCTIONALITY",
    SetupL, UT_TMceSegmentedPreconditions_DecodeLL, Teardown )
    
EUNIT_TEST(
    "Reserved test",
    "TMceSegmentedPreconditions",
    "Reserved",
    "FUNCTIONALITY",
    SetupL, UT_TMceSegmentedPreconditions_ReservedL, Teardown )
    
EUNIT_TEST(
    "CloneL test",
    "TMceSegmentedPreconditions",
    "CloneL",
    "FUNCTIONALITY",
    SetupL, UT_TMceSegmentedPreconditions_CloneLL, Teardown )
    
EUNIT_TEST(
    "EncodeCurrentStatusL test",
    "TMceSegmentedPreconditions",
    "EncodeCurrentStatusL",
    "FUNCTIONALITY",
    SetupL, UT_TMceSegmentedPreconditions_EncodeCurrentStatusLL, Teardown )
    
EUNIT_TEST(
    "EncodeDesiredStatusL test",
    "TMceSegmentedPreconditions",
    "EncodeDesiredStatusL",
    "FUNCTIONALITY",
    SetupL, UT_TMceSegmentedPreconditions_EncodeDesiredStatusLL, Teardown )
    
EUNIT_TEST(
    "EncodeConfirmationL test",
    "TMceSegmentedPreconditions",
    "EncodeConfirmationL",
    "FUNCTIONALITY",
    SetupL, UT_TMceSegmentedPreconditions_EncodeConfirmationLL, Teardown )
    
EUNIT_TEST(
    "CurrentStausTextLC test",
    "TMceSegmentedPreconditions",
    "CurrentStausTextLC",
    "FUNCTIONALITY",
    SetupL, UT_TMceSegmentedPreconditions_CurrentStausTextLCL, Teardown )
    
EUNIT_TEST(
    "DesiredStausTextLC test",
    "TMceSegmentedPreconditions",
    "DesiredStausTextLC",
    "FUNCTIONALITY",
    SetupL, UT_TMceSegmentedPreconditions_DesiredStausTextLCL, Teardown )
    
EUNIT_TEST(
    "DecodeCurrentStatusL test",
    "TMceSegmentedPreconditions",
    "DecodeCurrentStatusL",
    "FUNCTIONALITY",
    SetupL, UT_TMceSegmentedPreconditions_DecodeCurrentStatusLL, Teardown )
    
EUNIT_TEST(
    "DecodeDesiredStatusL test",
    "TMceSegmentedPreconditions",
    "DecodeDesiredStatusL",
    "FUNCTIONALITY",
    SetupL, UT_TMceSegmentedPreconditions_DecodeDesiredStatusLL, Teardown )
    
EUNIT_TEST(
    "DecodeConfStatusL test",
    "TMceSegmentedPreconditions",
    "DecodeConfStatusL",
    "FUNCTIONALITY",
    SetupL, UT_TMceSegmentedPreconditions_DecodeConfStatusLL, Teardown )
    
EUNIT_TEST(
    "ParseCurrentStatus test",
    "TMceSegmentedPreconditions",
    "ParseCurrentStatus",
    "FUNCTIONALITY",
    SetupL, UT_TMceSegmentedPreconditions_ParseCurrentStatusL, Teardown )
    
EUNIT_TEST(
    "ParseDesiredStatus test",
    "TMceSegmentedPreconditions",
    "ParseDesiredStatus",
    "FUNCTIONALITY",
    SetupL, UT_TMceSegmentedPreconditions_ParseDesiredStatusL, Teardown )
    
EUNIT_TEST(
    "DecodeMediaDirectionL test",
    "TMceSegmentedPreconditions",
    "DecodeMediaDirectionL",
    "FUNCTIONALITY",
    SetupL, UT_TMceSegmentedPreconditions_DecodeMediaDirectionLL, Teardown )
    
EUNIT_TEST(
    "DecodeStrengthL test",
    "TMceSegmentedPreconditions",
    "DecodeStrengthL",
    "FUNCTIONALITY",
    SetupL, UT_TMceSegmentedPreconditions_DecodeStrengthLL, Teardown )
    
EUNIT_TEST(
    "SetState test",
    "TMceSegmentedPreconditions",
    "SetState",
    "FUNCTIONALITY",
    SetupL, UT_TMceSegmentedPreconditions_SetStateL, Teardown )
    
EUNIT_TEST(
    "SetRemoteStatus test",
    "TMceSegmentedPreconditions",
    "SetRemoteStatus",
    "FUNCTIONALITY",
    SetupL, UT_TMceSegmentedPreconditions_SetRemoteStatusL, Teardown )
    
EUNIT_TEST(
    "DesiredDirection test",
    "TMceSegmentedPreconditions",
    "DesiredDirection",
    "FUNCTIONALITY",
    SetupL, UT_TMceSegmentedPreconditions_DesiredDirectionL, Teardown )
    
EUNIT_TEST(
    "SetDesiredStatus test",
    "TMceSegmentedPreconditions",
    "SetDesiredStatus",
    "FUNCTIONALITY",
    SetupL, UT_TMceSegmentedPreconditions_SetDesiredStatusL, Teardown )
    
EUNIT_TEST(
    "DecodeL - UnKnownQoStest",
    "TMceSrvStreamIterator",
    "DecodeL",
    "FUNCTIONALITY",
    SetupL, UT_TMceSegmentedPreconditions_DecodeUnknownQoSLL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
