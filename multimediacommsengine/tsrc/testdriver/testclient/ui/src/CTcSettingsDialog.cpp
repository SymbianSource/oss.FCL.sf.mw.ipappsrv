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

#include <eikchlst.h>
#include <eikdef.h>
#include <eikenv.h>
#include "CTcIAPManager.h"

#include "CTcSettingsDialog.h"
#include "TTcSettings.h"
#include "TestClient.hrh"
#include <TestClient.rsg>

CTcSettingsDialog::CTcSettingsDialog( TTcSettings& aSettings )
	: iSettings( aSettings )
	{
	}

CTcSettingsDialog::~CTcSettingsDialog()
	{
	delete iIAPManager;
	}

void CTcSettingsDialog::ConstructL()
	{
	iIAPManager = CTcIAPManager::NewL();
	}

void CTcSettingsDialog::PreLayoutDynInitL()
	{
	CEikChoiceList* iapList =
		reinterpret_cast< CEikChoiceList* >( Control( ECtrlIAPName ) );

	CDesCArray* names = iapList->DesCArray();
	names->Delete( 0 );	// delete dummy item from array

	TInt count = iIAPManager->MdcaCount();
	for( TInt i = 0; i < count; i++ )
		{
		TBuf< KCommsDbSvrMaxFieldLength > name16;
		name16.Copy( iIAPManager->MdcaPoint( i ) );
		names->AppendL( name16 );
		if( (TInt)iIAPManager->Id( i ) == iSettings.iIAPId )
			{
			iapList->SetCurrentItem( i );
			}
		}

	SetChoiceListCurrentItem( ECtrlConnection, iSettings.iBearerType );
	
	TBuf<40> remoteAddrText;
	iSettings.iTCPRemoteAddr.Output( remoteAddrText );
	SetEdwinTextL(ECtrlRemoteAddr, &remoteAddrText);
	
	SetNumberEditorValue( ECtrlTcpPort, iSettings.iTCPPort );

    SetChoiceListCurrentItem( ECtrlCSYName, iSettings.iCSYName );
    SetChoiceListCurrentItem( ECtrlCommPort, iSettings.iCommPort );
    SetNumberEditorValue( ECtrlBaudrate, iSettings.iBaudrate );
    
	TInt autoconnect( 1 );
	if( !iSettings.iAutoConnect )
		{
		autoconnect = 0;	// no autoconnect
		}
	SetChoiceListCurrentItem( ECtrlAutoConnect, autoconnect );
	}

TBool CTcSettingsDialog::OkToExitL( TInt /*aKeycode*/ )
	{
	CEikChoiceList* iapList =
		reinterpret_cast< CEikChoiceList* >( Control( ECtrlIAPName ) );
	iSettings.iIAPId = iIAPManager->Id( iapList->CurrentItem() );

	iSettings.iBearerType = ChoiceListCurrentItem( ECtrlConnection );
	
	TBuf<40> remoteAddrText;
	GetEdwinText(remoteAddrText, ECtrlRemoteAddr);
	if (KErrNone != iSettings.iTCPRemoteAddr.Input(remoteAddrText))
		{
		_LIT(KInvalidAddressText, "Invalid remote IP");
		CEikonEnv::Static()->AlertWin(KInvalidAddressText);
		return EFalse;
		}

	iSettings.iTCPPort = NumberEditorValue( ECtrlTcpPort );
	
	iSettings.iCSYName = ChoiceListCurrentItem( ECtrlCSYName );
	                            
	iSettings.iCommPort = ChoiceListCurrentItem( ECtrlCommPort );
	                            
	iSettings.iBaudrate = NumberEditorValue( ECtrlBaudrate );

	if( ChoiceListCurrentItem( ECtrlAutoConnect ) == 1 )
		{
		iSettings.iAutoConnect = ETrue;
		}
	else
		{
		iSettings.iAutoConnect = EFalse;
		}

	iSettings.Store();
	return ETrue;
	}

