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



#include <videoplayer2.h>
#include <mmfclntutility.h>

CVideoPlayerUtility2::~CVideoPlayerUtility2()
    {
    
    }

CVideoPlayerUtility2* CVideoPlayerUtility2::NewL(
    MVideoPlayerUtilityObserver& /*aObserver*/, TInt /*aPriority*/, TInt /*aPref*/)    
    {
    CVideoPlayerUtility2* self = new ( ELeave ) CVideoPlayerUtility2;
    return self;
    }
                                    
void CVideoPlayerUtility2::AddDisplayWindowL(RWsSession& /*aWs*/, CWsScreenDevice& /*aScreenDevice*/, 
                                RWindow& /*aWindow*/, const TRect& /*aVideoExtent*/, 
                                const TRect& /*aWindowClipRect*/)
    {
    
    }

void CVideoPlayerUtility2::AddDisplayWindowL(
    RWsSession& /*aWs*/, CWsScreenDevice& /*aScreenDevice*/, RWindow& aWindow)
    {
    iTestWindow = &aWindow;
    }

void CVideoPlayerUtility2::RemoveDisplayWindow(RWindow& /*aWindow*/)
    {
    iTestWindow = NULL;
    }

void CVideoPlayerUtility2::SetVideoExtentL(const RWindow& /*aWindow*/, const TRect& /*aVideoExtent*/)
    {
    
    }

void CVideoPlayerUtility2::SetWindowClipRectL(const RWindow& /*aWindow*/, const TRect& /*aWindowClipRect*/)
    {
    
    }

void CVideoPlayerUtility2::SetRotationL(const RWindow& /*aWindow*/, TVideoRotation /*aRotation*/)
    {
    
    }

TVideoRotation CVideoPlayerUtility2::RotationL(const RWindow& /*aWindow*/)
    {
    return EVideoRotationNone;
    }

void CVideoPlayerUtility2::SetScaleFactorL(
    const RWindow& /*aWindow*/, TReal32 /*aWidthPercentage*/, TReal32 /*aHeightPercentage*/)
    {
    
    }

void CVideoPlayerUtility2::GetScaleFactorL(
    const RWindow& /*aWindow*/, TReal32& /*aWidthPercentage*/, TReal32& /*aHeightPercentage*/)
    {
    
    }

void CVideoPlayerUtility2::SetAutoScaleL(const RWindow& /*aWindow*/, TAutoScaleType /*aScaleType*/)
    {
    
    }

void CVideoPlayerUtility2::SetAutoScaleL(
    const RWindow& /*aWindow*/, TAutoScaleType /*aScaleType*/, TInt /*aHorizPos*/, TInt /*aVertPos*/)
    {
    
    }

void CVideoPlayerUtility2::AddDisplayL(
    RWsSession& /*aWs*/, TInt /*aDisplay*/, MMMFSurfaceEventHandler& /*aEventHandler*/)
    {
    
    }

void CVideoPlayerUtility2::RemoveDisplay(TInt /*aDisplay*/)
    {
    
    }
