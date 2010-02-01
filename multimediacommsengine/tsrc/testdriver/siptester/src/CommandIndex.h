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
* Description:  This file lists all supported commands of SIPTester
*
*/

#ifndef __COMMANDINDEX_H__
#define __COMMANDINDEX_H__

// INCLUDES
#include "TCmdCreateConnection.h"
#include "TCmdCreateInviteDialogAssociation.h"
#include "TCmdCreateNewConnection.h"
#include "TCmdCreateProfile.h"
#include "TCmdCreateProfileRegistry.h"
#include "TCmdCreateNotifyDialogAssociation.h"
#include "TCmdDeleteFile.h"
#include "TCmdDeleteProfile.h"
#include "TCmdDestroyProfile.h"
#include "TCmdDestroyAllProfiles.h"
#include "TCmdDisableProfile.h"
#include "TCmdEnableProfile.h"
#include "TCmdFetchRegistrations.h"
#include "TCmdGetConnectionState.h"
#include "TCmdGetDefaultProfile.h"
#include "TCmdGetDialogAOR.h"
#include "TCmdGetDialogFrom.h"
#include "TCmdGetDialogRegistration.h"
#include "TCmdGetDialogRemoteURI.h"
#include "TCmdGetDialogState.h"
#include "TCmdGetIAPName.h"
#include "TCmdGetIAPNames.h"
#include "TCmdGetMessage.h"
#include "TCmdGetMessageAndCreateAssoc.h"
#include "TCmdGetOutboundProxy.h"
#include "TCmdGetProfile.h"
#include "TCmdGetProfiles.h"
#include "TCmdGetProfileStatus.h"
#include "TCmdGetRefresh.h"
#include "TCmdGetRefreshState.h"
#include "TCmdGetRegistrationAOR.h"
#include "TCmdGetRegistrationContact.h"
#include "TCmdGetSecurityMechanisms.h"
#include "TCmdGetSigCompCompartmentCount.h"
#include "TCmdGetTransactionState.h"
#include "TCmdGetTransactionType.h"
#include "TCmdIsCancelAllowed.h"
#include "TCmdIsClientTransaction.h"
#include "TCmdIsContextActive.h"
#include "TCmdIsResponseAllowed.h"
#include "TCmdIsSigCompSupported.h"
#include "TCmdSendAck.h"
#include "TCmdSendBye.h"
#include "TCmdSendCancel.h"
#include "TCmdSendInvite.h"
#include "TCmdSendInviteUsingProfile.h"
#include "TCmdSendInviteWithinDialog.h"
#include "TCmdSendInviteWithinDriver.h"
#include "TCmdSendMessage.h"
#include "TCmdSendNonTargetRefreshRequest.h"
#include "TCmdSendNotify.h"
#include "TCmdSendRefer.h"
#include "TCmdSendReferResponseCreateDialog.h"
#include "TCmdSendReferWithinDialog.h"
#include "TCmdSendPrack.h"
#include "TCmdSendRegister.h"
#include "TCmdSendRequest.h"
#include "TCmdSendResponse.h"
#include "TCmdSendResponseCreateAssoc.h"
#include "TCmdSendResponseCreateDialog.h"
#include "TCmdSendSubscribe.h"
#include "TCmdSendSubscribeUsingProfile.h"
#include "TCmdSendSubscribeWithinDialog.h"
#include "TCmdSendUnsubscribe.h"
#include "TCmdSendUpdate.h"
#include "TCmdSetOutboundProxy.h"
#include "TCmdSetSecurityParams.h"
#include "TCmdTerminate.h"
#include "TCmdTerminateProcess.h"
#include "TCmdTest.h"
#include "TCmdTransferFile.h"
#include "TCmdUnregister.h"
#include "TCmdUpdateNotify.h"
#include "TCmdUpdateRefer.h"
#include "TCmdUpdateProfile.h"
#include "TCmdUpdateRefresh.h"
#include "TCmdUpdateRegistration.h"
#include "TCmdUpdateInvite.h"
#include "TCmdUpdateSubscribe.h"
#include "TCmdGetChallenge.h"
#include "TCmdIgnoreChallenge.h"
#include "TCmdRemoveCredentials.h"
#include "TCmdSetCredentials.h"
#include "TCmdDeleteConnection.h"
#include "TCmdRemoveHttpObserver.h"
#include "TCmdFreezeTester.h"
#include "TCmdCreateSession.h"
#include "TCmdRegisterClient.h"
#include "TCmdChannel.h"
#include "TCmdGetClientDiscoveryStatus.h"
#include "TCmdAuthenticate.h"
#include "TCmdGetMemoryUsage.h"
#include "TCmdStopIAP.h"
#include "TCmdSetHttpDigestObserver2.h"
#include "TCmdSetSecurityHandling.h"
#include "TCmdReuseDialogInitialRequestData.h"
#include "TCmdSetClientMatchType.h"
#include "TCmdAddUserToClientResolverCenRep.h"
#include "TCmdRemoveUserFromClientResolverCenRep.h"
#include "TCmdSetRepositoryIntValue.h"
#include "TCmdGetRepositoryIntValue.h"

