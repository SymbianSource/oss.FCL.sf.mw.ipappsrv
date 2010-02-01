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
* Description:    Server side SIP event class
*
*/




#include  <e32base.h>
#include  <sipsubscribedialogassoc.h>
#include  <sipreferdialogassoc.h>
#include  <sipdialog.h>
#include  <sipservertransaction.h>
#include  <sipconnection.h>
#include  <sipclienttransaction.h>
#include  "cmcecomevent.h"
#include  "mcesipevent.h"
#include  "mceeventcontext.h"
#include  "mcecssubsession.h"
#include  "mcesipconnection.h"

// -----------------------------------------------------------------------------
// CMceComMoSubscribeEvent::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
 CMceComEvent* CMceComEvent::NewL( CMceSipEvent& aSIPEvent,
								   TUint32 aRefreshInterval )
	{
    CMceComEvent* self = new( ELeave ) CMceComEvent( aSIPEvent,
                                                     aRefreshInterval );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }



// -----------------------------------------------------------------------------
// CMceComEvent::CMceComEvent
// -----------------------------------------------------------------------------
//
CMceComEvent::CMceComEvent( CMceSipEvent& aSIPEvent,
                            TUint32 aRefreshInterval ):
    iSIPEvent( aSIPEvent ),
    iPreviousAction( KErrNotFound ),
    iRefreshInterval( aRefreshInterval ),
    iReferType( CMceRefer::ENoSuppression ),
    iSilentSuppression( EFalse ),
    iLeave( EFalse ),
    iIdValue( KMceEventIdNotAssigned )
	{
	
	}
	
// -----------------------------------------------------------------------------
// CMceComEvent::~CMCESipEventContext
// -----------------------------------------------------------------------------
//	
void CMceComEvent::ConstructL()
	{
	iEventContext = CMceEventContext::NewL( *this );
	iMsgContent = KNullDesC8().AllocL();
	iMsgContentType = KNullDesC8().AllocL();
	}
	
// -----------------------------------------------------------------------------
// CMceComEvent::~CMCESipEventContext
// -----------------------------------------------------------------------------
//	
 CMceComEvent::~CMceComEvent()
	{
	delete iEventContext;
    if(iMsgHeaders)
    	{
    	if(iMsgHeaders->MdcaCount())
    		{
    		iMsgHeaders->Reset();
    		}
    	delete iMsgHeaders;
    	}
    
    if ( iMsgContent)
        {
        delete iMsgContent; 
        iMsgContent = NULL;   
        }
    
    if ( iMsgContentType)
        {
        delete iMsgContentType; 
        iMsgContentType = NULL;   
        }
    
    if(iReason)
    	{
    	delete iReason;
    	iReason =NULL;	
    	}
	}

// -----------------------------------------------------------------------------
// CMceComEvent::PerformSubscribeActionL
// This is to perform the specific action ex Subscribe...
// for the specific event
// -----------------------------------------------------------------------------
//	
 void CMceComEvent::PerformActionL( TMceItcFunctions aEventAction )
	{
	iPreviousAction = aEventAction;

    }

// -----------------------------------------------------------------------------
// CMceComEvent::EventContext
// -----------------------------------------------------------------------------
//		
 CMceEventContext& CMceComEvent::EventContext()
	{
	return *iEventContext;
	}


// -----------------------------------------------------------------------------
// CMceComEvent::PreviousAction
// -----------------------------------------------------------------------------
//		
 TInt CMceComEvent::PreviousAction() const
	{
	return iPreviousAction;
	}
	
// -----------------------------------------------------------------------------
// CMceComEvent::SetPreviousAction
// -----------------------------------------------------------------------------
//		         
 void CMceComEvent::SetPreviousAction( TInt aEventAction )
	{
	iPreviousAction = aEventAction;
	}

// -----------------------------------------------------------------------------
// CMceComEvent::SetPreviousAction
// -----------------------------------------------------------------------------
//		         
CMceSipEvent& CMceComEvent::SIPEvent()
	{
	return iSIPEvent;
	}	

// -----------------------------------------------------------------------------
// CMceComEvent::RefreshInterval
// -----------------------------------------------------------------------------
//		         
TUint32 CMceComEvent::RefreshInterval()
	{
	return iRefreshInterval;
	}

// -----------------------------------------------------------------------------
// CMceComEvent::SetRefreshInterval
// -----------------------------------------------------------------------------
//		         
void  CMceComEvent::SetRefreshInterval( TUint32 aRefreshInterval )
	{
	iRefreshInterval = aRefreshInterval;
	}

// -----------------------------------------------------------------------------
// CMceComEvent::ProceedL
// This is for incoming response and error occured
// -----------------------------------------------------------------------------
//		         
void CMceComEvent::ProceedL()
	{
	User::LeaveIfError( iLeave ? KErrTotalLossOfPrecision : KErrNone );
    }    
	
