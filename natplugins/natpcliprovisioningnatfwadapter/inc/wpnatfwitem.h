/*
* Copyright (c) 2006 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Holds and stores NATFW items.
*
*/


#ifndef CWPNATFWITEM_H
#define CWPNATFWITEM_H

// INCLUDES
#include <e32base.h> // CBase.

// FORWARD DECLARATIONS
class CWPCharacteristic;
class CNATFWIAPSpecificItem;
class CNATFWDomainSpecificItem;
class CRepository;
class CWPStunServerItem;
class CWPTurnServerItem;

/**
 *  This class stores domain and access point specific items 
 *  to central repository using UNSAF Protocols Config API
 *  @since S60 v3.1
 */
class CWPNATFWItem : public CBase
    {
    public: // enumerations

        enum TNatProtocol
            {
            EStunProtocol = 0,
            ETurnProtocol,
            EIceProtocol
            };

    public: 

        static CWPNATFWItem* NewL();

        virtual ~CWPNATFWItem();

        /**
         * Setter for item id.
         * @since S60 v3.1
         * @param aItemId. Id number for item.
         */
        void SetItemId( TUint aItemId );

        /**
         * Setter for user viewable name of the setting.
         * @since S60 v3.1
         * @param aName. 
         */
        void SetNameL( const TDesC& aName );

        /**
         * Setter for APPREF.
         * @since S60 v3.1
         * @param aAppRef. Application reference.
         */
        void SetAppRefL( const TDesC8& aAppRef );

        /**
         * Getter for APPREF.
         * @since S60 v3.1
         * @return Appref value
         */
        const TDesC8& AppRef() const;

        /**
         * Setter for Domain specific settings.
         * Ownership transfer.
         * @since S60 v3.1
         * @param aNATFWDomainSpecificItem. Domain specific settings item.
         */
        void AddDomainSpecificL( CNATFWDomainSpecificItem* 
            aNATFWDomainSpecificItem );

        /**
         * Setter for IAP specific settings.
         * Ownership transfer.
         * @since S60 v3.1
         * @param aNATFWIAPSpecificItem. IAP specific settings item.
         */
        void AddIAPSpecificL( CNATFWIAPSpecificItem* aNATFWIAPSpecificItem );
        
        /**
         * Getter for IAP specific settings.
         * @since S60 v3.2
         * @return Reference to RPointerArray containing the IAP specific 
         *         items.
         */
        const RPointerArray<CNATFWIAPSpecificItem>& IAPItems() const;

        /**
         * Getter for user viewable name of NAT/FW settings item.
         * @since S60 v3.1
         * @return Name parameter value
         */
        const TDesC& Name() const;

        /**
         * Getter for NAT/FW item id.
         * @since S60 v3.1
         * @return Item id.
         */
        TUint ItemId() const;

        /**
         * Saves the NAT/FW Settings
         * @since S60 v3.1
         */
        void StoreL();

        /**
         * Return save data.
         * @since S60 v3.1
         * @return Ids of the Domain and iap item.
         */
        const TDesC8& SaveData() const;

        /**
         * Return profile id.
         * @since S60 v3.1
         * @return Id of the saved Domain item.
         */
        const TDesC8& SavedDomainId() const;

    private:

        void ConstructL();

        /**
         * Create ICE settings.
         * @since S60 v3.2
         * @param aRepository Target repository.
         * @param aIndex Current domain item.
         * @param aKey Domain key.
         */
        void CreateIceSettingsL( CRepository* aRepository, TInt aIndex,
            TUint aKey );
        
        /**
         * Create TURN settings.
         * @since S60 v3.2
         * @param aRepository Target repository.
         * @param aIndex Current domain item.
         * @param aKey Domain key.
         */
        void CreateTurnSettingsL( CRepository* aRepository, TInt aIndex,
            TUint aKey );
        
        /**
         * Create STUN settings.
         * @since S60 v3.2
         * @param aRepository Target repository.
         * @param aIndex Current domain item.
         * @param aKey Domain key.
         * @param aTmpSavedata Buffer for temporary save data.
         */
        void CreateStunSettingsL( CRepository* aRepository, TInt aIndex,
            TUint aKey, HBufC8* aTmpSavedata );
        
        /**
         * Create IAP settings.
         * @since S60 v3.2
         * @param aRepository Target repository.
         * @param aTmpSavedata Buffer for temporary save data.
         */
        void CreateIapSettingsL( CRepository* aRepository,
            HBufC8* aTmpSavedata );

        /**
         * Update ICE settings.
         * @since S60 v3.2
         * @param aRepository Target repository.
         * @param aIndex Current domain item.
         * @param aKey Domain key.
         */
        void UpdateIceSettingsL( CRepository* aRepository, TInt aIndex,
            TUint aKey );
        
        /**
         * Update TURN settings.
         * @since S60 v3.2
         * @param aRepository Target repository.
         * @param aIndex Current domain item.
         * @param aKey Domain key.
         */
        void UpdateTurnSettingsL( CRepository* aRepository, TInt aIndex,
            TUint aKey );
        
        /**
         * Update STUN settings.
         * @since S60 v3.2
         * @param aRepository Target repository.
         * @param aIndex Current domain item.
         * @param aKey Domain key.
         * @param aTmpSavedata Buffer for temporary save data.
         */
        void UpdateStunSettingsL( CRepository* aRepository, TInt aIndex,
            TUint aKey, HBufC8* aTmpSavedata );

    private: // data

        /**
         * Domain specific items.
         */
        RPointerArray<CNATFWDomainSpecificItem> iDomainItems;

        /**
         * IAP specific items.
         */
        RPointerArray<CNATFWIAPSpecificItem> iIAPItems; 

        /**
         * NAT/FW settings reference identity in provisioning document.
         * Own.
         */
        HBufC8* iAppRef;

        /**
         * Identifies the item.
         */
        TUint iItemId;

        /**
         * Name of NAT/FW settings visible to user when viewing the provisioning 
         * message. See OMA ProvCont document.
         * CP parameter: NAME
         * Own.
         */
        HBufC* iName;

        /**
         * Saved Domain Specific settings ID.
         * Own.
         */
        HBufC8* iSavedDomainId;

        /**
         * Whole savedata
         * Own.
         */
        HBufC8* iSaveData;
    };

#endif      // CWPNATFWITEM_H
            
// End of File
