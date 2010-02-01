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

#include "TMCETestUIEngineCmdCreateMceValueSettings.h"
#include "CMCETestUIEngineOutSession.h"
#include "CMCETestUIQuestionSingleSelection.h"
#include "CMCETestUIEngineProfile.h"


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateMceValueSettings::Caption
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdCreateMceValueSettings::TMCETestUIEngineCmdCreateMceValueSettings(
		            CMCETestUIEngine& aEngine )
    : TMCETestUIEngineCmdBase( aEngine ),
      iProfile( NULL )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateMceValueSettings::TMCETestUIEngineCmdCreateMceValueSettings
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdCreateMceValueSettings::TMCETestUIEngineCmdCreateMceValueSettings(
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineProfile& aProfile )
    : TMCETestUIEngineCmdBase( aEngine ),
      iProfile( &aProfile )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateMceValueSettings::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdCreateMceValueSettings::ExecuteL()
	{

    iEngine.CreateMceValueSettingsL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCreateMceValueSettings::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdCreateMceValueSettings::Caption() const
	{
	return KCommandCaptionCreateMceValueSettings;
	}


// End of File
