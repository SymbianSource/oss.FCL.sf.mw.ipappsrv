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
* Description:    Implementation class
*
*/




#ifndef CNATSETTINGSIMPL_H
#define CNATSETTINGSIMPL_H


#include <e32base.h>

#include <cnatfwsettingsapi.h>

class CNATFWCenRepHandler;
class CDesC8Array;
class CNATFWIceSettings;
class CNATFWStunSettings;
class CNATFWTurnSettings;


/**
 * Implementation class for NAT FW Settings API.
 *
 * Class uses Central Repository to retrieve settings values.
 *
 * @lib natfwnatsettings.lib
 * @since S60 3.2 
 */
class CNATFWNatSettingsImpl : public CBase
    {

public:

    /**
     * A two-phase constructor.
     *
     * In initial phase domain name and internet acces point related settings
     * values are read from Central Repository.
     *
     * @since S60 3.2
     * @param aDomain The STUN/TURN server domain name
     * @return New object, ownership is transferred.
     * @leave KErrCorrupt if more than one identical domain
     *        names found at Central Repository
     */
    static CNATFWNatSettingsImpl* NewL( const TDesC8& aDomain );

    /**
     * A two-phase constructor.
     * The instance is put to the cleanup stack.
     *
     * In initial phase domain name and internet acces point related settings
     * values are read from Central Repository.
     *
     * @since S60 3.2
     * @param aDomain The STUN/TURN server domain name
     * @return New object, ownership is transferred.
     * @leave KErrCorrupt if more than one identical domain
     *        names found at Central repository
     */
     static CNATFWNatSettingsImpl* NewLC( const TDesC8& aDomain );

    /**
     * Desctructor
     *
     * @since S60 3.2
     */
    virtual ~CNATFWNatSettingsImpl();
    
    /**
     * Fetch Internet acces point settings from Central Repository
     * by specific IAP ID. Old settings are replaced if new found.
     *
     * @since S60 3.2
     * @param aIapId internet acces point ID
     * @leave KErrCorrupt if more than one identical IAP ID values
     *        found at Central Repository or other system wide error code
     */
    void RetrieveIapSettingsL( TUint aIapId );

    /**
     * Returns Domain name.
     *
     * @since S60 3.2
     * @return Domain name
     */
    const TDesC8& Domain() const;

    /**
     * Returns true if CRLF refresh is enabled.
     *
     * @since S60 3.2
     * @return True if enabled
     */
    TBool CrlfRefreshEnabled() const;
    
    /**
     * Returns true if shared secret is enabled.
     *
     * @since S60 3.2
     * @return True if used
     */
    TBool UseSharedSecret() const;

    /**
     * Returns refresh interval for UDP.
     *
     * @since S60 3.2
     * @return Refresh Interval for UDP
     */
    TInt RefreshIntervalUdp() const;

    /**
     * Returns refresh interval for TCP.
     *
     * @since S60 3.2
     * @return Refresh Interval for TCP
     */
    TInt RefreshIntervalTcp() const;

    /**
     * Returns available NAT protocols in priority order
     *
     * @since S60 3.2
     * @return Descriptor array of used NAT protocols
     */
    const CDesC8Array& AvailableNatProtocols() const;

    /**
     * Returns interface through which ICE related settings can be queried.
     * Also reads all Ice settings from Central Repository.
     * 
     * @since S60 3.2
     * @return Interface for ICE settings query
     * @leave System wide error code if CR fails
     */
    MNATFWIceSettings& IceSettingsL();

    /**
     * Returns interface through which STUN related settings can be queried.
     * Also reads all STUN settings from Central Repository.
     *
     * @since   S60 3.2
     * @return  Interface for STUN settings query
     * @leave System wide error code if CR fails
     */
    MNATFWStunSettings& StunSettingsL();

    /**
     * Returns interface through which TURN related settings can be queried.
     * Also reads all TURN settings from Central Repository.
     *
     * @since   S60 3.2
     * @return  Interface for TURN settings query
     * @leave System wide error code if CR fails
     */
    MNATFWTurnSettings& TurnSettingsL();

    /**
     * Returns port range for NAT FW via parameters.
     *
     * @since S60 3.2
     * @param aStartPort Start port of the port range
     * @param aEndPort End port of the port range
     */
    void GetPortArea( TUint& aStartPort, TUint& aEndPort ) const;

private:

    /**
     * Constructor 
     *
     * @since S60 3.2
     */
    CNATFWNatSettingsImpl();

    /**
     * Second phase Constructor
     *
     * @since S60 3.2
     * @param aDomain reference to domain name descriptor
     * @leave KErrNoMemory if out of memory or system wide error code
     */
    void ConstructL( const TDesC8& aDomain );

    /**
     * Method for reading general settings from the Central Repository.
     * Leaves if memory allocation or Central Repository reading fails.
     *
     * @since S60 3.2
     * @leave KErrNoMemory if out of memory or system wide error code if
     *        CR errors
     */
    void ReadGeneralSettingsL();

    /**
     * Method for reading internet accesspoint settings from
     * the Central Repository.
     *
     * @since S60 3.2
     * @param aIapId Internet accesspoint ID   
     * @leave KErrCorrupt if more than one contents of IAP settings found
     *        with this IAP ID. System wide error code if CR fails.
     */
    void ReadIapSettingsL( TUint32 aIapId );

private: // data

    /**
     * The Central Repository handler
     * Own.
     */
    CNATFWCenRepHandler* iRepHandler;

    /**
     * DomainKey to the Central Repository contets
     * value is in the field of the firs key mask bits
     * defined in KUNSAFProtocolsFieldTypeMask
     */
    TUint32 iDomainKey;
    
    /**
     * Domain where lays the server
     * Own.
     */
    HBufC8* iDomain;

    /**
     * UDP RefreshI nterval
     */
    TUint iRefreshIntervalUdp;

    /**
     * TCP Refresh Interval
     */
    TUint iRefreshIntervalTcp;

    /**
     * CRLF Refresh Enabled
     */
    TBool iCrlfRefreshEnabled;
    
    /**
     * indicates the SharedSecret use
     */
    TBool iUseSharedSecret;

    /**
     * Array of used NAT protocols and priority order
     * Own.
     */
    CDesC8Array* iEcomPlugInPriorityOrderArray;
    
    /**
     * Starting port for the port range
     */
    TUint iPortPoolStartPort;
    
    /**
     * Ending port for the port range
     */
    TUint iPortPoolEndPort;
    
    /**
     * ICE software settings.
     * Own.
     */
    CNATFWIceSettings* iIceSettings;
    
    /**
     * STUN software settings.
     * Own.
     */
    CNATFWStunSettings* iStunSettings;
     
    /**
     * TURN software settings.
     * Own.
     */
    CNATFWTurnSettings* iTurnSettings;
    };

#endif // CNATFWNatSettingsImpl_H
