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
* Description:    
*
*/




#include "cnatsettings.h"
//#include <unsafprotocolscrkeys.h>
//#include "tnatportpool.h"
#include "tnatserversettings.h"

// CONSTANTS
const TInt KDefaultUDPRefreshInterval = 28;
const TInt KDefaultTCPRefreshInterval = 1200;
const TInt KDefaultSTUNRetransmitTimer = 250; // milliseconds
const TInt KDefaultTURNBandwidth = 0; // KB/s
const TInt KDefaultRequestedTransport = 0; //UDP
const TInt KDefaultRequestedPortProps = 0; //no specific port alignment and 
                                           //special behavior requested
const TInt KDefaultTURNLifeTime = 0;
//const TInt KDefaultTURNRetryTime = 500; //milliseconds
const TInt KDefaultTURNServerTimerVal = 0; //milliseconds
const TInt KDefaultTransitioningStateTime = 0; //seconds
const TInt KDefaultICECanditateTimer = 0;

const TInt KDefaultSTUNServerPort = 3478;
const TInt KDefaultTURNServerPort = 0;
const TInt KDefaultTCPTimeout = 1200;
const TInt KDefaultUDPTimeout = 28; 
const TInt KDefaultConnectivityCheckTimeout = 2; 

const TInt KDefaultPortFieldLength = 25;
const TInt KDefaultPortPoolStarPort = 25000;
const TInt KDefaultPortPoolEndPort = 50000;

const TInt KLatestConnectedStunServerPort = 10;
const TInt KLatestConnectedTurnServerPort = 20;

_LIT8( KStunServer, "0.0.0.0");

// -----------------------------------------------------------------------------
// CNATFWNATSettings::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWNATSettings* CNATFWNATSettings::NewL( const TDesC8& aDomain,
                                            TNATFWNATSettings aSettings)
	{   
    CNATFWNATSettings* self = CNATFWNATSettings::NewLC(aDomain,aSettings);
    CleanupStack::Pop(self);
    return self;
	}
	
// -----------------------------------------------------------------------------
// CNATFWNATSettings::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CNATFWNATSettings* CNATFWNATSettings::NewLC( const TDesC8& aDomain,
                                             TNATFWNATSettings aSettings)
	{   
    CNATFWNATSettings* self = new(ELeave)CNATFWNATSettings(aSettings);
    CleanupStack::PushL(self);
    self->ConstructL(aDomain);
    return self;    
	}

// -----------------------------------------------------------------------------
// CNATFWNATSettings::CNATFWNATSettings
// -----------------------------------------------------------------------------
//
CNATFWNATSettings::CNATFWNATSettings(TNATFWNATSettings aSettings) : 
    iSettings(aSettings)
    {
    iSettings = aSettings;
    iLatestConnectedSTUNServerPort = KLatestConnectedStunServerPort;
    iLatestConnectedTURNServerPort = KLatestConnectedTurnServerPort;
    iUDPRefreshInterval = KDefaultUDPRefreshInterval;
    iTCPRefreshInterval = KDefaultTCPRefreshInterval;
    iSTUNRetransmitTimer = KDefaultSTUNRetransmitTimer;
    iTURNBandwidth = KDefaultTURNBandwidth;
    iRequestedTransport = KDefaultRequestedTransport;
    iRequestedPortProps = KDefaultRequestedPortProps;
    iTURNLifeTime = KDefaultTURNLifeTime;
//  iICECanditateTimer = KDefaultICECanditateTimer;
    iUseSharedSecret = EFalse;
    }

// -----------------------------------------------------------------------------
// CNATFWNATSettings::ConstructL
// -----------------------------------------------------------------------------
//	
void CNATFWNATSettings::ConstructL(const TDesC8& aDomain)
    {
    __ASSERT_ALWAYS(aDomain.Length() > 0, User::Leave(KErrArgument));

//    iCenRep = CRepository::NewL(KCRUidUNSAFProtocols);
    iLatestConnectedSTUNServerAddress = KNullDesC8().AllocL();
    iLatestConnectedTURNServerAddress = KNullDesC8().AllocL();
    iRequestedAddress = KNullDesC8().AllocL();
    
    iSTUNServerSettingsArray =
        new( ELeave ) CArrayFixFlat<TNATServerSettings> ( 1 );
    iTURNServerSettingsArray =
        new( ELeave ) CArrayFixFlat<TNATServerSettings> ( 1 );
        
    iDomain = aDomain.AllocL();
    }

// -----------------------------------------------------------------------------
// CNATFWNATSettings::~CNATFWNATSettings
//---------------------------------------------------------------------------
//
CNATFWNATSettings::~CNATFWNATSettings()
    {
    delete iRequestedAddress;
    delete iDomain;
    delete iLatestConnectedSTUNServerAddress;
    delete iLatestConnectedTURNServerAddress;
	
    delete iSTUNServerSettingsArray;
    delete iTURNServerSettingsArray;
	
/*    if (iDomainKey)
        {
        // Remove settings added during runtime
        if(iCenRep) iCenRep->Delete(KUNSAFProtocolsSharedSecretMask|iDomainKey);
        }
    if(iPortField.KeyMaskForCR() != 0)
        {
        // remove if added during runtime
        iCenRep->Delete(iPortField.KeyMaskForCR()|KUNSAFProtocolsStartPortMask);
        iCenRep->Delete(iPortField.KeyMaskForCR()|KUNSAFProtocolsEndPortMask);
        }
    delete iCenRep;*/
    }

