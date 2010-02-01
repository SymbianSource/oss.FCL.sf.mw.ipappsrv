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



#include "TMCETestUIEngineCmdDeleteEvent.h"
#include "CMCETestUIEngineEvent.h"
#include "CMCETestUIEngineRefer.h"


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDeleteEvent::TMCETestUIEngineCmdDeleteEvent
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdDeleteEvent::TMCETestUIEngineCmdDeleteEvent( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineEvent& aEvent )
    : TMCETestUIEngineCmdBase( aEngine ),
      iEvent( aEvent )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDeleteEvent::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdDeleteEvent::ExecuteL()
	{
	iEngine.DeleteEvent( iEvent );
	}
	
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDeleteEvent::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdDeleteEvent::Caption() const
	{
	return KCommandCaptionDeleteEvent;
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDeleteRefer::TMCETestUIEngineCmdDeleteRefer
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdDeleteRefer::TMCETestUIEngineCmdDeleteRefer( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineRefer& aRefer )
    : TMCETestUIEngineCmdBase( aEngine ),
      iRefer( aRefer )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDeleteRefer::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdDeleteRefer::ExecuteL()
	{
	iEngine.DeleteRefer( iRefer );
	}
	
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDeleteRefer::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdDeleteRefer::Caption() const
	{
	return KCommandCaptionDeleteRefer;
	}


// End of File
