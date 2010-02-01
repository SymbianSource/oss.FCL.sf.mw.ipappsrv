/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
#include "UT_tmcesecurepreconditions.h"

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
#define DEFINE_SECLINES()\
    CSdpAttributeField* sec1 = NULL;\
    CSdpAttributeField* sec2 = NULL;\
    CSdpAttributeField* sec3 = NULL;\
    CSdpAttributeField* sec4 = NULL;\
    CSdpAttributeField* sec5 = NULL;\
    CSdpAttributeField* sec6 = NULL

#define CREATE_SEC2( media, line1, line2 )\
    sec2 = CSdpAttributeField::DecodeLC( _L8( line2 ) );\
    sec1 = CSdpAttributeField::DecodeLC( _L8( line1 ) );\
    media->AttributeFields().AppendL( sec1 );\
    CleanupStack::Pop( sec1 );\
    media->AttributeFields().AppendL( sec2 );\
    CleanupStack::Pop( sec2 );
    
#define CREATE_SEC3( media, line1, line2, line3 )\
    sec3 = CSdpAttributeField::DecodeLC( _L8( line3 ) );\
    sec2 = CSdpAttributeField::DecodeLC( _L8( line2 ) );\
    sec1 = CSdpAttributeField::DecodeLC( _L8( line1 ) );\
    media->AttributeFields().AppendL( sec1 );\
    CleanupStack::Pop( sec1 );\
    media->AttributeFields().AppendL( sec2 );\
    CleanupStack::Pop( sec2 );\
    media->AttributeFields().AppendL( sec3 );\
    CleanupStack::Pop( sec3 );
        
#define CREATE_SEC4( media, line1, line2, line3, line4 )\
    sec4 = CSdpAttributeField::DecodeLC( _L8( line4 ) );\
    sec3 = CSdpAttributeField::DecodeLC( _L8( line3 ) );\
    sec2 = CSdpAttributeField::DecodeLC( _L8( line2 ) );\
    sec1 = CSdpAttributeField::DecodeLC( _L8( line1 ) );\
    media->AttributeFields().AppendL( sec1 );\
    CleanupStack::Pop( sec1 );\
    media->AttributeFields().AppendL( sec2 );\
    CleanupStack::Pop( sec2 );\
    media->AttributeFields().AppendL( sec3 );\
    CleanupStack::Pop( sec3 );\
    media->AttributeFields().AppendL( sec4 );\
    CleanupStack::Pop( sec4 );
    
#define CREATE_SEC5( media, line1, line2, line3, line4, line5 )\
    CREATE_SEC4( media, line1, line2, line3, line4 );\
    sec5 = CSdpAttributeField::DecodeLC( _L8( line5 ) );\
    media->AttributeFields().AppendL( sec5 );\
    CleanupStack::Pop( sec5 )

#define CREATE_SEC6( media, line1, line2, line3, line4, line5, line6 )\
    CREATE_SEC5( media, line1, line2, line3, line4, line5 );\
    sec6 = CSdpAttributeField::DecodeLC( _L8( line6 ) );\
    media->AttributeFields().AppendL( sec6 );\
    CleanupStack::Pop( sec6 )
    

#define ASSERT_SEC1( media, ind, attr1, line1 )\
    EUNIT_ASSERT_EQUALS( _L8( attr1 ), media->AttributeFields()[ind+0]->Attribute().DesC() );\
    EUNIT_ASSERT_EQUALS( _L8( line1 ), media->AttributeFields()[ind+0]->Value() )

#define ASSERT_SEC2( media, ind, attr1, line1, attr2, line2 )\
    ASSERT_SEC1( media, ind, attr1, line1 );\
    EUNIT_ASSERT_EQUALS( _L8( attr2 ), media->AttributeFields()[ind+1]->Attribute().DesC() );\
    EUNIT_ASSERT_EQUALS( _L8( line2 ), media->AttributeFields()[ind+1]->Value() );
    
#define ASSERT_SEC3( media, ind, attr1, line1, attr2, line2, attr3, line3 )\
    ASSERT_SEC2( media, ind, attr1, line1, attr2, line2 )\
    EUNIT_ASSERT( media->AttributeFields()[ind+2]->Attribute().DesC() == _L8( attr3 ) );\
    EUNIT_ASSERT( media->AttributeFields()[ind+2]->Value() == _L8( line3 ) );    
    
#define ASSERT_SEC4( media, ind, attr1, line1, attr2, line2, attr3, line3, attr4, line4 )\
    ASSERT_SEC2( media, ind, attr1, line1, attr2, line2 );\
    EUNIT_ASSERT( media->AttributeFields()[ind+2]->Attribute().DesC() == _L8( attr3 ) );\
    EUNIT_ASSERT( media->AttributeFields()[ind+2]->Value() == _L8( line3 ) );\
    EUNIT_ASSERT( media->AttributeFields()[ind+3]->Attribute().DesC() == _L8( attr4 ) );\
    EUNIT_ASSERT( media->AttributeFields()[ind+3]->Value() == _L8( line4 ) )

#define ASSERT_SEC5( media, ind, attr1, line1, attr2, line2, attr3, line3, attr4, line4, attr5, line5 )\
    ASSERT_SEC4( media, ind, attr1, line1, attr2, line2, attr3, line3, attr4, line4 );\
    EUNIT_ASSERT( media->AttributeFields()[ind+4]->Attribute().DesC() == _L8( attr5 ) );\
    EUNIT_ASSERT( media->AttributeFields()[ind+4]->Value() == _L8( line5 ) )

#define ASSERT_SEC6( media, ind, attr1, line1, attr2, line2, attr3, line3, attr4, line4, attr5, line5, attr6, line6 )\
    ASSERT_SEC5( media, ind, attr1, line1, attr2, line2, attr3, line3, attr4, line4, attr5, line5 );\
    EUNIT_ASSERT( media->AttributeFields()[ind+5]->Attribute().DesC() == _L8( attr6 ) );\
    EUNIT_ASSERT( media->AttributeFields()[ind+5]->Value() == _L8( line6 ) )

#define ASSERT_SEC7( media, ind, attr1, line1, attr2, line2, attr3, line3, attr4, line4, attr5, line5, attr6, line6, attr7, line7 )\
    ASSERT_SEC6( media, ind, attr1, line1, attr2, line2, attr3, line3, attr4, line4, attr5, line5, attr6, line6 );\
    EUNIT_ASSERT( media->AttributeFields()[ind+6]->Attribute().DesC() == _L8( attr7 ) );\
    EUNIT_ASSERT( media->AttributeFields()[ind+6]->Value() == _L8( line7 ) )


//  INTERNAL INCLUDES

