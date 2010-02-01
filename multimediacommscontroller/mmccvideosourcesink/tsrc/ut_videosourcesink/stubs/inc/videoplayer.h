/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef __VIDEOPLAYER_H__
#define __VIDEOPLAYER_H__

#include <mmf/common/mmfbase.h>
#include <mmf/common/mmfvideo.h>
#include <gdi.h>

class CFbsBitmap;
class RWsSession;
class CWsScreenDevice;
class RWindowBase;
class TMMSource;
class TMMFEvent;

/**
@publishedAll
@released

An interface to a set of video player callback functions.

The class is a mixin and is intended to be inherited by the client
class which is observing the video playing operation. The functions
encapsulated by this class are called when specific events occur in
the process of initialising and playing an video clip. A reference to
this object is passed as a parameter when constructing an audio player
utility object.

@since 7.0s
*/
class MVideoPlayerUtilityObserver
	{
public:

	/**
	Notification to the client that the opening of the video clip has completed,
	successfully, or otherwise.

	The status of the video sample after opening is given by aError. The following
	values have the same meaning across all devices:

	- KErrNone: the sample is ready to play;
	- KErrNotSupported: the controller plugin is not recognised or not supported;
	- KErrNotFound: the video sample cannot be found;
	- KErrNoMemory: there is insufficient memory to play this video sample.

	Other values are possible indicating a problem opening the video sample. These values
	are device dependent.

	The client must now call Prepare() on the video player utility before the
	video clip can be played or any of its properties (e.g. duration) can be
	queried.

	@param  aError
	        The status of the video player after initialisation.

	@since  7.0s
	*/
	virtual void MvpuoOpenComplete(TInt aError) = 0;

	/**
	Notification to the client that the opening of the video clip has been prepared
	successfully, or otherwise.  This callback is called in response to a call to
	CVideoPlayerUtility::Prepare().

	The video clip may now be played, or have any of its properties (e.g. duration) queried.

	@param  aError
	        The status of the video player after initialisation.
	        This is either KErrNone if the open has completed successfully,
	        or one of the system wide error codes.
	
	@since  7.0s
	*/
	virtual void MvpuoPrepareComplete(TInt aError) = 0;

	/**
	Notification that the frame requested by a call to GetFrameL is ready

	@param  aFrame
	        The returned frame. The bitmap will contain
	        the requested frame if the the error code is KErrNone (success).
	@param  aError
	        The status of the frame request.
	        This is either KErrNone if the frame request was successful,
	        or one of the system wide error codes.

	@since	7.0s
	*/
	virtual void MvpuoFrameReady(CFbsBitmap& aFrame,TInt aError) = 0;

	/**
	Notification that video playback has completed. This is not called if
	playback is explicitly stopped by calling Stop.

	@param  aError
	        The status of playback.
	        This is either KErrNone if the playback was completed successfully,
	        or one of the system wide error codes.
	@since  7.0s
	*/
	virtual void MvpuoPlayComplete(TInt aError) = 0;
	
	/**
	General event notification from controller. These events are specified by
	the supplier of the controller

	@param  aEvent
	        The event sent by the controller.

	@since  7.0s
	*/
	virtual void MvpuoEvent(const TMMFEvent& aEvent) = 0;
	};
	
/**
@publishedAll
@released

This is a mixin class to allow a client to receive notification of rebuffering
operations occurring in the video player

@since 7.0s
*/
class MVideoLoadingObserver
	{
public:

	/**
	Notification that video clip loading/rebuffering has started

	@since  7.0s
	*/
	virtual void MvloLoadingStarted() = 0;

	/**
	Notification that video clip loading/rebuffering has completed

	@since  7.0s
	*/
	virtual void MvloLoadingComplete() = 0;
	};

class CMMFVideoPlayerCallback;

/**
@publishedAll
@released

Plays sampled video data.

The class offers a simple interface to open, play and obtain information from sampled video data. 
The video data can be provided using files, descriptors or URLs.

Note: 
Some video formats also allow the storing of audio data. To accommodate this, this class contains 
audio functions that can manipulate such data.

While this class is abstract, NewL() constructs, initialises and returns pointers to instances of 
concrete classes derived from this abstract class. This concrete class is part of the MMF 
implementation and is private.

@since 7.0s
*/
class CVideoPlayerUtility : public CBase
	{
public:

	~CVideoPlayerUtility();

	static CVideoPlayerUtility* NewL(MVideoPlayerUtilityObserver& aObserver,
											  TInt aPriority,
											  TInt aPref,
											  RWsSession& aWs,
											  CWsScreenDevice& aScreenDevice,
											  RWindowBase& aWindow,
											  const TRect& aScreenRect,
											  const TRect& aClipRect);

	void OpenFileL(const TDesC& aFileName,TUid aControllerUid = KNullUid);
	void OpenFileL(const RFile& aFileName, TUid aControllerUid = KNullUid);

	void OpenFileL(const TMMSource& aSource, TUid aControllerUid = KNullUid);

	void OpenDesL(const TDesC8& aDescriptor,TUid aControllerUid = KNullUid);

	void OpenUrlL(const TDesC& aUrl, TInt aIapId, const TDesC8& aMimeType=KNullDesC8, TUid aControllerUid = KNullUid);

	void Prepare();

	void Close();

	void Play();

	void Play(const TTimeIntervalMicroSeconds& aStartPoint, const TTimeIntervalMicroSeconds& aEndPoint);

	TInt Stop();

	void PauseL();

	void SetPriorityL(TInt aPriority, TInt aPref);

	void PriorityL(TInt& aPriority, TInt& aPref) const;

	void SetDisplayWindowL(RWsSession& aWs,CWsScreenDevice& aScreenDevice,RWindowBase& aWindow,const TRect& aWindowRect,const TRect& aClipRect);

	void RegisterForVideoLoadingNotification(MVideoLoadingObserver& aCallback);

	void GetVideoLoadingProgressL(TInt& aPercentageComplete);

	void GetFrameL(TDisplayMode aDisplayMode);

	void RefreshFrameL();

	TReal32 VideoFrameRateL() const;

	void SetVideoFrameRateL(TReal32 aFramesPerSecond);

	void VideoFrameSizeL(TSize& aSize) const;

	const TDesC8& VideoFormatMimeType() const;

	TInt VideoBitRateL() const;

	TInt AudioBitRateL() const;

	TBool AudioEnabledL() const;

	void SetPositionL(const TTimeIntervalMicroSeconds& aPosition);

	TTimeIntervalMicroSeconds PositionL() const;

	TTimeIntervalMicroSeconds DurationL() const;

	void SetVolumeL(TInt aVolume);

	TInt Volume() const;

	TInt MaxVolume() const;

	void SetBalanceL(TInt aBalance);

	TInt Balance()const;

	void SetRotationL(TVideoRotation aRotation);

	TVideoRotation RotationL() const;

private:
	enum TMMFVideoPlayerState
		{
		EStopped,
		EOpening,
		EPaused,
		EPlaying		
		};

	};

#endif
