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

#include "CTcCTRLCodec.h"
#include "TTcIdentifier.h"
#include "TTcCommandBase.h"

#include "TesterConstants.h"
#include "TcLog.h"

EXPORT_C CTcContextBase::~CTcContextBase()
	{
	}

EXPORT_C CTcContextBase::CTcContextBase( CTcCTRLCodec& aCodec )
	: iCodec( aCodec )
	{
	}

// -- MTcTestContext functions ------------------------------------------------

EXPORT_C void CTcContextBase::ExecuteL()
	{
	TcLog::Write( _L8("CTcContextBase::ExecuteL() start\n") );
	// Get Parameter list from request
	iCodec.GetParametersL( iList );

	// Reset response parameter list
	iReturnList.Reset();

	// Get function identifier from request
	TTcIdentifier id;
	iCodec.GetIdentifierL( id );

	// Map identifier to a command and create it using derived class
	TTcCommandBase* cmd = CreateCommandL( id, *this );

	// execute command, catch errors
	TInt executionStatus( KErrNone );
	TcLog::Write( _L8("CTcContextBase::ExecuteL() executing command\n") );
	TRAP( executionStatus, cmd->ExecuteL() );
	delete cmd;
	TcLog::WriteFormat( _L8("CTcContextBase::ExecuteL() - finished with status %d\n"), executionStatus );

	// Propagate error if it's not related to the tester implementation.
	if( ( executionStatus >= KTcErrMax ) && ( executionStatus <= KTcErrBase ) )
		{
		User::Leave( executionStatus );
		}
	else if( executionStatus != KErrNone )
		{
		// Read the current execution log if it was generated
		HBufC8* logText = TcLog::ReadLogLC();

		// Send the log back to TTCN system
		CTcNameValue* logItem = CTcNameValue::NewLC();
		logItem->SetNameL( KTcParamLog );
		if( logText )
			{
			logItem->SetValueL( logText->Des() );
			}
		else
			{
			logItem->SetValueL( KTcErrUnableToRetrieveLog );
			}
		iReturnList.AddParameterL( logItem );
		CleanupStack::Pop( logItem );
		if( logText )
			{
			CleanupStack::PopAndDestroy( logText );
			}
		}

	// Construct return value
	CTcNameValue* retVal = CTcNameValue::NewLC();
	retVal->SetL( KTcActionReturnCode, executionStatus );

	// Construct response. iReturnList was filled in cmd->ExecuteL()
	iCodec.ConstructResponse( id, *retVal, iReturnList );

	// Destroy the return value object
	CleanupStack::PopAndDestroy( retVal );
	TcLog::Write( _L8("CTcContextBase::ExecuteL() end\n") );
	}

EXPORT_C void CTcContextBase::ResetL( const TTcIdentifier& aId )
	{
	TcLog::Write( _L8("CTcContextBase::ResetL() start") );
	if( aId.iContainer == KNullDesC8 )
		{
		iRegistry.DestroyAll();
		}
	else
		{
		iRegistry.DestroyObject( aId.iContainer );
		}
	TcLog::Write( _L8("CTcContextBase::ResetL() end") );
	}

EXPORT_C CTcCTRLCodec& CTcContextBase::Codec()
	{
	return iCodec;
	}

EXPORT_C CTcRegistry& CTcContextBase::Registry()
	{
	return iRegistry;
	}

EXPORT_C CTcParameterList& CTcContextBase::List()
	{
	return iList;
	}

EXPORT_C CTcParameterList& CTcContextBase::ReturnList()
	{
	return iReturnList;
	}

EXPORT_C TTcCommandBase* CTcContextBase::CreateCommandL(
											const TcCommand* aCommandArray,
											TInt aSizeOfArray,
											TTcIdentifier& aId,
											MTcTestContext& aContext )
	{
	TBuf8< 64 > tmp( aId.iFunction );
	TcLog::WriteFormat( _L8("CTcContextBase::CreateCommandL( \"%s\" )\n"), tmp.PtrZ() );

	TInt cmdCount = aSizeOfArray / sizeof( TcCommand );
	for( TInt i = 0; i < cmdCount; i++ )
		{
		if( aCommandArray[ i ].iMatchFunc( aId ) )
			{
			return aCommandArray[ i ].iNewFunc( aContext );
			}
		}
	User::Leave( KErrNotSupported );
	return NULL;	// keep compiler happy
	}
