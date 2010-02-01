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
* Description:  
*
*/




//  CLASS HEADER
#include "UT_CMccVideoSourceKeyRetriever.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#ifdef EUNIT_ALLOC_TEST_ON
	#include <digia/eunit/CEunitAllocTestCaseDecorator.h>  //for using EUNIT_ALLOC_TEST
#endif // end of EUNIT_ALLOC_TEST_ON

//  INTERNAL INCLUDES
#include "MccInternalCodecs.h"
#include "mcccamerahandler_STUB.h"
#include "mccvideosourceimpl.h"
#include "mccvideosourcekeyretriever.h"
#include "mccunittestmacros.h"
#include "mmcccodecavc.h"
#include "mccinternalevents.h"


// CONSTRUCTION
UT_CMccVideoSourceKeyRetriever* UT_CMccVideoSourceKeyRetriever::NewL()
    {
    UT_CMccVideoSourceKeyRetriever* self = UT_CMccVideoSourceKeyRetriever::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccVideoSourceKeyRetriever* UT_CMccVideoSourceKeyRetriever::NewLC()
    {
    UT_CMccVideoSourceKeyRetriever* self = new( ELeave ) UT_CMccVideoSourceKeyRetriever();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccVideoSourceKeyRetriever::~UT_CMccVideoSourceKeyRetriever()
    {
    }

// Default constructor
UT_CMccVideoSourceKeyRetriever::UT_CMccVideoSourceKeyRetriever()
    {
    }

// Second phase construct
void UT_CMccVideoSourceKeyRetriever::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccVideoSourceKeyRetriever::SetupL(  )
    {  
    iCameraHandler = CMccCameraHandlerStub::NewL();
    
    iRetriever = CMccVideoSourceConfigKeyRetriever::NewLC( *iCameraHandler );
    CleanupStack::Pop( iRetriever );
    
    // Make timeout shorter
    iRetriever->iGuardTimerTimeout = 100000;
    }

void UT_CMccVideoSourceKeyRetriever::Teardown(  )
    {
    delete iRetriever;
    delete iCameraHandler;
    }

void UT_CMccVideoSourceKeyRetriever::UT_CMccVideoSourceKeyRetriever_RetrieveL()
    {
    // Retrieve not supported for the codec
    //
    TMccCodecInfo codecInfo;
    codecInfo.iSdpName = KH263SdpName;
    codecInfo.iFourCC.Set(KMccFourCCIdH263);
    TBuf8<KMaxConfigKeyLen> configKey;
    configKey = KNullDesC8;                         
    
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iRetriever->RetrieveL( codecInfo, configKey ), KErrNotSupported );
    
    // Retrieve supported for the codec, retrieve ok
    //
    CCMRMediaRecorderStub::SetBehavior( KMccTestRecorderKeyRetrievalBehavior, KErrNone );
    TMccCodecInfo codecInfo2;
    codecInfo2.iSdpName = KAVCSdpName;
    codecInfo2.iFourCC.Set(KMccFourCCIdAVC);
    codecInfo2.iBitrateMask = KMccAvcCodecProfileIdBaseline | 
                              KMccAvcCodecProfileIopConstraintSet | 
                              KMccAvcBitrateLevel1;                  
    codecInfo2.iFrameSize = 10000;
    
    TRAPD( err, iRetriever->RetrieveL( codecInfo2, configKey ) );
    if ( err == KErrNotReady || err == KErrNotFound )
        {
        // Config key setting failed silently because of memory leave inside trap_ignore
        User::Leave( KErrNoMemory );
        }
    User::LeaveIfError( err );
    EUNIT_ASSERT( configKey.CompareF( KNullDesC8 ) != 0 );
    
    // Retrieve failed
    //
    CCMRMediaRecorderStub::SetBehavior( KMccTestRecorderKeyRetrievalBehavior, KErrGeneral );
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iRetriever->RetrieveL( codecInfo2, configKey ), KErrNotFound )
    EUNIT_ASSERT( iRetriever->iFailed );
    
    // Retrieve timeout
    //
    CCMRMediaRecorderStub::SetBehavior( KMccTestRecorderDefaultBehavior, KErrNone );
    TRAP( err, iRetriever->RetrieveL( codecInfo2, configKey ) );
    if ( err == KErrNotFound )
        {
        // Config key retrieval failed silently because of memory leave inside trap_ignore
        User::Leave( KErrNoMemory );
        }
    if ( err != KErrNotReady )
        {
        User::LeaveIfError( err );
        }
    else
        {
        EUNIT_ASSERT( err == KErrNotReady );
        EUNIT_ASSERT( !iRetriever->iFailed );
        }
    }

void UT_CMccVideoSourceKeyRetriever::UT_CMccVideoSourceKeyRetriever_SendEventToClientTestL()
    {
    TMMFEvent mmfEvent( KNullUid, KErrNone );
    EUNIT_ASSERT( iRetriever->SendEventToClient( mmfEvent ) == KErrNone );
    TMccEvent event( 0, 
                     0, 
                     0, 
                     0, 
                     KMccEventCategoryStream, 
                     KMccStreamPaused, 
                     KErrNone, 
                     KNullDesC8 );
	                     
	TMccInternalEvent internalEvent( KMccVideoSourceUid, 
		                             EMccInternalEventNone,
		                             event );
		                         
    EUNIT_ASSERT( iRetriever->SendEventToClient( internalEvent ) == KErrNone );
    }

void UT_CMccVideoSourceKeyRetriever::UT_CMccVideoSourceKeyRetriever_DummyTestL()
    {
    // Just cover no operation methods
    TMediaId id( KNullUid );
    iRetriever->SinkDataTypeCode( id );
    iRetriever->EmptyBufferL( NULL, NULL, id );
    iRetriever->CanCreateSinkBuffer();
    TBool ref;
    iRetriever->CreateSinkBufferL( id, ref );
    iRetriever->ConstructSinkL( KNullDesC8 );
    iRetriever->GuardTimerExpired( NULL );
    }
            
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccVideoSourceKeyRetriever,
    "CMccVideoSourceKeyRetriever",
    "CMccVideoSourceKeyRetriever" )

EUNIT_TEST(
    "RetrieveL - test ",
    "CMccVideoSourceKeyRetriever",
    "RetrieveL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceKeyRetriever_RetrieveL, Teardown)

EUNIT_TEST(
    "SendEventToClient - test ",
    "CMccVideoSourceKeyRetriever",
    "SendEventToClient",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceKeyRetriever_SendEventToClientTestL, Teardown)
    
EUNIT_TEST(
    "DummyTestL - test ",
    "CMccVideoSourceKeyRetriever",
    "DummyTestL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceKeyRetriever_DummyTestL, Teardown)
    
EUNIT_END_TEST_TABLE

//  END OF FILE
