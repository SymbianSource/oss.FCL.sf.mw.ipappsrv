/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Represents stream collection.
*
*/




#ifndef CNATFWSTREAMCOLLECTION_H
#define CNATFWSTREAMCOLLECTION_H

#include <e32base.h>
#include "natfwinternaldefs.h"    //UNIT_TEST

/**
 *  Streamcollection represents (SDP) media stream, which consists of
 *  multiple media components (e.g. RTP & RTCP).
 *
 *  @lib icecandidatehandler.lib
 *  @since S60 v3.2
 */
class CIceStreamCollection : public CBase
    {
    
    UNIT_TEST( UT_CIceStreamCollection )

public:

    /**
     * Two-phased constructor.
     * @param aCollectionId     Identifier for the stream collection
     */
    static CIceStreamCollection* NewL( TUint aCollectionId );
    
    /**
     * Two-phased constructor.
     * @param aCollectionId     Identifier for the stream collection
     */
    static CIceStreamCollection* NewLC( TUint aCollectionId );
    
    /**
     * Destructor.
     */
    virtual ~CIceStreamCollection();

    /**
     * Returns streamcollection identifier.
     *
     * @since   S60 v3.2
     * return   Streamcollection identifier
     */
    TUint StreamCollectionId() const;
    
    /**
     * Adds media component to the collection.
     *
     * @since   S60 v3.2
     * @param   aStreamId     The stream identifier
     * @param   aComponentId  The media component identifier
     */
    void AddMediaComponentL( TUint aStreamId, TUint aComponentId );
    
    /**
     * Removes media component from the collection.
     *
     * @since   S60 v3.2
     * @param   aStreamId     The stream identifier
     */    
    void RemoveMediaComponent( TUint aStreamId );
    
    /**
     * Returns ETrue if collection has spcified media component.
     *
     * @since   S60 v3.2
     * @param   aStreamId     The stream identifier
     */
    TBool HasMediaComponent( TUint aStreamId ) const;
    
    /**
     * Returns number of media components.
     *
     * @since   S60 v3.2
     * return   Number of media components
     */
    TInt NumOfMediaComponents() const;
    
private:

    CIceStreamCollection( TUint aCollectionId );

private: // data

    class TIceMediaComponent
        {
    public:
        inline TIceMediaComponent( TUint aStreamId, TUint aComponentId )
            : iStreamId( aStreamId ), iComponentId( aComponentId )
            {
            
            }
        
        TUint inline StreamId() const { return iStreamId; }
        TUint inline ComponentId() const { return iComponentId; }
    
    private:
        TUint iStreamId;
        TUint iComponentId;
        };
    
    /**
     * Stream collection identifier.
     */
    TUint iCollectionId;
    
    /**
     * Media components owned by this collection.
     */    
    RArray<TIceMediaComponent> iMediaComponents;
    };

#endif // CNATFWSTREAMCOLLECTION_H
