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




#include <badesca.h>

#include "mceservercore.h"
#include "mcecssessionimplementation.h"
#include "mcecsserver.h"
#include "mcecssession.h"
#include "mcecomaudiocodec.h"
#include "mcecomvideocodec.h"
#include "mcecssessionreceiver.h"
#include "mcecssubsessionreceiver.h"
#include "mcecssubsession.h"
#include "mcecsserveritc.h"
#include "mcesipmanager.h"
#include "mcemediamanager.h"
#include "mcesrvlogs.h"
#include "mceserial.h"
#include "mcemessagedispatcher.h"

// -----------------------------------------------------------------------------
// CMceCsSession::NewL
// -----------------------------------------------------------------------------
//
CMceCsSession* CMceCsSession::NewL( CMceServerCore& aServerCore )
	{
    CMceCsSession* self = CMceCsSession::NewLC( aServerCore );
    CleanupStack::Pop(self);
    return self;
	}

// -----------------------------------------------------------------------------
// CMceCsSession::NewLC
// -----------------------------------------------------------------------------
//
CMceCsSession* CMceCsSession::NewLC( CMceServerCore& aServerCore )
	{
    CMceCsSession* self = new (ELeave) CMceCsSession( aServerCore );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
	}


// -----------------------------------------------------------------------------
// CMceCsSession::CMceCsSession
// -----------------------------------------------------------------------------
//
CMceCsSession::CMceCsSession (CMceServerCore& aServerCore)
: iServerCore (aServerCore)
	{
    iServerCore.IncrementSessions();
	}

// -----------------------------------------------------------------------------
// CMceCsSession::ConstructL
// -----------------------------------------------------------------------------
//
void CMceCsSession::ConstructL ()
	{
    MCESRV_DEBUG("CMceCsSession::ConstructL, Entry");

	iITC = CMceCsServerITC::NewL();
    iImpl = CMceCsSessionImplementation::NewL( iServerCore.Manager(), this );
	
    MCESRV_DEBUG("CMceCsSession::ConstructL, Exit");
    
	}


// -----------------------------------------------------------------------------
// CMceCsSession::CreateSubSessionReceiverL
// -----------------------------------------------------------------------------
//
CMceCsSubSessionReceiver* CMceCsSession::CreateSubSessionReceiverL ()
	{
    return CMceCsSubSessionReceiver::NewL( ITC() );
	}



// -----------------------------------------------------------------------------
// CMceCsSession::Implementation
// -----------------------------------------------------------------------------
//
CMceCsSessionImplementation& CMceCsSession::Implementation()
    {
    return *iImpl;
    }

// -----------------------------------------------------------------------------
// CMceCsSession::~CMceCsSession
// -----------------------------------------------------------------------------
//
CMceCsSession::~CMceCsSession()
	{
    MCESRV_DEBUG("CMceCsSession::~CMceCsSession, Entry");
    
    iServerCore.DecrementSessions();
    iServerCore.Manager().UnRegisterClient( *this );
    if ( iImpl )
        {
        iImpl->RemoveAllSubSessions();
        delete iImpl;
        iImpl = NULL;
        }
    iReceivers.ResetAndDestroy();
    iReceivers.Close();
    delete iITC;
    
    MCESRV_DEBUG("MCE Server session deleted");
    
    MCESRV_DEBUG("CMceCsSession::~CMceCsSession, Exit");
	}

// -----------------------------------------------------------------------------
// CMceCsSession::ServerCore
// -----------------------------------------------------------------------------
//
CMceServerCore& CMceCsSession::ServerCore ()
    {
    return iServerCore;
    }
    
    
    
// -----------------------------------------------------------------------------
// CMceCsSession::ServiceL
// From CSession
// -----------------------------------------------------------------------------
//
void CMceCsSession::ServiceL( const RMessage2& aMessage )
	{
	TInt error = KErrNone;
	
    MCESRV_DEBUG("CMceCsSession::ServiceL, Entry");
	
#ifdef PLAT_SEC_TEST  
	iITC->Complete(aMessage, KErrNone);
	return;
#endif

	TInt function = iITC->Function( aMessage );
	TMceIds ids = iITC->ReadMCEIdsL( aMessage );
    TRAP( error, DoServiceL( function, aMessage ) );
    if ( error != KErrNone )
	    {
        MCESRV_DEBUG_DVALUE("ServiceL: Error occured in session/subsession. ERROR", error );
	    iITC->WriteL(aMessage, ids);
	    if ( function == EMceItcClientReadyToReceive ||
	         function == EMceItcClientCancelReceive )
		    {
            MCESRV_DEBUG("ServiceL: Error occured in ClientReadyToReceiveL -> Stop receive" );
		   	error = KErrCompletion;
		    }
	    }
	    
    MCESRV_DEBUG("CMceCsSession::ServiceL, Exit");

	User::LeaveIfError( error );

	
	}


