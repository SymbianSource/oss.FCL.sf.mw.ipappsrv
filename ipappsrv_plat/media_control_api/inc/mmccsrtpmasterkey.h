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
* Description:    Class is used for configuring MCC SRTP master key
*
*/




#ifndef MCCSRTPMASTERKEY_H
#define MCCSRTPMASTERKEY_H

// INCLUDES
#include <e32base.h>

// SRTP Master key length
// 16 octets (128 bits) 
const TUint KMasterKeyMaxLength = 16;

// default SRTP SESSION key lengths:
// default bit length (n_e) for session encryption key (k_e):
// 128 bits (16 octets)
const TUint KMccSRTPDefSessionEncryptionKeyLength = 0x80;

// default bit length (n_a) for session authentication key (k_a):
// 160 bits (20 octets)
const TUint KMccSRTPDefSessionAuthenticationKeyLength = 0xa0;

// default SRTP MASTER key lengths:
// default bit length for SRTP master key (128-bit AES):
// 128 bits (16 octets)
const TUint16 KMccSRTPDefaultMasterKeyLength = 0x80;

// Master key identifier length
// 128 octets 
const TUint KMKIMaxLength = 128;

/**
 * The TMccSrtpMasterKey captures the MCC SRTP master key params
 * into one class
 *
 * @lib N/A
 * @since Series 60 3.2
 */    
class TMccSrtpMasterKey
	{
public: 
	
    inline TMccSrtpMasterKey( const TDesC8& aMasterKey,
                              const TDesC8& aMKI,
                              TUint aEncKeyBitLength =
                                  KMccSRTPDefSessionEncryptionKeyLength,
                              TUint aAuthKeyBitLength =
                                  KMccSRTPDefSessionAuthenticationKeyLength ) :
        iMasterKey( aMasterKey ),
        iMKI( aMKI ),
        iEncKeyBitLength( aEncKeyBitLength ),
        iAuthKeyBitLength( aAuthKeyBitLength )                    
    	{
    	};
    	
private:

    TMccSrtpMasterKey() :
        iMasterKey( KNullDesC8 ),
        iMKI( KNullDesC8 ), 
        iEncKeyBitLength( KMccSRTPDefSessionEncryptionKeyLength ),
        iAuthKeyBitLength( KMccSRTPDefSessionAuthenticationKeyLength )                 
    	{
    	};

public:
    
    /**
     * The master key value.
     */
    TBuf8< KMasterKeyMaxLength > iMasterKey; 
    
    /**
     * The master key identifier value
     */
    TBuf8< KMKIMaxLength > iMKI;
    
    /**
     * The bit length of the encryption key
     */
    TUint iEncKeyBitLength;

    /**
     * The bit length of the authentication key
     */
    TUint iAuthKeyBitLength;
    	
	};
		
typedef TPckgBuf<TMccSrtpMasterKey> TMccSrtpMasterKeyPckg;


#endif /* MCCSRTPMASTERKEY_H */
            
// End of File
