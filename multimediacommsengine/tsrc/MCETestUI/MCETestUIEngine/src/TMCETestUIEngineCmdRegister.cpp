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
* Description:    Implementation.
*
*/




// INCLUDES

#include "TMCETestUIEngineCmdRegister.h"
#include "CMCETestUIEngineProfileContainer.h"


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdRegister::TMCETestUIEngineCmdRegister
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdRegister::TMCETestUIEngineCmdRegister(
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineProfile& aProfile )
    : TMCETestUIEngineCmdBase( aEngine ),
    iProfile( aProfile )
    {
    }
    
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdRegister::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdRegister::ExecuteL()
	{
	iEngine.ProfileContainer().ProfileRegistry().EnableL( 
	    iProfile.Profile(), 
	    iEngine.ProfileContainer() );
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdRegister::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdRegister::Caption() const
	{
	return KCommandCaptionRegister;
	}




// End of File
