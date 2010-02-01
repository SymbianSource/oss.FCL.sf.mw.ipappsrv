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
#include "TMCETestUIEngineCmdAcceptEvent.h"
#include "CMCETestUIEngineInEvent.h"
#include "CMCETestUIQuestionDataQuery.h"
#include "CMCETestUIQuestionSingleSelection.h"
#include <MCEInEvent.h>
#include <mceinrefer.h>


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdAcceptEvent::TMCETestUIEngineCmdAcceptEvent
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdAcceptEvent::TMCETestUIEngineCmdAcceptEvent( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineInEvent& aInEvent )
    : TMCETestUIEngineCmdBase( aEngine ),
      iInEvent( aInEvent)		            
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdAcceptEvent::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdAcceptEvent::ExecuteL()
	{
	iInEvent.InEvent().AcceptL();	    
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdAcceptEvent::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdAcceptEvent::Caption() const
	{
	return KCommandCaptionAcceptEvent;
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdAcceptRefer::TMCETestUIEngineCmdAcceptRefer
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdAcceptRefer::TMCETestUIEngineCmdAcceptRefer( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineInRefer& aInRefer )
    : TMCETestUIEngineCmdBase( aEngine ),
      iInRefer( aInRefer)		            
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdAcceptRefer::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdAcceptRefer::ExecuteL()
	{
	iEngine.CreateInEventL(iInRefer.InRefer().AcceptL());	    
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdAcceptRefer::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdAcceptRefer::Caption() const
	{
	return KCommandCaptionAcceptRefer;
	}
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdRespondRefer::TMCETestUIEngineCmdRespondRefer
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdRespondRefer::TMCETestUIEngineCmdRespondRefer( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineInRefer& aInRefer )
    : TMCETestUIEngineCmdBase( aEngine ),
      iInRefer( aInRefer)		            
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdRespondRefer::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdRespondRefer::ExecuteL()
	{
	TSessionResponseValues responseValues;
	if(iEngine.SessionResponseValues( responseValues))
    	{
		CMCETestUIQuestionDataQuery* question = 
    			CMCETestUIQuestionDataQuery::NewLC();
		question->SetCaptionL( KUserQuestionInputRecipient );
		question->SetDefaultValueL(responseValues.reason  );
		question->SetAnswerL( responseValues.reason );
		TBufC8<100> reason( question->Answer8() );
		CleanupStack::PopAndDestroy( question );
		iEngine.CreateInEventL(iInRefer.InRefer().RespondL(reason,responseValues.code));
	   	}
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdRespondRefer::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdRespondRefer::Caption() const
	{
	return KCommandCaptionRespondRefer;
	}

// End of File
