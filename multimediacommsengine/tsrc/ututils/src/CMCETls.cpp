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




#include "cmcetls.h"
#include "mceserial.h"
#include "mcemediamanager.h"
#include <sipmessageelements.h>
#include <sipresponseelements.h>
#include <sipstrings.h>
#include <sipstrconsts.h>


void CMCETls::OpenL ()
	{
    CMCETls* self = new (ELeave) CMCETls();
    Dll::SetTls( self );
	}

void CMCETls::Close ()
	{
	CMCETls* self = Storage();
    delete self;
    Dll::SetTls( NULL );
	}

	
CMCETls* CMCETls::Storage()
	{
	return static_cast<CMCETls*>(Dll::Tls());
	}


void CMCETls::Store( TMceIds aManagerIds )
    {
    iManagerIds = aManagerIds;
    }
    
void CMCETls::StoreL( TMceIds aIds,
                      CMceMsgBase& aContext,
                      HBufC8* aContent )

	{
	
	aIds.iMsgType = aContext.Type();
	iIds = aIds;
	delete iContextClient;
	iContextClient = NULL;
	aContext.EncodeL();
    iContextClient = aContext.EncodeBufferCloneL();
    
    delete iContentClient;
    iContentClient = aContent;
    
	}

void CMCETls::StoreL( TMceIds aIds,
                      const TDesC8& aContext )
	{
	
	iIds = aIds;
	delete iContextClient;
	iContextClient = NULL;
	iContextClient = aContext.AllocL();
    }


void CMCETls::SIPSends( RStringF aMethod, CSIPMessageElements* aMessage )
    {
    iSipSentMethod = aMethod;
    delete iSipSentMessage;
    iSipSentMessage = aMessage;
    if ( iSipSentMethod == SIPStrings::StringF( SipStrConsts::EAck ) )
        {
        iAckSent = ETrue;
        }
    }
	
void CMCETls::SIPSends( RStringF aMethod, CSIPResponseElements* aResponse )
    {
    iSipResponseToMethod = aMethod;
    delete iSipSentResponse;
    iSipSentResponse = aResponse;
    }
	
void CMCETls::MediaManagerAction( TMMActionType aType, const TAny* aObject )
    {
    iMediaManagerAction = aType;
    iMMObject = aObject;    
    }

void CMCETls::MediaManagerSdpAction( TMMActionType aType, TAny* aObject )
    {
    iMediaManagerSdpAction = aType;
    iMMObject = aObject;    
    }

	
CMCETls::CMCETls()
    : iMediaManagerAction( ENone ),
      iMediaManagerSdpAction( ENone ),
      iCompletionCode( -1 ),
      iAckSent( EFalse ),
      iIdsIndex( 0 ),
      iMediaManagerUpdateStatus( KMceReady ),
      iMediaManagerDecodeStatus( KErrNone ),
      iMediaManagerReserveStatus( KMceReady ),
      iMediaManagerNeedToNegotiate( ETrue ),
      iLeaveError( KErrNone ),
      iLeaveParam( KErrNone ),
      iRole(EMceRoleAnswerer)
    {
    //NOP
	}

void CMCETls::SetCallback( TCallBack aCallBack )
    {
    iCallBack = aCallBack;
    }

void CMCETls::ExecuteCallback()
    {
    iCallBack.CallBack();
    }



CMCETls::~CMCETls()
	{
	
	delete iContextClient;
	delete iContentClient;
	delete iContextServer;
	delete iContentServer;
	delete iSipSentMessage;
    delete iSipSentResponse;
    
	
	}

void CMCETls::SetLeave( TInt aError, TUint32 aParam )
    {
    iLeaveError = aError;
    iLeaveParam = aParam;
    }
    
TInt CMCETls::LeaveError( TUint32 aParam )
    {
    if ( aParam == iLeaveParam )
        {
        return iLeaveError;
        }
    else
        {
        return KErrNone;
        }
    }
	
//end of file