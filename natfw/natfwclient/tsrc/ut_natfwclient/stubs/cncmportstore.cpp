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




#include "cncmportstore.h"
#include "ncmconnectionmultiplexerlogs.h"

// ---------------------------------------------------------------------------
// CNcmPortStore::CNcmPortStore
// ---------------------------------------------------------------------------
//
CNcmPortStore::CNcmPortStore( TUint aStartPort, TUint aStopPort ) : 
    iStartPort( aStartPort ), iStopPort( aStopPort )
    {
    }


// ---------------------------------------------------------------------------
// CNcmPortStore::NewL
// ---------------------------------------------------------------------------
//
CNcmPortStore* CNcmPortStore::NewL( TUint aStartPort, TUint aStopPort )
    {
    __CONNECTIONMULTIPLEXER( "CNcmPortStore::NewL" )
    
    CNcmPortStore* self = CNcmPortStore::NewLC( aStartPort, aStopPort );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNcmPortStore::NewLC
// ---------------------------------------------------------------------------
//
CNcmPortStore* CNcmPortStore::NewLC( TUint aStartPort, TUint aStopPort )
    {
    __CONNECTIONMULTIPLEXER( "CNcmPortStore::NewLC" )
    
    CNcmPortStore* self = new( ELeave ) CNcmPortStore( aStartPort,
                                                           aStopPort );
    CleanupStack::PushL( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNcmPortStore::~CNcmPortStore
// ---------------------------------------------------------------------------
//
CNcmPortStore::~CNcmPortStore()
    {
    __CONNECTIONMULTIPLEXER( "CNcmPortStore::~CNcmPortStore" )
    
    iReservedPorts.Close();
    }


// ---------------------------------------------------------------------------
// CNcmPortStore::Port
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CNcmPortStore::Port()
    {
    __CONNECTIONMULTIPLEXER( "CNcmPortStore::Port" )
    
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
// CNcmPortStore::FreePort
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmPortStore::FreePort( TUint aPort )
    {
    __CONNECTIONMULTIPLEXER( "CNcmPortStore::FreePort" )
    
    TInt result( iReservedPorts.Find( aPort ) );
    
    if ( KErrNotFound != result )
        {
        iReservedPorts.Remove( result );
        }
    }