// -----------------------------------------------------------------------------
// CNATFWNATSettings::RetrieveIapSettings
// -----------------------------------------------------------------------------
//	    
EXPORT_C TInt CNATFWNATSettings::RetrieveIapSettingsL(const TUint /*aIapId*/ )
    {
    //iIapId = aIapId;
    //return ReadIapSettingsL(*iCenRep, aIapId);
    return KErrNone;
    }
/*	
// -----------------------------------------------------------------------------
// CNATFWNATSettings::UpdateL
// -----------------------------------------------------------------------------
//	  
EXPORT_C void CNATFWNATSettings::UpdateL()
    {
    UpdateL(iSettings);
    }

// -----------------------------------------------------------------------------
// CNATFWNATSettings::UpdateL
// -----------------------------------------------------------------------------
//	  
EXPORT_C void CNATFWNATSettings::UpdateL(TNATFWNATSettings aSettings)
    {
    TInt err = ReadDomainSettingsKeyL(*iCenRep, *iDomain, iDomainKey);
    if(err == KErrNone)
        {
        if((aSettings&ESTUNSettings) == ESTUNSettings)
            {
            ReadSTUNSettingsL(*iCenRep, iDomainKey); 
            }
        if((aSettings&ETURNSettings) == ETURNSettings)
            {
            ReadTURNSettingsL(*iCenRep, iDomainKey);
            } 
        if((aSettings&EICESettings) == EICESettings)
            {
            ReadICESettings(*iCenRep, iDomainKey);
            }
        if((aSettings&ESTUNSettings) == ESTUNSettings ||
             (aSettings&ETURNSettings) == ETURNSettings)
            {
            ReadGeneralSettingsL(*iCenRep, iDomainKey);
            }
        }
    ReadIapSettingsL(*iCenRep,iIapId);
    }

// -----------------------------------------------------------------------------
// CNATFWNATSettings::IapId
// -----------------------------------------------------------------------------
//	
EXPORT_C TUint32 CNATFWNATSettings::IapId() const
    {
    return iIapId;
    }

// -----------------------------------------------------------------------------
// CNATFWNATSettings::Domain
// -----------------------------------------------------------------------------
//	
EXPORT_C const TDesC8& CNATFWNATSettings::Domain() const
    {
    return *iDomain;
    }*/
// -----------------------------------------------------------------------------
// CNATFWNATSettings::GetSTUNServerArrayL
// -----------------------------------------------------------------------------
//	
EXPORT_C CArrayFix<TNATServerSettings>* 
    CNATFWNATSettings::GetSTUNServerArrayL() const
    {
    CArrayFix<TNATServerSettings>* StunServerSettingsArray =
        new( ELeave ) CArrayFixFlat<TNATServerSettings> ( 1 );
    TInt count = iSTUNServerSettingsArray->Count();
    
    for ( TInt i(0); i < count; i++ )
        {
        StunServerSettingsArray->AppendL( iSTUNServerSettingsArray->At( i ) );
        }
        
    return StunServerSettingsArray;
    }

// -----------------------------------------------------------------------------
// CNATFWNATSettings::GetTURNServerArrayL
// -----------------------------------------------------------------------------
//	
EXPORT_C CArrayFix<TNATServerSettings>* 
    CNATFWNATSettings::GetTURNServerArrayL() const
    {
    CArrayFix<TNATServerSettings>* TURNServerSettingsArray =
        new( ELeave ) CArrayFixFlat<TNATServerSettings> ( 1 );
    TInt count = iTURNServerSettingsArray->Count();
    
    for ( TInt i(0); i < count; i++ )
        {
        TURNServerSettingsArray->AppendL( iTURNServerSettingsArray->At( i ) );
        }
        
    return TURNServerSettingsArray;
    }
  
// -----------------------------------------------------------------------------
// CNATFWNATSettings::LatestConnectedSTUNServerAddress
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CNATFWNATSettings::LatestConnectedSTUNServerAddress() const
    {
    return *iLatestConnectedSTUNServerAddress;
    }
    
// -----------------------------------------------------------------------------
// CNATFWNATSettings::LatestConnectedSTUNServerPort
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CNATFWNATSettings::LatestConnectedSTUNServerPort() const
    {
    return iLatestConnectedSTUNServerPort;
    }
    
// -----------------------------------------------------------------------------
// CNATFWNATSettings::LatestConnectedTURNServerAddress
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CNATFWNATSettings::LatestConnectedTURNServerAddress() const
    {
    return *iLatestConnectedTURNServerAddress;
    }
    
// -----------------------------------------------------------------------------
// CNATFWNATSettings::LatestConnectedTURNServerPort
// -----------------------------------------------------------------------------
//  
EXPORT_C TUint CNATFWNATSettings::LatestConnectedTURNServerPort() const
    {
    return iLatestConnectedTURNServerPort;
    }
       
// -----------------------------------------------------------------------------
// CNATFWNATSettings::UDPRefreshInterval
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CNATFWNATSettings::UDPRefreshInterval() const
    {
    return iUDPRefreshInterval; 
    }
    
// -----------------------------------------------------------------------------
// CNATFWNATSettings::TCPRefreshInterval
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CNATFWNATSettings::TCPRefreshInterval() const
    {
    return iTCPRefreshInterval;
    }
    
// -----------------------------------------------------------------------------
// CNATFWNATSettings::STUNRetransmitTimer
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CNATFWNATSettings::STUNRetransmitTimer() const
    {
    return iSTUNRetransmitTimer;
    }
