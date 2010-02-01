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
* Description:    Holds a data packet and sender observer for buffering if socket is busy.
*
*/




#ifndef C_CNCMSENDITEM_H
#define C_CNCMSENDITEM_H

#include <e32base.h>

class MNcmSenderObserver;

/**
 *  Holds a data packet and sender observer for buffering if socket is busy.
 *
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CNcmSendItem ) : public CBase
    {
    
public:

    /**
     * Two-phased constructor.
     * @param aData Data for new send item
     * @param aSenderObserver   observer for this data
     */
    static CNcmSendItem* NewL( const TDesC8& aData,
            MNcmSenderObserver* aSenderObserver );

    /**
     * Two-phased constructor.
     * @param aData Data for new send item
     * @param aSenderObserver   observer for this data
     */
    static CNcmSendItem* NewLC( const TDesC8& aData,
            MNcmSenderObserver* aSenderObserver );
        

    /**
     * Destructor.
     */
    virtual ~CNcmSendItem();

    /**
     * Return send item data
     *
     * @since S60 v3.2
     * @return Send item data
     */
     const TDesC8& Data() const;

    /**
     * Return sender observer for this data
     *
     * @since S60 v3.2
     * @return Sender observer
     */
     MNcmSenderObserver* Observer();
    
    /**
     * Set boolean variable, which indicates is send in progress.
     *
     * @since S60 v3.2
     * @param aSendInProgress   ETrue if send is in progress
     * @return  void.
     */
    void SetSendInProgress( TBool aSendInProgress );
    
    /**
     * Return ETrue if send is in progress.
     *
     * @since S60 v3.2
     * @return  ETrue if send is in progress
     */
     TBool SendInProgress() const;
     
     
private:

    CNcmSendItem( MNcmSenderObserver* aObserver );

    void ConstructL( const TDesC8& aData );

private: // data

    /**
     * Data for sending
     * Own.
     */
    HBufC8* iData;

    /**
     * Observer for data
     * Not own.
     */
    MNcmSenderObserver* iObserver;
    
    /**
     * Indicates whether send is in progress.
     */
    TBool iSendInProgress;

    };

#endif // C_CNCMSENDITEM_H
