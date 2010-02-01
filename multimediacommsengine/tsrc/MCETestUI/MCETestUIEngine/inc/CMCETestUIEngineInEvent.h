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




#ifndef __CMCETESTUIENGINEINEVENT_H__
#define __CMCETESTUIENGINEINEVENT_H__

// INCLUDES
#include <e32base.h>
#include "TMCETestUIEngineCmdBase.h"
#include "CMCETestUIEngine.h"
#include "cmcetestuiengineevent.h"

// FORWARD DECLARATIONS

class CMceInEvent;


// CLASS DECLARATION
/**
* 
* CMCETestUIEngineInSession provides a wrapper for CMCEInSession.
* Class is not the traditional wrapper in a sense that it would 
* conform to interface provided by CMCEInSession. Instead class
* implements interface MMCETestUIEngineCmdProvider to provide
* same kind of functionality in controlled way on different levels 
* of abstraction.
*
*/
class CMCETestUIEngineInEvent : public CMCETestUIEngineEvent
    {
        
    public: // Constructors and destructors


        /**
        * Two-phased constructor. Not imported.
        */
        static CMCETestUIEngineInEvent* NewL(
            CMCETestUIEngine& aEngine, 
            CMceInEvent* aInEvent );
            
    	/**
    	* Destructor.
    	*/
    	IMPORT_C ~CMCETestUIEngineInEvent();

    public: // From MMCETestUICommandProvider

        /**
        * Gets the commands applicaple to this insession object.
        *
        * @return reference to array of commands
        */
        IMPORT_C const RPointerArray<TMCETestUIEngineCmdBase>& 
                    GetCommandsL();
	
    public: // From base class 
    
        /**
        * @return Id of this particular insession object
        */    
        IMPORT_C TInt Id() const;        	
	    
    public: // New functions, internal

        /**
        * Gets wrapped MCEInSession object
        * 
        * @return wrapped MCEInSession
        */    
	    CMceInEvent& InEvent();
    
    private:  // private constructors
	    
        /**
        * C++ default constructor. Not implemented.
        */
        CMCETestUIEngineInEvent();	

        /**
        * C++ first phase constructor.
        */
        CMCETestUIEngineInEvent( 
                CMCETestUIEngine& aEngine,
                CMceInEvent* aInEvent );

       	/**
        * Symbian second-phase constructor
        */
        void ConstructL();	   
        
        
    private:  // owned data

        // Wrapped CMCEInSession object
        CMceInEvent* iInEvent;
       
        // Id of this insession object
        TInt iId;   
        
    };

#endif // __CMCETESTUIENGINEINSESSION_H__
