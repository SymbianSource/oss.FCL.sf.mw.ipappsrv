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




#include "mcemediaidle.h"
#include "mcepreparingoffererstreams.h"
#include "mceansweringmedia.h"
#include "mcecomsession.h"
#include "mcesdpsession.h"
#include "mcemmlogs.h"


// -----------------------------------------------------------------------------
// TMceMediaIdle::SetAsCurrentStateL
// -----------------------------------------------------------------------------
//
void TMceMediaIdle::SetAsCurrentStateL( CMceComSession& aSession )
    {
    MCEMM_DEBUG("TMceMediaIdle::SetAsCurrentStateL() ");
    
    TMceMediaIdle*
        self = new (ELeave) TMceMediaIdle( aSession );

    CleanupStack::PushL( self );
    aSession.SetNegotiationStateL( self );
    CleanupStack::Pop( self );
    
    }

// -----------------------------------------------------------------------------
// TMceMediaIdle::TMceMediaIdle
// -----------------------------------------------------------------------------
//
TMceMediaIdle::TMceMediaIdle ( CMceComSession& aSession )
    : TMceMediaState( aSession )
    {
    }


// -----------------------------------------------------------------------------
// TMceMediaIdle::DecodeL
// -----------------------------------------------------------------------------
//
TMceSipWarningCode TMceMediaIdle::DecodeL( CSdpDocument& aSdpDocument )
    {
    
    MCEMM_DEBUG("TMceMediaIdle::DecodeL(), Entry ");
    SetRole( EMceRoleAnswerer );
    
    TMceSipWarningCode code = 
        iSession.SdpSession().DecodeOfferL( aSdpDocument, iSession );
        
    MCEMM_DEBUG("TMceMediaIdle::DecodeL(), Exit ");
    
    return code;
    
    }


// -----------------------------------------------------------------------------
// TMceMediaIdle::UpdateL
// -----------------------------------------------------------------------------
//
void TMceMediaIdle::UpdateL()
    {

    MCEMM_DEBUG("TMceMediaIdle::UpdateL(), Entry ");
    
    if ( iRole == EMceRoleOfferer )
        {
        OfferL();
        }
    else
        {
        AnswerL();
        }
    
    MCEMM_DEBUG("TMceMediaIdle::UpdateL(), Exit ");
    }

    
// -----------------------------------------------------------------------------
// TMceMediaIdle::OfferL
// -----------------------------------------------------------------------------
//
void TMceMediaIdle::OfferL()
    {
    MCEMM_DEBUG("TMceMediaIdle::OfferL(), Entry ");
    
    // for all streams requested, copy bound stream information and validate
    iSession.PrepareL();
    // create mcc streams structure
    iSession.PrepareL( iSession.SdpSession().Manager() );

    //prepare all inactive streams for offer
    TMceSrvStreamIterator streams( iSession.MccStreams() );
    CMceSrvStream* stream = NULL;
    while( streams.Next( stream, CMceSrvStream::EInactive ) )
        {
        iSession.SdpSession().Manager().ValidateMccStreamL( *stream );
        MCEMM_DEBUG_STREAM( "TMceMediaIdle::OfferL(): validated inactive stream", *stream );
        }
    
    //set next state, this state will become unusable
    TMcePreparingOffererStreams::SetAsCurrentStateL( iSession );
    
    MCEMM_DEBUG("TMceMediaIdle::OfferL(), Exit ");
    }


// -----------------------------------------------------------------------------
// TMceMediaIdle::AnswerL
// -----------------------------------------------------------------------------
//
void TMceMediaIdle::AnswerL()
    {
    MCEMM_DEBUG("TMceMediaIdle::AnswerL(), Entry ");

    // for all streams requested, copy bound stream information and validate
    iSession.PrepareL();
    // create mcc streams structure
    iSession.PrepareL( iSession.SdpSession().Manager() );

    TMceSrvStreamIterator streams( iSession.MccStreams() );
    CMceSrvStream* stream = NULL;
    
    while( streams.Next( stream ) )
        {
        iSession.SdpSession().Manager().InitializeMccStreamL( *stream );
        iSession.SdpSession().Manager().ValidateMccStreamL( *stream );
        MCEMM_DEBUG_STREAM( "TMceMediaIdle::AnswerL(): \
initialized & validated stream", *stream );
        }
        
    //set next state, this state will become unusable
    TMceAnsweringMedia::SetAsCurrentStateL( iSession );

    
    MCEMM_DEBUG("TMceMediaIdle::AnswerL(), Exit ");
    }


