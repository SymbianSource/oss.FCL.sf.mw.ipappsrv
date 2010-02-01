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
* Description:    
*
*/




#ifndef __CMCETESTUIENGINEOUTREFER_H__
#define __CMCETESTUIENGINEOUTREFER_H__

// INCLUDES
#include <e32base.h>
#include "CMCETestUIEngineRefer.h"

// FORWARD DECLARATIONS

class CMceOutRefer;
class CMCETestUIEngine;
class CMCETestUIEngineCmdBase;

// CLASS DECLARATION
/**
* 
* CMCETestUIEngineOutSession provides a wrapper for CMCEOutSession.
* Class is not the traditional wrapper in a sense that it would 
* conform to interface provided by CMCEOutSession. Instead class
* implements interface MMCETestUIEngineCmdProvider to provide
* same kind of functionality in controlled way on different levels 
* of abstraction.
*
*/

class TReferParams
		{
		public:
		 TBuf<200>	iRecipentAddress;
		 TBuf<200>	iReferTo;
		 TInt type;
		 TBuf<100> originator;
		};

class CMCETestUIEngineOutRefer : public CMCETestUIEngineRefer
    {
        
    public: // Constructors and destructors

	     
        /**
        * Two-phased constructor. Not imported.
        */
        static CMCETestUIEngineOutRefer* NewL(
            CMCETestUIEngine& aEngine, 
            CMCETestUIEngineProfile& aProfile );

		static CMCETestUIEngineOutRefer* NewL(
            CMCETestUIEngine& aEngine, 
            CMCETestUIEngineSession& aSession );

		static CMCETestUIEngineOutRefer* NewL(
            CMCETestUIEngine& aEngine, 
            CMCETestUIEngineEvent& aEvent );

	                
    	/**
    	* Destructor.
    	*/
    	IMPORT_C virtual ~CMCETestUIEngineOutRefer();

    public: // From MMCETestUICommandProvider

       /**
        * Gets the commands applicaple to this outsession object.
        *
        * @return reference to array of commands
        */
        IMPORT_C const RPointerArray<TMCETestUIEngineCmdBase>& 
                    GetCommandsL();


    public: // From base class 

        /**
        * @return Id of this particular outsession object
        */        
        IMPORT_C TInt Id() const;
	    
    public: // New functions, internal

        /**
        * Gets wrapped MCEOutSession object
        * 
        * @return wrapped MCEOutSession
        */    	    
	    CMceOutRefer& OutRefer();	
	    
	    TBool ReferParamsL(TReferParams &aReferParams);
	    
	    TBool ReferToParamsL(TReferParams &aReferParams);

    public:
     	TReferParams iReferParams;
     	 

    private:  // private constructors
	    
        /**
        * C++ default constructor.
        */
        CMCETestUIEngineOutRefer();	

        /**
        * C++ first phase constructor.
        */
        CMCETestUIEngineOutRefer( 
                CMCETestUIEngine& aEngine,
                CMCETestUIEngineProfile& aProfile);
                
		CMCETestUIEngineOutRefer( 
                CMCETestUIEngine& aEngine,
                CMCETestUIEngineSession& aSession);
        CMCETestUIEngineOutRefer( 
                CMCETestUIEngine& aEngine,
                CMCETestUIEngineEvent& aEvent);
       	/**
        * Symbian second-phase constructor
        */
        void ConstructL(CMCETestUIEngineProfile& aProfile);	
        
        void ConstructL(CMCETestUIEngineSession& aSession);	
        
        void ConstructL(CMCETestUIEngineEvent& aEvent);	
        
        
        // to fill profile registrar in the recipent
        
        void FillProfileRegistrar();   
        
        
    private:  // owned data
    
        // Wrapped CMCEOutSession object
        CMceOutRefer* iOutRefer;
 
    private:  // not owned data
        
        // Profile to be used in session creation
        CMCETestUIEngineProfile* iProfile;
        
        CMCETestUIEngineSession* iSession;
        
        CMCETestUIEngineEvent* iEvent;
        
        TInt iId;
                
        
        //MCE_UNIT_TEST_DEFS;
        
    };

#endif