// CONSTRUCTION
UT_TMceSecurePreconditions* UT_TMceSecurePreconditions::NewL()
    {
    UT_TMceSecurePreconditions* self = UT_TMceSecurePreconditions::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_TMceSecurePreconditions* UT_TMceSecurePreconditions::NewLC()
    {
    UT_TMceSecurePreconditions* self = new( ELeave ) UT_TMceSecurePreconditions();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_TMceSecurePreconditions::~UT_TMceSecurePreconditions()
    {
    }

// Default constructor
UT_TMceSecurePreconditions::UT_TMceSecurePreconditions()
    {
    }

// Second phase construct
void UT_TMceSecurePreconditions::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_TMceSecurePreconditions::SetupL()
    {
    iServer = new (ELeave) CMceServerStub();
    iManager = CMceMediaManager::NewL( *iServer );
    iSession = MceMediaManagerTestHelper::CreateMultimediaOutSessionL();
    iSession->SetObserver( *iServer );
    iManager->CreateSessionL( *iSession );
    
    iPreconditions = new (ELeave) TMceSecurePreconditions( *(iSession->Streams()[0]) );
    iPreconditions->ConstructL();
        
    } 

void UT_TMceSecurePreconditions::Teardown()
    {
    if ( iSdpDocument )
    	{
    	delete iSdpDocument;
    	}
    iSdpDocument = NULL;
	CMceComSession::Delete( iSession, *iManager );
	delete iManager;
	iManager = NULL;
	delete iServer;
	iServer = NULL;
	delete iPreconditions;
    }


void UT_TMceSecurePreconditions::UT_TMceSecurePreconditions_DoConstructLL()
    {
    
    EUNIT_ASSERT( iPreconditions->iStream.Direction() == SdpCodecStringConstants::EAttributeSendrecv );
    
    iPreconditions->DoConstructL();
    
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSendRecv == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ECreated );
    
    iPreconditions->iStream.Enable( EFalse, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( EFalse, ETrue );
    
    EUNIT_ASSERT( iPreconditions->iStream.Direction() == SdpCodecStringConstants::EAttributeInactive );
    
    iPreconditions->DoConstructL();
    
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSendRecv == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ECreated );

    iPreconditions->iStream.Enable( ETrue, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( EFalse, ETrue );
    
    EUNIT_ASSERT( iPreconditions->iStream.Direction() == SdpCodecStringConstants::EAttributeRecvonly );
    
    iPreconditions->DoConstructL();
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_PRINT( iPreconditions->iDesiredRemoteSendRecv.DesC() );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSendRecv == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ECreated );

    iPreconditions->iStream.Enable( EFalse, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( ETrue, ETrue );
    
    EUNIT_ASSERT( iPreconditions->iStream.Direction() == SdpCodecStringConstants::EAttributeSendonly );
    
    iPreconditions->DoConstructL();
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSendRecv == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ECreated );
    
    }

    
void UT_TMceSecurePreconditions::UT_TMceSecurePreconditions_EncodeLL()
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
                            ASSERT_SEC2( audioLine, 0,
                                         "curr", "sec e2e none",
                                         "des" , "sec optional e2e sendrecv");
                            }
                        else if ( variant == KReserved )
                            {
                            ASSERT_SEC2( audioLine, 0,
                                         "curr", "sec e2e sendrecv",
                                         "des" , "sec optional e2e sendrecv" );
                            }
                        break;
                        }
                    case KInactive:
                        {
                        if ( variant == KNotReserved )
                            {
                            ASSERT_SEC2( audioLine, 0,
                                         "curr", "sec e2e none",
                                         "des" , "sec optional e2e sendrecv" );
                            }
                        else if ( variant == KReserved )
                            {
                            ASSERT_SEC2( audioLine, 0,
                                         "curr", "sec e2e sendrecv",
                                         "des" , "sec optional e2e sendrecv" );
                            }
                        break;
                        }
                    case KSendOnly:
                        {
                        if ( variant == KNotReserved )
                            {
                            ASSERT_SEC2( audioLine, 0,
                                         "curr", "sec e2e none",
                                         "des" , "sec optional e2e sendrecv" );
                            }
                        else if ( variant == KReserved )
                            {
                            ASSERT_SEC2( audioLine, 0,
                                         "curr", "sec e2e sendrecv",
                                         "des" , "sec optional e2e sendrecv" );
                            }
                        break;
                        }
                    case KRecvOnly:
                        {
                        if ( variant == KNotReserved )
                            {
                            ASSERT_SEC2( audioLine, 0,
                                         "curr", "sec e2e none",
                                         "des" , "sec optional e2e sendrecv" );
                            }
                        else if ( variant == KReserved )
                            {
                            ASSERT_SEC2( audioLine, 0,
                                         "curr", "sec e2e sendrecv",
                                         "des" , "sec optional e2e sendrecv" );
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
                    iPreconditions->iDesiredRemoteSendRecv = CONSTANT( iKAttributeMandatory );
                    
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
                    iPreconditions->iDesiredRemoteSendRecv = CONSTANT( iKAttributeMandatory );
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
                    iPreconditions->iDesiredRemoteSendRecv = CONSTANT( iKAttributeMandatory );
                    if ( variant == KReserved )
                        {
                        iPreconditions->iCurrentRemoteStatus = CONSTANT( iKAttributeSendRecv );
                        }
                    EUNIT_ASSERT( iPreconditions->iStream.Direction() == SdpCodecStringConstants::EAttributeSendonly );
                    break;
                    }
                case KRecvOnly:
                    {
                    iPreconditions->iStream.Enable( ETrue, ETrue );
                    iPreconditions->iStream.iLinkedStream->Enable( EFalse, ETrue );
                    iPreconditions->DoConstructL();
                    iPreconditions->iDesiredRemoteSendRecv = CONSTANT( iKAttributeMandatory );
                    if ( variant == KReserved )
                        {
                        iPreconditions->iCurrentRemoteStatus = CONSTANT( iKAttributeSendRecv );
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
                
                switch( casendx )
                    {
                    case KSendRecv:
                    case KSendOnly:
                    case KRecvOnly:
                        {
                        if ( variant == KNotReserved )
                            {
                            
                            
                            iPreconditions->Reserved();
                            iPreconditions->iCurrentLocalStatus = CONSTANT( iKAttributeNone );
			                iPreconditions->EncodeL( *audioLine );
            			    EUNIT_PRINT ( iPreconditions->iCurrentLocalStatus.DesC());
                			EUNIT_PRINT ( iPreconditions->iCurrentRemoteStatus.DesC());

                            EUNIT_PRINT( iPreconditions->iConfirmation.DesC());
                            
                            ASSERT_SEC3( audioLine, 0,
                                         "curr" , "sec e2e recv",
                                         "des"  , "sec mandatory e2e sendrecv",
                                         "conf" , "sec e2e sendrecv" );
                            EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ENegotiating );
                                         
                            }
                        else if ( variant == KReserved )
                            {
                            iPreconditions->Reserved();
                			iPreconditions->EncodeL( *audioLine );
                			EUNIT_PRINT ( iPreconditions->iCurrentLocalStatus.DesC());
                			EUNIT_PRINT ( iPreconditions->iCurrentRemoteStatus.DesC());

                            ASSERT_SEC2( audioLine, 0,
                                         "curr" , "sec e2e sendrecv",
                                         "des"  , "sec mandatory e2e sendrecv" );
                            EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::EReserved );
                            }
                        break;
                        }
                    case KInactive:
                        {
                        if ( variant == KNotReserved )
                            {
                            iPreconditions->Reserved();
                            iPreconditions->iCurrentLocalStatus = CONSTANT( iKAttributeNone );
                			iPreconditions->EncodeL( *audioLine );
                			EUNIT_PRINT ( iPreconditions->iCurrentLocalStatus.DesC());
                			EUNIT_PRINT ( iPreconditions->iCurrentRemoteStatus.DesC());

                             ASSERT_SEC3( audioLine, 0,
                                         "curr" , "sec e2e recv",
                                         "des"  , "sec mandatory e2e sendrecv",
                                         "conf" , "sec e2e sendrecv" );
                            EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ENegotiating );
                            }
                        else if ( variant == KReserved )
                            {
                            iPreconditions->Reserved();
                			iPreconditions->EncodeL( *audioLine );
                			EUNIT_PRINT ( iPreconditions->iCurrentLocalStatus.DesC());
                			EUNIT_PRINT ( iPreconditions->iCurrentRemoteStatus.DesC());

                            ASSERT_SEC2( audioLine, 0,
                                         "curr" , "sec e2e sendrecv",
                                         "des"  , "sec mandatory e2e sendrecv" );
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
    

    audioLine->AttributeFields().ResetAndDestroy();
    CleanupStack::PopAndDestroy( audioLine );
    
    }
    
    
void UT_TMceSecurePreconditions::UT_TMceSecurePreconditions_DecodeLL()
    {
    RStringF audio = MCE_SDP_STRING_AUDIOL();
    RStringF protocol = SDP_STRINGL( SdpCodecStringConstants::EProtocolRtpAvp );
    
    CSdpMediaField* audioLine = 
        CSdpMediaField::NewLC( audio, 5005, protocol, _L8("0") );

    iPreconditions->iStream.Session()->NegotiationState().SetRole( EMceRoleAnswerer );

//no sec lines
    iPreconditions->iState = TMcePreconditions::ECreated;
    iPreconditions->iStream.Enable( ETrue, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( ETrue, ETrue );
    iPreconditions->DoConstructL();
    iPreconditions->Reserved();

    iPreconditions->DecodeL( *audioLine );
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSendRecv == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeNone ) );
        
    // Ensure if other party does not include secure precondition
    // Its assumes that preconditions are met.
	EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::EReserved );
    
    DEFINE_SECLINES();

//answerer reserved, offerer not reserved
    iPreconditions->iState = TMcePreconditions::ECreated;
    iPreconditions->iStream.Enable( ETrue, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( ETrue, ETrue );
    iPreconditions->DoConstructL();
    iPreconditions->Reserved();
    
    CREATE_SEC2( audioLine,
                 "a=curr:sec e2e none\r\n",
                 "a=des:sec mandatory e2e sendrecv\r\n");
                 
    
    iPreconditions->DecodeL( *audioLine );
    
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_PRINT( iPreconditions->iDesiredRemoteSendRecv.DesC() );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSendRecv == CONSTANT( iKAttributeFailure ) );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ECreated );

