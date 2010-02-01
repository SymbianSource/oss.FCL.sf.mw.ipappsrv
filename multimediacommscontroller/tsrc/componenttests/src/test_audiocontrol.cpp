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
#include "test_audiocontrol.h"
#include "mccteststubs.h"
#include "mmccinterface.h"
#include "mmcccodecinformation.h"
#include "testdefinitions.hrh"

// ======== MEMBER FUNCTIONS =================================================

// ---------------------------------------------------------------------------
// CTestAudioControlAudioControl::CTestAudioControlAudioControl
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CTestAudioControl::CTestAudioControl( CConsoleBase& aConsole,
    const TMccNetSettings& aNetsettings, CMccInterface& aIF,
    CMccInterfaceStub& aIFStub )
    : iConsole( aConsole ), iNetsettings( aNetsettings ), iInterface( aIF ),
    iIfStub( aIFStub )
    {
    }
    
// ---------------------------------------------------------------------------
// CTestAudioControl::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CTestAudioControl::ConstructL(  )
    {
    }
    
// ---------------------------------------------------------------------------
// CTestAudioControl::NewL
// Static constructor.
// ---------------------------------------------------------------------------
//   
CTestAudioControl* CTestAudioControl::NewL( CConsoleBase& aConsole,
        const TMccNetSettings& aNetsettings, CMccInterface& aIF,
        CMccInterfaceStub& aIFStub )
    {
    CTestAudioControl* self 
        = new ( ELeave ) CTestAudioControl( aConsole, aNetsettings, aIF, aIFStub );
    CleanupStack::PushL( self );

    self->ConstructL();

    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CTestAudioControl::~CTestAudioControl
// Destructor.
// ---------------------------------------------------------------------------
//
CTestAudioControl::~CTestAudioControl()
    {
    }

// ---------------------------------------------------------------------------
// CTestAudioControl::DoRunAudioControlTestsL
// ---------------------------------------------------------------------------
//
void CTestAudioControl::DoRunAudioControlTestsL()
    {
    iConsole.Printf( _L("\n*** AUDIO CONTROL TESTS ***\n") );
    
    CreateStreamsL();
    
    iConsole.Printf( _L("Get volume test\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestGetVolumeL") );
    __UHEAP_MARK;
    TestGetVolumeL();
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestGetVolumeL") );
    
    iConsole.Printf( _L("Get gain test\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestGetGainL") );
    __UHEAP_MARK;
    TestGetGainL();
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestGetGainL") );
    
    iConsole.Printf( _L("Get balance test\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestGetBalanceL") );
    __UHEAP_MARK;
    TestGetBalanceL();
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestGetBalanceL") );
    
    iConsole.Printf( _L("Get max volume test\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestGetMaxVolumeL") );
    __UHEAP_MARK;
    TestGetMaxVolumeL();
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestGetMaxVolumeL") );
    
    iConsole.Printf( _L("Get max gain test\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestGetMaxGainL") );
    __UHEAP_MARK;
    TestGetMaxGainL();
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestGetMaxGainL") );
    
    iConsole.Printf( _L("Set volume test\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestSetVolumeL") );
    __UHEAP_MARK;
    TestSetVolumeL();
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestSetVolumeL") );
    
    iConsole.Printf( _L("Set gain test\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestSetGainL") );
    __UHEAP_MARK;
    TestSetGainL();
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestSetGainL") );
    
    iConsole.Printf( _L("Set balance test\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestSetBalanceL") );
    __UHEAP_MARK;
    TestSetBalanceL();
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestSetBalanceL") );
    
    DeleteStreamsL();
    }


// ---------------------------------------------------------------------------
// CTestAudioControl::TestGetVolumeL
// ---------------------------------------------------------------------------
//
void CTestAudioControl::TestGetVolumeL()
    {
    TInt volume( 0 );
    TPckgBuf<TInt> volPckg( volume );
    User::LeaveIfError( iInterface.GetParameter(
        iSessionId, iLinkId, iDlStreamId, iAudioSinkId,
        KMccSpeakerVolume, volPckg ) );
        
    TInt err = iInterface.GetParameter( 
        iSessionId, iLinkId, iUlStreamId, iAudioSourceId,
        KMccSpeakerVolume, volPckg );
    if ( KErrNone == err )
        {
        // VOLUME IS PROPERTY OF PLAYBACK DEVICE ONLY
        //User::Leave( KErrTotalLossOfPrecision );
        }
    }
    
// ---------------------------------------------------------------------------
// CTestAudioControl::TestGetGainL
// ---------------------------------------------------------------------------
//
void CTestAudioControl::TestGetGainL()
    {
    TInt gain( 0 );
    TPckgBuf<TInt> gainPckg( gain );
    User::LeaveIfError( iInterface.GetParameter( 
        iSessionId, iLinkId, iUlStreamId, iAudioSourceId,
        KMccMicGain, gainPckg ) );
        
    TInt err = iInterface.GetParameter(
        iSessionId, iLinkId, iDlStreamId, iAudioSinkId,
        KMccMicGain, gainPckg );
    if ( KErrNone == err )
        {
        // GAIN IS PROPERTY OF RECORD DEVICE ONLY
        //User::Leave( KErrTotalLossOfPrecision );
        }
    }

// ---------------------------------------------------------------------------
// CTestAudioControl::TestGetBalanceL
// ---------------------------------------------------------------------------
//
void CTestAudioControl::TestGetBalanceL()
    {
    TPckgBuf<TInt> val;
    User::LeaveIfError( iInterface.GetParameter( 
        iSessionId, iLinkId, iUlStreamId, iAudioSourceId, KMccMicBalance, val ) );
    TInt err = iInterface.GetParameter( 
        iSessionId, iLinkId, iDlStreamId, iAudioSinkId, KMccMicBalance, val );
    if ( KErrNone == err )
        {
        //User::Leave( KErrTotalLossOfPrecision );
        }
        
    User::LeaveIfError( iInterface.GetParameter( 
        iSessionId, iLinkId, iDlStreamId, iAudioSinkId, KMccSpeakerBalance, val ) );
    err = iInterface.GetParameter( 
        iSessionId, iLinkId, iUlStreamId, iAudioSourceId, KMccSpeakerBalance, val );
    if ( KErrNone == err )
        {
        //User::Leave( KErrTotalLossOfPrecision );
        }
    }

// ---------------------------------------------------------------------------
// CTestAudioControl::TestGetMaxVolumeL
// ---------------------------------------------------------------------------
//
void CTestAudioControl::TestGetMaxVolumeL()
    {
    TInt maxVol( 0 );
    TPckgBuf<TInt> maxVolPckg( maxVol );
        
    User::LeaveIfError( iInterface.GetParameter( 
        iSessionId, iLinkId, iDlStreamId, iAudioSinkId,
        KMccSpeakerMaxVolume, maxVolPckg ) );
    if ( 0 == maxVolPckg() )
        {
        User::Leave( KErrTotalLossOfPrecision );
        }
        
    TInt err = iInterface.GetParameter( 
        iSessionId, iLinkId, iUlStreamId, iAudioSourceId,
        KMccSpeakerMaxVolume, maxVolPckg );
    if ( KErrNone == err )
        {
        // VOLUME IS PROPERTY OF PLAYBACK DEVICE ONLY
        //User::Leave( KErrTotalLossOfPrecision );
        }
    }
    
// ---------------------------------------------------------------------------
// CTestAudioControl::TestGetMaxGainL
// ---------------------------------------------------------------------------
//
void CTestAudioControl::TestGetMaxGainL()
    {
    TInt maxGain( 0 );
    TPckgBuf<TInt> maxGainPckg( maxGain );
    User::LeaveIfError( iInterface.GetParameter( iSessionId, 
        iLinkId, iUlStreamId, iAudioSourceId, KMccMicMaxGain, maxGainPckg ) );
    if ( 0 == maxGainPckg() )
        {
        User::Leave( KErrTotalLossOfPrecision );
        }
        
    TInt err = iInterface.GetParameter( iSessionId, 
        iLinkId, iDlStreamId, iAudioSinkId, KMccMicMaxGain, maxGainPckg );
    if ( KErrNone == err )
        {
        // GAIN IS PROPERTY OF RECORD DEVICE ONLY
        //User::Leave( KErrTotalLossOfPrecision );
        }
    }

// ---------------------------------------------------------------------------
// CTestAudioControl::TestSetVolumeL
// ---------------------------------------------------------------------------
//
void CTestAudioControl::TestSetVolumeL()
    {
    TInt maxVol( 0 );
    TPckgBuf<TInt> maxVolPckg( maxVol );
    User::LeaveIfError( iInterface.GetParameter( 
        iSessionId, iLinkId, iDlStreamId, iAudioSinkId,
        KMccSpeakerMaxVolume, maxVolPckg ) );
    
    TPckgBuf<TInt> volPckg( maxVolPckg() );
    User::LeaveIfError( iInterface.SetParameter( iSessionId, 
        iLinkId, iDlStreamId, iAudioSinkId, KMccSpeakerVolume, volPckg ) );
    TInt setVol = volPckg();
    User::LeaveIfError( iInterface.GetParameter( iSessionId, 
        iLinkId, iDlStreamId, iAudioSinkId, KMccSpeakerVolume, volPckg ) );
    if ( setVol != volPckg() )
        {
        User::Leave( KErrTotalLossOfPrecision );
        }
        
    TPckgBuf<TInt> volPckg2( maxVolPckg() + 1 );
    TInt err = iInterface.SetParameter( iSessionId, 
        iLinkId, iDlStreamId, iAudioSinkId, KMccSpeakerVolume, volPckg2 );
    if ( KErrNone == err )
        {
        User::Leave( KErrTotalLossOfPrecision );
        }
    
    TPckgBuf<TInt> volPckg3( 0 );
    User::LeaveIfError( iInterface.SetParameter( iSessionId, 
        iLinkId, iDlStreamId, iAudioSinkId, KMccSpeakerVolume, volPckg3 ) );

    
    err = iInterface.SetParameter( iSessionId, iLinkId, iUlStreamId,
        iAudioSourceId, KMccSpeakerVolume, volPckg3 );
    if ( KErrNone == err )
        {
        // VOLUME IS PROPERTY OF PLAYBACK DEVICE ONLY
        //User::Leave( KErrTotalLossOfPrecision );
        }
    }
    
// ---------------------------------------------------------------------------
// CTestAudioControl::TestSetGainL
// ---------------------------------------------------------------------------
//
void CTestAudioControl::TestSetGainL()
    {
    TInt maxGain( 0 );
    TPckgBuf<TInt> maxGainPckg( maxGain );
    User::LeaveIfError( iInterface.GetParameter( iSessionId, 
        iLinkId, iUlStreamId, iAudioSourceId, KMccMicMaxGain, maxGainPckg ) );

    TPckgBuf<TInt> gainPckg( maxGainPckg() );
    User::LeaveIfError( iInterface.SetParameter( iSessionId, 
        iLinkId, iUlStreamId, iAudioSourceId, KMccMicGain, gainPckg ) );
    TInt setGain = gainPckg();
    User::LeaveIfError( iInterface.GetParameter( iSessionId, 
        iLinkId, iUlStreamId, iAudioSourceId, KMccMicGain, gainPckg ) );
    if ( setGain != gainPckg() )
        {
        User::Leave( KErrTotalLossOfPrecision );
        }

    TPckgBuf<TInt> gainPckg2( maxGainPckg() + 1 );
    TInt err = iInterface.SetParameter( iSessionId, 
        iLinkId, iUlStreamId, iAudioSourceId, KMccMicGain, gainPckg2 );
    if ( KErrNone == err )
        {
        //User::Leave( KErrTotalLossOfPrecision );
        }

    TPckgBuf<TInt> gainPckg3( 0 );
    User::LeaveIfError( iInterface.SetParameter( iSessionId, 
        iLinkId, iUlStreamId, iAudioSourceId, KMccMicGain, gainPckg3 ) );
        
    err = iInterface.SetParameter( iSessionId, 
        iLinkId, iDlStreamId, iAudioSinkId, KMccMicGain, gainPckg3 );
    if ( KErrNone == err )
        {
        // GAIN IS PROPERTY OF RECORD DEVICE ONLY
        //User::Leave( KErrTotalLossOfPrecision );
        }
    }
    
// ---------------------------------------------------------------------------
// CTestAudioControl::TestSetBalanceL
// ---------------------------------------------------------------------------
//
void CTestAudioControl::TestSetBalanceL()
    {
    TInt error( KErrNone );            
    TPckgBuf<TInt> val;
    
    // TRY SOME INVALID VALUES
    val = KMMFBalanceMaxLeft - 1;
    error = iInterface.SetParameter( 
        iSessionId, iLinkId, iUlStreamId, iAudioSourceId, KMccMicBalance, val );
    if ( error == KErrNone )
        {
        //User::Leave( KErrTotalLossOfPrecision );
        }
    error = iInterface.SetParameter( 
        iSessionId, iLinkId, iDlStreamId, iAudioSinkId, KMccSpeakerBalance, val );
    if ( error == KErrNone )
        {
        //User::Leave( KErrTotalLossOfPrecision );
        }

    val = KMMFBalanceMaxRight + 1;
    error = iInterface.SetParameter( 
        iSessionId, iLinkId, iUlStreamId, iAudioSourceId, KMccMicBalance, val );
    if ( error == KErrNone )
        {
        //User::Leave( KErrTotalLossOfPrecision );
        }
    error = iInterface.SetParameter( 
        iSessionId, iLinkId, iDlStreamId, iAudioSinkId, KMccSpeakerBalance, val );
    if ( error == KErrNone )
        {
        //User::Leave( KErrTotalLossOfPrecision );
        }
        
    // TRY SOME VALID VALUES
    val = KMMFBalanceMaxLeft;
    User::LeaveIfError( iInterface.SetParameter( 
        iSessionId, iLinkId, iUlStreamId, iAudioSourceId, KMccMicBalance, val ) );
    User::LeaveIfError( iInterface.SetParameter( 
        iSessionId, iLinkId, iDlStreamId, iAudioSinkId, KMccSpeakerBalance, val ) );
        
    val = KMMFBalanceMaxRight;
    User::LeaveIfError( iInterface.SetParameter( 
        iSessionId, iLinkId, iUlStreamId, iAudioSourceId, KMccMicBalance, val ) );
    User::LeaveIfError( iInterface.SetParameter( 
        iSessionId, iLinkId, iDlStreamId, iAudioSinkId, KMccSpeakerBalance, val ) );

    // Check that the new value is taken into use
    val = KMMFBalanceCenter;
    User::LeaveIfError( iInterface.SetParameter( 
        iSessionId, iLinkId, iUlStreamId, iAudioSourceId, KMccMicBalance, val ) );
    TInt setBalance = val();
    User::LeaveIfError( iInterface.GetParameter( 
        iSessionId, iLinkId, iUlStreamId, iAudioSourceId, KMccSpeakerBalance, val ) );
    if ( setBalance != val() )
        {
        User::Leave( KErrTotalLossOfPrecision );
        }

    User::LeaveIfError( iInterface.SetParameter( 
        iSessionId, iLinkId, iDlStreamId, iAudioSinkId, KMccMicBalance, val ) );
    setBalance = val();
    User::LeaveIfError( iInterface.GetParameter( 
        iSessionId, iLinkId, iUlStreamId, iAudioSourceId, KMccSpeakerBalance, val ) );
    if ( setBalance != val() )
        {
        User::Leave( KErrTotalLossOfPrecision );
        }

    // WRONG ENUMERATION
    error = iInterface.SetParameter( 
        iSessionId, iLinkId, iUlStreamId, iAudioSourceId, KMccSpeakerBalance, val );
    if ( KErrNone == error )
        {
        //User::Leave( KErrTotalLossOfPrecision );
        }
    error = iInterface.SetParameter( 
        iSessionId, iLinkId, iDlStreamId, iAudioSinkId, KMccMicBalance, val );
    if ( KErrNone == error )
        {
        //User::Leave( KErrTotalLossOfPrecision );
        }
    }
    

// ---------------------------------------------------------------------------
// CTestAudioControl::CreateStreamsL
// ---------------------------------------------------------------------------
//
void CTestAudioControl::CreateStreamsL()
    {
    User::LeaveIfError( iInterface.CreateSession( iSessionId ) );
    User::LeaveIfError(
        iInterface.CreateLink( iSessionId, KMccLinkGeneral, iLinkId,
            iNetsettings ) );
    iIfStub.WaitForEvent( iSessionId, iLinkId, 0, KMccLinkCreated );
    
    User::LeaveIfError(
        iInterface.AddDataSink( KMccRtpSinkUid, KNullDesC8, iRtpSinkId ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KUidMmfAudioInput, KNullDesC8, iAudioSourceId ) );
    
    CMccCodecInformation* codec
        = FetchCodecByMimeSubtypeNameL( iInterface, KAMRSdpName );
    CleanupStack::PushL( codec );
    
    User::LeaveIfError( iInterface.CreateStream( iSessionId, iLinkId, iUlStreamId, 
        KMccAudioUplinkStream, *codec ) );

    User::LeaveIfError(
        iInterface.AddDataSink( KUidMmfAudioOutput, KNullDesC8, iAudioSinkId ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KMccRtpSourceUid, KNullDesC8, iRtpSourceId ) );
    User::LeaveIfError( iInterface.CreateStream( iSessionId, iLinkId, iDlStreamId, 
        KMccAudioDownlinkStream, *codec ) );
    
    CleanupStack::PopAndDestroy( codec );
    }
    

// ---------------------------------------------------------------------------
// CTestAudioControl::DeleteStreamsL
// ---------------------------------------------------------------------------
//
void CTestAudioControl::DeleteStreamsL()
    {
    /*
    User::LeaveIfError(
        iInterface.RemoveDataSource( iSessionId, iLinkId, iUlStreamId, iAudioSourceId ) );
    User::LeaveIfError(
        iInterface.RemoveDataSink( iSessionId, iLinkId, iUlStreamId, iRtpSinkId ) );
    User::LeaveIfError(
        iInterface.RemoveDataSource( iSessionId, iLinkId, iDlStreamId, iRtpSourceId ) );
    User::LeaveIfError(
        iInterface.RemoveDataSink( iSessionId, iLinkId, iDlStreamId, iAudioSinkId ) );
    */
    User::LeaveIfError( 
        iInterface.DeleteStream( iSessionId, iLinkId, iUlStreamId ) );
    User::LeaveIfError( 
        iInterface.DeleteStream( iSessionId, iLinkId, iDlStreamId ) );
    User::LeaveIfError( iInterface.CloseLink( iSessionId, iLinkId ) );
    User::LeaveIfError( iInterface.CloseSession( iSessionId ) );
    }

// ---------------------------------------------------------------------------
// CTestAudioControl::FetchCodecByMimeSubtypeNameL
// ---------------------------------------------------------------------------
//
CMccCodecInformation* CTestAudioControl::FetchCodecByMimeSubtypeNameL( 
        const CMccInterface& aInterface, const TDesC8& aName ) const
    {
    RPointerArray<CMccCodecInformation> codecArray;
    CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray );
    User::LeaveIfError( aInterface.GetCapabilities( codecArray ) );
    
    CMccCodecInformation* tmp = NULL;
    TInt ind( codecArray.Count() );
    
    while ( ( ind-- ) && ( NULL == tmp ) )
        {
        if ( codecArray[ind]->SdpName().Compare( aName ) == 0 )
            {
            tmp = codecArray[ind]->CloneDetailedL();
            }
        }
    
    if ( NULL == tmp )
        {
        User::Leave( KErrNotFound );
        }
    
    CleanupStack::PopAndDestroy( &codecArray );
    return tmp;
    }
