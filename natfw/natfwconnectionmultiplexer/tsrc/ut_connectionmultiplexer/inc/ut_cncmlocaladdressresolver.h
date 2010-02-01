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




#ifndef __UT_UT_CNcmLocalAddressResolver_H__
#define __UT_UT_CNcmLocalAddressResolver_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <in_sock.h>

//  INTERNAL INCLUDES

//  FORWARD DECLARATIONS
class CNcmLocalAddressResolver;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CNcmLocalAddressResolver
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNcmLocalAddressResolver* NewL();
        static UT_CNcmLocalAddressResolver* NewLC();
        
        /**
         * Destructor
         */
        ~UT_CNcmLocalAddressResolver();
        
    private:    // Constructors and destructors

        UT_CNcmLocalAddressResolver();
        void ConstructL();

    private:    // New methods

        void SetupL();

        void Teardown();


        void UT_CNcmLocalAddressResolver_GetLocalAddrLL();
        
        void UT_CNcmLocalAddressResolver_CheckAndSetAddrLL();
       
        void UT_CNcmLocalAddressResolver_MatchL(); 
         
    private:    // Data

		CNcmLocalAddressResolver* iResolver;
        
        TInetAddr iIPv4Address;
        TInetAddr iIPv6Address;
        
        RSocketServ iSocketSrv;
        RConnection iConnection;
        RSocket iSocket;
        
		EUNIT_DECLARE_TEST_TABLE;	
    };

#endif      //  __UT_UT_CNcmLocalAddressResolver_H__

