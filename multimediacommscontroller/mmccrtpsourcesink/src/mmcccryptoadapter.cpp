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
* Description:    Implementation of the MCC Crypto Adapter
*
*/




// System includes
#include <e32base.h>
#include <srtpcryptocontext.h>
#include <srtpmasterkey.h>
#include <srtpmastersalt.h>
#include <mmf/common/mmfcontrollerframework.h>

// User include
#include "mmcccryptoadapter.h"
#include "mccrtpdefs.h"
#include "mmcccryptocontext.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// TMccCryptoAdapter::CreateCryptoContextL
// 
// ---------------------------------------------------------------------------
//    
CSRTPCryptoContext* TMccCryptoAdapter::CreateCryptoContextL( 
                                   const TMccCryptoContext& aContextParams )
    {
	TRACE_RTP_CRYPTO_ADAPTER_PRINT( "TMccCryptoAdapter::CreateCryptoContextL IN" )
    
    const TMccSrtpMasterKey mccMasterkey( aContextParams.MasterKey() ); 
    const TMccSrtpMasterSalt mccMasterSalt( aContextParams.MasterSalt() );
    const TMccSrtpCryptoParams mccCryptoParams( aContextParams.CryptoParams() ); 

    // Creates Srtp master key
    CSRTPMasterKey* masterKey = CSRTPMasterKey::NewL( mccMasterkey.iMasterKey,
                                                      mccMasterkey.iMKI,
                                                      mccMasterkey.iEncKeyBitLength,
                                                      mccMasterkey.iAuthKeyBitLength );
    CleanupStack::PushL( masterKey );
    
    TRACE_RTP_CRYPTO_ADAPTER_PRINT2( "TMccCryptoAdapter::masterKey - iEncKeyBitLength: %d ", mccMasterkey.iEncKeyBitLength )
    TRACE_RTP_CRYPTO_ADAPTER_PRINT2( "TMccCryptoAdapter::masterKey - iAuthKeyBitLength: %d ", mccMasterkey.iAuthKeyBitLength  )    

    
    // Creates Srtp master salt                                   
    CSRTPMasterSalt* masterSalt = CSRTPMasterSalt::NewL( mccMasterSalt.iMasterSalt,
                                                         mccMasterSalt.iSaltKeyLength );
    CleanupStack::PushL( masterSalt );
    
    TRACE_RTP_CRYPTO_ADAPTER_PRINT2( "TMccCryptoAdapter::masterSalt - iSaltKeyLength: %d ", mccMasterSalt.iSaltKeyLength )    

    // Creates Srtp crypto params
    TSrtpCryptoParams cryptoParams;
    cryptoParams.iSrtpEncAlg = ( TSRTPEncAlg )mccCryptoParams.iSrtpEncAlg;
    cryptoParams.iSrtpAuthAlg = ( TSRTPAuthAlg )mccCryptoParams.iSrtpAuthAlg;
    cryptoParams.iSrtpAuthTagLen = mccCryptoParams.iSrtpAuthTagLen;
    cryptoParams.iPrefixLen = mccCryptoParams.iPrefixLen;
    cryptoParams.iSrtpReplayProtection = mccCryptoParams.iSrtpReplayProtection;
    cryptoParams.iSrtpKeyDervRate = mccCryptoParams.iSrtpKeyDervRate;
    
    // RTCP Crypto Params
    cryptoParams.iSrtcpEncAlg = ( TSRTPEncAlg )mccCryptoParams.iSrtcpEncAlg;
    cryptoParams.iSrtcpAuthAlg = ( TSRTPAuthAlg )mccCryptoParams.iSrtcpAuthAlg;
    cryptoParams.iSrtcpAuthTagLen = mccCryptoParams.iSrtcpAuthTagLen;
    cryptoParams.iSrtcpReplayProtection = mccCryptoParams.iSrtcpReplayProtection;
    cryptoParams.iSrtcpKeyDervRate = mccCryptoParams.iSrtcpKeyDervRate;
    
    cryptoParams.iROCTransRate = mccCryptoParams.iROCTransRate;
    cryptoParams.iMasterKeysLifeTime = mccCryptoParams.iMasterKeysLifeTime;
    cryptoParams.iReplayWindowSizeHint = mccCryptoParams.iReplayWindowSizeHint;
    cryptoParams.iIsRCCm3Sync = mccCryptoParams.iIsRCCm3Sync;
    

    TRACE_RTP_CRYPTO_ADAPTER_PRINT2( "TMccCryptoAdapter::cryptoParams - iSrtpEncAlg, [0=null, 1=AES_CM]: %d", ( TUint )cryptoParams.iSrtpEncAlg )
    TRACE_RTP_CRYPTO_ADAPTER_PRINT2( "TMccCryptoAdapter::cryptoParams - iSrtpAuthAlg [0=null, 1=HMAC_SHA1 2=RCCm1, 3=RCCm2, 4=RCCm3]: %d", ( TUint )cryptoParams.iSrtpAuthAlg )
    TRACE_RTP_CRYPTO_ADAPTER_PRINT2( "TMccCryptoAdapter::cryptoParams - iSrtpAuthTagLen: %d", cryptoParams.iSrtpAuthTagLen )
    TRACE_RTP_CRYPTO_ADAPTER_PRINT2( "TMccCryptoAdapter::cryptoParams - iPrefixLen: %d", cryptoParams.iPrefixLen )
    TRACE_RTP_CRYPTO_ADAPTER_PRINT2( "TMccCryptoAdapter::cryptoParams - iSrtpReplayProtection: %d", cryptoParams.iSrtpReplayProtection )
    TRACE_RTP_CRYPTO_ADAPTER_PRINT2( "TMccCryptoAdapter::cryptoParams - iSrtpKeyDervRate: %d", cryptoParams.iSrtpKeyDervRate )
    TRACE_RTP_CRYPTO_ADAPTER_PRINT( "" )  
    TRACE_RTP_CRYPTO_ADAPTER_PRINT2( "TMccCryptoAdapter::cryptoParams - iSrtcpEncAlg, [0=null, 1=AES_CM] : %d", ( TUint )cryptoParams.iSrtcpEncAlg )
    TRACE_RTP_CRYPTO_ADAPTER_PRINT2( "TMccCryptoAdapter::cryptoParams - iSrtcpAuthAlg [0=null, 1=HMAC_SHA1 2=RCCm1, 3=RCCm2, 4=RCCm3]: %d", ( TUint )cryptoParams.iSrtcpAuthAlg )   
    TRACE_RTP_CRYPTO_ADAPTER_PRINT2( "TMccCryptoAdapter::cryptoParams - iSrtcpAuthTagLen: %d", cryptoParams.iSrtcpAuthTagLen )
    TRACE_RTP_CRYPTO_ADAPTER_PRINT2( "TMccCryptoAdapter::cryptoParams - iSrtcpReplayProtection: %d", cryptoParams.iSrtcpReplayProtection )
    TRACE_RTP_CRYPTO_ADAPTER_PRINT2( "TMccCryptoAdapter::cryptoParams - iSrtcpKeyDervRate: %d", cryptoParams.iSrtcpKeyDervRate )


    TRACE_RTP_CRYPTO_ADAPTER_PRINT2( "TMccCryptoAdapter::cryptoParams - iROCTransRate: %d", cryptoParams.iROCTransRate)
    TRACE_RTP_CRYPTO_ADAPTER_PRINT2( "TMccCryptoAdapter::cryptoParams - iMasterKeysLifeTime: %d", cryptoParams.iMasterKeysLifeTime )
    TRACE_RTP_CRYPTO_ADAPTER_PRINT2( "TMccCryptoAdapter::cryptoParams - iReplayWindowSizeHint: %d", cryptoParams.iReplayWindowSizeHint )
    TRACE_RTP_CRYPTO_ADAPTER_PRINT2( "TMccCryptoAdapter::cryptoParams - iIsRCCm3Sync: %d", cryptoParams.iIsRCCm3Sync )     

    
    CleanupStack::Pop( masterSalt );
    CleanupStack::Pop( masterKey );
                                                               
    CSRTPCryptoContext* context = CSRTPCryptoContext::NewL( masterKey,
                                                            masterSalt, 
                                                            cryptoParams );

	TRACE_RTP_CRYPTO_ADAPTER_PRINT( "TMccCryptoAdapter::CreateCryptoContextL OUT" );   
    return context;   
    };
 
