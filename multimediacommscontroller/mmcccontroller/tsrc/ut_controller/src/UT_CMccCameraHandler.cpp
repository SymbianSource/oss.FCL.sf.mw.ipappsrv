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
#include "UT_CMccCameraHandler.h"
#include "mccresourcepool.h"
#include "mccscreen.h"

#include "mccunittestmacros.h"

#include <fbs.h>

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

// CONSTANTS
const TInt KMccCamera2Supported = 999;
const TInt KMccCamera2CreationLeaves = 1000;


//  INTERNAL INCLUDES


// CONSTRUCTION
UT_CMccCameraHandler* UT_CMccCameraHandler::NewL()
    {
    UT_CMccCameraHandler* self = UT_CMccCameraHandler::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccCameraHandler* UT_CMccCameraHandler::NewLC()
    {
    UT_CMccCameraHandler* self = new( ELeave ) UT_CMccCameraHandler();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccCameraHandler::~UT_CMccCameraHandler()
    {
    }

// Default constructor
UT_CMccCameraHandler::UT_CMccCameraHandler()
    {
    }

// Second phase construct
void UT_CMccCameraHandler::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  INTERFACE IMPLEMENTATIONS

void UT_CMccCameraHandler::CameraReady( TInt /*aError*/ )
    {
    iCameraReadyNotified = ETrue;
    }
    
//  METHODS



void UT_CMccCameraHandler::SetupL(  )
    {
    // User::LeaveIfError( RFbsSession::Connect() ); // Needed if WS 
    iResources = CMccResourcePool::NewL();
    iCameraHandler = CMccCameraHandler::NewL( *iResources, 0, 0 );
    }

void UT_CMccCameraHandler::Teardown(  )
    {
    delete iCameraHandler;
    delete iResources;
    // RFbsSession::Disconnect();  // Needed if WS not stubbed
    }

void UT_CMccCameraHandler::UT_CMccCameraHandler_GetNumOfCamerasL()
    {
    TInt numOfCameras = iCameraHandler->GetNumOfCamerasL();
    EUNIT_ASSERT_EQUALS( numOfCameras, 1 );
    }

        
void UT_CMccCameraHandler::UT_CMccCameraHandler_ReserveResourcesL()
    {
    EUNIT_ASSERT_EQUALS( iCameraHandler->Handle(), 0 );
    
    // Test first situation where some error has occured already earlier
    iCameraHandler->iState = CMccCameraHandler::EFailed;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iCameraHandler->ReserveResourcesL( 0 ), KErrInUse );
    
    // Then test ok scenario
    iCameraHandler->iState = CMccCameraHandler::EInit;
    
    iCameraHandler->ReserveResourcesL( 0 );
    EUNIT_ASSERT( iCameraHandler->iState == CMccCameraHandler::EReserving );
    
    // Simulate reserve completion with error
    iCameraHandler->ReserveComplete( KErrGeneral );
    EUNIT_ASSERT( iCameraHandler->iState == CMccCameraHandler::EFailed );
    
    // Simulate succesful completion
    iCameraHandler->ReserveComplete( KErrNone );
    EUNIT_ASSERT( iCameraHandler->iState == CMccCameraHandler::EReserved );
    
    // Camera is powering, simulate completion with error
    iCameraHandler->PowerOnComplete( KErrGeneral );
    EUNIT_ASSERT( iCameraHandler->iState == CMccCameraHandler::EFailed );
    
    // Simluate succesful completion
    iCameraHandler->PowerOnComplete( KErrNone );
    EUNIT_ASSERT( iCameraHandler->iState == CMccCameraHandler::EPowered );
    
    EUNIT_ASSERT_EQUALS( iCameraHandler->Handle(), 1 );
    
    EUNIT_ASSERT( iCameraHandler->IsCameraReady() );
    
    // Then try to use other camera (not allowed)
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iCameraHandler->ReserveResourcesL( 1 ), KErrInUse );
    }


