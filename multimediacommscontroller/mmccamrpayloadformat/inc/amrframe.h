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




#ifndef AMRFRAME_H
#define AMRFRAME_H

#include <e32base.h>

// DATA TYPES

/**
*  AMR frame type ( narrow band ).
*  Digits from enumeration names refer to bit-rate in kbps.
*/
enum TAmrFrameType
    {
    EAmrFrame4_75 = 0,
    EAmrFrame5_15 = 1,
    EAmrFrame5_9 = 2,
    EAmrFrame6_7 = 3,
    EAmrFrame7_4 = 4,
    EAmrFrame7_95 = 5,
    EAmrFrame10_2 = 6,
    EAmrFrame12_2 = 7,
    EAmrFrameSid = 8,       // Silence indicator ( SID ) frame
                            // contains only comfort noise ( CN ) parameters
    EAmrFrameNoData = 15    // No data
    };
/**
*  AMR frame type ( wide band ).
*  Digits from enumeration names refer to bit-rate in kbps.
*/
enum TAmrWbFrameType
    {
    EAmrWbFrame6_60 = 0,
    EAmrWbFrame8_85 = 1,
    EAmrWbFrame12_65 = 2,
    EAmrWbFrame14_25 = 3,
    EAmrWbFrame15_85 = 4,
    EAmrWbFrame18_25 = 5,
    EAmrWbFrame19_85 = 6,
    EAmrWbFrame23_05 = 7,
    EAmrWbFrame23_85 = 8,
    EAmrWbFrameSid = 9,       // Silence indicator ( SID ) frame
                            // contains only comfort noise ( CN ) parameters
    EAmrWbFrameNoData = 15    // No data
    };
    

// CLASS DECLARATION

/**
* The AMR payload decoder for bandwidth efficient mode.
*
* @lib      Mccamrplformat.dll
* @since    Series 60 3.1
*/
class TAmrFrame
    {
    
public:  // Constructors and destructor

    /**
    * Constructor.
    */
    TAmrFrame( );

public:     // Data

    // Frame type ( FT, 4 bits )
    TAmrFrameType iFrameType;

    // Frame quality indicator ( Q, 1 bit ); 1 if frame OK; 0 if frame damaged
    TUint8 iFrameQualityInd;
        
    // CRC ( Cyclic Redundancy Code, 8 bits ) for the frame
    TUint8 iCrc;
        
    // Audio channel ( starts from zero ) which this frame belongs to
    TInt iChannel;
        
    // Pointer to frame data; data length depends on frame type
    TUint8* iFrameData;
        
    // Starting bit index of frame data pointed to by `iFrameData'
    TUint iBitIndex;
    
    };




#include "amrframe.inl"

#endif        //    AMRFRAME_H