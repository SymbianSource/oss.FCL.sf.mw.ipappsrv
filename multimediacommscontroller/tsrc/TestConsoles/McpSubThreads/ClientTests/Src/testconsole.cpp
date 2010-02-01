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
#include "TestConsole.h"
#include "mccsymsubthreadclient.h"
#include "mcculdlclient.h"
#include "mccdefs.h"
#include "mccinternalcodecs.h"
#include "mcculdlclient.h"
#include <mmf/server/mmfaudioinput.h>
#include "mccrtpdatasource.h"
#include "mccrtpdatasink.h"

const TUint32 KUplinkStreamId = 1;
const TUint32 KDownlinkStreamId = 2;

// Main Instruction Text
_LIT(KTxtMainInstructions, "Please select one option:\n"
 L"1. Test basic controls in UL/DL 1\n"
 L"3. Test UL Basic controls\n"
 L"4. Test DL Basic controls\n"
 L"5. Test memory management\n"
 L"9. Quit\n");

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
    /*
	TFileUtil::InitLogFile(KLogFileTest);	// test events
	TFileUtil::InitLogFile(KLogFileRtp);	// Rtp packets
	TFileUtil::InitLogFile(KLogFileRtcp);	// Rtcp packets
	TFileUtil::InitLogFile(KLogFileStat);	// Statistic
    */
    ConstructDlSinkSource();
    ConstructUlSinkSource();
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
    
    if(iDlsource)
        delete iDlsource;

    if(iUlsink)
        delete iUlsink;

    if(iDlsink)   
        delete iDlsink;
    
    if(iUlsource)
        delete iUlsource;
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
            __UHEAP_MARK;
            RunTest1();
            __UHEAP_MARKEND;
            break;
        case '3':
            __UHEAP_MARK;
            RunTest3();
            __UHEAP_MARKEND;
            break;
        case '4':
            __UHEAP_MARK;
            RunTest4();
            __UHEAP_MARKEND;
            break;
        case '5':
            __UHEAP_MARK;
            RunTest5();
            __UHEAP_MARKEND;
            break;
        case '6':
            RunTest6();
            break;
        case '9':
            CActiveScheduler::Stop();
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
    SetUpRtpStack();

    TInt cells = User::CountAllocCells();
    iConsole->Printf( _L("Allocated cells: %d\n"), cells);

    iKeepgoingUL = ETrue;
    CMccSymSubthreadClient* ul_client = CMccSymSubthreadClient::NewL(this);
    
    iKeepgoingDL = ETrue;
    CMccSymSubthreadClient* dl_client = CMccSymSubthreadClient::NewL(this);

    if(!ul_client)
        {
        iKeepgoingUL = EFalse;
        }

    if(!dl_client)
        {
        iKeepgoingDL = EFalse;
        }

    iUlError = KErrNone;
    iDlError = KErrNone;

    if(iKeepgoingUL && iKeepgoingDL)
        {
        iConsole->Printf( _L("Allocations OK for UL & DL, going on...\n") );
        }
    else
        {
        if(!iKeepgoingDL)
            {
            iConsole->Printf( _L("DL Allocations failed...\n") );
            }
        else if(!iKeepgoingDL)
            {
            iConsole->Printf( _L("UL Allocations failed...\n") );
            }
        else
            {
            iConsole->Printf( _L("DL & UL Allocations failed!!!\n") );
            }
        }
    
    for(TInt k = 0; k < 1; k++)
        {
        TestOpen( ul_client, dl_client );

        TestParameters( ul_client, dl_client );

        TestPrepare( ul_client, dl_client );

        TestControls( ul_client, dl_client );
        }

    if(iKeepgoingDL && iKeepgoingDL)
        {
        iConsole->Printf( _L("All tests ran OK\n") );
        }
    else
        {
        iConsole->Printf( _L("Not all tests done\n") );
        }

    
    delete ul_client;
    delete dl_client;

    cells = User::CountAllocCells();
    iConsole->Printf( _L("Allocated cells: %d\n"), cells);
    
    iConsole->Getch();
   };

void CTestAppConsole::ErrorNotify(TInt aError)
    {
    iConsole->Printf( _L("Error notify: %d"), aError );
    };

