/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    See class definition below.
*
*/





#ifndef __MMCETESTUIUSERINTERFACE_H
#define __MMCETESTUIUSERINTERFACE_H

#include "CMCETestUIQuestionSingleSelection.h"
#include "CMCETestUIQuestionDataQuery.h"
#include "CMCETestUIEngine.h"


/**
* 
* Interface description: 
*
* Class defines interface to communicate between UI and UI engine.
*
*/
class MMCETestUIUserInterface
    {
    
    public: // Functions to give information to user

	    /**
	    * Informs user interface that internal state of engine has changed. 
	    */       
	    virtual void EngineStateChangedL() = 0;
	    
	     
	    /**
	    * Notifies user. 
	    * @param aMessage notification to be shown to user
	    */     
        virtual void NotifyUser( const TDesC& aMessage ) = 0;
        
	    /**
	    * Notifies user. The note has to be dismissed by the user.
	    * @param aMessage notification to be shown to user
	    */     
        virtual void NotifyUserWaitL( const TDesC& aMessage ) = 0;

        /////////////////////////////////////// 
        //
        // TODO: Functions to inform and warn user 
        //
        ////////////////////////////////////////
        
    public: // Functions to retrieve information from user
        
        /**
        * Asks user to select one of multiple choices.
        * 
        * @param aQuestion data structure holding the question and choices.
        * @return index of chosen choice. Negative if canceled or otherwise not
        *         answered. 
        */ 
        virtual TInt AskQuestionL( 
                const CMCETestUIQuestionSingleSelection& aQuestion ) = 0;
                
        /**
        * Asks user to input single textual value
        * 
        * @param aQuestion data structure holding the question,
        *                  default value and answering function.
        */                 
        virtual void AskQuestionL( CMCETestUIQuestionDataQuery& aQuestion ) = 0;
        
        virtual TBool SessionParamsL(TSessionParams& aSessionParams, TSessionType aSessionType) = 0;
        				
        virtual TBool SessionEstablishValues(TSessionEstablishValues &sessionValues) = 0;
        
        virtual TBool SessionResponseValues(TSessionResponseValues &responseValues) = 0;
        
        virtual TBool NotifyValues(TNotifyValues &responseValues) = 0;
        				
        virtual TBool EventParamsL(TEventParams& aEventParams) = 0;
        
        virtual TBool ReferParamsL(TReferParams& aReferParams) = 0;
        
        virtual TBool ReferToParamsL(TReferParams& aReferParams) = 0;
        
        virtual void ChangeToMainViewL() = 0;
        
        virtual void CreateMceSettingsL() = 0;
        
        virtual void CreateMceValueSettingsL() = 0;
        
        virtual TBool IsVideoEnabled() = 0;
        
        virtual void EnableVideo() = 0;
        
        virtual void DisableVideo() = 0;
        
        virtual TBool StartDTMFToneL( TChar& aChar ) = 0;
        
        virtual TBool SendDTMFToneL( TChar& aChar, TInt& aInterval ) = 0;
        
        virtual TBool SendDTMFToneSequenceL( TDes& aSequence  ) = 0;
        
        virtual TBool GetIapIdL( TInt& aIapId  ) = 0;
        
        virtual TBool GetCodecTypeL( TInt& aCodecIndex  ) = 0;
        
        virtual void ShowCurrentCryptoContextsL(TBool aCrypto32,TBool aCrypto80) = 0;
        
        virtual void ShowSupportedCryptoContextsL(TBool aCrypto32,TBool aCrypto80) = 0;
        
        virtual void GetCryptoContextsL(TBool& aCrypto32,TBool& aCrypto80) = 0;
        
        virtual TRect GetRect() = 0;

    };
    
#endif	// __MMCETESTUIUSERINTERFACE_H

