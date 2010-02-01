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
#include "UT_CMccSecureInterface.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

#include <mmccsrtpmasterkey.h>
#include <mmccsrtpmastersalt.h>
#include <mmccsrtpcryptoparams.h>

//  INTERNAL INCLUDES
#include "mmccsecureinterface.h"
#include "mmcccodecinformation.h"
#include "mmccevents.h"
#include "mmccevent.h"

#include "mccunittestmacros.h"

_LIT8(KTestKey128bits,      "1122334455667788");
_LIT8(KTestMKI128Bits,      "AAAAAAAAAAAAAAAA");
_LIT8(KRFCTestSalt112bits,  "0EC675AD498AFE");

#ifndef __WINSCW__
 
const TInt KTestIapId = 1;

#else

const TInt KTestIapId = 6;
const TInetAddr KNetworkAddr = KInetAddrLoop;

#endif

//  INHERITED FUNCTIONS

/**
* Callback function to receive media signalling
* @param aSignal Received media signal
* @return void
*/
void UT_CMccSecureInterface::MccEventReceived( const TMccEvent& aEvent )
    {
    if ( KMccLinkCreated == aEvent.iEventType )
        {
        RDebug::Print( _L("KMccLinkCreated Event arrived succesfully!") );

        TMccNetSettingsPackage package;
        package.Copy( aEvent.iEventData );
        TBuf<50> localAddr;
        TBuf<50> localPublicAddr;
        TMccNetSettings netSettings = package();
        netSettings.iLocalAddress.Output( localAddr );
        netSettings.iLocalPublicAddress.Output( localPublicAddr );
        
        RDebug::Print( _L("Local ADDR: %S, PORT: %d, Public ADDR: %S, PORT: %d,"),
            &localAddr, netSettings.iLocalAddress.Port(), 
            &localPublicAddr, netSettings.iLocalPublicAddress.Port() );
        RDebug::Print( _L("Local RTCP PORT: %d, Public RTCP PORT: %d"), 
            netSettings.iLocalRtcpPort, netSettings.iLocalPublicRtcpPort );
              
        
        CActiveScheduler::Stop();     
        }
        
    }


/**
* Callback function to receive stream started event.
* @param aSessionId Session ID
* @param aLinkId Link ID
* @param aStreamId Stream ID
*/
void UT_CMccSecureInterface::MccMediaStarted( TUint32 aSessionId,
                            TUint32 aLinkId,     
                            TUint32 aStreamId,
                            TUint32 /*aSinkSourceId*/ )
    {
    RDebug::Print( _L("\n/********************* CALLBACK FUNCTION UT_CMccSecureInterface *****************************/") );
    RDebug::Print( _L("UT_CMccSecureInterface::StreamStarted -- Stream Started SessionId = %d"), aSessionId );
    RDebug::Print( _L("UT_CMccSecureInterface::StreamStarted -- Stream Started LinkId    = %d"), aLinkId );
    RDebug::Print( _L("UT_CMccSecureInterface::StreamStarted -- Stream Started StreamId  = %d\n"), aStreamId );
   
 //   CActiveScheduler::Stop();
    }

/**
* Callback function to receive stream stopped event.
* @param aSessionId Session ID
* @param aLinkId Link ID
* @param aStreamId Stream ID
*/
void UT_CMccSecureInterface::MccMediaStopped( TUint32 aSessionId,
                            TUint32 aLinkId,
                            TUint32 aStreamId,
                            TUint32 /*aSinkSourceId*/ )
    {
    RDebug::Print( _L("\n/********************* CALLBACK FUNCTION UT_CMccSecureInterface *****************************/") );
    RDebug::Print( _L("UT_CMccSecureInterface::StreamStopped -- Stream Stopped SessionId = %d"), aSessionId );
    RDebug::Print( _L("UT_CMccSecureInterface::StreamStopped -- Stream Stopped LinkId    = %d"), aLinkId );
    RDebug::Print( _L("UT_CMccSecureInterface::StreamStopped -- Stream Stopped StreamId  = %d\n"), aStreamId );
   
 //   CActiveScheduler::Stop();
    }

