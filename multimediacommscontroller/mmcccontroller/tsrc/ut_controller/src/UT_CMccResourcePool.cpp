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
#include "UT_CMccResourcePool.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <mmf/server/mmfdatabuffer.h>




//  INTERNAL INCLUDES
#include "mccresourcepool.h"
#include "mccrtpdatasink.h"
#include "mccrtpdatasource.h"
#include "mccuids.hrh"
#include "mccinternaldef.h"
#include "mccresourceitem.h"
#include "mcccamerahandler.h"

#include "mccunittestmacros.h"

const TUid KRtpDataSource            =  { KImplUidRtpDataSource   }; 
const TUid KRtpDataSink              =  { KImplUidRtpDataSink     };


// CONSTRUCTION
UT_CMccResourcePool* UT_CMccResourcePool::NewL()
    {
    UT_CMccResourcePool* self = UT_CMccResourcePool::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccResourcePool* UT_CMccResourcePool::NewLC()
    {
    UT_CMccResourcePool* self = new( ELeave ) UT_CMccResourcePool();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccResourcePool::~UT_CMccResourcePool()
    {
    }

// Default constructor
UT_CMccResourcePool::UT_CMccResourcePool()
    {
    }

// Second phase construct
void UT_CMccResourcePool::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccResourcePool::SetupL(  )
    {
    iSourceStub = static_cast< CMccRtpDataSource* >( MDataSource::NewSourceL( KRtpDataSource, KNullDesC8 ) );
    iSinkStub = static_cast< CMccRtpDataSink* >(MDataSink::NewSinkL( KRtpDataSink, KNullDesC8 ));
     
    iOwnItem1 = CMccResourceItem::NewL( 0, iSinkStub, NULL, 3, EFalse );
    iOwnItem2 = CMccResourceItem::NewL( 1, NULL, iSourceStub, 4, EFalse );

    iResourcePool = CMccResourcePool::NewL();
    
    iSinkStubEndpointId = 1;
    iSourceStubEndpointId = 2;
    iResourcePool->ReserveSinkL( iSinkStub, iSinkStubEndpointId );
    iResourcePool->ReserveSourceL(iSourceStub, iSourceStubEndpointId );
    TMccResourceParams param(0,1,1,0,EFalse,0);

    EUNIT_ASSERT_NO_LEAVE(iResourcePool->ReserveResourcesL(param));
    /*
    iResourcePool->ReserveSinkL( iSinkStub, MCC_ENDPOINT_ID(iSinkStub) );

    iResourcePool->ReserveSourceL(iSourceStub, MCC_ENDPOINT_ID(iSourceStub) );

    TMccResourceParams param(0,1,1,0,EFalse);

    EUNIT_ASSERT_NO_LEAVE(iResourcePool->ReserveResourcesL(param));
    */
   
    }

void UT_CMccResourcePool::Teardown(  )
    {
    delete iResourcePool;
    delete iOwnItem2;
    delete iOwnItem1;
    delete iSinkStub;
    delete iSourceStub;
    iStandbySources.ResetAndDestroy();
    iStandbySinks.ResetAndDestroy();
    }
    
// HELPERS

void UT_CMccResourcePool::InitializeStandbyStreamL( TUint32 aStandbyStreamId )
    {
    TInt err( KErrNone );
    
    MDataSource* standbySource = MDataSource::NewSourceL( KRtpDataSource, KNullDesC8 );
    err = iStandbySources.Append( standbySource );
    if ( err )
        {
        delete standbySource;
        User::Leave( err );
        }
    
    MDataSink* standbySink = MDataSink::NewSinkL( KUidMmfAudioOutput, KNullDesC8 );
    err = iStandbySinks.Append( standbySink );
    if ( err )
        {
        delete standbySink;
        User::Leave( err );
        }
    
    iResourcePool->ReserveSinkL( standbySink, MCC_ENDPOINT_ID( standbySink ) );
    iResourcePool->ReserveSourceL( standbySource, MCC_ENDPOINT_ID( standbySource ) );
    TMccResourceParams param(0,1,aStandbyStreamId,0,ETrue,0);

    iResourcePool->ReserveResourcesL(param);
    }

// TEST FUNCTIONS
    
void UT_CMccResourcePool::UT_CMccResourcePool_ReuseLL(  )
    {
    
    EUNIT_ASSERT_SPECIFIC_LEAVE( iResourcePool->ReuseL(5),KErrNotFound );
    EUNIT_ASSERT_NO_LEAVE(iResourcePool->ReuseL(1));
    EUNIT_ASSERT_NO_LEAVE(iResourcePool->ReuseL(2));

    }

void UT_CMccResourcePool::UT_CMccResourcePool_ReferenceCountLL(  )
    {
    TInt refCount = 0;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iResourcePool->ReferenceCountL(5,refCount),KErrNotFound );
    EUNIT_ASSERT( refCount == 0 );
    EUNIT_ASSERT_NO_LEAVE(iResourcePool->ReferenceCountL(1, refCount));
    EUNIT_ASSERT( refCount != 0 );
    }

void UT_CMccResourcePool::UT_CMccResourcePool_SourceCandidateL(  )
    {
    iResourcePool->ReserveSourceL(iSourceStub, 3);
    
    EUNIT_ASSERT( iResourcePool->SourceCandidate() == iSourceStub );
    }

void UT_CMccResourcePool::UT_CMccResourcePool_SinkCandidateL(  )
    {
    iResourcePool->ReserveSinkL( iSinkStub, 4 );

    EUNIT_ASSERT( iResourcePool->SinkCandidate() == iSinkStub );
    }

void UT_CMccResourcePool::UT_CMccResourcePool_SetEnableRtcpLL(  )
    {
    EUNIT_ASSERT_SPECIFIC_LEAVE( iResourcePool->SetEnableRtcpL(5,ETrue),KErrNotFound );
    EUNIT_ASSERT_NO_LEAVE( iResourcePool->SetEnableRtcpL(1,ETrue) );

    }

void UT_CMccResourcePool::UT_CMccResourcePool_EnableRtcpL(  )
    {
    EUNIT_ASSERT( !iResourcePool->EnableRtcp(1) );
    }

void UT_CMccResourcePool::UT_CMccResourcePool_CameraHandlerLL(  )
    {
    CMccCameraHandler& cameraHandler = iResourcePool->CameraHandlerL(ETrue, 0);
    EUNIT_ASSERT(&cameraHandler);
    
    // Test emulator version of camerahandler
    }

void UT_CMccResourcePool::UT_CMccResourcePool_CreateNewCameraHandlerLL(  )
    {
    
    CMccCameraHandler* hander = iResourcePool->CreateNewCameraHandlerL();
    delete hander;
    hander = NULL;
    }

void UT_CMccResourcePool::UT_CMccResourcePool_UpdateResourceLL(  )
    {
    HBufC8* buf = HBufC8::NewLC(10);
    buf->Des().Copy(_L8("hello"));
    EUNIT_ASSERT_SPECIFIC_LEAVE(iResourcePool->UpdateResourceL(5, *buf),KErrNotFound);
    EUNIT_ASSERT_SPECIFIC_LEAVE(iResourcePool->UpdateResourceL(2, *buf),KErrNotSupported);
    CleanupStack::PopAndDestroy(buf);
    }

void UT_CMccResourcePool::UT_CMccResourcePool_ReserveSinkLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE(iResourcePool->ReserveSinkL( iSinkStub, 1 ));
    }

