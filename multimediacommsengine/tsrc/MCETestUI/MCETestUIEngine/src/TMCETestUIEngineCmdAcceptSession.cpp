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
#include "TMCETestUIEngineCmdAcceptSession.h"
#include "CMCETestUIEngineInSession.h"
#include "CMCETestUIQuestionDataQuery.h"
#include "CMCETestUIQuestionSingleSelection.h"
#include <MCEInSession.h>


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdAcceptSession::TMCETestUIEngineCmdAcceptSession
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdAcceptSession::TMCETestUIEngineCmdAcceptSession( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineInSession& aInSession )
    : TMCETestUIEngineCmdBase( aEngine ),
      iInSession( aInSession )		            
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdAcceptSession::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdAcceptSession::ExecuteL()
	{

	iInSession.InSession().AcceptL();	    
	iEngine.EngineStateChangedL();
/*	CMCETestUIQuestionSingleSelection* question = 
	    CMCETestUIQuestionSingleSelection::NewLC();
	    
	question->SetCaptionL( KMiscIncomingSession );
	
	question->AddChoiceL( KUserQuestionOptionAccept );
	question->AddChoiceL( KUserQuestionOptionAcceptWithCustomReason );
	
	TInt index = iEngine.AskQuestionL( *question );
	
	CleanupStack::PopAndDestroy( question );
	
	if ( index == 0 ) // Accept now
	    {
        iInSession.InSession().AcceptL();	    
	    }
	else if ( index == 1 )
	    {
	    User::Leave( KErrNotSupported ); // yet...
	    }
*/	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdAcceptSession::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdAcceptSession::Caption() const
	{
	return KCommandCaptionAcceptSession;
	}


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdRespondSession::TMCETestUIEngineCmdRespondSession
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdRespondSession::TMCETestUIEngineCmdRespondSession( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineInSession& aInSession )
    : TMCETestUIEngineCmdBase( aEngine ),
      iInSession( aInSession )		            
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdRespondSession::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdRespondSession::ExecuteL()
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
		iInSession.InSession().RespondL(reason,responseValues.code);
	   	}
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdRespondSession::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdRespondSession::Caption() const
	{
	return KCommandCaptionRespondSession;
	}



// End of File
