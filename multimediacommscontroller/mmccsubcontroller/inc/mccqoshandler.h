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
* Description:    Class definition 3G QoS enabling
*
*/




#ifndef CMCCQOSHANDLER_H
#define CMCCQOSHANDLER_H

// INCLUDES
#include <e32std.h>
#include <es_sock.h>

// FORWARD DECLARATIONS
class CMccQosSocketHandler;
class CMccQosEventMonitor;
class MMccQosEventObserver;

// CLASS DECLARATION

/**
 *
 * The main purpose of this class is to enable 3G conversational/streaming QoS
 * and secondary PDP context using RSubConnection class.
 *
 * @lib mmccsubcontroller.dll
 * @since   Series 60 3.2
 */
NONSHARABLE_CLASS( CMccQosHandler ) : public CBase
    {
#ifdef TEST_EUNIT    
    friend class UT_CMccQosHandler;
#endif    

public: // Constructors and destructor

    /**
     * NewL
     *
     * @since   S60 3.2
     * @param   Observer for RSubConnection events
     * @return  Instance of class interface
     */
    static CMccQosHandler* NewL( MMccQosEventObserver& aObserver );

    /**
     * Destructor.
     *
     * @return void
     */
    virtual ~CMccQosHandler();

public: // New functions

    /**
     * Method for enabling QoS. QoS handler will try to obtain conversational
     * or streaming traffic classes.
     *
     * @since   S60 3.2
     * @pre SetSubConnectionParams()
     * @leave KErrInUse     If QoS has been already enabled
     * @leave KErrNotReady  If SetSubConnectionParams has not been called.
     * @return void
     */
    void EnableQosL();

    /**
     * Method for disabling Qos
     *
     * @since   S60 3.2
     * @return void
     */
    void DisableQoS();

    /**
     * Method for setting params for subconnection.
     *
     * @since   S60 3.2
     * @param aConnection RConnection handle
     * @param aSocketServ RSocketServ handle
     * @param aSocket     RSocket handle
     * @return void
     */  
    void SetSubConnectionParams( RConnection& aConnection, 
        RSocketServ& aSocketServ, RSocket& aSocket );
    
    /**
     * Method for querying QoS status (Enabled/Disabled)
     *
     * @since S60 3.2
     * @return TBool ETrue if QoS is enabled.
     */
    TBool QoSEnabled() const;

private:

    /**
     * C++ default constructor.
     *
     * @return void
     */
    CMccQosHandler( MMccQosEventObserver& aObserver );
    
    /**
     * Handles sockethandler deletion. Needed because AddSocket may return
     * after the subconnection has gone down and QoS handler has been deleted.
     *
     * @since S60 v3.2
     * @return void
     */
    void HandleSocketHandlerDeletion();

private: // data
    
    /**
     * Pointer to RConnection handle.
     * Not Own.
     */
    RConnection* iConnection;

    /**
     * Pointer to RSocketServ handle.
     * Not Own.
     */
    RSocketServ* iSocketServ;

    /**
     * Pointer to socket object.
     * Not Own.    
     */
    RSocket* iSocket;

    /**
     * RSubConnection handle.
     */
    RSubConnection iSubConnection;

    /**
     * Pointer to object that monitors RSubConnection related events.
     * Own.
     */
    CMccQosSocketHandler* iQosSocketHandler;

    /**
     * Observer reference
     */
    MMccQosEventObserver& iObserver;
    
    /**
     * Instance of event monitor
     * Own.
     */
    CMccQosEventMonitor* iEventMonitor;
    
    };

#endif // CMCCQOSHANDLER_H

// End of File
