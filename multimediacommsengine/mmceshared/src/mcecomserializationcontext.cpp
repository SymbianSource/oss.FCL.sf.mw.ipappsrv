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




#include "mcecomserializationcontext.h"
#include "mcecommediasink.h"
#include "mcecommediasource.h"


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// MMceComSerializationContext::MMceComSerializationContext
// -----------------------------------------------------------------------------
//
MMceComSerializationContext::MMceComSerializationContext( RReadStream& aReadStream )
    : iSinkCache( RPointerArray<CBase>( KMceArrayGranularity ) ),
      iSourceCache( RPointerArray<CBase>( KMceArrayGranularity ) ),
      iReadStream( &aReadStream ),
      iWriteStream( NULL )
    {
    }

// -----------------------------------------------------------------------------
// MMceComSerializationContext::MMceComSerializationContext
// -----------------------------------------------------------------------------
//
MMceComSerializationContext::MMceComSerializationContext( RWriteStream& aWriteStream )
    : iSinkCache( RPointerArray<CBase>( KMceArrayGranularity ) ),
      iSourceCache( RPointerArray<CBase>( KMceArrayGranularity ) ),
      iReadStream( NULL ),
      iWriteStream( &aWriteStream )
    {
    }

// -----------------------------------------------------------------------------
// MMceComSerializationContext::Close
// -----------------------------------------------------------------------------
//
void MMceComSerializationContext::Close()
    {
    iSinkCache.Reset();
    iSourceCache.Reset();
    iSinkCache.Close();
    iSourceCache.Close();
    }
    
// -----------------------------------------------------------------------------
// MMceComSerializationContext::IsCached
// -----------------------------------------------------------------------------
//
TBool MMceComSerializationContext::IsCached( CBase* aItem )
    {
    TBool found = EFalse;
    TInt index = 0;
    
    while( !found && index < iSinkCache.Count() )
        {
        found = iSinkCache[ index++ ] == aItem;
        }

    index = 0;        
    while( !found && index < iSourceCache.Count() )
        {
        found = iSourceCache[ index++ ] == aItem;
        }
    
    return found;
    
    }
    
// -----------------------------------------------------------------------------
// MMceComSerializationContext::ReadStream
// -----------------------------------------------------------------------------
//
RReadStream& MMceComSerializationContext::ReadStream()
    {
    return *iReadStream;
    }

// -----------------------------------------------------------------------------
// MMceComSerializationContext::WriteStream
// -----------------------------------------------------------------------------
//
RWriteStream& MMceComSerializationContext::WriteStream()
    {
    return *iWriteStream;
    }


// -----------------------------------------------------------------------------
// MMceComSerializationContext::SinkCache
// -----------------------------------------------------------------------------
//
RPointerArray<CBase>& MMceComSerializationContext::SinkCache()
    {
    return iSinkCache;
    }

// -----------------------------------------------------------------------------
// MMceComSerializationContext::SourceCache
// -----------------------------------------------------------------------------
//
RPointerArray<CBase>& MMceComSerializationContext::SourceCache()
    {
    return iSourceCache;
    }

