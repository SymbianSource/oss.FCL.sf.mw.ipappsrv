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




#include "mceofferingmedia.h"
#include "mcemediamanager.h"
#include "mcestartingofferersendstreams.h"
#include "mcesdpsession.h"
#include "mcecomsession.h"
#include "mcemediaobserver.h"
#include "mcesrvstreamiterator.h"
#include "mceevents.h"
#include "mcemmlogs.h"
#include <sdpdocument.h>




// -----------------------------------------------------------------------------
// TMceOfferingMedia::SetAsCurrentStateL
// -----------------------------------------------------------------------------
//
void TMceOfferingMedia::SetAsCurrentStateL( CMceComSession& aSession, TBool aFromSubstate )
    {
    MCEMM_DEBUG("TMceOfferingMedia::SetAsCurrentStateL() ");
    
    if ( !aFromSubstate )
        {
        TBool signalling = aSession.SdpSession().SignallingRequired() != KMceNoSignalling ||
                           aSession.iFcSignallingRequired ||
                           aSession.iRequireSignalling;
                           
        MCEMM_DEBUG_DVALUE( "TMceOfferingMedia: fc signalling required:", 
                            aSession.iFcSignallingRequired );
                            
        MCEMM_DEBUG_DVALUE( "TMceOfferingMedia: signalling required:", 
                            aSession.iRequireSignalling );
                            
        TBool update = MCE_NOT_NULL_PTR( aSession.Backup() );
        
        if ( !update )
            {
            if ( !signalling )
                //wait answer offer
                {
                MCEMM_DEBUG("TMceOfferingMedia: waiting answer to offer ");
                TMceOfferingMedia::TMceMediaReady::SetAsCurrentStateL( aSession );
                }
            else
                {
                //wait answer re-offer
                MCEMM_DEBUG("TMceOfferingMedia: waiting answer to update ");
                TMceOfferingMedia::TMceMediaUpdateReady::SetAsCurrentStateL( aSession );
                }
            }
        else//update
            {
            if ( !signalling )
                //no signalling pass through
                {
                MCEMM_DEBUG("TMceOfferingMedia: update - no signalling, no waiting");
                TMceStartingOffererSendStreams::SetAsCurrentStateL( aSession );
                }
            else
                {
                MCEMM_DEBUG("TMceOfferingMedia: update - waiting answer to update ");
                TMceOfferingMedia::TMceMediaUpdateReady::SetAsCurrentStateL( aSession );
                }
            }
        }
   else
        {
        TMceOfferingMedia*
            self = new (ELeave) TMceOfferingMedia( aSession );

        CleanupStack::PushL( self );
        aSession.SetNegotiationStateL( self );
        CleanupStack::Pop( self );
        
        }
   }


// -----------------------------------------------------------------------------
// TMceOfferingMedia::TMceMediaReady::SetAsCurrentStateL
// -----------------------------------------------------------------------------
//
void TMceOfferingMedia::TMceMediaReady::SetAsCurrentStateL( CMceComSession& aSession )
    {
    MCEMM_DEBUG("TMceOfferingMedia::TMceMediaReady::SetAsCurrentStateL() ");
    
    TMceOfferingMedia::TMceMediaReady*
        self = new (ELeave) TMceOfferingMedia::TMceMediaReady( aSession );
    
    CleanupStack::PushL( self );
    aSession.SetNegotiationStateL( self );
    CleanupStack::Pop( self );
    
    }

// -----------------------------------------------------------------------------
// TMceOfferingMedia::TMceMediaUpdateReady::SetAsCurrentStateL
// -----------------------------------------------------------------------------
//
void TMceOfferingMedia::TMceMediaUpdateReady::SetAsCurrentStateL( CMceComSession& aSession )
    {
    MCEMM_DEBUG("TMceOfferingMedia::TMceMediaUpdateReady::SetAsCurrentStateL() ");
    
    TMceOfferingMedia::TMceMediaUpdateReady*
        self = new (ELeave) TMceOfferingMedia::TMceMediaUpdateReady( aSession );

    CleanupStack::PushL( self );
    aSession.SetNegotiationStateL( self );
    CleanupStack::Pop( self );
    
    }