//answerer not reserved, offerer not reserved
    iPreconditions->iState = TMcePreconditions::ECreated;
    iPreconditions->iStream.Enable( ETrue, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( ETrue, ETrue );
    iPreconditions->DoConstructL();
    
    CREATE_SEC2( audioLine,
                 "a=curr:sec e2e none\r\n",
                 "a=des:sec mandatory e2e sendrecv\r\n" );
    
    iPreconditions->DecodeL( *audioLine );
    
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_PRINT( iPreconditions->iDesiredRemoteSendRecv.DesC());
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSendRecv == CONSTANT( iKAttributeFailure ) );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ECreated );

//answerer reserved, offerer not reserved
    iPreconditions->iState = TMcePreconditions::ECreated;
    iPreconditions->iStream.Enable( ETrue, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( EFalse, ETrue );
    iPreconditions->DoConstructL();
    iPreconditions->Reserved();
    
    CREATE_SEC2( audioLine,
                 "a=curr:sec e2e none\r\n",
                 "a=des:sec mandatory e2e send\r\n");
                 
    
    iPreconditions->DecodeL( *audioLine );
    
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
   	EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSendRecv == CONSTANT( iKAttributeFailure ) );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ECreated );

//answerer not reserved, offerer reserved
    iPreconditions->iState = TMcePreconditions::ECreated;
    iPreconditions->iStream.Enable( ETrue, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( EFalse, ETrue );
    iPreconditions->DoConstructL();
    
    CREATE_SEC2( audioLine,
                 "a=curr:sec e2e none\r\n",
                 "a=des:sec mandatory e2e send\r\n");
    
    iPreconditions->DecodeL( *audioLine );
    
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSendRecv == CONSTANT( iKAttributeFailure ) );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ECreated );



    iPreconditions->iStream.Session()->NegotiationState().SetRole( EMceRoleOfferer );


//offerer not reserved, answerer not reserved
    audioLine->AttributeFields().ResetAndDestroy();
    iPreconditions->iState = TMcePreconditions::ENegotiating;
    iPreconditions->iStream.Enable( ETrue, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( ETrue, ETrue );
    iPreconditions->DoConstructL();
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_PRINT( iPreconditions->iDesiredLocalSendRecv.DesC() );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSendRecv == CONSTANT( iKAttributeNone ) );
    

    CREATE_SEC3( audioLine,
                 "a=curr:sec e2e recv\r\n",
                 "a=des:sec mandatory e2e sendrecv\r\n",
                 "a=conf:sec e2e sendrecv\r\n");
    
    iPreconditions->DecodeL( *audioLine );
    
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeSend ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSendRecv == CONSTANT( iKAttributeFailure ) );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ECreated );

//offerer reserved, answerer not reserved
    audioLine->AttributeFields().ResetAndDestroy();
    iPreconditions->iState = TMcePreconditions::ENegotiating;
    iPreconditions->iStream.Enable( ETrue, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( ETrue, ETrue );
    iPreconditions->DoConstructL();
    iPreconditions->Reserved();
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSendRecv == CONSTANT( iKAttributeNone ) );
    

    CREATE_SEC2( audioLine,
                 "a=curr:sec e2e none\r\n",
                 "a=des:sec mandatory e2e sendrecv\r\n");

    
    iPreconditions->DecodeL( *audioLine );
    
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_PRINT( iPreconditions->iDesiredRemoteSendRecv.DesC() );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSendRecv == CONSTANT( iKAttributeFailure ) );
     
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ECreated );




//offerer: e2e reserved + conf
    audioLine->AttributeFields().ResetAndDestroy();
    iPreconditions->iState = TMcePreconditions::EReserved;
    iPreconditions->iConfirmation = CONSTANT( iKAttributeNone );

    CREATE_SEC3( audioLine,
                 "a=curr:sec e2e recv\r\n",
                 "a=des:sec mandatory e2e sendrecv\r\n",
                 "a=conf:sec e2e sendrecv\r\n");
    
    iPreconditions->DecodeL( *audioLine );
    
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeSendRecv ) );
	EUNIT_PRINT( iPreconditions->iCurrentRemoteStatus.DesC() );
   
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeSend ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_PRINT( iPreconditions->iDesiredRemoteSendRecv.DesC() );
    
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSendRecv == CONSTANT( iKAttributeFailure ) );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ECreated );

//offerer: e2e reserved no conf
    audioLine->AttributeFields().ResetAndDestroy();
    iPreconditions->iState = TMcePreconditions::EReserved;
    iPreconditions->iConfirmation = CONSTANT( iKAttributeNone );

    CREATE_SEC2( audioLine,
                  "a=curr:sec e2e sendrecv\r\n",
                 "a=des:sec mandatory e2e sendrecv\r\n");
    
    iPreconditions->DecodeL( *audioLine );
    
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_PRINT( iPreconditions->iDesiredRemoteSendRecv.DesC() );
    
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSendRecv == CONSTANT( iKAttributeFailure ) );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iState == TMcePreconditions::ECreated );

    CleanupStack::PopAndDestroy( audioLine );
    
    
    }
    
