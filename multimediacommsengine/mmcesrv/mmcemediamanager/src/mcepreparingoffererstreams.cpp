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




#include "mcepreparingoffererstreams.h"
#include "mcestartingoffererreceivestreams.h"
#include "mcecomsession.h"
#include "mcesdpsession.h"
#include "mcemmlogs.h"

// -----------------------------------------------------------------------------
// TMcePreparingOffererStreams::SetAsCurrentStateL
// -----------------------------------------------------------------------------
//
void TMcePreparingOffererStreams::SetAsCurrentStateL( CMceComSession& aSession )
    {
    MCEMM_DEBUG("TMcePreparingOffererStreams::SetAsCurrentStateL() ");
    
    TMcePreparingOffererStreams*
        self = new (ELeave) TMcePreparingOffererStreams( aSession );

    CleanupStack::PushL( self );
    aSession.SetNegotiationStateL( self );
    CleanupStack::Pop( self );
    
    }

// -----------------------------------------------------------------------------
// TMcePreparingOffererStreams::TMcePreparingOffererStreams
// -----------------------------------------------------------------------------
//
TMcePreparingOffererStreams::TMcePreparingOffererStreams ( 
                                            CMceComSession& aSession )
                                            
    : TMcePreparingStreams( aSession,
                            EMceRoleOfferer,
                            TMceSrvStreamIterator::EAny )
    {
    }



// -----------------------------------------------------------------------------
// TMcePreparingOffererStreams::EntryL
// -----------------------------------------------------------------------------
//
void TMcePreparingOffererStreams::EntryL()
    {
    if ( !iSession.IsMerged() && iSession.iMccID == KMceNotAssigned )
        {
        iSession.SdpSession().Manager().CreateMccSessionL( iSession );
        }
        
	if ( iSession.iClientCryptoSuites.Count() )
		{
		iSession.SecureSessionL();
		}
        
    PrepareStreamsL();
    }

// -----------------------------------------------------------------------------
// TMcePreparingOffererStreams::StreamsPreparedL
// -----------------------------------------------------------------------------
//
void TMcePreparingOffererStreams::StreamsPreparedL()
    {
    
    //set next state, this state will become unusable
    TMceStartingOffererReceiveStreams::SetAsCurrentStateL( iSession );
    
    }

// -----------------------------------------------------------------------------
// TMcePreparingOffererStreams::StreamsStartedL
// -----------------------------------------------------------------------------
//
void TMcePreparingOffererStreams::StreamsStartedL()
    {

    User::Leave( KErrGeneral );
    
    }

