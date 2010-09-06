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




#include "mcemessagestream.h"
#include "mcecommessagestream.h"
#include "mcecommessagecodec.h"
#include "mcecommediasource.h"
#include "mcecommediasink.h"
#include "mceserial.h"
#include "mceevents.h"
#include "cleanupresetanddestroy.h"

#ifdef MCE_COMMON_SERVER_SIDE

#include <MmccCodecInformationFactory.h>
#include <MmccCodecInformation.h>

    
#endif//MCE_COMMON_SERVER_SIDE


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceComMessageStream::NewL
// -----------------------------------------------------------------------------
//
CMceComMessageStream* CMceComMessageStream::NewL()
    {
    CMceComMessageStream* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComMessageStream::NewLC
// -----------------------------------------------------------------------------
//
CMceComMessageStream* CMceComMessageStream::NewLC()
    {
    CMceComMessageStream* self = new (ELeave) CMceComMessageStream();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }



// -----------------------------------------------------------------------------
// CMceComMessageStream::~CMceComMessageStream
// -----------------------------------------------------------------------------
//
CMceComMessageStream::~CMceComMessageStream()
    {
    iCodecs.ResetAndDestroy();
    }



// -----------------------------------------------------------------------------
// CMceComMessageStream::CMceComMessageStream
// -----------------------------------------------------------------------------
//
CMceComMessageStream::CMceComMessageStream()
    :CMceComMediaStream( KMceMessage ),
    iUseMsrpPathAttrForHostAddrAndPort(ETrue),
    iMsrpConnUsage(EFalse)
    {
    }




// -----------------------------------------------------------------------------
// CMceComMessageStream::AddCodecL
// -----------------------------------------------------------------------------
//
void CMceComMessageStream::AddCodecL( CMceComCodec* aCodec)
    {
    iCodecs.AppendL( static_cast<CMceComMessageCodec*>(aCodec) );
    }

// -----------------------------------------------------------------------------
// CMceComMessageStream::RemoveCodecL
// -----------------------------------------------------------------------------
//
void CMceComMessageStream::RemoveCodecL( CMceComCodec* aCodec )
    {
    CMceComMessageCodec* codec = static_cast<CMceComMessageCodec*>( aCodec );
    TInt index = Codecs().Find( codec );
    User::LeaveIfError( index );
    
    Codecs().Remove( index );
    delete aCodec;
    
    }


// -----------------------------------------------------------------------------
// CMceComMessageStream::RemoveCodecL
// -----------------------------------------------------------------------------
//
void CMceComMessageStream::DestroyCodecs( )
    {
    iCodecs.ResetAndDestroy();
    } 

// -----------------------------------------------------------------------------
// CMceComAudioStream::CodecL
// -----------------------------------------------------------------------------
//
CMceComMessageCodec* CMceComMessageStream::CodecL( TInt aIndex ) const
    {
    __ASSERT_ALWAYS( aIndex >= 0 && aIndex < iCodecs.Count(),
        User::Leave( KErrArgument ) );
    
    return iCodecs[aIndex];
    }

// -----------------------------------------------------------------------------
// CMceComMessageStream::CodecCount
// -----------------------------------------------------------------------------
//
TInt CMceComMessageStream::CodecCount() const
    {
    return iCodecs.Count();
    }

// -----------------------------------------------------------------------------
// CMceComMessageStream::FindCodec
// -----------------------------------------------------------------------------
//

CMceComCodec* CMceComMessageStream::FindCodecL( CMceComCodec& aCodec )
    {
    CMceComCodec* foundCodec = NULL;
    CMceComMessageCodec* codec = static_cast<CMceComMessageCodec*>( &aCodec );
    TInt index = Codecs().Find( codec );
    if ( index == KErrNotFound )
        {
        for ( TInt i = 0; i < Codecs().Count() && !foundCodec; i++ )
            {
            if ( Codecs()[ i ]->iSdpName.CompareF( aCodec.iSdpName ) == 0 )
                {
                foundCodec = Codecs()[ i ];
                }     
            }
        }
    else
        {
        foundCodec = Codecs()[ index ];
        }
    return foundCodec;
    }
    

// -----------------------------------------------------------------------------
// CMceComMessageStream::Codecs
// -----------------------------------------------------------------------------
//
RPointerArray<CMceComMessageCodec>& CMceComMessageStream::Codecs()
    {
    return iCodecs;
    }


// -----------------------------------------------------------------------------
// CMceComMessageStream::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComMessageStream::InternalizeFlatL( RReadStream& aReadStream )
    {
    CMceComMediaStream::InternalizeFlatL( aReadStream );
    this->iUseMsrpPathAttrForHostAddrAndPort = aReadStream.ReadInt8L();
    this->iMsrpConnUsage = aReadStream.ReadInt8L();
    }



// -----------------------------------------------------------------------------
// CMceComMessageStream::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComMessageStream::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    CMceComMediaStream::ExternalizeFlatL( aWriteStream );
    aWriteStream.WriteInt8L(this->iUseMsrpPathAttrForHostAddrAndPort);
    aWriteStream.WriteInt8L(this->iMsrpConnUsage);
    }

  