void UT_TMceSecurePreconditions::UT_TMceSecurePreconditions_ReservedL()
    {
    EUNIT_ASSERT( iPreconditions->iStream.Direction() == SdpCodecStringConstants::EAttributeSendrecv );
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeMandatory ) );
   
    iPreconditions->Reserved();

    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeMandatory ) );
    
    iPreconditions->iStream.Enable( EFalse, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( EFalse, ETrue );
    iPreconditions->DoConstructL();
    
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeMandatory ) );
    
    iPreconditions->Reserved();
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeMandatory ) );
    
    iPreconditions->iStream.Enable( ETrue, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( EFalse, ETrue );
    iPreconditions->DoConstructL();
    
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeMandatory ) );
    
    iPreconditions->Reserved();
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeMandatory ) );
    
    iPreconditions->iStream.Enable( EFalse, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( ETrue, ETrue );
    iPreconditions->DoConstructL();
    
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeMandatory ) );
    
    iPreconditions->Reserved();
    EUNIT_PRINT( iPreconditions->iCurrentLocalStatus.DesC() );
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeMandatory ) );
    
    }
    
void UT_TMceSecurePreconditions::UT_TMceSecurePreconditions_CloneLL()
    {

    iPreconditions->iStream.Enable( ETrue, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( EFalse, ETrue );
    iPreconditions->DoConstructL();
    iPreconditions->Reserved();
    
    TMceSecurePreconditions* clone = 
        static_cast<TMceSecurePreconditions*>( iPreconditions->CloneL( iPreconditions->iStream ) );
    
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
    EUNIT_ASSERT( clone->iDesiredLocalSendRecv == iPreconditions->iDesiredLocalSendRecv );
    EUNIT_ASSERT( clone->iDesiredRemoteSendRecv == iPreconditions->iDesiredRemoteSendRecv );
    EUNIT_ASSERT( clone->iConfirmation == iPreconditions->iConfirmation );
    EUNIT_ASSERT( clone->iState == iPreconditions->iState );
    
    CleanupStack::PopAndDestroy( clone );
    
    }
    
void UT_TMceSecurePreconditions::UT_TMceSecurePreconditions_EncodeCurrentStatusLL()
    {
    RStringF audio = MCE_SDP_STRING_AUDIOL();
    RStringF rtpavp = SDP_STRINGL( SdpCodecStringConstants::EProtocolRtpAvp );
    _LIT8( KPayloadType, "0" );   

    // Role offerer, local status recv, remote status send -> sendrecv in attribute
    CSdpMediaField* audioLine = 
        CSdpMediaField::NewLC( audio, 5005, rtpavp, KPayloadType );
    iPreconditions->iCurrentLocalStatus = iPreconditions->iKAttributeRecv;
    iPreconditions->iCurrentRemoteStatus = iPreconditions->iKAttributeSend;
    iPreconditions->EncodeCurrentStatusL( *audioLine, EMceRoleOfferer );
    ASSERT_SEC1( audioLine, 0, "curr", "sec e2e sendrecv" )
    CleanupStack::PopAndDestroy( audioLine ); 

    // Role offerer, local status sendrecv, remote status send -> sendrecv in attribute
    audioLine = CSdpMediaField::NewLC( audio, 5005, rtpavp, KPayloadType );
    iPreconditions->iCurrentLocalStatus = iPreconditions->iKAttributeSendRecv;
    iPreconditions->iCurrentRemoteStatus = iPreconditions->iKAttributeSend;
    iPreconditions->EncodeCurrentStatusL( *audioLine, EMceRoleOfferer );
    ASSERT_SEC1( audioLine, 0, "curr", "sec e2e sendrecv" )
    CleanupStack::PopAndDestroy( audioLine ); 

    // Role offerer, local status recv, remote status none -> sendrecv in attribute
    audioLine = CSdpMediaField::NewLC( audio, 5005, rtpavp, KPayloadType );
    iPreconditions->iCurrentLocalStatus = iPreconditions->iKAttributeRecv;
    iPreconditions->iCurrentRemoteStatus = iPreconditions->iKAttributeNone;
    iPreconditions->EncodeCurrentStatusL( *audioLine, EMceRoleOfferer );
    ASSERT_SEC1( audioLine, 0, "curr", "sec e2e sendrecv" )
    CleanupStack::PopAndDestroy( audioLine );
    
    // Role offerer, local status sendrecv, remote status none -> sendrecv in attribute
    audioLine = CSdpMediaField::NewLC( audio, 5005, rtpavp, KPayloadType );
    iPreconditions->iCurrentLocalStatus = iPreconditions->iKAttributeSendRecv;
    iPreconditions->iCurrentRemoteStatus = iPreconditions->iKAttributeNone;
    iPreconditions->EncodeCurrentStatusL( *audioLine, EMceRoleOfferer );
    ASSERT_SEC1( audioLine, 0, "curr", "sec e2e sendrecv" )
    CleanupStack::PopAndDestroy( audioLine );         
    
    // Role answerer, local resources not reserved -> recv in attribute
    audioLine = CSdpMediaField::NewLC( audio, 5005, rtpavp, KPayloadType );
    iPreconditions->iCurrentLocalStatus = iPreconditions->iKAttributeNone;
    iPreconditions->iCurrentRemoteStatus = iPreconditions->iKAttributeNone;
    iPreconditions->EncodeCurrentStatusL( *audioLine, EMceRoleAnswerer );
    ASSERT_SEC1( audioLine, 0, "curr", "sec e2e recv" )
    EUNIT_ASSERT( iPreconditions->iKAttributeRecv == iPreconditions->iCurrentLocalStatus )
    CleanupStack::PopAndDestroy( audioLine );

    // Role answerer, local resources reserved, remote not -> none in attribute
    audioLine = CSdpMediaField::NewLC( audio, 5005, rtpavp, KPayloadType );
    iPreconditions->iCurrentLocalStatus = iPreconditions->iKAttributeSendRecv;
    iPreconditions->iCurrentRemoteStatus = iPreconditions->iKAttributeNone;
    iPreconditions->EncodeCurrentStatusL( *audioLine, EMceRoleAnswerer );
    ASSERT_SEC1( audioLine, 0, "curr", "sec e2e none" )
    EUNIT_ASSERT_EQUALS( TMcePreconditions::ECreated, iPreconditions->iState  )
    CleanupStack::PopAndDestroy( audioLine );
    
    // Role answerer, local resources reserved, remote reserved -> sendrecv in attribute
    audioLine = CSdpMediaField::NewLC( audio, 5005, rtpavp, KPayloadType );
    iPreconditions->iCurrentLocalStatus = iPreconditions->iKAttributeSendRecv;
    iPreconditions->iCurrentRemoteStatus = iPreconditions->iKAttributeSend;
    iPreconditions->EncodeCurrentStatusL( *audioLine, EMceRoleAnswerer );
    ASSERT_SEC1( audioLine, 0, "curr", "sec e2e sendrecv" )
    EUNIT_ASSERT_EQUALS( TMcePreconditions::EReserved, iPreconditions->iState  )
    CleanupStack::PopAndDestroy( audioLine );     
    }
    
void UT_TMceSecurePreconditions::UT_TMceSecurePreconditions_EncodeDesiredStatusLL()
    {
    RStringF audio = MCE_SDP_STRING_AUDIOL();
    RStringF protocol = SDP_STRINGL( SdpCodecStringConstants::EProtocolRtpAvp );
    
    CSdpMediaField* audioLine = 
        CSdpMediaField::NewLC( audio, 5005, protocol, _L8("0") );

    iPreconditions->iStream.Enable( ETrue, ETrue );
    iPreconditions->iStream.iLinkedStream->Enable( ETrue, ETrue );
    EUNIT_ASSERT( iPreconditions->iStream.Direction() == SdpCodecStringConstants::EAttributeSendrecv );

    audioLine->AttributeFields().ResetAndDestroy();
    iPreconditions->iDesiredLocalSendRecv = CONSTANT( iKAttributeMandatory );
    iPreconditions->iDesiredRemoteSendRecv = CONSTANT( iKAttributeMandatory );
    
    iPreconditions->EncodeDesiredStatusL( *audioLine );

    ASSERT_SEC1( audioLine, 0,
                 "des" , "sec mandatory e2e sendrecv" );

                 
    audioLine->AttributeFields().ResetAndDestroy();
    iPreconditions->iDesiredLocalSendRecv = CONSTANT( iKAttributeNone );
    
    iPreconditions->EncodeDesiredStatusL( *audioLine );

    ASSERT_SEC1( audioLine, 0,
                 "des" , "sec mandatory e2e sendrecv" );
                 

    
    audioLine->AttributeFields().ResetAndDestroy();
    
    iPreconditions->EncodeDesiredStatusL( *audioLine );

    ASSERT_SEC1( audioLine, 0,
                 "des" , "sec mandatory e2e sendrecv" );


    audioLine->AttributeFields().ResetAndDestroy();
    iPreconditions->iDesiredRemoteSendRecv = CONSTANT( iKAttributeMandatory );
    
    iPreconditions->EncodeDesiredStatusL( *audioLine );
    
    ASSERT_SEC1( audioLine, 0,
                 "des" , "sec mandatory e2e sendrecv");

    audioLine->AttributeFields().ResetAndDestroy();
    iPreconditions->iDesiredRemoteSendRecv = CONSTANT( iKAttributeOptional );
    
    iPreconditions->EncodeDesiredStatusL( *audioLine );
    
    ASSERT_SEC1( audioLine, 0,
                 "des" , "sec mandatory e2e sendrecv" );

    audioLine->AttributeFields().ResetAndDestroy();
    iPreconditions->iDesiredRemoteSendRecv = CONSTANT( iKAttributeFailure );
    
    iPreconditions->EncodeDesiredStatusL( *audioLine );
    
    ASSERT_SEC1( audioLine, 0,
                 "des" , "sec failure e2e sendrecv" );

    audioLine->AttributeFields().ResetAndDestroy();
    iPreconditions->iDesiredRemoteSendRecv = CONSTANT( iKAttributeUnknown );
    
    iPreconditions->EncodeDesiredStatusL( *audioLine );
    
    ASSERT_SEC1( audioLine, 0,
                 "des" , "sec mandatory e2e sendrecv" );

    CleanupStack::PopAndDestroy( audioLine );

    }
    
void UT_TMceSecurePreconditions::UT_TMceSecurePreconditions_EncodeConfirmationLL()
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

    iPreconditions->iConfirmation = CONSTANT( iKAttributeSendRecv );
    iPreconditions->EncodeConfirmationL( *audioLine );

    ASSERT_SEC1( audioLine, 0,
                 "conf" , "sec e2e sendrecv" );
    
    CleanupStack::PopAndDestroy( audioLine );
    }
    
