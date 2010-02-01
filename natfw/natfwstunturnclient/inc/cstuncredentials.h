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
* Description:    
*
*/




#ifndef C_CSTUNCREDENTIALS_H
#define C_CSTUNCREDENTIALS_H

// INCLUDES
#include <e32base.h>


// CLASS DECLARATION

/**
 *  This class represents the credentials obtained either from a STUN server
 *  with a Shared Secret Request, or set directly by application.
 */
class CSTUNCredentials : public CBase
    {
    public: // Constructors and destructor

        /**
         * Creates a new instance of CSTUNCredentials.
         * @param aUsername Username
         * @param aPassword Password
         * @return A new instance of CSTUNCredentials, ownership is transferred.
         */
        static CSTUNCredentials*
            NewL( const TDesC8& aUsername, const TDesC8& aPassword );

        /**
         * Destructor
         */
        ~CSTUNCredentials();    

    public: // New functions

        /**
         * Get the username.
         * @return Username
         */
        const TDesC8& Username() const;

        /**
         * Get the password.        
         * @return Password
         */
        const TDesC8& Password() const;
        
        /**
         * Compares the username and password to given values.
         * @param aUsername Username to compare with
         * @param aPassword Password to compare with
         * @return ETrue Credentials match with parameters, EFalse otherwise
         */
        TBool Compare( const TDesC8& aUsername, 
            const TDesC8& aPassword ) const;

    private: // Constructors, for internal use

        /**
         * Constructor
         */
        CSTUNCredentials();
        
        /**
         * Copy constructor
         */
        CSTUNCredentials( const CSTUNCredentials& aCredentials );

        /**
         * Second phase constructor.
         *
         * @param aUsername  Username
         * @param aPassword  Password
         */
        void ConstructL( const TDesC8& aUsername, 
            const TDesC8& aPassword );

    private: // Data

        /**
         * Username to use in the authentication process.
         * own.
         */
        HBufC8* iUsername;
        
        /**
         * Password is the shared secret.
         * own.
         */
        HBufC8* iPassword;

    };

#endif // C_CSTUNCREDENTIALS_H
