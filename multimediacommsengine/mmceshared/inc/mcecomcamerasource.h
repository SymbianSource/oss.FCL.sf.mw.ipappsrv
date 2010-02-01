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




#ifndef CMCECOMCAMERASOURCE_H
#define CMCECOMCAMERASOURCE_H

#include <e32std.h>  
#include <ecam.h>
#include "mcecommediasource.h"

class TMceCameraInfo
    {
public:
    TInt iIndex;
    TCameraInfo iInfo;
    };


/**
 * 
 *
 * @lib 
 */
class CMceComCameraSource : public CMceComMediaSource
	{

public: // Constructors and destructor

    /**
    * Constructor
    */
    static CMceComCameraSource* NewL();

    /**
    * Constructor
    */
    static CMceComCameraSource* NewLC();

    /**
    * Destructor.
    */
	~CMceComCameraSource();

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
    * Called after endpoint has been prepared.
    */ 
    TInt DoPrepared();
    
#endif//MCE_COMMON_SERVER_SIDE

protected:

    /**
    * C++ default constructor.
    */
	CMceComCameraSource();
		

public: // serialized data

    TInt iCameraIndex;
    TInt iZoomFactor;
    TInt iDigitalZoomFactor;
    TInt iContrast;
    TInt iBrightness;
    TInt iExposure;
    TInt iWhiteBalance;
    
    TInt iCamerasAvailable;
    TCameraInfo iCameraInfo;
    TBool iInitialInfoRetrieved;

private: // Data

    HBufC8* iSettings;
    
	};


#endif