void UT_CMccResourcePool::UT_CMccResourcePool_ReserveSourceLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iResourcePool->ReserveSourceL(iSourceStub, 2) );
    }

void UT_CMccResourcePool::UT_CMccResourcePool_PrepareEndpointRemovalLL(  )
    {
    RArray<TMccResourceParams> affectedStreams;
    CleanupClosePushL(affectedStreams);
   
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(iResourcePool->PrepareEndpointRemovalL(5, affectedStreams),KErrNotFound);
    MCC_EUNIT_ASSERT_NO_LEAVE(iResourcePool->PrepareEndpointRemovalL(1 ,affectedStreams));
    
    CleanupStack::PopAndDestroy(&affectedStreams); 
    }

void UT_CMccResourcePool::UT_CMccResourcePool_RemoveEndpointLL(  )
    {
    EUNIT_ASSERT_SPECIFIC_LEAVE(iResourcePool->RemoveEndpointL(5),KErrNotFound);
    EUNIT_ASSERT_NO_LEAVE(iResourcePool->RemoveEndpointL(1));
    EUNIT_ASSERT_NO_LEAVE(iResourcePool->RemoveEndpointL(2));


    }

void UT_CMccResourcePool::UT_CMccResourcePool_ReserveResourcesLL(  )
    {
    EUNIT_ASSERT( ETrue );
    }

