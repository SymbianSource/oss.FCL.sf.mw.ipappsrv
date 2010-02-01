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




#ifndef __CREATEREFERDIALOG_H__
#define __CREATEREFERDIALOG_H__ (0x1)

// INCLUDES
#include <E32Base.h>
#include <eikbutb.h>
#include <eikenv.h>
#include <eikdialg.h>
#include <eikedwin.h>
#include <in_sock.h> // for TInetAddr
#include <es_sock.h> // for SetPort()
#include <utf.h>
#include <coeinput.h>
#include <eikmfne.h>

//#include <akndialog.h>
#include <aknform.h>
#include <AknPopupFieldText.h>

#include "MCETestUI.hrh"
#include "CMCETestUIEngine.h"

class TEventParams ;
/**
 * @class CCreateSessionDialog
 *
 * A dialog that uses a CAknForm control
 * and allows a the user to edit session type parameters
 */

class CCreateReferDialog: public CAknForm
    {
    public:
       /**
         Perform the first phase of two phase construction
        @param aParams Receive parameters
        */
      //  CCreatePoCSessionDialog( CMCETestUIEngine ::TPoCSessionParams* aParams );
         CCreateReferDialog(TReferParams* aParams);

       /**
         Destroy the object
        */
        ~CCreateReferDialog();

    private: // from CEikDialog
       /**
         Exit and return values
        @param aKeyCode Key that terminated dialog
        @result Always returns ETrue, Ok to exit
        */
        TBool OkToExitL(TInt aKeyCode);

       /**
         Perform dynamic initialisation prior to displaying dialog
        */
        void PreLayoutDynInitL();

    private:
        /** @var iParams Return item after dialog has run, owned by caller */
        TReferParams* iParams;
    };

class CCreateReferToDialog: public CAknForm
    {
    public:
       /**
         Perform the first phase of two phase construction
        @param aParams Receive parameters
        */
      //  CCreatePoCSessionDialog( CMCETestUIEngine ::TPoCSessionParams* aParams );
         CCreateReferToDialog(TReferParams* aParams);

       /**
         Destroy the object
        */
        ~CCreateReferToDialog();

    private: // from CEikDialog
       /**
         Exit and return values
        @param aKeyCode Key that terminated dialog
        @result Always returns ETrue, Ok to exit
        */
        TBool OkToExitL(TInt aKeyCode);

       /**
         Perform dynamic initialisation prior to displaying dialog
        */
        void PreLayoutDynInitL();

    private:
        /** @var iParams Return item after dialog has run, owned by caller */
        TReferParams* iParams;
    };

/**
 Append two message and pass it to a single message dialog.
 @param aMessage1
 @param aMessage2
 */
extern void MessageBox( const TDesC& aMessage1, const TDesC& aMessage2 );
       
/**
 Display message box dialog.
 @param aMessage
 */
extern void MessageBox( const TDesC& aMessage );

#endif // __CREATESESSIONDIALOG_H__

// End of File
