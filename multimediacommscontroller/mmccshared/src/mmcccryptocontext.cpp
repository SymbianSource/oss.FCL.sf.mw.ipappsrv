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
* Description:    Mcc crypto context.
*
*/




// System includes
#include <e32base.h>
#include <mmccsrtpmasterkey.h>
#include <mmccsrtpmastersalt.h>
#include <mmccsrtpcryptoparams.h>

#include "mmcccryptocontext.h"
#include "mcccontrollerlogs.h"


// ======== MEMBER FUNCTIONS ========
    
       
// ---------------------------------------------------------------------------
// TMccCryptoContext::ValidateContext
// 
// ---------------------------------------------------------------------------
// 
TInt TMccCryptoContext::ValidateContext() const
    {
    // Checks Master key validity
    if ( iSrtpMasterKey.iMasterKey.Length() != KMccSRTPDefaultMasterKeyLength/8 )
        {
        __CONTROLLER( "TMccCryptoContext::ValidateContext - Master Key Length Invalid" )
        return KErrArgument;
        }       
    
    if ( iSrtpMasterKey.iEncKeyBitLength != KMccSRTPDefSessionEncryptionKeyLength )
        {
        __CONTROLLER( "TMccCryptoContext::ValidateContext - Encryption Key Length Invalid" )
        return KErrArgument;
        }   
        
    if ( iSrtpMasterKey.iAuthKeyBitLength != KMccSRTPDefSessionAuthenticationKeyLength )
        {
        __CONTROLLER( "TMccCryptoContext::ValidateContext - Authentication Key Length Invalid" )
        return KErrArgument;
        }
       
    // Checks  master salt validity
    if ( iSrtpMasterSalt.iMasterSalt.Length() != KMccSRTPDefaultMasterSaltLength/8 )
        {
        __CONTROLLER( "TMccCryptoContext::ValidateContext - Master Salt Length Invalid" )
        return KErrArgument;
        }       
    
    if ( iSrtpMasterSalt.iSaltKeyLength != KMccSRTPDefSessionSaltingKeyLength )
        {
        __CONTROLLER( "TMccCryptoContext::ValidateContext - Salt Key Length Invalid" )
        return KErrArgument;
        }  
            
    // Checks crypto params validity
    if ( ( iSrtpCryptoParams.iSrtpEncAlg != ESrtpEncAES_CM &&
           iSrtpCryptoParams.iSrtpEncAlg != ESrtpNullAlg ) && 
         ( iSrtpCryptoParams.iSrtcpEncAlg != ESrtpEncAES_CM &&
           iSrtpCryptoParams.iSrtcpEncAlg != ESrtpNullAlg ) )
        {
        __CONTROLLER( "TMccCryptoContext::ValidateContext - Crypto Params Enc Invalid" )
        return KErrArgument;
        }  

    if ( ( iSrtpCryptoParams.iSrtpAuthAlg != ESrtpAuthHMAC_SHA1 &&
           iSrtpCryptoParams.iSrtpAuthAlg != ESrtpAuthNull  &&
           iSrtpCryptoParams.iSrtpAuthAlg != ESrtpAuthRCCm1 &&
           iSrtpCryptoParams.iSrtpAuthAlg != ESrtpAuthRCCm2 &&
           iSrtpCryptoParams.iSrtpAuthAlg != ESrtpAuthRCCm3 ) || 
         ( iSrtpCryptoParams.iSrtcpAuthAlg != ESrtpAuthHMAC_SHA1 &&
           iSrtpCryptoParams.iSrtcpAuthAlg != ESrtpAuthNull ) )
        {
        __CONTROLLER( "TMccCryptoContext::ValidateContext - Crypto Params AuthAlg Invalid" )
        return KErrArgument;
        }  

	if ( iSrtpCryptoParams.iSrtpAuthAlg == ESrtpAuthHMAC_SHA1 &&
		( iSrtpCryptoParams.iSrtpAuthTagLen != KMccSRTPAuthTagLength80 &&
		  iSrtpCryptoParams.iSrtpAuthTagLen != KMccSRTPAuthTagLength32 ) )
		{
		__CONTROLLER( "TMccCryptoContext::ValidateContext - Crypto Params SRTP AuthTag32 Invalid" )
		return KErrArgument;
		}
	
	if ( iSrtpCryptoParams.iSrtcpAuthAlg == ESrtpAuthHMAC_SHA1 &&
		 ( iSrtpCryptoParams.iSrtcpAuthTagLen != KMccSRTPAuthTagLength80 &&
		   iSrtpCryptoParams.iSrtcpAuthTagLen != KMccSRTPAuthTagLength32 ) )
		{
		__CONTROLLER( "TMccCryptoContext::ValidateContext - Crypto Params SRTCP AuthTag32 Invalid" )
		return KErrArgument;
		}
			
    if ( ( iSrtpCryptoParams.iSrtpAuthAlg == ESrtpAuthRCCm1 ||
          iSrtpCryptoParams.iSrtpAuthAlg == ESrtpAuthRCCm2 ) && 
          iSrtpCryptoParams.iSrtpAuthTagLen != KMccSRTPAuthTagLength112 )
        {
        __CONTROLLER( "TMccCryptoContext::ValidateContext - Crypto Params SRTP AuthTag112 Invalid" )
        return KErrArgument;
        }
        
	if ( iSrtpCryptoParams.iSrtpAuthAlg == ESrtpAuthRCCm3 && 
         iSrtpCryptoParams.iSrtpAuthTagLen != KMccSRTPAuthTagLength32 )
        {
        __CONTROLLER( "TMccCryptoContext::ValidateContext - Crypto Params SRTP RCC3 && AuthTag32 Invalid" )
        return KErrArgument;
        }
        
	if ( ( iSrtpCryptoParams.iSrtpAuthAlg == ESrtpAuthNull  && 
           iSrtpCryptoParams.iSrtpAuthTagLen != 0 ) &&
         ( iSrtpCryptoParams.iSrtpAuthAlg == ESrtpAuthNull && 
           iSrtpCryptoParams.iSrtpAuthTagLen != 0) )
        {
        __CONTROLLER( "TMccCryptoContext::ValidateContext - Crypto Params SRTP AuthNull Invalid" )
        return KErrArgument;
        }  

    if ( iSrtpCryptoParams.iPrefixLen != KMccSRTPPrefixLength )
        {
        __CONTROLLER( "TMccCryptoContext::ValidateContext - Crypto Params Prefix Invalid" )
        return KErrArgument;
        }  
        
    if ( !iSrtpCryptoParams.iROCTransRate )
        {
        __CONTROLLER( "TMccCryptoContext::ValidateContext - Crypto Params ROCTransRate Invalid" )
        return KErrArgument;
        }
             
    return KErrNone;
    }
    
