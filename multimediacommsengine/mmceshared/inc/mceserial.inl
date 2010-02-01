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





        
        
// -----------------------------------------------------------------------------
// CMceMsgObject::CMceMsgObject
// -----------------------------------------------------------------------------
//
template <class T>
inline CMceMsgObject<T>::CMceMsgObject()
  : CMceMsgBase( EMceItcMsgTypeSession ),
    iObject( NULL )
    {
    }


// -----------------------------------------------------------------------------
// CMceMsgObject::CMceMsgObject
// -----------------------------------------------------------------------------
//
template <class T>
inline CMceMsgObject<T>::CMceMsgObject( T& aObject, TMceItcDataType aItcType )
  : CMceMsgBase( aItcType ),
    iObject( &aObject )
    {
    }
    
// -----------------------------------------------------------------------------
// CMceMsgObject::~CMceMsgObject
// -----------------------------------------------------------------------------
//
template <class T>
inline CMceMsgObject<T>::~CMceMsgObject()
    {
    if ( iPushed )
        {
        LocalClose();
        }
    }

    
// -----------------------------------------------------------------------------
// CMceMsgObject::EncodeL
// -----------------------------------------------------------------------------
//
template <class T>
inline void CMceMsgObject<T>::EncodeL()
    {
    
    delete iEncodeBuf;
    iEncodeBuf = NULL;
    
	iEncodeBuf = CBufFlat::NewL( KMceExternalizeBufferExpandSize );

	RBufWriteStream writeStream( *iEncodeBuf, 0 );
	writeStream.PushL();
    MMceComSerializationContext serCtx( writeStream );
    CleanupClosePushL( serCtx );
	
	iObject->ExternalizeL( serCtx );
	
	CleanupStack::PopAndDestroy(); // serCtx
	CleanupStack::PopAndDestroy(); // writeStream
    
    }


// -----------------------------------------------------------------------------
// CMceMsgObject::DoDecodeL
// -----------------------------------------------------------------------------
//
template <class T>
inline void CMceMsgObject<T>::DoDecodeL( RReadStream& aReadStream )
    {
    MMceComSerializationContext serCtx( aReadStream );
    CleanupClosePushL( serCtx );
    DoDecodeL( serCtx );
    
    CleanupStack::PopAndDestroy();//serCtx
    
    }

// -----------------------------------------------------------------------------
// CMceMsgObject::DoDecodeL
// -----------------------------------------------------------------------------
//
template <class T>
inline void CMceMsgObject<T>::DoDecodeL( MMceComSerializationContext& aSerCtx )
    {
    
    iObject = (T*)T::Factory().CreateLC( aSerCtx );
	CleanupStack::Pop( iObject );
    
    }
        


// -----------------------------------------------------------------------------
// CMceMsgObject::Object
// -----------------------------------------------------------------------------
//
template <class T>
inline T* CMceMsgObject<T>::Object()
    {
    T* ret = iObject;
    iObject = NULL;
    return ret;
    }
    

// -----------------------------------------------------------------------------
// CMceMsgObject::Close
// -----------------------------------------------------------------------------
//
template <class T>
inline void CMceMsgObject<T>::Close()
    {
    MCE_DELETE( iObject );
    }

// -----------------------------------------------------------------------------
// CMceMsgObject::LocalClose
// -----------------------------------------------------------------------------
//
template <class T>
inline void CMceMsgObject<T>::LocalClose()
    {
    MCE_DELETE( iObject );
    }
    

// -----------------------------------------------------------------------------
// CMceMsgArray::CMceMsgArray
// -----------------------------------------------------------------------------
//
template <class T>
inline CMceMsgArray<T>::CMceMsgArray( T& aFactory )
  : CMceMsgBase( EMceItcMsgTypeMessageArray ),
    iFactory( aFactory )
    {
    }


// -----------------------------------------------------------------------------
// CMceMsgArray::~CMceMsgArray
// -----------------------------------------------------------------------------
//
template <class T>
inline CMceMsgArray<T>::~CMceMsgArray()
    {
    if ( iPushed )
        {
        LocalClose();
        }
    else
        {
        iArray.ResetAndDestroy();
        iArray.Close();
        }
    }

