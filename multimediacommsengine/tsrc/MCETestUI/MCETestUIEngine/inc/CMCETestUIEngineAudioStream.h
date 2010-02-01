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




#ifndef __CMCETESTUIENGINEAUDIOSTREAM_H__
#define __CMCETESTUIENGINEAUDIOSTREAM_H__

// INCLUDES
#include <MCEAudioCodec.h>
#include <MCEMediaSource.h>
#include <MCEMediaSink.h>

#include "MMCETestUIEngineCmdProvider.h"
#include "CMCETestUIEngineAudioStream.h"
#include <e32base.h>

// FORWARD DECLARATIONS

class CMCETestUIEngine;
class CMceAudioStream;
class CMCETestUIEngineSink;
class CMCETestUIEngineSource;
class CMCETestUIEngineCodec;


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
class CMCETestUIEngineAudioStream : public CBase,
                                    public MMCETestUIEngineCmdProvider
    {

    public: // public enumerations
    
        enum TDirection
            {
            EUpLink = 0,
            EDownLink,
            ENotSpesified            
            };
        
    public: // Constructors and destructors
            
    	/**
    	* Destructor.
    	*/
    	IMPORT_C virtual ~CMCETestUIEngineAudioStream();

       /**
        * Two-phased constructor. Not imported.
        */
        static CMCETestUIEngineAudioStream* NewL(
            CMCETestUIEngine& aEngine, 
            CMceAudioStream& aAudioStream );
            
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
        * @return Textual representation of the state of the wrapped MCEStream
        */
	    IMPORT_C const TDesC16& StreamState() ;
	    
	    /**
	    * @return Direction of the session
	    */
	    IMPORT_C CMCETestUIEngineAudioStream::TDirection Direction() const;
	    
	    /**
	    * @return Textual representation of session's direction
	    */
	    IMPORT_C const TDesC16& TextualDirection() const;

	    /**
	    * Gets the state of the stream
	    * @return Textual representation of streams state
	    */
        IMPORT_C const TDesC16& State() const;
        
        
        IMPORT_C const RPointerArray<CMCETestUIEngineSource>& SourcesL();
        
        IMPORT_C const RPointerArray<CMCETestUIEngineSink>& SinksL();
        
        IMPORT_C RPointerArray<CMCETestUIEngineCodec>& CodecsL(); 
        
        IMPORT_C RPointerArray<CMCETestUIEngineCodec>& CurrentCodecs();       
        
        	    
    public: // New functions, internal

        /**
        * Gets wrapped MCEAudioStream object
        * 
        * @return wrapped MCEMCEAudioStream
        */
        CMceAudioStream& AudioStream();		
        
        /**
        *
        * Solves and sets the direction of session based on streams and
        * their properties.
        *
        * @return the newly set direction of session
        */
        
        CMCETestUIEngineAudioStream::TDirection SolveDirection();
        
        TInt iOneTimeRTCPFlag;
        
        
    private:  // private constructors
	    
        /**
        * C++ default constructor. Not implemented.
        */
        CMCETestUIEngineAudioStream();	
    
    protected:
    
        /**
        * C++ first phase constructor.
        */
        CMCETestUIEngineAudioStream( 
                    CMCETestUIEngine& aEngine,
                    CMceAudioStream& aAudioStream );

       	/**
        * Symbian second-phase constructor
        */
        void ConstructL();	   
        
    protected:  // owned data  
    
        // Commands applicable to an audiostream. Owned.
               
       RPointerArray<TMCETestUIEngineCmdBase> iCommands;
       
       RPointerArray<CMCETestUIEngineSource> iSources;
       RPointerArray<CMCETestUIEngineSink> iSinks;
       RPointerArray<CMCETestUIEngineCodec> iCodecs;
	    
 
    protected:  // not owned data
        
       CMCETestUIEngine& iEngine;       
    
       // Reference to wrapped MCEAudioStream
       CMceAudioStream& iAudioStream;

       // Indicator of direction
       CMCETestUIEngineAudioStream::TDirection iDirection;
        
    };

#endif
