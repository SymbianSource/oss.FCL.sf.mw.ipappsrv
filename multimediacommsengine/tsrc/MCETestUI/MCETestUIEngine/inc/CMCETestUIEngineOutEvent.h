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




#ifndef __CMCETESTUIENGINEOUTEVENT_H__
#define __CMCETESTUIENGINEOUTEVENT_H__

// INCLUDES
#include <e32base.h>
#include "CMCETestUIEngineEvent.h"

// FORWARD DECLARATIONS

class CMceOutEvent;
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

class TEventParams
		{
		public:
	//	 IMPORT_C TSessionParams(void);
		 TBuf<200>	iRecipentAddress;
		 TInt refreshInterval;
		 TBuf<100> originator;
		 TBuf<100> iEventHeader;
		};

class CMCETestUIEngineOutEvent : public CMCETestUIEngineEvent
    {
        
    public: // Constructors and destructors

	     
        /**
        * Two-phased constructor. Not imported.
        */
        static CMCETestUIEngineOutEvent* NewL(
            CMCETestUIEngine& aEngine, 
            CMCETestUIEngineProfile& aProfile );

		/**
        * Two-phased constructor. Not imported.
        */
        static CMCETestUIEngineOutEvent* NewL(
            CMCETestUIEngine& aEngine,
            CMceOutEvent* aOutEvent);
	    /**
	    * Two-phased constructor.
	    * @param aEvent, reference to existing event that is associated
	    *        with this session.
	    */
        //static CMCETestUIEngineOutSession* NewL(
        //    CMCETestUIEngine& aEngine, TDirection aDirection );


	    /**
	    * Two-phased constructor.
	    * @param aRefer, reference to existing refer that is associated
	    *        with this session.
	    */
        //static CMCETestUIEngineOutSession* NewL(
        //    CMCETestUIEngine& aEngine, TDirection aDirection );

            
    	/**
    	* Destructor.
    	*/
    	IMPORT_C virtual ~CMCETestUIEngineOutEvent();

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
	    CMceOutEvent& OutEvent();	
	    
	    TBool EventParamsL(TEventParams &aEventParams);

    public:
     	TEventParams iEventParams;
     	 

    private:  // private constructors
	    
        /**
        * C++ default constructor.
        */
        CMCETestUIEngineOutEvent();	

        /**
        * C++ first phase constructor.
        */
        CMCETestUIEngineOutEvent( 
                CMCETestUIEngine& aEngine,
                CMceOutEvent* aOutEvent);
                
        /**
        * C++ first phase constructor.
        */
        CMCETestUIEngineOutEvent( 
                CMCETestUIEngine& aEngine,
                CMCETestUIEngineProfile& aProfile);

       	/**
        * Symbian second-phase constructor
        */
        void ConstructL();
        
        void ConstructL(CMceOutEvent* aOutEvent);	
        
        // to fill profile registrar in the recipent
        
        void FillProfileRegistrar();   
        
        
    private:  // owned data
    
        // Wrapped CMCEOutSession object
        CMceOutEvent* iOutEvent;
 
    private:  // not owned data
        
        // Profile to be used in session creation
        CMCETestUIEngineProfile* iProfile;
        
        TInt iId;
                
        
        //MCE_UNIT_TEST_DEFS;
        
    };

#endif
