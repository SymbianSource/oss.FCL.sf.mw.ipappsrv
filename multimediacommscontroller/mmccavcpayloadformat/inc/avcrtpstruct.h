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
* Description:    Class to hold depayloadized RTP packet information
**				 i.e. depayloadized NAL units with appropriate information
** 			 like timestamp, marker bit etc
**
*/




#ifndef AVCRTPSTRUCT_H
#define AVCRTPSTRUCT_H

#include <e32std.h>

// constants, taken from rfc3984base.h, these are possible values of PT field in the TRTPInfo struct
// NAL packet types
#define	PACKET_NAL_SLICE	1
#define	PACKET_NAL_PARTA	2
#define	PACKET_NAL_PARTB	3
#define	PACKET_NAL_PARTC	4
#define	PACKET_NAL_IDR  	5
#define PACKET_NAL_SEI  	6
#define PACKET_NAL_SPS  	7
#define PACKET_NAL_PPS  	8
#define PACKET_NAL_AUD  	9
#define PACKET_NAL_EOS  	10
#define PACKET_NAL_EOSTR	11
#define PACKET_NAL_FD   	12
#define PACKET_NAL_UNIT 	23
#define PACKET_STAP_A   	24
#define PACKET_STAP_B   	25
#define PACKET_MTAP16   	26
#define PACKET_MTAP24   	27
#define PACKET_FU_A     	28
#define PACKET_FU_B     	29

// AVC information
struct TRTPInfo {
	TInt64 TS;	// this is in microseconds like TTimeIntervalMicroSeconds
	TUint16 SN;
	TUint16 DON;
	TUint16 NRI;
	TUint8	PT;		// packet type for H.264
	TUint8	FUH;	// FU-Header Byte (contains, start, end and reserve bit)
};


// -----------------------------------------------------------------------------
// SeqDiff -- compares two N but sequence numbers.  Returns the distance of 
// the second arguement relative to the first.  E.g. SeqDiff<16>(10, 12) == 2 and
// SeqDiff<16>(12, 10) == -2
//
// N must be <= 32
// -----------------------------------------------------------------------------
//
template < unsigned N >
TInt SeqDiff( TUint aSeqNum1, TUint aSeqNum2 )
{
	struct { TInt s:N; } diff;
	diff.s = aSeqNum2 - aSeqNum1;
	return diff.s;	// force sign extension of a B bit integer to a 32 bit integer
}


#endif 	// AVCRTPSTRUCT_H

