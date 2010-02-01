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
#include "UT_TMccCryptoContext.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mmcccryptocontext.h"


_LIT8(KTestInvalidKey128bits,      "11223344");
_LIT8(KTestKey128bits,      "1122334455667788");
_LIT8(KTestKey128bits2,      "1122334455669999");
_LIT8(KTestMKI128Bits,      "AAAAAAAAAAAAAAAA");
_LIT8(KRFCTestSalt112bits,  "0EC675AD498AFE");


// CONSTRUCTION
UT_TMccCryptoContext* UT_TMccCryptoContext::NewL()
    {
    UT_TMccCryptoContext* self = UT_TMccCryptoContext::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_TMccCryptoContext* UT_TMccCryptoContext::NewLC()
    {
    UT_TMccCryptoContext* self = new( ELeave ) UT_TMccCryptoContext();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_TMccCryptoContext::~UT_TMccCryptoContext()
    {
    }

// Default constructor
UT_TMccCryptoContext::UT_TMccCryptoContext()
    {
    }

// Second phase construct
void UT_TMccCryptoContext::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_TMccCryptoContext::SetupL(  )
    {
     
    }

void UT_TMccCryptoContext::Teardown(  )
    {

    }

void UT_TMccCryptoContext::UT_TMccCryptoContext_ValidateContextL(  )
    {
    TMccCryptoContext cryptoContext;
    
    EUNIT_ASSERT( cryptoContext.ValidateContext() == KErrArgument );
    
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
    
    TUint32 contextId( 0 );
    
    TMccCryptoContext cryptoContext2( contextId,
                                     mccMasterKey,
                                     salt,
                                     cryptoParams );
                                     
    EUNIT_ASSERT( cryptoContext2.ValidateContext() == KErrNone );                                 
    }

void UT_TMccCryptoContext::UT_TMccCryptoContext_UpdateContextL(  )
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
    
    TUint32 contextId( 0 );
    TUint32 contextId2( 0 );
    
    TMccCryptoContext cryptoContext( contextId,
                                     mccMasterKey,
                                     salt,
                                     cryptoParams );
    
    TMccCryptoContext cryptoContext2( contextId2,
                                      master,
                                      salt,
                                      cryptoParams );  
                                      
                                                                             
    EUNIT_ASSERT(  cryptoContext.UpdateContext( cryptoContext2 ) == KErrNone ); 
    }

void UT_TMccCryptoContext::UT_TMccCryptoContext_GetDestinationInfoL(  )
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
    
    TUint32 contextId( 0 );
    
    TMccCryptoContext cryptoContext( contextId,
                                     mccMasterKey,
                                     salt,
                                     cryptoParams );

    TUint32 sessionId;
    TUint32 linkId;
    TUint32 streamId;
    TUint32 endPointId;
                                
    EUNIT_ASSERT( cryptoContext.GetDestinationInfo( sessionId, linkId,
                                                    streamId, endPointId ) == KErrNotFound );
    }

void UT_TMccCryptoContext::UT_TMccCryptoContext_SetDestinationInfoL(  )
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
    
    TUint32 contextId( 0 );
    
    TMccCryptoContext cryptoContext( contextId,
                                     mccMasterKey,
                                     salt,
                                     cryptoParams );
                                       
    TUint32 sessionId = 1;
    TUint32 linkId = 1;
    TUint32 streamId = 1;
    TUint32 endPointId = 98500;
    
    EUNIT_ASSERT( cryptoContext.SetDestinationInfo( sessionId, linkId,
                                                    streamId, endPointId ) == KErrNone );
                                      
    EUNIT_ASSERT( cryptoContext.GetDestinationInfo( sessionId, linkId,
                                                    streamId, endPointId ) == KErrNone );
                                                                                 
    }

void UT_TMccCryptoContext::UT_TMccCryptoContext_SetMasterKeyL(  )
    {
    // Creates new master key
    HBufC8* masterKey = KTestKey128bits().AllocLC();
    TMccSrtpMasterKey mccMasterKey( *masterKey, KNullDesC8() );
    CleanupStack::PopAndDestroy( masterKey ); 
    
    
    TMccCryptoContext cryptoContext;
    
    cryptoContext.SetMasterKey( mccMasterKey );
    EUNIT_ASSERT( ETrue );
    }

void UT_TMccCryptoContext::UT_TMccCryptoContext_SetMasterSaltL(  )
    {    
    // Creates new master salt  
    HBufC8* masterSalt = KRFCTestSalt112bits().AllocLC();    
    TMccSrtpMasterSalt salt( *masterSalt );
    CleanupStack::PopAndDestroy( masterSalt );
   
    
    TMccCryptoContext cryptoContext;
    cryptoContext.SetMasterSalt( salt );
    EUNIT_ASSERT( ETrue );
    }