void UT_TMceSecurePreconditions::UT_TMceSecurePreconditions_CurrentStatusTextLCL()
    {
    HBufC8* text = 
        iPreconditions->CurrentStatusTextLC( 
            CONSTANT( iKAttributeSendRecv ).DesC() );
                                                       
    EUNIT_ASSERT_EQUALS( _L8("sec e2e sendrecv"), *text )
    
    CleanupStack::PopAndDestroy( text );                                               
    }
    
void UT_TMceSecurePreconditions::UT_TMceSecurePreconditions_DesiredStatusTextLCL()
    {
    HBufC8* text = 
        iPreconditions->DesiredStatusTextLC( 
            CONSTANT( iKAttributeMandatory ).DesC(), 
            CONSTANT( iKAttributeSendRecv ).DesC() );
                                                       
    EUNIT_ASSERT_EQUALS( _L8("sec mandatory e2e sendrecv"), *text )
    
    CleanupStack::PopAndDestroy( text );
    }
    
void UT_TMceSecurePreconditions::UT_TMceSecurePreconditions_DecodeCurrentStatusLL()
    {
    iPreconditions->iCurrentLocalStatus = CONSTANT( iKAttributeNone );
    iPreconditions->iCurrentRemoteStatus = CONSTANT( iKAttributeNone );

    CSdpAttributeField* attribute = 
        CSdpAttributeField::DecodeLC( _L8("a=curr:sec e2e sendrecv\r\n" ) );
    
    EUNIT_ASSERT( iPreconditions->DecodeCurrentStatusL( *attribute ) == 1 );
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeSendRecv ) );
    
    CleanupStack::PopAndDestroy( attribute );
    iPreconditions->iCurrentLocalStatus = CONSTANT( iKAttributeNone );
    iPreconditions->iCurrentRemoteStatus = CONSTANT( iKAttributeNone );

     attribute = CSdpAttributeField::DecodeLC( _L8("a=curr:sec e2e sendrecv\r\n" ) );
    
    EUNIT_ASSERT( iPreconditions->DecodeCurrentStatusL( *attribute ) == 1 );
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeSendRecv ) );

    CleanupStack::PopAndDestroy( attribute );
    iPreconditions->iCurrentLocalStatus = CONSTANT( iKAttributeNone );
    iPreconditions->iCurrentRemoteStatus = CONSTANT( iKAttributeNone );

    attribute = CSdpAttributeField::DecodeLC( _L8("a=curr:x e2e sendrecv\r\n" ) );
    
    EUNIT_ASSERT( iPreconditions->DecodeCurrentStatusL( *attribute ) == 0 );
    EUNIT_ASSERT( iPreconditions->iCurrentLocalStatus == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );

    CleanupStack::PopAndDestroy( attribute ); 
    }
    
void UT_TMceSecurePreconditions::UT_TMceSecurePreconditions_DecodeDesiredStatusLL()
    {
    //Test 1
    iPreconditions->iDesiredLocalSendRecv = CONSTANT( iKAttributeNone );
    iPreconditions->iDesiredRemoteSendRecv = CONSTANT( iKAttributeNone );

    CSdpAttributeField* attribute = CSdpAttributeField::DecodeLC( _L8("a=des:sec mandatory e2e sendrecv\r\n" ) );
    
    EUNIT_ASSERT( iPreconditions->DecodeDesiredStatusL( *attribute ) == KMceSipPreconditionFailure );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSendRecv == CONSTANT( iKAttributeFailure ) );
    
    CleanupStack::PopAndDestroy( attribute );
    
    //Test 2
     iPreconditions->iDesiredLocalSendRecv = CONSTANT( iKAttributeNone );
    iPreconditions->iDesiredRemoteSendRecv = CONSTANT( iKAttributeNone );

    attribute = CSdpAttributeField::DecodeLC( _L8("a=des:sec mandatory e2e sendrecv\r\n" ) );
    iPreconditions->iStream.Session()->iClientCryptoSuites.Append( EAES_CM_128_HMAC_SHA1_32 );
