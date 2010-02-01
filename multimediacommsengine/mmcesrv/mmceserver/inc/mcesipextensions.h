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




#ifndef CMCESIPEXTENSIONS_H
#define CMCESIPEXTENSIONS_H

//  INCLUDES
#include <e32base.h>
#include "mcesrv.h"
#include "mcedefs.h"

//  FORWARD CLASS DECLARATIONS
class CMceDictionary;
class CSIPMessageElements;
class CSIPServerTransaction;
class CSIPClientTransaction;
class CDesC8Array;
class CMceComSession;

/**
*  Class for modeling the level of support 
*  for SIP extensions for local and remote
*  participants of a session.
*  @lib 
*  @since
*/
class CMceSipExtensions : public CBase
    {
public: // Enums
    
    // Extensions
    enum TExtension
        {
        E100rel = 1,
        EQosPreconds,
        ESecPreconds,
        EUpdate,
        EPrack,
        ESubscribe,
        ERefer,
        ENotify
        };
    
    // Support levels
    enum TLevel
        {
        ENotSupported,
        ESupported,
        ERequired
        };
    
public: // Constructors and destructor

	/**
	* Two-phased constructor.
	*/
	static CMceSipExtensions* NewL();

	/**
	* Destructor.
	*/
	~CMceSipExtensions();
    
public: // New functions

    /**
    * Query the level of local support for given extension.
    * Not stored extension is considered not supported.
    * @param aExtension the extension
    * @return the level of support
    */
    TLevel Local( TExtension aExtension );
    
    /**
    * Query the level of remote support for given extension.
    * Not stored extension is considered not supported.
    * @param aExtension the extension
    * @return the level of support
    */
    TLevel Remote( TExtension aExtension );
    
    /**
    * Set the level of local support for given extension.
    * Ignored, if the extension is not stored.
    * @param aExtension the extension
    * @param aLevel the level
    */
    void SetLocal( TExtension aExtension, TLevel aLevel );
    
    /**
    * Set the level of remote support for given extension.
    * Ignored, if the extension is not stored.
    * @param aExtension the extension
    * @param aLevel the level
    */
    void SetRemote( TExtension aExtension, TLevel aLevel );
    
    /**
    * Returns the level of support for certain SIP extension,
    * @param aTransaction the transaction
    * @param aExtension the extension
    * @return the level
    */
    TLevel LevelL( CSIPTransactionBase& aTransaction,
                  TExtension aExtension );
    
    /**
    * Updates the extensions according to incoming request.
    * Leaves with KErrNotSupported, if unsupported extension is required.
    * @param aTransaction the transaction
    */
    void UpdateL( CSIPServerTransaction& aTransaction );
    
    /**
    * Updates the extensions according to incoming response
    * Leaves with KErrNotSupported, if unsupported extensions is required.
    * @param aTransaction the transaction
    */
    void UpdateL( CSIPClientTransaction& aTransaction );
    
    /**
    * Updates the extensions according to client preferences
    * Leaves with KErrNotSupported, if unsupported extensions is required.
    * @param aBody the client data
    */
    void Update( CMceComSession& aBody );
    
    /**
    * Finds out, is the remote UA IMS Rel5 terminal
    */
    TBool RemoteIMSRel5();
    
    /**
    * Put all allow headers supported by MCE into array
    * @param aArray contains the allow header values
    */
    void ExtensionAllowMethodsL( RArray<TInt>& aArray );
	
	/**
    * Put all Sip extension headers supported by MCE into array
    * @param aArray contains the allow header values
    */
	void LocalSipExtensionsL(RArray<TExtension>& aArray );
    
private:
    
    /**
    * Constructor
    */
    CMceSipExtensions();
    
    /**
    * Second phase constructor
    */
    void ConstructL();

private: // utility    
    /**
    * Adds the default items in dictionary
    * @param aDictionary the dictionary
    */
    void AddDefaultsL( CMceDictionary& aDictionary );
    
    /**
    * Returns the level of extension in dictionary
    * @param aDictionary the dictionary
    * @param aExtension the extension
    * @return the level of support
    */
    TLevel Level( const CMceDictionary& aDictionary, 
                  TExtension aExtension ) const;
    
    /**
    * Set the level of support for given extension in dictionary.
    * Ignored, if the extension is not stored.
    * @param aDictionary the dictionary
    * @param aExtension the extension
    * @param aLevel the level
    */
    void SetLevel( CMceDictionary& aDictionary, 
                   TExtension aExtension, 
                   TLevel aLevel );

    /**
    * Update extension value specified by session modifier 
    * according to given value.
    * @param aModifier the session modifier
    * @param aValue the value
    */
    void Update( TMceSessionModifier aModifier, TUint aValue );
    
    /**
    * Setting romete extesion header support status
    * @param aMessage SipMessages
    * @return void
    */
    void SetExtensionAllowHeadersL( const CSIPMessageElements* aMessage );
 
private: // Data
    
    // Dictionary for local extensions
    CMceDictionary* iLocal;
    
    // Dictionary for remote extensions
    CMceDictionary* iRemote;
    
    MCESRV_UT_DEFINITIONS
    };

#endif //CMceSipEXTENSIONS_H

// End of File
