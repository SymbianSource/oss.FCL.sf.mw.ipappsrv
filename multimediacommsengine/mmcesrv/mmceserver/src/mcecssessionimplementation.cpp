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
#include <pathinfo.h>

#include "mcecssessionimplementation.h"
#include "mcecssession.h"
#include "mcecssessionreceiver.h"
#include "mcecomaudiocodec.h"
#include "mcecomvideocodec.h"
#include "mcecssubsession.h"
#include "mcesipmanager.h"
#include "mcemediamanager.h"
#include "mcesipdefaults.h"
#include "mcesrvlogs.h"
#include "mceserial.h"
#include "mcecommediasink.h"
#include "mcefilesource.h"
#include "mcecomcamerasource.h"


 
#define MCE_WRITE( aType, aData )\
    aType writeBuf( aData );\
    aInfo.Copy( writeBuf );
    
    
#define MCE_READ( aCastType, aType, aData )\
    aCastType readBuf;\
    readBuf.Copy( aInfo );\
    aType aData = readBuf();


_LIT(KMceApplPrivatePath, "%3Sprivate\\%x\\");
_LIT(KMceRootPath, "%3S");
 
// -----------------------------------------------------------------------------
// CMceCsSessionImplementation::NewL
// -----------------------------------------------------------------------------
//
CMceCsSessionImplementation* CMceCsSessionImplementation::NewL(
                                                CMceSipManager& aManager,
                                                CMceCsSession* aITCProvider )
	{
    CMceCsSessionImplementation* self = 
       CMceCsSessionImplementation::NewLC( aManager, aITCProvider );
    CleanupStack::Pop(self);
    return self;
	}

// -----------------------------------------------------------------------------
// CMceCsSessionImplementation::NewLC
// -----------------------------------------------------------------------------
//
CMceCsSessionImplementation* CMceCsSessionImplementation::NewLC(
                                               CMceSipManager& aManager,
                                               CMceCsSession* aITCProvider )
	{
    CMceCsSessionImplementation* self = 
            new (ELeave) CMceCsSessionImplementation( aManager, *aITCProvider );
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
	}

// -----------------------------------------------------------------------------
// CMceCsSessionImplementation::CMceCsSessionImplementation
// -----------------------------------------------------------------------------
//
CMceCsSessionImplementation::CMceCsSessionImplementation(
                                                CMceSipManager& aManager,
                                                CMceCsSession& aITCProvider )
: iManager( aManager ),
  iITCProvider( &aITCProvider )
	{
	}

// -----------------------------------------------------------------------------
// CMceCsSessionImplementation::ConstructL
// -----------------------------------------------------------------------------
//
void CMceCsSessionImplementation::ConstructL ()
	{
    MCESRV_DEBUG("CMceCsSessionImplementation::ConstructL, Entry");
    iDefaults = CMceSipDefaults::NewL();
    MCESRV_DEBUG("CMceCsSessionImplementation::ConstructL, Exit");
	}

// -----------------------------------------------------------------------------
// CMceCsSessionImplementation::~CMceCsSessionImplementation
// -----------------------------------------------------------------------------
//
CMceCsSessionImplementation::~CMceCsSessionImplementation()
	{
	delete iDefaults;
    iSubSessions.ResetAndDestroy();
    iSubSessions.Close();
	}

    
// -----------------------------------------------------------------------------
// CMceCsSessionImplementation::ServerCore
// -----------------------------------------------------------------------------
//
RPointerArray<CMceCsSubSession>& CMceCsSessionImplementation::SubSessions()
    {
    return iSubSessions;
    }


