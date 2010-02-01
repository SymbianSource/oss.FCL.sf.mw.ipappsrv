/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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




#include <e32const.h>
#include "MCEConstants.h"
#include "TCmdModifyVideoCodec.h"
#include "CTcMCEContext.h"

#include <MCEVideoCodec.h>

void TCmdModifyVideoCodec::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------

	// Get codec 
	CMceVideoCodec* codec = 
		reinterpret_cast<CMceVideoCodec*>(GetObjectForIdL(KCodecId, ETrue));
	
	// ---------- Execution ----------------------------------------------------
    TInt err;
    
    // Setting MaxBitrate
    TInt maxBitrate( 0 );
    maxBitrate = ExtractIntegerL( KParamMaxBitRate, 0, EFalse );
    if ( maxBitrate != 0 )
        {
        TRAP_IGNORE( codec->SetMaxBitrateL(maxBitrate) )
        }   
        
    //Bitrate    
    TInt bitrate( 0 );
    bitrate = ExtractIntegerL( KParamBitrates, 0, EFalse );
    if ( bitrate != 0 )
        {
        if( codec->SetBitrate(bitrate) == KErrNotSupported )
        	AddIntegerResponseL( KResponseBitrates, KErrNotSupported );
        }  
        
    //AllowedBitrates    
    TInt allowedBitrates( 0 );
    allowedBitrates = ExtractIntegerL( KParamAllowedBitRate, 0, EFalse );
    if ( allowedBitrates != 0 )
        {
        if( codec->SetAllowedBitrates(allowedBitrates) == KErrNotSupported )
        	AddIntegerResponseL( KResponseAllowedBitRate, KErrNotSupported );
        }      
            
    
    // Setting allowed Resolutions
    TInt allowedResolutions( 0 );
    allowedResolutions = ExtractIntegerL( KParamResolution, 0, EFalse );
    if ( allowedResolutions != 0 )
        {
        TRAP_IGNORE( codec->SetAllowedResolutionsL( allowedResolutions ) )
        }   
            
//TODO
/*    
    // Resolution
    CDesC8Array* res = ExtractArrayL(KParamResolution, EFalse);
    if ( res )
        {
        TInt w( 0 );
        TInt h( 0 ); //= static_cast< TInt >(*res)[1];
        
        TLex8 lexer( (*res)[0]->Des() );
		lexer.Val( w );
        TSize resolution( w, h );
        TRAPD(err,codec->SetResolutionL( resolution ));
        }   
*/
            
    // CodecMode
    TInt codecMode( 0 );
    codecMode = ExtractIntegerL( KParamCodecMode, 0, EFalse );
    if ( codecMode != 0 )
        {
        if( codec->SetCodecMode( codecMode ) == KErrNotSupported )
        	AddIntegerResponseL( KResponseCodecMode, KErrNotSupported );
        }   
            
    // Allowed FrameRates
    TInt allowedFrameRates( 0 );
    allowedFrameRates = ExtractIntegerL( KParamAllowedFrameRates, 0, EFalse );
    if ( allowedFrameRates != 0 )
        {
        TRAP_IGNORE( codec->SetAllowedFrameRatesL( allowedFrameRates ) )
        }   
            
    // FrameRate
    TInt frameRate( 0 );
    frameRate = ExtractIntegerL( KParamFrameRate, 0, EFalse );
    if ( frameRate != 0 )
        {
        TRAP_IGNORE( codec->SetFrameRateL( frameRate ) )
        }       
        
    //preferred EncodingDecodingDevice
    TUint id( 0 );
    TPtrC8 idTxt = ExtractTextL(KParamPreferredEncodingDecodingDeviceUid, EFalse);
    TLex8 lexer = TLex8(idTxt);
    err = lexer.Val(id, EHex);
    
    if(err == KErrNone) 
		{			
		const TUid preferredEncodingDecodingDevice = TUid::Uid( id );
		if(id != 0)
			{
			TRAP_IGNORE( codec->SetPreferredEncodingDecodingDeviceL(preferredEncodingDecodingDevice) )
			}			
		}
		
    //Config Keys
    TPtrC8 configKeys = ExtractTextL( KParamConfigKeys, EFalse );
   	if( configKeys.Compare( KNullDesC8 ) != 0 )
    	{
    	TRAP_IGNORE( codec->SetConfigKeyL(configKeys) )
    	}
    	
    // Payload Type
    TInt pType( 0 );
    pType = ExtractIntegerL( KParamPayloadType, 0, ETrue );
    if ( pType != 0 )
        {
        if( codec->SetPayloadType( pType ) == KErrNotSupported)	
        		AddIntegerResponseL( KResponsePayloadType, KErrNotSupported );
        } 		
    
	// ---------- Response creation --------------------------------------------
 
    AddIdResponseL( KCodecId, *codec );
    
 	AddTextResponseL( KResponseCodecName, codec->SdpName() );

 	AddIntegerResponseL( KResponseBitrates, codec->Bitrate() );
 	
 	AddIntegerResponseL( KParamMaxBitRate, codec->MaxBitRate() );
 	
 	AddIntegerResponseL( KParamAllowedResolutions, codec->AllowedResolutions() );
 	
 	AddIntegerResponseL( KResponseCodecMode, codec->CodecMode() );
 	
 	AddIntegerResponseL( KParamAllowedFrameRates, codec->AllowedFrameRates() );
 	
 	AddIntegerResponseL( KParamFrameRate, codec->FrameRate() );
 	
 	AddIntegerResponseL( KResponsePayloadType, codec->PayloadType() );

	}
	
TBool TCmdModifyVideoCodec::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("ModifyVideoCodec") );
	}

TTcCommandBase* TCmdModifyVideoCodec::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdModifyVideoCodec( aContext );
	}
