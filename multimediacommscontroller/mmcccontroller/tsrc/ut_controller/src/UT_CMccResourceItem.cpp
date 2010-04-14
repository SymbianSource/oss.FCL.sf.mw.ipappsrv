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
#include "UT_CMccResourceItem.h"
#include "mccuids.hrh"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mccresourceitem.h"
#include "mccrtpdatasink.h"
#include "mccrtpdatasource.h"
#include "mccresourcepool_STUB.h"
#include "mcccamerahandler.h"
#include "mccunittestmacros.h"

const TUid KRtpDataSource            =  { KImplUidRtpDataSource   }; 
const TUid KRtpDataSink              =  { KImplUidRtpDataSink     };


// CONSTRUCTION
UT_CMccResourceItem* UT_CMccResourceItem::NewL()
    {
    UT_CMccResourceItem* self = UT_CMccResourceItem::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccResourceItem* UT_CMccResourceItem::NewLC()
    {
    UT_CMccResourceItem* self = new( ELeave ) UT_CMccResourceItem();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccResourceItem::~UT_CMccResourceItem()
    {
    }

// Default constructor
UT_CMccResourceItem::UT_CMccResourceItem()
    {
    }

// Second phase construct
void UT_CMccResourceItem::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccResourceItem::SetupL(  )
    {
    iSourceStub = static_cast< CMccRtpDataSource* >( MDataSource::NewSourceL( KRtpDataSource, KNullDesC8 ) );
    iSinkStub = static_cast< CMccRtpDataSink* >(MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 ));
    iItem = CMccResourceItem::NewL( 0, iSinkStub, NULL, 1, EFalse );  
    }

void UT_CMccResourceItem::Teardown(  )
    {
    delete iItem;
    delete iSinkStub;
    delete iSourceStub;
    }

void UT_CMccResourceItem::UT_CMccResourceItem_IncreaseRefCountL(  )
    {
    TMccResourceParams param(0,0,0,0,EFalse,0);

    MCC_EUNIT_ASSERT_EQUALS( iItem->IncreaseRefCount( param), KErrNone );
    }

void UT_CMccResourceItem::UT_CMccResourceItem_DecreaseRefCountL(  )
    {
    delete iItem;
    iItem = NULL;
    iItem = CMccResourceItem::NewL( 0, NULL, iSourceStub, 3, ETrue );  
    TMccResourceParams param(0,0,0,0,EFalse,0);
    
    CMccMultiplexer* multuplexer = iItem->MultiplexerL();
    MCC_EUNIT_ASSERT_EQUALS( iItem->IncreaseRefCount( param ), KErrNone );
    EUNIT_ASSERT( iItem->DecreaseRefCount( param));

    }

void UT_CMccResourceItem::UT_CMccResourceItem_RefCountL(  )
    {
    TMccResourceParams param(0,0,0,0,EFalse,0);

    MCC_EUNIT_ASSERT_EQUALS( iItem->IncreaseRefCount( param ), KErrNone );
    
    EUNIT_ASSERT( iItem->RefCount() == 1 );
    }

void UT_CMccResourceItem::UT_CMccResourceItem_EndpointIdL(  )
    {
    EUNIT_ASSERT( iItem->EndpointId() == 1);
    }

void UT_CMccResourceItem::UT_CMccResourceItem_IsNetworkResourceL(  )
    {
    EUNIT_ASSERT( iItem->IsNetworkResource());
    CMccResourceItem*  item = CMccResourceItem::NewLC( 0, NULL, iSourceStub, 1, EFalse );  
    EUNIT_ASSERT( item->IsNetworkResource());
    CleanupStack::PopAndDestroy(item);
    }

void UT_CMccResourceItem::UT_CMccResourceItem_IsResourceActiveL(  )
    {
    EUNIT_ASSERT( !iItem->IsResourceActive() );
    }

void UT_CMccResourceItem::UT_CMccResourceItem_IsSharedResourceL(  )
    {
    EUNIT_ASSERT( !iItem->IsSharedResource() );
    }

void UT_CMccResourceItem::UT_CMccResourceItem_SetResourceStateLL(  )
    {
    TMccResourceParams param(0,0,0,0,EFalse,0);
    User::LeaveIfError( iItem->IncreaseRefCount( param ) );
    
    TBool result;
    
    EUNIT_ASSERT_NO_LEAVE( result = iItem->SetResourceStateL(0, MMccResources::EPrepared, EFalse, EFalse ) );
    EUNIT_ASSERT_NO_LEAVE( result = iItem->SetResourceStateL(0, MMccResources::EStarted, EFalse, EFalse ) );
    EUNIT_ASSERT_NO_LEAVE( result = iItem->SetResourceStateL(0, MMccResources::EResourcePaused, EFalse, EFalse ) );
    
    // State is not downgraded
    EUNIT_ASSERT( iItem->iState == MMccResources::EResourcePaused );
    EUNIT_ASSERT_NO_LEAVE( result = iItem->SetResourceStateL(0, MMccResources::EPrepared, EFalse, EFalse ) );
    EUNIT_ASSERT( iItem->iState == MMccResources::EResourcePaused );
    
    EUNIT_ASSERT_NO_LEAVE( result = iItem->SetResourceStateL(0, MMccResources::EResourceStopped, EFalse, EFalse ) );
 
    delete iItem;
    iItem = NULL;
    
    iItem = CMccResourceItem::NewL( 0, NULL, iSourceStub, 1, ETrue );  
    
    CMccMultiplexer* multuplexer = iItem->MultiplexerL();
    MCC_EUNIT_ASSERT_EQUALS( iItem->IncreaseRefCount( param ), KErrNone );
    MCC_EUNIT_ASSERT_NO_LEAVE( result = iItem->SetResourceStateL(0, MMccResources::EPrepared, EFalse, EFalse ) );
    }

void UT_CMccResourceItem::UT_CMccResourceItem_IsSinkL(  )
    {
    EUNIT_ASSERT( iItem->IsSink() );
    }

void UT_CMccResourceItem::UT_CMccResourceItem_IsSourceL(  )
    {
    EUNIT_ASSERT( !iItem->IsSource() );
    }

void UT_CMccResourceItem::UT_CMccResourceItem_IsInternalL(  )
    {
    EUNIT_ASSERT( iItem->IsInternal() );
    }

void UT_CMccResourceItem::UT_CMccResourceItem_SourceL(  )
    {
    EUNIT_ASSERT( iItem->Source() == NULL );
    }

void UT_CMccResourceItem::UT_CMccResourceItem_SinkL(  )
    {
    EUNIT_ASSERT( iItem->Sink() == iSinkStub );
    }

void UT_CMccResourceItem::UT_CMccResourceItem_MultiplexerLL(  )
    {
    delete iItem;
    iItem = NULL;
    iItem = CMccResourceItem::NewL( 0, NULL, iSourceStub, 1, ETrue );  
    
    CMccMultiplexer* multuplexer = iItem->MultiplexerL();
    EUNIT_ASSERT( multuplexer != NULL );
    }

void UT_CMccResourceItem::UT_CMccResourceItem_GetMultiplexerLL(  )
    {
    CMccMultiplexer* multiplexer = NULL; 
    EUNIT_ASSERT_SPECIFIC_LEAVE( multiplexer = iItem->GetMultiplexerL(), KErrNotReady );
    EUNIT_ASSERT( multiplexer == NULL );

    delete iItem;
    iItem = NULL;
    iItem = CMccResourceItem::NewL( 0, NULL, iSourceStub, 1, ETrue );  
    EUNIT_ASSERT_NO_LEAVE(multiplexer = iItem->GetMultiplexerL());
    EUNIT_ASSERT( multiplexer != NULL );

    }

void UT_CMccResourceItem::UT_CMccResourceItem_UserSessionMatchL(  )
    {
    TMccResourceParams param(0,0,0,0,EFalse,0);

    TMccResourceParams param2(0,0,0,0,EFalse,0);
    
    EUNIT_ASSERT(iItem->UserSessionMatch(param, param2));
    
    param.iSessionId = 2;
    param2.iSessionId = 3;
    EUNIT_ASSERT(!iItem->UserSessionMatch(param, param2));

    }

void UT_CMccResourceItem::UT_CMccResourceItem_UserMatchL(  )
    {
    TMccResourceParams param(0,0,0,0,EFalse,0);

    TMccResourceParams param2(0,0,0,0,EFalse,0);
    
    EUNIT_ASSERT(iItem->UserMatch(param, param2));
    
    param.iStreamId = 2;
    param2.iStreamId = 3;
    EUNIT_ASSERT(!iItem->UserMatch(param, param2));

    }

void UT_CMccResourceItem::UT_CMccResourceItem_UserMatchNotStrictL(  )
    {
    TMccResourceParams param(0,1,0,0,EFalse,0);
    TMccResourceParams param2(0,1,0,0,EFalse,0);
    EUNIT_ASSERT(iItem->UserMatchNotStrict(param, param2))
    
    TMccResourceParams para3(0,0,1,0,EFalse,0);
    TMccResourceParams param4(0,0,1,0,EFalse,0);
    EUNIT_ASSERT(iItem->UserMatchNotStrict(param, param2))

    }

void UT_CMccResourceItem::UT_CMccResourceItem_UsersInfoLL(  )
    {
    TMccResourceParams param(0,0,0,0,EFalse,0);

    MCC_EUNIT_ASSERT_EQUALS( iItem->IncreaseRefCount( param ), KErrNone );
    param.iLinkId = 2; 
    EUNIT_ASSERT( iItem->IncreaseRefCount( param) == KErrNone );
    
    RArray<TMccResourceParams> paramsarray;
    CleanupClosePushL(paramsarray);
    iItem->UsersInfoL( paramsarray );
    EUNIT_ASSERT( paramsarray.Count() == 2 );
    CleanupStack::PopAndDestroy(&paramsarray);
    }

void UT_CMccResourceItem::UT_CMccResourceItem_CurrentStateL(  )
    {
    EUNIT_ASSERT( iItem->CurrentState() == MMccResources::EConstructed );
    }

void UT_CMccResourceItem::UT_CMccResourceItem_MatchL(  )
    {
    TMccResourceParams param(0,0,0,0,EFalse,0);

    MCC_EUNIT_ASSERT_EQUALS( iItem->IncreaseRefCount( param ), KErrNone );
    EUNIT_ASSERT( iItem->Match(param) );
    }

void UT_CMccResourceItem::UT_CMccResourceItem_MatchSessionL(  )
    {
    TUint32 sessionId = 1;
    TMccResourceParams param(sessionId,0,0,0,EFalse,0);

    MCC_EUNIT_ASSERT_EQUALS( iItem->IncreaseRefCount( param ), KErrNone );
    EUNIT_ASSERT( iItem->MatchSession(sessionId) );
    }

void UT_CMccResourceItem::UT_CMccResourceItem_IsCurrentUserL()
    {
    EUNIT_ASSERT(iItem->IsCurrentUser(0)); 
    }
    
void UT_CMccResourceItem::UT_CMccResourceItem_NonSharableResourceL()
    {
    EUNIT_ASSERT(!iItem->NonSharableResource(KRtpDataSource)); 

    }
    
void UT_CMccResourceItem::UT_CMccResourceItem_StandbyStateChangeL()
    {
    TMccResourceParams param(0,0,0,0,EFalse,0);
    User::LeaveIfError( iItem->IncreaseRefCount( param ) );
    
    TBool result( EFalse );
    
    iItem->SpecialStateChangeL( 0, MMccResources::EPrepared, EFalse, EFalse, result );
    iItem->SpecialStateChangeL( 0, MMccResources::EResourcePaused, EFalse, EFalse, result );
    iItem->SpecialStateChangeL( 0, MMccResources::EStarted, EFalse, EFalse, result );
    iItem->SpecialStateChangeL( 0, MMccResources::EResourcePaused, EFalse, EFalse, result );
    
    // State is not downgraded
    EUNIT_ASSERT( iItem->iState == MMccResources::EPrepared );
    iItem->SpecialStateChangeL( 0, MMccResources::EPrepared, EFalse, EFalse, result );
    EUNIT_ASSERT( iItem->iState == MMccResources::EPrepared );
    
    iItem->SpecialStateChangeL( 0, MMccResources::EResourceStopped, EFalse, EFalse, result );
    iItem->SpecialStateChangeL( 0, MMccResources::EResourceStopped, ETrue, EFalse, result );
    }

void UT_CMccResourceItem::UT_CMccResourceItem_NeedsCameraL()
    {
    CMccResourcePoolStub* pool = CMccResourcePoolStub::NewL();
    CleanupStack::PushL( pool );
    CMccCameraHandler* camera = CMccCameraHandler::NewL( *pool );
    CleanupStack::PushL( camera );
    iItem->iState = MMccResources::EResourceStopped;
    EUNIT_ASSERT( !iItem->NeedsCamera( *camera ) )
    
    // In started state, camera is needed always
    iItem->iState = MMccResources::EStarted;
    EUNIT_ASSERT( iItem->NeedsCamera( *camera ) )
    
    // In prepared state camera is needed only when viewfinder is enabled
    iItem->iState = MMccResources::EPrepared;
    EUNIT_ASSERT( !iItem->NeedsCamera( *camera ) )
    camera->iViewFinderEnabled = ETrue;
    camera->iViewFinderSettings.iSize = TSize( 100, 100 );
    EUNIT_ASSERT( iItem->NeedsCamera( *camera ) )

    // In paused state camera is needed only when viewfinder is enabled
    camera->iViewFinderEnabled = EFalse;
    iItem->iState = MMccResources::EResourcePaused;
    EUNIT_ASSERT( !iItem->NeedsCamera( *camera ) )
    camera->iViewFinderEnabled = ETrue;
    camera->iViewFinderSettings.iSize = TSize( 100, 100 );
    EUNIT_ASSERT( iItem->NeedsCamera( *camera ) )
    
    CleanupStack::PopAndDestroy( camera );
    CleanupStack::PopAndDestroy( pool );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccResourceItem,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "IncreaseRefCount - test ",
    "CMccResourceItem",
    "IncreaseRefCount",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceItem_IncreaseRefCountL, Teardown)

EUNIT_TEST(
    "DecreaseRefCount - test ",
    "CMccResourceItem",
    "DecreaseRefCount",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceItem_DecreaseRefCountL, Teardown)

EUNIT_TEST(
    "RefCount - test ",
    "CMccResourceItem",
    "RefCount",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceItem_RefCountL, Teardown)

EUNIT_TEST(
    "EndpointId - test ",
    "CMccResourceItem",
    "EndpointId",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceItem_EndpointIdL, Teardown)

EUNIT_TEST(
    "IsNetworkResource - test ",
    "CMccResourceItem",
    "IsNetworkResource",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceItem_IsNetworkResourceL, Teardown)

EUNIT_TEST(
    "IsResourceActive - test ",
    "CMccResourceItem",
    "IsResourceActive",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceItem_IsResourceActiveL, Teardown)

EUNIT_TEST(
    "IsSharedResource - test ",
    "CMccResourceItem",
    "IsSharedResource",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceItem_IsSharedResourceL, Teardown)

EUNIT_TEST(
    "SetResourceStateL - test ",
    "CMccResourceItem",
    "SetResourceStateL",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceItem_SetResourceStateLL, Teardown)

EUNIT_TEST(
    "IsSink - test ",
    "CMccResourceItem",
    "IsSink",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceItem_IsSinkL, Teardown)

EUNIT_TEST(
    "IsSource - test ",
    "CMccResourceItem",
    "IsSource",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceItem_IsSourceL, Teardown)

EUNIT_TEST(
    "IsInternal - test ",
    "CMccResourceItem",
    "IsInternal",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceItem_IsInternalL, Teardown)

EUNIT_TEST(
    "Source - test ",
    "CMccResourceItem",
    "Source",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceItem_SourceL, Teardown)

EUNIT_TEST(
    "Sink - test ",
    "CMccResourceItem",
    "Sink",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceItem_SinkL, Teardown)

EUNIT_TEST(
    "MultiplexerL - test ",
    "CMccResourceItem",
    "MultiplexerL",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceItem_MultiplexerLL, Teardown)

EUNIT_TEST(
    "GetMultiplexerL - test ",
    "CMccResourceItem",
    "GetMultiplexerL",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceItem_GetMultiplexerLL, Teardown)

EUNIT_TEST(
    "UserSessionMatch - test ",
    "CMccResourceItem",
    "UserSessionMatch",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceItem_UserSessionMatchL, Teardown)

EUNIT_TEST(
    "UserMatch - test ",
    "CMccResourceItem",
    "UserMatch",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceItem_UserMatchL, Teardown)

EUNIT_TEST(
    "UserMatchNotStrict - test ",
    "CMccResourceItem",
    "UserMatchNotStrict",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceItem_UserMatchNotStrictL, Teardown)

EUNIT_TEST(
    "UsersInfoL - test ",
    "CMccResourceItem",
    "UsersInfoL",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceItem_UsersInfoLL, Teardown)

EUNIT_TEST(
    "CurrentState - test ",
    "CMccResourceItem",
    "CurrentState",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceItem_CurrentStateL, Teardown)

EUNIT_TEST(
    "Match - test ",
    "CMccResourceItem",
    "Match",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceItem_MatchL, Teardown)

EUNIT_TEST(
    "MatchSession - test ",
    "CMccResourceItem",
    "MatchSession",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceItem_MatchSessionL, Teardown)

EUNIT_TEST(
    "IsCurrentUserL - test ",
    "CMccResourceItem",
    "IsCurrentUserL",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceItem_IsCurrentUserL, Teardown)

EUNIT_TEST(
    "NonSharableResourceL - test ",
    "CMccResourceItem",
    "NonSharableResourceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceItem_NonSharableResourceL, Teardown)

EUNIT_TEST(
    "StandbyStateChange - test ",
    "CMccResourceItem",
    "StandbyStateChange",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceItem_StandbyStateChangeL, Teardown)

EUNIT_TEST(
    "NeedsCamera - test ",
    "CMccResourceItem",
    "NeedsCamera",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourceItem_NeedsCameraL, Teardown)
    
EUNIT_END_TEST_TABLE

//  END OF FILE
