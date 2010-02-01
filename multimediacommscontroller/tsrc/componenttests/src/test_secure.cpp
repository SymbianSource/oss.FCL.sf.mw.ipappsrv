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




// INCLUDES

#include "test_secure.h"
#include "mmcccodecinformation.h"
#include "mccteststubs.h"
#include "mmccinterface.h"
#include "testdefinitions.hrh"
#include "mmccsecureinterface.h"


_LIT8(KTestKey128bits,      "1122334455667788");
_LIT8(KTestMKI128Bits,      "AAAAAAAAAAAAAAAA");
_LIT8(KRFCTestSalt112bits,  "0EC675AD498AFE");

// ======== MEMBER FUNCTIONS =================================================

// ---------------------------------------------------------------------------
// CTestInterfaceInterface::CTestInterfaceInterface
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CTestSecure::CTestSecure( CConsoleBase& aConsole,
    const TMccNetSettings& aSettings, CMccInterface& aIF, CMccInterfaceStub& aIFStub )
    :
    iConsole( aConsole ), iNetsettings( aSettings ), iInterface( aIF ),
    iInterfaceStub( aIFStub )
    {
    }
    
// ---------------------------------------------------------------------------
// CTestInterface::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CTestSecure::ConstructL()
    {
    
    }
    
// ---------------------------------------------------------------------------
// CTestInterface::NewL
// Static constructor.
// ---------------------------------------------------------------------------
//   
CTestSecure* CTestSecure::NewL( CConsoleBase& aConsole,
    const TMccNetSettings& aSettings, CMccInterface& aIF, CMccInterfaceStub& aIFStub )
    {
    CTestSecure* self 
        = new ( ELeave ) CTestSecure( aConsole, aSettings, aIF, aIFStub );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CTestInterface::~CTestInterface
// Destructor.
// ---------------------------------------------------------------------------
//
CTestSecure::~CTestSecure()
    {
    //delete iSecureInterface;
    }

// ---------------------------------------------------------------------------
// CTestInterface::DoRunInterfaceTestsL
// ---------------------------------------------------------------------------
//
void CTestSecure::DoRunSecureTestsL()
    {
    iConsole.ClearScreen();
    iConsole.Printf( _L("\n*** INTERFACE TESTS ***\n") );

    iConsole.Printf( _L("DUMMY SECURE INTERFACE\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: DummyMethodL") );
    //__UHEAP_MARK;
    DummyMethodL();
    //__UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: DummyMethodL") );
    
    iConsole.Printf( _L("CREATE SECURE INTERFACE\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestCreateSecureInterfaceL") );
    __UHEAP_MARK;
    TestCreateSecureInterfaceL();
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestCreateSecureInterfaceL") );
   
    iConsole.Printf( _L("CREATE CRYPTO CONTEXT\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestCreateCryptoContextL") );
    __UHEAP_MARK;
    TestCreateCryptoContextL();
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestCreateCryptoContextL") );
   
    iConsole.Printf( _L("CREATE INVALID CRYPTO CONTEXT\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestCreateInvalidCryptoContextL") );
    __UHEAP_MARK;
    TestCreateInvalidCryptoContextL();
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestCreateInvalidCryptoContextL") );
    
    iConsole.Printf( _L("UPDATE CRYPTO CONTEXT\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestUpdateCryptoContextL") );
    __UHEAP_MARK;
    TestUpdateCryptoContextL();
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestUpdateCryptoContextL") );
    
    iConsole.Printf( _L("BIND CRYPTO CONTEXT\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestBindCryptoContextL") );
    __UHEAP_MARK;
    TestBindCryptoContextL();
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestBindCryptoContextL") );
    
    iConsole.Printf( _L("BIND CRYPTO CONTEXT & REFRESH IT \n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestBindCryptoContextAndRefreshContextL") );
    __UHEAP_MARK;
    TestBindCryptoContextAndRefreshContextL();
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestBindCryptoContextAndRefreshContextL") );
  
    iConsole.Printf( _L("BIND CRYPTO CONTEXT & PREPARE STREAM \n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestBindCryptoContextAndPrepareStreamL") );
    __UHEAP_MARK;
    TestBindCryptoContextAndPrepareStreamL();
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestBindCryptoContextAndPrepareStreamL") );
    
    iConsole.Printf( _L("BIND CRYPTO CONTEXT & PREPARE MULTIPLE STREAMS \n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestBindCryptoContextAndPrepareMultipleStreamsL") );
    __UHEAP_MARK;
    TestBindCryptoContextAndPrepareMultipleStreamsL();
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestBindCryptoContextAndPrepareMultipleStreamsL") );
 
    iConsole.Printf( _L("REMOVE CRYPTO CONTEXT\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestRemoveCryptoContextL") );
    __UHEAP_MARK;
    TestRemoveCryptoContextL();
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestRemoveCryptoContextL") );    
 
    }
    
// ---------------------------------------------------------------------------
// CTestSecure::TestCreateSecureInterface
// ---------------------------------------------------------------------------
//
void CTestSecure::TestCreateSecureInterfaceL()
    {
    RDebug::Print( _L("CTestSecure::TestCreateSecureInterfaceL") );
    }

// ---------------------------------------------------------------------------
// CTestSecure::DummyMethodL
// ---------------------------------------------------------------------------
//
void CTestSecure::DummyMethodL()
    {
    iSecureInterface = CMccSecureInterface::NewL( iInterface );
    
    this->CreateSecureSessioLinkAddDlUlSinkSourceCreateStreamsL();
    
    User::LeaveIfError( iInterface.DeleteStream( iSessionId, iLinkId, iDlStreamId ) );
    User::LeaveIfError( iInterface.DeleteStream( iSessionId, iLinkId, iUlStreamId ) );
    
    this->CloseSecureSessioL();
    delete iSecureInterface;
    }
        
        
// ---------------------------------------------------------------------------
// CTestSecure::TestCreateCryptoContextL
// ---------------------------------------------------------------------------
//
void CTestSecure::TestCreateCryptoContextL()
    {

    // Creates new master key
    HBufC8* masterKey = KTestKey128bits().AllocLC();
    TMccSrtpMasterKey mccMasterKey( *masterKey, KNullDesC8() );
    TMccSrtpMasterKeyPckg mccMasterKeyPckg = mccMasterKey;
    CleanupStack::PopAndDestroy( masterKey );
     
    
    // Creates new master salt  
    HBufC8* masterSalt = KRFCTestSalt112bits().AllocLC();    
    TMccSrtpMasterSalt salt( *masterSalt );
    TMccSrtpMasterSaltPckg saltPckg = salt;
    CleanupStack::PopAndDestroy( masterSalt );
    
    // Creates new crypto params
    TMccSrtpCryptoParams cryptoParams;
    TMccSrtpCryptoParamsPckg cryptoParamsPckg = cryptoParams;
    
    
    iSecureInterface = CMccSecureInterface::NewL( iInterface );
    


    TInt error = iSecureInterface->CreateContext( mccMasterKeyPckg,
                                                  saltPckg,
                                                  iContextId,
                                                  cryptoParamsPckg );
                                                  
    RDebug::Print( _L("TestCreateCryptoContextL context id: %u\n"), iContextId );
    
    error = iSecureInterface->CreateContext( mccMasterKeyPckg,
                                             saltPckg,
                                             iContextId2,
                                             cryptoParamsPckg ); 
                                                                                       
    RDebug::Print( _L("TestCreateCryptoContextL context id: %u\n"), iContextId2 );        
    delete iSecureInterface;
    }    
   
// ---------------------------------------------------------------------------
// CTestSecure::TestCreateInvalidCryptoContextL()
// ---------------------------------------------------------------------------
//
void CTestSecure::TestCreateInvalidCryptoContextL()
    {
    iSecureInterface = CMccSecureInterface::NewL( iInterface );
    
    _LIT8(KTestInvalidKey128bits,      "11223344");

    // Creates invalid master key
    HBufC8* masterKey2 = KTestInvalidKey128bits().AllocLC();
    TMccSrtpMasterKey master( *masterKey2, KNullDesC8() );
    TMccSrtpMasterKeyPckg masterKeyPckg( master );   
    CleanupStack::PopAndDestroy( masterKey2 );
    

    // Creates new master key
    HBufC8* masterKey = KTestKey128bits().AllocLC();
    TMccSrtpMasterKey mccMasterKey( *masterKey, KNullDesC8() );
    TMccSrtpMasterKeyPckg mccMasterKeyPckg = mccMasterKey;
    CleanupStack::PopAndDestroy( masterKey );
     
    
    // Creates new master salt  
    HBufC8* masterSalt = KRFCTestSalt112bits().AllocLC();    
    TMccSrtpMasterSalt salt( *masterSalt );
    TMccSrtpMasterSaltPckg saltPckg = salt;
    CleanupStack::PopAndDestroy( masterSalt );
    
    // Creates new crypto params
    TMccSrtpCryptoParams cryptoParams;
    TMccSrtpCryptoParamsPckg cryptoParamsPckg = cryptoParams;


    TInt error = iSecureInterface->CreateContext( masterKeyPckg,
                                                  saltPckg,
                                                  iContextId,
                                                  cryptoParamsPckg );
    
                     
    delete iSecureInterface;
    RDebug::Print( _L("TestCreateInvalidCryptoContextL context id: %d\n"), iContextId );                                           
    RDebug::Print( _L("CTestSecure::TestCreateInvalidCryptoContextL KErrArgument[-6] OK - %d"), error );                                
    }

// ---------------------------------------------------------------------------
// CTestSecure::TestUpdateCryptoContextL()
// ---------------------------------------------------------------------------
//
void CTestSecure::TestUpdateCryptoContextL()
    {
    iSecureInterface = CMccSecureInterface::NewL( iInterface );
    
    _LIT8(KUpdateTestKey128bits,"9922334455667711");

        // Creates new master key
    HBufC8* masterKey = KTestKey128bits().AllocLC();
    TMccSrtpMasterKey mccMasterKey( *masterKey, KNullDesC8() );
    TMccSrtpMasterKeyPckg mccMasterKeyPckg = mccMasterKey;
    CleanupStack::PopAndDestroy( masterKey );
     
    
    // Creates new master salt  
    HBufC8* masterSalt = KRFCTestSalt112bits().AllocLC();    
    TMccSrtpMasterSalt salt( *masterSalt );
    TMccSrtpMasterSaltPckg saltPckg = salt;
    CleanupStack::PopAndDestroy( masterSalt );
    
    // Creates new crypto params
    TMccSrtpCryptoParams cryptoParams;
    TMccSrtpCryptoParamsPckg cryptoParamsPckg = cryptoParams;
    
    User::LeaveIfError( iSecureInterface->CreateContext( mccMasterKeyPckg,
                                                         saltPckg,
                                                         iContextId,
                                                         cryptoParamsPckg ) );
                                                  
    HBufC8* masterKey2 = KUpdateTestKey128bits().AllocLC();
    TMccSrtpMasterKey master2( *masterKey2, KNullDesC8() );
    TMccSrtpMasterKeyPckg masterKeyPckg2( master2 );
    CleanupStack::PopAndDestroy( masterKey2 );
                               
    User::LeaveIfError( iSecureInterface->UpdateContext( masterKeyPckg2,
                                                         saltPckg,
                                                         iContextId,
                                                         cryptoParamsPckg ) );
                                                    
    delete iSecureInterface;                                                      
    }
    
// ---------------------------------------------------------------------------
// CTestSecure::TestBindCryptoContextL()
// ---------------------------------------------------------------------------
//    
void CTestSecure::TestBindCryptoContextL() 
    {
    iSecureInterface = CMccSecureInterface::NewL( iInterface );
    
    this->CreateSecureSessioLinkAddDlUlSinkSourceCreateStreamsL();
    
        // Creates new master key
    HBufC8* masterKey = KTestKey128bits().AllocLC();
    TMccSrtpMasterKey mccMasterKey( *masterKey, KNullDesC8() );
    TMccSrtpMasterKeyPckg mccMasterKeyPckg = mccMasterKey;
    CleanupStack::PopAndDestroy( masterKey );
     
    
    // Creates new master salt  
    HBufC8* masterSalt = KRFCTestSalt112bits().AllocLC();    
    TMccSrtpMasterSalt salt( *masterSalt );
    TMccSrtpMasterSaltPckg saltPckg = salt;
    CleanupStack::PopAndDestroy( masterSalt );
    
    // Creates new crypto params
    TMccSrtpCryptoParams cryptoParams;
    TMccSrtpCryptoParamsPckg cryptoParamsPckg = cryptoParams;

    User::LeaveIfError( iSecureInterface->CreateContext( mccMasterKeyPckg,
                                                         saltPckg,
                                                         iContextId,
                                                         cryptoParamsPckg ) );
                                                         
    RDebug::Print( _L("TestBindCryptoContextL context id: %d\n"), iContextId ); 
    
    User::LeaveIfError( iSecureInterface->CreateContext( mccMasterKeyPckg,
                                                         saltPckg,
                                                         iContextId2,
                                                         cryptoParamsPckg ) );
                                                         
    RDebug::Print( _L("TestBindCryptoContextL context id: %d\n"), iContextId2 ); 
    
    User::LeaveIfError( iSecureInterface->Bind( iSessionId,
                                                iLinkId,
                                                iDlStreamId,
                                                iRtpSourceId,
                                                iContextId ) );
                                            
    User::LeaveIfError( iSecureInterface->Bind( iSessionId,
                                                iLinkId,
                                                iUlStreamId,
                                                iRtpSinkId,
                                                iContextId2 ) );
    // Context is already binded  
    TInt error( KErrNone ); 
                                              
    error = iSecureInterface->Bind( iSessionId,
                                    iLinkId,
                                    iUlStreamId,
                                    iRtpSinkId,
                                    iContextId );
    
    RDebug::Print( _L("TestBindCryptoContextL context error OK - error:  %d\n"), error );
                                   
    error = iSecureInterface->Bind( iSessionId,
                                    iLinkId,
                                    iDlStreamId,
                                    iRtpSinkId,
                                    iContextId2 );
    
    RDebug::Print( _L("TestBindCryptoContextL context error OK - error:  %d\n"), error );
                                              
    User::LeaveIfError( iInterface.DeleteStream( iSessionId, iLinkId, iDlStreamId ) );
    User::LeaveIfError( iInterface.DeleteStream( iSessionId, iLinkId, iUlStreamId ) );
                                                
    this->CloseSecureSessioL();
    
    delete iSecureInterface;
    }
    
// ---------------------------------------------------------------------------
// CTestSecure::TestBindCryptoContextAndRefreshContextL()
// ---------------------------------------------------------------------------
//    
void CTestSecure::TestBindCryptoContextAndRefreshContextL() 
    {
    iSecureInterface = CMccSecureInterface::NewL( iInterface );

    this->CreateSecureSessioLinkAddDlUlSinkSourceCreateStreamsL();
    
        // Creates new master key
    HBufC8* masterKey = KTestKey128bits().AllocLC();
    TMccSrtpMasterKey mccMasterKey( *masterKey, KNullDesC8() );
    TMccSrtpMasterKeyPckg mccMasterKeyPckg = mccMasterKey;
    CleanupStack::PopAndDestroy( masterKey );
     
    
    // Creates new master salt  
    HBufC8* masterSalt = KRFCTestSalt112bits().AllocLC();    
    TMccSrtpMasterSalt salt( *masterSalt );
    TMccSrtpMasterSaltPckg saltPckg = salt;
    CleanupStack::PopAndDestroy( masterSalt );
    
    // Creates new crypto params
    TMccSrtpCryptoParams cryptoParams;
    TMccSrtpCryptoParamsPckg cryptoParamsPckg = cryptoParams;
    
    /******* Make new context ***********/
    _LIT8(KUpdateTestKey128bits,"9922334455667711");
                                                  
    HBufC8* masterKey2 = KUpdateTestKey128bits().AllocLC();
    TMccSrtpMasterKey master2( *masterKey2, KNullDesC8() );
    TMccSrtpMasterKeyPckg masterKeyPckg2( master2 );
    CleanupStack::PopAndDestroy( masterKey2 );
   /******** End  ********/                                                      

    User::LeaveIfError( iSecureInterface->CreateContext( mccMasterKeyPckg,
                                                         saltPckg,
                                                         iContextId,
                                                         cryptoParamsPckg ) );
                                                         
    User::LeaveIfError( iSecureInterface->CreateContext( mccMasterKeyPckg,
                                                         saltPckg,
                                                         iContextId2,
                                                         cryptoParamsPckg ) );
     
      
    User::LeaveIfError( iSecureInterface->Bind( iSessionId,
                                                iLinkId,
                                                iDlStreamId,
                                                iRtpSourceId,
                                                iContextId) );
                                                
    User::LeaveIfError( iSecureInterface->Bind( iSessionId,
                                                iLinkId,
                                                iUlStreamId,
                                                iRtpSinkId,
                                                iContextId2 ) );

    
    User::LeaveIfError( iSecureInterface->UpdateContext( masterKeyPckg2,
                                                         saltPckg,
                                                         iContextId,
                                                         cryptoParamsPckg ) );
                                                         
    User::LeaveIfError( iSecureInterface->UpdateContext( masterKeyPckg2,
                                                         saltPckg,
                                                         iContextId2,
                                                         cryptoParamsPckg ) );
                                                          

    User::LeaveIfError( iSecureInterface->RemoveContext( iContextId ) );
    User::LeaveIfError( iSecureInterface->RemoveContext( iContextId2 ) );                                                    

    
    this->CloseSecureSessioL();
    
    delete iSecureInterface;
    } 
        
// ---------------------------------------------------------------------------
// CTestSecure::TestBindCryptoContextAndPrepareStreamL()
// ---------------------------------------------------------------------------
//    
void CTestSecure::TestBindCryptoContextAndPrepareStreamL() 
    {
    iSecureInterface = CMccSecureInterface::NewL( iInterface );
    
    this->CreateSecureSessioLinkAddDlUlSinkSourceCreateStreamsL();
    
    /******* Make new context ***********/
    _LIT8(KUpdateTestKey128bits,"9922334455667711");
                                                  
    HBufC8* masterKey2 = KUpdateTestKey128bits().AllocLC();
    TMccSrtpMasterKey master2( *masterKey2, KNullDesC8() );
    TMccSrtpMasterKeyPckg masterKeyPckg2( master2 );
    CleanupStack::PopAndDestroy( masterKey2 );
   /******** End  ********/   
   
       // Creates new master key
    HBufC8* masterKey = KTestKey128bits().AllocLC();
    TMccSrtpMasterKey mccMasterKey( *masterKey, KNullDesC8() );
    TMccSrtpMasterKeyPckg mccMasterKeyPckg = mccMasterKey;
    CleanupStack::PopAndDestroy( masterKey );
     
    
    // Creates new master salt  
    HBufC8* masterSalt = KRFCTestSalt112bits().AllocLC();    
    TMccSrtpMasterSalt salt( *masterSalt );
    TMccSrtpMasterSaltPckg saltPckg = salt;
    CleanupStack::PopAndDestroy( masterSalt );
    
    // Creates new crypto params
    TMccSrtpCryptoParams cryptoParams;
    TMccSrtpCryptoParamsPckg cryptoParamsPckg = cryptoParams;
    
    User::LeaveIfError( iSecureInterface->CreateContext( mccMasterKeyPckg,
                                                         saltPckg,
                                                         iContextId,
                                                         cryptoParamsPckg ) );
                                                         
    User::LeaveIfError( iSecureInterface->CreateContext( mccMasterKeyPckg,
                                                         saltPckg,
                                                         iContextId2,
                                                         cryptoParamsPckg ) );

       
    User::LeaveIfError( iSecureInterface->Bind( iSessionId,
                                                iLinkId,
                                                iDlStreamId,
                                                iRtpSourceId,
                                                iContextId) );
                                                
    User::LeaveIfError( iSecureInterface->Bind( iSessionId,
                                                iLinkId,
                                                iUlStreamId,
                                                iRtpSinkId,
                                                iContextId2 ) );

    
    User::LeaveIfError( iInterface.PrepareStream( iSessionId, iLinkId, iDlStreamId ) );
    iInterfaceStub.WaitForEvent( iSessionId, iLinkId, iDlStreamId, KMccStreamPrepared );
    
    User::LeaveIfError( iInterface.PrepareStream( iSessionId, iLinkId, iUlStreamId ) );
    iInterfaceStub.WaitForEvent( iSessionId, iLinkId, iUlStreamId, KMccStreamPrepared );
    
    User::LeaveIfError( iSecureInterface->UpdateContext( masterKeyPckg2,
                                                         saltPckg,
                                                         iContextId,
                                                         cryptoParamsPckg ) );
                                                         
    User::LeaveIfError( iSecureInterface->UpdateContext( masterKeyPckg2,
                                                         saltPckg,
                                                         iContextId2,
                                                         cryptoParamsPckg ) );
                                                           

    iInterface.RemoveDataSource( iSessionId, iLinkId, iDlStreamId, iRtpSourceId );
    iInterface.RemoveDataSink( iSessionId, iLinkId, iUlStreamId, iRtpSinkId );
    
    User::LeaveIfError( iSecureInterface->RemoveContext( iContextId ) );
    User::LeaveIfError( iSecureInterface->RemoveContext( iContextId2 ) );       
    
    this->CloseSecureSessioL();
    
    delete iSecureInterface;
    }   

// ---------------------------------------------------------------------------
// CTestSecure::TestBindCryptoContextAndPrepareMultipleStreamL()
// ---------------------------------------------------------------------------
//    
void CTestSecure::TestBindCryptoContextAndPrepareMultipleStreamsL()
    {
    iSecureInterface = CMccSecureInterface::NewL( iInterface );
    
    // Get codecs
    RPointerArray<CMccCodecInformation> codecArray;
    CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray );
    User::LeaveIfError( iInterface.GetCapabilities( codecArray ) );
    
    
    this->CreateSecureSessioLinkAddDlUlSinkSourceCreateStreamsL();
    
    /******* Make new context ***********/
    _LIT8(KUpdateTestKey128bits,"9922334455667711");
                                                  
    HBufC8* masterKey2 = KUpdateTestKey128bits().AllocLC();
    TMccSrtpMasterKey master2( *masterKey2, KNullDesC8() );
    TMccSrtpMasterKeyPckg masterKeyPckg2( master2 );
    CleanupStack::PopAndDestroy( masterKey2 );
   /******** End  ********/   
   
       // Creates new master key
    HBufC8* masterKey = KTestKey128bits().AllocLC();
    TMccSrtpMasterKey mccMasterKey( *masterKey, KNullDesC8() );
    TMccSrtpMasterKeyPckg mccMasterKeyPckg = mccMasterKey;
    CleanupStack::PopAndDestroy( masterKey );
     
    
    // Creates new master salt  
    HBufC8* masterSalt = KRFCTestSalt112bits().AllocLC();    
    TMccSrtpMasterSalt salt( *masterSalt );
    TMccSrtpMasterSaltPckg saltPckg = salt;
    CleanupStack::PopAndDestroy( masterSalt );
    
    // Creates new crypto params
    TMccSrtpCryptoParams cryptoParams;
    TMccSrtpCryptoParamsPckg cryptoParamsPckg = cryptoParams;
    
    // Reuse datasource/sink
    TUint32 ulStreamId2;
    TUint32 micSourceId2;
    User::LeaveIfError(
        iInterface.Reuse( iSessionId, iLinkId, iUlStreamId, iRtpSinkId ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KUidMmfAudioInput, KNullDesC8, micSourceId2 ) );
    User::LeaveIfError( iInterface.CreateStream( iSessionId, iLinkId, ulStreamId2, 
        KMccAudioUplinkStream, *codecArray[0] ) );
        
        
    User::LeaveIfError( iSecureInterface->CreateContext( mccMasterKeyPckg,
                                                         saltPckg,
                                                         iContextId,
                                                         cryptoParamsPckg ) );
                                                         
    User::LeaveIfError( iSecureInterface->CreateContext( mccMasterKeyPckg,
                                                         saltPckg,
                                                         iContextId2,
                                                         cryptoParamsPckg ) );

       
    User::LeaveIfError( iSecureInterface->Bind( iSessionId,
                                                iLinkId,
                                                iDlStreamId,
                                                iRtpSourceId,
                                                iContextId) );
                                                
    User::LeaveIfError( iSecureInterface->Bind( iSessionId,
                                                iLinkId,
                                                iUlStreamId,
                                                iRtpSinkId,
                                                iContextId2 ) );

    
    User::LeaveIfError( iInterface.PrepareStream( iSessionId, iLinkId, iDlStreamId ) );
    iInterfaceStub.WaitForEvent( iSessionId, iLinkId, iDlStreamId, KMccStreamPrepared );
    
    User::LeaveIfError( iInterface.PrepareStream( iSessionId, iLinkId, ulStreamId2 ) );
    iInterfaceStub.WaitForEvent( iSessionId, iLinkId, ulStreamId2, KMccStreamPrepared );
    
    User::LeaveIfError( iSecureInterface->UpdateContext( masterKeyPckg2,
                                                         saltPckg,
                                                         iContextId,
                                                         cryptoParamsPckg ) );
                                                         
    User::LeaveIfError( iSecureInterface->UpdateContext( masterKeyPckg2,
                                                         saltPckg,
                                                         iContextId2,
                                                         cryptoParamsPckg ) );
                                                           

    User::LeaveIfError( iInterface.DeleteStream( iSessionId, iLinkId, ulStreamId2 ) );
    
    CleanupStack::PopAndDestroy( &codecArray );     
    this->CloseSecureSessioL();
    
    delete iSecureInterface;
    }
     
