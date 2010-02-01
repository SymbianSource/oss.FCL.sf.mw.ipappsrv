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




#ifndef TMCEPREPARINGSTREAMS_H
#define TMCEPREPARINGSTREAMS_H

class CMceComSession;

#include "mcemediastate.h"
#include "mcesrvstreamiterator.h"
#include "mcemediamanager.h"
#include "mcesrv.h"

/**
*  Represents MM base class state for preparing receive stream
*
*/
class TMcePreparingStreams : public TMceMediaState
    {
    
public: // Constructors

    /**
    * Consructor
    */
    TMcePreparingStreams( CMceComSession& aSession,
                          TMceNegotiationRole aRole,
                          TMceSrvStreamIterator::TDirection aDirection );


    
public: // from TMceMediaState


    void MccLinkCreatedL( TMceMccComEvent& aEvent );

    void MccMediaPreparedL( TMceMccComEvent& aEvent );
    
    void MccMediaStartedL( TMceMccComEvent& aEvent );
    
    void MccMediaPausedL( TMceMccComEvent& aEvent );
    
    void MccMediaResumedL( TMceMccComEvent& aEvent );
    

protected://new functions

    
    void PrepareStreamsL();
    void StartStreamsL( TBool aUpdateRemoteAddress = EFalse );
    
    virtual void StreamsPreparedL() = 0;
    virtual void StreamsStartedL() = 0;

protected://data
    
    TMceSrvStreamIterator iIterator;
    
	//definitions for unit testing
	MCEMM_UT_DEFINITIONS
    
    };

#endif//TMCEPREPARINGSTREAMS_H

// End of File
