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
* Description:    Encapsulates TURN related software settings.
*
*/




#ifndef C_CNATFWTURNSETTINGS_H
#define C_CNATFWTURNSETTINGS_H

#include <e32base.h>
#include <mnatfwturnsettings.h>

class CNATFWServerSettings;
class CNATFWCenRepHandler;


/**
 *  Class instance encapsulates TURN related software settings.
 *
 *  @lib natfwnatsettings.dll
 *  @since S60 v3.2
 */
class CNATFWTurnSettings : public CBase, public MNATFWTurnSettings
    {

public:

    /**
     * Two-phased constructor.
     */
    static CNATFWTurnSettings* NewL();

    /**
     * Two-phased constructor.
     */
    static CNATFWTurnSettings* NewLC();

    /**
     * Destructor.
     */
    virtual ~CNATFWTurnSettings();

    /**
     * Sets TURN server array, ownership is transferred
     * 
     * @since   S60 v3.2
     * @param   aTurnServers Server array
     */
    void SetTurnServersL( CArrayPtr<CNATFWServerSettings>* aTurnServers );

    /**
     * Sets retransmission timeout (RTO).
     *
     * @since   S60 v3.2
     * @param  aRto Retransmission timeout value
     */  
    void SetRetransmissionTimeout( TUint aRto );

    /**
     * Sets latest connected server address
     * 
     * @since   S60 v3.2
     * @param   aServerAddr Server address
     */
    void SetLatestConnectedServerAddrL( const TDesC8& aServerAddr );

    /**
     * Sets latest connected server port
     * 
     * @since   S60 v3.2
     * @param   aServerPort Server port
     */
    void SetLatestConnectedServerPort( TInt aServerPort );

    /**
     * Read settings from Central Repository
     * 
     * @since   S60 v3.2
     * @param   aDomainKey Key for domain settings
     * @param   aRepHandler Central Repository handler
     */
    void ReadSettingsL( TUint32 aDomainKey,
        const CNATFWCenRepHandler& aRepHandler );
    
// from base class MNATFWTurnSettings

    /**
     * From MNATFWTurnSettings.
     * Method for getting array of TURN server related settings
     * Ownership is not transferred.
     *
     * @since S60 3.2
     * @param aServers Returns array of a TURN server settings pointers
     */
    void GetTurnServerArrayL(
        RPointerArray<MNATFWServerSettings>& aServers ) const;

    /**
     * From MNATFWTurnSettings.
     * Returns initial retransmission timeout (RTO).
     *
     * @since S60 v3.2
     * @return Retransmission timeout
     */  
    TUint RetransmissionTimeout() const;
    
    /**
     * From MNATFWTurnSettings.
     * Method for getting latest successfully connected TURN server address
     *
     * @since S60 3.2
     * @return Latest successfully connected TURN server address
     */
    const TDesC8& LatestConnectedServerAddr() const;

    /**
     * From MNATFWTurnSettings.
     * Method for getting latest successfully connected TURN server port
     *
     * @since S60 3.2
     * @return latest successfully connected TURN server port
    */
    TInt LatestConnectedServerPort() const;
    
private:

    CNATFWTurnSettings();

    void ConstructL();

private: // data

    /**
     * Array of server related settings.
     */
    CArrayPtr<CNATFWServerSettings>* iServerArray;
    
    /**
     * Retransmission timeout.
     */
    TUint iRto;
    
    /**
     * Latest successfully connected TURN server address.
     */
    HBufC8* iLatestConnectedServerAddr;
    
    /**
     * Latest successfully connected TURN server port.
     */
    TUint iLatestConnectedServerPort;

    };

#endif // C_CNATFWTURNSETTINGS_H
