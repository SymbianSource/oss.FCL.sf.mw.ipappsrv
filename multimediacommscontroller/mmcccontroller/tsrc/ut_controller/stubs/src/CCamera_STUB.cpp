/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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



#include <ecam.h>
#include <fbs.h>
#include <gdi.h>
	
TInt CCamera::CamerasAvailable()
    {
    return 1;
    }

CCamera* CCamera::NewL(MCameraObserver2& aObserver,TInt aCameraIndex,TInt aPriority)
    {
    CCameraStub* stub = CCameraStub::NewL();
    return stub;
    }

CCamera* CCamera::NewL(MCameraObserver& aObserver,TInt aCameraIndex)
    {
    CCameraStub* stub = CCameraStub::NewL();
    return stub;
    }
    
CCamera* CCamera::New2L(MCameraObserver2& aObserver,TInt aCameraIndex,TInt aPriority)
    {
    if ( aPriority == 999 )
        {
        CCameraStub* stub = CCameraStub::NewL();
        return stub;
        }
    else if ( aPriority == 1000 )
        {
        User::Leave( KErrGeneral );
        }
    else
        {
        User::Leave( KErrNotSupported );
        }
    return NULL;
    }

CCameraStub* CCameraStub::NewL()
    {
    CCameraStub* self = new ( ELeave ) CCameraStub;
    CleanupStack::PushL( self );
    self->iVfStub = new ( ELeave ) CDirectViewFinderStub;
    CleanupStack::Pop( self );
    return self;
    }
    
CCameraStub::CCameraStub()
    {
    iInfo.iMaxZoom = 100;
    }
    
CCameraStub::~CCameraStub()
    {
    delete iVfStub;
    }
    
void CCameraStub::CameraInfo(TCameraInfo& aInfo) const
    {
    aInfo = iInfo;
    }

void CCameraStub::Reserve()
    {
    }

void CCameraStub::Release()
    {
    }

void CCameraStub::PowerOn()
    {
    }

void CCameraStub::PowerOff()
    {
    }

TInt CCameraStub::Handle()
    {
    return 1;
    }

void CCameraStub::SetZoomFactorL(TInt aZoomFactor)
    {
    }

TInt CCameraStub::ZoomFactor() const
    {
    return 1;
    }

void CCameraStub::SetDigitalZoomFactorL(TInt aDigitalZoomFactor)
    {
    }

TInt CCameraStub::DigitalZoomFactor() const
    {
    return 2;
    }

void CCameraStub::SetContrastL(TInt aContrast)
    {
    }

TInt CCameraStub::Contrast() const
    {
    return 3;
    }

void CCameraStub::SetBrightnessL(TInt aBrightness)
    {
    }

TInt CCameraStub::Brightness() const
    {
    return 4;
    }

void CCameraStub::SetFlashL(TFlash aFlash)
    {
    }

CCamera::TFlash CCameraStub::Flash() const
    {
    return CCamera::EFlashNone;
    }

void CCameraStub::SetExposureL(TExposure aExposure)
    {
    }

CCamera::TExposure CCameraStub::Exposure() const
    {
    return CCamera::EExposureAuto;
    }

void CCameraStub::SetWhiteBalanceL(TWhiteBalance aWhiteBalance)
    {
    
    }

CCamera::TWhiteBalance CCameraStub::WhiteBalance() const
    {
    return CCamera::EWBAuto;
    }

void CCameraStub::StartViewFinderBitmapsL(TSize& aSize)
    {
    iVfStarted = ETrue;
    }	

void CCameraStub::StartViewFinderBitmapsL(TSize& aSize,TRect& aClipRect)
    {
    iVfStarted = ETrue;
    }

void CCameraStub::StopViewFinder()
    {
    iVfStarted = EFalse;
    }

TBool CCameraStub::ViewFinderActive() const
    {
    return iVfStarted;
    }

void CCameraStub::GetFrameSize(TSize& aSize) const
    {
    }

TReal32 CCameraStub::FrameRate() const
    {
    return 15;
    }

void CCameraStub::StartViewFinderDirectL(
    RWsSession& /*aWs*/,CWsScreenDevice& /*aScreenDevice*/,RWindowBase& /*aWindow*/,TRect& /*aScreenRect*/)
    {
    iVfStarted = ETrue;
    }
            
TAny* CCameraStub::CustomInterface(TUid /*aInterface*/)
    {
    if ( iCustomInterfaceNotAvailable )
        {
        return NULL;
        }
    return static_cast<MCameraDirectViewFinder*>( iVfStub );
    }

CDirectViewFinderStub::CDirectViewFinderStub()
    {
    iVfState = CCamera::CCameraDirectViewFinder::EViewFinderInActive;
    }

CDirectViewFinderStub::~CDirectViewFinderStub()
    {
    
    }

void CDirectViewFinderStub::Release()
    {
    
    }

void CDirectViewFinderStub::PauseViewFinderDirectL()
    {
    iVfState = CCamera::CCameraDirectViewFinder::EViewFinderPause;  
    }
    
void CDirectViewFinderStub::ResumeViewFinderDirectL()
    {
    iVfState = CCamera::CCameraDirectViewFinder::EViewFinderActive;
    }
    
CCamera::CCameraDirectViewFinder::TViewFinderState CDirectViewFinderStub::ViewFinderState() const
    {
    return iVfState;
    }
    
TECAMEvent::TECAMEvent(TUid aEventType, TInt aErrorCode)
    {
    iEventType = aEventType;
    iErrorCode = aErrorCode;
    }
    
TECAMEvent::TECAMEvent()
    {
    }

CMccTestCameraBuffer::CMccTestCameraBuffer()
    {
    }
    
CMccTestCameraBuffer::~CMccTestCameraBuffer()
    {
    delete iBitmap;
    }
    
TInt CMccTestCameraBuffer::NumFrames()
    {
    return iNumFrames;
    }
	
TDesC8* CMccTestCameraBuffer::DataL(TInt aFrameIndex)
    {
    return NULL;
    }

CFbsBitmap& CMccTestCameraBuffer::BitmapL(TInt aFrameIndex)
    {
    if ( !iBitmap )
        {
        CFbsBitmap* bitmap = new ( ELeave ) CFbsBitmap;
        CleanupStack::PushL( bitmap );
        TSize bmpSize( 176, 144 );
        User::LeaveIfError( bitmap->Create( bmpSize, EColor4K ) );
        iBitmap = bitmap;
        CleanupStack::Pop( bitmap );
        }
    return *iBitmap;
    }

RChunk& CMccTestCameraBuffer::ChunkL()
    {
    return *(RChunk*)( iBitmap );
    }

TInt CMccTestCameraBuffer::ChunkOffsetL(TInt aFrameIndex)
    {
    return 0;
    }

TInt CMccTestCameraBuffer::FrameSize(TInt aFrameIndex)
    {
    return 0;
    }

void CMccTestCameraBuffer::Release()
    {
    delete iBitmap;
    iBitmap = NULL;
    }

