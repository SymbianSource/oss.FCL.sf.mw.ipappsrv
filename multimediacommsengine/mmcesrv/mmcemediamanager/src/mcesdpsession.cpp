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
* Description:    
*
*/




// INCLUDE FILES

#include <e32base.h>
#include <e32std.h>

#include <sdporiginfield.h>
#include <sdpconnectionfield.h>
#include <sdpfmtattributefield.h>
#include <sdpmediafield.h>
#include <sdpattributefield.h>
#include <sdpcodecstringpool.h>
#include <sdpdocument.h>
#include <sdpbandwidthfield.h>
#include <siptoheader.h>

#include "mcemediadefs.h"
#include "mcesdpsession.h"
#include "mcecomsession.h"
#include "mcecommediastream.h"
#include "mcemediastream.h"
#include "mcesdpcodec.h"
#include "mcemediamanager.h"
#include "mcemediadefs.h"
#include "mcesrvlogs.h"
#include "mcemmlogs.h"
#include "mcepreconditions.h"
#include "mcemediaobserver.h"
#include "mcemediastate.h"


_LIT8(KTBCP, "TBCP"); 

// ================= MEMBER FUNCTIONS ==========================================

// -----------------------------------------------------------------------------
// CMceSdpSession::NewL
// -----------------------------------------------------------------------------
//
CMceSdpSession* CMceSdpSession::NewL( 
    CMceComSession& aSession,
    CMceMediaManager& aManager )
    {
    CMceSdpSession* self = NewLC( aSession, aManager );
    CleanupStack::Pop( self ); 
    return self;
    }

