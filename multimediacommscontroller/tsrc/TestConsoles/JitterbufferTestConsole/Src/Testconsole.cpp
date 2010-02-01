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
#include <e32math.h>
#include "TestConsole.h"
#include "MccInternalCodecs.h"


_LIT(KTxtMainInstructions, "Please select one option:\n"
 L"s. Setup\n"
 L"1. FillBuffer\n"
 L"2. EmptyBuffer\n"
 L"r. Set SeqNum\n"
 L"\n"

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
CTestAppConsole::CTestAppConsole( ) : CActive(EPriorityStandard),
	MDataSink( KTestApp ), MDataSource( KTestApp )
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
    _LIT(KTxtTitle, " MCCJitterBuffer Test ");
	iConsole = Console::NewL(KTxtTitle, TSize(KConsFullScreen, KConsFullScreen));
	DisplayConsoleMenu(KTxtMainInstructions);

	//__KHEAP_MARK;
	//iMccInterface = CMCCInterface::NewL(*this);
	iJitterBuffer = CMccJitterBuffer::NewL();

	iSeqNum = 1;
	//iCodecArray.Reset();
    //iMccInterface->GetCapabilities(iCodecArray); // RPointerArray<TFourCC>& aCodecs

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
//			iCodecArray.Close();
	//iMccInterface->Close();	
	delete iJitterBuffer;


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

	iConsole->Read(iStatus);
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
	    //ProcessKey(TChar(iConsole->KeyCode()));
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
	//TChar inputChar = iConsole->KeyCode();
    TDes* line = new TBuf<80>;

	GetStringFromConsole(*line);
	
	TBuf<80> line2 = *line;
	delete line;
	
	if ( line2.Length() > 0 )
	{
		TChar inputChar = line2[0];
		
		switch(inputChar)
			{
			case 's':	
				RunTestSetupJitterbuffer();
				break;
			case '1':
				RunTestFillBuffer();
				break;
			case '2':
				RunTestEmptyBuffer();
				break;
			case 'r':
				SetSeqNum();
				break;
	

			case '9':
				//iMccInterface->Close();
				CActiveScheduler::Stop();
				break;
			default:
				_LIT(KTxtWrongOption, "Wrong Option! Try Again.");
				DisplayMsg(KTxtWrongOption);
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

// -----------------------------------------------------------------------------
// CTestAppConsole::GenerateRandomSourceData
// Fills parameter buffer with random data.
// -----------------------------------------------------------------------------
//    
void CTestAppConsole::GenerateRandomSourceData( TDes8& aToBuffer )
    {
    aToBuffer.Zero();
    TInt length( aToBuffer.MaxSize() );
    for ( TInt i = 0; i < length; i++ )
        {
        TUint8 byte = static_cast<TUint8>( Math::Random() );
        aToBuffer.Append( byte );    
        }
    }

void CTestAppConsole::PrintBuffer( TDes8& aPayload )
{
	for ( TInt i = 0; i < /*3 */aPayload.Length(); i++ )
	{
		RDebug::Print(_L("buf[%d] = [%d]"), i, aPayload[i]);
	}
}

void CTestAppConsole::SetSeqNum()
{
    TDes* line = new TBuf<80>;

	iConsole->Printf(_L("\nEnter Sequence Number: "));
	TUint aSeqNum;
	GetStringFromConsole(*line);
	TLex lex(*line);
	lex.Val(aSeqNum,EDecimal);

	iSeqNum = TInt(aSeqNum);
	delete line;

}

//*******************************************************************************
// Method      : CTestAppConsole::RunTest1()
// Purpose     : 
// Parameters  : 
// Return Value: 
//*******************************************************************************
void CTestAppConsole::RunTestSetupJitterbuffer()
{

	iCodec = KMccFourCCIdAMRNB;

    TDes* line = new TBuf<80>;

	iConsole->Printf(_L("\nEnter Initial Size (ms): "));
	TUint aInitSize;
	GetStringFromConsole(*line);
	TLex lex(*line);
	lex.Val(aInitSize,EDecimal);

	//iSeqNum = TInt(aSeqNum);
	delete line;

	iJitterBuffer->AddDataSinkL( this );
	
	TMccCodecInfo info;
	info.iFourCC = iCodec;
	info.iPtime = 40;
	info.iBitrate = 12200;

	iJitterBuffer->SetupL( aInitSize, 5, info );


}

void CTestAppConsole::RunTestFillBuffer()
    {

	CMMFDataBuffer* iFrameBuffer = CMMFDataBuffer::NewL( 320 );
	TMediaId                iMediaId;
	iMediaId = KUidMediaTypeAudio;
	iJitterBuffer->FillBufferL( iFrameBuffer, this, iMediaId );

	if ( iFrameBuffer->Data().Length() > 0 )
		PrintBuffer( static_cast<TDes8&>( iFrameBuffer->Data() ) );

	delete iFrameBuffer;
    };

void CTestAppConsole::RunTestEmptyBuffer()
    {

	TInt iFrameSize;

	if ( KMCCFourCCIdAMRNB == iCodec )
		iFrameSize = 32;
	else if ( KMCCFourCCIdG711 == iCodec )
		iFrameSize = 162;

	CMMFDataBuffer* iFrameBuffer = CMMFDataBuffer::NewL( iFrameSize );
	TMediaId                iMediaId;
	iMediaId = KUidMediaTypeAudio;

	GenerateRandomSourceData( static_cast<TDes8&>( iFrameBuffer->Data() ) );

	iFrameBuffer->SetFrameNumber( iSeqNum );

	iJitterBuffer->EmptyBufferL( iFrameBuffer, this, iMediaId );

	//PrintBuffer( static_cast<TDes8&>( iFrameBuffer->Data() ) );
	
	delete iFrameBuffer;
	iSeqNum++;

	if ( iSeqNum == 32 )
		iSeqNum = 1;
    };



TFourCC CTestAppConsole::SinkDataTypeCode( TMediaId /*aMediaId*/ )
	{
	//TFourCC iFourCC;
	return NULL;//iFourCC;
	}	

// -----------------------------------------------------------------------------
// CMCCJitterBuffer::CanCreateSinkBuffer
// From MDataSink
// -----------------------------------------------------------------------------
//
TBool CTestAppConsole::CanCreateSinkBuffer()
	{
    // CMCCJitterBuffer can't create buffers
	return EFalse;
	}
// -----------------------------------------------------------------------------
// CMCCJitterBuffer::CreateSinkBufferL
// From MDataSink
// -----------------------------------------------------------------------------
//
CMMFBuffer* CTestAppConsole::CreateSinkBufferL( TMediaId /*aMediaId*/, TBool& /*aReference*/ )
	{
	return CMMFDataBuffer::NewL( 162 );
	}

// -----------------------------------------------------------------------------
// CMCCJitterBuffer::EmptyBufferL
// From MDataSink
// -----------------------------------------------------------------------------
//
void CTestAppConsole::EmptyBufferL( CMMFBuffer* aBuffer, 
									 MDataSource* aSupplier, 
									 TMediaId /*aMediaId*/)
	{
	RDebug::Print(_L("CTestAppConsole::EmptyBufferL"));
	if ( static_cast<CMMFDataBuffer*>(aBuffer)->Data().Length() > 0 )
		PrintBuffer( static_cast<TDes8&>( static_cast<CMMFDataBuffer*>(aBuffer)->Data() ) );

	}	
	
void CTestAppConsole::BufferFilledL( CMMFBuffer* aBuffer )
{
	RDebug::Print(_L("CTestAppConsole::BufferFilledL"));
	if ( static_cast<CMMFDataBuffer*>(aBuffer)->Data().Length() > 0 )
		PrintBuffer( static_cast<TDes8&>( static_cast<CMMFDataBuffer*>(aBuffer)->Data() ) );

}
// -----------------------------------------------------------------------------
// CMCCJitterBuffer::ConstructSinkL
// From MDataSink
// -----------------------------------------------------------------------------
//
void CTestAppConsole::ConstructSinkL( const TDesC8& /*aInitData*/ )
	{
	
	}

// -----------------------------------------------------------------------------
// CMCCJitterBuffer::SetSourceDataTypeCode
// 
// -----------------------------------------------------------------------------
//
TInt CTestAppConsole::SetSourceDataTypeCode( TFourCC /*aCodec*/, TMediaId /*aMedia*/ )
	{
	return KErrNone;	
	}

// -----------------------------------------------------------------------------
// CMCCJitterBuffer::SourceDataTypeCode
// From MDataSource
// -----------------------------------------------------------------------------
//
TFourCC CTestAppConsole::SourceDataTypeCode( TMediaId /*aMediaId*/ )
	{
	return NULL;
	}

// -----------------------------------------------------------------------------
// CMCCJitterBuffer::FillBufferL
// From MDataSource
// -----------------------------------------------------------------------------
//
void CTestAppConsole::FillBufferL( CMMFBuffer* aBuffer,
                      MDataSink* aConsumer,
                      TMediaId /*aMediaId*/ )
    {

}

void CTestAppConsole::BufferEmptiedL( CMMFBuffer* aBuffer )
{
	RDebug::Print(_L("CTestAppConsole::BufferEmptiedL"));
}

	
// -----------------------------------------------------------------------------
// CMCCJitterBuffer::CreateSourceBufferL
// From MDataSource
// -----------------------------------------------------------------------------
//
CMMFBuffer* CTestAppConsole::CreateSourceBufferL(TMediaId /*aMediaId*/, TBool &/*aReference*/)
	{
    // CMCCJitterBuffer can't create buffers
    User::Leave( KErrNotSupported );
	return NULL;
	}			  

// -----------------------------------------------------------------------------
// CMCCJitterBuffer::ConstructSourceL
// From MDataSource
// -----------------------------------------------------------------------------
//
void CTestAppConsole::ConstructSourceL( const TDesC8& /*aInitData*/ )
	{	
	
	}		
	
// -----------------------------------------------------------------------------
// CMCCJitterBuffer::CanCreateSourceBuffer
// From MDataSource
// -----------------------------------------------------------------------------
//
TBool CTestAppConsole::CanCreateSourceBuffer()
	{
    // CMCCJitterBuffer can't create buffers
	return EFalse;
	}

