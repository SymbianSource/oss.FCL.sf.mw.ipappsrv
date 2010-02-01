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


#ifndef CAMC3GPDATASINKIMPL_H
#define CAMC3GPDATASINKIMPL_H

#include <e32base.h>
#include <CamC3GPDataSink.h>

// If this value is passed in SetSizeLimit method, 
// OpenFileL method will leave with KErrDiskFull
const TUint KMccFileSinkTestMemoryFullAtBeginning = 999999;

// If this value is passed for SetAverageVideoBitRate method,
// return value will tell last received buffer type
const TInt KMccTestGetLastBufferType = 9999;

class CMMFFile;

class CCamC3GPDataSinkImlp : public CCamC3GPDataSink
    {
public:
    
    CCamC3GPDataSinkImlp();

    void OpenFileL(TFileName aFileName, TFourCC aAudioCodecType, const TDesC8& aVideoCodecType, TCamCSinkFileFormat aFileFormat);
    
    void OpenFileL(CMMFFile* aMMFFile, TFourCC aAudioCodecType, const TDesC8& aVideoCodecType, TCamCSinkFileFormat aFileFormat);

    void SinkStopL();

    void SetSizeLimit(TUint aSize);

    TTimeIntervalMicroSeconds GetElapsedTime();

    TTimeIntervalMicroSeconds GetRemainingTimeL();

    TInt SetVideoFrameSize(TSize aSize);

    TInt SetAverageVideoBitRate(TInt aBitRate);

    TInt SetMaxVideoBitRate(TInt aBitRate);

    TInt SetAverageAudioBitRate(TInt aBitRate);

    void WriteBufferL(CCMRMediaBuffer* aBuffer);

private:

    TUint iSizeLimit;
    
    CCMRMediaBuffer::TBufferType iLastBufferType;
    
    };
    
#endif