/**
* Callback function to receive stream paused event.
* @param aSessionId Session ID
* @param aLinkId Link ID
* @param aStreamId Stream ID
*/
void UT_CMccSecureInterface::MccMediaPaused( TUint32 aSessionId,
                           TUint32 aLinkId,
                            TUint32 aStreamId,
                            TUint32 /*aSinkSourceId*/ )
    {
    RDebug::Print( _L("\n/********************* CALLBACK FUNCTION UT_CMccSecureInterface *****************************/") );
    RDebug::Print( _L("UT_CMccSecureInterface::StreamPaused -- Stream Paused SessionId = %d"), aSessionId );
    RDebug::Print( _L("UT_CMccSecureInterface::StreamPaused -- Stream Paused LinkId    = %d"), aLinkId );
    RDebug::Print( _L("UT_CMccSecureInterface::StreamPaused -- Stream Paused StreamId  = %d\n"), aStreamId );
    
 //   CActiveScheduler::Stop();
    }

/**
* Callback function to receive stream resumed event.
* @param aSessionId Session ID
* @param aLinkId Link ID
* @param aStreamId Stream ID
*/
void UT_CMccSecureInterface::MccMediaResumed( TUint32 aSessionId,
                            TUint32 aLinkId,
                            TUint32 aStreamId,
                            TUint32 /*aSinkSourceId*/ )
    {
    RDebug::Print( _L("\n/********************* CALLBACK FUNCTION UT_CMccSecureInterface *****************************/") );
    RDebug::Print( _L("UT_CMccSecureInterface::StreamResumed -- Stream Resumed SessionId = %d"), aSessionId );
    RDebug::Print( _L("UT_CMccSecureInterface::StreamResumed -- Stream Resumed LinkId    = %d"), aLinkId );
    RDebug::Print( _L("UT_CMccSecureInterface::StreamResumed -- Stream Resumed StreamId  = %d\n"), aStreamId );
    
 //   CActiveScheduler::Stop();
    }

/**
* Callback function to receive stream prepared event.
* @param aSessionId Session ID
* @param aLinkId Link ID
* @param aStreamId Stream ID
*/
void UT_CMccSecureInterface::MccMediaPrepared( TUint32 /*aSessionId*/,
                             TUint32 /*aLinkId*/,
                            TUint32 /*aStreamId*/,
                            TUint32 /*aSinkSourceId*/ )
    {
    CActiveScheduler::Stop();     
    }

/**
* Callback function to receive stream inactivity event.
* @param aSessionId Session ID
* @param aLinkId Link ID
* @param aStreamId Stream ID
*/
void UT_CMccSecureInterface::MccMediaInactive( TUint32 aSessionId,
                             TUint32 aLinkId,
                            TUint32 aStreamId,
                            TUint32 /*aSinkSourceId*/ )
    {
    RDebug::Print( _L("\n/********************* CALLBACK FUNCTION UT_CMccSecureInterface *****************************/") );
    RDebug::Print( _L("UT_CMccSecureInterface::StreamInactive -- Stream Prepared SessionId = %d"), aSessionId );
    RDebug::Print( _L("UT_CMccSecureInterface::StreamInactive -- Stream Prepared LinkId    = %d"), aLinkId );
    RDebug::Print( _L("UT_CMccSecureInterface::StreamInactive -- Stream Prepated StreamId  = %d\n"), aStreamId );
    
//    CActiveScheduler::Stop();
    }

/**
* Callback function to receive stream activity event.
* @param aSessionId Session ID
* @param aLinkId Link ID
* @param aStreamId Stream ID
*/
void UT_CMccSecureInterface::MccMediaActive( TUint32 aSessionId,
                             TUint32 aLinkId,
                            TUint32 aStreamId,
                            TUint32 /*aSinkSourceId*/ )
    {
    RDebug::Print( _L("\n/********************* CALLBACK FUNCTION UT_CMccSecureInterface *****************************/") );
    RDebug::Print( _L("UT_CMccSecureInterface::StreamActive -- Stream Prepared SessionId = %d"), aSessionId );
    RDebug::Print( _L("UT_CMccSecureInterface::StreamActive -- Stream Prepared LinkId    = %d"), aLinkId );
    RDebug::Print( _L("UT_CMccSecureInterface::StreamActive -- Stream Prepated StreamId  = %d\n"), aStreamId );
    
 //   CActiveScheduler::Stop();
    }

