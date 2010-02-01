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




#ifndef CMCECOMMICSOURCE_H
#define CMCECOMMICSOURCE_H

#include <e32std.h>  
#include "mcecommediasource.h"

/**
 * 
 *
 * @lib 
 */
class CMceComMicSource: public CMceComMediaSource
	{

public: // Constructors and destructor

    /**
    * Constructor
    */
    static CMceComMicSource* NewL();

    /**
    * Constructor
    */
    static CMceComMicSource* NewLC();

    /**
    * Destructor.
    */
	~CMceComMicSource();


public: // from CMceComMediaSource

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
    * Updates this object
    * @param aUpdate object updates this
    */
    void UpdateL( CMceComMediaSource& aUpdate );

    /**
    * Clones this object
    * @return the cloned object
    */
    CMceComMediaSource* CloneL();

    /**
    * Is this endpoint re-usable
    * @return ETrue, if this endpoint is re-usable
    */
    TBool Reusable( const CMceComMediaStream& aParent ) const;
    
    
#ifdef MCE_COMMON_SERVER_SIDE

    /**
    * Returns mcc type for mcc endpoint
    * @return mcc type
    */
    const TUid MccType();
    
    /**
    * Prepares this endpoint
    */
    void PrepareL();

#endif//MCE_COMMON_SERVER_SIDE
    

protected:

    /**
    * C++ default constructor.
    */
	CMceComMicSource();
		
public: // Data

    /**
    * Mic gain
    */
    TInt iGain;
    
	};


#endif