void UT_CMccResourcePool::UT_CMccResourcePool_FreeResourcesL(  )
    {
    EUNIT_ASSERT_NO_LEAVE(iResourcePool->FreeResources(1));
    }

void UT_CMccResourcePool::UT_CMccResourcePool_IsResourceUsageAllowedL(  )
    {
    EUNIT_ASSERT( iResourcePool->IsResourceUsageAllowed(1) );
    }

void UT_CMccResourcePool::UT_CMccResourcePool_SetResourceStateLL(  )
    {
    RArray<TUint32> controlledEndpoints;
    CleanupClosePushL( controlledEndpoints );
    RArray<TUint32> unControlledEndpoints;
    CleanupClosePushL( unControlledEndpoints );

    TBool controllerNR;
    MCC_EUNIT_ASSERT_NO_LEAVE(iResourcePool->SetResourceStateL(1,1, MMccResources::EPrepared, 
        controllerNR, controlledEndpoints, unControlledEndpoints));
    
    EUNIT_ASSERT( controllerNR ); 
    EUNIT_ASSERT( controlledEndpoints.Count() > 0 );
    EUNIT_ASSERT( unControlledEndpoints.Count() == 0 );

    CleanupStack::PopAndDestroy(&unControlledEndpoints);
    CleanupStack::PopAndDestroy(&controlledEndpoints);
    }

void UT_CMccResourcePool::UT_CMccResourcePool_ResourceStateSetLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE(iResourcePool->ResourceStateSetL());
    }

