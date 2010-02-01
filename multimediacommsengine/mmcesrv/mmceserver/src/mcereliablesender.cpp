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




#include <e32base.h>
#include <e32cmn.h>
#include <siprackheader.h>
#include <sipconnection.h>
#include <sipresponseelements.h>
#include <siprequestelements.h>
#include <sipcseqheader.h> 
#include <siprseqheader.h> 
#include <centralrepository.h>
#include <sipsdkcrkeys.h>
#include <stringpool.h>
#include "mcesip.h"
#include "mcesrvlogs.h"
#include "mcereliableobserver.h"
#include "mcereliablesender.h"
#include "mceeventstatebase.h"

// CONSTANTS
const TInt KMaxTransTimeOut = 64;
const TUint KTransInterval = 2;


// -----------------------------------------------------------------------------
// CMCEReliableSender::NewL ;
// Two-phased constructor. ;
// -----------------------------------------------------------------------------
//
CMceReliableSender* CMceReliableSender::NewL( MMceReliableObserver& aObserver )
    {
	CMceReliableSender* self = new( ELeave ) CMceReliableSender( aObserver );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
    }

// -----------------------------------------------------------------------------
// CMceReliableSender::CMceReliableSender
// Constructor
// -----------------------------------------------------------------------------
//
CMceReliableSender::CMceReliableSender( MMceReliableObserver& aObserver )
: iObserver( &aObserver )
    {
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CMceReliableSender::~CMceReliableSender()
    {
	delete iDTimer;
	delete iResponseElements;
    }

// -----------------------------------------------------------------------------
// CMceReliableSender::ConstructL
// Symbian 2nd phase constructor.
// -----------------------------------------------------------------------------
//
void CMceReliableSender::ConstructL()
    {
	const TUint	KMaxRSec = 0x7fffffff;
    const TUint	KMinRSec = 1;
	TInt timerT1;
	CRepository* repository = CRepository::NewLC( KCRUidSIP );
	User::LeaveIfError( repository->Get( KSIPTransactionTimerT1, timerT1 ));
	CleanupStack::PopAndDestroy( repository );
    
    iT1 = timerT1;
     
    iRetransTimer = 1;
    iTotaltime = 0;
    
    iSequenceNumber = MceSip::Random( KMinRSec, KMaxRSec );
	iTransactionMatched = EFalse;
	iTransactionStatus = EFalse;	// No Pending transaction Yet 
	iTransTimeoutFlag = EFalse;
	
    // Create the timer entry 
	iDTimer = CDeltaTimer::NewL( CActive::EPriorityStandard );
		
	TCallBack cb( TimerExpire, this );
	iDeltaEntry.Set( cb );
    }

// -----------------------------------------------------------------------------
// CMceReliableSender::Rseq
// -----------------------------------------------------------------------------
//
TUint CMceReliableSender::Rseq( CSIPServerTransaction& aTransaction )
    {
    TInt tInProcess = 0;
	if  ( iTransactionStatus )
	    {
	    MCESRV_DEBUG( "CMceReliableSender::Rseq: Transsaction in Process");
	    return tInProcess;
	    }
   	else if( &aTransaction == iServerTransaction )
	    {
		iSequenceNumber = iSequenceNumber + 1;
		return iSequenceNumber;
	    }
	else
	    {
	    return iSequenceNumber;
	    }
    }

// -----------------------------------------------------------------------------
// CMceReliableSender::SendResponseL
// -----------------------------------------------------------------------------
//
void CMceReliableSender::SendResponseL( CSIPServerTransaction& aTransaction, 
                                        CSIPResponseElements* aElements )
    {
	if  ( iTransactionStatus )
	    {
        User::Leave( KErrInUse );
	    }

    iServerTransaction = &aTransaction;
	CSIPResponseElements* xResponses;

	// Set the Transaction in progree Flag  
	
	iTransactionStatus = ETrue;
    TUint queueTimer = 0;
         	
	xResponses = ResponseCloneL( *aElements );
	CleanupStack::PushL( xResponses );
    iServerTransaction->SendResponseL( xResponses );
    MCESRV_DEBUG_DVALUE( 
        "CMceReliableSender::SendResponseL - timer value:", iRetransTimer );
    CleanupStack::Pop( xResponses );
   	iResponseElements = aElements;  
   	
   	queueTimer = iRetransTimer * iT1 *KConversionMilliToMicro;
	iDTimer->Queue( queueTimer, iDeltaEntry );
    iRetransTimer = iRetransTimer * KTransInterval;    
    }

// -----------------------------------------------------------------------------
// CMceReliableSender::ResponseCloneL
// -----------------------------------------------------------------------------
//
CSIPResponseElements* CMceReliableSender::ResponseCloneL( 
                                            CSIPResponseElements& aElements )
    {
	CSIPResponseElements* cElements = 
	    CSIPResponseElements::NewLC( aElements.StatusCode(), 
	                                 aElements.ReasonPhrase() );
	// Creating messge Elements
	CSIPMessageElements& msgElements = aElements.MessageElements();
	CSIPMessageElements& cloneMsgElements = cElements->MessageElements();


	// Copying Contents 
	if  ( msgElements.Content().Length() )
	    {
    	const CSIPContentTypeHeader* contentType = msgElements.ContentType();
		CSIPContentTypeHeader* cloneContentType = 
		    static_cast<CSIPContentTypeHeader *>( contentType->CloneL() );
		CleanupStack::PushL( cloneContentType );

		HBufC8* contentBuffer = HBufC8::NewLC( msgElements.Content().Length() );
		TPtr8 contentsPtr = contentBuffer->Des();
		contentsPtr.Append( msgElements.Content() );
    
		cloneMsgElements.SetContentL( contentBuffer, cloneContentType );
		CleanupStack::Pop( contentBuffer );
		CleanupStack::Pop( cloneContentType );
    	}

  	// User Headers
	const RPointerArray<CSIPHeaderBase>& headers = msgElements.UserHeaders();
	if  ( headers.Count() )
	    {
		RPointerArray<CSIPHeaderBase> tmpHeaders;
		CSIPHeaderBase::PushLC( &tmpHeaders );
		for(TInt i = 0; i < headers.Count(); i++)
		    {
			CSIPHeaderBase* userHeader = headers[i]->CloneL();
			CleanupStack::PushL( userHeader );
			tmpHeaders.AppendL( userHeader );
			
			CleanupStack::Pop( userHeader );
		    }

		cloneMsgElements.SetUserHeadersL( tmpHeaders );
		CleanupStack::Pop( &tmpHeaders );
	    }
    CleanupStack::Pop( cElements );
	return cElements;
    }

// -----------------------------------------------------------------------------
// CMceReliableSender::PrackReceived
// -----------------------------------------------------------------------------
//
TBool CMceReliableSender::PrackReceived( CSIPServerTransaction& aPrack )
    {
	TBool transactionMatched = EFalse;
	RStringF rackHeaderStr = SIPStrings::StringF( SipStrConsts::ERAckHeader );
	RStringF rseqStr = SIPStrings::StringF( SipStrConsts::ERSeqHeader );
			
	const CSIPRequestElements* reqElements = aPrack.RequestElements();
	CSIPHeaderBase*	headerBase = 
	    MceSip::FindHeader( reqElements->MessageElements(), rackHeaderStr );
	const CSIPRAckHeader* rackHeader = static_cast<CSIPRAckHeader *>( headerBase );
	
	const CSIPRequestElements* sReqElements = iServerTransaction->RequestElements();    
    const CSIPCSeqHeader* cseqHeader = sReqElements->CSeqHeader();
    
    CSIPHeaderBase* headerBase3 =  
        MceSip::FindHeader( iResponseElements->MessageElements(), rseqStr );
    const CSIPRSeqHeader* rseqHeader = static_cast<CSIPRSeqHeader *> ( headerBase3 );
  
      
   	if  ( ( rackHeader && cseqHeader && rseqHeader ) && 
   	    ( rackHeader->CSeqNum() == cseqHeader->Seq() ) &&
	    ( rackHeader->Method() == sReqElements->Method() ) && 
	    ( rackHeader->Seq() == rseqHeader->Value()) )

	    {
	    MCESRV_DEBUG( "CMceReliableSender::PrackReceived: prack matched");
	    transactionMatched = ETrue;
        }
    
    if  ( transactionMatched )
	    {
        // Cease the Re-Transmission of Reliable Response 
                
        iDTimer->Remove( iDeltaEntry ); 
        iTransactionMatched = ETrue;
        iTransactionStatus = EFalse;    
   
      	return ETrue;
  	    }

    MCESRV_DEBUG( "CMceReliableSender::PrackReceived: no prack matched");
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CMceReliableSender::TimerExpire
// -----------------------------------------------------------------------------
//
TInt CMceReliableSender::TimerExpire( TAny *aSelf )
    { 
    TInt error;
    TUint queueTimer;
    TUint tmpTotalTime;
               
    CMceReliableSender* me = static_cast<CMceReliableSender *>( aSelf );
            
    if ( !me->iTransTimeoutFlag )        
        {
        TRAP(error, me->CloneAndSendResponseL());
  
        if ( error )
            {
            me->iObserver->ReliableFailed( *me->iServerTransaction, error );
            }
        }
        
    else 
        {
        me->TimeOut( me->iServerTransaction );    
        }
		
	if ( me->iRetransTimer <= KMaxTransTimeOut && me->iTransactionStatus )
		{
	    tmpTotalTime = me->iTotaltime;
		me->iTotaltime = me->iTotaltime + me->iRetransTimer; // acumlated time
			
		if ( me->iTotaltime <= KMaxTransTimeOut )
		    {
		    queueTimer = me->iRetransTimer * me->iT1 * KConversionMilliToMicro;
		    }
        else 
           {
            me->iTotaltime = tmpTotalTime;
            queueTimer = ( KMaxTransTimeOut - me->iTotaltime ) * me->iT1 * KConversionMilliToMicro;
            me->iTransTimeoutFlag = ETrue;
            }
            
        me->iDTimer->Queue( queueTimer, me->iDeltaEntry );
        me->iRetransTimer = me->iRetransTimer * KTransInterval;
        MCESRV_DEBUG_DVALUE( 
		"CMceReliableSender::TimerExpire: timer added", me->iRetransTimer );
          
		}

	return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMceReliableSender::CloneAndSendResponseL 
// -----------------------------------------------------------------------------
//
void CMceReliableSender::CloneAndSendResponseL()
    {
    CSIPResponseElements* oResponses;
    oResponses = ResponseCloneL( *iResponseElements );
    MCESRV_DEBUG( 
        "CMceReliableSender::CloneAndSendResponseL: responseElements cloned");
    CleanupStack::PushL( oResponses );
    iServerTransaction->SendResponseL( oResponses );
    MCESRV_DEBUG( 
        "CMceReliableSender::CloneAndSendResponseL: reliableResponse sent");
    CleanupStack::Pop( oResponses );
    }

// -----------------------------------------------------------------------------
// CMceReliableSender::TimeOut
// -----------------------------------------------------------------------------
//
void CMceReliableSender::TimeOut( CSIPServerTransaction* aTransaction )
    {  
	iObserver->NoPrackReceived( *aTransaction ); 
	}

// End of File
