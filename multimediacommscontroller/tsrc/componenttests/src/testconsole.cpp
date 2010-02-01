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
#include "testconsole.h"
#include "testdefinitions.hrh"
#include "mccteststubs.h"
#include "test_interface.h"
#include "test_audiocontrol.h"
#include "test_dtmf.h"
#include "test_rtcp.h"
#include "test_async_features.h"
#include "test_secure.h"


#ifndef __WINSCW__
const TInt KTestIapId = 1;
#else
const TInt KTestIapId = 6;
#endif

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
    : CActive( EPriorityStandard )
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
    ResetNetworkSettings();
    
	iConsole = Console::NewL( KMainTitle, TSize(KConsFullScreen, KConsFullScreen) );
	this->DisplayConsoleMenuL( KTxtMainInstructions );
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
    delete iMccInterface;
    delete iMccInterfaceStub;
    if ( iFeatTest )
        {
        delete iFeatTest;
        }
    }

//*******************************************************************************
// Method      : CTestAppConsole::ResetNetworkSettings()
// Purpose     : 
// Parameters  : 
// Return Value: 
//*******************************************************************************
void CTestAppConsole::ResetNetworkSettings()
    {
    iNetsettings.iRemoteAddress.SetAddress( KInetAddrLoop );
    iNetsettings.iRemoteAddress.SetPort( KDefaultRtpPort );
    iNetsettings.iRemoteRtcpPort = KDefaultRtcpPort;
    iNetsettings.iLocalAddress.SetAddress( KInetAddrAny );
    iNetsettings.iLocalAddress.SetPort( KDefaultRtpPort );
    iNetsettings.iLocalPublicAddress.SetAddress( KInetAddrAny );
    iNetsettings.iLocalRtcpPort = KDefaultRtcpPort;
    iNetsettings.iLocalPublicRtcpPort = KDefaultRtcpPort;
    iNetsettings.iMediaQosValue = KDefaultMediaQos;
    iNetsettings.iIapId = KTestIapId;
    };

//*******************************************************************************
// Method      : CTestAppConsole::QueryNetworkSettings()
// Purpose     : 
// Parameters  : 
// Return Value: 
//*******************************************************************************
void CTestAppConsole::QueryNetworkSettings()
    {
    TBuf16<80> line;
    
    iConsole->Printf( _L("\nQUERYING NETWORK SETTINGS") );
    iConsole->Printf( _L("\nPress enter to use existing value") );
    
    TUint iap( KTestIapId );
    TInt inputErr( 0 );
    do 
        {
        iConsole->Printf( _L("\nINPUT IAPID: ") );
    	GetStringFromConsole( line );
    	if ( line.Length() != 0 )
    	    {
        	TLex lex( line );
        	inputErr = lex.Val( iap );    	    
    	    }
        } while ( inputErr );
    
    iNetsettings.iIapId = iap;
    }
    
