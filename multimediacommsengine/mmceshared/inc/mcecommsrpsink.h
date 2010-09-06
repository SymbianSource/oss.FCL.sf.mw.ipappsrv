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




#ifndef CMCECOMMSRPSINK_H
#define CMCECOMMSRPSINK_H

#include <e32std.h>
#include "mcecommediasink.h"
#include<e32cmn.h>
#include<uri8.h>
#include<badesca.h>

//FORWARD DECLARATIONS

/**
 * 
 *
 * @lib 
 */
class CMceComMsrpSink: public CMceComMediaSink
    {

public: // Constructors and destructor


    /**
    * Constructor
    */
    static CMceComMsrpSink* NewL();

    /**
    * Constructor
    */
    static CMceComMsrpSink* NewLC();

    /**
    * Destructor.
    */
    ~CMceComMsrpSink();

public: // from CMceComMediaSource

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
    * Clones this object
    * @return the cloned object
    */
    CMceComMediaSink* CloneL();
    
    /**
     * Updates this object
     */
    
    void UpdateFileShareAttrL( CMceComMsrpSink& aUpdate );
    
    /**
    * Updates this object
    * @param aUpdate object updates this
    */
    void UpdateL( CMceComMediaSink& aUpdate );
    
    /**
     * Should not use RTCP for msrp sessions
     * return EFalse, if not required
     */
    TBool UseRTCP();
    
#ifdef MCE_COMMON_SERVER_SIDE

    /**
    * Returns mcc type for mcc endpoint
    * @return mcc type
    */
    const TUid MccType();
        
    /**
    * Prepares this endpoint
    */
    void PrepareL();
    

#endif//MCE_COMMON_SERVER_SIDE
 
public:
    RPointerArray <TUriC8> iPath;    
    HBufC8* iSessInfo;
    CDesC8ArrayFlat iAcceptTypes; 
    TBuf8<50> iAcceptWrappedTypes;
    TBool iWrappedTypes;
    TBool iFileShare;
    HBufC* iFileName;
    TUint      iFileSize;
    HBufC8*  iFileType;
    TBuf8<32>      iFileTransferid;
    TBuf8<25>  iDispositionValue;
    TBool iSecureConnection;
    HBufC*    iFilePath;
    TBool iFTProgressNotification;

protected:

    /**
    * C++ default constructor.
    * @param type of source
    */
    CMceComMsrpSink();
    
    void ConstructL(CMceComMsrpSink &aObj);
    
    void ConstructL();
    
    #ifdef EUNIT_TEST
    friend class UT_CMceComMsrpSink;
    #endif


    };


#endif 
