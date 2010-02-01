/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Base class for event states.
*
*/





// INCLUDE FILES
#include "mceeventstatebase.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceEventStateBase::CMceEventStateBase
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMceEventStateBase::CMceEventStateBase()
    {
    }

 // Destructor
CMceEventStateBase::~CMceEventStateBase()
    {
    }


// -----------------------------------------------------------------------------
// CMceEventStateBase::ProcessStateL
// Applies the current concrete state.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMceEventStateBase::ProcessStateL( CMceComEvent& aEvent )
    {
    HandleL( aEvent );
    }

// -----------------------------------------------------------------------------
// CMceEventStateBase::ProcessReqRecvStateL
// Applies the current concrete state.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMceEventStateBase::ProcessReqRecvStateL(CMceComEvent& aEvent)
	{
	HandleReceiveRequestL( aEvent );
	}
	
// -----------------------------------------------------------------------------
// CMceEventStateBase::ApplyL
// Applies the current concrete state.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMceEventStateBase::ProcessRespRecvStateL(CMceComEvent& aEvent)
	{
	HandleResponseReceivedL( aEvent );
	}

//  End of File  
