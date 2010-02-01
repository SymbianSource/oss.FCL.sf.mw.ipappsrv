/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef __MCCMEDIASTREAM_H__
#define __MCCMEDIASTREAM_H__

#include <e32base.h>

enum TStreamDirection
    {
    EUpStream,
    EDownStream  
    };

enum TMccStreamState
    {
    EStreamReady,
    EStreamCreated,
    ERtpStreamPrepared,
    ERtcpStreamPrepared,
    ERtpStreamStarted,
    ERtcpStreamStarted
    };

class CMccLink;
class CMccSession;


class CMccMediaStream : public CBase
{
public:
    static CMccMediaStream* NewL( CMccLink* aLink, CMccSession* aSession,
        TStreamDirection aStreamDirection );

    void CreateStreamL( TInt32& aStreamId, TRequestStatus& aStatus );
    
    void PrepareStreamL( TRequestStatus& aStatus );
    
    void StartStreamL( TRequestStatus& aStatus );
    
    TMccStreamState State();
    
    void SetState( TMccStreamState aState );
    
    TInt Delete();
    
    TUint32 StreamId();
        
    virtual ~CMccMediaStream();
     
private:
    void ConstructL();
    
    CMccMediaStream( CMccLink* aLink, CMccSession* aSession,
        TStreamDirection aStreamDirection );
                    
private: // data
    CMccLink* iLink; // not own
    CMccSession* iSession; // not own
    TStreamDirection iStreamDirection;
    TMccStreamState iState;
    TUint32 iStreamId;

};

#endif
