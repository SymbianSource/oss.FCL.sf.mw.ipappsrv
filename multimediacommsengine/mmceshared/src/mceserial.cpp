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
* Description:    
*
*/




#include "mceserial.h"
#include "mcefactory.h"
#include "ecam.h"



// ============================ MEMBER FUNCTIONS ===============================
// MceSerial

// -----------------------------------------------------------------------------
// MceSerial::DecodeL
// -----------------------------------------------------------------------------
//
void MceSerial::DecodeL( TDes8& aString, RReadStream& aReadStream )
    {
    TUint32 len = aReadStream.ReadUint32L();
    if ( len > 0 )
        {
        aReadStream.ReadL( aString, len );
        }
    else
        {
        aString.SetLength( 0 );
        }
    }

// -----------------------------------------------------------------------------
// MceSerial::EncodeL
// -----------------------------------------------------------------------------
//
void MceSerial::EncodeL( const TDesC8& aString, RWriteStream& aWriteStream )
    {
    aWriteStream.WriteUint32L( aString.Length() );
    if ( aString.Length() > 0 )
        {
        aWriteStream.WriteL( aString );
        }
    }

// -----------------------------------------------------------------------------
// MceSerial::EncodeL
// -----------------------------------------------------------------------------
//
void MceSerial::EncodeL( HBufC8* aString, RWriteStream& aWriteStream )
    {
    if ( aString )
        {
        EncodeL( *aString, aWriteStream );
        }
    else
        {
        EncodeL( KNullDesC8, aWriteStream );
        }
    }

// -----------------------------------------------------------------------------
// MceSerial::DecodeL
// -----------------------------------------------------------------------------
//
void MceSerial::DecodeL( HBufC8*& aBuffer, RReadStream& aReadStream )
    {
   delete aBuffer;
   aBuffer = NULL;
       
   TUint32 len = aReadStream.ReadUint32L();
   
    if ( len > 0 )
	    {
	    HBufC8* tmpBuffer = HBufC8::NewLC ( len );
	    TPtr8 bufPtr = tmpBuffer->Des();
	    aReadStream.ReadL( bufPtr, len );
	    CleanupStack::Pop( tmpBuffer );
	    aBuffer = tmpBuffer;
        }
    else
	    {
	    aBuffer = KNullDesC8().AllocL();
        }           
    }

// -----------------------------------------------------------------------------
// MceSerial::DecodeL
// -----------------------------------------------------------------------------
//
void MceSerial::DecodeL( CDesC8Array*& aArray, RReadStream& aReadStream )
    {
    delete aArray;
    aArray = NULL;
    
    TInt count = aReadStream.ReadUint32L();
	CDesC8ArrayFlat* tmpArray = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( tmpArray );
	for ( int i=0;i<count;i++ )
	    {
	    TInt length = aReadStream.ReadUint32L();
	    HBufC8* item = HBufC8::NewLC( length );
	    TPtr8 ptr = item->Des();
	    aReadStream.ReadL( ptr , length );
	    tmpArray->AppendL( *item );
	    CleanupStack::PopAndDestroy( item );//item
	    }
    
    CleanupStack::Pop( tmpArray );
    aArray = tmpArray;
    }
    
// -----------------------------------------------------------------------------
// MceSerial::EncodeL
// -----------------------------------------------------------------------------
//
void MceSerial::EncodeL( CDesC8Array* aArray, RWriteStream& aWriteStream )
    {
    TInt count = aArray ? aArray->MdcaCount() : 0;
    aWriteStream.WriteUint32L( count );
	for ( int i=0;i<count;i++ )
	    {
	    TPtrC8 item = aArray->MdcaPoint( i );
	    aWriteStream.WriteUint32L( item.Length() );
	    aWriteStream.WriteL( item );
	    }
    }

// -----------------------------------------------------------------------------
// MceSerial::DecodeL
// -----------------------------------------------------------------------------
//
void MceSerial::DecodeL( TMceMediaId& aId, RReadStream& aReadStream )
    {
    aId.iAppId = aReadStream.ReadUint32L();
    aId.iId = aReadStream.ReadUint32L();
    }

