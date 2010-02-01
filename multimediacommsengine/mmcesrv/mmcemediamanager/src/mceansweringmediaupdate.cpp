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




#include "mceansweringmediaupdate.h"
#include "mcemediamanager.h"
#include "mcepreparinganswererstreams.h"
#include "mcemedianegotiated.h"
#include "mcecomsession.h"
#include "mcesdpsession.h"
#include "mcemmlogs.h"


// -----------------------------------------------------------------------------
// TMceAnsweringMediaUpdate::SetAsCurrentStateL
// -----------------------------------------------------------------------------
//
void TMceAnsweringMediaUpdate::SetAsCurrentStateL( CMceComSession& aSession )
    {
    MCEMM_DEBUG("TMceAnsweringMediaUpdate::SetAsCurrentStateL() ");
    
    TMceAnsweringMediaUpdate*
        self = new (ELeave) TMceAnsweringMediaUpdate( aSession );

    CleanupStack::PushL( self );
    aSession.SetNegotiationStateL( self );
    CleanupStack::Pop( self );
    
    }


// -----------------------------------------------------------------------------
// TMceAnsweringMediaUpdate::TMceAnsweringMediaUpdate
// -----------------------------------------------------------------------------
//
TMceAnsweringMediaUpdate::TMceAnsweringMediaUpdate ( CMceComSession& aSession )
    : TMceAnsweringMedia( aSession )
    {
    }


// -----------------------------------------------------------------------------
// TMceAnsweringMediaUpdate::UpdateL
// -----------------------------------------------------------------------------
//
/*lint -e666 */
void TMceAnsweringMediaUpdate::UpdateL()
    {

    MCEMM_DEBUG("TMceAnsweringMediaUpdate::UpdateL(), Entry ");

    //(re-)create mcc streams
    iSession.PrepareL( iSession.SdpSession().Manager() );

    //do validation
    TMceSrvStreamIterator streams( iSession.MccStreams() );
    CMceSrvStream* stream = NULL;
    while( streams.Next( stream ) )
        {
        iSession.SdpSession().Manager().ValidateMccStreamL( *stream );
        MCEMM_DEBUG_STREAM( "TMceAnsweringMediaUpdate::UpdateL(): validated stream", *stream );
        }
    
    iSession.SdpSession().Manager().PrepareUpdateL( iSession.SdpSession() );
    
    //set next state, this state will become unusable
    TMcePreparingAnswererStreams::SetAsCurrentStateL( iSession );
    
    MCEMM_DEBUG("TMceAnsweringMediaUpdate::UpdateL(), Exit ");
    
    }

