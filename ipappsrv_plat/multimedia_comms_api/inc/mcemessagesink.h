/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef CMCEMESSAGESINK_H
#define CMCEMESSAGESINK_H

// INCLUDES
#include <e32std.h>  
#include <mcedatasinkobserver.h>


// FORWARD DECLARATIONS
class CMceManager;
class CMceItcSender;
class CMceDataReceiver;
#include <MceMediaSink.h>

// CONSTANTS
const TMceSinkType KMceMessageSink = 12;

// CLASS DECLARATION

/**
* Class representing a sink for a media stream that is implemented outside MCE.
* 
*  @lib mceclient.lib
*/
class CMceMessageSink: public CMceMediaSink, public MMceDataSinkObserver
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
		IMPORT_C static CMceMessageSink* NewL( CMceManager& aManager );

	    /**
	    * Two-phased constructor.
	    * @param aManager reference to the manager
	    */
		IMPORT_C static CMceMessageSink* NewLC( CMceManager& aManager );

	    /**
	    * Destructor.
	    */
		IMPORT_C ~CMceMessageSink();


	public: // From CMceMediaSink

	    /**
	    * Enables the sink explicitly.
	    */
		IMPORT_C void EnableL(); 

	    /**
	    * Disables the sink explicitly.
	    */
		IMPORT_C void DisableL(); 
    		    
	public: 

	    /**
	    * Two-phased constructor.
	    */
	    static CMceMessageSink* NewL();

	    /**
	    * Two-phased constructor.
	    */
		static CMceMessageSink* NewLC();

	private:
                           
		void DataReceived( CMceMediaStream& aStream,
	        CMceMediaSink& aSink, const TDesC8& aData );
	
	private:

	    /**
	     * C++ constructor.
	     */
		CMceMessageSink();

	    /**
	    * Second-phase constructor.
	    */
		void ConstructL( CMceManager* aManager );
		
    private: // Reserved for future use
    
        CMceDataReceiver* iReceiver;
        TAny* iReserved;    
        
        MCE_UNIT_TEST_DEFS
    };

#endif // CMCEMESSAGESINK_H
