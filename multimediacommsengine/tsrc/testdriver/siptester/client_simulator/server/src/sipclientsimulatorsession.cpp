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
* Description:  Implementation of the server side Client-Server session
*
*/

#include <s32mem.h>
#include "sipclientsimulatorserver.h"
#include "sipclientsimulatorsession.h"
#include "sipcontenttypeheader.h"
#include "sdpmediafield.h"
#include "sdpdocument.h"

// ----------------------------------------------------------------------------
// CSipClientSimulatorSession::NewL
// ----------------------------------------------------------------------------
//
CSipClientSimulatorSession* CSipClientSimulatorSession::NewL( 
    CSipClientSimulatorServer& aServer )
	{
	CSipClientSimulatorSession* self = 
	    new( ELeave )CSipClientSimulatorSession( aServer );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

// ----------------------------------------------------------------------------
// CSipClientSimulatorSession::CSipClientSimulatorSession
// ----------------------------------------------------------------------------
//
CSipClientSimulatorSession::CSipClientSimulatorSession(
    CSipClientSimulatorServer& aServer )
    : iMyServer(aServer)
	{
	}

// ----------------------------------------------------------------------------
// CSipClientSimulatorSession::ConstructL
// ----------------------------------------------------------------------------
//
void CSipClientSimulatorSession::ConstructL()
	{
	iMyServer.SessionAdded();
	}

// ----------------------------------------------------------------------------
// CSipClientSimulatorSession::~CSipClientSimulatorSession
// ----------------------------------------------------------------------------
//
CSipClientSimulatorSession::~CSipClientSimulatorSession()
	{
	iMyServer.SessionRemoved();
	}

// ----------------------------------------------------------------------------
// CSipClientSimulatorSession::ServiceL
// ----------------------------------------------------------------------------
//
void CSipClientSimulatorSession::ServiceL( const RMessage2& aMessage )
	{
	// Any leaves from this function will be catched by the Active Scheduler
	// and forwarded to CSipClientSimulatorServer::RunError().
	// Check for session related requests
	TInt status( KErrNone );
	switch( aMessage.Function() )
		{
		case ESipClientSimulatorSetClientMatchType:
			{
			SetClientMatchTypeL( aMessage );
			break;
			}
		case ESipClientSimulatorGetClientMatchType:
			{
			GetClientMatchTypeL( aMessage );	
			break;
			}
		case ESipClientSimulatorConnectClient:
			{
			ConnectClientL( aMessage );	
			break;
			}
		case ESipClientSimulatorDisconnectClient:
			{
			DisconnectClient();
			break;
			}
			
		case ESipClientSimulatorSetClientContentTypes:
			{
			SetClientSupportedContentTypesL( aMessage );
			break;
			}
			
		case ESipClientSimulatorGetClientContentTypes:
			{
			GetClientSupportedContentTypesL( aMessage );
			break;
			}
			
		case ESipClientSimulatorSetClientSdpMedias:
			{
			SetClientSupportedSdpMediasL( aMessage );
			break;
			}
			
		case ESipClientSimulatorGetClientSdpMedias:
			{
			GetClientSupportedSdpMediasL( aMessage );
			break;
			}			
	
		default:
			{
			// Unknown request
			status = KErrUnknown;
			break;
			}
		}
	aMessage.Complete( status );
	}

// ----------------------------------------------------------------------------
// CSipClientSimulatorSession::SetClientMatchTypeL
// ----------------------------------------------------------------------------
//
void CSipClientSimulatorSession::SetClientMatchTypeL( 
    const RMessage2& aMessage )
    {
    TUid clientUid( TUid::Null() );
    ReadUidL( clientUid, aMessage );
    TInt matchType( 0 );
    GetIpcArgIntValueL( ESipClientSimulatorIpcArgClientMatchType, 
                        matchType,
                        aMessage );
    iMyServer.SetClientMatchType( 
        static_cast< RSipClientSimulator::TMatchType >( matchType ), 
        clientUid );
    }

// ----------------------------------------------------------------------------
// CSipClientSimulatorSession::SetClientSupportedContentTypesL
// ----------------------------------------------------------------------------
//
void CSipClientSimulatorSession::SetClientSupportedContentTypesL( 
    const RMessage2& aMessage )
    {
    TUid clientUid( TUid::Null() );
    ReadUidL( clientUid, aMessage );
    HBufC8* buf = GetIpcArgDesValueLC( ESipClientSimulatorIpcArgContentTypes, 
                        			  aMessage );
    RDesReadStream readStream( *buf );
    CleanupStack::Pop( buf );
    readStream.PushL();
    RPointerArray<CSIPContentTypeHeader> contentTypeHeaders;
    InternalizeArrayL( contentTypeHeaders, readStream );
    readStream.Pop();
    readStream.Close();
    iMyServer.SetClientSupportedContentTypesL( contentTypeHeaders, clientUid );
    }
    
// ----------------------------------------------------------------------------
// CSipClientSimulatorSession::SetClientSupportedSdpMediasL
// ----------------------------------------------------------------------------
//
void CSipClientSimulatorSession::SetClientSupportedSdpMediasL( 
    const RMessage2& aMessage )
    {
    TUid clientUid( TUid::Null() );
    ReadUidL( clientUid, aMessage );
    HBufC8* buf = GetIpcArgDesValueLC( ESipClientSimulatorIpcArgSdpMediaFields, 
                        			  aMessage );
    RDesReadStream readStream( *buf );
    CleanupStack::Pop( buf );
    readStream.PushL();
    RPointerArray<CSdpMediaField> sdpMediaFields;
    InternalizeArrayL( sdpMediaFields, readStream );
    readStream.Pop();
    readStream.Close();
    iMyServer.SetClientSupportedSdpMediasL( sdpMediaFields, clientUid );
    }    
    
// ----------------------------------------------------------------------------
// CSipClientSimulatorSession::GetClientSupportedContentTypesL
// ----------------------------------------------------------------------------
//     
void CSipClientSimulatorSession::GetClientSupportedContentTypesL( 
    const RMessage2& aMessage )
    {
    CBufFlat* buf = ExternalizeLC (iMyServer.ClientSupportedContentTypesL());
	TPtr8 externalized = buf->Ptr(0);
	WriteL(ESipClientSimulatorIpcArgContentTypes, aMessage, externalized);
	CleanupStack::PopAndDestroy(buf);
    
    TInt clientUidValue = iMyServer.ClientUid().iUid;
    WriteToClientL( ESipClientSimulatorIpcArgClientUid, 
                    clientUidValue,
                    aMessage);
    }
    
// ----------------------------------------------------------------------------
// CSipClientSimulatorSession::GetClientSupportedSdpMediasL
// ----------------------------------------------------------------------------
//     
void CSipClientSimulatorSession::GetClientSupportedSdpMediasL( 
    const RMessage2& aMessage )
    {
    CBufFlat* buf = ExternalizeLC (iMyServer.ClientSupportedSdpMediasL());
	TPtr8 externalized = buf->Ptr(0);
	WriteL(ESipClientSimulatorIpcArgSdpMediaFields, aMessage, externalized);
	CleanupStack::PopAndDestroy(buf);
    
    TInt clientUidValue = iMyServer.ClientUid().iUid;
    WriteToClientL( ESipClientSimulatorIpcArgClientUid, 
                    clientUidValue,
                    aMessage);
    }       
    
// -----------------------------------------------------------------------------
// CSipClientSimulatorSession::ExternalizeLC
// -----------------------------------------------------------------------------
//
CBufFlat* CSipClientSimulatorSession::ExternalizeLC(
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
// CSipClientSimulatorSession::ExternalizeLC
// -----------------------------------------------------------------------------
//
CBufFlat* CSipClientSimulatorSession::ExternalizeLC(
	                RPointerArray<CSdpMediaField> aSdpMediaFields )
	{
	CSdpDocument* sdpdocument = CSdpDocument::NewLC();
	for ( TInt i = 0;i < aSdpMediaFields.Count();i++ )
		{
		sdpdocument->MediaFields().AppendL(aSdpMediaFields[i]->CloneL());
		}
	CBufFlat* buf = CBufFlat::NewL(300);
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
	    
// -----------------------------------------------------------------------------
// CSipClientSimulatorSession::WriteL
// -----------------------------------------------------------------------------
//
void CSipClientSimulatorSession::WriteL(TSipClientSimulatorIpcArgs aItcArgIndex,
	const RMessage2&  aMessage,
	const TDesC8&    aDes)
	{
    TInt length = aMessage.GetDesMaxLength(aItcArgIndex);
    __ASSERT_ALWAYS(length >= aDes.Length(), User::Leave (KErrOverflow));

    aMessage.WriteL (aItcArgIndex, aDes);
    }    
    
// ---------------------------------------------------------------------------
// CSipClientSimulatorSession::InternalizeArrayL
// ---------------------------------------------------------------------------
//
void CSipClientSimulatorSession::InternalizeArrayL(
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
// CSipClientSimulatorSession::InternalizeArrayL
// ---------------------------------------------------------------------------
//
void CSipClientSimulatorSession::InternalizeArrayL(
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
    
// -----------------------------------------------------------------------------
// CSipClientSimulatorSession::GetIpcArgDesValueL
// -----------------------------------------------------------------------------
//
HBufC8* CSipClientSimulatorSession::GetIpcArgDesValueLC(
    TSipClientSimulatorIpcArgs aIpcArgIndex,
    const RMessage2&  aMessage ) const
	{
	TInt length = aMessage.GetDesLength(aIpcArgIndex);
	__ASSERT_ALWAYS( length >= 0, User::Leave( KErrBadDescriptor ));
	HBufC8* buf = HBufC8::NewLC( length );
	TPtr8 bufPtr(buf->Des());
	aMessage.ReadL( aIpcArgIndex, bufPtr );
	return buf;
	}   

// ----------------------------------------------------------------------------
// CSipClientSimulatorSession::GetClientMatchTypeL
// ----------------------------------------------------------------------------
//     
void CSipClientSimulatorSession::GetClientMatchTypeL( 
    const RMessage2& aMessage )
    {
    RSipClientSimulator::TMatchType matchType = iMyServer.ClientMatchType();
    WriteToClientL( ESipClientSimulatorIpcArgClientMatchType, 
                    matchType, 
                    aMessage);
    TInt clientUidValue = iMyServer.ClientUid().iUid;
    WriteToClientL( ESipClientSimulatorIpcArgClientUid, 
                    clientUidValue,
                    aMessage);
    }

// ----------------------------------------------------------------------------
// CSipClientSimulatorSession::ConnectClientL
// ----------------------------------------------------------------------------
//
void CSipClientSimulatorSession::ConnectClientL( const RMessage2& aMessage )
    {
    TUid clientUid( TUid::Null() );
    ReadUidL( clientUid, aMessage );
    iMyServer.CreateClientSimulatorL( clientUid );
    }

// ----------------------------------------------------------------------------
// CSipClientSimulatorSession::DisconnectClient
// ----------------------------------------------------------------------------
//        
void CSipClientSimulatorSession::DisconnectClient()
    {
    iMyServer.DestroyClientSimulator();
    }

// ----------------------------------------------------------------------------
// CSipClientSimulatorSession::ReadUidL
// ----------------------------------------------------------------------------
//
void CSipClientSimulatorSession::ReadUidL( 
    TUid& aValue,
    const RMessage2& aMessage ) const
    {
    TInt value( 0 );
    GetIpcArgIntValueL( ESipClientSimulatorIpcArgClientUid, value, aMessage );
    aValue.iUid = value;
    }

// -----------------------------------------------------------------------------
// CSipClientSimulatorSession::GetIpcArgIntValueL
// -----------------------------------------------------------------------------
//
void CSipClientSimulatorSession::GetIpcArgIntValueL(
    TSipClientSimulatorIpcArgs aIpcArgIndex,
    TInt& aValue,
    const RMessage2&  aMessage ) const
	{
    switch( static_cast<TInt>( aIpcArgIndex ) )
		{
        case 0: aValue = aMessage.Int0(); break;
        case 1: aValue = aMessage.Int1(); break;
        case 2: aValue = aMessage.Int2(); break;
        case 3: aValue = aMessage.Int3(); break;
        default: User::Leave(KErrArgument); break;
		}
	}
	
// -----------------------------------------------------------------------------
// CSipClientSimulatorSession::WriteToClientL
// -----------------------------------------------------------------------------
//
void CSipClientSimulatorSession::WriteToClientL(
    TSipClientSimulatorIpcArgs aIpcArgIndex,
    TInt aValue,
    const RMessage2& aMessage ) const
    {
    TPckg<TInt> valuePckg( aValue );
    TInt length = aMessage.GetDesMaxLength( aIpcArgIndex );
    if ( length < 0 )
        {
        User::Leave( KErrArgument );
        }
    if ( length < valuePckg.Length() )
        {
        User::Leave( KErrOverflow );
        }
    aMessage.WriteL( aIpcArgIndex,valuePckg );
    }
