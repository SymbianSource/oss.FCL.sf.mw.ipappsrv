/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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

#include <e32std.h>
#include <e32base.h>
#include <e32debug.h>
#include <e32cons.h>
#include <badesca.h>
#include <sdpconnectionfield.h>
#include <sdpmediafield.h>
#include "nsptestconsolecenrep.h"
#include "nsptestconsolestream.h"

_LIT8( KLocalAddr, "192.168.0.2" );
const TUint KLocalPort = 8000;

#define __PANIC( aError ) User::Panic( _L( "NSP Test console stream" ), aError )

// -----------------------------------------------------------------------------
// CTestConsoleStream::NewL
// -----------------------------------------------------------------------------
// 
CTestConsoleStream* CTestConsoleStream::NewL()
    {
    CTestConsoleStream* self = CTestConsoleStream::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CTestConsoleStream::NewLC
// -----------------------------------------------------------------------------
// 
CTestConsoleStream* CTestConsoleStream::NewLC()
    {
    CTestConsoleStream* self = new ( ELeave ) CTestConsoleStream();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CTestConsoleStream::CTestConsoleStream
// -----------------------------------------------------------------------------
// 
CTestConsoleStream::CTestConsoleStream()
	: iLocalPort( KLocalPort )
	{
    }

// -----------------------------------------------------------------------------
// CTestConsoleStream::ConstructL
// -----------------------------------------------------------------------------
// 
void CTestConsoleStream::ConstructL()
    {
    iLocalAddress = KLocalAddr().AllocL();
	}

// -----------------------------------------------------------------------------
// CTestConsoleStream::~CTestConsoleStream
// -----------------------------------------------------------------------------
// 
CTestConsoleStream::~CTestConsoleStream()
    {
	delete iLocalAddress;
	delete iRemoteAddress;
    }

// -----------------------------------------------------------------------------
// CTestConsoleStream::EncodeL
// -----------------------------------------------------------------------------
// 
TInt CTestConsoleStream::EncodeL( CSdpMediaField& aMediaField )
    {
    __ASSERT_ALWAYS( &aMediaField, User::Leave( KErrArgument ) );
    
    CSdpConnectionField* connField =
    		( aMediaField.ConnectionFields().Count() ?
    				aMediaField.ConnectionFields()[0] : NULL );
    
    if ( iLocalAddress && connField )
    	{
    	connField->SetAddressL( connField->NetType(),
    			connField->AddressType(), iLocalAddress->Des() );
    	}
    
	aMediaField.SetPortL( iLocalPort );
	
	return KErrNone;
    }

// -----------------------------------------------------------------------------
// CTestConsoleStream::DecodeL
// -----------------------------------------------------------------------------
// 
TInt CTestConsoleStream::DecodeL( CSdpMediaField& aMediaField )
    {
    __ASSERT_ALWAYS( &aMediaField, User::Leave( KErrArgument ) );
    
    CSdpConnectionField* connField =
    		( aMediaField.ConnectionFields().Count() ?
    				aMediaField.ConnectionFields()[0] : NULL );
    
    iRemoteAddress = ( connField ? connField->Address().AllocL() : NULL );
    
    iRemotePort = aMediaField.Port();
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CTestConsoleStream::Start
// -----------------------------------------------------------------------------
// 
TInt CTestConsoleStream::Start()
	{
	return KErrNotSupported;
	}

// -----------------------------------------------------------------------------
// CTestConsoleStream::Stop
// -----------------------------------------------------------------------------
// 
TInt CTestConsoleStream::Stop()
	{
	return KErrNotSupported;
	}

// -----------------------------------------------------------------------------
// CTestConsoleStream::LocalAddress
// -----------------------------------------------------------------------------
// 
const TDesC8& CTestConsoleStream::LocalAddress() const
	{
	return *iLocalAddress;
	}

// -----------------------------------------------------------------------------
// CTestConsoleStream::RemoteAddress
// -----------------------------------------------------------------------------
// 
const TDesC8& CTestConsoleStream::RemoteAddress() const
	{
	return *iRemoteAddress;
	}

// -----------------------------------------------------------------------------
// CTestConsoleStream::LocalPort
// -----------------------------------------------------------------------------
// 
TUint CTestConsoleStream::LocalPort() const
	{
	return iLocalPort;
	}

// -----------------------------------------------------------------------------
// CTestConsoleStream::RemotePort
// -----------------------------------------------------------------------------
// 
TUint CTestConsoleStream::RemotePort() const
	{
	return iRemotePort;
	}
