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

#include "TMCETestUIEngineCmdTerminateSession.h"
#include "CMCETestUIEngineSession.h"
#include <MCESession.h>

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdTerminateSession::TMCETestUIEngineCmdTerminateSession
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdTerminateSession::TMCETestUIEngineCmdTerminateSession( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSession& aSession )
    : TMCETestUIEngineCmdBase( aEngine ),
      iSession( aSession )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdTerminateSession::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdTerminateSession::ExecuteL()
	{
	iSession.Session().TerminateL();
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdTerminateSession::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdTerminateSession::Caption() const
	{
	return KCommandCaptionTerminateSession;
	}




// End of File
