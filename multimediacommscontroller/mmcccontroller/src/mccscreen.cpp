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

#include <bitstd.h>
#include <ecam.h>
#include <ecam/mcameradirectviewfinder.h>

#include "mccscreen.h"
#include "mcccontrollerlogs.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

_LIT(KMccWindowGroupName,"32MccVideoWindow");

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccScreen::NewL
// -----------------------------------------------------------------------------
//
CMccScreen* CMccScreen::NewL( CCamera* aCamera,
    TPoint aPos, 
    TSize aArea, 
    TInt aIndex, 
    TInt aWindowOrdinalPosition,
    TInt aWindowOrdinalPriority )
    {
    // Resolve screen type based on ecam capabilities, asssume direct vf
    // to be available if camera not present yet.
    CMccScreen* screen = NULL;
    TBool useDirectVf = ETrue; 
    
    if ( aCamera )
        {
        TCameraInfo info;
        aCamera->CameraInfo( info );
        useDirectVf = ( info.iOptionsSupported & TCameraInfo::EViewFinderDirectSupported );
        }
    
    if ( useDirectVf )
        {
        TRAPD( err, screen = CMccScreenDirect::NewL( 
                    aCamera, aPos, aArea, aIndex, 
                    aWindowOrdinalPosition, aWindowOrdinalPriority ) );
        // If creation failes for some reason, bitmap vf is tried to be
        // created as a fallback
        if ( err == KErrNoMemory )
            {
            User::Leave( err );
            }
        if ( err )
            {
            __CONTROLLER_INT1( "CMccScreen::NewL, direct screen creation failed:", err )  
            }
        }
    
    if ( !screen )
        {
        screen = CMccScreenBitmap::NewL( 
                    aCamera, aPos, aArea, aIndex, 
                    aWindowOrdinalPosition, aWindowOrdinalPriority );
        }
    return screen;
    }

// -----------------------------------------------------------------------------
// CMccScreen::~CMccScreen
// -----------------------------------------------------------------------------
//
CMccScreen::~CMccScreen()
    {
    __CONTROLLER( "CMccScreen::~CMccScreen" ) 

    StopCamera();
    
    delete iGraphicsContext;
    delete iDev;
    delete iRw;
    delete iRwGroup;
    iRwSession.Close();
    
    __CONTROLLER( "CMccScreen::~CMccScreen, exit" )
    }

// -----------------------------------------------------------------------------
// CMccScreen::StartL
// -----------------------------------------------------------------------------
//
void CMccScreen::StartL()
    {
    
    }

// -----------------------------------------------------------------------------
// CMccScreen::Stop
// -----------------------------------------------------------------------------
//
void CMccScreen::Stop()
    {
    
    }

// -----------------------------------------------------------------------------
// CMccScreen::Draw
// -----------------------------------------------------------------------------
//
void CMccScreen::Draw( CFbsBitmap& /*aFrame*/ )
    {
    
    }

