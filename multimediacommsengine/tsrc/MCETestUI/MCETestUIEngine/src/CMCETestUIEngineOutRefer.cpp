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
* Description:    
*
*/




// MCE API

#include <MCEManager.h>
#include <MCEOutRefer.h>
#include <MCERefer.h>
#include <Uri8.h>

#include "MCETestUIEngineDefaultValues.h"
#include "CMCETestUIQuestionDataQuery.h"
#include "CMCETestUIEngine.h"
#include "CMCETestUIEngineProfileContainer.h"
#include "CMCETestUIEngineProfile.h"
#include "CMCETestUIEngineOutRefer.h"
#include "TMCETestUIEngineCmdSubscribeEvent.h"
#include "TMCETestUIEngineCmdDeleteEvent.h"
#include "cmcetestuienginesession.h"
#include "cmcetestuiengineevent.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutRefer::NewL()
// -----------------------------------------------------------------------------
//
CMCETestUIEngineOutRefer* CMCETestUIEngineOutRefer::NewL(
            CMCETestUIEngine& aEngine,
            CMCETestUIEngineProfile& aProfile) 
    {

    CMCETestUIEngineOutRefer* self = 
        new (ELeave) CMCETestUIEngineOutRefer( aEngine, 
                                                 aProfile);
    CleanupStack::PushL( self );
    self->ConstructL(aProfile);
    CleanupStack::Pop( self );
    return self;  
    }