// -----------------------------------------------------------------------------
// CMceCsSessionImplementation::SupportedAudioCodecsL
// -----------------------------------------------------------------------------
//
HBufC8* CMceCsSessionImplementation::SupportedAudioCodecsL()
	{
	
    MCESRV_DEBUG("CMceCsSessionImplementation::SupportedAudioCodecsL, Entry");
    CDesC8ArrayFlat* codecs = NULL;
    HBufC8* codecsAsString = NULL;
    
	RPointerArray<CMceComAudioCodec> supportedCodecs;
	CleanupStack::PushL( TCleanupItem( ArrayCleanup, &supportedCodecs ) );
		
	iManager.MediaManager().GetCapabilitiesL( supportedCodecs );

    codecs = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( codecs );
    
	for ( int i=0;i<supportedCodecs.Count();i++ )
		{
		CMceComAudioCodec* codec = supportedCodecs[i];
		
		CMceMsgObject<CMceComAudioCodec>* codecMsg = 
                    new (ELeave) CMceMsgObject<CMceComAudioCodec>
                    ( *codec, EMceItcMsgTypeCodec );
		CleanupStack::PushL( codecMsg );

		codecMsg->EncodeL();
		codecs->AppendL( codecMsg->EncodeBuffer().Ptr(0) );
		CleanupStack::PopAndDestroy( codecMsg );
		}
    CMceMsgTextArray* msgCodecs = new (ELeave) CMceMsgTextArray( *codecs );
    CleanupStack::PushL( msgCodecs );
    msgCodecs->EncodeL();
    
    codecsAsString = msgCodecs->EncodeBufferCloneL();
    
    CleanupStack::PopAndDestroy( msgCodecs );
    CleanupStack::PopAndDestroy( codecs );
	CleanupStack::PopAndDestroy( &supportedCodecs ); //TCleanupItem
    	
    MCESRV_DEBUG("CMceCsSessionImplementation::SupportedAudioCodecsL, Exit");
	
	return codecsAsString;
        	
	}

// -----------------------------------------------------------------------------
// CMceCsSessionImplementation::SupportedVideoCodecsL
// -----------------------------------------------------------------------------
//
HBufC8* CMceCsSessionImplementation::SupportedVideoCodecsL()
	{
	
    MCESRV_DEBUG("CMceCsSessionImplementation::SupportedVideoCodecsL, Entry");
    CDesC8ArrayFlat* codecs = NULL;
    HBufC8* codecsAsString = NULL;
    
	RPointerArray<CMceComVideoCodec> supportedCodecs;
	CleanupStack::PushL( TCleanupItem( ArrayCleanup, &supportedCodecs ) );
		
	iManager.MediaManager().GetCapabilitiesL( supportedCodecs );

    codecs = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( codecs );
    
	for ( int i=0;i<supportedCodecs.Count();i++ )
		{
		CMceComVideoCodec* codec = supportedCodecs[i];
		
		CMceMsgObject<CMceComVideoCodec>* codecMsg = 
                    new (ELeave) CMceMsgObject<CMceComVideoCodec>
                    ( *codec, EMceItcMsgTypeCodec );
		CleanupStack::PushL( codecMsg );

		codecMsg->EncodeL();
		codecs->AppendL( codecMsg->EncodeBuffer().Ptr(0) );
		CleanupStack::PopAndDestroy( codecMsg );
		}
    CMceMsgTextArray* msgCodecs = new (ELeave) CMceMsgTextArray( *codecs );
    CleanupStack::PushL( msgCodecs );
    msgCodecs->EncodeL();
    
    codecsAsString = msgCodecs->EncodeBufferCloneL();
    
    CleanupStack::PopAndDestroy( msgCodecs );
    CleanupStack::PopAndDestroy( codecs );
	CleanupStack::PopAndDestroy( &supportedCodecs ); //TCleanupItem
    	
    MCESRV_DEBUG("CMceCsSessionImplementation::SupportedVideoCodecsL, Exit");
	
	return codecsAsString;
        	
	}



