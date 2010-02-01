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


#ifndef CNATTRAVERSALCONFIG_H
#define CNATTRAVERSALCONFIG_H

//  INCLUDES
#include <e32base.h>
#include <centralrepository.h>

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
* A class for retrieving NAT traversal configuration for a domain.
* @lib nattraversalcontroller.lib
*/
class CNATTraversalConfig : public CBase
    {
    public: // Enumerations

	// Values for KUNSAFProtocolsDomainEnableCRLFRefresh parameter
    enum TRefreshValue
        {
        EDisabled  = 0, // Refresh disabled in repository
        EEnabled   = 1, // Refresh enabled in repository
        EUndefined = 2  // Parameter not present in repository
        };

    public:  // Constructors and destructor

        static CNATTraversalConfig* NewL(
            TUint32 aIapId,
            const TDesC8& aDomain);

        static CNATTraversalConfig* NewLC(
            TUint32 aIapId,
            const TDesC8& aDomain);

        ~CNATTraversalConfig();

    public: // New functions

        TUint32 IapId() const;
        const TDesC8& Domain() const;
        const TDesC8& STUNServer() const;
        TUint STUNServerPort() const;
        TInt UDPRefreshInterval() const;
        TInt TCPRefreshInterval() const;
        TInt STUNRetransmitTimer() const;
        TBool CRLFRefreshEnabledToProxy(TUint aProtocol) const;
        CNATTraversalConfig::TRefreshValue
        	CRLFRefreshEnabledToProxyValue() const;
        TBool SharedSecretNotSupported() const;
        void SetSharedSecretNotSupportedL(TBool aNotSupported=ETrue);
        TBool BindingRequestFailed() const;
        void SetBindingRequestFailedL(TBool aFailed=ETrue);
        const TDesC8& Username() const;
        const TDesC8& Password() const; 

    private: // Second phase constructors

        CNATTraversalConfig(TUint32 aIapId);
        void ConstructL(const TDesC8& aDomain);

    private: // New functions

        void ReadDomainSettingsL(
            CRepository& aRepository,
            const TDesC8& aDomain);

        void ReadIapSettingsL(
            CRepository& aRepository,
            TUint32 aIapId);

		void ReadCRLFRefreshEnabled(CRepository& aRepository,
									const TUint32 aKey);

        void ReadBoolValue(CRepository& aRepository,
                           const TUint32& aKey,
                           TBool& aKeyValue) const;

        TBool Read(CRepository& aRepository,
                   const TUint32& aKey,
                   TInt& aKeyValue) const;

        void WriteL(TUint32 aFieldMask, TBool aValue);

        TUint32 CreateNewKeyL(CRepository& aRepository,
                              TUint32 aKey,
                              TUint32 aTable) const;

    private: // Data

        TUint32 iIapId;
        CRepository* iCenRep;
        HBufC8* iDomain;
        TUint32 iDomainKey;
        HBufC8* iSTUNServer;
        TUint iSTUNServerPort;
        TInt iUDPRefreshInterval;
        TInt iTCPRefreshInterval;
        TInt iSTUNRetransmitTimer;
        TRefreshValue iCRLFRefreshEnabled;
        TBool iSharedSecretNotSupported;
        TBool iBindingRequestFailed;
        HBufC8* iUsername;
        HBufC8* iPassword;

    friend class UT_CNATTraversalSession;
    friend class UT_CNATTraversalController;
    friend class UT_CNATTraversalConfig;
    };

#endif // CNATTRAVERSALCONFIG_H

// End of File
