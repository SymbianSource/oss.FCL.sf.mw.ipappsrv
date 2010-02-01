/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Client-side handle to SIP Client Simulator server session
*
*/

#include <s32strm.h>
#include <s32mem.h>
#include "sipclientsimulatorcli.h"
#include "sipclientsimulatorconsts.h"
#include "sdpmediafield.h"
#include "sipcontenttypeheader.h"
#include "sipstrings.h"
#include "sdpdocument.h"
// ----------------------------------------------------------------------------
// RSipClientSimulator::RSipClientSimulator
// ----------------------------------------------------------------------------
//
EXPORT_C RSipClientSimulator::RSipClientSimulator()
	: RSessionBase(), iIsConnected( EFalse )
	{
	}

// ----------------------------------------------------------------------------
// RSipClientSimulator::Connect
// ----------------------------------------------------------------------------
//
EXPORT_C TInt RSipClientSimulator::Connect()
	{
	if( iIsConnected )
		{
		return KErrAlreadyExists;
		}

	TRAPD( status, StartServerL() );

	// If server was started or was already running, establish a session to it.
	if( status == KErrNone )
		{
	    TVersion version( KSipClientSimulatorMajorVersion,
					      KSipClientSimulatorMinorVersion,
					      KSipClientSimulatorBuildVersion );		
		
		status = CreateSession( KSipClientSimulatorName, version );
		}

    iIsConnected = ( status == KErrNone ); 

	return status;
	}

// ----------------------------------------------------------------------------
// RSipClientSimulator::Close
// ----------------------------------------------------------------------------
//
EXPORT_C void RSipClientSimulator::Close()
	{
	// Close the session if we are connected
	if( iIsConnected )
		{		
		RHandleBase::Close();
		iIsConnected = EFalse;
		}
	}

// ----------------------------------------------------------------------------
// RSipClientSimulator::SetClientMatchType
// ----------------------------------------------------------------------------
//
EXPORT_C TInt RSipClientSimulator::SetClientMatchType(
    TMatchType aMatchType, 
    const TUid& aClientUid )
    {
    TIpcArgs args;
    args.Set( ESipClientSimulatorIpcArgClientUid, aClientUid.iUid );
    args.Set( ESipClientSimulatorIpcArgClientMatchType, aMatchType );
    return SendReceive( ESipClientSimulatorSetClientMatchType, args );
    }

// ----------------------------------------------------------------------------
// RSipClientSimulator::SetClientSupportedContentTypesL
// ----------------------------------------------------------------------------
//
EXPORT_C TInt RSipClientSimulator::SetClientSupportedContentTypesL(
    				RPointerArray<CSIPContentTypeHeader> aContentTypeHeaders,
    				const TUid& aClientUid )
    {
    CBufFlat* buf = ExternalizeLC( aContentTypeHeaders );
    TPtr8 externalized = buf->Ptr( 0 );
    TIpcArgs args;
    args.Set( ESipClientSimulatorIpcArgClientUid, aClientUid.iUid );
    args.Set( ESipClientSimulatorIpcArgContentTypes, &externalized );
    return SendReceive( ESipClientSimulatorSetClientContentTypes, args );
    }

// ----------------------------------------------------------------------------
// RSipClientSimulator::SetClientSupportedSdpMediasL
// ----------------------------------------------------------------------------
//
EXPORT_C TInt RSipClientSimulator::SetClientSupportedSdpMediasL(
    				RPointerArray<CSdpMediaField> aSdpMediaFields,
    				const TUid& aClientUid )
    {
    CBufFlat* buf = ExternalizeLC( aSdpMediaFields );
    TPtr8 externalized = buf->Ptr( 0 );
    TIpcArgs args;
    args.Set( ESipClientSimulatorIpcArgClientUid, aClientUid.iUid );
    args.Set( ESipClientSimulatorIpcArgSdpMediaFields, &externalized );
    return SendReceive( ESipClientSimulatorSetClientSdpMedias, args );
    }

