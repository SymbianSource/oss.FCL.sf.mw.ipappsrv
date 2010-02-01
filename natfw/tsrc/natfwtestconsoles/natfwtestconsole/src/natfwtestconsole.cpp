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
#include <e32base.h>
#include <e32debug.h>
#include <e32cons.h>
#include "natfwconnectivityapi.h"
#include <badesca.h>
#include "natfwtestconsole.h"
#include "testdefinitions.hrh"
#include "natfwtestconsolestubs.h"
#include "mediateststubs.h"
#include "iceteststubs.h"
#include "stunturntests.h"
#include "icetests.h"
#include "stunudpmediatest.h"
#include "natsettingstest.h"

#include "generalsettings.h"
#include "stunudpsettings.h"
#include "stuntcpsettings.h"
#include "turnudpsettings.h"
#include "turntcpsettings.h"
#include "additionalstunudpsettings.h"
#include "additionalstuntcpsettings.h"
#include "additionalturnudpsettings.h"
#include "additionalturntcpsettings.h"
#include "additionalicesettings.h"
#include "iceudpsettings.h"
#include "icetcpsettings.h"

#include "natfwcandidate.h"
#include "natfwcandidatepair.h"
#include "natcandidatestore.h"
#include "iap.h"

_LIT8( KIsp1vDomain, "isp1v.wipsl.com" );
_LIT8( KIsp1tDomain, "isp1t.wipsl.com" );
_LIT8( KIsp1eDomain, "isp1e.wipsl.com" );
_LIT8( KIsp1sDomain, "isp1s.wipsl.com" );
_LIT8( KDefaultDomain, "isp1v.wipsl.com" );

_LIT8( KStun, "nokia.stun" );
_LIT8( KTurn, "nokia.turn" );

const TInt32 KDefaultDestAddress = INET_ADDR( 0,0,0,0 );
const TUint KDefaultDestPort = 0;
const TUint KDefaultGranularity = 1;

