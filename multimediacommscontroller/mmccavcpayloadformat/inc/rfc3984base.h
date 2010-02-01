/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Class definition for CRFC3984Base
**
*/




#ifndef RFC3984BASE_H
#define RFC3984BASE_H

#include <e32base.h>
#include <e32std.h>
#include <e32def.h>
#include <e32cmn.h>
#include "avcrtpstruct.h"

// AVC packetization modes
#define AVC_MODE_SNALU 0
#define AVC_MODE_NONINTERLEAVED 1
#define AVC_MODE_INTERLEAVED 2


class CRFC3984Base : public CBase
{
	public:
	
	virtual ~CRFC3984Base();
	virtual void SetPacketizationMode(TInt aPacketizationMode);				// Sets the packetization-mode value
	
	
	protected:
	
	CRFC3984Base();	
	
	TInt iPacketizationMode;												// packetization-mode instance


};


// CAvcRTPStruct class, has public member functions used in depayloadization
class CAvcRtpStruct : public CBase
{

public:

	HBufC8 * iDes;					// pointer to data element
	TUint8 iMarkerBit;				// marker bit value
	TUint64 iArrivalTime;			// arrival time of the NAL unit
	TUint64 iTimeStamp;				// timestamp of the NAL unit
	TUint16 iSeqNo;					// Sequence number received from RTP layer
	TInt16 iDon;					// don number for the packet
	TUint 	iPacketType;			// packet types as defined in rtp base 
	TUint8	iFUHeader;				// FU Header (containing start,end, reserve and type field, if required)

public:
	
	inline CAvcRtpStruct() : iDes(NULL), iMarkerBit(0), iArrivalTime(0), iTimeStamp(0), iSeqNo(0), iDon(0), iPacketType(0), iFUHeader(0)
	{ };

};


#endif

// EOF