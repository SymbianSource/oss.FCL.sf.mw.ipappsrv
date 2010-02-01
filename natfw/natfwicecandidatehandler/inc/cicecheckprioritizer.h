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
* Description:    Provides services for check prioritization.
*
*/




#ifndef C_NATFWCHECKPRIORITIZER_H
#define C_NATFWCHECKPRIORITIZER_H

#include <e32base.h>
#include "natfwconnectivityapidefs.h"
#include "natfwinternaldefs.h"    //UNIT_TEST

class CIceConnectivityCheck;
class CNATFWCandidatePair;

/**
 *  Provides services for check prioritization.
 *
 *  @lib icecandidatehandler.dll
 *  @since S60 v3.2
 */
class CIceCheckPrioritizer : public CBase
    {

    UNIT_TEST( UT_CIceCheckPrioritizer )
    
public:

    static CIceCheckPrioritizer* NewL();

    static CIceCheckPrioritizer* NewLC();

    /**
     * Destructor.
     */
    virtual ~CIceCheckPrioritizer();

    /**
     * Calculates priority for a candidate pair.
     *
     * @since   S60 v3.2
     * @param   aPair           The pair to prioritize
     * @param   aCurrentRole    The role of local agent
     */
    void PrioritizePair( CNATFWCandidatePair& aPair,
        TNATFWIceRole aCurrentRole ) const;
    
    /**
     * Calculates priorities for connectivity checks.
     *
     * @since   S60 v3.2
     * @param   aPairs          Pairs to prioritize
     * @param   aCurrentRole    The role of local agent
     */
    void PrioritizePairs( 
        RPointerArray<CNATFWCandidatePair>& aPairs,
        TNATFWIceRole aCurrentRole ) const;
    
    /**
     * Calculates priority for a connectivity check.
     *
     * @since   S60 v3.2
     * @param   aCheck          The check to prioritize
     * @param   aCurrentRole    The role of local agent
     */
    void PrioritizeCheck( CIceConnectivityCheck& aCheck,
        TNATFWIceRole aCurrentRole ) const;
    
    /**
     * Calculates priorities for connectivity checks.
     *
     * @since   S60 v3.2
     * @param   aChecks         Checks to prioritize
     * @param   aCurrentRole    The role of local agent
     */
    void PrioritizeChecks( 
        RPointerArray<CIceConnectivityCheck>& aChecks,
        TNATFWIceRole aCurrentRole ) const;

private:

    CIceCheckPrioritizer();

    };

#endif // C_NATFWCHECKPRIORITIZER_H