// -----------------------------------------------------------------------------
// CMceCsSession::DoServiceL
// -----------------------------------------------------------------------------
//
void CMceCsSessionImplementation::DoServiceL( TMceIds& aIds, TInt aFunction, TPtr8& aInfo )
    {

    MCESRV_DEBUG("CMceCsSessionImplementation::DoServiceL( read ), Entry");

    switch( aFunction )
        {
        case EMceItcCameraInfo:
            {
    		MCESRV_DEBUG("getting camera info");
            MCE_READ( TMceItcArgTCameraInfo, TMceCameraInfo, cameraInfo );
            iManager.MediaManager().GetCameraInfoL( cameraInfo.iIndex,
                                                    cameraInfo.iInfo );
            MCE_WRITE( TMceItcArgTCameraInfo, cameraInfo ); 
            break;
            }
        case EMceItcCamerasAvailable:
            {
            TInt camerasAvailable = KErrNotFound;
            iManager.MediaManager().GetCamerasAvailableL( camerasAvailable );
    		MCESRV_DEBUG_DVALUE("getting available cameras. #", camerasAvailable );
            MCE_WRITE( TMceItcArgTInt, camerasAvailable ); 
            break;
            }
        case EMceItcDisplaysAvailable:
            {
            TInt displaysAvailable = KErrNotFound;
            iManager.MediaManager().GetDisplaysAvailableL( displaysAvailable );
    		MCESRV_DEBUG_DVALUE("getting displays available. value", displaysAvailable );
            MCE_WRITE( TMceItcArgTInt, displaysAvailable ); 
            break;
            }
        case EMceItcFileInfo:
            {
    		MCESRV_DEBUG("getting file info");
            TMceItcArgTFileInfo readBuf;
            readBuf.Copy( aInfo );
            TMceFileInfo& fileInfo = readBuf();
            GetFileInfoL( aIds, fileInfo );
            MCE_WRITE( TMceItcArgTFileInfo, fileInfo ); 
            break;
            }
        case EMceItcMaxVolume:
            {
            TInt maxVolume = KErrNotFound;
            iManager.MediaManager().GetMaxVolumeL( maxVolume );
    		MCESRV_DEBUG_DVALUE("getting max volume. value", maxVolume );
            MCE_WRITE( TMceItcArgTInt, maxVolume );  
            break;
            }
        case EMceItcMaxGain:
            {
            TInt maxGain = KErrNotFound;
            iManager.MediaManager().GetMaxGainL( maxGain );
    		MCESRV_DEBUG_DVALUE("getting max volume. value", maxGain );
            MCE_WRITE( TMceItcArgTInt, maxGain );  
            break;
            }
        default:
            {
        	#ifndef ALLOW_NOT_SUPPORTED_ITC
        	User::Leave(KErrNotSupported);
        	#endif
            }
        }
    
    MCESRV_DEBUG("CMceCsSessionImplementation::DoServiceL( read ), Exit");
    }
    
// -----------------------------------------------------------------------------
// CMceCsSessionImplementation::DoServiceL
// -----------------------------------------------------------------------------
//
HBufC8* CMceCsSessionImplementation::DoServiceL( TMceIds& /*aIds*/, TInt /*aFunction*/ )
    {
    MCESRV_DEBUG("CMceCsSessionImplementation::DoServiceL(no data), Entry");
    HBufC8* returnMessage = NULL;
    #ifndef ALLOW_NOT_SUPPORTED_ITC
    User::Leave(KErrNotSupported);
    #endif
    
    MCESRV_DEBUG("CMceCsSessionImplementation::DoServiceL(no data), Exit");
    
    return returnMessage;
    
    }
    
// -----------------------------------------------------------------------------
// CMceCsSessionImplementation::DoServiceL
// -----------------------------------------------------------------------------
//
void CMceCsSessionImplementation::DoServiceL( TMceIds& /*aIds*/, TInt aFunction, const TDesC8& aInfo )
    {
    MCESRV_DEBUG("CMceCsSessionImplementation::DoServiceL( write ), Entry");
    
    switch( aFunction )
        {
        case EMceItcCancelTranscode:
            {
    		MCESRV_DEBUG("cancelling transcode");
            MCE_READ( TMceItcArgTUint32, TUint32, sessionId );
            iManager.MediaManager().CancelTranscodeFileL( sessionId );
            break;
            }
        default:
            {
        	#ifndef ALLOW_NOT_SUPPORTED_ITC
        	User::Leave(KErrNotSupported);
        	#endif
            }
        }
    
    MCESRV_DEBUG("CMceCsSessionImplementation::DoServiceL( write ), Exit");
    }
    
