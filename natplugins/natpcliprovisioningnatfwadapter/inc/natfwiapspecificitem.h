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
* Description:  NAT-FW Traversal access point specific settings
*
*/


//  INCLUDES
#include <e32base.h>

class CWPCharacteristic;

/**
* Buffer class for accesspoint specific settings in NAT/FW Traversal adapter
* @since S60 v3.1
*/
class CNATFWIAPSpecificItem : public CBase
    {
    public:
        
        static CNATFWIAPSpecificItem* NewL();

        static CNATFWIAPSpecificItem* NewLC();
    
        virtual ~CNATFWIAPSpecificItem();

    public: // New methods
    
        /**
         * Getter for NAPDEF name (IAP settings name).
         * @since S60 v3.1
         * @return NAPDEF name
         */
        const TDesC8& NapDefName() const;

        /**
         * Getter for NAPDEF (IAP settings).
         * @since S60 v3.1
         * @return NAPDEF characteristic.
         */
        CWPCharacteristic* NapDef();
        
        /**
         * Setter for NAPDEF name (IAP settings).
         * @since S60 v3.1
         * @param aToNapId. To NapId value
         */
        void SetNapDefNameL( const TDesC8& aToNapId );
      
        /**
        * Setter for NAPDEF (IAP settings).
        * @since Series 60 3.0.
        * @param aNapDef. NAPDEF characteristic.
        */ 
        void SetNapDef( CWPCharacteristic* aNapDef );
        
        /**
        * Setter for IAPKey.
        * @since Series 60_3.1
        * @param aIAPKey value to set.
        */
        void SetIAPKey( TUint32 aIAPKey );
        
        /**
        * Setter for IAPId.
        * @since Series 60_3.1
        * @param aIAPId value to set.
        */
        void SetIAPId( TUint32 aIAPId );
         
        /**
        * Setter for IntervalUDP.
        * @since Series 60_3.1
        * @param aIntervalUDP value to set.
        */   
        void SetIntervalUDP( TInt aIntervalUDP );

        /**
        * Setter for IntervalTCP.
        * @since Series 60_3.1
        * @param aIntervalTCP value to set.
        */
        void SetIntervalTCP( TInt aIntervalTCP );

        /**
        * Setter for STUNRetransmitTimer.
        * @since Series 60_3.1
        * @param aSTUNRetransmitTimer value to set.
        */
        void SetSTUNRetransmitTimer( TInt aSTUNRetransmitTimer );
                         
        /**
        * Getter for IAPKey. 
        * @since Series 60_3.1
        * @return Key for IAP table in CenRep.
        */                    
        TUint32 GetIAPKey() const;

        /**
        * Getter for IAPId. 
        * @since Series 60_3.1
        * @return IAP ID.
        */
        TUint32 GetIAPId() const;

        /**
        * Getter for IntervalUDP. 
        * @since Series 60_3.1
        * @return IntervalUDP value
        */
        TInt GetIntervalUDP() const;

        /**
        * Getter for IntervalTCP. 
        * @since Series 60_3.1
        * @return IntervalTCP value
        */    
        TInt GetIntervalTCP() const;
        
        /**
        * Getter for STUNRetransmitTimer. 
        * @since Series 60_3.1
        * @return STUN retransmit timer value.
        */
        TInt GetSTUNRetransmitTimer() const;

    private:

        void ConstructL();

    private: // Data

        /**
         * Key of the record
         */
        TUint32 iIAPKey;

        /**
         * Identifier of the iap table. IAP Id
         */
        TUint32 iIAPId;
        
        /**
         * NAT refresh interval for UDP in UNSAF Protocols IAP settings
         */
        TInt iIntervalUDP;
        
        /**
         * NAT refresh interval for TCP in UNSAF Protocols IAP settings
         */
        TInt iIntervalTCP;
        
        /**
         * STUN server port
         */
        TInt iSTUNRetransmitTimer;
        
        /**
         * The access point name. 
         * OMA ProvCont TO-NAPID/NAPDEF.
         * Own.
         */
        HBufC8* iNapDefName;        
        
        /**
         * NAPDEF. Pointer to IAP settings.
         * Not own.
         */
        CWPCharacteristic* iNapDef;
        
        /**
         * Uncomment this for testing.
         */
        //friend class T_NATFWTraversalAdapter;
    };