void CTestAppConsole::RunTest3()
    {
    SetUpRtpStack();
    TInt cells = User::CountAllocCells();
    iConsole->Printf( _L("Allocated cells: %d\n"), cells);

    CMccSymSubthreadClient* ul_client = CMccSymSubthreadClient::NewL(this);
    
    TFourCC fourAMR(KMccFourCCIdAMRNB);
    TRtpId ulsession = KNullId;
    TRtpId ulstream = KNullId;

    ul_client->CreateSessionL(iPort, iIapid, EFalse, 0);
    iConsole->Printf( _L("UL Session created\n") );
    iConsole->Getch();

    TMMFPrioritySettings prior;
    prior.iPriority = EMdaPriorityNormal;
    prior.iPref = EMdaPriorityPreferenceQuality;
    
    ul_client->OpenL(EMccAudioUplinkStream, fourAMR, iUlsource, iUlsink, KUplinkStreamId, prior );
   
    iConsole->Printf( _L("UL stream created\n") );
    iConsole->Getch();

    ul_client->SetRemoteAddressL(iRemoteAddr);
    iConsole->Printf( _L("Setted remote address\n") );
    iConsole->Getch();

    ul_client->SetPrioritySettingsL( KUplinkStreamId, prior );
    iConsole->Printf( _L("Setted priority settings\n") );
    iConsole->Getch();


    ul_client->PrepareL( KUplinkStreamId, 0 );
    iConsole->Printf( _L("Stream prepared\n") );
    iConsole->Getch();

    ul_client->PlayL( KUplinkStreamId, 0, EFalse, ETrue );
    iConsole->Printf( _L("Stream recording\n") );
    iConsole->Getch();
    
    
    ul_client->PauseL( KUplinkStreamId, 0, ETrue );
    iConsole->Printf( _L("Stream paused\n") );
    iConsole->Getch();

    ul_client->ResumeL( KUplinkStreamId, 0, ETrue );
    iConsole->Printf( _L("Stream resumed\n") );
    iConsole->Getch();
    
    ul_client->StopL( KUplinkStreamId, 0 );
    iConsole->Printf( _L("Stream stopped\n") );
    iConsole->Getch();

    ul_client->CloseL( KUplinkStreamId );
    iConsole->Printf( _L("Stream closed\n") );
    
    delete ul_client;

    cells = User::CountAllocCells();
    iConsole->Printf( _L("Allocated cells: %d\n"), cells);
    iConsole->Getch();   
    };

void CTestAppConsole::RunTest4()
    {
    SetUpRtpStack();

    TInt cells = User::CountAllocCells();
    iConsole->Printf( _L("Allocated cells: %d\n"), cells);

    CMccSymSubthreadClient* dl_client = CMccSymSubthreadClient::NewL(this);
    
    TFourCC fourAMR(KMccFourCCIdAMRNB);
    TRtpId dlsession = KNullId;
    TRtpId dlstream = KNullId;

    dl_client->CreateSessionL(iPort, iIapid, EFalse, 0);
    iConsole->Printf( _L("DL Session created\n") );
    iConsole->Getch();

        
    TMMFPrioritySettings prior;
    prior.iPriority = EMdaPriorityNormal;
    prior.iPref = EMdaPriorityPreferenceQuality;
    
    dl_client->OpenL( EMccAudioDownlinkStream, fourAMR, iDlsource, iDlsink, KDownlinkStreamId, prior );
    iConsole->Printf( _L("DL stream created\n") );
    iConsole->Getch();


    dl_client->SetPrioritySettingsL( KDownlinkStreamId, prior );
    iConsole->Printf( _L("Setted priority settings\n") );
    iConsole->Getch();

    dl_client->PrepareL( KDownlinkStreamId, 0 );
    iConsole->Printf( _L("Stream prepared\n") );
    iConsole->Getch();

    dl_client->PlayL( KDownlinkStreamId, 0, EFalse, ETrue );
    iConsole->Printf( _L("Stream playing\n") );
    iConsole->Getch();
    
    dl_client->PauseL( KDownlinkStreamId, 0, ETrue );
    iConsole->Printf( _L("Stream paused\n") );
    iConsole->Getch();

    dl_client->ResumeL( KDownlinkStreamId, 0, ETrue );
    iConsole->Printf( _L("Stream resumed\n") );
    iConsole->Getch();

    dl_client->StopL( KDownlinkStreamId, 0 );
    iConsole->Printf( _L("Stream stopped\n") );
    iConsole->Getch();

    dl_client->CloseL( KDownlinkStreamId );
    iConsole->Printf( _L("Stream closed\n") );
    
    delete dl_client;

    cells = User::CountAllocCells();
    iConsole->Printf( _L("Allocated cells: %d\n"), cells);
    iConsole->Getch();
    };

