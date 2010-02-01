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




#ifndef __CMCETESTUIENGINEVIDEOSOURCE_H__
#define __CMCETESTUIENGINEVIDEOSOURCE_H__

// INCLUDES

#include "MMCETestUIEngineCmdProvider.h"
#include <e32base.h>
#include <MceMediaSource.h>
// FORWARD DECLARATIONS

class CMCETestUIEngine;
class CMceVideoStream;
class CMCETestUIEngineVideoStream;
class CMCETestUIEngineSession;


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
class CMCETestUIEngineVideoSource : public CBase,
                                    public MMCETestUIEngineCmdProvider
    {

    public: // public enumerations
    
    public: // Constructors and destructors
            
    	/**
    	* Destructor.
    	*/
    	IMPORT_C virtual ~CMCETestUIEngineVideoSource();

       /**
        * Two-phased constructor. Not imported.
        */
        static CMCETestUIEngineVideoSource* NewL(
            CMCETestUIEngine& aEngine, 
            CMceMediaSource& aSource );
            
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
        
     //   IMPORT_C const TMCESourceType Type() const;
	    	    
    public: // New functions, internal

        /**
        * Gets wrapped MCEAudioStream object
        * 
        * @return wrapped MCEMCEAudioStream
        */
       
        IMPORT_C CMceMediaSource& Source();
        /**
        *
        * Solves and sets the direction of session based on streams and
        * their properties.
        *
        * @return the newly set direction of session
        */
               
        IMPORT_C TMceSourceType Type() const;
        
    private:  // private constructors
	    
        /**
        * C++ default constructor. Not implemented.
        */
        CMCETestUIEngineVideoSource();	
    
    protected:
    
        /**
        * C++ first phase constructor.
        */
        CMCETestUIEngineVideoSource( 
                    CMCETestUIEngine& aEngine, 
            CMceMediaSource& aSource );

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
       CMceMediaSource& iSource;

      
        
    };

#endif
