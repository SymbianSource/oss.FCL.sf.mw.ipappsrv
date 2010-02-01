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
* Description:    
*
*/




#ifndef CSIPDIALOGASSOCBASE_H
#define CSIPDIALOGASSOCBASE_H

// INCLUDES
#include <stringpool.h>
#include "_sipcodecdefs.h"

// FORWARD DECLARATIONS
class CSIPMessageElements;
class CSIPClientTransaction;
class CSIPDialog;
class CSIPRefresh;
class CSIPServerTransaction;
class CSIPDialogAssocImplementation;

// CLASS DECLARATION

/**
*  Base class for SIP dialog associations. It provides services for getting
*  associated SIP dialog, actual dialog association type and for sending non
*  target refresh requests within the dialog association.
*  @lib sipclient
*/
class CSIPDialogAssocBase : public CBase
	{
	public: // Destructor

		/**
		* Destructor
		*/
		virtual ~CSIPDialogAssocBase();    

	public: // New functions

		/**
		* Gets the dialog this dialog association belongs to
		* @return associated dialog
		*/
		IMPORT_C const CSIPDialog& Dialog() const;

		/**
		* Gets dialog this dialog association belongs to
		* @return associated dialog
		*/
		IMPORT_C CSIPDialog& Dialog();

		/**
		* Creates SIP request and sends it to the remote target.
		* This function should be used for sending SIP extension
		* requests within dialog assocation that do not cause 
		* sip dialog associations to be created. In SIP terms
		* this request is non target refresh request.
		* @pre SIPDialog().State() != CSIPDialog::EInit &&
		*      SIPDialog().State() != CSIPDialog::ETerminated
        * @pre Dialog().Connection().State() == EActive
		* @pre aMethod != "REGISTER" |"INVITE" | "BYE" | "CANCEL" | "ACK" |
        *   "SUBSCRIBE" | "NOTIFY" | "UPDATE" | "PRACK" | "REFER"
		* @param aElements contains optional SIP message headers and body.
        *   Ownership is transferred.
		* @return SIP extension client transaction. Ownership is transferred.
		* @leave KErrSIPInvalidDialogState If dialog's state is not correct
		* @leave KErrArgument if invalid value has been set to aMethod		
		* @leave KErrSIPResourceNotAvailable if a required SIP Client API
		*	object has been deleted
		* @capability NetworkServices
		*/
		IMPORT_C virtual CSIPClientTransaction*
            SendNonTargetRefreshRequestL(RStringF aMethod,
                                         CSIPMessageElements* aElements);

        /**
        * Gets dialog association type.
        *
        * @return dialog association type e.g. "INVITE", "SUBSCRIBE" etc...
        */
        IMPORT_C RStringF Type() const;

		
	protected: // Constructors, for internal use

        /*
		* Constructor
		*/
        CSIPDialogAssocBase();

        void ConstructL(RStringF aType, CSIPDialog* aDialog);
        void ConstructL(RStringF aType,
        				CSIPDialog* aDialog,
        				CSIPServerTransaction& aTransaction);

    protected: // Data

        CSIPDialog* iDialog;
        RStringF iType;
        CSIPServerTransaction* iTransaction;

	};

#endif
