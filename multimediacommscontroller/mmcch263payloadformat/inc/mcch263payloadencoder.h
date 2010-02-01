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



#ifndef MCCH263PAYLOADENCODER_H
#define MCCH263PAYLOADENCODER_H

// ============================ INCLUDES =======================================

#include <e32base.h>
#include <mmf/server/mmfformat.h>
#include "rtpheader.h"
#include "mccrtpdatasink.h"

const TInt KConst2 = 2;
const TInt KThree = 3;
const TInt KConst3 = 3;
const TInt KFour = 4;
const TInt KConst4 = 4;
const TInt KFive = 5;
const TInt KConst5 = 5;
const TInt KSix = 6;
const TInt KConst6 = 6;
const TInt KSeven = 7;
const TInt KConst7 = 7;
const TInt KEight = 8;
const TInt KConst9 = 9;
const TInt KTen = 10;
const TInt KConst10 = 10;
const TInt KConst13 = 13;
const TInt KConst32 = 32;
const TInt KConst100 = 100;
const TInt KThousand = 1000;

const TUint KMaxPacketSize = 2048;  //maximum data size of RTP packet in bytes
const TUint KAudioPacketSize = 150; //target size for audio packet in bytes. 
const TUint KVideoPacketSize = 920; //target size for video packet in bytes.
const TInt  KPacketTimeOut = 10;    //RTCP time out check interval 
const TInt  KBufferSize = 50;       //maximum number of RTP packets that are
                                    //buffered

// ============================ CLASS DECLARATIONS =============================
/**
*  Implements RFC 2190
*/

class CBufferContainer : public CBase
	{
	public:
		TRtpSendHeader iSendHeader;
		CMMFBuffer* iBuffer;
		
		CBufferContainer(){	};
		inline ~CBufferContainer()
			{
			delete iBuffer;
			}
	};
	
/**
* Structure used to store H263 Header A Information in CIPMediaVideoSink
* @since SWIS Lite
*/ 
class CH263headerA : public CBase
    {
    public:
		TUint8 F;
		TUint8 P;
		TUint8 SBIT;
		TUint8 EBIT;
		TUint8 SRC;
		TUint8 I;
		TUint8 U;
		TUint8 S;
		TUint8 A;
		TUint8 R;
		TUint8 DBQ;
		TUint8 TRB;
		TUint8 TR;
    };

class CMccH263PayloadEncoder: public CBase
	{
	public:
    
	    /**
	    */
	    virtual void EncodeL( CMMFBuffer* aBuffer, 
	    	RPointerArray<CBufferContainer>& aDesBuffers ) = 0;

	    virtual void SetPayloadType( TUint8 aPayloadType ) = 0;
	    
	    CMccH263PayloadEncoder() {};

	};


#endif
