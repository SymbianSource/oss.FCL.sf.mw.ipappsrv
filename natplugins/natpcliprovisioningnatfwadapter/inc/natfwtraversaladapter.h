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
* Description:  Main implementation of NAT/FW Traversal adapter
*
*/


#ifndef CNATFWTRAVERSALADAPTER_H
#define CNATFWTRAVERSALADAPTER_H

// INCLUDES
#include <CWPAdapter.h>
#include <MWPContextExtension.h>

// FORWARD DECLARATIONS
class CWPCharacteristic;
class CNATFWDomainSpecificItem;
class CNATFWIAPSpecificItem;
class CWPNATFWItem;
class CWPStunServerItem;
class CWPTurnServerItem;

/**
* Main implementation of NAT/FW Traversal adapter
* @since S60 v3.1
*/
class CNATFWTraversalAdapter :  public CWPAdapter, public MWPContextExtension
    {
    public:  // Constructors and destructor

        /**
         * Two-phased constructor.
         */
        static CNATFWTraversalAdapter* NewL();
        
        /**
         * Destructor.
         */
        virtual ~CNATFWTraversalAdapter();

    public: // Methods from base classes 
  
        /**
         * From CWPAdapter
         * @since Series60 3.1.
         * Returns the number of the summary lines to be shown in 
         * BIO control.
         * @return Quantity of items (NAT/FW settings items).
         */
        TInt  ItemCount() const;

        /**
         * From CWPAdapter
         * @since S60 v3.1.
         * Getter for the summary text of the NAT/FW item (NAT/FW settings) 
         * located in the array. Text is shown to the user in 
         * opened configuration message.
         * @param aIndex. Location of the NAT/FW item.
         * @return Summary title.
         */
        const TDesC16& SummaryTitle( TInt aIndex ) const;

        /**
         * From CWPAdapter
         * @since S60 v3.1.
         * Getter for the summary text of the NAT/FW item 
         * located in the array. Text is shown to the user in 
         * opened configuration message.
         * @param aIndex. Location of the NAT/FW item.
         * @return Summary text.
         */
        const TDesC16& SummaryText( TInt aIndex ) const;

        /**
         * From CWPAdapter
         * @since S60 v3.1.
         * Saves NAT/FW item (NAT/FW settings) located in array.
         * @param aIndex. Location of the NAT/FW item to be saved.
         */
        void SaveL( TInt aIndex );

        /**
         * From CWPAdapter
         * @since S60 v3.1.
         * Query if the NAT/FW item on place pointed by parameter can 
         * be set as default.
         * @param aIndex. Location of the NAT/FW item to be queried.
         * @return TBool.
         */
        TBool CanSetAsDefault( TInt aIndex ) const;
    
        /**
         * From CWPAdapter
         * @since S60 v3.1.
         * Sets the NAT/FW item on place pointed by parameter as default.
         * @param aIndex. Location of the NAT/FW item to be queried.
         */
        void SetAsDefaultL( TInt aIndex );

        /**
         * From CWPAdapter
         * @since S60 v3.1.
         * Query for the detail information about the NAT/FW settings. 
         * MWPPairVisitor is used for retrieving the details of a single 
         * setting entry. This is not supported feature as in the other 
         * adapters in the framework.
         * @param aItem.
         * @param aVisitor.
         * @return TInt. 
         */
        TInt DetailsL( TInt aItem, MWPPairVisitor& aVisitor );

        /**
         * From MWPVisitor
         * @since S60 v3.1.
         * Visit method. Visits the adapter. Adapter then checks from the 
         * aCharacteristic if the data is targetet to it and acts according to 
         * that.
         * @param aCharacteristic.
         */
        void VisitL( CWPCharacteristic& aCharacteristic );		

        /**
         * From MWPVisitor
         * @since S60 v3.1.
         * Visit method. Visits the adapter. Method set the iAppId value.
         * @param aParameter.
         */
        void VisitL( CWPParameter& aParameter );		

        /**
         * From MWPVisitor
         * @since S60 v3.1.
         * Visit method. Visits the adapter. Adapter checks from the 
         * aParameter if the link is targetet to it and acts according to 
         * that. IAP bound to the NAT/FW access point specific settings is 
         * handled with help of this method.
         * @param aParameter.
         */
        void VisitLinkL( CWPCharacteristic& aLink );
            
        /**
         * From CWPAdapter
         * Informs the adapters of the saved settings.
         * @since S60 v3.1.
         * @param aAppIdOfSavingItem. UID of the adapter component. 
         * @param aAppRef. APPREF of the saved settings.
         * @param aStorageIdValue. Value that identifies the settings in 
         *        its storage.
         * @param aIsLastOfItsType. If the settings saved is the last of 
         *        its kind, the value of the parameter is ETrue (or 1, because 
         *        the TBool type does not work correctly via interface class 
         *        definition (based on information from MVC documentation).
         */
        void SettingsSavedL ( const TDesC8& aAppIdOfSavingItem,
            const TDesC8& aAppRef, const TDesC8& aStorageIdValue );

        /**
         * From CWPAdapter
         * @since S60 v3.1.
         * Lets the adapters know when the saving has been finalized in 
         * viewpoint of the provisioning framework. The adapters can make 
         * finalization of saving the settings after receiving this call.
         */
        void SavingFinalizedL();
        
        /**
         * From CWPAdapter
         * @since S60 v3.1.
         * Gets the saving information from the adapter that has saved settings.
         * @param aIndex. Index of the previously saved item.
         * @param aSavingInfo.
         * @param aIsLastOfItsType. Adapter can tell to the framework if 
         * the saved item was the last one of its type.
         */
        void GetSavingInfoL( TInt aIndex, RPointerArray<HBufC8> &aSavingInfo );

        /**
         * From MWPContextExtension.
         * Returns a pointer to a context extension.
         * @param aExtension Contains a pointer to MWPContextExtension if supported
         * @return KErrNotSupported if not supported, KErrNone otherwise
         */
        TInt ContextExtension( MWPContextExtension*& aExtension );

        /**
         * From MWPContextExtension.
         * Returns the data used for saving.
         * @param aIndex The index of the data
         * @return The data. Ownership is transferred.
         */
        const TDesC8& SaveDataL( TInt aIndex ) const;

        /**
         * From CWPAdapter
         * Deletes a saved item.
         * @since S60 v3.1.
         * @param aSaveData The data used for saving
         */
        void DeleteL( const TDesC8& aSaveData );

        /**
         * Returns the UID of the adapter.
         * @return UID
         */
        TUint32 Uid() const;
        
        /**
         * Converts descriptor to TInt.
         * @param aDes Value to be converted.
         * @return Converted value.
         */
        TInt DesToInt( const TDesC8& aDes );
        
        /**
         * Converts descriptor to TUint
         * @param aDes Value to be converted.
         * @return Converted value.
         */
        TUint DesToTUint( const TDesC8& aDes );
       
    private:

        /**
        * C++ default constructor.
        */
        CNATFWTraversalAdapter();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
         * Gets IAP ID according to WAP ID.
         * @since S60 3.2
         * @param aWapId WAP ID of access point.
         * @return IAP ID of access point
         */
        TUint32 IapIdFromWapIdL( TUint32 aWapId );

    private: // Data
    
        /**
         * Current NATFW domain specific data. Ownership is transferred to
         * CWPNATFWItem.
         * Not own.
         */ 
        CNATFWDomainSpecificItem* iCurrentDomainItem;
        
        /**
         * Current NATFW iap specific data. Ownership is transferred to 
         * CWPNATFWItem.
         * Not own.
         */
        CNATFWIAPSpecificItem* iCurrentIapItem;
        
        /**
         * Current NATFW item setting data.
         * Own.
         */
        CWPNATFWItem* iCurrentNATFWItem;
        
        /**
         * The application id of the current characteristic. 
         * Not own.
         */
        TPtrC iAppID;
 
	    /**
         * Default name for NAT/FW settings. Value is read from the resource file.
         * Own.
         */
        HBufC* iDefaultName;

        /**
         * Title for NAT/FW settings. Value is read from the resource file.
         * Own.
         */
        HBufC* iTitle;

        /**
         * Holds the received NAT/FW item data.
         * Own.
         */
        RPointerArray<CWPNATFWItem> iDatas;

        /**
         * For telling whether it is OK to set NW parameters.
         */
        TBool iCharacteristicNw;

        /**
         * For telling whether it is OK to set ICE parameters.
         */
        TBool iCharacteristicIce;

        /**
         * For telling whether it is OK to set TURN parameters.
         */
        TBool iCharacteristicTurn;

        /**
         * For telling whether it is OK to set NATIAP parameters.
         */
        TBool iCharacteristicNatIap;

        /**
         * For telling whether it is OK to set TURNSERVER parameters.
         */
        TBool iCharacteristicTurnServer;

        /**
         * For telling whether it is OK to set ADDITIONALSTUN parameters.
         */
        TBool iCharacteristicAdditionalStun;

        /**
         * Current STUN server item. Ownership is transferred to 
         * CWPNATFWDomainSpecificItem.
         * Not own.
         */
        CWPStunServerItem* iCurrentStunServer;

        /**
         * Current TURN server item. Ownership is transferred to 
         * CWPNATFWDomainSpecificItem.
         * Not own.
         */
        CWPTurnServerItem* iCurrentTurnServer;

        /**
         * For EUnit testing. Uncomment when running tests.
         */
        //friend class T_NATFWTraversalAdapter;
    };

#endif      // CNATFWTRAVERSALADAPTER_H
            
// End of File
