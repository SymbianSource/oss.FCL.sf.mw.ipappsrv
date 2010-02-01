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
* Description:    Encapsulates credentials needed with ICE-checks.
*
*/



#ifndef C_CNATFWCREDENTIAL_H
#define C_CNATFWCREDENTIAL_H

#include <e32base.h>

/**
 *  Encapsulates credentials needed with ICE connectivity checks.
 *  
 *  Credential is composed of a username and associated password that present
 *  a shared secret between local and remote agent. Username is composed of
 *  concatenated username fragments of agents A and B.
 *  
 *  Connectivity check from A to B utilizes username BFRAG:AFRAG and password
 *  BPASS. Connectivity check from B to A utilizes AFRAG:BFRAG and password
 *  APASS.
 *  
 *  @lib  natconfw.lib
 *  @since S60 v3.2
 */
class CNATFWCredentials : public CBase
    {

public:
    
    /** Defines utilization direction of the credentials. */
    enum TUtilizationDirection
        {
        /** Credential is used for validation of incoming connectivity
        check.*/
        EInbound,
        
        /** Credential is used for outgoing connectivity check forming. */
        EOutbound
        };
    
    /**
     * Two-phased constructor.
     */
    IMPORT_C static CNATFWCredentials* NewL();
    
    /**
     * Two-phased constructor.
     */
    IMPORT_C static CNATFWCredentials* NewLC();
    
    /**
     * Two-phased copy constructor.
     * @param aCredentials               The credentials to copy
     */
    IMPORT_C static CNATFWCredentials* NewL(
        const CNATFWCredentials& aCredentials );
    
    /**
     * Two-phased copy constructor.
     * @param aCredentials               The credentials to copy
     */
    IMPORT_C static CNATFWCredentials* NewLC(
        const CNATFWCredentials& aCredentials );
    
    /**
     * Destructor.
     */
    virtual ~CNATFWCredentials();
    
    /**
     * Sets stream collection identifier.
     *
     * @since   S60 v3.2
     * @param   aStreamCollectionId     Stream collection identifier
     */
    IMPORT_C void SetStreamCollectionId( TUint aStreamCollectionId );
    
    /**
     * Returns stream collection identifier.
     *
     * @since   S60 v3.2
     * @return  Stream collection identifier
     */                          
    IMPORT_C TUint StreamCollectionId() const;
    
    /**
     * Sets stream identifier.
     *
     * @since   S60 v3.2
     * @param   aStreamId     Stream identifier
     */
    IMPORT_C void SetStreamId( TUint aStreamId );
    
    /**
     * Returns stream identifier.
     *
     * @since   S60 v3.2
     * @return  Stream identifier
     */                          
    IMPORT_C TUint StreamId() const;
    
    /**
     * Returns utilization direction of these credentials.
     *
     * @since   S60 v3.2
     * @return  Utilization direction
     */
    IMPORT_C TUtilizationDirection Direction() const;
    
    /**
     * Sets utilization direction of these credentials.
     *
     * @since   S60 v3.2
     * @param   aDirection      Utilization direction
     */
    IMPORT_C void SetDirection( TUtilizationDirection aDirection );
    
    /**
     * Sets username.
     *
     * @since   S60 v3.2
     * @param   aUsername               The username
     */
    IMPORT_C void SetUsernameL( const TDesC8& aUsername );
    
    /**
     * Gets username.
     *
     * @since   S60 v3.2
     * @return  Username
     */
    IMPORT_C const TDesC8& Username() const;
    
    /**
     * Sets password to be utilized.
     *
     * @since   S60 v3.2
     * @param   aPassword               The password
     */
    IMPORT_C void SetPasswordL( const TDesC8& aPassword );
    
    /**
     * Returns password.
     *
     * @since   S60 v3.2
     * @return  Password
     */
    IMPORT_C const TDesC8& Password() const;
    
    /**
     * Implements TIdentityRelation.
     *
     * @since   S60 v3.2
     * @param   aCredential1            First credentials to compare
     * @param   aCredential2            Second credentials to compare
     */
    IMPORT_C static TBool Compare( 
        const CNATFWCredentials& aCredential1,
        const CNATFWCredentials& aCredential2 );
    
    /**
     * Makes a copy of credentials.
     *
     * @since   S60 v3.2
     * @param   aCredentials            The credentials to copy
     */
    IMPORT_C void CopyL( const CNATFWCredentials& aCredentials );
    
    IMPORT_C TBool operator==(const CNATFWCredentials& aCandidate) const;
    
    IMPORT_C TBool operator!=(const CNATFWCredentials& aCandidate) const;
    
private:
    
    CNATFWCredentials();
    
    void ConstructL();
    
    void ConstructL( const CNATFWCredentials& aCredentials );
    
private: // data
    
    /**
     * Stream collection identifier
     */
    TUint iStreamCollectionId;
    
    /**
     * Stream identifier
     */
    TUint iStreamId;
    
    /**
     * Utilization direction of the credentials
     */
    TUtilizationDirection iDirection;
    
    /**
     * Username
     * Own.
     */
    RBuf8 iUsername;
    
    /**
     * Password
     * Own.
     */
    RBuf8 iPassword;
    
    };

#endif // C_CNATFWCREDENTIAL_H