// -----------------------------------------------------------------------------
// MceSerial::EncodeL
// -----------------------------------------------------------------------------
//
void MceSerial::EncodeL( TMceMediaId aId, RWriteStream& aWriteStream )
    {
    aWriteStream.WriteUint32L( aId.iAppId );
    aWriteStream.WriteUint32L( aId.iId );
    }

// -----------------------------------------------------------------------------
// MceSerial::EncodeL
// -----------------------------------------------------------------------------
//
void MceSerial::EncodeL( RArray<TMceCryptoContext>& aArray,
						 RWriteStream& aWriteStream )
    {
    TInt count = aArray.Count();
    aWriteStream.WriteUint32L( count );
    for (int i=0; i<count; i++)
    	{
    	aWriteStream.WriteInt32L(aArray[i]);
    	}
    }

// -----------------------------------------------------------------------------
// MceSerial::DecodeL
// -----------------------------------------------------------------------------
//
void MceSerial::DecodeL( RArray<TMceCryptoContext>& aArray,
						 RReadStream& aReadStream )
    {
    aArray.Reset();
    TInt count = aReadStream.ReadUint32L();
	for ( int i=0; i < count; i++ )
	    {
	    TMceCryptoContext item =
	    	static_cast <TMceCryptoContext> ( aReadStream.ReadInt32L() );
	    aArray.InsertL( item, i );
	    }
    }


// ============================ MEMBER FUNCTIONS ===============================
// CMceMsgBase

// -----------------------------------------------------------------------------
// CMceMsgBase::CMceMsgBase
// Initialize all members, since this class is instantiated also from stack,
// without using new (ELeave). That means the members are not automatically
// initialized to zero, like CBase derived objects would normally do if
// allocated from the heap.
// -----------------------------------------------------------------------------
//
CMceMsgBase::CMceMsgBase( TMceItcDataType aType )
    : iEncodeBuf( NULL ),
      iType( aType ),
      iPushed( EFalse )
    {
    }

// -----------------------------------------------------------------------------
// CMceMsgBase::~CMceMsgBase
// -----------------------------------------------------------------------------
//
CMceMsgBase::~CMceMsgBase()
    {
    MCE_DELETE( iEncodeBuf );
    }

// -----------------------------------------------------------------------------
// CMceMsgBase::Type
// -----------------------------------------------------------------------------
//
TMceItcDataType CMceMsgBase::Type()
    {
    return iType;
    }

// -----------------------------------------------------------------------------
// CMceMsgBase::EncodeBuffer
// -----------------------------------------------------------------------------
//
CBufFlat& CMceMsgBase::EncodeBuffer()
    {
    return *iEncodeBuf;
    }

// -----------------------------------------------------------------------------
// CMceMsgBase::EncodeBufferCloneL
// -----------------------------------------------------------------------------
//
HBufC8* CMceMsgBase::EncodeBufferCloneL()
	{
	HBufC8* clone = NULL;
	if ( iEncodeBuf )
		{
		clone = iEncodeBuf->Ptr(0).AllocL();
		}
	return clone;		
	}

// -----------------------------------------------------------------------------
// CMceMsgBase::DecodeL
// -----------------------------------------------------------------------------
//
void CMceMsgBase::DecodeL( const TDesC8& aContext )
    {
    
	RDesReadStream readStream( aContext );
    CleanupClosePushL( readStream );
    
    TBool pushed = iPushed;
    iPushed = ETrue;
    
    DoDecodeL( readStream );
    
    iPushed = pushed;
    
	CleanupStack::PopAndDestroy(); // readStream
    }

// -----------------------------------------------------------------------------
// CMceMsgBase::DoDecodeL
// -----------------------------------------------------------------------------
//
void CMceMsgBase::DoDecodeL( MMceComSerializationContext& aSerCtx )
    {
    DoDecodeL( aSerCtx.ReadStream() );
    }

// -----------------------------------------------------------------------------
// CMceMsgBase::PushL
// -----------------------------------------------------------------------------
//
void CMceMsgBase::PushL()
    {
    User::LeaveIfError( iPushed ? KErrGeneral : KErrNone );
    iPushed = ETrue;
    }

// -----------------------------------------------------------------------------
// CMceMsgBase::Pop
// -----------------------------------------------------------------------------
//
void CMceMsgBase::Pop()
    {
    iPushed = EFalse;        
    }

