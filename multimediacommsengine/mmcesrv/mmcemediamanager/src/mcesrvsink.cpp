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




#include "mcesrvsink.h"
#include "mcesrvstream.h"
#include "mcemediamanager.h"
#include "mcecommediasink.h"
#include "mcertpsink.h"
#include "mceevent.h"
#include "mcesrvstreamiterator.h"
#include "mcesdpsession.h"

#define _MEDIA_SINK static_cast<CMceComMediaSink*>( &Data() )

// ============================ MEMBER FUNCTIONS ===============================


    
// -----------------------------------------------------------------------------
// CMceSrvSink::CMceSrvSink
// -----------------------------------------------------------------------------
//
CMceSrvSink::CMceSrvSink( CMceMediaManager& aManager,
                          CMceComMediaSink& aData )
    : CMceSrvEndpoint( aManager, aData )
    {
    
    }
 
// -----------------------------------------------------------------------------
// CMceSrvSink::~CMceSrvSink
// -----------------------------------------------------------------------------
//
CMceSrvSink::~CMceSrvSink()
    {
    //NOP
    }

// -----------------------------------------------------------------------------
// CMceSrvSink::MccPrepareCalledL
// -----------------------------------------------------------------------------
//
void CMceSrvSink::MccPrepareCalledL( CMceSrvStream& aStream )
    {

    if ( Data().iType == KMceRTPSink )
        {
        iSSRC = iManager->GetSSRCL( aStream );
        //store it also to sink
        _MEDIA_SINK->SetSSRC( iSSRC );
        }
    }

// -----------------------------------------------------------------------------
// CMceSrvSink::Endpoint
// -----------------------------------------------------------------------------
//
CMceSrvEndpoint& CMceSrvSink::Endpoint( CMceSrvStream& aStream )
    {
    return aStream.Sink();
    }

// -----------------------------------------------------------------------------
// CMceSrvSink::HandleSecureEventReceivedL
// -----------------------------------------------------------------------------
//
void CMceSrvSink::HandleSecureEventReceivedL( )
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
            Data().MediaStream()->Session()->SdpSession().Manager().PauseL(  *(_MEDIA_SINK) );
            }
        }
    
    }
// End of file

