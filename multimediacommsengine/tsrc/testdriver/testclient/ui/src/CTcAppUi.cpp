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
* Description:  Implementation.
*
*/

#include <bldvariant.hrh>
#include <uikon.hrh>	// EEikCmdExit
#include <eikmenup.h>
#include <eikdoc.h>
#include <barsread.h>

#ifdef __SERIES60_
#include <errorui.h>
#include <aknquerydialog.h>
#else
#ifdef _SERIES_90_
#include <cknconf.h>
#else
#include <eikinfo.h>
#endif
#include <eikcore.rsg>
#endif

#include "CTcBearer.h"
#include "TTcBtFactory.h"
#include "TTcTcpFactory.h"
#include "TTcSerialFactory.h"
#include "CTcTestRunner.h"
#include "CTcWatcher.h"

#include "CTcAppUi.h"
#include "CTcContainer.h"
#include "CTcSettingsDialog.h"
#include "CTcBacklight.h"
#include "TestClientConstants.h"
#include "TestClient.hrh"
#include <TestClient.rsg>
	
CTcAppUi::CTcAppUi()
	{
	}

CTcAppUi::~CTcAppUi()
	{
	// These must be deleted first, they might generate some observer
	// calls to the container
	delete iRunner;
	delete iBearer;

#ifdef __SERIES60_
	delete iErrorUi;
#endif

	// Remove the container from the control stack and destroy
	if( iContainer )
		{
		RemoveFromStack( iContainer );
		delete iContainer;
		}

	delete iWatcher;
	delete iBacklight;
	}

void CTcAppUi::ConstructL()
	{
	// Have to use this one with the SIP stack
	CCoeEnv::Static()->DisableExitChecks( ETrue );

#ifdef __SERIES60_
    // Skin support is required by S60 3.0 sdk (wk02 and later?)
	BaseConstructL( EAknEnableSkin );	
#else
    BaseConstructL();
#endif

	// Load settings
	iSettings.Load();

#ifdef __SERIES60_
	// Construct error ui
	iErrorUi = CErrorUI::NewL();
#endif

	// Construct our main view
	iContainer = CTcContainer::NewL( ClientRect() );
	AddToStackL( iContainer );

	// If this is the first we are running, automatically display settings.
	if( iSettings.iIsFirstRun )
		{
		iSettings.iIsFirstRun = EFalse;
		CmdSettingsL();
		}

	// Initiate connect if AutoConnect is requested by user settings
	if( iSettings.iAutoConnect )
		{
		CmdConnectL();
		}
	else
		{
		SetStatus( KTcNotConnected );
		}

/*  causes crash!! 
	iWatcher = CTcWatcher::NewL( Document()->Application()->DllName(),
								 KWatcherTimeout );*/
	iBacklight = CTcBacklight::NewL();
	}

void CTcAppUi::HandleCommandL( TInt aCommand )
	{
	// Dispatch commands
	switch( aCommand )
		{
		case ECmdConnect:
			{
			CmdConnectL();
			break;
			}
		case ECmdDisconnect:
			{
			CmdDisconnect();
			break;
			}
		case ECmdSettings:
			{
			CmdSettingsL();
			break;
			}
		case ECmdLocalAddress:
			{
			CmdLocalAddressL();
			break;
			}
		case EEikCmdExit:
			{
			Exit();
			break;
			}
		default:
			{
			// do nothing
			break;
			}
		}
	}

void CTcAppUi::DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane )
	{
	switch( aResourceId )
		{
		// Hide "Connect" in connected mode and "Disconnect" in disconnected mode
		case R_TC_MENU:
			{
			TBool state( EFalse );
			if( iBearer )
				{
				state = ETrue;
				}
			aMenuPane->SetItemDimmed( ECmdConnect, state );
			aMenuPane->SetItemDimmed( ECmdDisconnect, !state );
			aMenuPane->SetItemDimmed( ECmdLocalAddress, !state );
			break;
			}
		default:
			{
			// not for us..
			break;
			}
		}
	}

