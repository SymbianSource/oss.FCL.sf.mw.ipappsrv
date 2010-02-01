/*
* Copyright (c) 2004-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Interface for classes capable to act as data sink for RTP data.
*
*/





#ifndef RTPDATASINK_H
#define RTPDATASINK_H

// FORWARD DECLARATIONS
class CMMFBuffer;
class MDataSource;
class TRtpSendHeader;

// CLASS DECLARATION

/**
*  Interface for classes capable to act as data sink for RTP data.
*
*  @lib N/A
*  @since Series 60 3.0
*/
class MMccRtpDataSink
    {
    public: // New functions

        /**
        * Empties specified buffer.
        *
        * @since    Series 60 3.0
        * @param    aBuffer     Buffer containing the RTP packet
        * @param    aSupplier   Data source to notify when buffer is emptied
        * @param    aMediaId    Identifies the media type of a particular stream
        * @param    aHeaderInfo RTP packet header
        * @return   void
        */
        virtual void EmptyBufferL( CMMFBuffer* aBuffer, 
                                   MDataSource* aSupplier,
                                   TMediaId aMediaId,
                                   TRtpSendHeader& aHeaderInfo ) = 0;
    };

#endif      // RTPDATASINK_H   
            
// End of File
