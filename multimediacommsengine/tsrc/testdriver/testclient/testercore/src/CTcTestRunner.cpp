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

#include <f32file.h>
#include <ecom/ecom.h>

#include "CTcBearer.h"
#include "CTcTestRunner.h"
#include "CTcContextFactory.h"
#include "MTcTestObserver.h"
#include "TTcIdentifier.h"
#include "TcLog.h"

static void CleanupResetAndDestroy( TAny* aPtr )
    {
    RImplInfoPtrArray* arr = reinterpret_cast< RImplInfoPtrArray* >( aPtr );
    arr->ResetAndDestroy();
    }

EXPORT_C CTcTestRunner* CTcTestRunner::NewL( CTcBearer& aBearer,
											 MTcTestObserver& aObserver,
											 TInt aIAPId )
	{
	CTcTestRunner* self = new( ELeave ) CTcTestRunner( aBearer,
													   aObserver,
													   aIAPId );

	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );

	return self;
	}

EXPORT_C CTcTestRunner::~CTcTestRunner()
	{
	// Stop testing, delete context etc.
	Stop();

	// Delete factories
	iContextFactories.ResetAndDestroy();

	// Close all plugin handles
	TInt count = iPlugins.Count();
	for( TInt i = 0; i < count; i++ )
		{
		iPlugins[ i ].Close();
		}
		
	REComSession::FinalClose();
	}

CTcTestRunner::CTcTestRunner( CTcBearer& aBearer,
							  MTcTestObserver& aObserver,
							  TInt aIAPId )
	: iCodec( iRequest, iResponse ), iBearer( aBearer ),
	  iFreeRequest( NULL, 0 ), iPlugins( KTcGranularity ),
	  iContextFactories( KTcGranularity ), iObserver( aObserver ),
	  iIAPId( aIAPId )
	{
	iBearer.SetObserver( this );
	}

void CTcTestRunner::ConstructL()
	{
	LoadPluginsL();
	}

void CTcTestRunner::BearerCompletion( MTcBearerObserver::TOperation aOp,
									  TInt aStatus )
	{
	// If we get an error from the bearer, "recommend" disconnect to the AppUi :-)
	// (usually the only error is KErrDisconnected)
	if( aStatus != KErrNone )
		{
		iObserver.NotifyDisconnect();
		return;
		}

	switch( aOp )
		{
		case MTcBearerObserver::ESend:
			{
			// Clear buffers
			iResponse.Zero();
			iRequest.Zero();
			// Start receiving data again
			Start();
			break;
			}
		case MTcBearerObserver::EReceive:
			{
			// Update request data length (add new data).
			iRequest.SetLength( iRequest.Length() + iFreeRequest.Length() );
			// Do we have a complete request in iRequest?
			if( !iCodec.IsComplete() )
				{
				// No, read more
				Start();
				}
			else
				{
				// Yes, try to execute the request
				TRAPD( err, ExecuteL() );
				// Report any errors
				if( err )
					{
					SendErrorResponse( err );
					}
				iCurrentRequest.Zero();
				iObserver.NotifyStatusChange();
				}
			break;
			}
		default:
			{
			// does not happen, ignored
			break;
			}
		}
	}

EXPORT_C void CTcTestRunner::Start()
	{
	// Is there still room in the receive buffer?
	if( iRequest.Length() < iRequest.MaxLength() )
		{
		// Construct a pointer descriptor for the free data area of iRequest
		// This is done to accomplish "appending" socket read.
		TInt dataLen( iRequest.Length() );
		iFreeRequest.Set( const_cast< TUint8* >( iRequest.Ptr() ) + dataLen, 0,
						  iRequest.MaxLength() - dataLen );
		iBearer.ReceiveOneOrMore( iFreeRequest );
		}
	else
		{
		SendErrorResponse( KTcErrReceiveOverflow );
		}
	}

