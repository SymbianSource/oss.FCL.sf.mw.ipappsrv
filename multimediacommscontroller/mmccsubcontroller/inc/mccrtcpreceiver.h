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
* Description:    RTCP receiver for Mcc RTP Datasource
*
*/




#ifndef MCCRTCPRECEIVER_H
#define MCCRTCPRECEIVER_H

//  INCLUDES
#include <e32base.h>
#include <mmf/common/mmfcontrollerframework.h>
#include "rtpdef.h"
#include "rtpapi.h"
#include "mmccevents.h"

// CONSTANTS

// MACROS

// DATA TYPES
    
// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class MAsyncEventHandler;
class MMccResources;
class CMccRtpManager;

// CLASS DECLARATION

/**
*  RTCP receiver for Mcc RTP Datasource.
*  @lib MccRtpSourceSink.dll
*  @since Series 60 3.0
*/
#ifdef EKA2
NONSHARABLE_CLASS( CMccRtcpReceiver ) : public CBase, 
                                        public MRtcpObserver,
                                        public MNonRTPDataObserver
#else
class CMccRtcpReceiver : public CBase, 
                         public MRtcpObserver,
                         public MNonRTPDataObserver
#endif
    {
    public:     // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CMccRtcpReceiver* NewL( CMccRtpManager* aRtpManager,
                                       MAsyncEventHandler& aEventHandler, 
                                       MMccResources& aMccResources,
                                       CRtpAPI& aRtpAPI );
        
        /**
        * Destructor
        */
        virtual ~CMccRtcpReceiver();

    public:     // New functions
    
        TInt HandleReceiving( TRtpId aSessionId, TBool aEnableReceiving );
        
    public:     // Functions from base classes
    
        /**
        * From MRtcpObserver
        */
        void SdesReceived( TRtpSSRC aSSRC, const TRtpSdesParams& aParams );

        /**
        * From MRtcpObserver
        */
        void ByeReceived( TRtpId aStreamId, TRtpSSRC aSSRC,
                          const TDesC8& aReason );
        
        /**
        * From MRtcpObserver
        */
        void AppReceived( TRtpId aStreamId, TRtpSSRC aSSRC,
                          const TRtcpApp& aApp );
        
        /**
        * From MRtcpObserver
        */
        void SrReceived( TRtpId aStreamId, TRtpSSRC aSSRC,
                         const TTimeStamps& aTimeStamps );
        
        /**
        * From MRtcpObserver
        */
        void RrReceived( TRtpId aStreamId, TRtpSSRC aSSRC );

        /**
        * From MNonRTPDataObserver
        */
        void NonRTPDataReceived( TUint aPort, TBool aRTPPort,
                                 const TDesC8& aNonRTPData );

    protected:  // New functions
    
    protected:  // Functions from base classes

    private:    // Constructors

        /**
        * C++ default constructor.
        */
        CMccRtcpReceiver( CMccRtpManager* aRtpManager,
                          MAsyncEventHandler& aEventHandler,
                          MMccResources& aMccResources, 
                          CRtpAPI& aRtpAPI );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // New functions
    
        TInt SendRtcpEventWithPacketDataToClient( TMccRtcpPacketType aPacketType, 
                                                  const TRtpSSRC& aSSRC,
                                                  const TDesC8& aData,
                                                  TBool aResolveEndpointBySsrc );
                            
        void SendRtcpEventToClient( TMccRtcpPacketType aPacketType,
                                    TBool aResolveEndpointBySsrc,
                                    TBool aResolveEndpointByRtpStreamId );
                                    
        void DoSdesMemberCopy( TPtrC8& aDestination,
                               TUint8** aCurrentPtr, 
                               TInt& aCurrentPos,
                               TDes8& aRtcpPacketData, 
                               const TDesC8& aSource );
                               
        void ClearEventData();
        

    public:     // Data
    
    protected:  // Data

    private:    // Data
        
        // Stream ID for the RTCP
        TRtpId iStream;
        
        // Callback for RTCP
        MAsyncEventHandler& iEventHandler;
        
        MMccResources& iMccResources;

        // RTP stack API reference
        CRtpAPI& iRtpAPI;

        TRtpPeerStat iPeerstat;
        
        // Used for storing data of received event temporarily (one at a time)
        TMccRtcpEventData iEventData;
        
        TBool iEnableRtcp;
        
        TRtpId iSessionId;
        
        CMccRtpManager* iRtpManager;


    public:     // Friend classes

    protected:  // Friend classes

    private:    // Friend classes
    };
    
#endif      // MCCRTCPRECEIVER_H

// End of File