void CTestAppConsole::RunTest5()
    {
    TFourCC fourAMR(KMccFourCCIdAMRNB);
    TInt bitrate = 12200;

    iConsole->Printf( _L("Memory allocate/deallocate tests\n") );
    iConsole->Getch();

    __UHEAP_MARK;
    TInt cells = User::CountAllocCells();
    iConsole->Printf( _L("Allocated cells: %d\n"), cells);
    CMccSymSubthreadClient* dl_client = CMccSymSubthreadClient::NewL(this);
    delete dl_client;
    cells = User::CountAllocCells();
    iConsole->Printf( _L("Allocated cells: %d\n"), cells);
    __UHEAP_MARKEND;
    iConsole->Printf( _L("Basic DL Create/Delete OK\n") );
    iConsole->Getch();

    __UHEAP_MARK;
    cells = User::CountAllocCells();
    iConsole->Printf( _L("Allocated cells: %d\n"), cells);
    CMccSymSubthreadClient* ul_client = CMccSymSubthreadClient::NewL(this);
    delete ul_client;
    cells = User::CountAllocCells();
    iConsole->Printf( _L("Allocated cells: %d\n"), cells);
    __UHEAP_MARKEND;
    iConsole->Printf( _L("Basic UL Create/Delete OK\n") );
    iConsole->Getch();
    
    SetUpRtpStack();

    __UHEAP_MARK;
    cells = User::CountAllocCells();
    iConsole->Printf( _L("Allocated cells: %d\n"), cells);
    ul_client = CMccSymSubthreadClient::NewL(this);
    ul_client->CreateSessionL(iPort, iIapid, EFalse, 0);
    delete ul_client;
    cells = User::CountAllocCells();
    iConsole->Printf( _L("Allocated cells: %d\n"), cells);
    __UHEAP_MARKEND;
    iConsole->Printf( _L("UL Create session OK\n") );
    iConsole->Getch();
    
    __UHEAP_MARK;
    cells = User::CountAllocCells();
    iConsole->Printf( _L("Allocated cells: %d\n"), cells);
    dl_client = CMccSymSubthreadClient::NewL(this);
    dl_client->CreateSessionL(iPort, iIapid, EFalse, 0);
    delete dl_client;
    cells = User::CountAllocCells();
    iConsole->Printf( _L("Allocated cells: %d\n"), cells);
    __UHEAP_MARKEND;
    iConsole->Printf( _L("DL Create session OK\n") );
    iConsole->Getch();

    __UHEAP_MARK;
    cells = User::CountAllocCells();
    iConsole->Printf( _L("Allocated cells: %d\n"), cells);
    ul_client = CMccSymSubthreadClient::NewL(this);
    ul_client->CreateSessionL(iPort, iIapid, EFalse, 0);
    
    TMMFPrioritySettings prior;
    prior.iPriority = EMdaPriorityNormal;
    prior.iPref = EMdaPriorityPreferenceQuality;
    
    ul_client->OpenL( EMccAudioUplinkStream, fourAMR, iUlsource, iUlsink, KUplinkStreamId, prior );
    ul_client->CloseL();
    delete ul_client;
    cells = User::CountAllocCells();
    iConsole->Printf( _L("Allocated cells: %d\n"), cells);
    __UHEAP_MARKEND;
    iConsole->Printf( _L("UL Open/Close OK\n") );
    iConsole->Getch();
    
    __UHEAP_MARK;
    cells = User::CountAllocCells();
    iConsole->Printf( _L("Allocated cells: %d\n"), cells);
    dl_client = CMccSymSubthreadClient::NewL(this);
    dl_client->CreateSessionL(iPort, iIapid, EFalse, 0);
    
    dl_client->OpenL( EMccAudioDownlinkStream, fourAMR, iDlsource, iDlsink, KDownlinkStreamId, prior );
    dl_client->CloseL();
    delete dl_client;
    cells = User::CountAllocCells();
    iConsole->Printf( _L("Allocated cells: %d\n"), cells);
    __UHEAP_MARKEND;
    iConsole->Printf( _L("DL Open/Close OK\n") );
    iConsole->Getch();
    
    __UHEAP_MARK;
    cells = User::CountAllocCells();
    iConsole->Printf( _L("UL Allocated cells: %d\n"), cells);
    ul_client = CMccSymSubthreadClient::NewL(this);

    ul_client->CreateSessionL(iPort, iIapid, EFalse, 0);

    ul_client->OpenL( EMccAudioUplinkStream, fourAMR, iUlsource, iUlsink, KUplinkStreamId, prior );

    ul_client->PrepareL( KUplinkStreamId, 0 );
    
    ul_client->PlayL( KUplinkStreamId, 0, EFalse, ETrue );
    iConsole->Getch();
    ul_client->StopL( KUplinkStreamId, 0 );
    
    //ul_client->CloseL();

    delete ul_client;

    cells = User::CountAllocCells();
    iConsole->Printf( _L("UL Allocated cells: %d\n"), cells);
    __UHEAP_MARKEND;
    iConsole->Printf( _L("UL Open/Close/Prepare OK\n") );
    iConsole->Getch();

    __UHEAP_MARK;
    cells = User::CountAllocCells();
    iConsole->Printf( _L("Allocated cells: %d\n"), cells);
    dl_client = CMccSymSubthreadClient::NewL(this);
    dl_client->CreateSessionL(iPort, iIapid, EFalse, 0 );
    dl_client->OpenL( EMccAudioDownlinkStream, fourAMR, iDlsource, iDlsink, KDownlinkStreamId, prior );
    dl_client->PrepareL( KDownlinkStreamId, 0 );
    dl_client->PlayL( KDownlinkStreamId, 0, EFalse, ETrue );

    iConsole->Getch();

    dl_client->StopL( KDownlinkStreamId, 0 );
    //dl_client->CloseL();
    delete dl_client;
    cells = User::CountAllocCells();
    iConsole->Printf( _L("Allocated cells: %d\n"), cells);
    __UHEAP_MARKEND;
    iConsole->Printf( _L("DL Open/Close/Prepare OK\n") );
    iConsole->Getch();
    };


