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




#ifndef MCESERIAL_H
#define MCESERIAL_H

#include <e32std.h>  	
#include <s32strm.h>
#include <s32mem.h>
#include <e32des8.h>

#include "mcerefer.h"
#include "mcecomsession.h"
#include "mcecomaudiostream.h"
#include "mcecomvideostream.h"
#include "mcecomstreambundle.h"

#include "mceclientserver.h"
#include "mcefactory.h"
#include "mcecomfactory.h"
#include "mcecomserializationcontext.h"

class CMceManager;
class CSIPProfile;


#define MCE_DELETE( val ) delete val;val=NULL
#define MCE_HAS_DATA( buf ) ( buf && buf->Length() > 0 )
#define MCE_MOVE_BUFPTR( from, to ) HBufC8* to = from;from=NULL
#define MCE_MOVE_ARRPTR( from, to ) CDesC8Array* to = from;from=NULL
#define MCE_IS_ERROR( error ) ( error < KErrNone )
#define MCE_NEED_TO_SEND( session, receivedSdp )\
    ( session.NeedToSend( receivedSdp ) ||\
      session.FCActions().NeedToSendOffer() )
#define MCE_NEED_TO_RECEIVE( session ) session.NeedToReceive()
#define MCE_FORCED_TO_RECEIVE( session ) session.ForceSDPSendingStatus()

_LIT8( KMceEmpty, "" );


enum TMceContentType
    {
    EMceContentTypeUserDefined,
    EMceContentTypeSDP,
    EMceContentTypeMultiPart
    };



/**
 * static class providing decoding and  encoding
 * functionality of basic data types.
 *
 * @lib 
 */
class MceSerial
    {
public:

    /**
     * Decodes descriptor from stream
     * @param aString placeholder for data to be read from stream
     * @param aReadStream stream from which data is decoded
     **/
    static void DecodeL( TDes8& aString, RReadStream& aReadStream );

    /**
    * Encodes descriptor to stream
    * @param aString string to be encoded to stream
    * @param aWriteStream stream to which string is encoded
    **/
    static void EncodeL( const TDesC8& aString, RWriteStream& aWriteStream );
    
    /**
     * Encodes string buffer to stream. If buffer is NULL empty string is encoded
     * @param aString string buffer to be encoded to stream
     * @param aWriteStream stream to which string is encoded
     **/
    static void EncodeL( HBufC8* aString, RWriteStream& aWriteStream );
    
    /**
     * Decodes string buffer from stream
     * @param aBuffer placeholder for data to be read from stream
     * @param aReadStream stream from which data is decoded
     **/
    static void DecodeL( HBufC8*& aBuffer, RReadStream& aReadStream );

    /**
     * Decodes descriptor array from stream
     * @param aArray placeholder for data to be read from stream
     * @param aReadStream stream from which data is decoded
     **/
    static void DecodeL( CDesC8Array*& aArray, RReadStream& aReadStream );

    /**
     * Encodes descriptor array  to stream. If arrays is NULL empty arrays is encoded
     * @param aArray  descriptor array to be encoded to stream
     * @param aWriteStream stream to which string is encoded
     **/
    static void EncodeL( CDesC8Array* aArray, RWriteStream& aWriteStream );
    
    /**
     * Decodes media id from stream
     * @param aId placeholder for data to be read from stream
     * @param aReadStream stream from which data is decoded
     **/
    static void DecodeL( TMceMediaId& aId, RReadStream& aReadStream );

    /**
     * Encodes media id to stream. 
     * @param aId media id  to be encoded to stream
     * @param aWriteStream stream to which media id is encoded
     **/
    static void EncodeL( TMceMediaId aId, RWriteStream& aWriteStream );
	
	static void EncodeL( RArray<TMceCryptoContext>& aArray, RWriteStream& aWriteStream  );
    
    static void DecodeL( RArray<TMceCryptoContext>& aArray, RReadStream& aReadStream );
    };
    


/**
 * Base class of messages passed between client and server
 *
 * @lib 
 */
