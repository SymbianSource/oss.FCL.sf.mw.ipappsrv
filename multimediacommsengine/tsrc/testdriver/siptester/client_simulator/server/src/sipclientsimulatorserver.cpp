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
* Description:  Implementation of the server's main class
*
*/

#include "sipclientsimulatorserver.h"
#include "sipclientsimulatorsession.h"
#include "sipclientsimulator.h"
#include "sipclientsimulatorconsts.h"
#include "sdpmediafield.h"
#include "sipcontenttypeheader.h"
#include "sdpcodecstringpool.h"

// ----------------------------------------------------------------------------
// E32Main
// ----------------------------------------------------------------------------
//
TInt E32Main() 
    { 
    return CSipClientSimulatorServer::Main();
    }

// ----------------------------------------------------------------------------
// CSipClientSimulatorServer::NewLC
// ----------------------------------------------------------------------------
//
CSipClientSimulatorServer* CSipClientSimulatorServer::NewLC()
	{
	CSipClientSimulatorServer* self = new( ELeave )CSipClientSimulatorServer;
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

// ----------------------------------------------------------------------------
// CSipClientSimulatorServer::CSipClientSimulatorServer
// ----------------------------------------------------------------------------
//
CSipClientSimulatorServer::CSipClientSimulatorServer()
	: CServer2( EPriorityStandard ),
	  iMatchType( RSipClientSimulator::ESipNoClientMatch )
	{
	}

// ----------------------------------------------------------------------------
// CSipClientSimulatorServer::ConstructL
// ----------------------------------------------------------------------------
//
void CSipClientSimulatorServer::ConstructL()
	{
	SdpCodecStringPool::OpenL();
	}


// ----------------------------------------------------------------------------
// CSipClientSimulatorServer::~CSipClientSimulatorServer
// ----------------------------------------------------------------------------
//
CSipClientSimulatorServer::~CSipClientSimulatorServer()
	{
	delete iSimulator;
	iContentTypeHeaders.ResetAndDestroy();
	iSdpMediaField.ResetAndDestroy();
	SdpCodecStringPool::Close();
	}

// ----------------------------------------------------------------------------
// CSipClientSimulatorServer::Main
// ----------------------------------------------------------------------------
//
TInt CSipClientSimulatorServer::Main()
	{
	__UHEAP_MARK;

	// Create the cleanup stack
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if( cleanup == NULL )
		{
		PanicServer( KErrNoMemory );
		}

	// Initialize and start the server
	TRAPD( error, InitServerL() );
	if( error != KErrNone )
		{
		PanicServer( error );
		}

	// InitServerL() will hold execution for the lifetime
	// of the server. The server has died now, delete cleanup stack.
	delete cleanup;

	__UHEAP_MARKEND;

	return KErrNone;
	}

// ----------------------------------------------------------------------------
// CSipClientSimulatorServer::NewSessionL
// ----------------------------------------------------------------------------
//
CSession2* CSipClientSimulatorServer::NewSessionL( 
    const TVersion& aVersion,
	const RMessage2& /*aMessage*/ ) const
	{
	// Check that the client version is OK
	TVersion version( KSipClientSimulatorMajorVersion,
					  KSipClientSimulatorMinorVersion,
					  KSipClientSimulatorBuildVersion );
	if( !User::QueryVersionSupported( version, aVersion ) )
		{
		User::Leave( KErrNotSupported );
		}

	// Create a new session
	return CSipClientSimulatorSession::NewL( 
	    const_cast< CSipClientSimulatorServer& >( *this ) );
	}

// ----------------------------------------------------------------------------
// CSipClientSimulatorServer::RunError
// ----------------------------------------------------------------------------
//
TInt CSipClientSimulatorServer::RunError( TInt aError )
	{
	// Panic client if we had an error that is caused by the client itself
	if( ( aError == KErrBadDescriptor ) ||
		( aError == KErrBadHandle ) )
		{
		Message().Panic( KSipClientSimulatorName, aError );
		}
	// Otherwise just complete the request with an error code
	else
		{
		Message().Complete( aError );
		}

	// Restart server as it was interrupted when
	// the leave at CServer::RunL() happened
	ReStart();

	// We're fully recovered now
	return KErrNone;
	}

// ----------------------------------------------------------------------------
// CSipClientSimulatorServer::SessionAdded
// ----------------------------------------------------------------------------
//
void CSipClientSimulatorServer::SessionAdded()
	{
	iSessionCount++;
	}

// ----------------------------------------------------------------------------
// CSipClientSimulatorServer::SessionRemoved
// ----------------------------------------------------------------------------
//
void CSipClientSimulatorServer::SessionRemoved()
	{
	iSessionCount--;

	if( iSessionCount == 0 )
		{
		// No more server users, shutdown server
		CActiveScheduler::Stop();
		}
	}

// ----------------------------------------------------------------------------
// CSipClientSimulatorServer::InitServerL
// ----------------------------------------------------------------------------
//
void CSipClientSimulatorServer::InitServerL()
	{
	// Open the semaphore that was created by the first client
	RSemaphore semaphore;
	User::LeaveIfError( semaphore.OpenGlobal( KSipClientSimulatorName ) );

	CActiveScheduler* scheduler = NULL;
	CSipClientSimulatorServer* srv = NULL;

	// We don't want the client waiting on the semaphore indefinitely
	// even if server start fails.
	TRAPD( err,
		{
		// Start scheduler and server
		scheduler = new( ELeave ) CActiveScheduler;
		CleanupStack::PushL( scheduler );
		CActiveScheduler::Install( scheduler );

		// Create server instance
		srv = CSipClientSimulatorServer::NewLC( );
		// Start the server using CServer::StartL()
		srv->StartL( KSipClientSimulatorName );
		// we have to pop this before crossing TRAP boundary
		CleanupStack::Pop( 2 );	// srv, scheduler
		} )

	// Signal the client that we are ready and willing
	semaphore.Signal();
	semaphore.Close();
	User::LeaveIfError( err );

	// Start fielding requests from clients (execution stops here)
	CActiveScheduler::Start();

	// Remove the active scheduler
	CActiveScheduler::Install( NULL );

	delete srv;
	delete scheduler;
	}

// ----------------------------------------------------------------------------
// CSipClientSimulatorServer::CreateClientSimulatorL
// ----------------------------------------------------------------------------
//
void CSipClientSimulatorServer::CreateClientSimulatorL( 
    const TUid& aClientUid )
    {
    delete iSimulator;
    iSimulator = CSipClientSimulator::NewL( aClientUid );
    }

// ----------------------------------------------------------------------------
// CSipClientSimulatorServer::DestroyClientSimulator
// ----------------------------------------------------------------------------
//        
void CSipClientSimulatorServer::DestroyClientSimulator()
    {
    delete iSimulator;
    iSimulator = NULL; 
    }

// ----------------------------------------------------------------------------
// CSipClientSimulatorServer::SetClientMatchType
// ----------------------------------------------------------------------------
// 
void CSipClientSimulatorServer::SetClientMatchType(
    RSipClientSimulator::TMatchType aMatchType,
    const TUid& aClientUid )
    {
    iMatchType = aMatchType;
    iClientUid.iUid = aClientUid.iUid;
    }

// ----------------------------------------------------------------------------
// CSipClientSimulatorServer::SetClientSupportedContentTypesL
// ----------------------------------------------------------------------------
// 
void CSipClientSimulatorServer::SetClientSupportedContentTypesL(
    				RPointerArray<CSIPContentTypeHeader> aContentTypeHeaders,
    				const TUid& aClientUid )
    {
    iContentTypeHeaders.ResetAndDestroy();
    iContentTypeHeaders = aContentTypeHeaders;
    iClientUid.iUid = aClientUid.iUid;
    }

// ----------------------------------------------------------------------------
// CSipClientSimulatorServer::SetClientSupportedSdpMediasL
// ----------------------------------------------------------------------------
// 
void CSipClientSimulatorServer::SetClientSupportedSdpMediasL(
    				RPointerArray<CSdpMediaField> aSdpMediaFields,
    				const TUid& aClientUid )
    {
    iSdpMediaField.ResetAndDestroy();
    iSdpMediaField = aSdpMediaFields;
    iClientUid.iUid = aClientUid.iUid;
    }
    
// ----------------------------------------------------------------------------
// CSipClientSimulatorServer::ClientSupportedContentTypesL
// ----------------------------------------------------------------------------
//        
RPointerArray<CSIPContentTypeHeader>& 
CSipClientSimulatorServer::ClientSupportedContentTypesL()
    {   
    return iContentTypeHeaders;
    }    
    
// ----------------------------------------------------------------------------
// CSipClientSimulatorServer::ClientSupportedSdpMediasL
// ----------------------------------------------------------------------------
//        
RPointerArray<CSdpMediaField>&
CSipClientSimulatorServer::ClientSupportedSdpMediasL() 
    {
    return iSdpMediaField;
    }       

// ----------------------------------------------------------------------------
// CSipClientSimulatorServer::ClientMatchType
// ----------------------------------------------------------------------------
//        
RSipClientSimulator::TMatchType 
CSipClientSimulatorServer::ClientMatchType() const
    {
    return iMatchType;
    }

// ----------------------------------------------------------------------------
// CSipClientSimulatorServer::ClientUid
// ----------------------------------------------------------------------------
//
const TUid& CSipClientSimulatorServer::ClientUid() const
    {
    return iClientUid;
    }

// ----------------------------------------------------------------------------
// CSipClientSimulatorServer::PanicServer
// ----------------------------------------------------------------------------
//
void CSipClientSimulatorServer::PanicServer( TInt aPanic )
	{
	User::Panic( KSipClientSimulatorName, aPanic );
	}