void CTestAppConsole::RunTest6()
    {
    TFourCC fourAMR(KMccFourCCIdAMRNB);

    SetUpRtpStack();

    for(TInt k = 0; k < 2; k++)
        {
        iConsole->Printf( _L("Cells BEFORE Tests: %d"), User::CountAllocCells() );

        iClient = CMccUlDlClient::NewL(this, iSessionID);
        
        iClient->CreateLinkL( KMccLinkGeneral, iLinkId, iPort, iIapid, 0, 0 );
        
        TMMFPrioritySettings prior;
        prior.iPriority = EMdaPriorityNormal;
        prior.iPref = EMdaPriorityPreferenceQuality;
        
        TFourCC fourAMR(KMccFourCCIdAMRNB);
    
        TInt ulstream = iClient->OpenL( iLinkId, EMccAudioUplinkStream, fourAMR, iUlsource, iUlsink, prior );
        TInt dlstream = iClient->OpenL( iLinkId, EMccAudioDownlinkStream, fourAMR, iDlsource, iDlsink, prior );
        /*
        iClient->SetRemoteAddressL( iRemoteAddr, ulstream );
        iClient->SetRemoteAddressL( iRemoteAddr, dlstream );
        iClient->SetTargetBitrateL(ulstream, 12200);
        iClient->SetTargetBitrateL(dlstream, 12200);
        */
        //iClient->PrepareL(ulstream);
        //iClient->PrepareL(dlstream);

        //iClient->CloseL(ulstream);
        //iClient->CloseL(dlstream);
        delete iClient;

        iConsole->Printf( _L("Cells AFTER Tests: %d"), User::CountAllocCells() );

        iConsole->Printf( _L("Test 6 OK if you see this") );
        iConsole->Getch();
        }

    };

void CTestAppConsole::ErrorOccured( TInt aError,
                                    TUint32 aSessionId,
                                    TUint32 aLinkId,
            	                    TUint32 aStreamId, 
            	                    TUint32 aEndpointId )
    {
    iConsole->Printf( _L("Error Occured: %d, Session: %u, Link: %u, Stream: %u, Endpoint %u"), 
                      aError,
                      aSessionId,
                      aLinkId,
            	      aStreamId, 
            	      aEndpointId );
    };

void CTestAppConsole::StateChange( TInt aState, TUint32 aLink )
    {
    iConsole->Printf( _L("State change STATE: %d, LINK: %u"), aState, aLink );
    };

TInt CTestAppConsole::SendMccEventToClient( TMccEvent& /*aEvent*/ )
    {
    iConsole->Printf( _L("Event received...") );
    
    return KErrNone;
    };

void CTestAppConsole::SetUpRtpStack()
    {
    TDes* line = new TBuf<80>;
   
    iConsole->ClearScreen();
    iConsole->Printf( _L("\nEnter the IAP ID: ") );
    GetStringFromConsole(*line);

    TLex lex(*line);
    lex.Val(iIapid,EDecimal);

    // Get Remote Port number
	iConsole->ClearScreen();
    iConsole->Printf(_L("\nMccEnter the Local port number "));

	GetStringFromConsole(*line);

    //Use INPUT
    TLex lex2(*line);
    lex2.Val(iPort,EDecimal);

    //Use INPUT
    iConsole->ClearScreen();
		
	TBuf<128> theInput;

	iConsole->Printf(_L("\nEnter the Remote IP address "));	
	GetStringFromConsole(*line);

	theInput.Copy( *line );
	iRemoteAddr.SetFamily(KAfInet);
	iRemoteAddr.Input(theInput);
    iRemoteAddr.SetPort(iPort);
    
    delete line;
    };

