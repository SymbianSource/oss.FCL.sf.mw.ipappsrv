/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides iLBC payload format spesific definitions.
*
*/



#ifndef ILBCPAYLOADFORMATUTIL_H
#define ILBCPAYLOADFORMATUTIL_H

// INCLUDES

// CONSTANTS
const TUint KiLBCSampleRate = 8000; //8 kHz
const TUint8 KiLBCFrameTime20ms = 20;
const TUint8 KiLBCFrameTime30ms = 30;

const TUint8 KDefNumOfChannels = 1;

// Codec header bytes indicating type of the frame
const TUint8 KAudioFrameHeaderByte( 1 );
const TUint8 KCNoiseFrameHeaderByte( 2 );

// Number of header bytes
const TUint8 KiLBCNumOfHeaderBytes = 2;

// Time stamp increments with iLBC modes
const TUint8 KiLBCTimeStampInc20Ms = 160;
const TUint8 KiLBCTimeStampInc30Ms = 240;

// RFC3952
const TInt KiLBCBitrateWith20ms = 15200;
const TInt KiLBCBitrateWith30ms = 13330;

const TUint8 KiLBCFrameSize20ms   = 38;
const TUint8 KiLBCFrameSize30ms   = 50;

// DATA TYPES
  
// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

#endif      // ILBCPAYLOADFORMATUTIL_H

// End of File