// -----------------------------------------------------------------------------
// CMceMsgBase::OwnershipRollbackPushLC
// -----------------------------------------------------------------------------
//   
void CMceMsgBase::OwnershipRollbackPushLC()
    {
    TCleanupItem paramsCleanup( OwnershipRollback, this );
    CleanupStack::PushL( paramsCleanup );
    }
    
// -----------------------------------------------------------------------------
// CMceMsgBase::OwnershipRollback
// -----------------------------------------------------------------------------
//
void CMceMsgBase::OwnershipRollback( TAny* aMsg )
    {
    if ( !aMsg )
        {
        return;
        }
    CMceMsgBase* msg = reinterpret_cast< CMceMsgBase* >( aMsg );
    msg->DoOwnershipRollback();
    }

// -----------------------------------------------------------------------------
// CMceMsgBase::OwnershipRollback
// -----------------------------------------------------------------------------
//    
void CMceMsgBase::DoOwnershipRollback()
    {
    }
    
// ============================ MEMBER FUNCTIONS ===============================
// CMceMsgTextArray


// -----------------------------------------------------------------------------
// CMceMsgTextArray::CMceMsgTextArray
// -----------------------------------------------------------------------------
//
CMceMsgTextArray::CMceMsgTextArray()
  : CMceMsgBase( EMceItcMsgTypeTextArray ),
    iArray( NULL )
    {
    }

// -----------------------------------------------------------------------------
// CMceMsgTextArray::CMceMsgTextArray
// -----------------------------------------------------------------------------
//
CMceMsgTextArray::CMceMsgTextArray( CDesC8Array& aArray )
  : CMceMsgBase( EMceItcMsgTypeTextArray ),
    iArray( &aArray )
    {
    }
    
// -----------------------------------------------------------------------------
// CMceMsgTextArray::~CMceMsgTextArray
// -----------------------------------------------------------------------------
//
CMceMsgTextArray::~CMceMsgTextArray()
    {
    if ( iPushed )
        {
        LocalClose();
        }
    }

// -----------------------------------------------------------------------------
// CMceMsgTextArray::EncodeL
// -----------------------------------------------------------------------------
//
void CMceMsgTextArray::EncodeL()
    {
    delete iEncodeBuf;
    iEncodeBuf = NULL;
	iEncodeBuf = CBufFlat::NewL( KMceExternalizeBufferExpandSize );

	RBufWriteStream writeStream( *iEncodeBuf, 0 );
	writeStream.PushL();
	
	MceSerial::EncodeL( iArray, writeStream );
	
	CleanupStack::PopAndDestroy(); // writeStream
    }

// -----------------------------------------------------------------------------
// CMceMsgTextArray::DoDecodeL
// -----------------------------------------------------------------------------
//
void CMceMsgTextArray::DoDecodeL( RReadStream& aReadStream )
    {
    MceSerial::DecodeL( iArray, aReadStream );
    }
    
// -----------------------------------------------------------------------------
// CMceMsgTextArray::Close
// -----------------------------------------------------------------------------
//
void CMceMsgTextArray::Close()
    {
    LocalClose();
    }

// -----------------------------------------------------------------------------
// CMceMsgTextArray::LocalClose
// -----------------------------------------------------------------------------
//
void CMceMsgTextArray::LocalClose()
    {
    MCE_DELETE( iArray );
    }

// -----------------------------------------------------------------------------
// CMceMsgTextArray::Array
// -----------------------------------------------------------------------------
//
CDesC8Array* CMceMsgTextArray::Array()
    {
    MCE_MOVE_ARRPTR( iArray, ret );
    return ret;
    }


// ============================ MEMBER FUNCTIONS ===============================
// CMceMsgSIPData


// -----------------------------------------------------------------------------
// CMceMsgSIPData::CMceMsgSIPData
// -----------------------------------------------------------------------------
//
CMceMsgSIPData::CMceMsgSIPData()
  : CMceMsgBase( EMceItcMsgTypeSIPData ),
    iTrxType( (TUint32)KErrNotFound ),
    iSIPHeaders( NULL ),
	iContentType( NULL ),
	iContentTypeId( EMceContentTypeUserDefined )
    {
    }