void CTestAppConsole::TestOpen(CMccSymSubthreadClient* aUlclient, CMccSymSubthreadClient* aDlclient)
    {
    TFourCC fourAMR(KMccFourCCIdAMRNB);
    
    TMMFPrioritySettings prior;
    prior.iPriority = EMdaPriorityNormal;
    prior.iPref = EMdaPriorityPreferenceQuality;

    // Test session creation
    if(iKeepgoingUL)
        {
        TRAP(iUlError, aUlclient->CreateSessionL(iPort, iIapid, EFalse, 0) );
        if(iUlError != KErrNone)
            {
            iKeepgoingUL = EFalse;
            iConsole->Printf( _L("Error in UL CreateSessionL: %d\n"), iUlError );
            iConsole->Getch();
            }
        else
            {
            iConsole->Printf( _L("UL CreateSessionL OK\n") );
            iConsole->Getch();
            }
        }
    else
        {
        iConsole->Printf( _L("UL CreateSessionL Test not run\n") );
        iConsole->Getch();
        }

    if(iKeepgoingDL)
        {
        TRAP(iDlError, aDlclient->CreateSessionL(iPort, iIapid, EFalse, 0) );
        if(iDlError != KErrNone)
            {
            iKeepgoingDL = EFalse;
            iConsole->Printf( _L("Error in DL CreateSessionL: %d\n"), iDlError );
            iConsole->Getch();
            }
        else
            {
            iConsole->Printf( _L("DL CreateSessionL OK\n") );
            iConsole->Getch();
            }
        }
    else
        {
        iConsole->Printf( _L("DL CreateSessionL Test not run\n") );
        iConsole->Getch();
        }

    if(iKeepgoingUL)
        {
        TRAP(iUlError, aUlclient->OpenL(EMccAudioUplinkStream, fourAMR, iUlsource, iUlsink, KUplinkStreamId, prior) );
        if(iUlError != KErrNone)
            {
            iKeepgoingUL = EFalse;
            iConsole->Printf( _L("Error in UL OpenL: %d\n"), iUlError );
            iConsole->Getch();
            }
        else
            {
            iConsole->Printf( _L("UL OpenL OK\n") );
            iConsole->Getch();
            }
        }
    else
        {
        iConsole->Printf( _L("UL OpenL Test not run\n") );
        iConsole->Getch();
        }

    if(iKeepgoingDL)
        {
        TRAP(iDlError, aDlclient->OpenL(EMccAudioDownlinkStream, fourAMR, iDlsource, iDlsink, KDownlinkStreamId, prior ) );
        if(iDlError != KErrNone)
            {
            iKeepgoingDL = EFalse;
            iConsole->Printf( _L("Error in DL OpenL: %d\n"), iDlError );
            iConsole->Getch();
            }
        else
            {
            iConsole->Printf( _L("DL OpenL OK\n") );
            iConsole->Getch();
            }
        }
    else
        {
        iConsole->Printf( _L("DL OpenL Test not run\n") );
        iConsole->Getch();
        }

    // Test remote address setup...
    if(iKeepgoingUL)
        {
        TRAP(iUlError, aUlclient->SetRemoteAddressL(iRemoteAddr) );
        if(iUlError != KErrNone)
            {
            iKeepgoingUL = EFalse;
            iConsole->Printf( _L("Error in UL SetRemoteAddress: %d\n"), iUlError );
            iConsole->Getch();
            }
        else
            {
            iConsole->Printf( _L("UL SetRemoteAddress OK\n") );
            iConsole->Getch();
            }
        }
    else
        {
        iConsole->Printf( _L("UL SetRemoteAddress Test not run\n") );
        iConsole->Getch();
        }

    if(iKeepgoingDL)
        {
        TRAP( iDlError, aDlclient->SetRemoteAddressL(iRemoteAddr) );
        if(iDlError != KErrNone)
            {
            iKeepgoingDL = EFalse;
            iConsole->Printf( _L("Error in DL SetRemoteAddress: %d\n"), iDlError );
            iConsole->Getch();
            }
        else
            {
            iConsole->Printf( _L("DL SetRemoteAddress OK\n") );
            iConsole->Getch();
            }
        }
    else
        {
        iConsole->Printf( _L("DL SetRemoteAddress Test not run\n") );
        iConsole->Getch();
        }
    };

