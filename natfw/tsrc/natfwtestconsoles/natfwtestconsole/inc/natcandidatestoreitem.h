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
* Description:    Candidate store item
*
*/




#ifndef NATCANDIDATESTOREITEM_H
#define NATCANDIDATESTOREITEM_H

#include <e32base.h>
#include <s32strm.h> 

class CNATFWCandidate;

class CNATCandidateStoreItem : public CBase
{
public:
    static CNATCandidateStoreItem* NewL();
    static CNATCandidateStoreItem* NewLC();
    virtual ~CNATCandidateStoreItem();

    void SetNATCandidate( const CNATFWCandidate& aCandidate );
 
    const CNATFWCandidate& NATCandidate() const;
    
    void ExternalizeL( RWriteStream& aStream ) const;
    void InternalizeL( RReadStream& aStream );
    
private:

    CNATCandidateStoreItem();
    void ConstructL();
    
private: // date

    CNATFWCandidate* iCandidate;
};

#endif // NATCANDIDATESTOREITEM_H