// -----------------------------------------------------------------------------
// CMceMsgSIPData::CMceMsgSIPData
// -----------------------------------------------------------------------------
//
CMceMsgSIPData::CMceMsgSIPData( TUint32 aType,
                                CDesC8Array& aSIPHeaders,
                                HBufC8& aContentType )
  : CMceMsgBase( EMceItcMsgTypeSIPData ),
    iTrxType( aType ),
    iSIPHeaders( &aSIPHeaders ),
	iContentType ( &aContentType ),
	iContentTypeId( EMceContentTypeUserDefined )
    {   
    }

// -----------------------------------------------------------------------------
// CMceMsgSIPData::CMceMsgSIPData
// -----------------------------------------------------------------------------
//
CMceMsgSIPData::CMceMsgSIPData(TMceItcDataType aType )
  : CMceMsgBase( aType ),
    iTrxType( (TUint32)KErrNotFound ),
    iSIPHeaders( NULL ),
	iContentType( NULL ),
	iContentTypeId( EMceContentTypeUserDefined )
    {
    }

// -----------------------------------------------------------------------------
// CMceMsgSIPData::CMceMsgSIPData
// -----------------------------------------------------------------------------
//
CMceMsgSIPData::CMceMsgSIPData( TMceItcDataType aType,
                                TUint32 aTrxType,
                                CDesC8Array& aSIPHeaders,
                                HBufC8& aContentType )
  : CMceMsgBase( aType ),
    iTrxType( aTrxType ),
    iSIPHeaders( &aSIPHeaders ),
	iContentType( &aContentType ),
	iContentTypeId( EMceContentTypeUserDefined )
    {        
    }

// -----------------------------------------------------------------------------
// CMceMsgSIPData::~CMceMsgSIPData
// -----------------------------------------------------------------------------
//
CMceMsgSIPData::~CMceMsgSIPData()
    {
    if ( iPushed )
        {
        LocalClose();
        }
    }

// -----------------------------------------------------------------------------
// CMceMsgSIPData::EncodeL
// -----------------------------------------------------------------------------
//
void CMceMsgSIPData::EncodeL()
    {
    delete iEncodeBuf;
    iEncodeBuf = NULL;

	iEncodeBuf = CBufFlat::NewL( KMceExternalizeBufferExpandSize );

	RBufWriteStream writeStream( *iEncodeBuf, 0 );
	writeStream.PushL();
    
    EncodeStreamL( writeStream );

	CleanupStack::PopAndDestroy(); // writeStream   
    }

// -----------------------------------------------------------------------------
// CMceMsgSIPData::EncodeL
// -----------------------------------------------------------------------------
//
void CMceMsgSIPData::EncodeStreamL( RBufWriteStream& aWriteStream )
    {
    aWriteStream.WriteUint32L( iTrxType );
    MceSerial::EncodeL( iSIPHeaders, aWriteStream );
	MceSerial::EncodeL( iContentType, aWriteStream );
    }

// -----------------------------------------------------------------------------
// CMceMsgSIPData::DoDecodeL
// -----------------------------------------------------------------------------
//
void CMceMsgSIPData::DoDecodeL( RReadStream& aReadStream )                         
    {
    iTrxType = aReadStream.ReadUint32L(); 
    MceSerial::DecodeL( iSIPHeaders, aReadStream );
	MceSerial::DecodeL( iContentType, aReadStream );
    }

// -----------------------------------------------------------------------------
// CMceMsgSIPData::Close
// -----------------------------------------------------------------------------
//
void CMceMsgSIPData::Close()
    {
    LocalClose();
    }

// -----------------------------------------------------------------------------
// CMceMsgSIPData::LocalClose
// -----------------------------------------------------------------------------
//
void CMceMsgSIPData::LocalClose()
    {
    MCE_DELETE( iContentType );
    MCE_DELETE( iSIPHeaders );
    }

// -----------------------------------------------------------------------------
// CMceMsgSIPData::DoOwnershipRollback
// -----------------------------------------------------------------------------
//
void CMceMsgSIPData::DoOwnershipRollback()
    {
    iSIPHeaders = 0;
    iContentType = 0;
    }
    
// -----------------------------------------------------------------------------
// CMceMsgSIPData::Headers
// -----------------------------------------------------------------------------
//
CDesC8Array* CMceMsgSIPData::Headers()
    {
    MCE_MOVE_ARRPTR( iSIPHeaders, ret );
    return ret;
    }

