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
* Description:
*
*/

#include <e32std.h>
#include <digia/eunit/CEunitAllocTestCaseDecorator.h>
#include "nsptls.h"

// METHODS:
// ---------------------------------------------------------------------------
// TNSPStorage::TNSPStorage
// ---------------------------------------------------------------------------
// 
TNSPStorage::TNSPStorage()
	: iOOMService( CEUnitTestCaseDecorator::ActiveTestCaseDecorator(
     		KEUnitAllocTestCaseDecoratorName ) ? ETrue : EFalse ),
	  iLeaveService( KErrNone ),
	  iLastAction( TNSPStorage::ENone )
    {
    }

// ---------------------------------------------------------------------------
// TNSPStorage::TNSPStorage
// ---------------------------------------------------------------------------
// 
TNSPStorage::~TNSPStorage()
    {
    }

// ---------------------------------------------------------------------------
// NSPTls::OpenL
// ---------------------------------------------------------------------------
// 
void NSPTls::OpenL()
    {
    Dll::SetTls( new (ELeave) TNSPStorage() );
    }

// ---------------------------------------------------------------------------
// NSPTls::Close()
// ---------------------------------------------------------------------------
// 
void NSPTls::Close()
    {
    delete Storage();
    Dll::SetTls( NULL );
    }

// ---------------------------------------------------------------------------
// NSPTls::Storage
// ---------------------------------------------------------------------------
// 
TNSPStorage* NSPTls::Storage()
    {
    return ( static_cast<TNSPStorage*>( Dll::Tls() ) );
    }

// End of file
