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
* Description:    
*
*/




#ifndef M_STUNTRANSACTIONOBSERVER_H
#define M_STUNTRANSACTIONOBSERVER_H

// INCLUDES
#include <e32def.h>
#include <in_sock.h>
#include "tstuncallbackinfo.h"

// FORWARD DECLARATIONS
class CNATFWUNSAFUnknownAttributesAttribute;

// CLASS DECLARATION

/**
 *  Defines the callbacks used by CSTUNTransaction. 
 */
class MSTUNTransactionObserver
    {
public:
    /**
      * Destructor
     */
    virtual ~MSTUNTransactionObserver() {}

    /**
     * Transaction has terminated successfully and a public address has been
     * received.
     * TransactionError will only be called after PublicAddressObtainedL, if
     * PublicAddressObtainedL leaves.
     *
     * NOTE: If this function leaves, the associated CSTUNTransaction must NOT
     * have been deleted, since it will call TransactionError!
     *
     * @param aAddress Public address
     */
    virtual void PublicAddressObtainedL( const TInetAddr& aAddress ) = 0;
    
    /**
     * Transaction has terminated successfully and a public address has been
     * received.
     * TransactionError will only be called after PublicAddressObtainedL, if
     * PublicAddressObtainedL leaves.
     *
     * NOTE: If this function leaves, the associated CSTUNTransaction must NOT
     * have been deleted, since it will call TransactionError!
     *
     * @param   aReflexiveAddr  Public server reflexive IP address
     * @param   aRelayAddr      Public stun server relay IP address
     */
    virtual void PublicAddressObtainedL( const TInetAddr& aReflexiveAddr,
                                         const TInetAddr& aRelayAddr ) = 0;
    
    /**
     * Transaction terminated abnormally, or leave occurred from
     * MSTUNTransactionObserver::PublicAddressObtainedL. If STUN server sent a
     * Binding Error Response with response code 420 and UNKNOWN-ATTRIBUTES,
     * the UNKNOWN-ATTRIBUTES is also passed as parameter.
     * @pre aError != KErrNone
     * @param aError Error reason
     * @param aUnknownAttr UNKNOWN-ATTRIBUTES attribute, can be NULL.
     *                       Ownership is transferred.
     */
    virtual void TransactionError( TInt aError,
        CNATFWUNSAFUnknownAttributesAttribute* aUnknownAttr ) = 0;
        
    /**
     * Notifies about transaction events
     *
     * @since   s60 3.2
     * @param   aEvent  Enumerated transaction event
     * @return  void
     */
    virtual void TransactionEventOccurredL( 
        TSTUNCallbackInfo::TFunction aEvent ) = 0;
         
    /**
     * Notifies about new RTT ( Round-Trip Time ) sample is measured. If segment is retransmitted
     * then backed-off RTO value is given in param.
     * 
     * @since   s60 3.2
     * @param aRttSample The new RTT sample that was measured
     * @param aRetransmitted Indicates whether last segment retransmitted
     * @return  void    
     */
    virtual void NewRTTSampleMeasured( TInt64& aRTTSample,
        TBool aRetransmitted ) = 0;
    };

#endif // M_STUNTRANSACTIONOBSERVER_H
