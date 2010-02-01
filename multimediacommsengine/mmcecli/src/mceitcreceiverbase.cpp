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





#include "mceitcreceiverbase.h"
#include "mceitc.h"
#include "mceclilogs.h"


_LIT8( KMceEmpty, "");

#ifdef MCE_UNIT_TEST
#define MCE_UNIT_TEST_RETURN { if ( aError == KErrNoMemory ) return aError; }
#else
#define MCE_UNIT_TEST_RETURN
#endif
 
// -----------------------------------------------------------------------------
// CMceItcReceiverBase::CMceItcReceiverBase
// -----------------------------------------------------------------------------
//
CMceItcReceiverBase::CMceItcReceiverBase ( MMceItc& aITC ) 
 : CActive( EPriorityStandard ),
   iITC( aITC )
	{
    CActiveScheduler::Add ( this );
	
	}

// -----------------------------------------------------------------------------
// CMceItcReceiverBase::~CMceItcReceiverBase
// -----------------------------------------------------------------------------
//
CMceItcReceiverBase::~CMceItcReceiverBase () 
	{
	CActive::Cancel();
	}
	

// -----------------------------------------------------------------------------
// CMceItcReceiverBase::Init
// -----------------------------------------------------------------------------
//
void CMceItcReceiverBase::Init() 
	{
	
    iITCMsgArgs.Set (EMceItcArgIds, &iIdsPckg);
    iITCMsgArgs.Set (EMceItcArgBufSizes, &iSizesPckg);
	
	ReceiveNext();
	
	}


// -----------------------------------------------------------------------------
// CMceItcReceiverBase::RunL
// -----------------------------------------------------------------------------
//
void CMceItcReceiverBase::RunL ()
	{
    MCECLI_DEBUG("CMceItcReceiverBase::RunL, Entry");
	
	TInt err = iStatus.Int();
    MCECLI_DEBUG_DVALUE("status", err );
	
	if (err == KErrServerTerminated)
		{
        MCECLI_DEBUG("unrecoverable error!");
		// This will leave to RunError-function. See below.
		User::Leave( err );
		}
    // Take copy of the ids. iIdsPckg could be overwritten by ReceiveNext.
    TMceIds ids( iIdsPckg() );		
    if ( err == KErrNone )
        {
        MCECLI_DEBUG("itc received");
		IncomingEventL( ids );
        }
    else if ( err == KErrCompletion )
        {
        MCECLI_DEBUG("itc completed");
        err = KErrNone;
        }
    else
        {
        MCECLI_DEBUG("renew itc");
        // For errors receive has to be renewed before calling the observer 
        // which might delete the CMCE or CMCEConnection, 
        // which leads to deletion of this object.
        ReceiveNext();
        ErrorOccuredL( ids, err );
        }    
    MCECLI_DEBUG("CMceItcReceiverBase::RunL, Exit");
	}

