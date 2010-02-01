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




#include "mcepreparinganswererstreams.h"
#include "mcestartinganswererstreams.h"
#include "mcecomsession.h"
#include "mcesdpsession.h"
#include "mcemmlogs.h"

// -----------------------------------------------------------------------------
// TMcePreparingAnswererStreams::SetAsCurrentStateL
// -----------------------------------------------------------------------------
//
void TMcePreparingAnswererStreams::SetAsCurrentStateL( CMceComSession& aSession )
    {
    MCEMM_DEBUG("TMcePreparingAnswererStreams::SetAsCurrentStateL() ");
    
    TMcePreparingAnswererStreams*
        self = new (ELeave) TMcePreparingAnswererStreams( aSession );

    CleanupStack::PushL( self );
    aSession.SetNegotiationStateL( self );
    CleanupStack::Pop( self );
    
    }

// -----------------------------------------------------------------------------
// TMcePreparingAnswererStreams::TMcePreparingAnswererStreams
// -----------------------------------------------------------------------------
//
TMcePreparingAnswererStreams::TMcePreparingAnswererStreams ( 
                                            CMceComSession& aSession )
                                            
    : TMcePreparingStreams( aSession,
                            EMceRoleAnswerer,
                            TMceSrvStreamIterator::EAny )
	{
	}



// -----------------------------------------------------------------------------
// TMcePreparingAnswererStreams::EntryL
// -----------------------------------------------------------------------------
//
void TMcePreparingAnswererStreams::EntryL()
    {
    if ( !iSession.IsMerged() && iSession.iMccID == KMceNotAssigned )
        {
        iSession.SdpSession().Manager().CreateMccSessionL( iSession );
        }

    
    if ( iSession.SecureSession() && iSession.iClientCryptoSuites.Count() == 0 )
        {
        User::Leave( KErrArgument );
        }
        
    PrepareStreamsL();
    
    }

// -----------------------------------------------------------------------------
// TMcePreparingAnswererStreams::StreamsPreparedL
// -----------------------------------------------------------------------------
//
void TMcePreparingAnswererStreams::StreamsPreparedL()
    {
    
	//set next state, this state will become unusable
	TMceStartingAnswererStreams::SetAsCurrentStateL( iSession );
	
    }

// -----------------------------------------------------------------------------
// TMcePreparingAnswererStreams::StreamsStartedL
// -----------------------------------------------------------------------------
//
void TMcePreparingAnswererStreams::StreamsStartedL()
    {

    User::Leave( KErrGeneral );
    
    }

