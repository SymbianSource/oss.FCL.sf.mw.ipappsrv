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
#include "UT_CMccCryptoContextContainer.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mmcccryptocontextcontainer.h"
#include "mmccsrtpmasterkey.h"
#include "mmccsrtpmastersalt.h"
#include "mmccsrtpcryptoparams.h"
#include "mmcccryptocontext.h"

_LIT8(KTestInvalidKey128bits,      "11223344");
_LIT8(KTestKey128bits,      "1122334455667788");
_LIT8(KTestKey128bits2,      "1122334455669999");
_LIT8(KTestMKI128Bits,      "AAAAAAAAAAAAAAAA");
_LIT8(KRFCTestSalt112bits,  "0EC675AD498AFE");

// CONSTRUCTION
UT_CMccCryptoContextContainer* UT_CMccCryptoContextContainer::NewL()
    {
    UT_CMccCryptoContextContainer* self = UT_CMccCryptoContextContainer::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccCryptoContextContainer* UT_CMccCryptoContextContainer::NewLC()
    {
    UT_CMccCryptoContextContainer* self = new( ELeave ) UT_CMccCryptoContextContainer();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccCryptoContextContainer::~UT_CMccCryptoContextContainer()
    {
    }

// Default constructor
UT_CMccCryptoContextContainer::UT_CMccCryptoContextContainer()
    {
    }

// Second phase construct
void UT_CMccCryptoContextContainer::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccCryptoContextContainer::SetupL(  )
    {
    iContextContainer = CMccCryptoContextContainer::NewL();
    }

void UT_CMccCryptoContextContainer::Teardown(  )
    {
    delete iContextContainer;
    }

void UT_CMccCryptoContextContainer::UT_CMccCryptoContextContainer_NewLL(  )
    {
    CMccCryptoContextContainer* iContainer;
    iContainer = CMccCryptoContextContainer::NewL();
    delete iContainer;
    }

void UT_CMccCryptoContextContainer::UT_CMccCryptoContextContainer_CreateContextL(  )
    {
    // Creates new master key
    HBufC8* masterKey = KTestKey128bits().AllocLC();
    TMccSrtpMasterKey mccMasterKey( *masterKey, KNullDesC8() );
    CleanupStack::PopAndDestroy( masterKey ); 
    
    // Creates new master salt  
    HBufC8* masterSalt = KRFCTestSalt112bits().AllocLC();    
    TMccSrtpMasterSalt salt( *masterSalt );
    CleanupStack::PopAndDestroy( masterSalt );
    
    // Creates new crypto params
    TMccSrtpCryptoParams cryptoParams;
    
    TUint32 contextId;
    TUint32 contextId2;
       
    TInt err = iContextContainer->CreateContext( mccMasterKey, salt, contextId, cryptoParams );
    if ( err == KErrNoMemory )
    	{
    	User::Leave( KErrNoMemory );
    	}
    EUNIT_ASSERT( err == KErrNone );

    err = iContextContainer->CreateContext( mccMasterKey, salt, contextId2, cryptoParams );
    if ( err == KErrNoMemory )
    	{
    	User::Leave( KErrNoMemory );
    	}
    EUNIT_ASSERT( err == KErrNone );
    }

void UT_CMccCryptoContextContainer::UT_CMccCryptoContextContainer_RemoveContextL(  )
    {
        // Creates new master key
    HBufC8* masterKey = KTestKey128bits().AllocLC();
    TMccSrtpMasterKey mccMasterKey( *masterKey, KNullDesC8() );
    CleanupStack::PopAndDestroy( masterKey ); 
    
    // Creates new master salt  
    HBufC8* masterSalt = KRFCTestSalt112bits().AllocLC();    
    TMccSrtpMasterSalt salt( *masterSalt );
    CleanupStack::PopAndDestroy( masterSalt );
    
    // Creates new crypto params
    TMccSrtpCryptoParams cryptoParams;
    
    TUint32 contextId;
    TUint32 contextId2;
    TUint32 invalidContextId( 90 );
	
	TInt err = iContextContainer->CreateContext( mccMasterKey, salt, contextId, cryptoParams );
    if ( err == KErrNoMemory )
    	{
    	User::Leave( KErrNoMemory );
    	}
    EUNIT_ASSERT( err == KErrNone );

    err = iContextContainer->CreateContext( mccMasterKey, salt, contextId2, cryptoParams );
    if ( err == KErrNoMemory )
    	{
    	User::Leave( KErrNoMemory );
    	}
    EUNIT_ASSERT( err == KErrNone );
	
    EUNIT_ASSERT( iContextContainer->RemoveContext( invalidContextId ) == KErrNotFound );
    
    EUNIT_ASSERT( iContextContainer->RemoveContext( contextId ) == KErrNone );
    EUNIT_ASSERT( iContextContainer->RemoveContext( contextId2 ) == KErrNone );
    EUNIT_ASSERT( iContextContainer->RemoveContext( contextId ) == KErrNotFound );
    }

void UT_CMccCryptoContextContainer::UT_CMccCryptoContextContainer_UpdateContextL(  )
    {
    // Creates second master key
    HBufC8* masterKey2 = KTestKey128bits2().AllocLC();
    TMccSrtpMasterKey master( *masterKey2, KNullDesC8() );   
    CleanupStack::PopAndDestroy( masterKey2 );
    
    // Creates new master key
    HBufC8* masterKey = KTestKey128bits().AllocLC();
    TMccSrtpMasterKey mccMasterKey( *masterKey, KNullDesC8() );
    CleanupStack::PopAndDestroy( masterKey ); 
    
    // Creates new master salt  
    HBufC8* masterSalt = KRFCTestSalt112bits().AllocLC();    
    TMccSrtpMasterSalt salt( *masterSalt );
    CleanupStack::PopAndDestroy( masterSalt );
    
    // Creates new crypto params
    TMccSrtpCryptoParams cryptoParams;
    
    TUint32 contextId;
    TUint32 contextId2( 10 );
    TUint32 invalidContextId( 90 );
    
    TMccCryptoContext cryptoContext2( contextId2,
                                      master,
                                      salt,
                                      cryptoParams ); 

	TInt err = iContextContainer->CreateContext( mccMasterKey, salt, contextId, cryptoParams );
    if ( err == KErrNoMemory )
    	{
    	User::Leave( KErrNoMemory );
    	}
    EUNIT_ASSERT( err == KErrNone );
       
    EUNIT_ASSERT( iContextContainer->UpdateContext( invalidContextId, cryptoContext2 ) == KErrNotFound ); 
    EUNIT_ASSERT( iContextContainer->UpdateContext( contextId, cryptoContext2 ) == KErrNone ); 
    }

void UT_CMccCryptoContextContainer::UT_CMccCryptoContextContainer_GetContextL(  )
    {
    // Creates new master key
    HBufC8* masterKey = KTestKey128bits().AllocLC();
    TMccSrtpMasterKey mccMasterKey( *masterKey, KNullDesC8() );
    CleanupStack::PopAndDestroy( masterKey ); 
    
    // Creates new master salt  
    HBufC8* masterSalt = KRFCTestSalt112bits().AllocLC();    
    TMccSrtpMasterSalt salt( *masterSalt );
    CleanupStack::PopAndDestroy( masterSalt );
    
    // Creates new crypto params
    TMccSrtpCryptoParams cryptoParams;
    
    TUint32 contextId;
    TUint32 invalidContextId( 90 );

	TInt err = iContextContainer->CreateContext( mccMasterKey, salt, contextId, cryptoParams );
    if ( err == KErrNoMemory )
    	{
    	User::Leave( KErrNoMemory );
    	}
    EUNIT_ASSERT( err == KErrNone );
               
    TMccCryptoContext* cryptoContex;
    
    EUNIT_ASSERT( iContextContainer->GetContext( invalidContextId, cryptoContex ) == KErrNotFound );
    EUNIT_ASSERT( iContextContainer->GetContext( contextId, cryptoContex ) == KErrNone );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccCryptoContextContainer,
    "CMccCryptoContextContainer test",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMccCryptoContextContainer",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCryptoContextContainer_NewLL, Teardown)

EUNIT_TEST(
    "CreateContext - test ",
    "CMccCryptoContextContainer",
    "CreateContext",
    "FUNCTIONALITY",
    SetupL, UT_CMccCryptoContextContainer_CreateContextL, Teardown)

EUNIT_TEST(
    "RemoveContext - test ",
    "CMccCryptoContextContainer",
    "RemoveContext",
    "FUNCTIONALITY",
    SetupL, UT_CMccCryptoContextContainer_RemoveContextL, Teardown)

EUNIT_TEST(
    "UpdateContext - test ",
    "CMccCryptoContextContainer",
    "UpdateContext",
    "FUNCTIONALITY",
    SetupL, UT_CMccCryptoContextContainer_UpdateContextL, Teardown)

EUNIT_TEST(
    "GetContext - test ",
    "CMccCryptoContextContainer",
    "GetContext",
    "FUNCTIONALITY",
    SetupL, UT_CMccCryptoContextContainer_GetContextL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