// -----------------------------------------------------------------------------
// CMceCsSessionImplementation::DoServiceL
// -----------------------------------------------------------------------------
//
void CMceCsSessionImplementation::DoServiceL( TMceIds& aIds, TInt aFunction, 
                                              CMceMsgBase& aMessage )
    {
    MCESRV_DEBUG("CMceCsSessionImplementation::DoServiceL( message ), Entry");
    switch( aFunction )
        {
        case EMceItcTranscode:
            {
    		MCESRV_DEBUG("transcoding");
            CMceMsgObject<CMceComFileSource>* msg = 
                static_cast<CMceMsgObject<CMceComFileSource>*>( &aMessage );
            
            CMceComFileSource* file = msg->Object();
            CleanupStack::PushL( file );
            file->iClient = this;
            TUint32 sessionId = iManager.MediaManager().TranscodeFileL( file );
            aIds.Set( KMceIdsIndexTCId, sessionId );
            CleanupStack::Pop( file );
            break;
            }
        case EMceItcTranscodeWithSession:
            {
           	MCESRV_DEBUG("transcoding with session");
           	
           	// Session structure is needed temporarily by mediamanager
           	// in order to find out output codec types. 
           	//
            CMceMsgObject<CMceComSession>* msg = 
                static_cast<CMceMsgObject<CMceComSession>*>( &aMessage );
            
            CMceComSession* session = msg->Object();
            CleanupStack::PushL( session );
            
            CMceComFileSource* file = NULL;
            for( TInt i = 0; session && ( i < session->Streams().Count() ) && !file; i++ )
                {
                if ( session->Streams()[ i ]->Source() &&
                     session->Streams()[ i ]->Source()->Type() == KMceFileSource )
                    {
                    file = static_cast<CMceComFileSource*>( 
                            session->Streams()[ i ]->Source() );
                    file->iClient = this;
                    }
                }
            __ASSERT_ALWAYS( file, User::Leave( KErrArgument ) );
            TUint32 sessionId = iManager.MediaManager().TranscodeFileL( file );
            aIds.Set( KMceIdsIndexTCId, sessionId );
            
            // Filesource was taken in use from session by mediamanager
            // (took ownership) and rest can be discarded. File is not deleted
            // while deleting session because of ref count increase.
            //
            file->ReferenceCount()++;
            CleanupStack::PopAndDestroy( session );
            break;
            }
        default:
            {
        	#ifndef ALLOW_NOT_SUPPORTED_ITC
        	User::Leave(KErrNotSupported);
        	#endif
            }
        }
    
    MCESRV_DEBUG("CMceCsSessionImplementation::DoServiceL( message ), Exit");
    
    }


