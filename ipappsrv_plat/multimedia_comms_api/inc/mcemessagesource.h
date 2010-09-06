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



#ifndef CMCEMESSAGESOURCE_H
#define CMCEMESSAGESOURCE_H

// INCLUDES
#include <e32std.h>
#include <mcemediasource.h>

//FORWARD DECLARATIONS
class CMceManager;
class CMceItcSender;
class CMceDataSender;

// CONSTANTS
const TMceSourceType KMceMessageSource = 12;

// CLASS DECLARATION
/**
* Class representing a source for a Message stream.
* 
*  @lib mceclient.lib
*/
class CMceMessageSource: public CMceMediaSource
    {

    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CMceMessageSource* NewL(CMceManager& aManager);

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CMceMessageSource* NewLC(CMceManager& aManager);

        /**
        * Destructor.
        */
        IMPORT_C ~CMceMessageSource();

    public: // From CMceMediaSource

        /**
        * Sets the source enabled.
        */
        IMPORT_C void EnableL(); 

        /**
        * Sets the source disabled.
        */
        IMPORT_C void DisableL(); 
        
	public: // New functions

	    /**
	    * Sends data into stream
	    */
        IMPORT_C void SendDataL( const TDesC8& aData ); 
	public: // internal
	
	    /**
	    * Initializes 
	    * @param aManager the manager. Ownership is NOT transferred
	    */
	    void InitializeL( CMceManager* aManager );
    public:

	    /**
	    * Two-phased constructor.
	    */
		static CMceMessageSource* NewL();

	    /**
	    * Two-phased constructor.
	    */
		static CMceMessageSource* NewLC();

	private: //methods

	    /**
	     * C++ default constructor.
	     */
		CMceMessageSource();

	    /**
	    * Second-phase constructor.
	    */
		void ConstructL(CMceManager* aManager);
		
	    void ConstructL();
    
		
	private:
	    
        CMceDataSender* iDataSender;
        TAny* iReserved;  
    };

#endif // CMCEMESSAGESOURCE_H
