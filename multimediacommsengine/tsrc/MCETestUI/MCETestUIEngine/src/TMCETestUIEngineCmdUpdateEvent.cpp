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

#include "TMCETestUIEngineCmdUpdateEvent.h"
#include "cmcetestuiengineoutevent.h"
#include <MCEOutEvent.h>



// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdUpdateEvent::TMCETestUIEngineCmdUpdateEvent
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdUpdateEvent::TMCETestUIEngineCmdUpdateEvent(
            CMCETestUIEngine& aEngine,
		    CMCETestUIEngineOutEvent& aOutEvent )
    : TMCETestUIEngineCmdBase( aEngine ),
    iOutEvent( aOutEvent )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdUpdateEvent::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdUpdateEvent::ExecuteL()
	{
	iOutEvent.OutEvent().UpdateL(300);
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdUpdateEvent::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdUpdateEvent::Caption() const
	{
	return KCommandCaptionUpdateEvent;
	}




// End of File
