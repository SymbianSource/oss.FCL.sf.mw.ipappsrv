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




#ifndef CMCECOMSERIALIZABLE_H
#define CMCECOMSERIALIZABLE_H

#include <e32std.h>  		
#include <s32strm.h>
#include "mcedefs.h"
#include "mceclientserver.h"
#include "mcecomserializationcontext.h"




/**
 * 
 *
 * @lib 
 */
class MMceComSerializable
	{

public:

    /**
    * Returns serialization id
    * @return serialization id
    */
    virtual TUint64 SerializationId() const = 0;
    
    /**
    * Internalizes flat data
    * @param aReadStream read stream
    */
    virtual void InternalizeFlatL( RReadStream& aReadStream ) = 0;

    /**
    * Externalizes flat data
    * @param aWriteStream write stream
    */
    virtual void ExternalizeFlatL( RWriteStream& aWriteStream ) = 0;
    
    /**
    * Internalizes
    * @param aSerCtx context for serialization
    */
    virtual void InternalizeL( MMceComSerializationContext& aSerCtx ) = 0;
    
    /**
    * Externalizes
    * @param aSerCtx context for serialization
    */
    virtual void ExternalizeL( MMceComSerializationContext& aSerCtx ) = 0;
    
    
	};


#endif // CMCECOMSERIALIZABLE_H
