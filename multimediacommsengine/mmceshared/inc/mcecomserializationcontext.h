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



#ifndef MCECOMSERIALIZATIONCONTEXT_H
#define MCECOMSERIALIZATIONCONTEXT_H

// INCLUDES
#include <e32cmn.h>
#include <e32base.h>
#include "mceclientserver.h"

// CONSTANTS

// CLASS DECLARATION
class CMceComMediaSink;
class CMceComMediaSource;
class RReadStream;
class RWriteStream;

/**
* Context used in serialization to cache externalized and internalized sinks and sources
*
*  
*/
class MMceComSerializationContext 
    {
    
public:

    MMceComSerializationContext( RReadStream& aReadStream );

    MMceComSerializationContext( RWriteStream& aWriteStream );
    
    void Close();
        
    RReadStream& ReadStream();
    RWriteStream& WriteStream();

    TBool IsCached( CBase* aItem );
    
    RPointerArray<CBase>& SinkCache();
    RPointerArray<CBase>& SourceCache();

    
private://data
    
    
    RPointerArray<CBase> iSinkCache;
    RPointerArray<CBase> iSourceCache;
    
    RReadStream* iReadStream;
    RWriteStream* iWriteStream;
    
    
    };


#endif      //MCECOMSERIALIZATIONCONTEXT_H

// End of File
