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




#ifndef __TESTSETTINGS_H__
#define __TESTSETTINGS_H__

#include <e32base.h>

#include "settings.h" 


class CTestSettings : public CSettings
{
public:
    static CTestSettings* NewL( const TDesC8& aDomain, TUint aIapId );
    static CTestSettings* NewLC( const TDesC8& aDomain, TUint aIapId );

    virtual ~CTestSettings( );
    
public: // from CSettings
    
    virtual void CreateSettingsL();

public:

    TInt GetDomainKey( TUint32& aDomainKey );
    
protected:
    
    void ConstructL( const TDesC8& aDomain );
    CTestSettings( TUint aIapId );
           
public: //DATA
    
    TUint iIapId;
    
    HBufC8* iDomain;

    TUint32 iDomainKey;

    HBufC8* iStunServerAddr;

    HBufC8* iTurnServerAddr;
    
    TUint iStunServerPort;
    
 	TUint iTurnServerPort;

    TUint iUdpTimeOut;
    
    TUint iTcpTimeOut;

    HBufC8* iServerUsername; 
    
    HBufC8* iServerPassword;
    
    TUint iUdpRefreshInterval;

    TUint iTcpRefreshInterval;

    TUint iStunRetransmitTimer;

    TBool iUseSharedSecret;

    TUint iTurnBandwidth;

    TUint iRequestedTransport;

    HBufC8* iRequestedAddress;

    TUint iRequestedPortProps;

    TUint iTurnLifeTime;

    TUint iTurnTimerValue;

    HBufC8* iLatestConnectedStunServerAddress;
    
    HBufC8* iLatestConnectedTurnServerAddress;

    HBufC8* iEcomPlugInPriorityOrder;

    HBufC8* iIceUsedPlugIns;

    TUint iPortPoolStartPort;

    TUint iPortPoolEndPort;
    
    TBool iEnableCrlfRefresh;
    
    TUint iConnectivityCheckTimeout;
};

#endif //__GENERALSETTINGS_H__