class CMceMsgBase : public CBase
    {
        
public:

    /**
    * C++ constructor
    * @param aType  the type of message
    **/
    CMceMsgBase( TMceItcDataType aType );
    
    /**
    * C++ destructor
    **/
    ~CMceMsgBase();

    /**
     * Encodes message
     **/
    virtual void EncodeL() = 0;

    /**
     * Decodes message from descriptor
     * @param aContext encoded string representing this message
     **/
    void DecodeL( const TDesC8& aContext );

    /**
    * Decodes message from stream
    * @param aReadStream encoded string representing 
    * this message as stream
    * @param aReadStream the stream
    **/
    virtual void DoDecodeL( RReadStream& aReadStream ) = 0;
    
    /**
    * Decodes message from stream
    * @param aSerCtx containing stream as encoded string representing 
    * this message as stream
    **/
    virtual void DoDecodeL( MMceComSerializationContext& aSerCtx );
        
    /**
    * Returns type of message
    * @returnt ype of message
    **/
    TMceItcDataType Type();
        
    /**
    * Returns buffer to which messge has been encoded
    * @return buffer to which messge has been encoded
    **/
    CBufFlat& EncodeBuffer();

    /**
    * Clones encode buffer
    * @return cloned encode buffer
    **/
    HBufC8* EncodeBufferCloneL();

    /**
    * Pushes all (encoded) data to cleanup stack
    **/
	virtual void PushL();

    /**
    * Pops all (encoded) data from cleanup stack
    **/
	virtual void Pop();
	    
    /**
    * Destroys all (encoded) data
    **/
    virtual void Close() = 0;
    
    /**
    * Handles ownership rollback of leave situations.
    **/
    void OwnershipRollbackPushLC();
    
protected:

    /**
	* TCleanupItem funtion.
	*/
	static void OwnershipRollback( TAny* aData );
	
    /**
    * Releases ownership of certain owned member data.
    */
    virtual void DoOwnershipRollback();
    	    
protected://output

    CBufFlat* iEncodeBuf;
    
    TMceItcDataType iType;
    
    TBool iPushed;
    
    };


/**
 * Message for transferring descriptor array
 * between client and server
 *
 * @lib 
 */
class CMceMsgTextArray : public CMceMsgBase
    {
public:

    /**
    * C++ constructor for decoding
    **/
    CMceMsgTextArray();
    
    /**
    * C++ constructor for encoding
    * @param aArray data to be encoded
    **/
    CMceMsgTextArray( CDesC8Array& aArray );

    /**
    * Destructor
    **/
    ~CMceMsgTextArray();
    
    /**
     * Encodes message
     **/
    void EncodeL();
    
    /**
    * Decodes message from stream
    * @param aReadStream encoded string representing 
    * this message as stream
    * @param aReadStream the stream
    **/
    void DoDecodeL( RReadStream& aReadStream );

    /**
    * Destroys all (encoded) data
    **/
	void Close();
	
    /**
    * Destroys all (encoded) data
    **/
	void LocalClose();
	
    /**
    * Transfers ownership
    * @return array 
    **/
    CDesC8Array* Array();
	
                   
public://input/output
    
    CDesC8Array* iArray;
    };



/**
 * Message for transferring SIP data
 * between client and server
 *
 * @lib 
 */
class CMceMsgSIPData : public CMceMsgBase
    {


public:

    /**
    * C++ constructor for decoding
    **/
    CMceMsgSIPData();

    
    /**
    * C++ constructor for encoding
    * @param aTrxType data to be encoded
    * @param aSIPHeaders data to be encoded
    * @param aContentType data to be encoded
    **/
    CMceMsgSIPData( TUint32 aTrxType,
                    CDesC8Array& aSIPHeaders,
                    HBufC8& aContentType );
    
    /**
    * Destructor
    **/
    ~CMceMsgSIPData();
    
protected:
                     
    /**
    * C++ constructor for encoding
    * @param aType type of message
    **/
    CMceMsgSIPData( TMceItcDataType aType );
    
    /**
    * C++ constructor for encoding
    * @param aType type of message
    * @param aTrxType data to be encoded
    * @param aSIPHeaders data to be encoded
    * @param aContentType data to be encoded
    **/
    CMceMsgSIPData( TMceItcDataType aType,
                    TUint32 aTrxType,
                    CDesC8Array& aSIPHeaders,
                    HBufC8& aContentType );
                    
    /**
    * Encodes message to stream
    * @param aWriteStream the stream
    **/
    void EncodeStreamL( RBufWriteStream& aWriteStream );


public:
                    
    /**
     * Encodes message
     **/
    void EncodeL();
    
    /**
    * Decodes message from stream
    * @param aReadStream encoded string representing 
    * this message as stream
    * @param aReadStream the stream
    **/
    void DoDecodeL( RReadStream& aReadStream );
    
    /**
    * Destroys all (encoded) data
    **/
	void Close();

    /**
    * Destroys all (encoded) data
    **/
	void LocalClose();
	
    /**
    * Transfers ownership
    * @return SIP headers 
    **/
    CDesC8Array* Headers();

    /**
    * Transfers ownership
    * @return content type 
    **/
    HBufC8* ContentType();

protected:

    /**
    * Releases ownership of certain owned member data.
    */
    void DoOwnershipRollback();
                      
public://input/output
    
    //method code from SipStrings
    TUint32 iTrxType;
    
    //sip headers
    CDesC8Array* iSIPHeaders;
    
    //content type
    HBufC8* iContentType;
    
public://internal 

    TMceContentType iContentTypeId;
    
    };


