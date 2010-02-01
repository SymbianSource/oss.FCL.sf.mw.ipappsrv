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




#include "mcepreparingstreams.h"
#include "mcecomsession.h"
#include "mcesdpsession.h"
#include "mcesrvstream.h"
#include "mceevents.h"
#include "mcemmlogs.h"


    
    
// -----------------------------------------------------------------------------
// TMcePreparingStreams::TMcePreparingStreams
// -----------------------------------------------------------------------------
//
TMcePreparingStreams::TMcePreparingStreams ( 
                        CMceComSession& aSession,
                        TMceNegotiationRole aRole,
                        TMceSrvStreamIterator::TDirection aDirection )
    : TMceMediaState( aSession ),
      iIterator( TMceSrvStreamIterator( aSession.MccStreams(), aDirection ) )
    {
    SetRole( aRole );
    }



// -----------------------------------------------------------------------------
// TMcePreparingStreams::MccLinkCreatedL
// -----------------------------------------------------------------------------
//
void TMcePreparingStreams::MccLinkCreatedL( TMceMccComEvent& aEvent )
    {
    MCEMM_DEBUG("TMcePreparingStreams::MccLinkCreatedL(), Entry ");
    
    CMceSrvStream::EventReceived( iSession.MccStreams(), aEvent );
    CMceSrvStream* stream = NULL;
    
    iIterator.Reset();
    
    //try to find first stream, which is in state 'greater or equal' than link created
    //if no stream => all streams are in state link created
    //=> links are created
    if ( !iIterator.Next( stream, CMceSrvStream::ECreatingLink, 
                          TMceSrvStreamIterator::ExactMatch ) )
        {
        PrepareStreamsL();
        }
    else
        {
        MCEMM_DEBUG("TMcePreparingStreams::MccLinkCreatedL(): \
waiting links to be created");
        //just wait
        }
    
    MCEMM_DEBUG("TMcePreparingStreams::MccLinkCreatedL(), Exit ");
    }


// -----------------------------------------------------------------------------
// TMcePreparingStreams::MccMediaPreparedL
// -----------------------------------------------------------------------------
//
void TMcePreparingStreams::MccMediaPreparedL( TMceMccComEvent& aEvent )
    {
    MCEMM_DEBUG("TMcePreparingStreams::MccMediaPreparedL(), Entry ");
    
    CMceSrvStream::EventReceived( iSession.MccStreams(), aEvent );
    CMceSrvStream* stream = NULL;
    
    iIterator.Reset();
    
    //try to find first stream, which is in state 'greater or equal' than prepared
    //if no stream => all streams are in state prepared or stopped (or started, paused) 
    //=> streams are prepared        
    if ( !iIterator.Next( stream, CMceSrvStream::EPreparing, 
                          TMceSrvStreamIterator::ExactMatch ) )
        {
        StreamsPreparedL();
        }
    else
        {
        MCEMM_DEBUG("TMcePreparingStreams::MccMediaPreparedL(): \
waiting streams to be prepared");
        //just wait
        }
    
    MCEMM_DEBUG("TMcePreparingStreams::MccMediaPreparedL(), Exit ");
    }


// -----------------------------------------------------------------------------
// TMcePreparingStreams::MccMediaStartedL
// -----------------------------------------------------------------------------
//
void TMcePreparingStreams::MccMediaStartedL( TMceMccComEvent& aEvent )
    {
    MCEMM_DEBUG("TMcePreparingStreams::MccMediaStartedL(), Entry ");

    CMceSrvStream::EventReceived( iSession.MccStreams(), aEvent );
    
    CMceSrvStream* stream = NULL;
    
    iIterator.Reset();
    
    //try to find first stream, which is in state 'greater or equal' than stopped
    //if no stream => all streams are in state stopped, started or paused
    if ( !iIterator.Next( stream, CMceSrvStream::EStarting, 
                          TMceSrvStreamIterator::ExactMatch ) )
        {
        StreamsStartedL();
        }
    else
        {
        MCEMM_DEBUG("TMcePreparingStreams::MccMediaStartedL(): \
waiting streams to be started");
        //just wait
        }

    MCEMM_DEBUG("TMcePreparingStreams::MccMediaStartedL(), Exit ");
    }
    
// -----------------------------------------------------------------------------
// TMcePreparingStreams::MccMediaPausedL
// -----------------------------------------------------------------------------
//
void TMcePreparingStreams::MccMediaPausedL( TMceMccComEvent& aEvent )
    {
    MCEMM_DEBUG("TMcePreparingStreams::MccMediaPausedL(), Entry ");

    MccMediaStartedL( aEvent );

    MCEMM_DEBUG("TMcePreparingStreams::MccMediaPausedL(), Exit ");
    }


