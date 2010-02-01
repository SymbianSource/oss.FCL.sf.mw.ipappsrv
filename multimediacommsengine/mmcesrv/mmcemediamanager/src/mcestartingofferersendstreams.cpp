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




#include "mcestartingofferersendstreams.h"
#include "mcemediamanager.h"
#include "mcemedianegotiated.h"
#include "mcesdpsession.h"
#include "mcecomsession.h"
#include "mcemediaobserver.h"
#include "mceofferingmedia.h"
#include "mcemmlogs.h"


// -----------------------------------------------------------------------------
// TMceStartingOffererSendStreams::SetAsCurrentStateL
// -----------------------------------------------------------------------------
//
void TMceStartingOffererSendStreams::SetAsCurrentStateL( CMceComSession& aSession )
    {
    MCEMM_DEBUG("TMceStartingOffererSendStreams::SetAsCurrentStateL() ");
    
    TMceStartingOffererSendStreams*
        self = new (ELeave) TMceStartingOffererSendStreams( aSession );

    CleanupStack::PushL( self );
    aSession.SetNegotiationStateL( self );
    CleanupStack::Pop( self );
    
    }

// -----------------------------------------------------------------------------
// TMceStartingOffererSendStreams::TMceStartingOffererSendStreams
// -----------------------------------------------------------------------------
//
TMceStartingOffererSendStreams::TMceStartingOffererSendStreams ( 
                                                CMceComSession& aSession )
    : TMcePreparingStreams( aSession,
                            EMceRoleOfferer,
                            TMceSrvStreamIterator::ESendOrLocal )
    {
    }


// -----------------------------------------------------------------------------
// TMceStartingOffererSendStreams::EntryL
// -----------------------------------------------------------------------------
//
void TMceStartingOffererSendStreams::EntryL()
    {
    StartStreamsL( ETrue );
    }

// -----------------------------------------------------------------------------
// TMceStartingOffererSendStreams::StreamsPreparedL
// -----------------------------------------------------------------------------
//
void TMceStartingOffererSendStreams::StreamsPreparedL()
    {

    User::Leave( KErrGeneral );
    
    }

// -----------------------------------------------------------------------------
// TMceStartingOffererSendStreams::StreamsStartedL
// -----------------------------------------------------------------------------
//
void TMceStartingOffererSendStreams::StreamsStartedL()
    {

    //set next state, this state will become unusable
    TMceMediaNegotiated::SetAsCurrentStateL( iSession, iRole );
    

    }