// -----------------------------------------------------------------------------
// TMceOfferingMedia::TMceWaitingAnswer::SetAsCurrentStateL
// -----------------------------------------------------------------------------
//
void TMceOfferingMedia::TMceWaitingAnswer::SetAsCurrentStateL( CMceComSession& aSession )
    {
    MCEMM_DEBUG("TMceOfferingMedia::TMceWaitingAnswer::SetAsCurrentStateL() ");

    TMceOfferingMedia::TMceWaitingAnswer*
        self = new (ELeave) TMceOfferingMedia::TMceWaitingAnswer( aSession );

    CleanupStack::PushL( self );
    aSession.SetNegotiationStateL( self );
    CleanupStack::Pop( self );

    }


// -----------------------------------------------------------------------------
// TMceOfferingMedia::TMceOfferingMedia
// -----------------------------------------------------------------------------
//
TMceOfferingMedia::TMceOfferingMedia( CMceComSession& aSession )
    : TMceMediaState( aSession )
    {
    }

// -----------------------------------------------------------------------------
// TMceOfferingMedia::TMceMediaReady::TMceMediaReady
// -----------------------------------------------------------------------------
//
TMceOfferingMedia::TMceMediaReady::TMceMediaReady( CMceComSession& aSession )
    : TMceMediaState( aSession )
    {
    }

// -----------------------------------------------------------------------------
// TMceOfferingMedia::TMceMediaUpdateReady::TMceMediaUpdateReady
// -----------------------------------------------------------------------------
//
TMceOfferingMedia::TMceMediaUpdateReady::TMceMediaUpdateReady( CMceComSession& aSession )
    : TMceOfferingMedia::TMceMediaReady( aSession )
    {
    }

// -----------------------------------------------------------------------------
// TMceOfferingMedia::TMceWaitingAnswer::TMceWaitingAnswer
// -----------------------------------------------------------------------------
//
TMceOfferingMedia::TMceWaitingAnswer::TMceWaitingAnswer( CMceComSession& aSession )
    : TMceMediaState( aSession )
    {
    }



// -----------------------------------------------------------------------------
// TMceOfferingMedia::TMceMediaReady::Callback
// -----------------------------------------------------------------------------
//
void TMceOfferingMedia::TMceMediaReady::Callback()
    {
    
    iSession.MediaObserver().Updated( iSession );
    
    }


// -----------------------------------------------------------------------------
// TMceOfferingMedia::TMceMediaReady::EncodeL
// -----------------------------------------------------------------------------
//
CSdpDocument* TMceOfferingMedia::TMceMediaReady::EncodeL()
    {
    MCEMM_DEBUG("TMceOfferingMedia::TMceMediaReady::EncodeL, Entry ");

    CSdpDocument* sdp = iSession.SdpSession().CreateOfferL( iSession,
                                                            CMceSdpSession::EFirst );
    CleanupStack::PushL( sdp );
    iSession.SdpSession().StockMediaSessionL();
    TMceOfferingMedia::TMceWaitingAnswer::SetAsCurrentStateL( iSession );
    CleanupStack::Pop( sdp );
    
    MCEMM_DEBUG("TMceOfferingMedia::TMceMediaReady::EncodeL, Exit ");
    
    return sdp;
    
    }

// -----------------------------------------------------------------------------
// TMceOfferingMedia::TMceMediaUpdateReady::EncodeL
// -----------------------------------------------------------------------------
//
CSdpDocument* TMceOfferingMedia::TMceMediaUpdateReady::EncodeL()
    {
    
    MCEMM_DEBUG("TMceOfferingMedia::TMceMediaUpdateReady::EncodeL, Entry ");
    
    CSdpDocument* sdp = iSession.SdpSession().CreateOfferL( iSession, 
                                                            CMceSdpSession::EUpdate );
    
    // Sdp not pushed, ownership elsewhere
    TMceOfferingMedia::TMceWaitingAnswer::SetAsCurrentStateL( iSession );
    
    MCEMM_DEBUG("TMceOfferingMedia::TMceMediaUpdateReady::EncodeL, Exit ");
    
    return sdp;

    }


// -----------------------------------------------------------------------------
// TMceOfferingMedia::TMceMediaUpdateReady::NeedToNegotiate
// -----------------------------------------------------------------------------
//
TBool TMceOfferingMedia::TMceMediaUpdateReady::NeedToNegotiate( CSdpDocument* /*aSdpDocument*/ )
    {
    MCEMM_DEBUG("TMceOfferingMedia::TMceMediaUpdateReady::NeedToNegotiate() ");
    
    return ETrue;
    }


