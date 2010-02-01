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




#ifndef __CMCETESTUIENGINESESSION_H__
#define __CMCETESTUIENGINESESSION_H__

// USER INCLUDES

// SYSTEM INCLUDES
#include <e32base.h>
#include <mcemediasource.h>


// FORWARD DECLARATIONS

class CMceManager;
class CMceFCMsgExchange;
class CMceSession;
class CMCETestUIEngine;
class CMCETestUIEngineCmdBase;
class CMCETestUIEngineProfile;
class CMCETestUIEngineAudioStream;
class CMCETestUIEngineVideoStream;
class CMceVideoCodec;

const TInt KMceOutSession = 1;
const TInt KMceInSession = 2;

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
class CMCETestUIEngineSession : public CBase,
                                public MMCETestUIEngineCmdProvider
    {
    public: // Constructors and destructors
            
    	/**
    	* Destructor.
    	*/
    	IMPORT_C virtual ~CMCETestUIEngineSession();

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
	    
	    /**
	    * @return Direction of the session
	    */
	    IMPORT_C TDirection Direction() const;
	    
	    /**
	    * @return Video Direction of the session
	    */
	    IMPORT_C TVideoDirection VideoDirection() const;
	    
	    /**
	    * @return Textual representation of session's direction
	    */
	    IMPORT_C const TDesC16& TextualDirection() const;

		/**
	    * @return Textual representation of session's Video direction
	    */
	    IMPORT_C const TDesC16& TextualVideoDirection() const;

	    /**
	    * @return Audiostreams of a session
	    */
        IMPORT_C const RPointerArray<CMCETestUIEngineAudioStream>& 
                        AudioStreamsL();
	    	    
	    IMPORT_C RPointerArray<CMCETestUIEngineAudioStream>&
	                    CurrentAudioStreams();	  
	                    
	    /**
	    * @return Videostreams of a session
	    */
        IMPORT_C const RPointerArray<CMCETestUIEngineVideoStream>& 
                        VideoStreamsL();
	    	    
	    IMPORT_C RPointerArray<CMCETestUIEngineVideoStream>&
	                    CurrentVideoStreams();	   
	                    
	    IMPORT_C TUint32 ProfileId();  
    public: // New functions, internal

        /**
        * Gets wrapped MCESession object
        * 
        * @return wrapped MCESession
        */
        CMceSession& Session();		
        
        /**
        *
        * Solves and sets the direction of session based on streams and
        * their properties.
        *
        * @return the newly set direction of session
        */
        TDirection SolveDirection();
        
        /**
        *
        * Solves and sets the Video direction of session based on streams and
        * their properties.
        *
        * @return the newly set direction of session
        */
        TVideoDirection SolveVideoDirection();
        
        IMPORT_C TInt StatusCode();
        
        void EnableAudioL();
        
        void EnableAudioStreamL( CMceMediaStream& aAudioStream );
        
        void AdjustVideoCodecsL( const RPointerArray<CMceVideoCodec>& aCodecs );
        
        CMceMediaStream* FindStreamForSourceL( CMceMediaStream& aStream, 
                                               TMceSourceType aSourceType );
        
    private:  // private constructors
	    
        /**
        * C++ default constructor. Not implemented.
        */
        CMCETestUIEngineSession();	
    
    protected:
    
        /**
        * C++ first phase constructor.
        */
        CMCETestUIEngineSession( 
                    CMCETestUIEngine& aEngine,
                    TDirection aDirection,
                    TVideoDirection aVideoDirection );

       	/**
        * Symbian second-phase constructor
        */
        void BaseConstructL();	   
        
    protected:  // owned data  
    
        // Commands applicable to all sessions. Owned.
       RPointerArray<TMCETestUIEngineCmdBase> iCommands;

       // Wrapppers for audiostreams
       RPointerArray<CMCETestUIEngineAudioStream> iAudioStreams;
       
       // Wrapppers for videostreams
       RPointerArray<CMCETestUIEngineVideoStream> iVideoStreams;
        
    protected:  // not owned data
        
       CMCETestUIEngine& iEngine;       
    
       // Pointer to wrapped OutSession/InSession
       CMceSession* iSession;
       
      // TInt iStatusCode;

       // Indicator of audio direction
       TDirection iDirection;
       // Indicator of video direction
       TVideoDirection iVideoDirection;
    public:
      
		TInt iType;
		
		TBool iHold;
		
		TBool iHoldInProgress;
        
    };

#endif
