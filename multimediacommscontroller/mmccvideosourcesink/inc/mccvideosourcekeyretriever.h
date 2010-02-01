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



#ifndef MCCVIDEOSOURCEKEYRETRIEVER_H
#define MCCVIDEOSOURCEKEYRETRIEVER_H

// INCLUDES
#include <e32base.h>
#include <mmf/server/mmfdatasink.h>
#include "mmccevents.h"
#include "mcccamerahandler.h"

// FORWARD DECLARATIONS
class CMccVideoSourceImpl;
class CMccPeriodicRunner;

// CONSTANTS
const TInt KMccKeyRetrieveGuardTimeoutMicrosecs = 10000000; // 10 secs

// CLASS DECLARATION

/**
* Class used to fecth encoder configuration information.
* It wraps long taking asynchronous calls behind one sync function call.
* Operation has guard timer if fetching takes too long.
*
*/
class CMccVideoSourceConfigKeyRetriever : 
    public CBase, public MDataSink, public MAsyncEventHandler
    {
public:

    static CMccVideoSourceConfigKeyRetriever* NewLC( MMccCameraHandler& aCameraHandler );
    
    ~CMccVideoSourceConfigKeyRetriever();
    
    /**
    * Fetch config info. If operation takes too long, it is interrupted
    * and key retrieval fails
    */
    void RetrieveL( const TMccCodecInfo& aVideoCodec, TDes8& aConfigKey );

public: // From MDataSink

    TFourCC SinkDataTypeCode( TMediaId aMediaId );   
     
    void EmptyBufferL( CMMFBuffer* aBuffer, MDataSource* aSupplier, TMediaId aMediaId );
    
    void BufferFilledL( CMMFBuffer* aBuffer );
    
    TBool CanCreateSinkBuffer(); 
    
    CMMFBuffer* CreateSinkBufferL( TMediaId aMediaId, TBool &aReference );
    
    void ConstructSinkL( const TDesC8& aInitData );
        
public: // From MAsyncEventHandler

    TInt SendEventToClient( const TMMFEvent& aEvent );

private:

    void ConstructL();

    CMccVideoSourceConfigKeyRetriever( MMccCameraHandler& aCameraHandler );
    
    void HandleEventL( TMccEvent& aEvent );
    
    void HandleError();
    
    void StopWaiting();
    
    void StartGuardTimer();
    
    void StopGuardTimer();
    
    static TInt GuardTimerExpired( TAny* aObject );
    
private:

    MMccCameraHandler& iCameraHandler;   
    
    CActiveSchedulerWait* iWait;   
    
    CMccVideoSourceImpl* iSource;
    
    CMMFDataBuffer* iBuffer;
    
    TBool iFailed;
    
    CMccPeriodicRunner* iGuardTimer;
    
    TInt iGuardTimerTimeout;

    #ifdef TEST_EUNIT
		friend class UT_CMccVideoSourceImpl;
		friend class UT_CMccVideoSourceKeyRetriever;	
    #endif
   		      
    };
    

#endif 


// End of file