//    EUNIT_ASSERT( iPreconditions->DecodeDesiredStatusL( *attribute ) == KMceSipPreconditionFailure);
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeNone ) );
//    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSendRecv == CONSTANT( iKAttributeMandatory ) );
    iPreconditions->iStream.Session()->iClientCryptoSuites.Reset();
    CleanupStack::PopAndDestroy( attribute );
    
    // Test 3
    
    iPreconditions->iDesiredLocalSendRecv = CONSTANT( iKAttributeNone );
    iPreconditions->iDesiredRemoteSendRecv = CONSTANT( iKAttributeNone );

    attribute = CSdpAttributeField::DecodeLC( _L8("a=des:x mandatory e2e sendrecv\r\n" ) );
    
    EUNIT_ASSERT( iPreconditions->DecodeDesiredStatusL( *attribute ) == 0 );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSendRecv == CONSTANT( iKAttributeNone ) );

    CleanupStack::PopAndDestroy( attribute );

	// Test 4
 	iPreconditions->iDesiredLocalSendRecv = CONSTANT( iKAttributeNone );
    iPreconditions->iDesiredRemoteSendRecv = CONSTANT( iKAttributeNone );

    attribute = CSdpAttributeField::DecodeLC( _L8("a=des:sec optional e2e sendrecv\r\n" ) );
    EUNIT_ASSERT( iPreconditions->DecodeDesiredStatusL( *attribute ) == 1 );
    EUNIT_ASSERT( iPreconditions->iStream.Session()->Modifier( KMceSecPreconditions ) == 
    								KMcePreconditionsNotUsed );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSendRecv == CONSTANT( iKAttributeOptional ) );
    
    CleanupStack::PopAndDestroy( attribute );

	// Test 5
	iPreconditions->iDesiredLocalSendRecv = CONSTANT( iKAttributeNone );
    iPreconditions->iDesiredRemoteSendRecv = CONSTANT( iKAttributeNone );
    attribute = CSdpAttributeField::DecodeLC( _L8("a=des:sec optional e2e sendrecv\r\n" ) );
    iPreconditions->iStream.Session()->iClientCryptoSuites.Append( EAES_CM_128_HMAC_SHA1_32 );
    EUNIT_ASSERT( iPreconditions->DecodeDesiredStatusL( *attribute ) == 1 );
/*    EUNIT_ASSERT( iPreconditions->iStream.Session()->Modifier( KMceSecPreconditions ) 
	        	== KMcePreconditionsE2ESupported );
	EUNIT_ASSERT( iPreconditions->iStream.Session()->Modifier( KMcePreconditions ) 
	        	== KMcePreconditionsNotUsed );
	EUNIT_ASSERT( iPreconditions->iStream.Session()->iRemoteSecPreconditionsRequired == ETrue );
*/    
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSendRecv == CONSTANT( iKAttributeOptional ) );
    iPreconditions->iStream.Session()->iClientCryptoSuites.Reset();
    CleanupStack::PopAndDestroy( attribute );        
    
    // Test 6
    iPreconditions->iDesiredLocalSendRecv = CONSTANT( iKAttributeNone );
    iPreconditions->iDesiredRemoteSendRecv = CONSTANT( iKAttributeNone );
    attribute = CSdpAttributeField::DecodeLC( _L8("a=des:sec failure e2e sendrecv\r\n" ) );
    iPreconditions->iStream.Session()->iClientCryptoSuites.Append( EAES_CM_128_HMAC_SHA1_32 );
    EUNIT_ASSERT( iPreconditions->DecodeDesiredStatusL( *attribute ) == KMceSipPreconditionFailure );
    
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSendRecv == CONSTANT( iKAttributeFailure ) );
    iPreconditions->iStream.Session()->iClientCryptoSuites.Reset();
    CleanupStack::PopAndDestroy( attribute );        		

    // Test 7
    // Decoding 200 to PRACK
    iPreconditions->iCurrentLocalStatus = CONSTANT( iKAttributeRecv );
    iPreconditions->iCurrentRemoteStatus = CONSTANT( iKAttributeSend );
    // strength upgraded to mandatory
    iPreconditions->iDesiredLocalSendRecv = CONSTANT( iKAttributeMandatory );
    iPreconditions->iDesiredRemoteSendRecv = CONSTANT( iKAttributeOptional );
    // attempt to downgrade strength 
    attribute = CSdpAttributeField::DecodeLC( _L8("a=des:sec optional e2e sendrecv\r\n" ) );
    iPreconditions->iStream.Session()->iClientCryptoSuites.Append( EAES_CM_128_HMAC_SHA1_32 );
    EUNIT_ASSERT( iPreconditions->DecodeDesiredStatusL( *attribute ) == KMceSipPreconditionFailure );
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSendRecv == CONSTANT( iKAttributeOptional ) );
    iPreconditions->iStream.Session()->iClientCryptoSuites.Reset();
    CleanupStack::PopAndDestroy( attribute );        		
    
    }
    
void UT_TMceSecurePreconditions::UT_TMceSecurePreconditions_DecodeConfStatusLL()
    {
    iPreconditions->iConfirmation = CONSTANT( iKAttributeNone );

    CSdpAttributeField* attribute = CSdpAttributeField::DecodeLC( _L8("a=conf:sec e2e sendrecv\r\n" ) );
    
    EUNIT_ASSERT( iPreconditions->DecodeConfStatusL( *attribute ) == 1 );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeSendRecv ) );
    
    CleanupStack::PopAndDestroy( attribute );
    iPreconditions->iConfirmation = CONSTANT( iKAttributeNone );

     attribute = CSdpAttributeField::DecodeLC( _L8("a=conf:sec e2e sendrecv\r\n" ) );
    
    EUNIT_ASSERT( iPreconditions->DecodeConfStatusL( *attribute ) == 1 );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeSendRecv ) );

    CleanupStack::PopAndDestroy( attribute );
    iPreconditions->iConfirmation = CONSTANT( iKAttributeNone );

    attribute = CSdpAttributeField::DecodeLC( _L8("a=conf:x e2e sendrecv\r\n" ) );
    
    EUNIT_ASSERT( iPreconditions->DecodeConfStatusL( *attribute ) == 0 );
    EUNIT_ASSERT( iPreconditions->iConfirmation == CONSTANT( iKAttributeNone ) );

    CleanupStack::PopAndDestroy( attribute );
    }
    
void UT_TMceSecurePreconditions::UT_TMceSecurePreconditions_ParseCurrentStatusL()
    {
    TPtrC8 directionDesc;
    
    EUNIT_ASSERT( iPreconditions->ParseCurrentStatus( _L8("sec e2e sendrecv"), 
                                                      directionDesc ) )
    EUNIT_ASSERT( directionDesc == CONSTANT( iKAttributeSendRecv ).DesC() )

    EUNIT_ASSERT( iPreconditions->ParseCurrentStatus( _L8("sec e2e send trailing tokens"),
                                                      directionDesc ) );
    EUNIT_ASSERT( directionDesc == CONSTANT( iKAttributeSend ).DesC() )


    EUNIT_ASSERT( !iPreconditions->ParseCurrentStatus( _L8("x e2e sendrecv"), 
                                                       directionDesc ) )
    }
    
void UT_TMceSecurePreconditions::UT_TMceSecurePreconditions_ParseDesiredStatusL()
    {
    TPtrC8 strengthDesc; 
    TPtrC8 directionDesc;
    
    
    EUNIT_ASSERT( iPreconditions->ParseDesiredStatus( _L8("sec optional e2e sendrecv"),
                                                      strengthDesc, 
                                                      directionDesc ) )    
    EUNIT_ASSERT( strengthDesc == CONSTANT( iKAttributeOptional ).DesC() )
    EUNIT_ASSERT( directionDesc == CONSTANT( iKAttributeSendRecv ).DesC() )

    EUNIT_ASSERT( iPreconditions->ParseDesiredStatus( _L8("sec mandatory e2e send trailing tokens"),
                                                      strengthDesc, 
                                                      directionDesc ) )   
    EUNIT_ASSERT( strengthDesc == CONSTANT( iKAttributeMandatory ).DesC() );
    EUNIT_ASSERT( directionDesc == CONSTANT( iKAttributeSend ).DesC() );


    EUNIT_ASSERT( !iPreconditions->ParseDesiredStatus( _L8("x mandatory e2e sendrecv"),
                                                       strengthDesc, 
                                                       directionDesc ) );
    }
    
