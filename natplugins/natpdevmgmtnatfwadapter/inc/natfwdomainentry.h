/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Device Management NAT-FW Traversal domain specific settings
*
*/


//  INCLUDES
#include <e32base.h>


/*
* CNATFWDomainEntry
* Buffer class for holding domain specific NAT/FW settings
*/
class CNATFWDomainEntry : public CBase
    {
    public:  // Constructors and destructor

        static CNATFWDomainEntry* NewL();
        static CNATFWDomainEntry* NewLC();
        virtual ~CNATFWDomainEntry();

    private:

        CNATFWDomainEntry();
        void ConstructL();

    public: // New methods

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
         * Add STUN server address.
         * @since S60 3.2
         * @param aStunSrvAddr STUN server address.
         */
        void AddStunSrvAddrL( const TDesC8& aStunSrvAddr );

        /**
         * Add STUN server port.
         * @since S60 3.2
         * @param aStunSrvPort STUN server port.
         */
        void AddStunSrvPortL( TInt aStunSrvPort );

        /**
         * Add STUN server username.
         * @since S60 3.2
         * @param aStunUsername STUN server username.
         */
        void AddStunUsernameL( const TDesC8& aStunUsername );

        /**
         * Add STUN server password.
         * @since S60 3.2
         * @param aStunPassword STUN server password.
         */
        void AddStunPasswordL( const TDesC8& aStunPassword );

        /**
         * Setter for DisableStunSharedSecret.
         * @since S60 3.2
         * @param aDisableStunSharedSecret Value to be set.
         */
        void SetDisableStunSharedSecret( TInt aDisableStunSharedSecret );

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
         * Setter for NATUtilities.
         * @since S60 3.2
         * @param aNatUtilities NAT utilities to be se.
         */
        void SetNatUtilitiesL( const TDesC8& aNatUtilities );

        /**
         * Getter for HostPreference.
         * @since S60 3.2
         * @param aHostPref Preference value to be set.
         */
        void SetHostPref( TInt aHostPref );

        /**
         * Getter for ServerReflexPreference.
         * @since S60 3.2
         * @param aServerReflexPref Preference value to be set.
         */
        void SetServerReflexPref( TInt aServerReflexPref );

        /**
         * Getter for RelayPreference.
         * @since S60 3.2
         * @param aRelayPref Preference value to be set.
         */
        void SetRelayPref( TInt aRelayPref );

        /**
         * Getter for PeerReflexPreference.
         * @since S60 3.2
         * @param aPeerReflexPref Preference value to be set.
         */
        void SetPeerReflexPref( TInt aPeerReflexPref );

        /**
         * Getter for IPv4Preference.
         * @since S60 3.2
         * @param aIPv4Pref Preference value to be set.
         */
        void SetIPv4Pref( TInt aIPv4Pref );

        /**
         * Getter for IPv6Preference.
         * @since S60 3.2
         * @param aIPv6Pref Preference value to be set.
         */
        void SetIPv6Pref( TInt aIPv6Pref );

        /**
         * Getter for VpnPreference.
         * @since S60 3.2
         * @param aVpnPref Preference value to be set.
         */
        void SetVpnPref( TInt aVpnPref );

        /**
         * Getter for UdpPreference.
         * @since S60 3.2
         * @param aUdpPref Preference value to be set.
         */
        void SetUdpPref( TInt aUdpPref );

        /**
         * Getter for TcpPreference.
         * @since S60 3.2
         * @param aTcpPref Preference value to be set.
         */
        void SetTcpPref( TInt aTcpPref );

        /**
         * Getter for TcpActivePreference.
         * @since S60 3.2
         * @param aTcpActivePref Preference value to be set.
         */
        void SetTcpActivePref( TInt aTcpActivePref );

        /**
         * Getter for TcpPassivePreference.
         * @since S60 3.2
         * @param aTcpPassivePref Preference value to be set.
         */
        void SetTcpPassivePref( TInt aTcpPassivePref );

        /**
         * Setter for TcpSimultPreference.
         * @since S60 3.2
         * @param aTcpSimultPref Preference value to be set.
         */
        void SetTcpSimultPref( TInt aTcpSimultPref );

        /**
         * Add TURN server address.
         * @since S60 3.2
         * @param aTurnSrvAddr TURN server address.
         */
        void AddTurnSrvAddrL( const TDesC8& aTurnSrvAddr );

        /**
         * Add TURN server port.
         * @since S60 3.2
         * @param aTurnSrvPort TURN server port.
         */
        void AddTurnSrvPortL( TInt aTurnSrvPort );

        /**
         * Add TURN server username.
         * @since S60 3.2
         * @param aTurnSrvAddr TURN server username.
         */
        void AddTurnUsernameL( const TDesC8& aTurnUsername );

        /**
         * Add TURN server password.
         * @since S60 3.2
         * @param aTurnPassword TURN server password.
         */
        void AddTurnPasswordL( const TDesC8& aTurnPassword );

        /**
         * Update STUNSrvAddr in array.
         * @since S60 3.2
         * @param aIndex Array index.
         * @param aAddr New STUN server address.
         * @return KErrNone if successful,
         *         KErrArgument if aIndex negative or too big.
         */
        TInt UpdateStunSrvAddrL( TInt aIndex, const TDesC8& aAddr );

        /**
         * Update STUNSrvPort in array.
         * @since S60 3.2
         * @param aIndex Array index.
         * @param aPort New STUN server port.
         * @return KErrNone if successful,
         *         KErrArgument if aIndex negative or too big.
         */
        TInt UpdateStunSrvPort( TInt aIndex, TInt aPort );

        /**
         * Update STUNUsername in array.
         * @since S60 3.2
         * @param aIndex Array index.
         * @param aUsername New STUN server username.
         * @return KErrNone if successful,
         *         KErrArgument if aIndex negative or too big.
         */
        TInt UpdateStunUsernameL( TInt aIndex, const TDesC8& aUsername );

        /**
         * Update STUNPassword in array.
         * @since S60 3.2
         * @param aIndex Array index.
         * @param aPassword New STUN server password.
         * @return KErrNone if successful,
         *         KErrArgument if aIndex negative or too big.
         */
        TInt UpdateStunPasswordL( TInt aIndex, const TDesC8& aPassword );

        /**
         * Update TURN server address in array.
         * @since S60 3.2
         * @param aIndex Array index.
         * @param aAddr New TURN server address.
         * @return KErrNone if successful,
         *         KErrArgument if aIndex negative or too big.
         */
        TInt UpdateTurnSrvAddrL( TInt aIndex, const TDesC8& aAddr );

        /**
         * Update TURN server port in array.
         * @since S60 3.2
         * @param aIndex Array index.
         * @param aPort New TURN server port.
         * @return KErrNone if successful,
         *         KErrArgument if aIndex negative or too big.
         */
        TInt UpdateTurnSrvPort( TInt aIndex, TInt aPort );

        /**
         * Update TURN server username in array.
         * @since S60 3.2
         * @param aIndex Array index.
         * @param aUsername New TURN server username.
         * @return KErrNone if successful,
         *         KErrArgument if aIndex negative or too big.
         */
        TInt UpdateTurnUsernameL( TInt aIndex, const TDesC8& aUsername );

        /**
         * Update TURN server password in array.
         * @since S60 3.2
         * @param aIndex Array index.
         * @param aPassword New TURN server password.
         * @return KErrNone if successful,
         *         KErrArgument if aIndex negative or too big.
         */
        TInt UpdateTurnPasswordL( TInt aIndex, const TDesC8& aUsername );

        /**
         * Remove STUNSrvAddr from array.
         * @since S60 3.2
         * @param aIndex Array index.
         * @return KErrNone if successful,
         *         KErrArgument if aIndex negative or too big.
         */
        TInt RemoveStunSrvAddr( TInt aIndex );

        /**
         * Remove STUNSrvPort from array.
         * @since S60 3.2
         * @param aIndex Array index.
         * @return KErrNone if successful,
         *         KErrArgument if aIndex negative or too big.
         */
        TInt RemoveStunSrvPort( TInt aIndex );

        /**
         * Remove STUNUsername from array.
         * @since S60 3.2
         * @param aIndex Array index.
         * @return KErrNone if successful,
         *         KErrArgument if aIndex negative or too big.
         */
        TInt RemoveStunUsername( TInt aIndex );

        /**
         * Remove STUNPassword from array.
         * @since S60 3.2
         * @param aIndex Array index.
         * @return KErrNone if successful,
         *         KErrArgument if aIndex negative or too big.
         */
        TInt RemoveStunPassword( TInt aIndex );

        /**
         * Remove TURN server address from array.
         * @since S60 3.2
         * @param aIndex Array index.
         * @return KErrNone if successful,
         *         KErrArgument if aIndex negative or too big.
         */
        TInt RemoveTurnSrvAddr( TInt aIndex );

        /**
         * Remove TURN server port from array.
         * @since S60 3.2
         * @param aIndex Array index.
         * @return KErrNone if successful,
         *         KErrArgument if aIndex negative or too big.
         */
        TInt RemoveTurnSrvPort( TInt aIndex );

        /**
         * Remove TURN server username from array.
         * @since S60 3.2
         * @param aIndex Array index.
         * @return KErrNone if successful,
         *         KErrArgument if aIndex negative or too big.
         */
        TInt RemoveTurnUsername( TInt aIndex );

        /**
         * Remove TURN server password from array.
         * @since S60 3.2
         * @param aIndex Array index.
         * @return KErrNone if successful,
         *         KErrArgument if aIndex negative or too big.
         */
        TInt RemoveTurnPassword( TInt aIndex );

        /**
         * Getter for DomainKey.
         * @since Series 60_3.1
         * @return TUint32. Key for domain table in CenRep.
         */
        TUint32 GetDomainKey() const;

        /**
         * Getter for Domain.
         * @since Series 60_3.1
         * @return TDesC8.
         */
        const TDesC8& GetDomain() const;

        /**
         * Getter for STUNServerAddr.
         * @since Series 60_3.1
         * @return TDesC8.
         */           
        const TDesC8& GetSTUNAddr() const;

        /**
         * Getter for STUNServerPort.
         * @since Series 60_3.1
         * @return TUint.
         */     
        TInt GetSTUNPort() const;

        /**
         * Getter for NATRefreshTCP.
         * @since Series 60_3.1
         * @return TInt.
         */
        TInt GetNATRefreshTCP() const;

        /**
         * Getter for NATRefreshUDP.
         * @since Series 60_3.1
         * @return TInt.
         */
        TInt GetNATRefreshUDP() const;

        /**
         * Getter for aCRLFRefresh.
         * @since Series 60_3.1
         * @return TInt.
         */
        TInt GetEnableCRLFRefresh() const;

        /**
         * Getter for STUNUsername.
         * @since S60 3.2
         * @return TDesC8.
         */           
        const TDesC8& GetSTUNUsername() const;

        /**
         * Getter for STUNPassword.
         * @since S60 3.2
         * @return TDesC8.
         */           
        const TDesC8& GetSTUNPassword() const;

        /**
         * Getter for a specific STUN server address.
         * @since S60 3.2
         * @param aIndex Array index.
         * @return STUN server address or "NotSet" if aIndex out of bounds.
         */
        const TDesC8& StunSrvAddrL( TInt aIndex ) const;

        /**
         * Getter for a specific STUN server port.
         * @since S60 3.2
         * @param aIndex Array index.
         * @return STUN server port or KErrNotFound if aIndex out of bounds.
         */
        TInt StunSrvPort( TInt aIndex ) const;

        /**
         * Getter for a specific STUN server username.
         * @since S60 3.2
         * @param aIndex Array index.
         * @return STUN server username or "NotSet" if aIndex out of bounds.
         */
        const TDesC8& StunUsernameL( TInt aIndex ) const;

        /**
         * Getter for a specific STUN server password.
         * @since S60 3.2
         * @param aIndex Array index.
         * @return STUN server password or "NotSet" if aIndex out of bounds.
         */
        const TDesC8& StunPasswordL( TInt aIndex ) const;

        /**
         * Getter for DisableStunSharedSecret.
         * @since S60 3.2
         * @return Value for STUN_SHARED_SECRET.
         */           
        TInt DisableStunSharedSecret() const;

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
         * Getter for NATUtilities.
         * @since S60 3.2
         * @return NAT utilities.
         */
        const TDesC8& NatUtilities () const;

        /**
         * Getter for HostPreference.
         * @since S60 3.2
         * @return HostPreference value.
         */
        TInt HostPref() const;

        /**
         * Getter for ServerReflexPreference.
         * @since S60 3.2
         * @return ServerReflexPreference value.
         */
        TInt ServerReflexPref() const;

        /**
         * Getter for RelayPreference.
         * @since S60 3.2
         * @return RelayPreference value.
         */
        TInt RelayPref() const;

        /**
         * Getter for PeerReflexPreference.
         * @since S60 3.2
         * @return PeerReflexPreference value.
         */
        TInt PeerReflexPref() const;

        /**
         * Getter for IPv4Preference.
         * @since S60 3.2
         * @return IPv4Preference value.
         */
        TInt IPv4Pref() const;

        /**
         * Getter for IPv6Preference.
         * @since S60 3.2
         * @return IPv6Preference value.
         */
        TInt IPv6Pref() const;

        /**
         * Getter for VpnPreference.
         * @since S60 3.2
         * @return VpnPreference value.
         */
        TInt VpnPref() const;

        /**
         * Getter for UdpPreference.
         * @since S60 3.2
         * @return UdpPreference value.
         */
        TInt UdpPref() const;

        /**
         * Getter for TcpPreference.
         * @since S60 3.2
         * @return TcpPreference value.
         */
        TInt TcpPref() const;

        /**
         * Getter for TcpActivePreference.
         * @since S60 3.2
         * @return TcpActivePreference value.
         */
        TInt TcpActivePref() const;

        /**
         * Getter for TcpPassivePreference.
         * @since S60 3.2
         * @return TcpPassivePreference value.
         */
        TInt TcpPassivePref() const;

        /**
         * Getter for TcpSimultPreference.
         * @since S60 3.2
         * @return TcpSimultPreference value.
         */
        TInt TcpSimultPref() const;

        /**
         * Getter for a specific TURN server address.
         * @since S60 3.2
         * @param aIndex Array index.
         * @return TURN server address or "NotSet" if aIndex out of bounds.
         */
        const TDesC8& TurnSrvAddrL( TInt aIndex ) const;

        /**
         * Getter for a specific TURN server port.
         * @since S60 3.2
         * @param aIndex Array index.
         * @return TURN server port or KErrNotFound if aIndex out of bounds.
         */
        TInt TurnSrvPort( TInt aIndex ) const;

        /**
         * Getter for a specific TURN server username.
         * @since S60 3.2
         * @param aIndex Array index.
         * @return TURN server username or "NotSet" if aIndex out of bounds.
         */
        const TDesC8& TurnUsernameL( TInt aIndex ) const;

        /**
         * Getter for a specific TURN server password.
         * @since S60 3.2
         * @param aIndex Array index.
         * @return TURN server password or "NotSet" if aIndex out of bounds.
         */
        const TDesC8& TurnPasswordL( TInt aIndex ) const;

private: // Data

        // Identifier of the codec.
        TUint32 iDomainKey;

        // Domain address
        HBufC8* iDomain;

        // STUN server addr
        HBufC8* iSTUNServerAddr;

        // STUN server port
        TInt iSTUNServerPort;

        // NAT refresh tcp
        TInt iNATRefreshTCP;

        // NAT refresh udp
        TInt iNATRefreshUDP;

        // CRLF refresh
        TInt iEnableCRLFRefresh;

        // STUN server username
        HBufC8* iSTUNUsername;

        // STUN server password
        HBufC8* iSTUNPassword;

        /**
         * List of STUN server addresses.
         */
        RPointerArray<HBufC8> iStunSrvAddrArray;

        /**
         * List of STUN server ports.
         */
        RArray<TInt> iStunSrvPortArray;
        
        /**
         * List of STUN server usernames.
         */
        RPointerArray<HBufC8> iStunUsernameArray;

        /**
         * List of STUN server passwords.
         */
        RPointerArray<HBufC8> iStunPasswordArray;

        /**
         * Whether to disable STUN shared secret mechanism.
         */
        TInt iDisableStunSharedSecret;

        /**
         * Start port of the port range.
         */
        TInt iStartPortRange;

        /**
         * End port of the port range.
         */
        TInt iEndPortRange;

        /**
         * Used NAT protocol.
         * Own.
         */
        HBufC8* iUsedNatProtocol;

        /**
         * ICE: NAT Utilties.
         * Own.
         */
        HBufC8* iNatUtilities;

        /**
         * ICE: Host preference
         */
        TInt iHostPref;

        /**
         * ICE: Server-reflexive preference
         */
        TInt iServerReflexPref;

        /**
         * ICE: Relay preference
         */
        TInt iRelayPref;

        /**
         * ICE: Peer-reflexive preference
         */
        TInt iPeerReflexPref;

        /**
         * ICE: IPv4 preference
         */
        TInt iIPv4Pref;

        /**
         * ICE: IPv6 preference
         */
        TInt iIPv6Pref;

        /**
         * ICE: VPN preference
         */
        TInt iVpnPref;

        /**
         * ICE: UDP preference
         */
        TInt iUdpPref;

        /**
         * ICE: TCP preference
         */
        TInt iTcpPref;

        /**
         * ICE: TCP-active preference
         */
        TInt iTcpActivePref;

        /**
         * ICE: TCP-passive preference
         */
        TInt iTcpPassivePref;

        /**
         * ICE: TCP-simultaneous-open preference
         */
        TInt iTcpSimultPref;

        /**
         * List of TURN server addresses.
         */
        RPointerArray<HBufC8> iTurnSrvAddrArray;

        /**
         * List of TURN server ports.
         */
        RArray<TInt> iTurnSrvPortArray;

        /**
         * List of TURN server usernames.
         */
        RPointerArray<HBufC8> iTurnUsernameArray;

        /**
         * List of TURN server passwords.
         */
        RPointerArray<HBufC8> iTurnPasswordArray;

        /**
         * Uncomment this for unit testing.
         */
        //friend class UT_CNSmlDmNATFWTraversalAdapter;
    };