// -----------------------------------------------------------------------------
// CMceSdpSession::NewLC
// -----------------------------------------------------------------------------
 CMceSdpSession* CMceSdpSession::NewLC( 
    CMceComSession& aSession,
    CMceMediaManager& aManager )
    {
    CMceSdpSession* self = new ( ELeave ) CMceSdpSession( aSession, aManager );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CMceSdpSession::CMceSdpSession
// -----------------------------------------------------------------------------
//
CMceSdpSession::CMceSdpSession( 
    CMceComSession& aSession,
    CMceMediaManager& aManager )
    :       iManager( aManager ),
            iSessionId( 0 ),
            iSessionVersion( 0 ),
            iSession( &aSession ),
            iStringTable( NULL ),
            iOOldSchool( 0 ),
            iIOldSchool( 0 ),
            iOOldSchoolProceeding( 0 ),
            iOOldSchoolCompleted( 0 ),
            iIsMaster( ETrue ),
            iOldLocalMediaPort( 0 ),
            iStoreRemoteOrigin ( ETrue )
    {
    iSessionId = GetRandomNumber();
    iSessionVersion = GetRandomNumber();
    iIsSignallingRequired = KMceNoSignalling;
    }

// -----------------------------------------------------------------------------
// CMceSdpSession::ConstructL
// -----------------------------------------------------------------------------
void CMceSdpSession::ConstructL()
    {
    SetUserNameL(KMceSdpDefaultUserName);
    SetSessionNameL(KMceSdpDefaultSessionName);

    iStringPool = SdpCodecStringPool::StringPoolL();
    iStringTable = &SdpCodecStringPool::StringTableL(); 
    }

// -----------------------------------------------------------------------------
// CMceSdpSession::~CMceSdpSession
// -----------------------------------------------------------------------------
//   
CMceSdpSession::~CMceSdpSession()
    {
    delete iInitialSession;
    delete iSessionName;
    delete iUserName;
    delete iRemoteOrigin;
    iSdpRemoteMediaFields.ResetAndDestroy();
    if ( MediaSession() )
        {
        MediaSession()->DetachSDPSession();
        }
    if ( Backup() )
        {
        Backup()->DetachSDPSession();
        }
    }

// -----------------------------------------------------------------------------
// CMceSdpSession::Manager
// -----------------------------------------------------------------------------
//
CMceMediaManager& CMceSdpSession::Manager() const
    {
    return iManager;
    }

// -----------------------------------------------------------------------------
// CMceSdpSession::SignallingRequired
// -----------------------------------------------------------------------------
//
TInt& CMceSdpSession::SignallingRequired()
    {
    return iIsSignallingRequired;
    }

// -----------------------------------------------------------------------------
// CMceSdpSession::CreateOfferL
// -----------------------------------------------------------------------------
//
CSdpDocument* CMceSdpSession::CreateOfferL( 
    CMceComSession& aSession,
    CMceSdpSession::TOfferType aType )
    {
    MCEMM_DEBUG("CMceSdpSession::CreateOfferL(), Entry ")
    
    CSdpDocument* sdpDocument = NULL;

    if ( aType == CMceSdpSession::ERefresh )
        {
        User::LeaveIfNull( iSdpDocument );
        EncodeSessionParamsL( aSession, *iSdpDocument, aType );
        if(iOldLocalMediaPort != iSdpDocument->MediaFields()[0]->Port())
            {
            iSdpDocument->MediaFields()[0]->SetPortL( iOldLocalMediaPort );
            }
        return iSdpDocument;
        }
        
    if ( aType == CMceSdpSession::EFirst )
        {
        sdpDocument = CSdpDocument::NewLC();
        }
    else
        {
        sdpDocument = iSdpDocument;
        CleanSessionParams( *iSdpDocument );
        }

    EncodeSessionParamsL( aSession, *sdpDocument, aType );
    
    EncodeClientAttributesL( aSession, *sdpDocument );
    
    //create offer based on receive, receive only or send stream
    RPointerArray <CMceComMediaStream>& streams = aSession.Streams();
    RPointerArray< CSdpMediaField >& mediaLines = sdpDocument->MediaFields();
    CSdpMediaField* mediaLine = NULL;
    CMceSdpCodec* sdpCodec = NULL;
    CMceComMediaStream* mediastream = NULL;

    if ( aSession.iPullModeUpdate && aType == CMceSdpSession::EUpdate ) 
	    {
	    // matching the streams to media lines in Pull mode update case
	    MatchingStreamsToMLinesL( *sdpDocument, aSession );
	    }
    
    for ( TInt index = 0; index < streams.Count(); index++ )
        {
        mediastream = streams[ index ];
        TBool add = MediaLineLC( mediaLine, sdpCodec, mediastream,
                                 mediaLines, streams );
        if ( mediaLine )
        	{
        	
            if (!add)
                {
                mediaLine->SetPortL( mediastream->iLocalMediaPort );
                }
            mediastream = mediastream->OfferStream();//downlink
            sdpCodec->PrepareForEncodeL( *mediastream, *mediaLine );
            sdpCodec->EncodeMediaOfferL( *mediastream, *mediaLine, *sdpDocument );
            if ( add )
                {
                mediaLines.AppendL( mediaLine );
                CleanupStack::Pop( mediaLine );
                mediastream->SdpIndex() = mediaLines.Count() - 1;
                }
                
            }
        }   

    User::LeaveIfError( sdpDocument->IsValid() );
    
    if ( aType == CMceSdpSession::EFirst )
        {
        CleanupStack::Pop( sdpDocument );
        }

    iOldLocalMediaPort = sdpDocument->MediaFields()[0]->Port();
        
    MCEMM_DEBUG("CMceSdpSession::CreateOfferL(), Exit ")
    
    return sdpDocument;
    }

// -----------------------------------------------------------------------------
// CMceSdpSession::MatchingStreamsAndMLinesL
// -----------------------------------------------------------------------------
//
void CMceSdpSession::MatchingStreamsToMLinesL( 
    CSdpDocument& aSdpDocument, 
    CMceComSession& aSession )
    {
    MCEMM_DEBUG("CMceSdpSession::MatchingStreamsToMLinesL(),entry ")

    //create offer based on receive, receive only or send stream
    RPointerArray <CMceComMediaStream>& streams = aSession.Streams();
    RPointerArray< CSdpMediaField >& mediaLines = aSdpDocument.MediaFields();
  
    // set SDPindex for stream if related meida line is found
    for ( TInt index = 0; index < streams.Count(); index++ )
        {
        CMceComMediaStream* mediastream = streams[ index ];
        CMceSdpCodec* sdpCodecStream = NULL;
      
        sdpCodecStream = Manager().SdpCodec( *mediastream );
        if ( sdpCodecStream )
          	{
          	TInt mediaIndex = 0;
            while( mediaIndex < mediaLines.Count() )
              	{
                CSdpMediaField* mediaLine = mediaLines[ mediaIndex ];
                CMceSdpCodec* sdpCodecMediaLine = NULL;
                sdpCodecMediaLine = Manager().SdpCodec( *mediaLine  );
                if ( sdpCodecMediaLine && sdpCodecMediaLine->Media() == sdpCodecStream->Media() )
                    {
                    mediastream->SdpIndex() = mediaIndex;
                    }  
                mediaIndex++;          
                }
          	}
        MCEMM_DEBUG_DVALUE( "CMceSdpSession::MatchingStreamsToMLinesL(), stream index:", index )
        MCEMM_DEBUG_DVALUE( "CMceSdpSession::MatchingStreamsToMLinesL(), stream SdpIndex:", 
            mediastream->SdpIndex() )
        }
      
    // set port to 0 if there are unused media lines
    for ( TInt index = 0; index < mediaLines.Count(); index++ )
        {
        // set old session media lines port to 0
        CSdpMediaField* mediaLine = mediaLines[ index ];
        
        TBool indexFoundFromStream = EFalse;
        TInt streamIndex = 0;
        while( !indexFoundFromStream && streamIndex < streams.Count() )
        	{
        	CMceComMediaStream* mediastream = streams[ streamIndex ];
        	if ( index == mediastream->SdpIndex() ) 
        		{
        		indexFoundFromStream = ETrue;
        		}
        	streamIndex++;
        	}
        if ( !indexFoundFromStream )
        	{
            MCEMM_DEBUG_DVALUE( "CMceSdpSession::MatchingStreamsToMLinesL(), \
    set media line to 0, index:", index )
        	mediaLine->SetPortL( 0 );  
        	}
        }
        
    MCEMM_DEBUG("CMceSdpSession::MatchingStreamsToMLinesL(),exit ")
    }

// -----------------------------------------------------------------------------
// CMceSdpSession::DecodeAnswerL
// -----------------------------------------------------------------------------
//
TInt CMceSdpSession::DecodeAnswerL( 
    CSdpDocument& aSdpDocument, 
    CMceComSession& aSession )                                
    {
    MCEMM_DEBUG("CMceSdpSession::DecodeAnswerL(), Entry ")
    
    iSdpDocument = &aSdpDocument;
    RPointerArray< CSdpMediaField >& mediaLines = iSdpDocument->MediaFields();
    if ( mediaLines.Count() == 0 )
        {
        User::Leave( KErrArgument );
        }
    
    // store the remote o= field
    StoreRemoteOriginL();
    // store the remote a= field
    StoreRemoteMediaFieldsL();

    TInt err = SetRemoteIpAddressL( aSession, aSdpDocument );
    if ( err != KErrNone )
        {
        MCEMM_DEBUG("ERROR: Incompatible network address")
        User::Leave( KErrArgument );
        }

    // handle media lines
    RPointerArray <CMceComMediaStream>& streams = aSession.Streams();
    
    CSdpMediaField* mediaLine = NULL;
    CMceSdpCodec* sdpCodec = NULL;
    CMceComMediaStream* mediastream = NULL;
    TInt index = 0;

	// Go through the answer's media lines. If there is a line for which there
	// is no stream (remote has put wrong port to answer), set port to zero.
	// Do this before the second for-loop, as DecodeMediaAnswerL may leave with
	// KMceErrOldSchool, starting old school hold. In that case any further
	// media lines would not be processed and if they had wrong port (non-zero
	// port but no media stream), we'd use that in our next offer.
	if ( !aSession.iPullModeUpdate )
		{
		for ( index = 0; index < mediaLines.Count(); index++ )
        	{
        	mediaLine = mediaLines[ index ];
			if ( Manager().SdpCodec( *mediaLine ) &&
				 !MediaSlotInUse( index, streams ) )
				{
				mediaLine->SetPortL( 0 );
				}
            }
        }

    for ( index = 0; index < mediaLines.Count(); index++ )
        {
        mediaLine = mediaLines[ index ];
        sdpCodec = Manager().SdpCodec( *mediaLine );

        if ( sdpCodec )
            {
            if ( !aSession.iPullModeUpdate )
                {
                TUint remotePort = mediaLine->Port();            
                aSession.iRemoteIpAddress.SetPort( remotePort );

                mediastream = MediaSlotInUse( index, streams );
                // Check for local Support of media and only decode media
        		// which we support or in use
				if ( mediastream )
					{
	                mediastream = mediastream->AnswerStream();//uplink
	                __ASSERT_ALWAYS( mediastream , User::Leave( KErrArgument ) );
	                
	                mediastream->SetRemoteMediaPort( remotePort );
	                
	                if ( remotePort )
	                    {
	                    TInt warnings = KErrNone;
	                    sdpCodec->PrepareForDecodeL( *mediaLine, mediastream );
	                    warnings = sdpCodec->DecodeMediaAnswerL( *mediaLine,
	                    										 *mediastream,
	                    										 aSdpDocument );
	                    if ( warnings  >= KMceSipBadRequest)
	                    	{
	                    	return warnings;
	                    	}
	                    mediastream->DecodedL( KMceRoleOfferer );
	                    }
	                else //rejected
	                    {
	                    mediastream->SetDirection( SdpCodecStringConstants::EAttributeInactive );
	                    mediastream->SdpIndex() = KErrNotFound;
	                    }
					}
                }
            else
                {
                DecodePullModeUpdateMediaLineL(
                    aSdpDocument, aSession, *mediaLine, index, *sdpCodec );
                }
            }
        else
            {
            MCEMM_DEBUG_SVALUE("No SDP codec. m-line", mediaLine->Media().DesC() )
            }
        }

    //check that all answers to offers were received        
    User::LeaveIfError( MediaSlotInUse( index, streams ) ? KErrArgument : KErrNone );
    
    // set the o= field with the local information
    EncodeSessionParamsL( aSession, *iSdpDocument );
    
    // store session level attributes
    DecodeClientAttributesL( aSdpDocument, aSession );
    
    MCEMM_DEBUG("CMceSdpSession::DecodeAnswerL(), Exit ")
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMceSdpSession::DecodePullModeUpdateMediaLineL
// -----------------------------------------------------------------------------
//
void CMceSdpSession::DecodePullModeUpdateMediaLineL( 
    CSdpDocument& aSdpDocument, 
    CMceComSession& aSession,
    CSdpMediaField& aMediaLine,
    TInt aIndex,
    CMceSdpCodec& aSdpCodec ) 
    {
    MCEMM_DEBUG("CMceSdpSession::DecodePullModeUpdateMediaLineL(), Entry ")

    RPointerArray <CMceComMediaStream>& streams = aSession.Streams();
    CMceComMediaStream* mediastream = NULL;
   
    TUint remotePort = aMediaLine.Port();
       
    aSession.iRemoteIpAddress.SetPort( remotePort );

    // this media line might be from previous session, and it's
    // port is 0, if so mediastream can't be found
    mediastream = MediaSlotInUse( aIndex, streams );
    
    MCEMM_DEBUG_DVALUE( "CMceSdpSession::DecodePullModeUpdateMediaLineL, remotePort:", 
                         remotePort )
    if ( mediastream )
        {
        MCEMM_DEBUG("CMceSdpSession::DecodePullModeUpdateMediaLineL(), stream found ")

        mediastream = mediastream->AnswerStream();//uplink
        __ASSERT_ALWAYS( mediastream , User::Leave( KErrArgument ) );
        mediastream->SetRemoteMediaPort( remotePort ); 
        if ( remotePort )
            {
            aSdpCodec.PrepareForDecodeL( aMediaLine, mediastream );
            aSdpCodec.DecodeMediaAnswerL( aMediaLine, *mediastream, aSdpDocument );
            mediastream->DecodedL( KMceRoleOfferer );
            }
        else
            {
            mediastream->SetDirection( SdpCodecStringConstants::EAttributeInactive );
            mediastream->SdpIndex() = KErrNotFound;
            }
        }
    
    // if streams not found and port in meida line is not 0
    // leave with KErrNotFound       
    User::LeaveIfError( (!mediastream && remotePort) ? KErrNotFound : KErrNone );
    
    MCEMM_DEBUG("CMceSdpSession::DecodePullModeUpdateMediaLineL(), Exit ")
    }
                                             
// -----------------------------------------------------------------------------
// CMceSdpSession::DecodeOfferL
// -----------------------------------------------------------------------------
//
TInt CMceSdpSession::DecodeOfferL( 
    CSdpDocument& aSdpDocument, 
    CMceComSession& aSession )
    {
    MCEMM_DEBUG("CMceSdpSession::DecodeOfferL(), Entry ")
    TInt result = KErrNone;
    
    iSdpDocument = &aSdpDocument;
    
    RPointerArray <CSdpMediaField>& mediaLines = iSdpDocument->MediaFields();
    if ( mediaLines.Count() == 0 )
        {
        MCEMM_DEBUG("CMceSdpSession::DecodeOfferL(), ERROR: No media lines")
        MCEMM_DEBUG("CMceSdpSession::DecodeOfferL(), Exit ")
        return KMceSipWarnIncompatibleMediaFormat;
        }
    
    //avoid to store the value second time when in nat environment
    if ( iStoreRemoteOrigin )
        {
        // store the remote o= field
        StoreRemoteOriginL();
        // store the remote a= field
        StoreRemoteMediaFieldsL();
        }
    //set remote ip address
    result = SetRemoteIpAddressL( aSession, aSdpDocument );
    
    if ( result != KErrNone )
        {
        MCEMM_DEBUG("CMceSdpSession::DecodeOfferL(), ERROR: Incompatible network address")
        MCEMM_DEBUG("CMceSdpSession::DecodeOfferL(), Exit ")
        return result;
        }

    DecodeConnectionFieldL( *iSdpDocument, aSession );
    
    // handle media lines
    RPointerArray<CMceComMediaStream>& streams = aSession.Streams();
    CSdpMediaField* mediaLine = NULL;
    CMceSdpCodec* sdpCodec = NULL;
    CMceComMediaStream* mediastream = NULL;
    
    TInt streamCount( 0 );
    for ( TInt i = 0; i < streams.Count(); i++ )
        {
        if ( streams[ i ]->iStreamType != CMceComMediaStream::ELocalStream )
            {
            streamCount++;
            }
        }
   
   	if ( streamCount > mediaLines.Count() )
   		{
   		return KMceSipNotAcceptableHere;
   		}
   
    TInt index = 0;
   
    while( index < mediaLines.Count() )
        {
        mediaLine = mediaLines[ index ];
        TUint remotePort = mediaLine->Port();
        if ( remotePort )
            {
            sdpCodec = Manager().SdpCodec( *mediaLine );
            if ( sdpCodec )
                {
                TInt warning = KErrNone;
                
                aSession.iRemoteIpAddress.SetPort( remotePort );
                
                mediastream = MediaSlotInUse( index, streams );
                if ( !mediastream )
                    {
                    sdpCodec->PrepareForDecodeL( *mediaLine, mediastream );
                    warning = sdpCodec->DecodeMediaOfferL( *mediaLine, mediastream, 
                                                            aSession, aSdpDocument );
                    }
                else
                    {
                    //update
                    mediastream = mediastream->AnswerStream();//uplink
                    sdpCodec->PrepareForDecodeL( *mediaLine, mediastream );
                    warning = sdpCodec->DecodeMediaUpdateL( *mediaLine, *mediastream, 
                                                            aSdpDocument );
                    }
                if ( warning >= KMceSipBadRequest )
                    {
                    return warning;
                    }
                    
                if ( warning != KErrNone )
                    {
                    //media will be rejected when answer is created
                    if ( mediastream )
                        {
                        TInt streamIndex = aSession.Streams().Find( &(*mediastream)() );
                        __ASSERT_ALWAYS( streamIndex >= 0, User::Leave( KErrArgument ) );
                        MCEMM_DEBUG_DVALUE("removing stream", streamIndex )
                        aSession.Streams().Remove( streamIndex );
                        delete mediastream;
                        }
                    }
                else
                    {
                    __ASSERT_ALWAYS( mediastream, User::Leave( KErrArgument ) );
                    mediastream->DecodedL( KMceRoleAnswerer );
                    mediastream->SdpIndex() = index;
                    }
                }
            else
                {
                MCEMM_DEBUG_SVALUE("No SDP codec. m-line", mediaLine->Media().DesC() )
                }
            }
        index++;
        }

    
    if( streams.Count() > 0 )
        {
        // at least one media line successfully decoded and corresponding 
        // server streams created or inactive media line received
        MCEMM_DEBUG("Incoming OFFER successfully decoded")
        }
    else 
        {
        MCEMM_DEBUG("ERROR: Negotiation failed")
        result = KMceSipWarnMediaTypeNotAvailable;
        }
    
    if ( result == KErrNone )
        {
        // store session level attributes
        DecodeClientAttributesL( aSdpDocument, aSession );
        }
        
    MCEMM_DEBUG_DVALUE("CMceSdpSession::DecodeOfferL(), Exit ", result )
    return result;
    }      
    
// -----------------------------------------------------------------------------
// CMceSdpSession::CreateAnswerL
// -----------------------------------------------------------------------------
//
CSdpDocument& CMceSdpSession::CreateAnswerL( 
    CMceComSession& aSession, 
    TBool aInvalid )
    {       
    MCEMM_DEBUG("CMceSdpSession::CreateAnswerL(), Entry ")
    
    TOfferType type = !Backup() ? EFirst : EUpdate;
    
    CleanSessionParams( *iSdpDocument );

    EncodeSessionParamsL( aSession, *iSdpDocument, type );
    
    EncodeClientAttributesL( aSession, *iSdpDocument );
    
    RPointerArray <CSdpMediaField>& mediaLines = iSdpDocument->MediaFields();
    
    __ASSERT_ALWAYS( mediaLines.Count() > 0, User::Leave( KErrArgument ) );

    RPointerArray<CMceComMediaStream>& streams = aSession.Streams();
    CSdpMediaField* mediaLine = NULL;
    CMceSdpCodec* sdpCodec = NULL;
    CMceComMediaStream* mediastream = NULL;
    TInt index = 0;

    if ( !aInvalid )
        {
        
        for ( index = 0; index < streams.Count(); index++ )
            {
            mediastream = streams[ index ];
            TBool add = MediaLineLC( mediaLine, sdpCodec, mediastream,
                                     mediaLines, streams );

            User::LeaveIfError( add ? KErrNotFound : KErrNone );
                                     
            if ( mediaLine )
                {
                mediastream = mediastream->OfferStream();//downlink
                sdpCodec->PrepareForEncodeL( *mediastream, *mediaLine );
                PrepareForAnswerEncodeL( *mediaLine );
                sdpCodec->EncodeMediaAnswerL( *mediastream, *mediaLine, *iSdpDocument );
                }
            }
        //reject removed streams
        //ignore the Supported File Stream will be handled by other plugin
        
        for ( index = 0; index < mediaLines.Count(); index++ )
            {
            mediaLine = mediaLines[ index ];
            sdpCodec = Manager().SdpCodec( *mediaLine );
                        
            if ( sdpCodec || mediaLine->FormatList() != KTBCP )
                {
                if ( !MediaSlotInUse( index, streams ) )
                    {
                    MCEMM_DEBUG_SVALUE("Rejecting media line", mediaLine->Media().DesC() )
                    mediaLines[ index ]->RejectMedia();
                    }
                }
            }
        MCEMM_DEBUG("Negotiation successful! Answer created ")
        }
    else
        {
        MCEMM_DEBUG("Negotiation failed! Reject all media lines ")
        // negotiation failed, reject all media lines in the offer
        for (index = 0; index < mediaLines.Count(); index++)
            {
            mediaLines[ index ]->RejectMedia();
            }
        }
        
    User::LeaveIfError( iSdpDocument->IsValid() );

    iOldLocalMediaPort = iSdpDocument->MediaFields()[0]->Port();

    MCEMM_DEBUG("CMceSdpSession::CreateAnswerL(), Exit ")
    
    return *iSdpDocument;
    }       

// -----------------------------------------------------------------------------
// CMceSdpSession::PrepareForAnswerEncodeL
// -----------------------------------------------------------------------------
//
void CMceSdpSession::PrepareForAnswerEncodeL( CSdpMediaField& aMediaLine )
    {   
    TLex8 formats( aMediaLine.FormatList() );
    while( !formats.Eos() )
        {
        TPtrC8 format = formats.NextToken();
        aMediaLine.RemoveFormatL( format );
        }
    }

// -----------------------------------------------------------------------------
// CMceSdpSession::CreateMediaLineLC
// -----------------------------------------------------------------------------
//
CSdpMediaField* CMceSdpSession::CreateMediaLineLC( 
    CMceComMediaStream& aStream ) const
    {
    RStringF protocol = SDP_STRING( SdpCodecStringConstants::EProtocolRtpAvp );
        
    CSdpMediaField* medialine = 
            CSdpMediaField::NewLC( Manager().SdpCodec( aStream )->Media(), 
                                   aStream.iLocalMediaPort,
                                   protocol, 
                                   KMceSdpNullFormatList );
    return medialine;
    }    

// -----------------------------------------------------------------------------
// CMceSdpSession::MediaLineLC
// -----------------------------------------------------------------------------
//
TBool CMceSdpSession::MediaLineLC( 
    CSdpMediaField*& aMediaLine,
    CMceSdpCodec*& sdpCodec,
    CMceComMediaStream* aStream,
    RPointerArray<CSdpMediaField>& aMediaLines,
    RPointerArray<CMceComMediaStream>& aStreams )
    {
    
    //filter out local streams
    if ( !aStream || 
         !Manager().SdpCodec( *aStream ) )
        {
        aMediaLine = NULL;
        sdpCodec = NULL;
        return EFalse;
        }
        
    sdpCodec = Manager().SdpCodec( *aStream );
    
    TInt mediaIndex = aStream->SdpIndex();
    
    if ( mediaIndex == KErrNotFound )
        {
        TInt index = 0;
        TBool inUse = ETrue;
        while( inUse && index < aMediaLines.Count() )
            {
            CSdpMediaField* mediaLine = aMediaLines[ index ];
            if ( Manager().SdpCodec( *mediaLine ) )
                {
                inUse = MCE_NOT_NULL_PTR( MediaSlotInUse( index, aStreams ) );
                }
            mediaIndex = !inUse ? index : mediaIndex;
            index++;                
            }
        }

                    
    if ( mediaIndex == KErrNotFound )//all in use
        {
        aMediaLine = CreateMediaLineLC( *aStream );
        }
    else 
        {
        __ASSERT_ALWAYS( aMediaLines.Count() > mediaIndex , User::Leave( KErrArgument ) );
        aMediaLine = aMediaLines[ mediaIndex ];
        }
    
    __ASSERT_ALWAYS( Manager().SdpCodec( *aMediaLine ), User::Leave( KErrArgument ) );
                     
    return mediaIndex == KErrNotFound;  
    }

// -----------------------------------------------------------------------------
// CMceSdpSession::MediaSlotInUse
// -----------------------------------------------------------------------------
//
CMceComMediaStream* CMceSdpSession::MediaSlotInUse( 
    TInt aMediaLine, 
    RPointerArray<CMceComMediaStream>& aStreams )
    {
    CMceComMediaStream* stream = NULL;
    TInt index = 0;
    while( !stream && index < aStreams.Count() )
        {
        stream = aStreams[ index++ ];
        stream = Manager().SdpCodec( *stream ) && 
                 stream->SdpIndex() == aMediaLine ? stream : NULL;
        }
    
    return stream;   
    }

// -----------------------------------------------------------------------------
// CMceSdpSession::CleanSessionParams
// -----------------------------------------------------------------------------
//
void CMceSdpSession::CleanSessionParams( CSdpDocument& aDocument ) const
    {
    MCEMM_DEBUG("CMceSdpSession::CleanSessionParams()")
    aDocument.AttributeFields().ResetAndDestroy();
    aDocument.BandwidthFields().ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CMceSdpSession::EncodeSessionParamsL
// -----------------------------------------------------------------------------
//
void CMceSdpSession::EncodeSessionParamsL( 
    CMceComSession& aSession, 
    CSdpDocument& aDocument, 
    TOfferType aType )
    {
    MCEMM_DEBUG("CMceSdpSession::EncodeSessionParamsL(), Entry ")
     // Set session name: s=
    aDocument.SetSessionNameL( *iSessionName );

    if( aType == EFirst )
        {
        // update the name in o= field only when creating intial offer/answer
        HBufC8* originator = NULL;
        if ( aSession.iType == CMceComSession::EOutSession )
            {
            originator = aSession.iOriginator;
            }
        else
            {
            originator = aSession.iRecipient;
            }
        
        if (originator && originator->Length() > 0)
            {
            CSIPToHeader* to = CSIPToHeader::DecodeL( *originator);
            CleanupStack::PushL(to);
            const TDesC8& username = to->SIPAddress().Uri8().Uri().Extract( EUriUserinfo );
            if ( username.Length() )
                {
                SetUserNameL( username );
                }  
            else
                {
                const TDesC8& uripath = to->SIPAddress().Uri8().Uri().Extract( EUriPath );
                const TDesC8& uri = to->SIPAddress().Uri8().Uri().UriDes();

                if ( uripath.Length() )
                    {
                    SetUserNameL( uripath );
                    }
                else
                    {
                    SetUserNameL( uri );
                    }
                }
			    
            CleanupStack::PopAndDestroy(to);
            }
        }
    else if( aType == EUpdate )
        {
        iSessionVersion++;
        }
        
    CSdpOriginField* originField = 
                CSdpOriginField::NewL( UserName(),
                                      iSessionId,
                                      iSessionVersion,
                                      aSession.iLocalIpAddress );
                                      
    aDocument.SetOriginField( originField );

    EncodeConnectionFieldL( aSession, aDocument );
    
    MCEMM_DEBUG("CMceSdpSession::EncodeSessionParamsL(), Exit ")
    }

// -----------------------------------------------------------------------------
// CMceSdpSession::SetRemoteIpAddressL
// -----------------------------------------------------------------------------
//
TMceSipWarningCode CMceSdpSession::SetRemoteIpAddressL( 
    CMceComSession& aSession,
    CSdpDocument& aSdpDocument )
    {
    TMceSipWarningCode result = KErrNone;
    
    RPointerArray< CSdpMediaField >& mediaLines = aSdpDocument.MediaFields();
        
    // get the remote ip address
    CSdpConnectionField* connfield = aSdpDocument.ConnectionField();
    const TInetAddr* inetAddr = NULL;
    
    // find "c-" line from media level
    TInt index = 0;
    TBool found = ETrue;
    while( found && index < mediaLines.Count() )
        {
        RPointerArray<CSdpConnectionField>& connfields = 
                                            mediaLines[index]->ConnectionFields();
        
        if ( mediaLines[index++]->Port() > 0 )
            {
            TInt cfindex = 0;
	        TBool cffound = EFalse;
	        while( !cffound && cfindex < connfields.Count() )
	            {
	            inetAddr = connfields[cfindex++]->InetAddress();
	            cffound = MCE_NOT_NULL_PTR( inetAddr );
	            }
	        found = cffound;
            }
        }
    
    if( connfield && !found )
        {
		inetAddr = connfield->InetAddress();
		if( inetAddr )
		    {
		    const TInetAddr inetAddress = *(connfield->InetAddress() );
		    // if present, if not then should be media level
		    MCE_SET_REMOTE_IP_ADDR( &aSession, inetAddress );
		    }
        }
        
    if ( inetAddr )
        {
        MCE_SET_REMOTE_IP_ADDR( &aSession, *inetAddr );
        }
    else
        {
        result = KMceSipWarnIncompatibleNetworkAddressFormat;
        }

    return result;
    }
    
// -----------------------------------------------------------------------------
// CMceSdpSession::SetSessionNameL
// -----------------------------------------------------------------------------
//
void CMceSdpSession::SetSessionNameL( const TDesC8& aSessionName )
    {
    HBufC8* tmp = aSessionName.AllocL();
    delete iSessionName;
    iSessionName = tmp;
    }

// -----------------------------------------------------------------------------
// CMceSdpSession::SetMediaSession
// -----------------------------------------------------------------------------
//
void CMceSdpSession::SetMediaSession( CMceComSession* aSession )
    {
    iSession = aSession;
    }  

// -----------------------------------------------------------------------------
// CMceSdpSession::MediaSession
// -----------------------------------------------------------------------------
//
CMceComSession* CMceSdpSession::MediaSession()
    {
    return iSession;
    }  

// -----------------------------------------------------------------------------
// CMceSdpSession::SetBackup
// -----------------------------------------------------------------------------
//
void CMceSdpSession::SetBackup( CMceComSession* aSession )
    {
    iBackup = aSession;
    }  
    
// -----------------------------------------------------------------------------
// CMceSdpSession::Backup
// -----------------------------------------------------------------------------
//
CMceComSession* CMceSdpSession::Backup()
    {
    return iBackup;
    }  

// -----------------------------------------------------------------------------
// CMceSdpSession::DetachMedia
// ----------------------------------------------------------------------------- 
//
TBool CMceSdpSession::DetachMedia( CMceComSession& aSession )
    {
    if ( &aSession == MediaSession() )
        {
        SetMediaSession( NULL );
        }
    else if ( &aSession == Backup() )
        {
        SetBackup( NULL );
        }
    else
        {
        return EFalse;
        }
    return ETrue;
    }
    
// -----------------------------------------------------------------------------
// CMceSdpSession::SetUserNameL
// -----------------------------------------------------------------------------
//
void CMceSdpSession::SetUserNameL( const TDesC8& aUserName )
    {
    HBufC8* tmp = aUserName.AllocL();
    delete iUserName; 
    iUserName = tmp;
    }
    
// -----------------------------------------------------------------------------
// CMceSdpSession::UserName
// -----------------------------------------------------------------------------
//
const TDesC8& CMceSdpSession::UserName() const
    {
    return *iUserName;
    }

// ------------------------------------------------------------------------------
// CMceSdpSession::StoreRemoteOriginL
// ------------------------------------------------------------------------------
//
void CMceSdpSession::StoreRemoteOriginL()
    {
    CSdpOriginField* origin = iSdpDocument->OriginField();
    User::LeaveIfNull( origin );
    CSdpOriginField* tmp = origin->CloneL();
    delete iRemoteOrigin;
    iRemoteOrigin = tmp;    	
    }      

// ------------------------------------------------------------------------------
// CMceSdpSession::StoreRemoteMediaFieldsL
// ------------------------------------------------------------------------------
//
void CMceSdpSession::StoreRemoteMediaFieldsL()
    {
    iSdpRemoteMediaFields.ResetAndDestroy();
    for ( TInt i = 0; i < iSdpDocument->MediaFields().Count(); i++ )
    	{
    	CSdpMediaField* field = iSdpDocument->MediaFields()[i]->CloneL();
    	CleanupStack::PushL( field );
    	iSdpRemoteMediaFields.AppendL( field );
    	CleanupStack::Pop( field );
    	}
    }     

// -----------------------------------------------------------------------------
// CMceSdpSession::DecodeConnectionFieldL
// ----------------------------------------------------------------------------- 
//
void CMceSdpSession::DecodeConnectionFieldL( 
    CSdpDocument& aSdpDocument,
    CMceComSession& aSession )
    {
    CSdpConnectionField* nullAddress = 
                    CSdpConnectionField::DecodeL( KMceSipNullAddressA );
    CleanupStack::PushL( nullAddress );
    
    CMceSdpSession& sdpSession = aSession.SdpSession();
    CSdpConnectionField* connectionField = aSdpDocument.ConnectionField();
    
    if ( connectionField && 
        ( connectionField->Address() == nullAddress->Address() ) )
    	{
    	sdpSession.iIOldSchool = 1;
    	}

	else 
		{
		sdpSession.iIOldSchool = 0;		
		}    	

    CleanupStack::PopAndDestroy( nullAddress );
    }

// -----------------------------------------------------------------------------
// CMceSdpSession::EncodeConnectionFieldL
// ----------------------------------------------------------------------------- 
//
void CMceSdpSession::EncodeConnectionFieldL( 
    CMceComSession& aSession,
    CSdpDocument& aSdpDocument )   
    {   
    RPointerArray <CSdpMediaField>& mediaLines = aSdpDocument.MediaFields();
    TInt mediaLinesIndex = 0;
    TBool found = EFalse;
    TInt mCLineCount = 0;
    while( mediaLinesIndex < mediaLines.Count() )
        {
        RPointerArray<CSdpConnectionField>& connfields = 
            mediaLines[mediaLinesIndex++]->ConnectionFields();
       
        TInt connfieldsIndex = 0;        
        while( connfieldsIndex < connfields.Count() )
            {
            if ( !connfieldsIndex )
                {
                mCLineCount++;
                }

            CSdpConnectionField* connectionField = connfields[connfieldsIndex++];
            
            // if there are media level c= lines, copy the local address
            // to be media c= lines
            if ( iIOldSchool )
                {
                connectionField->SetAddressL( 
                    connectionField->NetType(), 
                    connectionField->AddressType(),
                    KMceSipNullAddress );
                }
            else
                {
                connectionField->SetInetAddressL( aSession.iLocalIpAddress ); 
                }
            }

        // each media line have a media c= line
        if ( mCLineCount == mediaLines.Count() )
            {
            found = ETrue;
            }
        }

    if ( !found || aSdpDocument.ConnectionField() )
        {
        
        // rfc4566 5.7 Connection Data ("c="), media level c= lines are more 
        // important and will override session level c= line if both exists, copy 
        // the local address to be session level c= line when MO provide it also
        CSdpConnectionField* connectionField = NULL;
        if ( iIOldSchool )
            {
            connectionField = CSdpConnectionField::DecodeL( KMceSipNullAddressA );
            }
        else
            {
            connectionField = CSdpConnectionField::NewL( aSession.iLocalIpAddress );
            }
            
        aSdpDocument.SetConnectionField( connectionField );  
        }
    }
        
// -----------------------------------------------------------------------------
// CMceSdpSession::IsSessionRefresh
// -----------------------------------------------------------------------------
//
TBool CMceSdpSession::IsSessionRefresh( CSdpDocument* aSdpDocument )
    {
    TBool isRefresh = EFalse;
    
    if ( aSdpDocument )
        {
        iSdpDocument = aSdpDocument;
        }

    if ( iSdpDocument )
        {
        CSdpOriginField* origin = iSdpDocument->OriginField();
        if( origin && iRemoteOrigin && *iRemoteOrigin == *origin )
            {
            isRefresh = CompareMediaLines( iSdpRemoteMediaFields, 
            								iSdpDocument->MediaFields() );
            }
        }
        
    return isRefresh;
    }    

// -----------------------------------------------------------------------------
// CMceSdpSession::CompareMediaLines
// -----------------------------------------------------------------------------
//
TBool CMceSdpSession::CompareMediaLines( 
    RPointerArray<CSdpMediaField>& aFieldsSrc,
    RPointerArray<CSdpMediaField>& aFieldsDst ) const
    {
    TBool result = EFalse;
    TInt srcCount = aFieldsSrc.Count();
    TInt dstCount = aFieldsDst.Count();
    
    // Return if count of media fields are not equal.
    if ( srcCount == dstCount )
        {
        result = ETrue;
        
        // Return if every media field from source array 
        // cannot be found from destination array.
        for ( TInt i = 0; i < srcCount && result; i++ )
            {
            result = EFalse;
            for ( TInt j = 0; j < dstCount && !result; j++ )
                {
                if ( *aFieldsSrc[i] == *aFieldsDst[j] )
                    {
                    result = ETrue;
                    }
				}
            }   
        }
    
    return result;
    }
        
// -----------------------------------------------------------------------------
// CMceSdpSession::PrepareSessionRefreshL
// -----------------------------------------------------------------------------
//
void CMceSdpSession::PrepareSessionRefreshL( CMceComSession& aSession )
    {
    User::LeaveIfNull( iSdpDocument );
    EncodeSessionParamsL( aSession, *iSdpDocument );
    }

// -----------------------------------------------------------------------------
// CMceSdpSession::SdpCleanup
// -----------------------------------------------------------------------------
//
void CMceSdpSession::SdpCleanup(
    CSdpDocument* aPrevious, 
    CSdpDocument* aReplacement )
    {
    if ( iSdpDocument && iSdpDocument == aPrevious )
        {
        MCEMM_DEBUG("CMceSdpSession::SdpCleanup(), replacing")
        iSdpDocument = aReplacement;
        }
    }
    
// -----------------------------------------------------------------------------
// CMceSdpSession::GetRandomNumber
// -----------------------------------------------------------------------------
//
TInt64 CMceSdpSession::GetRandomNumber()
    {
    TTime currentTime;
    currentTime.HomeTime();
    TInt64 value( currentTime.Int64() );
    return value;
    }  
    
// -----------------------------------------------------------------------------
// CMceSdpSession::NeedToNegotiate
// -----------------------------------------------------------------------------
//
TBool CMceSdpSession::NeedToNegotiate( CMceComSession& aSession )
    {
    TBool needToNegotiate = EFalse;
    
    RPointerArray<CMceComMediaStream>& streams = aSession.Streams();
    
    TInt index = 0;
    while( !needToNegotiate && index < streams.Count() )
        {
        CMceComMediaStream* stream = streams[ index++ ];
        for( int i = 0; i < stream->Preconditions().Count(); i++ )
        	{
        	needToNegotiate = stream->Preconditions().Count() && 
                          !stream->Preconditions()[i]->IsMet();
        	}
        }
        
    return needToNegotiate; 
    }

// -----------------------------------------------------------------------------
// CMceSdpSession::ReservationNeeded
// -----------------------------------------------------------------------------
//  
TBool CMceSdpSession::ReservationNeeded( CMceComSession& aSession )
    {
    TBool reservationNeeded = EFalse;
    
    RPointerArray<CMceComMediaStream>& streams = aSession.Streams();
    
    TInt index = 0;
    while( !reservationNeeded && index < streams.Count() )
        {
        CMceComMediaStream* stream = streams[ index++ ];
        for( int i = 0; i < stream->Preconditions().Count(); i++ )
        	{
        	reservationNeeded = ( stream->Preconditions().Count() && 
                              stream->Preconditions()[i]->IsMet() ) ? EFalse : ETrue;
        	}
        }
        
    return reservationNeeded;
    }

// -----------------------------------------------------------------------------
// CMceSdpSession::ReserveL
// -----------------------------------------------------------------------------
// 
TMceReturnStatus CMceSdpSession::ReserveL( CMceComSession& aSession )
    {
    //fake implementation    
    TMceReturnStatus status = KMceReady;
    
    RPointerArray<CMceComMediaStream>& streams = aSession.Streams();
    
    TInt index = 0;
    while( index < streams.Count() )
        {
        CMceComMediaStream* stream = streams[ index++ ];
        if ( stream->Preconditions().Count() )
            {
            for( int i = 0; i < stream->Preconditions().Count(); i++ )
        		{
            	stream->Preconditions()[ i ]->Reserved();
        		}
            }
        }
        
    return status;
    }

// -----------------------------------------------------------------------------
// CMceSdpSession::EncodeClientAttributesL
// -----------------------------------------------------------------------------
//
void CMceSdpSession::EncodeClientAttributesL( 
    CMceComSession& aSession,
    CSdpDocument& aDocument ) const
    {
    MCEMM_DEBUG("CMceSdpSession::EncodeClientAttributesL, Entry ")
    CDesC8Array* clientAttributes = aSession.iLocalSessionSDPLines;
    __ASSERT_ALWAYS( clientAttributes, User::Leave( KErrArgument ) );
    
    CMceSdpCodec::EncodeClientSdpFieldsL<CSdpDocument>( *clientAttributes, aDocument );
        
    MCEMM_DEBUG("CMceSdpSession::EncodeClientAttributesL, Exit ")
    }    

// -----------------------------------------------------------------------------
// CMceSdpSession::DecodeClientAttributesL
// -----------------------------------------------------------------------------
//
void CMceSdpSession::DecodeClientAttributesL( 
    CSdpDocument& aDocument, 
    CMceComSession& aSession ) const
    {
    MCEMM_DEBUG("CMceSdpSession::DecodeClientAttributesL, Entry ")
    CDesC8Array* remoteAttributes = 
        new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( remoteAttributes );
    
    CBufFlat* encodeBuf = NULL;
    
    for ( TInt i = 0; i < aDocument.AttributeFields().Count(); i++ )
        {
        
    	encodeBuf = CBufFlat::NewL( KMceExternalizeBufferExpandSize );
    	CleanupStack::PushL( encodeBuf );
    	RBufWriteStream writeStream( *encodeBuf, 0 );
	    writeStream.PushL();
        
        aDocument.AttributeFields()[ i ]->EncodeL( writeStream );
        MCEMM_DEBUG_SVALUE("found attribute", encodeBuf->Ptr( 0 ) )
        remoteAttributes->AppendL( encodeBuf->Ptr( 0 ) );
    	
    	CleanupStack::PopAndDestroy(); // writeStream
        CleanupStack::PopAndDestroy( encodeBuf ); // encodeBuf
        }

    for ( TInt i = 0; i < aDocument.BandwidthFields().Count(); i++ )
        {
        
    	encodeBuf = CBufFlat::NewL( KMceExternalizeBufferExpandSize );
    	CleanupStack::PushL( encodeBuf );
    	RBufWriteStream writeStream( *encodeBuf, 0 );
	    writeStream.PushL();
        
        aDocument.BandwidthFields()[ i ]->EncodeL( writeStream );
        MCEMM_DEBUG_SVALUE("found attribute", encodeBuf->Ptr( 0 ) )
        remoteAttributes->AppendL( encodeBuf->Ptr( 0 ) );
    	
    	CleanupStack::PopAndDestroy(); // writeStream
        CleanupStack::PopAndDestroy( encodeBuf ); // encodeBuf
        }
    
    CleanupStack::Pop( remoteAttributes );
    MCE_DELETE( aSession.iRemoteSessionSDPLines );
    aSession.iRemoteSessionSDPLines = remoteAttributes;
    MCEMM_DEBUG("CMceSdpSession::DecodeClientAttributesL, Exit ")
    }

// -----------------------------------------------------------------------------
// CMceSdpSession::Consumes
// -----------------------------------------------------------------------------
//
CMceComSession* CMceSdpSession::Consumes( TUint32 aSessionId )
    {
    CMceComSession* session = MediaSession();
    CMceComSession* backup = Backup();
    CMceComSession* foundSession = NULL;

    foundSession = ( session && 
                     session->iMccID != KMceNotAssigned &&
                     session->iMccID == aSessionId ) ? 
                        session :
                      ( backup &&
                        backup->iMccID != KMceNotAssigned &&
                        backup->iMccID == aSessionId ) ? 
                            backup :
                            NULL;
    
    return foundSession;
    }

// -----------------------------------------------------------------------------
// CMceSdpSession::StockMediaSessionL
// -----------------------------------------------------------------------------
//
void CMceSdpSession::StockMediaSessionL()
    {
    MCEMM_DEBUG("CMceSdpSession::StockMediaSessionL, Entry ")
    
    if ( !iInitialSession && !Backup() )
        {
        iInitialSession = MediaSession()->CloneL();
        MCEMM_DEBUG("Stocked")
        }
        
    MCEMM_DEBUG("CMceSdpSession::StockMediaSessionL, Exit ")
    }


// -----------------------------------------------------------------------------
// CMceSdpSession::ForkL
// -----------------------------------------------------------------------------
//
CMceSdpSession& CMceSdpSession::ForkL()
    {
    MCEMM_DEBUG("CMceSdpSession::ForkL, Entry ")
    
    __ASSERT_ALWAYS( MediaSession(), User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( IsMaster(), User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( !Backup(), User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iInitialSession, User::Leave( KErrArgument ) );

    CMceComSession* forkedMediaSession = 
        iInitialSession->CloneAndMergeLC( *MediaSession() );
    CMceSdpSession& forkedSession = Manager().CreateSessionL( *forkedMediaSession );

    ContextSwitch( &forkedSession );
    
    MediaSession()->MediaObserver().SessionCreatedL( forkedMediaSession );
    CleanupStack::Pop( forkedMediaSession );
        
    MCEMM_DEBUG("CMceSdpSession::ForkL, Exit ")
    
    return forkedSession;   
    }

// -----------------------------------------------------------------------------
// CMceSdpSession::IsMaster
// -----------------------------------------------------------------------------
//
TBool CMceSdpSession::IsMaster()
    {
    return iIsMaster;
    }

// -----------------------------------------------------------------------------
// CMceSdpSession::ContextSwitch
// -----------------------------------------------------------------------------
//
void CMceSdpSession::ContextSwitch( CMceSdpSession* aForkedSession )
    {
    MCEMM_DEBUG("CMceSdpSession::ContextSwitch, Entry ")
    
    CMceSdpSession* oldMaster = this;
    CMceSdpSession* newMaster = aForkedSession;
    
    if ( !newMaster && !oldMaster->IsMaster() )
        {
        newMaster = this;
        oldMaster = NULL;
        CMceSdpSession::TIterator 
                masterCandidates( Manager().iMceSdpSessions, 
                                  CMceSdpSession::TIterator::EFilterMaster );

        MCE_ITERATOR_FIND_NEXT( masterCandidates, oldMaster,
                                oldMaster->MediaSession()->iMccID ==
                                newMaster->MediaSession()->iMccID );
                                
        }

    if ( oldMaster && newMaster )
        {
        newMaster->iIsMaster = ETrue;
        oldMaster->iIsMaster = EFalse;
        delete newMaster->iInitialSession;
        newMaster->iInitialSession = oldMaster->iInitialSession;
        oldMaster->iInitialSession = NULL;
        newMaster->iContext = oldMaster->iRequestedContext ? 
                              oldMaster->iRequestedContext :
                              newMaster->iContext;
        newMaster->iRequestedContext = NULL;
        oldMaster->iRequestedContext = NULL;
        if( aForkedSession )
        	{
        newMaster->MediaSession()->Merge( *oldMaster->MediaSession(), 
                                          KMceDeepMergeYes );
        	}
        
        MCEMM_DEBUG("context switch performed")
        }
    else
        {
        MCEMM_DEBUG("no context switch performed!!")
        }

    MCEMM_DEBUG("CMceSdpSession::ContextSwitch, Exit ")
    }

// -----------------------------------------------------------------------------
// CMceSdpSession::ContextSwitchRequested
// -----------------------------------------------------------------------------
//
TBool CMceSdpSession::ContextSwitchRequested()
    {
    return iContext && iRequestedContext && iRequestedContext != iContext;
    }

// -----------------------------------------------------------------------------
// CMceSdpSession::Context
// -----------------------------------------------------------------------------
//
CSIPDialogAssocBase* CMceSdpSession::Context()
    {
    return iContext;
    }

// -----------------------------------------------------------------------------
// CMceSdpSession::AttachContext
// -----------------------------------------------------------------------------
//
void CMceSdpSession::AttachContext( CSIPDialogAssocBase* aContext )
    {
    if ( !iContext || !aContext )
        {
        iContext = aContext;
        }
    else if ( aContext != iContext )
        {
        iRequestedContext = aContext;
        }
    else
        {
        iRequestedContext = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CMceSdpSession::TIterator::TIterator
// -----------------------------------------------------------------------------
//
CMceSdpSession::TIterator::TIterator( 
    RPointerArray<CMceSdpSession>& aCodecs, 
    CMceSdpSession::TIterator::TFilter aFilter )
    : iFilter( aFilter ),
      iSessions( aCodecs ),
      iCurrentIndex( 0 )
    {
    }

// -----------------------------------------------------------------------------
// CMceSdpSession::TIterator::IsEof
// -----------------------------------------------------------------------------
//
TBool CMceSdpSession::TIterator::IsEof()
    {
    return iSessions.Count() == 0 || iCurrentIndex >= iSessions.Count();
    }          

// -----------------------------------------------------------------------------
// CMceSdpSession::TIterator::Next
// -----------------------------------------------------------------------------
//
TBool CMceSdpSession::TIterator::Next( CMceSdpSession*& aCandidate )
    {
    CMceSdpSession* next = NULL;
    
    while( !next && !IsEof() )
        {
        CMceSdpSession* session = iSessions[ iCurrentIndex ];
        TBool condition = EFalse;
        switch( iFilter )
            {
            case EFilterMaster:
                {
                condition = session->IsMaster();
                break;
                }
            default:
                {
                condition = ETrue;
                break;
                }
            }

        next = condition ? session : NULL;
               
        iCurrentIndex++;
        }

    aCandidate = next;
    return aCandidate ? ETrue : EFalse;  
    }
    
// -----------------------------------------------------------------------------
// CMceSdpSession::Current
// -----------------------------------------------------------------------------
//
TInt CMceSdpSession::TIterator::Current()
    {
    TInt current = KErrNotFound;
    
    if ( iSessions.Count() > 0 )
        {
        current = iCurrentIndex-1;
        current = current < 0 ? 0 : current;
        }
    return current;
    }
    
// -----------------------------------------------------------------------------
// CMceSdpSession::Remove
// -----------------------------------------------------------------------------
//
TInt CMceSdpSession::TIterator::Remove()
    {  
    TInt current = Current();
    
    if ( current != KErrNotFound )
        {
        iSessions.Remove( current );
        iCurrentIndex = current;
        }
    
    return current;
    }
    
// -----------------------------------------------------------------------------
// CMceSdpSession::TIterator::Reset
// -----------------------------------------------------------------------------
//
void CMceSdpSession::TIterator::Reset()
    {
    iCurrentIndex = 0;
    }
            
// -----------------------------------------------------------------------------
// CMceSdpSession::TIterator::SetFilter
// -----------------------------------------------------------------------------
//
void CMceSdpSession::TIterator::SetFilter( 
    CMceSdpSession::TIterator::TFilter aFilter )
    {
    iFilter = aFilter;
    }
            
// -----------------------------------------------------------------------------
// CMceSdpSession::TIterator::Count
// -----------------------------------------------------------------------------
//
TInt CMceSdpSession::TIterator::Count()
    {
    TInt count = iSessions.Count();
    if ( iFilter )
        {
        TInt currentIndex = iCurrentIndex;
        Reset();
        CMceSdpSession* codec = NULL;
        count = 0;
        while( Next( codec ) )
            {
            count++;
            }
        iCurrentIndex = currentIndex;
        }

    return count;
    }
void CMceSdpSession::UpdateSecureStreamL( CMceComSession& aSession )
	{
	MCEMM_DEBUG("CMceSdpSession::UpdateSecureStream() : Entry ")
	RPointerArray< CSdpMediaField >& mediaLines = iSdpRemoteMediaFields;
	RPointerArray <CMceComMediaStream>& streams = aSession.Streams();
	CSdpMediaField* mediaLine = NULL;
	CMceComMediaStream* aStream = NULL;
	for ( TInt index = 0; index < mediaLines.Count(); index++ )
        {
        mediaLine = mediaLines[ index ];
        aStream = MediaSlotInUse( index, streams );
        if( aSession.SecureSession() && aStream )
        	{
        	aStream = aStream->OfferStream();
        	aSession.SecureSession()->ForceUpdateSecureStreamL(*aStream, *mediaLine );
        	} 
        }
	MCEMM_DEBUG("CMceSdpSession::UpdateSecureStream() : Exit ")
	}