// -----------------------------------------------------------------------------
// CMceCsSession::DoServiceL
// -----------------------------------------------------------------------------
//
void CMceCsSession::DoServiceL( TInt aFunction, const RMessage2& aMessage )
    {
    MCESRV_DEBUG("CMceCsSession::ServiceL, Entry");
	 
    MCESRV_DEBUG_EVENT("ITC", aFunction);

    TMceIds ids = iITC->ReadMCEIdsL( aMessage );
    
    MCESRV_DEBUG_IDS("IDS", ids );
    
    if ( ids.IsAppUIDId() )
    	{
    	 
   	 	MCESRV_DEBUG("passing to manager receiver");
   	 	switch (aFunction)
			{
        	case EMceItcSetAppUid:
        		{
            	SetClientUidL(ids, aMessage);
            	break; 
        		}
        	case EMceItcCreateSession:
        		{
            	CreateSubSessionL(ids, aMessage);
            	break; 
        		}
			case EMceItcDeleteSession:
        		{
            	DeleteSubSessionL(ids, aMessage);
            	break; 
        		}
        	case EMceItcClientCancelReceive:
        		{
            	CancelClientReceiveL( ids );
        		break;
        		}
        	case EMceItcClientReadyToReceive: // Asynchronous. Do not complete yet.
        		{
            	ClientReadyToReceiveL(ids, aMessage);
            	return; 
        		}
        	case EMceItcGetSupportedAudioCodecs:
        	case EMceItcGetSupportedVideoCodecs:
        		{
        		SupportedCodecsL(aFunction, aMessage);
        		break;
        		}
			case EMceItcDisplaysAvailable:
			case EMceItcCameraInfo:
			case EMceItcCamerasAvailable:
            case EMceItcTranscode:
            case EMceItcTranscodeWithSession:
            case EMceItcCancelTranscode:
            case EMceItcFileInfo:
			case EMceItcMaxVolume:
			case EMceItcMaxGain:
			    {
			    MediaServiceL( ids, (TMceItcFunctions)aFunction, aMessage );
			    break;
			    }
	        case EMceItcReceive:
	        	{
	            ReceiveL (ids, aMessage);
	            break;		    
	        	} 
			case EMceItcSetDefaultHeaders:
                {
                StoreDefaultHeadersL( aMessage );
                break;
                }
			case EMceItcGetDefaultHeaders:
			    {
			    DefaultHeadersL( aMessage );
			    break;
			    }
            default:
            	{
            	#ifndef ALLOW_NOT_SUPPORTED_ITC
            	User::Leave(KErrNotSupported);
            	#endif
            	}
			}
			iITC->WriteL(aMessage, ids);
	        iITC->Complete(aMessage, KErrNone);
	    }
	else
    	{
   	 	MCESRV_DEBUG("passing to session receiver");
        CallSubSessionL(ids, aFunction, aMessage);
    	}
    	
    MCESRV_DEBUG("CMceCsSession::ServiceL, Exit");
    
    }


// -----------------------------------------------------------------------------
// CMceCsSession::Uid
// -----------------------------------------------------------------------------
//
TUid CMceCsSession::Uid()
	{
	return iClientUid;
	}


// -----------------------------------------------------------------------------
// CMceCsSession::SetClientUidL
// -----------------------------------------------------------------------------
//
void CMceCsSession::SetClientUidL (TMceIds& aIds, const RMessage2& /*aMessage*/)
	{
    MCESRV_DEBUG("CMceCsSession::SetClientUidL, Entry");
	
	TInt id = (TInt) aIds.iAppUID;
	iClientUid = TUid::Uid(id);
	ServerCore().Manager().RegisterClientL( *this );
	
    MCESRV_DEBUG("CMceCsSession::SetClientUidL, Exit");
	
	}

