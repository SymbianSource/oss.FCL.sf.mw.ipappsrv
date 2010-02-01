/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Plugin interface implementation.
*
*/




#include "CNSPPluginStub.h"
#include "nspsessionobserver.h"


const TInt KDefaultSessionID		      = 0;

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// CNSPPluginStub::CNSPPluginStub
// ---------------------------------------------------------------------------
//
CNSPPluginStub::CNSPPluginStub()
    {
   	iRequestType = CNSPPluginStub::ENone; 
    }


// ---------------------------------------------------------------------------
// CNSPPluginStub::ConstructL
// ---------------------------------------------------------------------------
//
void CNSPPluginStub::ConstructL()
    {
    RDebug::Print(_L("ICEPlugin Dummy: Entry (CNSPPluginStub::ConstructL)"));
    // Create the timer entry 
        
    iOfferReady = 0;
    iAnswerReady = 0;
	iDTimer = CDeltaTimer::NewL( CActive::EPriorityStandard );
	iReservStatus = CNSPPlugin::ENSPResourcesNotReserved;
		
	TCallBack cb( TimerExpire, this );
	iDeltaEntry.Set( cb );
	RDebug::Print(_L("ICEPlugin Dummy: Exit (CNSPPluginStub::ConstructL)"));
	}


// ---------------------------------------------------------------------------
// CNSPPluginStub::NewL
// ---------------------------------------------------------------------------
//
CNSPPluginStub* CNSPPluginStub::NewL()
    {
    CNSPPluginStub* self = new( ELeave ) CNSPPluginStub;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPPluginStub::~CNSPPluginStub
// ---------------------------------------------------------------------------
//
CNSPPluginStub::~CNSPPluginStub() 
    {
	delete iDTimer;
	}


// ---------------------------------------------------------------------------
// CNSPPluginStub::NewSessionL
// ---------------------------------------------------------------------------
//
TUint CNSPPluginStub::NewSessionL( MNSPSessionObserver& aSessionObserver,
                                  TUint32 /*aIapId*/,
                                  const TDesC8& /*aDomain*/,
                                  TUint /*aProtocol*/ )
    {
	RDebug::Print(_L("ICEPlugin Dummy: Entry (CNSPPluginStub::NewSessionL)"));

	iDelay = 50000;
   	   	
   	iSessionOb = &aSessionObserver;
	iDTimer->Queue( iDelay, iDeltaEntry );
	RDebug::Print(_L("ICEPlugin Dummy: Exit (CNSPPluginStub::NewSessionL)"));
	// Set the timer to get fired for the Initialized Event
	return KDefaultSessionID;
    }


// ---------------------------------------------------------------------------
// CNSPPluginStub::CloseSessionL
// ---------------------------------------------------------------------------
//
void CNSPPluginStub::CloseSessionL( TUint /*aSessionId*/ )
    {
   	iSessionId = 0; 
    }


// ---------------------------------------------------------------------------
// CNSPPluginStub::CreateOfferL
// ---------------------------------------------------------------------------
//
TNatReturnStatus CNSPPluginStub::CreateOfferL( TUint aSessionId, CSdpDocument*& aOffer)
    {
    RDebug::Print(_L("ICEPlugin Dummy: Entry (CNSPPluginStub::CreateOfferL)"));   
	iRequestType = CNSPPluginStub::ECreateOffer;  
    User::LeaveIfNull( &aOffer );
    iSessionId = aSessionId; 
    iReservStatus =  CNSPPlugin::ENSPResourcesReserved;;
    iOffer = aOffer;
    iDelay = iDelay + (iDelay*2);
    iDTimer->Queue( iDelay, iDeltaEntry );
    RDebug::Print(_L("ICEPlugin Dummy: Exit (CNSPPluginStub::CreateOfferL)"));
    return KNatAsync;             
    }


// ---------------------------------------------------------------------------
// CNSPPluginStub::ResolveL
// ---------------------------------------------------------------------------
//
TNatReturnStatus CNSPPluginStub::ResolveL( TUint aSessionId,
                              CSdpDocument*& aOffer,
                              CSdpDocument*& aAnswer )
    {
    RDebug::Print(_L("ICEPlugin Dummy: Entry (CNSPPluginStub::ResolveL)"));   
    iRequestType = CNSPPluginStub::EResolve; 
    User::LeaveIfNull( &aOffer );
    User::LeaveIfNull( &aAnswer ); 
    iSessionId = aSessionId;
       
  	iOffer = aOffer;
  	iAnswer = aAnswer;
  	
  	iDelay = iDelay + iDelay;
  	iDTimer->Queue( iDelay, iDeltaEntry );
  	RDebug::Print(_L("ICEPlugin Dummy: Exit (CNSPPluginStub::ResolveL)"));
  	return KNatAsync;
    }


// ---------------------------------------------------------------------------
// CNSPPluginStub::DecodeAnswerL
// ---------------------------------------------------------------------------
//
TNatReturnStatus CNSPPluginStub::DecodeAnswerL( TUint aSessionId, CSdpDocument*& aAnswer )
    {
    RDebug::Print(_L("ICEPlugin Dummy: Entry (CNSPPluginStub::DecodeAnswerL)"));   
    iRequestType = CNSPPluginStub::EDecodeAnswer;
    User::LeaveIfNull(&aAnswer);
    iSessionId = aSessionId;
	iAnswer = aAnswer;
    iDelay = iDelay + iDelay;
    iDTimer->Queue( iDelay, iDeltaEntry );
    RDebug::Print(_L("ICEPlugin Dummy: Exit (CNSPPluginStub::DecodeAnswerL)"));
    return KNatAsync;
    }


// ---------------------------------------------------------------------------
// CNSPPluginStub::UpdateL
// ---------------------------------------------------------------------------
//
void CNSPPluginStub::UpdateL( TUint aSessionId, CSdpDocument*& aOffer )
    {
    RDebug::Print(_L("ICEPlugin Dummy: Entry (CNSPPluginStub::UpdateL)"));   
    iRequestType = CNSPPluginStub::EResolve; 
    User::LeaveIfNull( &aOffer );
	iSessionId = aSessionId;
	iOffer = aOffer;
	RDebug::Print(_L("ICEPlugin Dummy: Exit (CNSPPluginStub::UpdateL)"));
    }
    

// ---------------------------------------------------------------------------
// CNSPPluginStub::RestartL
// ---------------------------------------------------------------------------

TNatReturnStatus CNSPPluginStub::RestartL( TUint aSessionId )
	{
	RDebug::Print(_L("ICEPlugin Dummy: Entry (CNSPPluginStub::RestartL)"));   
	iSessionId = aSessionId;	
	RDebug::Print(_L("ICEPlugin Dummy: Exit (CNSPPluginStub::RestartL)"));
	return KNatAsync;	
	}


// ---------------------------------------------------------------------------
// CNSPPluginStub::GetSessionParam
// ---------------------------------------------------------------------------

 TInt CNSPPluginStub::GetSessionParam( TUint /*aSessionId*/, 
                       TNSPSessionParamKey /*aParamKey*/ )
 	{
 	RDebug::Print(_L("ICEPlugin Dummy: Entry (CNSPPluginStub::GetSessionParam)"));   
	RDebug::Print(_L("ICEPlugin Dummy: Exit (CNSPPluginStub::GetSessionParam)"));   
	return iReservStatus;
 	}
                           
                               
    
// -----------------------------------------------------------------------------
// CNSPPluginStub::TimerExpire
// -----------------------------------------------------------------------------
//

TInt CNSPPluginStub::TimerExpire( TAny *aSelf )
    { 
    CNSPPluginStub* me = static_cast<CNSPPluginStub *>( aSelf );
    
    if ( me->iInitialized == 0)
    	{
    	RDebug::Print(_L("ICEPlugin Dummy: (Plugin->Initialized)"));   
    	me->iInitialized = 1;	
		me->iSessionOb->Initialized(0);
		}
    	
	else if ( me->iOfferReady == 1 )
		{
		RDebug::Print(_L("ICEPlugin Dummy: (Plugin->OfferReady)"));   
		me->iOfferReady = 0;
		me->iReservStatus =  CNSPPlugin::ENSPResourcesReserved;;
		me->iSessionOb->OfferReady( KDefaultSessionID, me->iOffer );
		}
	//TBD: For Future Use when Re-Invite for UpdateSDP is supported
	
	/*	
	else if ( me->iAnswerReady == 1 )
		{
		RDebug::Print(_L("ICEPlugin Dummy: (Plugin->OfferReady)"));   
		me->iOfferReady = 0;
		me->iSessionOb->UpdateSdp( KDefaultSessionID, me->iOffer );	
		}
	*/
	else 
		{ 
		switch ( me->iRequestType )
		{
		
		case ECreateOffer:
            {
            RDebug::Print(_L("ICEPlugin Dummy: (Plugin->OfferReady)"));   
            me->iSessionOb->OfferReady(KDefaultSessionID, me->iOffer);
            break;
            }
          
		case EResolve:
            {
            RDebug::Print(_L("ICEPlugin Dummy: (Plugin->AnswerReady)"));   
            me->iSessionOb->AnswerReady(KDefaultSessionID, me->iAnswer);
            me->iOfferReady = 1;
            me->iDelay = me->iDelay + me->iDelay;
            me->iDTimer->Queue( me->iDelay*2, me->iDeltaEntry );
            break;
            }

		case EDecodeAnswer:
            {
            RDebug::Print(_L("ICEPlugin Dummy: (Plugin->AnswerReady)"));   
			me->iAnswerReady = 1;
			me->iSessionOb->AnswerReady( KDefaultSessionID, me->iAnswer );
			
			//TBD: For Future Use when Re-Invite for UpdateSDP is supported
			
			/*
			me->iDelay = me->iDelay + me->iDelay;
            me->iDTimer->Queue( me->iDelay*50, me->iDeltaEntry );
            */
			
			break;
            }
      
        default:
            {
            //NOP
            break;
            }
		}
		}
	   
    return KErrNone;
    }


// end of file
