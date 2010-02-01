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
* Description:    Implementation
*
*/



#include "MCEConstants.h"
#include "TCmdGetStreamInfo.h"
#include "CTcMCEContext.h"
#include <MCEAudioStream.h>
#include <MCEVideoStream.h>
#include <MCERTPSource.h>
#include <MCEMicSource.h>
#include <MCECameraSource.h>
#include <MCERTPSink.h>
#include <MCESpeakerSink.h>
#include <MCEDisplaySink.h>
#include <mcefilesink.h>
#include <MCEAudioCodec.h>
#include <badesca.h>

#include "mcefilesource.h"

// -----------------------------------------------------------------------------
// TCmdGetStreamInfo::ExecuteL
// -----------------------------------------------------------------------------
//
void TCmdGetStreamInfo::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------

	// Get stream 
	CMceSession* session= 
		reinterpret_cast<CMceSession*>(GetObjectForIdL(KSessionId, ETrue));	
	CDesC8Array* mediaStreams = new(ELeave)CDesC8ArraySeg(1);
	CleanupStack::PushL( mediaStreams );
	const RPointerArray<CMceMediaStream>& streamPtrs = session->Streams();
	
	for (TInt streamIndex=0; streamIndex < streamPtrs.Count(); streamIndex++)
		{
		// Add to registry by reference and fetch name
		iContext.Registry().AddObjectL( *(streamPtrs[streamIndex]) );	
		TBuf8< KTcMaxObjectName > streamId = 
					iContext.Registry().ObjectNameL( streamPtrs[streamIndex] );	
		mediaStreams->AppendL( streamId );
		}
	
	// ---------- Execution ----------------------------------------------------

	// ---------- Response creation --------------------------------------------
	
	AddIdResponseL(KSessionId, session); // uses CleanupStack internally

	if ( mediaStreams->Count() > 0 )
		{
		AddArrayResponseL( KResponseStreams, *mediaStreams );	
		
		for (TInt streamIndex=0; streamIndex < streamPtrs.Count(); streamIndex++)
			{
			if ((streamPtrs[streamIndex])->Type()==KMceAudio)
				{
				CMceAudioStream* audioStream =reinterpret_cast<CMceAudioStream*>
						(iContext.Registry().ObjectPtrL( mediaStreams->MdcaPoint(streamIndex) ));
				//get the audio stream info
				GetAudioStreamInfo( audioStream );
				}
			else if ((streamPtrs[streamIndex])->Type()==KMceVideo)
				{
				CMceVideoStream* videoStream =reinterpret_cast<CMceVideoStream*>
						(iContext.Registry().ObjectPtrL( mediaStreams->MdcaPoint(streamIndex) ));
				//get the video stream info
				GetVideoStreamInfo( videoStream );
				}
			else
				{
				// Keep pc-lint happy
				}
			}
		CleanupStack::PopAndDestroy( mediaStreams );
		}
	else
		{
		CleanupStack::PopAndDestroy( mediaStreams );
		}

	}

// -----------------------------------------------------------------------------
// TCmdGetStreamInfo::AddSourceToResponse
// -----------------------------------------------------------------------------
//
void
TCmdGetStreamInfo::AddSourceToResponseL( const TDesC8& aParamNameForSourceId,
										 const TDesC8& aParamNameForSource,
										 CMceMediaStream& aStream )
	{
	CMceMediaSource* source = aStream.Source();

	iContext.Registry().AddObjectL( *source );
	TBuf8< KTcMaxObjectName > sourceId =
		iContext.Registry().ObjectNameL( source );
	AddTextResponseL( aParamNameForSourceId, sourceId );
	switch ( source->Type() )
		{
		case KMceRTPSource:
			{
			AddTextResponseL( aParamNameForSource, KValueSourceRTP );
			break;
			}
		case KMceMicSource:
			{
			AddTextResponseL( aParamNameForSource, KValueSourceMic );
			break;
			}	
		case KMceCameraSource:
			{
			AddTextResponseL( aParamNameForSource, KValueSourceCamera );
			break;	
			}		
		case KMceFileSource:
			{
			AddTextResponseL( aParamNameForSource, KValueSourceFile );
			break;	
			}
		default:
			{
			AddTextResponseL( aParamNameForSource, KValueSourceUnknown );
			break;	
			}
		}
	}

