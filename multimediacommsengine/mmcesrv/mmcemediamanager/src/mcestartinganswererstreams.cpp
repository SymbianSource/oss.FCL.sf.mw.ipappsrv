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




#include "mcestartinganswererstreams.h"
#include "mcemediamanager.h"
#include "mcemedianegotiated.h"
#include "mcesdpsession.h"
#include "mcecomsession.h"
#include "mcemmlogs.h"


// -----------------------------------------------------------------------------
// TMceStartingAnswererStreams::SetAsCurrentStateL
// -----------------------------------------------------------------------------
//
void TMceStartingAnswererStreams::SetAsCurrentStateL( CMceComSession& aSession )
    {
    MCEMM_DEBUG( "TMceStartingAnswererStreams::SetAsCurrentStateL()" )
    
    TMceStartingAnswererStreams*
        self = new (ELeave) TMceStartingAnswererStreams( aSession );

    CleanupStack::PushL( self );
    aSession.SetNegotiationStateL( self );
    CleanupStack::Pop( self );
    
    }


// -----------------------------------------------------------------------------
// TMceStartingAnswererStreams::TMceStartingAnswererStreams
// -----------------------------------------------------------------------------
//
TMceStartingAnswererStreams::TMceStartingAnswererStreams ( 
                                                CMceComSession& aSession )
    : TMcePreparingStreams( aSession,
                            EMceRoleAnswerer,
                            TMceSrvStreamIterator::EAny )
	{
	}


// -----------------------------------------------------------------------------
// TMceStartingAnswererStreams::EntryL
// -----------------------------------------------------------------------------
//
void TMceStartingAnswererStreams::EntryL()
    {
    MCEMM_DEBUG( "TMceStartingAnswererStreams::EntryL()" )
    
    // Do remote address update.
    // If old-school and new school holds are used together, remote address
    // was not updated to MCC.
    // When entering into a old-school + new school hold, remote address is set
    // as 0.0.0.0 to MCC and would not be updated if not done here.
    // Setting remote-address as 0.0.0.0 is now blocked also on mediamanager
    // level, but if remote-end updates it's media address during hold/unhold,
    // it needs to be taken into account here.
    StartStreamsL( ETrue );
    }

// -----------------------------------------------------------------------------
// TMceStartingAnswererStreams::StreamsPreparedL
// -----------------------------------------------------------------------------
//
void TMceStartingAnswererStreams::StreamsPreparedL()
    {
    User::Leave( KErrGeneral );
    }

// -----------------------------------------------------------------------------
// TMceStartingAnswererStreams::StreamsStartedL
// -----------------------------------------------------------------------------
//
void TMceStartingAnswererStreams::StreamsStartedL()
    {

	//set next state, this state will become unusable
	TMceMediaNegotiated::SetAsCurrentStateL( iSession, iRole );

    }
