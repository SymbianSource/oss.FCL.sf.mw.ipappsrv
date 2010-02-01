/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Candidate store
*
*/




#ifndef NATCANDIDATESTORE_H
#define NATCANDIDATESTORE_H

#include <e32base.h>
#include <f32file.h>
 
class CNATFWCandidate;

class CNATCandidateStore : public CBase
{
public:

    static CNATCandidateStore* NewL();
    static CNATCandidateStore* NewLC();
    virtual ~CNATCandidateStore();

    /**
     * Write candidates to memory card
     *
     * @since   S60 3.2
     * @param   aCandidates    The candidate array
     * @return  void
     */
    void WriteL( const RPointerArray<CNATFWCandidate>& aCandidates );
    
    /**
     * Read candidates from memory card 
     *
     * @since   S60 3.2
     * @param   aCandidates    Returns candidate array
     * @return  void
     */
    void ReadL( RPointerArray<CNATFWCandidate>& aCandidates );
      
private:

    CNATCandidateStore();
    void ConstructL();
    
private: // date

    RFs iFileServer;
};

#endif // NATCANDIDATESTORE_H
