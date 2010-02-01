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

#include "TMCETestUIEngineCmdFloorControlSession.h"
#include "CMCETestUIQuestionSingleSelection.h"
#include "CMCETestUIQuestionDataQuery.h"
#include "CMCETestUIEngineSession.h"
#include <MCESession.h>
#include <MCEFCMsgExchange.h>


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdFCSendSession::TMCETestUIEngineCmdFCSendSession
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdFCSendSession::TMCETestUIEngineCmdFCSendSession( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSession& aSession )
    : TMCETestUIEngineCmdBase( aEngine ),
      iSession( aSession )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdFCSendSession::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdFCSendSession::ExecuteL()
	{
	HBufC8* buf = HBufC8::NewLC(100);
	TPtr8 itemPtr = buf->Des();
        itemPtr.Append( KFCMessage );
   	CleanupStack::Pop(buf); 
   	TInt err = KErrNone;    
   	TRAP(err, iEngine.FloorControl().SendL(iSession.Session(),buf));
	if( err != KErrNone )
		{
		delete buf;
		}
	iEngine.EngineStateChangedL();    
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdFCSendSession::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdFCSendSession::Caption() const
	{
	return KCommandCaptionSendFCSession;
	}




// End of File