// -----------------------------------------------------------------------------
// CMceCsSession::ClientReadyToReceiveL
// -----------------------------------------------------------------------------
//
void CMceCsSession::ClientReadyToReceiveL (TMceIds& aIds, const RMessage2& aMessage)
	{
    MCESRV_DEBUG("CMceCsSession::ClientReadyToReceiveL, Entry");

    ReceiverL( aIds )->ClientReadyToReceiveL (aMessage);

    MCESRV_DEBUG("CMceCsSession::ClientReadyToReceiveL, Exit");
    
	}
	

// -----------------------------------------------------------------------------
// CMceCsSession::CancelClientReceiveL
// -----------------------------------------------------------------------------
//
void CMceCsSession::CancelClientReceiveL ( TMceIds& aIds )
	{
    MCESRV_DEBUG("CMceCsSession::CancelClientReceiveL, Entry");

    ReceiverL( aIds )->CancelClientReceiveL();

    MCESRV_DEBUG("CMceCsSession::CancelClientReceiveL, Exit");
	}

// -----------------------------------------------------------------------------
// CMceCsSession::SupportedCodecsL
// -----------------------------------------------------------------------------
//
void CMceCsSession::SupportedCodecsL( TInt aFunction, const RMessage2& aMessage )
	{
    MCESRV_DEBUG("CMceCsSession::SupportedCodecsL, Entry");

    HBufC8* codecs = NULL;

	if ( aFunction == EMceItcGetSupportedAudioCodecs )
	    {
        codecs = iImpl->SupportedAudioCodecsL();
	    }
    else
        {
        codecs = iImpl->SupportedVideoCodecsL();
        }

    CleanupStack::PushL( codecs );
    iITC->WriteL( aMessage, *codecs, EMceItcArgContext);

    CleanupStack::PopAndDestroy( codecs );

    MCESRV_DEBUG("CMceCsSession::SupportedCodecsL, Exit");
    
	}

// -----------------------------------------------------------------------------
// CMceCsSession::MediaServiceL
// -----------------------------------------------------------------------------
//
void CMceCsSession::MediaServiceL( TMceIds& aIds, TMceItcFunctions aFunction, 
                                const RMessage2& aMessage )
    {

    MCESRV_DEBUG("CMceCsSession::MediaServiceL, Entry");

    TMceMessageDispatcher<CMceCsSessionImplementation> dispatcher( *iImpl, *iITC );
    
    dispatcher.DispatchL( aIds, aFunction, aMessage );

    MCESRV_DEBUG("CMceCsSubSession::MediaServiceL, Exit");
        
    }
    
    


// -----------------------------------------------------------------------------
// CMceCsSession::CallSubSessionL
// -----------------------------------------------------------------------------
//
void CMceCsSession::CallSubSessionL (TMceIds& aIds, TInt aFunction,
                                     const RMessage2& aMessage)
    {
    
	CMceCsSubSession* subSession = iImpl->SubSessionL( aIds.iSessionID );
    subSession->ServiceL( aIds, (TMceItcFunctions)aFunction, aMessage);
    
    }



// -----------------------------------------------------------------------------
// CMceCsSession::CreateSubSessionL
// -----------------------------------------------------------------------------
//
void CMceCsSession::CreateSubSessionL( TMceIds& aIds, const RMessage2& aMessage )
    {

    MCESRV_DEBUG("CMceCsSession::CreateSubSessionL, Entry");
        
    HBufC8* encodedMessage = NULL;
    encodedMessage = ITC().ReadLC( aMessage, EMceItcArgContext );
    
    CMceMsgTextArray* msgParams = new (ELeave) CMceMsgTextArray();
    CleanupStack::PushL( msgParams );
    
    msgParams->DecodeL( *encodedMessage );
    
    CDesC8Array* params = msgParams->iArray;
    CleanupStack::PushL( params );
    
    iImpl->CreateSubSessionL( aIds, *params );
        
    CleanupStack::PopAndDestroy( params );
    CleanupStack::PopAndDestroy( msgParams );
    CleanupStack::PopAndDestroy( encodedMessage );
        
    iITC->WriteL(aMessage, aIds);
    
    MCESRV_DEBUG("CMceCsSession::CreateSubSessionL, Exit");
    }

