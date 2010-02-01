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

#include "TMCETestUIEngineCmdRejectEvent.h"
#include "CMCETestUIEngineInEvent.h"
#include "CMCETestUIQuestionSingleSelection.h"
#include <MCEInEvent.h>
#include <mceinrefer.h>

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdRejectEvent::TMCETestUIEngineCmdRejectEvent
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdRejectEvent::TMCETestUIEngineCmdRejectEvent( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineInEvent& aInEvent )
    : TMCETestUIEngineCmdBase( aEngine ),
      iInEvent( aInEvent )		            
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdRejectEvent::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdRejectEvent::ExecuteL()
	{
	iInEvent.InEvent().RejectL();
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdRejectEvent::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdRejectEvent::Caption() const
	{
	return KCommandCaptionRejectEvent;
	}


//-----------------------------------------------------------------------------
// TMCETestUIEngineCmdRejectRefer::TMCETestUIEngineCmdRejectRefer
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdRejectRefer::TMCETestUIEngineCmdRejectRefer( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineInRefer& aInRefer )
    : TMCETestUIEngineCmdBase( aEngine ),
      iInRefer( aInRefer )		            
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdRejectRefer::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdRejectRefer::ExecuteL()
	{
	iInRefer.InRefer().RejectL();
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdRejectRefer::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdRejectRefer::Caption() const
	{
	return KCommandCaptionRejectRefer;
	}

// End of File
