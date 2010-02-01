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
* Description:    Class for reserving ports on specific range
*
*/




#include "natfwportstore.h"
#include "ncmconnectionmultiplexerlogs.h"

// ---------------------------------------------------------------------------
// CNATFWPortStore::CNATFWPortStore
// ---------------------------------------------------------------------------
//
CNATFWPortStore::CNATFWPortStore( TUint aStartPort, TUint aStopPort ) : 
    iStartPort( aStartPort ), iStopPort( aStopPort )
    {
    }


// ---------------------------------------------------------------------------
// CNATFWPortStore::NewL
// ---------------------------------------------------------------------------
//
CNATFWPortStore* CNATFWPortStore::NewL( TUint aStartPort, TUint aStopPort )
    {
    __CONNECTIONMULTIPLEXER( "CNATFWPortStore::NewL" )
    
    CNATFWPortStore* self = CNATFWPortStore::NewLC( aStartPort, aStopPort );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWPortStore::NewLC
// ---------------------------------------------------------------------------
//
CNATFWPortStore* CNATFWPortStore::NewLC( TUint aStartPort, TUint aStopPort )
    {
    __CONNECTIONMULTIPLEXER( "CNATFWPortStore::NewLC" )
    
    CNATFWPortStore* self = new( ELeave ) CNATFWPortStore( aStartPort,
                                                           aStopPort );
    CleanupStack::PushL( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWPortStore::~CNATFWPortStore
// ---------------------------------------------------------------------------
//
CNATFWPortStore::~CNATFWPortStore()
    {
    __CONNECTIONMULTIPLEXER( "CNATFWPortStore::~CNATFWPortStore" )
    
    iReservedPorts.Close();
    }


// ---------------------------------------------------------------------------
// CNATFWPortStore::Port
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CNATFWPortStore::Port()
    {
    __CONNECTIONMULTIPLEXER( "CNATFWPortStore::Port" )
    
    TUint port( iStartPort );
    
    while ( port <= iStopPort )
        {
        if ( iReservedPorts.Find( port ) == KErrNotFound )
            {
            break;
            }
        port++;
        }
        
    if ( port > iStopPort )
        {
        // no more ports exists!
        return KErrNone;
        }
        
    iReservedPorts.Append( port );
    return port;
    }


// ---------------------------------------------------------------------------
// CNATFWPortStore::FreePort
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWPortStore::FreePort( TUint aPort )
    {
    __CONNECTIONMULTIPLEXER( "CNATFWPortStore::FreePort" )
    
    TInt result( iReservedPorts.Find( aPort ) );
    
    if ( KErrNotFound != result )
        {
        iReservedPorts.Remove( result );
        }
    }