// -----------------------------------------------------------------------------
// CMceMsgSIPData::ContentType
// -----------------------------------------------------------------------------
//
HBufC8* CMceMsgSIPData::ContentType()
    {
    MCE_MOVE_BUFPTR( iContentType, ret );
    return ret;
    }


// ============================ MEMBER FUNCTIONS ===============================
// CMceMsgSIPReply



// -----------------------------------------------------------------------------
// CMceMsgSIPReply::CMceMsgSIPReply
// -----------------------------------------------------------------------------
//
CMceMsgSIPReply::CMceMsgSIPReply()
  : CMceMsgSIPData( EMceItcMsgTypeSIPReply ),
    iReason( NULL ),
    iCode( (TUint32)KErrNotFound ),
    iReasonAsPoolIndex( KErrNone ) 
    {
    }

// -----------------------------------------------------------------------------
// CMceMsgSIPReply::CMceMsgSIPReply
// -----------------------------------------------------------------------------
//
CMceMsgSIPReply::CMceMsgSIPReply( TUint32 aTrxType,
                                  HBufC8& aReason, 
                                  TUint32 aCode,
                                  CDesC8Array& aSIPHeaders,
                                  HBufC8& aContentType )
  : CMceMsgSIPData( EMceItcMsgTypeSIPReply, aTrxType, aSIPHeaders, aContentType ),
    iReason( &aReason ),
    iCode( aCode ),
    iReasonAsPoolIndex( KErrNone ) 
    {
    }

// -----------------------------------------------------------------------------
// CMceMsgSIPReply::CMceMsgSIPReply
// -----------------------------------------------------------------------------
//
CMceMsgSIPReply::CMceMsgSIPReply( HBufC8& aReason,
                                  TUint32 aCode,
                                  CDesC8Array& aSIPHeaders,
								  HBufC8& aContentType)
  : CMceMsgSIPData( EMceItcMsgTypeSIPReply, 
                    (TUint32)KErrNotFound,
                    aSIPHeaders, aContentType ),
    iReason( &aReason ),
    iCode( aCode ),
    iReasonAsPoolIndex( KErrNone ) 
    {
    }

// -----------------------------------------------------------------------------
// CMceMsgSIPReply::~CMceMsgSIPReply
// -----------------------------------------------------------------------------
//
CMceMsgSIPReply::~CMceMsgSIPReply()
    {
    if ( iPushed )
        {
        MCE_DELETE( iReason );
        }
    }

// -----------------------------------------------------------------------------
// CMceMsgSIPReply::EncodeL
// -----------------------------------------------------------------------------
//
void CMceMsgSIPReply::EncodeL()
    {
    delete iEncodeBuf;
    iEncodeBuf = NULL;

	iEncodeBuf = CBufFlat::NewL( KMceExternalizeBufferExpandSize );

	RBufWriteStream writeStream( *iEncodeBuf, 0 );
	writeStream.PushL();
    
    CMceMsgSIPData::EncodeStreamL( writeStream );

	MceSerial::EncodeL( iReason, writeStream );
    writeStream.WriteUint32L( iCode );
	
	CleanupStack::PopAndDestroy(); // writeStream
    }

    
// -----------------------------------------------------------------------------
// CMceMsgSIPReply::DoDecodeL
// -----------------------------------------------------------------------------
//
void CMceMsgSIPReply::DoDecodeL( RReadStream& aReadStream )
    {
    CMceMsgSIPData::DoDecodeL( aReadStream );

    MceSerial::DecodeL( iReason, aReadStream );
    iCode = aReadStream.ReadUint32L();    
    }
    
// -----------------------------------------------------------------------------
// CMceMsgSIPReply::Close
// -----------------------------------------------------------------------------
//
void CMceMsgSIPReply::Close()
    {
    LocalClose();
    }

// -----------------------------------------------------------------------------
// CMceMsgSIPReply::LocalClose
// -----------------------------------------------------------------------------
//
void CMceMsgSIPReply::LocalClose()
    {
    CMceMsgSIPData::Close();
    MCE_DELETE( iReason );
    }

