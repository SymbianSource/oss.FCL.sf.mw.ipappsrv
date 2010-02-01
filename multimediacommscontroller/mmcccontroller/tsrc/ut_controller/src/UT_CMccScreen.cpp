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
#include "UT_CMccScreen.h"
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
UT_CMccScreen* UT_CMccScreen::NewL()
    {
    UT_CMccScreen* self = UT_CMccScreen::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccScreen* UT_CMccScreen::NewLC()
    {
    UT_CMccScreen* self = new( ELeave ) UT_CMccScreen();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccScreen::~UT_CMccScreen()
    {
    }

// Default constructor
UT_CMccScreen::UT_CMccScreen()
    {
    }

// Second phase construct
void UT_CMccScreen::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

CMccScreenBitmap* UT_CMccScreen::BitmapScreen()
    {
    return static_cast<CMccScreenBitmap*>( iScreen );
    }

CMccScreenDirect* UT_CMccScreen::DirectScreen()
    {
    return static_cast<CMccScreenDirect*>( iScreen );
    }
        
//  INTERFACE IMPLEMENTATIONS

void UT_CMccScreen::HandleEvent(const TECAMEvent& /*aEvent*/)
    {
    
    }
void UT_CMccScreen::ViewFinderReady(MCameraBuffer& /*aCameraBuffer*/,TInt /*aError*/)
    {
    
    }
void UT_CMccScreen::ImageBufferReady(MCameraBuffer& /*aCameraBuffer*/,TInt /*aError*/)
    {
    
    }
void UT_CMccScreen::VideoBufferReady(MCameraBuffer& /*aCameraBuffer*/,TInt /*aError*/)
    {
    
    }
        
//  METHODS



void UT_CMccScreen::SetupL(  )
    {
    //User::LeaveIfError( RFbsSession::Connect() );  // Needed if WS not stubbed

    }

void UT_CMccScreen::Setup2L(  )
    {
    //User::LeaveIfError( RFbsSession::Connect() );   // Needed if WS not stubbed
    
    iCamera = CCamera::New2L( *this, 0, KMccCamera2Supported );
    TPoint pos( 0, 0 ); 
    TSize area( 100, 100 ); 
    TInt index( 0 );
    TInt windowOrdinalPosition( 1 );
    TInt windowOrdinalPriority( 0 );
        
    iScreen = CMccScreen::NewL( 
          iCamera, pos, area, index, windowOrdinalPosition, windowOrdinalPriority );
    }

void UT_CMccScreen::Setup3L(  )
    {
    //User::LeaveIfError( RFbsSession::Connect() );   // Needed if WS not stubbed
    
    TPoint pos( 0, 0 ); 
    TSize area( 100, 100 ); 
    TInt index( 0 );
    TInt windowOrdinalPosition( 1 );
    TInt windowOrdinalPriority( 0 );
             
    iCamera = CCamera::New2L( *this, 0, KMccCamera2Supported );
 
    iCamera->iInfo.iOptionsSupported = TCameraInfo::EViewFinderDirectSupported;
    iScreen = CMccScreen::NewL( 
       iCamera, pos, area, index, windowOrdinalPosition, windowOrdinalPriority );

    }

void UT_CMccScreen::Teardown(  )
    {
    delete iScreen;
    iScreen = NULL;
    delete iCamera;
    iCamera = NULL;   
    //RFbsSession::Disconnect();   // Needed if WS not stubbed
    }

void UT_CMccScreen::UT_CMccScreen_CMccScreen_NewLL()

    {
    CCamera* camera = NULL;
    TPoint pos( 0, 0 ); 
    TSize area( 100, 100 ); 
    TInt index( 0 );
    TInt windowOrdinalPosition( 1 );
    TInt windowOrdinalPriority( 0 );
        
    // No camera given at construction phase, direct vf created
    CMccScreen* screen1 = CMccScreen::NewL( 
        camera, pos, area, index, windowOrdinalPosition, windowOrdinalPriority );
    CleanupStack::PushL( screen1 );
    EUNIT_ASSERT( screen1->iScreenType == CMccScreen::EDirectScreen )
    CleanupStack::PopAndDestroy( screen1 );
    
    // Camera given but direct vf not supported based on camera options
    camera = CCamera::New2L( *this, 0, KMccCamera2Supported );
    CleanupStack::PushL( camera );
    CMccScreen* screen2 = CMccScreen::NewL( 
        camera, pos, area, index, windowOrdinalPosition, windowOrdinalPriority );
    CleanupStack::PushL( screen2 );
    EUNIT_ASSERT( screen2->iScreenType == CMccScreen::EBitmapScreen )
    CleanupStack::PopAndDestroy( screen2 );
    
    // Camera given and direct vf is supported based on options
    camera->iInfo.iOptionsSupported = TCameraInfo::EViewFinderDirectSupported;
    CMccScreen* screen3 = CMccScreen::NewL( 
        camera, pos, area, index, windowOrdinalPosition, windowOrdinalPriority );
    CleanupStack::PushL( screen3 );
    EUNIT_ASSERT( screen3->iScreenType == CMccScreen::EDirectScreen )
    CleanupStack::PopAndDestroy( screen3 );
   
    CleanupStack::PopAndDestroy( camera );
    
    }

void UT_CMccScreen::UT_CMccScreen_CMccScreenBitmap_StartLL()
    {
    // Start without camera and pause frame
    iScreen->iCamera = NULL;
    BitmapScreen()->iStopped = ETrue;
    iScreen->StartL();
    EUNIT_ASSERT( !BitmapScreen()->iStopped )
    
    // Start without camera and with pause frame
    delete BitmapScreen()->iPausedFrameData;
    BitmapScreen()->iPausedFrameData = NULL;
    CFbsBitmap* bitmap = new ( ELeave ) CFbsBitmap();
    CleanupStack::PushL( bitmap );
    TSize bmpSize( 176, 144 );
    User::LeaveIfError( bitmap->Create( bmpSize, EColor4K ) );
    BitmapScreen()->StoreFrameDataL( *bitmap );
    CleanupStack::PopAndDestroy( bitmap );

    EUNIT_ASSERT( BitmapScreen()->iAttachedFrame == NULL );
    iScreen->StartL();
    EUNIT_ASSERT( BitmapScreen()->iAttachedFrame != NULL );
    
    // Start with camera
    iScreen->iCamera = iCamera;
    EUNIT_ASSERT( !iCamera->ViewFinderActive() );
    iScreen->StartL();
    EUNIT_ASSERT( iCamera->ViewFinderActive() );
    }

void UT_CMccScreen::UT_CMccScreen_CMccScreenBitmap_StopL()
    {
    BitmapScreen()->iStopped = EFalse;
    iScreen->Stop();
    EUNIT_ASSERT( BitmapScreen()->iStopped )
    }

void UT_CMccScreen::UT_CMccScreen_CMccScreenBitmap_DrawL()
    {
    iScreen->iCamera = iCamera;
    
    // Normal draw
    CFbsBitmap* bitmap = new ( ELeave ) CFbsBitmap();
    CleanupStack::PushL( bitmap );
    TSize bmpSize( 176, 144 );
    User::LeaveIfError( bitmap->Create( bmpSize, EColor4K ) );
    BitmapScreen()->AttachFrameL( bitmap );
    CleanupStack::Pop( bitmap );
    iScreen->Draw( *bitmap );
    EUNIT_ASSERT( BitmapScreen()->iAttachedFrame == NULL )
    
    // Draw after stop, vf stopping is pending for this frame
    CFbsBitmap* bitmap2 = new ( ELeave ) CFbsBitmap();
    CleanupStack::PushL( bitmap2 );
    User::LeaveIfError( bitmap2->Create( bmpSize, EColor4K ) );
    BitmapScreen()->iStopped = ETrue;
    iCamera->iVfStarted = ETrue;
    iScreen->Draw( *bitmap2 );
    EUNIT_ASSERT( !iCamera->iVfStarted )
    CleanupStack::PopAndDestroy( bitmap2 );
    }

void UT_CMccScreen::UT_CMccScreen_CMccScreenBitmap_SetCameraL()
    {
    iScreen->SetCamera( iCamera );
    EUNIT_ASSERT( iCamera == iScreen->iCamera );
    
    // Setting null camera causes vf stop
    iCamera->iVfStarted = ETrue;
    iScreen->SetCamera( NULL );
    EUNIT_ASSERT( !iCamera->iVfStarted )
    }

void UT_CMccScreen::UT_CMccScreen_CMccScreenDirect_StartLL()
    {
    // No camera, nothing happens
    iScreen->StartL();
    
    // Camera exists, viewfinder is started
    iCamera->iVfStarted = EFalse;
    iScreen->iCamera = iCamera;
    iScreen->StartL();
    EUNIT_ASSERT( iCamera->iVfStarted )
    
    // Vf already started, NOP
    iScreen->StartL();
    EUNIT_ASSERT( iCamera->iVfStarted )
    
    // Paused viewfinder exists, it is resumed
    DirectScreen()->iDirectViewFinder->PauseViewFinderDirectL();
    EUNIT_ASSERT( DirectScreen()->iDirectViewFinder->ViewFinderState() == 
        CCamera::CCameraDirectViewFinder::EViewFinderPause )
    iScreen->StartL();
    EUNIT_ASSERT( DirectScreen()->iDirectViewFinder->ViewFinderState() == 
            CCamera::CCameraDirectViewFinder::EViewFinderActive )
    }

void UT_CMccScreen::UT_CMccScreen_CMccScreenDirect_StopL()
    {
    iScreen->Stop();
    EUNIT_ASSERT( DirectScreen()->iDirectViewFinder->ViewFinderState() == 
            CCamera::CCameraDirectViewFinder::EViewFinderPause )
    }

void UT_CMccScreen::UT_CMccScreen_CMccScreenDirect_DrawL()
    {
    CFbsBitmap* bitmap = new ( ELeave ) CFbsBitmap();
    CleanupStack::PushL( bitmap );
    TSize bmpSize( 176, 144 );
    User::LeaveIfError( bitmap->Create( bmpSize, EColor4K ) );
       
    iScreen->Draw( *bitmap ); // NOP
    
    CleanupStack::PopAndDestroy( bitmap );
    }
void UT_CMccScreen::UT_CMccScreen_CMccScreenDirect_SetCameraL()
    {
    // Setting camera when none exists yet
    iScreen->iCamera = NULL;
    DirectScreen()->iDirectViewFinder = NULL;
    iScreen->SetCamera( iCamera );
    EUNIT_ASSERT( iScreen->iCamera == iCamera )
    EUNIT_ASSERT( DirectScreen()->iDirectViewFinder != NULL )
    
    // Setting camera when one exists already
    iScreen->SetCamera( iCamera );
    EUNIT_ASSERT( iScreen->iCamera == iCamera )
    EUNIT_ASSERT( DirectScreen()->iDirectViewFinder != NULL )
      
    // Setting null camera when direct vf exists
    iScreen->SetCamera( NULL );
    EUNIT_ASSERT( iScreen->iCamera == NULL )
    EUNIT_ASSERT( DirectScreen()->iDirectViewFinder == NULL )
    
    // Setting null camera when direct vf does not exist
    iScreen->SetCamera( NULL );
    EUNIT_ASSERT( iScreen->iCamera == NULL )
    EUNIT_ASSERT( DirectScreen()->iDirectViewFinder == NULL )
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccScreen,
    "Add test suite description here.",
    "UNIT" )
    

EUNIT_TEST(
    "CMccScreen::NewL - test ",
    "CMccScreen",
    "CMccScreen::NewL ",
    "FUNCTIONALITY",
    SetupL, UT_CMccScreen_CMccScreen_NewLL, Teardown)

EUNIT_TEST(
    "CMccScreenBitmap::StartL - test ",
    "CMccScreen",
    "CMccScreenBitmap::StartL ",
    "FUNCTIONALITY",
    Setup2L, UT_CMccScreen_CMccScreenBitmap_StartLL, Teardown)
    
EUNIT_TEST(
    "CMccScreenBitmap::Stop - test ",
    "CMccScreen",
    "CMccScreenBitmap::Stop ",
    "FUNCTIONALITY",
    Setup2L, UT_CMccScreen_CMccScreenBitmap_StopL, Teardown)
 
EUNIT_TEST(
    "CMccScreenBitmap::Draw - test ",
    "CMccScreen",
    "CMccScreenBitmap::Draw ",
    "FUNCTIONALITY",
    Setup2L, UT_CMccScreen_CMccScreenBitmap_DrawL, Teardown)
    
EUNIT_TEST(
    "CMccScreenBitmap::SetCamera - test ",
    "CMccScreen",
    "CMccScreenBitmap::SetCamera ",
    "FUNCTIONALITY",
    Setup2L, UT_CMccScreen_CMccScreenBitmap_SetCameraL, Teardown)

EUNIT_TEST(
    "CMccScreenDirect::StartL - test ",
    "CMccScreen",
    "CMccScreenDirect::StartL ",
    "FUNCTIONALITY",
    Setup3L, UT_CMccScreen_CMccScreenDirect_StartLL, Teardown)
    
EUNIT_TEST(
    "CMccScreenDirect::Stop - test ",
    "CMccScreen",
    "CMccScreenDirectp::Stop ",
    "FUNCTIONALITY",
    Setup3L, UT_CMccScreen_CMccScreenDirect_StopL, Teardown)
 
EUNIT_TEST(
    "CMccScreenDirect::Draw - test ",
    "CMccScreen",
    "CMccScreenDirect::Draw ",
    "FUNCTIONALITY",
    Setup3L, UT_CMccScreen_CMccScreenDirect_DrawL, Teardown)
    
EUNIT_TEST(
    "CMccScreenDirect::SetCamera - test ",
    "CMccScreen",
    "CMccScreenDirect::SetCamera ",
    "FUNCTIONALITY",
    Setup3L, UT_CMccScreen_CMccScreenDirect_SetCameraL, Teardown)
    
EUNIT_END_TEST_TABLE

//  END OF FILE
