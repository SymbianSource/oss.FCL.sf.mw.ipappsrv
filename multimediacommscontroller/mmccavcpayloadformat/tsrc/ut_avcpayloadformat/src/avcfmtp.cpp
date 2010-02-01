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
* Description:    AVC FMTP parameter parsing class.
*
*/




// ============================ INCLUDES =======================================

#include"avcfmtp.h"


// Constant Descriptors for FMTP parameters

_LIT8( KAvcProfileLevelId, "profile-level-id=" );
_LIT8( KAvcPacketizationMode, "packetization-mode=");
_LIT8( KAvcMaxMbps, "max-mbps=");
_LIT8( KAvcMaxFs, "max-fs=");
_LIT8( KAvcMaxCpb, "max-cpb=");
_LIT8( KAvcMaxDpb, "max-dpb=");
_LIT8( KAvcMaxBr, "max-br=");
_LIT8( KAvcRedundantPicCap, "redundant-pic-cap=");
_LIT8( KAvcParameterAdd, "parameter-add=");
_LIT8( KAvcSpropInterleavingDepth, "sprop-interleaving-depth=");
_LIT8( KAvcSpropDeintBufReq, "sprop-deint-buf-req=");
_LIT8( KAvcSpropBufCap, "sprop-buf-cap=");
_LIT8( KAvcSpropInitBufTime, "sprop-init-buf-time=");
_LIT8( KAvcSpropMaxDonDiff, "sprop-max-don-diff=");
_LIT8( KAvcMaxRcmdNaluSize, "max-rcmd-nalu-size=");
_LIT8( KAvcSpropParameterSets, "sprop-parameter-sets=");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CRFC3984Base::CRFC3984Base()
// Default Constructor
// Initializes the members
// -----------------------------------------------------------------------------

CAVCFmtp::CAVCFmtp()
{
//	iProfileIdc = '42';		// put 42 in some constant and assign here.
//	iLevelIdc = '11'; 	  	// level 1.1 basically (level = iLevelIdc/10)
//	iProfileIop = '31';	  	// profile IOP bits, indicates 3 flags and the rest 5-bits are zeros.
	
	
	
	// packetization mode
	iPacketizationMode = ESingleNal;
	
	iSprop_interleaving_depth = 9;   // 9 NAL Unit interleaving depth		
	
	iParameterSetCount = 0;			 // initialized to 0 parameter sets
} 

CAVCFmtp::~CAVCFmtp()
{
	TDes8 * pDes = NULL;
	TInt entries = iArrayBase64.Count( );
	if(entries > 0)
	{
		for(int count = 0; count < entries; count++)
		{
			pDes = iArrayBase64[count];
			if(pDes) 
			{
				delete pDes;
				pDes = NULL;
			}
		}
	}

	iArrayBase64.Reset( );
	iArrayParameterSets.Reset( );
	
}

// -----------------------------------------------------------------------------
// CAVCFmtp::NewL()
// First stage constructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

CAVCFmtp * CAVCFmtp::NewL()
{
	CAVCFmtp * self = new(ELeave) CAVCFmtp;
	CleanupStack::PushL( self );
    self->ConstructL( /*initialazation parameters*/ );
    CleanupStack::Pop();
    return self;		
}  

// -----------------------------------------------------------------------------
// CAVCFmtp::ConstructL()
// Initializing common members
// -----------------------------------------------------------------------------
//

void CAVCFmtp::ConstructL()
{
	//	iProfileIdc = '42';		// put 42 in some constant and assign here.
	//	iLevelIdc = '11'; 	  	// level 1.1 basically (level = iLevelIdc/10)
	//	iProfileIop = '31';	  	// profile IOP bits, indicates 3 flags and the rest 5-bits are zeros.
	
	
	
	// packetization mode
	iPacketizationMode = ESingleNal;
	
	iSprop_interleaving_depth = 9;   // 9 NAL Unit interleaving depth	
	
	iParameterSetCount = 0;	
}

