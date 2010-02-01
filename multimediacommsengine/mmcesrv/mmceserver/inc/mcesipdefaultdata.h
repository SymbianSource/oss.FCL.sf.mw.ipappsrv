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




#ifndef __CMCESIPDEFAULTDATA_H
#define __CMCESIPDEFAULTDATA_H

// INCLUDES
#include <e32base.h>
#include <badesca.h>
#include "mcesrv.h"

// CLASS DECLARATION

/**
*  Default data (headers) for certain SIP request. 
*
*  @lib 
*/
class CMceSipDefaultData : public CBase
    {
    public: // Constructors and destructor
        
        /**
        * Two-phased constructor.
        * @param aMethod the SIP method for which the headers are set
        * @param aHeaders the default headers, ownership is taken
        * @param aDataArray the data array
        */
        static CMceSipDefaultData* NewL(
                        const TDesC8& aMethod,
                        CDesC8Array* aHeaders,
                        RPointerArray< CMceSipDefaultData >& aDataArray );
        /*
	    * Destructor.
	    */
		~CMceSipDefaultData();

    public: // New functions
        
        /**
        * Compares given method to the method index.
        * @oaram aMethod the method
        * @return ETrue, if the given method is the one this.
        *                instance stroes headers for.
        */
        TBool Match( const TDesC8& aMethod );
        
        /**
        * Compares given method index to the method index.
        * @oaram aMethodInd the method index
        * @return ETrue, if the given method is the one this.
        *                instance stroes headers for.
        */
        TBool Match( TInt aMethodInd );
        
        /**
        * Resets dehault headers for this method
        * @param aHeaders the new headers, ownership is taken.
        */
        void SetHeaders( CDesC8Array* aHeaders );
        
        /**
        * Returns the default headers seet for this method.
        * @return the headers.
        */
        CDesC8Array& DefaultHeaders();
           
    private:
    
        /**
	    * C++ default constructor.
	    */
        CMceSipDefaultData();
        
        /**
	    * By default Symbian 2nd phase constructor is private.
	    */
	    void ConstructL( const TDesC8& aMethod,
                         CDesC8Array* aHeaders,
                         RPointerArray< CMceSipDefaultData >& aDataArray );


    private: // Data
        
        // Index of the method in SIP String table.
        TInt iMethodIndex;
        
        // Default headers, owned
        CDesC8Array* iHeaders;
        
        __DECLARE_TEST;
        
        //definitions for unit testing
	    MCESRV_UT_DEFINITIONS

    };

#endif  // __CMCESIPDEFAULTDATA_H

// End of File