void UT_CMccCameraHandler::UT_CMccCameraHandler_ReserveResources2L()
    {
    // Test to simulate a situation when camera index is changed 
    // and camrea is recreated. Camera related internal camerahandler
    // variables should be reset for unassigned values.
    delete iCameraHandler->iCamera;    
    iCameraHandler->iCamera = NULL;
    iCameraHandler->iCameraIndex = 0;
    iCameraHandler->iZoom = 5;
    iCameraHandler->iDigitalZoom = 5;
    iCameraHandler->iContrast = 5;
    iCameraHandler->iBrightness = 5;
    iCameraHandler->iWhiteBalance = 5;
    iCameraHandler->iExposure = 5;    
    iCameraHandler->ReserveResourcesL( 1 );
    EUNIT_ASSERT( iCameraHandler->iZoom == KMaxTInt);
    EUNIT_ASSERT( iCameraHandler->iDigitalZoom == KMaxTInt);
    EUNIT_ASSERT( iCameraHandler->iContrast == KMaxTInt);
    EUNIT_ASSERT( iCameraHandler->iBrightness == KMaxTInt);
    EUNIT_ASSERT( iCameraHandler->iWhiteBalance == KMaxTInt);
    EUNIT_ASSERT( iCameraHandler->iExposure == KMaxTInt);
    
    // Test to simulate a situation when camera index is not changed 
    // and camrea is recreated. Like a pause and play case.
    // Camera related internal camerahandler variables should 
    // hold previous user settings.
    delete iCameraHandler->iCamera;
    iCameraHandler->iCameraIndex = 0;
    iCameraHandler->iCamera = NULL;
    iCameraHandler->iZoom = 6;
    iCameraHandler->iDigitalZoom = 6;
    iCameraHandler->iContrast = 6;
    iCameraHandler->iBrightness = 6;
    iCameraHandler->iWhiteBalance = 6;
    iCameraHandler->iExposure = 6;    
    iCameraHandler->ReserveResourcesL( 0 );
    EUNIT_ASSERT( iCameraHandler->iZoom == 6);
    EUNIT_ASSERT( iCameraHandler->iDigitalZoom == 6);
    EUNIT_ASSERT( iCameraHandler->iContrast == 6);
    EUNIT_ASSERT( iCameraHandler->iBrightness == 6);
    EUNIT_ASSERT( iCameraHandler->iWhiteBalance == 6);
    EUNIT_ASSERT( iCameraHandler->iExposure == 6);
    
    // Values are set for real in power-on complete

    }

void UT_CMccCameraHandler::UT_CMccCameraHandler_ReleaseResources()
    {
    // Test releasing when not reserved
    iCameraHandler->ReleaseResources();
    
    
    // Test releasing when reserved
    DoReserveL();
    
    iCameraHandler->ReleaseResources();
    EUNIT_ASSERT( iCameraHandler->iState == CMccCameraHandler::EInit );
    EUNIT_ASSERT( !iCameraHandler->iCamera )
    
    // Test releasing when viewfinder still enabled (not actually released)
    DoReserveL();
    
    TMccVideoSinkSetting settings;
    settings.iSize = TSize( 100, 100 );
    settings.iLocation = TPoint( 100, 100 );
    settings.iDeviceIndex = 0;
    iCameraHandler->EnableViewFinderL( settings );
    
    EUNIT_ASSERT( iCameraHandler->iScreen )
    EUNIT_ASSERT( iCameraHandler->iCamera )
    iCameraHandler->ReleaseResources();
    EUNIT_ASSERT( iCameraHandler->iScreen )
    EUNIT_ASSERT( iCameraHandler->iCamera )
    
    // Releasing when reserved and viewfinder is enabled but zero size
    // (release can occur immediately)
    DoReserveL();
        
    TMccVideoSinkSetting settings2;
    settings.iSize = TSize( 0, 0 );
    settings.iLocation = TPoint( 100, 100 );
    settings.iDeviceIndex = 0;
    iCameraHandler->EnableViewFinderL( settings );
    
    iCameraHandler->ReleaseResources();
    EUNIT_ASSERT( !iCameraHandler->iScreen )
    EUNIT_ASSERT( !iCameraHandler->iCamera )
    
    // Releasing is disabled while resource is forcefully reserved
    DoReserveL();
    iCameraHandler->iForcedResourceReservation = ETrue;
    iCameraHandler->ReleaseResources();
    EUNIT_ASSERT( iCameraHandler->iCamera != NULL )
    }
                
