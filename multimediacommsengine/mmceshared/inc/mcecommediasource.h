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




#ifndef CMCECOMMEDIASOURCE_H
#define CMCECOMMEDIASOURCE_H

#include "mcemediasource.h"
#include "mcecomendpoint.h"

// CONSTANTS
const TMceSourceType KMceEmptySource = 100;
const TMceEndpointCategory KMceCategorySource = 1;

#ifdef MCE_COMMON_SERVER_SIDE

class CMceMediaManager;

#endif//MCE_COMMON_SERVER_SIDE

class TMceComSourceFactory;
class CMceComCodec;


/**
 * 
 *
 * @lib 
 */
class CMceComMediaSource : public CMceComEndpoint
	{

public: // Constructors and destructor

    /**
    * Constructor
    */
    static CMceComMediaSource* NewL();

    /**
    * Constructor
    */
    static CMceComMediaSource* NewLC();

    /**
    * Destructor
    */
	~CMceComMediaSource();
	

public: // from CMceComEndpoint


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


public: // virtual functions
    
    /**
    * Updates this object
    * @param aUpdate object updates this
    */
    virtual void UpdateL( CMceComMediaSource& aUpdate );
    
    /**
    * Clones this object
    * @return the cloned object
    */
    virtual CMceComMediaSource* CloneL();


public: // new functions

    /**
    * Factory
    * @return factory
    */
    static TMceComSourceFactory Factory();
    
#ifdef MCE_COMMON_SERVER_SIDE

    /**
    * Returns mcc type for mcc endpoint
    * @return mcc type
    */
    const TUid MccType();

#endif//MCE_COMMON_SERVER_SIDE

    
protected:

    /**
     * C++ default constructor.
     * @param aType type of source
     */
	CMceComMediaSource( TMceSourceType aType );
    
	};


#endif 