void UT_CMccResourcePool::UT_CMccResourcePool_EventReceivedL(  )
    {
    // No standby streams exist
    TMccEvent* additionalEvent = NULL;
    TMccEvent event;
    event.iLinkId = 1;
    event.iStreamId = 100;
    event.iEventType = KMccStandbyInactivityEvent;
    TBool ignoreEvent( EFalse );
    TBool effective( EFalse );
    
    User::LeaveIfError( 
        iResourcePool->EventReceived( event, ignoreEvent, effective, &additionalEvent ) );
    EUNIT_ASSERT_EQUALS( ignoreEvent, EFalse );
    
    // Standby stream exists but event is not standby related
    InitializeStandbyStreamL( 100 );
    event.iEventType = KMccStreamPaused;
    User::LeaveIfError( 
        iResourcePool->EventReceived( event, ignoreEvent, effective, &additionalEvent ) );
    EUNIT_ASSERT_EQUALS( ignoreEvent, EFalse );
    
    // Standby inactivity, no need to do anything as endpoint is already inactive
    event.iEventType = KMccStandbyInactivityEvent;
    effective = EFalse;
    User::LeaveIfError( 
        iResourcePool->EventReceived( event, ignoreEvent, effective, &additionalEvent ) );
    EUNIT_ASSERT_EQUALS( ignoreEvent, ETrue );
    EUNIT_ASSERT_EQUALS( effective, EFalse );
    
    // Standby inactivity when endpoint is active
    CMccResourceItem* standbyResource = 
        iResourcePool->FindResourceItem( MCC_ENDPOINT_ID( iStandbySinks[ 0 ] ) );
    EUNIT_ASSERT( standbyResource != NULL );
    standbyResource->iState = MMccResources::EStarted;
    ignoreEvent = EFalse;
    effective = EFalse;
    User::LeaveIfError( 
        iResourcePool->EventReceived( event, ignoreEvent, effective, &additionalEvent ) );
    EUNIT_ASSERT_EQUALS( ignoreEvent, ETrue );
    EUNIT_ASSERT_EQUALS( effective, ETrue );
    
    // Next event is discarded as well
    event.iEventType = KMccStreamPaused;
    ignoreEvent = EFalse;
    effective = EFalse;
    User::LeaveIfError( 
        iResourcePool->EventReceived( event, ignoreEvent, effective, &additionalEvent ) );
    EUNIT_ASSERT_EQUALS( ignoreEvent, ETrue );
    EUNIT_ASSERT_EQUALS( effective, EFalse );
    
    // Standby activity, no need to do anything as endpoint is already active
    event.iEventType = KMccStandbyActivityEvent;
    effective = EFalse;
    User::LeaveIfError( 
        iResourcePool->EventReceived( event, ignoreEvent, effective, &additionalEvent ) );
    EUNIT_ASSERT_EQUALS( ignoreEvent, ETrue );
    EUNIT_ASSERT_EQUALS( effective, EFalse );
    
    // Other standby stream exists but it does not need pausing
    standbyResource->iState = MMccResources::EResourcePaused;
    InitializeStandbyStreamL( 101 );
    User::LeaveIfError( 
        iResourcePool->EventReceived( event, ignoreEvent, effective, &additionalEvent ) );
    EUNIT_ASSERT_EQUALS( ignoreEvent, ETrue );
    EUNIT_ASSERT_EQUALS( effective, ETrue );
    EUNIT_ASSERT( additionalEvent == NULL );
    
    // Next event is discarded as well
    event.iEventType = KMccStreamStarted;
    ignoreEvent = EFalse;
    effective = EFalse;
    User::LeaveIfError( 
        iResourcePool->EventReceived( event, ignoreEvent, effective, &additionalEvent ) );
    EUNIT_ASSERT_EQUALS( ignoreEvent, ETrue );
    EUNIT_ASSERT_EQUALS( effective, EFalse );
    
    // Other standby stream needs pausing before other can be activated
    event.iEventType = KMccStandbyActivityEvent;
    CMccResourceItem* standbyResource2 = 
        iResourcePool->FindResourceItem( MCC_ENDPOINT_ID( iStandbySinks[ 1 ] ) );
    standbyResource2->iState = MMccResources::EStarted;
    effective = EFalse;
    User::LeaveIfError( 
        iResourcePool->EventReceived( event, ignoreEvent, effective, &additionalEvent ) );
    EUNIT_ASSERT_EQUALS( ignoreEvent, ETrue );
    EUNIT_ASSERT_EQUALS( effective, ETrue );
    EUNIT_ASSERT( additionalEvent != NULL );
    
    delete additionalEvent;
    }

void UT_CMccResourcePool::UT_CMccResourcePool_SinksL(  )
    {
    RPointerArray<MDataSink> sinks;
    sinks = iResourcePool->Sinks(EFalse);
    
    // No sinks returned if allocation fails, cannot assert
    //EUNIT_ASSERT(sinks.Count() > 0);
    }

void UT_CMccResourcePool::UT_CMccResourcePool_SourcesL(  )
    {
    RPointerArray<MDataSource> sources;
    sources = iResourcePool->Sources(EFalse);
    
    // No sources returned if allocation fails, cannot assert
    //EUNIT_ASSERT(sources.Count() > 0); 
    }