// ---------------------------------------------------------------------------
// TMccCryptoAdapter::UpdateCryptoContextL()
// 
// ---------------------------------------------------------------------------
//   
void TMccCryptoAdapter::UpdateCryptoContextL( const TMccCryptoContext& aContextParams,
                                              CSRTPCryptoContext& aContext )
    {
    TRACE_RTP_CRYPTO_ADAPTER_PRINT( "TMccCryptoAdapter::UpdateCryptoContextL IN" )
    
    const TMccSrtpMasterKey mccMasterkey( aContextParams.MasterKey() ); 
    const TMccSrtpMasterSalt mccMasterSalt( aContextParams.MasterSalt() );
    const TMccSrtpCryptoParams mccCryptoParams( aContextParams.CryptoParams() );
       
    // Creates Srtp master key
    CSRTPMasterKey* masterKey = CSRTPMasterKey::NewL( mccMasterkey.iMasterKey,
                                                      mccMasterkey.iMKI,
                                                      mccMasterkey.iEncKeyBitLength,
                                                      mccMasterkey.iAuthKeyBitLength );
                                                      
    CleanupStack::PushL( masterKey );
                                                      
    CSRTPMasterSalt* masterSalt = CSRTPMasterSalt::NewL( mccMasterSalt.iMasterSalt,
                                                         mccMasterSalt.iSaltKeyLength );
    CleanupStack::PushL( masterSalt );
                                               
    if ( EFalse == masterKey->Valid() )
        {
        TRACE_RTP_CRYPTO_ADAPTER_PRINT( "TMccCryptoAdapter::UpdateCryptoContextL Masterkey isn't valid" )

        User::Leave( KErrArgument );
        }
        
    if ( EFalse == masterSalt->Valid() )
        {
        TRACE_RTP_CRYPTO_ADAPTER_PRINT( "TMccCryptoAdapter::UpdateCryptoContextL Mastersalt isn't valid" )

        User::Leave( KErrArgument );
        } 
         
    if ( ( aContext.MasterSalt().MasterSalt()).Compare( masterSalt->MasterSalt() ) != 0 )
        {               
        aContext.SetMasterSalt( masterSalt );
        CleanupStack::Pop( masterSalt );
        TRACE_RTP_CRYPTO_ADAPTER_PRINT( "TMccCryptoAdapter::UpdateCryptoContextL MasterSalt Is changed" )  
        }
    else
        {
        CleanupStack::PopAndDestroy( masterSalt );
        TRACE_RTP_CRYPTO_ADAPTER_PRINT( "TMccCryptoAdapter::UpdateCryptoContextL MasterSalt Isn't changed" )   
        }

    if ( ( aContext.MasterKey().MasterKey()).Compare( masterKey->MasterKey() ) != 0 ||
         ( aContext.MasterKey().MKI()).Compare( masterKey->MKI() ) != 0 )
        {               
        aContext.SetMasterKey( masterKey );
        CleanupStack::Pop( masterKey );
        TRACE_RTP_CRYPTO_ADAPTER_PRINT( "TMccCryptoAdapter::UpdateCryptoContextL MasterKey Is changed" ) 
        }
    else
        {
        CleanupStack::PopAndDestroy( masterKey );
        TRACE_RTP_CRYPTO_ADAPTER_PRINT( "TMccCryptoAdapter::UpdateCryptoContextL MasterKey Isn't changed" )  
        }
    
    TRACE_RTP_CRYPTO_ADAPTER_PRINT( "TMccCryptoAdapter::UpdateCryptoContextL OUT" )            
    }
//  End of File  
