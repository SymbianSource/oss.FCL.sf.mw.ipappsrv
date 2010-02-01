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
* Description:    Convert MCC Crypto context to the SRTP crypto context form 
*
*/




#ifndef MMCCCRYPTOADAPTER_H
#define MMCCCRYPTOADAPTER_H

#include <e32base.h>

class TMccCryptoContext;

/**.
 * This class Converts MCC Crypto context to the SRTP Crypto context form
 *
 *
 * @lib Mccrtpsourcesink.dll
 * @since S60 3.1
 */

class TMccCryptoAdapter
    { 
public:

    /**
     * Creates Crypto context from serialized crypto policy data
     *
     * @since    S60 3.2
     * @param    aContextParams Context params.       
     * @return   Crypto context
     */
    static CSRTPCryptoContext* CreateCryptoContextL( 
                                const TMccCryptoContext& aContextParams );
    
    /**
     * Update crypto context.
     *
     * @since    S60 3.2
     * @param    aContextParams Context params.
     * @param    aContext SRTP crypto context.      
     * @return   voi
     */
    static void UpdateCryptoContextL( const TMccCryptoContext& aContextParams,
                                      CSRTPCryptoContext& aContext );   
    };

#endif // MMCCCRYPTOADAPTER_H