// -----------------------------------------------------------------------------
// CMceMsgArray::EncodeL
// -----------------------------------------------------------------------------
//
template <class T>
inline void CMceMsgArray<T>::EncodeL()
    {
    
    delete iEncodeBuf;
    iEncodeBuf = NULL;

	iEncodeBuf = CBufFlat::NewL( KMceExternalizeBufferExpandSize );

	RBufWriteStream writeStream( *iEncodeBuf, 0 );
	writeStream.PushL();
    
    TInt count = iArray.Count();
    writeStream.WriteUint32L( count );
	for ( TInt i=0; i<count; i++ )
	    {
	    CMceMsgBase* object = iArray[i];
        writeStream.WriteUint32L( object->Type() );
	    object->EncodeL();
	    writeStream.WriteL( object->EncodeBuffer().Ptr( 0 ) );
	    }

	CleanupStack::PopAndDestroy(); // writeStream
        
    }


// -----------------------------------------------------------------------------
// CMceMsgArray::DoDecodeL
// -----------------------------------------------------------------------------
//
template <class T>
inline void CMceMsgArray<T>::DoDecodeL( RReadStream& aReadStream )
    {

    TInt count = aReadStream.ReadUint32L();
	for ( TInt i=0; i<count; i++ )
	    {
	    TMceItcDataType type = (TMceItcDataType)aReadStream.ReadUint32L();
        MMceComSerializationContext serCtx( aReadStream );
        CleanupClosePushL( serCtx );
	    
	    CMceMsgBase* object = iFactory.CreateL( type, serCtx );
	    CleanupStack::PushL( object );
	    object->PushL();
	    iArray.AppendL( object );
	    object->Pop();
	    CleanupStack::Pop( object );

    	CleanupStack::PopAndDestroy(); // serCtx
	    
	    
	    }
        
    }


// -----------------------------------------------------------------------------
// CMceMsgArray::Close
// -----------------------------------------------------------------------------
//
template <class T>
inline void CMceMsgArray<T>::Close()
    {
	for ( TInt i=0; i<iArray.Count();i++)
	    {
	    iArray[ i ]->Close();
	    }
	    
    iArray.ResetAndDestroy();
    iArray.Close();
    
    }

// -----------------------------------------------------------------------------
// CMceMsgArray::LocalClose
// -----------------------------------------------------------------------------
//
template <class T>
inline void CMceMsgArray<T>::LocalClose()
    {
	for ( TInt i=0; i<iArray.Count();i++)
	    {
	    iArray[ i ]->Close();
	    }
	    
    iArray.ResetAndDestroy();
    iArray.Close();
    
    }

// -----------------------------------------------------------------------------
// TMceSessionSerializer::TMceSessionSerializer
// -----------------------------------------------------------------------------
//
template <class T>
inline TMceSessionSerializer<T>::TMceSessionSerializer( T& aSession )
  : iSession( aSession ),
    iFlatData( NULL )
    {
		iFlatData = static_cast<CMceComSession*>(&aSession);
    }

// -----------------------------------------------------------------------------
// TMceSessionSerializer::TMceSessionSerializer
// -----------------------------------------------------------------------------
//
template <class T>
inline TMceSessionSerializer<T>::TMceSessionSerializer( T& aSession, 
                                                        CMceComSession* aFlatData )
  : iSession( aSession ),
    iFlatData( aFlatData )
    {
    }



// -----------------------------------------------------------------------------
// TMceSessionSerializer::InternalizeL
// -----------------------------------------------------------------------------
//
template <class T>
inline void TMceSessionSerializer<T>::InternalizeL( MMceComSerializationContext& aSerCtx )
    {
    RReadStream& readStream = aSerCtx.ReadStream();
    
    iFlatData->InternalizeFlatL( readStream );
    
    //streams
    TUint32 streamCount = readStream.ReadUint32L();
    TUint32 i = 0;
    for( i = 0; i<streamCount; i++)
        {
        
        iSession.T::AddStreamL( 
            iSession.BaseFactory().MediaStreamFactory().CreateLC( aSerCtx ) );
            
        CleanupStack::Pop();
        }

    //bundles
    TUint32 bundleCount = readStream.ReadUint32L();
    for( i = 0; i<bundleCount; i++)
        {
        
        iSession.AddBundleL( 
            iSession.BaseFactory().CreateBundleLC( iSession, aSerCtx ) );
            
        CleanupStack::Pop();
        }
    
    }
    
