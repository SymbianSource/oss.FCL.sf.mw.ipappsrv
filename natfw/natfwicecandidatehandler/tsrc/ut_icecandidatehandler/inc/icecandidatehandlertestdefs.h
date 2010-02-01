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




#ifndef ICECANDIDATEHANDLERTESTDEFS_H
#define ICECANDIDATEHANDLERTESTDEFS_H

#include <digia/eunit/CEunitAllocTestCaseDecorator.h>
#include <in_sock.h>
/*
_LIT8( KNatPluginIdNokiaIce, "nokia.ice" );
_LIT8( KNatPluginTypeTurn, "turn" );
_LIT8( KNatPluginTypeStun, "stun" );
_LIT8( KNatPluginTypeHost, "host" );
*/
_LIT8( KTestDomain, "nokia.com" );
const TUint32 KTestIap                  = 6;

_LIT8( KUsername, "user" );
_LIT8( KPassword, "pass" );

const TUint KComponentIdRtp             = 1;
const TUint KComponentIdRtcp            = 2;

const TUint KTestStreamIdRtp1           = 1;
const TUint KTestStreamIdRtcp1          = 2;
const TUint KTestStreamIdRtp2           = 3;
const TUint KTestStreamIdRtcp2          = 4;
const TUint KTestStreamCollectionId1    = 1;
const TUint KTestStreamCollectionId2    = 2;

const TUint32 KTestAddressRtp1( INET_ADDR( 192, 168, 1, 1 ) );
const TUint32 KTestPortRtp1( 5000 );

const TUint32 KTestAddressRtcp1( INET_ADDR( 192, 168, 1, 1 ) );
const TUint32 KTestPortRtcp1( 5001 );

const TUint32 KLocalAddr = INET_ADDR( 10, 0, 1, 1 );
const TUint KLocalPort = 5000;

const TUint32 KRemoteAddr = INET_ADDR( 16, 0, 0, 1 );
const TUint KRemotePort = 10000;

const TUint32 KMappedAddr = INET_ADDR( 192, 168, 1, 2 );
const TUint KMappedPort = 16384;

const TUint KTaTimerValueForGathering   = 20;
const TUint KTaTimerValueForChecks      = 20;

class TIceTestUtils
    {
public:
    static TBool AllocTestEnabled()
        {
        CEUnitTestCaseDecorator* decorator 
            = CEUnitTestCaseDecorator::ActiveTestCaseDecorator( 
            KEUnitAllocTestCaseDecoratorName );
        return ( decorator ? ETrue : EFalse );
        }
    };

#endif // ICECANDIDATEHANDLERTESTDEFS_H
