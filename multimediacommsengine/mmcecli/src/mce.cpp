/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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





#include "mce.h"
#include "mceserverstarter.h"
#include "mceclient.pan"
#include "mceclilogs.h"

#define RETURN_IF_ERROR(err) {TInt _err=err; if(_err!=KErrNone) {return _err;}}

// -----------------------------------------------------------------------------
// RMce::RMce
// -----------------------------------------------------------------------------
//
RMce::RMce() 
 : RSessionBase (),
   iConnected (EFalse)
	{
	}

// -----------------------------------------------------------------------------
// RMce::Version
// -----------------------------------------------------------------------------
//
TVersion RMce::Version(void) const
	{
    TVersion version (KMceServerMajorVersionNumber,
                      KMceServerMinorVersionNumber,
                      KMceServerBuildVersionNumber);
    return version;
	}

// -----------------------------------------------------------------------------
// RMce::Connect
// -----------------------------------------------------------------------------
//
TInt RMce::Connect (const TUid& aUid)
	{
    MCECLI_DEBUG("RMce::Connect, Entry");
    MCECLI_DEBUG_DVALUE("uid", aUid.iUid );

    RETURN_IF_ERROR (MceServerStarter::Start ())

    RETURN_IF_ERROR (CreateSession (KMceServerName,Version()))

	RETURN_IF_ERROR (SendClientUid(aUid))

    iConnected = ETrue;

    MCECLI_DEBUG("RMce::Connect, Exit");
    
    return KErrNone;
	}

// -----------------------------------------------------------------------------
// RMce::Connect
// -----------------------------------------------------------------------------
//
TInt RMce::Connect ()
	{
    MCECLI_DEBUG("RMce::Connect, Entry");
    RETURN_IF_ERROR (MceServerStarter::Start ())

    RETURN_IF_ERROR (CreateSession (KMceServerName,Version()))

    iConnected = ETrue;
    MCECLI_DEBUG("RMce::Connect, Exit");
    
    return KErrNone;
	}

// -----------------------------------------------------------------------------
// RMce::Close
// -----------------------------------------------------------------------------
//
void RMce::Close()
	{
    MCECLI_DEBUG("RMce::Close, Entry");
    iConnected = EFalse;
    RHandleBase::Close();
    MCECLI_DEBUG("RMce::Close, Exit");
	}

// -----------------------------------------------------------------------------
// RMce::Send
// From MMceItc
// -----------------------------------------------------------------------------
//
TInt RMce::Send (TMceItcFunctions aFunction, TIpcArgs& aArgs)
	{
	__ASSERT_ALWAYS (iConnected, Panic( KErrCouldNotConnect ));

	return SendReceive (aFunction, aArgs);
	}

// -----------------------------------------------------------------------------
// RMce::Receive
// From MMceItc
// -----------------------------------------------------------------------------
//
void RMce::Receive (TIpcArgs& aArgs, TRequestStatus& aStatus)
	{
    MCECLI_DEBUG("RMce::Receive, Entry");
	__ASSERT_ALWAYS (iConnected, Panic(KErrCouldNotConnect));
	
    MCECLI_DEBUG("ITC=EMceItcClientReadyToReceive");
	SendReceive( EMceItcClientReadyToReceive,aArgs,aStatus);

    MCECLI_DEBUG("RMce::Receive, Exit");
	
	}

// -----------------------------------------------------------------------------
// RMce::Receive
// From MMceItc
// -----------------------------------------------------------------------------
//
TInt RMce::Receive (TIpcArgs& aArgs, TPtr8 aContext, TPtr8 aContent )
	{
    MCECLI_DEBUG("RMce::Receive, Entry");
	
	__ASSERT_ALWAYS (iConnected, Panic( KErrCouldNotConnect ) );

    aArgs.Set( EMceItcArgContext, &aContext);
    aArgs.Set( EMceItcArgMessageContent, &aContent );

    MCECLI_DEBUG("ITC=EMceItcReceive");
    TInt status = SendReceive( EMceItcReceive, aArgs);
    
    MCECLI_DEBUG_DVALUE("RMce::Receive, Exit. status", status );

	return status;
	}


// -----------------------------------------------------------------------------
// RMce::CancelReceive
// From MMceItc
// -----------------------------------------------------------------------------
//
void RMce::CancelReceive ( TIpcArgs& aArgs )
	{
    MCECLI_DEBUG("RMce::CancelReceive, Entry");
    __ASSERT_ALWAYS (iConnected, Panic(KErrCouldNotConnect));

    MCECLI_DEBUG("ITC=EMceItcClientCancelReceive");
    SendReceive ( EMceItcClientCancelReceive, aArgs );
    MCECLI_DEBUG("RMce::CancelReceive, Exit");
	}

// -----------------------------------------------------------------------------
// RMce::SendAppUid
// -----------------------------------------------------------------------------
//
TInt RMce::SendClientUid ( const TUid& aUid ) const
	{
    MCECLI_DEBUG("RMce::SendClientUid, Entry");
	TMceIds ids;
	ids.iAppUID = aUid.iUid;
	
    TPckgBuf<TInt> clientUidPckg( aUid.iUid );
    TPckgBuf<TMceIds> idsPckg( ids );
    TIpcArgs itcMsgArgs;
    
    itcMsgArgs.Set(EMceItcArgIds, &idsPckg);
    itcMsgArgs.Set(EMceItcArgAppUid, &clientUidPckg);

    TInt status = SendReceive ( EMceItcSetAppUid, itcMsgArgs);

    MCECLI_DEBUG_DVALUE("RMce::SendClientUid, Exit. status", status );

	return status;
    
	
	}

// -----------------------------------------------------------------------------
// RMce::Panic
// -----------------------------------------------------------------------------
//
void RMce::Panic (TInt aReason) const
	{
    MCECLI_DEBUG("RMce::Panic, Entry");
    User::Panic( KMceClientPanic, aReason);
	}
