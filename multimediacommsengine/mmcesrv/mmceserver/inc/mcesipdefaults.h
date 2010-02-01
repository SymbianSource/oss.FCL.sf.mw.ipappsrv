/*
* Copyright (c) 2004-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Default data (headers) for certain SIP request. 
*
*/




#ifndef __CMCESIPDEFAULTS_H
#define __CMCESIPDEFAULTS_H

// INCLUDES
#include <e32base.h>
#include <badesca.h>
#include "mcesrv.h"

// FORWARD DECLARATIONS
class CMceSipDefaultData;

// CLASS DECLARATION

/**
*  Default SIP data container. 
*
*  @lib 
*/
class CMceSipDefaults : public CBase
    {
    public: // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CMceSipDefaults* NewL();
        
        /**
	    * Destructor.
	    */
		~CMceSipDefaults();

    public: // New functions
        
        /**
        * Store default headers
        * @param aMethods the methods
        * @param aHeaders the default headers
        */
        void StoreDefaultHeadersL( const CDesC8Array& aMethods,
                                   CDesC8Array* aHeaders );

        /**
        * Returns the default headers for the given method.
        * NULL, if not stored.
        * @param aMehtod the method
        */
        CDesC8Array& DefaultHeaders( const TDesC8& aMethod );
        
        /**
	    * Returns the default headers for the given method
	    * @param aMethodInd the method index in SIP Strings
	    */ 
	    CDesC8Array& DefaultHeaders( TInt aMethodInd );
	
	    
    private:
    
        /**
	    * C++ default constructor.
	    */
        CMceSipDefaults();
        
        /**
	    * By default Symbian 2nd phase constructor is private.
	    */
	    void ConstructL( );
	    
    private: 
        
        /**
        * Store default headers
        * @param aMethod, the method
        * @param aHeaders, the default headers
        */
        void StoreDefaultHeadersL( const TDesC8& aMethod, 
                                   CDesC8Array* aHeaders );
                               
        /**
        * Finds default data for a given method
        * @param aMethod, the method
        * @return the index of the data, KErrNotFound, if not found.
        */
        TInt FindData( const TDesC8& aMethod ) const;
        
        /**
        * Finds default data for a given method index
        * @param aMethodInd, the methodInd
        * @return the index of the data, KErrNotFound, if not found.
        */
        TInt FindData( TInt aMethodInd ) const;
        
        
        /**
        * Clear the defaults.
        */
        void Clear();
        
        /**
        * Copy headers.
        * @param the headers to be copied.
        * @return the copied headers, ownership is transfered.
        */
        CDesC8Array* CopyHeadersL( const CDesC8Array* aHeaders );
        
    private: // Data
        
        // The stored default data in array, owned.
        RPointerArray< CMceSipDefaultData > iDataArray;
        
        CDesC8Array* iEmptyHeaders;
        
        //definitions for unit testing
	    MCESRV_UT_DEFINITIONS

    };

#endif  // __CMCESIPDEFAULTS_H

// End of File