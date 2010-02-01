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
#include "TCmdGetVideoCodecInfo.h"
#include "CTcMCEContext.h"
#include <MCEvideoCodec.h>

void TCmdGetVideoCodecInfo::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------

	// Get video codec 

	CMceVideoCodec* codec = 
		reinterpret_cast<CMceVideoCodec*>(GetObjectForIdL(KCodecId, ETrue));	

	// ---------- Execution ----------------------------------------------------


	// ---------- Response creation --------------------------------------------
 
 	AddIdResponseL( KCodecId, *codec );
 	
 	AddTextResponseL( KResponseCodecName, codec->SdpName() );
 	
 	AddIntegerResponseL( _L8("MaxBitRate"), codec->MaxBitRate() );
 	
 	AddIntegerResponseL( _L8("AllowedFrameRates"), codec->AllowedFrameRates() );

 	AddIntegerResponseL( _L8("FrameRate"), codec->FrameRate() );
 	
 	AddIntegerResponseL( _L8("FrameSize"), codec->FrameSize() );
 	
 	AddIntegerResponseL( KResponseCodecMode, codec->CodecMode() );
 	
  	AddIntegerResponseL( KResponsePayloadType, codec->PayloadType() );

	}
	
TBool TCmdGetVideoCodecInfo::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("GetVideoCodecInfo") );
	}

TTcCommandBase* TCmdGetVideoCodecInfo::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetVideoCodecInfo( aContext );
	}
