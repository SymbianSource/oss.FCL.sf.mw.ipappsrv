/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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

#include <btmanclient.h>
#include <bt_sock.h>

#ifdef __SERIES60_
	#include <btnotif.h>
#endif

#include "CTcBtManager.h"
#include "TTcBtFactory.h"

#include "debuglog.h"

/// See https://www.bluetooth.org/foundry/assignnumb/document/service_discovery
const TInt KTcSerialClassID( 0x1101 );
const TInt KTcRootBrowseGroup( 0x1002 );

/// Name and description of our BT service in SDP.
_LIT( KTcServiceName, "TestClient" );
_LIT( KTcServiceDescription, "Serial IUTCP connection" );

CTcBtManager* CTcBtManager::NewL( const TTcBtFactory& aFactory )
	{
	CTcBtManager* self = new( ELeave ) CTcBtManager( aFactory );

	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );

	return self;
	}

CTcBtManager::~CTcBtManager()
	{
	LOG( _L("CTcBtManager::~CTcBtManager()") );
	Close();
	iSockServ.Close();
	}

CTcBtManager::CTcBtManager( const TTcBtFactory& /*aFactory*/ )
	{
	}

void CTcBtManager::ConstructL()
	{
	LOG( _L("CTcBtManager::ConstructL()") );
	User::LeaveIfError( iSockServ.Connect() );
	iConn.SetObserver( this );
	iConn.SetConnection( &iSockServ, NULL );
	}

void CTcBtManager::ConnectL( TInetAddr* /*aRemoteAddr*/ )
	{
	LOG( _L("CTcBtManager::ConnectL()") );
	// Make sure that BT is enabled
	TurnOnBtL();

	// Start waiting for connections
	iConn.ConnectL();

#ifndef __BLUETOOTH_API_V2__
	// Define security requirements (lowest possible)
	SetSecurityL( iConn.LocalPort(), EFalse, EFalse, EFalse );
#endif

	// Start advertising the local BT channel for our service
	StartAdvertiserL( iConn.LocalPort() );
	LOG( _L("CTcBtManager::ConnectL() end") );
	}

void CTcBtManager::Close()
	{
	LOG( _L("CTcBtManager::Close()") );
	// Stop advertising our service
	StopAdvertiser();

	// Shut down the service port
	iConn.Close();
	}

void CTcBtManager::Send( const TDesC8& aDes )
	{
	iConn.Send( aDes );
	}

void CTcBtManager::Receive( TDes8& aDes )
	{
	iConn.Receive( aDes );
	}

void CTcBtManager::ReceiveOneOrMore( TDes8& aDes )
	{
	iConn.ReceiveOneOrMore( aDes );
	}

void CTcBtManager::SetObserver( MTcBearerObserver* aObserver )
	{
	iObserver = aObserver;
	}

void CTcBtManager::GetLocalAddressL( TDes& /*aDes*/ )
	{
	User::Leave( KErrNotSupported );
	}

void CTcBtManager::BearerCompletion( MTcBearerObserver::TOperation aOp,
									 TInt aStatus )
	{
	LOG( _L("CTcBtManager::BearerCompletion( %d, %d ) start"), aOp, aStatus );
	if( ( aOp == MTcBearerObserver::EConnect ) && !aStatus )
		{
		// A client has connected, mark the service as fully booked
		// It's safe to ignore any leaves
		TRAP_IGNORE( MakeAvailableL( EFalse ) )
		}

	if( iObserver )
		{
		iObserver->BearerCompletion( aOp, aStatus );
		}
	LOG( _L("CTcBtManager::BearerCompletion() end") );
	}

void CTcBtManager::StartAdvertiserL( TInt aChannel )
    {
	LOG( _L("CTcBtManager::StartAdvertiserL( %d ) start"), aChannel );
	StopAdvertiser();

	User::LeaveIfError( iSdpServ.Connect() );
	User::LeaveIfError( iSdpDb.Open( iSdpServ ) );

	// Create service record
	iSdpDb.CreateServiceRecordL( KTcSerialClassID, iSdpRecord );

 	LOG( _L("CTcBtManager::StartAdvertiserL() created service record") );

	// Add a Protocol to the record
	CSdpAttrValueDES* protocolDesList = CSdpAttrValueDES::NewDESL( NULL );
	CleanupStack::PushL( protocolDesList );

    TBuf8< 1 > channel;
    channel.Append( (TChar)aChannel );

	protocolDesList
	->StartListL()	 //  List of protocols required for this method
		->BuildDESL()
		->StartListL()
			->BuildUUIDL( KL2CAP )
		->EndListL()

		->BuildDESL()
		->StartListL()
			->BuildUUIDL( KRFCOMM )
			->BuildUintL( channel )
		->EndListL()
	->EndListL();

	iSdpDb.UpdateAttributeL( iSdpRecord,
							 KSdpAttrIdProtocolDescriptorList,
							 *protocolDesList );

	CleanupStack::PopAndDestroy( protocolDesList );

	LOG( _L("CTcBtManager::StartAdvertiserL() adding uid") );

	// Add a Unique ID to the record
	iSdpDb.UpdateAttributeL( iSdpRecord,
							 KSdpAttrIdServiceID,
							 0x10001234 );	// REPLACE!!

	LOG( _L("CTcBtManager::StartAdvertiserL() adding name") );

	// Add a name to the record
	iSdpDb.UpdateAttributeL( iSdpRecord,
							 KSdpAttrIdBasePrimaryLanguage
							 + KSdpAttrIdOffsetServiceName,
							 KTcServiceName );

	LOG( _L("CTcBtManager::StartAdvertiserL() adding description") );

	// Add a description to the record
	iSdpDb.UpdateAttributeL( iSdpRecord,
							 KSdpAttrIdBasePrimaryLanguage
							 + KSdpAttrIdOffsetServiceDescription,
							 KTcServiceDescription );

	// "Attach" our service to the root browse group.
	// This is required by some BT devices for successful remote SDP query
	TUUID rootTUUID( KTcRootBrowseGroup );
	CSdpAttrValueUUID* rootBrowseGroupAttr = CSdpAttrValueUUID::NewUUIDL( rootTUUID );
	CleanupStack::PushL( rootBrowseGroupAttr );
	iSdpDb.UpdateAttributeL( iSdpRecord, KSdpAttrIdBrowseGroupList, *rootBrowseGroupAttr );
	CleanupStack::PopAndDestroy( rootBrowseGroupAttr );

	MakeAvailableL( ETrue );
	LOG( _L("CTcBtManager::StartAdvertiserL() end") );
	}

