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
* Description:  Base class description for all session states.
*
*/

#include "nspstatebase.h"
#include "nspsessiondata.h"
#include "nspdefs.h"

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// CNSPStateBase::CNSPStateBase
// ---------------------------------------------------------------------------
//
CNSPStateBase::CNSPStateBase()
    {
    }


// ---------------------------------------------------------------------------
// CNSPStateBase::~CNSPStateBase
// ---------------------------------------------------------------------------
//
CNSPStateBase::~CNSPStateBase()
    {
    }


// ---------------------------------------------------------------------------
// CNSPStateBase::AcceptL
// ---------------------------------------------------------------------------
//
TBool CNSPStateBase::AcceptL( TNSPStateMachineEvent& aEvent )
    {
    return DoAcceptL( aEvent );
    }


// ---------------------------------------------------------------------------
// CNSPStateBase::StateEntryL
// ---------------------------------------------------------------------------
//
void CNSPStateBase::StateEntryL( TNSPStateMachineEvent& aEvent )
    {
    DoStateEntryL( aEvent );
    }


// ---------------------------------------------------------------------------
// CNSPStateBase::Reverse
// ---------------------------------------------------------------------------
//
void CNSPStateBase::Reverse( TNSPStateMachineEvent& aEvent )
    {
    DoReverse( aEvent );
    }


// ---------------------------------------------------------------------------
// CNSPStateBase::DoApplyL
// ---------------------------------------------------------------------------
//
void CNSPStateBase::ApplyL( TNSPStateMachineEvent& aEvent )
    {
    DoApplyL( aEvent );
    }


// ---------------------------------------------------------------------------
// CNSPStateBase::ExitL
// ---------------------------------------------------------------------------
//
void CNSPStateBase::ExitL( TNSPStateMachineEvent& aEvent )
    {
    DoExitL( aEvent );
    }


// end of file
