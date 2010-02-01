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




#ifndef CMCECOMDISPLAYSINK_H
#define CMCECOMDISPLAYSINK_H

#include <e32std.h>  
#include "mcecommediasink.h"
#include "mcedisplaysink.h"

/**
 * 
 *
 * @lib 
 */
class CMceComDisplaySink : public CMceComMediaSink
	{

public: // Constructors and destructor

    /**
    * Constructor
    */
    static CMceComDisplaySink* NewL();

    /**
    * Constructor
    */
    static CMceComDisplaySink* NewLC();

    /**
    * Destructor.
    */
	~CMceComDisplaySink();

public: // from CMceComMediaSink

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
    CMceComMediaSink* CloneL();

    /**
    * Updates this object
    * @param aUpdate object updates this
    */
    void UpdateL( CMceComMediaSink& aUpdate );

    /**
    * Called endpoint proxy must be created
    * @return endpointProxy
    */
    MMceComEndPointProxy* CreateEndpointProxyL() const;

    /**
    * Should this endpoint use endpoint proxy
    * @return ETrue, this endpoint uses endpoint proxy
    */
    TBool UsesEndpointProxy() const;


public: //new functions

    /**
    * Returns display size
    * @return display size
    */
    TRect DisplayRect();
    
    /**
    * Sets display size
    * @param aRect display size
    */
    void SetDisplayRect( const TRect& aRect );

    /**
    * converts (point, size ) => rect
    * @param aPoint point
    * @param aSize size
    * @param aRect display size
    */
    static void ToRect( const TPoint& aPoint, const TSize& aSize, TRect& aRect );

    /**
    * converts rect => (point, size )
    * @param aRect display size
    * @param aPoint point
    * @param aSize size
    */
    static void ToPointAndSize( const TRect& aRect, TPoint& aPoint, TSize& aSize );


#ifdef MCE_COMMON_SERVER_SIDE

    /**
    * Returns mcc type for mcc endpoint
    * @return mcc type
    */
    const TUid MccType();
    
    /**
    * Returns init param for mcc endpoint
    * @param aCodec codec
    * @return init param
    */
    const TDesC8& InitParamL( const CMceComCodec& aCodec );
    
    const TDesC8& DoInitParamL( const CMceComCodec& aCodec );
    
    /**
    * Enables 'locally' this endpoint
    * @param aCodec the codec associated to this endpoint
    */
    void EnableL( const CMceComCodec& aCodec );
    
    /**
    * Disables 'locally' this endpoint
    */
    void DisableL();
    
    /**
    * Prepares this endpoint
    */
    void PrepareL();
    
    /**
    */
    void SetViewFinder( TBool aIsViewFinder );
    
    /**
    */
    TUint32 MccParameterName() const;
    

#endif//MCE_COMMON_SERVER_SIDE

protected:

    /**
    * C++ default constructor.
    */
	CMceComDisplaySink();
	
	
public: // Data

public: // Serialized data

    TInt iDisplayIndex;
    TInt iDisplayRectTlX;
    TInt iDisplayRectTlY;
    TInt iDisplayRectBrX;
    TInt iDisplayRectBrY;
    TUint iPriority; 
    TBool iIsViewFinder;
    CMceDisplaySink::TRotation iRotation;

    TInt iDisplaysAvailable;
    TBool iInitialInfoRetrieved;
    
private: // Data

    HBufC8* iSettings;
    
	};


#endif 
