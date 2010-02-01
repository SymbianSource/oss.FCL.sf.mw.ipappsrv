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




#include "mceaudiostream.h"
#include "mcemanager.h"
#include "mceaudiocodec.h"
#include "mcecomaudiostream.h"
#include "mceserial.h"
#include "mceevents.h"
#include "mcefilesource.h"
#include "mcecomfilesource.h"
#include "mcertpsource.h"
#include "mceclilogs.h"
#include "mcertpsink.h"
#include "cleanupresetanddestroy.h"

#define _FLAT_DATA static_cast<CMceComAudioStream*>( iFlatData )


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceAudioStream::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceAudioStream* CMceAudioStream::NewL()
    {
    CMceAudioStream* self = NewLC();
    CleanupStack::Pop( self );
    return self; 
    }
   
// -----------------------------------------------------------------------------
// CMceAudioStream::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CMceAudioStream* CMceAudioStream::NewLC()
    {
    CMceAudioStream* self = new (ELeave) CMceAudioStream();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;    
    }

// -----------------------------------------------------------------------------
// CMceAudioStream::~CMceAudioStream
// -----------------------------------------------------------------------------
//
EXPORT_C CMceAudioStream::~CMceAudioStream()
    {
    iCodecs.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CMceAudioStream::Codecs
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<CMceAudioCodec>& CMceAudioStream::Codecs()
    {
    return iCodecs;
    }

// -----------------------------------------------------------------------------
// CMceAudioStream::AddCodecL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceAudioStream::AddCodecL(CMceAudioCodec* aCodec)
    {
    __ASSERT_ALWAYS(aCodec, User::Leave(KErrArgument));
    iCodecs.AppendL(aCodec);
    aCodec->Attach( *this );
    }

// -----------------------------------------------------------------------------
// CMceAudioStream::RemoveCodecL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceAudioStream::RemoveCodecL(CMceAudioCodec& aCodec)
    {
    RemoveCodecL( aCodec, Binder() );
    }

// -----------------------------------------------------------------------------
// CMceAudioStream::ReplaceCodecsL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceAudioStream::ReplaceCodecsL(
    RPointerArray<CMceAudioCodec>* /*aCodecs*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMceAudioStream::CMceAudioStream
// -----------------------------------------------------------------------------
//
CMceAudioStream::CMceAudioStream()
    :CMceMediaStream()
    {
    }


// -----------------------------------------------------------------------------
// CMceAudioStream::ConstructL
// -----------------------------------------------------------------------------
//
void CMceAudioStream::ConstructL()
    {
    CMceComAudioStream* stream = CMceComAudioStream::NewLC();
    CMceMediaStream::ConstructL( stream );
    CleanupStack::Pop( stream );
    }

// -----------------------------------------------------------------------------
// CMceAudioStream::InitializeL
// -----------------------------------------------------------------------------
//
void CMceAudioStream::InitializeL(
    CMceSession& aParent, 
    TBool aDiscardUnusedCodecs )
    {
    CMceMediaStream::InitializeL( aParent, aDiscardUnusedCodecs );
            
    for( TInt i = 0; i < iCodecs.Count(); i++ )
        {
        iCodecs[i]->InitializeL( *this );
        }
    }


// -----------------------------------------------------------------------------
// CMceAudioStream::InitializeL
// -----------------------------------------------------------------------------
//
void CMceAudioStream::InitializeL( CMceManager* aManager, CMceSession& aSession )
    {
    
    __ASSERT_ALWAYS( aManager, User::Leave( KErrArgument ) );
    
    CMceMediaStream::InitializeL( aManager, aSession );
    
    if ( iCodecs.Count() == 0 )
        {
        const RPointerArray<const CMceAudioCodec>& supportedCodes = 
                                      aManager->SupportedAudioCodecs();
                                      
        for( TInt i = 0; i < supportedCodes.Count(); i++ )
            {
            CMceAudioCodec* codec = supportedCodes[i]->CloneL();
            CleanupStack::PushL( codec );
            AddCodecL( codec );
            CleanupStack::Pop( codec );
            MCECLI_DEBUG_SVALUE("CMceAudioStream::InitializeL, \
Added supported codec", codec->SdpName() );
            }
                                      
        }
        
    }
    

// -----------------------------------------------------------------------------
// CMceAudioStream::SynchronizeWithFileL
// -----------------------------------------------------------------------------
//
void CMceAudioStream::SynchronizeWithFileL( CMceFileSource& aFile )
    {
    MCECLI_DEBUG("CMceAudioStream::SynchronizeWithFileL, Entry");
    
    const TDesC8& audioCodec = aFile.SupportedAudio();
    
    TBool transcodingRequired( EFalse );
    
    CMceManager* manager = aFile.Manager();
    
    if ( iCodecs.Count() == 0 && manager )
        {
        const RPointerArray<const CMceAudioCodec>& supportedCodecs =
            manager->SupportedAudioCodecs();
         
        CMceAudioCodec* codec = NULL;
        
        for( TInt j = 0; j < supportedCodecs.Count() && !codec; j++ )
            {
            if ( audioCodec.CompareF( supportedCodecs[ j ]->SdpName() ) == 0 )
                {
                codec = supportedCodecs[ j ]->CloneL();
                CleanupStack::PushL( codec );
                AddCodecL( codec );
                CleanupStack::Pop( codec );
                MCECLI_DEBUG_SVALUE("CMceAudioStream::SynchronizeWithFileL, \
Added supported codec", codec->SdpName() );
                }
            }
        }
      
    for( TInt i = 0; i < iCodecs.Count(); i++ )
        {
        CMceAudioCodec* codec = iCodecs[i];
        if ( audioCodec.CompareF( codec->SdpName() ) == 0 )
            {
            const TMceFileInfo& info = aFile.FileInfo();
            
            TUint allowedBitrates = 0;
            TInt resolveError = 
                 static_cast<CMceComCodec*>( codec->FlatData() )->
                    ResolveAllowedBitrates( info.iAudioBitRate, allowedBitrates );
            
            if ( resolveError )
                {
                allowedBitrates = 0xFF;
                }
                
            codec->SetAllowedBitrates( allowedBitrates );

            TInt bitrateError = codec->SetBitrate( info.iAudioBitRate );
            
            if ( resolveError || bitrateError )
                {
                // Currently too high audio bitrate will not lead to transcoding
                // needed state change
                //transcodingRequired = ETrue;
                }
            }
        else
            {
            delete iCodecs[ i ];
            iCodecs.Remove( i );
            i--;
            }
        }
        
    if ( iCodecs.Count() == 0 || transcodingRequired )
        {
        MCECLI_DEBUG("CMceAudioStream::SynchronizeWithFileL, transcoding required" );
        SetState( CMceMediaStream::ETranscodingRequired );
        }
    else
        {
        SetState( CMceMediaStream::EInitialized );
        }
	TState state = State();
    MCECLI_DEBUG_MEDIASTATE("CMceAudioStream::SynchronizeWithFileL, state", state );
    MCECLI_DEBUG("CMceAudioStream::SynchronizeWithFileL, Exit");
    }

// -----------------------------------------------------------------------------
// CMceAudioStream::FindCodec
// -----------------------------------------------------------------------------
//
CMceCodec* CMceAudioStream::FindCodec( CMceCodec& aCodec )
    {
    const RPointerArray<CMceCodec>& codecs = 
        reinterpret_cast<const RPointerArray<CMceCodec>&>( Codecs() );
    return CMceMediaStream::FindCodec( codecs, aCodec );
    }
    
// -----------------------------------------------------------------------------
// CMceAudioStream::ReorderCodecsByPreferenceL
// -----------------------------------------------------------------------------
//   
void CMceAudioStream::ReorderCodecsByPreferenceL( TBool aDiscardUnusedCodecs )
    {
    // First remove all disabled codecs
    if ( aDiscardUnusedCodecs )
        {
        TInt lastIndex( iCodecs.Count() - 1 );
        for( TInt i = lastIndex; i >= 0; i-- )
            {
            if ( iCodecs[i]->State() == CMceCodec::EDisabled )
                {
                RemoveCodecL( *iCodecs[ i ] );
                }
            }
        }
    
    // Then change order    
    TBool inPreferenceOrder = ETrue; 
    
    for( TInt i = 0; i < iCodecs.Count() - 1 && inPreferenceOrder; i++ )
        {
        if ( iCodecs[i]->Preference() > iCodecs[i+1]->Preference() )
            {
            inPreferenceOrder = EFalse;   
            }
        }
       
    if ( !inPreferenceOrder ) 
        {
        RPointerArray<CMceAudioCodec> updated;
        
        MceCleanupResetAndDestroyPushL( updated );
        
        TLinearOrder<CMceAudioCodec> order( CMceAudioCodec::ComparePreferences );
                    
        for( TInt i = iCodecs.Count() -1; i >= 0; i-- )
            {
            CMceAudioCodec* codec = iCodecs[i];
            iCodecs.Remove( i );
            User::LeaveIfError( updated.InsertInOrderAllowRepeats( codec, order ) );
            }
           
        iCodecs.ResetAndDestroy();
         
        for( TInt i = updated.Count() -1; i >= 0; i-- )
            {
            CMceAudioCodec* newcodec = updated[i];
            updated.Remove( i );
            User::LeaveIfError( iCodecs.InsertInOrderAllowRepeats( newcodec, order ) );
            }
       
        CleanupStack::PopAndDestroy();//updated;   
        }   
    }

// -----------------------------------------------------------------------------
// CMceAudioStream::BaseCodecs
// -----------------------------------------------------------------------------
//
const RPointerArray<CMceCodec>& CMceAudioStream::BaseCodecs()
    {
    const RPointerArray<CMceCodec>& codecs = 
        reinterpret_cast<const RPointerArray<CMceCodec>&>( Codecs() );
    return codecs;
    }
    
// -----------------------------------------------------------------------------
// CMceAudioStream::FindDtmfRelatedComponents
// -----------------------------------------------------------------------------
//
void CMceAudioStream::FindDtmfRelatedComponents( 
    TBool& aRtpSourceExists, 
    TBool& aRtpSinkExists, 
    TBool& aDtmfCodecExists,
    TBool aBinderOriginated )
    {
    if ( !aRtpSourceExists && Source() && Source()->Type() == KMceRTPSource )
        {
        aRtpSourceExists = ETrue;
        }
        
    for ( TInt i = 0; i < Sinks().Count() && !aRtpSinkExists; i++ )
        {
        if ( Sinks()[ i ]->Type() == KMceRTPSink )
            {
            aRtpSinkExists = ETrue;
            }
        }
        
    for ( TInt j = 0; j < Codecs().Count() && !aDtmfCodecExists; j++ )
        {
        if ( Codecs()[ j ]->SdpName() == KMceSDPNameDtmf )
            {
            aDtmfCodecExists = ETrue;
            }
        }
    
    TBool commandBound = aBinderOriginated ? ( Binder() ) : ( !Binder() );
    if ( iLinkedStream && commandBound )
        {
        aBinderOriginated = EFalse;
        static_cast<CMceAudioStream*>( 
            iLinkedStream )->FindDtmfRelatedComponents( aRtpSourceExists, 
                                                        aRtpSinkExists,
                                                        aDtmfCodecExists,
                                                        aBinderOriginated );
        }
    }


// -----------------------------------------------------------------------------
// CMceAudioStream::RemoveCodecL
// -----------------------------------------------------------------------------
//
void CMceAudioStream::RemoveCodecL( CMceAudioCodec& aCodec, TBool aBinderOriginated )
    {
    MCECLI_DEBUG_SVALUE( "CMceAudioStream::RemoveCodecL, sdp name:", aCodec.SdpName() )
    
    TBool removed( EFalse );
    for(int i = 0; i < iCodecs.Count() && !removed; i++ )
        {
        if( iCodecs[i] == &aCodec || 
            iCodecs[i]->SdpName().CompareF( aCodec.SdpName() ) == 0 )
            {
            MCECLI_DEBUG( "CMceAudioStream::RemoveCodecL, removing" )
            
            TBool commandBound = aBinderOriginated ? ( Binder() ) : ( !Binder() );
            if ( BoundStream() && commandBound )
                {
                static_cast<CMceAudioStream*>( 
                    iLinkedStream )->RemoveCodecL( aCodec, aBinderOriginated );
                }
                
            delete iCodecs[i];
            iCodecs.Remove( i );
            removed = ETrue;
            }
        }
        
    MCECLI_DEBUG( "CMceAudioStream::RemoveCodecL, Exit" )
    }
        
// -----------------------------------------------------------------------------
// CMceAudioStream::InternalizeL
// -----------------------------------------------------------------------------
//
void CMceAudioStream::InternalizeL( MMceComSerializationContext& aSerCtx )
    {
    CMceMediaStream::InternalizeL( aSerCtx );
    
    TMceAudioStreamSerializer<CMceAudioStream> serial( *this );
    serial.InternalizeL( aSerCtx );

    }
    
// -----------------------------------------------------------------------------
// CMceAudioStream::ExternalizeL
// -----------------------------------------------------------------------------
//
void CMceAudioStream::ExternalizeL( MMceComSerializationContext& aSerCtx )
    {
    CMceMediaStream::ExternalizeL( aSerCtx );

    TMceAudioStreamSerializer<CMceAudioStream> serial( *this );
    serial.ExternalizeL( aSerCtx );
    
    }




// -----------------------------------------------------------------------------
// CMceAudioStream::EventReceivedL
// -----------------------------------------------------------------------------
//
TInt CMceAudioStream::EventReceivedL( TMceEvent& aEvent )
    {

    TInt status = CMceMediaStream::EventReceivedL( aEvent );
    if ( status != KMceEventNotConsumed )
        {
        return status;
        }

    //try codecs
    if ( aEvent.Id().IsCodecId() )
        {
        TInt codecStatus = status;
        TInt j = 0;
        while ( codecStatus != KMceEventConsumed && j < iCodecs.Count() )
            {
            CMceAudioCodec* codec = iCodecs[j]; 
            codecStatus = codec->EventReceivedL( aEvent );
            if ( codecStatus == KMceEventUpdate )
                {
                MCECLI_DEBUG("CMceAudioStream::EventReceivedL, \
update codec by replacing old version with new one");
                CMceMsgObject<CMceAudioCodec>* updateMsg = 
                    static_cast<CMceMsgObject<CMceAudioCodec>*>( aEvent.Message() );
                CMceAudioCodec* updateCodec = updateMsg->Object();
                CleanupStack::PushL( updateCodec );
                iCodecs.Remove( j );
                delete codec;
                updateCodec->InitializeL( *this );
                iCodecs.AppendL( updateCodec );
                CleanupStack::Pop( updateCodec );
                updateCodec->Updated();
                codecStatus = KMceEventConsumed;
                }
            j++;                
            }
        status = codecStatus == KMceEventNotOwner ? KMceEventNotConsumed : codecStatus;
        }

    return status;
    
    }
