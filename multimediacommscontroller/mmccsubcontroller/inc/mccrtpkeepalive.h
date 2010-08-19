/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    RTP Keepalive mechanism
*
*/




#ifndef MCCRTPKEEPALIVE_H
#define MCCRTPKEEPALIVE_H

//  INCLUDES
#include <e32base.h>
#include "rtpdef.h"
#include "rtpapi.h"
#include "rtpheader.h"

// CONSTANTS

// MACROS

// DATA TYPES
    
// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class MAsyncEventHandler;
class MMccResources;
class MMccRtpInterface;
class CMccRtpMediaClock;

// CLASS DECLARATION

/**
*
*/
NONSHARABLE_CLASS( CMccRtpKeepalive ) : public CActive
    {
    public: // enums
    
        enum TMccRtpKeepaliveState
            {
            EIdle,
            ESending,
            EWaitingTimer,
            ESendingPending
            };
            
    public: // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CMccRtpKeepalive* NewL( MAsyncEventHandler& aEventHandler,
                                       CRtpAPI& aRtpAPI,
                                       TRtpId aRtpSessionId,
                                       TUint8 aKeepalivePayloadType,
                                       TUint32 aKeepaliveInterval,
                                       const TDesC8& aKeepaliveData,
                                       TBool aRemoteAddressSet );
        
        /**
        * Destructor
        */
        virtual ~CMccRtpKeepalive();

    public:     // New functions
        
        /**
         * Start sending keepalive packets.
         *
         * @since Series 60 3.2
         * @param aUser rtp interface instance 
         * @param aRtpMediaClock rtp media clock instance
         * @return void
         */  
        void StartKeepaliveL( MMccRtpInterface& aUser, 
                              CMccRtpMediaClock& aRtpMediaClock );
        
        /**
         * Stop sending keepalive packets.
         *
         * @since Series 60 3.2
         * @param aUser rtp interface instance
         * @param aRefCount array index to rtp interface intance array
         * @return One of the standard system-wide error codes.
         */  
        TInt StopKeepalive( MMccRtpInterface& aUser, TInt& aRefCount );
        
        /**
         * Reset keepalive timer.
         *
         * @since Series 60 3.2
         * @param None
         * @return void
         */  
        void ResetKeepaliveTimer();
        
        /**
         * Getter function for rtp stream id.
         *
         * @since Series 60 3.2
         * @param None
         * @return rtp stream Id.
         */
        TRtpId StreamId() const;

        TUint8 PayloadType() const;
        
        TInt RemoteAddressSet();
        
        /**
         * Function for checking if there is a sink in the array.
         * 
         * @since Series 60 3.2
         * @return ETrue if sink exits
         */    
        TBool IsForSink();
        
        /**
         * Update session keepalive parameters
         *
         * @since Series 60 3.2
         * @param aKeepalivePT payload type for keepalive
         * @param aKeepaliveInterval interval to execute packet sending
         * @param aKeepaliveData data to add in keepalive packets
         * @return void
         */
        void UpdateParamsL( TUint8 aKeepalivePT,
                            TUint32 aKeepaliveInterval,
                            const TDesC8& aKeepaliveData, 
							CMccRtpMediaClock* aRtpMediaClock );
        
    protected:  // New functions
    
    protected:  // Functions from CActive
    
        void RunL();
        
        void DoCancel();
        
        TInt RunError( TInt aError );
    
    private:    // New functions
    
        TBool ResolveActivity();
    
        TInt ToggleKeepaliveSending( TBool aIsActive );
    
        TInt StartSending();
        
        TInt Send();
    
        TUint16 Random() const;
        
        void SendErrorToClient( TInt aError );
        
    private:    // Constructors

        /**
        * C++ default constructor.
        */
        CMccRtpKeepalive( MAsyncEventHandler& aEventHandler,
                          CRtpAPI& aRtpAPI,
                          TRtpId aRtpSessionId,
                          TBool aRemoteAddressSet );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( TUint8 aKeepalivePayloadType,
                         TUint32 aKeepaliveInterval,
                         const TDesC8& aKeepaliveData );       

    public:     // Data
    
    protected:  // Data

    private:    // Data
        
        // Callback for errors
        MAsyncEventHandler& iEventHandler;

        // RTP stack API reference
        CRtpAPI& iRtpAPI;
        
        TRtpId iRtpSessionId;
        
        // Stream id for the RTP Keepalive transmit stream
        TRtpId iRtpStreamId;
        
        RTimer iTimer;
        
        TMccRtpKeepaliveState iCurrentState;
        
        TRtpSSRC iSSRC;
        
        TRtpSequence iSequenceNum;
        
        TRtpSendHeader iRtpHeaderInfo;
        
        TInt iKeepaliveInterval;
        
        HBufC8* iKeepaliveData;
        
        TTime iInitialTime;
        TTime iCurrentTime;
        
        RPointerArray<MMccRtpInterface> iUsers;
        
        TBool iRemoteAddressSet;
        
        /**
         * Boolean indicating if rtp sink exists.
         */
        TBool iRtpSinkExists;
        
        /**
         * Rtp sink index in iUsers array.
         */
        TUint iRtpSinkIndex;
        
        /**
         * Rtp media clock instance
         * Not own.
         */
        CMccRtpMediaClock* iRtpMediaClock;
        
        /**
         * Boolean indicating if pre audio keep alive packet is sent
         */
        TBool iPreAudioPacketSent;


    public:     // Friend classes

    protected:  // Friend classes

    private:    // Friend classes
    
        #ifdef TEST_EUNIT
        friend class UT_CMccRtpKeepalive;
        #endif
    };
    
#endif      // MCCRTPKEEPALIVE_H

// End of File
