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




#ifndef CMCESRVENDPOINT_H
#define CMCESRVENDPOINT_H

#include <e32base.h>
#include "mcesrvstream.h"
#include "mcesrv.h"


class CMceComMediaSource;
class CMceMediaManager;
class TMceMccComEvent;
class CMceSrvStream;
class CMceComEndpoint;

/**
 * 
 *
 * @lib 
 */
class CMceSrvEndpoint: public CBase
    {


public: // Constructors & Destructor

    
    /**
    * Destructor.
    */
     ~CMceSrvEndpoint();

    
public: // Source and sink functions


    /**
    * Gets id
    * @return id
    */
    TUint32& Id();

    /**
    * Gets the original com endpoint
    * @return the original com endpoint
    */
    CMceComEndpoint& Data() const;

    /**
    * Handles event from MCC
    */
    void EventReceived( TMceMccComEvent& aEvent );


    /**
    * Gets the source type
    * @return the source type
    */
    const TUid Type() const;
    
    void SetState( CMceSrvStream::TState aState );

    CMceSrvStream::TState State() const;
    
    /**
    * Merges with stream from cloned session
    */
    void Merge( CMceSrvEndpoint& aMergeWith );
    
    TBool EnableL( CMceSrvStream& aStream, 
                   TBool aSync = KMceSrvStreamNoSync );

    TBool DisableL( CMceSrvStream& aStream, 
                    TBool aSync = KMceSrvStreamNoSync,
                    TBool aForce = KMceSrvStreamNoForce );
    
    TBool Reusable( const CMceSrvStream& aStream ) const;
    
    TBool StreamStateChangeEventReceived( TMceMccComEvent& aEvent );
    
    TBool EndpointMatch( const CMceComEndpoint& aEndpoint, 
                         TBool aUseProxyMatch );

public:

    /**
    * Called after prepare is called for this
    * stream
    */
    virtual void MccPrepareCalledL( CMceSrvStream& aStream );
    
    /**
    * Gets correct endpoint from stream.
    * @param aStream
    * @return CMceSrvEndpoint
    */
    virtual CMceSrvEndpoint& Endpoint( CMceSrvStream& aStream ) = 0;
    
    /**
    * Handle secure key expired event
    * @param aEvent MCC event
    * @return void
    */
    virtual void HandleSecureEventReceivedL( ) =0;
    
protected:

    /**
    * C++ default constructor.
    */
    CMceSrvEndpoint( CMceMediaManager& aManager, CMceComEndpoint& aData );
    
protected: // owned data
    
    TUint32 iID;
    CMceSrvStream::TState iState;
    
protected: // NOT owned data

    CMceMediaManager* iManager;
    CMceComEndpoint* iData;
    
    friend class CMceSrvStream;
    
	//definitions for unit testing
	MCEMM_UT_DEFINITIONS
    
    
    };



#endif //CMCESRVENDPOINT_H
