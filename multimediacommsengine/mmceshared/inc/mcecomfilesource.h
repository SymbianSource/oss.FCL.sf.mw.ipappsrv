/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef CMCECOMFILESOURCE_H
#define CMCECOMFILESOURCE_H

#include <e32std.h>  
#include "mcecommediasource.h"
#include "mcecomcodec.h"


class CMceCsSessionImplementation;


class TMceFileInfo
    {
    
public:    // Data

    enum TAccessType
        {
        EZero,
        EWrite,
        ERead,
        EReadWrite
        };
        
public:    // Data

    TFileName iFileName;
    TTimeIntervalMicroSeconds iDuration;
    TInt iAudioElementCount;
    TInt iVideoElementCount;
    TMceComCodecName iAudioCodec;      
    TMceComCodecName iVideoCodec;      
    TReal iVideoFrameRate;
    TUint32 iVideoBitRate;
    TSize iVideoFrameSize;
	TUint32 iAudioBitRate;
	TAccessType iDirection;
	TAccessType iAccessRights;
	
	inline TMceFileInfo() 
	    : iFileName(0),
	      iDuration(0),	
	      iAudioElementCount(0),
	      iVideoElementCount(0),
	      iAudioCodec(0),      
    	  iVideoCodec(0),
    	  iVideoFrameRate(0),
	      iVideoBitRate(0),
	      iVideoFrameSize(0,0),
	      iAudioBitRate(0),
	      iDirection( ERead ),
	      iAccessRights( EReadWrite ){}
	      
    };


class TMceTranscodeInfo
    {

public:    // Data

    TUint32 iTranscodeSessionId;
    TFileName iFileName;
    TInt iQuality;
    TInt iProgress;
    TInt iError;
    };
    


/**
 * 
 *
 * @lib 
 */
class CMceComFileSource: public CMceComMediaSource
	{

public: // Constructors and destructor

    /**
    * Constructor
    */
    static CMceComFileSource* NewL();

    /**
    * Constructor
    */
    static CMceComFileSource* NewLC();

    /**
    * Destructor.
    */
	~CMceComFileSource();

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
    * Traversal event handler
    * @param aEvent the event
    * @return status, if event was consumed or not or object needs update
    */
    TInt EventReceivedL( TMceComEvent& aEvent );

    /**
    * Clones this object
    * @return the cloned object
    */
    CMceComMediaSource* CloneL();

    /**
    * Updates this object
    * @param aUpdate object updates this
    */
    void UpdateL( CMceComMediaSource& aUpdate );

    
#ifdef MCE_COMMON_SERVER_SIDE

    /**
    * Returns mcc type for mcc endpoint
    * @return mcc type
    */
    const TUid MccType();

    /**
    * Returns init param for mcc endpoint
    * @return mcc type
    */
    const TDesC8& InitParamL( const CMceComCodec& aCodec );

    /**
    * Endpoint can be informed about initialization completion
    */
    void InitializedL();

#endif//MCE_COMMON_SERVER_SIDE
    

protected:

    /**
    * C++ default constructor.
    */
	CMceComFileSource();
		

public: // serialized data

    TBool iFastForward;
    TBool iFastRewind;
    TTimeIntervalMicroSeconds iPosition;

public: //data

    TMceFileInfo iFileInfo;
    
    TBool iInitialInfoRetrieved;    

    TInt iCurrentAudioElement;
    TInt iCurrentVideoElement;
    
    TMceTranscodeInfo iTranscodeInfo;
    
    CMceCsSessionImplementation* iClient;

private: // data
    
    HBufC8* iSettings;
    
    friend class UT_CMceComFileSource;
    
	};


#endif