void CTcBtManager::StopAdvertiser()
	{
	LOG( _L("CTcBtManager::StopAdvertiser()") );
	if( iSdpDb.SubSessionHandle() && iSdpRecord )
		{
		TRAP_IGNORE( iSdpDb.DeleteRecordL( iSdpRecord ) )
		iSdpRecord = 0;
		}
	}

void CTcBtManager::MakeAvailableL( TBool aIsAvailable )
    {
	// Fully unused (0xFF) or Fully used (0x00)
    TUint state = aIsAvailable ? 0xFF : 0x00;
	LOG( _L("CTcBtManager::MakeAvailableL( %d ) start, state = %d"), aIsAvailable, state );

     //  Update the availibility attribute field
    iSdpDb.UpdateAttributeL( iSdpRecord, KSdpAttrIdServiceAvailability, state );

    //  Mark the record as changed - by increasing its state number (version)
    iSdpDb.UpdateAttributeL( iSdpRecord, KSdpAttrIdServiceRecordState,
							 ++iSdpRecordState );
   	LOG( _L("CTcBtManager::MakeAvailableL() end") );
	}

// Note that security settings should be implemented for 8.x in BtConnection level
// That is not done yet!!!!! TODO when 8.x hw must be used
#ifndef __BLUETOOTH_API_V2__
void CTcBtManager::SetSecurityL( TInt aChannel,
								 TBool aAuthentication,
								 TBool aEncryption,
								 TBool aAuthorisation )
	{
   	LOG( _L("CTcBtManager::SetSecurityL() start") );
	RBTMan secManager;
	User::LeaveIfError( secManager.Connect() );
	CleanupClosePushL( secManager );

	RBTSecuritySettings secSession;
	User::LeaveIfError( secSession.Open( secManager ) );
	CleanupClosePushL( secSession );

	// Define security requirements for our application and RFCOMM.
	TBTServiceSecurity settings( TUid::Uid( 0x10001234 ), KSolBtRFCOMM, aChannel );
	settings.SetAuthentication( aAuthentication );
	settings.SetEncryption( aEncryption );
	settings.SetAuthorisation( aAuthorisation );

	// Register security settings, wait for async operation to complete
	TRequestStatus status;
	secSession.RegisterService( settings, status );
	User::WaitForRequest( status );

	// Report any errors
	User::LeaveIfError( status.Int() );

	CleanupStack::PopAndDestroy( 2 );	//	secSession, secManager
   	LOG( _L("CTcBtManager::SetSecurityL() end") );
	}
#endif

void CTcBtManager::TurnOnBtL()
	{
#ifdef __SERIES60_
   	LOG( _L("CTcBtManager::TurnOnBtL() start") );

	// Connect to notitifier service
	RNotifier notifier;
	User::LeaveIfError( notifier.Connect() );
	CleanupClosePushL( notifier );

	// Request notification about power, don't care about the results 
	// This results in a UI popup being displayed to the user shortly
	TRequestStatus status;
	TPckgBuf< TBool > pckg;
	TPckgBuf< TBool > resultPckg;
	notifier.StartNotifierAndGetResponse( status,
										  KPowerModeSettingNotifierUid,
										  pckg,
										  resultPckg );
	User::WaitForRequest( status );
	// Disable error checking. this would make auto-reconnect impossible
	// The power notifier reports an error if the time between the last
	// connection attempt and this connection is too small (even if it
	// doesn't hurt..)
	//User::LeaveIfError( status.Int() );

	notifier.CancelNotifier( KPowerModeSettingNotifierUid );

	CleanupStack::PopAndDestroy(); // notifier
   	LOG( _L("CTcBtManager::TurnOnBtL() end") );
#endif
	}
