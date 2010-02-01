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
* Description:  Implementation
*
*/

#include "CTcSerialManager.h"
#include "TTcSerialFactory.h"

#include "debuglog.h"

_LIT(LDD_NAME,"ECOMM");

#if defined (__WINS__)
_LIT(PDD_NAME,"ECDRV");
#else
_LIT(PDD_NAME,"EUART1");
#endif

CTcSerialManager* CTcSerialManager::NewL( const TTcSerialFactory& aFactory )
	{
	CTcSerialManager* self = new( ELeave ) CTcSerialManager( aFactory );

	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );

	return self;
	}

CTcSerialManager::~CTcSerialManager()
	{
	LOG( _L("CTcSerialManager::~CTcSerialManager()") );
	
	Close();	
	delete iConn;
	iCommServ.Close();
	}

CTcSerialManager::CTcSerialManager( const TTcSerialFactory& aFactory ) :
    iFactory( aFactory )
	{
	}

void CTcSerialManager::ConstructL()
	{
	LOG( _L("CTcSerialManager::ConstructL()") );
	
	LoadDriversAndCommModuleL();
	
	iConn = new ( ELeave ) CTcSerialConnection( iCommServ, 
	                                            iFactory.iPortName, 
	                                            iFactory.iPortSpeed );
	
	iConn->SetObserver( this );
	
	LOG( _L("CTcSerialManager::ConstructL() end") );
	}

void CTcSerialManager::ConnectL( TInetAddr* /*aRemoteAddr*/ )
	{
	LOG( _L("CTcSerialManager::ConnectL()") );

	iConn->ConnectL();

	LOG( _L("CTcSerialManager::ConnectL() end") );
	}

void CTcSerialManager::Close()
	{
	LOG( _L("CTcSerialManager::Close()") );

    iConn->Close();
	}

void CTcSerialManager::Send( const TDesC8& aDes )
	{
	iConn->Send( aDes );
	}

void CTcSerialManager::Receive( TDes8& aDes )
	{
	iConn->Receive( aDes );
	}

void CTcSerialManager::ReceiveOneOrMore( TDes8& aDes )
	{
	iConn->ReceiveOneOrMore( aDes );
	}

void CTcSerialManager::SetObserver( MTcBearerObserver* aObserver )
	{
	iObserver = aObserver;
	}

void CTcSerialManager::GetLocalAddressL( TDes& /*aDes*/ )
	{
	User::Leave( KErrNotSupported );
	}

void CTcSerialManager::BearerCompletion(
    MTcBearerObserver::TOperation aOp,
	TInt aStatus )
	{
	LOG( _L("CTcSerialManager::BearerCompletion( %d, %d ) start"), aOp, aStatus );
	if( ( aOp == MTcBearerObserver::EConnect ) && !aStatus )
		{
		// A client has connected
		
		}

	if( iObserver )
		{
		iObserver->BearerCompletion( aOp, aStatus );
		}
	LOG( _L("CTcSerialManager::BearerCompletion() end") );
	}
	
void CTcSerialManager::LoadDriversAndCommModuleL()
    {
    TInt err = User::LoadPhysicalDevice( PDD_NAME );
	if ( err != KErrNone && err != KErrAlreadyExists )
	    {
		User::Leave( err );
	    }
	    
	err = User::LoadLogicalDevice( LDD_NAME );
	if ( err != KErrNone && err != KErrAlreadyExists )
	    {
		User::Leave( err );
	    }

	// Both WINS and EIKON will have started the comms server process.
	// (this is only really needed for ARM hardware development racks)

#if !defined (__WINS__)
	err = StartC32 ();
	if ( err != KErrNone && err != KErrAlreadyExists )
	    {
		User::Leave( err );
	    }
#endif


	User::LeaveIfError( iCommServ.Connect() );

	// Load the CSY module
	// Symbian OS will automatically search \System\Libs
	// on all drives starting from C:

	User::LeaveIfError( iCommServ.LoadCommModule( iFactory.iCSYName ) );
    }
	
// End of file
