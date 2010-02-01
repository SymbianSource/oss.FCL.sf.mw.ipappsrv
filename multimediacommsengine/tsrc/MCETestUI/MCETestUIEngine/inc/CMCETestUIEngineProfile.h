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




#ifndef CMCETESTUIENGINEPROFILE_H
#define CMCETESTUIENGINEPROFILE_H

// INCLUDES
#include <e32base.h>
#include "MMCETestUIEngineCmdProvider.h"
#include "CMCETestUIEngine.h"

// FORWARD DECLARATIONS

class CSIPProfile;
class CMCETestUIEngineCmdBase;

// CLASS DECLARATION

/**
* 
* CMCETestUIEngineProfile provides a wrapper for CSIPProfile.
* Class is not the traditional wrapper in a sense that it would 
* conform to interface provided by CSIPProfile. Instead class
* implements interface MMCETestUIEngineCmdProvider to provide
* same kind of functionality in controlled way on different levels 
* of abstraction.
*
*/
class CMCETestUIEngineProfile : public CBase,
                                public MMCETestUIEngineCmdProvider
    {
        
    public: // Constructors and destructors


        /**
        * Two-phased constructor. Not imported.
        */
        static CMCETestUIEngineProfile* NewL( CSIPProfile* aProfile,
                                              CMCETestUIEngine& aEngine );

    	/**
    	* Destructor.
    	*/
    	virtual ~CMCETestUIEngineProfile();

    public: // From MMCETestUICommandProvider

        /**
        * Gets the commands applicaple to this profile object.
        *
        * @return reference to array of commands
        */
        IMPORT_C const RPointerArray<TMCETestUIEngineCmdBase>& 
                    GetCommandsL();
	
    public: // New functions

        /**
        * Gets wrapped CSIPProfile object
        * 
        * @return wrapped CSIPProfile
        */    
        CSIPProfile& Profile();

        /**    
        * @return ETrue if wrapped profile is set as default
        */
        IMPORT_C TBool IsDefault() const;

        /**    
        * @return ETrue if wrapped profile is registered
        */
        IMPORT_C TBool IsRegistered() const;

        /**    
        * @return Profile ID of the wrapped profile
        */        
        IMPORT_C TInt ProfileId() const;
        
        /**    
        * @return provider name of the wrapped profile
        */        
        IMPORT_C const TDesC8& ProviderName() const;
         /**    
        * @return aor of the wrapped profile
        */  
        
        IMPORT_C const TDesC8& Aor() const;
        	
    private:  // New functions
	    
        /**
        * C++ default constructor. Not implemented.
        */
        CMCETestUIEngineProfile();

        /**
        * C++ first phase constructor.
        */
        CMCETestUIEngineProfile( CSIPProfile* aProfile,
                                 CMCETestUIEngine& aEngine );        	

    	/**
        * Symbian second-phase constructor
        */
        void ConstructL();	    
        
    private:  // Data

        /// Wrapped CSIPProfile object. Owned.
        CSIPProfile* iProfile;
        
        CMCETestUIEngine& iEngine;

        // Commands applicable to profiles. Owned.        
        RPointerArray<TMCETestUIEngineCmdBase> iCommands;
        
    };

#endif
