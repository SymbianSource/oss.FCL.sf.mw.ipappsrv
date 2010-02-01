/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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


#include "mcevideostream.h"
#include "mcevideocodec.h"
#include "mcesession.h"
#include "mcemanager.h"
#include "mcemediasource.h"
#include "mcemediasink.h"
#include "mcecamerasource.h"
#include "mcertpsource.h"
#include "mcertpsink.h"
#include "mcedisplaysink.h"
#include "mceserial.h"
#include "mceevents.h"
#include "mceh263codec.h"
#include "mcefilesource.h"
#include "mcecomfilesource.h"
#include "mceclilogs.h"
#include "cleanupresetanddestroy.h"


#define _FLAT_DATA static_cast<CMceComVideoStream*>( iFlatData )




// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Factory method
// -----------------------------------------------------------------------------
//
EXPORT_C CMceVideoStream* CMceVideoStream::NewL()
    {
    CMceVideoStream* self = CMceVideoStream::NewLC();
	CleanupStack::Pop( self );
	return self; 
    }

// -----------------------------------------------------------------------------
// Factory method
// Leaves instance on CleanupStack.
// -----------------------------------------------------------------------------
//
EXPORT_C CMceVideoStream* CMceVideoStream::NewLC()
    {
    CMceVideoStream* self = new (ELeave) CMceVideoStream();
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
    }


// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CMceVideoStream::~CMceVideoStream()
    {
	iCodecs.ResetAndDestroy();
	iSupportedVideoCodecs.ResetAndDestroy();
    }