//*******************************************************************************
// Method      : CTestAppConsole::NewL()
// Purpose     : 
// Parameters  : 
// Return Value: 
//*******************************************************************************
CTestAppConsole* CTestAppConsole::NewLC()
    {
    CTestAppConsole* self = new ( ELeave ) CTestAppConsole();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


//*******************************************************************************
// Method      : CTestAppConsole::CTestAppConsole()
// Purpose     : Constructor
// Parameters  : 
// Return Value: 
//*******************************************************************************
CTestAppConsole::CTestAppConsole()
    : CActive( EPriorityStandard ),
    iDestAddress( KDefaultDestAddress), iDestPort( KDefaultDestPort )
    {
    CActiveScheduler::Add( this );
    iPause = EFalse;
    }


//*******************************************************************************
// Method      : CTestAppConsole::ConstructL()
// Purpose     : 
// Parameters  : 
// Return Value: 
//*******************************************************************************
void CTestAppConsole::ConstructL()
    {
    iDomainName = KDefaultDomain().AllocL();
	iConsole = Console::NewL( KMainTitle, TSize(KConsFullScreen, KConsFullScreen) );
    }


//*******************************************************************************
// Method      : CTestAppConsole::~CTestAppConsole()
// Purpose     : Destructor
// Parameters  : 
// Return Value: 
//*******************************************************************************
CTestAppConsole::~CTestAppConsole()
    {	
    Cancel();
    delete iConsole;
	delete iInstruct;
    delete iNat;
    delete iNatStub;
    delete iIceComTest;
    delete iDomainName;
    }


//*******************************************************************************
// Method      : CTestAppConsole::StartTesting()
// Purpose     : start this AO 
// Parameters  : 
// Return Value: 
//*******************************************************************************
void CTestAppConsole::StartTesting()
    {
    IapSelectionL();
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
	if ( iPause )
	    {
	    iPause = EFalse;
	    User::RequestComplete( iListenerStatus, KErrNone );
	    DoRead();
	    }
    else
        {
        switch( iInputProcess )
            {
            case KMainInput:
                ProcessMainInput();
                break;
            case KDomainNameInput:
                ProcessDomainSetupInputL();
                break;
            case KMediaTestInput:
                ProcessMediaTestInputL();
                break;
            case KIapInput:
                ProcessIapInputL();
                break;
            }
        }
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
void CTestAppConsole::DisplayConsoleMenuL( const TDesC& aInstructions )
    {
    if ( iInstruct )
	    {
        delete iInstruct;
        iInstruct = NULL;
        }
    
    iInstruct = aInstructions.AllocL();
    iConsole->ClearScreen();
    _LIT16( KDomainFormat, "Domain: %S\n" );
    TBuf16<60> buffer;
    buffer.Copy( iDomainName->Des() );
    iConsole->Printf( KDomainFormat, &buffer  );
    iConsole->Write( *iInstruct );
    }


//*******************************************************************************
// Method      : CTestAppConsole::ProcessIapInputL()
// Purpose     : Obtain user's option is mean to be run.
// Parameters  : 
// Return Value: void
//*******************************************************************************
void CTestAppConsole::ProcessIapInputL()
    {
    TBuf<80> line;
    if( !iInstruct )
        {
        iInstruct = KTxtMainInstructions().AllocL();
        }
    GetStringFromConsole(line);
    TInt input;
    TLex lex( line );
    if ( !lex.Val( input ) )
        {
        iIapId = input;
        iInputProcess = KMainInput;
        DisplayConsoleMenuL( KTxtMainInstructions );
        }
    DoRead();
    }

//*******************************************************************************
// Method      : CTestAppConsole::ProcessDomainSetupInputL()
// Purpose     : Obtain user's option and decide which media test
//             : is mean to be run.
// Parameters  : 
// Return Value: void
//*******************************************************************************
void CTestAppConsole::ProcessMediaTestInputL()
    {
    TBuf<80> line;
    GetStringFromConsole(line);
    TBool wrongOption = EFalse;
    TInt err = KErrNone;
    TInt input;
    TLex lex( line );
        
    if ( !lex.Val( input ) )
        {
        switch( input )
            {
            case 1:
                err = FetchCandidates( KProtocolInetUdp );
                break;
            case 2:
                err = StartIceComTest( KProtocolInetUdp, EIceRoleControlling );
                break;
            case 3:
                err = StartIceComTest( KProtocolInetUdp, EIceRoleControlled );
                break;
            
            case 4:
                err = FetchCandidates( KProtocolInetUdp, ETrue );
                break;      
            case 5:
                err = StartIceComTest( KProtocolInetUdp, EIceRoleControlling, ETrue );
                break;
            case 6:
                err = StartIceComTest( KProtocolInetUdp, EIceRoleControlled, ETrue );
                break;
            
            case 7:
                err = FetchCandidate( KProtocolInetUdp, EStunComTest );
                break;      
            case 8:
                err = StartComTest( KProtocolInetUdp, EStunComTest );
                break;
            
            case 9:
                err = FetchCandidate( KProtocolInetUdp, EStunComTest, ETrue );
                break;      
            case 10:
                err = StartComTest( KProtocolInetUdp, EStunComTest, ETrue );
                break;
            
            case 11: 
                err = FetchCandidate( KProtocolInetUdp, ETurnComTest );
                break;
            
            case 12:
                err = StartComTest( KProtocolInetUdp, ETurnComTest );
                break;
            
            case 13:
                err = FetchCandidate( KProtocolInetUdp, ETurnComTest, ETrue );
                break;      
            case 14:
                err = StartComTest( KProtocolInetUdp, ETurnComTest, ETrue );
                break;
            
            case 15:
                StopIceComTest();
                break; 
            case 0:
                iInputProcess = KMainInput;
                DisplayConsoleMenuL( KTxtMainInstructions );
                break;
            default:
                wrongOption = ETrue;
                DisplayMsg( KTxtWrongOption );
                break;
            }
        if ( !wrongOption )
            {
            if ( iAudioTestICE )
                {
                iConsole->Printf( _L( "Com test ongoing. enter 15 to stop\n" ) );
                }
            else
                {
                if ( err )
                    {
                    iConsole->Printf( _L("Some tests failed: %d, check logs\n"), err );
                    RDebug::Print( _L("\nSome tests failed: %d, check logs\n"), err );
                    }
                else if ( input != 0 )
                    {
                    iConsole->Printf( _L( "TESTS COMPLETED\n" ) );
                    RDebug::Print( _L( "\nTESTS COMPLETED\n" ) );
                    }
                else
                    {
                    }
                }
            }
        wrongOption = EFalse;
        }
    // Ready to get next input option.
    DoRead();
    }


//*******************************************************************************
// Method      : CTestAppConsole::ProcessDomainSetupInputL()
// Purpose     : Obtain user's option and decide which domain name is set.
// Parameters  : 
// Return Value: void
//*******************************************************************************
void CTestAppConsole::ProcessDomainSetupInputL()
    {
    TBuf<80> line;
	GetStringFromConsole(line);

	TInt input;
	TLex lex( line );
	
	if ( !lex.Val( input ) )
	    {
		switch( input )
			{
			case 1:
			    delete iDomainName;
			    iDomainName = NULL;
                iDomainName = KIsp1vDomain().AllocL();
                iInputProcess = KMainInput;
                DisplayConsoleMenuL( KTxtMainInstructions );
			    break;
		    case 2:
                delete iDomainName;
                iDomainName = NULL;
                iDomainName = KIsp1tDomain().AllocL();
                iInputProcess = KMainInput;
                DisplayConsoleMenuL( KTxtMainInstructions );
			    break;
			case 3:
                delete iDomainName;
                iDomainName = NULL;
                iDomainName = KIsp1eDomain().AllocL();
                iInputProcess = KMainInput;
                DisplayConsoleMenuL( KTxtMainInstructions );
			    break;
			case 4:
                delete iDomainName;
                iDomainName = NULL;
                iDomainName = KIsp1sDomain().AllocL();
                iInputProcess = KMainInput;
                DisplayConsoleMenuL( KTxtMainInstructions );
			    break;
			default:
				DisplayMsg( KTxtWrongOption );
				break;
			}
	    }
	// Ready to get next input option.
   	DoRead();
    }

    
//*******************************************************************************
// Method      : CTestAppConsole::ProcessMainInput()
// Purpose     : Obtain user's option and decide which test case to run next.
// Parameters  : 
// Return Value: 
//*******************************************************************************
void CTestAppConsole::ProcessMainInput()
	{
    TBuf<80> line;
	GetStringFromConsole(line);
	
	TInt input;
	TLex lex( line );

	if ( !lex.Val( input ) )
	    {
		TInt err( KErrNone );
		TBool testsRun( ETrue );
		switch( input )
			{
			case 1:
                testsRun = EFalse;
			    DisplayConsoleMenuL( KTxtDomainSetupInstructions );
                iInputProcess = KDomainNameInput;
			    break;
            case 2:
                testsRun = EFalse;
                DisplayConsoleMenuL( KTxtMediaTestInstructions );
                iInputProcess = KMediaTestInput;
                break;
			case 3:
                err = RunNatSTUNUdpTests();
			    break;
		    case 4:
                err = RunNatSTUNTcpTests();
			    break;
			case 5:
                err = RunTurnUdpTests();
			    break;
            case 6:
                err = RunTurnTcpTests();
			    break;
			case 7:
			    err = RunIceUdpTestsL();
			    break;
			case 8:
			    err = RunIceTcpTestsL();
			    break;
			case 9:
			    err = SetLocalToDestinationAddrL();
			    if ( KErrNone == err )
			        {
			        err = RunStunUdpMediaTestL();
			        }
			    break;
            case 10:
			    StopStunUdpMediaTestL();
			    break;
            case 11:
                RunNatSettingsTest();
                break;
        	case 12:
				testsRun = EFalse;
				err = PrintLocalAddress();
				break;
        	case 13:
        		err =  PrintStoredCandidates();
                break; 
			case 0:
				testsRun = EFalse;
				CActiveScheduler::Stop();
				break;
			default:
			    testsRun = EFalse;
				DisplayMsg( KTxtWrongOption );
				break;
			}
			
	    if ( iAudioTestStun )
            {
            iConsole->Printf( _L( "Audio test ongoing. Press 10 to stop\n" ) );
            }
        if ( iAudioTestICE )
            {
            iConsole->Printf( _L( "Com test ongoing. enter 15 to stop\n" ) );
            }
        else
            {
            if ( err )
                {
                iConsole->Printf( _L("Some tests failed: %d, check logs\n"), err );
                RDebug::Print( _L("\nSome tests failed: %d, check logs\n"), err );
                }
            else if ( testsRun )
                {
                iConsole->Printf( _L( "TESTS COMPLETED\n" ) );
                RDebug::Print( _L( "\nTESTS COMPLETED\n" ) );
                }
            else
                {
                
                }
            }
	    }
	// Ready to get next input option.
   	DoRead();
	}


//*******************************************************************************
// Method      : CTestAppConsole::IapSelectionL()
// Purpose     : Display list of IAP:s and allow to choose one
// Parameters  :
// Return Value: 
//*******************************************************************************
void CTestAppConsole::IapSelectionL()
    {
    TInt err;
    RArray<TIap> iapArray;
    CleanupClosePushL( iapArray );
    iInputProcess = KIapInput;
    iConsole->ClearScreen();
    iConsole->Printf( KTxtIapList );
    
    err = TIap::ResolveIapL( iapArray );
    
    if (  err == KErrNone )
        {
        for( TUint i( 0 ); i < iapArray.Count(); i++ )
            {
            iConsole->Printf( KIapPrintFormat, &iapArray[i].IapName(),
                    iapArray[i].IapId() );
            }
        iConsole->Printf( KTxtTypeIap );
        }
    else if ( err == KErrNotFound )
        {
        iConsole->Printf( _L( "IAP NOT FOUND\n" ) );
        }
    else
        {
        iConsole->Printf( _L( "Error Occured\n" ) );
        }
    CleanupStack::PopAndDestroy( &iapArray );
    }


//*******************************************************************************
// Method      : CTestAppConsole::DisplayMsg()
// Purpose     : Display testing message on screen
// Parameters  : TDesC &
// Return Value: 
//*******************************************************************************
void CTestAppConsole::DisplayMsg(const TDesC &aMsg)
	{
	iConsole->ClearScreen();
	iConsole->Write( *iInstruct );
	iConsole->Printf( KTxtLineBreak );
	iConsole->Printf( aMsg );
	iConsole->Printf( KTxtLineBreak );
	}


//*******************************************************************************
// Method      : CTestAppConsole::GetStringFromConsole()
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
        // get one character1
        input = iConsole->Getch();
        // process it
        if(input == EKeyBackspace || input == EKeyDelete)
            {
            // backspace or delete
            if(iConsole->WhereX() > start_pos)
                {
				iConsole->SetPos(iConsole->WhereX() - 1);
				iConsole->ClearToEndOfLine();
				if(aAddr.Length() > 0)
                    {
					aAddr.SetLength(aAddr.Length() - 1);
                    }
                }
            }
        else
            {
            // other than backspace or delete
            TChar ch(input);
            if(ch.IsPrint())
                {
                aAddr.Append(ch);
                iConsole->Printf(_L("%c"), input);
                }
            }
        }
    while(aAddr.Length() < aAddr.MaxLength() && input != EKeyEnter && input != EKeyEscape);
    
    DisplayMsg( KNullDesC() );
    
    return input;
    }


