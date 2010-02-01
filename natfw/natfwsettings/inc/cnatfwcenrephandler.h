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
* Description:    Class handles Central Repository use.
*
*/




#ifndef C_NATFWREPOSITORYHANDLER_H
#define C_NATFWREPOSITORYHANDLER_H


#include <e32base.h>

class CRepository;


/**
 *  Class provides methods for make easier the usage of Central Repository.
 *
 *  @lib natsettings.lib
 *  @since S60 v3.2
 */
class CNATFWCenRepHandler : public CBase
    {
    
public:

    /**
     * Two-phased constructor.
     */
    static CNATFWCenRepHandler* NewL( const TUid& aRepositoryUid );
    
    /**
     * Destructor.
     */
    virtual ~CNATFWCenRepHandler();
    
    /**
     * Create new Central Repository key for setting
     * if settings haves already keys it increase the biggest key by one
     *
     * example: CR have keys 10010001 and 10010002.
     * aSettingKeyMask = 10010000, aFieldMask = fffff000
     * return value = 00000003.
     *
     * @since S60 3.2
     * @param aSettingKeyMask Keymask for the setting 
     * @param aFieldMask Field for the setting keymasks
     * @return New CR key
     * @leave KErrNone if successful or system wide error code from CR
     */
    TUint32 CreateNewKeyL( const TUint32 aSettingKeyMask,
                           const TUint32 aFieldMask ) const;
    
    /**
     * Reads domain settings key by using domain name for search.
     *
     * @since S60 3.2
     * @param aDomain Domain name which is searched from
     *        the Central Repository
     * @param aDomainKey Returns discovered CR key
     * @return KErrNone if successful, KErrCorrupt if more than one
     *         key found or System wide error code from CR
     */ 
    TInt ReadDomainSettingsKey(
        const TDesC8& aDomain, TUint32& aDomainKey ) const;
    
    /**
     * Reads IAP settings key from Central Repository
     * using IAP ID for search.
     *
     * @since S60 3.2
     * @param aIapId Which is searched from Central Repository
     * @param aIapKey Returns discovered CR key
     * @return KErrNone if successful, KErrCorrupt if more than
     *         one key found or System wide error code from CR
     */ 
    TInt ReadIapSettingsKey( TInt aIapId, TUint32& aIapKey ) const;
    
    /**
     * Finds and returns server keys from given partialId.
     *
     * @since S60 3.2
     * @param aPartialId Contains a bit pattern that all the keys returned
     *        must at least partially match
     * @param aFoundKeys All the keys found
     * @return KErrNone if successful or system wide error code from CR
     */
    TInt FindServerKeys(
        TUint32 aPartialId, RArray<TUint32>& aFoundKeys ) const;
    
    /**
     * Reads boolean value from the Central Repository
     *
     * @since S60 3.2
     * @param aKey ID wanted to be read
     * @param aKeyValue Read value
     * @return KErrNone if successful, KErrNotFound if the setting does not
     *         exist or system wide error code
     */
    TInt ReadBoolValue( const TUint32& aKey, TBool& aKeyValue ) const;
    
    /**
     * Reads TUint value from the Central Repository
     *
     * @since S60 3.2
     * @param aKey ID wanted to be read
     * @param aKeyValue Read value
     * @return KErrNone if successful, KErrNotFound if the setting does not
     *         exist or system wide error code
     */
    TInt Read( const TUint32& aKey, TUint& aKeyValue ) const;
    
    /**
     * Reads TInt value from the Central Repository
     *
     * @since S60 3.2
     * @param aKey ID wanted to be read
     * @param aKeyValue Read value
     * @return KErrNone if successful, KErrNotFound if the setting does not
     *         exist or system wide error code
     */
    TInt Read( const TUint32& aKey, TInt& aKeyValue ) const;
    
    /**
     * Reads descriptor from the Central Repository
     * 
     * @since S60 3.2
     * @param aKey ID wanted to be read
     * @return CR data, NULL if not found
     * @leave KErrNoMemory if memory allocation fails
     */
    HBufC8* ReadL( const TUint32& aKey ) const;
    
private:

    CNATFWCenRepHandler();

    void ConstructL( const TUid& aRepositoryUid );

private: // data

    /**
     * Central Repository
     * Own.
     */     
    CRepository* iCenRep;
    
    };

#endif // C_NATFWREPOSITORYHANDLER_H
