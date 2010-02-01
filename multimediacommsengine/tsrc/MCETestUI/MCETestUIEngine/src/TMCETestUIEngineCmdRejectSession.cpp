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

#include "TMCETestUIEngineCmdRejectSession.h"
#include "CMCETestUIEngineInSession.h"
#include "CMCETestUIQuestionSingleSelection.h"
#include <MCEInSession.h>

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdRejectSession::TMCETestUIEngineCmdRejectSession
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdRejectSession::TMCETestUIEngineCmdRejectSession( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineInSession& aInSession )
    : TMCETestUIEngineCmdBase( aEngine ),
      iInSession( aInSession )		            
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdRejectSession::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdRejectSession::ExecuteL()
	{

	iInSession.InSession().RejectL();
	iEngine.EngineStateChangedL();

/*	CMCETestUIQuestionSingleSelection* question = 
	    CMCETestUIQuestionSingleSelection::NewLC();
	    
	question->SetCaptionL( KMiscIncomingSession );
	
	question->AddChoiceL( KUserQuestionOptionReject );
	question->AddChoiceL( KUserQuestionOptionRejectWithCustomReason );
	
	TInt index = iEngine.AskQuestionL( *question );
	
	CleanupStack::PopAndDestroy( question );
	
	if ( index == 0 ) // Reject now
	    {
        iInSession.InSession().RejectL();	    
	    }
	else if ( index == 1 )
	    {
	    User::Leave( KErrNotSupported ); // yet...
	    }
*/	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdRejectSession::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdRejectSession::Caption() const
	{
	return KCommandCaptionRejectSession;
	}




// End of File