void CTcAppUi::BearerCompletion( MTcBearerObserver::TOperation aOp,
							     TInt aStatus )
	{
	// If there are any errors, disconnect and show an appropriate error note
	if( aStatus != KErrNone )
		{
		ShowErrorNote( aStatus );
		CmdDisconnect();
		return;
		}

	switch( aOp )
		{
		// Bearer is now listening for incoming connections
		case MTcBearerObserver::EListen:
			{
			SetStatus( KTcListening );
			break;
			}

		case MTcBearerObserver::EConnecting:
		    {
		    SetStatus( KTcConnecting );
		    break;
		    }
		// Bearer is now connected
		case MTcBearerObserver::EConnect:
			{
			SetStatus( KTcConnected );
			TRAPD( err,
				{
				// CTcTestRunner takes observer role for iBearer
				iRunner = CTcTestRunner::NewL( *iBearer, *this, iSettings.iIAPId );
				// Start test case runner
				iRunner->Start();
				} );
			// Report any errors and disconnect
			if( err != KErrNone )
				{
				ShowErrorNote( err );
				CmdDisconnect();
				}
			break;
			}

		default:
			{
			// do nothing for other events (basically not encountered)
			break;
			}
		}

	}

void CTcAppUi::NotifyDisconnect()
	{
	// Test Runner has detected a remote disconnect, connect again
	TRAP_IGNORE( CmdConnectL() )
	}

void CTcAppUi::NotifyStatusChange()
	{
	// iRunner may be NULL if CTestRunner construction failed but
	// it still caused a status change notification.
	if( iRunner )
		{
		// Test Runner status has changed, update main view labels
		iContainer->Update( *iRunner );
		}
	}

void CTcAppUi::CmdConnectL()
	{
	// Make sure we're properly disconnected
	CmdDisconnect();
	SetStatus( KTcInitializing );

	// Create bearer
	TTcSettingsBearerType btype = static_cast<TTcSettingsBearerType>( iSettings.iBearerType );
	
	switch ( btype )
	    {
	    case EBearerBluetooth:
	        {
	        TTcBtFactory factory;
		    iBearer = CTcBearer::NewL( factory );
	        break;
	        }
	    case EBearerTCP:
	        {
	        TTcTcpFactory factory( iSettings.iTCPPort, iSettings.iIAPId );
		    iBearer = CTcBearer::NewL( factory );
	        break;
	        }
	    case EBearerSerial:
	        {
	        HBufC* csyName = ReadFromResourceArrayLC( R_TC_SETTINGS_CSYNAME, iSettings.iCSYName );
	        HBufC* commPort = ReadFromResourceArrayLC( R_TC_SETTINGS_COMMPORT, iSettings.iCommPort );
	        TTcSerialFactory factory( *csyName, *commPort, iSettings.iBaudrate ); 
	        CleanupStack::PopAndDestroy( 2 );
		    iBearer = CTcBearer::NewL( factory );
	        break;
	        }
	    default:
	        {
	        User::Leave( KErrArgument );
	        break;
	        }
	    }

	// Register for bearer events (->CTcAppUi::BearerCompletion)
	iBearer->SetObserver( this );
	// Start the connect sequence
	
	TInetAddr* addr = NULL;
	if ( !iSettings.iTCPRemoteAddr.IsUnspecified() )
	    {
	    addr = new ( ELeave ) TInetAddr;
	    if(iSettings.iTCPRemoteAddr.Family() == KAfInet6)
	        {
	        addr->SetAddress( iSettings.iTCPRemoteAddr.Ip6Address() );
	        }
	    else
	        {
	        addr->SetAddress( iSettings.iTCPRemoteAddr.Address() );
	        }
	    addr->SetPort( iSettings.iTCPPort );
	    }
	
	TRAPD( err, iBearer->ConnectL( addr ) );
	if( err != KErrNone )
		{
		delete addr;
		CmdDisconnect();
		User::Leave( err );
		}
    delete addr;
	}

void CTcAppUi::CmdDisconnect()
	{
	SetStatus( KTcNotConnected );

	// Delete Test Runner and Bearer
	delete iRunner;
	iRunner = NULL;
	delete iBearer;
	iBearer = NULL;
	}

