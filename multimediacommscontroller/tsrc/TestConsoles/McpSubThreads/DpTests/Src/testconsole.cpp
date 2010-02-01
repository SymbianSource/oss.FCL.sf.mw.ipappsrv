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
#include <mmf/server/mmfformat.h>
#include "TestConsole.h"
#include "mccdefs.h"
#include "mccinternalcodecs.h"
#include <mmf/server/mmfaudioinput.h>
#include "mccrtpdatasource.h"
#include "mccrtpdatasink.h"
#include "mccdatapathbase.h"

#include "mcculdatapath.h"
#include "mccdldatapath.h"
#include "mccuids.hrh"
#include "mccrtpmanager.h"

// Main Instruction Text
_LIT(KTxtMainInstructions, "Please select one option:\n"
 L"1. Test case 1\n"
 L"2. Test case 2\n"
 L"3. Test case 3\n"
 L"4. Test case 4\n"
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

    if(iRtpmanager)
        delete iRtpmanager;
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
            RunTest2();
            break;
        case '3':
            RunTest3();
            break;
        case '4':
            RunTest4();
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
    __UHEAP_MARK;

    SetUpRtpStack();

    iConsole->Printf( _L("Allocating UL datapath..\n") );
    
    CMccUlDataPath* uldata = CMccUlDataPath::NewL(this, TMediaId(KUidMediaTypeAudio) );
    
    TUid pfuid;

    pfuid.iUid = KImplUidAmrPayloadFormatEncode;
    //pfuid.iUid = KImplUidG711PayloadFormatEncode;
    
    uldata->AddDataSourceL( iUlsource );

    iUlsink->SetSinkDataTypeCode( TFourCC(KMccFourCCIdAMRNB), (TMediaId)KUidMediaTypeAudio );
    CMMFFormatEncode* pfencoder = CMMFFormatEncode::NewL( pfuid, iUlsink );
    
    iUlsource->NegotiateSourceL(*pfencoder);
    pfencoder->NegotiateL(*iUlsource);

    uldata->AddDataSinkL( pfencoder );

    CRtpAPI* temp = iRtpmanager->GetRtpReferenceL();
    
    TMccRtpSessionParams sessionParams;
    sessionParams.iRtpAPI = temp;
    sessionParams.iSessionId = iSessionID;
    sessionParams.iEventHandler = this;
    sessionParams.iEnableRTCP = EFalse;
    
    TMccCodecInfo codecInfo;
    codecInfo.iPayloadType = 96;
    TMccCodecInfoBuffer buf( codecInfo );
    
    ( static_cast<CMccRtpDataSink*>(iUlsink) )->SetSessionParamsL( sessionParams );
    ( static_cast<CMccRtpDataSink*>(iUlsink) )->ConfigureL( buf );

    uldata->PrimeL();

    iRtpmanager->CloseSession();

    delete uldata;
    delete pfencoder;
    delete iRtpmanager;

    __UHEAP_MARKEND;
    
    iConsole->Printf( _L("Allocating UL datapath successful\n") );
    iConsole->Getch();
    };

void CTestAppConsole::RunTest2()
    {
    SetUpRtpStack();

    iConsole->Printf( _L("Allocating DL datapath..\n") );

    __UHEAP_MARK;

    CMccDlDataPath* dldata = CMccDlDataPath::NewL( this, TMediaId(KUidMediaTypeAudio) );
    
    TUid pfuid;
    pfuid.iUid = KImplUidAmrPayloadFormatDecode;
    // pfuid.iUid = KImplUidG711PayloadFormatEncode;
    
    dldata->AddDataSinkL(iDlsink);

    iDlsource->SetSourceDataTypeCode( TFourCC(KMccFourCCIdAMRNB), (TMediaId)KUidMediaTypeAudio );
    CMMFFormatDecode* pfdecoder = CMMFFormatDecode::NewL( pfuid, iDlsource );
    
    iDlsink->NegotiateL(*pfdecoder);

    dldata->AddDataSourceL( pfdecoder );
    
    CRtpAPI* temp = iRtpmanager->GetRtpReferenceL();
    TMccRtpSessionParams sessionParams;
    sessionParams.iRtpAPI = temp;
    sessionParams.iSessionId = iSessionID;
    sessionParams.iEventHandler = this;
    sessionParams.iEnableRTCP = EFalse;
    
    TMccCodecInfo codecInfo;
    codecInfo.iPayloadType = 96;
    TMccCodecInfoBuffer buf( codecInfo );
    
    ( static_cast<CMccRtpDataSource*>(iDlsource) )->SetSessionParamsL( sessionParams );
    ( static_cast<CMccRtpDataSource*>(iDlsource) )->ConfigureL( buf );

    dldata->PrimeL();
    
    iRtpmanager->CloseSession();

    delete dldata;
    delete pfdecoder;

    __UHEAP_MARKEND;
    
    iConsole->Printf( _L("Allocating DL datapath successful\n") );
    iConsole->Getch();
    };

void CTestAppConsole::RunTest3()
    {
    iConsole->Printf( _L("RTP Manager Test Starting") );

    __UHEAP_MARK;
    SetUpRtpStack();
    iRtpmanager->CloseSession();
    delete iRtpmanager;
    __UHEAP_MARKEND;

    iConsole->Printf( _L("RTP Manager Test OK") );
    iConsole->Getch();
    };

void CTestAppConsole::RunTest4()
    {
    SetUpRtpStack();
    };

TInt CTestAppConsole::SendEventToClient(const TMMFEvent& /*aEvent*/)
    {
    iConsole->Printf( _L("Received Event\n") );
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
    
    iRtpmanager = CMccRtpManager::NewL(*this);

    iRtpmanager->InitializeL(iIapid);
    User::LeaveIfError( iRtpmanager->SetRemoteAddress( iRemoteAddr ) );
    iRtpmanager->CreateSessionL(iPort, EFalse);
    
    iSessionID = iRtpmanager->SessionId();
    
    delete line;
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
    //iUlsource->SourceStopL();
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
