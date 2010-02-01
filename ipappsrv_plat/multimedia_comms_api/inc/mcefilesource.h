/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef MCEFILESOURCE_H
#define MCEFILESOURCE_H

// INCLUDES
#include <e32std.h>
#include <mcemediasource.h>
#include <mcemediastream.h>

// CONSTANTS
const TMceSourceType KMceFileSource = 4;

//FORWARD DECLARATIONS
class CMceManager;
class CMceItcSender;
class TMceFileInfo;

// CLASS DECLARATION

/**
* Class representing file source in MCE.
*
* CMceFileSource can be used for streaming file contents. The file may contain
* both audio and video data. If the user wants to stream both media types,
* the same file source should be attached to audio stream and video stream. 
*
* The file may contain also several elements of the same media type (e.g. 
* 3 audio channels) and the user might want to use only some of those. 
* This can be handled by defining index of the file's current media element:
*
* - Get number of audio media elements within the file (e.g. returns 3)
* - Set current audio element index to be 0
* - Attach the file source to the audioStreamOne (audio element 0 is attached)
* - Set current audio element index to be 2
* - Attach the file source to the audioStreamTwo (audio element 2 is attached)
* - Consequently, file's audio channel 0 would be streamed over the audioStreamOne
*   and channel 2 over the audioStreamTwo. Channel 1 would be ignored.
*
* Enable (continue) and Disable (pause) operations to it are considered
* to be local, so they are not signalled to remote terminal.
* 
* @lib mceclient.lib
*/
class CMceFileSource : public CMceMediaSource
	{

	public: // Constructors and destructor

	    /**
	    * Two-phased constructor.
	    * @param aManager associated MCE manager
	    * @param aFileName name of the file including full path info
	    */
		IMPORT_C static CMceFileSource* NewL( CMceManager& aManager,
		                                      const TFileName& aFileName );

	    /**
	    * Two-phased constructor.
	    * @param aManager associated MCE manager
	    * @param aFileName name of the file including full path info
	    */
		IMPORT_C static CMceFileSource* NewLC( CMceManager& aManager,
		                                       const TFileName& aFileName );
		
	    /**
	    * Destructor.
	    */
		IMPORT_C ~CMceFileSource();


	public: // From CMceMediaSource

	    /**
	    * Enables the source explicitly.
	    */
		IMPORT_C void EnableL(); 

	    /**
	    * Disables the source explicitly.
	    */
		IMPORT_C void DisableL(); 


	public: // Functions

	    /**
	    * Updates file for the source.
	    * @param aFileName name of the file including full path info
	    */
		IMPORT_C void UpdateL ( const TFileName& aFileName );

        /**
        * Determines how many media elements of the spesified type the file
        * source contains. E.g. audio file may contain N number of audio
        * channels which has to be streamed over N number of audio streams.
        * @param aType the type of media
        * @return the number of media elements (of spesified type)
        *         the file contains
        */
        IMPORT_C TInt MediaElementCountL( TMceMediaType aType ) const;
        
        /**
        * Sets currently managed media element of the spesified media type. 
        * The setting is effective when the file source is attached to
        * media stream of the spesified type. Current media element can be
        * defined for each media type the file contains. By default, media
        * element 0 of each media type is used.
        * @param aType the type of media
        * @param aIndex new index of the current media element
        * @leave KErrArgument if media element spesified by input parameters
        *        does not exist
        */
        IMPORT_C void SetCurrentMediaElementL( TMceMediaType aType, TInt aIndex );

        /**
        * Gets currently managed media element of the spesified media type. 
        * @param aType the type of media
        * @return index of the current media element or KErrNotFound if no media
        *         elements of the spesified type exists
        */
        IMPORT_C TInt CurrentMediaElement( TMceMediaType aType );
        
        /**
        * Sets the position within the file from where to start playback.
        * @pre IsEnabled() == EFalse and contains audio/video
        * @param aPosition from start of the file in microseconds
        */
        IMPORT_C void SetPositionL( const TTimeIntervalMicroSeconds& aPosition );
        
        /**
        * Gets the current playback position in microseconds from the start of the file.
        * @pre contains audio/video
        * @return current position from start of the file in microseconds
        */
        IMPORT_C TTimeIntervalMicroSeconds PositionL() const;
        
        /**
        * Returns the duration of the file in microseconds.
        * @pre contains audio/video
        * @return duration of the file in microseconds.
        */
        IMPORT_C TTimeIntervalMicroSeconds DurationL() const;

        /**
        * Sets fast forward on/off.
        * @pre contains video
        * @param aUseFFWD ETrue if fast forward is enabled; otherwise EFalse
        */
        IMPORT_C void SetFastForwardL( TBool aUseFFWD );
        
        /**
        * Sets fast rewind on/off.
        * @pre contains video
        * @param aUseFRWD ETrue if fast rewind is enabled; otherwise EFalse
        */
        IMPORT_C void SetFastRewindL( TBool aUseFRWD );

        /**
        * Starts transcoding of file. After the transcoding is complete,
        * file of this source is changed into the transcoded file.
        * @pre CMceMediaStream::State() == ETranscodingRequired
        * @post CMceMediaStream::State() == ETranscoding
        * @param aFileName Name of the transcoding output file.
        */
        IMPORT_C void TranscodeL( const TFileName& aFileName );

        /**
        * Cancels transcoding of file. 
        * @pre CMceMediaStream::State() == ETranscoding
        * @post CMceMediaStream::State() == ETranscodingRequired
        */
        IMPORT_C void CancelTranscodeL();

        /**
        * Gets the current transcoding progress in percentage.
        * Value is updated with MMceStreamObserver::StreamStateChanged() event.
        * @pre CMceMediaStream::State() == ETranscoding
        * @return Transcoding progress in persentage (0-100).
        */
        IMPORT_C TInt TranscodingProgressL() const;

	public: // internal

	    /**
	    * Initializes 
	    * @param aManager the manager. Ownership is NOT transferred
	    */
	    void InitializeL( CMceManager* aManager );
	    
	    /**
	    * returns supported video codec of this file
	    * @return supported video codec of this file
	    */
	    const TDesC8& SupportedVideo();
	    
	    /**
	    * returns supported audio codec of this file
	    * @return supported audio codec of this file
	    */
	    const TDesC8& SupportedAudio();
	    
	    /**
	    * returns details of this file
	    * @return details of this file
	    */
	    const TMceFileInfo& FileInfo();

	public: //from CMceMediaSource

	    /**
	    * Traversal event handler
	    * @param aEvent the event
	    * @return status, if event was consumed or not or object needs update
	    */
	    TInt EventReceivedL( TMceEvent& aEvent );

	    /**
	    * Handles event
	    * @param aEvent the event
	    * @return status, if event was consumed or not or object needs update
	    */
        TInt HandleEvent( TMceEvent& aEvent );

	    /**
	    * Called after update
	    */
	    void Updated();
	    
	    /**
	    * Stream has been added to session
	    * @param aParent the parent stream
	    */
        void StreamAddedL( CMceMediaStream& aParent );
	    
	public: 

	    /**
	    * Two-phased constructor.
	    */
	    static CMceFileSource* NewL();

	    /**
	    * Two-phased constructor.
	    */
		static CMceFileSource* NewLC();
		
		/**
		* Gets manager
		* @return manager, or NULL if not present
		*/
		CMceManager* Manager();

	private:

	    /**
	    * C++ default constructor.
	    */
		CMceFileSource();

	    /**
	    * Second-phase constructor.
	    */
		void ConstructL( CMceManager* aManager,
		                 const TFileName& aFileName );	  


        void SynchronizeWithStreamL();
        void UpdateState( CMceMediaStream::TState aState );
        void GetFileInfoL( TBool aUseSession = ETrue );   
        void SetFileInfoL();   
        void TranscodingCompletedL( TMceEvent& aEvent );  
        void DoCancelTranscodeL();

	private: // Data
        
        CMceManager* iManager; //Not owned

    private: // Reserved for future use
    
        TAny* iReserved;   
        
        friend class CMceManager;    
        
        
	//for testing

    MCE_UNIT_TEST_DEFS	
	    	    
	};


#endif
