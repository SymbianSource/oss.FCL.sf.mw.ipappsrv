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




#ifndef CMCECOMRTPSINK_H
#define CMCECOMRTPSINK_H

#include <e32std.h>  
#include <badesca.h>  
#include "mcecommediasink.h"

/**
 * 
 *
 * @lib 
 */
class CMceComRtpSink : public CMceComMediaSink
	{
public: // Constructors and destructor

    /**
    * Constructor
    */
    static CMceComRtpSink* NewL();

    /**
    * Constructor
    */
    static CMceComRtpSink* NewLC();

    /**
    * Destructor.
    */
	~CMceComRtpSink();

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
    * Should use RTCP
    * return ETrue, if uses RTCP
    */
    TBool UseRTCP();

    /**
    * Sets SSRC 
    * @param aSSRC SSRC
    */
    void SetSSRC( TUint32 aSSRC );


#ifdef MCE_COMMON_SERVER_SIDE

public: // from CMceComEndpoint

    /**
    * Returns ETrue, if enabled
    * @param aLocally, if ETrue means locally enabled,
    *                  EFalse means that local and remote 
    *                  endpoints are enabled.
    * @return ETrue, if enabled
    */
    TBool IsEnabled( TBool aLocally ) const;

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

protected:

    /**
    * C++ default constructor.
    * @param aType type of sink
    */
	CMceComRtpSink();
	
	/**
    * second-phase copy constructor
    */
	void ConstructL( CMceComRtpSink& aSink );
	


public: // Data


    /**
     * suppress RTCP
     */
    TBool iSuppressRTCP;

    /**
     * SSRC
     */
    TUint32 iSSRC;

    /**
     * last packet
     */
    TUint32 iLastPacket;
    
    /**
    * RTP CName
    */
    HBufC8* iIdentity;
     
     
	};


#endif 