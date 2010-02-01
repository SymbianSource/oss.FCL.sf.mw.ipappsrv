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




#ifndef __UT_CSTUNTRANSACTION_H__
#define __UT_CSTUNTRANSACTION_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/EUnitDecorators.h>


//  INTERNAL INCLUDES
#include "cstuntransaction.h"
#include "cncmconnectionmultiplexer.h"
#include "mncmconnectionmultiplexerobserver.h"
#include "mstuntransactionobserver.h"
#include "natfwunsaftransactionid.h"

//  FORWARD DECLARATIONS


//  CLASS DEFINITION
/**
 * TODO Auto-generated EUnit test suite
 *
 */
NONSHARABLE_CLASS( ut_cstuntransaction )
	: public CEUnitTestSuiteClass, public MNcmConnectionMultiplexerObserver,
	  public MSTUNTransactionObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static ut_cstuntransaction* NewL();
        static ut_cstuntransaction* NewLC();
        /**
         * Destructor
         */
        ~ut_cstuntransaction();

    private:    // Constructors and destructors

        ut_cstuntransaction();
        void ConstructL();

    public:     // From MNcmConnectionMultiplexerObserver
    
        void Notify( TUint aSessionId, TUint aStreamId, TNotifyType aType, TInt aError );
        void IcmpError( TUint aSessionId, TUint aStreamId, const TInetAddr& aAddress );

    public: // From MSTUNTransactionObserver

        void PublicAddressObtainedL( const TInetAddr& /*aAddress*/ ) {};

        void PublicAddressObtainedL( const TInetAddr& /*aReflexiveAddr*/,
                                     const TInetAddr& /*aRelayAddr*/ ){};

        void TransactionError( TInt /*aError*/,
                               CNATFWUNSAFUnknownAttributesAttribute* /*aUnknownAttr*/ ){};

        void TransactionEventOccurredL( TSTUNCallbackInfo::TFunction /*aEvent*/ ){};
        
        void NewRTTSampleMeasured( TInt64& /*aRTTSample*/, TBool /*aRetransmitted*/ ) {};
        
    private:    // New methods

         void SetupL();
        
         void Teardown();

         HBufC8* EncodeMessageL( const CNATFWUNSAFMessage& aRequest,
                                const TBool aUseSharedSecret = ETrue,
                                const TBool aAddFingerPrint = EFalse );
         
         void ut_cstuntransaction_NewL();
        
         void ut_cstuntransaction__CSTUNTransactionL();
        
         void ut_cstuntransaction_TimerExpiredLL();
        
         void ut_cstuntransaction_LeaveFromTimerExpiredL();
        
         void ut_cstuntransaction_UNSAFUdpMessageSentLL();
        
         void ut_cstuntransaction_UNSAFUdpMessageFailureL();
        
         void ut_cstuntransaction_MessageSentL();
        
         void ut_cstuntransaction_MessageSentFailureL();
        
         void ut_cstuntransaction_SendRequestLL();
        
         void ut_cstuntransaction_RequestSentL();
        
         void ut_cstuntransaction_RetransmitRequestLL();
        
         void ut_cstuntransaction_ReceiveLL();
        
         void ut_cstuntransaction_HasReceivedResponseL();
        
         void ut_cstuntransaction_TerminateL();
        
         void ut_cstuntransaction_TimerValueL();
        
         void ut_cstuntransaction_CancelRetransmissionL();
        
         void ut_cstuntransaction___DbgTestInvariantL();

         void ut_cstuntransaction_RetransmitIntervalL();

         void ut_cstuntransaction_ComputeWaitDurationL();

         void ut_cstuntransaction_ShouldIgnoreResponseL();

         void ut_cstuntransaction_ProcessResponseLL();

         void ut_cstuntransaction_CheckIntegrityLL();

         //void ut_cstuntransaction_AddressDiffersLL();

         void ut_cstuntransaction_CheckForNatOverwriteLL();

         void ut_cstuntransaction_GetResponseCodeL();

         void ut_cstuntransaction_CheckProtocolLL();

         void ut_cstuntransaction_StunRelayUsedL();

         void ut_cstuntransaction_MeasureNewRTTSampleL();         
        

    private:    // Data
		
        CSTUNTransaction* iCSTUNTransaction;
        CNATFWUNSAFBindingRequest* iRequest;
        CDeltaTimer* iDeltatimer;
        TInetAddr iServerAddr;
        TUint iStreamId;
        TUint iSubstreamId;
        TInt iInterval;
        CNcmConnectionMultiplexer* iMux;
        TTransportProtocol iTransportProtocol;
        
        EUNIT_DECLARE_TEST_TABLE; 

    };

#endif      //  __UT_CSTUNTRANSACTION_H__

// End of file