#ifdef UNSAF_COMMANDS_INCLUDED
#include "TCmdGetUNSAFTimers.h"
#include "TCmdSetUNSAFTimers.h"
#include "TCmdEnableCRLFRefresh.h"
#include "TCmdSetSTUNServer.h"
#include "TCmdSetDomainParams.h"
#include "TCmdGetDomainParams.h"
#endif

#ifndef NO_CLIENT_INSTALLER
#include "TCmdInstallClient.h"
#include "TCmdUninstallClient.h"
#endif

/// SIP Command index
/// List all commands in this table
const TcCommand KTcSIPCommands[] =
    {
    AddCmd( TCmdAuthenticate ),
    AddCmd( TCmdCreateConnection ),
    AddCmd( TCmdCreateInviteDialogAssociation ),
    AddCmd( TCmdCreateNewConnection ),
    AddCmd( TCmdCreateProfile ),
    AddCmd( TCmdCreateProfileRegistry ),
    AddCmd( TCmdCreateNotifyDialogAssociation ),
    AddCmd( TCmdDeleteConnection ),
    AddCmd( TCmdDeleteFile ),
    AddCmd( TCmdDeleteProfile ),
    AddCmd( TCmdDestroyProfile ),
    AddCmd( TCmdDestroyAllProfiles ),
    AddCmd( TCmdDisableProfile ),
    AddCmd( TCmdEnableProfile ),
    AddCmd( TCmdFetchRegistrations ),
    AddCmd( TCmdFreezeTester ),
    AddCmd( TCmdGetChallenge ),
    AddCmd( TCmdGetConnectionState ),
    AddCmd( TCmdGetDefaultProfile ),
    AddCmd( TCmdGetDialogAOR ),
    AddCmd( TCmdGetDialogFrom ),
    AddCmd( TCmdGetDialogRegistration ),
    AddCmd( TCmdGetDialogRemoteURI ),
    AddCmd( TCmdGetDialogState ),
    AddCmd( TCmdGetIAPName ),
    AddCmd( TCmdGetIAPNames ),
    AddCmd( TCmdGetMessage ),
    AddCmd( TCmdGetMessageAndCreateAssoc ),
    AddCmd( TCmdGetOutboundProxy ),
    AddCmd( TCmdGetProfile ),
    AddCmd( TCmdGetProfiles ),
    AddCmd( TCmdGetProfileStatus ),
    AddCmd( TCmdGetRefresh ),
    AddCmd( TCmdGetRefreshState ),
    AddCmd( TCmdGetRegistrationAOR ),
    AddCmd( TCmdGetRegistrationContact ),
    AddCmd( TCmdGetSecurityMechanisms ),
    AddCmd( TCmdGetSigCompCompartmentCount ),
    AddCmd( TCmdGetTransactionState ),
    AddCmd( TCmdGetTransactionType ),
    AddCmd( TCmdIgnoreChallenge ),
    AddCmd( TCmdIsCancelAllowed ),
    AddCmd( TCmdIsClientTransaction ),
    AddCmd( TCmdIsContextActive ),
    AddCmd( TCmdIsResponseAllowed ),
    AddCmd( TCmdIsSigCompSupported ),
    AddCmd( TCmdRemoveCredentials ),
    AddCmd( TCmdRemoveHttpObserver ),
    AddCmd( TCmdSendAck ),
    AddCmd( TCmdSendBye ),
    AddCmd( TCmdSendCancel ),
    AddCmd( TCmdSendInvite ),
    AddCmd( TCmdSendInviteUsingProfile ),
    AddCmd( TCmdSendInviteWithinDialog ),
    AddCmd( TCmdSendInviteWithinDriver ),
    AddCmd( TCmdSendMessage ),
    AddCmd( TCmdSendNonTargetRefreshRequest ),
    AddCmd( TCmdSendNotify ),
    AddCmd( TCmdSendPrack ),
    AddCmd( TCmdSendRefer ),
    AddCmd( TCmdSendReferResponseCreateDialog ),
    AddCmd( TCmdSendReferWithinDialog ),
    AddCmd( TCmdSendRegister ),
    AddCmd( TCmdSendRequest ),
    AddCmd( TCmdSendResponse ),
    AddCmd( TCmdSendResponseCreateAssoc ),
    AddCmd( TCmdSendResponseCreateDialog ),
    AddCmd( TCmdSendSubscribe ),
    AddCmd( TCmdSendSubscribeUsingProfile ),
    AddCmd( TCmdSendSubscribeWithinDialog ),
    AddCmd( TCmdSendUnsubscribe ),
    AddCmd( TCmdSendUpdate ),
    AddCmd( TCmdSetCredentials ),
    AddCmd( TCmdSetOutboundProxy ),
    AddCmd( TCmdSetSecurityParams ),
    AddCmd( TCmdStopIAP ),
    AddCmd( TCmdTerminate ),
    AddCmd( TCmdTerminateProcess ),
    AddCmd( TCmdTest ),
    AddCmd( TCmdTransferFile ),
    AddCmd( TCmdUnregister ),
    AddCmd( TCmdUpdateInvite ),
    AddCmd( TCmdUpdateNotify ),
    AddCmd( TCmdUpdateProfile ),
    AddCmd( TCmdUpdateRefer ),
    AddCmd( TCmdUpdateRefresh ),
    AddCmd( TCmdUpdateRegistration ),
    AddCmd( TCmdUpdateSubscribe ),
    AddCmd( TCmdGetMemoryUsage ),
    AddCmd( TCmdSetHttpDigestObserver2 ),
    AddCmd( TCmdSetSecurityHandling ),
    AddCmd( TCmdReuseDialogInitialRequestData ),
    AddCmd( TCmdSetClientMatchType ),
    AddCmd( TCmdAddUserToClientResolverCenRep ),
    AddCmd( TCmdRemoveUserFromClientResolverCenRep ),
    AddCmd( TCmdSetRepositoryIntValue ),
	AddCmd( TCmdGetRepositoryIntValue )

#ifdef RD_SIP_TESTER
    ,
	AddCmd( TCmdChannel ),
	AddCmd( TCmdCreateSession ),	
	AddCmd( TCmdGetClientDiscoveryStatus ),
	AddCmd( TCmdRegisterClient )
#endif // RD_SIP_TESTER    
    
#ifdef UNSAF_COMMANDS_INCLUDED
    ,
    AddCmd( TCmdGetUNSAFTimers ),
    AddCmd( TCmdSetUNSAFTimers ),
    AddCmd( TCmdEnableCRLFRefresh ),
    AddCmd( TCmdSetSTUNServer ),
    AddCmd( TCmdSetDomainParams ),
    AddCmd( TCmdGetDomainParams ) 
#endif

#ifndef NO_CLIENT_INSTALLER
    ,
    AddCmd( TCmdInstallClient ),
    AddCmd( TCmdUninstallClient )  
#endif
};

#endif // __COMMANDINDEX_H__
