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
* Description: 
*
*/


#include "cnattraversalcontroller.h"
#include "sipnattraversalcontrollerinitparams.h"
#include "sipnatbindingobserver.h"
#include "sipnattraversalrequestobserver.h"
#include "cnattraversalconfig.h"
#include "cnattraversalsession.h"
#include "nattraversalsocketuser.h"
#include "nattraversallog.h"
#include <siperr.h>

_LIT8(KZeroIPAddress, "0.0.0.0");

// -----------------------------------------------------------------------------
// CNATTraversalController::NewL
// -----------------------------------------------------------------------------
//
CNATTraversalController* CNATTraversalController::NewL(TAny* aInitParams)
    {
    __ASSERT_ALWAYS (aInitParams, User::Leave(KErrArgument));
    TSIPNATTraversalControllerInitParams* params =
        static_cast<TSIPNATTraversalControllerInitParams*>(aInitParams);
    CNATTraversalController* self =
        new(ELeave)CNATTraversalController(params->iSocketServ);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATTraversalController::CNATTraversalController
// -----------------------------------------------------------------------------
//
CNATTraversalController::CNATTraversalController(RSocketServ& aSocketServ)
 : iSocketServ(aSocketServ),
   iRequestOrder(TNATTraversalPendingRequest::RequestOrder)
    {
    }

// -----------------------------------------------------------------------------
// CNATTraversalController::ConstructL
// -----------------------------------------------------------------------------
//
void CNATTraversalController::ConstructL()
    {
    iDeltaTimer = CDeltaTimer::NewL(CActive::EPriorityStandard);
    }

// -----------------------------------------------------------------------------
// CNATTraversalController::~CNATTraversalController
// -----------------------------------------------------------------------------
//
CNATTraversalController::~CNATTraversalController()
    {
    iSessions.ResetAndDestroy();
    for (TInt i=0; i < iPendingRequests.Count(); i++)
        {
        if (iDeltaTimer)
            {
            iDeltaTimer->Remove(iPendingRequests[i].DeltaTimerEntry());
            }
        }
    iPendingRequests.Close();
    delete iDeltaTimer;
    iSocketUserQueue.Close();
    }

// -----------------------------------------------------------------------------
// CNATTraversalController::ResolvePublicAddrL
// From CSIPNATTraversalController
// -----------------------------------------------------------------------------
//
TUint32 CNATTraversalController::ResolvePublicAddrL(
    TUint32 aIapId,
    RConnection& aConnection,
    const TInetAddr& aLocalAddr,
    const TDesC8& aDomain,
    RSocket& aUdpSocket,
    const TInetAddr& aNextHopAddr,
    MSIPNATBindingObserver* aBindingObserver,
    MSIPNATTraversalRequestObserver& aRequestObserver)
    {
    __ASSERT_ALWAYS(NULL != aBindingObserver,User::Leave(KErrArgument));
    TUint32 requestId = NextRequestId();
    TBool asyncCallInitiated = EFalse;

    if (!BindingExists(aBindingObserver))
        {
        CNATTraversalConfig* config = CNATTraversalConfig::NewLC(aIapId,aDomain);

        CreateBindingL(requestId,config,aConnection,aLocalAddr,aUdpSocket,
                       aNextHopAddr,*aBindingObserver,aRequestObserver,
                       asyncCallInitiated);

        CleanupStack::Pop(config); // ownership transferred
        }

    if (!asyncCallInitiated)
        {
        // Start a timer with a minimum value to make the call asynchronous
        TNATTraversalPendingRequest request(requestId,&aRequestObserver,*this);
        iPendingRequests.InsertInOrderL(request,iRequestOrder);
        TInt index = iPendingRequests.FindInOrder(request,iRequestOrder);
        if (0 <= index && iPendingRequests.Count() > index)
            {
            iDeltaTimer->Queue(1,iPendingRequests[index].DeltaTimerEntry());
            }
        }

    return requestId;
    }

// -----------------------------------------------------------------------------
// CNATTraversalController::Cancel
// From CSIPNATTraversalController
// -----------------------------------------------------------------------------
//
void CNATTraversalController::Cancel(TUint32 aRequestId)
    {
    NATTRAVERSAL_LOG("CNATTraversalController::Cancel")
    TNATTraversalPendingRequest tmp(aRequestId,0,*this);
    TInt index = iPendingRequests.FindInOrder(tmp,iRequestOrder);
    if (0 <= index && iPendingRequests.Count() > index)
        {
        iDeltaTimer->Remove(iPendingRequests[index].DeltaTimerEntry());
        iPendingRequests.Remove(index);
        }
    else
        {
        TBool found = EFalse;
        for (TInt i=0; i < iSessions.Count() && !found; i++)
            {
            found = iSessions[i]->RemoveBinding(aRequestId);
            }
        }
    }

// -----------------------------------------------------------------------------
// CNATTraversalController::RefreshNATBindingL
// From CSIPNATTraversalController
// -----------------------------------------------------------------------------
//
TBool CNATTraversalController::RefreshNATBindingL(
    RSocket& aSocket,
    const MSIPNATBindingObserver* aBindingObserver)
    {
    NATTRAVERSAL_LOG("CNATTraversalController::RefreshNATBindingL - TCP/UDP")
    TBool maintainPersistentConnection = EFalse;
    TBool handled = EFalse;
    for (TInt i=0; i < iSessions.Count() && !handled; i++)
        {
        iSessions[i]->RefreshNATBindingL(
            aSocket,aBindingObserver,
            maintainPersistentConnection,handled);
        }
    return maintainPersistentConnection;
    }

// -----------------------------------------------------------------------------
// CNATTraversalController::RefreshNATBindingL
// From CSIPNATTraversalController
// -----------------------------------------------------------------------------
//
TBool CNATTraversalController::RefreshNATBindingL(
    CSecureSocket& aSecureSocket,
    const MSIPNATBindingObserver* aBindingObserver)
    {
    NATTRAVERSAL_LOG("CNATTraversalController::RefreshNATBindingL - TLS")
    TBool maintainPersistentConnection = EFalse;
    TBool handled = EFalse;
    for (TInt i=0; i < iSessions.Count() && !handled; i++)
        {
        iSessions[i]->RefreshNATBindingL(
            aSecureSocket,aBindingObserver,
            maintainPersistentConnection,handled);
        }
    return maintainPersistentConnection;
    }

// -----------------------------------------------------------------------------
// CNATTraversalController::UpdateNextHop
// -----------------------------------------------------------------------------
//
void CNATTraversalController::UpdateNextHop(
    const TInetAddr& aNextHopAddr,
    const MSIPNATBindingObserver* aBindingObserver)
    {
    NATTRAVERSAL_ADDRLOG("CNATTraversalController::UpdateNextHop ->",
        aNextHopAddr)
    for (TInt i=0; i < iSessions.Count(); i++)
        {
        CNATTraversalSession* session = iSessions[i];
        if (session->HasBinding(aBindingObserver))
            {
            session->UpdateNextHop(aNextHopAddr);
            return;
            }
        }
    }

// -----------------------------------------------------------------------------
// CNATTraversalController::SocketIdle
// From CSIPNATTraversalController
// -----------------------------------------------------------------------------
//
void CNATTraversalController::SocketIdle(
    TBool aIdle,
    RSocket& aSocket)
    {
    NATTRAVERSAL_INTLOG("CNATTraversalController::SocketIdle - TCP/UDP", aIdle)
    for (TInt i=0; i < iSessions.Count(); i++)
        {
        iSessions[i]->SocketIdle(aIdle,aSocket);
        }
    if (!aIdle)
        {
        TBool firstRemoved = EFalse;
        for (TInt i=iSocketUserQueue.Count()-1; i >= 0; i--)
            {
            const RSocket& socket = iSocketUserQueue[i]->Socket();
            if (socket.SubSessionHandle() == aSocket.SubSessionHandle())
                {
                iSocketUserQueue[i]->CancelSending();
                iSocketUserQueue.Remove(i);
                firstRemoved = (i == 0);
                }
            }
        if (firstRemoved)
            {
            HandleNextFromSocketUserQueue();
            }
        }
    }

// -----------------------------------------------------------------------------
// CNATTraversalController::SocketIdle
// From CSIPNATTraversalController
// -----------------------------------------------------------------------------
//
void CNATTraversalController::SocketIdle(
    TBool aIdle,
    CSecureSocket& aSecureSocket)
    {
    NATTRAVERSAL_INTLOG("CNATTraversalController::SocketIdle - TLS", aIdle)
    for (TInt i=0; i < iSessions.Count(); i++)
        {
        iSessions[i]->SocketIdle(aIdle,aSecureSocket);
        }
    }

// -----------------------------------------------------------------------------
// CNATTraversalController::DataReceived
// From CSIPNATTraversalController
// -----------------------------------------------------------------------------
//
void CNATTraversalController::DataReceivedL(
    const TDesC8& aData,
    const RSocket& aSocket,
    TBool& aHandled)
    {
    NATTRAVERSAL_LOG("CNATTraversalController::DataReceivedL");
    aHandled = EFalse;
    for (TInt i=0; i < iSessions.Count() && !aHandled; i++)
        {
        iSessions[i]->DataReceivedL(aData,aSocket,aHandled);
        }
    }

// -----------------------------------------------------------------------------
// CNATTraversalController::FreeResources
// From CSIPNATTraversalController
// -----------------------------------------------------------------------------
//
void CNATTraversalController::FreeResources(TUint32 aIapId)
    {
    NATTRAVERSAL_LOG("CNATTraversalController::FreeResources for IAP")
    for (TInt i = iSessions.Count()-1; i >= 0; i--)
        {
        CNATTraversalSession* session = iSessions[i];
        if (session->IapId() == aIapId)
            {
            iSessions.Remove(i);
            delete session;
            }
        }
    }

// -----------------------------------------------------------------------------
// CNATTraversalController::FreeResources
// From CSIPNATTraversalController
// -----------------------------------------------------------------------------
//
void CNATTraversalController::FreeResources(
    MSIPNATBindingObserver& aBindingObserver)
    {
    NATTRAVERSAL_LOG("CNATTraversalController::FreeResources for observer")
    for (TInt i=iSessions.Count()-1; i>=0; i--)
        {
        CNATTraversalSession* session = iSessions[i];
        session->RemoveBinding(aBindingObserver);
        if (!session->HasBindings())
            {
            iSessions.Remove(i);
            delete session;
            }
        }
    }

// -----------------------------------------------------------------------------
// CNATTraversalController::RequestCompleted
// From MNATTraversalPendingRequestContainer
// -----------------------------------------------------------------------------
//
void CNATTraversalController::RequestCompleted(
    TUint32 aRequestId)
    {
    NATTRAVERSAL_LOG("CNATTraversalController::RequestCompleted")
    TNATTraversalPendingRequest tmp(aRequestId,0,*this);
    TInt index = iPendingRequests.FindInOrder(tmp,iRequestOrder);
    if (0 <= index && iPendingRequests.Count() > index)
        {
        TNATTraversalPendingRequest request = iPendingRequests[index]; // copy
        iDeltaTimer->Remove(iPendingRequests[index].DeltaTimerEntry());
        iPendingRequests.Remove(index); // remove from array
        request.CompleteRequest(); // use copy for informing the observer
        }
    }

// -----------------------------------------------------------------------------
// CNATTraversalController::STUNClientInitCompleted
// From MSTUNClientObserver
// -----------------------------------------------------------------------------
//
void CNATTraversalController::STUNClientInitCompleted(
    const CSTUNClient& aClient,
    TInt aCompletionCode)
    {
    TBool found = EFalse;
    for (TInt i=0; i<iSessions.Count() && !found; i++)
        {
        CNATTraversalSession* session = iSessions[i];
        if (session->HasSTUNClient(aClient))
            {
            found = ETrue;
            if (aCompletionCode != KErrNone)
                {
                if (KErrNATFWDnsFailure == aCompletionCode)
                    {
                    aCompletionCode = KErrSIPResolvingFailure;
                    }
                session->InitFailed(aCompletionCode);
                }
            else
                {
                TRAPD(err,session->InitCompletedL());
                if (err)
                    {
                    session->InitFailed(err);
                    }
                }
            }
        }
    }

// -----------------------------------------------------------------------------
// CNATTraversalController::STUNBindingEventOccurredL
// From MSTUNClientObserver
// -----------------------------------------------------------------------------
//
void CNATTraversalController::STUNBindingEventOccurredL(
    TSTUNBindingEvent aEvent,
    const CBinding& aBinding)
    {
    if (aEvent == MSTUNClientObserver::EPublicAddressResolved)
        {
        TBool found = EFalse;
        for (TInt i=0; i<iSessions.Count() && !found; i++)
            {
            CNATTraversalSession* session = iSessions[i];
            found = session->AddressResolvedL(aBinding);
            }
        }
    }

// -----------------------------------------------------------------------------
// CNATTraversalController::STUNBindingErrorOccurred
// From MSTUNClientObserver
// -----------------------------------------------------------------------------
//
void CNATTraversalController::STUNBindingErrorOccurred(
    const CBinding& aBinding,
    TInt aError)
    {
    TBool found = EFalse;
    for (TInt i=0; i<iSessions.Count() && !found; i++)
        {
        CNATTraversalSession* session = iSessions[i];
        found = session->BindingFailed(aBinding,aError);
        }
    }

// -----------------------------------------------------------------------------
// CNATTraversalController::AddToSendingQueueL
// From MNATTraversalSocketManager
// -----------------------------------------------------------------------------
//
TBool CNATTraversalController::AddToSendingQueueL(
    MNATTraversalSocketUser& aUser)
    {
    TBool requestPendingForSocket = EFalse;
    for (TInt i=0; i < iSocketUserQueue.Count(); i++)
        {
        const RSocket& socket = iSocketUserQueue[i]->Socket();
        if (socket.SubSessionHandle() == aUser.Socket().SubSessionHandle())
            {
            requestPendingForSocket = ETrue;
            }
        }
    iSocketUserQueue.AppendL(&aUser);
    return requestPendingForSocket;
    }

// -----------------------------------------------------------------------------
// CNATTraversalController::SendingCompleted
// From MNATTraversalSocketManager
// -----------------------------------------------------------------------------
//
void CNATTraversalController::SendingCompleted(MNATTraversalSocketUser& aUser)
    {
    TInt index = iSocketUserQueue.Find(&aUser);
    if (index >= 0)
        {
        iSocketUserQueue.Remove(index);
        if (index == 0)
            {
            HandleNextFromSocketUserQueue();
            }
        }
    }

// -----------------------------------------------------------------------------
// CNATTraversalController::NextRequestId
// -----------------------------------------------------------------------------
//
TUint32 CNATTraversalController::NextRequestId()
    {
    if (iRequestIdCounter == KMaxTUint32)
        {
        iRequestIdCounter = 1;
        }
    else
        {
        iRequestIdCounter++;
        }
    return iRequestIdCounter;
    }

// -----------------------------------------------------------------------------
// CNATTraversalController::CreateBindingL
// -----------------------------------------------------------------------------
//
void CNATTraversalController::CreateBindingL(
    TUint32 aRequestId,
    CNATTraversalConfig* aConfig,
    RConnection& aConnection,
    const TInetAddr& aLocalAddr,
    RSocket& aUdpSocket,
    const TInetAddr& aNextHopAddr,
    MSIPNATBindingObserver& aBindingObserver,
    MSIPNATTraversalRequestObserver& aRequestObserver,
    TBool& aAsyncInitiated)
    {
    __ASSERT_ALWAYS(aConfig, User::Leave(KErrArgument));

    __ASSERT_ALWAYS(!BindingExists(&aBindingObserver),
                    User::Leave(KErrAlreadyExists));

    TBool useSTUN = (aConfig->STUNServer().Compare(KZeroIPAddress) != 0);
    if (!useSTUN)
        {
        aAsyncInitiated = EFalse;
        if ( aConfig->CRLFRefreshEnabledToProxyValue() !=
        	 CNATTraversalConfig::EEnabled )
            {
            // No need to create binding nor a dummy CRLF refresh
            delete aConfig;
            return;
            }
        }

    TInt index = FindSessionIndex(aConfig->IapId(),aConfig->Domain());
    if (index >= 0)
        {
        // Session exists
        CNATTraversalSession* session = iSessions[index];
        session->CreateNATBindingL(aRequestId,aUdpSocket,
                                   aBindingObserver,aRequestObserver,
                                   aAsyncInitiated);
        delete aConfig; // ownership transferred after leaving functions
        }
    else
        {
        // Create new session and add a binding
        CNATTraversalSession* session =
            CNATTraversalSession::NewLC(*iDeltaTimer,iSocketServ,aConnection,
                                        aLocalAddr,aNextHopAddr,aConfig,
                                        *this,*this);
        TCleanupItem cleanupItem(DetachConfigFromSession,session);
        CleanupStack::PushL(cleanupItem);
        session->CreateNATBindingL(aRequestId,aUdpSocket,
                                   aBindingObserver,aRequestObserver,
                                   aAsyncInitiated);
        iSessions.AppendL(session);
        CleanupStack::Pop(); // cleanupItem
        CleanupStack::Pop(session);
        }
    }

// -----------------------------------------------------------------------------
// CNATTraversalController::FindSessionIndex
// -----------------------------------------------------------------------------
//
TInt CNATTraversalController::FindSessionIndex(
    TUint32 aIapId,
    const TDesC8& aDomain) const
    {
    for (TInt i=0; i < iSessions.Count(); i++)
        {
        CNATTraversalSession* session = iSessions[i];
        if (session->IapId() == aIapId &&
            session->Domain().CompareF(aDomain) == 0)
            {
            return i;
            }
        }
    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CNATTraversalController::BindingExists
// -----------------------------------------------------------------------------
//
TBool CNATTraversalController::BindingExists(
    const MSIPNATBindingObserver* aObserver) const
    {
    for (TInt i=0; i < iSessions.Count(); i++)
        {
        if (iSessions[i]->HasBinding(aObserver))
            {
            return ETrue;
            }
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CNATTraversalController::DetachConfigFromSession
// -----------------------------------------------------------------------------
//
void CNATTraversalController::DetachConfigFromSession(TAny* aSession)
    {
    CNATTraversalSession* session =
        reinterpret_cast<CNATTraversalSession*>(aSession);
    if (session)
        {
        session->DetachConfig();
        }
    }

// -----------------------------------------------------------------------------
// CNATTraversalController::HandleNextFromSocketUserQueue
// -----------------------------------------------------------------------------
//
void CNATTraversalController::HandleNextFromSocketUserQueue()
    {
    if (iSocketUserQueue.Count() > 0 && !iSocketUserQueue[0]->IsSending())
        {
        iSocketUserQueue[0]->SendingAllowed();
        }
    }