// -----------------------------------------------------------------------------
// CMceMsgSIPReply::Reason
// -----------------------------------------------------------------------------
//
HBufC8* CMceMsgSIPReply::Reason()
    {
    MCE_MOVE_BUFPTR( iReason, ret );
    return ret;
    }
    
    
// ============================ MEMBER FUNCTIONS ===============================
// CMceMsgSIPRequest  
    



// -----------------------------------------------------------------------------
// CMceMsgSIPRequest::CMceMsgSIPRequest
// -----------------------------------------------------------------------------
//
CMceMsgSIPRequest::CMceMsgSIPRequest()
  : CMceMsgSIPData( EMceItcMsgTypeSIPRequest ),
    iMethod( NULL )
    {
    }


// -----------------------------------------------------------------------------
// CMceMsgSIPRequest::CMceMsgSIPRequest
// -----------------------------------------------------------------------------
//


CMceMsgSIPRequest::CMceMsgSIPRequest( HBufC8& aMethod,
									CDesC8Array& aSIPHeaders,
                     				HBufC8& aContentType )
                     				
	: CMceMsgSIPData( EMceItcMsgTypeSIPRequest, 
                    				(TUint32)KErrNotFound,
                    				aSIPHeaders, 
                    				aContentType ),
                     				
  	iMethod (&aMethod)
  	   
    {
    }
    
// -----------------------------------------------------------------------------
// CMceMsgSIPRequest::~CMceMsgSIPRequest
// -----------------------------------------------------------------------------
//
CMceMsgSIPRequest::~CMceMsgSIPRequest()
    {
    if ( iPushed )
   	 {
	 MCE_DELETE( iMethod );
	 }
    }
    
// -----------------------------------------------------------------------------
// CMceMsgSIPRequest::EncodeL
// -----------------------------------------------------------------------------
//
void CMceMsgSIPRequest::EncodeL()
    {
    delete iEncodeBuf;
    iEncodeBuf = NULL;
	iEncodeBuf = CBufFlat::NewL( KMceExternalizeBufferExpandSize );

	RBufWriteStream writeStream( *iEncodeBuf, 0 );
	writeStream.PushL();
	
	MceSerial::EncodeL( iMethod, writeStream );
		    
    CMceMsgSIPData::EncodeStreamL( writeStream );
    
	CleanupStack::PopAndDestroy(); // writeStream
    }


// -----------------------------------------------------------------------------
// CMceMsgSIPRequest::DoDecodeL
// -----------------------------------------------------------------------------
//
void CMceMsgSIPRequest::DoDecodeL( RReadStream& aReadStream )
    {
    
	MceSerial::DecodeL( iMethod, aReadStream );
	CMceMsgSIPData::DoDecodeL( aReadStream );
    }


// -----------------------------------------------------------------------------
// CMceMsgSIPRequest::Close
// -----------------------------------------------------------------------------
//
void CMceMsgSIPRequest::Close()
    {
    LocalClose();
    }


// -----------------------------------------------------------------------------
// CMceMsgSIPRequest::LocalClose
// -----------------------------------------------------------------------------
//
void CMceMsgSIPRequest::LocalClose()
    {
    CMceMsgSIPData::Close();
    MCE_DELETE( iMethod );
    }
    

// -----------------------------------------------------------------------------
// CMceMsgSIPRequest::RequestMethod
// -----------------------------------------------------------------------------
//
HBufC8* CMceMsgSIPRequest::RequestMethod()
    {
    return iMethod;
    }    


// ============================ MEMBER FUNCTIONS ===============================
// CMceMsgSipEvent


// -----------------------------------------------------------------------------
// CMceMsgSipEvent::CMceMsgSipEvent
// -----------------------------------------------------------------------------
//
CMceMsgSIPEvent::CMceMsgSIPEvent()
  : CMceMsgSIPData( EMceItcMsgTypeEvent ),
    iId( ( TUint32 ) KErrNotFound ),
    iDialogId( ( TUint32 ) KErrNotFound ),
    iEventType( ( TUint32 ) EMceItcEventTypeEither ),
    iRecipient( NULL ),
    iOriginator( NULL ),
    iEventHeader( NULL ),
    iReferTo( NULL ),
    iReferType( CMceRefer::ENoSuppression ),
    iRefreshInterval( ( TUint32 ) KErrNotFound )
    {
    }