// -----------------------------------------------------------------------------
// RSipClientSimulator::ExternalizeLC
// -----------------------------------------------------------------------------
//
CBufFlat* RSipClientSimulator::ExternalizeLC(
	                RPointerArray<CSIPContentTypeHeader> aContentTypeHeaders )
	{
	CBufFlat* buf = CBufFlat::NewL(100);
	CleanupStack::PushL(buf);
	RBufWriteStream writeStream(*buf,0);
	writeStream.PushL();
	
	writeStream.WriteUint32L ( aContentTypeHeaders.Count() );
	for ( TInt i = 0;i < aContentTypeHeaders.Count();i++ )
		{
	
		aContentTypeHeaders[i]->ExternalizeL( writeStream, EFalse );
	
		}
	CleanupStack::Pop(1); // writeStream
	writeStream.Close();
    return buf;
	}
	
// -----------------------------------------------------------------------------
// RSipClientSimulator::ExternalizeLC
// -----------------------------------------------------------------------------
//
CBufFlat* RSipClientSimulator::ExternalizeLC(
	                RPointerArray<CSdpMediaField> aSdpMediaFields )
	{
	CSdpDocument* sdpdocument = CSdpDocument::NewLC();
	for ( TInt i = 0;i < aSdpMediaFields.Count();i++ )
		{
		sdpdocument->MediaFields().AppendL(aSdpMediaFields[i]->CloneL());
		}
	CBufFlat* buf = CBufFlat::NewL(100);
	CleanupStack::PushL(buf);
	RBufWriteStream writeStream(*buf,0);
	writeStream.PushL();
	sdpdocument->ExternalizeL( writeStream );
	CleanupStack::Pop(1); // writeStream
	writeStream.Close();
	CleanupStack::Pop(2);//buf,sdpdocument
	delete sdpdocument;
	sdpdocument = NULL;
	CleanupStack::PushL(buf);
    return buf;
	}	

// ---------------------------------------------------------------------------
// RSipClientSimulator::InternalizeArrayL
// ---------------------------------------------------------------------------
//
void RSipClientSimulator::InternalizeArrayL(
					RPointerArray<CSIPContentTypeHeader>& aContentTypeHeaders, 
					RReadStream& aStream )
	{
	TInt count = aStream.ReadUint32L();
	for (TInt i = 0;i < count;i++)
		{
		CSIPContentTypeHeader* obj = static_cast<CSIPContentTypeHeader*>
							(CSIPContentTypeHeader::InternalizeValueL(aStream));
		CleanupStack::PushL(obj);
		User::LeaveIfError(aContentTypeHeaders.Append(obj));
		CleanupStack::Pop();//obj
		}
	}  

// ---------------------------------------------------------------------------
// RSipClientSimulator::InternalizeArrayL
// ---------------------------------------------------------------------------
//
void RSipClientSimulator::InternalizeArrayL(
					RPointerArray<CSdpMediaField>& aSdpMediaFields, 
					RReadStream& aStream )
	{
	CSdpDocument* sdpdocument = CSdpDocument::InternalizeL(aStream);
	CleanupStack::PushL(sdpdocument);
	for ( TInt i = 0;i < sdpdocument->MediaFields().Count();i++ )
		{
		aSdpMediaFields.AppendL(sdpdocument->MediaFields()[i]->CloneL());
		}
	CleanupStack::Pop(sdpdocument);	
	delete sdpdocument;
	sdpdocument = NULL;
	}    

// ----------------------------------------------------------------------------
// RSipClientSimulator::GetClientMatchType
// ----------------------------------------------------------------------------
//
EXPORT_C TInt RSipClientSimulator::GetClientMatchType( 
    TMatchType& aMatchType,
    TUid& aClientUid )
    {
    TIpcArgs args;
    TPckg< TMatchType > matchType( aMatchType );
    args.Set( ESipClientSimulatorIpcArgClientMatchType, &matchType );
    TPckg< TInt > uidValue( 0 );
    args.Set( ESipClientSimulatorIpcArgClientUid, &uidValue );
    TInt err = SendReceive( ESipClientSimulatorGetClientMatchType, args );
    aMatchType = matchType();
    aClientUid.iUid = uidValue();
    return err;
    }