// -----------------------------------------------------------------------------
// CAVCFmtp::ConstructL()
// Purpose     : This function parses the FMTP string passed to it. Stores values in the class for future use
// Parameters  : aString - The string containing the FMTP parameters
// Return Value: ETrue if correct parsing done, EFalse if an error occured
// -----------------------------------------------------------------------------
//

TBool CAVCFmtp::ParseFmtpL(TDes8 * aString)
{

	TInt posVal = 0;
	TInt value = 0; 				// Variable to store the value read from the string and then is inserted into the respective fields
	TLex8 correctSpot;
	
	// searching for packetization mode field
	posVal = aString->Find(KAvcPacketizationMode);
	if(KErrNotFound == posVal){
		iPacketizationMode = ESingleNal;
	}
	else{
		TInt packMode = 0;
		posVal += KPMLength; 			// moving to the value for this field
		correctSpot = aString->Mid(posVal, 2);
		correctSpot.Val(packMode);
		if(packMode >= 0 || packMode < 3)
			iPacketizationMode = (TAVCPktMode)packMode;
	}
	
	// searching for profile-level-id field
	posVal = 0;
	posVal = aString->Find(KAvcProfileLevelId);
	if(KErrNotFound != posVal){
		TUint16 profileLevelId = 0;
		posVal += KPLIDLength; 				// moving to the value for this field
		correctSpot = aString->Mid(posVal, 2);
		correctSpot.Val(profileLevelId, EHex);
		if(profileLevelId == 0)
			iProfileIdc = 0x42;
		else
			iProfileIdc = profileLevelId;
		
		correctSpot = aString->Mid(posVal+2, 2); 	// next reading profile IOP
		correctSpot.Val(profileLevelId, EHex);
		if(profileLevelId == 0)
			iProfileIop = 0xA0;
		else
			iProfileIop = profileLevelId;
		
		correctSpot = aString->Mid(posVal+4, 2);	// reading level 
		correctSpot.Val(profileLevelId, EHex);
		if(profileLevelId == 0)
			iLevelIdc = 10;
		else
			iLevelIdc = profileLevelId;
	} 
	
	// searching for AVC max-mbps field
	posVal = 0;
	posVal = aString->Find(KAvcMaxMbps);
	if(KErrNotFound != posVal){
		value = 0;	
		posVal += KMMbpsLength; 				// moving to the value for this field
		correctSpot = aString->Mid(posVal, 2);
		correctSpot.Val(value);
		if(value > 0)
			iMax_mbps = value;
	}
	else
		iMax_mbps = 0;
		
	// searching for the AVC max-fs field
	posVal = 0;
	posVal = aString->Find(KAvcMaxFs);
	if(KErrNotFound != posVal){
		value = 0;	
		posVal += KMFsLength; 				// moving to the value for this field
		correctSpot = aString->Mid(posVal, 2);
		correctSpot.Val(value);
		if(value > 0)
			iMax_fs = value;
	}
	else
		iMax_fs = 0;
		
		
	// searching for the AVC max-cpb size field
	posVal = 0;
	posVal = aString->Find(KAvcMaxCpb);
	if(KErrNotFound != posVal){
		value = 0;	
		posVal += KMCpbLength; 				// moving to the value for this field
		correctSpot = aString->Mid(posVal, 2);
		correctSpot.Val(value);
		if(value > 0)
			iMax_cpb = value;
	}
	else
		iMax_cpb = 0;
	
	// searching for the AVC max-dpb field
	posVal = 0;
	posVal = aString->Find(KAvcMaxDpb);
	if(KErrNotFound != posVal){
		value = 0;	
		posVal += KMDpbLength; 				// moving to the value for this field
		correctSpot = aString->Mid(posVal, 2);
		correctSpot.Val(value);
		if(value > 0)
			iMax_dpb = value;
	}
	else
		iMax_dpb = 0;
	
	
	// searching for the AVC max-br field
	posVal = 0;
	posVal = aString->Find(KAvcMaxBr);
	if(KErrNotFound != posVal){
		value = 0;	
		posVal += KMBrLength; 				// moving to the value for this field
		correctSpot = aString->Mid(posVal, 2);
		correctSpot.Val(value);
		if(value > 0)
			iMax_br = value;
	}
	else
		iMax_br = 0;
		
	// searching for the AVC Redundant picture capability field
	posVal = 0;
	posVal = aString->Find(KAvcRedundantPicCap);
	if(KErrNotFound != posVal){
		value = 0;	
		posVal += KRedPicCapLength; 				// moving to the value for this field
		correctSpot = aString->Mid(posVal, 2);
		correctSpot.Val(value);
		if(value == 1)
			iRedundant_pic_cap = ETrue;
	}
	else
		iRedundant_pic_cap = EFalse;
		
	// searching for the AVC parameter-add field
	posVal = 0;
	posVal = aString->Find(KAvcParameterAdd);
	if(KErrNotFound != posVal){
		value = 0;	
		posVal += KParAddLength; 				// moving to the value for this field
		correctSpot = aString->Mid(posVal, 2);
		correctSpot.Val(value);
		if(value == 1)
			iParameter_add = ETrue;
	}
	else
		iParameter_add = EFalse;
	
	// searching for the AVC sprop-interleaving-depth parameter (required for interleaved mode)
	posVal = 0;
	posVal = aString->Find(KAvcSpropInterleavingDepth);
	if(KErrNotFound != posVal){
		value = 0;	
		posVal += KInterDepthLength; 				// moving to the value for this field
		correctSpot = aString->Mid(posVal, 2);
		correctSpot.Val(value);
		if(value > 0)
			iSprop_interleaving_depth = value;
	}
	else
		iSprop_interleaving_depth = 0;
	
	// searching for the AVC sprop-deint-buf-req field
	posVal = 0;
	posVal = aString->Find(KAvcSpropDeintBufReq);
	if(KErrNotFound != posVal){
		value = 0;	
		posVal += KDeintBufReqLength; 				// moving to the value for this field
		correctSpot = aString->Mid(posVal, 2);
		correctSpot.Val(value);
		if(value > 0)
			iSprop_deint_buf_req = value;
	}
	else
		iSprop_deint_buf_req = 0;
		
	// searching for the AVC sprop-buf-cap field
	posVal = 0;
	posVal = aString->Find(KAvcSpropBufCap);
	if(KErrNotFound != posVal){
		value = 0;	
		posVal += KBufCapLength; 				// moving to the value for this field
		correctSpot = aString->Mid(posVal, 2);
		correctSpot.Val(value);
		if(value > 0)
			iSprop_buf_cap = value;
	}
	else
		iSprop_buf_cap = 0;
	
	// searching for the AVC sprop-init-buf-time parameter
	posVal = 0;
	posVal = aString->Find(KAvcSpropInitBufTime);
	if(KErrNotFound != posVal){
		value = 0;	
		posVal += KInitBufTimeLength; 				// moving to the value for this field
		correctSpot = aString->Mid(posVal, 2);
		correctSpot.Val(value);
		if(value > 0)
			iSprop_init_buf_time = value;
	}
	else
		iSprop_init_buf_time = 0;
		
	// searching for the AVC sprop-max-don-diff parameter
	posVal = 0;
	posVal = aString->Find(KAvcSpropMaxDonDiff);
	if(KErrNotFound != posVal){
		value = 0;	
		posVal += KMaxDonDiffLength; 				// moving to the value for this field
		correctSpot = aString->Mid(posVal, 2);
		correctSpot.Val(value);
		if(value > 0)
			iSprop_max_don_diff = value;
	}
	else
		iSprop_max_don_diff = 0;
	
	// searching for the AVC max-rcmd-nalu-size parameter
	posVal = 0;
	posVal = aString->Find(KAvcMaxRcmdNaluSize);
	if(KErrNotFound != posVal){
		value = 0;	
		posVal += KMaxRcmdNaluLength; 				// moving to the value for this field
		correctSpot = aString->Mid(posVal, 2);
		correctSpot.Val(value);
		if(value > 0)
			iMax_rcmd_nalu_size = value;
	}
	else
		iMax_rcmd_nalu_size = 0;
	
	// searching for the AVC sprop-parameter-sets field
	// TODO :: This parsing is left
	posVal = 0;
	posVal = aString->Find(KAvcSpropParameterSets);
	if(KErrNotFound != posVal){
		TChar chr = 'a';
		TDes8 * pDes = new (ELeave) TBuf8<KMaxParameterSetLength>;   // currently 100 bytes
		TBool tempBool = EFalse;
		
		posVal += KParameterSetsLength;              // moving to the value for the field	
		correctSpot = aString->Mid(posVal);				 // points to starting of values
		while(chr != ';' && chr != ' ')
		{
			tempBool = ETrue;
			chr = correctSpot.Get();
		
			if(chr == ',')
			{
				iArrayBase64.Insert(pDes, iParameterSetCount);		// inserting parameter-set in the array
				iParameterSetCount++;								// incrementing parameter set count
				pDes = new TBuf8<KMaxParameterSetLength>;			// allocating memory for new parameter set
				continue;
			}
			
			if(chr == ';' || chr == ' ') continue;					// skipping the last character
		
	
			// copying data to the buffer
			pDes->Append(chr);
	
			if(correctSpot.Eos())									// if next character is end of string, break
				break;
		}  // end while()		
		
		if(tempBool)
		{
			iArrayBase64.Insert(pDes, iParameterSetCount);		// inserting last parameter-set in the array
			iParameterSetCount++;								// incrementing parameter set count
			tempBool = EFalse;	
		}
		
		if(pDes)
		{
			delete pDes;
		}		
		
	} // if(KErrNotFound != posVal) -- for parameter sets
	
	
	
	return ETrue;
	
}


