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

#include "TMCETestUIEngineCmdCreateMceSettings.h"
#include "CMCETestUIEngineOutSession.h"
#include "CMCETestUIQuestionSingleSelection.h"
#include "CMCETestUIEngineProfile.h"


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateMceSettings::Caption
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdCreateMceSettings::TMCETestUIEngineCmdCreateMceSettings(
		            CMCETestUIEngine& aEngine )
    : TMCETestUIEngineCmdBase( aEngine ),
      iProfile( NULL )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateMceSettings::TMCETestUIEngineCmdCreateMceSettings
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdCreateMceSettings::TMCETestUIEngineCmdCreateMceSettings(
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineProfile& aProfile )
    : TMCETestUIEngineCmdBase( aEngine ),
      iProfile( &aProfile )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateMceSettings::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdCreateMceSettings::ExecuteL()
	{

    iEngine.CreateMceSettingsL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateMceSettings::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdCreateMceSettings::Caption() const
	{
	return KCommandCaptionCreateMceSettings;
	}


// End of File
