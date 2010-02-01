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
* Description:    Defines all necessary constants used in STUN Client
*
*/




#ifndef NATFWSTUNCLIENTDEFS_H
#define NATFWSTUNCLIENTDEFS_H

#include <e32std.h>

_LIT8( KStun, "stun" );
_LIT8( KStunRelay, "stun-relay" );
const TInt KErrNATFWDnsFailure = -27000;

/**  Transport protocols */
enum TTransportProtocol
    {
    EUndefinedTransportProtocol = 0,
    ETcpProtocol,
    EUdpProtocol
    };

/**  Known STUN error codes */
enum TTransactionError
    {
    E1XX             = 100,
    E300TryAlternate = 300,
    E4XX             = 400,
    E401Unauthorized = 401,

    // Server rejected request because it contained unknown attributes
    E420UnknownAttributes   = 420,
    
    E430StaleCredentials      = 430, // removed from stunrfc3489bis-08
    E431IntegrityCheckFailure = 431, // removed from stunrfc3489bis-08
    E432MissingUsername       = 432, // removed from stunrfc3489bis-08
    E433UseTLS                = 433, // removed from stunrfc3489bis-08
    E434MissingRealm          = 434, // removed from stunrfc3489bis-08
    E435MissingNonce          = 435, // removed from stunrfc3489bis-08
    E436UnknownUsername       = 436, // removed from stunrfc3489bis-08
    E438StaleNonce            = 438, 
    
    // draft-ietf-behave-turn-02 defines new error codes
    E437NoBindind                    = 437,
    E439Transitioning                = 439,
    E442UnsupportedTransportProtocol = 442,
    E443InvalidIPAddress             = 443,
    E444InvalidPort                  = 444,
    E445OperationForTCPOnly          = 445,
    E446ConnectionAlreadyExists      = 446,
    E486AllocationQuotaReached       = 486,
    E487RoleConflict                 = 487,            
    
    E500ServerError          = 500,
    E507InsufficientCapacity = 507,
    E600GlobalFailure        = 600, // removed from stunrfc3489bis-08

    // Request should be retried after adding XOR-ONLY attribute
    ERetryAfterAddingXorOnly = 1001
    };


/** ICE specific attributes */
class TICEAttributes
    {
public:

    inline TICEAttributes( ) :
        iPriority( 0 ),
        iUseCandidate( EFalse ),
        iControlled( 0 ),
        iControlling( 0 )
        {
        }
        
    inline TICEAttributes( TUint aPriority, 
                           TBool aUseCandidate, 
                           TUint64 aControlled, 
                           TUint64 aControlling ) :
        iPriority( aPriority ),
        iUseCandidate( aUseCandidate ),
        iControlled( aControlled ),
        iControlling( aControlling )
        {
        }
    
public:
    /**
     * Priority. Set to "0" if not used.
     */
    TUint iPriority;
    
    /**
     * Use candidate flag. If attribute not needed, set to "EFalse"
     */
    TBool iUseCandidate;
    
    /**
     * Controlled. If value is greater than "0", 
     * attribute is added to request.
     */
    TUint64 iControlled;
    
    /**
     * Controlling. If value is greater than "0",
     * attribute is added to request.
     */
    TUint64 iControlling; 
    };
    
#endif // NATFWSTUNCLIENTDEFS_H