/**
 * Message for transferring SIP reply
 * between client and server
 *
 * @lib 
 */
class CMceMsgSIPReply : public CMceMsgSIPData
    {
public:

    /**
    * C++ constructor for decoding
    **/
    CMceMsgSIPReply();

    /**
    * C++ constructor for encoding
    * @param aTrxType data to be encoded
    * @param aReason data to be encoded
    * @param aCode data to be encoded
    * @param aSIPHeaders data to be encoded
    * @param aContentType data to be encoded
    **/
    CMceMsgSIPReply( TUint32 aTrxType,
                     HBufC8& aReason, TUint32 aCode,
                     CDesC8Array& aSIPHeaders,
                     HBufC8& aContentType );
    
    /**
    * C++ constructor for encoding
    * @param aReason data to be encoded
    * @param aCode data to be encoded
    * @param aSIPHeaders data to be encoded
    * @param aContentType data to be encoded
    **/
    CMceMsgSIPReply( HBufC8& aReason, TUint32 aCode,
                     CDesC8Array& aSIPHeaders,
                     HBufC8& aContentType );
                     
    /**
    * Destructor
    **/
    ~CMceMsgSIPReply();
    
    /**
    * Encodes message
    **/
    void EncodeL();
    
    /**
    * Decodes message from stream
    * @param aReadStream encoded string representing 
    * this message as stream
    * @param aReadStream the stream
    **/
    void DoDecodeL( RReadStream& aReadStream );
    
    /**
    * Destroys all (encoded) data
    **/
	void Close();

    /**
    * Destroys all (encoded) data
    **/
	void LocalClose();
	
    /**
    * Transfers ownership
    * @return reason 
    **/
    HBufC8* Reason();
                   
public://input/output
    
    HBufC8* iReason;
    TUint32 iCode;
    
public://internal

    TInt iReasonAsPoolIndex;    
    
    };
/** 
 * Message for transferring SIP request
 * between client and server
 *
 * @lib 
 */
class CMceMsgSIPRequest : public CMceMsgSIPData
    {
public:

    /**
    * C++ constructor for decoding
    **/
    CMceMsgSIPRequest();

       
    /**
    * C++ constructor for encoding
    * @param aReason data to be encoded
    * @param aCode data to be encoded
    * @param aSIPHeaders data to be encoded
    * @param aContentType data to be encoded
    **/
    CMceMsgSIPRequest( HBufC8& aMethod,
                     CDesC8Array& aSIPHeaders,
                     HBufC8& aContentType );
                     
    /**
    * Destructor
    **/
    ~CMceMsgSIPRequest();
    
    
    /**
    * Encodes message
    **/
    void EncodeL();
    
    /**
    * Decodes message from stream
    * @param aReadStream encoded string representing 
    * this message as stream
    * @param aReadStream the stream
    **/
    void DoDecodeL( RReadStream& aReadStream );



private:

    /**
    * Destroys all (encoded) data
    **/
	void Close();

    /**
    * Destroys all (encoded) data
    **/
	void LocalClose();
	
	
	
public:

    // Returns the request Method, ownership is transfered.
    HBufC8* RequestMethod();
    

private://input/output
    
    HBufC8* iMethod;
    };



/**
 * Message for transferring SIP event
 * between client and server
 *
 * @lib 
 */
