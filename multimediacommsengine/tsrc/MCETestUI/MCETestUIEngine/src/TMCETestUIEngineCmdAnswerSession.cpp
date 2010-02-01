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

#include "TMCETestUIEngineCmdAnswerSession.h"
#include "CMCETestUIEngineInSession.h"
#include "CMCETestUIQuestionSingleSelection.h"
#include <MCEInSession.h>


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdAnswerSession::TMCETestUIEngineCmdAnswerSession
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdAnswerSession::TMCETestUIEngineCmdAnswerSession( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineInSession& aInSession )
    : TMCETestUIEngineCmdBase( aEngine ),
      iInSession( aInSession )		            
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdAnswerSession::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdAnswerSession::ExecuteL()
	{
	
	CMCETestUIQuestionSingleSelection* question = 
	    CMCETestUIQuestionSingleSelection::NewLC();
	    
	question->SetCaptionL( KUserQuestionNewIncomingSession );
	
	question->AddChoiceL( KUserQuestionOptionAccept );
	question->AddChoiceL( KUserQuestionOptionAcceptWithCustomReason );
	question->AddChoiceL( KUserQuestionOptionReject );
	question->AddChoiceL( KUserQuestionOptionRejectWithCustomReason );
	question->AddChoiceL( KUserQuestionOptionHandleLater );
			
	TInt index = iEngine.AskQuestionL( *question );
	
	CleanupStack::PopAndDestroy( question );
	
	if ( index >= 0 &&  index <= 3 )
	    {
	    iInSession.InSession().UpdateL();
	    }
	
	if ( index == 0 ) // Accept now
	    {
        iInSession.InSession().AcceptL();
	    }
	else if ( index == 1 ) // Accept with with custom reason
	    {
	    User::Leave( KErrNotSupported ); // yet...
	    }
	else if ( index == 2 ) // Reject now
	    {
        iInSession.InSession().RejectL();	    
	    }
	else if ( index == 3 ) // Reject with with custom reason
	    {
	    User::Leave( KErrNotSupported ); // yet...
	    }

	if ( index >= 0 &&  index <= 3 )
	    {
	    iEngine.EngineStateChangedL();	 
	    }

	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdAnswerSession::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdAnswerSession::Caption() const
	{
	return KCommandCaptionAnswerSession;
	}




// End of File
