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
#include "createRtpSinkDialog.h"
#include "CMCETestUIEngineAudioStream.h"
#include "CMCETestUIEngineSink.h"
#include <MCEAudioStream.h>
#include <MCERtpSink.h>
#include <MCEMediaSink.h>
#include <MCESpeakerSink.h>
// CONSTANTS

const TInt KBufLength = 100;

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CCreateRtpSinkDialog::CCreateRtpSinkDialog( )
// .
// ----------------------------------------------------------------------------
//
CCreateRtpSinkDialog::CCreateRtpSinkDialog(
						 CMCETestUIEngineAudioStream& aStream, TInt aIndex ) :
    iStream(aStream),iSinkIndex( aIndex )
    {
    
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CCreateRtpSinkDialog::PreLayoutDynInitL()
// .
// ----------------------------------------------------------------------------
//
void CCreateRtpSinkDialog::PreLayoutDynInitL()
    {
    const RPointerArray<CMCETestUIEngineSink>& sinks = 
            iStream.SinksL();
   
   	CMceMediaSink& mediaSink = static_cast<CMceMediaSink&>(sinks[iSinkIndex]->Sink());
    CMceRtpSink& rtpSink = static_cast<CMceRtpSink&>(mediaSink);
    	
    CAknPopupFieldText* popupFieldText = 
        static_cast <CAknPopupFieldText*> (Control(ERtpSinkSuppressRtcp));
   
    if(rtpSink.IsEnabled())
    	{
    	popupFieldText->SetCurrentValueIndex(0);
       	}
    else
    	{
    	popupFieldText->SetCurrentValueIndex(1);
    	}
    
    CAknForm::PreLayoutDynInitL();
    }

// ----------------------------------------------------------------------------
// CCreateAudioCodecDialog::OkToExitL( TInt )
// .
// ----------------------------------------------------------------------------
//

TBool CCreateRtpSinkDialog::OkToExitL( TInt aKey )
    {
    if ( aKey == EEikCmdCanceled )
        {
        // Cancel pressed. Just exit.
        return ETrue;
        }
    const RPointerArray<CMCETestUIEngineSink>& sinks = 
            iStream.SinksL();
            
    if(sinks[iSinkIndex]->Type() == KMceRTPSink)
    	{
    	CMceMediaSink& mediaSink = static_cast<CMceMediaSink&>(sinks[iSinkIndex]->Sink());
    	CMceRtpSink& rtpSink = static_cast<CMceRtpSink&>(mediaSink);
    	CAknPopupFieldText* popupFieldText = 
        					static_cast <CAknPopupFieldText*> (Control(ERtpSinkSuppressRtcp));
   
	    if( popupFieldText->CurrentValueIndex() == 0)
	    	{
	    	rtpSink.EnableL();
	    	}
	    else
	    	{
	    	rtpSink.DisableL();
	    	}	
    	}
    
    
       
    return ETrue;
    }

// ----------------------------------------------------------------------------
// CCreateRtpSinkDialog::~CCreateRtpSinkDialog()
// .
// ----------------------------------------------------------------------------
//
CCreateRtpSinkDialog::~CCreateRtpSinkDialog()
    {
    return;
    } 

// End of File
