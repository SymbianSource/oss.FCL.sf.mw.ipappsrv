/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Collection of utility functions
*
*/




#ifndef STUNUTILS_H
#define STUNUTILS_H

// INCLUDES
#include <e32base.h>
#include <in_sock.h>
#include "natfwunsafattribute.h"

// FORWARD DECLARATIONS
class CNATFWUNSAFMessage;
class CNATFWUNSAFUnknownAttributesAttribute;

// CLASS DECLARATION

/**
 *  Collection of general purpose utility functions, used by several parts of
 *  the STUN client.
 *
 *  @lib ?library
 *  @since S60 ?S60_version *** for example, S60 v3.0
 */
class STUNUtils
    {
    public: // Enumerations

    enum TWaitPeriod
        {
        //Unit is milliseconds
        ERetryImmediately = 0,
        
        EWaitBeforeRetryDuration = 8000
        };

    public:

        /**
        * Returns the address inside MAPPED-ADDRESS attribute, located in the
        * aResponse.
        * @pre aResponse.Validate() == ETrue
        * @param aResponse UNSAF Binding Response
        * @param aAddr OUT: if aResponse contained MAPPED-ADDRESS, the address
        *    in the attribute is copied here
        * @return ETrue if aResponse contained MAPPED-ADDRESS,
        *          EFalse otherwise
        */
        static TBool GetMappedAddress( const CNATFWUNSAFMessage& aResponse,
                                       TInetAddr& aAddr );

        /**
        * Gets the address inside XOR-MAPPED-ADDRESS attribute, located in the
        * aResponse.
        * @pre aResponse.Validate() == ETrue
        * @param aResponse UNSAF Binding Response        
        * @param aAddr OUT: if aResponse contained XOR-MAPPED-ADDRESS, the
        *    address in the attribute is copied here
        * @return ETrue if aResponse contained XOR-MAPPED-ADDRESS,
        *          EFalse otherwise
        */
        static TBool GetXorMappedAddressL( const CNATFWUNSAFMessage& aResponse,
                                           TInetAddr& aAddr );

        /**
         * Gets the address inside RELAY-ADDRESS attribute, located in the
         * aResponse.
         *
         * @since   s60 3.2
         * @pre aResponse.Validate() == ETrue
         * @param aResponse UNSAF Binding Response        
         * @param aAddr OUT: if aResponse contained RELAY-ADDRESS, the
         *    address in the attribute is copied here
         * @return ETrue if aResponse contained RELAY-ADDRESS,
         *          EFalse otherwise
         */
        static TBool GetRelayAddressL( const CNATFWUNSAFMessage& aResponse,
                                       TInetAddr& aAddr );
        
        /**
         * Gets the address inside TIMERVAL attribute, located in the
         * aResponse.
         *
         * @since   s60 3.2
         * @pre     aResponse.Validate() == ETrue
         * @param   aResponse UNSAF Binding Response        
         * @param   aTimerVal OUT: if aResponse contained TIMERVAL, the
         *          address in the attribute is copied here
         * @return  ETrue if aResponse contained TIMERVAL,
         *          EFalse otherwise
         */
        static TBool GetTimerValueL( const CNATFWUNSAFMessage& aResponse,
                                     TUint32& aTimerVal );
                                     
        /**
        * Checks if the response code is a 5xx.
        * @param aResponseCode Response code
        * @return ETrue aResponseCode is 5xx, EFalse otherwise
        */
        static TBool Is5xxResponse( TInt aResponseCode );

        /**
        * Deletes the attributes listed in aUnknownAttr, from aMessage.
        * @param aMessage UNSAF message
        * @param aUnknownAttr UNKNOWN-ATTRIBUTES attribute, tells what
        *    attributes are removed from aMessage.
        */
        static void RemoveUnknownAttributes( CNATFWUNSAFMessage& aMessage,
            const CNATFWUNSAFUnknownAttributesAttribute& aUnknownAttr );
    };

#endif // STUNUTILS_H
