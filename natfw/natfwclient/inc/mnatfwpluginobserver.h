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
* Description:    Callback hook for the plug-ins to the client
*
*/




#ifndef M_MNATFWPLUGINONSERVER_H
#define M_MNATFWPLUGINONSERVER_H

#include <e32base.h>

class CNATFWPluginApi;
class CNATFWCandidatePair;
class CNATFWCandidate;

/**
 *  Observer interface for clients to receive events from NAT-plugin.
 *
 *  @lib natconfw.lib
 *  @since S60 v3.2
 */
class MNATFWPluginObserver
    {

public:
    
    /** NAT-plugin events. */
    enum TNATFWPluginEvent
        {
        /** Completing event for ConnectServerL. */
        EServerConnected        = 1,
        
        /** Completing event for FetchCandidate(s). */
        EFetchingCompleted      = 2,
        
        /** Completing event for SetupTcpConnectionL. */
        ETcpConnSetupCompleted  = 3,
        
        /** Completing event for SetReceivingStateL. */
        EReceivingActivated     = 4,
        
        /** Completing event for SetReceivingStateL. */
        EReceivingDeactivated   = 5,
        
        /** Completing event for SetSendingStateL. */
        ESendingActivated       = 6,
        
        /** Completing event for SetSendingStateL. */
        ESendingDeactivated     = 7,
        
        /** Completing event for PerformConnectivityChecksL. */
        EConnChecksCompleted    = 8
        };
    
    /**
     * Called when an error within a stream has occured and no
     * asynchronous operation is ongoing. Error during asynchronous
     * operation is reported by completing operation with error code.
     * 
     * @since   S60 v3.2
     * @param   aPlugin         The plugin raising event
     * @param   aStreamId       The ID identifying stream
     * @param   aErrorCode      Standard system wide error code
     */
    virtual void Error( const CNATFWPluginApi& aPlugin,
        TUint aStreamId, TInt aErrorCode ) = 0;
    
    /**
     * Notifies the client of plugin events.
     *
     * @since   S60 v3.2
     * @param   aPlugin         The plugin raising event
     * @param   aStreamId       The ID identifying stream
     * @param   aEvent          The event
     * @param   aErrCode        Standard system wide error code
     */
    virtual void Notify( const CNATFWPluginApi& aPlugin,
        TUint aStreamId, TNATFWPluginEvent aEvent, TInt aErrCode ) = 0;
    
    /**
     * Called when working candidate pair has been found. Ownership of
     * the candidate pair is transferred.
     *
     * @since   S60 v3.2
     * @param   aPlugin         The plugin raising event
     * @param   aCandidatePair  The candidate pair which was found
     */
    virtual void NewCandidatePairFound( const CNATFWPluginApi& aPlugin,
        CNATFWCandidatePair* aCandidatePair ) = 0;
    
    /**
     * Called when a new local candidate has been found. Ownership of the
     * candidate is transferred.
     *
     * @since   S60 v3.2
     * @param   aPlugin         The plugin raising event
     * @param   aCandidate      The local candidate that was found
     */
    virtual void NewLocalCandidateFound( const CNATFWPluginApi& aPlugin,
        CNATFWCandidate* aCandidate ) = 0;
    };


#endif // M_MNATFWPLUGINONSERVER_H