void CTcAppUi::CmdSettingsL()
	{
	// Construct the settings dialog
	CTcSettingsDialog* dlg = new( ELeave ) CTcSettingsDialog( iSettings );
	CleanupStack::PushL( dlg );
	dlg->ConstructL();
	CleanupStack::Pop();	// dlg

	// ..and display it. It is automatically deleted
	dlg->ExecuteLD( R_TC_SETTINGS_DLG );
	}

void CTcAppUi::CmdLocalAddressL()
	{
	// Fetch local address(es)
	TBuf<200> address;
	iBearer->GetLocalAddressL( address );
	address.Insert( 0, _L("Local IP:\n") );

#ifdef __SERIES60_
	// Construct and show a query dialog
	CAknQueryDialog* dlg = new( ELeave ) CAknQueryDialog;
	dlg->ExecuteLD( R_TC_LOCAL_ADDRESS_DLG, address );
#else
#ifdef _SERIES_90_
	// Construct and show a confirmation dialog
	CCknConfirmationDialog::RunDlgWithDefaultIconLD( address, R_EIK_BUTTONS_CONTINUE );
#else
	CEikInfoDialog::RunDlgLD( _L("Local IP"), address, CEikInfoDialog::EAllowEnter );
#endif
#endif
	}

void CTcAppUi::SetStatus( const TDesC8& aText )
	{
	iContainer->UpdateStatus( aText );
	}

#ifdef __SERIES60_

HBufC* CTcAppUi::ReadFromResourceArrayLC( TInt aResourceId, TInt aSettingIndex )
    {
    __ASSERT_ALWAYS( aSettingIndex >= 0, User::Leave( KErrArgument ) );
    
    TResourceReader reader;
    CEikonEnv::Static()->CreateResourceReaderLC( reader, aResourceId );
    
    TInt numberOfItems = reader.ReadInt16(); // item count

    __ASSERT_ALWAYS( aSettingIndex < numberOfItems, User::Leave( KErrArgument ) );
    
    TBool done( EFalse );
    HBufC* temp = 0;
    for ( TInt i = 0; i < numberOfItems && !done; i++ )
        {
        TInt value = reader.ReadInt16();  
        TPtrC text = reader.ReadTPtrC();
        if ( i == aSettingIndex )
            {
            done = ETrue;
            temp = text.AllocL();
            }
        }
    CleanupStack::PopAndDestroy(); // reader
    CleanupStack::PushL( temp );
    return temp;
    }
    
#else

HBufC* CTcAppUi::ReadFromResourceArrayLC( TInt aResourceId, TInt aSettingIndex )
    {
    __ASSERT_ALWAYS( aSettingIndex >= 0, User::Leave( KErrArgument ) );
    CDesC16ArrayFlat* array = iCoeEnv->ReadDesC16ArrayResourceL( aResourceId );
    CleanupStack::PushL( array );
    __ASSERT_ALWAYS( aSettingIndex < array->MdcaCount(), User::Leave( KErrArgument ) );
    HBufC* temp = array->MdcaPoint( aSettingIndex ).AllocL();
    CleanupStack::PopAndDestroy( array );
    CleanupStack::PushL( temp );
    return temp;
    }
    
#endif
    
void CTcAppUi::ShowErrorNote( TInt aStatus )
	{
	if( aStatus != KErrNone )
		{
#ifdef __SERIES60_
		// If we run out of memory (etc.), just ignore the error
		TRAP_IGNORE( iErrorUi->ShowGlobalErrorNoteL( aStatus ) )
#else
		// Construct and show a confirmation dialog
		TBuf< 32 > errorMsg;
		errorMsg.Format( _L("Fatal error: %d"), aStatus );
#ifdef _SERIES_90_
		TRAP_IGNORE( CCknConfirmationDialog::RunDlgWithDefaultIconLD( errorMsg, R_EIK_BUTTONS_CONTINUE ) )
#else
		TRAP_IGNORE( CEikInfoDialog::RunDlgLD( _L("Error"), errorMsg, CEikInfoDialog::EAllowEnter ) )
#endif
#endif
		}
	}
