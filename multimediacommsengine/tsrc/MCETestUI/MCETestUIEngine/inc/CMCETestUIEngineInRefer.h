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




#ifndef __CMCETESTUIENGINEINREFER_H__
#define __CMCETESTUIENGINEINREFER_H__

// INCLUDES
#include <e32base.h>
#include "TMCETestUIEngineCmdBase.h"
#include "CMCETestUIEngineRefer.h"

// FORWARD DECLARATIONS

class CMceInRefer;


// CLASS DECLARATION
/**
* 
* CMCETestUIEngineInSession provides a wrapper for CMCEInSession.
* Class is not the traditional wrapper in a sense that it would 
* conform to interface provided by CMCEInSession. Instead class
* implements interface MMCETestUIEngineCmdProvider to provide
* same kind of functionality in controlled way on different levels 
* of abstraction.
*
*/
class CMCETestUIEngineInRefer : public CMCETestUIEngineRefer
    {
        
    public: // Constructors and destructors


        /**
        * Two-phased constructor. Not imported.
        */
        static CMCETestUIEngineInRefer* NewL(
            CMCETestUIEngine& aEngine, 
            CMceInRefer* aInRefer );
            
    	/**
    	* Destructor.
    	*/
    	IMPORT_C ~CMCETestUIEngineInRefer();

    public: // From MMCETestUICommandProvider

        /**
        * Gets the commands applicaple to this insession object.
        *
        * @return reference to array of commands
        */
        IMPORT_C const RPointerArray<TMCETestUIEngineCmdBase>& 
                    GetCommandsL();
	
    public: // From base class 
    
        /**
        * @return Id of this particular insession object
        */    
        IMPORT_C TInt Id() const;        	
	    
    public: // New functions, internal

        /**
        * Gets wrapped MCEInSession object
        * 
        * @return wrapped MCEInSession
        */    
	    CMceInRefer& InRefer();
    
    private:  // private constructors
	    
        /**
        * C++ default constructor. Not implemented.
        */
        CMCETestUIEngineInRefer();	

        /**
        * C++ first phase constructor.
        */
        CMCETestUIEngineInRefer( 
                CMCETestUIEngine& aEngine,
                CMceInRefer* aInRefer );

       	/**
        * Symbian second-phase constructor
        */
        void ConstructL();	   
        
        
    private:  // owned data

        // Wrapped CMCEInSession object
        CMceInRefer* iInRefer;
       
        // Id of this insession object
        TInt iId;   
        
    };

#endif // __CMCETESTUIENGINEINSESSION_H__
