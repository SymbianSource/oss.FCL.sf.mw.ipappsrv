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




#ifndef MCCFILEAUDIO_H
#define MCCFILEAUDIO_H

// INCLUDES
#include <mmf/common/mmfutilities.h>

#include "mccfilesourcetypebase.h"

// FORWARD DECLARATIONS
class CMccFileSourceImpl;
	

class CMccFileAudio : public CMccFileSourceTypeBase
	{
public:
    static CMccFileAudio* NewL( CMccFileSourceImpl& aSource );


	~CMccFileAudio();

public:
    void FillBufferL( CMMFBuffer* aBuffer, MDataSink* aConsumer );
	void DoSetPositionL( TUint32 aPosition );
 	CCMRMediaBuffer::TBufferType Type();
    TUint32 Position();
    TUint32 Duration();
    TUint32 AudioBitRate();
    TSize VideoFrameSize();
    TReal VideoFrameRateL();
	void StartTimerL();
	TFourCC GetFourCC();
	TCallBack TickCallBack();
	
public:
    static TInt TickAudioL( TAny* aObject );
	TBool ReadFrameL( TBool aDoTimingCorrection );
	
public:

    TBool ParseUpdateAudioDescriptions( MP4Handle aMP4Handle );
    
private:

	CMccFileAudio( CMccFileSourceImpl& aSource );
	void ConstructL();
	
private:

	TTimeIntervalMicroSeconds GetTimeStamp( TUint32 aAddToTimeStamp );
	
	/**
    * Returns the frame lenght from given audio sample
    * @since 
    */
	TUint32 GetFrameLength( TPtrC8 aData );
	
	void WriteBufferL();

private: //data
		
	TUint32 iLength;
	TUint32 iType;
    TUint32 iAverageBitRate;
    TUint32 iBufferSize;
    TUint32 iAudioSize;
    TUint32 iReturnedFrames;

    TBool   iIsAudioLeft;

    TUint8  iFramesPerSample;
    
    TFourCC	iFourCC;

    CCMRMediaBuffer::TBufferType iBufferType;	
    
    TInt iReadsPerInterval;
    
private:
   
	#ifdef EUNIT_TEST
        friend class UT_CMccFileAudio;
        friend class UT_CMccMultiplexer;
        friend class UT_CMccFileSourceImpl;
    #endif	
	};

#endif // MCCFILEAUDIO_H

// end of file
