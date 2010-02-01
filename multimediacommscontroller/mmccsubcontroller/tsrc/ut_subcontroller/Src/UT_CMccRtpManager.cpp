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
#include "UT_CMccRtpManager.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "Mccrtpmanager.h"
#include "Mcctesteventhandler.h"
#include "mccresourcepool_stub.h"
#include "mccunittestmacros.h"

// CONSTRUCTION
UT_CMccRtpManager* UT_CMccRtpManager::NewL()
    {
    UT_CMccRtpManager* self = UT_CMccRtpManager::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccRtpManager* UT_CMccRtpManager::NewLC()
    {
    UT_CMccRtpManager* self = new( ELeave ) UT_CMccRtpManager();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccRtpManager::~UT_CMccRtpManager()
    {
    }

// Default constructor
UT_CMccRtpManager::UT_CMccRtpManager() :
    iRtpMan( NULL )
    {
    }

// Second phase construct
void UT_CMccRtpManager::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccRtpManager::SetupL(  )
    {
    iHandler = CMccTestEventHandler::NewL();
    iResources = CMccResourcePoolStub::NewL();
    iRtpMan = CMccRtpManager::NewL( *iHandler, *iResources, iMccSessionId );
    iHandler->iLastEvent = TMccEvent();
    }

void UT_CMccRtpManager::Teardown(  )
    {
    if ( iRtpMan )
        {
        delete iRtpMan;
        }
    if ( iHandler )
        {
        delete iHandler;
        }
    delete iResources;
    }

void UT_CMccRtpManager::UT_CMccRtpManager_RemoveStreamFromArray()
    {
    TRtpId rtpid(3);
    iRtpMan->RemoveStreamFromArray( rtpid );
    TInt err = iRtpMan->iStreamArray.Append( rtpid );
    EUNIT_ASSERT( !err || err == KErrNoMemory );
    iRtpMan->RemoveStreamFromArray( rtpid );
    }
    
void UT_CMccRtpManager::UT_CMccRtpManager_IsOpen()
    {
    EUNIT_ASSERT( !iRtpMan->IsOpen() );
    }
 
 
void UT_CMccRtpManager::UT_CMccRtpManager_CheckStreamExists()
    {

    TRtpId rtpid(3);
    EUNIT_ASSERT( !iRtpMan->CheckStreamExists( rtpid ) );
    TInt err = iRtpMan->iStreamArray.Append( rtpid );
    EUNIT_ASSERT( !err || err == KErrNoMemory );
    EUNIT_ASSERT( err == KErrNoMemory || iRtpMan->CheckStreamExists( rtpid ) );

    }
 

void UT_CMccRtpManager::UT_CMccRtpManager_CreateSessionLL(  )
    {
    TUint port( 5000 );
    MCC_EUNIT_ASSERT_NO_LEAVE( iRtpMan->CreateSessionL( port, EFalse, EFalse ) );
    
    // KErrAlreadyExists
    EUNIT_ASSERT_LEAVE( iRtpMan->CreateSessionL( port, EFalse, EFalse ) );
    }

void UT_CMccRtpManager::UT_CMccRtpManager_CloseSessionL(  )
    {
    iRtpMan->CloseSession();
    
    TUint port( 5000 );
    MCC_EUNIT_ASSERT_NO_LEAVE( iRtpMan->CreateSessionL( port, EFalse, EFalse ) );

    iRtpMan->CloseSession();
    }

void UT_CMccRtpManager::UT_CMccRtpManager_GetRtpReferenceLL(  )
    {
    CRtpAPI* rtp( NULL );
    EUNIT_ASSERT_LEAVE( rtp = iRtpMan->GetRtpReferenceL() );
    }

void UT_CMccRtpManager::UT_CMccRtpManager_SetRemoteAddressL(  )
    {
    TInetAddr addr;
    EUNIT_ASSERT_EQUALS( iRtpMan->SetRemoteAddress( addr ), KErrNotFound );

    TUint port( 5000 );
    MCC_EUNIT_ASSERT_NO_LEAVE( iRtpMan->CreateSessionL( port, EFalse, EFalse ) );

    addr.SetPort( port );
    EUNIT_ASSERT_EQUALS( iRtpMan->SetRemoteAddress( addr ), KErrNone );
    EUNIT_ASSERT( iRtpMan->iRemoteAddress.Address() == addr.Address() )
    EUNIT_ASSERT( iRtpMan->iRemoteAddress.Port() == addr.Port() )
    
    // Port not changed
    EUNIT_ASSERT_EQUALS( iRtpMan->SetRemoteAddress( addr ), KErrNone );
    
    // Port changed
    TUint port2( 5002 );
    addr.SetPort( port2 );
    EUNIT_ASSERT_EQUALS( iRtpMan->SetRemoteAddress( addr ), KErrNone );
    EUNIT_ASSERT( iRtpMan->iRemoteAddress.Address() == addr.Address() )
    EUNIT_ASSERT( iRtpMan->iRemoteAddress.Port() == addr.Port() )
    }
    
    
void UT_CMccRtpManager::UT_CMccRtpManager_SetRemoteRtcpAddrL(  )
    {
    TInetAddr addr;
    EUNIT_ASSERT_EQUALS( iRtpMan->SetRemoteRtcpAddr( addr ), KErrNotFound );

    TUint port( 5000 );
    MCC_EUNIT_ASSERT_NO_LEAVE( iRtpMan->CreateSessionL( port, EFalse, EFalse ) );

    addr.SetPort( port );
    EUNIT_ASSERT_EQUALS( iRtpMan->SetRemoteRtcpAddr( addr ), KErrNone );
    EUNIT_ASSERT( iRtpMan->iRemoteRtcpAddr.Address() == addr.Address() )
    EUNIT_ASSERT( iRtpMan->iRemoteRtcpAddr.Port() == addr.Port() )
    
    // Port not changed
    EUNIT_ASSERT_EQUALS( iRtpMan->SetRemoteRtcpAddr( addr ), KErrNone );
    
    // Port changed
    TUint port2( 5002 );
    addr.SetPort( port2 );
    EUNIT_ASSERT_EQUALS( iRtpMan->SetRemoteRtcpAddr( addr ), KErrNone );
    EUNIT_ASSERT( iRtpMan->iRemoteRtcpAddr.Address() == addr.Address() )
    EUNIT_ASSERT( iRtpMan->iRemoteRtcpAddr.Port() == addr.Port() )
    }    

void UT_CMccRtpManager::UT_CMccRtpManager_SetIpTOSL(  )
    {
    iRtpMan->SetIpTOS( 0 );
    }

void UT_CMccRtpManager::UT_CMccRtpManager_StartSessionLL(  )
    {
    EUNIT_ASSERT_LEAVE( iRtpMan->StartSessionL() );
    
    TUint port( 5000 );
    MCC_EUNIT_ASSERT_NO_LEAVE( iRtpMan->CreateSessionL( port, EFalse, EFalse ) );

    // StartSessionL contains a special fix for TEST_EUNIT 
    MCC_EUNIT_ASSERT_NO_LEAVE( iRtpMan->StartSessionL() );
    }

void UT_CMccRtpManager::UT_CMccRtpManager_InitializeLL(  )
    {
    TRequestStatus status;
    MCC_EUNIT_ASSERT_NO_LEAVE( iRtpMan->InitializeL( status, 5 ) );
    User::WaitForAnyRequest();
    }

void UT_CMccRtpManager::UT_CMccRtpManager_SessionIdL(  )
    {
    TRtpId id = iRtpMan->SessionId();
    }

void UT_CMccRtpManager::UT_CMccRtpManager_CreateTransmitStreamLL(  )
    {
    //EUNIT_ASSERT( ETrue );
    }

void UT_CMccRtpManager::UT_CMccRtpManager_CreateReceiveStreamLL(  )
    {
    //EUNIT_ASSERT( ETrue );
    }

void UT_CMccRtpManager::UT_CMccRtpManager_RefreshDownlinkLL(  )
    {
    //EUNIT_ASSERT( ETrue );
    }

void UT_CMccRtpManager::UT_CMccRtpManager_SetRTCPSendL(  )
    {
    EUNIT_ASSERT_EQUALS( iRtpMan->SetRTCPSendReceive( ETrue ), KErrNotFound );
    
    TUint port( 5000 );
    MCC_EUNIT_ASSERT_NO_LEAVE( iRtpMan->CreateSessionL( port, EFalse, EFalse ) );
    
    EUNIT_ASSERT_EQUALS( iRtpMan->SetRTCPSendReceive( ETrue ), KErrNone );
    }

void UT_CMccRtpManager::UT_CMccRtpManager_ErrorNotifyL(  )
    {
    iRtpMan->ErrorNotify( KErrNone );
    EUNIT_ASSERT_EQUALS( iHandler->iLastEvent.iEventType, KMccEventNone );
    
    iRtpMan->ErrorNotify( KErrGeneral );
    EUNIT_ASSERT_EQUALS( iHandler->iLastEvent.iEventType, KMccEventNone );
    
    // test handling of ICMP host unreachable error
    iRtpMan->ErrorNotify( KErrHostUnreach );
    EUNIT_ASSERT_EQUALS( iHandler->iLastEvent.iEventType, KMccStreamError );
    EUNIT_ASSERT_EQUALS( iHandler->iLastEvent.iEventCategory, KMccEventCategoryRtp );
    EUNIT_ASSERT_EQUALS( iHandler->iLastEvent.iErrorCode, KErrHostUnreach );
    
    // test handling of ICMP port unreachable error
    iRtpMan->ErrorNotify( KErrCouldNotConnect );
    EUNIT_ASSERT_EQUALS( iHandler->iLastEvent.iEventType, KMccStreamError );
    EUNIT_ASSERT_EQUALS( iHandler->iLastEvent.iEventCategory, KMccEventCategoryRtp );
    EUNIT_ASSERT_EQUALS( iHandler->iLastEvent.iErrorCode, KErrHostUnreach );
    }

void UT_CMccRtpManager::UT_CMccRtpManager_GetLocalIpAddressesL()
    {
    TMccCreateLink createlink;
    
    iRtpMan->iRtpSessionId = KNullId;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iRtpMan->GetLocalIpAddressesL( createlink ), KErrNotReady );
    
    iRtpMan->iRtpSessionId = 5;
    delete iRtpMan->iRtpApi;
    iRtpMan->iRtpApi = NULL;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iRtpMan->GetLocalIpAddressesL( createlink ), KErrNotReady );
    
    iRtpMan->iRtpApi = CRtpAPI::NewL( *iRtpMan );
    
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iRtpMan->GetLocalIpAddressesL( createlink ), KErrGeneral );
    }

