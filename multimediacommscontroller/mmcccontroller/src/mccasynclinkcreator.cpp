/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Implementation of MCC session creator active object
*
*/




#include <mmf/common/mmfcontrollerframework.h>
#include <e32math.h>
#include <es_sock.h>
#include "mccasynclinkcreator.h"
#include "mcculdlclient.h"
#include "masynclinkcreationobserver.h"

#ifdef _DEBUG
#include <e32svr.h>
#define TRACE_LINK_CREATOR
#endif

#ifdef VOIP_TRACE_ENABLED
#include <voip_trace.h>
#endif


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ Default constructor
// ---------------------------------------------------------------------------
//
CMccAsyncLinkCreator::CMccAsyncLinkCreator( 
    MAsyncLinkCreationObserver& aObserver, CMccUlDlClient* aSession ) : 
    CActive( EPriorityStandard ), iObserver( aObserver ),
    iClientData( TMccCreateLinkPckg() ), iSession( aSession )
    {
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// Static constructor, does not leave instance in cleanupstack
// ---------------------------------------------------------------------------
//
CMccAsyncLinkCreator* CMccAsyncLinkCreator::NewL( 
    MAsyncLinkCreationObserver& aObserver, 
    CMccUlDlClient* aSession )
    {
    CMccAsyncLinkCreator* self = 
        CMccAsyncLinkCreator::NewLC( aObserver, aSession );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Static constructor, leaves instance in cleanupstack
// ---------------------------------------------------------------------------
//
CMccAsyncLinkCreator* CMccAsyncLinkCreator::NewLC( 
    MAsyncLinkCreationObserver& aObserver, 
    CMccUlDlClient* aSession )
    {
    CMccAsyncLinkCreator* self =
        new ( ELeave ) CMccAsyncLinkCreator( aObserver, aSession );
    CleanupStack::PushL( self );

    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CMccAsyncLinkCreator::~CMccAsyncLinkCreator()
    {
    this->Cancel();
    }

// ---------------------------------------------------------------------------
// From class CActive
// Active object eventhandler
// ---------------------------------------------------------------------------
//
void CMccAsyncLinkCreator::RunL()
    {
    #ifdef TRACE_LINK_CREATOR
        RDebug::Print( _L("CMccAsyncLinkCreator::RunL status: %d"),
           iStatus.Int() );
    #endif
    
    if( KErrNone == iStatus.Int() )
        {
        if ( iLinkCreated == EFalse )
            {                                                           
            iSession->CreateRtpSessionL( iClientData().iLinkID,
                                         iClientData().iLocalAddress.Port(),
                                         iClientData().iIpTOS,
                                         iClientData().iMediaSignaling );
            iLinkCreated = ETrue;
            }

        TRAPD( error, iSession->GetLocalIpAddressesL( iClientData() ) );
        
        #ifdef TRACE_LINK_CREATOR
            RDebug::Print( _L("CMccAsyncLinkCreator::RunL complete request: %d"),
                error );
        #endif
        
        this->CompleteRequest( error );
        }
    else
        {
        #ifdef TRACE_LINK_CREATOR
            RDebug::Print( _L("CMccAsyncLinkCreator::RunL ERR: %d"), 
                iStatus.Int() );
        #endif
        
        // Connection process failed for some reason, so complete the client
        this->CompleteRequest( iStatus.Int() );
        }
    }

// ---------------------------------------------------------------------------
// From class CActive
// Active object request cancellation
// ---------------------------------------------------------------------------
//
void CMccAsyncLinkCreator::DoCancel()
    {
    if( iSession )
        {
        #ifdef TRACE_LINK_CREATOR
            RDebug::Print( _L("CMccAsyncLinkCreator::DoCancel iSession") );
        #endif

        TRAP_IGNORE( iSession->CloseLinkL( iClientData().iLinkID ) );
        
        // Null the session in order to avoid looping if this object
        // is deleted inside callback (docancel is re-entried)
        iSession = NULL; 
        
        iObserver.CreationProcessEnd( iClientData, NULL, KErrCancel );
        }
    }

// ---------------------------------------------------------------------------
// From class CActive
// RunL error handling
// ---------------------------------------------------------------------------
//
TInt CMccAsyncLinkCreator::RunError( TInt aError )
    {
    #ifdef TRACE_LINK_CREATOR
        RDebug::Print( _L("CMccAsyncLinkCreator::RunError ERR: %d"), aError );
    #endif
    
    // We have a leave when RunL is executing
    CompleteRequest( aError );

    if ( aError != KErrNoMemory )
        {
        aError = KErrNone;
        }
    
    return aError;
    }

// ---------------------------------------------------------------------------
// Completes the client request with given error code
// ---------------------------------------------------------------------------
//
void CMccAsyncLinkCreator::CompleteRequest( TInt aError )
    {
    #ifdef TRACE_LINK_CREATOR
        RDebug::Print( _L("CMccAsyncLinkCreator::CompleteRequest") );
    #endif

    // Notify controller that we are through with the link creation
    // process so it can delete this link creator. Resolved addresses
    // and ports are also passed to the controller. 
    iObserver.CreationProcessEnd( iClientData, this, aError );
    }

// ---------------------------------------------------------------------------
// CMccAsyncLinkCreator::StartLinkCreationL
// ---------------------------------------------------------------------------
//
void CMccAsyncLinkCreator::StartLinkCreationL( TMMFMessage& aMessage )
    {
    #ifdef TRACE_LINK_CREATOR
        RDebug::Print( _L("CMccAsyncLinkCreator::StartLinkCreationL") );
    #endif

    __ASSERT_ALWAYS( !IsActive(), User::Leave( KErrInUse ) );
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );
     
    aMessage.ReadData1FromClientL( iClientData );

    iSession->CreateLinkL( iClientData().iLinkID, iClientData().iLinkType );
    iSession->InitializeLinkL( iStatus,
                               iClientData().iLinkID,
                               iClientData().iIapId );

    this->SetActive();
    aMessage.WriteDataToClientL( iClientData );
    }
    
    
// ---------------------------------------------------------------------------
// CMccAsyncLinkCreator::GetSessionId
// ---------------------------------------------------------------------------
//

TUint32 CMccAsyncLinkCreator::GetSessionId()
 	{
	return iSession->GetSessionId();
 	}


// ======== GLOBAL FUNCTIONS ========