// ---------------------------------------------------------------------------
// CTestSecure::TestRemoveCryptoContextL()
// ---------------------------------------------------------------------------
//
void CTestSecure::TestRemoveCryptoContextL()
    {
    iSecureInterface = CMccSecureInterface::NewL( iInterface );

        // Creates new master key
    HBufC8* masterKey = KTestKey128bits().AllocLC();
    TMccSrtpMasterKey mccMasterKey( *masterKey, KNullDesC8() );
    TMccSrtpMasterKeyPckg mccMasterKeyPckg = mccMasterKey;
    CleanupStack::PopAndDestroy( masterKey );
     
    
    // Creates new master salt  
    HBufC8* masterSalt = KRFCTestSalt112bits().AllocLC();    
    TMccSrtpMasterSalt salt( *masterSalt );
    TMccSrtpMasterSaltPckg saltPckg = salt;
    CleanupStack::PopAndDestroy( masterSalt );
    
    // Creates new crypto params
    TMccSrtpCryptoParams cryptoParams;
    TMccSrtpCryptoParamsPckg cryptoParamsPckg = cryptoParams;
    
    User::LeaveIfError( iSecureInterface->CreateContext( mccMasterKeyPckg,
                                                         saltPckg,
                                                         iContextId,
                                                         cryptoParamsPckg ) );
                                     
    RDebug::Print( _L("TestRemoveCryptoContextL context id: %d\n"), iContextId );                            
    

   
    User::LeaveIfError( iSecureInterface->CreateContext( mccMasterKeyPckg,
                                                         saltPckg,
                                                         iContextId2,
                                                         cryptoParamsPckg ) );
                                                                                         
    RDebug::Print( _L("TestRemoveCryptoContextL context id: %d\n"), iContextId2 );                            
    
    //User::LeaveIfError( iSecureInterface->RemoveContext( iContextId ) );
    //User::LeaveIfError( iSecureInterface->RemoveContext( iContextId2 ) );

    delete iSecureInterface;
    }


    
