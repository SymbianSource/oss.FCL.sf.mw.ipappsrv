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





#ifndef PLUGIN_STUB_H
#define PLUGIN_STUB_H

//  INCLUDES
#include <e32base.h>
#include <e32const.h>
#include "nspplugin.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Container class for the codec information.
*
*/
class CPluginStub : public CNSPPlugin
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CPluginStub* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CPluginStub();

    public: // New functions
    	TInt SetSessionParam( TUint aSessionId, CNSPPlugin::TNSPSessionParamKey aParamKey, TUint aParamValue );
    	TInt GetSessionParam( TUint aSessionId, TNSPSessionParamKey aParamKey );
    	
    	TUint NewSessionL(MNSPSessionObserver& aNatSessionObserver, TUint32 aIapId, const TDesC8& aDomain, TUint aProtocol );
    	void CloseSessionL( TUint aSessionId );
		TNatReturnStatus CreateOfferL( TUint aSessionId, CSdpDocument*& aOffer );
		TNatReturnStatus ResolveL( TUint aSessionId,CSdpDocument*& aOffer, CSdpDocument*& aAnswer );
		TNatReturnStatus DecodeAnswerL( TUint aSessionId, CSdpDocument*& aAnswer );
		void UpdateL( TUint aSessionId, CSdpDocument*& aOffer );
		TNatReturnStatus RestartL( TUint aSessionId );
		
    public: // Functions from base classes
        
    private:

        /**
        * C++ default constructor.
        */
        CPluginStub();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
    };

#endif      // PLUGIN_STUB_H   
            
// End of File