void UT_CMccResourcePool::UT_CMccResourcePool_MultiplexerLL(  )
    {
   
    iResourcePool->ReserveSinkL( iSinkStub, MCC_ENDPOINT_ID(iSinkStub) );
    
    CMccRtpDataSource* sourceStub = static_cast< CMccRtpDataSource* >( MDataSource::NewSourceL( KRtpDataSource, KNullDesC8 ) );
    CleanupStack::PushL( sourceStub );
    iResourcePool->ReserveSourceL(sourceStub, MCC_ENDPOINT_ID( static_cast<MDataSource*>( sourceStub ) ) );

    EUNIT_ASSERT_SPECIFIC_LEAVE(iResourcePool->MultiplexerL((MDataSource*)8),KErrNotFound);
    
    TMccResourceParams param(0,1,2,0,EFalse,0);

    iResourcePool->ReserveResourcesL(param);  

    CMccMultiplexer* multiplexer = NULL;
    multiplexer = iResourcePool->MultiplexerL( sourceStub );
    EUNIT_ASSERT(multiplexer);
    
    CleanupStack::PopAndDestroy( sourceStub );
    }

void UT_CMccResourcePool::UT_CMccResourcePool_FindResourceL(  )
    {
    MDataSink* sinkGot = NULL;
    MDataSource* sourceGot = NULL;

    EUNIT_ASSERT(iResourcePool->FindResource(0, MCC_ENDPOINT_ID(iSinkStub), KRtpDataSink, &sourceGot, &sinkGot ) == KErrNotFound);
    EUNIT_ASSERT(!sourceGot);
    EUNIT_ASSERT(!sinkGot);
    sinkGot = NULL;
    sourceGot = NULL;
    EUNIT_ASSERT(iResourcePool->FindResource(1, MCC_ENDPOINT_ID(iSinkStub), KRtpDataSource, &sourceGot, &sinkGot ) == KErrNone);
    EUNIT_ASSERT(sourceGot);
    EUNIT_ASSERT(!sinkGot);
 
    }

void UT_CMccResourcePool::UT_CMccResourcePool_FindNetworkResourceByRtpStreamIdL(  )
    {
    TRtpId rtpstreamid = KNullId;
    MDataSink* sinkGot = NULL;
    MDataSource* sourceGot = NULL;
    EUNIT_ASSERT(KErrNone == iResourcePool->FindNetworkResourceByRtpStreamId(iMccSessionId, rtpstreamid, 
                                                                             &sourceGot, &sinkGot) );

    }

void UT_CMccResourcePool::UT_CMccResourcePool_FindNetworkResourceByRtpSsrcL(  )
    {
    TRtpSSRC rtpSSRC;
    MDataSink* sinkGot = NULL;
    MDataSource* sourceGot = NULL;
    EUNIT_ASSERT(KErrNone == iResourcePool->FindNetworkResourceByRtpSsrc(rtpSSRC, &sourceGot, &sinkGot) );

    }
    
void UT_CMccResourcePool::UT_CMccResourcePool_GetEndpointL()
    {
    MDataSource* sourceGot = NULL;
    MDataSink* sinkGot = NULL;
    iResourcePool->GetEndpoint(*iOwnItem1, KNullUid, &sourceGot, &sinkGot );
    EUNIT_ASSERT( sinkGot );
    EUNIT_ASSERT( !sourceGot );
    sourceGot = NULL;
    sinkGot = NULL; 
    iResourcePool->GetEndpoint(*iOwnItem2, KNullUid, &sourceGot, &sinkGot );
    EUNIT_ASSERT( !sinkGot );
    EUNIT_ASSERT( sourceGot );
    
    }

void UT_CMccResourcePool::UT_CMccResourcePool_StoreKeyFrameInfoL()
    {
    CMMFDataBuffer* buf = CMMFDataBuffer::NewL( 10 );
    CleanupStack::PushL( buf );
    buf->SetTimeToPlay( 1000 );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iResourcePool->StoreKeyFrameInfoL( 99999, *buf ), KErrNotFound );
    
    iResourcePool->StoreKeyFrameInfoL( iSourceStubEndpointId, *buf );
    
    // Num of concurrently stored key infos is restricted (old info is discarded)
    //
    for ( TInt i = 1; i < 10; i++ )
        {
        buf->SetTimeToPlay( i );
        iResourcePool->StoreKeyFrameInfoL( iSourceStubEndpointId, *buf );
        }
    
    buf->SetTimeToPlay( 9 );
    EUNIT_ASSERT( iResourcePool->IsKeyFrame( iSourceStubEndpointId, *buf ) );
    buf->SetTimeToPlay( 1 );
    EUNIT_ASSERT( !iResourcePool->IsKeyFrame( iSourceStubEndpointId, *buf ) );
    
    CleanupStack::PopAndDestroy( buf );
    }