void UT_CMccCameraHandler::UT_CMccCameraHandler_EnableViewFinderL()
    {
    TMccVideoSinkSetting settings;
    settings.iSize = TSize( 100, 100 );
    settings.iLocation = TPoint( 100, 100 );
    settings.iDeviceIndex = 0;
    
    // Test first enabling while camera does not exist
    iCameraHandler->EnableViewFinderL( settings );
    
    TMccVideoSinkSetting settings2;
    iCameraHandler->GetViewFinderSettingsL( settings2 );
    EUNIT_ASSERT_EQUALS( settings2.iLocation.iX, 100 );
    
    // Then test enabling when camera exists, as enabled already, camera power-on
    // will enable viewfinder automatically
    DoReserveL();
    EUNIT_ASSERT( iCameraHandler->iCamera->iVfStarted == ETrue )
    
    // Test updating settings
    settings.iSize = TSize( 200, 200 );
    iCameraHandler->EnableViewFinderL( settings );
    iCameraHandler->GetViewFinderSettingsL( settings2 );
    EUNIT_ASSERT_EQUALS( settings2.iSize.iWidth, 200 );
    EUNIT_ASSERT( iCameraHandler->iCamera->iVfStarted == ETrue )
    
    // Setting same values does not have any further effect
    iCameraHandler->EnableViewFinderL( settings );
    iCameraHandler->GetViewFinderSettingsL( settings2 );
    EUNIT_ASSERT_EQUALS( settings2.iSize.iWidth, 200 );
    EUNIT_ASSERT( iCameraHandler->iCamera->iVfStarted == ETrue )
    
    // Test setting zero size vf -> vf is actually stopped
    settings.iSize = TSize( 0, 0 );
    iCameraHandler->EnableViewFinderL( settings );
    iCameraHandler->GetViewFinderSettingsL( settings2 );
    EUNIT_ASSERT_EQUALS( settings2.iSize.iWidth, 0 );
    EUNIT_ASSERT( iCameraHandler->iCamera->iVfStarted == EFalse )
    
    // Setting to zero again does not have any further effect
    iCameraHandler->EnableViewFinderL( settings );
    iCameraHandler->GetViewFinderSettingsL( settings2 );
    EUNIT_ASSERT_EQUALS( settings2.iSize.iWidth, 0 );
    EUNIT_ASSERT( iCameraHandler->iCamera->iVfStarted == EFalse )
    
    // Check that iEnabled in settings is handled correctly
    iCameraHandler->iViewFinderEnabled = EFalse;
    settings.iEnabled = EFalse;
    iCameraHandler->EnableViewFinderL( settings );
    EUNIT_ASSERT( iCameraHandler->iViewFinderEnabled == EFalse )
    EUNIT_ASSERT( iCameraHandler->iCamera->iVfStarted == EFalse )
    iCameraHandler->GetViewFinderSettingsL( settings2 );
    EUNIT_ASSERT( settings2.iEnabled == EFalse )
    
    settings.iEnabled = ETrue;
    iCameraHandler->EnableViewFinderL( settings );
    EUNIT_ASSERT( iCameraHandler->iViewFinderEnabled == ETrue )
    EUNIT_ASSERT( iCameraHandler->iCamera->iVfStarted == EFalse )
    iCameraHandler->GetViewFinderSettingsL( settings2 );
    EUNIT_ASSERT( settings2.iEnabled == ETrue )
    }

void UT_CMccCameraHandler::UT_CMccCameraHandler_DisableViewFinderL()
    {
    iCameraHandler->DisableViewFinderL();
    
    DoReserveL();
    
    TMccVideoSinkSetting settings;
    settings.iSize = TSize( 100, 100 );
    settings.iLocation = TPoint( 100, 100 );
    settings.iDeviceIndex = 0;
    iCameraHandler->EnableViewFinderL( settings );
    EUNIT_ASSERT( iCameraHandler->iViewFinderEnabled == ETrue );
    
    iCameraHandler->DisableViewFinderL();
    
    EUNIT_ASSERT( iCameraHandler->iViewFinderEnabled == EFalse );
    }
    	
