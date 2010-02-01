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
* Description:    Encapsulates STUN related software settings.
*
*/




#ifndef C_CNATFWSTUNSETTINGS_H
#define C_CNATFWSTUNSETTINGS_H

#include <e32base.h>
#include <mnatfwstunsettings.h>

class CNATFWServerSettings;
class CNATFWCenRepHandler;


/**
 *  Class instance encapsulates STUN related software settings.
 *
 *  @lib natfwnatsettings.dll
 *  @since S60 v3.2
 */
class CNATFWStunSettings : public CBase, public MNATFWStunSettings
    {

public:

    /**
     * Two-phased constructor.
     */
    static CNATFWStunSettings* NewL();
    
    /**
     * Two-phased constructor.
     */
    static CNATFWStunSettings* NewLC();
    
    /**
     * Destructor.
     */
    virtual ~CNATFWStunSettings();

    /**
     * Sets STUN server array, ownership is transferred
     * 
     * @since   S60 v3.2
     * @param   aStunServers Server array
     */
    void SetStunServersL( CArrayPtr<CNATFWServerSettings>* aStunServers );
    
    /**
     * Sets retransmission timeout (RTO).
     *
     * @since   S60 v3.2
     * @param  aRto Retransmission timeout value
     */  
    void SetRetransmissionTimeout( TUint aRto );
   
    /**
     * Sets latest connected STUN server address
     * 
     * @since   S60 v3.2
     * @param   aServerAddr Server address
     */
    void SetLatestConnectedServerAddrL( const TDesC8& aServerAddr );
    
    /**
     * Sets latest connected STUN server port
     * 
     * @since   S60 v3.2
     * @param   aServerPort Server port
     */
    void SetLatestConnectedServerPort( TInt aServerPort );
    
    /**
     * Read settings from Central Repository
     * 
     * @since   S60 v3.2
     * @param   aDomainKey key for domain settings
     * @param   aRepHandler Central Repository handler
     */
    void ReadSettingsL( TUint32 aDomainKey,
        const CNATFWCenRepHandler& aRepHandler );
        
// from base class MNATFWStunSettings

    /**
     * From MNATFWStunSettings.
     * Method for getting array of STUN server related settings
     * Ownership is not transferred.
     *
     * @since S60 3.2
     * @param   aServers array of a STUN server settings pointers
     */
    void GetStunServerArrayL(
        RPointerArray<MNATFWServerSettings>& aServers ) const;
   
    /**
     * From MNATFWStunSettings.
     * Returns initial retransmission timeout (RTO).
     *
     * @since   S60 v3.2
     * @return  Retransmission timeout
     */  
    TUint RetransmissionTimeout() const;
   
    /**
     * From MNATFWStunSettings.
     * Method for getting latest successfully connected STUN server address
     *
     * @since S60 3.2
     * @return latest successfully connected STUN server address
     */
    const TDesC8& LatestConnectedServerAddr() const;
    
    /**
     * From MNATFWStunSettings.
     * Method for getting latest successfully connected STUN server port
     *
     * @since S60 3.2
     * @return latest successfully connected STUN server port
    */
    TInt LatestConnectedServerPort() const;
    
private:

    CNATFWStunSettings();

    void ConstructL();

private: // data

    /**
     * Array of server related settings.
     * own.
     */
    CArrayPtr<CNATFWServerSettings>* iServerArray;
    
    /**
     * Retransmission timeout.
     */
    TUint iRto;
    
    /**
     * Latest successfully connected STUN server address.
     * own.
     */
    HBufC8* iLatestConnectedServerAddr;
    
    /**
     * Latest successfully connected STUN server port.
     */
    TUint iLatestConnectedServerPort;

    };

#endif // C_CNATFWSTUNSETTINGS_H