// -----------------------------------------------------------------------------
// CAVCFmtp::SetPacketizationMode(TInt aPacketizationMode)
// Purpose     : This function sets the value of the packetization-mode parameter
// Parameters  : aPacketizationMode - The packetization mode value between 0 and 2 inclusive
// Return Value: None
// -----------------------------------------------------------------------------
//

void CAVCFmtp::SetPacketizationMode(TInt aPacketizationMode)
{
	iPacketizationMode = (TAVCPktMode)aPacketizationMode;	
}

// -----------------------------------------------------------------------------
// TInt CAVCFmtp::GetPacketizationMode(void)
// Purpose     : This function returns the value of the packetization-mode parameter
// Parameters  : None
// Return Value: Returns the value of the packetization mode
// -----------------------------------------------------------------------------
//

TInt CAVCFmtp::GetPacketizationMode(void)
{
	return((TInt)iPacketizationMode);	
}

// -----------------------------------------------------------------------------
// CAVCFmtp::SetProfileIdc(TInt aProfileIdc)
// Purpose     : This function sets the value of the profile Idc field that goes in the profile-level-id field
// Parameters  : aProfileIdc - profile Idc value (baseline profile = 42hex)
// Return Value: None
// -----------------------------------------------------------------------------
//		
			        
void CAVCFmtp::SetProfileIdc(TInt aProfileIdc)
{
	iProfileIdc = aProfileIdc;	
}	