class CMceMsgSIPEvent : public CMceMsgSIPData
    {
public:

    /**
    * C++ constructor for decoding
    **/
    CMceMsgSIPEvent();
    
    /**
    * C++ constructor for encoding
    * @param aId data to be encoded
    * @param aDialogId data to be encoded
    * @param aEventType data to be encoded
    * @param aRecipient data to be encoded
    * @param aOriginator data to be encoded
    * @param aEventHeader data to be encoded
    * @param aReferTo data to be encoded
    * @param aReferType data to be encoded
    * @param aRefreshInterval data to be encoded
    * @param aSIPHeaders data to be encoded
    * @param aContentType data to be encoded
    **/
    CMceMsgSIPEvent( TUint32 aId,
                     TUint32 aDialogId,
                     TMceItcEventType aEventType,
                     HBufC8* aRecipient,
                     HBufC8* aOriginator,
                     HBufC8* aEventHeader,
                     HBufC8* aReferTo,
                     CMceRefer::TType aReferType,
                     TUint32 aRefreshInterval,
                     CDesC8Array* aSIPHeaders,
                     HBufC8* aContentType );

    /**
    * C++ constructor for encoding
    * @param aId data to be encoded
    * @param aDialogId data to be encoded
    * @param aEventType data to be encoded
    * @param aReferType data to be encoded
    * @param aRefreshInterval data to be encoded
    * @param aSIPHeaders data to be encoded
    * @param aContentType data to be encoded
    **/
    CMceMsgSIPEvent( TUint32 aId,
                     TUint32 aDialogId,
                     TMceItcEventType aEventType,
                     CMceRefer::TType aReferType,
                     TUint32 aRefreshInterval,
                     CDesC8Array* aSIPHeaders,
                     HBufC8* aContentType );
                  
    ~CMceMsgSIPEvent();

    /**
    * Encodes message
    **/
    void EncodeL();
    
    /**
    * Decodes message from stream
    * @param aReadStream encoded string representing 
    * this message as stream
    * @param aReadStream the stream
    **/
    void DoDecodeL( RReadStream& aReadStream );

private:

    /**
    * Destroys all (encoded) data
    **/
	void Close();

    /**
    * Destroys all (encoded) data
    **/
	void LocalClose();

public:
	// Getters for members
	
	// Returns the ID.
	TUint32 Id();
	
	// Returns the dialog ID.
	TUint32 DialogId();
	
	// Returns the event type.
	TUint32 EventType();
	
	// Returns the recipient, ownership is transfered.
    HBufC8* Recipient();
    
    // Returns the originator, ownership is transfered.
    HBufC8* Originator();
    
    // Returns the event header, ownership is transfered.
    HBufC8* EventHeader();
    
    // Returns the refer-to header, ownership is transfered.
    HBufC8* ReferTo();
    
    // Returns the refer type.
	TUint32 ReferType();
	
	// Returns the refresh interval.
	TUint32 RefreshInterval();
    
    // Returns the SIP headers, ownership is transfered.
    CDesC8Array* SIPHeaders();
    
    // Returns the content type, ownership is transfered.
    HBufC8* ContentType();

protected:

    /**
    * Releases ownership of certain owned member data.
    */
    void DoOwnershipRollback();
        
private://input/output
    
    TUint32 iId;
    TUint32 iDialogId;
    TUint32 iEventType;
    HBufC8* iRecipient;
    HBufC8* iOriginator;
    HBufC8* iEventHeader;
    HBufC8* iReferTo;
    TUint32 iReferType;
    TUint32 iRefreshInterval;
    
    //for testing

	MCE_UNIT_TEST_DEFS	
    };
    


/**
 * Message for transferring objects between client and server
 * The class of object must implement:
 * - ExternalizeL( RWriteStream& aWriteStream )
 * - Factory(), which implements CreateLC( RReadStream& aReadStream )
 *
 * @lib 
 */
template <class T> 
class CMceMsgObject : public CMceMsgBase
    {
public:

    /**
    * C++ constructor for decoding
    **/
    inline CMceMsgObject();
    
    /**
    * C++ constructor for encoding
    * @param aObject data to be encoded
    * @param aItcType data to be encoded
    **/
    inline CMceMsgObject( T& aObject, TMceItcDataType aItcType );

    /**
    * C++ destructor
    **/
    inline ~CMceMsgObject();
    
    /**
    * Encodes message
    **/
    inline void EncodeL();
    
    /**
    * Decodes message from stream
    * @param aReadStream encoded string representing 
    * this message as stream
    * @param aReadStream the stream
    **/
    inline void DoDecodeL( RReadStream& aReadStream );
    
    /**
    * Decodes message from stream
    * @param aSerCtx containing stream as encoded string representing 
    * this message as stream
    **/
    inline void DoDecodeL( MMceComSerializationContext& aSerCtx );

    /**
    * Transfers ownership
    * @return object 
    **/
    inline T* Object();
    
    /**
    * Destroys all (encoded) data
    **/
	inline void Close();

    /**
    * Destroys all (encoded) data
    **/
	inline void LocalClose();
                       
public://input/output
    
    T* iObject;
        
    };


