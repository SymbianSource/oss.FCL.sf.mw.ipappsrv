/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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


////////////////////////////////////////////////////////////////////////
//
// CMCETestUIAppUi
//
////////////////////////////////////////////////////////////////////////

#ifndef __MCETESTUIAPPUI_H
#define __MCETESTUIAPPUI_H

//System Includes
#include <aknviewappui.h>
#include <eikmenup.h>
#include <eikbctrl.h>
#include <eiktxlbx.h>
#include <eiktxlbm.h>
#include <barsread.h>
#include <coemain.h>
#include <aknlists.h>

#include <aknviewappui.h>// AppUi()
#include <aknlists.h>
#include <akngloballistquery.h>
#include <aknselectionlist.h>
#include <AknNoteWrappers.h>
#include <AknQueryDialog.h>
#include <AknProgressDialog.h>
#include <AknWaitNoteWrapper.h> 
#include <AknGlobalNote.h>

//Eik
#include <eikenv.h>
#include <eikmenup.h> //DynInitMenuPaneL

//
#include <barsread.h> //TResourceReader
#include <StringLoader.h>

#include "MMCETestUIUserInterface.h"
#include "CMCETestUIQuestionSingleSelection.h"
#include "CMCETestUIQuestionDataQuery.h"
#include "CMCETestUIEngine.h"
//User Includes
#include "MCETestUI.hrh"
#include <MCETestUI.rsg>


//FORWARD DECLARATIONS
class CMCETestUIEngine;
class CMCETestUIAppView;
class CMCETestUISessionView;
class CMCETestUIStreamView;
class TSessionParams;
class TEventParams;
class TSessionEstablishValues;
class TSessionResponseValues;
class TReferParams;
class CMCETestUIActivityManager;

class CMCETestUIAppUi : public CAknViewAppUi, public MMCETestUIUserInterface	
    {
	public:
	    CMCETestUIAppUi( CMCETestUIEngine& aEngine ); 
	    void ConstructL();
		~CMCETestUIAppUi();


	public: // from class MMCETestUIUserInterface
	
        void EngineStateChangedL();
        void NotifyUser( const TDesC& aMessage );
        void NotifyUserWaitL ( const TDesC& aMessage );
        TInt AskQuestionL( const CMCETestUIQuestionSingleSelection& aQuestion );
        void AskQuestionL( CMCETestUIQuestionDataQuery& aQuestion );
        TBool SessionParamsL(TSessionParams &aSessionParams,TSessionType aSessionType);        	    				;
        TBool EventParamsL(TEventParams &aEventParams);
        TBool ReferParamsL(TReferParams &aReferParams);
        TBool ReferToParamsL(TReferParams &aReferParams);
        TBool SessionResponseValues(TSessionResponseValues &responseValues);
        TBool SessionEstablishValues(TSessionEstablishValues &sessionValues);
        TBool NotifyValues(TNotifyValues &notifyValues);
        void ChangeToMainViewL();
        void CreateMceSettingsL();
        void CreateMceValueSettingsL();
        TBool IsVideoEnabled();
        void EnableVideo();
        void DisableVideo();
        TBool StartDTMFToneL( TChar& aChar );
        
        TBool SendDTMFToneL( TChar& aChar, TInt& aInterval );
        
        TBool SendDTMFToneSequenceL( TDes& aSequence );
        
        TBool GetIapIdL( TInt &aNumber);
        
        TBool GetCodecTypeL( TInt &aNumber);
        
        void ShowCurrentCryptoContextsL(TBool aCrypto32,TBool aCrypto80);
        
        void ShowSupportedCryptoContextsL(TBool aCrypto32,TBool aCrypto80);
        
        void GetCryptoContextsL(TBool& aCrypto32,TBool& aCrypto80);
        
        TRect GetRect();

        TInt iCurrentSessionViewIndex;
        TInt iCurrentStreamViewIndex;
        
        TInt iEnableVideo;
	private: // Inherited from class CEikAppUi
		void HandleCommandL(TInt aCommand);
		
		void ProcessCommandL(TInt aCommand);
        
        
    protected:
	    //From CCoeEnv
	    //void HandleForegroundEventL(TBool aForeground);

    private: // C++ default contructor
    
        CMCETestUIAppUi(); // Not implemented 

	private: // Data
	
		CMCETestUIAppView* iMainView;
		CMCETestUISessionView* iSessionView;
		CMCETestUIStreamView* iStreamView;
	    CMCETestUIEngine& iEngine;
	    CMCETestUIActivityManager* iActivityManager;
		
	};


#endif
