/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    
*
*/




#ifndef CMCESECURESESSION_H
#define CMCESECURESESSION_H

// INCLUDES
#include <e32base.h> 
#include <e32std.h>  
#include <badesca.h>  
#include <mcedefs.h>
#include <mcemanager.h>
#include <mcesession.h>

// CONSTANTS
const TMceSessionType KMceSessionSecure = 2;

enum TMceCryptoContext
    {
    // Counter Mode cipher HMAC-SHA1 message authentication, 32-bit authentication tag
    EAES_CM_128_HMAC_SHA1_32,
    // Counter Mode cipher HMAC-SHA1 message authentication, 80-bit authentication tag
    EAES_CM_128_HMAC_SHA1_80,  
    };

// CLASS DECLARATION

/**
* Base class for secure MCE sessions, inherited by both CMceSecureInSession
* and CMceSecureOutSession.
*
* @lib mceclient.lib
*/
class MMceSecureSession
	{
public: // Crypto contexts


public: // Functions
		             
    /*
    * Get the supported crypto contexts of the terminal.
    * @return supported crypto contexts.
    */	    
	virtual const RArray<TMceCryptoContext>& SupportedCryptoContextsL() const = 0;
		             
    /*
    * Get the used crypto contexts in the session.
    * @return used crypto contexts.
    */	    
	virtual const RArray<TMceCryptoContext>& CryptoContexts() const = 0;

    /*
    * Set the used crypto contexts in the session.
	* @pre State() == CMceSession::EIdle ||
	*      State() == CMceSession::EIncoming ||
	*      State() == CMceSession::EEstablished
    * @param aArray Array of crypto contexts to be used in session.
    */	    
	virtual void SetCryptoContextsL( const RArray<TMceCryptoContext>& aArray ) = 0;
	};

#endif 
