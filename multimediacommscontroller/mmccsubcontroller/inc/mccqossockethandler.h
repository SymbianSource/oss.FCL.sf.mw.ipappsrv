/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Class definition for the MCC QoS Sockethandler
*
*/




#ifndef MCCQOSSOCKETHANDLER_H
#define MCCQOSSOCKETHANDLER_H

// INCLUDES
#include <e32std.h>

// FORWARD DECLARATIONS
class MMccQosEventObserver;


// CLASS DECLARATION

/**
 *  CMccQosSocketHandler 
 *
 *  CMccQosSocketHandler handles socket adding and removing from 
 *  subconnection. 
 *
 *  @lib mccqosplugin.lib
 *  @since S60 3.1  
 */
NONSHARABLE_CLASS( CMccQosSocketHandler ) : public CActive
    {
#ifdef TEST_EUNIT    
    friend class UT_CMccQosSocketHandler;
#endif    

public:

    /**
     * Create a CMccQosSocketHandler object using two phase construction,
     * and return a pointer to the created object.
     *
     * @since S60 3.1
     * @param aObserver Observer to asynchronous completion
     * @param aSubConnection RSubConnection object
     * @param aSocket RSocket object to be used with the subconnection
     * @return Instance of class interface
     */
    static CMccQosSocketHandler* NewL( 
            MMccQosEventObserver& aObserver,
            RSubConnection& aSubConnection, 
            RSocket& aSocket );

    /**
     * Create a CMccQosSocketHandler object using two phase construction,
     * and return a pointer to the created object.
     *
     * @since S60 3.1
     * @param aObserver Observer to asynchronous completion
     * @param aSubConnection RSubConnection object
     * @param aSocket RSocket object to be used with the subconnection
     * @return Instance of class interface
     */
    static CMccQosSocketHandler* NewLC( 
            MMccQosEventObserver& aObserver,
            RSubConnection& aSubConnection,
            RSocket& aSocket );

    /**
     * Destructor
     * 
     * @return void
     */
    virtual ~CMccQosSocketHandler();
    
    /**
     * Sets a flag to indicate that asynchronous deletion should be done. This
     * is because AddSocket returns after the (sub)connection has gone down and
     * QoS handler is already deleted.
     *
     * @since S60 v3.2
     * @param TBool ETrue if asyncronous deletion is done
     * @return void
     */
    void AsyncDeleteNeeded( TBool aAsyncDelete );

protected: // from CActive

    /**
     * Callback function. Invoked to handle responses from the server.
     *
     * @return void
     */
    void RunL();

    /**
     * Cancel any outstanding operation.
     *
     * @return void
     */
    void DoCancel();
    
    /**
     * Called if RunL leaves
     *
     * @return TInt System-wide error code
     */
    TInt RunError( TInt aError );

private:

    /**
     * Perform the first phase of two phase construction.
     *
     * @since S60 3.1
     * @param aObserver observer to asynchronous completion
     * @param aSubConnection RSubConnection object
     * @param aSocket RSocket object to be used with the subconnection
     * @return void
     */
    CMccQosSocketHandler( MMccQosEventObserver& aObserver, 
                          RSubConnection& aSubConnection,
                          RSocket& aSocket );

    /**
     * By default Symbian 2nd phase constructor is private.
     *
     * @return void
     */
    void ConstructL();
    
    /**
     * Disassociate the socket with the subconnection.
     * 
     * @since S60 3.1
     * @return void
     */
    void RemoveSocketFromSubConn();

private:
    
    /**
     * Pending action to subconnection
     */
    enum TPendingAction
        {
        ENoPendingAction    = 1,
        EAddingSocket       = 2,    
        ERemovingSocket     = 3
        };
    
    /**
     * Member variable for pending action
     */
    TPendingAction iPendingAction;
    
    /**
     * Observer to report RSubConnection(QoS) status/error information.
     */
    MMccQosEventObserver& iEventObserver;

    /**
     * Reference to current Sub-Connection handle.
     */       
    RSubConnection& iSubConnection;

    /**
     * Last error reported from RSubConnection channel.
     */
    TInt iLastError;

    /**
     * Reference to Socket.     
     */
    RSocket& iSocket;
    
    /**
     * Flag to indicate async deletion need
     */
    TBool iAsyncDelete;

    };

#endif      // MCCQOSSOCKETHANDLER_H

// End of File