/**
* Callback function to receive error event.
* @param aError Error code
* @return None
*/
void UT_CMccSecureInterface::MccCtrlError( TInt /*aError*/ )
    {
    }

/**
* Callback function to receive error event.
* @param aError Error code
* @param aSessionId Session ID
* @param aLinkId Link ID
*/
void UT_CMccSecureInterface::MccCtrlError( TInt /*aError*/,
                           TUint32 /*aSessionId*/,
                           TUint32 /*aLinkId*/,
                            TUint32 /*aStreamId*/,
                            TUint32 /*aSinkSourceId*/ )
    {
    }



void UT_CMccSecureInterface::WaitForEvent( TUint32 /*aSessionId*/, TUint32 /*aLinkId*/,
        TUint32 /*aStreamId*/, TMccEventType /*aEventType*/ )
        {
        CActiveScheduler::Start();
        }
        
// CONSTRUCTION
UT_CMccSecureInterface* UT_CMccSecureInterface::NewL()
    {
    UT_CMccSecureInterface* self = UT_CMccSecureInterface::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccSecureInterface* UT_CMccSecureInterface::NewLC()
    {
    UT_CMccSecureInterface* self = new( ELeave ) UT_CMccSecureInterface();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccSecureInterface::~UT_CMccSecureInterface()
    {
//    delete iInterface;   
    }

// Default constructor
UT_CMccSecureInterface::UT_CMccSecureInterface()
    {
    }

// Second phase construct
void UT_CMccSecureInterface::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccSecureInterface::SetupL(  )
    {
    iInterface = CMccInterface::NewL( *this );

    iNetsettings.iRemoteAddress = KInetAddrLoop;
    iNetsettings.iLocalRtcpPort = 5000;
    iNetsettings.iRemoteRtcpPort = 5001;
    iNetsettings.iIapId = KTestIapId;

	iSecureIntf = CMccSecureInterface::NewL( *iInterface );
    }

void UT_CMccSecureInterface::Teardown(  )
    {
    if( iInterface )
        {
        // Close session
        iInterface->CloseSession( iSessionId );
        
        delete iInterface;
        iInterface = NULL;
        }

    delete iSecureIntf;
    }

void UT_CMccSecureInterface::UT_CMccSecureInterface_NewLL(  )
    {
    CMccSecureInterface* iSecure;
    iSecure = CMccSecureInterface::NewL( *iInterface );
    delete iSecure;
    }

void UT_CMccSecureInterface::UT_CMccSecureInterface_CreateContextL(  )
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
    
    TUint32 contextId( 0 );
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iSecureIntf->CreateContext( mccMasterKeyPckg,
                                                       saltPckg,
                                                       contextId,
                                                       cryptoParamsPckg ) );                                                     
    }

