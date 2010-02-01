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
* Description:  NAT FW SDP Provider test console test manager
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
#include "nsptestmanager.h"
#include "nsptest.h"
#include "nspreleasetest.h"

// -----------------------------------------------------------------------------
// CNSPTestManager::NewL
// -----------------------------------------------------------------------------
// 
CNSPTestManager* CNSPTestManager::NewL( CNSPPlugin& aApi, CRepository& aRep )
    {
    CNSPTestManager* self = CNSPTestManager::NewLC( aApi, aRep );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CNSPTestManager::NewLC
// -----------------------------------------------------------------------------
// 
CNSPTestManager* CNSPTestManager::NewLC( CNSPPlugin& aApi, CRepository& aRep )
    {
    CNSPTestManager* self = new ( ELeave ) CNSPTestManager( aApi, aRep );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CNSPTestManager::~CNSPTestManager
// -----------------------------------------------------------------------------
// 
CNSPTestManager::~CNSPTestManager()
    {
    iTestArray.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CNSPTestManager::CNSPTestManager
// -----------------------------------------------------------------------------
// 
CNSPTestManager::CNSPTestManager( CNSPPlugin& aApi, CRepository& aRep )
	: iApi( aApi ),
	  iRep( aRep )
	{
    }

// -----------------------------------------------------------------------------
// CNSPTestManager::ConstructL
// -----------------------------------------------------------------------------
// 
void CNSPTestManager::ConstructL()
    {
	}

// -----------------------------------------------------------------------------
// CNSPTestManager::RelTestL
// -----------------------------------------------------------------------------
// 
MNSPTest& CNSPTestManager::RelTestL()
    {
    CNSPTest* reltest = Find( ERelease );
    
    if ( NULL == reltest )
    	{
    	reltest = CNSPReleaseTest::NewLC( iApi, iRep );
        iTestArray.AppendL( reltest );
        CleanupStack::Pop( reltest );
    	}
    
    return *reltest;
    }

// -----------------------------------------------------------------------------
// CNSPTestManager::Remove
// -----------------------------------------------------------------------------
// 
TInt CNSPTestManager::Remove( MNSPTest& aTest )
    {
	const TInt index = iTestArray.Find( static_cast<CNSPTest*>(&aTest) );
	
	if ( KErrNotFound != index )
		{
		delete iTestArray[index];
		iTestArray.Remove(index);
		iTestArray.Compress();
		return KErrNone;
		}
	
	return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CNSPTestManager::StartL
// -----------------------------------------------------------------------------
// 
void CNSPTestManager::StartL( MNSPTest& aTest, CConsoleBase& aConsole )
	{
	const TInt index = iTestArray.Find( static_cast<CNSPTest*>(&aTest) );
	User::LeaveIfError( index );
	iTestArray[index]->StartL( aConsole );
	}

// -----------------------------------------------------------------------------
// CNSPTestManager::Cancel
// -----------------------------------------------------------------------------
// 
void CNSPTestManager::Cancel( MNSPTest& aTest )
	{
	const TInt index = iTestArray.Find( static_cast<CNSPTest*>(&aTest) );
	
	if ( KErrNotFound != index )
		{
		iTestArray[index]->Cancel();
		}
	}

// -----------------------------------------------------------------------------
// CNSPTestManager::StartAllL
// -----------------------------------------------------------------------------
// 
void CNSPTestManager::StartAllL( CConsoleBase& aConsole )
	{
	for ( TInt i = 0; i < iTestArray.Count(); i++ )
		{
		iTestArray[i]->StartL( aConsole );
		}
	}

// -----------------------------------------------------------------------------
// CNSPTestManager::CancelAll
// -----------------------------------------------------------------------------
// 
void CNSPTestManager::CancelAll()
	{
	for ( TInt i = 0; i < iTestArray.Count(); i++ )
		{
		iTestArray[i]->Cancel();
		}
	}

// -----------------------------------------------------------------------------
// CNSPTestManager::AcceptL
// -----------------------------------------------------------------------------
// 
void CNSPTestManager::AcceptL( MNSPTestVisitor& aVisitor )
	{
	for ( TInt i = 0; i < iTestArray.Count(); i++ )
		{
		iTestArray[i]->AcceptL( aVisitor );
		}
	}

// -----------------------------------------------------------------------------
// CNSPTestManager::Find
// -----------------------------------------------------------------------------
// 
CNSPTest* CNSPTestManager::Find( TestType aType )
	{
	TFinder finder( aType );
	
	for ( TInt i = 0; i < iTestArray.Count(); i++ )
		{
		iTestArray[i]->AcceptL( finder );
		}
	
	return finder.Ptr();
	}

// -----------------------------------------------------------------------------
// TFinder::TFinder
// -----------------------------------------------------------------------------
// 
TFinder::TFinder( CNSPTestManager::TestType aType )
	: iType( aType ),
	  iTest( NULL )
	{	
	}

// -----------------------------------------------------------------------------
// TFinder::VisitL
// -----------------------------------------------------------------------------
// 
void TFinder::VisitL( MNSPTest& /*aTest*/ )
	{
	User::Leave( KErrTotalLossOfPrecision );
	}

// -----------------------------------------------------------------------------
// TFinder::VisitL
// -----------------------------------------------------------------------------
// 
void TFinder::VisitL( CNSPTest& aTest )
	{
	iTest = ( !iTest && CNSPTestManager::ESingle == iType ? &aTest : iTest );
	}

// -----------------------------------------------------------------------------
// TFinder::VisitL
// -----------------------------------------------------------------------------
// 
void TFinder::VisitL( CNSPReleaseTest& aTest )
	{
	iTest = ( !iTest && CNSPTestManager::ERelease == iType ? &aTest : iTest );
	}

// -----------------------------------------------------------------------------
// TFinder::Ptr
// -----------------------------------------------------------------------------
// 
CNSPTest* TFinder::Ptr()
	{
	return iTest;
	}
