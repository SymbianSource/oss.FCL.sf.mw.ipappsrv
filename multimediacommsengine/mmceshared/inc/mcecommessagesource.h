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




#ifndef CMCECOMMESSAGESOURCE_H
#define CMCECOMMESSAGESOURCE_H

#include <e32std.h>  
#include "mcecommediasource.h"
#include "mcemediasource.h"

/**
 * 
 *
 * @lib 
 */
class CMceComMessageSource: public CMceComMediaSource
    {

public: // Constructors and destructor

    /**
    * Constructor
    */
    static CMceComMessageSource* NewL();

    /**
    * Constructor
    */
    static CMceComMessageSource* NewLC();

    /**
    * Destructor.
    */
    ~CMceComMessageSource();


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
       * Clones this object
       * @return the cloned object
       */
    CMceComMediaSource* CloneL();
    
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
    CMceComMessageSource();
        
    };


#endif