// -----------------------------------------------------------------------------
// TInt CAVCFmtp::GetProfileIdc(void)
// Purpose     : This function returns the value of the profile-idc field parameter
// Parameters  : None
// Return Value: Returns the value of profile-idc
// -----------------------------------------------------------------------------
//
		    
TInt CAVCFmtp::GetProfileIdc(void)
{
	return(iProfileIdc);	
}

// -----------------------------------------------------------------------------
// CAVCFmtp::SetProfileIop(TInt aProfileIop)
// Purpose     : This function sets the value of the profile Iop field that goes in the profile-level-id field
// Parameters  : aProfileIop - profile Iop value (msb three bits are constraint flags - lsb 5 bits are always zero)
// Return Value: None
// -----------------------------------------------------------------------------
//	
								
void CAVCFmtp::SetProfileIop(TInt aProfileIop)
{
	iProfileIop = aProfileIop;	
}

// -----------------------------------------------------------------------------
// TInt CAVCFmtp::GetProfileIop(void)
// Purpose     : This function returns the value of the profile-iop field parameter
// Parameters  : None
// Return Value: Returns the value of profile-iop
// -----------------------------------------------------------------------------
//

TInt CAVCFmtp::GetProfileIop(void)
{
	return(iProfileIop);	
}						

// -----------------------------------------------------------------------------
// CAVCFmtp::SetLevelIdc(TInt aLevel)
// Purpose     : This function sets the value of the level idc field that goes in the profile-level-id field
// Parameters  : aLevel - Level Value ( aLevel = actual_level * 10, so if level is 1.1, pass 11)
// Return Value: None
// -----------------------------------------------------------------------------
//	
		