EXPORT_C void CTcTestRunner::Stop()
	{
	delete iContext;
	iContext = NULL;

	iCurrentTestCase.Zero();
	iCurrentRequest.Zero();
	iObserver.NotifyStatusChange();
	}

EXPORT_C const TDesC8& CTcTestRunner::CurrentTestCaseName() const
	{
	return iCurrentTestCase;
	}

EXPORT_C const TDesC8& CTcTestRunner::CurrentRequestName() const
	{
	return iCurrentRequest;
	}

void CTcTestRunner::ExecuteL()
	{
	TcLog::ClearLog();
	TcLog::Write( _L8("\nCTcTestRunner::ExecuteL()") );

	CTcCTRLCodec::TType type = iCodec.RequestType();
	TTcIdentifier id;
	iCodec.GetIdentifierL( id );

	switch( type )
		{
		case CTcCTRLCodec::ERequest:
			{
			TcLog::Write( _L8(" ERequest\n") );
			if( !iContext )
				{
				User::Leave( KTcErrNoActiveTestCase );
				}
			// Store function name and notify observer about that
			iCurrentRequest = id.iFunction.Left( iCurrentRequest.MaxLength() );
			iObserver.NotifyStatusChange();
			// Start executing the function
			iContext->ExecuteL();
			// Send a response (constructed inside ExecuteL())
			iBearer.Send( iResponse );
			break;
			}
		case CTcCTRLCodec::EResetRequest:
			{
			TcLog::Write( _L8(" EResetRequest\n") );
			if( !iContext )
				{
				User::Leave( KTcErrNoActiveTestCase );
				}
			iContext->ResetL( id );
			SendOkResponse();
			break;
			}
		case CTcCTRLCodec::EStartRequest:
			{
			TcLog::Write( _L8(" EStartRequest\n") );

			// If there is an active test case, deactivate it
			// This is wiser than reporting an error - the previous test case
			// might have failed unexpectedly and the TTCN tester did not clean up properly
			if( iContext )
				{
				delete iContext;
				iContext = NULL;
				iCurrentTestCase = KNullDesC8;
				}

			// Create and activate a new context
			CreateContextL( id );

			// Update test case Sip stack and request names
			iCurrentTestCase = id.iContainer.Left( iCurrentTestCase.MaxLength() );
			iCurrentRequest.Zero();
			SendOkResponse();
			break;
			}
		case CTcCTRLCodec::EEndRequest:
			{
			TcLog::Write( _L8(" EEndRequest\n") );
			// Also, it does not really hurt if we receive an END request
			// while having no active test case. Just happily respond with OK :-)
			delete iContext;
			iContext = NULL;
			iCurrentTestCase = KNullDesC8;
 			SendOkResponse();
			break;
			}
		default:	// non-recognized request type
			{
			TcLog::Write( _L8(" Unknown request type\n") );
			User::Leave( KTcErrRequestCorrupt );
			break;
			}
		}

	TcLog::Write( _L8("CTcTestRunner::ExecuteL() end\n\n") );
	}

void CTcTestRunner::CreateContextL( const TTcIdentifier& aId )
	{
	TBuf8< 64 > tmp( aId.iFunction );
	TcLog::WriteFormat( _L8("CTcTestRunner::CreateContextL( %s ) start\n"), tmp.PtrZ() );
	
	// Try to find a context factory which would create a test context for us
	TInt status( KTcErrUnknownContext );
	TInt count( iContextFactories.Count() );
	for( TInt i = 0; i < count; i++ )
		{
		// CreateLC returns NULL if it didn't support the context type/name
		// specified in the test case name (in aId)
		MTcTestContext* context =
			iContextFactories[ i ]->CreateLC( aId.iContainer, 
                                              iCodec, 
                                              iIAPId );

		// Assign the context for use and stop trying if we got a new context
		if( context )
			{
			iContext = context;
			CleanupStack::Pop();
			status = KErrNone;
			break;
			}
		}

	User::LeaveIfError( status );
	
	TcLog::Write( _L8("CTcTestRunner::CreateContextL() end\n") );
	}

