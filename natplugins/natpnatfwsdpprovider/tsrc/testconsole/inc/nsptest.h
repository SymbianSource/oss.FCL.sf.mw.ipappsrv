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
* Description:  NAT FW SDP Provider test interface & implementations
*
*/

#ifndef NSPTEST_H
#define NSPTEST_H

#include <e32std.h>
#include <e32cons.h>
#include <s32strm.h>
#include "nsptestconsolemacros.h"

class CConsoleBase;
class MNSPTest;
class CNSPTest;
class CNSPReleaseTest;

/**
 * General visitor interface.
 */
class MNSPTestVisitor
	{
public: // Methods
	
	virtual void VisitL( MNSPTest& aTest ) = 0;
	
	virtual void VisitL( CNSPTest& aTest ) = 0;
	
	virtual void VisitL( CNSPReleaseTest& aTest ) = 0;
	
	};


/**
 * Abstract interface for tests.
 */
class MNSPTest
	{
public: // Methods
	
	/**
	 * Start test
	 */
	virtual void StartL( CConsoleBase& aConsole ) = 0;
	
	/**
	 * Cancel test
	 */
	virtual void Cancel() = 0;
	
	/**
	 * Visitor's accept method
	 */
	virtual void AcceptL( MNSPTestVisitor& aVisitor ) = 0;
	
	};


/**
 * Test 'state' implementation.
 */
class CNSPTest : public CBase, public MNSPTest
	{
public: // Methods

	inline virtual ~CNSPTest()
		{
		}
	
	
	
	inline void StartL( CConsoleBase& aConsole )
		{
		iResult.iState = TResult::ERunning; // Can be re-run, prev result overwritten.
		TRAPD( err, SetupL() );
		START( err, aConsole, iResult.iDescription );
		
		if ( KErrNone == err )
			{
			TRAP( err, ExecuteL( aConsole ) );
			Teardown();
			}
		
		iResult.iState = ( TResult::ERunning != iResult.iState && KErrNone == err ?
								iResult.iState : ( KErrNone == err ?
									TResult::EPass : ( KAssertFailure == err ?
										TResult::EFail : (TResult::TState) err ) ) );
		END( aConsole, iResult.iDescription, iResult.iState );
		User::LeaveIfError( KErrCancel == err ? KErrCancel : KErrNone );
		}
	
	virtual void SetupL() = 0;
	
	virtual void Teardown() = 0;
	
	inline virtual void ExecuteL( CConsoleBase& /*aConsole*/ )
		{
		User::Leave( KErrTotalLossOfPrecision );
		}
	
	inline virtual void AcceptL( MNSPTestVisitor& aVisitor )
		{
		aVisitor.VisitL( *this );
		}
	
	inline void ExternalizeL( RWriteStream& aOutStream )
		{
		aOutStream.WriteL( TPckgC<TResult>( iResult ) );
		}

public: // Methods

	inline TResult::TState State() const
		{
		return iResult.iState;
		}

protected: // Data

	TResult iResult;
	
	};

#endif // NSPTEST_H
