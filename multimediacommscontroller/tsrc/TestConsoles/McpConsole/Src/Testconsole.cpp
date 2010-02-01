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
* Description:    Part of TestConsole application.
**              Methods for the class CTestAppConsole
**
*/




#include <e32std.h>
#include <e32svr.h>
#include <e32base.h>
#include <AudioPreference.h>
#include "TestConsole.h"
#include "MccInternalCodecs.h"
#include "MmccEvents.h"
#include "MccDef.h"
#include "MmccCodecAMR.h"


_LIT( KTxtMainInstructions, "Please select one option:\n"
 L"1. Session & Q. Stream creation\n"
 L"2. Prepare & W. Start Stream\n"
 L"3. Pause Stream\n"
 L"4. Resume Stream\n"
 L"5. Stop & d. Delete Stream\n"
 L"6. Close Session\n"
 L"7. Set volume & gain\n"
 L"8. Get gain & maxgain & codec & volume\n"
 L"\n"
 L"c. Codec menu\n"
 L"m. DTMF menu\n"
 L"n. Network settings\n"
 L"r. Send non-RTCP data\n"
 L"t. Toggle inactivity timer on/off\n"
 L"9. Quit\n" );

_LIT( KTxtCodecInstructions, "Select option:\n"
L"1. Supported Codecs\n"
L"2. Change codec\n"
L"3. Set Codec Settings\n"
L"4. Toggle VAD/FW\n"
L"5. Set Codec\n"
L"6. Set FTMP attribute\n"
L"9. Return main menu\n" );


_LIT( KTxtDTMFInstructions, "Select option:\n "
L"1. Start DTMF Tone\n"
L"2. Stop DTMF Tone\n"
L"3. Send DTMF Tones\n"
L"4. Continue sending\n"
L"9. Return main menu\n" );

//*******************************************************************************
// Method      : CTestAppConsole::NewL()
// Purpose     : 
// Parameters  : 
// Return Value: 
//*******************************************************************************
CTestAppConsole* CTestAppConsole::NewL()
    {
    CTestAppConsole* self = new( ELeave ) CTestAppConsole();
    CleanupStack::PushL( self );

    self->ConstructL();

    CleanupStack::Pop();
    return self;
    }

//*******************************************************************************
// Method      : CTestAppConsole::CTestAppConsole()
// Purpose     : Constructor
// Parameters  : 
// Return Value: 
//*******************************************************************************
CTestAppConsole::CTestAppConsole()
    :
    CActive( EPriorityStandard ),
    iDTMFSessionID( KNullId ),
    iDTMFStreamID( 0 ),
    iInactivityTimerActive( EFalse ),
    iIapId( -1 ),
    iNetSettingsDone( EFalse ),
    iCurMenu( EMainMenu ),
    iDtmfMode( EFalse ),
    iSdesReported( EFalse ),
    iSrReported( EFalse ),
    iRrReported( EFalse ),
    iNumOfNonRtcpDataSent( 0 )
    {
    CActiveScheduler::Add( this );
    }

//*******************************************************************************
// Method      : CTestAppConsole::ConstructL()
// Purpose     : 
// Parameters  : 
// Return Value: 
//*******************************************************************************
void CTestAppConsole::ConstructL()
    {
    _LIT( KTxtTitle, " Mcc Test " );
    iConsole = Console::NewL( KTxtTitle, TSize( KConsFullScreen, KConsFullScreen ) );
    DisplayConsoleMenu( KTxtMainInstructions );

    //__KHEAP_MARK;
    iMccInterface = CMccInterface::NewL( *this );

    iCodecArray.Reset();
    iMccInterface->GetCapabilities( iCodecArray/*iCodecInformation*/ ); // RPointerArray<TFourCC>& aCodecs

    RunTestCodecFactory();
    }

//*******************************************************************************
// Method      : CTestAppConsole::~CTestAppConsole()
// Purpose     : Destructor
// Parameters  : 
// Return Value: 
//*******************************************************************************
CTestAppConsole::~CTestAppConsole()
    {    
    Cancel(); // any outstanding request
   
    if ( iConsole )
        {
        delete iConsole;
        }
        
    delete iInstruct;
    
    iCodecInformation.ResetAndDestroy();
    iCodecInformation.Close();
    
    iCodecArray.ResetAndDestroy();
    iCodecArray.Close();
    
    iMccInterface->Close();    
    delete iMccInterface;


    //__KHEAP_MARKEND;
    }

//*******************************************************************************
// Method      : CTestAppConsole::StartTesting()
// Purpose     : start this AO 
// Parameters  : 
// Return Value: 
//*******************************************************************************
void CTestAppConsole::StartTesting()
    {
    DoRead();
    }

//*******************************************************************************
// Method      : CTestAppConsole::DoRead()
// Purpose     : get the user's option and send request to scheduler
// Parameters  : 
// Return Value: 
//*******************************************************************************
void CTestAppConsole::DoRead()
    {
    //PrintOptions();

    iConsole->Read( iStatus );
    SetActive();
    }

//*******************************************************************************
// Method      : CTestAppConsole::RunL()
// Purpose     : 
// Parameters  : 
// Return Value: 
//*******************************************************************************
void CTestAppConsole::RunL()
    {    
    // According to current test case and direct the user's command
    // to proper command handler.
    switch ( iCurMenu )    
        {
        case EMainMenu:
            DisplayConsoleMenu( KTxtMainInstructions );
            ProcessMainInput();
            break;
        case EDTMFTestMenu:
            DisplayConsoleMenu( KTxtDTMFInstructions );
            ProcessDTMFInput();
            break;
        case ECodecTestMenu:
            DisplayConsoleMenu( KTxtCodecInstructions );
            ProcessCodecInput();
            break;
        default:
            break;
        }

        //ProcessKey( TChar( iConsole->KeyCode() ) );
        //DoRead();

    }

//*******************************************************************************
// Method      : CTestAppConsole::DoCancel()
// Purpose     : 
// Parameters  : 
// Return Value: 
//*******************************************************************************
void CTestAppConsole::DoCancel()
    {
    iConsole->ReadCancel();
    }


//*******************************************************************************
// Method      : CTestAppConsole::DisplayConsoleMenu()
// Purpose     : Display main or sub console menus for different test cases
// Parameters  : TDesc &aInstructions
// Return Value: void
//*******************************************************************************
void CTestAppConsole::DisplayConsoleMenu( const TDesC &aInstructions )
    {
    if ( iInstruct )
        {
        delete iInstruct;
        iInstruct = NULL;
        }
    iInstruct = aInstructions.AllocL();
    iConsole->ClearScreen();
    iConsole->Write( *iInstruct );
    }


//*******************************************************************************
// Method      : CTestAppConsole::ProcessMainInput()
// Purpose     : Obtain user's option and decide which test case to run next.
// Parameters  : 
// Return Value: 
//*******************************************************************************
void CTestAppConsole::ProcessMainInput()
    {
    TBuf<20> line;
    GetStringFromConsole( line );
    
    if ( line.Length() > 0 )
        {
        TChar inputChar = line.Ptr()[0];
        
        switch( inputChar )
            {
            case '1':
                RunTestCreateSession();
                break;
            case 'q':
                RunTestCreateStreams();
                break;

            case 'z':
                RunTest1c();
                break;

            case '2':
                RunTestPrepareStreams();
                break;
            case 'p':
                RunTest2p();
                break;

            case 'w':
                RunTestStartStream();
                break;
            case '3':
                RunTestPauseStreams();
                break;
            case '4':
                RunTestResumeStreams();
                break;
            case '5':
                RunTestStopStream();
                break;
            case 'd':
                RunTestDeleteStreams();
                break;
            case '6':
                RunTestCloseSession();
                break;
            case '7':
                RunTest7();
                break;
            case '8':
                RunTestGetCodecAndAudioSettings();
                break;

            case 'l':
                SetRemoteAddr();
                break;
            case 'n':
                SetNetSettings();
                break;

            case 'r':
                SendNonRtcpData();
                break;

            case 't':
                ToggleInactivityTimer();
                break;

            // Menus
			case 'c':
			    iCurMenu = ECodecTestMenu;
			    DisplayConsoleMenu( KTxtCodecInstructions );
			    break;
            case 'm':
                iCurMenu = EDTMFTestMenu;
                DisplayConsoleMenu( KTxtDTMFInstructions );
                break;

            case '9':
                iMccInterface->Close();
                CActiveScheduler::Stop();
                break;
            default:
                _LIT( KTxtWrongOption, "Wrong Option! Try Again." );
                DisplayMsg( KTxtWrongOption );
                break;        
            }
        }
    // Ready to get next input option.
    DoRead();
    }


