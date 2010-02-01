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

#include "TMCETestUIEngineCmdTerminateEvent.h"
#include "CMCETestUIEngineEvent.h"
#include "CMCETestUIQuestionSingleSelection.h"
#include "CMCETestUIQuestionDataQuery.h"
#include "MCETestUIDebug.h"
#include <MCEEvent.h>


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdTerminateEvent::TMCETestUIEngineCmdTerminateEvent
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdTerminateEvent::TMCETestUIEngineCmdTerminateEvent( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineEvent& aEvent )
    : TMCETestUIEngineCmdBase( aEngine ),
      iEvent( aEvent )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdTerminateEvent::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdTerminateEvent::ExecuteL()
	{
	TNotifyValues notifyValues;
    if( iEngine.NotifyValues( notifyValues) )
    	{
    	CDesC8Array* headers;
    	headers = new (ELeave) CDesC8ArrayFlat( 1 );
    	CleanupStack::PushL( headers );

		CMCETestUIQuestionDataQuery* question = 
    			CMCETestUIQuestionDataQuery::NewLC();
		question->SetCaptionL( KUserQuestionInputEventHeader );
		question->SetDefaultValueL(notifyValues.headers  );
		question->SetAnswerL( notifyValues.headers );
		TBufC8<100> headersbuffer( question->Answer8() );
		CleanupStack::PopAndDestroy( question );
		headers->AppendL( headersbuffer );

/*
        CMCETestUIQuestionDataQuery* question2 = 
		    CMCETestUIQuestionDataQuery::NewLC();
		    
        question2->SetCaptionL( KUserQuestionInputEventContentType );
    	question2->SetDefaultValueL(notifyValues.contentType  );
    	question2->SetAnswerL( notifyValues.contentType  );
    	
    	HBufC8 *contentType = HBufC8::NewLC( question2->Answer8().Length() );
    	contentType->Des().Copy( question2->Answer8() );
    	    	
    	CleanupStack::PopAndDestroy( question2 );

        CMCETestUIQuestionDataQuery* question3 = 
		    CMCETestUIQuestionDataQuery::NewLC();
		    
        question3->SetCaptionL( KUserQuestionInputEventContent );
    	question3->SetDefaultValueL(notifyValues.contentType  );
    	question3->SetAnswerL( notifyValues.contentType  );
    	HBufC8 *content = HBufC8::NewLC( question3->Answer8().Length() );
    	content->Des().Copy( question3->Answer8() );
    	
    	
    	CleanupStack::PopAndDestroy( question3 );
    	
    	CleanupStack::Pop( content );
    	CleanupStack::Pop( contentType );
   */
    	
    	
    	
    	HBufC8 *contentType = HBufC8::NewLC( KNotifyContentType().Length() );
    	contentType->Des().Copy( KNotifyContentType );
    	
    	HBufC8 *content = HBufC8::NewLC( KNotifyContent().Length() );
    	content->Des().Copy( KNotifyContent );
    	
    	CleanupStack::Pop( content );
    	CleanupStack::Pop( contentType );
    	CleanupStack::Pop( headers );
    	
    	if( headers == 0 )
    	    {
    	    MCE_TESTUI_DEBUG1( "Terminate Event with values");
    	    iEvent.Event().TerminateL( headers, contentType, content );
    	    }
    	else
    	    {
    	    MCE_TESTUI_DEBUG1( "Terminate Event without values");
    	    iEvent.Event().TerminateL( );
    	    }
    	
    	iEngine.EngineStateChangedL();
    	}
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdTerminateEvent::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdTerminateEvent::Caption() const
	{
	return KCommandCaptionTerminateEvent;
	}


// End of File
