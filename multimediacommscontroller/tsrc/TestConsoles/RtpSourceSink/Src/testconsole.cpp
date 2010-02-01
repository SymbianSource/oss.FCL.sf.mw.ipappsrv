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
#include <mmfcontroller.h>

#include "TestConsole.h"
#include "FileUtil.h"
#include "mccuids.hrh"

const TUid KControllerUID =  { KImplUidMccController };
const TUid KSourceUID =      { KImplUidRtpDataSource };
const TUid KSinkUID =        { KImplUidRtpDataSink   };

// Main Instruction Text
_LIT(KTxtMainInstructions, "Please select one option:\n"
 L"1. Create instances from MccRtpDataSource and -Sink\n"
 L"9. Quit\n----------------------------");

//*******************************************************************************
// Method      : CTestAppConsole::NewL()
// Purpose     : 
// Parameters  : 
// Return Value: 
//*******************************************************************************
CTestAppConsole* CTestAppConsole::NewL( )
    {
	CTestAppConsole* self = new(ELeave)CTestAppConsole();
    CleanupStack::PushL(self);

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
CTestAppConsole::CTestAppConsole( ) : CActive(EPriorityStandard)
    {
	CActiveScheduler::Add(this);
    }

//*******************************************************************************
// Method      : CTestAppConsole::ConstructL()
// Purpose     : 
// Parameters  : 
// Return Value: 
//*******************************************************************************
void CTestAppConsole::ConstructL()
    {
    _LIT(KTxtTitle, " RTP Test ");
	iConsole = Console::NewL(KTxtTitle, TSize(KConsFullScreen, KConsFullScreen));
	DisplayConsoleMenu(KTxtMainInstructions);

	// Create Log files
	TFileUtil::InitLogFile(KLogFileTest);	// test events
	TFileUtil::InitLogFile(KLogFileRtp);	// Rtp packets
	TFileUtil::InitLogFile(KLogFileRtcp);	// Rtcp packets
	TFileUtil::InitLogFile(KLogFileStat);	// Statistic
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
   
	if (iConsole)
	    {
		delete iConsole;
		}
		
	delete iInstruct;
    }

//*******************************************************************************
// Method      : CRtpAppConsole::StartTesting()
// Purpose     : start this AO 
// Parameters  : 
// Return Value: 
//*******************************************************************************
void CTestAppConsole::StartTesting()
    {
	DoRead();
    }

//*******************************************************************************
// Method      : CRtpAppConsole::DoRead()
// Purpose     : get the user's option and send request to scheduler
// Parameters  : 
// Return Value: 
//*******************************************************************************
void CTestAppConsole::DoRead()
    {
	iConsole->Read(iStatus);
	SetActive();
    }

//*******************************************************************************
// Method      : CRtpAppConsole::RunL()
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
// Method      : CRtpAppConsole::DoCancel()
// Purpose     : 
// Parameters  : 
// Return Value: 
//*******************************************************************************
void CTestAppConsole::DoCancel()
    {
	iConsole->ReadCancel();
    }


//*******************************************************************************
// Method      : CRtpAppConsole::DisplayConsoleMenu()
// Purpose     : Display main or sub console menus for different test cases
// Parameters  : TDesc &aInstructions
// Return Value: void
//*******************************************************************************

void CTestAppConsole::DisplayConsoleMenu(const TDesC &aInstructions)
	{
	if (iInstruct)
	    {
	    delete iInstruct;
	    iInstruct = NULL;
	    }
	iInstruct = aInstructions.AllocL();
	iConsole->ClearScreen();
	iConsole->Write(*iInstruct);
	}

//*******************************************************************************
// Method      : CTestAppConsole::ProcessMainInput()
// Purpose     : Obtain user's option and decide which test case to run next.
// Parameters  : 
// Return Value: 
//*******************************************************************************
void CTestAppConsole::ProcessMainInput()
	{
	TChar inputChar = iConsole->KeyCode();

	switch(inputChar)
        {
        case '1':
            RunTest1();
            break;
        case '2':
            //RunTest2();
            break;
        case '9':
            CActiveScheduler::Stop();
			Stop();
            break;
        default:
			_LIT(KTxtWrongOption, "Wrong Option! Try Again.");
            DisplayMsg(KTxtWrongOption);
            break;		
        }

	// Ready to get next input option.
   	DoRead();
	}

//*******************************************************************************
// Method      : CRtpAppConsole::DisplayMsg()
// Purpose     : Display testing message on screen
// Parameters  : TDesC &
// Return Value: 
//*******************************************************************************
void CTestAppConsole::DisplayMsg(const TDesC &aMsg)
	{
	iConsole->ClearScreen();
	iConsole->Write(*iInstruct);
	iConsole->Printf(KTxtLineBreak);
	iConsole->Printf(aMsg);
	iConsole->Printf(KTxtLineBreak);
	}

//*******************************************************************************
// Method      : CTestAppConsole::GetAddrFromConsole()
// Purpose     : 
// Parameters  : 
// Return Value: 
//*******************************************************************************
TKeyCode CTestAppConsole::GetStringFromConsole(TDes &aAddr)
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

    DisplayMsg(KTxtLineBreak);
    return input;
    }

//*******************************************************************************
// Method      : CTestAppConsole::RunTest1()
// Purpose     : 
// Parameters  : 
// Return Value: 
//*******************************************************************************

void CTestAppConsole::RunTest1()
    {
    //DisplayMsg(_L("------Test2 End------\n"));
    RMMFController controller;
    TMMFPrioritySettings settings;
	settings.iPriority = EPriorityNormal;

    controller.Open( KControllerUID, settings );
    
    TInt err = controller.AddDataSource( KSourceUID, KNullDesC8 );

    err = controller.AddDataSink( KSinkUID, KNullDesC8 );    

    }


//*******************************************************************************
void CTestAppConsole::RunTest2()
    {    
    }

//*******************************************************************************
void CTestAppConsole::Stop()
	{
    #ifdef DEBUG_INFO
		RDebug::Print (_L ("------The End------\n"));
	#endif

	DisplayMsg(_L("------The End------"));
//	iConsole->Getch();
	}


