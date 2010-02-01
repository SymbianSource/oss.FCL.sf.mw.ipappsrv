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
* Description:    
*
*/




#ifndef CMCECOMFILESINK_H
#define CMCECOMFILESINK_H

#include <e32std.h>  
#include "mcecommediasink.h"
#include "mcecomfilesource.h"


/**
 * 
 *
 * @lib 
 */
class CMceComFileSink : public CMceComMediaSink
	{

public: // Constructors and destructor

    /**
    * Constructor
    */
    static CMceComFileSink* NewL();

    /**
    * Constructor
    */
    static CMceComFileSink* NewLC();

    /**
    * Destructor.
    */
	~CMceComFileSink();

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
    * Clones this object
    * @return the cloned object
    */
    CMceComMediaSink* CloneL();

    /**
    * Updates this object
    * @param aUpdate object updates this
    */
    void UpdateL( CMceComMediaSink& aUpdate );


#ifdef MCE_COMMON_SERVER_SIDE

    /**
    * Returns mcc type for mcc endpoint
    * @return mcc type
    */
    const TUid MccType();

    /**
    * Returns initialization params
    * @param aCodec used codec
    */
    const TDesC8& InitParamL( const CMceComCodec& aCodec );

#endif//MCE_COMMON_SERVER_SIDE
    

protected:

    /**
    * C++ default constructor.
    */
	CMceComFileSink();
		

public: // serialized data

    TMceFileInfo iFileInfo;
    TBool iInitialInfoRetrieved;    

private: // data

    HBufC8* iSettings;
    
	};


#endif//CMCECOMFILESINK_H