/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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




// INCLUDES
#include <es_sock.h>
#include "mccqossockethandler.h"
#include "mccqoslogging.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccQosSocketHandler::CMccQosSocketHandler
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CMccQosSocketHandler::CMccQosSocketHandler( MMccQosEventObserver& aObserver,
    RSubConnection& aSubConnection, RSocket& aSocket ) : 
    CActive( EPriorityStandard ), 
    iPendingAction( ENoPendingAction ), 
    iEventObserver( aObserver ),
    iSubConnection( aSubConnection ),
    iSocket( aSocket )
    {
    CActiveScheduler::Add( this );
    }


// -----------------------------------------------------------------------------
// CMccQosSocketHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMccQosSocketHandler::ConstructL()
    {
    __QOSLOG( "CMccQosSocketHandler::ConstructL" )
    
    iSubConnection.Add( iSocket, iStatus );
    SetActive();
    iPendingAction = EAddingSocket;
    }


// -----------------------------------------------------------------------------
// CMccQosSocketHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMccQosSocketHandler* CMccQosSocketHandler::NewL( 
    MMccQosEventObserver& aObserver,
    RSubConnection& aSubConnection,
    RSocket& aSocket )
    {
    __QOSLOG( "CMccQosSocketHandler::NewL" )

    CMccQosSocketHandler* self = NewLC( aObserver, aSubConnection, aSocket );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CMccQosSocketHandler::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMccQosSocketHandler* CMccQosSocketHandler::NewLC( 
    MMccQosEventObserver& aObserver,
    RSubConnection& aSubConnection,
    RSocket& aSocket )
    {
    __QOSLOG("CMccQosSocketHandler::NewLC")

    CMccQosSocketHandler* self = new ( ELeave ) CMccQosSocketHandler( aObserver,
        aSubConnection, aSocket );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
// CMccQosSocketHandler::~CMccQosSocketHandler
// Destructor
// -----------------------------------------------------------------------------
//
CMccQosSocketHandler::~CMccQosSocketHandler()
    {
    __QOSLOG( "CMccQosSocketHandler::~CMccQosSocketHandler" )
    Cancel();
    }


// -----------------------------------------------------------------------------
// CMccQosSocketHandler::RemoveSocketFromSubConn
// Remove socket from subconnection and associate it with.
// -----------------------------------------------------------------------------
//    
void CMccQosSocketHandler::RemoveSocketFromSubConn()
    {
    __QOSLOG_INT1("CMccQosSocketHandler::RemoveSocketFromSubConn pending: ",
        iPendingAction )
    
    // We cannot remove the socket unless the adding of the socket has
    // finished. Also if adding the socket failed, we should not try to remove
    // the socket as there is not socket to remove
    if ( ENoPendingAction == iPendingAction && KErrNone == iLastError )
        {
        iSubConnection.Remove( iSocket, iStatus );
        SetActive();
        iPendingAction = ERemovingSocket;
        }
    }


// -----------------------------------------------------------------------------
// CMccQosSocketHandler::DoCancel
// Cancel any outstanding operation.
// -----------------------------------------------------------------------------
//
void CMccQosSocketHandler::DoCancel()
    {
    __QOSLOG( "CMccQosSocketHandler::DoCancel" )
    
    RemoveSocketFromSubConn();
    }


// -----------------------------------------------------------------------------
// CMccQosSocketHandler::RunL
// Handle completion of asynchronous requests to RSubConnection
// -----------------------------------------------------------------------------
//
void CMccQosSocketHandler::RunL()
    {
    iLastError = iStatus.Int();
        
    __QOSLOG_INT1("CMccQosSocketHandler::RunL iLastError: ", iLastError )
    __QOSLOG_INT1("CMccQosSocketHandler::RunL iPendingAction: ",
        iPendingAction )
    __QOSLOG_INT1("CMccQosSocketHandler::RunL iAsyncDelete: ", iAsyncDelete )
    
    switch ( iPendingAction )
        {
        case EAddingSocket:
            iPendingAction = ENoPendingAction;
            break;
        case ERemovingSocket:
            iPendingAction = ENoPendingAction;
            break;
        case ENoPendingAction:
        default:
            User::Leave( KErrArgument );
            break;
        }
        
    if ( iAsyncDelete )
        {
        delete this;
        }
    }


// -----------------------------------------------------------------------------
// CMccQosSocketHandler::RunError
// -----------------------------------------------------------------------------
//
TInt CMccQosSocketHandler::RunError( TInt aError )
    {
    __QOSLOG_INT1("CMccQosSocketHandler::RunError aError: ", aError )
    
    if ( KErrNoMemory == aError )
        {
        return aError;
        }
    else
        {
        
        #ifdef _DEBUG

        return aError;
        
        #else // UREL

        return KErrNone;
        
        #endif // _DEBUG

        }
    }


// -----------------------------------------------------------------------------
// CMccQosSocketHandler::AsyncDeleteNeeded
// -----------------------------------------------------------------------------
//
void CMccQosSocketHandler::AsyncDeleteNeeded( TBool aAsyncDelete )
    {
    __QOSLOG_INT1("CMccQosSocketHandler::AsyncDeleteNeeded aAsyncDelete: ",
        aAsyncDelete )
    
    iAsyncDelete = aAsyncDelete;
    }

