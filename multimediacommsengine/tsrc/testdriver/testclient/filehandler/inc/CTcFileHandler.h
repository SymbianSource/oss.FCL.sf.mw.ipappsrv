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
* Description:  See class definition below.
*
*/

#ifndef __CTCFILEHANDLER_H__
#define __CTCFILEHANDLER_H__

//  INCLUDES
#include <e32base.h>
#include "RTcFileHandler.h"

//  CLASS DEFINITION
//
class CTcFileHandler : public CBase
    {
    public: // Constructors and destructor

		/// Static constructor.
		IMPORT_C static CTcFileHandler* NewL();

		/// Destructor
		IMPORT_C ~CTcFileHandler();
		
		IMPORT_C void CreateFileL( const TDesC8& aDestinationPath, 
		                           const TDesC8& aData );
		                         
		IMPORT_C void CopyFileL( const TDesC8& aSourcePath, 
		                         const TDesC8& aDestiantionPath );
		                         
		IMPORT_C void DeleteFileL( const TDesC8& aDestinationPath );

    private: // Constructors

		/// Default constructor.
		CTcFileHandler();

        // Second phase constructor
        void ConstructL();

    private: // Data

		/// FileHandler client. Owned.
		RTcFileHandler iHandler;

    };

#endif // __CTCFILEHANDLER_H__
