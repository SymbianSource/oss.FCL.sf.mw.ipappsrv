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
* Description:    Stores STUN server related data for plug-in's internal use
*
*/




#ifndef CNATFWSTUNSERVERSETTINGS_H
#define CNATFWSTUNSERVERSETTINGS_H

#include <e32base.h>

/**
 *  Encapsulates STUN server related data.
 *
 *  @lib  stunplugin.dll
 *  @since S60 v3.2
 */
class CStunServerSettings : public CBase
    {
    
public:

    /**
     * Two-phased constructor.
     */
    static CStunServerSettings* NewL();

    /**
     * Two-phased constructor.
     */
    static CStunServerSettings* NewLC();

    virtual ~CStunServerSettings();

public:

    /**
     * Set address
     *
     * @since S60 v3.2
     * @param aAddress Address to set
     */
     void SetAddressL( const TDesC8& aAddress );
     
    /**
     * Return address
     *
     * @since S60 v3.2
     * @return Address
     */
     const TDesC8& Address() const;

    /**
     * Set port
     *
     * @since S60 v3.2
     * @param aPort Port to set
     */
     void SetPort( TUint aPort );
     
    /**
     * Return port
     *
     * @since S60 v3.2
     * @return Port
     */
     TUint Port() const;
          
    /**
     * Set the username
     *
     * @since S60 v3.2
     * @param aUsername Username to set
     */
     void SetUsernameL( const TDesC8& aUsername );
     
    /**
     * Return username
     *
     * @since S60 v3.2
     * @return Username
     */
     const HBufC8* Username() const;
     
    /**
     * Set the password
     *
     * @since S60 v3.2
     * @param aPassword Password to set
     */
     void SetPasswordL( const TDesC8& aPassword );
     
    /**
     * Return password
     *
     * @since S60 v3.2
     * @return Password
     */
     const HBufC8* Password() const;

private:

    CStunServerSettings();


private:

    /*
     * Address.
     * Own.
     */
    HBufC8* iAddress;

    /*
     * Port
     */
    TUint iPort;
        
    /*
     * Username.
     * Own.
     */
    HBufC8* iUsername;
    
    /*
     * Password.
     * Own.
     */
    HBufC8* iPassword;
    };

#endif // CNATFWSTUNSERVERSETTINGS_H
