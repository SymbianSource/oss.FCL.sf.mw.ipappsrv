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




#ifndef T_TICECHECKQUEITEM_H
#define T_TICECHECKQUEITEM_H

#include <e32base.h>
#include "ciceconnectivitycheck.h"

/**
 *  (Triggered) check queue item encapsulating information relating to the
 *  connectivity check.
 *
 *  @lib icecandidatehandler.dll
 *  @since S60 v3.2
 */
class TIceCheckQueItem
    {
    
public:

    /**
     * Constructor.
     *
     * @since   S60 v3.2
     * @param   aCheck              The connectivity check
     * @param   aType               Type of the check
     * @param   aPriority           Priority for the local check
     */
    TIceCheckQueItem( 
            CIceConnectivityCheck& aCheck, 
            CIceConnectivityCheck::TIceCheckType aType, TUint aPriority )
        : 
        iCheck( aCheck ), iType( aType ), iPriority( aPriority ) { }
    
    /**
     * Constructor.
     *
     * @since   S60 v3.2
     * @param   aCheck              The connectivity check
     */
    TIceCheckQueItem( CIceConnectivityCheck& aCheck )
        : 
        iCheck( aCheck ), 
        iType( CIceConnectivityCheck::EIceCheckTypeTriggered ), 
        iPriority( 0 ) { }
        
    /**
     * Return connectivity check instance.
     *
     * @since   S60 v3.2
     * @return  Connectivity check
     */
    CIceConnectivityCheck& Check() const { return iCheck; } // lint#1536
    
    /**
     * Return type of the check.
     *
     * @since   S60 v3.2
     */
    CIceConnectivityCheck::TIceCheckType Type() const { return iType; }
    
    /**
     * Return priority for the local check.
     *
     * @since   S60 v3.2
     * @return  Priority for the check
     */
    TUint Priority() const { return iPriority; }
    
    /**
     * Implements TIdentityRelation. Returns ETrue if checks contained by
     * given items are same.
     *
     * @since   S60 v3.2
     * @return  ETrue/EFalse
     */
    static TBool MatchCheck( 
        const TIceCheckQueItem& aItem1,
        const TIceCheckQueItem& aItem2 )
        {
        return ( &( aItem1.Check() ) == &( aItem2.Check() ) );
        }

private:
    
    TIceCheckQueItem();

private:    // data

    /**
     * Connectivity check.
     */
    CIceConnectivityCheck& iCheck;
    
    /**
     * Type of the check.
     */
    CIceConnectivityCheck::TIceCheckType iType;
    
    /**
     * Value for PRIORITY attribute.
     */
    TUint iPriority;
    };

#endif // T_TICECHECKQUEITEM_H
