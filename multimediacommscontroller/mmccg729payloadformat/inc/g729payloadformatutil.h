/*
* Copyright (c) 2004-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides G729 payload format spesific definitions.
*
*/




#ifndef G729PAYLOADFORMATUTIL_H
#define G729PAYLOADFORMATUTIL_H

//  INCLUDES
#ifdef _DEBUG
    #include <e32svr.h>
    #define TRACE_G729_PAYLOAD_FORMAT_WRITE
    #define TRACE_G729_PAYLOAD_FORMAT_READ
#endif

// CONSTANTS
const TUint KG729SampleRate = 8000;
const TUint8 KDefNumOfChannels = 1;

// Codec header bytes indicating type of the frame
const TUint8 KAudioFrameHeaderByte( 1 );
const TUint8 KCNoiseFrameHeaderByte( 2 );

// Number of header bytes
const TUint8 KG729NumOfHeaderBytes = 2;

// Size of the speech parameter buffer codec uses
const TUint8 KG729CodecEncBufSize = 24;
const TUint8 KG729CodecDecBufSize = 24;

// Number of 16-bit speech parameters codec uses
const TUint8 KG729NumOfAudioParams = 11;
const TUint8 KG729NumOfCNoiseParams = 4;

// Number of bits used for each codec input/output speech parameter
const TUint8 KG729CodecBufAudioBits[] = { 8, 10, 8, 1, 13, 4, 7, 5, 13, 4, 7 };
const TUint8 KG729CodecBufCNoiseBits[] = { 1, 5, 4, 5 };

// RFC3551
const TInt KG729FrameSize10ms       = 10;
const TInt KG729CNFrameSize         = 2;
const TInt KG729FrameTimeInMs       = 10;

// Bits in one byte
const TUint8 KBitsInByte = 8;

#endif      // G729PAYLOADFORMATUTIL_H
            
// End of File
