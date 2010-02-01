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



#include "UT_CMCEReliableSender.h"

/* EXTERNAL INCLUDES */
#include <digia/eunit/EUnitMacros.h>
#include <sipconnection.h>
#include <centralrepository.h>
#include <SIPSDKCRKeys.h>
#include <centralrepository.h>
#include "MCETestHelper.h"
#include <siprackheader.h> 
#include <sipresponseelements.h> 
#include <sipcseqheader.h> 
#include <siprseqheader.h> 

/* INTERNAL INCLUDES */
#include "mcereliablesender.h"
#include "mceeventstatebase.h"


// =======================================================================================================================
// Two Phase Constructor ( NewL )
// =======================================================================================================================
//
UT_CMceReliableSender* UT_CMceReliableSender::NewL()
    {
	UT_CMceReliableSender* self = UT_CMceReliableSender::NewLC();
	CleanupStack::Pop();

 	return self; 
    }

// =======================================================================================================================
// Two Phase Constructor ( NewLC )
// =======================================================================================================================
//
UT_CMceReliableSender* UT_CMceReliableSender::NewLC()
    {
	UT_CMceReliableSender* self = new( ELeave ) UT_CMceReliableSender();
	CleanupStack::PushL( self );
	self->ConstructL();
	
	return self;
    }

// =======================================================================================================================
// Default Constructor. 
// =======================================================================================================================
//
UT_CMceReliableSender::UT_CMceReliableSender( )
    {
    }

// =======================================================================================================================
// Destructor ( virtual by CBase )
// =======================================================================================================================
//
UT_CMceReliableSender::~UT_CMceReliableSender( )
    {
    }

// =======================================================================================================================
// Second phase construct
// =======================================================================================================================
//
void UT_CMceReliableSender::ConstructL( )
    {
    /**
	 * The ConstructL from the base class CEUnitTestSuiteClass must be called. ;
	 * It generates the test case table.
	 */ 
	CEUnitTestSuiteClass::ConstructL( );
	}

// =======================================================================================================================
// Setting up function defination.    
// =======================================================================================================================
//
void UT_CMceReliableSender::SetupL( )
    { 
    SIPStrings::OpenL();
    iReliableSender = CMceReliableSender::NewL( *this );
    iReliableSender->iT1 = 50; 
    iTimeOut = 0;
    }

// =======================================================================================================================
// Setting up function defination.    
// =======================================================================================================================
//
void UT_CMceReliableSender::SetupL1( )
    { 
    SIPStrings::OpenL();
    iReliableSender = CMceReliableSender::NewL( *this );
    iReliableSender->iT1 = 50; 
    iTimeOut = 0;
    }

// =======================================================================================================================
// Teardonw function definiation.
// =======================================================================================================================
//
void UT_CMceReliableSender::Teardown( )
    {
    delete iReliableSender;
    SIPStrings::Close();
    }

// =======================================================================================================================
// Send Response Test
// =======================================================================================================================
// 
void UT_CMceReliableSender::UT_CMceReliableSender_SendResponseL()
    { 
    // Test 1: If there is no pending transaction.
    static TInt i = 0;       
    EUNIT_ASSERT( !iReliableSender->iTransactionStatus ); 

    CSIPServerTransaction* serverTransaction =
    	MCETestHelper::ServerTransactionL(SipStrConsts::EInvite, ETrue);
    CleanupStack::PushL( serverTransaction );
    
    _LIT8( KRinging, "Ringing" );
    RStringF ringing = SIPStrings::Pool().OpenFStringL( KRinging );
    CleanupClosePushL( ringing );
    
    CSIPResponseElements* aElements = CSIPResponseElements::NewL( 180, ringing );
    CleanupStack::PopAndDestroy(); // ringing

    CleanupStack::PushL( aElements );

    iReliableSender->iServerTransaction = serverTransaction;

    iReliableSender->SendResponseL( *serverTransaction, aElements );
    CleanupStack::Pop( aElements );
    
    CActiveScheduler::Start();    
            
    // Assert it should not go beyond 64T1
    EUNIT_ASSERT(iReliableSender->iRetransTimer >= 64);
        
    // Test 2: If there is pending tranaction.
    iReliableSender->iTransactionStatus = ETrue;
    MCE_EUNIT_ASSERT_LEAVE( iReliableSender->SendResponseL( *serverTransaction, aElements ) );          

    CleanupStack::PopAndDestroy( serverTransaction );    
    }