// ---------------------------------------------------------------------------
// TMccCryptoContext::UpdateContext
// 
// ---------------------------------------------------------------------------
//
TInt TMccCryptoContext::UpdateContext( TMccCryptoContext& aCandidate )
    {
    iSrtpMasterKey = aCandidate.iSrtpMasterKey;
    iSrtpMasterSalt = aCandidate.iSrtpMasterSalt;
    
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// TMccCryptoContext::GetDestinationInfo
// 
// ---------------------------------------------------------------------------
//    
TInt TMccCryptoContext::GetDestinationInfo( TUint32& aSessionId, TUint32& aLinkId,
    					                    TUint32& aStreamId, TUint32& aEndPointId ) const
    {
    if ( !iBindingDone )
        {
        return KErrNotFound;
        }
        
    aSessionId = iSessionId;
    aLinkId = iLinkId;
    aStreamId = iStreamId;
    aEndPointId = iEndPointId;    
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// TMccCryptoContext::GetDestinationInfo
// 
// ---------------------------------------------------------------------------
//    
TInt TMccCryptoContext::SetDestinationInfo( TUint32 aSessionId, TUint32 aLinkId,
    					                    TUint32 aStreamId, TUint32 aEndPointId )
    {
    iSessionId = aSessionId;
    iLinkId = aLinkId;
    iStreamId = aStreamId;
    iEndPointId =  aEndPointId;
    
    iBindingDone = ETrue;
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// TMccCryptoContext::SetMasterKey
// 
// ---------------------------------------------------------------------------
//
void TMccCryptoContext::SetMasterKey( const TMccSrtpMasterKey& aSrtpMasterKey )
    {
    iSrtpMasterKey = aSrtpMasterKey; 
    }

// ---------------------------------------------------------------------------
// TMccCryptoContext::SetMasterSalt
// 
// ---------------------------------------------------------------------------
//
void TMccCryptoContext::SetMasterSalt( const TMccSrtpMasterSalt& aSrtpMasterSalt )
    {
    iSrtpMasterSalt = aSrtpMasterSalt; 
    }

// ---------------------------------------------------------------------------
// TMccCryptoContext::SetCryptoParams
// 
// ---------------------------------------------------------------------------
//
void TMccCryptoContext::SetCryptoParams( const TMccSrtpCryptoParams& aSrtpCryptoParams )
    {
    iSrtpCryptoParams = aSrtpCryptoParams;   
    }

// ---------------------------------------------------------------------------
// TMccCryptoContext::MasterKey
// 
// ---------------------------------------------------------------------------
//   
const TMccSrtpMasterKey& TMccCryptoContext::MasterKey() const
    {
    return iSrtpMasterKey;
    }

// ---------------------------------------------------------------------------
// TMccCryptoContext::MasterSalt
// 
// ---------------------------------------------------------------------------
//   
const TMccSrtpMasterSalt& TMccCryptoContext::MasterSalt() const
    {
    return iSrtpMasterSalt;
    }

// ---------------------------------------------------------------------------
// TMccCryptoContext::CryptoParams
// 
// ---------------------------------------------------------------------------
//   
const TMccSrtpCryptoParams& TMccCryptoContext::CryptoParams() const
    {
    return iSrtpCryptoParams;
    }
            					                      
// ---------------------------------------------------------------------------
// TMccCryptoContext::ContextId
// 
// ---------------------------------------------------------------------------
//
TUint32 TMccCryptoContext::ContextId() const
    {
    return iContextId;
    }
//  End of File  
