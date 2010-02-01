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

#include "TMCETestUIEngineCmdSRReqRes.h"
#include "CMCETestUIQuestionSingleSelection.h"
#include "CMCETestUIQuestionDataQuery.h"
#include "CMCETestUIEngineSession.h"
#include <MCESession.h>




// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdSendReqRes::TMCETestUIEngineCmdSendInfo 
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdSendInfo::TMCETestUIEngineCmdSendInfo( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSession& aSession )
    : TMCETestUIEngineCmdBase( aEngine ),
      iSession( aSession )
    {
    }
    
    
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdSendReqRes::TMCETestUIEngineCmdSendDTMFInfo 
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdDTMFInfo::TMCETestUIEngineCmdDTMFInfo( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSession& aSession )
    : TMCETestUIEngineCmdBase( aEngine ),
      iSession( aSession )
    {
    }
    
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdSendReqRes::TMCETestUIEngineCmdSendInfoResponse 
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdSendInfoResponse::TMCETestUIEngineCmdSendInfoResponse( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSession& aSession )
    : TMCETestUIEngineCmdBase( aEngine ),
      iSession( aSession )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdSendInfo::Caption 
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdSendInfo::Caption() const
	{
	return KUserQuestionSendInfo;
	}	


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdSendInfo::ExecuteL 
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdSendInfo::ExecuteL()
	{
	_LIT8( KInfo,"INFO"); 

	TMceTransactionId transactionID = KErrNone;
	
	CDesC8ArrayFlat* sipHeaders = new (ELeave) CDesC8ArrayFlat( 1 );
	CleanupStack::PushL( sipHeaders );
	sipHeaders->AppendL( _L8( "User-Agent: MCETestUI/SendInfo" ) );
	
	
	TRAPD( err, transactionID = 
			transactionID = iSession.Session().SendRequestL( KInfo, sipHeaders ));
	
	if ( err != KErrNone )
		{
		iEngine.NotifyUser( KRequestFailed );
		}
	
	CleanupStack::Pop( sipHeaders );

	}
	
	
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdSendInfoDTMF::Caption 
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdDTMFInfo::Caption() const
	{
	return KUserQuestionSendInfoDTMF;
	}	


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdSendInfoDTMF::ExecuteL 
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdDTMFInfo::ExecuteL()
	{
	_LIT8( KContentX, "Signal= 1\r\n\
	Duration= 160\r\n"); 				
	_LIT8( KContentTypeX, "application/dtmf-relay");
	_LIT8( KInfo,"INFO"); 
		
	TMceTransactionId transactionID = 0;
	
	HBufC8* ContentX = KContentX().AllocLC();
	HBufC8* ContentTypeX = KContentTypeX().AllocLC();
		
	CDesC8ArrayFlat* sipHeaders = new (ELeave) CDesC8ArrayFlat( 1 );
	CleanupStack::PushL( sipHeaders );
	sipHeaders->AppendL( _L8( "User-Agent: MCETestUI/SendDTMFInfo" ) );
	
	TRAPD( err, transactionID = 
			iSession.Session().SendRequestL( KInfo, sipHeaders, ContentTypeX, ContentX ));
	
	if ( err != KErrNone )
		{
		iEngine.NotifyUser( KRequestFailed );
		}

	CleanupStack::Pop( sipHeaders );
	CleanupStack::Pop( ContentTypeX );
	CleanupStack::Pop( ContentX );
	
	}





EXPORT_C const TDesC& TMCETestUIEngineCmdSendInfoResponse::Caption() const
	{
	return KUserQuestionSendInfoResponse;
	}	


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdSendInfoDTMF::ExecuteL 
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdSendInfoResponse::ExecuteL()
	{
	const TUint code = 200;
	_LIT8( KMceSipOK, "OK" ); 

	const RArray< TMceTransactionId > transactionIds = iEngine.TransactionIds();
	if ( transactionIds.Count() == 0 )
		{
		iEngine.NotifyUser( KUserQuestionSendInfoNoTransactions );
		return;
		}
	
	CMCETestUIQuestionSingleSelection* transactionQuestion = 
		CMCETestUIQuestionSingleSelection::NewLC();
	
	transactionQuestion->SetCaptionL( KUserQuestionSendInfoResponseCaption );
	
	_LIT( KFormat, "%u" );
	TBuf<32> transaction;
	
	for( TInt i = transactionIds.Count()-1 ; i >= 0 ; --i )
		{
		transaction.Format( KFormat, static_cast< TInt >( transactionIds[i] ) );
		transactionQuestion->AddChoiceL( transaction );
		}
	
	TInt index = iEngine.AskQuestionL( *transactionQuestion );
	
	CleanupStack::PopAndDestroy( transactionQuestion );

	if ( index >= 0 && index < transactionIds.Count() )
		{
		CDesC8ArrayFlat* sipHeaders = new (ELeave) CDesC8ArrayFlat( 1 );
		CleanupStack::PushL( sipHeaders );
		sipHeaders->AppendL( _L8( "User-Agent: MCETestUI/SendInfoResponse" ) );
		
		iSession.Session().SendResponseL( transactionIds[index],
									  KMceSipOK,
									  code,
									  sipHeaders );
		
		CleanupStack::Pop( sipHeaders );
		
		iEngine.RemoveTransactionId( transactionIds[index] );
		}

	}	

// End of File