void UT_CMccRtpManager::UT_CMccRtpManager_SetCNameL()
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iRtpMan->SetCNameL( _L8("Name") ) );  
    }
    
void UT_CMccRtpManager::UT_CMccRtpManager_QoSParamEvent()
    {
    TInt error = 0; 
    iRtpMan->QoSParamEvent(error, 0);  
    }

void UT_CMccRtpManager::UT_CMccRtpManager_QoSConnEvent()
    {
    TInt error = 0; 
    iRtpMan->QoSConnEvent(error, 0);  
    }
    


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccRtpManager,
    "CMccRtpManager",
    "UNIT" )

EUNIT_TEST(
    "RemoveStreamFromArray - test ",
    "CMccRtpManager",
    "RemoveStreamFromArray",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpManager_RemoveStreamFromArray, Teardown)

EUNIT_TEST(
    "IsOpen - test ",
    "CMccRtpManager",
    "IsOpen",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpManager_IsOpen, Teardown)
    
EUNIT_TEST(
    "CheckStreamExists - test ",
    "CMccRtpManager",
    "CheckStreamExists",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpManager_CheckStreamExists, Teardown)
  
EUNIT_TEST(
    "CreateSessionL - test ",
    "CMccRtpManager",
    "CreateSessionL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpManager_CreateSessionLL, Teardown)

EUNIT_TEST(
    "CloseSession - test ",
    "CMccRtpManager",
    "CloseSession",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpManager_CloseSessionL, Teardown)

EUNIT_TEST(
    "GetRtpReferenceL - test ",
    "CMccRtpManager",
    "GetRtpReferenceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpManager_GetRtpReferenceLL, Teardown)

EUNIT_TEST(
    "SetRemoteAddress - test ",
    "CMccRtpManager",
    "SetRemoteAddress",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpManager_SetRemoteAddressL, Teardown)

EUNIT_TEST(
    "SetIpTOS - test ",
    "CMccRtpManager",
    "SetIpTOS",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpManager_SetIpTOSL, Teardown)

EUNIT_TEST(
    "StartSessionL - test ",
    "CMccRtpManager",
    "StartSessionL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpManager_StartSessionLL, Teardown)

EUNIT_TEST(
    "InitializeL - test ",
    "CMccRtpManager",
    "InitializeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpManager_InitializeLL, Teardown)

EUNIT_TEST(
    "SessionId - test ",
    "CMccRtpManager",
    "SessionId",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpManager_SessionIdL, Teardown)

EUNIT_TEST(
    "CreateTransmitStreamL - test ",
    "CMccRtpManager",
    "CreateTransmitStreamL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpManager_CreateTransmitStreamLL, Teardown)

EUNIT_TEST(
    "CreateReceiveStreamL - test ",
    "CMccRtpManager",
    "CreateReceiveStreamL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpManager_CreateReceiveStreamLL, Teardown)

EUNIT_TEST(
    "RefreshDownlinkL - test ",
    "CMccRtpManager",
    "RefreshDownlinkL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpManager_RefreshDownlinkLL, Teardown)

EUNIT_TEST(
    "SetRTCPSend - test ",
    "CMccRtpManager",
    "SetRTCPSend",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpManager_SetRTCPSendL, Teardown)

EUNIT_TEST(
    "ErrorNotify - test ",
    "CMccRtpManager",
    "ErrorNotify",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpManager_ErrorNotifyL, Teardown)

EUNIT_TEST(
    "GetLocalIpAddressesL - test ",
    "CMccRtpManager",
    "GetLocalIpAddressesL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpManager_GetLocalIpAddressesL, Teardown)

EUNIT_TEST(
    "SetCNameL - test ",
    "CMccRtpManager",
    "SetCNameL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpManager_SetCNameL, Teardown)

EUNIT_TEST(
    "QoSParamEvent - test ",
    "CMccRtpManager",
    "QoSParamEvent",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpManager_QoSParamEvent, Teardown)

EUNIT_TEST(
    "QoSConnEvent - test ",
    "CMccRtpManager",
    "QoSConnEvent",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpManager_QoSConnEvent, Teardown)





EUNIT_END_TEST_TABLE

//  END OF FILE
