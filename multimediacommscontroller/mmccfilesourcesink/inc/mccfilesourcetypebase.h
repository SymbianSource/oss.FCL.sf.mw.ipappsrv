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




#ifndef MCCFILESOURCETYPEBASE_H
#define MCCFILESOURCETYPEBASE_H

// INCLUDES
#include <e32base.h>
#include <3gplibrary/mp4lib.h>

#include "CCMRMediaSink.h"
#include "mccfilesourceimpl.h"

class CMMFBuffer;
class MDataSink;
class CMccPeriodicRunner;

// CONSTANTS

class CMccFrameItem : public CBase
	{
	public:
		HBufC8* iFrame;
		TTimeIntervalMicroSeconds iTimeStamp;
		TBool iKeyFrame;
		
		CMccFrameItem()
		    {
		   	}
		   	
		inline ~CMccFrameItem()
			{
			delete iFrame;
			}
	};

/**
 * Base class for all file video and audio.
 */
#ifdef EKA2
NONSHARABLE_CLASS( CMccFileSourceTypeBase ) : public CBase
#else
class CMccFileSourceTypeBase : public CBase
#endif
	{
public:
	virtual ~CMccFileSourceTypeBase();

    virtual void FillBufferL( CMMFBuffer* aBuffer, MDataSink* aConsumer );
    virtual void DoSetPositionL( TUint32 aPosition ) = 0;
    virtual	CCMRMediaBuffer::TBufferType Type() = 0;
    virtual TUint32 Position() = 0;
    virtual TUint32 Duration() = 0;
    virtual TUint32 AudioBitRate() = 0;
    virtual TSize VideoFrameSize() = 0;
    virtual TReal VideoFrameRateL() = 0;
    virtual void StartTimerL() = 0;
    virtual void StopTimer();
    virtual TFourCC GetFourCC() = 0;
    virtual TCallBack TickCallBack() = 0;
    virtual void ResetTimeL();
    
public:

    void SetPositionL( TUint32 aPosition, TBool aResuming = EFalse );
    void Cleanup();
	void SetFileHandle( MP4Handle aMP4Handle );
	void ParseUpdateStreamDescription( MP4Handle aMP4Handle );
    TUint32 StreamAverageBitRate();
    void SetStartTime();
    
    TBool IsPaused() const;
    
    TMediaId MediaType() const;
    
protected:

    TUint16 Random() const;
    
    void SetPaused( TBool aIsPaused );
    
	CMccFileSourceTypeBase( CMccFileSourceImpl& aSource, TMediaId aMediaType );
	
	void ConstructL();
	
	void DoTimingCorrection( TUint32 aCurrentPosition );

protected: // Data
       
    CMccPeriodicRunner* iPeriodicRunner;

    MDataSink*  iConsumer;
	CMMFBuffer* iConsumerBuffer;
	
	MP4Handle iMP4Handle;
	CMccFileSourceImpl& iSource;
	TMediaId iMediaType;
	
	TUint32 iStreamSize;
	TUint32 iStreamAverageBitRate;
    TUint32 iPosition;
	
	TUint32 iTimeDifference; //Difference of timestamps in file and playback
	TUint32 iPausedTime; //How long we have been paused in total 
	TUint32 iReplayTimeStamp;
	TTime iPauseStarted; //Time of last Pause
	TTime iPauseStopped; //Time of last Pause release
    TTime iStartTime;
    
    TUint16 iSequenceNum;
    
    TBool iIsPaused;
    
    TInt64 iPositionModifier;
    TUint32 iReadInterval;
    
    TTime iPreviousCorrection;
    TUint32 iTimerInterval;
    TUint32 iCorrectionInterval;
    
    RPointerArray<CMccFrameItem> iFrames;
    
    TBool iPositionChangedWhilePaused;
    

#ifdef EUNIT_TEST
    friend class UT_CMccFileSourceImpl;
#endif	

	};

#endif // MCCFILESOURCETYPEBASE_H

// end of file