// -----------------------------------------------------------------------------
// CMceComMessageStream::InternalizeL
// -----------------------------------------------------------------------------
//
void CMceComMessageStream::InternalizeL( MMceComSerializationContext& aSerCtx )
    {
    CMceComMediaStream::InternalizeL( aSerCtx );
    
    TMceMessageStreamSerializer<CMceComMessageStream> serial( *this );
    serial.InternalizeL( aSerCtx );
        
    }
    
// -----------------------------------------------------------------------------
// CMceComMessageStream::ExternalizeL
// -----------------------------------------------------------------------------
//
void CMceComMessageStream::ExternalizeL( MMceComSerializationContext& aSerCtx )
    {
    CMceComMediaStream::ExternalizeL( aSerCtx );
    
    TMceMessageStreamSerializer<CMceComMessageStream> serial( *this );
    serial.ExternalizeL( aSerCtx );
    
    }

// -----------------------------------------------------------------------------
// CMceComMessageStream::CloneL
// -----------------------------------------------------------------------------
//
CMceComMediaStream* CMceComMessageStream::CloneL()
    {
    CMceComMessageStream* copy = new (ELeave) CMceComMessageStream();
    CleanupStack::PushL( copy );
    copy->ConstructL( *this );
    CleanupStack::Pop( copy );
    return copy;
    }

// -----------------------------------------------------------------------------
// CMceComMessageStream::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComMessageStream::ConstructL()
    {
    
    CMceComMediaStream::ConstructL();
    
    }


// -----------------------------------------------------------------------------
// CMceComMessageStream::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComMessageStream::ConstructL( CMceComMessageStream& aStream )
    {
    CMceComMediaStream::ConstructL( aStream );
    for (int i = 0; i<aStream.Codecs().Count();i++)
        {
        CMceComMessageCodec* codec = aStream.Codecs()[ i ]->CloneL();
        CleanupStack::PushL( codec );
        AddCodecL( codec );
        CleanupStack::Pop( codec );
        }
    
    }


// -----------------------------------------------------------------------------
// CMceComMessageStream::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComMessageStream::InitializeL( CMceComSession& aParent )
    {
    CMceComMediaStream::InitializeL( aParent );

    for (int i = 0; i<Codecs().Count();i++)
        {
        Codecs()[i]->InitializeL( *this );
        }
    }


// -----------------------------------------------------------------------------
// CMceComMessageStream::UpdateL
// -----------------------------------------------------------------------------
//
void CMceComMessageStream::UpdateL( CMceComMediaStream& aStream )
    {  
    CMceComMediaStream::UpdateL( aStream );

    CMceComMessageStream* stream = static_cast<CMceComMessageStream*>( &aStream );
	
	
    if (iUseMsrpPathAttrForHostAddrAndPort != stream->iUseMsrpPathAttrForHostAddrAndPort)
    {
    iUseMsrpPathAttrForHostAddrAndPort = stream->iUseMsrpPathAttrForHostAddrAndPort;
    }
    this->iMsrpConnUsage = stream->iMsrpConnUsage ;
    
    RPointerArray<CMceComMessageCodec> updated;
    
    MceCleanupResetAndDestroyPushL( updated );
    
    for( TInt i = 0; i < stream->Codecs().Count(); i++ )
        {
        CMceComMessageCodec* update = stream->Codecs()[i];
        TBool isUpdated = EFalse;
        TInt j = 0;
        while( !isUpdated && j < Codecs().Count() )
            {
            CMceComMessageCodec* codec = Codecs()[j];
            if ( codec->Id() == update->Id() )
                {
                codec->UpdateL( *update );
                stream->Codecs().Remove( i );
                delete update;
                updated.AppendL( codec );
                Codecs().Remove( j );
                i--;
                isUpdated = ETrue;
                }
            j++;                
            }
        }
	this->iMsrpConnUsage = stream->iMsrpConnUsage ;        
    if ( Codecs().Count() > 0 )//codecs have been removed
        {
        Session()->IsStructureChanged() = ETrue;
        }
        
    iCodecs.ResetAndDestroy();
    while( updated.Count() > 0 )
        {
        iCodecs.AppendL( updated[0] );
        updated.Remove( 0 );
        }
        
    CleanupStack::PopAndDestroy();//updated
            
    if ( stream->Codecs().Count() > 0 )//streams have been added
        {
        Session()->IsStructureChanged() = ETrue;
            
        while( stream->Codecs().Count() > 0 )
            {
            CMceComMessageCodec* add = stream->Codecs()[0];
            iCodecs.AppendL( add );
            stream->Codecs().Remove( 0 );
            }
        }
    }



