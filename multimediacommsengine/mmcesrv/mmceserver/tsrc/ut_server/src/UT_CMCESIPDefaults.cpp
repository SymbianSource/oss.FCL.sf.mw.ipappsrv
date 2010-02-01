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
#include "ut_cmcesipdefaults.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sipstrings.h>
#include <badesca.h>

//  INTERNAL INCLUDES
#include "MCETestHelper.h"
#include "mcesipdefaults.h"
#include "mcesipdefaultdata.h"

// CONSTRUCTION
UT_CMceSipDefaults* UT_CMceSipDefaults::NewL()
    {
    UT_CMceSipDefaults* self = UT_CMceSipDefaults::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceSipDefaults* UT_CMceSipDefaults::NewLC()
    {
    UT_CMceSipDefaults* self = new( ELeave ) UT_CMceSipDefaults();
    CleanupStack::PushL( self );

	self->ConstructL(); 


    return self;
    }

// Destructor (virtual by CBase)
UT_CMceSipDefaults::~UT_CMceSipDefaults()
    {
    }

// Default constructor
UT_CMceSipDefaults::UT_CMceSipDefaults()
    {
    }

// Second phase construct
void UT_CMceSipDefaults::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceSipDefaults::SetupL(  )
    {
    SIPStrings::OpenL();
    iDefaults = CMceSipDefaults::NewL();
    } 

void UT_CMceSipDefaults::Teardown(  )
    {
    delete iDefaults;
    SIPStrings::Close();
   	}

void UT_CMceSipDefaults::UT_CMceSipDefaults_StoreDefaultHeadersLL()
    {
    // for a method
    
    EUNIT_ASSERT( iDefaults->iDataArray.Count() == 0 );
    // Successfull, method not stored
    RStringF invite = SIPStrings::StringF( SipStrConsts::EInvite );
    CleanupClosePushL( invite );
    CDesC8Array* inviteHdrs = new( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( inviteHdrs );
    inviteHdrs->AppendL( KHeader );
    inviteHdrs->AppendL( KHeader2 );
    
    iDefaults->StoreDefaultHeadersL( invite.DesC(), inviteHdrs );
    CleanupStack::Pop( inviteHdrs );
    CleanupStack::PopAndDestroy(); // invite
    
    EUNIT_ASSERT( iDefaults->iDataArray.Count() == 1 );
    CMceSipDefaultData* inviteData = iDefaults->iDataArray[ 0 ];
    EUNIT_ASSERT( inviteData->iMethodIndex == SipStrConsts::EInvite );
    EUNIT_ASSERT( inviteData->iHeaders );
    EUNIT_ASSERT( inviteData->iHeaders->Count() == 2 );
    EUNIT_ASSERT( inviteData->iHeaders->MdcaPoint( 0 ).Compare( KHeader ) == 0 );
    EUNIT_ASSERT( inviteData->iHeaders->MdcaPoint( 1 ).Compare( KHeader2 ) == 0 );
    
    // Store another method
    RStringF subscribe = SIPStrings::StringF( SipStrConsts::ESubscribe );
    CleanupClosePushL( subscribe );
    CDesC8Array* subscribeHdrs = new( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( subscribeHdrs );
    subscribeHdrs->AppendL( KHeader3 );
    
    iDefaults->StoreDefaultHeadersL( subscribe.DesC(), subscribeHdrs );
    CleanupStack::Pop( subscribeHdrs );
    CleanupStack::PopAndDestroy(); // subscribe
    
    EUNIT_ASSERT( iDefaults->iDataArray.Count() == 2 );
    CMceSipDefaultData* subscribeData = iDefaults->iDataArray[ 1 ];
    EUNIT_ASSERT( subscribeData->iMethodIndex == SipStrConsts::ESubscribe );
    EUNIT_ASSERT( subscribeData->iHeaders );
    EUNIT_ASSERT( subscribeData->iHeaders->Count() == 1 );
    EUNIT_ASSERT( subscribeData->iHeaders->MdcaPoint( 0 ).Compare( KHeader3 ) == 0 );
    
    // Change headers for a method
    RStringF subscribe2 = SIPStrings::StringF( SipStrConsts::ESubscribe );
    CleanupClosePushL( subscribe2 );
    CDesC8Array* subscribeHdrs2 = new( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( subscribeHdrs2 );
    subscribeHdrs2->AppendL( KHeader );
    subscribeHdrs2->AppendL( KHeader2 );
    subscribeHdrs2->AppendL( KHeader3 );
    
    iDefaults->StoreDefaultHeadersL( subscribe2.DesC(), subscribeHdrs2 );
    CleanupStack::Pop( subscribeHdrs2 );
    CleanupStack::PopAndDestroy(); // subscribe2
    
    EUNIT_ASSERT( iDefaults->iDataArray.Count() == 2 );
    CMceSipDefaultData* subscribeData2 = iDefaults->iDataArray[ 1 ];
    EUNIT_ASSERT( subscribeData2->iMethodIndex == SipStrConsts::ESubscribe );
    EUNIT_ASSERT( subscribeData2->iHeaders );
    EUNIT_ASSERT( subscribeData2->iHeaders->Count() == 3 );
    EUNIT_ASSERT( subscribeData2->iHeaders->MdcaPoint( 0 ).Compare( KHeader ) == 0 );
    EUNIT_ASSERT( subscribeData2->iHeaders->MdcaPoint( 1 ).Compare( KHeader2 ) == 0 );
    EUNIT_ASSERT( subscribeData2->iHeaders->MdcaPoint( 2 ).Compare( KHeader3 ) == 0 );
    
    // method found, headers NULL
    RStringF invite2 = SIPStrings::StringF( SipStrConsts::EInvite );
    CleanupClosePushL( invite2 );
    iDefaults->StoreDefaultHeadersL( invite2.DesC(), NULL );
    CleanupStack::PopAndDestroy(); //invite2
    
    EUNIT_ASSERT( iDefaults->iDataArray.Count() == 1 );
    CMceSipDefaultData* data = iDefaults->iDataArray[ 0 ];
    EUNIT_ASSERT( data->iMethodIndex != SipStrConsts::EInvite );
    
    // method not found, headers empty
    RStringF refer = SIPStrings::StringF( SipStrConsts::ERefer );
    CleanupClosePushL( refer );
    CDesC8Array* emptyHdrs = new ( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( emptyHdrs );
    iDefaults->StoreDefaultHeadersL( refer.DesC(), emptyHdrs );
    CleanupStack::Pop( emptyHdrs );
    CleanupStack::PopAndDestroy(); //refer
    
    EUNIT_ASSERT( iDefaults->iDataArray.Count() == 1 );
    CMceSipDefaultData* data2 = iDefaults->iDataArray[ 0 ];
    EUNIT_ASSERT( data2->iMethodIndex != SipStrConsts::ERefer );
    
    }
    
void UT_CMceSipDefaults::UT_CMceSipDefaults_StoreDefaultHeadersL2L()
    {
    // For array of methods
    CDesC8Array* methods = new( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( methods );
    
    // First with empty methods
    CDesC8Array* hdrs = new( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( hdrs );
    hdrs->AppendL( KHeader );
    iDefaults->StoreDefaultHeadersL( *methods, hdrs );
    CleanupStack::Pop( hdrs );
    
    EUNIT_ASSERT( iDefaults->iDataArray.Count() == 0 );
    
    // Add some methods methods
    RStringF invite = SIPStrings::StringF( SipStrConsts::EInvite );
    CleanupClosePushL( invite );
    methods->AppendL( invite.DesC() );
    CleanupStack::PopAndDestroy();//invite
    
    RStringF notify = SIPStrings::StringF( SipStrConsts::ENotify );
    CleanupClosePushL( notify );
    methods->AppendL( notify.DesC() );
    CleanupStack::PopAndDestroy();//notify
    
    RStringF refer = SIPStrings::StringF( SipStrConsts::ERefer );
    CleanupClosePushL( refer );
    methods->AppendL( refer.DesC() );
    CleanupStack::PopAndDestroy();//refer
    
    CDesC8Array* hdrs2 = new( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( hdrs2 );
    hdrs2->AppendL( KHeader );
    iDefaults->StoreDefaultHeadersL( *methods, hdrs2 );
    CleanupStack::Pop( hdrs2 );
    
    EUNIT_ASSERT( iDefaults->iDataArray.Count() == 3 );
    
    // Again with empty methods
    methods->Reset();
    
    CDesC8Array* hdrs3 = new( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( hdrs3 );
    hdrs3->AppendL( KHeader );
    iDefaults->StoreDefaultHeadersL( *methods, hdrs3 );
    CleanupStack::Pop( hdrs3 );
    
    EUNIT_ASSERT( iDefaults->iDataArray.Count() == 0 );
    CleanupStack::PopAndDestroy( methods );
    }
    
void UT_CMceSipDefaults::UT_CMceSipDefaults_DefaultHeadersL()
    {
    // Store headers for a method
    
    RStringF invite = SIPStrings::StringF( SipStrConsts::EInvite );
    CleanupClosePushL( invite );
    
    CDesC8Array* methods = new( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( methods );
    methods->AppendL( invite.DesC() );
    
    CDesC8Array* storeHdrs = new( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( storeHdrs );
    storeHdrs->AppendL( KHeader );
    
    iDefaults->StoreDefaultHeadersL( *methods, storeHdrs );
    CleanupStack::Pop( storeHdrs );
    CleanupStack::PopAndDestroy( methods );
    
    // index, not found
    CDesC8Array& hdrs = iDefaults->DefaultHeaders( SipStrConsts::ENotify );
    EUNIT_ASSERT( hdrs.Count() == 0 );
    
    // index, found
    CDesC8Array& hdrs2 = iDefaults->DefaultHeaders( SipStrConsts::EInvite );
    EUNIT_ASSERT( hdrs2.Count() == 1 );
    EUNIT_ASSERT( hdrs2.MdcaPoint( 0 ).Compare( KHeader ) == 0 );
    
    // method, not found
    CDesC8Array& hdrs3 = iDefaults->DefaultHeaders( KMethod );
    EUNIT_ASSERT( hdrs3.Count() == 0 );
    
    // method, found
    CDesC8Array& hdrs4 = iDefaults->DefaultHeaders( invite.DesC() );
    EUNIT_ASSERT( hdrs4.Count() == 1 );
    EUNIT_ASSERT( hdrs4.MdcaPoint( 0 ).Compare( KHeader ) == 0 ); 
    
    CleanupStack::PopAndDestroy();//invite
  	}


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceSipDefaults,
    "UT_CMceSipDefaults",
    "UNIT" )

EUNIT_TEST(
    "StoreDefaultHeadersL - method ",
    "CMceSipDefaults",
    "StoreDefaultHeadersL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipDefaults_StoreDefaultHeadersLL, Teardown)

EUNIT_TEST(
    "StoreDefaultHeadersL - array ",
    "CMceSipDefaults",
    "StoreDefaultHeadersL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipDefaults_StoreDefaultHeadersL2L, Teardown)


EUNIT_TEST(
    "DefaultHeaders - test ",
    "CMceSipDefaults",
    "DefaultHeaders",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipDefaults_DefaultHeadersL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
