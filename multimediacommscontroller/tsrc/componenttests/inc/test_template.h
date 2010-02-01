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




#ifndef TEST_H
#define TEST_H

// INCLUDES
#include <e32cons.h>
#include <e32base.h>
#include "mmccnetworksettings.h"

// FORWARD DECLARATIONS

// CLASS
class CTest : public CBase
    {
    public:
        static CTest* NewL( CConsoleBase& aConsole, 
                            TMccNetSettings aNetsettings );
        virtual ~CTest();
    
    public:
    
    private:
    
    private:
        CTest();
        void ConstructL( CConsoleBase& aConsole, 
                         TMccNetSettings aNetsettings );
    
    private:
        // Not own
        CConsoleBase* iConsole;
        
        TMccNetSettings aNetsettings;
    };
    
#endif //TEST_H

// End of File
