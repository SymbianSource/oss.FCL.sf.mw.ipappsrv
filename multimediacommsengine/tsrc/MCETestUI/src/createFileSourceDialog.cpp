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


// INCLUDES
#include "createFileSourceDialog.h"
#include "CMCETestUIEngineAudioStream.h"
#include "CMCETestUIEngineVideoStream.h"
#include "CMCETestUIEngineSource.h"
#include "CMCETestUIEngineVideoSource.h"
#include <MCEAudioStream.h>
#include <MCEVideoStream.h>
#include <MCEFileSource.h>
#include <MCEMediaSource.h>
#include <MCEMediaStream.h>
#include "mcetestuiengineconstants.h"

// CONSTANTS
const TInt KBufLength = 300;
// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CCreateAudioFileSourceDialog::CCreateAudioFileSourceDialog( )
// .
// ----------------------------------------------------------------------------
//
CCreateAudioFileSourceDialog::CCreateAudioFileSourceDialog(
						 CMCETestUIEngineAudioStream& aStream, TInt aIndex ) :
    iStream(aStream),iSourceIndex( aIndex )
    {
    
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CCreateAudioFileSourceDialog::PreLayoutDynInitL()
// .
// ----------------------------------------------------------------------------
//
void CCreateAudioFileSourceDialog::PreLayoutDynInitL()
    {
    CAknForm::PreLayoutDynInitL();
    const RPointerArray<CMCETestUIEngineSource>& sources = 
            iStream.SourcesL();
       
   	CMceMediaSource& mediaSource = static_cast<CMceMediaSource&>(sources[iSourceIndex]->Source());
    CMceFileSource& fileSource = static_cast<CMceFileSource&>(mediaSource);
    	
   	TBuf<KBufLength> sourceFileName;
    sourceFileName.Copy(_L("None"));
    CEikEdwin* sourceFile = static_cast<CEikEdwin*>(
                                 Control( EFileSourceAudioFileUpdating ) );
   	sourceFile->SetTextL(&sourceFileName);
   	
   	CEikNumberEditor* fileposition = static_cast<CEikNumberEditor*>(
                                  Control( EFileSourceAudioFilePosition ) );
    TTimeIntervalMicroSeconds time1 = fileSource.PositionL();
    fileposition->SetNumber(time1.Int64()/1000000);  
    iPosition = time1.Int64()/1000000;
   
   	CEikNumberEditor* fileduration = static_cast<CEikNumberEditor*>(
                                  Control( EFileSourceAudioFileDuration ) );
    TTimeIntervalMicroSeconds time2 = fileSource.DurationL();
    fileduration->SetNumber(time2.Int64()/1000000);    
    
    CAknPopupFieldText* popupFieldText1 = 
        static_cast <CAknPopupFieldText*> (Control(EFileSourceAudioFileFastForward));
        
    CAknPopupFieldText* popupFieldText2 = 
        static_cast <CAknPopupFieldText*> (Control(EFileSourceAudioFileFastRewind));
        
    CEikNumberEditor* fileTransProgress = static_cast<CEikNumberEditor*>(
                                  Control( EFileSourceAudioFileTransProgress ) );
    fileTransProgress->SetNumber(fileSource.TranscodingProgressL());    
  
   	CEikNumberEditor* mediaelementCount = static_cast<CEikNumberEditor*>(
                                  Control( EFileSourceAudioFileMediaElementCount ) );
    mediaelementCount->SetNumber(fileSource.MediaElementCountL(1));    
    
    CEikNumberEditor* currentmediaElement = static_cast<CEikNumberEditor*>(
                                  Control( EFileSourceAudioFileCurrentMediaElement ) );
    currentmediaElement->SetNumber(fileSource.CurrentMediaElement(1));     
    }

// ----------------------------------------------------------------------------
// CCreateAudioFileSourceDialog::OkToExitL( TInt )
// .
// ----------------------------------------------------------------------------
//

TBool CCreateAudioFileSourceDialog::OkToExitL( TInt aKey )
    {
    if ( aKey == EEikCmdCanceled )
        {
        // Cancel pressed. Just exit.
        return ETrue;
        }
    const RPointerArray<CMCETestUIEngineSource>& sources = 
            iStream.SourcesL();
            
    if(sources[iSourceIndex]->Type() == KMceFileSource)
    	{
    	CMceMediaSource& mediaSource = static_cast<CMceMediaSource&>(sources[iSourceIndex]->Source());
    	CMceFileSource& fileSource = static_cast<CMceFileSource&>(mediaSource);
    	TBuf<KBufLength> buffer;
    	CEikEdwin* sourceFile = static_cast<CEikEdwin*>(
                                 Control( EFileSourceAudioFileUpdating ) );
    	sourceFile->GetText(buffer);
		if(buffer.Compare(KNone))
		{
			fileSource.UpdateL(buffer);
		}
       
       	
   		CEikNumberEditor* fileposition = static_cast<CEikNumberEditor*>(
                              Control( EFileSourceAudioFilePosition ) );
        
        if(iPosition != fileposition->Number())
        {
       		fileSource.SetPositionL(fileposition->Number()*1000000);	
        }
        
       	
      /* 	CAknPopupFieldText* popupFieldText1 = 
        static_cast <CAknPopupFieldText*> (Control(EFileSourceAudioFileFastForward));
        if(popupFieldText1->CurrentValueIndex())
        	{
        	fileSource.SetFastForwardL(ETrue);
        	}
        else
        	{
        	fileSource.SetFastForwardL(EFalse);
        	}
       
       	CAknPopupFieldText* popupFieldText2 = 
        static_cast <CAknPopupFieldText*> (Control(EFileSourceAudioFileFastRewind));
         if(popupFieldText2->CurrentValueIndex())
        	{
        	fileSource.SetFastRewindL(ETrue);
        	}
        else
        	{
        	fileSource.SetFastRewindL(EFalse);
        	}
        	*/
        CEikNumberEditor* currentmediaElement = static_cast<CEikNumberEditor*>(
                                  Control( EFileSourceAudioFileCurrentMediaElement ) );
    	
    	fileSource.SetCurrentMediaElementL(1,currentmediaElement->Number());
        }
    
        
    return ETrue;
    }

// ----------------------------------------------------------------------------
// CCreateAudioFileSourceDialog::~CCreateAudioFileSourceDialog)
// .
// ----------------------------------------------------------------------------
//
CCreateAudioFileSourceDialog::~CCreateAudioFileSourceDialog()
    {
    return;
    } 


// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CCreateVideoFileSourceDialog::CCreateVideoFileSourceDialog( )
// .
// ----------------------------------------------------------------------------
//
CCreateVideoFileSourceDialog::CCreateVideoFileSourceDialog(
						 CMCETestUIEngineVideoStream& aStream, TInt aIndex ) :
    iStream(aStream),iSourceIndex( aIndex )
    {
    
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CCreateVideoFileSourceDialog::PreLayoutDynInitL()
// .
// ----------------------------------------------------------------------------
//
void CCreateVideoFileSourceDialog::PreLayoutDynInitL()
    {
    CAknForm::PreLayoutDynInitL();
    const RPointerArray<CMCETestUIEngineVideoSource>& sources = 
            iStream.SourcesL();
   
   	CMceMediaSource& mediaSource = static_cast<CMceMediaSource&>(sources[iSourceIndex]->Source());
    CMceFileSource& fileSource = static_cast<CMceFileSource&>(mediaSource);
        
    TBuf<KBufLength> sourceFileName;
    sourceFileName.Copy(_L("None"));
    CEikEdwin* sourceFile = static_cast<CEikEdwin*>(
                                 Control( EFileSourceVideoFileUpdating ) );
   	sourceFile->SetTextL(&sourceFileName);  
   	
   	CEikNumberEditor* fileposition = static_cast<CEikNumberEditor*>(
                                  Control( EFileSourceVideoFilePosition ) );
    TTimeIntervalMicroSeconds time1 = fileSource.PositionL();
    fileposition->SetNumber(time1.Int64()/1000000);    
   	iPosition = time1.Int64()/1000000;
   	
   	CEikNumberEditor* fileduration = static_cast<CEikNumberEditor*>(
                                  Control( EFileSourceVideoFileDuration ) );
    TTimeIntervalMicroSeconds time2 = fileSource.DurationL();
    fileduration->SetNumber(time2.Int64()/1000000);    
    
    CAknPopupFieldText* popupFieldText1 = 
        static_cast <CAknPopupFieldText*> (Control(EFileSourceVideoFileFastForward));
        
    CAknPopupFieldText* popupFieldText2 = 
        static_cast <CAknPopupFieldText*> (Control(EFileSourceVideoFileFastRewind));
        
//    CEikNumberEditor* fileTransProgress = static_cast<CEikNumberEditor*>(
//                                  Control( EFileSourceVideoFileTransProgress ) );
//    TInt streamState = iStream.State();
//	if(streamState == CMceMediaStream::ETranscodingRequired) 
//		{
//		fileposition->SetNumber(fileSource.TranscodingProgressL());  	
//		}
    
  	CEikNumberEditor* mediaelementCount = static_cast<CEikNumberEditor*>(
                                  Control( EFileSourceVideoFileMediaElementCount ) );
    mediaelementCount->SetNumber(fileSource.MediaElementCountL(2));    
    
    CEikNumberEditor* currentmediaElement = static_cast<CEikNumberEditor*>(
                                  Control( EFileSourceVideoFileCurrentMediaElement ) );
    currentmediaElement->SetNumber(fileSource.CurrentMediaElement(2));  
    }

// ----------------------------------------------------------------------------
// CCreateVideoFileSourceDialog::OkToExitL( TInt )
// .
// ----------------------------------------------------------------------------
//

TBool CCreateVideoFileSourceDialog::OkToExitL( TInt aKey )
    {
    if ( aKey == EEikCmdCanceled )
        {
        // Cancel pressed. Just exit.
        return ETrue;
        }
    const RPointerArray<CMCETestUIEngineVideoSource>& sources = 
            iStream.SourcesL();
            
    if(sources[iSourceIndex]->Type() == KMceFileSource)
    	{
    	CMceMediaSource& mediaSource = static_cast<CMceMediaSource&>(sources[iSourceIndex]->Source());
    	CMceFileSource& fileSource = static_cast<CMceFileSource&>(mediaSource);
    	TBuf<KBufLength> buffer;
    	CEikEdwin* sourceFile = static_cast<CEikEdwin*>(
                                 Control( EFileSourceVideoFileUpdating ) );
    	sourceFile->GetText(buffer);
    	if(buffer.Compare(KNone))
    		{
    		fileSource.UpdateL(buffer);
    		}
       	       	
       	CEikNumberEditor* fileposition = static_cast<CEikNumberEditor*>(
                              Control( EFileSourceVideoFilePosition ) );
        if(iPosition != fileposition->Number())
        {
       		fileSource.SetPositionL(fileposition->Number()*1000000);	
        }
       	
       	CAknPopupFieldText* popupFieldText1 = 
        static_cast <CAknPopupFieldText*> (Control(EFileSourceVideoFileFastForward));
        if(popupFieldText1->CurrentValueIndex())
        	{
        	fileSource.SetFastForwardL(ETrue);
        	}
        else
        	{
        	fileSource.SetFastForwardL(EFalse);
        	}
       
       	CAknPopupFieldText* popupFieldText2 = 
        static_cast <CAknPopupFieldText*> (Control(EFileSourceVideoFileFastRewind));
         if(popupFieldText2->CurrentValueIndex())
        	{
        	fileSource.SetFastRewindL(ETrue);
        	}
        else
        	{
        	fileSource.SetFastRewindL(EFalse);
        	}
        CEikNumberEditor* currentmediaElement = static_cast<CEikNumberEditor*>(
                                  Control( EFileSourceVideoFileCurrentMediaElement ) );
    	TInt index = fileSource.CurrentMediaElement(2); 
    	fileSource.SetCurrentMediaElementL(2,currentmediaElement->Number());
        }
    
    
        
    return ETrue;
    }

// ----------------------------------------------------------------------------
// CCreateVideoFileSourceDialog::~CCreateVideoFileSourceDialog)
// .
// ----------------------------------------------------------------------------
//
CCreateVideoFileSourceDialog::~CCreateVideoFileSourceDialog()
    {
    return;
    } 

// End of File
