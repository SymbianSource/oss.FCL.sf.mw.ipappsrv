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




#include "mcesrvsource.h"
#include "mcesrvstream.h"
#include "mcecommediasource.h"
#include "mceevent.h"
#include "mcesrvstreamiterator.h"
#include "mcesdpsession.h"
#include "mcemediamanager.h"

#define _MEDIA_SOURCE static_cast<CMceComMediaSource*>( &Data() )

// ============================ MEMBER FUNCTIONS ===============================


    
// -----------------------------------------------------------------------------
// CMceSrvSource::CMceSrvSource
// -----------------------------------------------------------------------------
//
CMceSrvSource::CMceSrvSource( CMceMediaManager& aManager,
                              CMceComMediaSource& aData )
    : CMceSrvEndpoint( aManager, aData )
    {
    }       
    
// -----------------------------------------------------------------------------
// CMceSrvSource::~CMceSrvSource
// -----------------------------------------------------------------------------
//
CMceSrvSource::~CMceSrvSource()
    {
    //NOP
    }

// -----------------------------------------------------------------------------
// CMceSrvSource::Endpoint
// -----------------------------------------------------------------------------
//
CMceSrvEndpoint& CMceSrvSource::Endpoint( CMceSrvStream& aStream )
    {
    return aStream.Source();
    }
 
// -----------------------------------------------------------------------------
// CMceSrvSink::HandleSecureEventReceivedL
// -----------------------------------------------------------------------------
//
void CMceSrvSource::HandleSecureEventReceivedL( )
    {
	TMceSrvStreamIterator streams( Data().MediaStream()->Session()->MccStreams(), 
                                   Data() );
    CMceSrvStream* stream = NULL;
    while ( streams.Next( stream )  )
        {
        if ( stream->Codec().iIsEnabled &&
             Endpoint( *stream ).State() > CMceSrvStream::EPrepared &&
             Endpoint( *stream ).State() != CMceSrvStream::EPaused )
            {
            Data().MediaStream()->Session()->SdpSession().Manager().PauseL(  *(_MEDIA_SOURCE) );
            }
        }
    
    }
    
// End of file

