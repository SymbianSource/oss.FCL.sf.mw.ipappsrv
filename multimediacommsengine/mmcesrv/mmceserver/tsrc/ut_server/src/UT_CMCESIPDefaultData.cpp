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
#include "ut_cmcesipdefaultdata.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sipstrings.h>
#include <badesca.h>

//  INTERNAL INCLUDES
#include "MCETestHelper.h"
#include "mcesipdefaultdata.h"


// CONSTRUCTION
UT_CMceSipDefaultData* UT_CMceSipDefaultData::NewL()
    {
    UT_CMceSipDefaultData* self = UT_CMceSipDefaultData::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceSipDefaultData* UT_CMceSipDefaultData::NewLC()
    {
    UT_CMceSipDefaultData* self = new( ELeave ) UT_CMceSipDefaultData();
    CleanupStack::PushL( self );

	self->ConstructL(); 


    return self;
    }

// Destructor (virtual by CBase)
UT_CMceSipDefaultData::~UT_CMceSipDefaultData()
    {
    }

// Default constructor
UT_CMceSipDefaultData::UT_CMceSipDefaultData()
    {
    }

// Second phase construct
void UT_CMceSipDefaultData::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceSipDefaultData::SetupL(  )
    {
    SIPStrings::OpenL();
    CDesC8Array* array = new ( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( array );
    array->AppendL( KHeader );
    array->AppendL( KHeader2 );
    iIndex = SipStrConsts::ENotify;
    RStringF string = SIPStrings::StringF( iIndex );
    CleanupClosePushL( string );
    iDefaultData = CMceSipDefaultData::NewL( string.DesC(), array, iDataArray );
    CleanupStack::PopAndDestroy(); // string
    CleanupStack::Pop( array );
    } 

void UT_CMceSipDefaultData::Teardown(  )
    {
    iDataArray.ResetAndDestroy();
    SIPStrings::Close();
   	}

void UT_CMceSipDefaultData::UT_CMceSipDefaultData_NewLL()
    {
    // Successfull
    RStringF invite = SIPStrings::StringF( SipStrConsts::EInvite );
    CleanupClosePushL( invite );
    CDesC8Array* array = new ( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( array );
    array->AppendL( KHeader );
    array->AppendL( KHeader2 );
    
    CMceSipDefaultData* data = 
    CMceSipDefaultData::NewL( invite.DesC(), array, iDataArray );
    CleanupStack::Pop( array );
    
    EUNIT_ASSERT( data );
    TInt index = MceSip::SIPStringIndex( invite.DesC() );
    CleanupStack::PopAndDestroy(); // invite
    EUNIT_ASSERT( data->iMethodIndex == index );
    EUNIT_ASSERT( data->iHeaders );
    EUNIT_ASSERT( data->iHeaders->Count() == 2 );
    EUNIT_ASSERT( data->iHeaders->MdcaPoint( 0 ).Compare( KHeader ) == 0 );
    EUNIT_ASSERT( data->iHeaders->MdcaPoint( 1 ).Compare( KHeader2 ) == 0 );
    iDataArray.ResetAndDestroy();
    data = NULL;
    
    // Unknown method
    CDesC8Array* array2 = new ( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( array2 );
    array2->AppendL( KHeader );
    EUNIT_ASSERT_LEAVE( data = CMceSipDefaultData::NewL( KMethod, array2, iDataArray ) );
    CleanupStack::PopAndDestroy( array2 );
    EUNIT_ASSERT( !data );
    
    //Panic with NULL headers
    RStringF refer = SIPStrings::StringF( SipStrConsts::ERefer );
    CleanupClosePushL( refer );
    CDesC8Array* arrayNull = NULL;
    CleanupStack::PushL( arrayNull );
    EUNIT_ASSERT_LEAVE( data = CMceSipDefaultData::NewL( refer.DesC(), arrayNull, iDataArray ) );
    CleanupStack::PopAndDestroy( arrayNull );
    CleanupStack::PopAndDestroy();//refer
    EUNIT_ASSERT( !data );
  	
    }
    
    
void UT_CMceSipDefaultData::UT_CMceSipDefaultData_NewL_1L()
    {
    
    CMceSipDefaultData* data = NULL;
    
    // Panic with empty headers
    RStringF subscribe = SIPStrings::StringF( SipStrConsts::ESubscribe );
    CleanupClosePushL( subscribe );
    CDesC8Array* arrayEmpty = new ( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( arrayEmpty );
    EUNIT_ASSERT_LEAVE( data = CMceSipDefaultData::NewL( subscribe.DesC(), arrayEmpty, iDataArray ) );
    CleanupStack::PopAndDestroy( arrayEmpty );
    CleanupStack::PopAndDestroy();//subscribe
    EUNIT_ASSERT( !data );
    
    }
   
    
void UT_CMceSipDefaultData::UT_CMceSipDefaultData_DefaultHeadersL()
    {
    CDesC8Array& defs = iDefaultData->DefaultHeaders();
    TInt count = defs.Count();
    EUNIT_ASSERT( count == iDefaultData->iHeaders->Count() );
    for ( TInt i = 0; i < count; i++ )
        {
        EUNIT_ASSERT( iDefaultData->iHeaders->MdcaPoint( i ).Compare( defs.MdcaPoint( i ) ) == 0 );    
        }
    }

void UT_CMceSipDefaultData::UT_CMceSipDefaultData_MatchL()
    {
    // TInt, successfull
    EUNIT_ASSERT( iDefaultData->Match( iIndex ) );
    
    // TInt, fail
    TInt fuu = iIndex + 1;
    EUNIT_ASSERT( !iDefaultData->Match( fuu ) );
    
    // TDesC8&, successfull
    RStringF string = SIPStrings::StringF( iIndex );
    CleanupClosePushL( string );
    EUNIT_ASSERT( iDefaultData->Match( string.DesC() ) );
    CleanupStack::PopAndDestroy(); // string
    
    // TDesC8&, fail
    EUNIT_ASSERT( !iDefaultData->Match( KMethod ) );
  	}


void UT_CMceSipDefaultData::UT_CMceSipDefaultData_SetHeadersL()
    {
    // Successfull
    CDesC8Array* array = new ( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( array );
    array->AppendL( KHeader3 );
    array->AppendL( KHeader4 );
    CleanupStack::Pop( array );
    iDefaultData->SetHeaders( array );
    EUNIT_ASSERT( iDefaultData->iHeaders );
    EUNIT_ASSERT( iDefaultData->iHeaders->Count() == 2 );
    EUNIT_ASSERT( iDefaultData->iHeaders->MdcaPoint( 0 ).Compare( KHeader3 ) == 0 );
    EUNIT_ASSERT( iDefaultData->iHeaders->MdcaPoint( 1 ).Compare( KHeader4 ) == 0);
    
    //Panic with NULL headers
    CDesC8Array* arrayNull = NULL;
    CleanupStack::PushL( arrayNull );
    // !! EUNIT_ASSERT_PANIC( iDefaultData->SetHeaders( arrayNull ), "USER", 0 );
    CleanupStack::PopAndDestroy( arrayNull );
    
    }

void UT_CMceSipDefaultData::UT_CMceSipDefaultData_SetHeaders_1L()
    {
    
    // Panic with empty headers
    CDesC8Array* arrayEmpty = new ( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( arrayEmpty );
    // !!EUNIT_ASSERT_PANIC( iDefaultData->SetHeaders( arrayEmpty ), "USER", 0 );
    CleanupStack::PopAndDestroy( arrayEmpty );

    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceSipDefaultData,
    "UT_CMceSipDefaultData",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMceSipDefaultData",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipDefaultData_NewLL, Teardown)

EUNIT_TEST(
    "NewL - test ",
    "CMceSipDefaultData",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipDefaultData_NewL_1L, Teardown)

EUNIT_TEST(
    "DefaultHeaders - test ",
    "CMceSipDefaultData",
    "DefaultHeaders",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipDefaultData_DefaultHeadersL, Teardown)

EUNIT_TEST(
    "Match - test ",
    "CMceSipDefaultData",
    "Match",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipDefaultData_MatchL, Teardown)

EUNIT_TEST(
    "SetHeaders - test ",
    "CMceSipDefaultData",
    "SetHeaders",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipDefaultData_SetHeadersL, Teardown)

EUNIT_TEST(
    "SetHeaders - test ",
    "CMceSipDefaultData",
    "SetHeaders",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipDefaultData_SetHeaders_1L, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
