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
* Description:    Static functions to handle default values for 
*                MCETestUIEngine objects
*
*/





// INCLUDES
#include "MCETestUIEngineDefaultValues.h"


// CONSTANTS

// Profile default values
const TBool KTakeDefaultProfileToUseAtStartup = EFalse;
const TBool KRegisterUsedProfiles = ETrue;

// Session default values
_LIT8( KSessionDefaultReceiver, "sip:receiver@10.21.32.101" );



// -----------------------------------------------------------------------------
// MCETestUIEngineDefaultValues::TakeDefaultProfileIntoUse
//
// -----------------------------------------------------------------------------
//
TBool MCETestUIEngineDefaultValues::TakeDefaultProfileIntoUse()
    {
    return KTakeDefaultProfileToUseAtStartup;
    };

// -----------------------------------------------------------------------------
// MCETestUIEngineDefaultValues::RegisterUsedProfiles
//
// -----------------------------------------------------------------------------
//
TBool MCETestUIEngineDefaultValues::RegisterUsedProfiles()
    {
    return KRegisterUsedProfiles;
    };

// -----------------------------------------------------------------------------
// MCETestUIEngineDefaultValues::DefaultSessionReceiver
//
// -----------------------------------------------------------------------------
//
const TDesC8& MCETestUIEngineDefaultValues::DefaultSessionReceiver()
    {
    return KSessionDefaultReceiver;
    };
    