// =======================================================================================================================
// PrackReceived Test1 (Transaction Matched)
// =======================================================================================================================
//
void UT_CMceReliableSender::UT_CMceReliableSender_PrackReceivedL()
    { 
    TUint seq = 2;
    TUint cseq = KInviteCSeq;

    // Create a dummy Server Transaction
    CSIPServerTransaction* serverTransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue );
    CleanupStack::PushL( serverTransaction );
      
    iReliableSender->iServerTransaction = serverTransaction;
       
    // Create a Response Elements include the Rseq Header 
    
    _LIT8( KRinging, "Ringing" );
    RStringF ringing = SIPStrings::Pool().OpenFStringL( KRinging );
    CleanupClosePushL( ringing );
       
    CSIPResponseElements* aElements = CSIPResponseElements::NewL( 180,ringing );
    
    CleanupStack::PopAndDestroy();
    CleanupStack::PushL( aElements );
        
    CSIPMessageElements* mElements;
    mElements = &aElements->MessageElements();
    
    
    // Add the RSeqHeader to CSIPResponseElements i.e. to the CSIPMessgeElements of Response
 
    RPointerArray<CSIPHeaderBase> respHeaders; 
    CSIPHeaderBase::PushLC( &respHeaders );
    
    _LIT8( KInvitee, "INVITE" );
    RStringF invitee = SIPStrings::Pool().OpenFStringL( KInvitee );
    CleanupClosePushL( invitee );
       
    CSIPRSeqHeader* rseqHdr = new( ELeave ) CSIPRSeqHeader( seq );
    CleanupStack::PopAndDestroy();
    CleanupStack::PushL( rseqHdr ); 
        
    User::LeaveIfError( respHeaders.Append( rseqHdr ) );
    CleanupStack::Pop( rseqHdr );

    mElements->SetUserHeadersL( respHeaders );
    CleanupStack::Pop( &respHeaders );
    iReliableSender->iResponseElements = aElements;

    CleanupStack::Pop( aElements );
        

    /** Match the PRack Against saved Transaction in our case there is only one
     *  outstanding transaction .
     */
        
    TBool mTransaction = EFalse;
     
   
    /** Create a PRACK (Server Transaction) include, RACK Header such that it matches with CSeqNum, MethodName 
     *  in the Server Transaction & Rseq Header of the ResponseElements received prior 
     */

    
    RPointerArray<CSIPHeaderBase> sipHeaders2;
    CSIPHeaderBase::PushLC( &sipHeaders2 );
    
    _LIT8( KInvite2, "INVITE" );
    RStringF invite2 = SIPStrings::Pool().OpenFStringL( KInvite2 );
    CleanupClosePushL( invite2 );
       
    CSIPRAckHeader* rackHdr = CSIPRAckHeader::NewL( seq, cseq, invite2 );
    CleanupStack::PopAndDestroy();
    
    CleanupStack::PushL( rackHdr ); 
       
    User::LeaveIfError( sipHeaders2.Append( rackHdr ) );
    CleanupStack::Pop( rackHdr );
        
    CSIPServerTransaction* tPrack = MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, sipHeaders2, ETrue );
    CleanupStack::Pop( &sipHeaders2 );
    CleanupStack::PushL( tPrack );

 
    // Call the Method PRackReceived to send the PRACK 
  
    mTransaction = iReliableSender->PrackReceived( *tPrack );
    
    CleanupStack::PopAndDestroy( tPrack );
    
    CleanupStack::PopAndDestroy( serverTransaction );
             
    EUNIT_ASSERT( mTransaction == ETrue );         
	}