void UT_TMceSecurePreconditions::UT_TMceSecurePreconditions_DecodeMediaDirectionLL()
    {
    EUNIT_ASSERT( iPreconditions->DecodeMediaDirectionL( _L8( "send" ) ) == CONSTANT( iKAttributeRecv ) );
    EUNIT_ASSERT( iPreconditions->DecodeMediaDirectionL( _L8( "recv" ) ) == CONSTANT( iKAttributeSend ) );
    EUNIT_ASSERT( iPreconditions->DecodeMediaDirectionL( _L8( "sendrecv" ) ) == CONSTANT( iKAttributeSendRecv ) );
    EUNIT_ASSERT_LEAVE( iPreconditions->DecodeMediaDirectionL( _L8( "foo" ) ) );
    }
    
void UT_TMceSecurePreconditions::UT_TMceSecurePreconditions_DecodeStrengthLL()
    {
    EUNIT_ASSERT( iPreconditions->DecodeStrengthL( _L8( "mandatory" ) ) == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->DecodeStrengthL( _L8( "optional" ) ) == CONSTANT( iKAttributeOptional ) );
    EUNIT_ASSERT( iPreconditions->DecodeStrengthL( _L8( "none" ) ) == CONSTANT( iKAttributeNone ) );
    EUNIT_ASSERT( iPreconditions->DecodeStrengthL( _L8( "failure" ) ) == CONSTANT( iKAttributeFailure ) );
    EUNIT_ASSERT( iPreconditions->DecodeStrengthL( _L8( "unknown" ) ) == CONSTANT( iKAttributeUnknown ) );
    EUNIT_ASSERT_LEAVE( iPreconditions->DecodeStrengthL( _L8( "foo" ) ) );
    }
    
void UT_TMceSecurePreconditions::UT_TMceSecurePreconditions_SetStateL()
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
    
void UT_TMceSecurePreconditions::UT_TMceSecurePreconditions_SetRemoteStatusL()
    {
    
    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeNone ) );
    
    iPreconditions->SetRemoteStatus( CONSTANT( iKAttributeSend ) );

    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeSend ) );

    iPreconditions->SetRemoteStatus( CONSTANT( iKAttributeRecv ) );

    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeSend ) );

    iPreconditions->SetRemoteStatus( CONSTANT( iKAttributeSendRecv ) );

    EUNIT_ASSERT( iPreconditions->iCurrentRemoteStatus == CONSTANT( iKAttributeSendRecv ) );
    
    }
    
void UT_TMceSecurePreconditions::UT_TMceSecurePreconditions_SetDesiredStatusL()
    {
    iPreconditions->SetDesiredStatus( CONSTANT( iKAttributeLocal ),  
                                      CONSTANT( iKAttributeMandatory ) );
                                      
    
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSendRecv == CONSTANT( iKAttributeNone ) );
    
    iPreconditions->SetDesiredStatus( CONSTANT( iKAttributeRemote ), 
                                      CONSTANT( iKAttributeOptional ) );
                                      
    
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeMandatory ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSendRecv == CONSTANT( iKAttributeOptional ) );
    
    iPreconditions->SetDesiredStatus( CONSTANT( iKAttributeLocal ), 
                                      CONSTANT( iKAttributeNone ) );
                                      
    
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeNone) );
   	EUNIT_ASSERT( iPreconditions->iDesiredRemoteSendRecv == CONSTANT( iKAttributeOptional ) );
    
    iPreconditions->SetDesiredStatus( CONSTANT( iKAttributeRemote ), 
                                      CONSTANT( iKAttributeNone ) );
                                      
    
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeNone ) );
     EUNIT_ASSERT( iPreconditions->iDesiredRemoteSendRecv == CONSTANT( iKAttributeNone ) );
    
    iPreconditions->SetDesiredStatus( CONSTANT( iKAttributeLocal ), 
                                      CONSTANT( iKAttributeOptional ) );
                                      
    
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeOptional ) );
     EUNIT_ASSERT( iPreconditions->iDesiredRemoteSendRecv == CONSTANT( iKAttributeNone ) );
    
    iPreconditions->SetDesiredStatus( CONSTANT( iKAttributeRemote ), 
                                      CONSTANT( iKAttributeFailure ) );
                                      
    
    EUNIT_ASSERT( iPreconditions->iDesiredLocalSendRecv == CONSTANT( iKAttributeOptional ) );
    EUNIT_ASSERT( iPreconditions->iDesiredRemoteSendRecv == CONSTANT( iKAttributeFailure ) );    
    }

    

/**
* Test: Decode Ignores the Unknow Sec parameters
*
*/
void UT_TMceSecurePreconditions::UT_TMceSecurePreconditions_DecodeUnknownSecLL()
    {
    
    iSdpDocument = CSdpDocument::DecodeL( KMceMMTestSdpUnKnownSecParameter );
    
    RPointerArray< CSdpMediaField >& mediaLines = iSdpDocument->MediaFields();
    
    for( TInt i = 0;i<mediaLines.Count();i++)
        {
        TUint error = KErrNone;
        iPreconditions->DecodeL( *mediaLines[ i ] );
        }
    }


void UT_TMceSecurePreconditions::UT_TMceSecurePreconditions_StrengthDowngradedL()
    {
    iPreconditions->iDesiredLocalSendRecv = CONSTANT( iKAttributeNone );
    EUNIT_ASSERT(!iPreconditions->StrengthDowngraded( CONSTANT( iKAttributeNone ) ) );
    EUNIT_ASSERT(!iPreconditions->StrengthDowngraded( CONSTANT( iKAttributeOptional ) ) );
    EUNIT_ASSERT(!iPreconditions->StrengthDowngraded( CONSTANT( iKAttributeMandatory ) ) );
    EUNIT_ASSERT(!iPreconditions->StrengthDowngraded( CONSTANT( iKAttributeFailure ) ) );
    EUNIT_ASSERT(!iPreconditions->StrengthDowngraded( CONSTANT( iKAttributeUnknown ) ) );
    
    iPreconditions->iDesiredLocalSendRecv = CONSTANT( iKAttributeOptional );
    EUNIT_ASSERT(iPreconditions->StrengthDowngraded( CONSTANT( iKAttributeNone ) ) );
    EUNIT_ASSERT(!iPreconditions->StrengthDowngraded( CONSTANT( iKAttributeOptional ) ) );
    EUNIT_ASSERT(!iPreconditions->StrengthDowngraded( CONSTANT( iKAttributeMandatory ) ) );
    EUNIT_ASSERT(iPreconditions->StrengthDowngraded( CONSTANT( iKAttributeFailure ) ) );
    EUNIT_ASSERT(iPreconditions->StrengthDowngraded( CONSTANT( iKAttributeUnknown ) ) );
    
    iPreconditions->iDesiredLocalSendRecv = CONSTANT( iKAttributeMandatory );
    EUNIT_ASSERT(iPreconditions->StrengthDowngraded( CONSTANT( iKAttributeNone ) ) );
    EUNIT_ASSERT(iPreconditions->StrengthDowngraded( CONSTANT( iKAttributeOptional ) ) );
    EUNIT_ASSERT(!iPreconditions->StrengthDowngraded( CONSTANT( iKAttributeMandatory ) ) );
    EUNIT_ASSERT(iPreconditions->StrengthDowngraded( CONSTANT( iKAttributeFailure ) ) );
    EUNIT_ASSERT(iPreconditions->StrengthDowngraded( CONSTANT( iKAttributeUnknown ) ) );
    
    iPreconditions->iDesiredLocalSendRecv = CONSTANT( iKAttributeFailure );
    EUNIT_ASSERT(!iPreconditions->StrengthDowngraded( CONSTANT( iKAttributeNone ) ) );
    EUNIT_ASSERT(!iPreconditions->StrengthDowngraded( CONSTANT( iKAttributeOptional ) ) );
    EUNIT_ASSERT(!iPreconditions->StrengthDowngraded( CONSTANT( iKAttributeMandatory ) ) );
    EUNIT_ASSERT(!iPreconditions->StrengthDowngraded( CONSTANT( iKAttributeFailure ) ) );
    EUNIT_ASSERT(!iPreconditions->StrengthDowngraded( CONSTANT( iKAttributeUnknown ) ) );
    
    iPreconditions->iDesiredLocalSendRecv = CONSTANT( iKAttributeUnknown );
    EUNIT_ASSERT(!iPreconditions->StrengthDowngraded( CONSTANT( iKAttributeNone ) ) );
    EUNIT_ASSERT(!iPreconditions->StrengthDowngraded( CONSTANT( iKAttributeOptional ) ) );
    EUNIT_ASSERT(!iPreconditions->StrengthDowngraded( CONSTANT( iKAttributeMandatory ) ) );
    EUNIT_ASSERT(!iPreconditions->StrengthDowngraded( CONSTANT( iKAttributeFailure ) ) );
    EUNIT_ASSERT(!iPreconditions->StrengthDowngraded( CONSTANT( iKAttributeUnknown ) ) );
    }

	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_TMceSecurePreconditions,
    "TMceSecurePreconditions tests",
    "UNIT" )