// -----------------------------------------------------------------------------
// TMceOfferingMedia::TMceWaitingAnswer::CloneL
// -----------------------------------------------------------------------------
//
TMceMediaState* TMceOfferingMedia::TMceWaitingAnswer::CloneL( CMceComSession& aSession ) const
    {
    TMceOfferingMedia::TMceWaitingAnswer*
        state = new (ELeave) TMceOfferingMedia::TMceWaitingAnswer( aSession );
    
    state->SetRole( Role() );
    state->iStackIndex = iStackIndex;
    
    return state;
    
    }

// -----------------------------------------------------------------------------
// TMceOfferingMedia::TMceWaitingAnswer::DecodeL
// -----------------------------------------------------------------------------
//
TMceSipWarningCode TMceOfferingMedia::TMceWaitingAnswer::DecodeL( CSdpDocument& aSdpDocument )
    {
    MCEMM_DEBUG("TMceOfferingMedia::TMceWaitingAnswer::DecodeL, Entry ");

    TMceSipWarningCode status = (TMceSipWarningCode)KErrNone;
    
    if ( !iSession.SdpSession().ContextSwitchRequested() )
        {
        status = iSession.SdpSession().DecodeAnswerL( aSdpDocument, iSession );
        TMceOfferingMedia::SetAsCurrentStateL( iSession, KFromSubstate );
        }
    else
        {
        CMceSdpSession& forkedSdpSession = iSession.SdpSession().ForkL();
        CMceComSession& forkedSession = *forkedSdpSession.MediaSession();
        status = forkedSession.NegotiationState().DecodeL( aSdpDocument );
        }

    
    MCEMM_DEBUG("TMceOfferingMedia::TMceWaitingAnswer::DecodeL, Exit ");
    
    return status;
    
    }

// -----------------------------------------------------------------------------
// TMceOfferingMedia::TMceMediaReady::NeedToNegotiate
// -----------------------------------------------------------------------------
//
TBool TMceOfferingMedia::TMceWaitingAnswer::NeedToNegotiate( CSdpDocument* /*aSdpDocument*/ )
    {
    MCEMM_DEBUG("TMceOfferingMedia::TMceWaitingAnswer::NeedToNegotiate() ");
    
    return ETrue;
    }

// -----------------------------------------------------------------------------
// TMceOfferingMedia::UpdateL
// -----------------------------------------------------------------------------
//

void TMceOfferingMedia::UpdateL()
    {

    MCEMM_DEBUG("TMceOfferingMedia::UpdateL(), Entry ");
    
    CMceSrvStream* stream = NULL;
    
    // check if non-local media was rejected, if yes then stop the stream
    iSession.SdpSession().Manager().CleanupL( iSession );
    
    //set remote ip address for all non stopped receive only streams (RTP keep alive)
    TMceSrvStreamIterator recvonlyStreams( iSession.MccStreams(), 
                                           TMceSrvStreamIterator::EReceive );
    while( recvonlyStreams.Next( stream, CMceSrvStream::EStopped, 
                                 TMceSrvStreamIterator::ExactReverseMatch ) )
                                     
        {
        if ( stream->State() != CMceSrvStream::EInactive &&
             stream->Direction() == CMceComMediaStream::EReceiveOnlyStream )
            {
            MCEMM_DEBUG_STREAM( "TMceOfferingMedia::UpdateL(): \
setting remote address for downlink stream", *stream );
            iSession.SdpSession().Manager().SetRemoteAddressL( *stream );
            }
        }
    
    //do validation & adjustments for all non stopped active send streams
    TMceSrvStreamIterator sendStreams( iSession.MccStreams(), TMceSrvStreamIterator::ESend );
    while( sendStreams.Next( stream, CMceSrvStream::EStopped, 
                             TMceSrvStreamIterator::ExactReverseMatch ) )
                                     
        {
        if ( stream->State() != CMceSrvStream::EInactive &&
             stream->Codec().iIsNegotiated )
            { 
            iSession.SdpSession().Manager().AdjustMccStreamL( *stream );
            MCEMM_DEBUG_STREAM( "TMceOfferingMedia::UpdateL(): adjusted stream", *stream );
            }
        }



    //set next state, this state will become unusable
    TMceStartingOffererSendStreams::SetAsCurrentStateL( iSession  );
    
    
    MCEMM_DEBUG("TMceOfferingMedia::UpdateL(), Exit ");
    }

