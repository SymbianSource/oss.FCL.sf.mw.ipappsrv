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





#include "mceactivescheduler.h"
#include "mcesrvlogs.h"

// -----------------------------------------------------------------------------
// CMceActiveScheduler::CMceActiveScheduler
// -----------------------------------------------------------------------------
//
CMceActiveScheduler::CMceActiveScheduler ()
	{
	}

// -----------------------------------------------------------------------------
// CMceActiveScheduler::~CMceActiveScheduler
// -----------------------------------------------------------------------------
//
CMceActiveScheduler::~CMceActiveScheduler ()
	{
	}

// -----------------------------------------------------------------------------
// CMceActiveScheduler::Error
// -----------------------------------------------------------------------------
//
void CMceActiveScheduler::Error( TInt aError ) const
	{
    MCESRV_DEBUG("CMceActiveScheduler::Error, Entry");
	
    // This function will be entered only if a RunL-function of 
    // a non-C/S-session related active object leaves.
    // The SIP C/S-server has to be kept running under any conditions.
    MCESRV_DEBUG_DVALUE("ERROR", aError );
    aError = aError;
    
    MCESRV_DEBUG("CMceActiveScheduler::Error, Exit");
    
	}
