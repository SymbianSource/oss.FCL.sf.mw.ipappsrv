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




#ifndef MCCRTPKEEPALIVECONTAINER_H
#define MCCRTPKEEPALIVECONTAINER_H

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
class CMccRtpKeepalive;
class TMccCodecInfo;
class CMccRtpMediaClock;

// CLASS DECLARATION

class MMccRtpKeepalive
    {
    public:
        
        /**
         * Start sending keepalive packets.
         *
         * @since Series 60 3.2
         * @param aUser rtp interface instance
         * @param aCodecInfo codec info instance
         * @param aRtpMediaClock rtp media clock instance
         * @return void
         */  
        virtual void StartKeepaliveL( MMccRtpInterface& aUser, 
                                      TMccCodecInfo& aCodecInfo, 
                                      CMccRtpMediaClock& aRtpMediaClock ) = 0;
                                      
        /**
         * Stop sending keepalive packets.
         *
         * @since Series 60 3.2
         * @param aUser rtp interface instance
         * @return One of the standard system-wide error codes.
         */ 
        virtual TInt StopKeepalive( MMccRtpInterface& aUser, 
                                    const TMccCodecInfo& aCodecInfo ) = 0;
        
        /**
         * Reset keepalive timer.
         *
         * @since Series 60 3.2
         * @param None
         * @return void
         */  
        virtual void ResetKeepaliveTimer() = 0;
        
        /**
         * Update session keepalive parameters
         *
         * @since Series 60 3.2
         * @param aUser rtp interface instance
         * @param aCodecInfo codec info instance
         * @param aRtpMediaClock rtp media clock instance
         * @return void
         */
        virtual void UpdateParamsL( MMccRtpInterface& aUser, 
                                    const TMccCodecInfo& aCodecInfo,
                                    CMccRtpMediaClock& aRtpMediaClock ) = 0;
    };

NONSHARABLE_CLASS( CMccRtpKeepaliveContainer ) : public CBase, 
    public MMccRtpKeepalive
    {
    public:
    
        /**
        * Two-phased constructor.
        */
        static CMccRtpKeepaliveContainer* NewL( 
                                       MAsyncEventHandler& aEventHandler,
                                       CRtpAPI& aRtpAPI,
                                       TRtpId aRtpSessionId );
        
        /**
        * Destructor
        */
        virtual ~CMccRtpKeepaliveContainer();

    public: // From MMccRtpKeepalive
        
        /**
         * Start sending keepalive packets.
         *
         * @since Series 60 3.2
         * @param aUser rtp interface instance
         * @param aCodecInfo codec info instance
         * @param aRtpMediaClock rtp media clock instance
         * @return void
         */  
        virtual void StartKeepaliveL( MMccRtpInterface& aUser, 
                                      TMccCodecInfo& aCodecInfo,
                                      CMccRtpMediaClock& aRtpMediaClock );
        
        /**
         * Stop sending keepalive packets.
         *
         * @since Series 60 3.2
         * @param aUser rtp interface instance
         * @return One of the standard system-wide error codes.
         */ 
        virtual TInt StopKeepalive( MMccRtpInterface& aUser, 
                                    const TMccCodecInfo& aCodecInfo );
        /**
         * Reset keepalive timer.
         *
         * @since Series 60 3.2
         * @return void
         */  
        virtual void ResetKeepaliveTimer();
        
        /**
         * Update session keepalive parameters
         *
         * @since Series 60 3.2
         * @param aUser rtp interface instance
         * @param aConfig Configuration
         * @param aRtpMediaClock rtp media clock instance
         * @return void
         */
        virtual void UpdateParamsL( MMccRtpInterface& aUser, 
                                    const TMccCodecInfo& aConfig,
                                    CMccRtpMediaClock& aRtpMediaClock );                            
    
    public: // Functions
       
        void StopAll();
        
        TInt RemoteAddressSet();
    
    private:
    
        CMccRtpKeepalive* FindKeepaliveHandler( TUint8 aKeepAlivePT,
            MMccRtpInterface& aUser );  
       
    private: // Constructors
    
        CMccRtpKeepaliveContainer( MAsyncEventHandler& aEventHandler,
                                   CRtpAPI& aRtpAPI,
                                   TRtpId aRtpSessionId );
        
    
    private: // Data
    
        MAsyncEventHandler& iEventHandler;

        CRtpAPI& iRtpAPI;
        
        TRtpId iRtpSessionId;
        
        RPointerArray<CMccRtpKeepalive> iKeepaliveHandlers;
        
        TBool iStopped;
        
        /**
         * Boolen indicating if rtp sink exists.
         */
        TBool iRtpSinkExist;

        TBool iRemoteAddressSet;
        
        #ifdef TEST_EUNIT
        friend class UT_CMccRtpKeepalive;
        friend class UT_CMccRtpDataSink;
        friend class UT_CMccRtpDataSource;
        #endif
    };
    
#endif      // MCCRTPKEEPALIVECONTAINER_H

// End of File