void CAVCFmtp::SetLevelIdc(TInt aLevel)
{
	iLevelIdc = aLevel;	
}	

// -----------------------------------------------------------------------------
// TInt CAVCFmtp::GetLevelIdc(void)
// Purpose     : This function returns the value of the level idc field parameter
// Parameters  : None
// Return Value: Returns the value of level idc
// -----------------------------------------------------------------------------
//
						
TInt CAVCFmtp::GetLevelIdc(void)
{
	return(iLevelIdc);	
}			

// -----------------------------------------------------------------------------
// CAVCFmtp::SetMaxMbps(TInt aMaxMbps)
// Purpose     : This function sets the value of the max macroblock per second field (max-mbps)
// Parameters  : aMaxMbps - max-mbps value
// Return Value: None
// -----------------------------------------------------------------------------
//	
						
void CAVCFmtp::SetMaxMbps(TInt aMaxMbps)
{
	iMax_mbps = aMaxMbps;
}

// -----------------------------------------------------------------------------
// TInt CAVCFmtp::GetMaxMbps(void)
// Purpose     : This function returns the value of the max-mbps field parameter
// Parameters  : None
// Return Value: Returns the value of max-mbps
// -----------------------------------------------------------------------------
//
						
TInt CAVCFmtp::GetMaxMbps(void)
{
	return(iMax_mbps);	
}		

// -----------------------------------------------------------------------------
// CAVCFmtp::SetMaxFs(TInt aMaxFs)
// Purpose     : This function sets the value of the max. frame size field (in terms of number of macroblocks)
// Parameters  : aMaxFs - max-fs value
// Return Value: None
// -----------------------------------------------------------------------------
//	
							
void CAVCFmtp::SetMaxFs(TInt aMaxFs)
{
	iMax_fs = aMaxFs;	
}		

// -----------------------------------------------------------------------------
// TInt CAVCFmtp::GetMaxFs(void)
// Purpose     : This function returns the value of the max-fs field
// Parameters  : None
// Return Value: Returns the value of max-fs (in terms of number of macroblocks)
// -----------------------------------------------------------------------------
//
						
TInt CAVCFmtp::GetMaxFs(void)
{
	return(iMax_fs);	
}					

// -----------------------------------------------------------------------------
// CAVCFmtp::SetMaxCpb(TInt aMaxCpb)
// Purpose     : This function sets the value of the maximum codec picture buffer
// Parameters  : aMaxCpb - maximum codec picture buffer size in units of 1000 bits
// Return Value: None
// -----------------------------------------------------------------------------
//	
				
void CAVCFmtp::SetMaxCpb(TInt aMaxCpb)
{
	iMax_cpb = aMaxCpb;	
}

// -----------------------------------------------------------------------------
// TInt CAVCFmtp::GetMaxCpb(void)
// Purpose     : This function returns the value of the max-cpb field
// Parameters  : None
// Return Value: Returns the value of max-cpb (in units of 1000 bits)
// -----------------------------------------------------------------------------
//
						
