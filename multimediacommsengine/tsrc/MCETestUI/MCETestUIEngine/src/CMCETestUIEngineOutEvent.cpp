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
#include <MCEOutEvent.h>
#include <Uri8.h>

#include "MCETestUIEngineDefaultValues.h"
#include "CMCETestUIQuestionDataQuery.h"
#include "CMCETestUIEngine.h"
#include "CMCETestUIEngineProfileContainer.h"
#include "CMCETestUIEngineProfile.h"
#include "CMCETestUIEngineOutEvent.h"
#include "TMCETestUIEngineCmdSubscribeEvent.h"
#include "TMCETestUIEngineCmdUpdateEvent.h"
#include "TMCETestUIEngineCmdDeleteEvent.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutEvent::NewL()
// -----------------------------------------------------------------------------
//
CMCETestUIEngineOutEvent* CMCETestUIEngineOutEvent::NewL(
            CMCETestUIEngine& aEngine,
            CMCETestUIEngineProfile& aProfile) 
    {

    CMCETestUIEngineOutEvent* self = 
        new (ELeave) CMCETestUIEngineOutEvent( aEngine, 
                                                 aProfile);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;  
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutEvent::CMCETestUIEngineOutEvent
// -----------------------------------------------------------------------------
//
CMCETestUIEngineOutEvent::CMCETestUIEngineOutEvent( 
                CMCETestUIEngine& aEngine,
                CMCETestUIEngineProfile& aProfile )
    : CMCETestUIEngineEvent( aEngine ),
      iProfile( &aProfile ),
      iId( 0 )
    {
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutEvent::ConstructL()
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineOutEvent::ConstructL()
    {
 
     /////////////////////
    //
    // TODO: After implementing wrappers for streams, sinks and sources 
    //       this function should be refactored to make use of them. That will 
    //       also simplify the function to some extent.
    //
    //////////////////////
    
 
    BaseConstructL();
    FillProfileRegistrar();
    
    if(EventParamsL(iEventParams))
    	{
    	// Solve recipient
	    CMCETestUIQuestionDataQuery* question = 
		    CMCETestUIQuestionDataQuery::NewLC();
		question->SetCaptionL( KUserQuestionInputRecipient );
		question->SetDefaultValueL(iEventParams.iRecipentAddress  );
		question->SetAnswerL( iEventParams.iRecipentAddress  );
		TBufC8<100> recipient( question->Answer8() );
		
		CleanupStack::PopAndDestroy( question );
		//solve eventheader
		
		CMCETestUIQuestionDataQuery* questioneventheader = 
		    CMCETestUIQuestionDataQuery::NewLC();
		questioneventheader->SetCaptionL( KUserQuestionInputEventHeader );
		questioneventheader->SetDefaultValueL(iEventParams.iEventHeader  );
		questioneventheader->SetAnswerL( iEventParams.iEventHeader  );
		TBufC8<100> eventHeader( questioneventheader->Answer8() );
		
		CleanupStack::PopAndDestroy( questioneventheader );
	   // solve originator
	   
	   if ( iEventParams.originator.Compare(KNone))
	   	{
		   	HBufC8* orig8 = HBufC8::NewL(iEventParams.originator.Length());
		   	TPtr8 ptr = orig8->Des();
		    ptr.Copy(iEventParams.originator);
		    // Create OutEvent
		    CMceOutEvent* outEvent = CMceOutEvent::NewL(
		       	iEngine.MCEManager(), 
			    iProfile->Profile(),
			    recipient,eventHeader,iEventParams.refreshInterval,orig8 );
			CleanupStack::PushL(outEvent);
		    CleanupStack::Pop( outEvent );
			iEvent = outEvent;
			iOutEvent = outEvent;        
	   	}
	   else 
	   	{
		   	// Create OutEvent
		    CMceOutEvent* outEvent = CMceOutEvent::NewL(
		       iEngine.MCEManager(),
		       iProfile->Profile(),
			   recipient,eventHeader,iEventParams.refreshInterval );
			CleanupStack::PushL(outEvent);
		    CleanupStack::Pop( outEvent );
			iEvent = outEvent;
			iOutEvent = outEvent;
	   	}
	    // Get identity
	    iId = iEngine.NextOutEventId();
    	}
    
    }
// -----------------------------------------------------------------------------
// CMCETestUIEngineOutEvent::NewL()
// -----------------------------------------------------------------------------
//
CMCETestUIEngineOutEvent* CMCETestUIEngineOutEvent::NewL(
            CMCETestUIEngine& aEngine,
            CMceOutEvent* aOutEvent) 
    {

    CMCETestUIEngineOutEvent* self = 
        new (ELeave) CMCETestUIEngineOutEvent( aEngine, 
                                                 aOutEvent);
    CleanupStack::PushL( self );
    self->ConstructL(aOutEvent);
    CleanupStack::Pop( self );
    return self;  
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutEvent::CMCETestUIEngineOutEvent
// -----------------------------------------------------------------------------
//
CMCETestUIEngineOutEvent::CMCETestUIEngineOutEvent( 
                CMCETestUIEngine& aEngine,
                CMceOutEvent* aOutEvent )
    : CMCETestUIEngineEvent( aEngine ),
      iOutEvent( aOutEvent ),
      iId( 0 )
    {
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutEvent::ConstructL()
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineOutEvent::ConstructL(CMceOutEvent* /*aOutEvent*/)
    {
 
     /////////////////////
    //
    // TODO: After implementing wrappers for streams, sinks and sources 
    //       this function should be refactored to make use of them. That will 
    //       also simplify the function to some extent.
    //
    //////////////////////
    
 
    User::LeaveIfError( iOutEvent ? KErrNone : KErrArgument );
     
    BaseConstructL();
    iEvent = iOutEvent;
    
    // Get identity
	iId = iEngine.NextOutEventId();
    
    }


// -----------------------------------------------------------------------------
// CMCETestUIEngineOutEvent::~CMCETestUIEngineOutEvent
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngineOutEvent::~CMCETestUIEngineOutEvent()
    {
    delete iOutEvent;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutEvent::GetCommands
// -----------------------------------------------------------------------------
//
const RPointerArray<TMCETestUIEngineCmdBase>& 
            CMCETestUIEngineOutEvent::GetCommandsL()
    {
    iCommands.ResetAndDestroy();

    // Get base class commands
    CMCETestUIEngineEvent::GetCommandsL();
        
    // Get own commands       
    TInt state = iOutEvent->State();

	switch( state ) 
		{
		case CMceEvent::EIdle:
		    iCommands.Append( new (ELeave) TMCETestUIEngineCmdSubscribeEvent( 
                                iEngine, *this ) );
            iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdDeleteEvent( iEngine, *this ) );	  
		    break;
		case CMceEvent::EActive: 
		    iCommands.Append( new (ELeave) TMCETestUIEngineCmdUpdateEvent( 
                                iEngine, *this ) );
            iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdDeleteEvent( iEngine, *this ) );	  
		    break;
		case CMceEvent::EPending:  
			iCommands.Append( new (ELeave) TMCETestUIEngineCmdSubscribeEvent( 
                                iEngine, *this ) );
            iCommands.Append( new (ELeave) TMCETestUIEngineCmdUpdateEvent( 
                                iEngine, *this ) );
            iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdDeleteEvent( iEngine, *this ) );
		    break;    	     
		default: 
		    break;
		}  
		  
    return iCommands; 
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutEvent::Id
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMCETestUIEngineOutEvent::Id() const
    {
    return iId;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutSession::OutSession
// -----------------------------------------------------------------------------
//
CMceOutEvent& CMCETestUIEngineOutEvent::OutEvent()
    {
    return *iOutEvent;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutEvent::EventParams
// -----------------------------------------------------------------------------
//

TBool CMCETestUIEngineOutEvent::EventParamsL(TEventParams &aEventParams)
{
  return iEngine.EventParamsL( aEventParams);
 
}

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutEvent::FillProfileRegistrar
// -----------------------------------------------------------------------------
//

void CMCETestUIEngineOutEvent::FillProfileRegistrar()
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
    
    iEventParams.iRecipentAddress.Append(KRecipent) ;
    iEventParams.iRecipentAddress.Append(*registrar16)  ;
    
    CleanupStack::PopAndDestroy( registrar16 );
    registrar16 = NULL;
    
    CleanupStack::PopAndDestroy( uri8 );
    uri8 = NULL; 
    // profile registrar end .....
	}