void CTestAppConsole::TestParameters(CMccSymSubthreadClient* aUlclient, CMccSymSubthreadClient* aDlclient)
    {
    TFourCC fourAMR(KMccFourCCIdAMRNB);
    TFourCC fourG711(KMccFourCCIdG711);
    TFourCC fourILBC(KMccFourCCIdILBC);
    
    TMccCodecInfo codec;
    codec.iFourCC = fourAMR;
    codec.iBitrate = 12200;
    codec.iPtime = 20;
    codec.iMaxPtime = 40;
    codec.iAlgoUsed = ENoAlgoUsed;
    codec.iCodecMode = KAmrCodecModeOctetAlign;
    codec.iEnableDTX = EFalse;
    
    /*
    if(iKeepgoingUL)
        {
        TRAP( iUlError, aUlclient->SetCodecL(fourG711) );
        if(iUlError != KErrNone)
            {
            iKeepgoingUL = EFalse;
            iConsole->Printf( _L("Error in UL SetCodecL G.711: %d\n"), iUlError );
            iConsole->Getch();
            }
        else
            {
            iConsole->Printf( _L("UL SetCodecL G.711 OK\n") );
            iConsole->Getch();
            }
        }
    else
        {
        iConsole->Printf( _L("UL SetCodecL G.711 Test not run\n") );
        iConsole->Getch();
        }

    if(iKeepgoingDL)
        {
        TRAP( iDlError, aDlclient->SetCodecL(fourG711) );
        if(iDlError != KErrNone)
            {
            iKeepgoingDL =EFalse;
            iConsole->Printf( _L("Error in DL SetCodecL G.711: %d\n"), iDlError );
            iConsole->Getch();
            }
        else
            {
            iConsole->Printf( _L("DL SetCodecL G.711 OK\n") );
            iConsole->Getch();
            }
        }
    else
        {
        iConsole->Printf( _L("DL SetCodecL G.711 Test not run\n") );
        iConsole->Getch();
        }
    */
 
    
    TMMFPrioritySettings prior;
    prior.iPriority = EMdaPriorityMax;
    prior.iPref = EMdaPriorityPreferenceTimeAndQuality;
    
    if(iKeepgoingUL)
        {
        TRAP( iUlError, aUlclient->SetPrioritySettingsL(KUplinkStreamId, prior) );
        if(iUlError != KErrNone)
            {
            iKeepgoingUL = EFalse;
            iConsole->Printf( _L("Error in UL SetPrioritySettingsL: %d\n"), iUlError );
            iConsole->Getch();
            }
        else
            {
            iConsole->Printf( _L("UL SetPrioritySettingsL OK\n") );
            iConsole->Getch();
            }
        }
    else
        {
        iConsole->Printf( _L("UL SetPrioritySettingsL Test not run\n") );
        iConsole->Getch();
        }

    if(iKeepgoingDL)
        {
        TRAP( iDlError, aDlclient->SetPrioritySettingsL(KDownlinkStreamId, prior) );
        if(iDlError != KErrNone)
            {
            iKeepgoingDL =EFalse;
            iConsole->Printf( _L("Error in DL SetPrioritySettingsL: %d\n"), iDlError );
            iConsole->Getch();
            }
        else
            {
            iConsole->Printf( _L("DL SetPrioritySettingsL OK\n") );
            iConsole->Getch();
            }
        }
    else
        {
        iConsole->Printf( _L("DL SetPrioritySettingsL Test not run\n") );
        iConsole->Getch();
        }
    
    TInt volume = 0;
    
    if(iKeepgoingUL)
        {
        TRAP( iUlError, aUlclient->MaxGainL(volume) );
        if(iUlError != KErrNone)
            {
            iKeepgoingUL =EFalse;
            iConsole->Printf( _L("Error in UL MaxGainL: %d\n"), iUlError );
            iConsole->Getch();
            }
        else
            {
            iConsole->Printf( _L("UL MaxGainL OK\n") );
            iConsole->Getch();
            }
        }

    if(iKeepgoingUL)
        {
        TRAP( iUlError, aUlclient->SetGainL(volume) );
        if(iUlError != KErrNone)
            {
            iKeepgoingUL =EFalse;
            iConsole->Printf( _L("Error in UL SetGainL: %d\n"), iUlError );
            iConsole->Getch();
            }
        else
            {
            iConsole->Printf( _L("UL SetGainL OK\n") );
            iConsole->Getch();
            }
        }
    
    if(iKeepgoingDL)
        {
        TRAP( iDlError, aDlclient->MaxVolumeL(volume) );
        if(iDlError != KErrNone)
            {
            iKeepgoingDL =EFalse;
            iConsole->Printf( _L("Error in DL MaxVolumeL: %d\n"), iDlError );
            iConsole->Getch();
            }
        else
            {
            iConsole->Printf( _L("DL MaxVolumeL OK\n") );
            iConsole->Getch();
            }
        }

    if(iKeepgoingDL)
        {
        TRAP( iDlError, aDlclient->SetVolumeL(volume) );
        if(iDlError != KErrNone)
            {
            iKeepgoingDL =EFalse;
            iConsole->Printf( _L("Error in DL SetVolumeL: %d\n"), iDlError );
            iConsole->Getch();
            }
        else
            {
            iConsole->Printf( _L("DL SetVolumeL OK\n") );
            iConsole->Getch();
            }
        }
    
    if(iKeepgoingUL)
        {
        TRAP( iUlError, aUlclient->SetBalanceL(KUplinkStreamId, 50, 50) );
        if(iUlError != KErrNone)
            {
            iKeepgoingUL = EFalse;
            iConsole->Printf( _L("Error in UL SetBalanceL: %d\n"), iUlError);
            iConsole->Getch();
            }
        else
            {
            iConsole->Printf( _L("UL SetBalanceL OK\n") );
            iConsole->Getch();
            }
        }

    if(iKeepgoingDL)
        {
        TRAP( iDlError, aDlclient->SetBalanceL(KDownlinkStreamId, 50, 50) );
        if(iDlError != KErrNone)
            {
            iKeepgoingDL = EFalse;
            iConsole->Printf( _L("Error in DL SetBalanceL: %d\n"), iDlError);
            iConsole->Getch();
            }
        else
            {
            iConsole->Printf( _L("DL SetBalanceL OK\n") );
            iConsole->Getch();
            }
        }
 
    TInt lbal = 0;
    TInt rbal = 0;
    
    if(iKeepgoingUL)
        {
        TRAP( iUlError, aUlclient->GetBalanceL(KUplinkStreamId, lbal, rbal) );
        if(iUlError != KErrNone)
            {
            iKeepgoingUL = EFalse;
            iConsole->Printf( _L("Error in UL GetRecordBalanceL: %d\n"), iUlError);
            iConsole->Getch();
            }
        else
            {
            iConsole->Printf( _L("UL GetRecordBalanceL OK: %d ja %d\n"), lbal, rbal );
            iConsole->Getch();
            }
        }

    if(iKeepgoingDL)
        {
        TRAP( iDlError, aDlclient->GetBalanceL(KDownlinkStreamId, lbal, rbal) );
        if(iDlError != KErrNone)
            {
            iKeepgoingDL = EFalse;
            iConsole->Printf( _L("Error in DL GetPlayBalanceL: %d\n"), iDlError);
            iConsole->Getch();
            }
        else
            {
            iConsole->Printf( _L("DL GetPlayBalanceL OK: %d ja %d\n"), lbal, rbal );
            iConsole->Getch();
            }
        }
        
    if(iKeepgoingUL)
        {
        TRAP( iUlError, aUlclient->SetCodecInformationL(KUplinkStreamId, codec, KNullDesC8) );
        if(iUlError != KErrNone)
            {
            iKeepgoingUL = EFalse;
            iConsole->Printf( _L("Error in UL SetCodecInformationL: %d\n"), iUlError);
            iConsole->Getch();
            }
        else
            {
            iConsole->Printf( _L("UL SetCodecInformationL OK\n") );
            iConsole->Getch();
            }
        }

    if(iKeepgoingDL)
        {
        TRAP( iDlError, aDlclient->SetCodecInformationL(KDownlinkStreamId, codec, KNullDesC8) );
        if(iDlError != KErrNone)
            {
            iKeepgoingDL = EFalse;
            iConsole->Printf( _L("Error in DL SetCodecInformationL: %d\n"), iDlError);
            iConsole->Getch();
            }
        else
            {
            iConsole->Printf( _L("DL SetCodecInformationL OK\n") );
            iConsole->Getch();
            }
        }        
    };