TInt CAVCFmtp::GetMaxCpb(void)
{
	return(iMax_cpb);	
}	

// -----------------------------------------------------------------------------
// CAVCFmtp::SetMaxDpb(TInt aMaxDpb)
// Purpose     : This function sets the value of the maximum decoded picture buffer
// Parameters  : aMaxDpb - maximum decoded picture buffer size in units of 1024 bytes
// Return Value: None
// -----------------------------------------------------------------------------
//	
								
void CAVCFmtp::SetMaxDpb(TInt aMaxDpb)
{
	iMax_dpb = aMaxDpb;	
}						

// -----------------------------------------------------------------------------
// TInt CAVCFmtp::GetMaxDpb(void)
// Purpose     : This function returns the value of the max-dpb field
// Parameters  : None
// Return Value: Returns the value of max-dpb (in units of 1024 bytes)
// -----------------------------------------------------------------------------
//
	
TInt CAVCFmtp::GetMaxDpb(void)
{
	return(iMax_dpb);	
}					

// -----------------------------------------------------------------------------
// CAVCFmtp::SetMaxBr(TInt aMaxBr)
// Purpose     : This function sets the value of the maximum bit rate (max-br) field
// Parameters  : aMaxBr - max. bit rate in units of 1000 bits for VCL and 1200 bits for NAL
// Return Value: None
// -----------------------------------------------------------------------------
//	
				
void CAVCFmtp::SetMaxBr(TInt aMaxBr)
{
	iMax_br = aMaxBr;
}	

// -----------------------------------------------------------------------------
// TInt CAVCFmtp::GetMaxBr(void)
// Purpose     : This function returns the value of the max-br field
// Parameters  : None
// Return Value: Returns the value of max-br (in units of 1000 bits for VCL and 1200 bits for NAL)
// -----------------------------------------------------------------------------
//
							
TInt CAVCFmtp::GetMaxBr(void)
{
	return(iMax_br);	
}	

// -----------------------------------------------------------------------------
// CAVCFmtp::SetReduandantPicCap(TBool aRedPicCap)
// Purpose     : This function sets the value of the Redundant Pic Capability flag
// Parameters  : aRedPicCap - value of redundant pic capability flag, if set implies sender can send redundant pics to receiver
// Return Value: None
// -----------------------------------------------------------------------------
//	
								
void CAVCFmtp::SetReduandantPicCap(TBool aRedPicCap)
{
	iRedundant_pic_cap = aRedPicCap;	
}			

// -----------------------------------------------------------------------------
// TBool CAVCFmtp::GetRedundantPicCap(void)
// Purpose     : This function returns the value of the redundant-pic-cap field
// Parameters  : None
// Return Value: Returns the value of the redundant picture capability field
// -----------------------------------------------------------------------------
//
TBool CAVCFmtp::GetRedundantPicCap(void)
{
	return(iRedundant_pic_cap);	
}							

// -----------------------------------------------------------------------------
// CAVCFmtp::SetParameterAdd(TBool aParameterAdd)
// Purpose     : This function changes the value of the parameter-add flag
// Parameters  : aParameterAdd - value of parameter add flag, default is 1.
// Return Value: None
// -----------------------------------------------------------------------------
//	
void CAVCFmtp::SetParameterAdd(TBool aParameterAdd){
	iParameter_add = aParameterAdd;	
}		

// -----------------------------------------------------------------------------
// TBool CAVCFmtp::GetParameterAdd(void)
// Purpose     : This function returns the value of the parameter-add field
// Parameters  : None
// Return Value: Returns the value of the parameter-add field
// -----------------------------------------------------------------------------
//
	
TBool CAVCFmtp::GetParameterAdd(void)
{
	return(iParameter_add);	
}		