// =======================================================================================================================
// PrackReceived Test2 (Transaction not Matched)
// =======================================================================================================================
//
void UT_CMceReliableSender::UT_CMceReliableSender_PrackReceivedL1()
    {
    TUint seq = 2;
    TUint cseq = KInviteCSeq;

    // Create a dummy Server Transaction
       
    CSIPServerTransaction* serverTransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue );
    CleanupStack::PushL( serverTransaction );
      
    iReliableSender->iServerTransaction = serverTransaction;
        
    // Create a Response Elements include the Rseq Header 
    
    _LIT8( KRinging, "Ringing" );
    RStringF ringing = SIPStrings::Pool().OpenFStringL( KRinging );
    CleanupClosePushL( ringing );
       
    CSIPResponseElements* aElements = CSIPResponseElements::NewL( 180,ringing );
    
    CleanupStack::PopAndDestroy();
    CleanupStack::PushL( aElements );
        
    CSIPMessageElements* mElements;
    mElements = &aElements->MessageElements();
    
    // Add the RSeqHeader to CSIPResponseElements i.e. to the CSIPMessgeElements of Response
 
    RPointerArray<CSIPHeaderBase> respHeaders; 
    CSIPHeaderBase::PushLC( &respHeaders );
    
    _LIT8( KInvitee, "INVITE" );
    RStringF invitee = SIPStrings::Pool().OpenFStringL( KInvitee );
    CleanupClosePushL( invitee );
        
    CSIPRSeqHeader* rseqHdr = new( ELeave ) CSIPRSeqHeader( seq );
    CleanupStack::PopAndDestroy();
    CleanupStack::PushL( rseqHdr ); 
        
    User::LeaveIfError( respHeaders.Append( rseqHdr ) );
    CleanupStack::Pop( rseqHdr );

    mElements->SetUserHeadersL( respHeaders );
    CleanupStack::Pop( &respHeaders );
            
    iReliableSender->iResponseElements = aElements;

    CleanupStack::Pop( aElements );
        
    
    /** Create a PRACK (Server Transaction) include, RACK Header such that it matches with CSeqNum, MethodName 
     *  in the Server Transaction & dont match Rseq Header with the RSeq value in the ResponseElements received prior 
     */
           
    TBool mTransaction = EFalse;
    TUint cseq2 = 5004;
           
    RPointerArray<CSIPHeaderBase> sipHeaders2;
    CSIPHeaderBase::PushLC( &sipHeaders2 );
    
    _LIT8( KInvite2, "INVITE" );
    RStringF invite2 = SIPStrings::Pool().OpenFStringL( KInvite2 );
    CleanupClosePushL( invite2 );
       
    CSIPRAckHeader* rackHdr = CSIPRAckHeader::NewL( seq, cseq2, invite2 );
    CleanupStack::PopAndDestroy();
    
    CleanupStack::PushL( rackHdr ); 
       
    User::LeaveIfError( sipHeaders2.Append( rackHdr ) );
    CleanupStack::Pop( rackHdr );
            
    CSIPServerTransaction* tPrack = MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, sipHeaders2, ETrue );
    CleanupStack::Pop( &sipHeaders2 );
    CleanupStack::PushL( tPrack );
 
    // Call the Method PRackReceived to send the PRACK
    
    mTransaction = iReliableSender->PrackReceived( *tPrack );
    
    CleanupStack::PopAndDestroy( tPrack );
    
    CleanupStack::PopAndDestroy( serverTransaction );
             
    EUNIT_ASSERT( !mTransaction );
    }