//*******************************************************************************
// Method      : CTestAppConsole::RunNatSTUNUdpTests()
// Purpose     : Test the NAT Interface functions
// Parameters  : 
// Return Value: System wide error code
//*******************************************************************************
//
TInt CTestAppConsole::RunNatSTUNUdpTests()
    {
    TInt err( KErrNone );
    
    iConsole->ClearScreen();
    iConsole->Printf( _L("\n*** STUN UDP TESTS ***\n") );
    RDebug::Print( _L( "\n\nSTUN UDP TESTS" ) );
    
    __UHEAP_MARK;
    
    TRAP( err, SetupSTUNUDPTestL() );
    if ( KErrNone != err )
        {
        iConsole->Printf( _L("STUN UDP TESTSETUP FAILED: %d\n"), err );
        RDebug::Print( _L("STUN UDP TESTSETUP FAILED: %d\n"), err );
        return err;
        }
    CDesC8Array* array = new ( ELeave ) CDesC8ArrayFlat( KDefaultGranularity );
    CleanupStack::PushL( array );
    array->AppendL( KStun );
    
    CStunTurnTests* turnUdpTests = CStunTurnTests::NewL( *iConsole,
        *iNat, *iNatStub, *iDomainName, iIapId, KProtocolInetUdp, *array );
        
    CleanupStack::PopAndDestroy( array );    
    CleanupStack::PushL( turnUdpTests );
    
    TRAP( err, turnUdpTests->DoRunStunTurnTestsL() );

    CleanupStack::PopAndDestroy( turnUdpTests );
    TeardownSTUNTestL();
    
    __UHEAP_MARKEND;
   
    return err;
    }


//*******************************************************************************
// Method      : CTestAppConsole::RunNatSTUNTcpTests()
// Purpose     : Test the NAT Interface functions
// Parameters  : 
// Return Value: System wide error code
//*******************************************************************************
//
TInt CTestAppConsole::RunNatSTUNTcpTests()
    {
    TInt err( KErrNone );
    
    iConsole->ClearScreen();
    iConsole->Printf( _L("\n*** STUN TCP TESTS ***\n") );
    RDebug::Print( _L("\n\nSTUN TCP TESTS") );
    
    __UHEAP_MARK;
    
    TRAP( err, SetupSTUNTCPTestL() );
    if ( KErrNone != err )
        {
        iConsole->Printf( _L("STUN TCP TESTSETUP FAILED: %d\n"), err );
        RDebug::Print( _L("STUN TCP TESTSETUP FAILED: %d\n"), err );
        return err;
        }
    
    CDesC8Array* array = new ( ELeave ) CDesC8ArrayFlat( KDefaultGranularity );
    CleanupStack::PushL( array );
    array->AppendL( KStun );
    
    CStunTurnTests* turnTcpTests = CStunTurnTests::NewL( *iConsole,
        *iNat, *iNatStub, *iDomainName, iIapId, KProtocolInetTcp, *array );
        
    CleanupStack::PopAndDestroy( array );
    CleanupStack::PushL( turnTcpTests );
    
    TRAP( err, turnTcpTests->DoRunStunTurnTestsL() );

    CleanupStack::PopAndDestroy( turnTcpTests );
    TeardownSTUNTestL();
    
    __UHEAP_MARKEND;
        
    return err;
    }

    
//*******************************************************************************
// Method      : CTestAppConsole::RunTurnUdpTests()
// Purpose     : Test the Mcc Interface functions
// Parameters  : 
// Return Value: System wide error code
//*******************************************************************************
//
TInt CTestAppConsole::RunTurnUdpTests()
    {
    TInt err( KErrNone );
    
    iConsole->ClearScreen();
    iConsole->Printf( _L("\n*** TURN UDP TESTS ***\n") );
    RDebug::Print( _L("\n\nTURN UDP TESTS") );
    
    __UHEAP_MARK;
    
    TRAP( err, NATTurnUdpPreambleL() );
    if ( KErrNone != err )
        {
        iConsole->Printf( _L("TESTSETUP FAILED: %d\n"), err );
        RDebug::Print( _L("TEST PRINT: TEST SETUP FAILED: %d\n"), err );
        return err;
        }
    CDesC8Array* array = new ( ELeave ) CDesC8ArrayFlat( KDefaultGranularity);
    CleanupStack::PushL( array );
    array->AppendL( KTurn );
    
    CStunTurnTests* turnUdpTests = CStunTurnTests::NewL( *iConsole,
        *iNat, *iNatStub, *iDomainName, iIapId, KProtocolInetUdp, *array );
        
    CleanupStack::PopAndDestroy( array );    
    CleanupStack::PushL( turnUdpTests );
    
    TRAP( err, turnUdpTests->DoRunStunTurnTestsL() );

    CleanupStack::PopAndDestroy( turnUdpTests );
    TeardownSTUNTestL();
    
    __UHEAP_MARKEND;
    
    return err;
    }


