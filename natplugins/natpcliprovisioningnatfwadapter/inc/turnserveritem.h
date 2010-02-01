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
* Description:  TURN server item.
*
*/


#ifndef TURNSERVERITEM_H
#define TURNSERVERITEM_H

#include <e32base.h>

/**
 *  TURN server item.
 *
 *  Holds transiently one TURN server item before it is saved to
 *  Central Repository in class CWPNATFWItem.
 *
 *  @lib wpnatfwtraversaladapter.lib
 *  @since S60 3.2
 */
class CWPTurnServerItem : public CBase
    {

    /**
     * For EUnit tests; uncomment when running tests.
     */
    //friend class T_NATFWTraversalAdapter;

public:

    static CWPTurnServerItem* NewL();
    static CWPTurnServerItem* NewLC();

    /**
     * Destructor.
     */
    virtual ~CWPTurnServerItem();

    /**
     * Setter for TURN server address.
     *
     * @since S60 3.2
     * @param aTurnSrvAddr TURN server address.
     */
    void SetTurnSrvAddrL( const TDesC8& aTurnSrvAddr );

    /**
     * Setter for TURN server address.
     *
     * @since S60 3.2
     * @param aTurnSrvPort TURN server port.
     */
    void SetTurnSrvPort( TInt aTurnSrvPort );

    /**
     * Setter for TURN server username.
     *
     * @since S60 3.2
     * @param aUsername TURN server username.
     */
    void SetTurnUsernameL( const TDesC8& aUsername );

    /**
     * Setter for TURN server password.
     *
     * @since S60 3.2
     * @param aPassword TURN server password.
     */
    void SetTurnPasswordL( const TDesC8& aPassword );

    /**
     * Getter for TURN server address.
     *
     * @since S60 3.2
     * @return TURN server address.
     */
    const TDesC8& TurnSrvAddr() const;

    /**
     * Getter for TURN server address.
     *
     * @since S60 3.2
     * @return TURN server port.
     */
    TInt TurnSrvPort() const;

    /**
     * Getter for TURN server username.
     *
     * @since S60 3.2
     * @return TURN server username.
     */
    const TDesC8& TurnUsername() const;

    /**
     * Getter for TURN server password.
     *
     * @since S60 3.2
     * @return TURN server password.
     */
    const TDesC8& TurnPassword() const;

private:

    CWPTurnServerItem();

    void ConstructL();

private: // data

    /**
     * TURN server address.
     * OMA CP Parameter: TURNSRVADDR. See w902E.txt for more information.
     * Own.
     */
    HBufC8* iTurnSrvAddr;

    /**
     * TURN server port.
     * OMA CP Parameter: TURNSRVPORT. See w902E.txt for more information.
     */
    TInt iTurnSrvPort;

    /**
     * TURN server username.
     * OMA CP Parameter: AAUTHNAME. See w902E.txt for more information.
     * Own.
     */
    HBufC8* iTurnUsername;

    /**
     * TURN server password.
     * OMA CP Parameter: AAUTHSECRET. See w902E.txt for more information.
     * Own.
     */
    HBufC8* iTurnPassword;

    };

#endif // TURNSERVERITEM_H

