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
#include "mcecomaudiostream.h"
#include "mcecomaudiocodec.h"
#include "mcecommediasource.h"
#include "mcecommediasink.h"
#include "mcertpsource.h"
#include "mcertpsink.h"
#include "mceserial.h"
#include "mceevents.h"
#include "cleanupresetanddestroy.h"

#ifdef MCE_COMMON_SERVER_SIDE

#include <mmcccodecinformationfactory.h>
#include <mmcccodecinformation.h>

    
#endif//MCE_COMMON_SERVER_SIDE


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceComAudioStream::NewL
// -----------------------------------------------------------------------------
//
CMceComAudioStream* CMceComAudioStream::NewL()
    {
    CMceComAudioStream* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceComAudioStream::NewLC
// -----------------------------------------------------------------------------
//
CMceComAudioStream* CMceComAudioStream::NewLC()
    {
    CMceComAudioStream* self = new (ELeave) CMceComAudioStream();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
// CMceComAudioStream::~CMceComAudioStream
// -----------------------------------------------------------------------------
//
CMceComAudioStream::~CMceComAudioStream()
    {
    iCodecs.ResetAndDestroy();
    }


// -----------------------------------------------------------------------------
// CMceComAudioStream::CMceComAudioStream
// -----------------------------------------------------------------------------
//
CMceComAudioStream::CMceComAudioStream()
    :CMceComMediaStream( KMceAudio )
    {
    }


// -----------------------------------------------------------------------------
// CMceComAudioStream::AddCodecL
// -----------------------------------------------------------------------------
//
void CMceComAudioStream::AddCodecL( CMceComCodec* aCodec )
    {
    __ASSERT_ALWAYS( aCodec, User::Leave( KErrArgument ) );

    CMceComAudioCodec* codec = static_cast< CMceComAudioCodec* >( aCodec );
    iCodecs.AppendL( codec );
    }


// -----------------------------------------------------------------------------
// CMceComAudioStream::RemoveCodecL
// -----------------------------------------------------------------------------
//
void CMceComAudioStream::RemoveCodecL( CMceComCodec* aCodec )
    {
    TInt index = FindCodecFromList( aCodec );

    if ( index >= 0 )
        {
        RemoveCodecFromListL( index );
        }

    delete aCodec;
    }


// -----------------------------------------------------------------------------
// CMceComAudioStream::CodecL
// -----------------------------------------------------------------------------
//
CMceComAudioCodec* CMceComAudioStream::CodecL( TInt aIndex ) const
	{
	__ASSERT_ALWAYS( aIndex >= 0 && aIndex < iCodecs.Count(),
		User::Leave( KErrArgument ) );
	
	return iCodecs[aIndex];
	}


// -----------------------------------------------------------------------------
// CMceComAudioStream::CodecCount
// -----------------------------------------------------------------------------
//
TInt CMceComAudioStream::CodecCount() const
	{
	return iCodecs.Count();
	}


// -----------------------------------------------------------------------------
// CMceComAudioStream::Codecs
// -----------------------------------------------------------------------------
//
const RPointerArray<CMceComAudioCodec>& CMceComAudioStream::Codecs() const
    {
    return iCodecs;
    }


// -----------------------------------------------------------------------------
// CMceComAudioStream::FindCodecL
// -----------------------------------------------------------------------------
//
CMceComCodec* CMceComAudioStream::FindCodecL( CMceComCodec& aCodec )
    {
    CMceComCodec* foundCodec = NULL;
    CMceComAudioCodec* codec = static_cast<CMceComAudioCodec*>( &aCodec );

    TInt index = FindCodecFromList( codec );
    if ( index == KErrNotFound )
        {
        const TInt codecCount = CodecCount();

        for ( TInt i = 0 ; i < codecCount && !foundCodec; ++i )
            {
            if ( CodecL( i )->iSdpName.CompareF( aCodec.iSdpName ) == 0 )
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
// CMceComAudioStream::FindCodecFromList
// -----------------------------------------------------------------------------
//
TInt CMceComAudioStream::FindCodecFromList( CMceComCodec* aCodec ) const
	{
	CMceComAudioCodec* codec = static_cast< CMceComAudioCodec* >( aCodec );
	return iCodecs.Find( codec );
	}


// -----------------------------------------------------------------------------
// CMceComAudioStream::RemoveCodecFromListL
// -----------------------------------------------------------------------------
//
void CMceComAudioStream::RemoveCodecFromListL( TInt aIndex )
	{
	__ASSERT_ALWAYS( aIndex >= 0 && aIndex < iCodecs.Count(),
		User::Leave( KErrArgument ) );

	iCodecs.Remove( aIndex );
	}


// -----------------------------------------------------------------------------
// CMceComAudioStream::DestroyCodecs
// -----------------------------------------------------------------------------
//
void CMceComAudioStream::DestroyCodecs()
	{
	iCodecs.ResetAndDestroy();
	}


// -----------------------------------------------------------------------------
// CMceComAudioStream::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComAudioStream::InternalizeFlatL( RReadStream& aReadStream )
    {
    CMceComMediaStream::InternalizeFlatL( aReadStream );
    }


// -----------------------------------------------------------------------------
// CMceComAudioStream::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComAudioStream::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    CMceComMediaStream::ExternalizeFlatL( aWriteStream );
    }

  
// -----------------------------------------------------------------------------
// CMceComAudioStream::InternalizeL
// -----------------------------------------------------------------------------
//
void CMceComAudioStream::InternalizeL( MMceComSerializationContext& aSerCtx )
    {
    CMceComMediaStream::InternalizeL( aSerCtx );
    
    TMceAudioStreamSerializer<CMceComAudioStream> serial( *this );
    serial.InternalizeL( aSerCtx );
        
    }
    
// -----------------------------------------------------------------------------
// CMceComAudioStream::ExternalizeL
// -----------------------------------------------------------------------------
//
void CMceComAudioStream::ExternalizeL( MMceComSerializationContext& aSerCtx )
    {
    CMceComMediaStream::ExternalizeL( aSerCtx );
    
    TMceAudioStreamSerializer<CMceComAudioStream> serial( *this );
    serial.ExternalizeL( aSerCtx );
    
    }

// -----------------------------------------------------------------------------
// CMceComAudioStream::CloneL
// -----------------------------------------------------------------------------
//
CMceComMediaStream* CMceComAudioStream::CloneL()
    {
    CMceComAudioStream* copy = new (ELeave) CMceComAudioStream();
    CleanupStack::PushL( copy );
    copy->ConstructL( *this );
    CleanupStack::Pop( copy );
    return copy;
    }

// -----------------------------------------------------------------------------
// CMceComAudioStream::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComAudioStream::ConstructL()
    {
    CMceComMediaStream::ConstructL();
    }


// -----------------------------------------------------------------------------
// CMceComAudioStream::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComAudioStream::ConstructL( CMceComAudioStream& aStream )
    {
    CMceComMediaStream::ConstructL( aStream );
    
    const TInt codecCount = aStream.CodecCount();

    for ( TInt i = 0 ; i < codecCount ; ++i )
        {
        CMceComAudioCodec* codec = aStream.CodecL( i )->CloneL();
        CleanupStack::PushL( codec );
        AddCodecL( codec );
        CleanupStack::Pop( codec );
        }
    
    }


// -----------------------------------------------------------------------------
// CMceComAudioStream::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComAudioStream::InitializeL( CMceComSession& aParent )
    {
    CMceComMediaStream::InitializeL( aParent );

	const TInt codecCount = CodecCount();
	
    for ( TInt i = 0 ; i < codecCount ; ++i )
        {
        CodecL( i )->InitializeL( *this );
        }
    }


// -----------------------------------------------------------------------------
// CMceComAudioStream::UpdateL
// -----------------------------------------------------------------------------
//
void CMceComAudioStream::UpdateL( CMceComMediaStream& aStream )
    {  
    CMceComMediaStream::UpdateL( aStream );

    CMceComAudioStream* stream = static_cast<CMceComAudioStream*>( &aStream );
    
    RPointerArray<CMceComAudioCodec> updated;
    
    MceCleanupResetAndDestroyPushL( updated );

    for ( TInt i = 0 ; i < stream->CodecCount() ; ++i )
        {
        CMceComAudioCodec* update = stream->CodecL( i );
        TBool isUpdated = EFalse;
        TInt j = 0;

        while( !isUpdated && j < CodecCount() )
            {
            CMceComAudioCodec* codec = CodecL( j );
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
        
    if ( CodecCount() > 0 )	//	Codecs have been removed
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
            CMceComAudioCodec* add = stream->CodecL( 0 );
            AddCodecL( add );
            stream->RemoveCodecFromListL( 0 );
            }
        }
    }


// -----------------------------------------------------------------------------
// CMceComAudioStream::ReorderCodecs
// -----------------------------------------------------------------------------
//
void CMceComAudioStream::ReorderCodecs()
    {
    TLinearOrder<CMceComAudioCodec> order( CMceComAudioCodec::CompareSdpIndex );
    
    iCodecs.Sort( order );
    }


#ifdef MCE_COMMON_SERVER_SIDE


// -----------------------------------------------------------------------------
// CMceComAudioStream::SynchronizeL
// -----------------------------------------------------------------------------
//
void CMceComAudioStream::SynchronizeL( TBool aRoleAnswerer )
    {
    TInt codecIndex( 0 );
	
    while ( codecIndex < CodecCount() )
        {
        CMceComCodec& codec = *CodecL( codecIndex );
        
        //When role is offerer stream cannot remove codecs. 
        //It will be done by mediamanager together with mcc
        //if there is session update decoded there might be codecs
        //which are not valid anymore, and thus, needed to be removed.
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
                MCE_DEFINE_AUDIO_CODEC( audioCodec, codec );
                CMceComAudioCodec* copy = audioCodec.CloneL();
                CleanupStack::PushL( copy );
                copy->InitializeL( BoundStreamL() );
                MCE_AUDIO( BoundStreamL() ).AddCodecL( copy );
                CleanupStack::Pop( copy );
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
void CMceComAudioStream::UpdateDefaultCodecL()
    {
    CMceComCodec* highestPrioritySendCodec = NULL;
    CMceComCodec* codec = NULL;

    const RPointerArray< CMceComCodec >& allCodecs = reinterpret_cast< const RPointerArray<CMceComCodec>& >( Codecs() );
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
        TBool newDefaultCodecSet( EFalse );
        codecs.Reset();
        codecs.SetFilter( CMceComCodec::TIterator::EFilterNone );

        while( codecs.Next( codec ) )
            {
            if ( !newDefaultCodecSet && codec->iIsNegotiated && codec->SendSupported() )
                {
                codec->SetEnabled( ETrue );
                newDefaultCodecSet = ETrue;
                }
            else
                {
                codec->SetEnabled( EFalse );
                }
            }
        }
    }


// -----------------------------------------------------------------------------
// CMceComAudioStream::PrepareL
// -----------------------------------------------------------------------------
//
void CMceComAudioStream::PrepareL()
    {
    __ASSERT_ALWAYS ( CodecCount() > 0, User::Leave( KErrArgument ) );

    if ( BoundStream() )
    	{
    	MCE_DEFINE_AUDIO( boundStream, BoundStreamL() );
        __ASSERT_ALWAYS ( boundStream.CodecCount() > 0, User::Leave( KErrArgument ) );
    	}
    }


// -----------------------------------------------------------------------------
// CMceComAudioStream::MccStreamType
// -----------------------------------------------------------------------------
//
TInt CMceComAudioStream::MccStreamType() const
    {
    
    TInt type = KMccAudioLocalStream;
    
    if ( iStreamType == EReceiveStream || 
         iStreamType == EReceiveOnlyStream )
        {
        type = KMccAudioDownlinkStream;
        }
    else if ( iStreamType == ESendStream || 
              iStreamType == ESendOnlyStream )
        {
        type = KMccAudioUplinkStream;
        }
    else
        {
        type = KMccAudioLocalStream;
        }
        
    return type;
    
    }


// -----------------------------------------------------------------------------
// CMceComAudioStream::MccStreamType
// -----------------------------------------------------------------------------
//
TInt CMceComAudioStream::MccLinkType() const
    {
    TInt type = KMccLinkGeneral;
    
    if ( iStreamType == ELocalStream )
        {
        type = KMccLinkLocal;
        }
    if (iSession->iClientCryptoSuites.Count()>0)
    	{
    	type =KMccLinkSecure;
    	}
    return type;        
    }

#endif//MCE_COMMON_SERVER_SIDE