// -----------------------------------------------------------------------------
// CMceMsgSIPEvent::CMceMsgSIPEvent
// -----------------------------------------------------------------------------
//
CMceMsgSIPEvent::CMceMsgSIPEvent( 
                  TUint32 aId,
                  TUint32 aDialogId,
                  TMceItcEventType aEventType,
                  HBufC8* aRecipient,
                  HBufC8* aOriginator,                  
                  HBufC8* aEventHeader,
                  HBufC8* aReferTo,
                  CMceRefer::TType aReferType,
                  TUint32 aRefreshInterval,
                  CDesC8Array* aSIPHeaders,
                  HBufC8* aContentType )
  : CMceMsgSIPData( EMceItcMsgTypeEvent, 
                    (TUint32)KErrNotFound, 
                    *aSIPHeaders, 
                    *aContentType ),
    iId( aId ),
    iDialogId( aDialogId ),
    iEventType( aEventType ),
    iRecipient( aRecipient ),
    iOriginator( aOriginator ),
    iEventHeader( aEventHeader ),
    iReferTo( aReferTo ),
    iReferType( aReferType ),
    iRefreshInterval( aRefreshInterval )
    {
    }

// -----------------------------------------------------------------------------
// CMceMsgSIPEvent::CMceMsgSIPEvent
// -----------------------------------------------------------------------------
//
CMceMsgSIPEvent::CMceMsgSIPEvent( TUint32 aId,
                     TUint32 aDialogId,
                     TMceItcEventType aEventType,
                     CMceRefer::TType aReferType,
                     TUint32 aRefreshInterval,
                     CDesC8Array* aSIPHeaders,
                     HBufC8* aContentType )
  : CMceMsgSIPData( EMceItcMsgTypeEvent, 
                    (TUint32)KErrNotFound,
                    *aSIPHeaders, 
                    *aContentType ),
    iId( aId ),
    iDialogId( aDialogId ),
    iEventType( aEventType ),
    iRecipient( NULL ),
    iOriginator( NULL ),
    iEventHeader( NULL ),
    iReferTo( NULL ),
    iReferType( aReferType ),
    iRefreshInterval( aRefreshInterval )
    {
    }

// -----------------------------------------------------------------------------
// CMceMsgSIPEvent::~CMceMsgSIPEvent
// -----------------------------------------------------------------------------
//
CMceMsgSIPEvent::~CMceMsgSIPEvent()
    {
    MCE_DELETE( iRecipient );
    MCE_DELETE( iOriginator );
    MCE_DELETE( iEventHeader );
    MCE_DELETE( iReferTo );
    MCE_DELETE( iSIPHeaders );
    MCE_DELETE( iContentType );
    }
    
// -----------------------------------------------------------------------------
// CMceMsgSIPEvent::EncodeL
// -----------------------------------------------------------------------------
//
void CMceMsgSIPEvent::EncodeL()
    {
    delete iEncodeBuf;
    iEncodeBuf = NULL;
	iEncodeBuf = CBufFlat::NewL( KMceExternalizeBufferExpandSize );

	RBufWriteStream writeStream( *iEncodeBuf, 0 );
	writeStream.PushL();
	writeStream.WriteUint32L( iId );
	writeStream.WriteUint32L( iDialogId );
	writeStream.WriteUint32L( iEventType );
	MceSerial::EncodeL( iRecipient, writeStream );
	MceSerial::EncodeL( iOriginator, writeStream );
	MceSerial::EncodeL( iEventHeader, writeStream );
	MceSerial::EncodeL( iReferTo, writeStream );
    writeStream.WriteUint32L( iReferType );
    writeStream.WriteUint32L( iRefreshInterval );
    
    CMceMsgSIPData::EncodeStreamL( writeStream );
    
	CleanupStack::PopAndDestroy(); // writeStream
    }

