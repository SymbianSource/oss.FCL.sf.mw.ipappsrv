/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef M_NATFWCONNCHECKLISTENER_H
#define M_NATFWCONNCHECKLISTENER_H

#include <e32base.h>

class CNATFWCandidatePair;
class CNATFWCandidate;
class CIceConnectivityCheck;

/**
 *  Used to notify results of connectivity checks.
 *
 *  @lib icecandidatehandler.dll
 *  @since S60 v3.2
 */
class MIceConnCheckListener
    {

public:

    /**
     * Notifies result of a check.
     *
     * @since   S60 v3.2
     * @param   aCompletionCode     The completion code
     * @param   aCheck              The check which was completed
     * @param   aValidatedPair      Candidate pair validated by the check
     */
    virtual void CheckCompletedL( 
        TInt aCompletionCode,
        const CIceConnectivityCheck& aCheck,
        CNATFWCandidatePair* aValidatedPair ) = 0;

    /**
     * Notifies result of a nomination check.
     *
     * @since   S60 v3.2
     * @param   aCompletionCode     The completion code
     * @param   aCheck              The check
     * @param   aValidatedPair      Candidate pair validated by the check
     */
    virtual void NominationCompletedL( 
        TInt aCompletionCode,
        const CIceConnectivityCheck& aCheck,
        const CNATFWCandidatePair& aValidatedPair ) = 0;
    };


#endif // M_NATFWCONNCHECKLISTENER_H
