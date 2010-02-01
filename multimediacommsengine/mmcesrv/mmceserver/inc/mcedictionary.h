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




#ifndef CMCEDICTIONARY_H
#define CMCEDICTIONARY_H

//  INCLUDES
#include <e32base.h>
#include "mcesrv.h"

/**
*  Abstract base class for key - value pairs
*
*  @lib 
*  @since
*/
class CMceDictionaryItem : public CBase
    {
public: // New functions

    /**
    * Returns the key
    * @return the key
    */
    TInt Key();
    
protected:
    
    /**
    * Constructor
    * @param aKey the key
    */
    CMceDictionaryItem( TInt aKey );
    
private: // Data
    
    // the key
    TInt iKey;
    };

/**
*  Template class for key-value pairs, where
*  the value is a T-class object.
*
*  @lib 
*  @since
*/
template <class T> 
class CMceKeyTValuePair : public CMceDictionaryItem
    {
public: // Constructors and destructors

	/**
	* Constructor
	* @param aKey the key
	* @param aValue the value
	*/
	CMceKeyTValuePair( TInt aKey, T aValue );

public: // Functions from base classes
    
    // From CMceKeyValuePairBase
    /**
    * Returns the value.
    * @return the value
    */
	T& Value();
        
public: // New functions       
    
    /**
    * Sets the value
    * @param aValue
    */
    void SetValue( T aValue );
    
private: // Data
    
        // the value
        T iValue;
    MCESRV_UT_DEFINITIONS
    };

/**
*  Template class for key-value pairs, where
*  the value is a C-class object.
*
*  @lib 
*  @since
*/
template <class C> 
class CMceKeyCValuePair : public CMceDictionaryItem
    {

public: // Constructors and destructor

    /**
	* Constructor
    * @param aKey the key
	* @param aValue the value, ownership is transfered
	*/
	CMceKeyCValuePair( TInt aKey, C* aValue );
    
    /**
	* Destructor
	*/
	~CMceKeyCValuePair();

public: // Functions from base classes
    
    // From CMceKeyValuePairBase
    /**
    * Returns the value.
    * @return the value, ownership is not transfered
    */
	C* Value();
        
public: // New functions           
    /**
    * Sets the value
    * @param aValue
    */
    void SetValue( C* aValue );
        
private: // data
        
    // the value, owned
    C* iValue;
    
    MCESRV_UT_DEFINITIONS    
    };
    
    
/**
* Cictionary that holds key-value pairs and allows modification of them. 
*/
class CMceDictionary : public CBase
    {
    
public: // Constructors and destructor
    
    /**
	* Destructor
	*/
    ~CMceDictionary();
   
public: // New functions
    
    /**
    * Returns the dictionary item with given key.
    * Returns NULL, if the item is not found.
    * @param aKey
    * @return the item, ownership is not transfered.
    */
    CMceDictionaryItem* Pair( TInt aKey ) const;
    
    /**
    * Adds a key-value pair to the dictionary.
    * aPair the pair, ownership is transfered.
    * The pair 
    */
    void AddL( CMceDictionaryItem* aPair );
    
	/**
	* Deletes a pair that has given key from the dictionary.
	* @param aKey the key
	*/	
	void Delete( TInt aKey );
	
private:
    
    // data
    RPointerArray< CMceDictionaryItem > iPairs;

    MCESRV_UT_DEFINITIONS		
    };

#include "mcedictionary.inl"
#endif // CMCEDICTIONARY_H

// End of File