//*******************************************************************************
// Method      : CTestAppConsole::RunTurnTcpTests()
// Purpose     : Test the Mcc Interface functions
// Parameters  : 
// Return Value: System wide error code
//*******************************************************************************
//
TInt CTestAppConsole::RunTurnTcpTests()
    {
    TInt err( KErrNone );
    
    iConsole->ClearScreen();
    iConsole->Printf( _L("\n*** TURN TCP TESTS ***\n") );
    RDebug::Print( _L("\n\nTURN TCP TESTS") );
    
    __UHEAP_MARK;
    
    TRAP( err, NATTurnTcpPreambleL() );
    if ( KErrNone != err )
        {
        iConsole->Printf( _L("TESTSETUP FAILED: %d\n"), err );
        RDebug::Print( _L("TEST PRINT: TEST SETUP FAILED: %d\n"), err );
        return err;
        }
    CDesC8Array* array = new ( ELeave ) CDesC8ArrayFlat( KDefaultGranularity);
    CleanupStack::PushL( array );
    array->AppendL( KTurn );
     
    CStunTurnTests* turnTcpTests = CStunTurnTests::NewL( *iConsole,
        *iNat, *iNatStub, *iDomainName, iIapId, KProtocolInetTcp, *array );
        
    CleanupStack::PopAndDestroy( array );
    CleanupStack::PushL( turnTcpTests );
    
    TRAP( err, turnTcpTests->DoRunStunTurnTestsL() );
    CleanupStack::PopAndDestroy( turnTcpTests );
    TeardownSTUNTestL();
    
    __UHEAP_MARKEND;
    
    return err;
    }
    
    
// ---------------------------------------------------------------------------
// CTestAppConsole::RunStunUdpMediaTest
// ---------------------------------------------------------------------------
//    
TInt CTestAppConsole::RunStunUdpMediaTestL()
    {
    iAudioTestStun = ETrue;
    TInt err( KErrNone );
    RDebug::Print( _L("CTestAppConsole::RunStunUdpMediaTestL -Start\n") );
    iConsole->ClearScreen();
    iConsole->Printf( _L("\n*** STUN UDP MEDIA TEST***\n") );
    RDebug::Print( _L("\n\nSTUN UDP MEDIA TEST") );
        
    __UHEAP_MARK;
    
    TRAP( err, SetupStunUdpMediaTestL() );
    if ( KErrNone != err )
        {
        iConsole->Printf( _L("TESTSETUP FAILED: %d\n"), err );
        RDebug::Print( _L("TEST PRINT: TEST SETUP FAILED: %d\n"), err );
        return err;
        }
    iIntTest = CStunUdpMediaTest::NewL( *iConsole,
        *iNat, *iNatMediaStub, *iDomainName, iIapId, iDestAddress, iDestPort );
    
    TRAP( err, iIntTest->TestUdpAudioStreamingL() );
    RDebug::Print( _L("CTestAppConsole::RunStunUdpMediaTestL -End\n") );
    return err;
    }

    
// ---------------------------------------------------------------------------
// CTestAppConsole::StopStunUdpMediaTestL
// ---------------------------------------------------------------------------
//
void CTestAppConsole::StopStunUdpMediaTestL()
    {
    iAudioTestStun = EFalse;
    delete iIntTest;
    TeardownSTUNTestL();
    __UHEAP_MARKEND;
    RDebug::Print( _L("TestAppConsole::StopStunUdpMediaTestL\n") );
    }
    
    
// ---------------------------------------------------------------------------
// CTestAppConsole::RunIceUdpTestsL 
// ---------------------------------------------------------------------------
//
TInt CTestAppConsole::RunIceUdpTestsL()
    {
    TInt err( KErrNone );
    
    iConsole->ClearScreen();
    iConsole->Printf( _L("\n*** ICE UPD TESTS ***\n") );
    RDebug::Print( _L("\n\nICE UPD TESTS") );
    
    __UHEAP_MARK;
    
    TRAP( err, SetupICEUDPTestL() );
    if ( KErrNone != err )
        {
        iConsole->Printf( _L("TESTSETUP FAILED: %d\n"), err );
        RDebug::Print( _L("TEST PRINT: TEST SETUP FAILED: %d\n"), err );
        return err;
        }
    CICETests* iceUdpTests = CICETests::NewL( *iConsole,
        *iNat, *iNatICEStubs, *iDomainName, iIapId, iDestAddress, iDestPort, KProtocolInetUdp );
    
    CleanupStack::PushL( iceUdpTests );
    TRAP( err, iceUdpTests->DoRunIceTestsL() );
    CleanupStack::PopAndDestroy( iceUdpTests );
    TeardownICETestL();
    
    __UHEAP_MARKEND;
    
    return err;
    }


