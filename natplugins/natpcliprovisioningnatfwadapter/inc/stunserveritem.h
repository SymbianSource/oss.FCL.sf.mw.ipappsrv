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
* Description:  STUN server item.
*
*/


#ifndef STUNSERVERITEM_H
#define STUNSERVERITEM_H

#include <e32base.h>

/**
 *  STUN server item
 *  Holds transiently one STUN server item before it is saved to
 *  Central Repository.
 *
 *  @lib wpnatfwtraversaladapter.lib
 *  @since S60 3.2
 */
class CWPStunServerItem : public CBase
    {

    /**
     * For EUnit tests; uncomment when running tests.
     */
    //friend class T_NATFWTraversalAdapter;

public:

    static CWPStunServerItem* NewL();
    static CWPStunServerItem* NewLC();

    /**
     * Destructor.
     */
    virtual ~CWPStunServerItem();

    /**
     * Setter for STUN server address.
     *
     * @since S60 3.2
     * @param aStunSrvAddr STUN server address.
     */
    void SetStunSrvAddrL( const TDesC8& aStunSrvAddr );

    /**
     * Setter for STUN server address.
     *
     * @since S60 3.2
     * @param aStunSrvPort STUN server port.
     */
    void SetStunSrvPort( TInt aStunSrvPort );

    /**
     * Setter for STUN server username.
     *
     * @since S60 3.2
     * @param aStunUsername STUN server username.
     */
    void SetStunUsernameL( const TDesC8& aStunUsername );

    /**
     * Setter for STUN server password.
     *
     * @since S60 3.2
     * @param aStunSrvAddr STUN server password.
     */
    void SetStunPasswordL( const TDesC8& aStunPassword );

    /**
     * Getter for STUN server address.
     *
     * @since S60 3.2
     * @return aStunSrvAddr STUN server address.
     */
    const TDesC8& StunSrvAddr() const;

    /**
     * Getter for STUN server address.
     *
     * @since S60 3.2
     * @return aStunSrvPort STUN server port.
     */
    TInt StunSrvPort() const;

    /**
     * Getter for STUN server username.
     *
     * @since S60 3.2
     * @return aStunUsername STUN server username.
     */
    const TDesC8& StunUsername() const;

    /**
     * Getter for STUN server password.
     *
     * @since S60 3.2
     * @return aStunPassword STUN server password.
     */
    const TDesC8& StunPassword() const;

private:

    CWPStunServerItem();

    void ConstructL();

private: // data

    /**
     * STUN server address.
     * OMA CP Parameter: STUNSRVADDR. See w902E.txt for more information.
     * Own.
     */
    HBufC8* iStunSrvAddr;

    /**
     * STUN server port.
     * OMA CP Parameter: STUNSRVPORT. See w902E.txt for more information.
     */
    TInt iStunSrvPort;

    /**
     * STUN server username.
     * OMA CP Parameter: AAUTHNAME. See w902E.txt for more information.
     * Own.
     */
    HBufC8* iStunUsername;

    /**
     * STUN server password.
     * OMA CP Parameter: AAUTHSECRET. See w902E.txt for more information.
     * Own.
     */
    HBufC8* iStunPassword;

    };

#endif // STUNSERVERITEM_H

