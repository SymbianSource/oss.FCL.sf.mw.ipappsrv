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
* Description:    Class definition for AVC Fmtp parameter handling class.
**
*/




#ifndef AVCFMTP_H
#define AVCFMTP_H


#include <E32Base.h>
#include <e32def.h>
#include <e32std.h>
#include <e32cmn.h>
#include <e32des8.h>


// defining the lengths of the above fields (including the = sign)
#define KPMLength 19
#define KPLIDLength 17
#define KMMbpsLength 9
#define KMFsLength 7
#define KMCpbLength 8
#define KMDpbLength 8
#define KMBrLength 7
#define KRedPicCapLength 18
#define KParAddLength 14
#define KInterDepthLength 25
#define KDeintBufReqLength 20
#define KBufCapLength 14
#define KInitBufTimeLength 20
#define KMaxDonDiffLength 19
#define KMaxRcmdNaluLength 19
#define KParameterSetsLength 21
#define KMaxParameterSetLength 100


// Enumerations

enum TAVCPktMode {ESingleNal=0, ENoninterleaved, EInterleaved};


// Classes

class CAVCFmtp : public CBase
{

public:

	
	// public methods
	~CAVCFmtp();
	static CAVCFmtp* NewL();
	TBool ParseFmtpL(TDes8*);
	
	// set and get functions
	void SetPacketizationMode(TInt aPacketizationMode);		// Sets the packetization mode
	TInt GetPacketizationMode(void);					    // Returns the packetization mode
	void SetProfileIdc(TInt aProfileIdc);					// Sets the ProfileIdc field
	TInt GetProfileIdc(void);								// Gets the value of Profile Idc field
	void SetProfileIop(TInt aProfileIop);					// Sets the Profile Iop field
	TInt GetProfileIop(void);								// Gets the value of the Profile Iop filed
	void SetLevelIdc(TInt aLevel);							// Sets the Level Idc field, 'alevel' should be actual level times 10
	TInt GetLevelIdc(void);									// Gets the Level Idc field, actual level = LevelIdc/10;
	void SetMaxMbps(TInt aMaxMbps);							// Sets the max-mbps field (maximum macroblock per sec mb/s)
	TInt GetMaxMbps(void);									// Gets the max-mbps field
	void SetMaxFs(TInt aMaxFs);								// Sets the max-fs field (max. frame size in number of macroblocks)
	TInt GetMaxFs(void);									// Gets the max-fs field
	void SetMaxCpb(TInt aMaxCpb);							// Set the max-cpb field (maximum coded picture buffer in 1000 bits
	TInt GetMaxCpb(void);									// Gets the max-cpb field
	void SetMaxDpb(TInt aMaxDpb);							// Sets the max-dpb field (maximum decoded picture buffer in 1024 bytes)
	TInt GetMaxDpb(void);									// Gets the max-dpb field
	void SetMaxBr(TInt aMaxBr);								// Sets the max-br field (in 1000 bits for VCL and 1200 bits for NAL)
	TInt GetMaxBr(void);									// Gets the max-br field
	void SetReduandantPicCap(TBool aRedPicCap);				// Sets the value of redundant pic capability
	TBool GetRedundantPicCap(void);							// Gets the value of redundant pic capability
	void SetParameterAdd(TBool aParameterAdd);				// Sets the value of the parameter add field
	TBool GetParameterAdd(void);							// Gets the value of the parameter add field
	void SetSpropInterleavingDepth(TInt aInterleavingDepth);// Sets the value of sprop-interleaving-depth field
	TInt GetSpropInterleavingDepth(void);					// Gets the value of the sprop-interleaving-depth field
	void SetSpropDeintBufReq(TInt aReqSize);					// Sets the value of the deinterleaving buffer required field (in bytes)
	TInt GetSpropDeintBufReq(void);							// Gets the value of the sprop-deint-buf-req field
	void SetSpropBufCap(TInt aBufSpace);						// Sets the deint buf space available in sprop-buf-cap field (in bytes)
	TInt GetSpropBufCap(void);								// Gets the value of sprop-buf-cap field
	void SetSpropInitBufTime(TInt aInitBufTime);				// Sets the value of sprop-init-buf-time field (check RFC for setting instructions)
	TInt GetSpropInitBufTime(void);							// Gets the value of sprop-init-buf-time field (in 90000 per seconds for 1 slice/frame)
	void SetSpropMaxDonDiff(TInt aMaxDonDiff);				// Sets the sprop-max-don-diff field
	TInt GetSpropMaxDonDiff(void);							// Gets the sprop-max-don-diff field
	void SetMaxRcmdNaluSize(TInt aMaxRcmdNaluSize);			// Sets the max-rcmd-nalu-size field (in bytes)
	TInt GetMaxRcmdNaluSize(void);							// Gets the max-rcmd-nalu-size 

	// TODO :: Functions for Getting and Setting Parameter sets
	TDes8* GetParameterSet(TInt aIndex);				// Get Parameter set for aIndex. 0 returned if there is nothing to return.
	void SetParameterSet(TInt aIndex, TDes8 &aBuf);    // sets parameter sets for index 'aIndex', increments total parameter set count too


private:


	// methods
	void ConstructL(void);
	CAVCFmtp();
	
	// members
	
	TInt iParameterSetCount;
	RPointerArray<TDes8> iArrayBase64;
	RPointerArray<TDes8> iArrayParameterSets;
		
	// profile level id parameters
	TInt iProfileIdc;		// indicates profile used
	TInt iProfileIop;		// indicates the constraint flags
	TInt iLevelIdc;			// indicates the level of the profile used ( actual level = iLevelIdc / 10)
	
	
	// other fmtp parameters
	TInt iMax_mbps;			// max. macroblock processing rate (mb/s)
	TInt iMax_fs;			// max. frame size (in number of macroblocks)
	TInt iMax_cpb;			// max. coded picture buffer (in 1000 bits)
	TInt iMax_dpb;			// max. decoded picture buffer (1024 bytes)
	TInt iMax_br;			// max. video bitrate (in 1000 bits for VCL and 1200 bits for NAL)
	
	TBool iRedundant_pic_cap;		// bool indicating if reduandant pictures can be added in the stream
	TBool iParameter_add;			// bool indicating if receiver can sends its own parameter sets in the answer
	
	TAVCPktMode iPacketizationMode; // indicates the packetization mode, 0=ESingleNAL, 1=ENoninterleave, 2=EInterleaved
	
		
	// parameters for interleaved mode.
	TInt iSprop_interleaving_depth;			// maximum number of out-of-order NAL units           
	TInt iSprop_deint_buf_req;              // required size of interleaving buffer [bytes]       
	TInt iSprop_buf_cap;                    // deinterleaving buffer space available [bytes]      
	TInt iSprop_init_buf_time;              // initial buffering time for deinterleaving [90000/s]
	TInt iSprop_max_don_diff;               // maximum DON difference allowable                   

	TInt iMax_rcmd_nalu_size;				// largest NALU size handled efficiently  [bytes]
	

	

};



#endif

// EOF