// -----------------------------------------------------------------------------
// CMceItcReceiverBase::RunError
// -----------------------------------------------------------------------------
//
TInt CMceItcReceiverBase::RunError( TInt aError )
	{
    MCECLI_DEBUG("CMceItcReceiverBase::RunError, Entry");
    MCECLI_DEBUG_DVALUE("error", aError );

	if (aError == KErrServerTerminated)
		{
		// Return the error to the active scheduler and stop receiving.
		// Receiving must be stopped.
		// It would lead to infinite RunL-RunError-loop.
		return aError;
		}
    // Ignore all the other errors and continue receiving
	ReceiveNext ();
    
    MCECLI_DEBUG("CMceItcReceiverBase::RunError, Exit");
    
    MCE_UNIT_TEST_RETURN
    
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CMceItcReceiverBase::RunError
// -----------------------------------------------------------------------------
//
void CMceItcReceiverBase::DoCancel()
    {
    MCECLI_DEBUG("CMceItcReceiverBase::DoCancel, Entry");
    
    iITC.CancelReceive( iITCMsgArgs );

    MCECLI_DEBUG("CMceItcReceiverBase::DoCancel, Exit");
    
    }

// -----------------------------------------------------------------------------
// CMceItcReceiverBase::ReceiveNext
// -----------------------------------------------------------------------------
//
void CMceItcReceiverBase::ReceiveNext()
	{
    MCECLI_DEBUG("CMceItcReceiverBase::ReceiveNext, Entry");
	
	if (!IsActive())
        {
        iITC.Receive( iITCMsgArgs,iStatus );
	    SetActive();
        }
    MCECLI_DEBUG("CMceItcReceiverBase::ReceiveNext, Exit");
	}
	

// -----------------------------------------------------------------------------
// CMceItcReceiverBase::ReceiveDataLC
// -----------------------------------------------------------------------------
//
void CMceItcReceiverBase::ReceiveDataLC( HBufC8*& aContext, HBufC8*& aContent )
	{
    MCECLI_DEBUG("CMceItcReceiverBase::ReceiveDataLC, Entry");
	
    TIpcArgs tmpITCArgs;
    TMceIds ids( iIdsPckg() );
    TPckgBuf<TMceIds> tmpIdsPckg( ids );
    tmpITCArgs.Set( EMceItcArgIds, &tmpIdsPckg );
    
    if ( iSizesPckg().iContextBufSize != KErrNotFound )
        {
        aContext = HBufC8::NewLC( iSizesPckg().iContextBufSize );
        }
        
    if ( iSizesPckg().iContentBufSize != KErrNotFound )
        {
        aContent = HBufC8::NewLC( iSizesPckg().iContentBufSize );
        }

    __ASSERT_ALWAYS( !( !aContext && aContent ), User::Leave( KErrGeneral ) );
    
    if ( aContext || aContent )
        {
    	MCECLI_DEBUG("CMceItcReceiverBase::ReceiveDataLC, receiving data!!") 

		HBufC8* dummy = KMceEmpty().AllocLC();
        TInt err = iITC.Receive( tmpITCArgs, 
                                 ( aContext ? aContext->Des() : dummy->Des() ), 
                                 ( aContent ? aContent->Des() : dummy->Des() ) );
                                 
    	MCECLI_DEBUG("CMceItcReceiverBase::ReceiveDataLC, data received") 	
		CleanupStack::PopAndDestroy( dummy );
		    		        
        if ( err != KErrNone ) 
            {
    		MCECLI_DEBUG("CMceItcReceiverBase::ReceiveDataLC, receiving failed")
            User::Leave( err );
            }
        }
    MCECLI_DEBUG("CMceItcReceiverBase::ReceiveDataLC, Exit");
        
	}
	


// -----------------------------------------------------------------------------
// CMceItcReceiverBase::IncomingEventL
// -----------------------------------------------------------------------------
//
void CMceItcReceiverBase::IncomingEventL( TMceIds& aIds )
    {
    MCECLI_DEBUG("CMceItcReceiverBase::IncomingEventL, Entry");
    
    HBufC8* context = NULL;
    HBufC8* content = NULL;
    TMceItcCallBack callback = static_cast<TMceItcCallBack>( aIds.iCallbackID );
    
    MCECLI_DEBUG_CALLBACK( "CALLBACK", callback );
    MCECLI_DEBUG_IDS( "IDS", aIds );

    ReceiveDataLC( context, content );
        
    ReceiveNext();
    
    if ( context && content )
        {
    	MCECLI_DEBUG("CMceItcReceiverBase::IncomingEventL, received both data") 	
        IncomingEventL( callback, aIds, *context, content );
        CleanupStack::Pop( content );
        CleanupStack::PopAndDestroy( context );//context
        }
    else if ( !context && !content )
        {
    	MCECLI_DEBUG("CMceItcReceiverBase::IncomingEventL, received no data") 	
        IncomingEventL( callback, aIds );
        }
    else
        {
    	MCECLI_DEBUG("CMceItcReceiverBase::IncomingEventL, received context") 	
        IncomingEventL( callback, aIds, *context );
        CleanupStack::PopAndDestroy( context );
        }

    MCECLI_DEBUG("CMceItcReceiverBase::IncomingEventL, Exit");
    }