// ---------------------------------------------------------------------------
// CTestAppConsole::RunIceTcpTestsL 
// ---------------------------------------------------------------------------
//
TInt CTestAppConsole::RunIceTcpTestsL()
    {
    TInt err( KErrNone );
    
    iConsole->ClearScreen();
    iConsole->Printf( _L("\n*** ICE TCP TESTS ***\n") );
    RDebug::Print( _L("\n\nICE TCP TESTS") );
    
    __UHEAP_MARK;
    
    TRAP( err, SetupICETCPTestL() );
    if ( KErrNone != err )
        {
        iConsole->Printf( _L("TESTSETUP FAILED: %d\n"), err );
        RDebug::Print( _L("TEST PRINT: TEST SETUP FAILED: %d\n"), err );
        return err;
        }
    CICETests* iceTcpTests = CICETests::NewL( *iConsole,
        *iNat, *iNatICEStubs, *iDomainName, iIapId, iDestAddress, iDestPort, KProtocolInetTcp );
        
    CleanupStack::PushL( iceTcpTests );
    TRAP( err, iceTcpTests->DoRunIceTestsL() );
    CleanupStack::PopAndDestroy( iceTcpTests );
    TeardownICETestL();
    
    __UHEAP_MARKEND;
    
    return err;
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::FetchLocalCandidates 
// ---------------------------------------------------------------------------
//   
TInt CTestAppConsole::FetchCandidates( TUint aProtocol,
    TBool aUseRtcp )
    {
    iAudioTestICE = ETrue;
   
    iConsole->ClearScreen();
    iConsole->Printf( _L("\n*** FETCH CANDIDATES ICE ***\n") );
    
    delete iIceComTest;
    iIceComTest = NULL;
    
    iIceComTest = CNatComTest::NewL(
        *iDomainName, iIapId, *this );

    if ( KProtocolInetUdp == aProtocol )
        {
        #ifdef __WINSCW__
        RDebug::Print( _L("TEST PRINT: Create CICEUDPSettings") );
        iSettings = CICEUDPSettings::NewL( *iDomainName, iIapId);
        iSettings->EraseL( 0x0, 0x0 );
        iSettings->CreateSettingsL();
        #endif
        }
    else if ( KProtocolInetTcp == aProtocol )
        {
        #ifdef __WINSCW__
        RDebug::Print( _L("TEST PRINT: Create CICEUDPSettings") );
        iSettings = CICETCPSettings::NewL( *iDomainName, iIapId);
        iSettings->EraseL( 0x0, 0x0 );
        iSettings->CreateSettingsL();
        #endif
        }
    #ifndef __WINSCW__
    RDebug::Print( _L( "TEST PRINT: Create CAdditionalIceSettings" ) );
    iSettings = CAdditionalIceSettings::NewL( *iDomainName );
    iSettings->CreateSettingsL();
    #endif   

    return iIceComTest->StoreLocalCondidates( aProtocol, aUseRtcp );
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::StartIceComTest 
// ---------------------------------------------------------------------------
//    
TInt CTestAppConsole::StartIceComTest( TUint aProtocol,
        TNATFWIceRole aRole, TBool aUseRtcp )
    {
    iAudioTestICE = ETrue;
    
    iConsole->ClearScreen();
    iConsole->Printf( _L("\n*** NAT COM TEST ICE ***\n") );
    
    delete iIceComTest;
    iIceComTest = NULL;
    
    iIceComTest = CNatComTest::NewL(
            *iDomainName, iIapId, *this );
    
    if ( KProtocolInetUdp == aProtocol )
        {
        #ifdef __WINSCW__
        RDebug::Print( _L("TEST PRINT: Create CICEUDPSettings") );
        iSettings = CICEUDPSettings::NewL( *iDomainName, iIapId);
        iSettings->EraseL( 0x0, 0x0 );
        iSettings->CreateSettingsL();
        #endif
        }
    else if ( KProtocolInetTcp == aProtocol )
        {
        #ifdef __WINSCW__
        RDebug::Print( _L("TEST PRINT: Create CICEUDPSettings") );
        iSettings = CICETCPSettings::NewL( *iDomainName, iIapId);
        iSettings->EraseL( 0x0, 0x0 );
        iSettings->CreateSettingsL();
        #endif
        }
    #ifndef __WINSCW__
    RDebug::Print( _L( "TEST PRINT: Create CAdditionalIceSettings" ) );
    iSettings = CAdditionalIceSettings::NewL( *iDomainName );
    iSettings->CreateSettingsL();
    #endif   

    return iIceComTest->StartUsingIce( aProtocol, aRole, aUseRtcp );
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::FetchCandidate 
// ---------------------------------------------------------------------------
//
TInt CTestAppConsole::FetchCandidate( TUint aProtocol, TComTestType aType,
    TBool aUseRtcp )
    {
    iAudioTestICE = ETrue;
   
    iConsole->ClearScreen();
    iConsole->Printf( _L("\n*** FETCH CANDIDATE STUN/TURN***\n") );
    
    delete iIceComTest;
    iIceComTest = NULL;
    
    iIceComTest = CNatComTest::NewL(
            *iDomainName, iIapId, *this );
    if ( EStunComTest == aType )
        {
        RDebug::Print( _L( "TEST PRINT: Create CAdditionalStunSettings" ) );
        iSettings = CAdditionalStunUdpSettings::NewL( *iDomainName );
        iSettings->CreateSettingsL();
        }
    if ( ETurnComTest == aType )
    {
        RDebug::Print( _L( "TEST PRINT: Create CAdditionalTurnSettings" ) );
        iSettings = CAdditionalTurnUdpSettings::NewL( *iDomainName );
        iSettings->CreateSettingsL();
    }
    
    return iIceComTest->StoreLocalCandidate( aProtocol, aType, aUseRtcp );
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::StartComTest 
// ---------------------------------------------------------------------------
//
TInt CTestAppConsole::StartComTest( TUint aProtocol, TComTestType aType,
    TBool aUseRtcp )
    {
    iAudioTestICE = ETrue;
   
    iConsole->ClearScreen();
    iConsole->Printf( _L("\n*** NAT COM TEST STUN/TURN ***\n") );
    
    delete iIceComTest;
    iIceComTest = NULL;
    
    iIceComTest = CNatComTest::NewL(
            *iDomainName, iIapId, *this );
    
    if ( EStunComTest == aType )
        {
        RDebug::Print( _L( "TEST PRINT: Create CAdditionalStunSettings" ) );
        iSettings = CAdditionalStunUdpSettings::NewL( *iDomainName );
        iSettings->CreateSettingsL();
        }
    if ( ETurnComTest == aType )
        {
        RDebug::Print( _L( "TEST PRINT: Create CAdditionalTurnSettings" ) );
        iSettings = CAdditionalTurnUdpSettings::NewL( *iDomainName );
        iSettings->CreateSettingsL();
        }
    
    return iIceComTest->StartComTest( aProtocol, aType, aUseRtcp );
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::StopIceComTest 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::StopIceComTest()
    {
    iAudioTestICE = EFalse;
    
    iIceComTest->StopL();
    
    delete iSettings;
    iSettings = NULL;
    }
    
// ---------------------------------------------------------------------------
// CTestAppConsole::RunNatSettingsTest 
// ---------------------------------------------------------------------------
//
TInt CTestAppConsole::RunNatSettingsTest()
    {
    TInt err( KErrNone );
    
    iConsole->ClearScreen();
    iConsole->Printf( _L( "\n*** NAT SETTINGS TEST***\n" ) );
    RDebug::Print( _L( "\n\nNAT SETTINGS TESTS") );
    
    __UHEAP_MARK;
    
    TRAP( err, SetupSettingsTestL() );
    if ( KErrNone != err )
        {
        iConsole->Printf( _L( "TESTSETUP FAILED: %d\n" ), err );
        RDebug::Print( _L( "TEST PRINT: TEST SETUP FAILED: %d\n" ), err );
        return err;
        }
    
    iSettingsTest = CNATSettingsTest::NewL( *iDomainName, iIapId );
    
    TRAP( err, iSettingsTest->PrintSettingsL( *iConsole ) );
    TeardownSettingsTestL();
    
    __UHEAP_MARKEND;
    
    return err;
    }
    
    
// ---------------------------------------------------------------------------
// CTestAppConsole::SetupSTUNUDPTestL
// ---------------------------------------------------------------------------
//
void CTestAppConsole::SetupSTUNUDPTestL()
    {
    #ifdef __WINSCW__
    RDebug::Print( _L( "TEST PRINT: Load CStunUdpSettings\n" ) );
    iSettings = CStunUdpSettings::NewL( *iDomainName, iIapId);
    iSettings->EraseL( 0x0, 0x0 );
    iSettings->CreateSettingsL();
    #endif
    
    #ifndef __WINSCW__
    RDebug::Print( _L( "TEST PRINT: Load CAdditionalStunUdpSettings\n" ) );
    iSettings = CAdditionalStunUdpSettings::NewL( *iDomainName );
    iSettings->CreateSettingsL();
    #endif

    iNatStub = CNATConnFWTestConsoleStubs::NewL( *iConsole );
    
    // NATFW creation test
    RDebug::Print( _L( "TEST PRINT: NATFW Creation test -enter\n" ) );
    __UHEAP_MARK;
    iNat = CNATFWConnectivityApi::NewL();
    iNat->RegisterObserverForEventsL( *iNatStub,
        MNATFWConnectivityObserver::EAllEvents );
    
    iNat->UnregisterObserverForEvents( *iNatStub,
        MNATFWConnectivityObserver::EAllEvents );
    delete iNat;
    iNat = NULL;
    RDebug::Print( _L( "TEST PRINT: NATFW Creation test -exit\n" ) );
    __UHEAP_MARKEND;
    
    iNatStub = CNATConnFWTestConsoleStubs::NewL( *iConsole );
    iNat = CNATFWConnectivityApi::NewL();
    iNat->RegisterObserverForEventsL( *iNatStub,
        MNATFWConnectivityObserver::EAllEvents );
    }


// ---------------------------------------------------------------------------
// CTestAppConsole::SetupSTUNTCPTestL
// ---------------------------------------------------------------------------
//
void CTestAppConsole::SetupSTUNTCPTestL()
    {
    #ifdef __WINSCW__
    RDebug::Print( _L( "TEST PRINT: Load CStunTcpSettings\n" ) );
    iSettings = CStunTcpSettings::NewL( *iDomainName, iIapId);
    iSettings->EraseL( 0x0, 0x0 );
    iSettings->CreateSettingsL();
    #endif
    
    #ifndef __WINSCW__
    RDebug::Print( _L( "TEST PRINT: Load CAdditionalStunTcpSettings\n" ) );
    iSettings = CAdditionalStunTcpSettings::NewL( *iDomainName );
    iSettings->CreateSettingsL();
    #endif
    
    iNatStub = CNATConnFWTestConsoleStubs::NewL( *iConsole );
    iNat = CNATFWConnectivityApi::NewL();
    iNat->RegisterObserverForEventsL( *iNatStub,
        MNATFWConnectivityObserver::EAllEvents );
    }
    
    
// ---------------------------------------------------------------------------
// CTestAppConsole::TeardownSTUNUTestL
// ---------------------------------------------------------------------------
//
void CTestAppConsole::TeardownSTUNTestL()
    {
    delete iNat;
    iNat = NULL;
    delete iNatStub;
    iNatStub = NULL;
    
    delete iSettings;
    iSettings = NULL;
    }
    
    
// ---------------------------------------------------------------------------
// CTestAppConsole::SetupSettingsTestL
// ---------------------------------------------------------------------------
//
void CTestAppConsole::SetupSettingsTestL()
    {
    #ifdef __WINSCW__
    RDebug::Print( _L( "TEST PRINT: Load CGeneralSettings" ) );
    iSettings = CGeneralSettings::NewL( *iDomainName, iIapId);
    iSettings->EraseL( 0x0, 0x0 );
    iSettings->CreateSettingsL();
    #endif
    }
    

// ---------------------------------------------------------------------------
// CTestAppConsole::TeardownSettingsTestL
// ---------------------------------------------------------------------------
//
void CTestAppConsole::TeardownSettingsTestL()
    {
    delete iSettingsTest;
    iSettingsTest = NULL;

    delete iSettings;
    iSettings = NULL;
    }
    
        
// ---------------------------------------------------------------------------
// CTestAppConsole::SetupStunUdpMediaTestL
// ---------------------------------------------------------------------------
//
void CTestAppConsole::SetupStunUdpMediaTestL()
    {
    #ifdef __WINSCW__
    RDebug::Print( _L( "TEST PRINT: Create CStunUdpSettings" ) );
    iSettings = CStunUdpSettings::NewL( *iDomainName, iIapId);
    iSettings->EraseL( 0x0, 0x0 );
    iSettings->CreateSettingsL();
    #endif
    
    #ifndef __WINSCW__
    RDebug::Print( _L( "TEST PRINT: Create CAdditionalStunUdpSettings" ) );
    iSettings = CAdditionalStunUdpSettings::NewL( *iDomainName );
    iSettings->CreateSettingsL();
    #endif
    
    iNatMediaStub = CMediaTestStubs::NewL( *iConsole );
    iNat = CNATFWConnectivityApi::NewL();
    iNat->RegisterObserverForEventsL( *iNatMediaStub,
        MNATFWConnectivityObserver::EAllEvents );
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::SetupICEUDPTestL 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::SetupICEUDPTestL()
    {
    #ifdef __WINSCW__
    RDebug::Print( _L("TEST PRINT: Create CICEUDPSettings") );
    iSettings = CICEUDPSettings::NewL( *iDomainName, iIapId);
    iSettings->EraseL( 0x0, 0x0 );
    iSettings->CreateSettingsL();
    #endif
    
    #ifndef __WINSCW__
    RDebug::Print( _L( "TEST PRINT: Create CAdditionalIceSettings" ) );
    iSettings = CAdditionalIceSettings::NewL( *iDomainName );
    iSettings->CreateSettingsL();
    #endif
    
    iNatICEStubs = CICETestStubs::NewL( *iConsole );
    iNat = CNATFWConnectivityApi::NewL();
    iNat->RegisterObserverForEventsL( *iNatICEStubs,
        MNATFWConnectivityObserver::EAllEvents );
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::SetupICETCPTestL 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::SetupICETCPTestL()
    {
    #ifdef __WINSCW__
    RDebug::Print( _L("TEST PRINT: Create CICETCPSettings") );
    iSettings = CICETCPSettings::NewL( *iDomainName, iIapId);
    iSettings->EraseL( 0x0, 0x0 );
    iSettings->CreateSettingsL();
    #endif
    
    #ifndef __WINSCW__
    RDebug::Print( _L( "TEST PRINT: Create CAdditionalIceSettings" ) );
    iSettings = CAdditionalIceSettings::NewL( *iDomainName );
    iSettings->CreateSettingsL();
    #endif
    
    iNatICEStubs = CICETestStubs::NewL( *iConsole );
    iNat = CNATFWConnectivityApi::NewL();
    iNat->RegisterObserverForEventsL( *iNatICEStubs,
        MNATFWConnectivityObserver::EAllEvents );
    }

  
// ---------------------------------------------------------------------------
// CTestAppConsole::TeardownICETestL
// ---------------------------------------------------------------------------
//
void CTestAppConsole::TeardownICETestL()
    {
    delete iNat;
    iNat = NULL;
    delete iNatICEStubs;
    iNatICEStubs = NULL;
    
    delete iSettings;
    iSettings = NULL;
    }
        

// ---------------------------------------------------------------------------
// CTestAppConsole::NATTurnUdpPreambleL
// ---------------------------------------------------------------------------
//
void CTestAppConsole::NATTurnUdpPreambleL()
    {
    #ifdef __WINSCW__
    RDebug::Print( _L( "TEST PRINT: Create CTurnUdpSettings" ) );
    iSettings = CTurnUdpSettings::NewL( *iDomainName, iIapId);
    iSettings->EraseL( 0x0, 0x0 );
    iSettings->CreateSettingsL();    
    
    #else
    RDebug::Print( _L( "TEST PRINT: Create CAdditionalTurnUdpSettings" ) );
    iSettings = CAdditionalTurnUdpSettings::NewL( *iDomainName );
    iSettings->CreateSettingsL();
    #endif
    
    iNatStub = CNATConnFWTestConsoleStubs::NewL( *iConsole );
    iNat = CNATFWConnectivityApi::NewL();
    iNat->RegisterObserverForEventsL( *iNatStub,
        MNATFWConnectivityObserver::EAllEvents );
    }


// ---------------------------------------------------------------------------
// CTestAppConsole::NATTurnTcpPreambleL
// ---------------------------------------------------------------------------
//
void CTestAppConsole::NATTurnTcpPreambleL()
    {
    #ifdef __WINSCW__
    RDebug::Print( _L( "TEST PRINT: Create CTurnTcpSettings" ) );
    iSettings = CTurnTcpSettings::NewL( *iDomainName, iIapId);
    iSettings->EraseL( 0x0, 0x0 );
    iSettings->CreateSettingsL();    
    
    #else
    RDebug::Print( _L( "TEST PRINT: Create CAdditionalTurnTcpSettings" ) );
    iSettings = CAdditionalTurnTcpSettings::NewL( *iDomainName );
    iSettings->CreateSettingsL();
    #endif
    
    iNatStub = CNATConnFWTestConsoleStubs::NewL( *iConsole );
    iNat = CNATFWConnectivityApi::NewL();
    iNat->RegisterObserverForEventsL( *iNatStub,
        MNATFWConnectivityObserver::EAllEvents );
    }


// ---------------------------------------------------------------------------
// CTestAppConsole::PrintLocalAddress
// ---------------------------------------------------------------------------
//
TInt CTestAppConsole::PrintLocalAddress()
    {
    TInt err( KErrNone );
    __UHEAP_MARK;
    TRAP( err, SetupStunUdpMediaTestL() );
    if ( KErrNone != err )
        {
        iConsole->Printf( _L( "TESTSETUP FAILED: %d\n" ), err );
        RDebug::Print( _L( "TEST PRINT: TEST SETUP FAILED: %d" ), err );
        return err;
        }
    iIntTest = CStunUdpMediaTest::NewL( *iConsole,
        *iNat, *iNatMediaStub, *iDomainName, iIapId, iDestAddress, iDestPort );
    
    TRAP( err, iIntTest->PrintLocalAddressL() );
    
    delete iIntTest;
    TeardownSTUNTestL();
    __UHEAP_MARKEND;
    return err;
    }


// ---------------------------------------------------------------------------
// CTestAppConsole::SetLocalToDestinationAddrL
// ---------------------------------------------------------------------------
//    
TInt CTestAppConsole::SetLocalToDestinationAddrL()
    {
     TInt err( KErrNone );
     RDebug::Print( _L("TEST PRINT: CTestAppConsole::SetLocalToDestinationAddrL -Start\n") );
     
    __UHEAP_MARK;
    
    TRAP( err, SetupStunUdpMediaTestL() );
    if ( KErrNone != err )
        {
        iConsole->Printf( _L( "TESTSETUP FAILED: %d\n" ), err );
        RDebug::Print( _L( "TEST PRINT: TEST SETUP FAILED: %d" ), err );
        return err;
        }
    iIntTest = CStunUdpMediaTest::NewL( *iConsole,
        *iNat, *iNatMediaStub, *iDomainName, iIapId, iDestAddress, iDestPort );
    
    TRAP( err, iIntTest->PrintLocalAddressL() );
    if ( err )
        {
        return err;
        }
    
    TInetAddr localAddr;
    err = iIntTest->GetLocalAddress( localAddr );
    if( !err )
        {
        iDestAddress = localAddr.Address();
        iDestPort = localAddr.Port();
        }
    
    delete iIntTest;
    TeardownSTUNTestL();
    
    __UHEAP_MARKEND;
    
    RDebug::Print( _L("TEST PRINT: CTestAppConsole::SetLocalToDestinationAddrL -End\n") );
    return err;
    }


// ---------------------------------------------------------------------------
// CTestAppConsole::CloseMediaStreamL
// ---------------------------------------------------------------------------
// 	
void CTestAppConsole::CloseMediaStreamL()
    {
    iAudioTestStun = EFalse;
    delete iIntTest;
    TeardownSTUNTestL();
    __UHEAP_MARKEND;
    }


// ---------------------------------------------------------------------------
// CICETests::NewCandidatePairFound
// From MNatComTestObserver
// ---------------------------------------------------------------------------
//
void CTestAppConsole::Notify( TNatComTestEvent aEvent, TInt aError )
    {
    TBuf16<25> notifyDesc;
    
    switch( aEvent )
        {
        case ESessionCreated:
            iConsole->Printf( _L("SessionCreated err: %d\n"), aError );
            notifyDesc.Copy( _L16( "ESessionCreated" ) ); 
            break;
        case EStreamCreated:
            iConsole->Printf( _L("StreamCreated err: %d\n"), aError );
            notifyDesc.Copy( _L16( "EStreamCreated" ) );
            break;
        case ESetIdentification:
            iConsole->Printf( _L("SetIdentification err: %d\n"), aError );
            notifyDesc.Copy( _L16( "ESetIdentification" ) );
            break;
        case ELocalCandidateFound:
            iConsole->Printf( _L("LocalCandidateFound err: %d\n"), aError );
            notifyDesc.Copy( _L16( "ELocalCandidateFound" ) );
            break;
        case ESetOperationMode:
            iConsole->Printf( _L("SetOperationMode err: %d\n"), aError );
            notifyDesc.Copy( _L16( "ESetOperationMode" ) );
            break;
        case ENewCandPairFound:
            iConsole->Printf( _L("NewCandPairFound err: %d\n"), aError );
            notifyDesc.Copy( _L16( "ENewCandPairFound" ) );
            break;
        case EConnChecksCompleted:
            iConsole->Printf( _L("EConnChecksCompleted err: %d\n"), aError );
            notifyDesc.Copy( _L16( "EConnChecksCompleted" ) );
            break;
        case ERunning:
            iConsole->Printf( _L("ERunning err: %d\n"), aError );
            notifyDesc.Copy( _L16( "ERunning" ) );
            break;
        case ETestError:
            iConsole->Printf( _L("Error err: %d\n"), aError );
            notifyDesc.Copy( _L16( "EError" ) );
            break;
        case ETestEnd:
            iConsole->Printf( _L("TestEnd err: %d\n"), aError );
            notifyDesc.Copy( _L16( "ETestEnd" ) );
            StopIceComTest();
            break;
        default:
            break;     
        }
        RDebug::Print( _L( "CTestAppConsole::Notify: Event: %S Error: %d\n" ),
            &notifyDesc, aError );
    }


// ---------------------------------------------------------------------------
// CICETests::LocalCandidates
// From MNatComTestObserver
// ---------------------------------------------------------------------------
//    
TInt CTestAppConsole::LocalCandidates(
    const RPointerArray<CNATFWCandidate>& aLocalCandidates )
    {
    //write to memorycard
    RDebug::Print(
        _L( "CTestAppConsole::LocalCandidates - write %d local candidates" ),
        aLocalCandidates.Count() );
        
    CNATCandidateStore* store = CNATCandidateStore::NewLC();
     
    TRAPD( err, store->WriteL( aLocalCandidates ) );
    CleanupStack::PopAndDestroy( store );
    return err;
    }


// ---------------------------------------------------------------------------
// CICETests::RemoteCandidates
// From MNatComTestObserver
// ---------------------------------------------------------------------------
//    
TInt CTestAppConsole::RemoteCandidates(
    RPointerArray<CNATFWCandidate>& aRemoteCandidates )
    {
    //Read from memorycard
    RDebug::Print(
        _L( "CTestAppConsole::RemoteCandidates -read remote candidates" ) );
        
    CNATCandidateStore* store = CNATCandidateStore::NewLC();

    TRAPD( err, store->ReadL( aRemoteCandidates ) );
    CleanupStack::PopAndDestroy( store );
    
    RDebug::Print(
        _L( "CTestAppConsole::RemoteCandidates: %d remote candidates readed\n" ),
        aRemoteCandidates.Count() );
        
    return err;
    }


// ---------------------------------------------------------------------------
// CICETests::DoPause
// From MNatComTestObserver
// ---------------------------------------------------------------------------
// 
void CTestAppConsole::DoPause( TRequestStatus& aStatus )
    { 
    RDebug::Print( _L( "CTestAppConsole::DoPause" ) );
    
    aStatus = KRequestPending;    

    iConsole->Printf( _L( "Press any key to continue...\n" ) );
    
    iListenerStatus = &aStatus;
    iPause = ETrue;
    }
    


// ---------------------------------------------------------------------------
// CTestAppConsole::PrintStoredCandidates
// ---------------------------------------------------------------------------
//  
TInt CTestAppConsole::PrintStoredCandidates()
    {
    RDebug::Print(
            _L( "CTestAppConsole::PrintStoredCandidates" ) );
    //Read from memorycard
    iConsole->ClearScreen();
    iConsole->Printf( _L( "\n*** STORED CANDIDATES ***\n" ) );

    RPointerArray<CNATFWCandidate> candidates;
    
    CNATCandidateStore* store = CNATCandidateStore::NewLC();
    TRAPD( err, store->ReadL( candidates ) );
    CleanupStack::PopAndDestroy( store );
    
    TInt count =  candidates.Count();
    RDebug::Print(
        _L( "CTestAppConsole::RemoteCandidates: %d remote candidates readed\n" ),
        count );
    iConsole->Printf(_L( "\n%d remote candidates readed\n" ), count );
    for ( TInt i( 0 ); i < count ;i++  )
        {
        TBuf<40> buffer;
        TInetAddr inetAddr;
        
        RDebug::Print( _L( "Candidate %d\n" ), i + 1 );
        iConsole->Printf( _L( "\nCandidate %d\n" ), i + 1 );
        // Base address
        inetAddr = candidates[i]->Base();
        inetAddr.Output( buffer );
        iConsole->Printf( _L( "Base = %S:%d\n" ), &buffer, inetAddr.Port() );
        RDebug::Print( _L( "Base = %S:%d\n" ), &buffer, inetAddr.Port() );
        // Transport address
        inetAddr = candidates[i]->TransportAddr();
        inetAddr.Output( buffer );
        iConsole->Printf( _L( "Transport addr = %S:%d\n" ), &buffer, inetAddr.Port() );
        RDebug::Print( _L( "Transport addr = %S:%d\n" ), &buffer, inetAddr.Port() );
        }
    candidates.ResetAndDestroy();
    return err;
    }


// ---------------------------------------------------------------------------
// CICETests::CandidatePairs
// From MNatComTestObserver
// ---------------------------------------------------------------------------
//      
TInt CTestAppConsole::CandidatePairs(
    const RPointerArray<CNATFWCandidatePair>& aCandidatePairs )
    {
    RDebug::Print( _L( "CTestAppConsole::CandidatePairs" ) );
    TInt count = aCandidatePairs.Count();
    
    for ( TInt i = 0; i < count; i++ )
        {
        TBuf<40> buffer;
        TInetAddr inetAddr;
        iConsole->Printf( _L( "\nCandidate pair %d\n" ), i + 1 );
        RDebug::Print( _L( "\nCandidate pair %d\n" ), i + 1 );
        // Local candidate
        iConsole->Printf( _L( "Local candidate:\n" ) );
        RDebug::Print( _L( "Local candidate:\n" ) );
        inetAddr = aCandidatePairs[i]->LocalCandidate().Base();
        inetAddr.Output( buffer );
        iConsole->Printf( _L( "Base = %S:%d\n" ), &buffer, inetAddr.Port() );
        RDebug::Print( _L( "Base = %S:%d\n" ), &buffer, inetAddr.Port() );
        inetAddr = aCandidatePairs[i]->LocalCandidate().TransportAddr();
        inetAddr.Output( buffer );
        iConsole->Printf( _L( "Transport addr = %S:%d\n" ), &buffer, inetAddr.Port() );
        RDebug::Print( _L( "Transport addr = %S:%d\n" ), &buffer, inetAddr.Port() );
        // Remote candidate
        iConsole->Printf( _L( "Remote candidate:\n" ) );
        RDebug::Print( _L( "Remote candidate:\n" ) );
        inetAddr = aCandidatePairs[i]->RemoteCandidate().Base();
        inetAddr.Output( buffer );
        iConsole->Printf( _L( "Base = %S:%d\n" ), &buffer, inetAddr.Port() );
        RDebug::Print( _L( "Base = %S:%d\n" ), &buffer, inetAddr.Port() );
        inetAddr = aCandidatePairs[i]->RemoteCandidate().TransportAddr();
        inetAddr.Output( buffer );
        iConsole->Printf( _L( "Transport addr = %S:%d\n" ), &buffer, inetAddr.Port() );
        RDebug::Print( _L( "Transport addr = %S:%d\n" ), &buffer, inetAddr.Port() );
        }
    return KErrNone;
    }
