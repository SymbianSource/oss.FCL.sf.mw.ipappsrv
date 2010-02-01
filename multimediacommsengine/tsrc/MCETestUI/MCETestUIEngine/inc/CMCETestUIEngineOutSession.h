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




#ifndef __CMCETESTUIENGINEOUTSESSION_H__
#define __CMCETESTUIENGINEOUTSESSION_H__

// INCLUDES
#include <e32base.h>
#include "cmcetestuiengine.h"
#include "CMCETestUIEngineSession.h"
#include <mcerefer.h>

// FORWARD DECLARATIONS

class CMceOutSession;
class CMCETestUIEngine;
class CMCETestUIEngineCmdBase;
class CMceVideoStream;
class CMceVideoCodec;

// CLASS DECLARATION
/**
* 
* CMCETestUIEngineOutSession provides a wrapper for CMCEOutSession.
* Class is not the traditional wrapper in a sense that it would 
* conform to interface provided by CMCEOutSession. Instead class
* implements interface MMCETestUIEngineCmdProvider to provide
* same kind of functionality in controlled way on different levels 
* of abstraction.
*
*/

class CMCETestUIEngineOutSession : public CMCETestUIEngineSession
    {
        
    public: // Constructors and destructors

        /**
        * Two-phased constructor. Not imported.
        */
        static CMCETestUIEngineOutSession* NewL(
            CMCETestUIEngine& aEngine, 
            CMCETestUIEngineProfile& aProfile,
            TDirection aDirection,
            TVideoDirection aVideoDirection,
            TSessionType aSessionType );
        
         /**
        * Two-phased constructor. Not imported.
        */
        static CMCETestUIEngineOutSession* NewL(
            CMCETestUIEngine& aEngine, 
            TUint32 aIAPId,
            TDirection aDirection,
            TVideoDirection aVideoDirection,
            TSessionType aSessionType );
            
         /**
        * Two-phased constructor. Not imported.
        */
        static CMCETestUIEngineOutSession* NewL(
            CMCETestUIEngine& aEngine, 
            CMceRefer &aRefer,
            TDirection aDirection,
            TVideoDirection aVideoDirection,
            TSessionType aSessionType);
	    /**
	    * Two-phased constructor.
	    * @param aEvent, reference to existing event that is associated
	    *        with this session.
	    */
        //static CMCETestUIEngineOutSession* NewL(
        //    CMCETestUIEngine& aEngine, TDirection aDirection );


	    /**
	    * Two-phased constructor.
	    * @param aRefer, reference to existing refer that is associated
	    *        with this session.
	    */
        //static CMCETestUIEngineOutSession* NewL(
        //    CMCETestUIEngine& aEngine, TDirection aDirection );

            
    	/**
    	* Destructor.
    	*/
    	IMPORT_C virtual ~CMCETestUIEngineOutSession();

    public: // From MMCETestUICommandProvider

       /**
        * Gets the commands applicaple to this outsession object.
        *
        * @return reference to array of commands
        */
        IMPORT_C const RPointerArray<TMCETestUIEngineCmdBase>& 
                    GetCommandsL();


    public: // From base class 

        /**
        * @return Id of this particular outsession object
        */        
        IMPORT_C TInt Id() const;
	    
    public: // New functions, internal

        /**
        * Gets wrapped MCEOutSession object
        * 
        * @return wrapped MCEOutSession
        */    	    
	    CMceOutSession& OutSession();	
	    
	    TBool SessionParamsL(TSessionParams &aSessionParams, TSessionType aSessionType);

    public:
     	TSessionParams iSessionParams;
     	 
     	TSessionType iSessionType;

    private:  // private constructors
	    
        /**
        * C++ default constructor.
        */
        CMCETestUIEngineOutSession();	

        /**
        * C++ first phase constructor.
        */
        CMCETestUIEngineOutSession( 
                CMCETestUIEngine& aEngine,
                CMCETestUIEngineProfile& aProfile,
                TDirection aDirection ,
                TVideoDirection aVideoDirection ,
                TSessionType aSessionType);
        
         /**
        * C++ first phase constructor.
        */
        CMCETestUIEngineOutSession( 
                CMCETestUIEngine& aEngine,
                TUint32 aIAPId,
                TDirection aDirection ,
                TVideoDirection aVideoDirection ,
                TSessionType aSessionType);

		/**
        * C++ first phase constructor.
        */
        CMCETestUIEngineOutSession( 
                CMCETestUIEngine& aEngine,
                CMceRefer &aRefer,
                TDirection aDirection ,
                TVideoDirection aVideoDirection ,
                TSessionType aSessionType);

       	/**
        * Symbian second-phase constructor
        */
        void ConstructL();	   
        
        void ConstructL(CMceRefer& aRefer);
        // to fill poc session related data
        void FillPoCSessionData(CMceOutSession* aSession);
        
        // to fill voip session related data
        void FillVoIPSessionData(CMceOutSession* aSession);
        // to fill custom session related data
        void FillCustomSessionData(CMceOutSession* aSession);
        
        //to fill profile registrar in the recipent 
        
        void FillProfileRegistrar();
        
        void FillPoCOrigAddress();
        
        void Fill2WayVideoSessionDataL(CMceOutSession* aOutSession);
        
        CMceVideoCodec* GetVideoCodec( 
            CMceVideoStream& aStream, const TDesC8& aSdpName );
        
        
    private:  // owned data
    
        // Wrapped CMCEOutSession object
        CMceOutSession* iOutSession;
 
    private:  // not owned data
        
        // Profile to be used in session creation
        CMCETestUIEngineProfile* iProfile;
        
        CMceRefer* iRefer;
        
        TInt iId;
        
        TUint32 iIapId;
       
        
        
        //MCE_UNIT_TEST_DEFS;
        
    };

#endif