void UT_CMccSecureInterface::UT_CMccSecureInterface_UpdateContextL(  )
    {
    RPointerArray<CMccCodecInformation> codecArray;
    CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray );
    User::LeaveIfError( iInterface->GetCapabilities( codecArray ) );
    
    
    User::LeaveIfError( iInterface->CreateSession( iSessionId ) );
    
    User::LeaveIfError(
        iInterface->CreateLink( iSessionId, KMccLinkSecure, iLinkId,
            iNetsettings ) );
                
    // DL
    User::LeaveIfError(
        iInterface->AddDataSink( KUidMmfAudioOutput, KNullDesC8, iAudioSinkId ) );
    User::LeaveIfError(
        iInterface->AddDataSource( KMccRtpSourceUid, KNullDesC8, iRtpSourceId ) );
     
    User::LeaveIfError( iInterface->CreateStream( iSessionId, iLinkId, iDlStreamId, 
        KMccAudioDownlinkStream, *codecArray[0] ) ); 
            
    // UL  
    User::LeaveIfError( 
        iInterface->AddDataSink( KMccRtpSinkUid, KNullDesC8, iRtpSinkId ) );
    User::LeaveIfError( 
        iInterface->AddDataSource( KUidMmfAudioInput, KNullDesC8, iMicSourceId ) );
        
    
    User::LeaveIfError( iInterface->CreateStream( iSessionId, iLinkId, iUlStreamId, 
        KMccAudioUplinkStream, *codecArray[0] ) ); 
        
    CleanupStack::PopAndDestroy( &codecArray ); 
        
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
    
    TUint32 contextId( 0 );
    TUint32 contextId2( 0 );
    
    User::LeaveIfError( iSecureIntf->CreateContext( mccMasterKeyPckg,
                                                       saltPckg,
                                                       contextId,
                                                       cryptoParamsPckg ) );
                                                       
    _LIT8(KUpdateTestKey128bits,"9922334455667711");
                                                  
    HBufC8* masterKey2 = KUpdateTestKey128bits().AllocLC();
    TMccSrtpMasterKey master2( *masterKey2, KNullDesC8() );
    TMccSrtpMasterKeyPckg masterKeyPckg2( master2 );
    CleanupStack::PopAndDestroy( masterKey2 );
    
    User::LeaveIfError( iSecureIntf->CreateContext( mccMasterKeyPckg,
                                                         saltPckg,
                                                         contextId2,
                                                         cryptoParamsPckg ) );
                                                         
    User::LeaveIfError( iSecureIntf->Bind( iSessionId,
                                                iLinkId,
                                                iDlStreamId,
                                                iRtpSourceId,
                                                contextId) );
                                                
    User::LeaveIfError( iSecureIntf->Bind( iSessionId,
                                                iLinkId,
                                                iUlStreamId,
                                                iRtpSinkId,
                                                contextId2 ) );
                                               
    User::LeaveIfError( iInterface->PrepareStream( iSessionId, iLinkId, iDlStreamId ) );
 //   this->WaitForEvent( iSessionId, iLinkId, iDlStreamId, KMccStreamPrepared );
    
    User::LeaveIfError( iInterface->PrepareStream( iSessionId, iLinkId, iUlStreamId ) );
//    this->WaitForEvent( iSessionId, iLinkId, iDlStreamId, KMccStreamPrepared );

    User::LeaveIfError( iSecureIntf->RemoveContext( contextId ) );
    User::LeaveIfError( iSecureIntf->RemoveContext( contextId2 ) );
    
    User::LeaveIfError( iInterface->DeleteStream( iSessionId, iLinkId, iDlStreamId ) );
    
    User::LeaveIfError( iInterface->DeleteStream( iSessionId, iLinkId, iUlStreamId ) );
    
    User::LeaveIfError( iInterface->CloseLink( iSessionId, iLinkId ) );
    
    User::LeaveIfError( iInterface->CloseSession( iSessionId ) ); 
    }