void UT_CMccCameraHandler::UT_CMccCameraHandler_GetCamInfoL()
    {
    TCameraInfo info;
    info.iMaxZoom = 0;
    
    // Test info retrieval when camera does not exist
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iCameraHandler->GetCamInfoL( info, EFalse ), KErrNotReady );
    
    // Test info retrieval with forced camera creation, camera is created temporarily
    iCameraHandler->GetCamInfoL( info, ETrue );
    EUNIT_ASSERT_EQUALS( info.iMaxZoom, 100 );
    EUNIT_ASSERT( iCameraHandler->iCamera == NULL );
    
    // Test retrieval from existing camera
    info.iMaxZoom = 0;
    DoReserveL();
    EUNIT_ASSERT( iCameraHandler->iCamera != NULL );
    iCameraHandler->GetCamInfoL( info, EFalse );
    EUNIT_ASSERT_EQUALS( info.iMaxZoom, 100 );
    EUNIT_ASSERT( iCameraHandler->iCamera != NULL );
    }
            
void UT_CMccCameraHandler::UT_CMccCameraHandler_GetValueL()
    {
    TInt val( 0 );
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iCameraHandler->GetValueL( KMccCamZoomFactor, val ), KErrNotReady );
    
    DoReserveL();
    
    iCameraHandler->GetValueL( KMccCamZoomFactor, val );
    EUNIT_ASSERT_EQUALS( val, 1 );
    
    iCameraHandler->GetValueL( KMccCamDigitZoomFactor, val );
    iCameraHandler->GetValueL( KMccCamContrast, val );
    iCameraHandler->GetValueL( KMccCamBrightness, val );
    iCameraHandler->GetValueL( KMccCamWhiteBalance, val );
    iCameraHandler->GetValueL( KMccCamExposure, val );
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iCameraHandler->GetValueL( 1000, val ), KErrNotSupported );
    
    TReal val2( 0 );
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iCameraHandler->GetValueL( 1, val2 ), KErrNotSupported );
    }
        
void UT_CMccCameraHandler::UT_CMccCameraHandler_SetValueL()
    {
    iCameraHandler->SetValueL( KMccCamZoomFactor, 2 );
    EUNIT_ASSERT_EQUALS( iCameraHandler->iZoom, 2 );
    
    // Unassigned value should not have any effect
    iCameraHandler->SetValueL( KMccCamZoomFactor, KMaxTInt );
    EUNIT_ASSERT_EQUALS( iCameraHandler->iZoom, 2 );
    
    iCameraHandler->SetValueL( KMccCamDigitZoomFactor, 3 );
    EUNIT_ASSERT_EQUALS( iCameraHandler->iDigitalZoom, 3 );
    iCameraHandler->SetValueL( KMccCamContrast, 4 );
    EUNIT_ASSERT_EQUALS( iCameraHandler->iContrast, 4 );
    iCameraHandler->SetValueL( KMccCamBrightness, 5 );
    EUNIT_ASSERT_EQUALS( iCameraHandler->iBrightness, 5 );
    iCameraHandler->SetValueL( KMccCamWhiteBalance, 6 );
    EUNIT_ASSERT_EQUALS( iCameraHandler->iWhiteBalance, 6 );
    iCameraHandler->SetValueL( KMccCamExposure, 7 );
    EUNIT_ASSERT_EQUALS( iCameraHandler->iExposure, 7 );
    
    DoReserveL();
    
    iCameraHandler->SetValueL( KMccCamZoomFactor, 8 );
    EUNIT_ASSERT_EQUALS( iCameraHandler->iZoom, 8 );
    iCameraHandler->SetValueL( KMccCamDigitZoomFactor, 9 );
    EUNIT_ASSERT_EQUALS( iCameraHandler->iDigitalZoom, 9 );
    iCameraHandler->SetValueL( KMccCamContrast, 10 );
    EUNIT_ASSERT_EQUALS( iCameraHandler->iContrast, 10 );
    iCameraHandler->SetValueL( KMccCamBrightness, 11 );
    EUNIT_ASSERT_EQUALS( iCameraHandler->iBrightness, 11 );
    iCameraHandler->SetValueL( KMccCamWhiteBalance, 12 );
    EUNIT_ASSERT_EQUALS( iCameraHandler->iWhiteBalance, 12 );
    iCameraHandler->SetValueL( KMccCamExposure, 13 );
    EUNIT_ASSERT_EQUALS( iCameraHandler->iExposure, 13 );
    
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iCameraHandler->SetValueL( 1000, 10 ), KErrNotSupported );
    
    TReal val2( 0 );
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iCameraHandler->SetValueL( 1, val2 ), KErrNotSupported );
    }
        
