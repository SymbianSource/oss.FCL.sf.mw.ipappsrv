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




#ifndef CMCEENDPOINTPROXY_H
#define CMCEENDPOINTPROXY_H

#include <e32std.h>  		

class CMceMediaSink;


/**
 * 
 *
 * @lib 
 */
class MMceEndPointProxy
	{

public:

    /**
    * Does proxy serve client
    * @param aClient a client
    * @return ETrue, if proxy serves client
    */
    virtual TBool ServesProxyClient( const CMceMediaSink& aClient ) const = 0;
    

    /**
    * Adds client to proxy
    * @param aClient a client
    */
    virtual void AddProxyClientL( CMceMediaSink& aClient ) = 0;

    /**
    * Removes client from proxy
    * @param aClient a client
    */
    virtual void RemoveProxyClient( CMceMediaSink& aClient ) = 0;
    
	};


#endif // CMCEENDPOINTPROXY_H
