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




#ifndef CMCEADOPTEDSRVSTREAM_H
#define CMCEADOPTEDSRVSTREAM_H

#include "mcesrvstream.h"
#include "mceclientserver.h"
#include "mcesrv.h"


/**
 * 
 *
 * @lib 
 */
class CMceAdoptedSrvStream : public CMceSrvStream
    {

public: // Constructors & Destructor


    /**
    * Constructor
    * This is factory method
    * @return array of server streams
    */
    static CMceAdoptedSrvStream* NewL( const CMceSrvStream& aStream );
    
    /**
    * Destructor.
    */
     ~CMceAdoptedSrvStream();

    
public: //from CMceSrvStream


    /**
    * Is adopted
    */
    TBool IsAdopted() const;

    /**
    * Returns state of stream, which depends on states of
    * sink and source
    * @return the state
    */
    CMceSrvStream::TState State() const;

    /**
    * Checks, if this is equal to another stream
    * @param aStream an another stream
    * @return ETrue, if this is equal
    */
    TBool IsEqual( const CMceSrvStream& aStream ) const;

    /**
    * Is Prepared by MCC
    * @return ETrue, if so
    */
    TBool IsMccPrepared() const;

    /**
    * Invalidates the stream
    */
    void Invalidate();

    /**
    * Gets session id
    * @return session id
    */
    TUint32 SessionId() const;
    
    /**
    * Gets link id
    * @return link id
    */
    TUint32 LinkId() const;

    /**
    * Sets link id
    * @param aLinkId link id
    */
    void SetLinkId( TUint32 aLinkId );

    /**
    * Gets IAP id
    * @return IAP id
    */
    TUint32 IapId() const;
    
    /**
    * Gets local media port of stream
    * @return local media port of stream
    */
    TUint LocalMediaPort() const;

    /**
    * Cleans up unused sinks/sources from mcc
    */
    void Cleanup();
    
private://methods

    /**
     * C++ default constructor.
     */
    CMceAdoptedSrvStream( const CMceSrvStream& aStream );

private: // owned data
    
    TMceMediaId iCodecId;
    TMceMediaId iSinkId;
    TMceMediaId iSourceId;

    TUint32 iSessionId;
    TUint32 iLinkId;
    TUint32 iIapId;
    TUint32 iSourceEndpointId;
    TUint32 iSinkEndpointId;
    TUint iLocalMediaPort;

	//definitions for unit testing
	MCEMM_UT_DEFINITIONS

    
    };



#endif //CMCEADOPTEDSRVSTREAM_H