// -----------------------------------------------------------------------------
// CMceCsSession::DeleteSubSessionL
// -----------------------------------------------------------------------------
//
void CMceCsSession::DeleteSubSessionL( TMceIds& /*aIds*/, const RMessage2& aMessage )
    {

    MCESRV_DEBUG("CMceCsSession::DeleteSubSessionL, Entry");

    TUint32 sessionId = iITC->ReadTUint32L(aMessage, EMceItcArgId );
    iImpl->RemoveSubSessionL( sessionId );
	
    MCESRV_DEBUG("CMceCsSession::DeleteSubSessionL, Exit");
    }


// -----------------------------------------------------------------------------
// CMceCsSession::Receiver
// -----------------------------------------------------------------------------
//
CMceCsSessionReceiver* CMceCsSession::ReceiverL( TMceIds& aIds )
    {
	TMceManagerType type = aIds.iManagerType;
    CMceCsSessionReceiver* receiver = NULL;
    
    TInt index = 0;
    while( !receiver && index < iReceivers.Count() )
        {
        if ( iReceivers[ index ]->Type() == type )
            {
            receiver = iReceivers[ index ];
            }
        index++;            
        }

    if (!receiver )
        {
        receiver = CMceCsSessionReceiver::NewLC( type, *iITC );
        iReceivers.AppendL( receiver );
        CleanupStack::Pop( receiver );
        }
        
    return receiver;
    
    }

// -----------------------------------------------------------------------------
// CMceCsSession::ITC
// -----------------------------------------------------------------------------
//
CMceCsServerITC& CMceCsSession::ITC()
    {
    return *iITC;
    }

// -----------------------------------------------------------------------------
// CMceCsSession::ReceiveL
// -----------------------------------------------------------------------------
//
void CMceCsSession::ReceiveL( TMceIds& aIds, const RMessage2& aMessage )
	{
    MCESRV_DEBUG("CMceCsSession::ReceiveL, Entry");
	
    ReceiverL( aIds )->ReceiveL( aMessage );
    
    MCESRV_DEBUG("CMceCsSession::ReceiveL, Exit");
    }

// -----------------------------------------------------------------------------
// CMceCsSession::StoreDefaultHeadersL
// -----------------------------------------------------------------------------
//
void CMceCsSession::StoreDefaultHeadersL( const RMessage2& aMessage )
    {
    HBufC8* encodedMsg = ITC().ReadLC( aMessage, EMceItcArgContext );
    User::LeaveIfNull( encodedMsg );
    
    TMceComFactory factory;
    CMceMsgArray<TMceComFactory>* msgArray = 
        new (ELeave) CMceMsgArray<TMceComFactory>( factory );
    CleanupStack::PushL( msgArray );
    msgArray->DecodeL( *encodedMsg );
    
    CDesC8Array* methods = static_cast< CMceMsgTextArray* >
        ( msgArray->iArray[0] )->iArray;
    CleanupStack::PushL( methods );
    CDesC8Array* headers = static_cast< CMceMsgTextArray* >
        ( msgArray->iArray[1] )->iArray;
    CleanupStack::PushL( headers );
    
    iImpl->StoreDefaultHeadersL( *methods, headers );
    
    CleanupStack::Pop( headers );
    CleanupStack::PopAndDestroy( methods );
    CleanupStack::PopAndDestroy( msgArray );
    CleanupStack::PopAndDestroy( encodedMsg );
    }

// -----------------------------------------------------------------------------
// CMceCsSession::DefaultHeadersL
// -----------------------------------------------------------------------------
//
void CMceCsSession::DefaultHeadersL( const RMessage2& aMessage )		
    {
    MCESRV_DEBUG("CMceCsSession::DefaultHeadersL, Entry");
    
    HBufC8* method = ITC().ReadLC( aMessage, EMceItcArgContext );
    
    HBufC8* headers = iImpl->DefaultHeadersL( *method );
    CleanupStack::PopAndDestroy( method );
    CleanupStack::PushL( headers );

    iITC->WriteL( aMessage, *headers, EMceItcArgContext);

    CleanupStack::PopAndDestroy( headers );

    MCESRV_DEBUG("CMceCsSession::DefaultHeadersL, Exit");
    }
    
//  End of File
