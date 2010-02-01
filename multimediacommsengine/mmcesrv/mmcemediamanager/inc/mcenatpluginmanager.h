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



#ifndef CMCENATPLUGINMANAGER_H
#define CMCENATPLUGINMANAGER_H

#include "nspplugin.h"
#include "mcesrv.h"
#include <e32base.h>

class CSdpDocument;
class MNSPSessionObserver;

/**
*  MCE NAT Plugin manager.
*
*  @lib 
*  @since S60 3.2
*/
class CMceNatPluginManager: public CBase
	{
	
public: // Constructors and destructor

	/**
	* Two-phased constructor.
	*/
	static CMceNatPluginManager* NewL();

	/**
	* Destructor.
	*/
    ~CMceNatPluginManager();
    
private: // Constructors and destructor    

    CMceNatPluginManager();
    
    void ConstructL();


public: // New functions

    /**
     * Wrapper for CNSPPlugin - class corresponding method
     */
	TInt NewSessionL( MNSPSessionObserver& aNatSessionObserver,
	                  TUint32 aIapId,
	                  const TDesC8& aDomain );
	
    /**
     * Wrapper for CNSPPlugin - class corresponding method
     */
	void CloseSessionL( TUint aSessionId );
    
    /**
     * Wrapper for CNSPPlugin - class corresponding method
     */
    TNatReturnStatus CreateOfferL( TUint aSessionId, CSdpDocument*& aOffer );
    
    /**
     * Wrapper for CNSPPlugin - class corresponding method
     */
    TNatReturnStatus ResolveL( TUint aSessionId,
                               CSdpDocument*& aOffer,
                               CSdpDocument*& aAnswer );
    
    /**
     * Wrapper for CNSPPlugin - class corresponding method
     */
    TNatReturnStatus DecodeAnswerL( TUint aSessionId, CSdpDocument*& aAnswer );
    
    /**
     * Wrapper for CNSPPlugin - class corresponding method
     */
    void UpdateL( TUint aSessionId, CSdpDocument*& aOffer );
    
    /**
     * Wrapper for CNSPPlugin - class corresponding method
     */
    TNatReturnStatus RestartL( TUint aSessionId );
    
    /**
     * Method checks if NAT plugin is present.
     *
     * @return ETrue if Nat plugin was present, EFalse otherwise.
     */
    TBool NatEnabledL();
    
    /**
     * Wrapper for CNSPPlugin - class corresponding method
     */
    TInt SetSessionParam( TUint aSessionId, CNSPPlugin::TNSPSessionParamKey aParamKey,
            TUint aParamValue );
    
    /**
     * Wrapper for CNSPPlugin - class corresponding method
     */
    TInt GetSessionParam( TUint aSessionId, CNSPPlugin::TNSPSessionParamKey aParamKey );


    static void WriteSdpToLog( const TDesC& aMsg, CSdpDocument* aSdp );

private:

	void GetPluginL();

    static CBufBase* EncodeSdpL( CSdpDocument& aSdp );
    
    static void ResetAndDestroyInfo( TAny* aArray );
    
    //friend class UT_CMceNatPluginManager;
    friend class UT_CMceNatPluginManager;

private: // Data

    // pointer to plugin implementation
    CNSPPlugin* iPlugin;
    
    //definitions for unit testing
	MCEMM_UT_DEFINITIONS
    

	};

#endif // CMCENATPLUGINMANAGER_H

// End of File
