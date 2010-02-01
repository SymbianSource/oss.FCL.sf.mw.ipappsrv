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




#include "mcevideostream.h"
#include "mcecomvideostream.h"
#include "mcecomvideocodec.h"
#include "mcecommediasource.h"
#include "mcecommediasink.h"
#include "mceserial.h"
#include "mceevents.h"
#include "cleanupresetanddestroy.h"


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceComVideoStream::NewL
// -----------------------------------------------------------------------------
//
CMceComVideoStream* CMceComVideoStream::NewL()
    {
    CMceComVideoStream* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComVideoStream::NewLC
// -----------------------------------------------------------------------------
//
CMceComVideoStream* CMceComVideoStream::NewLC()
    {
    CMceComVideoStream* self = new (ELeave) CMceComVideoStream();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
// CMceComVideoStream::~CMceComVideoStream
// -----------------------------------------------------------------------------
//
CMceComVideoStream::~CMceComVideoStream()
    {
    iCodecs.ResetAndDestroy();
    }


// -----------------------------------------------------------------------------
// CMceComVideoStream::CMceComVideoStream
// -----------------------------------------------------------------------------
//
CMceComVideoStream::CMceComVideoStream()
    :CMceComMediaStream( KMceVideo )
    {
    }


// -----------------------------------------------------------------------------
// CMceComVideoStream::AddCodecL
// -----------------------------------------------------------------------------
//
void CMceComVideoStream::AddCodecL( CMceComCodec* aCodec )
    {
    __ASSERT_ALWAYS( aCodec, User::Leave( KErrArgument ) );
    
    CMceComVideoCodec* codec = static_cast< CMceComVideoCodec* >( aCodec );
    iCodecs.AppendL( codec );
    }


// -----------------------------------------------------------------------------
// CMceComVideoStream::RemoveCodecL
// -----------------------------------------------------------------------------
//
void CMceComVideoStream::RemoveCodecL( CMceComCodec* aCodec )
    {
    TInt index = FindCodecFromList( aCodec );
    
    if ( index >= 0 )
    	{
    	RemoveCodecFromListL( index );
    	}
    
    delete aCodec;
    }


// -----------------------------------------------------------------------------
// CMceComVideoStream::CodecL
// -----------------------------------------------------------------------------
//
CMceComVideoCodec* CMceComVideoStream::CodecL( TInt aIndex ) const
	{
	__ASSERT_ALWAYS( aIndex >= 0 && aIndex < iCodecs.Count(),
		User::Leave( KErrArgument ) );

	return iCodecs[aIndex];
	}


// -----------------------------------------------------------------------------
// CMceComVideoStream::CodecCount
// -----------------------------------------------------------------------------
//
TInt CMceComVideoStream::CodecCount() const
	{
	return iCodecs.Count();
	}
	
// -----------------------------------------------------------------------------
// CMceComVideoStream::Codecs
// -----------------------------------------------------------------------------
//
const RPointerArray<CMceComVideoCodec>& CMceComVideoStream::Codecs() const
    {
    return iCodecs;
    }


// -----------------------------------------------------------------------------
// CMceComVideoStream::FindCodecL
// -----------------------------------------------------------------------------
//
CMceComCodec* CMceComVideoStream::FindCodecL( CMceComCodec& aCodec )
    {
    CMceComCodec* foundCodec = NULL;
    CMceComVideoCodec* codec = static_cast< CMceComVideoCodec* >( &aCodec );
    
    TInt index = FindCodecFromList( codec );
    
    if ( index == KErrNotFound )
        {
        const TInt codecCount = CodecCount();
        
        for ( TInt i = 0; i < codecCount && !foundCodec; ++i )
            {
            // Video codec may be identified also based on fmtp
            if ( CodecL( i )->Decodes( aCodec.iSdpName, aCodec.FmtpAttr() ) )
                {
                foundCodec = CodecL( i );
                }     
            }
        }
    else
        {
        foundCodec = CodecL( index );
        }
        
    return foundCodec;
    }


// -----------------------------------------------------------------------------
// CMceComVideoStream::FindCodecFromList
// -----------------------------------------------------------------------------
//
TInt CMceComVideoStream::FindCodecFromList( CMceComCodec* aCodec ) const
	{
	CMceComVideoCodec* codec = static_cast< CMceComVideoCodec* >( aCodec );
	return iCodecs.Find( codec );
	}


// -----------------------------------------------------------------------------
// CMceComVideoStream::RemoveCodecFromListL
// -----------------------------------------------------------------------------
//
void CMceComVideoStream::RemoveCodecFromListL( TInt aIndex )
	{
	__ASSERT_ALWAYS( aIndex >= 0 && aIndex < iCodecs.Count(),
		User::Leave( KErrArgument ) );

	iCodecs.Remove( aIndex );
	}


// -----------------------------------------------------------------------------
// CMceComVideoStream::DestroyCodecs
// -----------------------------------------------------------------------------
//
void CMceComVideoStream::DestroyCodecs()
	{
	iCodecs.ResetAndDestroy();
	}
	

// -----------------------------------------------------------------------------
// CMceComVideoStream::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComVideoStream::InternalizeFlatL( RReadStream& aReadStream )
    {
    CMceComMediaStream::InternalizeFlatL( aReadStream );
    }


// -----------------------------------------------------------------------------
// CMceComVideoStream::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComVideoStream::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    CMceComMediaStream::ExternalizeFlatL( aWriteStream );
    }

  
// -----------------------------------------------------------------------------
// CMceComVideoStream::InternalizeL
// -----------------------------------------------------------------------------
//
void CMceComVideoStream::InternalizeL( MMceComSerializationContext& aSerCtx )
    {
    CMceComMediaStream::InternalizeL( aSerCtx );
    
    TMceVideoStreamSerializer<CMceComVideoStream> serial( *this );
    serial.InternalizeL( aSerCtx );
        
    }
    
// -----------------------------------------------------------------------------
// CMceComVideoStream::ExternalizeL
// -----------------------------------------------------------------------------
//
void CMceComVideoStream::ExternalizeL( MMceComSerializationContext& aSerCtx )
    {
    
    CMceComMediaStream::ExternalizeL( aSerCtx );
    
    TMceVideoStreamSerializer<CMceComVideoStream> serial( *this );
    serial.ExternalizeL( aSerCtx );
    
    }

// -----------------------------------------------------------------------------
// CMceComVideoStream::CloneL
// -----------------------------------------------------------------------------
//
CMceComMediaStream* CMceComVideoStream::CloneL()
    {
    CMceComVideoStream* copy = new (ELeave) CMceComVideoStream();
    CleanupStack::PushL( copy );
    copy->ConstructL( *this );
    CleanupStack::Pop( copy );
    return copy;
    }

// -----------------------------------------------------------------------------
// CMceComVideoStream::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComVideoStream::ConstructL()
    {
    
    CMceComMediaStream::ConstructL();
    
    }


// -----------------------------------------------------------------------------
// CMceComVideoStream::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComVideoStream::ConstructL( CMceComVideoStream& aStream )
    {
    CMceComMediaStream::ConstructL( aStream );
    
    const TInt codecCount = aStream.CodecCount();
    
    for ( TInt i = 0; i < codecCount ; ++i )
        {
        CMceComVideoCodec* codec = aStream.CodecL( i )->CloneL();
        CleanupStack::PushL( codec );
        AddCodecL( codec );
        CleanupStack::Pop( codec );
        }
    
    }


// -----------------------------------------------------------------------------
// CMceComVideoStream::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComVideoStream::InitializeL( CMceComSession& aParent )
    {
    CMceComMediaStream::InitializeL( aParent );

	const TInt codecCount = CodecCount();
	
    for ( TInt i = 0; i < codecCount ; ++i )
        {
        CodecL( i )->InitializeL( *this );
        }
    }




// -----------------------------------------------------------------------------
// CMceComVideoStream::UpdateL
// -----------------------------------------------------------------------------
//
void CMceComVideoStream::UpdateL( CMceComMediaStream& aStream )
    {
    CMceComMediaStream::UpdateL( aStream );

    CMceComVideoStream* stream = static_cast< CMceComVideoStream* >( &aStream );

    RPointerArray<CMceComVideoCodec> updated;
    
    MceCleanupResetAndDestroyPushL( updated );
    
    for( TInt i = 0; i < stream->CodecCount() ; ++i )
        {
        CMceComVideoCodec* update = stream->CodecL( i );
        TBool isUpdated = EFalse;
        TInt j = 0;
        
        while( !isUpdated && j < CodecCount() )
            {
            CMceComVideoCodec* codec = CodecL( j );
            
            if ( codec->Id() == update->Id() )
                {
                codec->UpdateL( *update );
                stream->RemoveCodecFromListL( i );
                delete update;
                updated.AppendL( codec );
                RemoveCodecFromListL( j );
                i--;
                isUpdated = ETrue;
                }
            j++;                
            }
        }
        
    if ( CodecCount() > 0 )	// Codecs have been removed
        {
        Session()->IsStructureChanged() = ETrue;
        }
    
    DestroyCodecs();
    
    while( updated.Count() > 0 )
        {
        AddCodecL( updated[0] );
        updated.Remove( 0 );
        }
        
    CleanupStack::PopAndDestroy();	// updated
            
    if ( stream->CodecCount() > 0 )	// Streams have been added
        {
        Session()->IsStructureChanged() = ETrue;
        
        while( stream->CodecCount() > 0 )
            {
            CMceComVideoCodec* add = stream->CodecL( 0 );
            AddCodecL( add );
            stream->RemoveCodecFromListL( 0 );
            }
        }
    
    }


// -----------------------------------------------------------------------------
// CMceComVideoStream::ReorderCodecs
// -----------------------------------------------------------------------------
//
void CMceComVideoStream::ReorderCodecs()
    {
    TLinearOrder<CMceComVideoCodec> order( CMceComVideoCodec::CompareSdpIndex );
    
    iCodecs.Sort( order );
    }


#ifdef MCE_COMMON_SERVER_SIDE

// -----------------------------------------------------------------------------
// CMceComVideoStream::SynchronizeL
// -----------------------------------------------------------------------------
//
void CMceComVideoStream::SynchronizeL( TBool aRoleAnswerer )
    {
    TInt codecIndex( 0 );
    
    while ( codecIndex < CodecCount() )
        {
        CMceComCodec& codec = *CodecL( codecIndex );
        
        TBool remove( !codec.iIsNegotiated && aRoleAnswerer );
        
        if ( BoundStream() )
            {
            CMceComCodec* sync = BoundStreamL().FindCodecL( codec );
            
            if ( remove )
                {
                BoundStreamL().RemoveCodecL( sync );
                }
            else if ( sync )
                {
                sync->SynchronizeL( codec, aRoleAnswerer );
                }
            else if ( aRoleAnswerer )
                {
                MCE_DEFINE_VIDEO_CODEC( videoCodec, codec );
                CMceComVideoCodec* copy = videoCodec.CloneL();
                CleanupStack::PushL( copy );
                copy->InitializeL( BoundStreamL() );
                MCE_VIDEO( BoundStreamL() ).AddCodecL( copy );
                CleanupStack::Pop( copy );
                copy->SynchronizeL( codec, aRoleAnswerer );
                }
            else
                {
                //NOP
                }
            }
            
        if ( remove )
            {
            RemoveCodecL( &codec );
            }
        else
            {
            codecIndex++;
            }
        }
    }

// -----------------------------------------------------------------------------
// CMceComAudioStream::UpdateDefaultCodecL
// -----------------------------------------------------------------------------
//
void CMceComVideoStream::UpdateDefaultCodecL()
    {
    CMceComCodec* highestPrioritySendCodec = NULL;
    CMceComCodec* codec = NULL;
    
    const RPointerArray< CMceComCodec >& allCodecs =
    	reinterpret_cast< const RPointerArray< CMceComCodec >& >( Codecs() );
    
    CMceComCodec::TIterator codecs( allCodecs, 
                                    CMceComCodec::TIterator::EFilterIsNegotiated );
    
    MCE_ITERATOR_FIND_NEXT( codecs, highestPrioritySendCodec, 
                            highestPrioritySendCodec->SendSupported() );
    
    // If no negotiated send codecs, disable all.
    // If send codec changed, enable new one and disable other send codecs.                                     
    if ( !highestPrioritySendCodec ||
       ( highestPrioritySendCodec &&
         !highestPrioritySendCodec->iIsEnabled ) )
        {
        CMceComCodec* newDefaultCodec = NULL;
        codecs.Reset();
        codecs.SetFilter( CMceComCodec::TIterator::EFilterNone );
        while( codecs.Next( codec ) )
            {
            if ( !newDefaultCodec && codec->iIsNegotiated && codec->SendSupported() )
                {
                codec->SetEnabled( ETrue );
                newDefaultCodec = codec;
                }
            else
                {
                codec->SetEnabled( EFalse );
                }
            }
        
        // Enable also new codecs of associated local streams
        UpdateDefaultCodecForAssociatedLocalL( newDefaultCodec );    
        }
    }

// -----------------------------------------------------------------------------
// CMceComVideoStream::UpdateDefaultCodecForAssociatedLocalL
// -----------------------------------------------------------------------------
//
void CMceComVideoStream::UpdateDefaultCodecForAssociatedLocalL( 
    CMceComCodec* aNewDefaultCodec )
    {
    CMceComCodec* codec = NULL;
    if ( aNewDefaultCodec && Session() )
        {
        for ( TInt i = 0; i < Session()->Streams().Count(); i++ )
            {
            if ( Session()->Streams()[ i ]->iType == KMceVideo )
                {
                CMceComVideoStream* videoStream = static_cast<CMceComVideoStream*>( 
                    Session()->Streams()[ i ] );
                const RPointerArray< CMceComCodec >& allCodecs =
    	            reinterpret_cast< const RPointerArray< CMceComCodec >& >( 
    	                videoStream->Codecs() );
    	
    	        // Enabling
                CMceComCodec::TIterator codecs( allCodecs, 
                    CMceComCodec::TIterator::EFilterAssociatedLocal, aNewDefaultCodec );
                while( codecs.Next( codec ) )
                    {
                    codec->SetEnabled( ETrue );
                    }
                
                // Disabling    
                codecs.Reset();
                while( codecs.Next( codec, CMceComCodec::TIterator::EOtherSdpName ) )
                    {
                    codec->SetEnabled( EFalse );
                    }
                }
            }
        }
    }
    
// -----------------------------------------------------------------------------
// CMceComVideoStream::PrepareL
// -----------------------------------------------------------------------------
//
void CMceComVideoStream::PrepareL()
    {
    __ASSERT_ALWAYS ( CodecCount() > 0, User::Leave( KErrArgument ) );

    if ( BoundStream() )
    	{
    	MCE_DEFINE_VIDEO( boundStream, BoundStreamL() );
        __ASSERT_ALWAYS ( boundStream.CodecCount() > 0, User::Leave( KErrArgument ) );
    	}
    }



// -----------------------------------------------------------------------------
// CMceComVideoStream::MccStreamType
// -----------------------------------------------------------------------------
//
TInt CMceComVideoStream::MccStreamType() const
    {
    
    TInt type = KMccVideoLocalStream;
    
    if ( iStreamType == EReceiveStream || 
         iStreamType == EReceiveOnlyStream )
        {
        type = KMccVideoDownlinkStream;
        }
    else if ( iStreamType == ESendStream || 
              iStreamType == ESendOnlyStream )
        {
        type = KMccVideoUplinkStream;
        }
    else
        {
        type = KMccVideoLocalStream;
        }
        
    return type;
    
    }


// -----------------------------------------------------------------------------
// CMceComVideoStream::MccStreamType
// -----------------------------------------------------------------------------
//
TInt CMceComVideoStream::MccLinkType() const
    {
    TInt type = KMccLinkGeneralVideo;
    
    if ( iStreamType == ELocalStream )
        {
        type = KMccLinkLocal;
        }
    
    if (iSession->iClientCryptoSuites.Count()>0)
    	{
    	type =KMccLinkSecureVideo;
    	}    
        
    return type;        
    }
    
#endif//MCE_COMMON_SERVER_SIDE