// -----------------------------------------------------------------------------
// CMceCsSessionImplementation::GetFileInfoL
// -----------------------------------------------------------------------------
//
void CMceCsSessionImplementation::GetFileInfoL( TMceIds& aIds, 
                                                TMceFileInfo& aFileInfo )
    {
    MCESRV_DEBUG("CMceCsSessionImplementation::GetFileInfoL, Entry");
    
    // Save some stack
    {
    const RMessage2* itcContext = MCE_IDS_ITC_CONTEXT( aIds );
    const TDesC& fileName = aFileInfo.iFileName;
	const TDesC& rootPath = PathInfo::PhoneMemoryRootPath(); 
    TFileName privatePath;
    TFileName realRootPath;

    __ASSERT_ALWAYS( MCE_NOT_NULL_PTR( itcContext ) , User::Leave( KErrArgument ) );
    	
    
    TUid clientUId = itcContext->SecureId();
	privatePath.Format(KMceApplPrivatePath, &rootPath, clientUId.iUid );
	realRootPath.Format(KMceRootPath, &rootPath );
    
    MCESRV_DEBUG_SVALUE16("CMceCsSessionImplementation::GetFileInfoL, file", fileName );
    MCESRV_DEBUG_SVALUE16("CMceCsSessionImplementation::GetFileInfoL, root", rootPath );    
    MCESRV_DEBUG_SVALUE16("CMceCsSessionImplementation::GetFileInfoL, private", privatePath );

	if( itcContext->HasCapability( ECapabilityAllFiles ) )
		{
    	MCESRV_DEBUG("CMceCsSessionImplementation::GetFileInfoL, Client has AllFiles capability");
	    aFileInfo.iAccessRights = TMceFileInfo::EReadWrite;		
		}
	else if( fileName.FindF( rootPath ) == 0 )
		{
    	MCESRV_DEBUG("CMceCsSessionImplementation::GetFileInfoL, Client accessing phone root(data)");
	    aFileInfo.iAccessRights = TMceFileInfo::EReadWrite;		
		}
	else if( fileName.FindF( privatePath ) == 0 )
		{
    	MCESRV_DEBUG("CMceCsSessionImplementation::GetFileInfoL, Client accessing private path");
	    aFileInfo.iAccessRights = TMceFileInfo::EReadWrite;		
		}
	else if( fileName.FindF( privatePath ) == 0 )
		{
    	MCESRV_DEBUG("CMceCsSessionImplementation::GetFileInfoL, Client accessing private path");
	    aFileInfo.iAccessRights = TMceFileInfo::EReadWrite;		
		}
	else if( fileName.FindF( realRootPath ) == KErrNotFound )
		{
    	MCESRV_DEBUG("CMceCsSessionImplementation::GetFileInfoL, Client accessing another drive");
	    aFileInfo.iAccessRights = TMceFileInfo::EReadWrite;		
		}
	else
		{
    	MCESRV_DEBUG("CMceCsSessionImplementation::GetFileInfoL, Permission denied");
	    aFileInfo.iAccessRights = TMceFileInfo::EZero;				
		}	
    }
    
    if ( aFileInfo.iDirection >= TMceFileInfo::ERead && 
    	 aFileInfo.iAccessRights > TMceFileInfo::EZero )
        {
    	MCESRV_DEBUG("CMceCsSessionImplementation::GetFileInfoL, Getting file info");
        iManager.MediaManager().GetFileInfoL( aFileInfo );
        }			

    MCESRV_DEBUG("CMceCsSessionImplementation::GetFileInfoL, Exit");
    }

            

// -----------------------------------------------------------------------------
// CMceCsSessionImplementation::StoreClientContent
// -----------------------------------------------------------------------------
//
void CMceCsSessionImplementation::StoreClientContent( HBufC8* aClientContent )
    {
    delete aClientContent;
    }


// -----------------------------------------------------------------------------
// CMceCsSessionImplementation::EventReceived
// -----------------------------------------------------------------------------
//
void CMceCsSessionImplementation::EventReceived( TMceMccComEvent& aEvent )
    {
    MCESRV_DEBUG("CMceCsSessionImplementation::EventReceived, Entry");

    TMceIds ids;
    ids.iManagerType = KMceCSSIPSession;
    ids.iCallbackID = aEvent.iItcEvent;
	ids.iMediaID = aEvent.iStream ? aEvent.iStream->Id() : KMceMediaIdNotAssigned;
	ids.iSinkID = aEvent.iSink ? aEvent.iSink->Id() : KMceMediaIdNotAssigned;
	ids.iSourceID = aEvent.iSource ? aEvent.iSource->Id() : KMceMediaIdNotAssigned;
	ids.iState = aEvent.iItcData;
	ids.iStatus = aEvent.iError;

    TRAP_IGNORE( SendToClientL( ids, NULL, NULL ) );
        
    MCESRV_DEBUG("CMceCsSessionImplementation::EventReceived, Exit");
    }

