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




#include "mcemedianegotiated.h"
#include "mcemediamanager.h"
#include "mcecomsession.h"
#include "mcesdpsession.h"
#include "mceansweringmediaupdate.h"
#include "mcepreparingoffererstreams.h"
#include "mcepreparinganswererstreams.h"
#include "mceofferingmedia.h"
#include "mcemediaobserver.h"
#include "mcemmlogs.h"

// -----------------------------------------------------------------------------
// TMceMediaNegotiated::TMceEndState::SetAsCurrentStateL
// -----------------------------------------------------------------------------
//
void TMceMediaNegotiated::TMceEndState::SetAsCurrentStateL( CMceComSession& aSession )
    {
    MCEMM_DEBUG("TMceMediaNegotiated::TMceEndState::SetAsCurrentStateL() ");
    
    TMceMediaNegotiated::TMceEndState*
        self = new (ELeave) TMceMediaNegotiated::TMceEndState( aSession );

    CleanupStack::PushL( self );
    aSession.SetNegotiationStateL( self );
    CleanupStack::Pop( self );
    
    }


// -----------------------------------------------------------------------------
// TMceMediaNegotiated::TMceEndState::TMceEndState
// -----------------------------------------------------------------------------
//
TMceMediaNegotiated::TMceEndState::TMceEndState( CMceComSession& aSession )
    : TMceMediaState( aSession )
    {
    }


// -----------------------------------------------------------------------------
// TMceMediaNegotiated::TMceEndState::Callback
// -----------------------------------------------------------------------------
//
void TMceMediaNegotiated::TMceEndState::Callback()
    {

	iSession.MediaObserver().Updated( iSession );			
        
    }


// -----------------------------------------------------------------------------
// TMceMediaNegotiated::TMceEndState::NeedToNegotiate
// -----------------------------------------------------------------------------
//
TBool TMceMediaNegotiated::TMceEndState::NeedToNegotiate( CSdpDocument* aSdpDocument )
    {
    TBool needToNegotiate = EFalse;

    if ( aSdpDocument )
        {
        MCEMM_DEBUG("TMceMediaNegotiated::TMceEndState::NeedToNegotiate(): \
checking if session refresh");
        needToNegotiate = !iSession.SdpSession().IsSessionRefresh( aSdpDocument );
        }
    else if ( iSession.Backup() )//update
        {
        MCEMM_DEBUG("TMceMediaNegotiated::TMceEndState::NeedToNegotiate(): \
session update. checking if signalling is required");
        needToNegotiate = iSession.SdpSession().SignallingRequired() != KMceNoSignalling;
        }
    else
        {
        MCEMM_DEBUG("TMceMediaNegotiated::TMceEndState::NeedToNegotiate(): \
checking preconditions");
        needToNegotiate = iSession.SdpSession().NeedToNegotiate( iSession );
        }

    MCEMM_DEBUG_DVALUE("TMceMediaNegotiated::TMceEndState::NeedToNegotiate(): \
return", needToNegotiate );
    return needToNegotiate;
    
    }

// -----------------------------------------------------------------------------
// TMceMediaNegotiated::TMceEndState::EncodeL
// -----------------------------------------------------------------------------
//
CSdpDocument* TMceMediaNegotiated::TMceEndState::EncodeL()
    {
    
    //if answer to session refresh, offer is generated to answer aswell
    
    if ( iSession.SdpSession().IsSessionRefresh() )
        {
        iSession.SdpSession().PrepareSessionRefreshL( iSession );
        }
    
    CSdpDocument* sdp = iSession.SdpSession().CreateOfferL( iSession, CMceSdpSession::ERefresh );
    
    return sdp;

    }