void UT_CMccCameraHandler::UT_CMccCameraHandler_MonitorCameraReservation()
    {
    // Test start monitoring
    //
    
    iCameraReadyNotified = EFalse;
    
    EUNIT_ASSERT_EQUALS( iCameraHandler->iObservers.Count(), 0 );
    MCC_EUNIT_ASSERT_EQUALS( iCameraHandler->MonitorCameraReservation( *this, EFalse ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCameraHandler->iObservers.Count(), 1 );
    
    DoReserveL();
    EUNIT_ASSERT( iCameraReadyNotified );
    EUNIT_ASSERT_EQUALS( iCameraHandler->iObservers.Count(), 0 );
    
    // When camera is already powered, monitoring is not needed
    EUNIT_ASSERT_EQUALS( iCameraHandler->MonitorCameraReservation( *this, EFalse ), KErrArgument );
    
    // Start monitoring, forced reserve
    //
    iCameraHandler->iState = CMccCameraHandler::EInit;
    MCC_EUNIT_ASSERT_EQUALS( iCameraHandler->MonitorCameraReservation( *this, ETrue ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCameraHandler->iState, CMccCameraHandler::EReserving );
    EUNIT_ASSERT( iCameraHandler->iForcedResourceReservation );
    }

void UT_CMccCameraHandler::UT_CMccCameraHandler_StopCameraMonitoring()
    {
    // Test stop monitoring
    //
    iCameraHandler->ReleaseResources();
    MCC_EUNIT_ASSERT_EQUALS( iCameraHandler->MonitorCameraReservation( *this, EFalse ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCameraHandler->iObservers.Count(), 1 );
    EUNIT_ASSERT_EQUALS( iCameraHandler->StopCameraMonitoring( *this, EFalse ), KErrNone );
    EUNIT_ASSERT_EQUALS( iCameraHandler->iObservers.Count(), 0 );
    EUNIT_ASSERT_EQUALS( iCameraHandler->StopCameraMonitoring( *this, EFalse ), KErrNotFound );
    
    // Stop monitoring, forced release
    //
    EUNIT_ASSERT_EQUALS( iCameraHandler->MonitorCameraReservation( *this, EFalse ), KErrNone );
    iCameraHandler->iForcedResourceReservation = ETrue;
    MCC_EUNIT_ASSERT_EQUALS( iCameraHandler->StopCameraMonitoring( *this, ETrue ), KErrNone );
    EUNIT_ASSERT( !iCameraHandler->iForcedResourceReservation )
    }

void UT_CMccCameraHandler::UT_CMccCameraHandler_IsViewFinderEnabled()
    {
    iCameraHandler->iViewFinderEnabled = EFalse;
    EUNIT_ASSERT( !iCameraHandler->IsViewFinderEnabled() )
    iCameraHandler->iViewFinderEnabled = ETrue;
    iCameraHandler->iViewFinderSettings.iSize = TSize( 100, 100 );
    EUNIT_ASSERT( iCameraHandler->IsViewFinderEnabled() )
    }

void UT_CMccCameraHandler::UT_CMccCameraHandler_TestCamera2L()
    {
    // In some environments, "camera 2" is used with different observer interface
    //
    iCameraHandler->iPriority = KMccCamera2CreationLeaves;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iCameraHandler->ReserveResourcesL( 0 ), KErrGeneral );
    iCameraHandler->iPriority = KMccCamera2Supported;
    iCameraHandler->ReserveResourcesL( 0 );
    EUNIT_ASSERT( iCameraHandler->iState == CMccCameraHandler::EReserving );
    
    // Reserved
    TECAMEvent event( KUidECamEventReserveComplete, KErrNone );
    iCameraHandler->HandleEvent( event );
    EUNIT_ASSERT( iCameraHandler->iState == CMccCameraHandler::EReserved );
    
    // Power on
    TECAMEvent event2( KUidECamEventPowerOnComplete, KErrNone );
    iCameraHandler->HandleEvent( event2 );
    EUNIT_ASSERT( iCameraHandler->iState == CMccCameraHandler::EPowered );
    
    // Unkown event
    TECAMEvent event3( KUidECamEventCameraNoLongerReserved, KErrNone );
    iCameraHandler->HandleEvent( event3 );
    EUNIT_ASSERT( iCameraHandler->iState == CMccCameraHandler::EPowered );
    
    // View finder frame ready (with error)
    CMccTestCameraBuffer* buf = new ( ELeave ) CMccTestCameraBuffer;
    CleanupStack::PushL( buf );
    iCameraHandler->ViewFinderReady( *buf, KErrGeneral );
    EUNIT_ASSERT( iCameraHandler->iState == CMccCameraHandler::EFailed );
    
    // View finder frame ready (no frames)
    iCameraHandler->ViewFinderReady( *buf, KErrNone );
    
    // View finder frame ready (ok)
    buf->iNumFrames = 1;
    iCameraHandler->ViewFinderReady( *buf, KErrNone );
    
    CleanupStack::PopAndDestroy( buf );
    }


// HELPERS

void UT_CMccCameraHandler::DoReserveL()
    {
    iCameraHandler->ReserveResourcesL( 0 );
    
    // Simulate succesful completion
    iCameraHandler->ReserveComplete( KErrNone );
    
    // Simulate succesful completion
    iCameraHandler->PowerOnComplete( KErrNone );
    EUNIT_ASSERT( iCameraHandler->iState == CMccCameraHandler::EPowered );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccCameraHandler,
    "Add test suite description here.",
    "UNIT" )
    

EUNIT_TEST(
    "GetNumOfCamerasL - test ",
    "CMccCameraHandler",
    "GetNumOfCamerasL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCameraHandler_GetNumOfCamerasL, Teardown)
    
EUNIT_TEST(
    "ReserveResourcesL - test ",
    "CMccCameraHandler",
    "ReserveResourcesL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCameraHandler_ReserveResourcesL, Teardown)

EUNIT_TEST(
    "ReserveResourcesL - test 2",
    "CMccCameraHandler",
    "ReserveResourcesL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCameraHandler_ReserveResources2L, Teardown)
    
EUNIT_TEST(
    "ReleaseResources - test ",
    "CMccCameraHandler",
    "ReleaseResources",
    "FUNCTIONALITY",
    SetupL, UT_CMccCameraHandler_ReleaseResources, Teardown)   

EUNIT_TEST(
    "EnableViewFinderL - test ",
    "CMccCameraHandler",
    "EnableViewFinderL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCameraHandler_EnableViewFinderL, Teardown)
    
EUNIT_TEST(
    "DisableViewFinderL - test ",
    "CMccCameraHandler",
    "DisableViewFinderL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCameraHandler_DisableViewFinderL, Teardown)

EUNIT_TEST(
    "GetCamInfoL - test ",
    "CMccCameraHandler",
    "GetCamInfoL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCameraHandler_GetCamInfoL, Teardown)

EUNIT_TEST(
    "GetValueL - test ",
    "CMccCameraHandler",
    "GetValueL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCameraHandler_GetValueL, Teardown)

EUNIT_TEST(
    "SetValueL - test ",
    "CMccCameraHandler",
    "SetValueL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCameraHandler_SetValueL, Teardown)  
    
EUNIT_TEST(
    "MonitorCameraReservation - test ",
    "CMccCameraHandler",
    "MonitorCameraReservation",
    "FUNCTIONALITY",
    SetupL, UT_CMccCameraHandler_MonitorCameraReservation, Teardown)

EUNIT_TEST(
    "StopCameraMonitoring - test ",
    "CMccCameraHandler",
    "StopCameraMonitoring",
    "FUNCTIONALITY",
    SetupL, UT_CMccCameraHandler_StopCameraMonitoring, Teardown)

EUNIT_TEST(
    "IsViewFinderEnabled - test ",
    "CMccCameraHandler",
    "IsViewFinderEnabled",
    "FUNCTIONALITY",
    SetupL, UT_CMccCameraHandler_IsViewFinderEnabled, Teardown)
    
EUNIT_TEST(
    "TestCamera2L - test ",
    "CMccCameraHandler",
    "TestCamera2L",
    "FUNCTIONALITY",
    SetupL, UT_CMccCameraHandler_TestCamera2L, Teardown)
    
EUNIT_END_TEST_TABLE

//  END OF FILE