// -----------------------------------------------------------------------------
// CAVCFmtp::SetSpropInterleavingDepth(TInt aInterleavingDepth)
// Purpose     : This function sets the value of the interleavind depth field. This field
//				 is used in the interleaved mode, to determine the interleaving depth of 
//				 the NAL Units.
// Parameters  : aInterleavingDepth - Interleavind depth value, used in interleaved mode
// Return Value: None
// -----------------------------------------------------------------------------
//						
void CAVCFmtp::SetSpropInterleavingDepth(TInt aInterleavingDepth)
{
	iSprop_interleaving_depth = aInterleavingDepth;
}

// -----------------------------------------------------------------------------
// TInt CAVCFmtp::GetSpropInterleavingDepth(void)
// Purpose     : This function returns the value of the interleaving depth field
// Parameters  : None
// Return Value: Returns the value of the interleaving depth field
// -----------------------------------------------------------------------------
//

TInt CAVCFmtp::GetSpropInterleavingDepth(void)
{
	return(iSprop_interleaving_depth);
}

// -----------------------------------------------------------------------------
// CAVCFmtp::SetSpropDeintBufReq(TInt aReqSize)
// Purpose     : This function sets the value of the deinterleaving buffer size required
//				 for properly deinterleaving the stream. This field is used in the interleaved 
//				 mode.
// Parameters  : aReqSize - size of buffer in units of bytes
// Return Value: None
// -----------------------------------------------------------------------------
//	
					
void CAVCFmtp::SetSpropDeintBufReq(TInt aReqSize)
{
	iSprop_deint_buf_req = aReqSize;	
}	

// -----------------------------------------------------------------------------
// TInt CAVCFmtp::GetSpropDeintBufReq(void)
// Purpose     : This function returns the value of the deinterleaving buffer size
// 				 which the remote end should have to support interleaved stream
//				 having interleaving depth.
// Parameters  : None
// Return Value: Returns the value of the deinterleaving buffer size required at remote end
// -----------------------------------------------------------------------------
//
			
TInt CAVCFmtp::GetSpropDeintBufReq(void)
{
	return(iSprop_deint_buf_req);	
}	

// -----------------------------------------------------------------------------
// CAVCFmtp::SetSpropBufCap(TInt aBufSpace)
// Purpose     : This function sets the value of the deinterleaving buffer size 
//				 of the sender system. This tells the receiver how much buffering 
//				 the sender has for its deinterleaving buffer.
// Parameters  : aBufSpace - size of buffer in units of bytes
// Return Value: None
// -----------------------------------------------------------------------------
//						
void CAVCFmtp::SetSpropBufCap(TInt aBufSpace)
{
	iSprop_buf_cap = aBufSpace;	
}	

// -----------------------------------------------------------------------------
// TInt CAVCFmtp::GetSpropBufCap(void)
// Purpose     : This function returns the value of the deinterleaving buffer size
// 				 available at the sender. 
//				
// Parameters  : None
// Return Value: Returns the value of the deinterleaving buffer size in bytes
// -----------------------------------------------------------------------------
//
				
TInt CAVCFmtp::GetSpropBufCap(void)
{
	return(iSprop_buf_cap);	
}							

// -----------------------------------------------------------------------------
// CAVCFmtp::SetSpropInitBufTime(TInt aInitBufTime)
// Purpose     : This function sets the value of the buffering time at the reciever
//				 for proper deinterleaving of the stream. This is in units of 90000/s
// Parameters  : aInitBufTime - buffering time at the receiver
// Return Value: None
// -----------------------------------------------------------------------------
//	
	
void CAVCFmtp::SetSpropInitBufTime(TInt aInitBufTime)
{
	iSprop_init_buf_time = aInitBufTime;	
}	

// -----------------------------------------------------------------------------
// TInt CAVCFmtp::GetSpropInitBufTime(void)
// Purpose     : This function returns the value of the initial buffering time
// 				 which the receiver should wait before deinterleaving. In units  
//				 of 90000/s
// Parameters  : None
// Return Value: Returns the value of the initial buffering time
// -----------------------------------------------------------------------------
//		

TInt CAVCFmtp::GetSpropInitBufTime(void)
{
	return(iSprop_init_buf_time);	
}							

