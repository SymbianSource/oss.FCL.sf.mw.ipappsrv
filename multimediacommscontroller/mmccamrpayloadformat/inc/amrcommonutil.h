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
* Description:    Common AMR codec data utilities
*
*/




#ifndef AMRCOMMONUTIL_H
#define AMRCOMMONUTIL_H

//  INCLUDES
#include <e32base.h>
#include "AmrFrameInfo.h"

// CLASS DECLARATION

#ifdef _DEBUG 
    #include <e32svr.h>
    #define _DEBUG_AMR_CHECK_ELEMENT_RANGE
#endif

#ifdef _DEBUG_AMR_CHECK_ELEMENT_RANGE
    #define _DEBUG_AMR_ELEMENT_RANGE_ERROR (a, b) \
    	{ _LIT( KName, a); RDebug::Print(KName, b); }
#else
	#define _DEBUG_AMR_ELEMENT_RANGE_ERROR(a, b)
#endif

const TUint KNumValue0 = 0;
const TUint KNumValue1 = 1;
const TUint KNumValue2 = 2;
const TUint KNumValue3 = 3;
const TUint KNumValue4 = 4;
const TUint KNumValue5 = 5;
const TUint KNumValue8 = 8;
const TUint KNumValue9 = 9;
const TUint KNumValue10 = 10;
const TUint KNumValue11 = 11;
const TUint KNumValue100 = 100;
const TUint KNumValue400 = 400;

const TInt KBitIndex7 = 7;
  

/**
* Common utility functions for AMR encoded data
* 
* @lib      Mccamrplformat.lib
* @since    Series 60 3.0
*/
class CAmrCommonUtility : public CBase
    {
    public:
        /**
        * Calculates frame size and returns information about the frame.
        * @since    Series 60 3.0
        * @param    aBuf Contains frame to be examined at the beginning of the
        *           buffer
        * @param    aBufLen Length of the buffer
        * @param    aInfo - [output] Frame info
        * @param    aIsNb Is AMR narrow band?
        * @return   TInt - 0 if frame bit syntax is incorrect
        */
        static TInt FrameInfo( const TUint8* aBuf, TInt aBufLen, 
                TAmrFrameInfo& aInfo, TBool aIsNb );

        /**
        * Seeks the start position of the next frame and returns the byte
        * position of frames header.
        * @since    Series 60 3.0
        * @param    aBuf Buffer which contains frames
        * @param    aBufLen Length of the buffer
        * @param    aIsNb Is AMR narrow band?
        * @return   TInt - Byte position of the frames header or aBufLen, if no
        *           valid frame can not be found.
        */
        static TInt SeekSync( const TUint8* aBuf, TInt aBufLen, TBool aIsNb );

        /**
        * Returns the size of frame described by given coding parameters.
        * @since    Series 60 3.0
        * @param    aInfo Frame info containing coding parameters
        * @param    aIsNb Is AMR narrow band?
        * @return   TInt - Size of the frame
        */
        static TInt FrameSize( const TAmrFrameInfo& aInfo, TBool aIsNb );
        
        /**
        * Returns the size of one AMR frame header.
        * @since    Series 60 3.0
        * @return   TInt - Size of one AMR frame header in bytes
        */
        static TInt FrameHeaderSize( );
    };

#endif // MCCAMRCOMMONUTIL_H