// -----------------------------------------------------------------------------
// CMceCsSessionImplementation::ArrayCleanup
// -----------------------------------------------------------------------------
//
void CMceCsSessionImplementation::ArrayCleanup(TAny* aArray)
	{
    RPointerArray<CMceComAudioCodec>* array = 
        reinterpret_cast<RPointerArray<CMceComAudioCodec>*>(aArray);
    if (array)
        {
        array->ResetAndDestroy();
        }
	}



// -----------------------------------------------------------------------------
// CMceCsSessionImplementation::SubSessionL
// -----------------------------------------------------------------------------
//
CMceCsSubSession* CMceCsSessionImplementation::SubSessionL( TUint32 aSessionId )
    {
	CMceCsSubSession* subSession = 
	    reinterpret_cast<CMceCsSubSession*>( aSessionId );
    User::LeaveIfError( iSubSessions.Find( subSession ) );
    return subSession;
	
    }

// -----------------------------------------------------------------------------
// CMceCsSessionImplementation::FindSubSession
// -----------------------------------------------------------------------------
//
const CMceCsSubSession* 
    CMceCsSessionImplementation::FindSubSession( TUint32 aDialogId )
    {
    CMceCsSubSession* subSession = NULL;
    TInt i = 0;
    
    while ( !subSession && i < iSubSessions.Count() )
        {
        subSession = iSubSessions[i++];
        if ( subSession->DialogId() != aDialogId )
            {
            subSession = NULL;
            }
        }
    return subSession;
    }


// -----------------------------------------------------------------------------
// CMceCsSessionImplementation::CreateSubSessionL
// -----------------------------------------------------------------------------
//
void CMceCsSessionImplementation::CreateSubSessionL( TMceIds& aIds,
                                                     CDesC8Array& aParams )
    {
    MCESRV_DEBUG("CMceCsSessionImplementation::CreateSubSessionL, Entry");
	User::LeaveIfNull( iITCProvider );
        
    CMceCsSubSession *subSession = 
        iManager.CreateSubSessionL( ClientSession(), aIds, aParams );
        
    CleanupStack::PushL( subSession );
    iSubSessions.AppendL(subSession);
    CleanupStack::Pop(subSession);
    
    aIds.iSessionID = MCE_SESSION_ID( subSession );
    aIds.iDialogID = subSession->DialogId();
    
    MCESRV_DEBUG("CMceCsSessionImplementation::CreateSubSessionL, Exit");
    }


// -----------------------------------------------------------------------------
// CMceCsSessionImplementation::RemoveSubSessionL
// -----------------------------------------------------------------------------
//
void CMceCsSessionImplementation::RemoveSubSessionL( TUint32 aSessionId )
    {
    MCESRV_DEBUG("CMceCsSessionImplementation::RemoveSubSessionL, Entry");
    MCESRV_DEBUG_DVALUE("id", aSessionId );
    
    
	CMceCsSubSession* subSession = SubSessionL( aSessionId );
    TInt index = iSubSessions.Find( subSession );
    iSubSessions.Remove( index );
    
    if ( !subSession->CanDispose() && 
         !subSession->Terminate() )
        {
        MCESRV_DEBUG("RemoveSubSessionL: could not terminate -> to orphans");
        CleanupStack::PushL( subSession );
        iManager.AddOrphanSubSessionL( subSession );
        CleanupStack::Pop( subSession );
        }
    else
        {
        delete subSession;
        }

    MCESRV_DEBUG("CMceCsSessionImplementation::RemoveSubSessionL, Exit");
    
    }


