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




#ifndef __CMCETESTUIENGINESINK_H__
#define __CMCETESTUIENGINESINK_H__

// INCLUDES

#include "MMCETestUIEngineCmdProvider.h"
#include <e32base.h>
#include <MCEMediaSink.h>
// FORWARD DECLARATIONS

class CMCETestUIEngine;
class CMceAudioStream;
class CMCETestUIEngineSession;
class CMceMediaSink;

// CLASS DECLARATION
/**
* 
* CMCETestUIEngineAudioStream provides a wrapper for CMCEAudioStream.
* Class is not the traditional wrapper in a sense that it would 
* conform to interface provided by CMCESession. Instead class
* implements interface MMCETestUIEngineCmdProvider to provide
* same kind of functionality in controlled way on different levels 
* of abstraction.
*
*/
class CMCETestUIEngineSink : public CBase,
                                    public MMCETestUIEngineCmdProvider
    {

    public: // public enumerations
    
        
    public: // Constructors and destructors
            
    	/**
    	* Destructor.
    	*/
    	IMPORT_C virtual ~CMCETestUIEngineSink();

       /**
        * Two-phased constructor. Not imported.
        */
        static CMCETestUIEngineSink* NewL(
            CMCETestUIEngine& aEngine, 
            CMceMediaSink& aSink );
            
    public: // From MMCETestUICommandProvider

        /**
        * Gets the commands applicaple to this session object.
        *
        * @return reference to array of commands
        */
        IMPORT_C const RPointerArray<TMCETestUIEngineCmdBase>& 
                    GetCommandsL();

    public: // New functions
	    
	     
	    
	    /**
	    * Gets the state of the stream
	    * @return Textual representation of streams state
	    */
        IMPORT_C const TDesC16& State() const;
        
      	IMPORT_C const TMceSinkType Type() const;
	    	    
    public: // New functions, internal

        /**
        * Gets wrapped MCEAudioStream object
        * 
        * @return wrapped MCEMCEAudioStream
        */
        IMPORT_C CMceMediaSink& Sink();		
        

     
            
    private:  // private constructors
	    
        /**
        * C++ default constructor. Not implemented.
        */
        CMCETestUIEngineSink();	
    
    protected:
    
        /**
        * C++ first phase constructor.
        */
        CMCETestUIEngineSink( 
                   CMCETestUIEngine& aEngine, 
            	   CMceMediaSink& aSink );

       	/**
        * Symbian second-phase constructor
        */
        void ConstructL();	   
        
    protected:  // owned data  
    
        // Commands applicable to an audiostream. Owned.
       RPointerArray<TMCETestUIEngineCmdBase> iCommands;
 
    protected:  // not owned data
        
       CMCETestUIEngine& iEngine;       
    
       // Reference to wrapped MCEAudioStream
       CMceMediaSink& iSink;

        
    };

#endif