// -----------------------------------------------------------------------------
// CAVCFmtp::SetSpropMaxDonDiff(TInt aMaxDonDiff)
// Purpose     : This function sets the value of the sprop-max-don-diff field.
//				 This signifies the maximum difference in DON values of two NALU after
//				 which the receiver can start deinterleaving.
// Parameters  : aMaxDonDiff - max. DON difference after which receiver can start 
//				 deinterleaving.
// Return Value: None
// -----------------------------------------------------------------------------
//	

void CAVCFmtp::SetSpropMaxDonDiff(TInt aMaxDonDiff)
{
	iSprop_max_don_diff = aMaxDonDiff;	
}				

// -----------------------------------------------------------------------------
// TInt CAVCFmtp::GetSpropMaxDonDiff(void)
// Purpose     : This function returns the value of the sprop-max-don-diff field
// 				 
//				 
// Parameters  : None
// Return Value: Returns the value of max. DON difference field
// -----------------------------------------------------------------------------
//		

TInt CAVCFmtp::GetSpropMaxDonDiff(void)
{
	return(iSprop_max_don_diff);	
}		

// -----------------------------------------------------------------------------
// CAVCFmtp::SetMaxRcmdNaluSize(TInt aMaxRcmdNaluSize)
// Purpose     : This function sets the value of the maximum received nalu size
//				 which the sender of this field can easily handle.
//				
// Parameters  : aMaxRcmdNaluSize - max. received nalu size 
//				
// Return Value: None
// -----------------------------------------------------------------------------
//	
					
void CAVCFmtp::SetMaxRcmdNaluSize(TInt aMaxRcmdNaluSize)
{
	iMax_rcmd_nalu_size = aMaxRcmdNaluSize;	
}	

// -----------------------------------------------------------------------------
// TInt CAVCFmtp::GetMaxRcmdNaluSize(void)
// Purpose     : This function returns the value of the maximum received nalu size
// 				 which the sender of this field can easily handle.
//				 
// Parameters  : None
// Return Value: Returns the value of max. received nalu size
// -----------------------------------------------------------------------------
//				
TInt CAVCFmtp::GetMaxRcmdNaluSize(void)
{
	return(iMax_rcmd_nalu_size);	
}							
         
         
// -----------------------------------------------------------------------------
// CAVCFmtp::SetParameterSet(TInt aIndex, TBuf8 & aBuf)
// Purpose     : This function sets the parameter set in an array
//				 and increments the parameter sets count field
//				
// Parameters  : aIndex - Parameter set number (or index)
//				 aBuf - Reference to buffer containing parameter set 
//				
// Return Value: None
// -----------------------------------------------------------------------------
//	

void CAVCFmtp::SetParameterSet(TInt aIndex, TDes8 &aBuf)
{	
			iArrayBase64.Insert(&aBuf, aIndex);		// inserting last parameter-set in the array
			iParameterSetCount++;					// incrementing parameter set count
		
}	

// -----------------------------------------------------------------------------
// TInt CAVCFmtp::GetParameterSet(TInt aIndex)
// Purpose     : This function returns a copy of the parameter set through a pointerif found, otherwise returns
// 				 NULL. The calling function is responsible for deleting the pointer when it does
//				 not require it.
//				 
// Parameters  : aIndex - index of the parameter set to return
// Return Value: Returns the value of max. received nalu size
// -----------------------------------------------------------------------------
//		
	
TDes8 * CAVCFmtp::GetParameterSet(TInt aIndex)
{
	TDes8 * tempBuf = 0;
//	TDes8 * retBuf = 0;
	if(aIndex < 0 || aIndex > iParameterSetCount)
		return 0;									// if index not right, return 0
	
	tempBuf = iArrayBase64[aIndex];					// get pointer to parameter set
//	retBuf = new TBuf8<tempBuf->Size()+1>;			// allocate memory to new buffer to return
//	retBuf.Copy(*tempBuf);							// copy data
	return tempBuf;									// return new buffer
}


// EOF