/**
 * Message for transferring arrays of messages 
 * between client and server
 *
 * @lib 
 */
template <class T> 
class CMceMsgArray : public CMceMsgBase
    {
public:

    /**
    * C++ constructor for encoding/decoding
    * aFactory factory, which can encode messages
    * based on message type
    **/
    inline CMceMsgArray( T& aFactory );

    inline ~CMceMsgArray();

public:
                    
    /**
    * Encodes message
    **/
    inline void EncodeL();
        
    /**
    * Decodes message from stream
    * @param aReadStream encoded string representing 
    * this message as stream
    * @param aReadStream the stream
    **/
    inline void DoDecodeL( RReadStream& aReadStream );
    
    /**
    * Destroys all (encoded) data
    **/
	inline void Close();

    /**
    * Destroys all (encoded) data
    **/
	inline void LocalClose();
                   
public://input/output
    
    RPointerArray<CMceMsgBase> iArray;
    T& iFactory;
        
    };


	    
/**
 * One-way stream serializer for Server and Client side stream
 *
 * @lib 
 */
template <class T>
class TMceMediaStreamSerializer
    {
    
public:

    /**
    * Constructor for internalization for server side
    * @param aStream the stream
    */
    inline TMceMediaStreamSerializer( T& aStream );

    /**
    * Constructor for internalization for client side
    * @param aStream the stream
    * @param aFlatData to which flat data is fetched
    */
    inline TMceMediaStreamSerializer( T& aStream, CMceComMediaStream* aFlatData );

    /**
    * Internalizes
    * @param aSerCtx context for serialization
    */
    inline void InternalizeL( MMceComSerializationContext& aSerCtx );

    /**
    * Externalizes
    * @param aSerCtx context for serialization
    */
    inline void ExternalizeL( MMceComSerializationContext& aSerCtx );


private:

    /**
    * Server / client side stream
    */
    T& iStream;

    /**
    * Flat data container
    */
    CMceComMediaStream* iFlatData;
    
    };
    


/**
 * Session serializer for Server and Client side session
 *
 * @lib 
 */
template <class T>
class TMceSessionSerializer
    {
    
public:

    /**
    * Constructor for internalization for server side
    * @param aSession the session
    */
    inline TMceSessionSerializer( T& aSession );

    /**
    * Constructor for internalization for client side
    * @param aSession the session
    * @param aFlatData to which flat data is fetched
    */
    inline TMceSessionSerializer( T& aSession, CMceComSession*  aFlatData );

    /**
    * Internalizes
    * @param aSerCtx context for serialization
    */
    inline void InternalizeL( MMceComSerializationContext& aSerCtx );

    /**
    * Externalizes
    * @param aSerCtx context for serialization
    */
    inline void ExternalizeL( MMceComSerializationContext& aSerCtx );


private:

    /**
    * Server / client side session
    */
    T& iSession;

    /**
    * Flat data container
    */
    CMceComSession* iFlatData;
    
    };
    

/**
 * audio stream serializer for Server and Client side stream
 *
 * @lib 
 */
template <class T>
class TMceAudioStreamSerializer
    {
    
public:

    /**
    * Constructor for internalization for server side
    * @param aStream the stream
    */
    inline TMceAudioStreamSerializer( T& aStream );

    /**
    * Internalizes
    * @param aSerCtx context for serialization
    */
    inline void InternalizeL( MMceComSerializationContext& aSerCtx );

    /**
    * Externalizes
    * @param aSerCtx context for serialization
    */
    inline void ExternalizeL( MMceComSerializationContext& aSerCtx );


private:

      
    /**
    * Server / client side stream
    */
    T& iAudioStream;

    };


/**
 * Video stream serializer for Server and Client side stream
 *
 * @lib 
 */
template <class T>
class TMceVideoStreamSerializer
    {
    
public:

    /**
    * Constructor for internalization for server side
    * @param aStream the stream
    */
    inline TMceVideoStreamSerializer( T& aStream );

    /**
    * Internalizes
    * @param aSerCtx context for serialization
    */
    inline void InternalizeL( MMceComSerializationContext& aSerCtx );

    /**
    * Externalizes
    * @param aSerCtx context for serialization
    */
    inline void ExternalizeL( MMceComSerializationContext& aSerCtx );


private:

      
    /**
    * Server / client side stream
    */
    T& iVideoStream;

    };




#include "mceserial.inl"


#endif 