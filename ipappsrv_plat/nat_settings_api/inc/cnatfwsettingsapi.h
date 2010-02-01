/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    API class for querying NAT FW related software settings
*
*/




#ifndef C_NATSETTINGSAPI_H 
#define C_NATSETTINGSAPI_H 

#include <e32base.h>
#include <badesca.h>

class CNATFWNatSettingsImpl;
class MNATFWIceSettings;
class MNATFWStunSettings;
class MNATFWTurnSettings;


/**
 * API class to retrieve NAT FW Settings.
 *
 * Class uses Central Repository to retrieve settings values.
 *
 * @lib natfwnatsettings.lib
 * @since S60 3.2 
 */
class CNATFWNatSettingsApi : public CBase
    {
 
public:

    /**
     * A two-phase constructor.
     *
     * In initial phase domain name related settings values are read
     * from Central Repository.
     *
     * @since S60 3.2
     * @param aDomain The STUN/TURN server domain name
     * @return New object, ownership is transferred.
     * @leave KErrCorrupt if more than one identical domain
     *        names found at Central Repository.
     */
    IMPORT_C static CNATFWNatSettingsApi* NewL( const TDesC8& aDomain );
    
    /**
     * A two-phase constructor.
     * The instance is put to the cleanup stack.
     *
     * In initial phase domain name related settings values are read
     * from Central Repository.
     *
     * @since S60 3.2
     * @param aDomain The STUN/TURN server domain name
     * @return New object, ownership is transferred.
     * @leave KErrCorrupt if more than one identical domain
     *        names found at Central Repository.
     */
    IMPORT_C static CNATFWNatSettingsApi* NewLC( const TDesC8& aDomain );
    
    /**
     * Desctructor
     *
     * @since S60 3.2
     */
    virtual ~CNATFWNatSettingsApi();
    
    /**
     * Fetch Internet acces point settings from Central Repository
     * for specific IAP ID. Replaces domain specific UDP And TCP refresh
     * interval and retransmission timeout settings if new ones found.
     *
     * @since S60 3.2
     * @param aIapId The Internet acces point ID
     * @leave KErrCorrupt if more than one identical IAP ID values
     *        found or system wide error code in other CR errors
     */
    IMPORT_C void RetrieveIapSettingsL( TUint aIapId );
    
    /**
     * Returns Domain name
     *
     * @since S60 3.2
     * @return domain name
     */
    IMPORT_C const TDesC8& Domain() const;

    /**
     * Returns refresh interval for UDP.
     *
     * @since S60 3.2
     * @return Refresh Interval for UDP
     */
    IMPORT_C TInt RefreshIntervalUdp() const;
 
    /**
     * Returns refresh interval for TCP.
     *
     * @since S60 3.2
     * @return Refresh Interval for TCP
     */
    IMPORT_C TInt RefreshIntervalTcp() const;
    
    /**
     * Returns true if CRLF refresh is enabled.
     *
     * @since S60 3.2
     * @return True if enabled
     */
    IMPORT_C TBool CrlfRefreshEnabled() const;
    
    /**
     * Returns true if shared secret is enabled.
     *
     * @since S60 3.2
     * @return ETrue if used
     */
    IMPORT_C TBool UseSharedSecret() const;

    /**
     * Returns port range for NAT FW use.
     *
     * @since S60 3.2
     * @param aStartPort Start port of the port range
     * @param aEndPort End port of the port range
     */
    IMPORT_C void GetPortArea( TUint& aStartPort, TUint& aEndPort ) const;
    
    /**
     * Returns available NAT protocols in priority order
     *
     * @since S60 3.2
     * @return Descriptor array of used NAT protocols
     */ 
    IMPORT_C const CDesC8Array& AvailableNatProtocols() const;

    /**
     * Returns interface through which ICE related settings can be queried.
     * Also reads all ICE settings from Central Repository.
     * Ownership is not transferred.
     *
     * @since   S60 3.2
     * @return  Interface for ICE settings query
     * @leave System wide error code if CR fails
     */ 
    IMPORT_C MNATFWIceSettings& IceSettingsL() const;
    
    /**
     * Returns interface through which STUN related settings can be queried.
     * Also reads all STUN settings from Central Repository.
     * Ownership is not transferred.
     *
     * @since   S60 3.2
     * @return  Interface for STUN settings query
     * @leave System wide error code if CR fails
     */
    IMPORT_C MNATFWStunSettings& StunSettingsL() const;
    
    /**
     * Returns interface through which TURN related settings can be queried.
     * Also reads all TURN settings from Central Repository.
     * Ownership is not transferred.
     *
     * @since   S60 3.2
     * @return  Interface for TURN settings query
     * @leave System wide error code if CR fails
     */
    IMPORT_C MNATFWTurnSettings& TurnSettingsL() const;
    
private:
    
    /**
     * Constructor 
     */ 
    CNATFWNatSettingsApi();
    
    /**
     * Second phase Constructor
     */ 
    void ConstructL( const TDesC8& aDomain );
        
private: // data
  
    /**
     * NAT Settings implementation
     * own
     */
    CNATFWNatSettingsImpl* iSettingsImpl;
    
    };

#endif // C_NATSETTINGSAPI_H
