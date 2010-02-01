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




#ifndef CMCESPEAKERSINK_H
#define CMCESPEAKERSINK_H

// INCLUDES
#include <e32std.h>  
#include <mcemediasink.h>

// DATA TYPES
const TMceSinkType KMceSpeakerSink = 2;

const TInt KMceAudioRoutePrivate = 3;
const TInt KMceAudioRoutePublic = 4;

			
// CLASS DECLARATION

/**
* Class representing speaker in MCE.
*
* CMceSpeakerSink can be used to play out audio.
*
* Enable (un-mute) and Disable (mute) operations to it are considered
* to be local, so they are not signalled to remote terminal.
* 
*  @lib mceclient.lib
*/
class CMceSpeakerSink: public CMceMediaSink
	{
	public: // Constructors and destructor

	    /**
	    * Two-phased constructor.
	    */
		IMPORT_C static CMceSpeakerSink* NewL();

	    /**
	    * Two-phased constructor.
	    */
		IMPORT_C static CMceSpeakerSink* NewLC();

	    /**
	    * Destructor.
	    */
		IMPORT_C ~CMceSpeakerSink();

	public: // Functions

	    /**
	    * Enables the sink explicitly.
	    */
		IMPORT_C void EnableL(); 

	    /**
	    * Disables the sink explicitly.
	    */
		IMPORT_C void DisableL(); 
		
        /**
        * Sets volume 
        * @param aVolume volume value
        */
		IMPORT_C void SetVolumeL( TInt aVolume );
		
        /**
        * Gets current volume 
        * @return value of current volume, 
        */
		IMPORT_C TInt VolumeL() const;
		
        /**
        * Gets maximum volume 
        * @return value of mamimum volume, 
        */
		IMPORT_C TInt MaxVolumeL() const;
		
        /**
        * Sets audio routing 
        * @param aRoute Required audio route, like KMceAudioRoutePublic. 
        */
		IMPORT_C void SetRoutingL( TInt aRoute );

        /**
        * Gets current audio routing. 
        * @return TInt Current audio routing. 
        */
		IMPORT_C TInt RoutingL() const;


	public: // from CMceMediaSink

	    /**
	    * Called endpoint proxy must be created
	    * @return endpointProxy
	    */
        MMceEndPointProxy* CreateEndpointProxyL() const;

	    /**
	    * Should this endpoint use endpoint proxy
	    * @return ETrue, this endpoint uses endpoint proxy
	    */
        TBool UsesEndpointProxy() const;

	private:

	    /**
	     * C++ default constructor.
	     */
		CMceSpeakerSink();

	    /**
	    * second-phase constructor
	    */
		void ConstructL();
	};

#endif 
