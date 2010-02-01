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
* Description:    Iap
*
*/




#include "iap.h"
#include <commdb.h>

// ---------------------------------------------------------------------------
// TIap::TIap
// ---------------------------------------------------------------------------
//  
TIap::TIap( TUint32 aIapId, const TDesC& aIapName ) :
    iIapId( aIapId ), iIapName( aIapName )
    {
    }

// ---------------------------------------------------------------------------
// TIap::IapId
// ---------------------------------------------------------------------------
//  
TUint32 TIap::IapId()
    {
    return iIapId;
    }

// ---------------------------------------------------------------------------
// TIap::IapName
// ---------------------------------------------------------------------------
//  
const TDesC& TIap::IapName()
    {
    return iIapName;
    }

// ---------------------------------------------------------------------------
// TIap::ResolveIapL
// ---------------------------------------------------------------------------
//  
TInt TIap::ResolveIapL( RArray<TIap>& aIapArray )
    {
    RDebug::Print( _L("TEST PRINT: CTestAppConsole::ResolveIapL -start\n") );
    
    TBuf<40> name;
    TUint32 iapid;

    CCommsDatabase* db = CCommsDatabase::NewL( EDatabaseTypeIAP );
    CleanupStack::PushL( db );
    CCommsDbTableView* view = db->OpenTableLC( TPtrC( IAP ) );

    TInt res = view->GotoFirstRecord();
    
    if ( res != KErrNone )
        {
        CleanupStack::PopAndDestroy( 2 ); // db, view
        RDebug::Print( _L("TEST PRINT: CTestAppConsole::ResolveIapL -end err: %d\n"), res );
        return res;
        }
    while ( res == KErrNone )  
        {
        view->ReadTextL( TPtrC( COMMDB_NAME ), name );
        view->ReadUintL( TPtrC( COMMDB_ID ), iapid );
        aIapArray.AppendL( TIap( iapid, name ) );
        res = view->GotoNextRecord();
        }
    CleanupStack::PopAndDestroy( 2 ); // db, view

    RDebug::Print( _L("TEST PRINT: CTestAppConsole::ResolveIapL -end" ) );
    return KErrNone;
    }
