/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef __UT_UT_CMceLocalAddrResolver_H__
#define __UT_UT_CMceLocalAddrResolver_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <es_sock.h>

//  INTERNAL INCLUDES

//  FORWARD DECLARATIONS
class CMceLocalAddrResolver;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceLocalAddrResolver
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceLocalAddrResolver* NewL();
        static UT_CMceLocalAddrResolver* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceLocalAddrResolver();
        
    private:    // Constructors and destructors

        UT_CMceLocalAddrResolver();
        void ConstructL();

    private:    // New methods

        void SetupL();

        void Teardown();

        void UT_CMceLocalAddrResolver_MatchL();
        
        void UT_CMceLocalAddrResolver_SetAddressesIfNotSetL();
        
        void UT_CMceLocalAddrResolver_RefreshLocalAddressesLL();
        
         
    private:    // Data

        RSocketServ iSocketServ;
		CMceLocalAddrResolver* iResolver;
        
		EUNIT_DECLARE_TEST_TABLE;	
    };

#endif      //  __UT_UT_CMceLocalAddrResolver_H__