void CTestAppConsole::TestPrepare(CMccSymSubthreadClient* aUlclient, CMccSymSubthreadClient* aDlclient)
    {
    if(iKeepgoingUL)
        {
        TRAP( iUlError, aUlclient->PrepareL( KUplinkStreamId, 0 ) );
        if(iUlError != KErrNone)
            {
            iKeepgoingUL = EFalse;
            iConsole->Printf( _L("Error in UL PrepareL: %d\n"), iUlError );
            iConsole->Getch();
            }
        else
            {
            iConsole->Printf( _L("UL PrepareL OK\n") );
            iConsole->Getch();
            }
        }

    if(iKeepgoingDL)
        {
        TRAP( iDlError, aDlclient->PrepareL( KDownlinkStreamId, 0 ) );
        if(iDlError != KErrNone)
            {
            iKeepgoingDL = EFalse;
            iConsole->Printf( _L("Error in DL PrepareL: %d\n"), iDlError );
            iConsole->Getch();
            }
        else
            {
            iConsole->Printf( _L("DL PrepareL OK\n") );
            iConsole->Getch();
            }
        }
    };

void CTestAppConsole::TestControls(CMccSymSubthreadClient* aUlclient, CMccSymSubthreadClient* aDlclient)
    {
    if(iKeepgoingUL)
        {
        TRAP( iUlError, aUlclient->PlayL(KUplinkStreamId, 0, EFalse, ETrue) );
        if(iUlError != KErrNone)
            {
            iKeepgoingUL = EFalse;
            iConsole->Printf( _L("Error in UL PlayL: %d\n"), iUlError );
            iConsole->Getch();
            }
        else
            {
            iConsole->Printf( _L("UL PlayL OK\n") );
            iConsole->Getch();
            }
        }
    
    if(iKeepgoingDL)
        {
        TRAP( iDlError, aDlclient->PlayL(KDownlinkStreamId, 0, EFalse, ETrue) );
        if(iDlError != KErrNone)
            {
            iKeepgoingDL = EFalse;
            iConsole->Printf( _L("Error in DL PlayL: %d\n"), iDlError );
            iConsole->Getch();
            }
        else
            {
            iConsole->Printf( _L("DL PlayL OK\n") );
            iConsole->Getch();
            }
        }

    /*
    TTimeIntervalMicroSeconds32 wait(20000000);
    User::After(wait);
    
    if(iKeepgoingUL)
        {
        TRAP( iUlError, aUlclient->PauseL() );
        if(iUlError != KErrNone)
            {
            iKeepgoingUL = EFalse;
            iConsole->Printf( _L("Error in UL PauseL: %d\n"), iUlError );
            iConsole->Getch();
            }
        else
            {
            iConsole->Printf( _L("UL PauseL OK\n") );
            iConsole->Getch();
            }
        }

    
    if(iKeepgoingDL)
        {
        TRAP( iDlError, aDlclient->PauseL() );
        if(iDlError != KErrNone)
            {
            iKeepgoingDL = EFalse;
            iConsole->Printf( _L("Error in DL PauseL: %d\n"), iDlError );
            iConsole->Getch();
            }
        else
            {
            iConsole->Printf( _L("DL PauseL OK\n") );
            iConsole->Getch();
            }
        }
    
    if(iKeepgoingUL)
        {
        TRAP( iUlError, aUlclient->ResumeL() );
        if(iUlError != KErrNone)
            {
            iKeepgoingUL = EFalse;
            iConsole->Printf( _L("Error in UL ResumeL: %d\n"), iUlError );
            iConsole->Getch();
            }
        else
            {
            iConsole->Printf( _L("UL ResumeL OK\n") );
            iConsole->Getch();
            }
        }
    
    if(iKeepgoingDL)
        {
        TRAP( iDlError, aDlclient->ResumeL() );
        if(iDlError != KErrNone)
            {
            iKeepgoingDL = EFalse;
            iConsole->Printf( _L("Error in DL ResumeL: %d\n"), iDlError );
            iConsole->Getch();
            }
        else
            {
            iConsole->Printf( _L("DL ResumeL OK\n") );
            iConsole->Getch();
            }
        }
    */
    TRAP( iUlError, aUlclient->StopL(KUplinkStreamId, 0) );
    if(iUlError != KErrNone)
        {
        iConsole->Printf( _L("Error in UL StopL: %d\n"), iUlError );
        iConsole->Getch();
        iKeepgoingUL = EFalse;
        }
    else
        {
        iConsole->Printf( _L("UL StopL OK\n") );
        iConsole->Getch();
        }
    
    TRAP( iDlError, aDlclient->StopL(KDownlinkStreamId, 0) );
    if(iDlError != KErrNone)
        {
        iConsole->Printf( _L("Error in DL StopL: %d\n"), iDlError );
        iConsole->Getch();
        iKeepgoingDL = EFalse;
        }
    else
        {
        iConsole->Printf( _L("DL StopL OK\n") );
        iConsole->Getch();
        }
       

    TRAP( iUlError, aUlclient->CloseL() );
    if(iUlError != KErrNone)
        {
        iConsole->Printf( _L("Error in UL CloseL: %d\n"), iUlError );
        iConsole->Getch();
        iKeepgoingUL = EFalse;
        }
    else
        {
        iConsole->Printf( _L("UL CloseL OK\n") );
        iConsole->Getch();
        }

    TRAP( iDlError, aDlclient->CloseL() );
    if(iDlError != KErrNone)
        {
        iConsole->Printf( _L("Error in DL CloseL: %d\n"), iDlError );
        iConsole->Getch();
        iKeepgoingDL = EFalse;
        }
    else
        {
        iConsole->Printf( _L("DL CloseL OK\n") );
        iConsole->Getch();
        }
    };

