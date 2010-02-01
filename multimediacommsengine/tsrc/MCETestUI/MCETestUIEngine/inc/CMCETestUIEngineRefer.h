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




#ifndef __CMCETESTUIENGINEREFER_H__
#define __CMCETESTUIENGINEREFER_H__

// USER INCLUDES

// SYSTEM INCLUDES
#include <e32base.h>


// FORWARD DECLARATIONS

class CMceManager;
class CMceFCMsgExchange;
class CMceEvent;
class CMCETestUIEngine;
class CMCETestUIEngineCmdBase;
class CMCETestUIEngineProfile;
class CMceRefer;

// CLASS DECLARATION
/**
* 
* CMCETestUIEngineSession provides a wrapper for CMCESession.
* Class is not the traditional wrapper in a sense that it would 
* conform to interface provided by CMCESession. Instead class
* implements interface MMCETestUIEngineCmdProvider to provide
* same kind of functionality in controlled way on different levels 
* of abstraction.
*
* TODO: Wrappers for streams and use of them.
*
*/
class CMCETestUIEngineRefer : public CBase,
                                public MMCETestUIEngineCmdProvider
    {

    public: // public enumerations

       /**
        * Enumeration for defining session's direction
        */
        
    public: // Constructors and destructors
            
    	/**
    	* Destructor.
    	*/
    	IMPORT_C virtual ~CMCETestUIEngineRefer();

    public: // From MMCETestUICommandProvider

        /**
        * Gets the commands applicaple to this session object.
        *
        * @return reference to array of commands
        */
        IMPORT_C const RPointerArray<TMCETestUIEngineCmdBase>& 
                    GetCommandsL();
	
    public: // New functions, pure virtual

        /**
        * @return Id of this particular session object
        */
        IMPORT_C virtual TInt Id() const = 0;

    public: // New functions
        
        /**
        * @return Textual representation of the state of the wrapped MCESession
        */
	    IMPORT_C const TDesC& State() ;
	    
	    	    	    
    public: // New functions, internal

        /**
        * Gets wrapped MCESession object
        * 
        * @return wrapped MCESession
        */
        IMPORT_C CMceRefer& Refer();		
        
       
        
    private:  // private constructors
	    
        /**
        * C++ default constructor. Not implemented.
        */
        CMCETestUIEngineRefer();	
    
    protected:
    
        /**
        * C++ first phase constructor.
        */
        CMCETestUIEngineRefer( 
                    CMCETestUIEngine& aEngine );

       	/**
        * Symbian second-phase constructor
        */
        void BaseConstructL();	   
        
    protected:  // owned data  
    
        // Commands applicable to all sessions. Owned.
       RPointerArray<TMCETestUIEngineCmdBase> iCommands;

               
    protected:  // not owned data
        
       CMCETestUIEngine& iEngine;       
    
       // Pointer to wrapped OutSession/InSession
       CMceRefer* iRefer;
       
      
    };

#endif
