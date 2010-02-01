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
* Description:  NAT FW SDP Provider test console tests result collector
*
*/

#include <badesca.h>
#include <e32std.h>
#include <e32cons.h>
#include <s32mem.h>
#include "nspresultcollector.h"

const TUint KDefaultGranularity = 6;
// -----------------------------------------------------------------------------
// CResultCollector::NewL
// -----------------------------------------------------------------------------
// 
CResultCollector* CResultCollector::NewL( CConsoleBase& aConsole )
    {
    CResultCollector* self = CResultCollector::NewLC( aConsole );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CResultCollector::NewLC
// -----------------------------------------------------------------------------
// 
CResultCollector* CResultCollector::NewLC( CConsoleBase& aConsole )
    {
    CResultCollector* self = new ( ELeave ) CResultCollector( aConsole );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CResultCollector::CResultCollector
// -----------------------------------------------------------------------------
// 
CResultCollector::CResultCollector( CConsoleBase& aConsole )
	: iConsole( aConsole )
	{
    }

// -----------------------------------------------------------------------------
// CResultCollector::ConstructL
// -----------------------------------------------------------------------------
// 
void CResultCollector::ConstructL()
    {
	}

// -----------------------------------------------------------------------------
// CResultCollector::~CResultCollector
// -----------------------------------------------------------------------------
// 
CResultCollector::~CResultCollector()
    {
    }

// -----------------------------------------------------------------------------
// CResultCollector::VisitL
// -----------------------------------------------------------------------------
// 
void CResultCollector::VisitL( MNSPTest& /*aTest*/ )
	{
	User::Leave( KErrTotalLossOfPrecision );
	}

// -----------------------------------------------------------------------------
// CResultCollector::VisitL
// -----------------------------------------------------------------------------
// 
void CResultCollector::VisitL( CNSPTest& aTest )
	{
	CBufFlat* buffer = CBufFlat::NewL( 300 );
	CleanupStack::PushL( buffer );
	RBufWriteStream stream;
	stream.Open( *buffer );
	CleanupClosePushL( stream );
	aTest.ExternalizeL( stream );
	
	TPckgBuf<TResult> pkgIn;
	pkgIn.Copy( buffer->Ptr(0) );
	PrintResult( pkgIn() );
	
	CleanupStack::PopAndDestroy();
	CleanupStack::PopAndDestroy( buffer );
	}

// -----------------------------------------------------------------------------
// CResultCollector::VisitL
// -----------------------------------------------------------------------------
// 
void CResultCollector::VisitL( CNSPReleaseTest& /*aTest*/ )
	{
	
	}

// -----------------------------------------------------------------------------
// CResultCollector::PrintResult
// -----------------------------------------------------------------------------
// 
void CResultCollector::PrintResult( TResult& aResult )
	{
	iConsole.Write( aResult.iDescription );
	
	switch ( aResult.iState )
		{
		case TResult::ECreated:
			{
			iConsole.Printf( _L( " Created\n") );
			break;
			}
		
		case TResult::ERunning:
			{
			iConsole.Printf( _L( " Running\n") );
			break;
			}
		
		case TResult::EPass:
			{
			iConsole.Printf( _L( " PASS\n") );
			break;
			}
		
		case TResult::EFail:
			{
			iConsole.Printf( _L( " FAIL\n") );
			break;
			}
		
		default:
			{
			iConsole.Printf( _L( " %d\n" ), aResult.iState ) ;
			}
		}
	}