// -----------------------------------------------------------------------------
// CMceCsSessionImplementation::RemoveAllSubSessions
// -----------------------------------------------------------------------------
//
void CMceCsSessionImplementation::RemoveAllSubSessions( TBool aMakeOrphan )
    {
    MCESRV_DEBUG("CMceCsSessionImplementation::RemoveAllSubSessions, Entry");
    
    while( iSubSessions.Count() > 0 )
        {
        TBool deleteSS = ETrue;
        
    	CMceCsSubSession* subSession = iSubSessions[ 0 ];
        iSubSessions.Remove( 0 );
    
        if ( !subSession->CanDispose() && 
             !subSession->Terminate() )
            {
            if ( aMakeOrphan )
                {
                TRAPD( error, iManager.AddOrphanSubSessionL( subSession ) );
                deleteSS = error == KErrNone ? EFalse : ETrue;
                }
            }
            
        if ( deleteSS )
            {
            delete subSession;
            }
        }
    MCESRV_DEBUG("CMceCsSessionImplementation::RemoveAllSubSessions, Exit");
    }


// -----------------------------------------------------------------------------
// CMceCsSessionImplementation::SendToClientL
// -----------------------------------------------------------------------------
//
void CMceCsSessionImplementation::SendToClientL( TMceIds& aIds, 
                                                 HBufC8* aContext, 
                                                 HBufC8* aContent )
    {
    MCESRV_DEBUG("CMceCsSessionImplementation::SendToClientL, Entry");
    
    if ( !IsOrphan() )
        {
        CMceCsSessionReceiver* receiver = ClientSession().ReceiverL( aIds );
        TMceIds ids = receiver->Ids();
        ids.Copy( aIds );

        MCESRV_DEBUG_IDS("IDS", ids );
                        
        if ( aContent )
            {
            receiver->SendToClientL( ids, aContext, aContent );
            }
        else if( aContext )
            {
            receiver->SendToClientL( ids, aContext );
            }
        else
            {
            receiver->SendToClientL( ids );
            }
        }
    else
        {
        MCESRV_DEBUG("SendToClientL: orphan -> no recipient to send");
        delete aContext;
        delete aContent;
        }
        
    MCESRV_DEBUG("CMceCsSessionImplementation::SendToClientL, Exit");
    
    }

// -----------------------------------------------------------------------------
// CMceCsSessionImplementation::IsOrphan
// -----------------------------------------------------------------------------
//
TBool CMceCsSessionImplementation::IsOrphan()
    {
    return MCE_IS_NULL_PTR( iITCProvider );
    }

// -----------------------------------------------------------------------------
// CMceCsSessionImplementation::ClientSession
// -----------------------------------------------------------------------------
//
CMceCsSession& CMceCsSessionImplementation::ClientSession()
    {
    return *iITCProvider;
    }

// -----------------------------------------------------------------------------
// CMceCsSessionImplementation::StoreDefaultHeadersL
// -----------------------------------------------------------------------------
//
void CMceCsSessionImplementation::StoreDefaultHeadersL( CDesC8Array& aMethods, 
                                                        CDesC8Array* aHeaders )
    {
    iDefaults->StoreDefaultHeadersL( aMethods, aHeaders );
    }

// -----------------------------------------------------------------------------
// CMceCsSessionImplementation::DefaultHeaders
// -----------------------------------------------------------------------------
//
CDesC8Array& CMceCsSessionImplementation::DefaultHeaders( TInt aMethodInd )
    {
    return iDefaults->DefaultHeaders( aMethodInd );
    }

// -----------------------------------------------------------------------------
// CMceCsSessionImplementation::DefaultHeaders
// -----------------------------------------------------------------------------
//	
HBufC8* CMceCsSessionImplementation::DefaultHeadersL( TDesC8& aMethod )
    {
    MCESRV_DEBUG("CMceCsSessionImplementation::DefaultHeadersL, Entry");
	
	CDesC8Array& headers = iDefaults->DefaultHeaders( aMethod );
    
	CMceMsgTextArray* headersMsg = new (ELeave) CMceMsgTextArray( headers );
    CleanupStack::PushL( headersMsg );
    headersMsg->EncodeL();
    
    HBufC8* headersStr = headersMsg->EncodeBufferCloneL();
    
    CleanupStack::PopAndDestroy( headersMsg );
    
    MCESRV_DEBUG("CMceCsSessionImplementation::DefaultHeadersL, Exit");
	
	return headersStr;
    
	
    }

//  End of File
