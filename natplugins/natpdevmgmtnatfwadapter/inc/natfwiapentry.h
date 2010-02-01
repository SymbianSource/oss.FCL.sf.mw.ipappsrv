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
* Description:  Device Management NAT-FW Traversal IAP specific settings
*
*/


//  INCLUDES
#include <e32base.h>

/*
* CNATFWIAPEntry
* Buffer class for holding access point specific NAT/FW settings
*/
class CNATFWIAPEntry : public CBase
    {
public:  // Constructors and destructor
    
    /**
     * Two-phased constructor.
     */
    static CNATFWIAPEntry* NewL();
    static CNATFWIAPEntry* NewLC();
    
    /**
     * Destructor.
     */
    virtual ~CNATFWIAPEntry();

    public: // New methods
    
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
     * @return TUint32. Key for IAP table in CenRep.
     */                    
    TUint32 GetIAPKey() const;

    /**
     * Getter for IAPId. 
     * @since Series 60_3.1
     * @return TUint32. IAP ID.
     */
    TUint32 GetIAPId() const;

    /**
     * Getter for IntervalUDP. 
     * @since Series 60_3.1
     * @return TInt.
     */
    TInt GetIntervalUDP() const;

    /**
     * Getter for IntervalTCP. 
     * @since Series 60_3.1
     * @return TInt.
     */    
    TInt GetIntervalTCP() const;
    
    /**
     * Getter for STUNRetransmitTimer. 
     * @since Series 60_3.1
     * @return TInt.
     */
    TInt GetSTUNRetransmitTimer() const;

private:

    /**
     * C++ default constructor.
     * Initialises values to default
     */
    CNATFWIAPEntry();

private: // Data

    // Key of the record
    TUint32 iIAPKey;

    // Identifier of the iap table. IAP Id
    TInt iIAPId;

    // NAT refresh interval for UDP in UNSAF Protocols IAP settings.
    TInt iIntervalUDP;

    // NAT refresh interval for TCP in UNSAF Protocols IAP settings.
    TInt iIntervalTCP;

    // STUN server port
    TInt iSTUNRetransmitTimer;

    /**
     * Uncomment this for unit testing.
     */
    //friend class UT_CNSmlDmNATFWTraversalAdapter;

    };