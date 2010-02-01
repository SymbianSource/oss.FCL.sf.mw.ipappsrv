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
* Description:    Class is used for configuring secure RTP.
*
*/




#ifndef MMCCSECURECINTERFACE_H
#define MMCCSECURECINTERFACE_H

//  INCLUDES
#include <e32base.h>
#include <mmccinterface.h>

// CONSTANTS
const TInt KMccLinkSecure = 110; 
const TInt KMccLinkSecureVideo = 111; 

// FORWARD DECLARATIONS
class CMccInterface;
class CMccCryptoContextContainer;
class TMccCryptoContext;

/**
*  MCC subsystem client interface for secure sessions.
*
*  @lib mmccinterface.lib
*/
class CMccSecureInterface : public CBase
    {
public:  // Constructors and destructor

    /**
     * Two-phased constructor.
     * @param aInterface reference to MCC interface.
     * @return CMccSecureInterface* Pointer to the secure interface object created
     */
    IMPORT_C static CMccSecureInterface* NewL( CMccInterface& aInterface );

    /**
     * Destructor.
     */
    virtual ~CMccSecureInterface();

public: // New functions

    /**
     * Creates new crypto context.
     * @since Series 60 3.2
     * @param aMasterKey The master key value.
     * @param aSaltKey The master salt value.
     * @param aContextId ID of the new context if successful.
     * @param aParams Crypto context optional parameters.
     * @return KErrNone if success; otherwise system wide error code.
    */
    IMPORT_C TInt CreateContext( const TDesC8& aMasterKey, const TDesC8& aSaltKey,
                                 TUint32& aContextId, const TDesC8& aParams );

    /**
     * Updates existing crypto context.
     * @since Series 60 3.2
     * @param aMasterKey The master key value.
     * @param aSaltKey The master salt value.
     * @param aContextId ID of the updated context.
     * @param aParams Crypto context optional parameters.
     * @return KErrNone if success; otherwise system wide error code.
     */
    IMPORT_C TInt UpdateContext( const TDesC8& aMasterKey, const TDesC8& aSaltKey,
                                 TUint32 aContextId, const TDesC8& aParams );
    /**
     * Removes existing crypto context.
     * @since Series 60 3.2
     * @param aContextId ID of the context to be removed.
     * @return KErrNone if success; otherwise system wide error code.
     */
    IMPORT_C TInt RemoveContext( TUint32 aContextId );

    /**
     * Binds crypto context into stream. Binding must be done
     * before stream is prepared.
     * @since Series 60 3.2
     * @param aSessionId ID of session.
     * @param aLinkId ID of link.
     * @param aStreamId ID of stream.
     * @param aEndpointId ID identifying endpoint of the stream.    
     * @param aContextId ID of context.
     * @return KErrNone if success;
     *         KErrArgument if context is already binded.
     *         Otherwise one of the standard system-wide error codes.
     */
    IMPORT_C TInt Bind( TUint32 aSessionId, TUint32 aLinkId,
    					TUint32 aStreamId, TUint32 aEndPointId,
    				    TUint32 aContextId );

private:
    
    /**
     * C++ default constructor.
     */
    CMccSecureInterface( CMccInterface& aController );
    
    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL( );
        
private:
    
    /**
     * Instance of Mcc interface.
     */
    CMccInterface& iInterface;
    
    /**
     * Contains unique ID of controller interface
     */
    TMMFMessageDestinationPckg iMessageDest;
    
    /**
     * Handle to the controller plugin for sending custom commands
     */
    RMMFController iController;
    
    /**
     * Crypto context container for context containing and handling
     * Own.
     */
    CMccCryptoContextContainer* iContextContainer;	
	
    };
    
#endif /* MMCCSECUREINTERFACE_H */
