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




#ifndef CMCECOMRTPSOURCE_H
#define CMCECOMRTPSOURCE_H

#include <e32std.h>
#include "mcecommediasource.h"

//FORWARD DECLARATIONS

class TRtpSourceContainer
	{
	public:
		TUint32 iSilenceTimer;
		TUint iToneFrequency;
		TUint iToneLength;		
	};

/**
 * 
 *
 * @lib 
 */
class CMceComRtpSource: public CMceComMediaSource
	{

public: // Constructors and destructor


    /**
    * Constructor
    */
    static CMceComRtpSource* NewL();

    /**
    * Constructor
    */
    static CMceComRtpSource* NewLC();

    /**
    * Destructor.
    */
	~CMceComRtpSource();

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

public: // new functions

    /**
    * Return inactivity timer
    * @return inactivity timer
    */
    TUint InactivityTimer() const;

    
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
    * Prepares this endpoint
    */
    void PrepareL();
    

#endif//MCE_COMMON_SERVER_SIDE
    

protected:

    /**
    * C++ default constructor.
    * @param type of source
    */
	CMceComRtpSource();
	
    /**
     * second-phase copy constructor
     */
	void ConstructL( CMceComRtpSource& aSource );
	

public: // Owned data


    TUint32 iInactivityTimer;
    
	TUint iBufferLength;
	
	TUint iBufferTreshold;
	
	HBufC8* iTalkBurstIndicator;
    
    RArray<TUint> iNotSupported;    
    
    /**
    * RTP CName
    */
    HBufC8* iIdentity;

private:
    
    HBufC8* iSettings;
    
    
	};


#endif 