// -----------------------------------------------------------------------------
// CMCETestUIEngineOutRefer::NewL()
// -----------------------------------------------------------------------------
//
CMCETestUIEngineOutRefer* CMCETestUIEngineOutRefer::NewL(
            CMCETestUIEngine& aEngine,
            CMCETestUIEngineSession& aSession) 
    {

    CMCETestUIEngineOutRefer* self = 
        new (ELeave) CMCETestUIEngineOutRefer( aEngine, 
                                                 aSession);
    CleanupStack::PushL( self );
    self->ConstructL(aSession);
    CleanupStack::Pop( self );
    return self;  
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutRefer::NewL()
// -----------------------------------------------------------------------------
//
CMCETestUIEngineOutRefer* CMCETestUIEngineOutRefer::NewL(
            CMCETestUIEngine& aEngine,
            CMCETestUIEngineEvent& aEvent) 
    {

    CMCETestUIEngineOutRefer* self = 
        new (ELeave) CMCETestUIEngineOutRefer( aEngine, 
                                                 aEvent);
    CleanupStack::PushL( self );
    self->ConstructL(aEvent);
    CleanupStack::Pop( self );
    return self;  
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutRefer::CMCETestUIEngineOutRefer
// -----------------------------------------------------------------------------
//
CMCETestUIEngineOutRefer::CMCETestUIEngineOutRefer( 
                CMCETestUIEngine& aEngine,
                CMCETestUIEngineProfile& aProfile )
    : CMCETestUIEngineRefer( aEngine ),
      iProfile( &aProfile ),
      iId( 0 )
    {
    }
// -----------------------------------------------------------------------------
// CMCETestUIEngineOutRefer::CMCETestUIEngineOutRefer
// -----------------------------------------------------------------------------
//
CMCETestUIEngineOutRefer::CMCETestUIEngineOutRefer( 
                CMCETestUIEngine& aEngine,
                CMCETestUIEngineSession& aSession )
    : CMCETestUIEngineRefer( aEngine ),
      iSession( &aSession ),
      iId( 0 )
    {
    }
// -----------------------------------------------------------------------------
// CMCETestUIEngineOutRefer::CMCETestUIEngineOutRefer
// -----------------------------------------------------------------------------
//
CMCETestUIEngineOutRefer::CMCETestUIEngineOutRefer( 
                CMCETestUIEngine& aEngine,
                CMCETestUIEngineEvent& aEvent )
    : CMCETestUIEngineRefer( aEngine ),
      iEvent( &aEvent ),
      iId( 0 )
    {
    }


// -----------------------------------------------------------------------------
// CMCETestUIEngineOutRefer::ConstructL()
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineOutRefer::ConstructL(CMCETestUIEngineProfile& /*aProfile*/)
    {
 
     /////////////////////
    //
    // TODO: After implementing wrappers for streams, sinks and sources 
    //       this function should be refactored to make use of them. That will 
    //       also simplify the function to some extent.
    //
    //////////////////////
    
 
    BaseConstructL();
    if(iProfile )
    	{
    	FillProfileRegistrar();
    
    if(ReferParamsL(iReferParams))
    	{
    	// Solve recipient
	    CMCETestUIQuestionDataQuery* question = 
		    CMCETestUIQuestionDataQuery::NewLC();
		question->SetCaptionL( KUserQuestionInputRecipient );
		question->SetDefaultValueL(iReferParams.iRecipentAddress  );
		question->SetAnswerL( iReferParams.iRecipentAddress  );
		TBufC8<100> recipient( question->Answer8() );
		
		CleanupStack::PopAndDestroy( question );
		//solve referTo
		
		CMCETestUIQuestionDataQuery* questionreferto = 
		    CMCETestUIQuestionDataQuery::NewLC();
		questionreferto->SetCaptionL( KUserQuestionInputReferTo );
		questionreferto->SetDefaultValueL(iReferParams.iReferTo  );
		questionreferto->SetAnswerL( iReferParams.iReferTo  );
		TBufC8<100> referTo( questionreferto->Answer8() );
		
		CleanupStack::PopAndDestroy( questionreferto );
	   // solve originator
	   
	   if ( iReferParams.originator.Compare(KNone))
	   	{
		   	HBufC8* orig8 = HBufC8::NewL(iReferParams.originator.Length());
		   	TPtr8 ptr = orig8->Des();
		    ptr.Copy(iReferParams.originator);
		    // Create OutRfer
		    CMceOutRefer* outRefer = CMceOutRefer::NewL(
		       	iEngine.MCEManager(), 
			    iProfile->Profile(),
			    recipient,referTo);
			CleanupStack::PushL(outRefer);
		    CleanupStack::Pop( outRefer );
			iRefer = outRefer;
			iOutRefer = outRefer;        
	   	}
	   else 
	   	{
		   	// Create OutEvent
		    CMceOutRefer* outRefer = CMceOutRefer::NewL(
		       iEngine.MCEManager(),
		       iProfile->Profile(),
			   recipient,referTo );
			CleanupStack::PushL(outRefer);
		    CleanupStack::Pop( outRefer );
			iRefer = outRefer;
			iOutRefer = outRefer;
	   	}
	    // Get identity
	    iId = iEngine.NextOutReferId();
    	}
    	}
    
    
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutRefer::ConstructL(CMCETestUIEngineSession& aSession)
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineOutRefer::ConstructL(CMCETestUIEngineSession& aSession)
    {
 
     /////////////////////
    //
    // TODO: After implementing wrappers for streams, sinks and sources 
    //       this function should be refactored to make use of them. That will 
    //       also simplify the function to some extent.
    //
    //////////////////////
    
 
    BaseConstructL();
    if(iSession )
    	{
        
    if(ReferToParamsL(iReferParams))
    	{
    
		//solve referTo
		
		CMCETestUIQuestionDataQuery* questionreferto = 
		    CMCETestUIQuestionDataQuery::NewLC();
		questionreferto->SetCaptionL( KUserQuestionInputReferTo );
		questionreferto->SetDefaultValueL(iReferParams.iReferTo  );
		questionreferto->SetAnswerL( iReferParams.iReferTo  );
		TBufC8<100> referTo( questionreferto->Answer8() );
		
		CleanupStack::PopAndDestroy( questionreferto );
	   
	   
	    	// Create OutEvent
		   CMceOutRefer* outRefer = CMceOutRefer::NewL(
		       aSession.Session(),referTo );
			iRefer = outRefer;
			iOutRefer = outRefer;
	       // Get identity
	    iId = iEngine.NextOutReferId();
    	}
    	}
    
    
    }
// -----------------------------------------------------------------------------
// CMCETestUIEngineOutRefer::ConstructL()
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineOutRefer::ConstructL(CMCETestUIEngineEvent& aEvent)
    {
 
     /////////////////////
    //
    // TODO: After implementing wrappers for streams, sinks and sources 
    //       this function should be refactored to make use of them. That will 
    //       also simplify the function to some extent.
    //
    //////////////////////
    
 
    BaseConstructL();
    if(iEvent )
    	{
        
    if(ReferParamsL(iReferParams))
    	{
    	//solve referTo
		
		CMCETestUIQuestionDataQuery* questionreferto = 
		    CMCETestUIQuestionDataQuery::NewLC();
		questionreferto->SetCaptionL( KUserQuestionInputReferTo );
		questionreferto->SetDefaultValueL(iReferParams.iReferTo  );
		questionreferto->SetAnswerL( iReferParams.iReferTo  );
		TBufC8<100> referTo( questionreferto->Answer8() );
		
		CleanupStack::PopAndDestroy( questionreferto );
	  // Create Outrefer
		    CMceOutRefer* outRefer = CMceOutRefer::NewL(
		       aEvent.Event(),referTo );
			CleanupStack::PushL(outRefer);
		    CleanupStack::Pop( outRefer );
			iRefer = outRefer;
			iOutRefer = outRefer;
	    // Get identity
	    iId = iEngine.NextOutReferId();
    	}
    	}
    
    
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutRefer::~CMCETestUIEngineOutRefer
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngineOutRefer::~CMCETestUIEngineOutRefer()
    {
    delete iOutRefer;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutRefer::GetCommands
// -----------------------------------------------------------------------------
//
const RPointerArray<TMCETestUIEngineCmdBase>& 
            CMCETestUIEngineOutRefer::GetCommandsL()
    {
    iCommands.ResetAndDestroy();

    // Get base class commands
    CMCETestUIEngineRefer::GetCommandsL();
        
    // Get own commands       
    TInt state = iOutRefer->State();

	switch( state ) 
		{
		case CMceRefer::EIdle:
			{
			iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdRefer( iEngine, *this ) );	  
		    break;
			}
            
		case CMceRefer::EAccepted: 	 
			{
			break;
			}	  
		    
		case CMceRefer::EPending:  
			{
			break;   
			}
		default: 
		    break;
		}  
		  
    return iCommands; 
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutRefer::Id
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMCETestUIEngineOutRefer::Id() const
    {
    return iId;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutRefer::OutSession
// -----------------------------------------------------------------------------
//
CMceOutRefer& CMCETestUIEngineOutRefer::OutRefer()
    {
    return *iOutRefer;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutRefer::ReferParams
// -----------------------------------------------------------------------------
//

TBool CMCETestUIEngineOutRefer::ReferParamsL(TReferParams &aReferParams)
{
  return iEngine.ReferParamsL( aReferParams);
 
}
// -----------------------------------------------------------------------------
// CMCETestUIEngineOutRefer::ReferToParams
// -----------------------------------------------------------------------------
//

TBool CMCETestUIEngineOutRefer::ReferToParamsL(TReferParams &aReferParams)
{
  return iEngine.ReferToParamsL( aReferParams);
 
}
// -----------------------------------------------------------------------------
// CMCETestUIEngineOutRefer::FillProfileRegistrar
// -----------------------------------------------------------------------------
//

void CMCETestUIEngineOutRefer::FillProfileRegistrar()
	{
	// trying to get profile registrar 
          
    TUriParser8 parser;
    const TDesC8& aor = iProfile->Aor();
    parser.Parse( aor ) ; 
    CUri8* uri8 = CUri8::NewLC( parser );
    const TDesC8& registrar =  uri8->Uri().Extract( EUriHost ) ;
    
    HBufC*registrar16 = HBufC16::NewLC( KRecipent().Length() + registrar.Length() );
    TPtr registrarPtr = registrar16->Des();
    registrarPtr.Copy( registrar );
    
    iReferParams.iRecipentAddress.Append(KRecipent) ;
    iReferParams.iRecipentAddress.Append(*registrar16)  ;
    
    iReferParams.iReferTo.Append(KRecipent) ;
    iReferParams.iReferTo.Append(*registrar16)  ;
    
    CleanupStack::PopAndDestroy( registrar16 );
    registrar16 = NULL;
    
    CleanupStack::PopAndDestroy( uri8 );
    uri8 = NULL; 
    // profile registrar end .....
	}