// -----------------------------------------------------------------------------
// CMceComMessageStream::ReorderCodecs
// -----------------------------------------------------------------------------
//
void CMceComMessageStream::ReorderCodecs()
    {
    // The data path will not be handled in MCC for message type streams hence no codecs.
    TLinearOrder<CMceComMessageCodec> order( CMceComMessageCodec::CompareSdpIndex );
    
    iCodecs.Sort( order );
    }



#ifdef MCE_COMMON_SERVER_SIDE


// -----------------------------------------------------------------------------
// CMceComMessageStream::SynchronizeL
// -----------------------------------------------------------------------------
//
void CMceComMessageStream::SynchronizeL( TBool aRoleAnswerer )
    {
    TInt codecIndex( 0 );
// Presently there is no codecs support in MCE/MCC for message type streams
    while ( codecIndex < Codecs().Count() )
        {
        CMceComCodec& codec = *Codecs()[ codecIndex ];
        
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
                //MCE_DEFINE_TEXT_CODEC( textCodec, codec );
                CMceComMessageCodec& textCodec = static_cast<CMceComMessageCodec&>( codec );
                CMceComMessageCodec* copy = textCodec.CloneL();
                CleanupStack::PushL( copy );
                copy->InitializeL( BoundStreamL() );
                //MCE_TEXT( BoundStreamL() ).AddCodecL( copy );
                (static_cast<CMceComMessageStream&>( BoundStreamL() )).AddCodecL( copy );
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
// CMceComMessageStream::UpdateDefaultCodecL
// -----------------------------------------------------------------------------
//
void CMceComMessageStream::UpdateDefaultCodecL()
    {
    RPointerArray<CMceComMessageCodec>& messageCodecs = Codecs();
    
    if ( messageCodecs.Count() > 0 )
        {
        CMceComCodec& highestPriorityCodec = *messageCodecs[ 0 ];
        
        TBool defaultRemoved( highestPriorityCodec.iIsEnabled &&
                              !highestPriorityCodec.iIsNegotiated );
                              
        TBool priorityChanged( !highestPriorityCodec.iIsEnabled && 
                               highestPriorityCodec.iIsNegotiated );                
        if ( defaultRemoved || priorityChanged )
            {
            TBool newDefaultCodecSet( EFalse );
            for ( TInt i = 0; i < messageCodecs.Count(); i++ )
                {
                CMceComCodec& codec = *messageCodecs[ i ];
                if ( codec.iIsNegotiated )
                    {
                    if ( newDefaultCodecSet )
                        {
                        codec.iIsEnabled = EFalse;
                        }
                    else
                        {
                        codec.iIsEnabled = ETrue;
                        newDefaultCodecSet = ETrue;
                        }
                    }
                }
            }
        }
    }
    
// -----------------------------------------------------------------------------
// CMceComMessageStream::PrepareL
// -----------------------------------------------------------------------------
//
void CMceComMessageStream::PrepareL()
    {
// Data path will not be handled for Message type streams in MCE/MCC
    //Hence not required to prepare the streams
    RPointerArray<CMceComMessageCodec>& codecs = Codecs();
    __ASSERT_ALWAYS ( codecs.Count() > 0, User::Leave( KErrArgument ) );

    if ( BoundStream() )
        {
        CMceComMessageStream& boundStream = static_cast<CMceComMessageStream&>( BoundStreamL() );
        __ASSERT_ALWAYS ( boundStream.Codecs().Count() > 0, User::Leave( KErrArgument ) );
        }

    }



// -----------------------------------------------------------------------------
// CMceComMessageStream::MccStreamType
// -----------------------------------------------------------------------------
//
TInt CMceComMessageStream::MccStreamType() const
    {
    
    // The data path will not be handled in MCC for message type streams
    TInt type = KMccMessageLocalStream;
    
    if ( iStreamType == EReceiveStream || 
         iStreamType == EReceiveOnlyStream )
        {
        type = KMccMessageDownlinkStream;
        }
    else if ( iStreamType == ESendStream || 
              iStreamType == ESendOnlyStream )
        {
        type = KMccMessageUplinkStream;
        }
    else
        {
        type = KMccMessageLocalStream;
        }
        
    return type;
    }


// -----------------------------------------------------------------------------
// CMceComMessageStream::MccStreamType
// -----------------------------------------------------------------------------
//
TInt CMceComMessageStream::MccLinkType() const
    {
// The data path will not be handled in MCC for message type streams
    TInt type = KMccLinkMessage;
    
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