// -----------------------------------------------------------------------------
// Returns codec.
// Returns the codecs available to the session.
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<CMceVideoCodec>& CMceVideoStream::Codecs()
    {
    return iCodecs;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceVideoStream::AddCodecL( CMceVideoCodec* aCodec )
    {
    __ASSERT_ALWAYS(aCodec, User::Leave(KErrArgument));
    iCodecs.AppendL(aCodec);
    aCodec->Attach( *this );
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceVideoStream::RemoveCodecL( CMceVideoCodec& aCodec )
    {    
    RemoveCodecL( aCodec, Binder() );
    }


// -----------------------------------------------------------------------------
// NOT IMPLEMENTED YET
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceVideoStream::ReplaceCodecsL( RPointerArray<CMceVideoCodec>* /*aCodecs*/ )
    {
    User::Leave( KErrNotSupported );
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<const CMceVideoCodec>& 
        CMceVideoStream::SupportedVideoCodecs() const
    {
	return iSupportedVideoCodecs;
    }


// -----------------------------------------------------------------------------
// CMceVideoStream::InternalizeL
// -----------------------------------------------------------------------------
//
void CMceVideoStream::InternalizeL( MMceComSerializationContext& aSerCtx )
    {
    CMceMediaStream::InternalizeL( aSerCtx );
    
    TMceVideoStreamSerializer<CMceVideoStream> serial( *this );
    serial.InternalizeL( aSerCtx );

    }
    
// -----------------------------------------------------------------------------
// CMceVideoStream::ExternalizeL
// -----------------------------------------------------------------------------
//
void CMceVideoStream::ExternalizeL( MMceComSerializationContext& aSerCtx )
    {
    CMceMediaStream::ExternalizeL( aSerCtx );

    TMceVideoStreamSerializer<CMceVideoStream> serial( *this );
    serial.ExternalizeL( aSerCtx );
    
    }



// -----------------------------------------------------------------------------
// CMceVideoStream::InitializeL
// -----------------------------------------------------------------------------
//
void CMceVideoStream::InitializeL( 
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
// CMceVideoStream::InitializeL
// -----------------------------------------------------------------------------
//
void CMceVideoStream::InitializeL( CMceManager* aManager, CMceSession& aSession )
    {
    MCECLI_DEBUG("CMceVideoStream::InitializeL, Entry");
    
    __ASSERT_ALWAYS( aManager, User::Leave( KErrArgument ) );
    
    CMceMediaStream::InitializeL( aManager, aSession );
    
    if ( iCodecs.Count() == 0 )
        {
        const RPointerArray<const CMceVideoCodec>& supportedCodes = 
                                      aManager->SupportedVideoCodecs();
        for( TInt i = 0; i < supportedCodes.Count(); i++ )
            {
            CMceVideoCodec* codec = supportedCodes[i]->CloneL();
            CleanupStack::PushL( codec );
            AddCodecL( codec );
            CleanupStack::Pop( codec );
            MCECLI_DEBUG_SVALUE("CMceVideoStream::InitializeL, \
Added supported codec", codec->SdpName() );
            
            }
        }
    MCECLI_DEBUG("CMceVideoStream::InitializeL, Exit");
        
    }

    

// -----------------------------------------------------------------------------
// CMceVideoStream::SynchronizeWithFileL
// -----------------------------------------------------------------------------
//
void CMceVideoStream::SynchronizeWithFileL( CMceFileSource& aFile )
    {
    MCECLI_DEBUG("CMceVideoStream::SynchronizeWithFileL, Entry");
    
    const TDesC8& videoCodec = aFile.SupportedVideo();
    
    TBool transcodingRequired( EFalse );

    CMceManager* manager = aFile.Manager();
    
    if ( iCodecs.Count() == 0 && manager )
        {
        const RPointerArray<const CMceVideoCodec>& supportedCodecs =
            manager->SupportedVideoCodecs();
         
        CMceVideoCodec* codec = NULL;
        for( TInt j = 0; j < supportedCodecs.Count() && !codec; j++ )
            {
            if ( videoCodec.CompareF( supportedCodecs[ j ]->SdpName() ) == 0 )
                {
                codec = supportedCodecs[ j ]->CloneL();
                CleanupStack::PushL( codec );
                AddCodecL( codec );
                CleanupStack::Pop( codec );
                MCECLI_DEBUG_SVALUE("CMceVideoStream::SynchronizeWithFileL, \
Added supported codec", codec->SdpName() );
                
                }
            }
        }

    for( TInt i = 0; i < iCodecs.Count(); i++ )
        {
        CMceVideoCodec* codec = iCodecs[i];
        if ( videoCodec.CompareF( codec->SdpName() ) == 0 )
            {
            const TMceFileInfo& info = aFile.FileInfo();
            
            TUint allowedMaxBitrate = 0;
            TUint allowedBitrates = 0;
            
            TInt resolveError = 
                static_cast<CMceComCodec*>( codec->FlatData() )->
                    ResolveAllowedBitrates( info.iVideoBitRate, allowedBitrates );
              
            if ( resolveError )
                {
                allowedBitrates = 0xFF;
                }
                
            codec->SetAllowedBitrates( allowedBitrates );
            codec->SetBitrate( info.iVideoBitRate );
            codec->SetFrameRateL( info.iVideoFrameRate );
            codec->SetResolutionL( info.iVideoFrameSize );
            
            allowedMaxBitrate = codec->MaxBitRate();
            
            if ( resolveError || info.iVideoBitRate > allowedMaxBitrate )
                {
                transcodingRequired = ETrue;
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
        SetState( CMceMediaStream::ETranscodingRequired );
        }
    else
        {
        SetState( CMceMediaStream::EInitialized );
        }
    TState state = State();
    MCECLI_DEBUG_MEDIASTATE("CMceVideoStream::SynchronizeWithFileL, exit state", state );
    }

// -----------------------------------------------------------------------------
// CMceVideoStream::FindCodec
// -----------------------------------------------------------------------------
//
CMceCodec* CMceVideoStream::FindCodec( CMceCodec& aCodec )
    {
    const RPointerArray<CMceCodec>& codecs = 
        reinterpret_cast<const RPointerArray<CMceCodec>&>( Codecs() );
    return CMceMediaStream::FindCodec( codecs, aCodec );
    }
 
// -----------------------------------------------------------------------------
// CMceVideoStream::ReorderCodecsByPreferenceL
// -----------------------------------------------------------------------------
//   
void CMceVideoStream::ReorderCodecsByPreferenceL( TBool aDiscardUnusedCodecs )
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
        RPointerArray<CMceVideoCodec> updated;
        
        MceCleanupResetAndDestroyPushL( updated );
        
        TLinearOrder<CMceVideoCodec> order( CMceVideoCodec::ComparePreferences );
                    
        for( TInt i = iCodecs.Count() -1; i >= 0; i-- )
            {
            CMceVideoCodec* codec = iCodecs[i];
            iCodecs.Remove( i );
            User::LeaveIfError( updated.InsertInOrderAllowRepeats( codec, order ) );
            }
           
        iCodecs.ResetAndDestroy();
         
        for( TInt i = updated.Count() -1; i >= 0; i-- )
            {
            CMceVideoCodec* newcodec = updated[i];
            updated.Remove( i );
            User::LeaveIfError( iCodecs.InsertInOrderAllowRepeats( newcodec, order ) );
            }
       
        CleanupStack::PopAndDestroy();//updated;   
        }      
    }

// -----------------------------------------------------------------------------
// CMceVideoStream::BaseCodecs
// -----------------------------------------------------------------------------
//
const RPointerArray<CMceCodec>& CMceVideoStream::BaseCodecs()
    {
    const RPointerArray<CMceCodec>& codecs = 
        reinterpret_cast<const RPointerArray<CMceCodec>&>( Codecs() );
    return codecs;
    }
    
// -----------------------------------------------------------------------------
// CMceVideoStream::RemoveCodecL
// -----------------------------------------------------------------------------
//
void CMceVideoStream::RemoveCodecL( CMceVideoCodec& aCodec, TBool aBinderOriginated )
    {
    MCECLI_DEBUG_SVALUE( "CMceVideoStream::RemoveCodecL, sdp name:", aCodec.SdpName() )
    
    TBool removed( EFalse );
    for(int i = 0; i < iCodecs.Count() && !removed; i++ )
        {
        if( iCodecs[i] == &aCodec || 
            iCodecs[i]->SdpName().CompareF( aCodec.SdpName() ) == 0 )
            {
            MCECLI_DEBUG( "CMceVideoStream::RemoveCodecL, removing" )
            
            TBool commandBound = aBinderOriginated ? ( Binder() ) : ( !Binder() );
            if ( BoundStream() && commandBound )
                {
                static_cast<CMceVideoStream*>( 
                    iLinkedStream )->RemoveCodecL( aCodec, aBinderOriginated );
                }
                
            delete iCodecs[i];
            iCodecs.Remove( i );
            removed = ETrue;
            }
        }
        
    MCECLI_DEBUG( "CMceVideoStream::RemoveCodecL, Exit" )
    }
        
// -----------------------------------------------------------------------------
// CMceVideoStream::EventReceivedL
// -----------------------------------------------------------------------------
//
TInt CMceVideoStream::EventReceivedL( TMceEvent& aEvent )
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
            CMceVideoCodec* codec = iCodecs[j]; 
            codecStatus = codec->EventReceivedL( aEvent );
            if ( codecStatus == KMceEventUpdate )
                {
                MCECLI_DEBUG("CMceVideoStream::EventReceivedL, \
update codec by replacing old version with new one");
                
                CMceVideoCodec* updateCodec = static_cast<CMceVideoCodec*>( aEvent.Message() );
                iCodecs.Remove( j );
                delete codec;
                updateCodec->InitializeL( *this );
                iCodecs.AppendL( updateCodec );
                updateCodec->Updated();
                codecStatus = KMceEventConsumed;
                }
            j++;                
            }
        status = codecStatus == KMceEventNotOwner ? KMceEventNotConsumed : codecStatus;
        }
        
    return status;
    }


// -----------------------------------------------------------------------------
// CMceVideoStream::IsAllowedCombination
// -----------------------------------------------------------------------------
//    
TBool CMceVideoStream::IsAllowedCombination()
    {
    return ETrue;
    }


// -----------------------------------------------------------------------------
// Default C++ constructor.
// -----------------------------------------------------------------------------
//
CMceVideoStream::CMceVideoStream()
    :CMceMediaStream()
    {
    }


// -----------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// -----------------------------------------------------------------------------
//    
void CMceVideoStream::ConstructL()
    {
    CMceComVideoStream* stream = CMceComVideoStream::NewLC();
    CMceMediaStream::ConstructL( stream );
    CleanupStack::Pop( stream );
    }



 // End of File

    
    
    
    