void UT_CMccResourcePool::UT_CMccResourcePool_IsKeyFrameL()
    {
    CMMFDataBuffer* buf = CMMFDataBuffer::NewL( 10 );
    CleanupStack::PushL( buf );
    buf->SetTimeToPlay( 1000 );
    EUNIT_ASSERT( !iResourcePool->IsKeyFrame( 99999, *buf ) );
    EUNIT_ASSERT( !iResourcePool->IsKeyFrame( iSourceStubEndpointId, *buf ) );
    iResourcePool->StoreKeyFrameInfoL( iSourceStubEndpointId, *buf );
    
    // Sink should found keyinfo stored by source as buffers contain same timestamp
    // and sink and source are in the same stream
    //
    CMMFDataBuffer* buf2 = CMMFDataBuffer::NewL( 10 );
    CleanupStack::PushL( buf2 );
    buf2->SetTimeToPlay( 1000 );
    EUNIT_ASSERT( iResourcePool->IsKeyFrame( iSinkStubEndpointId, *buf2 ) );
    
    CleanupStack::PopAndDestroy( buf2 );
    CleanupStack::PopAndDestroy( buf );
    }


void UT_CMccResourcePool::UT_CMccResourcePool_EnableViewFinderL()
    {
    TMccVideoSinkSetting settings;
    settings.iSize = TSize( 0, 0 );
    settings.iLocation = TPoint( 100, 100 );
    settings.iDeviceIndex = 0;
    iResourcePool->EnableViewFinderL( settings );
    EUNIT_ASSERT( iResourcePool->iCameraHandler->iViewFinderEnabled )
    }

