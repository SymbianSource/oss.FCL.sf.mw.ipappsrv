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




#ifndef MCCFILEVIDEO_H
#define MCCFILEVIDEO_H

//  INCLUDES
#include <mmf/common/mmfutilities.h>

#include "mccfilesourcetypebase.h"


const TUint KMccReadFrameFailCountMax = 20;

// FORWARD DECLARATIONS
class CMccFileSourceImpl;
class MMccResources;

class CMccFileVideo : public CMccFileSourceTypeBase
	{
public:
    static CMccFileVideo* NewL( CMccFileSourceImpl& aSource, 
                                MMccResources* aMccResources = NULL,
                                TUint32 aEndpointId = 0 );

	~CMccFileVideo();
	
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
	HBufC8* GetConfigKeyL();
	TUint32 VideoType() const;

public:
    static TInt TickVideoL( TAny* aObject );
	void ReadFrameL();
	
public:
    
    TBool ParseUpdateVideoDescriptions( MP4Handle aMP4Handle );
    
private:

 	CMccFileVideo( CMccFileSourceImpl& aSource, 
                   MMccResources* aMccResources,
                   TUint32 aEndpointId );
 	
    void ConstructL();
    	
private:

    /** 
    *Marks NALU boundaries for H264 frames
    */
	void MarkWithNALUDelimiters( const TInt aAccessUnitSize, TDes8& aBuffer );
	
	/**
	* Extracts Seq. Param NALUs & Pic Param NALUs from H264 decoder specific info
	* @param "aBufferOut"
	*		  creates buffer and fills with NALUs, NULL if no NALUs were present
	* @param aSdpFormat, ETrue if sdp format buffer should be returned
	*/
	void ExtractH264ParameterSetNALUsL( HBufC8** aBufferOut, 
	                                    TBool aSdpFormat = EFalse );
	
	/**
	* Check whether parameter set NALUs should be inserted at current moment.
	* @return ETrue is parameter set NALUs should be inserted now
	*/	
	TBool InsertParameterSetNALUs();
	
	TBool IsAVC();
	
	TTimeIntervalMicroSeconds GetTimeStamp();
	
	void WriteBufferL();

private: //data
 
	TUint32 iLength;
	TUint32 iType;
	TUint32 iWidth;
	TUint32 iHeight;
	TUint32 iBufferSize;
	TUint32 iFrameSize;
	TUint32 iLevel;

	TReal   iFrameRate;
	TBool   iIsVideoLeft;
	TBool   iIsFirstFrame;
	TFourCC	iFourCC;
	
	TInt iReadFrameFailCount;
	TBool iSendReadFrameFailEvent;
    
	MMccResources* iMccResources;
	TUint32 iEndpointId;
    CCMRMediaBuffer::TBufferType iBufferType;
    
    HBufC8* iAVCNALUsBuffer;
    TTime iInsertParameterSetsTime;
    TBool iInsertParameterSets;

    
private:   
   	#ifdef EUNIT_TEST
        friend class UT_CMccFileAudio;
    #endif
	};

#endif // MCCFILEVIDEO_H

// end of file
