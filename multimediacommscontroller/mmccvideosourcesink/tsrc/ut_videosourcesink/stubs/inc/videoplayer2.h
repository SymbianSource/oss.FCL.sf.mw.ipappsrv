// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#ifndef VIDEOPLAYER2_H
#define VIDEOPLAYER2_H

#include <videoplayer.h>

class MMMFSurfaceEventHandler;
class RWindow;
class CWsScreenDevice;

/**
@publishedPartner
@released

Extension class from CVideoPlayerUtility to allow graphics surface support.
*/
class CVideoPlayerUtility2 : public CVideoPlayerUtility
	{
public:

	~CVideoPlayerUtility2();
	static CVideoPlayerUtility2* NewL(MVideoPlayerUtilityObserver& aObserver,
											  TInt aPriority,
											  TInt aPref);
										
	void AddDisplayWindowL(RWsSession& aWs, CWsScreenDevice& aScreenDevice, 
									RWindow& aWindow, const TRect& aVideoExtent, 
									const TRect& aWindowClipRect);

	void AddDisplayWindowL(RWsSession& aWs, CWsScreenDevice& aScreenDevice, RWindow& aWindow);

 	void RemoveDisplayWindow(RWindow& aWindow);
 	
 	void SetVideoExtentL(const RWindow& aWindow, const TRect& aVideoExtent);
	
	void SetWindowClipRectL(const RWindow& aWindow, const TRect& aWindowClipRect);
	
	void SetRotationL(const RWindow& aWindow, TVideoRotation aRotation);
	
	TVideoRotation RotationL(const RWindow& aWindow);
	
	void SetScaleFactorL(const RWindow& aWindow, TReal32 aWidthPercentage, TReal32 aHeightPercentage);
	
	void GetScaleFactorL(const RWindow& aWindow, TReal32& aWidthPercentage, TReal32& aHeightPercentage);
	
	void SetAutoScaleL(const RWindow& aWindow, TAutoScaleType aScaleType);

    void SetAutoScaleL(const RWindow& aWindow, TAutoScaleType aScaleType, TInt aHorizPos, TInt aVertPos);

    void AddDisplayL(RWsSession& aWs, TInt aDisplay, MMMFSurfaceEventHandler& aEventHandler);
    
    void RemoveDisplay(TInt aDisplay);    

private:
	friend class CTestStepUnitMMFVidClient;
	
public: // STUB STUFF
    
    RWindow* iTestWindow;
    
	};

#endif // VIDEOPLAYER2_H
