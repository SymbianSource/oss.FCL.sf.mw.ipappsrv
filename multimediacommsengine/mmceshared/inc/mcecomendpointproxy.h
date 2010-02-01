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
* Description:    
*
*/




#ifndef CMCECOMENDPOINTPROXY_H
#define CMCECOMENDPOINTPROXY_H

#include <e32std.h>  		

class CMceComEndpoint;


/**
 * 
 *
 * @lib 
 */
class MMceComEndPointProxy
	{

public:

    /**
    * Does proxy serve client
    * @param aClient a client
    * @return ETrue, if proxy serves client
    */
    virtual TBool ServesProxyClient( const CMceComEndpoint& aClient ) const = 0;
    

    /**
    * Adds client to proxy
    * @param aClient a client
    */
    virtual void AddProxyClientL( CMceComEndpoint& aClient ) = 0;

    /**
    * Removes client from proxy
    * @param aClient a client
    */
    virtual void RemoveProxyClient( CMceComEndpoint& aClient ) = 0;
    
    /**
    * Enables defined client
    * @return EFalse if the endpoint does not support
    * client enabling, otherwise returns ETrue
    */
    inline virtual TBool ClientEnableL( CMceComEndpoint& /*aClient*/, 
                                        const CMceComCodec& /*aCodec*/ )
                                      { return EFalse; }
    
    /**
    * Disables defined client
    * @return EFalse if the endpoint does not support
    * client disabling, otherwise returns ETrue
    */
    inline virtual TBool ClientDisableL( CMceComEndpoint& /*aClient*/ )
                                       { return EFalse; }
    
    /**
    * Client wants to change settings
    * @param aClient a client
    * @param aParam parameter name
    * @param aValue parameter value
    * @return EFalse if client does not support
    * this setting, otherwise returns ETrue
    */                                  
    inline virtual TBool SetParameterL( const CMceComEndpoint& /*aClient*/, 
                                        TUint32 /*aParam*/,
                                        const TDesC8& /*aValue*/ ) 
                                      { return EFalse; }
    
	};


#endif // CMCECOMENDPOINTPROXY_H
