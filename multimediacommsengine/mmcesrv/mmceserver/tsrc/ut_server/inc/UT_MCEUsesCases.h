/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef __UT_MCEUsesCases_H__
#define __UT_MCEUsesCases_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "mceclientserver.h"

//  FORWARD DECLARATIONS
class CMceSipSession;
class CMceSipStateMachine;
class CSIPConnection;
class CSIP;
class CMceServerCore;
class CMceCsSession;
class CMceSipConnection;
class CMceSipManager;
class CSIPProfile;
class CMCETls;




//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_MCEUsesCases
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_MCEUsesCases* NewL();
        static UT_MCEUsesCases* NewLC();
        /**
         * Destructor
         */
        ~UT_MCEUsesCases();

    private:    // Constructors and destructors

        UT_MCEUsesCases();
        void ConstructL();


    private:    // New methods

    void SetupL();

    void Teardown();

    
    /**
     * INVITE  -->
     * 100     <--
     * 180     <--
     * 200 OK  <--
     * ACK     -->
     * BYE     -->
     * 200 OK  <--
     */
    void UT_MCEUsesCases_TestCallL();
    
    /**
     * INVITE  <--
     * 200 OK  -->
     * ACK     <--
     * BYE     <--
     * 200 OK  -->
     */
    void UT_MCEUsesCases_TestCalledL();
    
    /**
     * INVITE  -->
     * 100     <--
     * 200 OK  <--
     * ACK     -->
     * INVITE  -->
     * 200 OK  <--
     * ACK     -->
     * BYE     -->
     * 200 OK  <--
     */
    void UT_MCEUsesCases_TestCallAndUpdateL();
    
    /**
     * INVITE  <--
     * 200 OK  -->
     * ACK     <--
     * INVITE  <--
     * 200 OK  -->
     * ACK     <--
     * BYE     -->
     * 200 OK  <--
     */
    void UT_MCEUsesCases_TestCalledAndUpdatedL();

    /**
     * INVITE  -->
     * 100     <--
     * 603     <--
     */
    void UT_MCEUsesCases_TestCallDeclinedL();

    /**
     * INVITE  <--
     * 603     -->
     */
    void UT_MCEUsesCases_TestCalledDeclineL();


    /**
     * INVITE  -->
     * 100     <--
     * CANCEL  -->
     * 200OK(c)<--
     * 487     <--
     */
    void UT_MCEUsesCases_TestCallCancelL();

    /**
     * INVITE  -->
     * 100     <--
     * CANCEL  -->
     * 487     <--
     * 200OK(c)<--
     */
    void UT_MCEUsesCases_TestCallCancel2L();

    /**
     * INVITE  -->
     * 100     <--
     * CANCEL  -->
     * 200OK(i)<--
     * ACK     -->
     * BYE     -->
     * 200OK(c)<--
     * 200OK(b)<--
     */
    void UT_MCEUsesCases_TestCallCancel3L();

    
    /**
     * INVITE  -->
     * cancel
     * 100     <--
     * CANCEL  -->
     * 200OK(c)<--
     * 487     <--
     */
    void UT_MCEUsesCases_TestCallCancel4L();
    
    /**
     * establish
     * cancel
     */
    void UT_MCEUsesCases_TestCallCancel5L();
    
    /**
     * SESSION ESTABLISH
     * update
     * INVITE  -->
     * 100     <--
     * CANCEL  -->
     * 487    <--
     * 200 OK(c)<--
     */
    void UT_MCEUsesCases_TestCallUpdateCancelL();

    /**
     * SESSION ESTABLISH
     * update
     * INVITE  -->
     * 100     <--
     * CANCEL  -->
     * 200 OK(c)<--
     * 487(i) 
     */
     void UT_MCEUsesCases_TestCallUpdateCancel2L();

    /**
     * INVITE  <--
     * CANCEL  <--
     * 487     -->
     */
    void UT_MCEUsesCases_TestCalledCanceledL();


    /**
     * SESSION ESTABLISH
     * refresh
     * INVITE -->
     * 200 OK <--
     * ACK    -->
     * SESSION TERMINATE
     */
    void UT_MCEUsesCases_TestCallSessionRefreshL();


    /**
     * SESSION ESTABLISH
     * INVITE <--
     * 200 OK -->
     * ACK    <--
     * SESSION TERMINATE
     */
    void UT_MCEUsesCases_TestCallSessionRefreshedL();

    /**
     * establish
     * INVITE  -->
     * 100     <--
     * 200OK   <--
     * answer decoding fails
     * BYE     -->
     * 200 OK  <--
     */
    void UT_MCEUsesCases_TestCallFailedL();

    /**  
     * establish
     * INVITE  -->
     * 100     <--
     * 200OK   <--
     * media error
     * No Change to State, Media Error is Ignored.
     */
    void UT_MCEUsesCases_TestCallFailed2L();

    /** 
     * establish
     * INVITE  -->
     * 100     <--
     * media error
     * No Change to State, Media Error is Ignored.
     */
    void UT_MCEUsesCases_TestCallFailed3L();

	
    /**
     * establish
     * INVITE  -->
     * media error
     * No Change to State, Media Error is Ignored.
     */
        
    void UT_MCEUsesCases_TestCallFailed4L();
    
    /**
     * INVITE  <--
     * bad SDP
     * 500      -->
     */
    void UT_MCEUsesCases_TestCalledFailedL();

    /**
     * INVITE  <--
     * offer decoding fails
     * 488     -->
     */
    void UT_MCEUsesCases_TestCalledFailed2L();

    /**
     * INVITE  <--
     * update
     * create answer fails
     * 488     -->
     */
    void UT_MCEUsesCases_TestCalledFailed3L();

    /**
     * INVITE  <--
     * update
     * answer created fails
     * 500     -->
     */
    void UT_MCEUsesCases_TestCalledFailed4L();

    /**
     * SESSION ESTABLISH
     * update 
     * update creation fails (sync)
     * get original
     * SESSION TERMINATE
     */
    void UT_MCEUsesCases_TestCallUpdateFailedL();

    /**
     * SESSION ESTABLISH
     * update 
     * INVITE -->
     * 200 OK <--
     * answer to update decode fails
     * SESSION TERMINATE
     */
    void UT_MCEUsesCases_TestCallUpdateFailed3L();

    /**
     * SESSION ESTABLISH
     * update 
     * INVITE -->
     * 200 OK <--
     * answer to update creation fails
     * SESSION TERMINATE
     */
    void UT_MCEUsesCases_TestCallUpdateFailed4L();

    /**
     * SESSION ESTABLISH
     * update 
     * INVITE -->
     * 500    <--
     * SESSION TERMINATE
     */
    void UT_MCEUsesCases_TestCallUpdateFailed5L();
    
    /**
    * INVITE <--
    * 180 -> 
    * INVITE <--
    * 491 (2) <--
    * 200 (1) -->
    * ACK <--
    * SESSION TERMINATE
    */
    void UT_MCEUsesCases_TestCalledPendingL();
    
    /**
    * SESSION ESTABLISH (MO)
    * INVITE ->
    * 491 <-
    * INVITE -> (after timeout)
    * 200 <-
    * ACK
    * SESSION TERMINATE
    */
    void UT_MCEUsesCases_TestCallReInvitePendingL();
    
    /**
    * SESSION ESTABLISH (MT)
    * INVITE ->
    * 491 <-
    * INVITE -> (after timeout)
    * 200 <-
    * ACK
    * SESSION TERMINATE
    */
    void UT_MCEUsesCases_TestCalledReInvitePendingL();
    
    /**
    * SESSION ESTABLISH (MT)
    * INVITE ->
    * 491 <-
    * INVITE -> (after timeout)
    * 491 <-
    * SESSION TERMINATE
    */
    void UT_MCEUsesCases_TestCalledReInvitePendingFailedL();
    
    /**
    * INVITE ->
    * 180 <- one dialog
    * 200 <- other dialog
    * ACK ->
    * SESSION TERMINATE
    */
    void UT_MCEUsesCases_TestCallInviteForksL();
    
    /**
    * INVITE ->
    * 180 <- 
    * 200 <-
    * ACK ->
    * 200 <- retransmission
    * ACK ->
    * SESSION TERMINATE
    */
    void UT_MCEUsesCases_TestCallRetransmissionL();
    
    /**
    * INVITE ->
    * 421    <-
    * INVITE ->  -- Precondition supported.
    * 183    <-
    * PRACK  ->  -- includes SDP
    * 200    <-  -- includes SDP
    * UPDATE ->  -- includes SDP
    * 200    ->  -- includes SDP
    * 180    <-  
    * 200 
    * SESSION TERMINATE
    */
    void UT_MCEUsesCases_TestIMSR6toR5IOPL();
    
    /**
    * INVITE <-
    * 183(Require: 100rel) ->
    * PRACK <-
    * 200(p) ->
    * 200(i) ->
    * ACK <-
    * SESSION TERMINATE
    */
    void UT_MCEUsesCases_TestCalled100RelL();
    
    /**
    * INVITE ->
    * 180(Require: 100rel) <-
    * PRACK(no SDP) ->
    * 200(p) <-
    * 200(i) <-
    * ACK ->
    * SESSION TERMINATE
    */

    void UT_MCEUsesCases_TestCall100RelL();

    /**
    * INVITE            ->
    * 183 (RSeq 500)	<- 
    * PRACK             ->
    * 200 (183)         <-
    * 181 (RSeq 501)    <-	
    * PRACK             ->
    * 200 (181)         <-
    * 181 (RSeq 508)    <-        //NoPrack
    * 182 (RSeq 509)    <-        //NoPrack 
    * 180 (RSeq 508)    <-        //NoPrack
    * 180 (RSeq 502)    <-        
    * PRACK             ->
    * 200 (180)         <-
    * 200 (INVITE)      <-
    * ACK               ->
    * SEEION TERMINATE
    */
       
    void UT_MCEUsesCases_TestSubsequentPrack();
    
    /**
    * INVITE	        	->
    * 183(RSeq 500)	    <-	100rel required
    * PRACK		        ->	wit SDP
    * 200 (Invite)	    <-	200 OK with SDP
    * ACK		        ->
    * (check Transaction is removed i.e. pending transaction counter is dereseaed)
    * SESSION ESTABLISHED.    
    */

    void UT_MCEUsesCases_TestEarlyResponseToInvite();
    
    /**
    * INVITE		    ->
    * 183(RSeq 500)	    <-	100 rel required
    * PRACK		        ->	with SDP
    * 200 (Invite)	    <-	200 OK No SDP
    * ACK		        ->
    * (Session != EEstablished) (Session == EConfirming)
    * 200 (183)	        <- 	with SDP
    * SESSION ESTABLISHED.
    */

    void UT_MCEUsesCases_TestEarlyResponseToInvite1();
    
    /**
    * INVITE		    ->
    * 183(RSeq 500)	    <-	100 rel required
    * PRACK		        ->	
    * 200 (Invite)	    <-	200 OK No SDP
    * ACK		        ->
    * (Session != EEstablished) (Session == EConfirming)
    * ( Prack TimeOut)
    * BYE	        	->
    * SESSION TERMIATE
    */
   
    void UT_MCEUsesCases_TestEarlyResponseToInvite2();
    
    
   /**
    * INVITE	        -> with SDP
    * 183(RSeq 500)	    <- 100 rel required with SDP
    * PRACK		        ->	with SDP
    * ((Prack Time out occurs )
    * CANCEL            ->
    * 200 (c)           <-
    * SESSION TERMINATED 
    */
    
    void UT_MCEUsesCases_TestProvisionalResponseTimeOut();   
   
   
   /**
    * INVITE	        -> with SDP
    * 183(RSeq 500)	    <- 100 rel required with SDP
    * PRACK		        ->	with SDP
    * 200 (PRACK)       <-  with SDP
    * UPDATE            ->  with SDP
    * ((UPDATE Time out occurs )
    * CANCEL            ->
    * 200 (c)           <-
    * SESSION TERMINATED
    */
    void UT_MCEUsesCases_TestProvisionalResponseTimeOut1();   
    
    
    
    /**
     * INVITE	            -> with SDP
     * 183(RSeq 500)	    <- 100 rel required with SDP
     * PRACK		        ->	with SDP
     * 4xx                  <-
     * CANCEL               ->
     * 200 (c)              <-
     * SESSION TERMINATED
     */ 
 
    void UT_MCEUsesCases_TestProvisionalBadRequest();   
     
    
    /**
     * INVITE	            -> with SDP
     * 183(RSeq 500)	    <- 100 rel required with SDP
     * PRACK		        -> 
     * 200                  <- 
     * UPDATE               ->
     * 5xx                  <-
     * CANCEL               ->
     * 200 (c)              <-
     * SESSION TERMINATED
     */ 

    void UT_MCEUsesCases_TestProvisionalBadRequest1();
    
    
    /**
     * INVITE	            -> with SDP
     * 183(RSeq 500)	    <- 100 rel required with SDP
     * PRACK		        ->	with SDP
     * 4xx (INVITE)         <-
     * SESSION TERMINATED
     */ 
    
    void UT_MCEUsesCases_TestInviteBadRequest();   
    
            
   /** 
    * INVITE        with SDP
    * 100        
    * 200 (INVITE)
    * ACK 
    * INVITE-2      with SDP 
    * 180 (RSeq 500) (Session in Progress)
    * PRACK
    * 4xx ( INVITE-2 ) ( 4xx != KMceSipSessionIntervalTooSmall (422)
    *                  && 4xx != KMceSipExtensionRequired (421)
    *                  && 4xx != KMceSipRequestPending
    *                  && 4xx != KMceSipCallOrTransactionDoesNotExist
    *                  && 4xx != KMceSipRequestTimeout )
    * 
    * SESSION ESTABLISHED
    */                  

    void UT_MCEUsesCases_TestReInviteBadRequest();   
        
        
        
   /** 
    * INVITE        with SDP
    * 100        
    * 200 (INVITE)
    * ACK 
    * INVITE-2      with SDP 
    * 180 (RSeq 500) (Session in Progress)
    * PRACK
    * 4xx ( INVITE-2 ) 4xx != KMceSipCallOrTransactionDoesNotExist
    *
    * SESSION ESTABLISHED
    */                  

    void UT_MCEUsesCases_TestReInviteBadRequest1();           
    
    /** 
    * <-INVITE        with SDP
    * ->180
    * <-PRACK
    * ->481
    * ->200(i)
    * <-ACK
    * TERMINATE SESSION
    */        
    void UT_MCEUsesCases_TestReceivePrackInWrongState();
    
    /**
     * INVITE  <--
     * PRACK   <--
     * 481     -->
     * 200 OK  -->
     * ACK     <--
     * BYE     <--
     * 200 OK  -->
     */
    void UT_MCEUsesCases_TestCalledPrackedInWrongStateL();
    
    /**
     * INVITE <--
     * 180    --> (Require:100rel)
     * PRACK  <--
     * 200(p) -->
     * 200(i) -->
     * ACK    <--
     */
    void UT_MCEUsesCases_TestCalledReliableRingingL();
    
    /**
    * INVITE --> (Require: 100rel)
    * 180    <-- no Require: 100rel
    * CANCEL -->
    * 200(c) <--
    * 487(i) <--
    */
    void UT_MCEUsesCases_TestCallCancelAfterWrongReliableL();

    /**
     * INVITE  -->
     * 100     <--
     * 180     <--
     * 200 OK  <--
     * ACK     -->
     * BYE     -->
     * ICMP    <--
     */
    void UT_MCEUsesCases_TestIcmpForByeL();

    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 
		
	TMceIds iIds;
    CMceServerCore* iServerCore;
    CMceCsSession* iSession;
    CMceSipSession* iSipSession;
    
    private: //not Owned
    	    
    CMCETls* iStorage;
    
		
    };

#endif      //  __UT_MCEUsesCases_H__

// End of file