// -----------------------------------------------------------------------------
// TMceMediaNegotiated::TMceEndState::UpdateL
// -----------------------------------------------------------------------------
//
void TMceMediaNegotiated::TMceEndState::UpdateL()
    {
    
    if ( iSession.AnswerType() == KMceNegotiationAnswerTypeFinal )
        {
        iSession.SdpSession().Manager().CleanupL( iSession );
        iSession.AnswerType() = KMceNegotiationAnswerTypeNotDefined;
        }
    else
        {
        iSession.SdpSession().SignallingRequired() = KMceRequiresSipSignallingOnly;
        iSession.AnswerType() = KMceNegotiationAnswerTypeNotDefined;
        TMcePreparingOffererStreams::SetAsCurrentStateL( iSession );
        }
        
    }
    
// -----------------------------------------------------------------------------
// TMceMediaNegotiated::TMceEndState::CloneL
// -----------------------------------------------------------------------------
//
TMceMediaState* TMceMediaNegotiated::TMceEndState::CloneL( CMceComSession& aSession ) const
    {
    TMceMediaNegotiated::TMceEndState*
        state = new (ELeave) TMceMediaNegotiated::TMceEndState( aSession );
    
    state->SetRole( Role() );
    state->iStackIndex = iStackIndex;
    
    return state;
    
    }

    
// -----------------------------------------------------------------------------
// TMceMediaNegotiated::TMceEndState::DecodeL
// -----------------------------------------------------------------------------
//
TMceSipWarningCode TMceMediaNegotiated::TMceEndState::DecodeL( CSdpDocument& aSdpDocument )
    {
    MCEMM_DEBUG("TMceOfferingMedia::TMceMediaNegotiated::DecodeL, Entry ");

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
    
    MCEMM_DEBUG("TMceOfferingMedia::TMceMediaNegotiated::DecodeL, Exit ");
    
    return status;
    
    
    }
    
// -----------------------------------------------------------------------------
// TMceMediaNegotiated::TMceAnswererWaitingAnswerState::SetAsCurrentStateL
// -----------------------------------------------------------------------------
//
void TMceMediaNegotiated::TMceAnswererWaitingAnswerState::SetAsCurrentStateL( 
      CMceComSession& aSession )
    {

    TMceMediaNegotiated::TMceAnswererWaitingAnswerState*
        self = new (ELeave) TMceMediaNegotiated::TMceAnswererWaitingAnswerState( aSession );

    CleanupStack::PushL( self );
    aSession.SetNegotiationStateL( self );
    CleanupStack::Pop( self );
    
    }

// -----------------------------------------------------------------------------
// TMceMediaNegotiated::TMceAnswererEndState::SetAsCurrentStateL
// -----------------------------------------------------------------------------
//
void TMceMediaNegotiated::TMceAnswererEndState::SetAsCurrentStateL( CMceComSession& aSession )
    {
    MCEMM_DEBUG("TMceMediaNegotiated::TMceAnswererEndState::SetAsCurrentStateL() ");
    
    TMceMediaNegotiated::TMceAnswererEndState*
        self = new (ELeave) TMceMediaNegotiated::TMceAnswererEndState( aSession );

    CleanupStack::PushL( self );
    aSession.SetNegotiationStateL( self );
    CleanupStack::Pop( self );
    
    }


// -----------------------------------------------------------------------------
// TMceMediaNegotiated::TMceAnswererEndState::TMceAnswererEndState
// -----------------------------------------------------------------------------
//
TMceMediaNegotiated::TMceAnswererEndState::TMceAnswererEndState( CMceComSession& aSession )
    : TMceEndState( aSession )
    {
    SetRole( EMceRoleAnswerer );
    }

// -----------------------------------------------------------------------------
// TMceMediaNegotiated::TMceAnswererWaitingAnswerState::TMceAnswererWaitingAnswerState
// -----------------------------------------------------------------------------
//
TMceMediaNegotiated::TMceAnswererWaitingAnswerState::TMceAnswererWaitingAnswerState( 
                                                        CMceComSession& aSession )
    : TMceEndState( aSession )
    {
    SetRole( EMceRoleAnswerer );
    }