// -----------------------------------------------------------------------------
// TMceSessionSerializer::ExternalizeL
// -----------------------------------------------------------------------------
//
template <class T>
inline void TMceSessionSerializer<T>::ExternalizeL( MMceComSerializationContext& aSerCtx )
    {
    RWriteStream& writeStream = aSerCtx.WriteStream();
    
    iFlatData->ExternalizeFlatL( writeStream );
    
    
    TInt streams = iSession.Streams().Count();
    writeStream.WriteUint32L( streams );
    TInt i = 0;
    for( i=0; i<streams; i++)
        {
        iSession.Streams()[ i ]->ExternalizeL( aSerCtx );
        }
        
    TInt bundles = iSession.Bundles().Count();
    writeStream.WriteUint32L( bundles );
    for( i=0; i<bundles; i++)
        {
        iSession.Bundles()[ i ]->ExternalizeL( aSerCtx );
        }
        
    }
    
    
    
    
    
    


// -----------------------------------------------------------------------------
// TMceMediaStreamSerializer::TMceMediaStreamSerializer
// -----------------------------------------------------------------------------
//
template <class T>
TMceMediaStreamSerializer<T>::TMceMediaStreamSerializer( T& aStream )
  : iStream( aStream ),
    iFlatData( NULL )
    {
    iFlatData = static_cast<CMceComMediaStream*>(&aStream);
    }

// -----------------------------------------------------------------------------
// TMceMediaStreamSerializer::TMceMediaStreamSerializer
// -----------------------------------------------------------------------------
//
template <class T>
TMceMediaStreamSerializer<T>::TMceMediaStreamSerializer( T& aStream, 
                                                   CMceComMediaStream* aFlatData )
  : iStream( aStream ),
    iFlatData( aFlatData )
    {
    }



// -----------------------------------------------------------------------------
// TMceMediaStreamSerializer::InternalizeL
// -----------------------------------------------------------------------------
//
template <class T>
void TMceMediaStreamSerializer<T>::InternalizeL( MMceComSerializationContext& aSerCtx )
    {
    RReadStream& readStream = aSerCtx.ReadStream();
    iFlatData->InternalizeFlatL( readStream );
    
    //linked stream
    if ( iStream.Binder() )
        {
        iStream.BindL( static_cast<T*> (
            iStream.BaseFactory().MediaStreamFactory().CreateLC( aSerCtx ) ) );
        CleanupStack::Pop();
        }
        
    //source
    TBool isSource = static_cast<TBool>( readStream.ReadUint8L() );
    if ( isSource )
        {
        iStream.SetSourceL( aSerCtx );
        }
            
    //sinks            
    TUint32 sinkCount = readStream.ReadUint32L();
    for( TUint i=0;i<sinkCount;i++)
        {
        iStream.AddSinkL( aSerCtx );
        }
    }
    
// -----------------------------------------------------------------------------
// TMceMediaStreamSerializer::ExternalizeL
// -----------------------------------------------------------------------------
//
template <class T>
void TMceMediaStreamSerializer<T>::ExternalizeL( MMceComSerializationContext& aSerCtx )
    {
    
    RWriteStream& writeStream = aSerCtx.WriteStream();
    iFlatData->ExternalizeFlatL( writeStream );
    
    
    //linked stream
    if ( iStream.Binder() && iStream.BoundStream() )
        {
        iStream.BoundStreamL().ExternalizeL( aSerCtx );
        }

    //source
    writeStream.WriteUint8L( iStream.Source() ? ETrue : EFalse );
    if ( iStream.Source() )
        {
        TBool externalizedReference = aSerCtx.IsCached( iStream.Source() );
	    writeStream.WriteUint8L( externalizedReference );
        if ( externalizedReference )
            {
            MceSerial::EncodeL( iStream.Source()->Id(), writeStream );
            }
        else
            {
            iStream.Source()->ExternalizeL( aSerCtx );
            aSerCtx.SourceCache().AppendL( iStream.Source() );
            }
        }


    //sinks            
    TUint32 sinkCount = iStream.Sinks().Count();
    writeStream.WriteUint32L( sinkCount );
    for( TUint i=0;i<sinkCount;i++)
        {
        TBool externalizedReference = aSerCtx.IsCached( iStream.Sinks()[i] );
	    writeStream.WriteUint8L( externalizedReference );
        if ( externalizedReference )
            {
            MceSerial::EncodeL( iStream.Sinks()[i]->Id(), writeStream );
            }
        else
            {
            iStream.Sinks()[i]->ExternalizeL( aSerCtx );
            aSerCtx.SinkCache().AppendL( iStream.Sinks()[i] );
            }
        }
    
    }





