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


#include "cnattraversalconfig.h"
#include "nattraversallog.h"
#include <unsafprotocolscrkeys.h>
#include <e32svr.h>
#include <in_sock.h>

const TInt KDefaultUDPRefreshInterval = 28;
const TInt KDefaultTCPRefreshInterval = 1200;
const TInt KDefaultSTUNRetransmitTimer = 250; // milliseconds


// -----------------------------------------------------------------------------
// CNATTraversalConfig::NewL
// -----------------------------------------------------------------------------
//
CNATTraversalConfig* CNATTraversalConfig::NewL(
    TUint32 aIapId,
    const TDesC8& aDomain)
    {
    CNATTraversalConfig* self = CNATTraversalConfig::NewLC(aIapId,aDomain);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATTraversalConfig::NewLC
// -----------------------------------------------------------------------------
//
CNATTraversalConfig* CNATTraversalConfig::NewLC(
    TUint32 aIapId,
    const TDesC8& aDomain)
    {
    CNATTraversalConfig* self = new(ELeave)CNATTraversalConfig(aIapId);
    CleanupStack::PushL(self);
    self->ConstructL(aDomain);
    return self;
    }

// -----------------------------------------------------------------------------
// CNATTraversalConfig::CNATTraversalConfig
// -----------------------------------------------------------------------------
//
CNATTraversalConfig::CNATTraversalConfig(TUint32 aIapId)
 : iIapId(aIapId),
   iUDPRefreshInterval(KDefaultUDPRefreshInterval),
   iTCPRefreshInterval(KDefaultTCPRefreshInterval),
   iSTUNRetransmitTimer(KDefaultSTUNRetransmitTimer),
   iCRLFRefreshEnabled(EUndefined),
   iSharedSecretNotSupported(EFalse),
   iBindingRequestFailed(EFalse)
    {
    }

// -----------------------------------------------------------------------------
// CNATTraversalConfig::ConstructL
// -----------------------------------------------------------------------------
//
void CNATTraversalConfig::ConstructL(const TDesC8& aDomain)
    {
    __ASSERT_ALWAYS(aDomain.Length() > 0, User::Leave(KErrArgument));

    iCenRep = CRepository::NewL(KCRUidUNSAFProtocols);

    // Read domain settings first.
    // This way IAP specific timers override the domain specific values.
    ReadDomainSettingsL(*iCenRep,aDomain);
    ReadIapSettingsL(*iCenRep,iIapId);

    iDomain = aDomain.AllocL();

    NATTRAVERSAL_INTLOG("CNATTraversalConfig::IAP",iIapId)
    NATTRAVERSAL_STRLOG("CNATTraversalConfig::Domain",*iDomain)
    NATTRAVERSAL_STRLOG("CNATTraversalConfig::STUNServer",*iSTUNServer)
    NATTRAVERSAL_STRLOG("CNATTraversalConfig::STUNUsername",*iUsername)
    NATTRAVERSAL_STRLOG("CNATTraversalConfig::STUNPassword",*iPassword)
    NATTRAVERSAL_INTLOG("CNATTraversalConfig::STUNServerPort",iSTUNServerPort)
    NATTRAVERSAL_INTLOG("CNATTraversalConfig::UDPRefresh",iUDPRefreshInterval)
    NATTRAVERSAL_INTLOG("CNATTraversalConfig::TCPRefresh",iTCPRefreshInterval)
    NATTRAVERSAL_INTLOG("CNATTraversalConfig::STUNRetransmit",iSTUNRetransmitTimer)
    NATTRAVERSAL_INTLOG("CNATTraversalConfig::CRLFRefresh",iCRLFRefreshEnabled)
    NATTRAVERSAL_INTLOG("CNATTraversalConfig::LastBindingReqFailed",iBindingRequestFailed)
    }

// -----------------------------------------------------------------------------
// CNATTraversalConfig::~CNATTraversalConfig
// -----------------------------------------------------------------------------
//
CNATTraversalConfig::~CNATTraversalConfig()
    {
    delete iSTUNServer;
    delete iDomain;
    delete iUsername;
    delete iPassword;
    if (iDomainKey)
        {
        // Remove settings added during runtime
        if (iCenRep)
            {
            iCenRep->Delete(
                KUNSAFProtocolsDomainSharedSecretNotSupported|iDomainKey);
            iCenRep->Delete(
                KUNSAFProtocolsDomainBindingRequestFailed|iDomainKey);
            }
        }
    delete iCenRep;
    }

// -----------------------------------------------------------------------------
// CNATTraversalConfig::IapId
// -----------------------------------------------------------------------------
//
TUint32 CNATTraversalConfig::IapId() const
    {
    return iIapId;
    }

// -----------------------------------------------------------------------------
// CNATTraversalConfig::Domain
// -----------------------------------------------------------------------------
//
const TDesC8& CNATTraversalConfig::Domain() const
    {
    return *iDomain;
    }

// -----------------------------------------------------------------------------
// CNATTraversalConfig::STUNServer
// -----------------------------------------------------------------------------
//
const TDesC8& CNATTraversalConfig::STUNServer() const
    {
    return *iSTUNServer;
    }

// -----------------------------------------------------------------------------
// CNATTraversalConfig::STUNServerPort
// -----------------------------------------------------------------------------
//
TUint CNATTraversalConfig::STUNServerPort() const
    {
    return iSTUNServerPort;
    }

// -----------------------------------------------------------------------------
// CNATTraversalConfig::UDPRefreshInterval
// -----------------------------------------------------------------------------
//
TInt CNATTraversalConfig::UDPRefreshInterval() const
    {
    return iUDPRefreshInterval;
    }

// -----------------------------------------------------------------------------
// CNATTraversalConfig::TCPRefreshInterval
// -----------------------------------------------------------------------------
//
TInt CNATTraversalConfig::TCPRefreshInterval() const
    {
    return iTCPRefreshInterval;
    }

// -----------------------------------------------------------------------------
// CNATTraversalConfig::STUNRetransmitTimer
// -----------------------------------------------------------------------------
//
TInt CNATTraversalConfig::STUNRetransmitTimer() const
    {
    return iSTUNRetransmitTimer;
    }

// -----------------------------------------------------------------------------
// CNATTraversalConfig::CRLFRefreshEnabledToProxy
// If CRLF refresh is not specified in central repository, the default value is
// "enabled" for TCP and "disabled" for UDP.
// -----------------------------------------------------------------------------
//
TBool CNATTraversalConfig::CRLFRefreshEnabledToProxy(TUint aProtocol) const
	{
	if ( aProtocol == KProtocolInetTcp )
		{
		return iCRLFRefreshEnabled != EDisabled;
		}

	return iCRLFRefreshEnabled == EEnabled;
	}

// -----------------------------------------------------------------------------
// CNATTraversalConfig::CRLFRefreshEnabledToProxyValue
// -----------------------------------------------------------------------------
//
CNATTraversalConfig::TRefreshValue
CNATTraversalConfig::CRLFRefreshEnabledToProxyValue() const
    {
    return iCRLFRefreshEnabled;
    }

// -----------------------------------------------------------------------------
// CNATTraversalConfig::SharedSecretNotSupported
// -----------------------------------------------------------------------------
//
TBool CNATTraversalConfig::SharedSecretNotSupported() const
    {
    return iSharedSecretNotSupported;
    }

// -----------------------------------------------------------------------------
// CNATTraversalConfig::Username
// -----------------------------------------------------------------------------
//
const TDesC8& CNATTraversalConfig::Username() const
    {
    return *iUsername;
    }

// -----------------------------------------------------------------------------
// CNATTraversalConfig::Password
// -----------------------------------------------------------------------------
//
const TDesC8& CNATTraversalConfig::Password() const
    {
    return *iPassword;
    }

// -----------------------------------------------------------------------------
// CNATTraversalConfig::SetSharedSecretNotSupportedL
// -----------------------------------------------------------------------------
//
void CNATTraversalConfig::SetSharedSecretNotSupportedL(TBool aNotSupported)
    {
    WriteL(KUNSAFProtocolsDomainSharedSecretNotSupported,aNotSupported);
    iSharedSecretNotSupported = aNotSupported;
    }

// -----------------------------------------------------------------------------
// CNATTraversalConfig::BindingRequestFailed
// -----------------------------------------------------------------------------
//
TBool CNATTraversalConfig::BindingRequestFailed() const
    {
    return iBindingRequestFailed;
    }

// -----------------------------------------------------------------------------
// CNATTraversalConfig::SetBindingRequestFailedL
// -----------------------------------------------------------------------------
//
void CNATTraversalConfig::SetBindingRequestFailedL(TBool aFailed)
    {
    WriteL(KUNSAFProtocolsDomainBindingRequestFailed,aFailed);
    iBindingRequestFailed = aFailed;
    }

// -----------------------------------------------------------------------------
// CNATTraversalConfig::ReadDomainSettingsL
// -----------------------------------------------------------------------------
//
void CNATTraversalConfig::ReadDomainSettingsL(
    CRepository& aRepository,
    const TDesC8& aDomain)
    {
    RArray<TUint32> keys;
    CleanupClosePushL(keys);
    TInt err = aRepository.FindEqL(KUNSAFProtocolsDomainMask,
                                   KUNSAFProtocolsFieldTypeMask,
                                   aDomain,
                                   keys);
    if (err == KErrNone && keys.Count() == 1)
        {
        TUint32 key = (KUNSAFProtocolsDomainMask^(keys[0]));
        key |= KUNSAFProtocolsDomainTableMask;
        // Server
        TBuf8<1> tmp;
        TInt actualLength = 0;
        TInt status =
            aRepository.Get(KUNSAFProtocolsSTUNServerMask|key,tmp,actualLength);
        if ((status == KErrNone || status == KErrOverflow) &&
            (actualLength > 0))
            {
            iSTUNServer = HBufC8::NewL(actualLength);
            TPtr8 ptr(iSTUNServer->Des());
            User::LeaveIfError(
                aRepository.Get(KUNSAFProtocolsSTUNServerMask|key,ptr));
            }
        else
            {
            iSTUNServer = KNullDesC8().AllocL();
            }
        // Username
        actualLength = 0;
        status = aRepository.Get(KUNSAFProtocolsSTUNUsernameMask|key,
                                 tmp,actualLength);
        if ((status == KErrNone || status == KErrOverflow) &&
            (actualLength > 0))
            {
            iUsername = HBufC8::NewL(actualLength);
            TPtr8 ptr(iUsername->Des());
            User::LeaveIfError(
                aRepository.Get(KUNSAFProtocolsSTUNUsernameMask|key,ptr));
            }
        else
            {
            iUsername = KNullDesC8().AllocL();
            }
        // Password
        actualLength = 0;
        status = aRepository.Get(KUNSAFProtocolsSTUNPasswordMask|key,
                                 tmp,actualLength);
        if ((status == KErrNone || status == KErrOverflow) &&
            (actualLength > 0))
            {
            iPassword = HBufC8::NewL(actualLength);
            TPtr8 ptr(iPassword->Des());
            User::LeaveIfError(
                aRepository.Get(KUNSAFProtocolsSTUNPasswordMask|key,ptr));
            }
        else
            {
            iPassword = KNullDesC8().AllocL();
            }
        // Server port
        TInt tmpPort = 0;
        if (Read(aRepository,KUNSAFProtocolsSTUNServerPortMask|key,tmpPort))
            {
            iSTUNServerPort = static_cast<TUint>(tmpPort);
            }
        // UDP refresh interval
        Read(aRepository,KUNSAFProtocolsDomainIntervalUDPMask|key,
             iUDPRefreshInterval);
        // TCP refresh interval
        Read(aRepository,KUNSAFProtocolsDomainIntervalTCPMask|key,
             iTCPRefreshInterval);
		// CRLF refresh enabled
		ReadCRLFRefreshEnabled(aRepository, key);
        // Shared secret support
        ReadBoolValue(aRepository,
                      KUNSAFProtocolsDomainSharedSecretNotSupported|key,
                      iSharedSecretNotSupported);
        // Latest binding request status
        ReadBoolValue(aRepository,
                      KUNSAFProtocolsDomainBindingRequestFailed|key,
                      iBindingRequestFailed);
        iDomainKey = key;
        }
    else
        {
        iSTUNServer = KNullDesC8().AllocL();
        iUsername = KNullDesC8().AllocL();
        iPassword = KNullDesC8().AllocL();
        }
    CleanupStack::PopAndDestroy(&keys);
    }

// -----------------------------------------------------------------------------
// CNATTraversalConfig::ReadIapSettingsL
// -----------------------------------------------------------------------------
//
void CNATTraversalConfig::ReadIapSettingsL(
    CRepository& aRepository,
    TUint32 aIapId)
    {
    RArray<TUint32> keys;
    CleanupClosePushL(keys);
    TInt err = aRepository.FindEqL(KUNSAFProtocolsIAPIdMask,
                                   KUNSAFProtocolsFieldTypeMask,
                                   static_cast<TInt>(aIapId),
                                   keys);
    if (err == KErrNone && keys.Count() == 1)
        {
        TUint32 key = (KUNSAFProtocolsIAPIdMask^(keys[0]));
        key |= KUNSAFProtocolsIAPTableMask;
        // UDP refresh interval
        Read(aRepository,KUNSAFProtocolsIntervalUDPMask|key,
             iUDPRefreshInterval);
        // TCP refresh interval
        Read(aRepository,KUNSAFProtocolsIntervalTCPMask|key,
             iTCPRefreshInterval);
        // Retransmit interval
        Read(aRepository,KUNSAFProtocolsSTUNRetransmitTimerMask|key,
             iSTUNRetransmitTimer);
        }
    CleanupStack::PopAndDestroy(&keys);
    }

// -----------------------------------------------------------------------------
// CNATTraversalConfig::ReadCRLFRefreshEnabled
// -----------------------------------------------------------------------------
//
void CNATTraversalConfig::ReadCRLFRefreshEnabled(CRepository& aRepository,
    											 const TUint32 aKey)
	{
	TInt value( 0 );
	if ( Read( aRepository,
			   KUNSAFProtocolsDomainEnableCRLFRefresh | aKey,
			   value ) )
		{
		if ( value )
			{
			iCRLFRefreshEnabled	= EEnabled;
			}
		else
			{
			iCRLFRefreshEnabled	= EDisabled;
			}
		}
	}

// -----------------------------------------------------------------------------
// CNATTraversalConfig::ReadBoolValue
// -----------------------------------------------------------------------------
//
void CNATTraversalConfig::ReadBoolValue(
    CRepository& aRepository,
    const TUint32& aKey,
    TBool& aKeyValue) const
    {
    TInt tmp = 0;
    Read(aRepository,aKey,tmp);
    aKeyValue = (tmp != 0);
    }

// -----------------------------------------------------------------------------
// CNATTraversalConfig::Read
// -----------------------------------------------------------------------------
//
TBool CNATTraversalConfig::Read(
    CRepository& aRepository,
    const TUint32& aKey,
    TInt& aKeyValue) const
    {
    TBool found = EFalse;
    TInt tmp;
    if (aRepository.Get(aKey,tmp) == KErrNone)
        {
        aKeyValue = tmp;
        found = ETrue;
        }
    return found;
    }

// -----------------------------------------------------------------------------
// CNATTraversalConfig::WriteL
// -----------------------------------------------------------------------------
//
void CNATTraversalConfig::WriteL(
    TUint32 aFieldMask,
    TBool aValue)
    {
    User::LeaveIfError(
        iCenRep->StartTransaction(
            CRepository::EConcurrentReadWriteTransaction));
    iCenRep->CleanupCancelTransactionPushL();

    if (!iDomainKey)
        {
        // No domain settings. Create a new key and value.
        iDomainKey = CreateNewKeyL(*iCenRep, KUNSAFProtocolsDomainMask,
                                   KUNSAFProtocolsDomainTableMask);
        User::LeaveIfError(
            iCenRep->Create(iDomainKey|KUNSAFProtocolsDomainMask,*iDomain));
        User::LeaveIfError(
            iCenRep->Create(iDomainKey|aFieldMask,aValue));
        }
    else
        {
        TUint32 key = (aFieldMask|iDomainKey);
        TInt tmp = 0;
        if (iCenRep->Get(key,tmp) == KErrNone)
            {
            // Update existing value
            User::LeaveIfError(iCenRep->Set(key,aValue));
            }
        else
            {
            // Create new value
            User::LeaveIfError(iCenRep->Create(key,aValue));
            }
        }

    TUint32 dummy;
    User::LeaveIfError(iCenRep->CommitTransaction(dummy));
    CleanupStack::Pop(1); // transaction
    }

// -----------------------------------------------------------------------------
// CNATTraversalConfig::CreateNewKeyL
// -----------------------------------------------------------------------------
//
TUint32 CNATTraversalConfig::CreateNewKeyL(
    CRepository& aRepository,
    TUint32 aKey,
    TUint32 aTable) const
    {
    TUint32 newKey = 0;
    RArray<TUint32> keys;
    CleanupClosePushL( keys );
    TInt err = aRepository.FindL( aKey,
                                  KUNSAFProtocolsFieldTypeMask,
                                  keys );
    TInt keyCount = keys.Count();
    if ( err == KErrNotFound )
        {
        newKey = 1;
        }
    else
        {
        User::LeaveIfError( err );
        if ( keyCount == 0 )
            {
            newKey = 1;
            }
        else
            {
            // Find the biggest key and increment it by one
            keys.SortUnsigned();
            TUint32 maxKey = aKey^keys[ keyCount - 1 ];
            newKey = maxKey + 1;
            }
        }
    newKey |= aTable;
    CleanupStack::PopAndDestroy( &keys );
    return newKey;
    }