EUNIT_TEST(
    "DoConstructL test",
    "TMceSecurePreconditions",
    "DoConstructL",
    "FUNCTIONALITY",
    SetupL, UT_TMceSecurePreconditions_DoConstructLL, Teardown )

EUNIT_TEST(
    "EncodeL test",
    "TMceSecurePreconditions",
    "EncodeL",
    "FUNCTIONALITY",
    SetupL, UT_TMceSecurePreconditions_EncodeLL, Teardown )

EUNIT_TEST(
    "DecodeL test",
    "TMceSecurePreconditions",
    "DecodeL",
    "FUNCTIONALITY",
    SetupL, UT_TMceSecurePreconditions_DecodeLL, Teardown )
    
EUNIT_TEST(
    "Reserved test",
    "TMceSecurePreconditions",
    "Reserved",
    "FUNCTIONALITY",
    SetupL, UT_TMceSecurePreconditions_ReservedL, Teardown )
    
EUNIT_TEST(
    "CloneL test",
    "TMceSecurePreconditions",
    "CloneL",
    "FUNCTIONALITY",
    SetupL, UT_TMceSecurePreconditions_CloneLL, Teardown )
    
EUNIT_TEST(
    "EncodeCurrentStatusL test",
    "TMceSecurePreconditions",
    "EncodeCurrentStatusL",
    "FUNCTIONALITY",
    SetupL, UT_TMceSecurePreconditions_EncodeCurrentStatusLL, Teardown )
    
EUNIT_TEST(
    "EncodeDesiredStatusL test",
    "TMceSecurePreconditions",
    "EncodeDesiredStatusL",
    "FUNCTIONALITY",
    SetupL, UT_TMceSecurePreconditions_EncodeDesiredStatusLL, Teardown )
    
EUNIT_TEST(
    "EncodeConfirmationL test",
    "TMceSecurePreconditions",
    "EncodeConfirmationL",
    "FUNCTIONALITY",
    SetupL, UT_TMceSecurePreconditions_EncodeConfirmationLL, Teardown )
    
EUNIT_TEST(
    "CurrentStausTextLC test",
    "TMceSecurePreconditions",
    "CurrentStausTextLC",
    "FUNCTIONALITY",
    SetupL, UT_TMceSecurePreconditions_CurrentStatusTextLCL, Teardown )
    
EUNIT_TEST(
    "DesiredStausTextLC test",
    "TMceSecurePreconditions",
    "DesiredStausTextLC",
    "FUNCTIONALITY",
    SetupL, UT_TMceSecurePreconditions_DesiredStatusTextLCL, Teardown )
    
EUNIT_TEST(
    "DecodeCurrentStatusL test",
    "TMceSecurePreconditions",
    "DecodeCurrentStatusL",
    "FUNCTIONALITY",
    SetupL, UT_TMceSecurePreconditions_DecodeCurrentStatusLL, Teardown )
    
EUNIT_TEST(
    "DecodeDesiredStatusL test",
    "TMceSecurePreconditions",
    "DecodeDesiredStatusL",
    "FUNCTIONALITY",
    SetupL, UT_TMceSecurePreconditions_DecodeDesiredStatusLL, Teardown )
    
EUNIT_TEST(
    "DecodeConfStatusL test",
    "TMceSecurePreconditions",
    "DecodeConfStatusL",
    "FUNCTIONALITY",
    SetupL, UT_TMceSecurePreconditions_DecodeConfStatusLL, Teardown )
    
EUNIT_TEST(
    "ParseCurrentStatus test",
    "TMceSecurePreconditions",
    "ParseCurrentStatus",
    "FUNCTIONALITY",
    SetupL, UT_TMceSecurePreconditions_ParseCurrentStatusL, Teardown )
    
EUNIT_TEST(
    "ParseDesiredStatus test",
    "TMceSecurePreconditions",
    "ParseDesiredStatus",
    "FUNCTIONALITY",
    SetupL, UT_TMceSecurePreconditions_ParseDesiredStatusL, Teardown )
    
EUNIT_TEST(
    "DecodeMediaDirectionL test",
    "TMceSecurePreconditions",
    "DecodeMediaDirectionL",
    "FUNCTIONALITY",
    SetupL, UT_TMceSecurePreconditions_DecodeMediaDirectionLL, Teardown )
    
EUNIT_TEST(
    "DecodeStrengthL test",
    "TMceSecurePreconditions",
    "DecodeStrengthL",
    "FUNCTIONALITY",
    SetupL, UT_TMceSecurePreconditions_DecodeStrengthLL, Teardown )
    
EUNIT_TEST(
    "SetState test",
    "TMceSecurePreconditions",
    "SetState",
    "FUNCTIONALITY",
    SetupL, UT_TMceSecurePreconditions_SetStateL, Teardown )
    
EUNIT_TEST(
    "SetRemoteStatus test",
    "TMceSecurePreconditions",
    "SetRemoteStatus",
    "FUNCTIONALITY",
    SetupL, UT_TMceSecurePreconditions_SetRemoteStatusL, Teardown )
    
EUNIT_TEST(
    "SetDesiredStatus test",
    "TMceSecurePreconditions",
    "SetDesiredStatus",
    "FUNCTIONALITY",
    SetupL, UT_TMceSecurePreconditions_SetDesiredStatusL, Teardown )
    
EUNIT_TEST(
    "DecodeL - UnKnownSectest",
    "TMceSrvStreamIterator",
    "DecodeL",
    "FUNCTIONALITY",
    SetupL, UT_TMceSecurePreconditions_DecodeUnknownSecLL, Teardown)

EUNIT_TEST(
    "StrengthDowngraded test",
    "TMceSSecurePreconditions",
    "DecodeL",
    "FUNCTIONALITY",
    SetupL, UT_TMceSecurePreconditions_StrengthDowngradedL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
