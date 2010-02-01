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
#include "TCmdModifyAudioCodec.h"
#include "CTcMCEContext.h"

#include <MCEAudioCodec.h>
#include <MCEAMRCodec.h>

void TCmdModifyAudioCodec::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------

	// Get codec 
	CMceAudioCodec* codec = 
		reinterpret_cast<CMceAudioCodec*>(GetObjectForIdL(KCodecId, ETrue));
	/*CMceAmrCodec* codec = 
		reinterpret_cast<CMceAmrCodec*>(GetObjectForIdL(KCodecId, ETrue));*/
	
	// ---------- Execution ----------------------------------------------------


    // VAD
    TBool enableVad = EFalse; 
    TRAPD( err, enableVad = ExtractBooleanL( KParamVAD, ETrue ) );
    if ( err != KTcErrMandatoryParameterNotFound )
        {
        codec->EnableVAD( enableVad );
        }
    
    // Bitrate
    TInt bitrate( 0 );
    TRAP( err, bitrate = ExtractIntegerL( KParamBitrates, 0, ETrue ));
    if ( err != KTcErrMandatoryParameterNotFound )
        {
        codec->SetBitrate( bitrate );
        }   
    
    // SamplingFrequency
    TInt samplingFrequency( 0 );
    TRAP( err, 
          samplingFrequency = ExtractIntegerL( KParamSamplingFreq, 0, ETrue ));
    if ( err != KTcErrMandatoryParameterNotFound )
        {
        codec->SetSamplingFreq( samplingFrequency );
        }   
            
    // PayloadType
    TInt payloadType( 0 );
    TRAP( err, payloadType = ExtractIntegerL( KParamPayloadType, 0, ETrue ));
    if ( err != KTcErrMandatoryParameterNotFound )
        {
        codec->SetPayloadType( payloadType );
        }   
            
    // CodecMode
    TInt codecMode( 0 );
    TRAP( err, codecMode = ExtractIntegerL( KParamCodecMode, 0, ETrue ));
    if ( err != KTcErrMandatoryParameterNotFound )
        {
        codec->SetCodecMode( codecMode );
        }   
            
    // PTime
    TInt pTime( 0 );
    TRAP( err, pTime = ExtractIntegerL( KParamPTime, 0, ETrue ));
    if ( err != KTcErrMandatoryParameterNotFound )
        {
        codec->SetPTime( pTime );
        }   
            
    // MaxPTime
    TInt maxPTime( 0 );
    TRAP( err, maxPTime = ExtractIntegerL( KParamMaxPTime, 0, ETrue ));
    if ( err != KTcErrMandatoryParameterNotFound )
        {
        codec->SetMaxPTime( maxPTime );
        }       
        
    //SetAllowedBiTRates
    TInt allowedBitRate( 0 );
    TRAP(err, allowedBitRate = ExtractIntegerL(KParamAllowedBitRate, 0, ETrue));
    if(err != KTcErrMandatoryParameterNotFound)
    	{
    	codec->SetAllowedBitrates(allowedBitRate);	
    	}
    
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
	
TBool TCmdModifyAudioCodec::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("ModifyAudioCodec") );
	}

TTcCommandBase* TCmdModifyAudioCodec::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdModifyAudioCodec( aContext );
	}
