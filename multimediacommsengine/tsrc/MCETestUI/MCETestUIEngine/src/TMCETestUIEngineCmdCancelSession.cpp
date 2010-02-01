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

#include "TMCETestUIEngineCmdCancelSession.h"
#include "cmcetestuiengineoutsession.h"
#include <MCEOutSession.h>



// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCancelSession::TMCETestUIEngineCmdCancelSession
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdCancelSession::TMCETestUIEngineCmdCancelSession(
            CMCETestUIEngine& aEngine,
		    CMCETestUIEngineOutSession& aOutSession )
    : TMCETestUIEngineCmdBase( aEngine ),
    iOutSession( aOutSession )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdCancelSession::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdCancelSession::ExecuteL()
	{

	iOutSession.OutSession().CancelL();
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEstablish::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdCancelSession::Caption() const
	{
	return KCommandCaptionCancelSession;
	}




// End of File