void CTestAppConsole::ConstructUlSinkSource()
    {
    TUid rtpsnk;
    rtpsnk.iUid = 0x10202861;
    
    iKeepgoingUL = EFalse;
    TInt allocerror = KErrNone;

    TRAP( allocerror, iUlsink = MDataSink::NewSinkL(rtpsnk, KNullDesC8) );
    if(allocerror != KErrNone)
        {
        iKeepgoingUL = EFalse;
        iConsole->Printf( _L("Error alloc UL SRC %d\n"), allocerror );
        iConsole->Getch();
        return;
        }

    TRAP( allocerror, iUlsource = MDataSource::NewSourceL(KUidMmfAudioInput, KNullDesC8) );
    if(allocerror != KErrNone)
        {
        iKeepgoingUL = EFalse;
        iConsole->Printf( _L("Error alloc UL SNK %d\n"), allocerror );
        iConsole->Getch();
        return;
        }

    iKeepgoingUL = ETrue;
    };

void CTestAppConsole::ConstructDlSinkSource()
    {
    TUid rtpsrc;
    rtpsrc.iUid = 0x1020285F;

    iKeepgoingDL = EFalse;
    
    TInt allocerror = KErrNone;

    TRAP( allocerror, iDlsource = MDataSource::NewSourceL(rtpsrc, KNullDesC8) );
    if(allocerror != KErrNone)
        {
        iKeepgoingDL = EFalse;
        iConsole->Printf( _L("Error alloc DL SRC %d\n"), allocerror );
        iConsole->Getch();
        return;
        }

    TRAP( allocerror, iDlsink = MDataSink::NewSinkL(KUidMmfAudioOutput, KNullDesC8) );
    if(allocerror != KErrNone)
        {
        iKeepgoingDL = EFalse;
        iConsole->Printf( _L("Error alloc DL SNK %d\n"), allocerror );
        iConsole->Getch();
        return;
        }
    //iDlsink->SinkStopL();
    iKeepgoingDL = ETrue;
    };

void CTestAppConsole::DestructSinksSources()
    {
    
    };
