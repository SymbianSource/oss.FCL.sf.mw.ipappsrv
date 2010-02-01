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




#ifndef CMCECOMAVSINK_H
#define CMCECOMAVSINK_H

#include <e32std.h>  
#include "mcecommediasink.h"
#include "mcecomendpointproxy.h"

/**
 * 
 *
 * @lib 
 */
NONSHARABLE_CLASS( CMceComAvSink ) : public CMceComMediaSink,
                                     public MMceComEndPointProxy
                      
	{

public: // Constructors and destructor

    /**
    * Constructor
    */
    static CMceComAvSink* NewL();

    /**
    * Constructor
    */
    static CMceComAvSink* NewLC();

    /**
    * Destructor.
    */
	~CMceComAvSink();

public: // from CMceComMediaSink

    /**
    * Internalizes flat data
    * @param aReadStream read stream
    */
    void InternalizeFlatL( RReadStream& aReadStream );

    /**
    * Externalizes flat data
    * @param aWriteStream write stream
    */
    void ExternalizeFlatL( RWriteStream& aWriteStream );
    
    /**
    * Traversal event handler
    * @param aEvent the event
    * @return status, if event was consumed or not or object needs update
    */
    TInt EventReceivedL( TMceComEvent& aEvent );

    /**
    * Clones this object
    * @return the cloned object
    */
    CMceComMediaSink* CloneL();

    /**
    * Updates this object
    * @param aUpdate object updates this
    */
    void UpdateL( CMceComMediaSink& aUpdate );

public: // from MMceComEndPointProxy

    /**
    * Does proxy serve client
    * @param aClient a client
    * @return ETrue, if proxy serves client
    */
    TBool ServesProxyClient( const CMceComEndpoint& aClient ) const;
    
    /**
    * Adds client to proxy
    * @param aClient a client
    */
    void AddProxyClientL( CMceComEndpoint& aClient );

    /**
    * Removes client from proxy
    * @param aClient a client
    */
    void RemoveProxyClient( CMceComEndpoint& aClient );



#ifdef MCE_COMMON_SERVER_SIDE

    /**
    * Returns mcc type for mcc endpoint
    * @return mcc type
    */
    const TUid MccType();
    
    /**
    * Returns init param for mcc endpoint
    * @return mcc type
    */
    const TDesC8& InitParamL( const CMceComCodec& aCodec );
    
    /**
    * Prepares this endpoint
    */
    void PrepareL();
    
public: 

    /**
    * Enables defined client
    * @return KMceSrvStreamDefaultSequence if client does not support
    * client enabling, otherwise returns KMceSrvStreamAlternativeSequence
    */
    TBool ClientEnableL( CMceComEndpoint& aClient, const CMceComCodec& aCodec );
    
    /**
    * Disables defined client
    * @return KMceSrvStreamDefaultSequence if client does not support
    * client disabling, otherwise returns KMceSrvStreamAlternativeSequence
    */
    TBool ClientDisableL( CMceComEndpoint& aClient );   
    
    /**
    * Client wants to change settings
    * @param aClient a client
    * @param aParam parameter name
    * @param aValue parameter value
    * @return KMceSrvStreamDefaultSequence if client does not support
    * this setting, otherwise returns KMceSrvStreamAlternativeSequence
    */ 
    TBool SetParameterL( const CMceComEndpoint& aClient, 
                         TUint32 aParam,
                         const TDesC8& aValue );
    

#endif//MCE_COMMON_SERVER_SIDE

protected:

    /**
    * C++ default constructor.
    */
	CMceComAvSink();
	
	
public: // Data

    RPointerArray<CMceComEndpoint> iProxyClients;


private: // Data

    HBufC8* iSettings;
    
    TInt iVolume;
    
	};


#endif //CMCECOMAVSINK_H