// -----------------------------------------------------------------------------
// CMceComEvent::SetMsgHeaders
// -----------------------------------------------------------------------------
//               
void CMceComEvent::SetMsgHeaders( CDesC8Array* aMsgHeaders )
	{
	delete iMsgHeaders;
	iMsgHeaders = aMsgHeaders;
	}

// -----------------------------------------------------------------------------
// CMceComEvent::ReceivedRequestL
// This is for incoming request, if the dialog is not existed then nothing will
// be done
// -----------------------------------------------------------------------------
//      
void CMceComEvent::ReceivedRequestL()
	{
    }
        
// -----------------------------------------------------------------------------
// CMceComEvent::SetMsgContentL
// -----------------------------------------------------------------------------
// 
void CMceComEvent::SetMsgContentL( HBufC8* aContentType, 
        					       HBufC8* aContent )
	{
	delete iMsgContentType;
	iMsgContentType = NULL;	
	delete iMsgContent;
	iMsgContent= NULL;	
	
	if ( !aContentType )
	    {
	    iMsgContentType = KNullDesC8().AllocL();
	    }
    if ( !aContent )
        {
        iMsgContent = KNullDesC8().AllocL();
        }
        
    iMsgContentType = aContentType ? aContentType : iMsgContentType;
    iMsgContent = aContent ? aContent : iMsgContent;
    
	}
       
// -----------------------------------------------------------------------------
// CMceComEvent::MsgContent
// -----------------------------------------------------------------------------
// 
HBufC8* CMceComEvent::MsgContent()
	{
	return iMsgContent;	
	}

// -----------------------------------------------------------------------------
// CMceComEvent::SetReasonPhraseL
// -----------------------------------------------------------------------------
// 
void CMceComEvent::SetReasonPhraseL( TUint32 aStatusCode,
									HBufC8* aReasonPhrase )
	{
	iStatusCode = aStatusCode;
	if(iReason)
		{
		delete iReason;
		iReason = NULL;	
		}
	if ( aReasonPhrase )
	    {
	    iReason = aReasonPhrase;    
	    }
	else
	    {
	    iReason = KNullDesC8().AllocL();
	    }
	}
	
// -----------------------------------------------------------------------------
// CMceComEvent::ReasonPhrase
// -----------------------------------------------------------------------------
// 
TUint32 CMceComEvent::StatusCode() const
	{
	return iStatusCode;	
	}
	
// -----------------------------------------------------------------------------
// CMceComEvent::ReasonPhrase
// -----------------------------------------------------------------------------
// 
TDesC8& CMceComEvent::ReasonPhrase()
	{
	return *iReason;	
	}  
	             
// -----------------------------------------------------------------------------
// CMceComEvent::MsgContentType
// -----------------------------------------------------------------------------
// 
TDesC8& CMceComEvent::MsgContentType()
	{
	return *iMsgContentType;	
	}
	
// -----------------------------------------------------------------------------
// CMceComEvent::MsgHeaders
// -----------------------------------------------------------------------------
//        
 CDesC8Array& CMceComEvent::MsgHeaders( TInt aMethodInd )
	{
	if ( iMsgHeaders )
	    {
	    return *iMsgHeaders;    
	    }
	else
	    {
	    return iSIPEvent.DefaultHeaders( aMethodInd );
	    }
	}

// -----------------------------------------------------------------------------
// CMceComEvent::SetReferType
// -----------------------------------------------------------------------------
//        
void CMceComEvent::SetReferType( CMceRefer::TType aType)
    {
    iReferType = aType;
    }
    
// -----------------------------------------------------------------------------
// CMceComEvent::ReferType
// -----------------------------------------------------------------------------
//        
CMceRefer::TType CMceComEvent::ReferType() const
    {
    return iReferType;
    }

// -----------------------------------------------------------------------------
// CMceComEvent::SetSilentSuppressionOn
// -----------------------------------------------------------------------------
//        
void CMceComEvent::SetSilentSuppressionOn()
    {
    iSilentSuppression = ETrue;
    }

// -----------------------------------------------------------------------------
// CMceComEvent::SilentSuppression
// -----------------------------------------------------------------------------
//        
TBool CMceComEvent::SilentSuppression() const
    {
    return iSilentSuppression;
    }

// -----------------------------------------------------------------------------
// CMceComEvent::SetIdValue
// -----------------------------------------------------------------------------
//        
void CMceComEvent::SetIdValue( TUint aIdValue )
    {
    iIdValue = aIdValue;
    }

// -----------------------------------------------------------------------------
// CMceComEvent::IdValue
// -----------------------------------------------------------------------------
//        
TUint CMceComEvent::IdValue() const
    {
    return iIdValue;
    }
        
//  End of File
