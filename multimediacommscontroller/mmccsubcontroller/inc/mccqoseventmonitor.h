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
* Description:    Class definition for the MCC QoS Event Monitor
*
*/




#ifndef MCCQOSEVENTMONITOR_H
#define MCCQOSEVENTMONITOR_H

// INCLUDES
#include <e32base.h>
#include <es_sock.h>

// FORWARD DECLARATIONS
class MMccQosEventObserver;

// CLASS DECLARATION

/**
 *  CMccQosEventMonitor for handling events from RSubConnection
 *
 *  CMccQosEventMonitor receives subconnection(Qos) events from RSubConnection
 *  class. Events are delivered to MCC subcontroller as callbacks.
 *
 *  @lib mccqosplugin.lib
 *  @since S60 3.1
 */
NONSHARABLE_CLASS( CMccQosEventMonitor ) : public CActive
    {
#ifdef TEST_EUNIT
    friend class UT_CMccQosEventMonitor;
#endif
    
public:

    /**
     * Create a CMccQosEventMonitor object using two phase construction,
     * and return a pointer to the created object.
     *
     * @since S60 3.1
     * @param aObserver observer to asynchronous completion
     * @param handle to RSubConnection object
     * @return  Instance of class interface
     */
    static CMccQosEventMonitor* NewL( MMccQosEventObserver& aObserver, 
        RSubConnection& aSubConnection );

    /**
     * Create a CMccQosEventMonitor object using two phase construction,
     * and return a pointer to the created object.
     *
     * @since S60 3.1
     * @param aObserver observer to asynchronous completion
     * @param handle to RSubConnection object
     * @return  Instance of class interface
     */
    static CMccQosEventMonitor* NewLC( MMccQosEventObserver& aObserver, 
        RSubConnection& aSubConnection );

    /**
     * Destructor
     *
     * @return void
     */
    virtual ~CMccQosEventMonitor();

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
     * RunL error handling
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
     * @param handle to RSubConnection object
     * @return void
     */
    CMccQosEventMonitor( MMccQosEventObserver& aObserver, 
        RSubConnection& aSubConnection );
        
    /**
     * By default Symbian 2nd phase constructor is private.
     *
     * @since S60 v3.1
     * @return void
     */
    void ConstructL();
    
    /**
     * Set active object active and start listening for QoS events
     * from RSubConnection.
     *
     * @since S60 3.1
     * @return void
     */
    void StartListenL();

private: // data

    /**
     * Observer to report QoS status/error information
     */
    MMccQosEventObserver& iEventObserver;

    /**
     * Buffer for QoS events
     */
    TNotificationEventBuf iQoSNotifBuffer;

    /**
     * Reference to current subconnection Handle
     */
    RSubConnection& iSubConnection;

    /**
     * Last error reported from RSubConnection channel
     */
    TInt iLastError;

    };

#endif      // MCCQOSEVENTMONITOR_H

// End of File