// -----------------------------------------------------------------------------
// TMceMediaNegotiated::TMceAnswererWaitingAnswerState::EncodeL
// -----------------------------------------------------------------------------
//
CSdpDocument* TMceMediaNegotiated::TMceAnswererWaitingAnswerState::EncodeL()
    {
    MCEMM_DEBUG("TMceMediaNegotiated::TMceAnswererWaitingAnswerState::EncodeL(), Entry ");
        
    CSdpDocument* answer = &iSession.SdpSession().CreateAnswerL( iSession );
    
    TMceMediaNegotiated::TMceAnswererEndState::SetAsCurrentStateL( iSession );
    
    MCEMM_DEBUG("TMceMediaNegotiated::TMceAnswererWaitingAnswerState::EncodeL(), Exit ");
    
    return answer;
    
    }

// -----------------------------------------------------------------------------
// TMceMediaNegotiated::TMceAnswererWaitingAnswerState::EncodeL
// -----------------------------------------------------------------------------
//
TBool TMceMediaNegotiated::TMceAnswererWaitingAnswerState::NeedToNegotiate( 
                                                      CSdpDocument* /*aSdpDocument*/ )
    {
    MCEMM_DEBUG("TMceMediaNegotiated::TMceAnswererWaitingAnswerState::NeedToNegotiate() ");
    
    return ETrue;
    }


// -----------------------------------------------------------------------------
// TMceMediaNegotiated::TMceAnswererEndState::DecodeL
// -----------------------------------------------------------------------------
//
TMceSipWarningCode TMceMediaNegotiated::TMceAnswererEndState::DecodeL( CSdpDocument& aSdpDocument )
    {
    MCEMM_DEBUG("TMceMediaNegotiated::TMceAnswererEndState::DecodeL(), Entry ");
    
    iSession.SdpSession().iStoreRemoteOrigin = EFalse;
    TMceSipWarningCode code =
         iSession.SdpSession().DecodeOfferL( aSdpDocument, iSession );
    iSession.SdpSession().iStoreRemoteOrigin = ETrue;
    
    MCEMM_DEBUG("TMceMediaNegotiated::TMceAnswererEndState::DecodeL(), Exit ");
    
    return code;
    
    }

// -----------------------------------------------------------------------------
// TMceMediaNegotiated::TMceAnswererEndState::UpdateL
// -----------------------------------------------------------------------------
//
void TMceMediaNegotiated::TMceAnswererEndState::UpdateL()
    {
    MCEMM_DEBUG("TMceMediaNegotiated::TMceAnswererEndState::UpdateL(), Entry ");
    
    TMcePreparingAnswererStreams::SetAsCurrentStateL( iSession );
    
    MCEMM_DEBUG("TMceMediaNegotiated::TMceAnswererEndState::UpdateL(), Exit ");
    }


// -----------------------------------------------------------------------------
// TMceMediaNegotiated::SetAsCurrentStateL
// -----------------------------------------------------------------------------
//
void TMceMediaNegotiated::SetAsCurrentStateL( CMceComSession& aSession, 
                                              TMceNegotiationRole aRole )
    {
    MCEMM_DEBUG("TMceMediaNegotiated::SetAsCurrentStateL() ");
    
    if ( aRole == EMceRoleOfferer )
        {
        TMceMediaNegotiated::TMceEndState::SetAsCurrentStateL( aSession );
        }
    else
        {
        TMceMediaNegotiated::TMceAnswererWaitingAnswerState::SetAsCurrentStateL( aSession );
        }
        
    }

// -----------------------------------------------------------------------------
// TMceMediaNegotiated::TMceMediaNegotiated
// -----------------------------------------------------------------------------
//
TMceMediaNegotiated::TMceMediaNegotiated ( CMceComSession& aSession )
    : TMceMediaState( aSession )
	{
	}


