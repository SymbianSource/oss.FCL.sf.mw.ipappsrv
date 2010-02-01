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
#include "muxtestconsole.h"
#include "testdefinitions.hrh"
#include "muxtestconsolestubs.h"
#include "multiplexerudptests.h"
#include "multiplexertcptests.h"



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
    delete iMux;
    delete iMuxStub;
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
                err = RunMuxUdpTests();
			    break;
		    case '2':
                err = RunMuxTcpTests();
			    break;
			case '3':
			    RunUdpAudioStreaming();
			    break;
			case '4':
			    StopUdpAudioStreaming();
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
			
        if ( iAudioTest )
            {
            iConsole->Printf( _L( "Audio test ongoing, Press 4 to stop\n" ) );
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
                iConsole->Printf( _L( "ALL TESTS OK\n" ) );
                RDebug::Print( _L( "\nAll tests OK\n" ) );
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
// Method      : CTestAppConsole::RunInterfaceTests()
// Purpose     : Test the Mcc Interface functions
// Parameters  : 
// Return Value: System wide error code
//*******************************************************************************
//
TInt CTestAppConsole::RunMuxUdpTests()
    {
    TInt err( KErrNone );
    
    TRAP( err, MccInterfacePreambleUdpL() );
    if ( KErrNone != err )
        {
        iConsole->Printf( _L("TESTSETUP FAILED: %d\n"), err );
        RDebug::Print( _L("TESTSETUP FAILED: %d\n"), err );
        return err;
        }
    
    CMultiplexerUdpTests* intTest = CMultiplexerUdpTests::NewL( *iConsole,
        *iMux, *iMuxStub );
    CleanupStack::PushL( intTest );
    
    
    TRAP( err, intTest->DoRunUdpMuxtTests() );

    CleanupStack::PopAndDestroy( intTest );
    MccInterfacePostamble();
    return err;
    }

//*******************************************************************************
// Method      : CTestAppConsole::RunInterfaceTests()
// Purpose     : Test the Mcc Interface functions
// Parameters  : 
// Return Value: System wide error code
//*******************************************************************************
//
TInt CTestAppConsole::RunMuxTcpTests()
    {
    TInt err( KErrNone );
    
    TRAP( err, MccInterfacePreambleTcpL() );
    if ( KErrNone != err )
        {
        iConsole->Printf( _L("TESTSETUP FAILED: %d\n"), err );
        RDebug::Print( _L("TESTSETUP FAILED: %d\n"), err );
        return err;
        }
        
    CMultiplexerTcpTests* intTest = CMultiplexerTcpTests::NewL( *iConsole,
        *iMux, *iMuxStub );
    CleanupStack::PushL( intTest );
    
    
    TRAP( err, intTest->DoRunTcpMuxTestsL() );

    CleanupStack::PopAndDestroy( intTest );
    MccInterfacePostamble();
    return err;
    }

//*******************************************************************************
// Method      : CTestAppConsole::RunUdpAudioStreaming()
//*******************************************************************************
//
void CTestAppConsole::RunUdpAudioStreaming()
    {
    iAudioTest = ETrue;
    
    TInt err( KErrNone );
    __UHEAP_MARK;
    
    TRAP( err, MccInterfacePreambleUdpL() );
    
    iIntTest = CMultiplexerUdpTests::NewL( *iConsole,
        *iMux, *iMuxStub );
    
    
    TRAP( err, iIntTest->TestUdpAudioStreamingL() );

    //MccInterfacePostamble();
    //return err;
    }
 
 //*******************************************************************************
// Method      : CTestAppConsole::StopUdpAudioStreaming()
//*******************************************************************************
//
void CTestAppConsole::StopUdpAudioStreaming()
    {
    iAudioTest = EFalse;
    delete iIntTest;
    MccInterfacePostamble();
    __UHEAP_MARKEND;
    }
    
// ---------------------------------------------------------------------------
// CTestAppConsole::MccInterfacePreambleL
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::MccInterfacePreambleUdpL()
    {
    iMuxStub = CMuxTestConsoleStubs::NewL();
    __UHEAP_MARK;
    iMux = CNcmConnectionMultiplexer::NewL( *iMuxStub );
    };

 // ---------------------------------------------------------------------------
// CTestAppConsole::MccInterfacePreambleL
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::MccInterfacePreambleTcpL()
    {
    iMuxStub = CMuxTestConsoleStubs::NewL();
    __UHEAP_MARK;
    iMux = CNcmConnectionMultiplexer::NewL( *iMuxStub );
    };
 
// ---------------------------------------------------------------------------
// CTestAppConsole::MccInterfacePostamble
// 
// ---------------------------------------------------------------------------
//
void CTestAppConsole::MccInterfacePostamble()
    {
    
    delete iMux;
    iMux = NULL;
    __UHEAP_MARKEND;
    
    delete iMuxStub;
    iMuxStub = NULL;
    }