//*******************************************************************************
// Method      : CTestAppConsole::DisplayMsg()
// Purpose     : Display testing message on screen
// Parameters  : TDesC &
// Return Value: 
//*******************************************************************************
void CTestAppConsole::DisplayMsg( const TDesC &aMsg )
    {
    iConsole->ClearScreen();
    iConsole->Write( *iInstruct );
    iConsole->Printf( KTxtLineBreak );
    iConsole->Printf( aMsg );
    iConsole->Printf( KTxtLineBreak );
    }

//*******************************************************************************
// Method      : CTestAppConsole::GetAddrFromConsole()
// Purpose     : 
// Parameters  : 
// Return Value: 
//*******************************************************************************
TKeyCode CTestAppConsole::GetStringFromConsole( TDes &aAddr )
    {
    // Get a line from console
    TKeyCode input = EKeyNull;
    const TInt start_pos = iConsole->WhereX();
    aAddr.Zero();

    // loop until descriptor full or EKeyEnter or EKeyEscape entered
    do {
        // get one character
        input = iConsole->Getch();
        // process it
        if( input == EKeyBackspace  ||  input == EKeyDelete )
            {
            // backspace or delete
            if( iConsole->WhereX() > start_pos )
                {
                iConsole->SetPos( iConsole->WhereX() - 1 );
                iConsole->ClearToEndOfLine();
                if( aAddr.Length() > 0 ) 
                    {
                    aAddr.SetLength( aAddr.Length() - 1 );
                    }
                }
            }
        else
            {
            // other than backspace or delete
            TChar ch( input );
            if( ch.IsPrint() )
                {
                aAddr.Append( ch );
                iConsole->Printf( _L( "%c" ), input );
                }
            }   
        } 
    while( aAddr.Length() < aAddr.MaxLength()  &&  input != EKeyEnter  &&  input != EKeyEscape );

    DisplayMsg( KTxtLineBreak );
    return input;
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::SetNetSettings
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::SetNetSettings()
    {
    TBuf<20> line;
    TUint port;
    TUint rport;
    TUint iapid;

    iConsole->Printf( _L( "\nEnter Iap Id: " ) );
    GetIntegerFromConsole( iapid );

    iNetSettings.iIapId = iapid;
    iNetSettings.iMediaQosValue = 46;  

	iConsole->Printf( _L( "\nEnter the Remote IP address " ) );
	
	GetStringFromConsole( line );

    iRemoteAddr.Input( line );

    iConsole->Printf( _L( "\nEnter the Remote port: " ) );
	GetIntegerFromConsole( rport );

    RDebug::Print( _L("RemotePort: %d"), rport );
	
	iRemoteAddr.SetPort( rport );

    iConsole->Printf( _L( "\nEnter the Local port: " ) );

	GetIntegerFromConsole( port );
    
    RDebug::Print( _L( "LocalPort: %d" ), port );

	iConsole->Printf( _L( "Addr: %d\n"), iRemoteAddr.Address() );
    RDebug::Print( _L( "RemoteAddr: %d"), iRemoteAddr.Address() );
    
    iNetSettings.iRemoteAddress.SetAddress( iRemoteAddr.Address() );
    iNetSettings.iRemoteAddress.SetPort( rport );
    iNetSettings.iLocalPort = port;

    DisplayMsg( _L( "Create DTMF session <y/n>: " ) );
    GetStringFromConsole( line );
    TChar inputChar = line.Ptr()[0];

    if ( inputChar == 'y' )
        {
        iDtmfMode = ETrue;
        }

    // RTCP is not available in DTMF sessions
    if ( !iDtmfMode )
        {
        iConsole->Printf( _L( "\nEnable RTCP? <y/n>: " ) );
    	GetStringFromConsole( line );
    	TChar inputChar = line.Ptr()[0];

    	if ( inputChar == 'y' )
    	    {
    		iNetSettings.iMediaSignalling = ESignalRtcp;
    		iConsole->Printf( _L( "\nRTCP enabled\n" ) );
    	    }    
        }

    iNetSettingsDone = ETrue;
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::RunTestCreateSession
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::RunTestCreateSession()
    {
    TBuf<20> line;

    if ( !iNetSettingsDone )
        {
        SetNetSettings();
        }

    if ( iDtmfMode )
        {
        DisplayMsg( _L( "Enable inband DTMF <y/n>: " ) );
        GetStringFromConsole( line );
        TChar inputChar = line.Ptr()[0];
        if ( inputChar == 'y' )
            {
            iNetSettings.iMediaSignalling 
                = TMCCMediaSignalingType( iNetSettings.iMediaSignalling | KSignalInbandDtmf );
            DisplayMsg( _L( "Inband DTMF enabled" ) );
            }

        DisplayMsg( _L( "Enable outband DTMF <y/n>: " ) );
        GetStringFromConsole( line );
        inputChar = line.Ptr()[0];

        if ( inputChar == 'y' )
            {
            iNetSettings.iMediaSignalling 
                = TMCCMediaSignalingType( iNetSettings.iMediaSignalling | KSignalOutbandDtmf );
            DisplayMsg( _L( "Outband DTMF enabled" ) );
            }

        TInt err = iMccInterface->CreateSession( iDTMFSessionID );
        if ( err == KErrNone )
            {
            iConsole->Printf( _L( "DTMF Session created, id: %d\n" ), iSessionId );
            }
        else
            {
            iConsole->Printf( _L( "Could not create DTMF session, error %d\n" ), err );
            }

        err = iMccInterface->CreateLink( iDTMFSessionID, KMccLinkGeneral, iDtmfLinkID, iNetSettings );
        if ( err == KErrNone )
            {
            iConsole->Printf( _L( "DTMF link created, id: %d\n" ), iDtmfLinkID );
            }
        else
            {
            iConsole->Printf( _L( "Could not create DTMF session, error %d\n" ), err );
            }
        }
    else
        {
        // Create session
        TInt err = iMccInterface->CreateSession( iSessionId );
        if ( err == KErrNone )
            {
            iConsole->Printf( _L( "Session created, id: %d\n" ), iSessionId );
            }
        else
            {
            iConsole->Printf( _L( "Could not create session, error %d\n" ), err );
            }
        }        

    TVersion version = iMccInterface->Version();
    iConsole->Printf( _L( "Version %d.%d.%d\n" ),
                      version.iMajor, version.iMinor, version.iBuild );
    };

// ---------------------------------------------------------------------------
// CTestAppConsole::RunTestCreateStreams
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::RunTestCreateStreams()
    {
    TInt err( KErrNone );
    if ( KMccFourCCIdDTMF == iCodecArray[iCodecSelected]->FourCC() )
        {
        // Create DTMF stream
        err = iMccInterface->CreateStream( iDTMFSessionID, iDtmfLinkID, iDTMFStreamID, 
                                           EMccAudioUplinkStream, iCodecArray[iCodecSelected] );
        if ( err != KErrNone )
            {
            iConsole->Printf( _L( "\nCould not create DTMF stream" ) );
            return;
            }
        iConsole->Printf( _L( "\nEnter the Remote port: " ) );
        TInt rport;
        GetIntegerFromConsole( rport );
        iRemoteAddr.SetPort( rport );
        iMccInterface->SetRemoteAddress( iDTMFSessionID, iDtmfLinkID, iRemoteAddr );
        iConsole->Printf( _L( "Stream created. ID: DTMF %d, pt: %d\n" ), iDTMFStreamID, iCodecArray[iCodecSelected]->PayloadType() );
        }
    else 
        {
        // Create uplink
        iConsole->Printf( _L( "Creating uplink...\n" ) );
        err = iMccInterface->CreateLink( iSessionId, KMccLinkGeneral, iUplinkId, iNetSettings );
        if ( err == KErrNone )
            {
            iConsole->Printf( _L( "Uplink created, id: %d, port: %d\n" ),
                              iUplinkId, iNetSettings.iLocalPort );
            }
        else
            {
            iConsole->Printf( _L( "Could not create uplink, error %d\n" ), err );
            return;
            }

        // Create upstream

        // Make the AMR codec compatible with the RTP sender program
        if ( iCodecSelected == 0 )
            {
            iCodecArray[iCodecSelected]->SetBitrate( KAmrNbBitrate122 );
            iCodecArray[iCodecSelected]->SetCodecMode( EOctetAligned );
            iCodecArray[iCodecSelected]->SetPayloadType( 96 );
            iCodecArray[iCodecSelected]->SetPTime( 20 );
            iCodecArray[iCodecSelected]->SetMaxPTime( 200 );
            }

        err = iMccInterface->CreateStream( iSessionId, iUplinkId, iUpStreamId,
                                           EMccAudioUplinkStream, iCodecArray[iCodecSelected] );
        if ( err != KErrNone )
            {
            iConsole->Printf( _L( "\nCould not create upstream, error %d" ), err );
            return;
            }
        iConsole->Printf( _L( "Upstream created. Sess %d, link %d, stream %d, pt: %d\n" ),
                          iSessionId, iUplinkId, iUpStreamId,
                          iCodecArray[iCodecSelected]->PayloadType() );

        // Create downlink
        iConsole->Printf( _L( "Creating downlink...\n" ) );
        err = iMccInterface->CreateLink( iSessionId, KMccLinkGeneral, iDownlinkId, iNetSettings );
        if ( err == KErrNone )
            {
            iConsole->Printf( _L( "Downlink created, id: %d, port: %d\n" ),
                                   iDownlinkId, iNetSettings.iLocalPort );
            }
        else
            {
            iConsole->Printf( _L( "Could not create downlink, error %d\n" ), err );
            }

        // Create downstream
        err = iMccInterface->CreateStream( iSessionId, iDownlinkId, iDownStreamId,
                                           EMccAudioDownlinkStream, iCodecArray[iCodecSelected] );
        if ( err != KErrNone )
            {
            iConsole->Printf( _L( "\nCould not create downstream, error %d" ), err );
            return;
            }
        iConsole->Printf( _L( "Downstream created. Sess %d, link %d, stream %d, pt: %d\n" ),
                          iSessionId, iDownlinkId, iDownStreamId,
                          iCodecArray[iCodecSelected]->PayloadType() );

        iMccInterface->SetRemoteAddress( iSessionId, iUplinkId, iRemoteAddr );
        iMccInterface->SetRemoteAddress( iSessionId, iDownlinkId, iRemoteAddr );

        iConsole->Printf( _L( "\nRemote addresses set" ) );
        }
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::RunTest1c
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::RunTest1c()
    {
    TInt err( KErrNone );
    if ( KMccFourCCIdDTMF == iCodecArray[iCodecSelected]->FourCC() )
        {
        err = iMccInterface->CreateStream( iDTMFSessionID, iDtmfLinkID, iDTMFStreamID,
                                           EMccAudioUplinkStream, iCodecArray[iCodecSelected] );
        if ( err != KErrNone )
            {
            iConsole->Printf( _L( "\nCould not create DTMF stream" ) );
            return;
            }
        iConsole->Printf( _L( "\nEnter the Remote port: " ) );
        TInt rport;
        GetIntegerFromConsole( rport );
        iRemoteAddr.SetPort( rport );
        iMccInterface->SetRemoteAddress( iDTMFSessionID, iDtmfLinkID, iRemoteAddr );
        iConsole->Printf( _L( "Stream created. ID: DTMF %d, pt: %d\n" ), iDTMFStreamID, iCodecArray[iCodecSelected]->PayloadType() );
        }
    else
        {
        iConsole->Printf( _L( "\nCreate Uplink or Downlink <u/d>: " ) );

        TBuf<20> line;
        GetStringFromConsole( line );
        TChar inputChar = line.Ptr()[0];
        
        if ( inputChar == 'u' ) 
            {
            err = iMccInterface->CreateStream( iSessionId, iUplinkId, iUpStreamId,
                                               EMccAudioUplinkStream, iCodecArray[iCodecSelected] );
            if ( err != KErrNone )
                {
                iConsole->Printf( _L( "\nCould not create uplink stream" ) );
                return;
                }
            }
        else if ( inputChar == 'd' )
            {
            err = iMccInterface->CreateStream( iSessionId, iDownlinkId, iDownStreamId,
                                               EMccAudioDownlinkStream, iCodecArray[iCodecSelected] );
            if ( err != KErrNone )
                {
                iConsole->Printf( _L( "\nCould not create downlink stream" ) );
                return;
                }
            }
        else
            {
            iConsole->Printf( _L( "\nInvalid input" ) );
            return;
            }

        iConsole->Printf( _L( "\nEnter the Remote port: " ) );
        TUint rport;

        GetStringFromConsole( line );
        TLex lex( line );
        lex.Val( rport,EDecimal );

        iRemoteAddr.SetPort( rport );
        if ( inputChar == 'u' ) 
            {
            err = iMccInterface->SetRemoteAddress( iSessionId, iUplinkId, iRemoteAddr );
            if ( err != KErrNone )
                {
                iConsole->Printf( _L( "\nCould not set remote address ( up )" ) );
                return;
                }
            }
        else if ( inputChar == 'd' )
            {
            err = iMccInterface->SetRemoteAddress( iSessionId, iDownlinkId, iRemoteAddr );
            if ( err != KErrNone )
                {
                iConsole->Printf( _L( "\nCould not set remote address ( down )" ) );
                return;
                }
            }

        iConsole->Printf( _L( "Stream created. ID: UL: %d, DL: %d, pt: %d\n" ), iUpStreamId, iDownStreamId, iCodecArray[iCodecSelected]->PayloadType() );
        }        
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::RunTestPrepareStreams
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::RunTestPrepareStreams()
    {
    TInt err( KErrNone );
    if ( KMccFourCCIdDTMF == iCodecArray[iCodecSelected]->FourCC() )
        {
        if ( KNullId != iDTMFStreamID )
            {
            iMccInterface->SetPriority( iDTMFSessionID, iDtmfLinkID, iDTMFStreamID, KAudioPriorityPhoneCall /*EMdaPriorityNormal*/ );        
            err = iMccInterface->PrepareStream( iDTMFSessionID, iDtmfLinkID, iDTMFStreamID );
            if ( err != KErrNone )
                {
                iConsole->Printf( _L( "\nCould not prepare DTMF stream" ) );
                return;
                }
            else
                {
                iConsole->Printf( _L( "\nDTMF stream prepared" ) );
                }
            }
        else
            {
            iConsole->Printf( _L( "\nCreate DTMF stream first" ) );
            }
        }
    else
        {
        // Set UL priority
        err = iMccInterface->SetPriority( iSessionId, iUplinkId, iUpStreamId, KAudioPriorityPhoneCall );
        if ( err != KErrNone )
            {
            iConsole->Printf( _L( "\nCould not set priority ( up )" ) );
            return;
            }
        else
            {
            iConsole->Printf( _L( "\nUpstream priority set" ) );
            }

        // Set DL priority
        err = iMccInterface->SetPriority( iSessionId, iDownlinkId, iDownStreamId, KAudioPriorityPhoneCall );
        if ( err != KErrNone )
            {
            iConsole->Printf( _L( "\nCould not set priority ( down )" ) );
            return;
            }
        else
            {
            iConsole->Printf( _L( "\nDownstream priority set" ) );
            }

        // Prepare upstream
        err = iMccInterface->PrepareStream( iSessionId, iUplinkId, iUpStreamId ); // UP
        if ( err != KErrNone )
            {
            iConsole->Printf( _L( "\nCould not prepare ( up )" ) );
            return;
            }
        else
            {
            iConsole->Printf( _L( "\nUpstream prepared" ) );
            }

        // Prepare downstream
        err = iMccInterface->PrepareStream( iSessionId, iDownlinkId, iDownStreamId ); // Down
        if ( err != KErrNone )
            {
            iConsole->Printf( _L( "\nCould not prepare ( down )" ) );
            return;
            }
        else
            {
            iConsole->Printf( _L( "\nDownstream prepared\n" ) );
            }
        }
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::RunTest2p
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::RunTest2p()
    {
    TUint iUpStreamId;
    TUint sessionID;
    TBool inputOK( ETrue );
    
    DisplayMsg( _L( "\nSessionId: " ) );
    if ( GetIntegerFromConsole( sessionID ) )
        {
        inputOK = EFalse;
        }
    
    DisplayMsg( _L( "\nStreamId to prepare: " ) );
    if ( GetIntegerFromConsole( iUpStreamId ) )
        {
        inputOK = EFalse;        
        }

    if ( inputOK )
        {
        iMccInterface->SetPriority( sessionID, 0/*TBI*/, iUpStreamId, 100 );
        iMccInterface->PrepareStream( sessionID, 0/*TBI*/, iUpStreamId );
        }
    else
        {
        DisplayMsg( _L( "Invalid input!" ) );
        }        
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::RunTestStartStream
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::RunTestStartStream()
    {
    TBuf<20> line;
    TUint input;
    TChar inputChar;
    TBool inputOK( ETrue );
    TInt err( KErrNone );
    TUint32 session( 0 );
    TUint32 link( 0 );
    TUint32 stream( 0 );
    TBool startPaused( EFalse );
    TBool enableRtcp( EFalse );
    
    DisplayMsg( _L( "\nStart which stream? 1 = up, 2 = down: " ) );
    if ( GetIntegerFromConsole( input ) )
        {
        inputOK = EFalse;        
        }
    if ( !inputOK )
        {
        DisplayMsg( _L( "Invalid input!" ) );
        }        
    else if ( input == 1 )
        {
        session = iSessionId;
        link = iUplinkId;
        stream = iUpStreamId;
        }
    else if ( input == 2 )
        {
        session = iSessionId;
        link = iDownlinkId;
        stream = iDownStreamId;
        }
    else
        {
        DisplayMsg( _L( "Please enter 1 or 2" ) );
        }

    // Get pause mode
    iConsole->Printf( _L( "\nStart stream paused? <y/n>: " ) );
	GetStringFromConsole( line );
	inputChar = line.Ptr()[0];

	if ( inputChar == 'y' )
	    {
		startPaused = ETrue;
	    }    

    // Get RTCP mode
    iConsole->Printf( _L( "\nEnable RTCP? <y/n>: " ) );
	GetStringFromConsole( line );
	inputChar = line.Ptr()[0];
	if ( inputChar == 'y' )
	    {
		enableRtcp = ETrue;
	    }    

    // Start stream
    iConsole->Printf( _L( "\nStarting stream..." ) );
    err = iMccInterface->StartStream( session, link, stream,
                                      startPaused, enableRtcp );
    if ( err != KErrNone )
        {
        iConsole->Printf( _L( "\nCould not start stream, error %d" ), err );
        }
    else
        {
        iConsole->Printf( _L( "\nStream started" ) );
        }
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::RunTestPauseStreams
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::RunTestPauseStreams()
    {    
    TBuf<20> line;
    TUint input;
    TChar inputChar;
    TBool enableRtcp( EFalse );
    TBool inputOK( ETrue );
    TInt err( KErrNone );
    
    DisplayMsg( _L( "\nPause which stream? 1 = up, 2 = down: " ) );
    if ( GetIntegerFromConsole( input ) )
        {
        inputOK = EFalse;        
        }

    if ( !inputOK )
        {
        DisplayMsg( _L( "Invalid input!" ) );
        return;
        }        

    // Get RTCP mode
    iConsole->Printf( _L( "\nEnable RTCP? <y/n>: " ) );
	GetStringFromConsole( line );
	inputChar = line.Ptr()[0];
	if ( inputChar == 'y' )
	    {
		enableRtcp = ETrue;
	    }    

    if ( input == 1 )
        {
        // Pause upstream
        iConsole->Printf( _L( "\nPausing upstream..." ) );
        err = iMccInterface->PauseStream( iSessionId, iUplinkId,
                                          iUpStreamId, enableRtcp );
        if ( err != KErrNone )
            {
            iConsole->Printf( _L( "\nCould not pause upstream, error %d" ), err );
            }
        else
            {
            iConsole->Printf( _L( "\nUpstream paused" ) );
            }
        }
    else if ( input == 2 )
        {
        // Pause downstream
        iConsole->Printf( _L( "\nPausing downstream..." ) );
        err = iMccInterface->PauseStream( iSessionId, iDownlinkId,
                                          iDownStreamId, enableRtcp );
        if ( err != KErrNone )
            {
            iConsole->Printf( _L( "\nCould not pause downstream, error %d" ), err );
            }
        else
            {
            iConsole->Printf( _L( "\nDownstream paused" ) );
            }
        }
    else
        {
        DisplayMsg( _L( "Please enter 1 or 2" ) );
        }
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::RunTestResumeStreams
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::RunTestResumeStreams()
    {    
    TBuf<20> line;
    TUint input;
    TBool inputOK( ETrue );
    TChar inputChar;
    TBool enableRtcp( EFalse );
    TInt err( KErrNone );
    
    DisplayMsg( _L( "\nResume which stream? 1 = up, 2 = down: " ) );
    if ( GetIntegerFromConsole( input ) )
        {
        inputOK = EFalse;        
        }

    if ( !inputOK )
        {
        DisplayMsg( _L( "Invalid input!" ) );
        return;
        }        

    // Get RTCP mode
    iConsole->Printf( _L( "\nEnable RTCP? <y/n>: " ) );
	GetStringFromConsole( line );
	inputChar = line.Ptr()[0];
	if ( inputChar == 'y' )
	    {
		enableRtcp = ETrue;
	    }    

    if ( input == 1 )
        {
        // Resume upstream
        iConsole->Printf( _L( "\nResuming upstream..." ) );
        err = iMccInterface->ResumeStream( iSessionId, iUplinkId,
                                           iUpStreamId, enableRtcp );
        if ( err != KErrNone )
            {
            iConsole->Printf( _L( "\nCould not resume upstream, error %d" ), err );
            }
        else
            {
            iConsole->Printf( _L( "\nUpstream resumed" ) );
            }
        }
    else if ( input == 2 )
        {
        // Resume downstream
        iConsole->Printf( _L( "\nResuming downstream..." ) );
        err = iMccInterface->ResumeStream( iSessionId, iDownlinkId,
                                           iDownStreamId, enableRtcp );
        if ( err != KErrNone )
            {
            iConsole->Printf( _L( "\nCould not resume downstream, error %d" ), err );
            }
        else
            {
            iConsole->Printf( _L( "\nDownstream resumed" ) );
            }
        }
    else
        {
        DisplayMsg( _L( "Please enter 1 or 2" ) );
        }
    }



// ---------------------------------------------------------------------------
// CTestAppConsole::RunTestStopStream
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::RunTestStopStream()
    {    
    TUint input;
    TBool inputOK( ETrue );
    TInt err( KErrNone );
    
    DisplayMsg( _L( "\nStop which stream? 1 = up, 2 = down: " ) );
    if ( GetIntegerFromConsole( input ) )
        {
        inputOK = EFalse;        
        }

    if ( !inputOK )
        {
        DisplayMsg( _L( "Invalid input!" ) );
        }        
    else if ( input == 1 )
        {
        // Stop upstream
        iConsole->Printf( _L( "\nStopping upstream..." ) );
        err = iMccInterface->StopStream( iSessionId, iUplinkId, iUpStreamId );
        if ( err != KErrNone )
            {
            iConsole->Printf( _L( "\nCould not stop upstream, error %d" ), err );
            }
        else
            {
            iConsole->Printf( _L( "\nUpstream stopped" ) );
            }
        }
    else if ( input == 2 )
        {
        // Stop downstream
        iConsole->Printf( _L( "\nStopping downstream..." ) );
        err = iMccInterface->StopStream( iSessionId, iDownlinkId, iDownStreamId );
        if ( err != KErrNone )
            {
            iConsole->Printf( _L( "\nCould not stop downstream, error %d" ), err );
            }
        else
            {
            iConsole->Printf( _L( "\nDownstream stopped" ) );
            }
        }
    else
        {
        DisplayMsg( _L( "Please enter 1 or 2" ) );
        }
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::RunTestDeleteStreams
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::RunTestDeleteStreams()
    {
    TUint input;
    TBool inputOK( ETrue );
    TInt err( KErrNone );
    
    DisplayMsg( _L( "\nDelete which stream? 1 = up, 2 = down: " ) );
    if ( GetIntegerFromConsole( input ) )
        {
        inputOK = EFalse;        
        }

    if ( !inputOK )
        {
        DisplayMsg( _L( "Invalid input!" ) );
        }        
    else if ( input == 1 )
        {
        // Delete upstream
        iConsole->Printf( _L( "\nDeleting upstream..." ) );
        err = iMccInterface->DeleteStream( iSessionId, iUplinkId, iUpStreamId );
        if ( err != KErrNone )
            {
            iConsole->Printf( _L( "\nCould not delete upstream, error %d" ), err );
            }
        else
            {
            iConsole->Printf( _L( "\nUpstream deleted" ) );
            }

        // Close link too
        iConsole->Printf( _L( "\nClosing uplink..." ) );
        err = iMccInterface->CloseLink( iSessionId, iUplinkId );
        if ( err != KErrNone )
            {
            iConsole->Printf( _L( "\nCould not close uplink, error %d" ), err );
            }
        else
            {
            iConsole->Printf( _L( "\nUplink closed" ) );
            }
        }
    else if ( input == 2 )
        {
        // Delete downstream
        iConsole->Printf( _L( "\nDeleting downstream..." ) );
        err = iMccInterface->DeleteStream( iSessionId, iDownlinkId, iDownStreamId );
        if ( err != KErrNone )
            {
            iConsole->Printf( _L( "\nCould not delete downstream, error %d" ), err );
            }
        else
            {
            iConsole->Printf( _L( "\nDownstream deleted" ) );
            }

        // Close link too
        iConsole->Printf( _L( "\nClosing downlink..." ) );
        err = iMccInterface->CloseLink( iSessionId, iDownlinkId );
        if ( err != KErrNone )
            {
            iConsole->Printf( _L( "\nCould not close downlink, error %d" ), err );
            }
        else
            {
            iConsole->Printf( _L( "\nDownlink closed" ) );
            }
        }
    else
        {
        DisplayMsg( _L( "Please enter 1 or 2" ) );
        }
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::RunTestCloseSession
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::RunTestCloseSession()
{    
    TUint aSessionId;
    iConsole->Printf( _L( "\nEnter SessionId: " ) );
    TBuf<20> line;

    GetStringFromConsole( line );
    TLex lex( line );
    lex.Val( aSessionId,EDecimal );

    TInt err = iMccInterface->CloseSession( aSessionId );
    if ( err == KErrNone )
        {
        iConsole->Printf( _L( "Session Closed.\n" ) );
        }
    else
        {
        iConsole->Printf( _L( "\nCould not close session, error %d" ), err );
        }
}    

// ---------------------------------------------------------------------------
// CTestAppConsole::RunTest7
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::RunTest7()
{    
    //TInt aBitrate = 12200;
    TUint aVolume = 5;
    TUint aGain = 4;
    TInt aBalance = 0;

    iConsole->Printf( _L( "\nEnter volume: " ) );
    GetIntegerFromConsole( aVolume );
    iMccInterface->SetVolume( aVolume );

    iConsole->Printf( _L( "\nEnter gain: " ) );
    GetIntegerFromConsole( aGain );

    iMccInterface->SetGain( aGain ); 
    iMccInterface->SetBalance( iSessionId, iUplinkId, iUpStreamId, aBalance,
                               EMccAudioPlay );
    iMccInterface->SetBalance( iSessionId, iDownlinkId, iDownStreamId, aBalance,
                               EMccAudioPlay );
    iConsole->Printf( _L( "Settings done.\n " ) );
}    

// ---------------------------------------------------------------------------
// CTestAppConsole::RunTestGetCodecAndAudioSettings
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::RunTestGetCodecAndAudioSettings()
    {
    // get codec
    TInt volume;
    TInt maxvolume;
    TInt aGain;
    TInt aMGain;
    CMccCodecInformation* codec( NULL );
    TInt aUpBalance;
    TInt aDownBalance;
    TInt error;

    iMccInterface->GetGain( aGain );
    iMccInterface->MaxGain( iSessionId, iUplinkId, iUpStreamId, aMGain );
    TRAP( error, codec = iMccInterface->GetCodecL( iSessionId, iUplinkId, iUpStreamId ) );
    if ( error != KErrNone )
        {
        iConsole->Printf( _L( "Could not get codec, error %d\n" ), error );
        }
    iMccInterface->Volume( volume ); 
    iMccInterface->MaxVolume( iSessionId, iDownlinkId, iDownStreamId, maxvolume );

    iMccInterface->Balance( iSessionId, iUplinkId, iUpStreamId, aUpBalance, EMccAudioPlay );
    iMccInterface->Balance( iSessionId, iDownlinkId, iDownStreamId, aDownBalance, EMccAudioPlay );

    iConsole->Printf( _L( "Gain: %d MaxGain: %d Codec: %d Vol: %d MaxVol: %d uBal: %d dBal: %d\n" ),
                      aGain, aMGain, codec->FourCC(), volume, maxvolume, aUpBalance, aDownBalance );
    iConsole->Printf( _L( "Get done.\n" ) );
    }    


// ---------------------------------------------------------------------------
// CTestAppConsole::RunTestDisplaySupportedCodecs
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::RunTestDisplaySupportedCodecs()
    {
    iConsole->Printf( _L( "Supported codecs: %d\n" ), iCodecArray.Count() );

    for ( TInt i=0; i<iCodecArray.Count(); i++ ) 
        {
        iConsole->Printf( _L( "codec %d: br:%d 4cc:%d\n" ),
                          i, iCodecArray[i]->Bitrate(), iCodecArray[i]->FourCC() );
        }
    }


// ---------------------------------------------------------------------------
// CTestAppConsole::RunSetCodecSettings
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::RunSetCodecSettings()
    {
    iConsole->Printf( _L( "\nEnter PTime: " ) );
    TUint iPTime;
    GetIntegerFromConsole( iPTime );

    iCodecArray[iCodecSelected]->SetPTime( iPTime );

    iConsole->Printf( _L( "\nEnter MaxPTime: " ) );
    TUint iMaxPTime;
    GetIntegerFromConsole( iMaxPTime );

    iCodecArray[iCodecSelected]->SetMaxPTime( iMaxPTime );

    iConsole->Printf( _L( "\nSettings done" ) );
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::RunSetFmtpAttr
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::RunSetFmtpAttr()
    {
    TBuf8<50> buf;
    TInt err( KErrNone );

    // First, try to set an invalid FMTP attribute string
    _LIT8( KFmtpInvalid, "!PQOI;oOwiU=45;#Ur(%UT" );
    buf.Append( KFmtpInvalid );
    TRAP( err, iCodecArray[iCodecSelected]->SetFmtpAttrL( buf ) );
    if ( err != KErrNone )
        {
        iConsole->Printf( _L( "\nCould not set invalid FMTP attribute, err %d" ), err );
        }
    else
        {
        iConsole->Printf( _L( "\nInvalid FMTP attribute set. (no error)" ) );
        }

    // Now make a valid one
    _LIT8( KFmtpPayload, "%d " );
    _LIT8( KFmtpOctetAlign, "octet-align=%d;" );
    _LIT8( KFmtpModeSet, "mode-set=%d,%d, %d" );
    
    TInt payload( 5 );
    TInt octetAlign( 1 );
    TInt fmtpModeSet( 2 );
    
    // The string should look like this:
    // "xx octet-align=0/1; mode-set=0-7;" (where xx is the payload type)

    buf.Format( KFmtpPayload, payload );
    buf.AppendFormat( KFmtpOctetAlign, octetAlign );
    buf.AppendFormat( KFmtpModeSet, fmtpModeSet, 0, 5 );

    TRAP( err, iCodecArray[iCodecSelected]->SetFmtpAttrL( buf ) );
    if ( err != KErrNone )
        {
        iConsole->Printf( _L( "\nCould not set FMTP attribute, err %d" ), err );
        }
    else
        {
        iConsole->Printf( _L( "\nFMTP attribute set." ) );
        }
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::ToggleVAD
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::ToggleVAD()
    {

    if ( iCodecArray[iCodecSelected]->VAD() )
        {
        iCodecArray[iCodecSelected]->EnableVAD( EFalse );
        iConsole->Printf( _L( "VAD: OFF\n" ) );
        }
    else
        {
        TInt err = iCodecArray[iCodecSelected]->EnableVAD( ETrue );
        if ( err == KErrNotSupported ) 
            iConsole->Printf( _L( "VAD: Not supported (in current codec)\n" ) );
        else
            iConsole->Printf( _L( "VAD: ON\n" ) );
        }
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::ChangeCodec
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::ChangeCodec()
    {
    TInt codecs = iCodecArray.Count();

    if ( codecs == 0 )
        return;

    RDebug::Print( _L( "CodecArray size: %d" ), codecs );

    iCodecSelected++;
    
    if ( iCodecSelected >= codecs )
        {
        iCodecSelected = 0;
        }

    if ( iCodecArray[iCodecSelected]->FourCC() == KMccFourCCIdAMRNB )
        {
        iConsole->Printf( _L( "Codec AMR-NB " ) );
        iCodecArray[iCodecSelected]->SetPayloadType( 128 );
        }

    if ( iCodecArray[iCodecSelected]->FourCC() == KMccFourCCIdG711 )
        {
        if ( iCodecArray[iCodecSelected]->CodecMode() == EPCMU )
            iConsole->Printf( _L( "Codec PCMU " ) );
        if ( iCodecArray[iCodecSelected]->CodecMode() == EPCMA )
            iConsole->Printf( _L( "Codec PCMA " ) );
        }

    if ( iCodecArray[iCodecSelected]->FourCC() == KMccFourCCIdG729 )
        {
        iConsole->Printf( _L( "Codec G.729 " ) );
        }

    if ( iCodecArray[iCodecSelected]->FourCC() == KMccFourCCIdILBC )
        {
        iConsole->Printf( _L( "Codec iLBC " ) );
        }
    
    if ( iCodecArray[iCodecSelected]->FourCC() == KMccFourCCIdDTMF )
        {
        iConsole->Printf( _L( "Codec DTMF " ) );
        }    
        
    iConsole->Printf( _L( "4CC: %d\n" ), iCodecArray[iCodecSelected]->FourCC() );
    }



// ---------------------------------------------------------------------------
// CTestAppConsole::SetCodec
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::SetCodec()
    {
    iMccInterface->SetCodec( iSessionId, iUplinkId, iUpStreamId, iCodecArray[iCodecSelected] );
    iMccInterface->SetCodec( iSessionId, iDownlinkId, iDownStreamId, iCodecArray[iCodecSelected] );
    iConsole->Printf( _L( "Codec set to %d\n" ), iCodecArray[iCodecSelected]->FourCC() );
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::SetRemoteAddr
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::SetRemoteAddr()
    {
    TBuf<20> line;
    TLex lex( line );
    
    iConsole->ClearScreen();
    iConsole->Printf( _L( "\nEnter the Remote IP address " ) );
    
    GetStringFromConsole( line );

    iRemoteAddr.Input( line );
    
    iConsole->Printf( _L( "\nEnter the Remote port: " ) );
    TUint rport;
    
    GetStringFromConsole( line );
    lex.Assign( line );
    lex.Val( rport,EDecimal );

    iRemoteAddr.SetPort( rport );

    iConsole->Printf( _L( "Addr: %d , port %d\n" ), iRemoteAddr.Address(), rport );

    // Get link type from user
    iConsole->Printf( _L( "Enter link type ( u for up, d for down ):\n" ) );
    GetStringFromConsole( line );
    TChar inputChar = line.Ptr()[0];
    if ( inputChar == 'u' ) 
        {
        iMccInterface->SetRemoteAddress( iSessionId, iUplinkId, iRemoteAddr );
        }
    else if ( inputChar == 'd' )
        {
        iMccInterface->SetRemoteAddress( iSessionId, iDownlinkId, iRemoteAddr );
        }
    else
        {
        iConsole->Printf( _L( "Invalid input\n" ) );
        }
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::RunTestCodecFactory
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::RunTestCodecFactory()
{
    RArray< CMccCodecInformation > codecArray;

    CMccCodecInformationFactory* infoFactory = CMccCodecInformationFactory::NewL();
    TBuf8<KMaxSdpNameLength> sdpName;

    TInt i;
    for ( i = 0; i < iCodecInformation.Count(); i++ )
    {

        sdpName = iCodecInformation[i]->SdpName();

        CMccCodecInformation* tempInfo = NULL;

        TRAPD( err1, tempInfo =  infoFactory->CreateCodecInformationL( sdpName ) );
        if ( err1 ) 
        {
            tempInfo = NULL;    
        }
        else
        {
            codecArray.Append( *tempInfo );
            delete tempInfo;
        }
    }

    for ( i = 0; i < codecArray.Count(); i++ )
        {
        iCodecArray.Append( &codecArray[ i ] );
        }
    
    codecArray.Close();

    RDebug::Print( _L( "CodecArray size: %d" ), iCodecArray.Count() );

    delete infoFactory;
}

// ---------------------------------------------------------------------------
// CTestAppConsole::ToggleInactivityTimer
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::ToggleInactivityTimer()
    {
    TInt result( KErrNone );
    TUint input;
    TBool inputOK( ETrue );
    TUint32 link( 0 );
    TUint32 stream( 0 );

    const TInt KInactivityTimeout( 8000000 );

    DisplayMsg( _L( "\nTimer, which stream? 1 = up, 2 = down: " ) );
    if ( GetIntegerFromConsole( input ) )
        {
        inputOK = EFalse;        
        }

    if ( !inputOK )
        {
        DisplayMsg( _L( "Invalid input!" ) );
        }        
    else if ( input == 1 )
        {
        link = iUplinkId;
        stream = iUpStreamId;
        }
    else if ( input == 2 )
        {
        link = iDownlinkId;
        stream = iDownStreamId;
        }
    else
        {
        DisplayMsg( _L( "Please enter 1 or 2" ) );
        return;
        }
    if ( iInactivityTimerActive )
        {
        result = iMccInterface->StopInactivityTimer( iSessionId, link, stream );
        if ( result == KErrNone )
            {
            iInactivityTimerActive = EFalse;
            iConsole->Printf( _L( "Inactivity timer stopped\n" ) );
            }
        else
            {
            iConsole->Printf( _L( "Could not stop timer, error %d\n" ), result );
            }
        }
    else
        {
        result = iMccInterface->StartInactivityTimer( iSessionId, link, stream,
                                                      KInactivityTimeout );
        if ( result == KErrNone )
            {
            iInactivityTimerActive = ETrue;
            iConsole->Printf( _L( "Inactivity timer started\n" ) );
            }
        else
            {
            iConsole->Printf( _L( "Could not start timer, error %d\n" ), result );
            }
        }
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::SendNonRtcpData
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::SendNonRtcpData()
    {
    TBuf8<50> data;
    _LIT8( KData, "NonRtcpData #%d" );
    data.Format( KData, iNumOfNonRtcpDataSent++ );
    TInt result = iMccInterface->SendRTCPAnyData( iSessionId,
                                                  iDownlinkId,
                                                  iDownStreamId,
                                                  data );
    if ( result == KErrNone )
        {
        iConsole->Printf( _L( "Non-RTCP data sent\n" ) );
        }
    else
        {
        iConsole->Printf( _L( "Could not send data, error %d\n" ), result );
        }
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::MccMediaStarted
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::MccMediaStarted( TUint32 aSessionID, TUint32 aLinkId, TUint32 aStreamID, TUint32 aSinkSourceId )
    {
    RDebug::Print( _L( "CALLBACK: Stream Started: sess %u, link %u, stream %u, sinksource %u" ),
                   aSessionID, aLinkId, aStreamID, aSinkSourceId );
    iConsole->Printf( _L( "\nCALLBACK: Stream Started: sess %u, link %u, stream %u, sinksource %u\n" ),
                   aSessionID, aLinkId, aStreamID, aSinkSourceId );
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::MccMediaStopped
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::MccMediaStopped( TUint32 aSessionID, TUint32 aLinkId, TUint32 aStreamID, TUint32 aSinkSourceId )
    {
    RDebug::Print( _L( "CALLBACK: Stream Stopped: sess %u, link %u, stream %u, sinksource %u" ),
                   aSessionID, aLinkId, aStreamID, aSinkSourceId );
    iConsole->Printf( _L( "\nCALLBACK: Stream Stopped: sess %u, link %u, stream %u, sinksource %u\n" ),
                   aSessionID, aLinkId, aStreamID, aSinkSourceId );
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::MccMediaPaused
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::MccMediaPaused( TUint32 aSessionID, TUint32 aLinkId, TUint32 aStreamID, TUint32 aSinkSourceId )
    {
    RDebug::Print( _L( "CALLBACK: Stream Paused: sess %u, link %u, stream %u, sinksource %u" ),
                   aSessionID, aLinkId, aStreamID, aSinkSourceId );
    iConsole->Printf( _L( "\nCALLBACK: Stream Paused: sess %u, link %u, stream %u, sinksource %u\n" ),
                   aSessionID, aLinkId, aStreamID, aSinkSourceId );
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::SMccMediaResumed
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::MccMediaResumed( TUint32 aSessionID, TUint32 aLinkId, TUint32 aStreamID, TUint32 aSinkSourceId )
    {
    RDebug::Print( _L( "CALLBACK: Stream Resumed: sess %u, link %u, stream %u, sinksource %u" ),
                   aSessionID, aLinkId, aStreamID, aSinkSourceId );
    iConsole->Printf( _L( "\nCALLBACK: Stream Resumed: sess %u, link %u, stream %u, sinksource %u\n" ),
                   aSessionID, aLinkId, aStreamID, aSinkSourceId );
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::MccMediaPrepared
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::MccMediaPrepared( TUint32 aSessionID, TUint32 aLinkId, TUint32 aStreamID, TUint32 aSinkSourceId )
    {
    RDebug::Print( _L( "CALLBACK: Stream Prepared: sess %u, link %u, stream %u, sinksource %u" ),
                   aSessionID, aLinkId, aStreamID, aSinkSourceId );
    iConsole->Printf( _L( "\nCALLBACK: Stream Prepared: sess %u, link %u, stream %u, sinksource %u" ),
                      aSessionID, aLinkId, aStreamID, aSinkSourceId );
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::MccMediaInactive
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::MccMediaInactive( TUint32 aSessionID, TUint32 aLinkId, TUint32 aStreamID, TUint32 aSinkSourceId )
    {
    RDebug::Print( _L( "CALLBACK: Stream Inactive: sess %u, link %u, stream %u, sinksource %u" ),
                   aSessionID, aLinkId, aStreamID, aSinkSourceId );
    iConsole->Printf( _L( "\nCALLBACK: Stream Inactive: sess %u, link %u, stream %u, sinksource %u\n" ),
                   aSessionID, aLinkId, aStreamID, aSinkSourceId );
    iInactivityTimerActive = EFalse;
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::MccMediaActive
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::MccMediaActive( TUint32 aSessionID, TUint32 aLinkId, TUint32 aStreamID, TUint32 aSinkSourceId )
    {
    RDebug::Print( _L( "CALLBACK: Stream active: sess %u, link %u, stream %u, sinksource %u" ),
                   aSessionID, aLinkId, aStreamID, aSinkSourceId );
    iConsole->Printf( _L( "\nCALLBACK: Stream active: sess %u, link %u, stream %u, sinksource %u\n" ),
                   aSessionID, aLinkId, aStreamID, aSinkSourceId );
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::MccEventReceived
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::MccEventReceived( const TMccEvent& aEvent )
    {
    if ( aEvent.iEventCategory == KMccEventCategoryDtmf )
        {
        TMccDtmfEventDataPackage package;
        package.Copy( aEvent.iEventData  );  
        const TMccDtmfEventData& event = package();
        
        switch ( event.iDtmfEventType )
            {
            case KMccDtmfManualStart:
                DisplayMsg( _L( "Manual start event" ) );
                break;
            case KMccDtmfManualStop:
                DisplayMsg( _L( "Manual stop event" ) );
                break;
            case KMccDtmfManualAbort:
                DisplayMsg( _L( "Manual abort event" ) );
                break;
            case KMccDtmfSequenceStart:
                DisplayMsg( _L( "Seq start event" ) );
                break;
            case KMccDtmfSequenceStop:
                DisplayMsg( _L( "Seq stop event" ) );
                break;
            case KMccDtmfSequenceAbort:
                DisplayMsg( _L( "Seq abort event" ) );
                break;
            case KMccDtmfStopInDtmfString:
                DisplayMsg( _L( "Stop in string" ) );
                break;
            }
        }
    else if ( aEvent.iEventCategory == KMccEventCategoryRtcp )
        {
        TMccRtcpEventDataPackage package;
        package.Copy( aEvent.iEventData  );  
        const TMccRtcpEventData& event = package();

        switch ( event.iRtcpPacketType )
            {
            case KRtcpSdesPacket:
                RDebug::Print( _L( "CTestAppConsole::MediaSignalReceived RTCP SDES" ) );
                if ( !iSdesReported )
                    {
                    iConsole->Printf( _L( "MediaSignalReceived RTCP SDES" ) );
                    iSdesReported = ETrue;
                    }
                break;

            case KRtcpByePacket:
                {
                RDebug::Print( _L( "CTestAppConsole::MediaSignalReceived RTCP BYE" ) );
                iConsole->Printf( _L( "MediaSignalReceived RTCP BYE" ) );
                }
                break;

            case KRtcpAppPacket:
                {
                RDebug::Print( _L( "CTestAppConsole::MediaSignalReceived RTCP APP" ) );
                iConsole->Printf( _L( "MediaSignalReceived RTCP APP" ) );

                TPckgBuf<TRtcpApp> appPackage;
                appPackage.Copy( event.iRtcpPacketData );
                TRtcpApp app = appPackage();
                }
                break;

            case KRtcpSrPacket:
                RDebug::Print( _L( "CTestAppConsole::MediaSignalReceived RTCP SR" ) );
                if ( !iSrReported )
                    {
                    iConsole->Printf( _L( "MediaSignalReceived RTCP SR" ) );
                    iSrReported = ETrue;
                    }
                break;

            case KRtcpRrPacket:
                RDebug::Print( _L( "CTestAppConsole::MediaSignalReceived RTCP RR" ) );
                if ( !iRrReported )
                    {
                    iConsole->Printf( _L( "MediaSignalReceived RTCP RR" ) );
                    iRrReported = ETrue;
                    }
                break;

            case KRtcpPacketUndefined:
                {
                RDebug::Print( _L( "CTestAppConsole::MediaSignalReceived Non-RTCP:" ) );
                iConsole->Printf( _L( "MediaSignalReceived Data" ) );

                // Convert to 16-bit text and print
                HBufC* undefBuf = HBufC::NewLC(  event.iRtcpPacketData.Length() );
                undefBuf->Des().Copy( event.iRtcpPacketData );
                RDebug::Print( _L( "%s" ), undefBuf->Des() );
                CleanupStack::PopAndDestroy( undefBuf );
                }
                break;

            default:
                RDebug::Print( _L( "CTestAppConsole::MediaSignalReceived unknown" ) );
                break;
            }
        }
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::MccCtrlError
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::MccCtrlError( TInt aError )
    {
    iConsole->Printf( _L( "Mcc error: %d\n" ), aError );
    RDebug::Print( _L( "Mcc Error: %d" ), aError );
}

// ---------------------------------------------------------------------------
// CTestAppConsole::MccCtrlError
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::MccCtrlError( TInt aError, TUint32 aSessionId, TUint32 aLinkId, TUint32 aStreamId, TUint32 aSinkSourceId )
    {
    iConsole->Printf( _L( "Mcc error: %d Session: %u, link %u, stream %u, sinksource %u\n" ),
                      aError, aSessionId, aLinkId, aStreamId, aSinkSourceId );
    RDebug::Print( _L( "Mcc Error: %d Session: %u, link %u, stream %u, sinksource %u" ),
                      aError, aSessionId, aLinkId, aStreamId, aSinkSourceId  );
}

// -----------------------------------------------------------------------------
// CTestAppConsole::GetIntegerFromConsole
// Reads one integer from console to the parameter.
// -----------------------------------------------------------------------------
// 
TInt CTestAppConsole::GetIntegerFromConsole( TInt& aVal )
    {
    TBuf<20> line;
    GetStringFromConsole( line );

    TLex lex( line );
    TInt err = lex.Val( aVal );

    return err;
    }

// -----------------------------------------------------------------------------
// CTestAppConsole::GetIntegerFromConsole
// Reads one integer from console to the parameter.
// -----------------------------------------------------------------------------
// 
TInt CTestAppConsole::GetIntegerFromConsole( TUint& aVal )
    {
    TBuf<20> line;
    GetStringFromConsole( line );

    TLex lex( line );
    TInt err = lex.Val( aVal );
    
    return err;
    }
        
// ---------------------------------------------------------------------------
// CTestAppConsole::ProcessDTMFInput
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::ProcessDTMFInput()
    {
    TInt choice;
    TInt err( KErrNone );
    GetIntegerFromConsole( choice );

    TMccEvent event;
    event.iSessionId = iDTMFSessionID;
    event.iStreamId = iDTMFStreamID;
    
    TMccDtmfEventData eventData;
    
    switch ( choice )
        {
        case 1:
            TUint input;
            DisplayMsg( _L( "Digit to send?" ) );
            GetIntegerFromConsole( input );
            eventData.iDtmfEventType = EMccDtmfSigStartTone;
            eventData.iDtmfString.Append( TChar( input ) );
            event.iEventData.Copy( TMccDtmfEventDataPackage( eventData ) );
            
            TRAP( err, iMccInterface->SendMediaSignalL( event ) );
            if ( err == KErrNone )
                {
                DisplayMsg( _L( "Sending started" ) );
                }
            else
                {
                iConsole->Printf( _L( "Could not send, err %d" ), err );
                }
            break;

        case 2:
            eventData.iDtmfEventType = EMccDtmfSigStopTone;
            event.iEventData.Copy( TMccDtmfEventDataPackage( eventData ) );
            
            TRAP( err, iMccInterface->SendMediaSignalL( event ) );
            if ( err == KErrNone )
                {
                DisplayMsg( _L( "Sending stopped" ) );
                }
            else
                {
                iConsole->Printf( _L( "Could not stop, err %d" ), err );
                }
            break;

        case 3:
            DisplayMsg( _L( "String to send?" ) );
            TBuf<KMccMaxDtmfStringLength> dtmfString;
            GetStringFromConsole( dtmfString );
            eventData.iDtmfString.Copy( dtmfString );
            eventData.iDtmfEventType = EMccDtmfSigSendString;
            event.iEventData.Copy( TMccDtmfEventDataPackage( eventData ) );
            TRAP( err, iMccInterface->SendMediaSignalL( event ) );
            if ( err == KErrNone )
                {
                DisplayMsg( _L( "Sending started" ) );
                }
            else
                {
                iConsole->Printf( _L( "Could not start, err %d" ), err );
                }
            break;

        case 4:
            eventData.iDtmfEventType = EMccDtmfSigContinueSending;
            eventData.iContinue = ETrue;
            event.iEventData.Copy( TMccDtmfEventDataPackage( eventData ) );
            TRAP( err, iMccInterface->SendMediaSignalL( event ) );
            if ( err == KErrNone )
                {
                DisplayMsg( _L( "Sending continues" ) );
                }
            else
                {
                iConsole->Printf( _L( "Could not continue, err %d" ), err );
                }
            break;

        case 9:
            iCurMenu = EMainMenu;
            DisplayConsoleMenu( KTxtMainInstructions );
            break;

        default:
            _LIT( KTxtWrongOption, "Wrong Option!" );
            DisplayMsg( KTxtWrongOption );
            break;
        }

    // Ready to get next input option.
    DoRead();
    }

// -----------------------------------------------------------------------------
// CTestAppConsole::ProcessCodecInput
// 
// -----------------------------------------------------------------------------
// 
void CTestAppConsole::ProcessCodecInput()
    {
    TInt choice;
    GetIntegerFromConsole( choice );
    
    switch ( choice )
        {
        case 1:
            RunTestDisplaySupportedCodecs();
            break;
        case 2:
            ChangeCodec();
            break;
        case 3:
            RunSetCodecSettings();
            break;
        case 4:
            ToggleVAD();
            break;
        case 5:
            SetCodec();
            break;
        case 6:
            RunSetFmtpAttr();
            break;
        case 9:
            iCurMenu = EMainMenu;
            DisplayConsoleMenu( KTxtMainInstructions );
            break;
        default:
            _LIT( KTxtWrongOption, "Wrong Option!" );
            DisplayMsg( KTxtWrongOption );
            break;
        }

    // Ready to get next input option.
    DoRead();
    }
