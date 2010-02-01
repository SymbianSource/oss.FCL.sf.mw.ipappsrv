/*
* Copyright (c) 2006 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  NAT-FW Traversal domain specific settings
*
*/


//  INCLUDES
#include <e32base.h>

class CRepository;
class CWPStunServerItem;
class CWPTurnServerItem;

/**
 * Buffer class for domain specific settings in NAT/FW Traversal adapter
 * @since S60 v3.1
 */
class CNATFWDomainSpecificItem : public CBase
    {
    public:  // Constructors and destructor
        /**
         * Two-phased constructor.
         */
        static CNATFWDomainSpecificItem* NewL();
        static CNATFWDomainSpecificItem* NewLC();
        
        /**
         * Destructor.
         */
        virtual ~CNATFWDomainSpecificItem();

    public: // New methods
        
        /**
         * Return profile id.
         * @since Series 60 3.0.
         * @return Id of the NATFW domain item.
         */
        const TDesC8& SaveData() const;
        
        /**
         * Setter for DomainKey.
         * @since Series 60_3.1
         * @param aDomainKey value to set. Key for domain table in CenRep.
         */
        void SetDomainKey( TUint32 aDomainKey );

        /**
         * Setter for Domain.
         * @since Series 60_3.1
         * @param aDomain value to set.
         */
        void SetDomainL( const TDesC8& aDomain );

        /**
         * Setter for STUNServerAddr.
         * @since Series 60_3.1
         * @param aSTUNServerAddr value to set.
         */
        void SetSTUNServerAddrL( const TDesC8& aSTUNServerAddr );

        /**
         * Setter for STUNServerPort.
         * @since Series 60_3.1
         * @param aSTUNServerPort value to set.
         */
        void SetSTUNServerPort( TUint aSTUNServerPort );

        /**
         * Setter for NATRefreshTCP.
         * @since Series 60_3.1
         * @param aNATRefreshTCP value to set.
         */
        void SetNATRefreshTCP( TInt aNATRefreshTCP );

        /**
         * Setter for NATRefreshUDP.
         * @since Series 60_3.1
         * @param aNATRefreshUDP value to set.
         */
        void SetNATRefreshUDP( TInt aNATRefreshUDP );

        /**
         * Setter for CRLFRefresh.
         * @since Series 60_3.1
         * @param aEnableCRLFRefresh value to set.
         */
        void SetEnableCRLFRefresh( TInt aEnableCRLFRefresh );

        /**
         * Set default domain specific setting values.
         * @since S60 3.2
         */
        void SetDefaultValuesL();

        /**
         * Setter for STUNUsername.
         * @since S60 3.2
         * @param aSTUNUsername value to set.
         */
        void SetSTUNUsernameL( const TDesC8& aSTUNUsername );

        /**
         * Setter for STUNPassword.
         * @since S60 3.2
         * @param aSTUNPassword value to set.
         */
        void SetSTUNPasswordL( const TDesC8& aSTUNPassword );

        /**
         * Setter for DisableStunSharedSecret.
         * @since S60 3.2
         * @param aStunSharedSecret Value to be set.
         */
        void SetDisableStunSharedSecret( 
            const TBool aDisableStunSharedSecret );

        /**
         * Setter for StartPortRange.
         * @since S60 3.2
         * @param aPortPoolStart value to set.
         */
        void SetStartPortRange( TInt aStartPortRange );

        /**
         * Setter for EndPortRange.
         * @since S60 3.2
         * @param aPortPoolEnd value to set.
         */
        void SetEndPortRange( TInt aEndPortRange );

        /**
         * Setter for UsedNATProtocol.
         * @since S60 3.2
         * @param aNatProtocol value to set.
         */
        void SetNatProtocolL( const TDesC8& aNatProtocol );

        /**
         * ICE: Setter for NatUtilities.
         * @since S60 3.2
         * @param aNatUtilities NAT utilities to be set.
         */
        void SetNatUtilitiesL( const TDesC8& aNatUtilities );

        /**
         * ICE: Setter for HostPreference.
         * @since S60 3.2
         * @param aHostPref HostPreference value to be set.
         */
        void SetHostPref( TInt aHostPref );

        /**
         * ICE: Setter for ServerReflexivePreference.
         * @since S60 3.2
         * @param aServerRefPref Preference value to be set.
         */
        void SetServerRefPref( TInt aServerRefPref );

        /**
         * ICE: Setter for RelayPreference.
         * @since S60 3.2
         * @param aRelayPref Preference value to be set.
         */
        void SetRelayPref( TInt aRelayPref );

        /**
         * ICE: Setter for PeerReflexivePreference.
         * @since S60 3.2
         * @param aPeerRefPref Preference value to be set.
         */
        void SetPeerRefPref( TInt aPeerRefPref );

        /**
         * ICE: Setter for IPv4Preference.
         * @since S60 3.2
         * @param aIPv4Pref Preference value to be set.
         */
        void SetIPv4Pref( TInt aIPv4Pref );

        /**
         * ICE: Setter for IPv6Preference.
         * @since S60 3.2
         * @param aIPv6Pref Preference value to be set.
         */
        void SetIPv6Pref( TInt aIPv6Pref );

        /**
         * ICE: Setter for VPNPreference.
         * @since S60 3.2
         * @param aVPNPref Preference value to be set.
         */
        void SetVpnPref( TInt aVPNPref );

        /**
         * ICE: Setter for UDPPreference.
         * @since S60 3.2
         * @param aUDPPref Preference value to be set.
         */
        void SetUdpPref(  TInt aUDPPref );

        /**
         * ICE: Setter for TCPPreference.
         * @since S60 3.2
         * @param aTCPPref Preference value to be set.
         */
        void SetTcpPref( TInt aTCPPref );

        /**
         * ICE: Setter for TCPActivePreference.
         * @since S60 3.2
         * @param aTCPActivePref Preference value to be set.
         */
        void SetTcpActivePref( TInt aTCPActivePref );

        /**
         * ICE: Setter for TCPPassivePreference.
         * @since S60 3.2
         * @param aTCPPassivePref Preference value to be set.
         */
        void SetTcpPassivePref( TInt aTCPPassivePref );

        /**
         * ICE: Setter for TCPSimultaneousOpenPreference.
         * @since S60 3.2
         * @param aTCPSimultPref Preference value to be set.
         */
        void SetTcpSimultPref( TInt aTCPSimultPref );

        /**
         * Add a STUN server item.
         *
         * @since S60 v3.2
         * @param aStunServerItem STUN server item.
         */
        void AddStunServerItemL( CWPStunServerItem* aStunServerItem );

        /**
         * Add a TURN server item.
         *
         * @since S60 v3.2
         * @param aStunServerItem STUN server item.
         */
        void AddTurnServerItemL( CWPTurnServerItem* aTurnServerItem );

        /**
         * Getter for DomainKey.
         * @since Series 60_3.1
         * @return Key for domain table in CenRep.
         */
        TUint32 GetDomainKey() const;

        /**
         * Getter for Domain.
         * @since Series 60_3.1
         * @return Domain value
         */
        const TDesC8& GetDomain() const;

        /**
         * Getter for STUNServerAddr.
         * @since Series 60_3.1
         * @return Stun server address value
         */           
        const TDesC8& GetSTUNAddr() const;

        /**
         * Getter for STUNServerPort.
         * @since Series 60_3.1
         * @return Stun server port value
         */     
        TUint GetSTUNPort() const;

        /**
         * Getter for NATRefreshTCP.
         * @since Series 60_3.1
         * @return NAT refresh for tcp value
         */
        TInt GetNATRefreshTCP() const;

        /**
         * Getter for NATRefreshUDP.
         * @since Series 60_3.1
         * @return NAT refresh for udp value
         */
        TInt GetNATRefreshUDP() const;

        /**
         * Getter for CRLFRefresh.
         * @since Series 60_3.1
         * @return CRLF refresh state
         */
        TInt GetEnableCRLFRefresh() const;

        /**
         * Getter for STUNUsername.
         * @since S60 3.2
         * @return Stun server username
         */           
        const TDesC8& GetSTUNUsername() const;

        /**
         * Getter for STUNPassword.
         * @since S60 3.2
         * @return Stun server password
         */           
        const TDesC8& GetSTUNPassword() const;

        /**
         * Getter for DisableStunSharedSecret.
         * @since S60 3.2
         * @return Value for disabling STUN shared secret.
         */           
        TBool DisableStunSharedSecret() const;

        /**
         * Getter for StartPortRange.
         * @since Series 60_3.1
         * @return Start port of port pool.
         */
        TInt StartPortRange() const;

        /**
         * Getter for EndPortRange.
         * @since S60 3.2
         * @return End port of port pool.
         */
        TInt EndPortRange() const;

        /**
         * Getter for UsedNATProtocol.
         * @since S60 3.2
         * @return Used NAT protocol.
         */
        const TDesC8& UsedNatProtocol() const;

        /**
         * Getter for NatUtilities.
         * @since S60 3.2
         * @return Used NAT utilities.
         */
        const TDesC8& NatUtilities() const;

        /**
         * ICE: Getter for HostPreference.
         * @since S60 3.2
         * @return Preference value.
         */
        TInt HostPref() const;

        /**
         * ICE: Getter for ServerReflexivePreference.
         * @since S60 3.2
         * @return Preference value.
         */
        TInt ServerRefPref() const;

        /**
         * ICE: Getter for RelayPreference.
         * @since S60 3.2
         * @return Preference value.
         */
        TInt RelayPref() const;

        /**
         * ICE: Getter for PeerReflexivePreference.
         * @since S60 3.2
         * @return Preference value.
         */
        TInt PeerRefPref() const;

        /**
         * ICE: Getter for IPv4Preference.
         * @since S60 3.2
         * @return Preference value.
         */
        TInt IPv4Pref() const;

        /**
         * ICE: Getter for IPv6Preference.
         * @since S60 3.2
         * @return Preference value.
         */
        TInt IPv6Pref() const;

        /**
         * ICE: Getter for VPNPreference.
         * @since S60 3.2
         * @return Preference value.
         */
        TInt VpnPref() const;

        /**
         * ICE: Getter for UDPPreference.
         * @since S60 3.2
         * @return Preference value.
         */
        TInt UdpPref() const;

        /**
         * ICE: Getter for TCPPreference.
         * @since S60 3.2
         * @return Preference value.
         */
        TInt TcpPref() const;

        /**
         * ICE: Getter for TCPActivePreference.
         * @since S60 3.2
         * @return Preference value.
         */
        TInt TcpActivePref() const;

        /**
         * ICE: Getter for TCPPassivePreference.
         * @since S60 3.2
         * @return Preference value.
         */
        TInt TcpPassivePref() const;

        /**
         * ICE: Getter for TCPSimultaneousOpenPreference.
         * @since S60 3.2
         * @return Preference value.
         */
        TInt TcpSimultPref() const;

        /**
         * Stores STUN server items to Central Repository.
         * @since S60 3.2
         * @param aRepository Cenral Repository object.
         * @param aKey Central Repository key used in storing.
         * @param aDomainKey Central Repository key used in finding keys.
         */
        void StoreStunServerItemsL( CRepository* aRepository, TUint32 aKey, 
            TUint32 aDomainKey );

        /**
         * Stores TURN server items to Central Repository.
         * @since S60 3.2
         * @param aRepository Cenral Repository object.
         * @param aKey Central Repository key.
         */
        void StoreTurnServerItemsL( CRepository* aRepository, TUint32 aKey );

    private:

        void ConstructL();

    private: // Data

        /**
         * Identifier of the Domain.
         */
        TUint32 iDomainKey;

        /**
         * Domain address.
         * Own.
         */
        HBufC8* iDomain;

        /**
         * STUN server addr.
         * Own.
         */
        HBufC8* iSTUNServerAddr;

        /**
         * STUN server port.
         */
        TUint iSTUNServerPort;

        /**
         * NAT refresh value for TCP.
         */
        TInt iNATRefreshTCP;

        /**
         * NAT refresh value for UDP.
         */
        TInt iNATRefreshUDP;

        /**
         * CRLF refresh value.
         */
        TInt iEnableCRLFRefresh;

        /**
         * STUN server username.
         * Own.
         */
        HBufC8* iSTUNUsername;

        /**
         * STUN server password.
         * Own.
         */
        HBufC8* iSTUNPassword;

        /**
         * Whether to use STUN shared secret mechanism. Parameter name: 
         * STUN_SHARED_SECRET.
         * See w902E.txt for more information.
         */
        TBool iDisableStunSharedSecret;

        /**
         * Start port of the port range.
         */
        TInt iStartPortRange;

        /**
         * End port of the port range.
         */
        TInt iEndPortRange;

        /**
         * ICE: NAT utilities to be used by ICE.
         * Parameter name: NATUTILITIES.
         * See w902E.txt for more details.
         */
        HBufC8* iNatUtilities;

        /**
         * ICE: Host candidate in type preference used in ICE priorization.
         * Parameter name: HOSTPREF.
         * See w902E.txt for more details.
         */
        TInt iHostPref;

        /**
         * ICE: Server reflexive candidate in type preference used in ICE 
         * priorization.
         * Parameter name: SERVERREFPREF.
         * See w902E.txt for more details.
         */
        TInt iServerRefPref;

        /**
         * ICE: Relay candidate in type preference used in ICE priorization.
         * Parameter name: RELAYPREF.
         * See w902E.txt for more details.
         */
        TInt iRelayPref;

        /**
         * ICE: Peer reflexive candidate in type preference used in ICE 
         * priorization.
         * Parameter name: PEERREFPREF.
         * See w902E.txt for more details.
         */
        TInt iPeerRefPref;

        /**
         * ICE: IPv4 part in local preference used in ICE priorization.
         * Parameter name: IPV4PREF.
         * See w902E.txt for more details.
         */
        TInt iIPv4Pref;

        /**
         * ICE: IPv6 part in local preference used in ICE priorization.
         * Parameter name: IPV6PREF.
         * See w902E.txt for more details.
         */
        TInt iIPv6Pref;

        /**
         * ICE: VPN part in local preference used in ICE priorization.
         * Parameter name: VPNPREF.
         * See w902E.txt for more details.
         */
        TInt iVPNPref;

        /**
         * ICE: The udp part in transport-pref (part of local-preference) used
         * in ICE priorization.
         * Parameter name: UDPPREF.
         * See w902E.txt for more details.
         */
        TInt iUDPPref;

        /**
         * ICE: The tcp part in transport-pref (part of local-preference) used
         * in ICE priorization.
         * Parameter name: TCPPREF.
         * See w902E.txt for more details.
         */
        TInt iTCPPref;

        /**
         * ICE: The tcp-active part in direction-pref (part of 
         * local-preference) used in ICE priorization.
         * Parameter name: TCPACTIVEPREF.
         * See w902E.txt for more details.
         */
        TInt iTCPActivePref;

        /**
         * ICE: The tcp-passive part in direction-pref (part of 
         * local-preference) used in ICE priorization.
         * Parameter name: TCPPASSIVEPREF.
         * See w902E.txt for more details.
         */
        TInt iTCPPassivePref;

        /**
         * ICE: The tcp-simultaneous-open part in direction-pref (part of 
         * local-preference) used in ICE priorization.
         * Parameter name: TCPSIMULTPREF.
         * See w902E.txt for more details.
         */
        TInt iTCPSimultPref;

        /**
         * Used NAT protocol
         * Parameter name: APROTOCOL.
         * See w902E.txt for more details.
         * Own.
         */
        HBufC8* iUsedNatProtocol;

        /**
         * An array of STUN server items.
         */
        RPointerArray<CWPStunServerItem> iStunServers;

        /**
         * An array of TURN server items.
         */
        RPointerArray<CWPTurnServerItem> iTurnServers;

		private:    // Friend class for testing
        #if defined( TEST_EUNIT )
        friend class T_NATFWTraversalAdapter;
        #endif

    };
