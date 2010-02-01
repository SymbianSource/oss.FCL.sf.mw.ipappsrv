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
* Description:    
*
*/




#ifndef T_STUNPLUGINCALLBACKINFO_H
#define T_STUNPLUGINCALLBACKINFO_H

// INCLUDES
#include <e32base.h>
#include "mnatfwpluginobserver.h"


/**
 * This class contains the information to execute a single callback 
 * function of MNATFWPluginObserver.
 */
class TStunPluginCallbackInfo
    {
public: // Enumerations

        /**
         * One entry for each function of MNATFWPluginObserver, including all
         * the different TNATFWPluginEvent values. In addition error value, 
         * for error notifications.
         */
        enum TFunction
            {
            EConnectServer,
            EActiveSending,
            EDeactiveSending,
            EActiveReceiving,
            EDeactiveReceiving,
            ELocalCandidateFound,
            EFetchingEnd,
            EError
            };

public: // Constructors and destructor

    /**
     * Constructor
     *
     * @since   s60 3.2     
     * @param   aStunPlugin STUN plugin instance
     * @param   aObserver   STUN plugin observer
     * @param   aFunction Identifies the function of MNATFWPluginObserver to
     *          be called
     * @param   aStreamid stream id
     * @param   aErrorCode Error reason
     * @param   aEventData Event data if any
     */
    TStunPluginCallbackInfo( const CNATFWPluginApi& aStunPlugin,
                             MNATFWPluginObserver& aObserver, 
                             TStunPluginCallbackInfo::TFunction aFunction,
                             TUint aStreamid,
                             TInt aErrorCode,
                             TAny* aEventData );

    /**
     * Checks that the selected callback function has required parameters
     * defined and that no unrelevant parameters have been specified.
     *
     * @since   s60 3.2
     * @return  result of validation process as a boolean type
     */
    TBool Validate() const;
    
    /**
     * Calls a selected function of observer with the given parameters.
     * The function to call and the parameters were specified in the
     * constructor.
     *
     * @since   S60 v3.2
     */
    void Execute() const;

private: // Constructors, for internal use

    TStunPluginCallbackInfo();

    TStunPluginCallbackInfo( const TStunPluginCallbackInfo& aCallbackInfo );

private: // New functions, for internal use
    
    /**
     * Passes events to observer
     *
     * @since   S60 v3.2
     * @param   aEvent Plugin event
     */
    void PassEvent( MNATFWPluginObserver::TNATFWPluginEvent aEvent ) const;
    
    /**
     * Passes errors to observer
     *
     * @since   S60 v3.2
     */
    void PassError() const;

public: // Data
    
    /**
     * Que link
     */
    TSglQueLink iLink;

    /**
     * STUN plugin reference
     */
    const CNATFWPluginApi& iStunPlugin;

    /**
     * Observer for which to pass events
     */
    MNATFWPluginObserver& iObserver;
    
    /**
     * Selected callback function
     */
    TStunPluginCallbackInfo::TFunction iFunction;

    /**
     * Stream ID
     */
    TUint iStreamId;
   
    /**
     * Error code
     */
    TInt iErrorCode;
    
    /**
     * The event data
     */
    TAny* iEventData; 
    };

#endif // T_STUNPLUGINCALLBACKINFO_H

