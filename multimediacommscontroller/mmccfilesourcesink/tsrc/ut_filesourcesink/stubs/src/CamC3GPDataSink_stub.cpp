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


#include <CamC3GPDataSink.h>
#include "CamC3GPDataSinkImpl_stub.h"


EXPORT_C CCamC3GPDataSink* CCamC3GPDataSink::NewL(M3GPDataSinkObserver* /*aObserver*/)
    {
    return new (ELeave) CCamC3GPDataSinkImlp();
    }

CCamC3GPDataSinkImlp::CCamC3GPDataSinkImlp() : CCamC3GPDataSink()
    {
    }
    
void CCamC3GPDataSinkImlp::OpenFileL(
    TFileName /*aFileName*/, 
    TFourCC /*aAudioCodecType*/, 
    const TDesC8& /*aVideoCodecType*/, 
    TCamCSinkFileFormat /*aFileFormat*/)
    {
    delete HBufC8::NewL( 10 );
    if ( iSizeLimit == KMccFileSinkTestMemoryFullAtBeginning )
        {
        User::Leave( KErrDiskFull );
        }
    }
    
void CCamC3GPDataSinkImlp::OpenFileL(
    CMMFFile* /*aMMFFile*/, 
    TFourCC /*aAudioCodecType*/, 
    const TDesC8& /*aVideoCodecType*/, 
    TCamCSinkFileFormat /*aFileFormat*/)
    {
    delete HBufC8::NewL( 10 );
    if ( iSizeLimit == KMccFileSinkTestMemoryFullAtBeginning )
        {
        User::Leave( KErrDiskFull );
        }
    }
    
void CCamC3GPDataSinkImlp::SetSizeLimit( TUint aSize )
    {
    iSizeLimit = aSize;
    }

TTimeIntervalMicroSeconds CCamC3GPDataSinkImlp::GetElapsedTime()
    {
    return TTimeIntervalMicroSeconds( 0 );
    }

void CCamC3GPDataSinkImlp::SinkStopL()
    {
    delete HBufC8::NewL( 10 );
    }

void CCamC3GPDataSinkImlp::WriteBufferL( CCMRMediaBuffer* aBuffer )
    {
    delete HBufC8::NewL( 10 );
    if ( aBuffer )
        {
        iLastBufferType = aBuffer->Type();
        }
    }

TTimeIntervalMicroSeconds CCamC3GPDataSinkImlp::GetRemainingTimeL()
    {
    delete HBufC8::NewL( 10 );
    return TTimeIntervalMicroSeconds( 0 );
    }

TInt CCamC3GPDataSinkImlp::SetVideoFrameSize(TSize /*aSize*/)
    {
    return KErrNone;
    }
    
TInt CCamC3GPDataSinkImlp::SetAverageVideoBitRate(TInt aBitRate)
    {
    if ( aBitRate == KMccTestGetLastBufferType )
        {
        TInt ret = iLastBufferType;
        iLastBufferType = CCMRMediaBuffer::TBufferType( 0 ); // Reset
        return ret;
        }
    return KErrNone;
    }

TInt CCamC3GPDataSinkImlp::SetMaxVideoBitRate(TInt /*aBitRate*/)
    {
    return KErrNone;
    }

TInt CCamC3GPDataSinkImlp::SetAverageAudioBitRate(TInt /*aBitRate*/)
    {
    return KErrNone;
    }
            
//  END OF FILE