// -----------------------------------------------------------------------------
// TMceAudioStreamSerializer::TMceAudioStreamSerializer
// -----------------------------------------------------------------------------
//
template <class T>
inline TMceAudioStreamSerializer<T>::TMceAudioStreamSerializer( T& aStream )
  : iAudioStream( aStream )
    {
    }



// -----------------------------------------------------------------------------
// TMceAudioStreamSerializer::InternalizeL
// -----------------------------------------------------------------------------
//
template <class T>
inline void TMceAudioStreamSerializer<T>::InternalizeL( MMceComSerializationContext& aSerCtx )
    {
    RReadStream& readStream = aSerCtx.ReadStream();
            
    //codecs
    TUint32 codecCount = readStream.ReadUint32L();
    for( TUint i=0;i<codecCount;i++)
        {
        
        iAudioStream.AddCodecL( 
            iAudioStream.BaseFactory().AudioCodecFactory().CreateLC( aSerCtx ) );
            
        CleanupStack::Pop();
        }
    }
    
// -----------------------------------------------------------------------------
// TMceAudioStreamSerializer::ExternalizeL
// -----------------------------------------------------------------------------
//
template <class T>
inline void TMceAudioStreamSerializer<T>::ExternalizeL( MMceComSerializationContext& aSerCtx )
    {
    RWriteStream& writeStream = aSerCtx.WriteStream();
    
    //codecs
    TUint32 codecCount = iAudioStream.Codecs().Count();
    writeStream.WriteUint32L( codecCount );
    for( TUint i=0 ; i<codecCount ; ++i)
        {
        iAudioStream.Codecs()[i]->ExternalizeL( aSerCtx );
        }
    
    }






// -----------------------------------------------------------------------------
// TMceVideoStreamSerializer::TMceVideoStreamSerializer
// -----------------------------------------------------------------------------
//
template <class T>
inline TMceVideoStreamSerializer<T>::TMceVideoStreamSerializer( T& aStream )
  : iVideoStream( aStream )
    {
    }


// -----------------------------------------------------------------------------
// TMceVideoStreamSerializer::InternalizeL
// -----------------------------------------------------------------------------
//
template <class T>
inline void TMceVideoStreamSerializer<T>::InternalizeL( MMceComSerializationContext& aSerCtx )
    {

    RReadStream& readStream = aSerCtx.ReadStream();
    
    //codecs
    TUint32 codecCount = readStream.ReadUint32L();
    for( TUint i=0;i<codecCount;i++)
        {
        
        iVideoStream.AddCodecL( 
            iVideoStream.BaseFactory().VideoCodecFactory().CreateLC( aSerCtx ) );
            
        CleanupStack::Pop();
        }
    }
    
// -----------------------------------------------------------------------------
// TMceVideoStreamSerializer::ExternalizeL
// -----------------------------------------------------------------------------
//
template <class T>
inline void TMceVideoStreamSerializer<T>::ExternalizeL( MMceComSerializationContext& aSerCtx )
    {
    RWriteStream& writeStream = aSerCtx.WriteStream();
    
    //codecs
    TUint32 codecCount = iVideoStream.Codecs().Count();
    writeStream.WriteUint32L( codecCount );
    for( TUint i=0;i<codecCount;i++)
        {
        iVideoStream.Codecs()[i]->ExternalizeL( aSerCtx );
        }
    
    }
