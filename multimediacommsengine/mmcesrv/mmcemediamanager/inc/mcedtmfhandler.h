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




#ifndef CMCEDTMFHANDLER_H
#define CMCEDTMFHANDLER_H

#include <e32base.h>
#include <mmccevents.h>
#include "mcemediadefs.h"
#include "mcedefs.h"
#include "mcesrv.h"
#include "mceclientserver.h"

class CMceComMediaSource;
class CMceSrvStream;
class TMceMccComEvent;
class CMceSrvStream;
class CMccInterface;
class TMceComEvent;

/**
 * 
 *
 * @lib 
 */
class CMceDtmfHandler: public CActive
    {


public: // Constructors & Destructor


    static CMceDtmfHandler* CMceDtmfHandler::NewL( CMceSrvStream& aStream, 
                                                   CMccInterface& aMccInterface );
    /**
    * Destructor.
    */
     ~CMceDtmfHandler();
     
protected: // From CActive

    void RunL();
    
    void DoCancel();
    
    TInt RunError( TInt aError );
    
public:

    void DtmfL( CMceComMediaSource& aSource, 
                TMceComEvent& aEvent,
                const TDesC8& aSequence = KNullDesC8 );

    void DtmfReceivedL( const TMccEvent& aMccEvent, 
                        TMceMccComEvent& aEvent );
    
private:

    void SendDtmfStringL( TMccDtmfEventType aDtmfEventType, 
                          const TDesC8& aDtmfString = KNullDesC8,
                          TUint32 aDurationInMicroSecs = 0 );
   
    void StopDtmfL();
    
    void SetDtmfActive( TBool aIsActive );
    
    TBool IsDtmfActive() const;
    
private:

    /**
     * C++ default constructor.
     */
    CMceDtmfHandler( CMceSrvStream& aStream, 
                     CMccInterface& aMccInterface );


private: // owned data
    
    CMceSrvStream& iStream;
    CMccInterface& iMccInterface;
    
    RTimer iTimer;
    
    TUint32 iCurrentUserStreamId;
    TUint32 iCurrentUserSourceId;
    
    TBool iSendingDtmf;
    
	//definitions for unit testing
	MCEMM_UT_DEFINITIONS
    
    };



#endif //CMCEDTMFHANDLER_H