void UT_CMccResourcePool::UT_CMccResourcePool_DisableViewFinderL()
    {
    iResourcePool->iCameraHandler->iViewFinderEnabled = ETrue;
    iResourcePool->DisableViewFinderL();
    EUNIT_ASSERT( !iResourcePool->iCameraHandler->iViewFinderEnabled )
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccResourcePool,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "ReuseL - test ",
    "CMccResourcePool",
    "ReuseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourcePool_ReuseLL, Teardown)

EUNIT_TEST(
    "ReferenceCountL - test ",
    "CMccResourcePool",
    "ReferenceCountL",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourcePool_ReferenceCountLL, Teardown)

EUNIT_TEST(
    "SourceCandidate - test ",
    "CMccResourcePool",
    "SourceCandidate",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourcePool_SourceCandidateL, Teardown)

EUNIT_TEST(
    "SinkCandidate - test ",
    "CMccResourcePool",
    "SinkCandidate",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourcePool_SinkCandidateL, Teardown)

EUNIT_TEST(
    "SetEnableRtcpL - test ",
    "CMccResourcePool",
    "SetEnableRtcpL",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourcePool_SetEnableRtcpLL, Teardown)

EUNIT_TEST(
    "EnableRtcp - test ",
    "CMccResourcePool",
    "EnableRtcp",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourcePool_EnableRtcpL, Teardown)

EUNIT_TEST(
    "CameraHandlerL - test ",
    "CMccResourcePool",
    "CameraHandlerL",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourcePool_CameraHandlerLL, Teardown)

EUNIT_TEST(
    "CreateNewCameraHandlerL - test ",
    "CMccResourcePool",
    "CreateNewCameraHandlerL",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourcePool_CreateNewCameraHandlerLL, Teardown)

EUNIT_TEST(
    "UpdateResourceL - test ",
    "CMccResourcePool",
    "UpdateResourceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourcePool_UpdateResourceLL, Teardown)

EUNIT_TEST(
    "ReserveSinkL - test ",
    "CMccResourcePool",
    "ReserveSinkL",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourcePool_ReserveSinkLL, Teardown)

EUNIT_TEST(
    "ReserveSourceL - test ",
    "CMccResourcePool",
    "ReserveSourceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourcePool_ReserveSourceLL, Teardown)

EUNIT_TEST(
    "PrepareEndpointRemovalL - test ",
    "CMccResourcePool",
    "PrepareEndpointRemovalL",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourcePool_PrepareEndpointRemovalLL, Teardown)

EUNIT_TEST(
    "RemoveEndpointL - test ",
    "CMccResourcePool",
    "RemoveEndpointL",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourcePool_RemoveEndpointLL, Teardown)

EUNIT_TEST(
    "ReserveResourcesL - test ",
    "CMccResourcePool",
    "ReserveResourcesL",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourcePool_ReserveResourcesLL, Teardown)

EUNIT_TEST(
    "FreeResources - test ",
    "CMccResourcePool",
    "FreeResources",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourcePool_FreeResourcesL, Teardown)

EUNIT_TEST(
    "IsResourceUsageAllowed - test ",
    "CMccResourcePool",
    "IsResourceUsageAllowed",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourcePool_IsResourceUsageAllowedL, Teardown)

EUNIT_TEST(
    "SetResourceStateL - test ",
    "CMccResourcePool",
    "SetResourceStateL",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourcePool_SetResourceStateLL, Teardown)

EUNIT_TEST(
    "ResourceStateSetL - test ",
    "CMccResourcePool",
    "ResourceStateSetL",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourcePool_ResourceStateSetLL, Teardown)

EUNIT_TEST(
    "EventReceived - test ",
    "CMccResourcePool",
    "EventReceived",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourcePool_EventReceivedL, Teardown)

EUNIT_TEST(
    "Sinks - test ",
    "CMccResourcePool",
    "Sinks",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourcePool_SinksL, Teardown)

EUNIT_TEST(
    "Sources - test ",
    "CMccResourcePool",
    "Sources",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourcePool_SourcesL, Teardown)

EUNIT_TEST(
    "MultiplexerL - test ",
    "CMccResourcePool",
    "MultiplexerL",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourcePool_MultiplexerLL, Teardown)

EUNIT_TEST(
    "FindResource - test ",
    "CMccResourcePool",
    "FindResource",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourcePool_FindResourceL, Teardown)

EUNIT_TEST(
    "FindNetworkResourceByRtpStreamId - test ",
    "CMccResourcePool",
    "FindNetworkResourceByRtpStreamId",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourcePool_FindNetworkResourceByRtpStreamIdL, Teardown)

EUNIT_TEST(
    "FindNetworkResourceByRtpSsrc - test ",
    "CMccResourcePool",
    "FindNetworkResourceByRtpSsrc",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourcePool_FindNetworkResourceByRtpSsrcL, Teardown)

EUNIT_TEST(
    "GetEndpointL - test ",
    "CMccResourcePool",
    "GetEndpointL",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourcePool_GetEndpointL, Teardown)

EUNIT_TEST(
    "StoreKeyFrameInfoL - test ",
    "CMccResourcePool",
    "StoreKeyFrameInfoL",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourcePool_StoreKeyFrameInfoL, Teardown)

EUNIT_TEST(
    "IsKeyFrame - test ",
    "CMccResourcePool",
    "IsKeyFrame",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourcePool_IsKeyFrameL, Teardown)

EUNIT_TEST(
    "EnableViewFinderL - test ",
    "CMccResourcePool",
    "EnableViewFinderL",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourcePool_EnableViewFinderL, Teardown)

EUNIT_TEST(
    "DisableViewFinderL - test ",
    "CMccResourcePool",
    "DisableViewFinderL",
    "FUNCTIONALITY",
    SetupL, UT_CMccResourcePool_DisableViewFinderL, Teardown)
    

EUNIT_END_TEST_TABLE

//  END OF FILE
