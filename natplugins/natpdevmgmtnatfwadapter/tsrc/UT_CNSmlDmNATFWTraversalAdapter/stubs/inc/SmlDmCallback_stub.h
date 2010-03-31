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
* Description:  ?Description
*
*/



#ifndef SmlDmCallback_stub_H
#define SmlDmCallback_stub_H

#include <smldmadapter.h>

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;

// CLASS DECLARATION

/**
*  ?one_line_short_description.
*  ?other_description_lines
*
*  @lib ?library
*  @since Series ?XX ?SeriesXX_version
*/
class CSmlDmCallback_stub : public MSmlDmCallback
    {
    public:
    
        static CSmlDmCallback_stub* NewL();
        
        /**
        * ?member_description.
        * @since Series ?XX ?SeriesXX_version
        */
        virtual ~CSmlDmCallback_stub();

        void SetResultsL(TInt aResultsRef, CBufBase& aObject,
                                  const TDesC8& aType);
        
        void SetResultsL(TInt aResultsRef, RReadStream*& aStream,
                                  const TDesC8& aType);

        void SetStatusL(TInt aStatusRef,
                                 MSmlDmAdapter::TError aErrorCode);

        void SetMappingL(const TDesC8& aURI, const TDesC8& aLUID);

        void FetchLinkL(const TDesC8& aURI, CBufBase& aData,
                                 MSmlDmAdapter::TError& aStatus);

        HBufC8* GetLuidAllocL(const TDesC8& aURI);
        
        TInt RemoveMappingL( TUint32 aAdapterId, const TDesC8& aURI, TBool aChildAlso );
        
    private:
    
        /**
        * ?member_description.
        * @since Series ?XX ?SeriesXX_version
        */
        CSmlDmCallback_stub();    
        
        void ConstructL();
        
    public:
    
        MSmlDmAdapter::TError iStatus;
        HBufC8* iLUID;
        HBufC8* iLUID2;
        TBuf8<255> iResult;

    };

#endif      // SmlDmCallback_stub_H

// End of File