// ----------------------------------------------------------------------------
// RSipClientSimulator::ClientSupportedContentTypesL
// ----------------------------------------------------------------------------
//
EXPORT_C RPointerArray<CSIPContentTypeHeader>  
	RSipClientSimulator::ClientSupportedContentTypesL( TUid& aClientUid )
    {
    TIpcArgs args;
    HBufC8* buf = HBufC8::NewLC( 100 );
    TPtr8 bufptr = buf->Des();
    args.Set( ESipClientSimulatorIpcArgContentTypes, &bufptr );
    TPckg< TInt > uidValue( 0 );
    args.Set( ESipClientSimulatorIpcArgClientUid, &uidValue );
    TInt err = SendReceive( ESipClientSimulatorGetClientContentTypes, args );
  
    RDesReadStream readStream( *buf );
    CleanupStack::Pop( buf );
    readStream.PushL();
    RPointerArray<CSIPContentTypeHeader> contentTypeHeaders;
    InternalizeArrayL( contentTypeHeaders, readStream );
    readStream.Pop();
    readStream.Close();
    aClientUid.iUid = uidValue();
    return contentTypeHeaders;
    } 

// ----------------------------------------------------------------------------
// RSipClientSimulator::ClientSupportedMediaFieldsL
// ----------------------------------------------------------------------------
//
EXPORT_C RPointerArray<CSdpMediaField>  
	RSipClientSimulator::ClientSupportedMediaFieldsL( TUid& aClientUid )
    {
	TIpcArgs args;
    HBufC8* buf = HBufC8::NewLC( 100 );
    TPtr8 bufptr = buf->Des();
    args.Set( ESipClientSimulatorIpcArgSdpMediaFields, &bufptr );
    TPckg< TInt > uidValue( 0 );
    args.Set( ESipClientSimulatorIpcArgClientUid, &uidValue );
    TInt err = SendReceive( ESipClientSimulatorGetClientSdpMedias, args );
    
    RDesReadStream readStream( *buf );
    CleanupStack::Pop( buf );
    readStream.PushL();
    RPointerArray<CSdpMediaField> sdpMediaFields;
    InternalizeArrayL( sdpMediaFields, readStream );
    readStream.Pop();
    readStream.Close();
    aClientUid.iUid = uidValue();
    return sdpMediaFields;
    }

// ----------------------------------------------------------------------------
// RSipClientSimulator::ConnectClient
// ----------------------------------------------------------------------------
//		
EXPORT_C TInt RSipClientSimulator::ConnectClient( 
    const TUid& aClientUid )
    {
    TIpcArgs args;
    args.Set( ESipClientSimulatorIpcArgClientUid, aClientUid.iUid );
    return SendReceive( ESipClientSimulatorConnectClient, args );
    }

// ----------------------------------------------------------------------------
// RSipClientSimulator::DisconnectClient
// ----------------------------------------------------------------------------
//        
EXPORT_C TInt RSipClientSimulator::DisconnectClient()
    {
    TIpcArgs args;
    return SendReceive( ESipClientSimulatorDisconnectClient, args );
    }

// ----------------------------------------------------------------------------
// RSipClientSimulator::StartServerL
// ----------------------------------------------------------------------------
//
void RSipClientSimulator::StartServerL()
	{
	if( !IsServerStarted() )
		{
		// Create a global semaphore for waiting the server to start
		RSemaphore semaphore;
		TInt status = semaphore.OpenGlobal( KSipClientSimulatorName );
		if( status == KErrNotFound )
			{
			User::LeaveIfError( 
			    semaphore.CreateGlobal( KSipClientSimulatorName, 0 ) );
			}
		CleanupClosePushL( semaphore );
		
    	const TUidType serverUid( 
    	    KNullUid, KNullUid, KSipClientSimulatorServerUid );
	
	    RProcess server;
	    User::LeaveIfError( 
	        server.Create( KSipClientSimulatorName, KNullDesC, serverUid ) );
	    server.Resume();
	    semaphore.Wait();
	    TInt exitReason = server.ExitReason();
	    server.Close();
	    CleanupStack::PopAndDestroy( 1 ); // semaphore
	    
        User::LeaveIfError( exitReason );
		}

	}

// ----------------------------------------------------------------------------
// RSipClientSimulator::IsServerStarted
// ----------------------------------------------------------------------------
//
TBool RSipClientSimulator::IsServerStarted() const
	{
	TFindServer findServer( KSipClientSimulatorName );
	TFullName name;
	return ( findServer.Next( name ) == KErrNone );
	}
