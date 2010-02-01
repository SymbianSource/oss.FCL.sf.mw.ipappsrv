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
* Description:    Implementation
*
*/



#include "MCEConstants.h"
#include "TCmdGetAudioCodecInfo.h"
#include "CTcMCEContext.h"
#include <MCEAMRCodec.h>

void TCmdGetAudioCodecInfo::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------

	// Get codec 

	CMceAudioCodec* codec = 
		reinterpret_cast<CMceAudioCodec*>(GetObjectForIdL(KCodecId, ETrue));	

	// ---------- Execution ----------------------------------------------------


	// ---------- Response creation --------------------------------------------
 
 	AddIdResponseL( KCodecId, *codec );
 	
 	AddTextResponseL( KResponseCodecName, codec->SdpName() );

 	AddIntegerResponseL( KResponseBitrates, codec->Bitrate() );
 	
 	AddIntegerResponseL( KResponseSamplingFreq, codec->SamplingFreq() );
 	
 	AddIntegerResponseL( KResponseCodecMode, codec->CodecMode() );

 	AddIntegerResponseL( KResponseFourCC, codec->FourCC() );
 	
  	AddIntegerResponseL( KResponsePayloadType, codec->PayloadType() );
 	
 	AddIntegerResponseL( KResponsePTime, codec->PTime() );

 	AddIntegerResponseL( KResponseMaxPTime, codec->MaxPTime() );
 	
 	AddIntegerResponseL (KResponseAllowedBitRate, codec->AllowedBitrates());

	AddBooleanResponseL( KResponseVAD, codec->VAD() );	

	}
	
TBool TCmdGetAudioCodecInfo::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("GetAudioCodecInfo") );
	}

TTcCommandBase* TCmdGetAudioCodecInfo::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetAudioCodecInfo( aContext );
	}