void UT_CMccSecureInterface::UT_CMccSecureInterface_RemoveContextL(  )
    {
    // Creates secure session
    RPointerArray< CMccCodecInformation > codecArray;
    CleanupResetAndDestroy< RPointerArray< CMccCodecInformation > >::PushL( codecArray );
    User::LeaveIfError( iInterface->GetCapabilities( codecArray ) );
     
    User::LeaveIfError( iInterface->CreateSession( iSessionId ) );
    User::LeaveIfError(
        iInterface->CreateLink( iSessionId, KMccLinkSecure, iLinkId,
            iNetsettings ) );
            
 //   this->WaitForEvent( iSessionId, iLinkId, 0, KMccLinkCreated );
    
    // DL
    User::LeaveIfError(
        iInterface->AddDataSink( KUidMmfAudioOutput, KNullDesC8, iAudioSinkId ) );
    User::LeaveIfError(
        iInterface->AddDataSource( KMccRtpSourceUid, KNullDesC8, iRtpSourceId ) );
     
    User::LeaveIfError( iInterface->CreateStream( iSessionId, iLinkId, iDlStreamId, 
        KMccAudioDownlinkStream, *codecArray[0] ) ); 
            
    // UL  
    User::LeaveIfError( 
        iInterface->AddDataSink( KMccRtpSinkUid, KNullDesC8, iRtpSinkId ) );
    User::LeaveIfError( 
        iInterface->AddDataSource( KUidMmfAudioInput, KNullDesC8, iMicSourceId ) );
        
    
    User::LeaveIfError( iInterface->CreateStream( iSessionId, iLinkId, iUlStreamId, 
        KMccAudioUplinkStream, *codecArray[0] ) ); 
        
    CleanupStack::PopAndDestroy( &codecArray ); 
    

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
    
    TUint32 contextId( 0 );
    TUint32 contextId2( 0 );
    
    User::LeaveIfError( iSecureIntf->CreateContext( mccMasterKeyPckg,
                                                       saltPckg,
                                                       contextId,
                                                       cryptoParamsPckg ) );
                                                       
    MCC_EUNIT_ASSERT_NO_LEAVE( iSecureIntf->RemoveContext( contextId ) );   
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iSecureIntf->CreateContext( mccMasterKeyPckg,
                                                       saltPckg,
                                                       contextId,
                                                       cryptoParamsPckg ) );                                                
                                                       
    _LIT8(KUpdateTestKey128bits,"9922334455667711");
                                                  
    HBufC8* masterKey2 = KUpdateTestKey128bits().AllocLC();
    TMccSrtpMasterKey master2( *masterKey2, KNullDesC8() );
    TMccSrtpMasterKeyPckg masterKeyPckg2( master2 );
    CleanupStack::PopAndDestroy( masterKey2 );
    
    User::LeaveIfError( iSecureIntf->CreateContext( mccMasterKeyPckg,
                                                         saltPckg,
                                                         contextId2,
                                                         cryptoParamsPckg ) );
                                                         
    User::LeaveIfError( iSecureIntf->Bind( iSessionId,
                                                iLinkId,
                                                iDlStreamId,
                                                iRtpSourceId,
                                                contextId) );
                                                
    User::LeaveIfError( iSecureIntf->Bind( iSessionId,
                                                iLinkId,
                                                iUlStreamId,
                                                iRtpSinkId,
                                                contextId2 ) );
                                                
    User::LeaveIfError( iInterface->PrepareStream( iSessionId, iLinkId, iDlStreamId ) );
 //   this->WaitForEvent( iSessionId, iLinkId, iDlStreamId, KMccStreamPrepared );
    
    
   TInt error( KErrNone );
    
    // Can't bind same crypto context twice                                               
    error = iSecureIntf->Bind( iSessionId,
                               iLinkId,
                               iUlStreamId,
                               iRtpSourceId,
                               contextId);
                               
    EUNIT_ASSERT( KErrArgument == error );
                              
    error = iSecureIntf->Bind( iSessionId,
                               iLinkId,
                               iDlStreamId,
                               iRtpSourceId,
                               contextId2 );
                              
    EUNIT_ASSERT( KErrArgument == error );
    
    
    iInterface->RemoveDataSource( iSessionId, iLinkId, iDlStreamId, iRtpSourceId );
    iInterface->RemoveDataSink( iSessionId, iLinkId, iUlStreamId, iRtpSinkId );
    
    User::LeaveIfError( iSecureIntf->RemoveContext( contextId ) );
    User::LeaveIfError( iSecureIntf->RemoveContext( contextId2 ) );
    
    User::LeaveIfError( iInterface->DeleteStream( iSessionId, iLinkId, iDlStreamId ) );
    User::LeaveIfError( iInterface->DeleteStream( iSessionId, iLinkId, iUlStreamId ) );
    User::LeaveIfError( iInterface->CloseLink( iSessionId, iLinkId ) );
    User::LeaveIfError( iInterface->CloseSession( iSessionId ) );
    }