// -----------------------------------------------------------------------------
// TMceMediaNegotiated::DecodeL
// -----------------------------------------------------------------------------
//
TMceSipWarningCode TMceMediaNegotiated::DecodeL( CSdpDocument& aSdpDocument )
    {

    MCEMM_DEBUG("TMceMediaNegotiated::DecodeL(), Entry ");
    
    SetRole( EMceRoleAnswerer );
    
    TMceSipWarningCode code =
         iSession.SdpSession().DecodeOfferL( aSdpDocument, iSession );
    
    MCEMM_DEBUG("TMceMediaNegotiated::DecodeL(), Exit ");
    
    return code;
    
    }


// -----------------------------------------------------------------------------
// TMceMediaNegotiated::UpdateL
// -----------------------------------------------------------------------------
//
void TMceMediaNegotiated::UpdateL()
    {

    MCEMM_DEBUG("TMceMediaNegotiated::UpdateL(), Entry ");
    if ( iRole == EMceRoleOfferer )
        {
        OfferL();
        }
    else
        {
        AnswerL();
        }
    MCEMM_DEBUG("TMceMediaNegotiated::UpdateL(), Exit ");
    }
        


// -----------------------------------------------------------------------------
// TMceMediaNegotiated::OfferL
// -----------------------------------------------------------------------------
//

void TMceMediaNegotiated::OfferL()
    {
    MCEMM_DEBUG("TMceMediaNegotiated::OfferL(), Entry ");
    
    // re-create mcc streams structure
    iSession.PrepareL( iSession.SdpSession().Manager() );
    
    iSession.SdpSession().Manager().PrepareUpdateL( iSession.SdpSession() );
    
    if ( iSession.SdpSession().SignallingRequired() != KMceNoSignalling )
        {
        //prepare all inactive streams for offer
        TMceSrvStreamIterator streams( iSession.MccStreams() );
        CMceSrvStream* stream = NULL;
    
        while( streams.Next( stream, CMceSrvStream::EInactive ) )
            {
            iSession.SdpSession().Manager().ValidateMccStreamL( *stream );
            MCEMM_DEBUG_STREAM( "TMceMediaNegotiated::OfferL(): \
validated inactive stream", *stream );
            }
        }
        

    //set next state, this state will become unusable
    TMcePreparingOffererStreams::SetAsCurrentStateL( iSession );

    MCEMM_DEBUG("TMceMediaNegotiated::OfferL(), Exit ");
    }


// -----------------------------------------------------------------------------
// TMceMediaNegotiated::AnswerL
// -----------------------------------------------------------------------------
//
/*lint -e666 */

void TMceMediaNegotiated::AnswerL()
    {
    MCEMM_DEBUG("TMceMediaNegotiated::AnswerL(), Entry ");
    
	//second incoming offer (prack) inside dialog iCodecSelection==
	// KMceCodecSelectionMerge.In this situation codec is not allowed to 
	//initialize, codec is not updated by mceclient
	if ( iSession.iCodecSelection != KMceCodecSelectionMerge )
		{
    	// create mcc streams structure
    	iSession.PrepareL( iSession.SdpSession().Manager() );

    	TMceSrvStreamIterator streams( iSession.MccStreams() );
    	CMceSrvStream* stream = NULL;
    
    	while( streams.Next( stream ) )
        	{
        	iSession.SdpSession().Manager().InitializeMccStreamL( *stream );
        	iSession.SdpSession().Manager().ValidateMccStreamL( *stream );
        	MCEMM_DEBUG_STREAM( "TMceMediaNegotiated::AnswerL(): \
			initialized & validated stream", *stream );
        
        	}
		}
    //set next state, this state will become unusable
    TMceAnsweringMediaUpdate::SetAsCurrentStateL( iSession );

    
    MCEMM_DEBUG("TMceMediaNegotiated::AnswerL(), Exit ");
    }

