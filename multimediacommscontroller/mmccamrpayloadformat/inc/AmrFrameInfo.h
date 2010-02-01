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
* Description:    Common Amr codec data utilities
*
*/




#ifndef AMRFRAMEINFO_H
#define AMRFRAMEINFO_H

//  INCLUDES
#include <e32base.h>

// CLASS DECLARATION

/**
* Represents Amr frame information.
*
* @lib      Mccamrplformat.lib
* @since    Series 60 3.1
*/
class TAmrFrameInfo
    {
    
public: // Constructor
    
    TAmrFrameInfo();
        
public: // Data
    
    // encoding mode
    TInt iMode;

    // bitrate ( kbit/s )
    TInt iBitrate;
        
    // sampling frequency ( Hz )
    TInt iSamplingRate;
        
    // number of channels
    TInt iChannels;        
        
    // encoded size ( bytes )
    TInt iFrameSize;
        
    // decoded size ( samples per channel )
    TInt iFrameSamples;

    // sampling frequency after conversion ( Hz )
    TInt iSamplingRateOut; 
        
    // number of audio channels after conversion ( 1 or 2 )
    TInt iChannelsOut;
        
    // decoded size after conversion ( samples per channel )
    TInt iFrameSamplesOut;

    };
    
#include "AmrFrameInfo.inl"

#endif           // AMRFRAMEINFO_H