void UT_TMccCryptoContext::UT_TMccCryptoContext_SetCryptoParamsL(  )
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
    
    TMccCryptoContext cryptoContext;
    cryptoContext.SetCryptoParams( cryptoParams );
    EUNIT_ASSERT( ETrue );
    }

void UT_TMccCryptoContext::UT_TMccCryptoContext_MasterKeyL(  )
    {
    TMccCryptoContext cryptoContext;
    TMccSrtpMasterKey mccMasterKey( cryptoContext.MasterKey() );
    EUNIT_ASSERT( ETrue );
    }

void UT_TMccCryptoContext::UT_TMccCryptoContext_MasterSaltL(  )
    {
    TMccCryptoContext cryptoContext;
    TMccSrtpMasterSalt mccMasterSalt( cryptoContext.MasterSalt() );
    EUNIT_ASSERT( ETrue );
    }

void UT_TMccCryptoContext::UT_TMccCryptoContext_CryptoParamsL(  )
    {
    TMccCryptoContext cryptoContext;
    TMccSrtpCryptoParams mccMasterSalt( cryptoContext.CryptoParams() );
    EUNIT_ASSERT( ETrue );
    }

void UT_TMccCryptoContext::UT_TMccCryptoContext_ContextIdL(  )
    {
    TUint32 contextId( 0 );
    
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
    
    TMccCryptoContext cryptoContext( contextId,
                                     mccMasterKey,
                                     salt,
                                     cryptoParams );
                                     
    EUNIT_ASSERT( cryptoContext.ContextId() == contextId );                                 
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_TMccCryptoContext,
    "TMccCryptoContext test",
    "UNIT" )


EUNIT_TEST(
    "ValidateContext - test ",
    "TMccCryptoContext",
    "ValidateContext",
    "FUNCTIONALITY",
    SetupL, UT_TMccCryptoContext_ValidateContextL, Teardown)

EUNIT_TEST(
    "UpdateContext - test ",
    "TMccCryptoContext",
    "UpdateContext",
    "FUNCTIONALITY",
    SetupL, UT_TMccCryptoContext_UpdateContextL, Teardown)

EUNIT_TEST(
    "GetDestinationInfo - test ",
    "TMccCryptoContext",
    "GetDestinationInfo",
    "FUNCTIONALITY",
    SetupL, UT_TMccCryptoContext_GetDestinationInfoL, Teardown)

EUNIT_TEST(
    "SetDestinationInfo - test ",
    "TMccCryptoContext",
    "SetDestinationInfo",
    "FUNCTIONALITY",
    SetupL, UT_TMccCryptoContext_SetDestinationInfoL, Teardown)

EUNIT_TEST(
    "SetMasterKey - test ",
    "TMccCryptoContext",
    "SetMasterKey",
    "FUNCTIONALITY",
    SetupL, UT_TMccCryptoContext_SetMasterKeyL, Teardown)

EUNIT_TEST(
    "SetMasterSalt - test ",
    "TMccCryptoContext",
    "SetMasterSalt",
    "FUNCTIONALITY",
    SetupL, UT_TMccCryptoContext_SetMasterSaltL, Teardown)

EUNIT_TEST(
    "SetCryptoParams - test ",
    "TMccCryptoContext",
    "SetCryptoParams",
    "FUNCTIONALITY",
    SetupL, UT_TMccCryptoContext_SetCryptoParamsL, Teardown)

EUNIT_TEST(
    "MasterKey - test ",
    "TMccCryptoContext",
    "MasterKey",
    "FUNCTIONALITY",
    SetupL, UT_TMccCryptoContext_MasterKeyL, Teardown)

EUNIT_TEST(
    "MasterSalt - test ",
    "TMccCryptoContext",
    "MasterSalt",
    "FUNCTIONALITY",
    SetupL, UT_TMccCryptoContext_MasterSaltL, Teardown)

EUNIT_TEST(
    "CryptoParams - test ",
    "TMccCryptoContext",
    "CryptoParams",
    "FUNCTIONALITY",
    SetupL, UT_TMccCryptoContext_CryptoParamsL, Teardown)

EUNIT_TEST(
    "ContextId - test ",
    "TMccCryptoContext",
    "ContextId",
    "FUNCTIONALITY",
    SetupL, UT_TMccCryptoContext_ContextIdL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
