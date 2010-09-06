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




#ifndef CMCECOMMESSAGESTREAM_H
#define CMCECOMMESSAGESTREAM_H

#include <e32std.h>         
#include "mcecommediastream.h"
#include <mcemessagestream.h>


class CMceComMessageCodec;

/**
 * 
 *
 * @lib 
 */
class CMceComMessageStream : public CMceComMediaStream
    {

public: // Constructors and destructor

    /**
    * Constructor
    */
    static CMceComMessageStream* NewL();

    /**
    * Constructor
    */
    static CMceComMessageStream* NewLC();

    /**
    * Destructor.
    */
    ~CMceComMessageStream();

public: // Functions


    /**
    * Removes codec from the stream.
    * @param aCodec codec to be added to the stream.
    *        Ownership is transferred.
    */
    void RemoveCodecL( CMceComCodec* aCodec );
    
	/**
	* Clears list of codecs and deletes each codec in it.
	*/
    void DestroyCodecs();    
    /**
    * Searches for codec matching with input codec instance or sdp name
    * @param aCodec the search term
    * @return matching codec or NULL if not found
    */
    CMceComCodec* FindCodecL( CMceComCodec& aCodec );
    
    /**
    * Adds codec to the stream.
    * @param aCodec codec to be added to the stream.
    *        Ownership is transferred.
    */    
     void AddCodecL( CMceComCodec* aCodec);

    
    /**
    * Returns the available codecs to the session.
    * @return codecs available for the session.
    */
    RPointerArray<CMceComMessageCodec>& Codecs();
    void ReorderCodecs();
    
    /**
    * Return pointer to codec from the list of codecs.
    * aIndex index of codec to get.
    */
    CMceComMessageCodec* CodecL( TInt aIndex ) const;
    
    /**
    * Returns number of codecs in the stream.
    */
    TInt CodecCount() const;


public: // from CMceComMediaStream


    /**
    * Internalizes flat data
    * @param aReadStream read stream
    */
    void InternalizeFlatL( RReadStream& aReadStream );

    /**
    * Externalizes flat data
    * @param aWriteStream write stream
    */
    void ExternalizeFlatL( RWriteStream& aWriteStream );
    
    /**
    * Internalizes
    * @param aReadStream read stream
    */
    void InternalizeL( MMceComSerializationContext& aSerCtx );
    
    /**
    * Externalizes
    * @param aWriteStream write stream
    */
    void ExternalizeL( MMceComSerializationContext& aSerCtx );


public://from CMceComMediaStream


    /**
    * Initializes 
    * @param aParent the parent
    */
    void InitializeL( CMceComSession& aParent );

    /**
    * Updates 
    * @param aStream new stream
    */
    void UpdateL( CMceComMediaStream& aStream );

    /**
    * Clones
    */
    CMceComMediaStream* CloneL();
    

#ifdef MCE_COMMON_SERVER_SIDE

    /**
    * Synchronizes stream's codecs
    * @param aRoleAnswerer is answerer
    */
    void SynchronizeL( TBool aRoleAnswerer );
    
    /**
    * Update default codec to reflect current negotiation
    */
    void UpdateDefaultCodecL();

    /**
    * Prepares stream for offer/answer
    */
    void PrepareL();
   
    /**
    * Return mcc stream type
    * @return mcc stream type
    */
    TInt MccStreamType() const;

    /**
    * Return mcc link type
    * @return mcc link type
    */
    TInt MccLinkType() const;


#endif//MCE_COMMON_SERVER_SIDE
    
public:
    TBool iUseMsrpPathAttrForHostAddrAndPort;
    CMceMessageStream::TConnSetup iConnSetup;
    TBool iMsrpConnUsage;
    
protected: // Functions

    /**
    * Constructor
    */
    CMceComMessageStream();
    
    
    /**
    * second-phase constructor
    */
    void ConstructL();
    
    /**
     * second-phase copy constructor
     */
    void ConstructL( CMceComMessageStream& aStream );    
    

protected: // Owned data

    /**
     * codecs
     */
    //The data path will not be handled in MCC for message type streams
    RPointerArray<CMceComMessageCodec> iCodecs;

    };



#endif
