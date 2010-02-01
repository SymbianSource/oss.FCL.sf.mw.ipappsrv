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




#ifndef CMCECOMSPEAKERSINK_H
#define CMCECOMSPEAKERSINK_H

#include <e32std.h>  
#include "mcecommediasink.h"

/**
 * 
 *
 * @lib 
 */
class CMceComSpeakerSink : public CMceComMediaSink
	{

public: // Constructors and destructor

    /**
    * Constructor
    */
    static CMceComSpeakerSink* NewL();

    /**
    * Constructor
    */
    static CMceComSpeakerSink* NewLC();

    /**
    * Destructor.
    */
	~CMceComSpeakerSink();

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

    /**
    * Is this endpoint re-usable
    * @return ETrue, if this endpoint is re-usable
    */
    TBool Reusable( const CMceComMediaStream& aParent ) const;

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
    
    /**
    * Prepares this endpoint
    */
    void PrepareL();
    

#endif//MCE_COMMON_SERVER_SIDE

protected:

    /**
    * C++ default constructor.
    */
	CMceComSpeakerSink();
	
	


public: // Data

    /**
    * Audio route
    */
    TInt iAudioRoute;
    
    /**
    * Audio volume
    */
    TInt iVolume;

	};


#endif 
