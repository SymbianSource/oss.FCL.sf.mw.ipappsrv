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



#include <videoplayer.h>
#include <mmfclntutility.h>

CVideoPlayerUtility::~CVideoPlayerUtility()
    {
    }

CVideoPlayerUtility* CVideoPlayerUtility::NewL(MVideoPlayerUtilityObserver& aObserver,
										  TInt aPriority,
										  TInt aPref,
										  RWsSession& aWs,
										  CWsScreenDevice& aScreenDevice,
										  RWindowBase& aWindow,
										  const TRect& aScreenRect,
										  const TRect& aClipRect)
    {
    CVideoPlayerUtility* self = new ( ELeave ) CVideoPlayerUtility;
    return self;
    }

void CVideoPlayerUtility::OpenFileL(const TDesC& aFileName,TUid aControllerUid)
    {
    }
void CVideoPlayerUtility::OpenFileL(const RFile& aFileName, TUid aControllerUid)
    {
    }

void CVideoPlayerUtility::OpenFileL(const TMMSource& aSource, TUid aControllerUid)
    {
    }

void CVideoPlayerUtility::OpenDesL(const TDesC8& aDescriptor,TUid aControllerUid)
    {
    }

void CVideoPlayerUtility::OpenUrlL(const TDesC& aUrl, TInt aIapId, const TDesC8& aMimeType, TUid aControllerUid)
    {
    }

void CVideoPlayerUtility::Prepare()
    {
    }

void CVideoPlayerUtility::Close()
    {
    }

void CVideoPlayerUtility::Play()
    {
    }

void CVideoPlayerUtility::Play(const TTimeIntervalMicroSeconds& aStartPoint, const TTimeIntervalMicroSeconds& aEndPoint)
    {
    }

TInt CVideoPlayerUtility::Stop()
    {
    return KErrNone;
    }

void CVideoPlayerUtility::PauseL()
    {
    }

void CVideoPlayerUtility::SetPriorityL(TInt aPriority, TInt aPref)
    {
    }

void CVideoPlayerUtility::PriorityL(TInt& aPriority, TInt& aPref) const
    {
    }

void CVideoPlayerUtility::SetDisplayWindowL(RWsSession& aWs,CWsScreenDevice& aScreenDevice,RWindowBase& aWindow,const TRect& aWindowRect,const TRect& aClipRect)
    {
    }

void CVideoPlayerUtility::RegisterForVideoLoadingNotification(MVideoLoadingObserver& aCallback)
    {
    }

void CVideoPlayerUtility::GetVideoLoadingProgressL(TInt& aPercentageComplete)
    {
    }

void CVideoPlayerUtility::GetFrameL(TDisplayMode aDisplayMode)
    {
    }

void CVideoPlayerUtility::RefreshFrameL()
    {
    }

TReal32 CVideoPlayerUtility::VideoFrameRateL() const
    {
    return 15;
    }

void CVideoPlayerUtility::SetVideoFrameRateL(TReal32 aFramesPerSecond)
    {
    }

void CVideoPlayerUtility::VideoFrameSizeL(TSize& aSize) const
    {
    }

const TDesC8& CVideoPlayerUtility::VideoFormatMimeType() const
    {
    }

TInt CVideoPlayerUtility::VideoBitRateL() const
    {
    return 64000;
    }

TInt CVideoPlayerUtility::AudioBitRateL() const
    {
    return 12200;
    }

TBool CVideoPlayerUtility::AudioEnabledL() const
    {
    return ETrue;
    }

void CVideoPlayerUtility::SetPositionL(const TTimeIntervalMicroSeconds& aPosition)
    {
    }

TTimeIntervalMicroSeconds CVideoPlayerUtility::PositionL() const
    {
    return TTimeIntervalMicroSeconds( 0 );
    }

TTimeIntervalMicroSeconds CVideoPlayerUtility::DurationL() const
    {
    return TTimeIntervalMicroSeconds( 0 );
    }

void CVideoPlayerUtility::SetVolumeL(TInt aVolume)
    {
    }

TInt CVideoPlayerUtility::Volume() const
    {
    return 50;
    }

TInt CVideoPlayerUtility::MaxVolume() const
    {
    return 100;
    }

void CVideoPlayerUtility::SetBalanceL(TInt aBalance)
    {
    }

TInt CVideoPlayerUtility::Balance()const
    {
    return 50;
    }

void CVideoPlayerUtility::SetRotationL(TVideoRotation aRotation)
    {
    }

TVideoRotation CVideoPlayerUtility::RotationL() const
    {
    return EVideoRotationNone;
    }

void MMMFClientUtility::ReservedVirtual1()
{
}

void MMMFClientUtility::ReservedVirtual2()
{
}

void MMMFClientUtility::ReservedVirtual3()
{
}

void MMMFClientUtility::ReservedVirtual4()
{
}

void MMMFClientUtility::ReservedVirtual5()
{
}

void MMMFClientUtility::ReservedVirtual6()
{
}