// -----------------------------------------------------------------------------
// TMcePreparingStreams::MccMediaResumedL
// -----------------------------------------------------------------------------
//
void TMcePreparingStreams::MccMediaResumedL( TMceMccComEvent& aEvent )
    {
    MCEMM_DEBUG("TMcePreparingStreams::MccMediaResumedL(), Entry ");

    MccMediaStartedL( aEvent );

    MCEMM_DEBUG("TMcePreparingStreams::MccMediaResumedL(), Exit ");
    }


// -----------------------------------------------------------------------------
// TMcePreparingStreams::PrepareStreamsL
// -----------------------------------------------------------------------------
//

void TMcePreparingStreams::PrepareStreamsL()
    {
    MCEMM_DEBUG("TMcePreparingStreams::PrepareStreamsL(), Entry ");
    
    CMceSrvStream* stream = NULL;
    TBool wait = EFalse;
    
    iIterator.Reset();
    while( iIterator.Next( stream, CMceSrvStream::ECreated ) )
        {
        MCEMM_DEBUG_STREAM( "TMcePreparingStreams::PrepareStreamsL(): \
create link", *stream );
        iSession.SdpSession().Manager().CreateMccLinkL( *stream );
        wait = !wait ? stream->State() != CMceSrvStream::ELinkCreated : wait;
        }

    if ( !wait )
        {
            
        iIterator.Reset();
        wait = EFalse;
        while( iIterator.Next( stream, CMceSrvStream::ELinkCreated ) )
            {
            MCEMM_DEBUG_STREAM( "TMcePreparingStreams::PrepareStreamsL(): \
create stream", *stream );
            iSession.SdpSession().Manager().CreateMccStreamL( *stream, iRole );
            }

        iIterator.Reset();
        wait = EFalse;
        while( iIterator.Next( stream, CMceSrvStream::ELinkCreated ) )
            {
            MCEMM_DEBUG_STREAM( "TMcePreparingStreams::PrepareStreamsL(): \
prepare stream", *stream );
            iSession.SdpSession().Manager().PrepareMccStreamL( *stream, iRole );
            wait = !wait ? stream->State() != CMceSrvStream::EPrepared : wait;
            }
    
        if (!wait)
            {
            MCEMM_DEBUG("TMcePreparingStreams::PrepareStreamsL(): streams prepared");
            StreamsPreparedL();
            }

        }
        
    MCEMM_DEBUG("TMcePreparingStreams::PrepareStreamsL(), Exit ");
        
    }


// -----------------------------------------------------------------------------
// TMcePreparingStreams::StartStreamsL
// -----------------------------------------------------------------------------
//
void TMcePreparingStreams::StartStreamsL( TBool aUpdateRemoteAddress )
    {
    MCEMM_DEBUG( "TMcePreparingStreams::StartStreamsL(), Entry" )
    
    // Hardware does not necessarily support two simulataneous mic users so
    // it is more safe to do pausing of old send stream before starting the
    // new one. This can be achieved by traversing streams in reverse order
    // as they are in priority order and new send stream has higher priority.
    //
    RPointerArray<CMceSrvStream>& origStreams = iSession.MccStreams();
    RPointerArray<CMceSrvStream> reverseOrderStream;
    CleanupClosePushL( reverseOrderStream );
    TInt lastIndex( origStreams.Count() - 1 );
    for ( TInt i = lastIndex; i >= 0; i-- )
        {
        reverseOrderStream.AppendL( origStreams[ i ] );
        }
    
    TMceSrvStreamIterator reversedIterator( reverseOrderStream, 
        iIterator.Direction() );
    
    TBool wait = EFalse;
    CMceSrvStream* stream = NULL;
    
    while( reversedIterator.Next( stream, CMceSrvStream::EPrepared, 
                            TMceSrvStreamIterator::GreaterOrEqualMatch ) )
        {
        const CMceSrvStream::TState state = stream->State();
        
        if ( state == CMceSrvStream::EPrepared &&
             stream->Codec().iIsEnabled )
            {
            MCEMM_DEBUG_STREAM( "TMcePreparingStreams::StartStreamsL() start stream",
                *stream )
			
            iSession.SdpSession().Manager().StartMccStreamL( *stream );
            wait = !wait ? stream->State() == CMceSrvStream::EStarting : wait;
            }
        else if ( aUpdateRemoteAddress )
            {
            MCEMM_DEBUG( "TMcePreparingStreams::StartStreamsL() remote address update" )
            iSession.SdpSession().Manager().SetRemoteAddressL( *stream );
            }
        else
            {
            // NOP
            }
            
        MCEMM_DEBUG_STREAM( "TMcePreparingStreams::StartStreamsL() synchronize stream",
            *stream )
        
        iSession.SdpSession().Manager().SynchronizeMccStreamL( *stream, iRole );    
        }
    
    if ( !wait )
        {
        MCEMM_DEBUG( "TMcePreparingStreams::StartStreamsL(): streams started" )
        StreamsStartedL();
        }
    
    CleanupStack::PopAndDestroy( &reverseOrderStream );
    
    MCEMM_DEBUG( "TMcePreparingStreams::StartStreamsL(), Exit" )
    }