void UT_CMccSecureInterface::UT_CMccSecureInterface_BindL(  )
    {
    // Creates secure session
    RPointerArray< CMccCodecInformation > codecArray;
    CleanupResetAndDestroy< RPointerArray< CMccCodecInformation > >::PushL( codecArray );
    User::LeaveIfError( iInterface->GetCapabilities( codecArray ) );
    
    
    User::LeaveIfError( iInterface->CreateSession( iSessionId ) );
    User::LeaveIfError(
        iInterface->CreateLink( iSessionId, KMccLinkSecure, iLinkId,
            iNetsettings ) );
            
 //   this->WaitForEvent( iSessionId, iLinkId, 0, KMccLinkCreated );
    
    // DL
    User::LeaveIfError(
        iInterface->AddDataSink( KUidMmfAudioOutput, KNullDesC8, iAudioSinkId ) );
    User::LeaveIfError(
        iInterface->AddDataSource( KMccRtpSourceUid, KNullDesC8, iRtpSourceId ) );
     
    User::LeaveIfError( iInterface->CreateStream( iSessionId, iLinkId, iDlStreamId, 
        KMccAudioDownlinkStream, *codecArray[0] ) ); 
            
    // UL  
    User::LeaveIfError( 
        iInterface->AddDataSink( KMccRtpSinkUid, KNullDesC8, iRtpSinkId ) );
    User::LeaveIfError( 
        iInterface->AddDataSource( KUidMmfAudioInput, KNullDesC8, iMicSourceId ) );
        
    
    User::LeaveIfError( iInterface->CreateStream( iSessionId, iLinkId, iUlStreamId, 
        KMccAudioUplinkStream, *codecArray[0] ) ); 
        
    CleanupStack::PopAndDestroy( &codecArray ); 

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
    
    TUint32 contextId( 0 );
    TUint32 contextId2( 0 );
    
    User::LeaveIfError( iSecureIntf->CreateContext( mccMasterKeyPckg,
                                                       saltPckg,
                                                       contextId,
                                                       cryptoParamsPckg ) );
                                                       
    _LIT8(KUpdateTestKey128bits,"9922334455667711");
                                                  
    HBufC8* masterKey2 = KUpdateTestKey128bits().AllocLC();
    TMccSrtpMasterKey master2( *masterKey2, KNullDesC8() );
    TMccSrtpMasterKeyPckg masterKeyPckg2( master2 );
    CleanupStack::PopAndDestroy( masterKey2 );
    
    User::LeaveIfError( iSecureIntf->CreateContext( mccMasterKeyPckg,
                                                         saltPckg,
                                                         contextId2,
                                                         cryptoParamsPckg ) );
                                                         
    User::LeaveIfError( iSecureIntf->Bind( iSessionId,
                                           iLinkId,
                                           iDlStreamId,
                                           iRtpSourceId,
                                           contextId) );
                                                
                                                
    User::LeaveIfError( iSecureIntf->Bind( iSessionId,
                                           iLinkId,
                                           iUlStreamId,
                                           iRtpSinkId,
                                           contextId2 ) );
                                           
    TInt error( KErrNone );
    
    // Can't bind same crypto context twice                                               
    error = iSecureIntf->Bind( iSessionId,
                               iLinkId,
                               iUlStreamId,
                               iRtpSourceId,
                               contextId);
                               
    EUNIT_ASSERT( KErrArgument == error );
                              
    error = iSecureIntf->Bind( iSessionId,
                               iLinkId,
                               iDlStreamId,
                               iRtpSourceId,
                               contextId2 );
                               
    EUNIT_ASSERT( KErrArgument == error );
                                                
    User::LeaveIfError( iInterface->PrepareStream( iSessionId, iLinkId, iDlStreamId ) );
 //   this->WaitForEvent( iSessionId, iLinkId, iDlStreamId, KMccStreamPrepared );
    
    
    
    iInterface->RemoveDataSource( iSessionId, iLinkId, iDlStreamId, iRtpSourceId );
    iInterface->RemoveDataSink( iSessionId, iLinkId, iUlStreamId, iRtpSinkId );
    
    User::LeaveIfError( iSecureIntf->RemoveContext( contextId ) );
    User::LeaveIfError( iSecureIntf->RemoveContext( contextId2 ) );
    
    User::LeaveIfError( iInterface->CloseLink( iSessionId, iLinkId ) );
    User::LeaveIfError( iInterface->CloseSession( iSessionId ) );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccSecureInterface,
    "CMccSecureInterface test",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMccSecureInterface",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSecureInterface_NewLL, Teardown)

EUNIT_TEST(
    "CreateContext - test ",
    "CMccSecureInterface",
    "CreateContext",
    "FUNCTIONALITY",
    SetupL, UT_CMccSecureInterface_CreateContextL, Teardown)

EUNIT_TEST(
    "UpdateContext - test ",
    "CMccSecureInterface",
    "UpdateContext",
    "FUNCTIONALITY",
    SetupL, UT_CMccSecureInterface_UpdateContextL, Teardown)

EUNIT_TEST(
    "RemoveContext - test ",
    "CMccSecureInterface",
    "RemoveContext",
    "FUNCTIONALITY",
    SetupL, UT_CMccSecureInterface_RemoveContextL, Teardown)

EUNIT_TEST(
    "Bind - test ",
    "CMccSecureInterface",
    "Bind",
    "FUNCTIONALITY",
    SetupL, UT_CMccSecureInterface_BindL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
