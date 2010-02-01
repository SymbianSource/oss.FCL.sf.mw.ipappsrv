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




#include "mcestartingoffererreceivestreams.h"
#include "mcemediamanager.h"
#include "mcemedianegotiated.h"
#include "mcesdpsession.h"
#include "mcecomsession.h"
#include "mcemediaobserver.h"
#include "mceofferingmedia.h"
#include "mcemmlogs.h"


// -----------------------------------------------------------------------------
// TMceStartingOffererReceiveStreams::SetAsCurrentStateL
// -----------------------------------------------------------------------------
//
void TMceStartingOffererReceiveStreams::SetAsCurrentStateL( CMceComSession& aSession )
    {
    MCEMM_DEBUG("TMceStartingOffererReceiveStreams::SetAsCurrentStateL() ");
    
    TMceStartingOffererReceiveStreams*
        self = new (ELeave) TMceStartingOffererReceiveStreams( aSession );

    CleanupStack::PushL( self );
    aSession.SetNegotiationStateL( self );
    CleanupStack::Pop( self );
    
    }

// -----------------------------------------------------------------------------
// TMceStartingOffererReceiveStreams::TMceStartingOffererReceiveStreams
// -----------------------------------------------------------------------------
//
TMceStartingOffererReceiveStreams::TMceStartingOffererReceiveStreams ( 
                                                CMceComSession& aSession )
    : TMcePreparingStreams( aSession,
                           EMceRoleOfferer,
                           TMceSrvStreamIterator::EReceiveOrLocal )
    {
    }


// -----------------------------------------------------------------------------
// TMceStartingOffererReceiveStreams::EntryL
// -----------------------------------------------------------------------------
//
void TMceStartingOffererReceiveStreams::EntryL()
    {
    StartStreamsL();
    }

// -----------------------------------------------------------------------------
// TMceStartingOffererReceiveStreams::StreamsPreparedL
// -----------------------------------------------------------------------------
//
void TMceStartingOffererReceiveStreams::StreamsPreparedL()
    {

    User::Leave( KErrArgument );
    
    }

// -----------------------------------------------------------------------------
// TMceStartingOffererReceiveStreams::StreamsStartedL
// -----------------------------------------------------------------------------
//
void TMceStartingOffererReceiveStreams::StreamsStartedL()
    {

    //set next state, this state will become unusable
    TMceOfferingMedia::SetAsCurrentStateL( iSession );
    

    }