/*
// -----------------------------------------------------------------------------
// CNATFWNATSettings::CRLFRefreshEnabledToProxy
// -----------------------------------------------------------------------------
//   
EXPORT_C TBool CNATFWNATSettings::CRLFRefreshEnabledToProxy() const
    {
    return iCRLFRefreshEnabled;
    }   
*/
// -----------------------------------------------------------------------------
// CNATFWNATSettings::SharedSecretNotSupported
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CNATFWNATSettings::UseSharedSecret() const
    {
    return iUseSharedSecret;
    }
/*
// -----------------------------------------------------------------------------
// CNATFWNATSettings::BindingRequestFailed
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CNATFWNATSettings::BindingRequestFailed() const
    {
    return iBindingRequestFailed;
    }
 
// -----------------------------------------------------------------------------
// CNATFWNATSettings::STUNUsed
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CNATFWNATSettings::STUNUsed() const
    {
    return iSTUNUsed;
    } 
        
// -----------------------------------------------------------------------------
// CNATFWNATSettings::TURNUsed
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CNATFWNATSettings::TURNUsed() const
    {
    return iTURNUsed;
    } 

// -----------------------------------------------------------------------------
// CNATFWNATSettings::ICEUsed
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CNATFWNATSettings::ICEUsed() const
    {
    return iOfferICE;
    }        
// -----------------------------------------------------------------------------
// CNATFWNATSettings::TURNBandwidth
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CNATFWNATSettings::TURNBandwidth() const
    {
    return iTURNBandwidth;
    }
    
// -----------------------------------------------------------------------------
// CNATFWNATSettings::RequestedTransport
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CNATFWNATSettings::RequestedTransport() const
    {
    return iRequestedTransport;
    }
        
// -----------------------------------------------------------------------------
// CNATFWNATSettings::RequestedIPAddress
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8&  CNATFWNATSettings::RequestedAddress() const
    {
    return *iRequestedAddress;
    }
        
// -----------------------------------------------------------------------------
// CNATFWNATSettings::RequestedPort
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CNATFWNATSettings::RequestedPort() const
    {
    return iRequestedPort;
    }
        
// -----------------------------------------------------------------------------
// CNATFWNATSettings::RequestedPortProps
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CNATFWNATSettings::RequestedPortProps() const
    {
    return iRequestedPortProps;
    }
        
// -----------------------------------------------------------------------------
// CNATFWNATSettings::TURNLifetime
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CNATFWNATSettings::TURNLifetime() const
    {
    return iTURNLifeTime;
    }
        
// -----------------------------------------------------------------------------
// CNATFWNATSettings::TURNRequestRetryTime
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CNATFWNATSettings::TURNRequestRetryTime() const
    {
    return iTURNRetryTime; 
    }
        
// -----------------------------------------------------------------------------
// CNATFWNATSettings::TURNServerTimerVal
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CNATFWNATSettings::TURNServerTimerVal() const
    {
    return iTURNServerTimerVal;
    }
        
// -----------------------------------------------------------------------------
// CNATFWNATSettings::TransitioningStateTime
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CNATFWNATSettings::TransitioningStateTime() const
    {
    return iTransitioningStateTime;
    }
       
// -----------------------------------------------------------------------------
// CNATFWNATSettings::SharedSecret
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CNATFWNATSettings::SharedSecret() const
    {
    return *iSharedSecret;
    }    
      
// -----------------------------------------------------------------------------
// CNATFWNATSettings::ActiveDestinationAddress
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CNATFWNATSettings::ActiveDestinationAddress() const
    {
    return *iActiveDestinationAddress;
    }
        
// -----------------------------------------------------------------------------
// CNATFWNATSettings::ActiveDestinationPort
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CNATFWNATSettings::ActiveDestinationPort() const
    {
    return iActiveDestinationPort;
    }
        
// -----------------------------------------------------------------------------
// CNATFWNATSettings::RelayedTransportAddress
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CNATFWNATSettings::RelayedTransportAddress() const
    {
    return *iRelayedAddress;
    }
        
// -----------------------------------------------------------------------------
// CNATFWNATSettings::RelayedTransportPort
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CNATFWNATSettings::RelayedTransportPort() const
    {
    return iRelayedPort; 
    }
        
// -----------------------------------------------------------------------------
// CNATFWNATSettings::ReflexiveTransportAddress
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CNATFWNATSettings::ReflexiveTransportAddress() const
    {
    return *iReflexiveAddress;
    }
        
// -----------------------------------------------------------------------------
// CNATFWNATSettings::ReflexiveTransportPort
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CNATFWNATSettings::ReflexiveTransportPort() const
    {
    return iReflexivePort;
    }
        
// -----------------------------------------------------------------------------
// CNATFWNATSettings::TransitioningStateTimer
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CNATFWNATSettings::TransitioningStateTimer() const
    {
    return iTransitioningStateTime;
    }

// -----------------------------------------------------------------------------
// CNATFWNATSettings::SetSharedSecretL
// -----------------------------------------------------------------------------
//
EXPORT_C void CNATFWNATSettings::SetSharedSecretL(const TDesC8& aValue)
    {
    WriteL(KUNSAFProtocolsSharedSecretMask|iDomainKey,aValue);
    if(iSharedSecret)
        {
        delete iSharedSecret;
        iSharedSecret = 0;
        }
    iSharedSecret = aValue.AllocL();
    }
    
// -----------------------------------------------------------------------------
// CNATFWNATSettings::PlugInParams
// -----------------------------------------------------------------------------
//
EXPORT_C TNATFWPlugInParams CNATFWNATSettings::PlugInParams() const
    {
    return iPlugInParams;
    }
    
// -----------------------------------------------------------------------------
// CNATFWNATSettings::UsedNATProtocol
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CNATFWNATSettings::UsedNATProtocol() const
    {
    return iUsedNATProtocol;
    }
    
// -----------------------------------------------------------------------------
// CNATFWNATSettings::GetPortAreaL
// -----------------------------------------------------------------------------
//
EXPORT_C void CNATFWNATSettings::GetPortAreaL(TUint& aStartPort, TUint& aEndPort)
    {
    TUint portpoolStart;
    TUint portpoolEnd;
    TUint portFieldLength;
   
    if(iPortField.KeyMaskForCR() == 0)
        {
        //read port pool settings
        if(Read(*iCenRep, KUNSAFProtocolsPortPoolStartPortMask|iDomainKey, portpoolStart)==EFalse)
            {   
            portpoolStart = KDefaultPortPoolStarPort;
            }
        if(Read(*iCenRep, KUNSAFProtocolsPortPoolEndPortMask|iDomainKey, portpoolEnd)==EFalse)
            {   
            portpoolEnd = KDefaultPortPoolEndPort;
            }
        if(Read(*iCenRep, KUNSAFProtocolsPortFieldLengthMask|iDomainKey, portFieldLength)==EFalse)
            {   
            portFieldLength = KDefaultPortFieldLength;
            }   
        TNATPortPool portPool(portpoolStart, portpoolEnd);
        iPortField.SetLength(portFieldLength);

        //read reserved port fields to PortPool
        RArray<TUint32> keys;
        CleanupClosePushL(keys);
        TInt error = iCenRep->FindL(KUNSAFProtocolsStartPortMask,
                                    KUNSAFProtocolsPortFieldFieldTypeMask,
                                    keys);
        TUint keyCount = keys.Count();
            
        if (error == KErrNone && keyCount > 0)
            {
            for(TUint i=0; i<keyCount; i++)
                {   
                TUint32 key = keys[i]^KUNSAFProtocolsStartPortMask;
                TUint start = 0;
                TUint end = 0;
                //Start port
                Read(*iCenRep, KUNSAFProtocolsStartPortMask|key, start);
                //End port
                Read(*iCenRep, KUNSAFProtocolsEndPortMask|key, end);
                    
                TNATPortField field;
                field.SetStartPort(start);
                field.SetEndPort(end);
                    
                User::LeaveIfError(portPool.AppendPortField(field));
                }
            }
        else if(error != KErrNotFound)
            {
            User::LeaveIfError(error);
            }
        CleanupStack::PopAndDestroy(&keys);
            
        //reserve port field
        if(portPool.ReserveNewPortArea(iPortField))
            { 
            //new key
            TUint32 newKey = CreateNewKeyL(*iCenRep,
                                            KUNSAFProtocolsStartPortMask,
                                            KUNSAFProtocolsPortFieldFieldTypeMask);
            //write to cr
            WriteL(KUNSAFProtocolsStartPortMask|newKey, iPortField.StartPort());
            WriteL(KUNSAFProtocolsEndPortMask|newKey, iPortField.EndPort());
            iPortField.SetKeyMask(newKey);
            }
        else
            {
            User::Leave(KErrOverflow );
            }
        }
    aStartPort = iPortField.StartPort();
    aEndPort = iPortField.EndPort();
    }
    
// Private Functions
// -----------------------------------------------------------------------------
// CNATFWNATSettings::ReadDomainSettingsKeyL
// -----------------------------------------------------------------------------
//    
TInt CNATFWNATSettings::ReadDomainSettingsKeyL(
    CRepository& aRepository,
    const TDesC8& aDomain,
    TUint32& aDomainKey)
    {
   	RArray<TUint32> keys;
    CleanupClosePushL(keys);
    TInt err = aRepository.FindEqL( KUNSAFProtocolsDomainMask,
                                    KUNSAFProtocolsFieldTypeMask,
                                  	aDomain,
                                   	keys);
    TInt count = keys.Count();
    if (err == KErrNone && count == 1)
        {
        aDomainKey = (KUNSAFProtocolsDomainMask^(keys[0]));
        }
    if(count > 1)
        {
        err = KErrCorrupt;
        }
    CleanupStack::PopAndDestroy(&keys);
    return err;
    }

// -----------------------------------------------------------------------------
// CNATFWNATSettings::ReadGeneralSettingsL
// -----------------------------------------------------------------------------
//
void CNATFWNATSettings::ReadGeneralSettingsL(
    CRepository& aRepository,
    TUint32 aDomainKey)
{
    HBufC8* tempBuf = NULL;

    //Plug-in params
    RArray<TUint32> plugInkeys;
    CleanupClosePushL(plugInkeys);
    TInt error = aRepository.FindL(KUNSAFProtocolsPlugInIdMask|aDomainKey, 
                                   KUNSAFProtocolsSubTableFieldTypeMask,
                                   plugInkeys);
    TUint keyCount = plugInkeys.Count();
    if (error == KErrNone && keyCount > 0)
    {
        for(TUint i=0; i<keyCount; i++)
            {   
            TUint32 key = plugInkeys[i]^KUNSAFProtocolsPlugInIdMask;
            //Plug-in id
            if(Read(aRepository, KUNSAFProtocolsPlugInIdMask|key, iPlugInParams.iPlugInId))
                {
                iPlugInParams.iPlugInId = 0;
                }
            //Plug-in service id
            if(Read(aRepository,
                 KUNSAFProtocolsPlugInServiceIdMask|key,
                 iPlugInParams.iPlugInServiceId))
                {
                iPlugInParams.iPlugInServiceId = 0;
                }
            }
    }
    else if(error == KErrNotFound)
        {
        iPlugInParams.iPlugInId = 0;
        iPlugInParams.iPlugInServiceId = 0;
        }
    else
        { 
        User::LeaveIfError(error);
        }
    CleanupStack::PopAndDestroy(&plugInkeys);
    
    
    // TURNUsed
    ReadBoolValue(aRepository,
                  KUNSAFProtocolsTURNUsedMask|aDomainKey,
                  iTURNUsed);
    // STUNUsed
    ReadBoolValue(aRepository,
                  KUNSAFProtocolsSTUNUsedMask|aDomainKey,
                  iSTUNUsed);     
    // OfferIce
    ReadBoolValue(aRepository,
                  KUNSAFProtocolsOfferICEMask|aDomainKey,
                  iOfferICE);

    // Shared secret support 
    ReadBoolValue(aRepository,
                  KUNSAFProtocolsDomainSharedSecretNotSupportedMask|aDomainKey,
                  iSharedSecretNotSupported);
    // Latest binding request status
    ReadBoolValue(aRepository,
                  KUNSAFProtocolsDomainBindingRequestFailedMask|aDomainKey,
                  iBindingRequestFailed);   
    // Shared secret
    if(ReadL(aRepository, KUNSAFProtocolsSharedSecretMask|aDomainKey, &tempBuf))
    {
    	 if(iSharedSecret) delete iSharedSecret;
         iSharedSecret = tempBuf;
    }
    else{ delete tempBuf; }
}

// -----------------------------------------------------------------------------
// CNATFWNATSettings::ReadServerSettingsL
// -----------------------------------------------------------------------------
//
void CNATFWNATSettings::ReadServerSettingsL(
    CRepository& aRepository,
    const TUint32 aServerKeyMask,
    TServerSettings aDefaultSettings,
    RPointerArray<CNATServerSettings>& aServerSettingsArray )
    {
    RArray<TUint32> serverKeys;
    CleanupClosePushL(serverKeys);
    aServerSettingsArray.ResetAndDestroy();
    TServerSettings settings;
      
    //finding server keys by using address
    TInt error = aRepository.FindL(
        KUNSAFProtocolsSTUNAddressMask^KUNSAFProtocolsSTUNServerTableMask|aServerKeyMask, 
        KUNSAFProtocolsSubTableFieldTypeMask,
        serverKeys);
    //read server settings
    TUint keyCount = serverKeys.Count();
    if (error == KErrNone && keyCount > 0)
        {
        for(TUint i=0; i<keyCount; i++)
            {
            HBufC8* address = NULL;
            //address
            ReadL(
                aRepository,
                KUNSAFProtocolsSTUNAddressMask^KUNSAFProtocolsSTUNServerTableMask|serverKeys[i],
                &address);
            //port
            if(Read(
                aRepository,
                KUNSAFProtocolsSTUNPortMask^KUNSAFProtocolsSTUNServerTableMask|serverKeys[i],
                settings.iPort) == EFalse) 
                {
                settings.iPort = aDefaultSettings.iPort;
                }
            //TCPTimeout
            if(Read(
                aRepository,
                KUNSAFProtocolsSTNUTCPTimeoutMask^KUNSAFProtocolsSTUNServerTableMask|serverKeys[i],
                settings.iTCPTimeout) == EFalse)
                {
                settings.iTCPTimeout = aDefaultSettings.iTCPTimeout;
                }
            //UDPTimeout
            if(Read(
                aRepository,
                KUNSAFProtocolsSTUNUDPTimeoutMask^KUNSAFProtocolsSTUNServerTableMask|serverKeys[i],
                settings.iUDPTimeout) == EFalse)
                {
                settings.iUDPTimeout = aDefaultSettings.iUDPTimeout;
                }
            //ConnectivityCheckTimeout
            if(Read(
                aRepository,
                KUNSAFProtocolsSTUNConnectivityCheckTimeoutMask^KUNSAFProtocolsSTUNServerTableMask|
                serverKeys[i],
                settings.iConnectivityCheckTimeout) == EFalse)
                {
                settings.iConnectivityCheckTimeout = aDefaultSettings.iConnectivityCheckTimeout;
                }
            //CRLFRefreshEnabled
            if(ReadBoolValue(
                aRepository,
                KUNSAFProtocolsSTUNEnableCRLFRefreshMask^KUNSAFProtocolsSTUNServerTableMask|
                serverKeys[i],
                settings.iCRLFRefreshEnabled) == EFalse)
                {
                settings.iCRLFRefreshEnabled = aDefaultSettings.iCRLFRefreshEnabled;
                }
            
            CNATServerSettings* serverSettings = CNATServerSettings::NewLC(
                *address,
                settings.iPort,
                settings.iTCPTimeout,
                settings.iUDPTimeout,
                settings.iConnectivityCheckTimeout,
                settings.iCRLFRefreshEnabled
                 );
            aServerSettingsArray.AppendL(serverSettings);
            CleanupStack::Pop(serverSettings);
            delete address;
            } 
        }
    else if(error != KErrNotFound)
        {
        User::Leave(error);
        }
    CleanupStack::PopAndDestroy(&serverKeys);
    }

// -----------------------------------------------------------------------------
// CNATFWNATSettings::ReadSTUNSettingsL
// -----------------------------------------------------------------------------
//
void CNATFWNATSettings::ReadSTUNSettingsL(
    CRepository& aRepository,
    TUint32 aDomainKey)
{
    RArray<TUint32> addressKeys;
    HBufC8* tempBufPointer = NULL;
    CleanupClosePushL(addressKeys);
    
    // STUN Servers
    TServerSettings stunSettings;
    stunSettings.iPort = KDefaultSTUNServerPort;
    stunSettings.iTCPTimeout = KDefaultTCPTimeout;
    stunSettings.iUDPTimeout = KDefaultUDPTimeout;
    stunSettings.iConnectivityCheckTimeout = KDefaultConnectivityCheckTimeout;
    stunSettings.iCRLFRefreshEnabled = EFalse;
    
    ReadServerSettingsL(
        aRepository,
        KUNSAFProtocolsSTUNServerTableMask|aDomainKey,
        stunSettings,
        iSTUNServerSettingsArray
        );
    
    //LatestConnectedStunServerAddress
    if(ReadL(aRepository,
             KUNSAFProtocolsLatestConnectedSTUNServerAddressMask|aDomainKey,
             &tempBufPointer))
    {
        if(iLatestConnectedSTUNServerAddress) delete iLatestConnectedSTUNServerAddress;
        iLatestConnectedSTUNServerAddress = tempBufPointer;
    }
    else{ delete tempBufPointer; }    
    //LatestConnectedStunServerPort
    if(Read(aRepository, KUNSAFProtocolsLatestConnectedSTUNServerPortMask|aDomainKey,
            iLatestConnectedSTUNServerPort)==EFalse)
        {   
            iLatestConnectedSTUNServerPort = 0;
        }
    // STUN retransmit timer
    if(Read(aRepository,
            KUNSAFProtocolsSTUNRetransmitTimerMask|aDomainKey,
            iSTUNRetransmitTimer)==EFalse)
        {   
        iSTUNRetransmitTimer = KDefaultSTUNRetransmitTimer;
        }
    else
    {
    }
    CleanupStack::PopAndDestroy(&addressKeys); 
}

// -----------------------------------------------------------------------------
// CNATFWNATSettings::ReadTURNSettingsL
// -----------------------------------------------------------------------------
//
void CNATFWNATSettings::ReadTURNSettingsL(
    CRepository& aRepository,
    TUint32 aDomainKey)
{
    RArray<TUint32> addressKeys;
    HBufC8* tempBufPointer = NULL;
    CleanupClosePushL(addressKeys);
     
    //TURN Servers
    TServerSettings turnSettings;
    turnSettings.iPort = KDefaultTURNServerPort;
    turnSettings.iTCPTimeout = KDefaultTCPTimeout;
    turnSettings.iUDPTimeout = KDefaultUDPTimeout;
    turnSettings.iConnectivityCheckTimeout = KDefaultConnectivityCheckTimeout;
    turnSettings.iCRLFRefreshEnabled = EFalse; 
     
        
    ReadServerSettingsL(
        aRepository,
        KUNSAFProtocolsTURNServerTableMask|aDomainKey,
        turnSettings,
        iTURNServerSettingsArray
        );
  
     //TURNBandwidth
    if(Read(aRepository,
            KUNSAFProtocolsTURNBandwidthMask|aDomainKey, iTURNBandwidth)==EFalse)
        {   
        iTURNBandwidth = KDefaultTURNBandwidth;
        } 
    //RequestedTransport
    if(Read(aRepository, KUNSAFProtocolsRequestedTransportMask|aDomainKey,
            iRequestedTransport)==EFalse)
        { 
        iRequestedTransport = KDefaultRequestedTransport;
        }  
    //RequestedPortProps
    if(Read(aRepository, KUNSAFProtocolsRequestedPortPropsMask|aDomainKey,
            iRequestedPortProps)==EFalse)
        {   
        iRequestedPortProps = KDefaultRequestedPortProps;
        }  
    //TURNLifeTime
    if(Read(aRepository,
            KUNSAFProtocolsTURNLifeTimeMask|aDomainKey, iTURNLifeTime)==EFalse)
        {   
        iTURNLifeTime = KDefaultTURNLifeTime;
        }    
    //TURNRetryTime
    if (Read(aRepository,
             KUNSAFProtocolsTURNRetryTimeMask|aDomainKey, iTURNRetryTime)==EFalse)
        {   
        iTURNRetryTime = KDefaultTURNRetryTime;
        }
    //TURNServerTimerVal
    if(Read(aRepository, KUNSAFProtocolsTURNServerTimerValMask|aDomainKey,
            iTURNServerTimerVal)==EFalse)
        {   
            iTURNServerTimerVal = KDefaultTURNServerTimerVal;
        }
    //TransitioningStateTime
    if(Read(aRepository, KUNSAFProtocolsTransitioningStateTimeMask|aDomainKey,
            iTransitioningStateTime)==EFalse)
        {   
            iTransitioningStateTime = KDefaultTransitioningStateTime;
        }
    //RequestedAddress
    if(ReadL(aRepository,
             KUNSAFProtocolsRequestedAdressMask|aDomainKey, &tempBufPointer))
    {
        if(iRequestedAddress) delete iRequestedAddress;
        iRequestedAddress = tempBufPointer;
    }
    else{ delete tempBufPointer; }
    //RequestedPort
    if(Read(aRepository,
            KUNSAFProtocolsRequestedPortMask|aDomainKey, iRequestedPort)==EFalse)
        {   
        iRequestedPort = 0;
        }
    //ActiveDestinationAddress    
    if(ReadL(aRepository,
             KUNSAFProtocolsActiveDestinationAddressMask|aDomainKey, &tempBufPointer))
        {
        if(iActiveDestinationAddress) delete iActiveDestinationAddress;
        iActiveDestinationAddress = tempBufPointer;
        }
    else{ delete tempBufPointer; }
    //ActiveDestinationPort  
    if (Read(aRepository,
             KUNSAFProtocolsActiveDestinationPortMask|aDomainKey,
             iActiveDestinationPort)==EFalse)
        {   
        iActiveDestinationPort = 0;
        }   
    //RelayedAddress
    if(ReadL(aRepository, KUNSAFProtocolsRelayedAddressMask|aDomainKey, &tempBufPointer))
        {
        if(iRelayedAddress) delete iRelayedAddress;
        iRelayedAddress = tempBufPointer;
        }
    else{ delete tempBufPointer; }
    //RelayedPort
    if(Read(aRepository, KUNSAFProtocolsRelayedPortMask|aDomainKey, iRelayedPort)==EFalse)
        {   
        iRelayedPort = 0;
        }
    //ReflexiveAddress
    if(ReadL(aRepository, KUNSAFProtocolsReflexiveAddressMask|aDomainKey, &tempBufPointer))
    {
        if(iReflexiveAddress) delete iReflexiveAddress;
        iReflexiveAddress = tempBufPointer;
    }
    else{ delete tempBufPointer; }
    //ReflexivePort
    if(Read(aRepository, KUNSAFProtocolsReflexivePorMask|aDomainKey, iReflexivePort)==EFalse)
        {   
        iReflexivePort = 0;
        }	
    //LatestConnectedTURNServerAddress
    if(ReadL(aRepository,
             KUNSAFProtocolsLatestConnectedTURNServerAddressMask|aDomainKey,
             &tempBufPointer))
        {
        if(iLatestConnectedTURNServerAddress) delete iLatestConnectedTURNServerAddress;
        iLatestConnectedTURNServerAddress = tempBufPointer;
        }
    else{ delete tempBufPointer; }
    //LatestConnectedTURNServerPort
    if(Read(aRepository, KUNSAFProtocolsLatestConnectedTURNServerPortMask|aDomainKey,
            iLatestConnectedTURNServerPort)==EFalse)
        {   
        iLatestConnectedTURNServerPort = 0;
        }        
    else
    {
    }
    CleanupStack::PopAndDestroy(&addressKeys);
}

// -----------------------------------------------------------------------------
// CNATFWNATSettings::ReadICESettingsL
// -----------------------------------------------------------------------------
//
void CNATFWNATSettings::ReadICESettings(
    CRepository& aRepository,
    TUint32 aDomainKey)
    {
    // ICEMode setting
    ReadBoolValue(aRepository,KUNSAFProtocolsICEFullModeUsedMask|aDomainKey,
         iICEFullModeUsed);         
    // ChooseAnotherCandidateTimer 
    if(Read(aRepository,
         KUNSAFProtocolsICECandidateTimerMask|aDomainKey, iICECanditateTimer)==EFalse)
        {
        iICECanditateTimer = KDefaultICECanditateTimer;
        }
    //UsedNATProtocol
    if(Read(aRepository,
        KUNSAFProtocolsUsedNATProtocolMask|aDomainKey, iUsedNATProtocol)==EFalse)
        {
        iUsedNATProtocol = KDefaultUsedNATProtocol;
        }    
    }
    
// -----------------------------------------------------------------------------
// CNATFWNATSettings::ReadIapSettingsL
// -----------------------------------------------------------------------------
//    
TInt CNATFWNATSettings::ReadIapSettingsL(
    CRepository& aRepository,
    TUint32 aIapId)
    {
    RArray<TUint32> keys;
    CleanupClosePushL(keys);
    TInt err = aRepository.FindEqL(KUNSAFProtocolsIAPIdMask,
                                   KUNSAFProtocolsFieldTypeMask,
                                   static_cast<TInt>(aIapId),
                                   keys);
                                   
    if (err == KErrNone)
        {
        if(keys.Count() > 1)
            {
            err = KErrCorrupt;
            }
        else
            {
            TUint32 key = (KUNSAFProtocolsIAPIdMask^(keys[0]));
            key |= KUNSAFProtocolsIAPTableMask;  
            // read UDP refresh interval
            Read(aRepository,KUNSAFProtocolsIntervalUDPMask|key,
                 iUDPRefreshInterval);
            // read TCP refresh interval 
            Read(aRepository,KUNSAFProtocolsIntervalTCPMask|key,
                 iTCPRefreshInterval);
            // read Retransmit interval 
            Read(aRepository,KUNSAFProtocolsSTUNRetransmitTimerMask|key,
                 iSTUNRetransmitTimer);
            }
        }

    CleanupStack::PopAndDestroy(&keys);
    return err;
    }

// -----------------------------------------------------------------------------
// CNATFWNATSettings::Read
// -----------------------------------------------------------------------------
//
TBool CNATFWNATSettings::ReadL(
    CRepository& aRepository,
    const TUint32& aKey,
    HBufC8** aBuf)
    {
    TBool found = EFalse;
    TBuf8<1> tmp;
    TInt actualLength = 0;
    TInt status = aRepository.Get(aKey,tmp,actualLength);
                
    if ((status == KErrNone || status == KErrOverflow) && (actualLength > 0))
        {
        found = ETrue;
        HBufC8* buf = HBufC8::NewL(actualLength);
        CleanupStack::PushL(buf);
        TPtr8 ptr(buf->Des());
        User::LeaveIfError(aRepository.Get(aKey,ptr));
        *aBuf = buf; 
        CleanupStack::Pop(buf);
        }
    else
        {
        *aBuf = KNullDesC8().AllocL();
        }
    return found;
    }

// -----------------------------------------------------------------------------
// CNATFWNATSettings::ReadBoolValue
// -----------------------------------------------------------------------------
//
TBool CNATFWNATSettings::ReadBoolValue(
    CRepository& aRepository,
    const TUint32& aKey,
    TBool& aKeyValue)
    {
    TBool found = EFalse;
    TInt tmp = 0;
    if (aRepository.Get(aKey,tmp) == KErrNone)
        {
        aKeyValue = (tmp != 0);
        found = ETrue;
        }
    return found;
    }

// -----------------------------------------------------------------------------
// CNATFWNATSettings::Read
// -----------------------------------------------------------------------------
//
TBool CNATFWNATSettings::Read(
    CRepository& aRepository,
    const TUint32& aKey,
    TUint& aKeyValue)
    {
    TBool found = EFalse;
    TInt tmp;
    if (aRepository.Get(aKey,tmp) == KErrNone)
        {
        aKeyValue = static_cast<TUint>(tmp);
        found = ETrue;
        }
    return found;
    }

// -----------------------------------------------------------------------------
// CNATFWNATSettings::CreateNewKeyL
// -----------------------------------------------------------------------------
//  
TUint32 CNATFWNATSettings::CreateNewKeyL( 
    CRepository& aRepository,
    const TUint32 aSettingKeyMask, 
    const TUint32 aField) const
    {
    TUint32 newKey = 0;
    RArray<TUint32> keys;
    CleanupClosePushL( keys );
    TInt err = aRepository.FindL( aSettingKeyMask,
                                  aField,
                                  keys );
    TInt keyCount = keys.Count();
    
    if ( err == KErrNotFound )
        {
        newKey = aField+1;
        }
    else
        {
        User::LeaveIfError( err );
        if ( keyCount == 0 )
            {
            newKey = aField+1;
            }
        else
            {
            // Find the biggest key and increment it by one
            keys.SortUnsigned();
            TUint32 maxKey = aSettingKeyMask^keys[ keyCount - 1 ];  
            newKey = aField + 1 + maxKey;
            }
        }
    newKey |= aField; 
    newKey ^= aField;
    CleanupStack::PopAndDestroy( &keys );
    return newKey;
    }

// -----------------------------------------------------------------------------
// CNATFWNATSettings::WriteL
// -----------------------------------------------------------------------------
//
void CNATFWNATSettings::WriteL(
    TUint32 aFieldMask,
    TInt aValue)
    {
    User::LeaveIfError(
        iCenRep->StartTransaction(
            CRepository::EConcurrentReadWriteTransaction));
    iCenRep->CleanupCancelTransactionPushL();

    TInt tmp = 0;
    if (iCenRep->Get(aFieldMask,tmp) == KErrNone)
        {
        // Update existing value
        User::LeaveIfError(iCenRep->Set(aFieldMask,aValue));
        }
    else
        {
        // Create new value
        User::LeaveIfError(iCenRep->Create(aFieldMask,aValue));
        } 
    TUint32 dummy;
    User::LeaveIfError(iCenRep->CommitTransaction(dummy));
    CleanupStack::Pop(); // transaction
    }

// -----------------------------------------------------------------------------
// CNATFWNATSettings::WriteL
// -----------------------------------------------------------------------------
//
void CNATFWNATSettings::WriteL(
    TUint32 aFieldMask,
    const TDesC8& aValue)
    {
    if(aFieldMask == KUNSAFProtocolsDomainMask)
        {
        User::Leave(KErrArgument);
        }
    User::LeaveIfError(
            iCenRep->StartTransaction(
            CRepository::EConcurrentReadWriteTransaction));
            
    iCenRep->CleanupCancelTransactionPushL();
    
    TBuf8<1> tmp;
    TInt err = iCenRep->Get(aFieldMask,tmp);
    if ( err == KErrNone || err == KErrOverflow)
        {
        // Update existing value
        User::LeaveIfError(iCenRep->Set(aFieldMask,aValue));
        }
    else
        {
        // Create new value
        User::LeaveIfError(iCenRep->Create(aFieldMask,aValue));
        }
    TUint32 dummy;
    User::LeaveIfError(iCenRep->CommitTransaction(dummy));
    CleanupStack::Pop(); // transaction
    }*/

// -----------------------------------------------------------------------------
// TNATPortField::TNATPortField
// -----------------------------------------------------------------------------
//
/*
TNATPortField::TNATPortField()
    {
    iCRKeyMask = 0;
    iLength = 0;
    iStartPort = 0;
    iEndPort = 0;
    }
*/
//End of file