// -----------------------------------------------------------------------------
// CMceMsgSipEvent::DoDecodeL
// -----------------------------------------------------------------------------
//
void CMceMsgSIPEvent::DoDecodeL( RReadStream& aReadStream )
    {
    iId = aReadStream.ReadUint32L();
    iDialogId = aReadStream.ReadUint32L();
    iEventType = aReadStream.ReadUint32L();
	MceSerial::DecodeL( iRecipient, aReadStream );
	MceSerial::DecodeL( iOriginator, aReadStream );
	MceSerial::DecodeL( iEventHeader, aReadStream );
	MceSerial::DecodeL( iReferTo, aReadStream );
	iReferType = aReadStream.ReadUint32L();
    iRefreshInterval = aReadStream.ReadUint32L();
    CMceMsgSIPData::DoDecodeL( aReadStream );
    }
    
// -----------------------------------------------------------------------------
// CMceMsgSIPEvent::Close
// -----------------------------------------------------------------------------
//
void CMceMsgSIPEvent::Close()
    {
    }

// -----------------------------------------------------------------------------
// CMceMsgSIPEvent::LocalClose
// -----------------------------------------------------------------------------
//
void CMceMsgSIPEvent::LocalClose()
    {
    }

// -----------------------------------------------------------------------------
// CMceMsgSIPEvent::DoOwnershipRollback
// -----------------------------------------------------------------------------
//
void CMceMsgSIPEvent::DoOwnershipRollback()
    {
    iRecipient = 0;
    iOriginator = 0;
    iEventHeader = 0;
    iReferTo = 0;
    iSIPHeaders = 0;
    iContentType = 0;
    }
    
// -----------------------------------------------------------------------------
// CMceMsgSIPEvent::Id
// -----------------------------------------------------------------------------
//
TUint32 CMceMsgSIPEvent::Id()
    {
    return iId;
    }
	
// -----------------------------------------------------------------------------
// CMceMsgSIPEvent::DialogId
// -----------------------------------------------------------------------------
//
TUint32 CMceMsgSIPEvent::DialogId()
    {
    return iDialogId;
    }
	
// -----------------------------------------------------------------------------
// CMceMsgSIPEvent::EventType
// -----------------------------------------------------------------------------
//
TUint32 CMceMsgSIPEvent::EventType()
    {
    return iEventType;
    }
	
// -----------------------------------------------------------------------------
// CMceMsgSIPEvent::Recipient
// -----------------------------------------------------------------------------
//
HBufC8* CMceMsgSIPEvent::Recipient()
    {
    MCE_MOVE_BUFPTR( iRecipient, ret );
    return ret;
    }
    
// -----------------------------------------------------------------------------
// CMceMsgSIPEvent::Originator
// -----------------------------------------------------------------------------
//
HBufC8* CMceMsgSIPEvent::Originator()
    {
    MCE_MOVE_BUFPTR( iOriginator, ret );
    return ret;
    }
    
// -----------------------------------------------------------------------------
// CMceMsgSIPEvent::EventHeader
// -----------------------------------------------------------------------------
//
HBufC8* CMceMsgSIPEvent::EventHeader()
    {
    MCE_MOVE_BUFPTR( iEventHeader, ret );
    return ret;
    }
    
// -----------------------------------------------------------------------------
// CMceMsgSIPEvent::ReferTo
// -----------------------------------------------------------------------------
//
HBufC8* CMceMsgSIPEvent::ReferTo()
    {
    MCE_MOVE_BUFPTR( iReferTo, ret );
    return ret;
    }
    
// -----------------------------------------------------------------------------
// CMceMsgSIPEvent::ReferType
// -----------------------------------------------------------------------------
//
TUint32 CMceMsgSIPEvent::ReferType()
    {
    return iReferType;
    }

// -----------------------------------------------------------------------------
// CMceMsgSIPEvent::RefreshInterval
// -----------------------------------------------------------------------------
//
TUint32 CMceMsgSIPEvent::RefreshInterval()
    {
    return iRefreshInterval;
    }
    
// -----------------------------------------------------------------------------
// CMceMsgSIPEvent::SIPHeaders
// -----------------------------------------------------------------------------
//
CDesC8Array* CMceMsgSIPEvent::SIPHeaders()
    {
    MCE_MOVE_ARRPTR( iSIPHeaders, ret );
    return ret;
    }
    
// -----------------------------------------------------------------------------
// CMceMsgSIPEvent::ContentType
// -----------------------------------------------------------------------------
//
HBufC8* CMceMsgSIPEvent::ContentType()
    {
    MCE_MOVE_BUFPTR( iContentType, ret );
    return ret;
    }



// End of File