// -----------------------------------------------------------------------------
// CMccScreen::SetCamera
// -----------------------------------------------------------------------------
//
TInt CMccScreen::SetCamera( CCamera* aCamera )
    {
    if ( !aCamera )
        {
        StopCamera();
        }
       
    iCamera = aCamera; 
       
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CMccScreen::StopCamera
// -----------------------------------------------------------------------------
//
void CMccScreen::StopCamera()
    {
    __CONTROLLER( "CMccScreen::StopCamera" )
    
    if ( iCamera )
        {
        iCamera->StopViewFinder();
        }
    
    __CONTROLLER( "CMccScreen::StopCamera, exit" )
    }

// -----------------------------------------------------------------------------
// CMccScreen::VfStartPossible
// -----------------------------------------------------------------------------
//
TBool CMccScreen::VfStartPossible()
    {
    return ( iCamera && !iCamera->ViewFinderActive() && 
             iArea.iWidth > 0 &&
             iArea.iHeight > 0 );
    }

// -----------------------------------------------------------------------------
// CMccScreen::ConstructL
// -----------------------------------------------------------------------------
//
void CMccScreen::ConstructL()
    {
    __CONTROLLER( "CMccScreen::ConstructL" )
    User::LeaveIfError( iRwSession.Connect() );
    
    // Create window group
    TInt groupId = iRwSession.GetFocusWindowGroup();
    iRwGroup = new (ELeave) RWindowGroup( iRwSession ) ;    

    __CONTROLLER_INT1( "CMccScreen::ConstructL, window ordinal pos", 
                       iWindowOrdinalPosition )
    __CONTROLLER_INT1( "CMccScreen::ConstructL, window ordinal priority", 
                       iWindowOrdinalPriority )
    
    User::LeaveIfError( iRwGroup->Construct( groupId, EFalse ) );
    iRwGroup->SetName( KMccWindowGroupName );
    
    iRwGroup->SetOrdinalPosition( iWindowOrdinalPosition, iWindowOrdinalPriority );
         
    // Create screen device
    iDev = new (ELeave) CWsScreenDevice( iRwSession );
    iDev->Construct( iDeviceIndex ); 
    
    TInt currMode = iDev->CurrentScreenMode();
    __CONTROLLER_INT1( "CMccScreen::ConstructL, screen device mode", currMode )
                       
    TPixelsTwipsAndRotation sizeAndRotation;
    iDev->GetScreenModeSizeAndRotation( currMode, sizeAndRotation );
    __CONTROLLER_INT2( "CMccScreen::ConstructL, screen device size", 
                       sizeAndRotation.iPixelSize.iWidth, 
                       sizeAndRotation.iPixelSize.iHeight )

    __CONTROLLER_INT1( "CMccScreen::ConstructL, screen device orientation", 
                       sizeAndRotation.iRotation )
    
    // Create window
    iRw = new (ELeave) RWindow( iRwSession );
    
    User::LeaveIfError( iRw->Construct( *iRwGroup, (TUint32)iRw ) );
    
    __CONTROLLER_INT2( "CMccScreen::ConstructL, window pos", iPosition.iX, iPosition.iY )
    __CONTROLLER_INT2( "CMccScreen::ConstructL, window size", iArea.iWidth, iArea.iHeight )

    iRw->SetPosition( iPosition );
    //iRw->SetBackgroundColor( KRgbBlack );
    iRw->SetSize( iArea );    
    iRw->SetOrdinalPosition( iWindowOrdinalPosition );
    
    __CONTROLLER( "CMccScreen: creating graphics context ..." )
    User::LeaveIfError( iDev->CreateContext( iGraphicsContext ) );
    __CONTROLLER( "CMccScreen: graphics context created!" )
    
    iRw->Activate();
    iRwSession.Flush();
    
    __CONTROLLER( "CMccScreen::ConstructL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccScreen::CMccScreen
// -----------------------------------------------------------------------------
//
CMccScreen::CMccScreen( 
    TMccScreenType aScreenType,
    CCamera* aCamera,
    TPoint aPos, 
    TSize aArea, 
    TInt aIndex, 
    TInt aWindowOrdinalPosition,
    TInt aWindowOrdinalPriority ) : 
    iCamera( aCamera ),
    iDeviceIndex( aIndex ), 
    iPosition( aPos ), 
    iArea( aArea ),
    iWindowOrdinalPosition( aWindowOrdinalPosition ),
    iWindowOrdinalPriority( aWindowOrdinalPriority ),
    iScreenType( aScreenType )
    {
    
    }
        

// -----------------------------------------------------------------------------
// CMccScreenBitmap::CMccScreenBitmap
// -----------------------------------------------------------------------------
//
CMccScreenBitmap* CMccScreenBitmap::NewL( 
    CCamera* aCamera,
    TPoint aPos, 
    TSize aArea, 
    TInt aIndex, 
    TInt aWindowOrdinalPosition,
	TInt aWindowOrdinalPriority )
	{
	CMccScreenBitmap* self = new ( ELeave ) CMccScreenBitmap( aCamera,
                                                  aPos, 
	                                              aArea, 
	                                              aIndex,
	                                              aWindowOrdinalPosition,
	                                              aWindowOrdinalPriority );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );	
	return self;
	}

// -----------------------------------------------------------------------------
// CMccScreenBitmap::CMccScreenBitmap
// -----------------------------------------------------------------------------
//
CMccScreenBitmap::CMccScreenBitmap( 
    CCamera* aCamera,
    TPoint aPos, 
    TSize aArea, 
    TInt aIndex, 
    TInt aWindowOrdinalPosition,
	TInt aWindowOrdinalPriority ) :
	CMccScreen( EBitmapScreen, aCamera, aPos, aArea, aIndex,
                aWindowOrdinalPosition, aWindowOrdinalPriority ),
	iAborted( ETrue )
	{
	}

// -----------------------------------------------------------------------------
// CMccScreenBitmap::CMccScreenBitmap
// -----------------------------------------------------------------------------
//
CMccScreenBitmap::~CMccScreenBitmap()
	{
	__CONTROLLER( "CMccScreenBitmap::~CMccScreenBitmap" )
	
	DetachFrame(); // Must do bitmap detach before releasing window resources
	delete iDirectScreenAccess;
    
    delete iPausedFrameData;
    
	__CONTROLLER( "CMccScreenBitmap::~CMccScreenBitmap, exit" )
	}

// -----------------------------------------------------------------------------
// CMccScreenBitmap::ConstructL
// -----------------------------------------------------------------------------
//
void CMccScreenBitmap::ConstructL()
	{
	__CONTROLLER( "CMccScreenBitmap::ConstructL" )
    
	CMccScreen::ConstructL();
	
	// Create direct screen access                                	
	iDirectScreenAccess = CDirectScreenAccess::NewL( iRwSession, *iDev, *iRw, *this );   
	
	UpdateViewFinderArea( iArea );
	
	__CONTROLLER_INT2( "CMccScreenBitmap::ConstructL, viewfinder pos", 
	                   iViewFinderImageRect.iTl.iX, iViewFinderImageRect.iTl.iY )
	__CONTROLLER_INT2( "CMccScreenBitmap::ConstructL, viewfinder size", 
	                   iViewFinderImageRect.Width(), iViewFinderImageRect.Height() )
	
	__CONTROLLER( "CMccScreenBitmap::ConstructL, exit" )
	}
    
// -----------------------------------------------------------------------------
// CMccScreenBitmap::StartL
// -----------------------------------------------------------------------------
//
void CMccScreenBitmap::StartL()
	{
	iStopped = EFalse;
	
    if ( iCamera )
        {
        StartCameraL();
        }
    else if ( iPausedFrameData )
        {
        __CONTROLLER( "CMccScreenBitmap::StartL, using paused frame" )
           
        CFbsBitmap* frame = RestoreFrameLC();
        AttachFrameL( frame );
        CleanupStack::Pop( frame );
        }
    else
        {
        // NOP
        }
    
    StartDsaL();
        		
	__CONTROLLER( "CMccScreenBitmap::StartL, exit" )
	}

// -----------------------------------------------------------------------------
// CMccScreenBitmap::StartCameraL
// -----------------------------------------------------------------------------
//
void CMccScreenBitmap::StartCameraL()
    {
    __CONTROLLER( "CMccScreenBitmap::StartCameraL" )    
    
    __ASSERT_ALWAYS( iCamera, User::Leave( KErrNotReady ) );
    
    // Camera may modify the size
    TSize vfBitmapSize = iArea;
    __CONTROLLER_INT2( "CMccScreenBitmap::StartL, size before",  
                       vfBitmapSize.iWidth,
                       vfBitmapSize.iHeight )
    if ( VfStartPossible() )
        {
        iCamera->StartViewFinderBitmapsL( vfBitmapSize );
        }
    
    __CONTROLLER_INT2( "CMccScreenBitmap::StartL, size after",  
                       vfBitmapSize.iWidth, 
                       vfBitmapSize.iHeight )
     
    UpdateViewFinderArea( vfBitmapSize );
    
    __CONTROLLER( "CMccScreenBitmap::StartCameraL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccScreenBitmap::StartDsaL
// -----------------------------------------------------------------------------
//
void CMccScreenBitmap::StartDsaL()
    {
    __CONTROLLER( "CMccScreenBitmap::StartDsaL" )    
    
    iAborted = EFalse;
    iDirectScreenAccess->Cancel();
    iDirectScreenAccess->StartL();
    iGc = iDirectScreenAccess->Gc();    
    iFbsDev = iDirectScreenAccess->ScreenDevice();
    iRegion = iDirectScreenAccess->DrawingRegion();     

#ifdef __MCC_CONTROLLER 
    if ( iRegion && !iRegion->IsEmpty() )
        {
        for ( TInt i = 0; i < iRegion->Count(); i++ )
            {
            const TRect& regionRect = (*iRegion)[ i ];
            __CONTROLLER_INT2( "CMccScreenBitmap::StartDsaL, rect tl", 
                               regionRect.iTl.iX, regionRect.iTl.iY )
            __CONTROLLER_INT2( "CMccScreenBitmap::StartDsaL, rect br", 
                               regionRect.iBr.iX, regionRect.iBr.iY )
            }
        }
#endif          

    iGc->SetClippingRegion( iRegion );
    
    // Fill with black areas which are not covered by viewfinder image.
    // If viewfinder image has not been yet received and drawn, fill whole area.
    //
      
    if ( !iFirstImageDrawn )
        {
        // Whole area
        iGc->SetBrushColor( KRgbBlack );
        iGc->SetBrushStyle( CGraphicsContext::ESolidBrush );
        TPoint pointTl( 0, 0 );
        TRect rect( pointTl, iArea );
        iGc->DrawRect( rect );
        }
    else
        {
        DrawBlackBorders();
        }
        
    if ( iAttachedFrame )
        {
        __CONTROLLER( "CMccScreenBitmap::StartDsaL, draw attached frame" )
        
        TSize viewFinderImageSize = iAttachedFrame->SizeInPixels();
        
        TPoint corner = UpdateViewFinderArea( viewFinderImageSize );
        
        iGc->BitBlt( corner, iAttachedFrame );
        }
        
    DoScreenDeviceUpdate();
    
    //iGc->SetBrushStyle( CGraphicsContext::ENullBrush );
        		
	__CONTROLLER( "CMccScreen::StartL, exit" )
	}

// -----------------------------------------------------------------------------
// CMccScreenBitmap::Stop
// -----------------------------------------------------------------------------
//
void CMccScreenBitmap::Stop()
	{
	__CONTROLLER( "CMccScreenBitmap::Stop" )
	
	// Don't do anything yet, wait for next frame and then stop camera
	iStopped = ETrue;
	
	__CONTROLLER( "CMccScreenBitmap::Stop, exit" )	
	}
	
// -----------------------------------------------------------------------------
// CMccScreenBitmap::Restart
// -----------------------------------------------------------------------------
//
void CMccScreenBitmap::Restart( RDirectScreenAccess::TTerminationReasons /*aReason*/ )
	{
	__CONTROLLER( "CMccScreenBitmap::Restart" )	
	
	TRAP_IGNORE( StartDsaL() );
	}

// -----------------------------------------------------------------------------
// CMccScreenBitmap::AbortNow
// -----------------------------------------------------------------------------
//
void CMccScreenBitmap::AbortNow( RDirectScreenAccess::TTerminationReasons /*aReason*/ )
	{
	__CONTROLLER( "CMccScreenBitmap::AbortNow" )	
	iDirectScreenAccess->Cancel();
	
	iAborted = ETrue;
	}

// -----------------------------------------------------------------------------
// CMccScreenBitmap::Draw
// -----------------------------------------------------------------------------
//
void CMccScreenBitmap::Draw( CFbsBitmap& aFrame )
	{
	if ( iStopped )
	    {
	    __CONTROLLER( "CMccScreenBitmap::Draw, store last frame and stop camera" )   
	    
	    // Screen was waiting for freeze frame from camera, inform observer
	    // that camera is not needed anymore
	    StoreFrameDataL( aFrame );
	    CFbsBitmap* frame = RestoreFrameLC();
	    AttachFrameL( frame );
	    CleanupStack::Pop( frame );

	    StopCamera();
	    __CONTROLLER( "CMccScreenBitmap::Draw, exit" )   
	    }
	else
	    {
        // When normal frame draw is requested, attached frame is not anymore drawn
        DetachFrame();
        
        DoDraw( aFrame );
	    }
	}

// -----------------------------------------------------------------------------
// CMccScreenBitmap::AttachFrameL
// -----------------------------------------------------------------------------
//
void CMccScreenBitmap::AttachFrameL( CFbsBitmap* aFrame )
    {
    __CONTROLLER( "CMccScreenBitmap::AttachFrameL" )	
    
    __ASSERT_ALWAYS( aFrame, User::Leave( KErrArgument ) );
    
    DetachFrame();
    
    iAttachedFrame = aFrame;
    
    DoDraw( *iAttachedFrame );

    __CONTROLLER( "CMccScreenBitmap::AttachFrameL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccScreenBitmap::DetachFrame
// -----------------------------------------------------------------------------
//		
void CMccScreenBitmap::DetachFrame()
    {	
    if ( iAttachedFrame )
        {
        __CONTROLLER( "CMccScreenBitmap::DetachFrame, detaching" )
        iAttachedFrame->Reset();
        delete iAttachedFrame;
        }
        
    iAttachedFrame = NULL;
    }
    
// -----------------------------------------------------------------------------
// CMccScreenBitmap::UpdateViewFinderArea
// -----------------------------------------------------------------------------
//
TPoint CMccScreenBitmap::UpdateViewFinderArea( TSize aViewFinderImageSize )
    {
    TPoint corner( 0, 0 );
	if ( aViewFinderImageSize.iWidth < iArea.iWidth )
	    {
	    // Divide the subtraction by two (i.e. do centering)
	    corner.iX = ( iArea.iWidth - aViewFinderImageSize.iWidth ) >> 1;
	    }
	    
    if ( aViewFinderImageSize.iHeight < iArea.iHeight )
	    {
	    // Divide the subtraction by two (i.e. do centering)
	    corner.iY = ( iArea.iHeight - aViewFinderImageSize.iHeight ) >> 1;
	    }
	    
	iViewFinderImageRect = TRect( corner, aViewFinderImageSize );
	
	return corner;
    }

// -----------------------------------------------------------------------------
// CMccScreenBitmap::DoDraw
// -----------------------------------------------------------------------------
//
void CMccScreenBitmap::DoDraw( CFbsBitmap& aFrame )
    {	
    if ( !iAborted && iGc )
        {
        iFirstImageDrawn = ETrue;
        	
       	TSize viewFinderImageSize = aFrame.SizeInPixels();
        
       	TBool imageSizeChanged = ( viewFinderImageSize != iViewFinderImageRect.Size() );
       	
    	TPoint corner = UpdateViewFinderArea( viewFinderImageSize );
    	
    	if ( imageSizeChanged )
    	    {
    	    // Size of bitmap changed suddenly, borders need to be redrawn
    	    DrawBlackBorders();
    	    }

        iGc->BitBlt( corner, &aFrame );
        
        DoScreenDeviceUpdate();
        }
    }

// -----------------------------------------------------------------------------
// CMccScreenBitmap::DoScreenDeviceUpdate
// -----------------------------------------------------------------------------
//
void CMccScreenBitmap::DoScreenDeviceUpdate()
    {
    if ( iFbsDev )
        {
        iFbsDev->Update();
        }
    }

// -----------------------------------------------------------------------------
// CMccScreenBitmap::StoreFrameDataL
// -----------------------------------------------------------------------------
//
void CMccScreenBitmap::StoreFrameDataL( CFbsBitmap& aFrame )
    {
    __CONTROLLER( "CMccScreenBitmap::StoreFrameDataL" )
    
    iPausedFrameSize = aFrame.SizeInPixels();
    iPausedFrameDisplayMode = aFrame.DisplayMode();
    TInt bitmapSizeInBytes = 
            CFbsBitmap::ScanLineLength( iPausedFrameSize.iWidth, iPausedFrameDisplayMode ) *
            iPausedFrameSize.iHeight;
    HBufC8* pausedFrameData = HBufC8::NewLC( bitmapSizeInBytes );
    aFrame.LockHeap();
    pausedFrameData->Des().Copy( (TUint8*)aFrame.DataAddress(), bitmapSizeInBytes );
    aFrame.UnlockHeap(); 
    delete iPausedFrameData;
    iPausedFrameData = pausedFrameData;
    CleanupStack::Pop( pausedFrameData );
    
    __CONTROLLER( "CMccScreenBitmap::StoreFrameDataL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccScreenBitmap::RestoreFrameLC
// -----------------------------------------------------------------------------
//
CFbsBitmap* CMccScreenBitmap::RestoreFrameLC()
    {
    __CONTROLLER( "CMccScreenBitmap::RestoreFrameLC" )
    
    __ASSERT_ALWAYS( iPausedFrameData, User::Leave( KErrNotReady ) );
    
    CFbsBitmap* tempFrame = new ( ELeave ) CFbsBitmap;
    CleanupStack::PushL( tempFrame );
    
    User::LeaveIfError( tempFrame->Create( iPausedFrameSize, iPausedFrameDisplayMode ) );
    TInt bitmapSizeInBytes = 
            CFbsBitmap::ScanLineLength( iPausedFrameSize.iWidth, iPausedFrameDisplayMode ) *
            iPausedFrameSize.iHeight;
   
    tempFrame->LockHeap();
    Mem::Copy( tempFrame->DataAddress(), 
               iPausedFrameData->Des().Ptr(), 
               bitmapSizeInBytes );
    tempFrame->UnlockHeap();
    
    __CONTROLLER( "CMccScreenBitmap::RestoreFrameLC, exit" )
    
    return tempFrame;
    }

// -----------------------------------------------------------------------------
// CMccScreenBitmap::DrawBlackBorders
// -----------------------------------------------------------------------------
//
void CMccScreenBitmap::DrawBlackBorders()
    {
    iGc->SetBrushColor( KRgbBlack );
    iGc->SetBrushStyle( CGraphicsContext::ESolidBrush );
            
    // Upper area
    TPoint pointTl( 0, 0 );
    TPoint pointBr( iArea.iWidth, iViewFinderImageRect.iTl.iY );
    TRect rect( pointTl, pointBr );
    iGc->DrawRect( rect );
    
    // Lower area
    pointTl.SetXY( 0, iViewFinderImageRect.iBr.iY );
    pointBr.SetXY( iArea.iWidth, iArea.iHeight );
    rect.SetRect( pointTl, pointBr );
    iGc->DrawRect( rect );
    
    // Left area
    pointTl.SetXY( 0, iViewFinderImageRect.iTl.iY );
    pointBr.SetXY( iViewFinderImageRect.iTl.iX, iViewFinderImageRect.iBr.iY );
    rect.SetRect( pointTl, pointBr );
    iGc->DrawRect( rect );
    
    // Right area
    pointTl.SetXY( iViewFinderImageRect.iBr.iX, iViewFinderImageRect.iTl.iY );
    pointBr.SetXY( iArea.iWidth, iViewFinderImageRect.iBr.iY );
    rect.SetRect( pointTl, pointBr );
    iGc->DrawRect( rect );
    }
	
// -----------------------------------------------------------------------------
// CMccScreenDirect::NewL
// -----------------------------------------------------------------------------
//
CMccScreenDirect* CMccScreenDirect::NewL( CCamera* aCamera,
    TPoint aPos, 
    TSize aArea, 
    TInt aIndex, 
    TInt aWindowOrdinalPosition,
    TInt aWindowOrdinalPriority )
    {
    __CONTROLLER( "CMccScreenDirect::NewL, v2" )
    
    CMccScreenDirect* self = new ( ELeave ) CMccScreenDirect( aCamera,
                                                      aPos, 
                                                      aArea, 
                                                      aIndex,
                                                      aWindowOrdinalPosition,
                                                      aWindowOrdinalPriority );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );  
    
    __CONTROLLER( "CMccScreenDirect::NewL, exit" )
    
    return self;
    }

// -----------------------------------------------------------------------------
// CMccScreenDirect::~CMccScreenDirect
// -----------------------------------------------------------------------------
//
CMccScreenDirect::~CMccScreenDirect()
    {
    __CONTROLLER( "CMccScreenDirect::~CMccScreenDirect" )
    
    if ( iDirectViewFinder )
        {
        iDirectViewFinder->Release();
        iDirectViewFinder = NULL;
        }
    
    __CONTROLLER( "CMccScreenDirect::~CMccScreenDirect, exit" )
    }

// -----------------------------------------------------------------------------
// CMccScreenDirect::StartL
// -----------------------------------------------------------------------------
//
void CMccScreenDirect::StartL()
    {
    __CONTROLLER( "CMccScreenDirect::StartL" )
    
    if ( iDirectViewFinder && iDirectViewFinder->ViewFinderState() == 
            CCamera::CCameraDirectViewFinder::EViewFinderPause )
        {
        __CONTROLLER( "CMccScreenDirect::StartL, resuming" )
        
        iDirectViewFinder->ResumeViewFinderDirectL();
        }
    else if ( VfStartPossible() )
        {
        __CONTROLLER( "CMccScreenDirect::StartL, starting, draw with alpha" )
        
		TRect vfRect( TPoint( 0, 0 ), iArea );
        iGraphicsContext->Activate( *iRw ); 
        iRw->Invalidate( vfRect );
        iRw->BeginRedraw( vfRect );
        iGraphicsContext->CancelClippingRect();
        iGraphicsContext->SetBrushStyle( CGraphicsContext::ESolidBrush );
        iGraphicsContext->SetBrushColor( TRgb( 255, 255, 255, 0 ) );
        iGraphicsContext->DrawRect( vfRect );
        iRw->EndRedraw();
        iGraphicsContext->Deactivate();
        iRwSession.Flush();
       
        StartDirectViewFinderL();
        }
    else
        {
        // NOP
        }
    __CONTROLLER( "CMccScreenDirect::StartL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccScreenDirect::Stop
// -----------------------------------------------------------------------------
//
void CMccScreenDirect::Stop()
    {
    __CONTROLLER( "CMccScreenDirect::Stop" )
    
    if ( iDirectViewFinder )
        {
        __CONTROLLER( "CMccScreenDirect::Stop, pausing disabled" )
		// TBD: pausing disabled temporarily due problems in it
        TRAP_IGNORE( iDirectViewFinder->PauseViewFinderDirectL() )
        }
    
    __CONTROLLER( "CMccScreenDirect::Stop, exit" )
    }

// -----------------------------------------------------------------------------
// CMccScreenDirect::SetCamera
// -----------------------------------------------------------------------------
//
TInt CMccScreenDirect::SetCamera( CCamera* aCamera )
    {
    CMccScreen::SetCamera( aCamera );
    
    return HandleDirectViewFinder();
    }

// -----------------------------------------------------------------------------
// CMccScreenDirect::ConstructL
// -----------------------------------------------------------------------------
//
void CMccScreenDirect::ConstructL()
    {
    __CONTROLLER( "CMccScreenDirect::ConstructL" )
        
    CMccScreen::ConstructL();
    
    User::LeaveIfError( HandleDirectViewFinder() );
 
    __CONTROLLER( "CMccScreenDirect::ConstructL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccScreenDirect::CMccScreenDirect
// -----------------------------------------------------------------------------
//
CMccScreenDirect::CMccScreenDirect( 
    CCamera* aCamera,
    TPoint aPos, 
    TSize aArea, 
    TInt aIndex, 
    TInt aWindowOrdinalPosition,
    TInt aWindowOrdinalPriority ) : 
    CMccScreen( EDirectScreen, aCamera, aPos, aArea, aIndex,
                aWindowOrdinalPosition, aWindowOrdinalPriority )
    {
    
    }

// -----------------------------------------------------------------------------
// CMccScreenDirect::StartDirectViewFinderL
// -----------------------------------------------------------------------------
//
void CMccScreenDirect::StartDirectViewFinderL()
    {
    if ( VfStartPossible() )
        {
        // Camera may modify the size
        TSize vfBitmapSize = iArea;
        __CONTROLLER_INT2( "CMccScreenDirect::StartDirectViewFinderL, size before",  
                  vfBitmapSize.iWidth,
                  vfBitmapSize.iHeight )
        
        TPoint point( 0, 0 );
        TRect vfRect( point, vfBitmapSize );
        iCamera->StartViewFinderDirectL( iRwSession, *iDev, *iRw, vfRect );
        
        __CONTROLLER_INT2( "CMccScreenDirect::StartDirectViewFinderL, size after",  
        vfRect.Width(), 
        vfRect.Height() )
        }
    }

// -----------------------------------------------------------------------------
// CMccScreenDirect::HandleDirectViewFinder
// -----------------------------------------------------------------------------
//
TInt CMccScreenDirect::HandleDirectViewFinder()
    {
    TInt err( KErrNone );
    if ( iCamera )
        {
        if ( !iDirectViewFinder )
            {
            __CONTROLLER( "CMccScreenDirect::HandleDirectViewFinder, create dvf" )

            iDirectViewFinder = static_cast<MCameraDirectViewFinder*>(
                iCamera->CustomInterface( TUid::Uid( KECamMCameraDirectViewFinderUidValue ) ) );
            
            if ( !iDirectViewFinder )
                {
                err = KErrNotSupported;
                }
            __CONTROLLER_INT1( "CMccScreenDirect::HandleDirectViewFinder, err:", err )
            }
        }
    else if ( iDirectViewFinder )
        {
        __CONTROLLER( "CMccScreenDirect::HandleDirectViewFinder, release dvf" )
        iDirectViewFinder->Release();
        iDirectViewFinder = NULL;
        }
    else
        {
        // NOP
        }
    return err; 
    }

// End of file

