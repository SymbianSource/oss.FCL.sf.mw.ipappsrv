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




#include "mceansweringmedia.h"
#include "mcemediamanager.h"
#include "mcepreparinganswererstreams.h"
#include "mcecomsession.h"
#include "mcesdpsession.h"
#include "mcemmlogs.h"


// -----------------------------------------------------------------------------
// TMceAnsweringMedia::SetAsCurrentStateL
// -----------------------------------------------------------------------------
//
void TMceAnsweringMedia::SetAsCurrentStateL( CMceComSession& aSession )
    {
    MCEMM_DEBUG("TMceAnsweringMedia::SetAsCurrentStateL() ");
    
    TMceAnsweringMedia*
        self = new (ELeave) TMceAnsweringMedia( aSession );

    CleanupStack::PushL( self );
    aSession.SetNegotiationStateL( self );
    CleanupStack::Pop( self );
    
    }


// -----------------------------------------------------------------------------
// TMceAnsweringMedia::TMceAnsweringMedia
// -----------------------------------------------------------------------------
//
TMceAnsweringMedia::TMceAnsweringMedia ( CMceComSession& aSession )
    : TMceMediaState( aSession )
    {
    SetRole( EMceRoleAnswerer );
    }



// -----------------------------------------------------------------------------
// TMceAnsweringMedia::UpdateL
// -----------------------------------------------------------------------------
//


void TMceAnsweringMedia::UpdateL()
    {

    MCEMM_DEBUG("TMceAnsweringMedia::UpdateL(), Entry ");

    //(re-)create mcc streams
    iSession.PrepareL( iSession.SdpSession().Manager() );
    
    //do validation
    TMceSrvStreamIterator streams( iSession.MccStreams() );
    CMceSrvStream* stream = NULL;
    while( streams.Next( stream ) )
        {
        iSession.SdpSession().Manager().ValidateMccStreamL( *stream );
        MCEMM_DEBUG_STREAM( "TMceAnsweringMedia::UpdateL(): validated stream", *stream );
        
        }
    
    //set next state, this state will become unusable
    TMcePreparingAnswererStreams::SetAsCurrentStateL( iSession );
    
    MCEMM_DEBUG("TMceAnsweringMedia::UpdateL(), Exit ");
    
    }


