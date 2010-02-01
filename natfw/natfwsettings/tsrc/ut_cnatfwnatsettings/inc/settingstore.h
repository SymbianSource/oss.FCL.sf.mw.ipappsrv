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




#ifndef __SETTINGSSTORE_H__
#define __SETTINGSSTORE_H__

#include <e32base.h> 
#include <centralrepository.h>
#include <e32debug.h>

//Forward declare
class CDataDepository;
class CNATFWStunSettings;
class CNATFWTurnSettings;
class MNATFWServerSettings;

class CSettingStore : public CBase
{
    
public:
  
    static CSettingStore* NewL( const TDesC8& aDomain, TUint aIapId );
    
    static CSettingStore* NewLC( const TDesC8& aDomain, TUint aIapId );

    TInt EraseL( TUint32 aTableMask, TUint32 aFieldTypeMask );
    
    TInt ReadDomainSettingsKeyL( const TDesC8& aDomain, TUint32& aDomainKey ) const;
    
    void StoreSettingsL();
    
    TUint32 GetDomainKeyL();
    
    TUint32 CreateNewDomainL( const TDesC8& aDomain );
    
    TUint32 CreateNewIapL( const TInt aIapId );
    
    ~CSettingStore( );

private:

    CSettingStore( TUint aIapId );
    void ConstructL( const TDesC8& aDomain );
    
    TUint32 CreateServerKeyL( TUint32 aDomainKey, TUint32 aServerMask );
    
private: // DATA

    CDataDepository* iDepository;
    CRepository* iRepository;
    
public: // Data

    TUint iIapId;
    HBufC8* iDomain;
    // Domain specific root settings
    TInt iRefreshIntervalUdp;
    TInt iRefreshIntervalTcp;
    HBufC8* iAvailableNatProtocols;
    TBool iCrlfRefresEnabled;
    TBool iUseSharedSecret;
    TInt iStartPort;
    TInt iEndPort;
    // Domain specific STUN settings
    CNATFWStunSettings* iStunSettings;
    // Domain specific TURN settings
    CNATFWTurnSettings* iTurnSettings;
    // Domain specific ICE settings
    HBufC8* iUtilityPlugins;
    TInt iHostPref;
    TInt iServerReflexivePref;
    TInt iRelayPref;
    TInt iPeerReflexivePref;
    TInt iIpv4AddrFamilyPref;
    TInt iIpv6AddrFamilyPref;
    TInt iVpnInterfacePref;
    TInt iUdpTransportPref;
    TInt iTcpTransportPref;
    TInt iTcpDirActivePref;
    TInt iTcpDirPassivePref;
    TInt iTcpDirSimultaneousOpenPref;
    // Iap specific settings
    TInt iIapRefreshIntervalUdp;
    TInt iIapRefreshIntervalTcp;
    TInt iIapStunRetransmitTimer;

};

#endif //__SETTINGSSTORE_H__
