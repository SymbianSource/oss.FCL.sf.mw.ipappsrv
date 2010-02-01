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
* Description:    Class for querying STUN/TURN server settings.
*
*/




#ifndef C_NATFWSERVERSETTINGS_H
#define C_NATFWSERVERSETTINGS_H


#include <e32base.h>
#include <mnatfwserversettings.h>

class CNATFWCenRepHandler;


/**
 * Class for querying STUN/TURN server settings.
 * 
 * @lib natfwnatsettings.lib
 * @since S60 v3.2
 */
class CNATFWServerSettings : public CBase, public MNATFWServerSettings
    {

public: // New functions

    /**
     * Two-phased constructor.
     */
    static CNATFWServerSettings* NewL(
        const TDesC8& aAddress,
        TUint aPort,
        const TDesC8& aUsername,
        const TDesC8& aPassword,
        const TBool aSharedSecretEnabled );

    /**
     * Two-phased constructor.
     */
    static CNATFWServerSettings* NewLC(
        const TDesC8& aAddress,
        TUint aPort, 
        const TDesC8& aUsername,
        const TDesC8& aPassword,
        const TBool aSharedSecretEnabled );

    /**
     * Destructor.
     */
    virtual ~CNATFWServerSettings();

    /**
     * Reads settings from Central Repository
     *
     * @since   S60 v3.2
     * @param   aServerSettingsKey ID key for server wanted to be read
     * @param   aRepHandler CR Handler
     */ 
    void ReadSettingsL( TUint32 aServerSettingsKey,
        const CNATFWCenRepHandler& aRepHandler );
    
// from MNATFWServerSettings

    /**
     * From MNATFWServerSettings
     * Returns STUN/TURN server address.
     *
     * @since   S60 v3.2
     * @return  Server address
     */
    const TDesC8& Address() const;

    /**
     * From MNATFWServerSettings
     * Returns STUN/TURN server port number.
     *
     * @since   S60 v3.2
     * @return  Port
     */
    TUint Port() const;

    /**
     * From MNATFWServerSettings
     * Returns username to the server.
     * 
     * @since   S60 v3.2
     * @return  Username to the server
     */   
    const TDesC8& Username() const;

    /**
     * From MNATFWServerSettings
     * Returns password to the server.
     *
     * @since   S60 v3.2
     * @return  Password to the server
     */   
    const TDesC8& Password() const;
    
    /**
     * From MNATFWServerSettings
     * Returns shared secred enabled
     *
     * @since   S60 v3.2
     * @return  TRUE if shared secret is enabled
     */   
    TBool SharedSecretEnabled() const;
    
private:

    CNATFWServerSettings( TUint aPort, const TBool aSecret );

    void ConstructL( const TDesC8& aAddress,
        const TDesC8& aUsername, const TDesC8& aPassword );

private: // data
    
    /**
     * Server address
     * own
     */
    HBufC8* iAddress;

    /**
     * Server username
     * own
     */
    HBufC8* iUsername;

    /**
     * Server password
     * own
     */
    HBufC8* iPassword;

    /**
     * Server port
     */
    TUint iPort;

    /**
     * Shared secret enabled
     */
    TBool iSharedSecretEnabled;
    
    };

#endif // C_NATFWSERVERSETTINGS_H
