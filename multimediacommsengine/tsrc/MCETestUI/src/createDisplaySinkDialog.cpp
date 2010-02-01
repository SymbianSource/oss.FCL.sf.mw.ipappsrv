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
#include "createDisplaySinkDialog.h"
#include "CMCETestUIEngineVideoStream.h"
#include "CMCETestUIEngineVideoSink.h"
#include <MCEAudioStream.h>
#include <MCEDisplaySink.h>
#include <MCEMediaSink.h>

// CONSTANTS

const TInt KBufLength = 100;

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CCreateDisplaySinkDialog::CCreateDisplaySinkDialog( )
// .
// ----------------------------------------------------------------------------
//
CCreateDisplaySinkDialog::CCreateDisplaySinkDialog(
						 CMCETestUIEngineVideoStream& aStream, TInt aIndex ) :
    iStream(aStream),iSinkIndex( aIndex )
    {
    
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CCreateDisplaySinkDialog::PreLayoutDynInitL()
// .
// ----------------------------------------------------------------------------
//
void CCreateDisplaySinkDialog::PreLayoutDynInitL()
    {
    
    CAknForm::PreLayoutDynInitL();
    const RPointerArray<CMCETestUIEngineVideoSink>& sinks = 
            iStream.SinksL();
   
   	CMceMediaSink& mediaSink = static_cast<CMceMediaSink&>(sinks[iSinkIndex]->Sink());
    CMceDisplaySink& displaySink = static_cast<CMceDisplaySink&>(mediaSink);
    
    CEikNumberEditor* dispPrority = static_cast<CEikNumberEditor*>(
                                  Control( EDisplaySinkPriority ) );
    dispPrority->SetNumber(/*displaySink.DisplaysAvailable()*/displaySink.PriorityL());    
    
    CEikNumberEditor* dispSinkAvailable = static_cast<CEikNumberEditor*>(
                                  Control( EDisplaySinksAvailable ) );
    dispSinkAvailable->SetNumber(displaySink.DisplaysAvailable());    
    
  /*  CEikNumberEditor* dispSinkIndex = static_cast<CEikNumberEditor*>(
                                  Control( EDisplaySinkIndex ) );
    dispSinkIndex->SetNumber(0);  */  
    
    TRect rect = displaySink.DisplayRectL();
    TPoint tl = rect.iTl;
    TPoint br = rect.iBr;
    
    TInt rectX1  = tl.iX;
    TInt rectY1  = tl.iY;
    
    TInt rectX2  = br.iX;
    TInt rectY2  = br.iY;
    
    CEikNumberEditor* rectiX1 = static_cast<CEikNumberEditor*>(
                                  Control( EDisplaySinkRectIX1 ) );
    rectiX1->SetNumber(rectX1);    
    
    CEikNumberEditor* rectiY1 = static_cast<CEikNumberEditor*>(
                                  Control( EDisplaySinkRectIY1 ) );
    rectiY1->SetNumber(rectY1);    
    
    CEikNumberEditor* rectiX2 = static_cast<CEikNumberEditor*>(
                                  Control( EDisplaySinkRectIX2 ) );
    rectiX2->SetNumber(rectX2);    
    
    CEikNumberEditor* rectiY2 = static_cast<CEikNumberEditor*>(
                                  Control( EDisplaySinkRectIY2 ) );
    rectiY2->SetNumber(rectY2);    
      
    }

// ----------------------------------------------------------------------------
// CCreateDisplaySinkDialog::OkToExitL( TInt )
// .
// ----------------------------------------------------------------------------
//

TBool CCreateDisplaySinkDialog::OkToExitL( TInt aKey )
    {
    if ( aKey == EEikCmdCanceled )
        {
        // Cancel pressed. Just exit.
        return ETrue;
        }
    const RPointerArray<CMCETestUIEngineVideoSink>& sinks = 
            iStream.SinksL();
            
    if(sinks[iSinkIndex]->Type() == KMceDisplaySink)
    	{
    	CMceMediaSink& mediaSink = static_cast<CMceMediaSink&>(sinks[iSinkIndex]->Sink());
    	CMceDisplaySink& displaySink = static_cast<CMceDisplaySink&>(mediaSink);
    	
    	CEikNumberEditor* dispPrority = static_cast<CEikNumberEditor*>(
                                  Control( EDisplaySinkPriority ) );
	    displaySink.SetPriorityL(dispPrority->Number());
	    
	    /*
	    CEikNumberEditor* dispSinkIndex = static_cast<CEikNumberEditor*>(
	                                  Control( EDisplaySinkIndex ) );
	    displaySink.SetDisplayIndexL(dispSinkIndex->Number()); */  
	    
	    	    
	    CEikNumberEditor* rectiX1 = static_cast<CEikNumberEditor*>(
	                                  Control( EDisplaySinkRectIX1 ) );
	    TInt rectX1  = rectiX1->Number();    
	    
	    CEikNumberEditor* rectiY1 = static_cast<CEikNumberEditor*>(
	                                  Control( EDisplaySinkRectIY1 ) );
	    TInt rectY1  = rectiY1->Number();
	    
	    CEikNumberEditor* rectiX2 = static_cast<CEikNumberEditor*>(
	                                  Control( EDisplaySinkRectIX2 ) );
	    TInt rectX2  = rectiX2->Number();    
	    
	    CEikNumberEditor* rectiY2 = static_cast<CEikNumberEditor*>(
	                                  Control( EDisplaySinkRectIY2 ) );
	    TInt rectY2  = rectiY2->Number();    
	    
	    TPoint tl(rectX1,rectY1);
	    TPoint br(rectX2,rectY2);
	    
	    TRect rect(tl,br);
	    
	    displaySink.SetDisplayRectL(rect);
   	
    	}
    
    
       
    return ETrue;
    }

// ----------------------------------------------------------------------------
// CCreateDisplaySinkDialog::~CCreateDisplaySinkDialog()
// .
// ----------------------------------------------------------------------------
//
CCreateDisplaySinkDialog::~CCreateDisplaySinkDialog()
    {
    return;
    } 

// End of File
