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




#ifndef NATFWCALLBACKINFO_H
#define NATFWCALLBACKINFO_H

// INCLUDES
#include <e32base.h>
#include "mnatfwconnectivityobserver.h"

// FORWARD DECLARATIONS
class MNATFWRegistrationController;

/**
 * This class contains the information to execute a single callback
 * function of MNATFWConnectivityObserver.
 */
class CNatFwCallbackInfo : public CBase
    {
public: // Enumerations

        /**
         * One entry for each function of MNATFWConnectivityObserver,
         * including all the different TNATFWPluginEvent values. In
         * addition error value, for error notifications.
         */
        enum TFunction
            {
            ECreatedSession         = 0x0001,
            ELocalCandidateFound    = 0x0002,
            EFetchingEnd            = 0x0004,
            EActiveReceiving        = 0x0008,
            EDeactiveReceiving      = 0x0010,
            EActiveSending          = 0x0020,
            EDeactiveSending        = 0x0040,
            ECandidatePairFound     = 0x0080,
            ECompletedConnChecks    = 0x0100,
            EError                  = 0x0200
            };

public: // Constructors and destructor

    /**
     * Constructor
     *
     * @since   s60 3.2
     * @param   aController Registration controller
     * @param   aFunction   Identifies the function of
     *                      MNATFWConnectivityObserver to be called
     * @param   aSessionId  Session ID
     * @param   aStreamId   Stream ID
     * @param   aErrorCode  Error reason
     * @param   aEventData  Event data if any
     */
    CNatFwCallbackInfo( MNATFWRegistrationController& aController,
                        CNatFwCallbackInfo::TFunction aFunction,
                        TUint aSessionId,
                        TUint aStreamId,
                        TInt aErrorCode,
                        TAny* aEventData );

    /**
     * Checks that the selected callback function has required parameters
     * defined and that no unrelevant parameters have been specified.
     *
     * @since   s60 3.2
     * @return  Result of validation process as a boolean type
     */
    TBool Validate() const;

    /**
     * Calls a selected function of observer with the given parameters.
     * The function to call and the parameters were specified in the
     * constructor.
     *
     * @since   S60 v3.2
     * @return  void
     */
    void Execute() const;

private: // Constructors, for internal use

    CNatFwCallbackInfo();

    CNatFwCallbackInfo( const CNatFwCallbackInfo& aCallbackInfo );

private: // New functions, for internal use

    /**
     * Passes events to observer
     *
     * @since   S60 v3.2
     * @param   aEvent      Plugin event
     * @return  void
     */
    void PassEvent(
        MNATFWConnectivityObserver::TNATFWConnectivityEvent aEvent ) const;

public: // Data

    /**
     * Que link
     */
    TSglQueLink iLink;


    /**
     * NAT FW Registration controller
     * Not own
     */
    MNATFWRegistrationController& iController;

    /**
     * Selected callback function
     */
    TFunction iFunction;

    /**
     * Session ID
     */
    TUint iSessionId;

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

#endif // NATFWCALLBACKINFO_H