void CTcTestRunner::SendOkResponse()
	{
	iCodec.ConstructOkResponse();
	iBearer.Send( iResponse );
	}

void CTcTestRunner::SendErrorResponse( TInt aError )
	{
	TBuf8< KTcMaxErrorMsg > msg;
	switch( aError )
		{
		case KTcErrUnknownContext:
			{
			msg = KTcErrUnknownContextMsg;
			break;
			}
		case KTcErrNoActiveTestCase:
			{
			msg = KTcErrNoActiveTestCaseMsg;
			break;
			}
		case KTcErrReceiveOverflow:
			{
			msg = KTcErrReceiveOverflowMsg;
			break;
			}
		case KErrCorrupt:
		case KTcErrRequestCorrupt:
			{
			msg = KTcErrRequestCorruptMsg;
			break;
			}
		case KTcErrMandatoryIdNotFound:
			{
			msg = KTcErrMandatoryIdNotFoundMsg;
			break;
			}
		case KTcErrMandatoryParameterNotFound:
			{
			msg = KTcErrMandatoryParameterNotFoundMsg;
			break;
			}
		case KTcErrMandatoryHeaderNotFound:
			{
			msg = KTcErrMandatoryHeaderNotFoundMsg;
			break;
			}
		case KTcErrMandatoryStructureItemNotFound:
			{
			msg = KTcErrMandatoryStructureItemNotFoundMsg;
			break;
			}
		case KTcErrHeaderNotAllowed:
			{
			msg = KTcErrHeaderNotAllowedMsg;
			break;
			}
		case KTcErrObjectNotFound:
			{
			msg = KTcErrObjectNotFoundMsg;
			break;
			}
		case KErrNoMemory:
			{
			msg = KTcErrNoMemoryMsg;
			break;
			}
		default:
			{
			msg.Format( KTcErrTestClientInternalMsg, aError );
			break;
			}
		}

	// Read the current execution log if it was generated
	TInt err( KErrNone );
	TRAP( err,
		{
		HBufC8* logText = TcLog::ReadLogLC();
		if( logText )
			{
			logText = logText->ReAllocL( logText->Length() + msg.Length() + 16 );
			CleanupStack::Pop();	// old logText
			TPtr8 logTextPtr( logText->Des() );
			logTextPtr.Insert( 0, _L8(" ** LOG ** \n\n") );
			logTextPtr.Insert( 0, msg );
			iCodec.ConstructErrorResponse( logTextPtr );
			delete logText;
			}
		else
			{
			err = KErrNotFound;
			}
		} )

	if( err )
		{
		iCodec.ConstructErrorResponse( msg );
		}

	iBearer.Send( iResponse );
	}

void CTcTestRunner::LoadPluginsL()
	{
	// List implementations in ECom registry for interface
	RImplInfoPtrArray infoArray;
	REComSession::ListImplementationsL( KTesterInterfaceUid, infoArray );
	CleanupStack::PushL( TCleanupItem( CleanupResetAndDestroy, &infoArray ) );
	
	for( TInt i = 0; i < infoArray.Count(); i++)
	    {
	    CImplementationInformation& info = *( infoArray[ i ] );
	    
        TEComResolverParams resolverParams;
	    resolverParams.SetDataType( info.DataType() );
	    	    
	    CTcContextFactory* factory = 
	        reinterpret_cast< CTcContextFactory* >( 
	            REComSession::CreateImplementationL(
	                KTesterInterfaceUid, 
                    _FOFF( CTcContextFactory, iInstanceKey ),
                    NULL,
                    resolverParams ) );
                    
        CleanupDeletePushL( factory );
		iContextFactories.AppendL( factory );
		CleanupStack::Pop();	// factory           
	    }
	    
	CleanupStack::PopAndDestroy( 1 ); // infoArray	
	}
