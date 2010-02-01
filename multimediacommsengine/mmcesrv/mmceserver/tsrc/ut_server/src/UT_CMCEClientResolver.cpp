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




//  CLASS HEADER
#include "ut_cmceclientresolver.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sipcontenttypeheader.h>

//  INTERNAL INCLUDES
#include "MCETestHelper.h"
#include "sipclientdiscovery.h"
#include "mceactionset.h"
#include "mcesipsession.h"
#include "mceservercore.h"
#include "mcecssession.h"
#include "mceclientresolver.h"
#include "mcecomsession.h"


// CONSTRUCTION
UT_CMceClientResolver* UT_CMceClientResolver::NewL()
    {
    UT_CMceClientResolver* self = UT_CMceClientResolver::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceClientResolver* UT_CMceClientResolver::NewLC()
    {
    UT_CMceClientResolver* self = new( ELeave ) UT_CMceClientResolver();
    CleanupStack::PushL( self );

	self->ConstructL(); 


    return self;
    }

// Destructor (virtual by CBase)
UT_CMceClientResolver::~UT_CMceClientResolver()
    {
    }

// Default constructor
UT_CMceClientResolver::UT_CMceClientResolver()
    {
    }

// Second phase construct
void UT_CMceClientResolver::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceClientResolver::SetupL(  )
    {
    SIPStrings::OpenL();
    
    iClientResolver = CMceClientResolver::NewL( *this );
    
    } 

void UT_CMceClientResolver::Teardown(  )
    {
   	delete iClientResolver;

    SIPStrings::Close();
   	
    

    }


void UT_CMceClientResolver::IncomingResolvedRequest( TUid /*aClientUid*/,
                              CMceSipConnection& /*aConnection*/,
                              CSIPServerTransaction* aRequest )
    {
  	delete aRequest;
    }


void UT_CMceClientResolver::UT_CMceClientResolver_RegisterClientLL()
    {
  	TUid clientUid = TUid::Uid( 1 );
  	iClientResolver->RegisterClientL( clientUid );
    }
    

void UT_CMceClientResolver::UT_CMceClientResolver_DeregisterClientL()
    {
  	TUid clientUid = TUid::Uid( 1 );
  	iClientResolver->DeregisterClient( clientUid );
  	
    }

void UT_CMceClientResolver::UT_CMceClientResolver_ResolveIncomingRequestLL()
    {
    
    EUNIT_ASSERT ( iClientResolver->iList.IsEmpty() );
  	CMceSipConnection* connection = NULL;
    
  	CSIPServerTransaction* request =
  	  MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue );
  	CleanupStack::PushL( request );
  	
  	iClientResolver->RegisterIncomingRequestL( request, *connection );
  	CleanupStack::Pop( request );
  	
    EUNIT_ASSERT ( !iClientResolver->iList.IsEmpty() );

    }

void UT_CMceClientResolver::UT_CMceClientResolver_ResolveIncomingRequestL_1L()
    {
  	TUid clientUid = TUid::Uid( 1 );
    
  	iClientResolver->RegisterClientL( clientUid );

  	iClientResolver->iDiscovery->iRequestId = 1;

    EUNIT_ASSERT ( iClientResolver->iList.IsEmpty() );
  	CMceSipConnection* connection = NULL;

  	CSIPServerTransaction* request =
  	  MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue );
  	CleanupStack::PushL( request );
  	
  	iClientResolver->RegisterIncomingRequestL( request, *connection );

  	CleanupStack::Pop( request );
  	
    EUNIT_ASSERT ( !iClientResolver->iList.IsEmpty() );
    
  	iClientResolver->ChannelResolvedL( clientUid, iClientResolver->iDiscovery->iRequestId );
  	
    EUNIT_ASSERT ( iClientResolver->iList.IsEmpty() );


  	iClientResolver->iDiscovery->iRequestId++;

  	request =
  	  MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue );
  	CleanupStack::PushL( request );
  	
  	iClientResolver->RegisterIncomingRequestL( request, *connection );
  	CleanupStack::Pop( request );

    EUNIT_ASSERT ( !iClientResolver->iList.IsEmpty() );
    
    RPointerArray<CSIPHeaderBase> sipHeaders;
    CSIPHeaderBase::PushLC( &sipHeaders );

    CSIPContentTypeHeader* contenttype = 
        CSIPContentTypeHeader::NewLC( _L8("application"), _L8("sdp") ); //OK
    sipHeaders.AppendL( contenttype );
    CleanupStack::Pop( contenttype );
        
    HBufC8* content = KSDPMessage_OK1().AllocLC();
    
  	iClientResolver->ClientNotFoundL( iClientResolver->iDiscovery->iRequestId, 
  	                                  KMceSipBadRequest, 
  	                                  SIPStrings::StringF( SipStrConsts::EPhraseRequestTimeout ),
  	                                  sipHeaders,
  	                                  content );

    CleanupStack::Pop( content );
    CleanupStack::Pop( &sipHeaders );

    EUNIT_ASSERT ( iClientResolver->iList.IsEmpty() );

  	iClientResolver->iDiscovery->iRequestId++;

  	request =
  	  MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue );
  	CleanupStack::PushL( request );
  	
  	iClientResolver->RegisterIncomingRequestL( request, *connection );
  	CleanupStack::Pop( request );

    EUNIT_ASSERT ( !iClientResolver->iList.IsEmpty() );
    
  	iClientResolver->ErrorOccurred( KErrNotFound, iClientResolver->iDiscovery->iRequestId );

    EUNIT_ASSERT ( iClientResolver->iList.IsEmpty() );
    
    // No request elements in the transaction
    CSIPServerTransaction* srvtransaction = 
        CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EInvite ) ); 
    CleanupStack::PushL( srvtransaction );
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iClientResolver->RegisterIncomingRequestL( srvtransaction, *connection ), KErrNotFound );
    CleanupStack::PopAndDestroy( srvtransaction );
    
    // No to and from headers
   	request =
  	  MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue, EFalse );
  	CleanupStack::PushL( request );
  	iClientResolver->RegisterIncomingRequestL( request, *connection );
    CleanupStack::Pop( request );
  	EUNIT_ASSERT ( !iClientResolver->iList.IsEmpty() );
    }
    
    
    
    


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceClientResolver,
    "UT_CMceClientResolver",
    "UNIT" )

EUNIT_TEST(
    "RegisterClientL - test ",
    "CMceClientResolver",
    "RegisterClientL",
    "FUNCTIONALITY",
    SetupL, UT_CMceClientResolver_RegisterClientLL, Teardown)

EUNIT_TEST(
    "DeregisterClient - test ",
    "CMceClientResolver",
    "DeregisterClient",
    "FUNCTIONALITY",
    SetupL, UT_CMceClientResolver_DeregisterClientL, Teardown)

EUNIT_TEST(
    "ResolveIncomingRequestL - test ",
    "CMceClientResolver",
    "ResolveIncomingRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CMceClientResolver_ResolveIncomingRequestLL, Teardown)

EUNIT_TEST(
    "ResolveIncomingRequestL - test ",
    "CMceClientResolver",
    "ResolveIncomingRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CMceClientResolver_ResolveIncomingRequestL_1L, Teardown)



EUNIT_END_TEST_TABLE

//  END OF FILE