// ---------------------------------------------------------------------------
// CTestSecure::CreateSecureSessioLinkAddDlUlSinkSourceCreateStreamsL()
// ---------------------------------------------------------------------------
//
void CTestSecure::CreateSecureSessioLinkAddDlUlSinkSourceCreateStreamsL()
    {
    // Creates secure session
    RPointerArray<CMccCodecInformation> codecArray;
    CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray );
    User::LeaveIfError( iInterface.GetCapabilities( codecArray ) );
    
    User::LeaveIfError( iInterface.CreateSession( iSessionId ) );
    User::LeaveIfError(
        iInterface.CreateLink( iSessionId, KMccLinkSecure, iLinkId,
            iNetsettings ) );
            
    iInterfaceStub.WaitForEvent( iSessionId, iLinkId, 0, KMccLinkCreated );
    
    // DL
    User::LeaveIfError(
        iInterface.AddDataSink( KUidMmfAudioOutput, KNullDesC8, iAudioSinkId ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KMccRtpSourceUid, KNullDesC8, iRtpSourceId ) );
     
      
    User::LeaveIfError( iInterface.CreateStream( iSessionId, iLinkId, iDlStreamId, 
        KMccAudioDownlinkStream, *codecArray[0] ) ); 
            
    // UL  
    User::LeaveIfError( 
        iInterface.AddDataSink( KMccRtpSinkUid, KNullDesC8, iRtpSinkId ) );
    User::LeaveIfError( 
        iInterface.AddDataSource( KUidMmfAudioInput, KNullDesC8, iMicSourceId ) );
        
    
    User::LeaveIfError( iInterface.CreateStream( iSessionId, iLinkId, iUlStreamId, 
        KMccAudioUplinkStream, *codecArray[0] ) ); 
        
    CleanupStack::PopAndDestroy( &codecArray );     
    }
  
// ---------------------------------------------------------------------------
// CTestSecure::CloseSecureSessioL()
// ---------------------------------------------------------------------------
//
void CTestSecure::CloseSecureSessioL()
    {        
    User::LeaveIfError( iInterface.CloseLink( iSessionId, iLinkId ) );
    User::LeaveIfError( iInterface.CloseSession( iSessionId ) );                                                         
    }    