//*******************************************************************************
// Method      : CTestAppConsole::QueryIPAddress()
// Purpose     : 
// Parameters  : 
// Return Value: 
//*******************************************************************************
void CTestAppConsole::QueryIPAddress()
    {    
    TBuf16<80> line;
    
    // Query IP address and parse it
    TUint32 address;
    RArray<TUint32> values;
    TInt inputErr( 0 );
    
    iConsole->Printf( _L("\nINPUT IP (use dot as a separator): ") );
    iConsole->Printf( _L("\nPress enter if IP not needed in tests\n\n") );
    GetStringFromConsole( line );
    	
    if ( line.Length() != 0 )
        {
        TInt dotIndex( 0 );
        TBool dotExist = ETrue;

        while ( dotExist )
            {
            dotIndex = line.Locate( '.' );

            // True if last attribute value
            if( KErrNotFound == dotIndex  )
                {
                dotExist = EFalse;
                dotIndex = line.Length();
                }

            TUint8 number;
            TLex16 lex = line.Mid( 0, dotIndex );

            inputErr = lex.Val( number, EDecimal );
                
            if ( !inputErr )
                {
                values.AppendL( number );
                }
                
            if( dotExist )
                {
                line.Delete( 0, dotIndex + 1 );
                }
            }
            
        if ( !inputErr && values.Count() == 4 )
            {
            address = INET_ADDR( values[0], values[1], values[2], values[3] );
            iNetsettings.iRemoteAddress.SetAddress( address );    
            }
	    }
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
    ProcessMainInput();
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

void CTestAppConsole::DisplayConsoleMenuL( const TDesC &aInstructions )
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
    TBuf<80> line;
	GetStringFromConsole(line);
	
	if ( line.Length() > 0 )
	    {
		TChar inputChar = line[0];
		
		TInt err( KErrNone );
		TBool testsRun( ETrue );
		switch( inputChar )
			{
			case '1':
                err = RunInterfaceTests();
			    break;
			case '2':
			    err = RunAudioControlTests();
			    break;
			case '3':
			    err = RunRtcpTests();
				break;
			case '4':
			    err = RunDtmfTests();
				break;
			case '5':
			    err = RunAsyncFeatureTests();
			    break;
		    case 's':
			    err = RunSecureTests();
			    break;
			case '0':
				testsRun = EFalse;
				CActiveScheduler::Stop();
				break;
			default:
			    testsRun = EFalse;
				DisplayMsg( KTxtWrongOption );
				break;		
			}
	    
        if ( err )
            {
            iConsole->Printf( _L("Some tests failed: %d, check logs\n"), err );
            RDebug::Print( _L("\nSome tests failed: %d, check logs\n"), err );
            }
        else if ( testsRun )
            {
            iConsole->Printf( _L( "ALL TESTS OK\n" ) );
            RDebug::Print( _L( "\nAll tests OK\n" ) );
            }
        else
            {
            
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
void CTestAppConsole::DisplayMsg(const TDesC &aMsg)
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
        if(input == EKeyBackspace  ||  input == EKeyDelete)
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
    while(aAddr.Length() < aAddr.MaxLength()  &&  input != EKeyEnter  &&  input != EKeyEscape);

    DisplayMsg( KTxtLineBreak );
    return input;
    }

//*******************************************************************************
// Method      : CTestAppConsole::RunAsyncFeatureTests()
// Purpose     : Test the async Mcc Interface functions
// Parameters  : 
// Return Value: System wide error code
//*******************************************************************************
//
TInt CTestAppConsole::RunAsyncFeatureTests()
    {
    QueryNetworkSettings();
    TRAPD( err, MccInterfacePreambleL() );
    
    if ( err )
        {
        iConsole->Printf( _L("TESTSETUP FAILED: %d\n"), err );
        RDebug::Print( _L("TESTSETUP FAILED: %d\n"), err );
        return err;
        }
    
    iFeatTest = CTestAsyncFeat::NewL( *iConsole, iNetsettings,
        *iMccInterface, *iMccInterfaceStub );
    TRAP( err, iFeatTest->DoRunAsyncFeatureTestsL() );
    
    //MccInterfacePostamble();
    return err;
    }

//*******************************************************************************
// Method      : CTestAppConsole::RunInterfaceTests()
// Purpose     : Test the Mcc Interface functions
// Parameters  : 
// Return Value: System wide error code
//*******************************************************************************
//
TInt CTestAppConsole::RunInterfaceTests()
    {
    TInt err( KErrNone );
    
    QueryNetworkSettings();
    QueryIPAddress();
    
    TRAP( err, MccInterfacePreambleL() );
    if ( KErrNone != err )
        {
        iConsole->Printf( _L("TESTSETUP FAILED: %d\n"), err );
        RDebug::Print( _L("TESTSETUP FAILED: %d\n"), err );
        return err;
        }
    
    CTestInterface* intTest = CTestInterface::NewL( *iConsole, iNetsettings,
        *iMccInterface, *iMccInterfaceStub );
    CleanupStack::PushL( intTest );
    
    TRAP( err, intTest->DoRunInterfaceTestsL() );

    CleanupStack::PopAndDestroy( intTest );
    MccInterfacePostamble();
    return err;
    }

//*******************************************************************************
// Method      : CTestAppConsole::RunAudioControlTests()
// Purpose     : Test the Mcc audio control functions
// Parameters  : 
// Return Value: System wide error code
//*******************************************************************************
//
TInt CTestAppConsole::RunAudioControlTests()
    {
    QueryNetworkSettings();
    TInt err( KErrNone );
    TRAP( err, MccInterfacePreambleL() );
    if ( KErrNone != err )
        {
        iConsole->Printf( _L("TESTSETUP FAILED: %d\n"), err );
        RDebug::Print( _L("TESTSETUP FAILED: %d\n"), err );
        return err;
        }
    
    CTestAudioControl* audio = CTestAudioControl::NewL( *iConsole, iNetsettings,
        *iMccInterface, *iMccInterfaceStub );
    CleanupStack::PushL( audio );
    
    TRAP( err, audio->DoRunAudioControlTestsL() );
    
    CleanupStack::PopAndDestroy( audio );
    MccInterfacePostamble();
    return err;
    }


//*******************************************************************************
// Method      : CTestAppConsole::RunDtmfTests()
// Purpose     : Test the Mcc RTCP sending
// Parameters  : 
// Return Value: System wide error code
//*******************************************************************************
//
TInt CTestAppConsole::RunDtmfTests()
    {
    TInt err( KErrNone );
    
    QueryNetworkSettings();
    
    CTestDtmf* dtmf = CTestDtmf::NewL( *iConsole, iNetsettings );
    CleanupStack::PushL( dtmf );
    
    TRAP( err, dtmf->DoRunDtmfTestsL() );

    CleanupStack::PopAndDestroy( dtmf );
    return err;
    }


//*******************************************************************************
// Method      : CTestAppConsole::RunRtcpTests()
// Purpose     : Test the Mcc DTMF features
// Parameters  : 
// Return Value: System wide error code
//*******************************************************************************
//
TInt CTestAppConsole::RunRtcpTests()
    {
    TInt err( KErrNone );
    
    QueryNetworkSettings();
    
    CTestRtcp* rtcp = CTestRtcp::NewL( *iConsole, iNetsettings );
    CleanupStack::PushL( rtcp );
    
    TRAP( err, rtcp->DoRunRtcpTestsL() );

    CleanupStack::PopAndDestroy( rtcp );
    return err;
    }

// ---------------------------------------------------------------------------
// CTestAppConsole::MccInterfacePreambleL
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::MccInterfacePreambleL()
    {
    iMccInterfaceStub = CMccInterfaceStub::NewL();
    iMccInterface = CMccInterface::NewL( *iMccInterfaceStub );
    };

// ---------------------------------------------------------------------------
// CTestAppConsole::MccInterfacePostamble
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::MccInterfacePostamble()
    {
    delete iMccInterface;
    iMccInterface = NULL;
    delete iMccInterfaceStub;
    iMccInterfaceStub = NULL;
    }

// ---------------------------------------------------------------------------
// 
// 
// ---------------------------------------------------------------------------
//
TInt CTestAppConsole::RunSecureTests()
    {
    TInt err( KErrNone );
    
    QueryNetworkSettings();
    
    TRAP( err, MccInterfacePreambleL() );
    if ( KErrNone != err )
        {
        iConsole->Printf( _L("TESTSETUP FAILED: %d\n"), err );
        RDebug::Print( _L("TESTSETUP FAILED: %d\n"), err );
        return err;
        }
    
    CTestSecure* secTest = CTestSecure::NewL( *iConsole, iNetsettings,
        *iMccInterface, *iMccInterfaceStub );
    CleanupStack::PushL( secTest );
    
    TRAP( err, secTest->DoRunSecureTestsL() );

    CleanupStack::Pop( secTest );
    MccInterfacePostamble();

    return err;
    }

