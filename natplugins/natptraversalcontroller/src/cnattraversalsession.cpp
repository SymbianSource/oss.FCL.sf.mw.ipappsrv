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


#include "cnattraversalsession.h"
#include "cnattraversalconfig.h"
#include "cnatbinding.h"
#include "cnatbindingstunrefresher.h"
#include "cnatbindingcrlfrefresher.h"
#include "cnatbindingstunandcrlfrefresher.h"
#include "cudpcrlfsender.h"
#include "ctcpcrlfsender.h"
#include "ctlscrlfsender.h"
#include "sipnatbindingobserver.h"
#include "nattraversallog.h"

_LIT8(KZeroIPAddress, "0.0.0.0");

// The following addresses are private (RFC 1918):
// 10.0.0.0 - 10.255.255.255
const TUint32 KPrivateRange1Low  = INET_ADDR(10,0,0,0);
const TUint32 KPrivateRange1High = INET_ADDR(10,255,255,255);

// 172.16.0.0 - 172.31.255.255
const TUint32 KPrivateRange2Low  = INET_ADDR(172,16,0,0);
const TUint32 KPrivateRange2High = INET_ADDR(172,31,255,255);

// 192.168.0.0 - 192.168.255.255
const TUint32 KPrivateRange3Low  = INET_ADDR(192,168,0,0);
const TUint32 KPrivateRange3High = INET_ADDR(192,168,255,255);