// -----------------------------------------------------------------------------
// TCmdGetStreamInfo::AddSinksToResponse
// -----------------------------------------------------------------------------
//
void TCmdGetStreamInfo::AddSinksToResponseL( const TDesC8& aParamNameForSinkIds,
                                             const TDesC8& aParamNameForSinks,
								             CMceMediaStream& aStream )
	{
	// Array for sink ids
	CDesC8Array* sinkIds = new(ELeave)CDesC8ArraySeg(1);
	CleanupStack::PushL( sinkIds );
	
	// Array for sink names
	CDesC8Array* sinks = new(ELeave)CDesC8ArraySeg(1);
	CleanupStack::PushL( sinks );

	const RPointerArray<CMceMediaSink>& sinkPtrs = aStream.Sinks();
	for ( TInt i = 0; i < sinkPtrs.Count(); i++ )
		{
		// Id
		iContext.Registry().AddObjectL( *(sinkPtrs[i]) );	
		TPtrC8 sinkId = iContext.Registry().ObjectNameL( sinkPtrs[i] );
		sinkIds->AppendL( sinkId );
		
		// Name
		switch ( sinkPtrs[i]->Type() )
			{
			case KMceRTPSink:
				{
				sinks->AppendL( KValueSinkRTP );
				break;	
				}
			case KMceSpeakerSink:
				{
				sinks->AppendL( KValueSinkSpeaker );
				break;	
				}
			case KMceDisplaySink:
				{
				sinks->AppendL( KValueSinkDisplay );
				break;	
				}
			case KMceFileSink:
				{
				sinks->AppendL( KValueSinkFile );
				break;	
				}	
			default:
				{
				sinks->AppendL( KValueSinkUnknown );
				break;
				}
			}
		}
	
	if ( sinks->Count() )
		{	
		AddArrayResponseL( aParamNameForSinkIds, *sinkIds );
		AddArrayResponseL( aParamNameForSinks, *sinks );
		}
		
	CleanupStack::PopAndDestroy( sinks );
	CleanupStack::PopAndDestroy( sinkIds );	
	}

// -----------------------------------------------------------------------------
// TCmdGetStreamInfo::AddAudioCodecsToResponse
// -----------------------------------------------------------------------------
//
void TCmdGetStreamInfo::AddAudioCodecsToResponseL( const TDesC8& aName,
											  	   CMceAudioStream& aStream )
	{
	CDesC8Array* codecs = new(ELeave)CDesC8ArraySeg(1);
	CleanupStack::PushL( codecs );

	const RPointerArray<CMceAudioCodec>& codecPtrs = aStream.Codecs();
	for (TInt codecIndex = 0; codecIndex < codecPtrs.Count(); codecIndex++ )
		{		
		// Add codec to registry by reference and fetch name
		iContext.Registry().AddObjectL( *(codecPtrs[codecIndex]) );	
		TBuf8< KTcMaxObjectName > codecId = 
					iContext.Registry().ObjectNameL( codecPtrs[codecIndex] );	
		codecs->AppendL( codecId );
		}

	if ( codecs->Count() )
		{
		AddArrayResponseL( aName, *codecs );
		}
		
	CleanupStack::PopAndDestroy( codecs );	
	}

// -----------------------------------------------------------------------------
// TCmdGetStreamInfo::AddVideoCodecsToResponse
// -----------------------------------------------------------------------------
//
void TCmdGetStreamInfo::AddVideoCodecsToResponseL( const TDesC8& aName,
											  	   CMceVideoStream& aStream )
	{
	CDesC8Array* codecs = new(ELeave)CDesC8ArraySeg(1);
	CleanupStack::PushL( codecs );

	const RPointerArray<CMceVideoCodec>& codecPtrs = aStream.Codecs();
	for (TInt codecIndex = 0; codecIndex < codecPtrs.Count(); codecIndex++ )
		{
		// Add codec to registry by reference and fetch name
		iContext.Registry().AddObjectL( *(reinterpret_cast<CBase*>(codecPtrs[codecIndex]) ));

		TBuf8< KTcMaxObjectName > codecId = 
					iContext.Registry().ObjectNameL( codecPtrs[codecIndex] );	
		codecs->AppendL( codecId );
		}

	if ( codecs->Count() )
		{
		AddArrayResponseL( aName, *codecs );
		}
		
	CleanupStack::PopAndDestroy( codecs );	
	}	

