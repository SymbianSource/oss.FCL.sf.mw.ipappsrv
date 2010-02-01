/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef M_ICECHECKLISTOBSERVER_H
#define M_ICECHECKLISTOBSERVER_H


#include <e32base.h>
#include "natfwconnectivityapidefs.h"

class CIceCheckList;
class CNATFWCandidatePair;

/**
 *  Obsever interface for the client of ICE check list.
 *
 *  @lib icecandidatehandler.dll
 *  @since S60 v3.2
 */
class MIceChecklistObserver
    {

public:

    /**
     * Indicates that check list has completed successfully.
     * Ownership of selected pairs is transferred.
     *
     * @since   S60 v3.2
     * @param   aChecklist      The checklist which was completed
     * @param   aSelectedPairs  The selected pairs for components
     */
    virtual void ChecklistCompletedL( 
        const CIceCheckList& aChecklist, 
        const RPointerArray<CNATFWCandidatePair>& aSelectedPairs ) = 0;

    /**
     * Indicates that check list has completed unsuccessfully.
     *
     * @since   S60 v3.2
     * @param   aChecklist      The checklist which was completed
     * @param   aFailedComps    The failed components
     * @param   aErrCode        The completion code
     */
    virtual void ChecklistCompletedL( 
        const CIceCheckList& aChecklist,
        const RArray<TUint>& aFailedComps,
        TInt aErrCode ) = 0;

    /**
     * Indicates that those checks in other checklists, which have same
     * foundation could be now unfrozen as an optimization.
     *
     * @since   S60 v3.2
     * @param   aChecklist      The checklist reporting event
     * @param   aFoundation     The foundation
     */
    virtual void ComponentsHaveValidPairsL( 
        const CIceCheckList& aChecklist, const TDesC8& aFoundation ) = 0;
    
    /**
     * Called when check list has detected role conflict between peers and
     * local peer needs to change it's role.
     * 
     * @since   S60 v3.2
     * @param   aDesiredRole    Desired role
     */
    virtual void RoleChangeNeeded( TNATFWIceRole aDesiredRole ) = 0;
    };


#endif // M_ICECHECKLISTOBSERVER_H