// -----------------------------------------------------------------------------
// CNATTraversalSession::NewL
// -----------------------------------------------------------------------------
//
CNATTraversalSession* CNATTraversalSession::NewL(
    CDeltaTimer& aDeltaTimer,
    RSocketServ& aSocketServ,
    RConnection& aConnection,
    const TInetAddr& aLocalAddr,
    const TInetAddr& aNextHopAddr,
    CNATTraversalConfig* aConfig,
    MSTUNClientObserver& aObserver,
    MNATTraversalSocketManager& aSocketManager)
    {
    CNATTraversalSession* self =
        CNATTraversalSession::NewLC(aDeltaTimer,aSocketServ,aConnection,
                                    aLocalAddr,aNextHopAddr,aConfig,
                                    aObserver,aSocketManager);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATTraversalSession::NewL
// -----------------------------------------------------------------------------
//
CNATTraversalSession* CNATTraversalSession::NewLC(
    CDeltaTimer& aDeltaTimer,
    RSocketServ& aSocketServ,
    RConnection& aConnection,
    const TInetAddr& aLocalAddr,
    const TInetAddr& aNextHopAddr,
    CNATTraversalConfig* aConfig,
    MSTUNClientObserver& aObserver,
    MNATTraversalSocketManager& aSocketManager)
    {
    CNATTraversalSession* self =
        new(ELeave)CNATTraversalSession(
            aLocalAddr,aNextHopAddr,aDeltaTimer,aSocketManager);
    CleanupStack::PushL(self);
    self->ConstructL(aSocketServ,aConnection,aConfig,aObserver);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATTraversalSession::CNATTraversalSession
// -----------------------------------------------------------------------------
//
CNATTraversalSession::CNATTraversalSession(
    const TInetAddr& aLocalAddr,
    const TInetAddr& aNextHopAddr,
    CDeltaTimer& aDeltaTimer,
    MNATTraversalSocketManager& aSocketManager)
    : iLocalAddr(aLocalAddr),
      iNextHopAddr(aNextHopAddr),
      iDeltaTimer(aDeltaTimer),
      iSocketManager(aSocketManager)
    {
    }

// -----------------------------------------------------------------------------
// CNATTraversalSession::ConstructL
// -----------------------------------------------------------------------------
//
void CNATTraversalSession::ConstructL(
    RSocketServ& aSocketServ,
    RConnection& aConnection,
    CNATTraversalConfig* aConfig,
    MSTUNClientObserver& aObserver)
    {
    __ASSERT_ALWAYS(aConfig, User::Leave(KErrArgument));

    if (aConfig->STUNServer().Compare(KZeroIPAddress) == 0)
        {
        // With 0.0.0.0 STUN server address, STUN is not used.
        // With this configuration CRLF bursts are sent
        // to keep the possible firewall hole open.
		__ASSERT_ALWAYS(aConfig->CRLFRefreshEnabledToProxyValue() ==
        				CNATTraversalConfig::EEnabled,
                        User::Leave(KErrArgument));
        }
    else
        {
        TBool stunServerConfigured = ETrue;
        TPtrC8 stunServerPtr(aConfig->STUNServer());
        if (stunServerPtr.Length() == 0)
            {
            stunServerPtr.Set(aConfig->Domain());
            stunServerConfigured = EFalse;
            }

        iSTUNClient = CSTUNClient::NewL(aConfig->STUNRetransmitTimer(),
                                        stunServerPtr,
                                        aConfig->STUNServerPort(),
                                        KStun,
                                        aSocketServ,
                                        aConnection,
                                        iDeltaTimer,
                                        aObserver,
                                        ETrue,
                                        !stunServerConfigured);

        if (aConfig->Username().Length() && aConfig->Password().Length())
            {
            iSTUNClient->SetCredentialsL(aConfig->Username(),
                                         aConfig->Password());
            }
        }

    // transfer the ownership only after calling all the leaving functions
    iConfig = aConfig;
    }

// -----------------------------------------------------------------------------
// CNATTraversalSession::~CNATTraversalSession
// -----------------------------------------------------------------------------
//
CNATTraversalSession::~CNATTraversalSession()
    {
    delete iSTUNClient;
    delete iConfig;
    iBindings.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CNATTraversalSession::HasSTUNClient
// -----------------------------------------------------------------------------
//
TBool CNATTraversalSession::HasSTUNClient(const CSTUNClient& aClient) const
    {
    return (iSTUNClient == &aClient);
    }

// -----------------------------------------------------------------------------
// CNATTraversalSession::Domain
// -----------------------------------------------------------------------------
//
const TDesC8& CNATTraversalSession::Domain() const
    {
    return iConfig->Domain();
    }

// -----------------------------------------------------------------------------
// CNATTraversalSession::IapId
// -----------------------------------------------------------------------------
//
TUint32 CNATTraversalSession::IapId() const
    {
    return iConfig->IapId();
    }

// -----------------------------------------------------------------------------
// CNATTraversalSession::UpdateNextHop
// -----------------------------------------------------------------------------
//
void CNATTraversalSession::UpdateNextHop(const TInetAddr& aNextHopAddr)
    {
    iNextHopAddr = aNextHopAddr;
    }

// -----------------------------------------------------------------------------
// CNATTraversalSession::HasBindings
// -----------------------------------------------------------------------------
//
TBool CNATTraversalSession::HasBindings() const
    {
    return (iBindings.Count() > 0);
    }

// -----------------------------------------------------------------------------
// CNATTraversalSession::HasBinding
// -----------------------------------------------------------------------------
//
TBool CNATTraversalSession::HasBinding(
    const MSIPNATBindingObserver* aObserver) const
    {
    return (FindBindingIndex(aObserver) >= 0);
    }

// -----------------------------------------------------------------------------
// CNATTraversalSession::InitCompletedL
// -----------------------------------------------------------------------------
//
void CNATTraversalSession::InitCompletedL()
    {
    NATTRAVERSAL_LOG("CNATTraversalSession::InitCompletedL")
    __ASSERT_ALWAYS(iSTUNClient, User::Leave(KErrCorrupt));
    iConfig->SetSharedSecretNotSupportedL(!iSTUNClient->SharedSecretObtained());
    for (TInt i=0; i<iBindings.Count(); i++)
        {
        CNATBinding* bindingHolder = iBindings[i];
        CSTUNBinding* stunBinding =
            CSTUNBinding::NewL(*iSTUNClient,bindingHolder->STUNSocket());
        bindingHolder->SetBinding(stunBinding);
        bindingHolder->SendSTUNRequestL();
        }
    }

// -----------------------------------------------------------------------------
// CNATTraversalSession::InitFailed
// -----------------------------------------------------------------------------
//
void CNATTraversalSession::InitFailed(TInt aError)
    {
    NATTRAVERSAL_LOG("CNATTraversalSession::InitFailedL")
    TRAP_IGNORE(iConfig->SetBindingRequestFailedL(ETrue))
    for (TInt i=iBindings.Count()-1; i>=0; i--)
        {
        iBindings[i]->SetBindingFailed(aError);
        }
    }

// -----------------------------------------------------------------------------
// CNATTraversalSession::AddressResolvedL
// -----------------------------------------------------------------------------
//
TBool CNATTraversalSession::AddressResolvedL(const CBinding& aBinding)
    {
    NATTRAVERSAL_ADDRLOG("CNATTraversalSession::AddressResolvedL",
        aBinding.PublicAddr())
    TBool found = EFalse;
    for (TInt i=iBindings.Count()-1; i>=0 && !found; i--)
        {
        CNATBinding* bindingHolder = iBindings[i];
        if (bindingHolder->Binding() == &aBinding)
            {
            found = ETrue;
            TBool publicAddrChanged =
                bindingHolder->SetPublicAddr(aBinding.PublicAddr());
            if (publicAddrChanged)
                {
                // Existing public address has changed -> flow failure
                iBindings.Remove(i);
                bindingHolder->NATBindingObserver().FlowFailure(KErrGeneral);
                delete bindingHolder;
                }
            else
                {
                bindingHolder->ContinueRefreshing();
                }
            }
        }
    return found;
    }

// -----------------------------------------------------------------------------
// CNATTraversalSession::BindingFailed
// -----------------------------------------------------------------------------
//
TBool CNATTraversalSession::BindingFailed(
    const CBinding& aBinding,
    TInt aError)
    {
    TBool found = EFalse;
    for (TInt i=iBindings.Count()-1; i>=0 && !found; i--)
        {
        CNATBinding* bindingHolder = iBindings[i];
        if (bindingHolder->Binding() == &aBinding)
            {
            found = ETrue;
            TRAP_IGNORE(iConfig->SetBindingRequestFailedL(ETrue))
            bindingHolder->SetBindingFailed(aError);
            }
        }
    return found;
    }

// -----------------------------------------------------------------------------
// CNATTraversalSession::CreateNATBindingL
// -----------------------------------------------------------------------------
//
CNATBinding& CNATTraversalSession::CreateNATBindingL(
    TUint32 aRequestId,
    RSocket& aSocket,
    MSIPNATBindingObserver& aBindingObserver,
    MSIPNATTraversalRequestObserver& aRequestObserver,
    TBool& aAsyncCallInitiated)
    {
    aAsyncCallInitiated = EFalse;
    // Initialize binding's public address with the local address
    CNATBinding* bindingHolder =
        CNATBinding::NewLC(aRequestId,aSocket,iLocalAddr,
                           aRequestObserver,aBindingObserver,
                           iSocketManager);

    if (iSTUNClient)
        {
        // STUN used, not only CRLF-refreshes.
        if (iSTUNClient->IsInitialized())
            {
            if (!iConfig->BindingRequestFailed())
                {
                CSTUNBinding* stunBinding =
                    CSTUNBinding::NewL(
                        *iSTUNClient,bindingHolder->STUNSocket());
                bindingHolder->SetBinding(stunBinding);
                bindingHolder->SendSTUNRequestL();
                aAsyncCallInitiated = ETrue;
                }
            }
        else
            {
            // STUN Client is not initialized.
            aAsyncCallInitiated = (!iConfig->BindingRequestFailed());
            }
        }

    iBindings.AppendL(bindingHolder);
    CleanupStack::Pop(bindingHolder);
    return *bindingHolder;
    }

// -----------------------------------------------------------------------------
// CNATTraversalSession::RefreshNATBindingL
// -----------------------------------------------------------------------------
//
void CNATTraversalSession::RefreshNATBindingL(
    RSocket& aSocket,
    const MSIPNATBindingObserver* aBindingObserver,
    TBool& aMaintainPersistentConnection,
    TBool& aHandled)
    {
    aMaintainPersistentConnection = EFalse;
    aHandled = EFalse;
    TInt index = FindBindingIndex(aBindingObserver);
    if (index >= 0)
        {
        CNATBinding* binding = iBindings[index];
        if (!binding->Refresher() &&
            (IsBehindNAT(*binding) ||
             iConfig->CRLFRefreshEnabledToProxy(Protocol(aSocket))))
            {
            CreateRefresherL(*binding,aSocket,aMaintainPersistentConnection);
            }
        aMaintainPersistentConnection = binding->PersistentConnectionRequired();
        aHandled = ETrue;
        }
    }

// -----------------------------------------------------------------------------
// CNATTraversalSession::RefreshNATBindingL
// -----------------------------------------------------------------------------
//
void CNATTraversalSession::RefreshNATBindingL(
    CSecureSocket& aSecureSocket,
    const MSIPNATBindingObserver* aBindingObserver,
    TBool& aMaintainPersistentConnection,
    TBool& aHandled)
    {
    aMaintainPersistentConnection = EFalse;
    aHandled = EFalse;
    TInt index = FindBindingIndex(aBindingObserver);
    if (index >= 0)
        {
        CNATBinding* binding = iBindings[index];
        if (!binding->Refresher() &&
			(IsBehindNAT(*binding) ||
			 iConfig->CRLFRefreshEnabledToProxy(KProtocolInetTcp)))
            {
            CTlsCRLFSender* sender =
                new(ELeave)CTlsCRLFSender(aSecureSocket,*binding);
            CleanupStack::PushL(sender);
            CNATBindingRefresher* refresher =
                new(ELeave)CNATBindingCRLFRefresher(
                    iDeltaTimer,
                    iConfig->TCPRefreshInterval(),
                    *binding,
                    sender);
            CleanupStack::Pop(sender);
            binding->SetRefresher(refresher);
            }
        aMaintainPersistentConnection = binding->PersistentConnectionRequired();
        aHandled = ETrue;
        }
    }

// -----------------------------------------------------------------------------
// CNATTraversalSession::SocketIdle
// -----------------------------------------------------------------------------
//
void CNATTraversalSession::SocketIdle(
    TBool aIdle,
    RSocket& aSocket)
    {
    SocketIdleImpl(aIdle,aSocket);
    }

// -----------------------------------------------------------------------------
// CNATTraversalSession::SocketIdle
// -----------------------------------------------------------------------------
//
void CNATTraversalSession::SocketIdle(
    TBool aIdle,
    CSecureSocket& aSocket)
    {
    SocketIdleImpl(aIdle,aSocket);
    }

// -----------------------------------------------------------------------------
// CNATTraversalSession::DataReceivedL
// -----------------------------------------------------------------------------
//
void CNATTraversalSession::DataReceivedL(
    const TDesC8& aData,
    const RSocket& aSocket,
    TBool& aHandled)
    {
    for (TInt i=0; i<iBindings.Count() && !aHandled; i++)
        {
        iBindings[i]->HandleDataL(aData,aSocket,aHandled);
        }
    }

// -----------------------------------------------------------------------------
// CNATTraversalSession::RemoveBinding
// -----------------------------------------------------------------------------
//
TBool CNATTraversalSession::RemoveBinding(TUint32 aRequestId)
    {
    TBool found = EFalse;
    for (TInt i=0; i<iBindings.Count() && !found; i++)
        {
        CNATBinding* binding = iBindings[i];
        if (binding->InitialRequestId() == aRequestId)
            {
            iBindings.Remove(i);
            delete binding;
            found = ETrue;
            }
        }
    return found;
    }

// -----------------------------------------------------------------------------
// CNATTraversalSession::RemoveBinding
// -----------------------------------------------------------------------------
//
TBool CNATTraversalSession::RemoveBinding(
    const MSIPNATBindingObserver& aObserver)
    {
    TBool found = EFalse;
    TInt index = FindBindingIndex(&aObserver);
    if (index >= 0)
        {
        CNATBinding* binding = iBindings[index];
        iBindings.Remove(index);
        delete binding;
        found = ETrue;
        }
    return found;
    }

// -----------------------------------------------------------------------------
// CNATTraversalSession::DetachConfig
// -----------------------------------------------------------------------------
//
void CNATTraversalSession::DetachConfig()
    {
    iConfig = NULL;
    }

// -----------------------------------------------------------------------------
// CNATTraversalSession::Protocol
// -----------------------------------------------------------------------------
//
TUint CNATTraversalSession::Protocol(RSocket& aSocket) const
    {
    TProtocolDesc proto;
    aSocket.Info(proto);
    return proto.iProtocol;
    }

// -----------------------------------------------------------------------------
// CNATTraversalSession::RefreshInterval
// -----------------------------------------------------------------------------
//
TInt CNATTraversalSession::RefreshInterval(TUint aProtocol) const
    {
    TInt interval = 0;
    if (aProtocol == KProtocolInetTcp)
        {
        interval = iConfig->TCPRefreshInterval();
        }
    if (aProtocol == KProtocolInetUdp)
        {
        interval = iConfig->UDPRefreshInterval();
        }
    return interval;
    }

// -----------------------------------------------------------------------------
// CNATTraversalSession::FindBindingIndex
// -----------------------------------------------------------------------------
//
TInt CNATTraversalSession::FindBindingIndex(
    const MSIPNATBindingObserver* aBindingObserver) const
    {
    TInt index = KErrNotFound;
    for (TInt i=0; i < iBindings.Count() && index == KErrNotFound; i++)
        {
        CNATBinding* binding = iBindings[i];
        if (&(binding->NATBindingObserver()) == aBindingObserver)
            {
            index = i;
            }
        }
    return index;
    }

// -----------------------------------------------------------------------------
// CNATTraversalSession::CreateRefresherL
// -----------------------------------------------------------------------------
//
void CNATTraversalSession::CreateRefresherL(
    CNATBinding& aBinding,
    RSocket& aSocket,
    TBool& aMaintainPersistentConnection)
    {
    aMaintainPersistentConnection = EFalse;
    CNATBindingRefresher* refresher = NULL;
    TUint protocol = Protocol(aSocket);
    TInt refreshInterval = RefreshInterval(protocol);
    if (protocol == KProtocolInetTcp)
        {
        CTcpCRLFSender* sender =
            new(ELeave)CTcpCRLFSender(aSocket,aBinding);
        CleanupStack::PushL(sender);
        refresher = new(ELeave)CNATBindingCRLFRefresher(iDeltaTimer,
                                                        refreshInterval,
                                                        aBinding,
                                                        sender);
        CleanupStack::Pop(sender);
        aMaintainPersistentConnection = ETrue;
        }
    else if (protocol == KProtocolInetUdp)
        {
        if (IsBehindNAT(aBinding) && aBinding.AddressResolved())
            {
            // STUN has been succesfully used to detect a NAT
            if (iSTUNClient &&
                iNextHopAddr.Match(iSTUNClient->STUNServerAddrL()))
                {
                // Next hop has a STUN server -> refresh using STUN
                refresher =
                    new(ELeave)CNATBindingSTUNRefresher(iDeltaTimer,
                                                        refreshInterval,
                                                        aBinding,
                                                        aBinding);
                }
            else
                {
                // Refresh using STUN towards STUN server and
                // using CRLF towards the SIP proxy
                refresher = CNATBindingSTUNAndCRLFRefresher::NewL(
                    iDeltaTimer,refreshInterval,aBinding,
                    aBinding,aSocket,iNextHopAddr,iSocketManager);
                }
            }
        else
            {
            if (iConfig->CRLFRefreshEnabledToProxy(protocol))
                {
                CUdpCRLFSender* sender =
                    new(ELeave)CUdpCRLFSender(
                        aSocket,iNextHopAddr,aBinding,iSocketManager);
                CleanupStack::PushL(sender);
                refresher =
                    new(ELeave)CNATBindingCRLFRefresher(
                        iDeltaTimer,refreshInterval,aBinding,sender);
                CleanupStack::Pop(sender);
                }
            }
        }
    else
        {
        User::Leave(KErrNotSupported);
        }
    aBinding.SetRefresher(refresher);
    }

// -----------------------------------------------------------------------------
// CNATTraversalSession::SocketIdleImpl
// -----------------------------------------------------------------------------
//
template<class T> void CNATTraversalSession::SocketIdleImpl(
    TBool aIdle,
    T& aSocket)
    {
    for (TInt i=0; i<iBindings.Count(); i++)
        {
        CNATBindingRefresher* bindingRefresher = iBindings[i]->Refresher();
        if (bindingRefresher && bindingRefresher->HasSocket(aSocket))
            {
            bindingRefresher->SetRefresh(aIdle);
            }
        }
    }

// -----------------------------------------------------------------------------
// CNATTraversalSession::IsBehindNAT
// -----------------------------------------------------------------------------
//
TBool CNATTraversalSession::IsBehindNAT(const CNATBinding& aBinding) const
    {
    TBool natDetected = EFalse;
    if (aBinding.AddressResolved())
        {
        if (!aBinding.PublicAddr().Match(iLocalAddr))
            {
            natDetected = ETrue;
            }
        }
    else
        {
        if (IsPrivateAddress(iLocalAddr))
            {
            natDetected = ETrue;
            }
        }
    return natDetected;
    }

// -----------------------------------------------------------------------------
// CNATTraversalSession::IsPrivateAddress
// -----------------------------------------------------------------------------
//
TBool CNATTraversalSession::IsPrivateAddress(const TInetAddr& aAddr) const
    {
    TBool isPrivate = EFalse;
    if (aAddr.Family() == KAfInet)
        {
        // aAddr is IPv4 address, IPv6 addresses are always public
        TUint32 addr = aAddr.Address();
        if ((addr >= KPrivateRange1Low && addr <= KPrivateRange1High) ||
            (addr >= KPrivateRange2Low && addr <= KPrivateRange2High) ||
            (addr >= KPrivateRange3Low && addr <= KPrivateRange3High))
                {
                isPrivate = ETrue;
                }
        }
    return isPrivate;
    }

