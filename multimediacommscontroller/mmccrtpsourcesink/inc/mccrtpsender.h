/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    RTP sender
*
*/





#ifndef MCCRTPSENDER_H
#define MCCRTPSENDER_H

// INCLUDES
#include <e32base.h>
#include "rtpapi.h"
#include "rtpheader.h"
#include "mccexpirationhandler.h"


// FORWARD DECLARATIONS
class CMccRtpSendItem;
class CMccTimerManager;
 
// CLASS DECLARATION
//   

class MMccRtpSenderObserver
    {
public:
    virtual void SendErrorOccured( TInt aError ) = 0;
    };
    
class CMccRtpSender : public CActive, public MMccExpirationHandler
    {
public:

    /**
    * Create asynchronous rtp sender.
    *
    * @param aObserver
    * @param aRtpApi
    * @param aSessionId
    * @param aDoMarkerBasedCleanup
    */  
    static CMccRtpSender* NewL( MMccRtpSenderObserver& aObserver, 
                                CRtpAPI& aRtpApi, 
                                TRtpId aSessionId,
                                TBool aDoMarkerBasedCleanup );
    
    ~CMccRtpSender();
    
    /**
    * Send rtp packet asynchronously. If previous send is pending, packet
    * is queued unless the queue is full. In that case the packet is discarded. 
    *
    * @param aRtpStreamId
    * @param aRtpHeader
    * @param aData
    */  
    void SendRtpPacketL( TRtpId aRtpStreamId,
                         const TRtpSendHeader& aRtpHeader, 
                         const TDesC8& aData );
    
    /**
    * Send a non-RTP (control) data packet.
    * @param aSessionId     RTP Session ID
    * @param aUseRtpSocket  Whether to use RTP or RTCP socket
    * @param aData          Non-RTP data packet to send
    */
    void SendDataL( TRtpId aSessionId, TBool aUseRtpSocket, 
        const TDesC8& aData );
    
    /**
    * Cancels pending send operation and empties the queue.
    */                     
    void Clear();

protected: // From CActive

    void RunL();
    
    void DoCancel();
    
    TInt RunError( TInt aError );

protected: // From MMccExpirationHandler
    
    void TimerExpiredL( TMccTimerId aTimerId, TAny* aTimerParam );
                             
private:

    void ConstructL();

    CMccRtpSender( MMccRtpSenderObserver& aObserver,
                   CRtpAPI& aRtpApi, 
                   TRtpId aSessionId,
                   TBool aDoMarkerBasedCleanup );
    
    void SendPacketL();
    
    void RemovePacket( TBool aDoQueueCleanup = EFalse );
    
    TBool ErrorReport( TInt aError );
    
    TBool CheckDiscarding( const TRtpSendHeader& aRtpHeader );
    
    /**
    * If packet for which aRtpHeader belongs was zero marker frame,
    * that packet and everything related to that frame is removed from
    * queue. If full frame couldn't yet be removed, upcoming parts
    * of the frame are discarded once they are tried to be sent.  
    *
    * @param aItemIndex
    * @param aRtpHeader
    * @return ETrue if queue item in aItemIndex was deleted, otherwise
    *         return EFalse
    */ 
    TBool DoQueueCleanup( TInt aItemIndex, const TRtpSendHeader& aRtpHeader );
    
private:
    
    MMccRtpSenderObserver& iObserver;

    CRtpAPI& iRtpApi;
    
    TRtpId iSessionId;
    
    CMccTimerManager* iTimeoutTimer;
    
    RPointerArray<CMccRtpSendItem> iSendQueue;
    
    TBool iDoMarkerBasedCleanup;
    
    TBool iDiscarding;
    
    TUint32 iTimeoutTimeMilliseconds;
    
    TMccTimerId iTimerId;

#ifdef TEST_EUNIT
public:
    friend class UT_CMccRtpSender;
#endif
    
    };

class CMccRtpSendItem : public CBase
    {
public:

    static CMccRtpSendItem* NewLC( TRtpId aRtpStreamId, 
                                   const TRtpSendHeader& aRtpHeader,
                                   const TDesC8& aData );
    
    static CMccRtpSendItem* NewLC( TRtpId aSessionId, 
        TBool aUseRtpSocket, const TDesC8& aData );
    
    ~CMccRtpSendItem();
    
    const TDesC8& DataPtr() const;
    
    TRtpId RtpStreamId() const;
    
    const TRtpSendHeader& RtpHeader() const;
    
    TRtpId SessionId() const;
    
    TBool UseRtpSocket() const;
    
private:

    void ConstructL( const TDesC8& aData );
    
    CMccRtpSendItem( TRtpId aRtpStreamId, const TRtpSendHeader& aRtpHeader );
    
    CMccRtpSendItem( TRtpId aSessionId, TBool aUseRtpSocket );
    
private:

    HBufC8* iData;
    
    TRtpId iRtpStreamId;
    TRtpSendHeader iRtpHeader; 
    
    TPtrC8 iDataPtr;

    TRtpId iSessionId;
    TBool iUseRtpSocket;

private:    // Friend classes

    #ifdef TEST_EUNIT
        friend class UT_CMccRtpSender;
    #endif    
    };
    
#endif // MCCRTPSENDER_H

// End of File 
