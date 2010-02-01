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
#include "ut_cmcedictionary.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

//  INTERNAL INCLUDES
#include "mcedictionary.h"
#include "MCETestHelper.h"

const TInt KKey1 = -22;
const TInt KKey2 = 0;
const TUint KUintValue = 456;
const TUint KUintValue2 = 675;
_LIT8( KStringValue, "some value" );
_LIT8( KStringValue2, "another value" );

// CONSTRUCTION
UT_CMceDictionary* UT_CMceDictionary::NewL()
    {
    UT_CMceDictionary* self = UT_CMceDictionary::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceDictionary* UT_CMceDictionary::NewLC()
    {
    UT_CMceDictionary* self = new( ELeave ) UT_CMceDictionary();
    CleanupStack::PushL( self );

	self->ConstructL(); 


    return self;
    }

// Destructor (virtual by CBase)
UT_CMceDictionary::~UT_CMceDictionary()
    {
    }

// Default constructor
UT_CMceDictionary::UT_CMceDictionary()
    {
    }

// Second phase construct
void UT_CMceDictionary::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceDictionary::SetupL(  )
    {
    iDictionary = new ( ELeave ) CMceDictionary();
    } 

void UT_CMceDictionary::Teardown(  )
    {
    delete iDictionary;
    }

void UT_CMceDictionary::UT_CMceDictionaryItem_KeyL()
    {
    CMceDictionaryItem* item = new( ELeave ) CMceKeyTValuePair<TInt>( KKey1, KUintValue  );
    EUNIT_ASSERT( item->Key() == KKey1 );
    delete item;
    
    HBufC8* valueBuf = KStringValue().AllocLC();
    item = new( ELeave ) CMceKeyCValuePair<HBufC8>( KKey2, valueBuf );
    CleanupStack::Pop( valueBuf );
    EUNIT_ASSERT( item->Key() == KKey2 );
    delete item;
    }  

void UT_CMceDictionary::UT_CMceKeyTValuePair_ValueL()
    {
    CMceKeyTValuePair<TInt>* item = new( ELeave ) CMceKeyTValuePair<TInt>( KKey1, KUintValue  );
    EUNIT_ASSERT( item->Value() == KUintValue );
    delete item;
    }

void UT_CMceDictionary::UT_CMceKeyTValuePair_SetValueL()
    {
    CMceKeyTValuePair<TInt>* item = new( ELeave ) CMceKeyTValuePair<TInt>( KKey2, KUintValue  );
    item->SetValue( KUintValue2 );
    EUNIT_ASSERT( item->iValue == KUintValue2 );
    delete item;
    }

void UT_CMceDictionary::UT_CMceKeyCValuePair_ValueL()
    {
    HBufC8* valueBuf = KStringValue().AllocLC();
    CMceKeyCValuePair<HBufC8>* item = new( ELeave ) CMceKeyCValuePair<HBufC8>( KKey1, valueBuf );
    CleanupStack::Pop( valueBuf );
    
    EUNIT_ASSERT( item->Value() );
    EUNIT_ASSERT( item->Value()->Compare( KStringValue ) == 0 );
    delete item;
    }

void UT_CMceDictionary::UT_CMceKeyCValuePair_SetValueL()
    {
    HBufC8* valueBuf = KStringValue().AllocLC();
    CMceKeyCValuePair<HBufC8>* item = new( ELeave ) CMceKeyCValuePair<HBufC8>( KKey2, valueBuf );
    CleanupStack::Pop( valueBuf );
    CleanupStack::PushL( item );
    
    HBufC8* valueBuf2 = KStringValue2().AllocL();
    item->SetValue( valueBuf2 );
    EUNIT_ASSERT( item->iValue->Compare( KStringValue2 ) == 0 );
    CleanupStack::PopAndDestroy( item );
    }

void UT_CMceDictionary::UT_CMceDictionary_PairL()
    {
    CMceDictionaryItem* item = new( ELeave ) CMceKeyTValuePair<TInt>( KKey1, KUintValue  );
    CleanupStack::PushL( item );
    iDictionary->iPairs.AppendL( item );
    CleanupStack::Pop( item );
    CMceKeyTValuePair<TInt>* pair = static_cast< CMceKeyTValuePair<TInt>* >(
        iDictionary->Pair( KKey1 ) );
    EUNIT_ASSERT( pair == item );
    
    HBufC8* valueBuf = KStringValue().AllocLC();
    CMceDictionaryItem* item2 = new( ELeave ) CMceKeyCValuePair<HBufC8>( KKey2, valueBuf );
    CleanupStack::Pop( valueBuf );
    CleanupStack::PushL( item2 );
    iDictionary->iPairs.AppendL( item2 );
    CleanupStack::Pop( item2 );
    CMceKeyCValuePair<TInt>* pair2 = static_cast< CMceKeyCValuePair<TInt>* >(
        iDictionary->Pair( KKey2 ) );
    EUNIT_ASSERT( pair2 == item2 );
    }

void UT_CMceDictionary::UT_CMceDictionary_AddLL()
    {
    CMceDictionaryItem* item = new( ELeave ) CMceKeyTValuePair<TInt>( KKey1, KUintValue  );
    CleanupStack::PushL( item );
    iDictionary->AddL( item );
    CleanupStack::Pop( item );
        
    HBufC8* valueBuf = KStringValue().AllocLC();
    CMceDictionaryItem* item2 = new( ELeave ) CMceKeyCValuePair<HBufC8>( KKey2, valueBuf );
    CleanupStack::Pop( valueBuf );
    CleanupStack::PushL( item2 );
    iDictionary->AddL( item2 );
    CleanupStack::Pop( item2 );
    
    EUNIT_ASSERT( iDictionary->iPairs.Count() == 2 );
    EUNIT_ASSERT( iDictionary->iPairs[0] == item );
    
    EUNIT_ASSERT( iDictionary->iPairs[1] == item2 );
    }
    
void UT_CMceDictionary::UT_CMceDictionary_DeleteL()
    {
    CMceDictionaryItem* item = new( ELeave ) CMceKeyTValuePair<TInt>( KKey1, KUintValue  );
    CleanupStack::PushL( item );
    iDictionary->iPairs.AppendL( item );
    CleanupStack::Pop( item );
    
    HBufC8* valueBuf = KStringValue().AllocLC();
    CMceDictionaryItem* item2 = new( ELeave ) CMceKeyCValuePair<HBufC8>( KKey2, valueBuf );
    CleanupStack::Pop( valueBuf );
    CleanupStack::PushL( item2 );
    iDictionary->iPairs.AppendL( item2 );
    CleanupStack::Pop( item2 );
    
    EUNIT_ASSERT( iDictionary->iPairs.Count() == 2 );
    
    iDictionary->Delete( KKey1 );
    EUNIT_ASSERT( !iDictionary->Pair( KKey1 ) );
    
    EUNIT_ASSERT( iDictionary->iPairs.Count() == 1 );
    
    iDictionary->Delete( KKey1 );
    EUNIT_ASSERT( iDictionary->iPairs.Count() == 1 );
    
    iDictionary->Delete( KKey2 );
    EUNIT_ASSERT( !iDictionary->Pair( KKey2 ) );
    
    EUNIT_ASSERT( iDictionary->iPairs.Count() == 0 );
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceDictionary,
    "UT_CMceDictionary",
    "UNIT" )

EUNIT_TEST(
    "Key - method ",
    "CMceDictionaryItem",
    "Key",
    "FUNCTIONALITY",
    SetupL, UT_CMceDictionaryItem_KeyL, Teardown)
    
EUNIT_TEST(
    "Value - method ",
    "CMceKeyTValuePair",
    "Value",
    "FUNCTIONALITY",
    SetupL, UT_CMceKeyTValuePair_ValueL, Teardown)

EUNIT_TEST(
    "SetValue - method ",
    "CMceKeyTValuePair",
    "SetValue",
    "FUNCTIONALITY",
    SetupL, UT_CMceKeyTValuePair_SetValueL, Teardown)

EUNIT_TEST(
    "Value - method ",
    "CMceKeyCValuePair",
    "Value",
    "FUNCTIONALITY",
    SetupL, UT_CMceKeyCValuePair_ValueL, Teardown)

EUNIT_TEST(
    "SetValue - method ",
    "CMceKeyCValuePair",
    "SetValue",
    "FUNCTIONALITY",
    SetupL, UT_CMceKeyCValuePair_SetValueL, Teardown)

EUNIT_TEST(
    "PairL - method ",
    "CMceDictionary",
    "PairL",
    "FUNCTIONALITY",
    SetupL, UT_CMceDictionary_PairL, Teardown)

EUNIT_TEST(
    "AddL - method ",
    "CMceDictionary",
    "AddL",
    "FUNCTIONALITY",
    SetupL, UT_CMceDictionary_AddLL, Teardown)
    
EUNIT_TEST(
    "DeleteL - method ",
    "CMceDictionary",
    "DeleteL",
    "FUNCTIONALITY",
    SetupL, UT_CMceDictionary_DeleteL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