void TCmdGetStreamInfo::GetAudioStreamInfo( CMceAudioStream* aAudioStream )
	{
	if (aAudioStream != NULL) 
		{
		AddTextResponseL( KResponseStreamType, KValueStreamTypeAudio );

		AddTextualStreamStateL( aAudioStream->IsEnabled(), EFalse );

		AddIntegerResponseL( KResponseLocalMediaPort, aAudioStream->LocalMediaPort() );

		AddSourceToResponseL( KResponseSourceId,
							  KResponseSource,
							  *aAudioStream );

		AddSinksToResponseL( KResponseSinkIds,
							 KResponseSinks,
							 *aAudioStream );

		AddAudioCodecsToResponseL( KResponseAudioCodecs, *aAudioStream );

		// Check boundStream
		if ( aAudioStream->BoundStream() )
			{
			// Add to registry by reference and fetch name
			iContext.Registry().AddObjectL( aAudioStream->BoundStreamL() );	
			TBuf8< KTcMaxObjectName > boundStreamId = 
				iContext.Registry().ObjectNameL( 
					&(aAudioStream->BoundStreamL()) );
			// Name is numeric, but adding is easier this way
			AddTextResponseL( KResponseBoundStream, boundStreamId );
			
			if ( aAudioStream->BoundStreamL().Type() == KMceAudio )
				{				
				CMceAudioStream* boundAudioStream = 
					static_cast<CMceAudioStream*>
						(&(aAudioStream->BoundStreamL()));
		
				AddTextResponseL( KResponseBoundStreamType, 
								  KValueStreamTypeAudio );	

				/*AddIntegerResponseL( KResponseBoundStreamState, 
									 boundAudioStream->State() );*/
		
				AddTextualStreamStateL( boundAudioStream->IsEnabled(), ETrue );

				AddIntegerResponseL( KResponseBoundLocalMediaPort, 
							 		 boundAudioStream->LocalMediaPort() );

				AddSourceToResponseL( KResponseBoundStreamSourceId, 
									  KResponseBoundStreamSource,
									  *boundAudioStream );

				AddSinksToResponseL( KResponseBoundStreamSinkIds,
				                     KResponseBoundStreamSinks, 
									 *boundAudioStream );
				AddAudioCodecsToResponseL( KResponseBoundStreamCodecs,
									  	   *boundAudioStream );
				}
			}
		}

	}
	
void TCmdGetStreamInfo::GetVideoStreamInfo( CMceVideoStream* aVideoStream )
	{
	if (aVideoStream != NULL) 
		{
		AddTextResponseL( KResponseStreamType, KValueStreamTypeVideo );
		
		AddTextualStreamStateL( aVideoStream->IsEnabled(), EFalse );
		
		AddIntegerResponseL( KResponseLocalMediaPort, aVideoStream->LocalMediaPort() );

		AddSourceToResponseL( KResponseSourceId,
							  KResponseSource,
							  *aVideoStream );
		
		AddSinksToResponseL( KResponseSinkIds, KResponseSinks, *aVideoStream );

		AddVideoCodecsToResponseL( KResponseVideoCodecs, *aVideoStream );
		
		// Check boundStream
		
		if ( aVideoStream->BoundStream() )
			{
			// Add to registry by reference and fetch name
			iContext.Registry().AddObjectL( aVideoStream->BoundStreamL() );	
			TBuf8< KTcMaxObjectName > boundStreamId = 
				iContext.Registry().ObjectNameL( 
					&(aVideoStream->BoundStreamL()) );
			// Name is numeric, but adding is easier this way
			AddTextResponseL( KResponseBoundStream, boundStreamId );
			
			if ( aVideoStream->BoundStreamL().Type() == KMceVideo )
				{	
			
				CMceVideoStream* boundVideoStream = 
					static_cast<CMceVideoStream*>
						(&(aVideoStream->BoundStreamL()));
		
				AddTextResponseL( KResponseBoundStreamType, 
								  KValueStreamTypeVideo );	
		
				AddTextualStreamStateL( boundVideoStream->IsEnabled(), ETrue );
		
				AddIntegerResponseL( KResponseBoundLocalMediaPort, 
									 boundVideoStream->LocalMediaPort() );

				AddSourceToResponseL( KResponseBoundStreamSourceId, 
									  KResponseBoundStreamSource,
									  *boundVideoStream );
				
				AddSinksToResponseL( KResponseBoundStreamSinkIds,
									 KResponseBoundStreamSinks, 
									 *boundVideoStream );
	
				AddVideoCodecsToResponseL( KResponseBoundStreamCodecs, 
										   *boundVideoStream );	 
				}
			}
		}
	}

// -----------------------------------------------------------------------------
// TCmdGetStreamInfo::Match
// -----------------------------------------------------------------------------
//	
TBool TCmdGetStreamInfo::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("GetStreamInfo") );
	}

// -----------------------------------------------------------------------------
// TCmdGetStreamInfo::CreateL
// -----------------------------------------------------------------------------
//
TTcCommandBase* TCmdGetStreamInfo::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetStreamInfo( aContext );
	}
