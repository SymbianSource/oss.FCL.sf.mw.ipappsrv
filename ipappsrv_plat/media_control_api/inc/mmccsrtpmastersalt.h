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
* Description:    Class is used for configuring MCC SRTP master salt
*
*/




#ifndef MMCCSRTPMASTERSALT_H
#define MMCCSRTPMASTERSALT_H

// INCLUDES
#include <e32base.h>


// default bit length for session salt key:
// 112 bits (14 octets)
const TUint KMccSRTPDefSessionSaltingKeyLength = 0x70;

// max octet length for session salt key:
// 14 octets (112 bits) 
const TUint KMasterSaltMaxLength = 14;

// default bit length for SRTP master salt 
// 112 bits (14 octets)
const TUint16 KMccSRTPDefaultMasterSaltLength = 0x70;

/**
 * The TMccSrtpMasterSalt captures the MCC SRTP master salt params
 * into one class
 *
 * @lib N/A
 * @since Series 60 3.2
 */    
class TMccSrtpMasterSalt
	{
public: 
	
    inline TMccSrtpMasterSalt( const TDesC8& aMasterSalt,
        TUint aSaltKeyLength = KMccSRTPDefSessionSaltingKeyLength ) :
        iMasterSalt( aMasterSalt ),
        iSaltKeyLength( aSaltKeyLength )                    
    	{
    	};

private:
    
    TMccSrtpMasterSalt() :
        iMasterSalt( KNullDesC8 ),
        iSaltKeyLength( KMccSRTPDefSessionSaltingKeyLength )                      
    	{
    	};

public:
    	
    /**
     * The master salt value.
     */
    TBufC8< KMasterSaltMaxLength > iMasterSalt; 

    /**
     * The length of the master salt
     */
    TUint iSaltKeyLength;   	
	};
		
typedef TPckgBuf<TMccSrtpMasterSalt> TMccSrtpMasterSaltPckg;


#endif /* MMCCSRTPMASTERSALT_H */
            
// End of File
