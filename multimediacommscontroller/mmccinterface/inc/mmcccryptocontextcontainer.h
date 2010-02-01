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
* Description:    Container class for containing MCC SRTP crypto contexts
*
*/




#ifndef MMCCCRYPTOCONTEXTCONTAINER_H
#define MMCCCRYPTOCONTEXTCONTAINER_H

//  INCLUDES
#include <e32base.h>


// FORWARD DECLARATIONS
class TMMFMessage;
class TMccCryptoContext;
class TMccSrtpMasterKey;
class TMccSrtpMasterSalt;
class TMccSrtpCryptoParams;

/**
*  Container class for containing and handling
*  MCC crypto contexts.
*
*  @lib mmccinterface.lib
*/
NONSHARABLE_CLASS( CMccCryptoContextContainer ) : public CBase
    {
public:  // Constructors and destructor

    /**
     * Two-phased constructor.
     */
    static CMccCryptoContextContainer* NewL( );

    /**
     * Destructor.
     */
    virtual ~CMccCryptoContextContainer();

public: // New functions

    /** 
     * Creates new crypto context.
     * @since Series 60 3.2
     * @param aMasterKey The master key value.e.
     * @param aSaltKey The master salt value.
     * @param aContextId ID of the new context if successful.
     * @param aCryptoParams Crypto context optional parameters.
     * @return KErrNone if success; otherwise system wide error code.
    */
    TInt CreateContext( const TMccSrtpMasterKey& aMasterKey,
                        const TMccSrtpMasterSalt& aSaltKey,
    					TUint32& aContextId,
    				    const TMccSrtpCryptoParams& aCryptoParams );

    /**
     * Removes crypto context.
     * @since Series 60 3.2
     * @param aContextId ID of the removable context.
     * @return KErrNone if success; otherwise system wide error code.
    */
    TInt RemoveContext( TUint32 aContextId );
    
    /** 
     * Updates existing context.
     * @since Series 60 3.2
     * @param aContextId ID of the updating context.
     * @param aContext New context which updates existing context.
     * @return KErrNone if success; otherwise system wide error code.
    */
    TInt UpdateContext( TUint32 aContextId,
                        TMccCryptoContext& aContext );
    
    /**
     * Finds right context and returns it to caller.
     * @since Series 60 3.2
     * @param aContextId ID of the new context if successful.
     * @param aContext Founded context
     * @return KErrNone if success; otherwise system wide error code.
     */
    TInt GetContext( TUint32 aContextId,
                     TMccCryptoContext* &aContext );
   
private:
    
    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();
    
    /**
     * Generates a new crypto context identifier
     * @since Series 60 3.2
     * @return TUint32  New crypto context id;
     */
    TUint32 GenerateContextId();
    
    /**
     * Finds given crypto context from context array.
     * @since Series 60 3.2
     * @param aContextId Crypto context ID
     * @return Index to context array or KErrNotFound.
     */
    TInt FindContext( TUint32 aContextId );
        
private: // Data
    
    /**
     * Array contains the Crypto contexts.
     */
    RArray<TMccCryptoContext> iContextArray;

    };
     
    
#endif /* MMCCCRYPTOCONTEXTCONTAINER_H */