// =======================================================================================================================
// TimeOut Test
// =======================================================================================================================
//
void UT_CMceReliableSender::UT_CMceReliableSender_timeOut()
    {      
    // Test 1: Check if the transction is in process already
    static TInt i = 0;       
    EUNIT_ASSERT(!iReliableSender->iTransactionStatus); 
    
   
    // Test 2: Send the Reliable Response for 64T1 and call the observer to
    // notify the timeout
    CSIPServerTransaction* serverTransaction =
    	MCETestHelper::ServerTransactionL(SipStrConsts::EInvite, ETrue);
    CleanupStack::PushL( serverTransaction );
    
    _LIT8( KRinging, "Ringing" );
    RStringF ringing = SIPStrings::Pool().OpenFStringL( KRinging );
    CleanupClosePushL( ringing );
    
    CSIPResponseElements* aElements =
    	CSIPResponseElements::NewL( 180, ringing );
    CleanupStack::PopAndDestroy(); // ringing

    CleanupStack::PushL( aElements );
       
    iReliableSender->iServerTransaction = serverTransaction;
    
    iReliableSender->SendResponseL( *serverTransaction, aElements );
    CActiveScheduler::Start();
        
    // Assert it has exceeded time out limit i.e. 64T1 
    EUNIT_ASSERT( iReliableSender->iRetransTimer > 64 )
    
    // Check some value for the observer to see if it has succesfully given time out value
    EUNIT_ASSERT( iTimeOut == 1 );
        
    CleanupStack::Pop( aElements );
    CleanupStack::PopAndDestroy( serverTransaction );
    }

// =======================================================================================================================
// GetRseq Test
// =======================================================================================================================
//
void UT_CMceReliableSender::UT_CMceReliableSender_GetRseq()
    {   
    TInt SNum;
    TInt tempSeq;
        
    CSIPServerTransaction* serverTransaction = MCETestHelper::ServerTransactionL( SipStrConsts::EInvite );
    CleanupStack::PushL( serverTransaction );
        
    // Test 1: If the Transaction in process return 0 to indicate error
    iReliableSender->iTransactionStatus = ETrue;
    SNum = iReliableSender->Rseq( *serverTransaction );
    EUNIT_ASSERT( SNum == 0 );
    
  
    // Test 2: If the transaction is newly created 
    iReliableSender->iTransactionStatus = EFalse;
    SNum = iReliableSender->Rseq( *serverTransaction );
    EUNIT_ASSERT( SNum == iReliableSender->iSequenceNumber ); 
   
     // Test 3: If the same server transaction want to send another reliable response
    iReliableSender->iServerTransaction = serverTransaction;
    iReliableSender->iTransactionStatus = EFalse;
    
    tempSeq = iReliableSender->iSequenceNumber;
    
    SNum = iReliableSender->Rseq( *serverTransaction );
        
    EUNIT_ASSERT( SNum == tempSeq + 1 );
        
    CleanupStack::Pop( serverTransaction );
    
    delete serverTransaction;
    }

// =========================================================================================================================
// Implemenation of NoPrackReceived Virtual Function.
// =========================================================================================================================
//
void UT_CMceReliableSender::NoPrackReceived( CSIPServerTransaction& /*aTransaction*/ )
    {
    CActiveScheduler::Stop(); 
    iTimeOut = 1;  // Time Out Flag 
    }
        
void UT_CMceReliableSender::ReliableFailed( CSIPServerTransaction& /*aTransaction*/, 
                                            TInt /*aError*/ )
    {
    }


EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceReliableSender,
    "UT_CMceReliableSender",
    "UNIT" )

EUNIT_TEST(
    "GetRseq - test ",
    "UT_CMceReliableSender",
    "GetRseq",
    "FUNCTIONALITY",
    SetupL, UT_CMceReliableSender_GetRseq, Teardown )

EUNIT_TEST(
    "SendResponse - test ",
    "UT_CMceReliableSender",
    "SendResponse",
    "FUNCTIONALITY",
    SetupL, UT_CMceReliableSender_SendResponseL, Teardown )

EUNIT_TEST(
    "PrackReceived - T1 ",
    "UT_CMceReliableSender",
    "PrackReceivedt",
    "FUNCTIONALITY",
    SetupL1, UT_CMceReliableSender_PrackReceivedL, Teardown )

EUNIT_TEST(
    "PrackReceived - T2 ",
    "UT_CMceReliableSender",
    "PrackReceivedt",
    "FUNCTIONALITY",
    SetupL1, UT_CMceReliableSender_PrackReceivedL1, Teardown )
    
EUNIT_TEST(
    "TimeOut - test ",
    "UT_CMceReliableSender",
    "TimeOut",
    "FUNCTIONALITY",
    SetupL, UT_CMceReliableSender_timeOut, Teardown )

EUNIT_END_TEST_TABLE
