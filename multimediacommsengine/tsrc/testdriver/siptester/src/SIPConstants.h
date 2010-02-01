/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
#ifndef __SIPCONSTANTS_H__
#define __SIPCONSTANTS_H__

// INCLUDES
#include "TesterConstants.h"

// CONSTANTS

/// SIP header names
_LIT8( KHeaderContact, "Contact");
_LIT8( KHeaderCSeq, "CSeq");
_LIT8( KHeaderFrom, "From" );
_LIT8( KHeaderProxy, "Proxy" );
_LIT8( KHeaderRoute, "Route" );
_LIT8( KHeaderServiceRoute, "Service-Route" );
_LIT8( KHeaderTo, "To");
_LIT8( KHeaderContentType, "Content-Type" );
_LIT8( KHeaderExpires, "Expires" );
_LIT8( KHeaderReferTo, "Refer-To" );
_LIT8( KHeaderSubState, "Subscription-State" );
_LIT8( KHeaderEvent, "Event" );


/// SIP Header separator character
const TUint8 KHeaderSeparator( ':' );
/// Whitespace
_LIT8( KSpace, " ");
/// Horizontal tab
_LIT8( KTab, "\t" );

/// CTRL parameter types
_LIT8( KParamActionName, "action" );
_LIT8( KParamIAPName, "IAPName" );
_LIT8( KParamIAPNames, "IAPNames" );
_LIT8( KParamAddress, "Address" );
_LIT8( KParamApplicationUid, "ApplicationUid" );
_LIT8( KParamPluginUid, "PluginUid" );
_LIT8( KParamAssociationType, "AssociationType" );
_LIT8( KParamAuthenticationMechanism, "AuthenticationMechanism" );
_LIT8( KParamBoolean, "Boolean" );
_LIT8( KParamClientDiscoveryErrorCode, "ClientDiscoveryErrorCode" );
_LIT8( KParamClientDiscoveryRequestId, "ClientDiscoveryRequestId" );
_LIT8( KParamClientDiscoveryStatus, "ClientDiscoveryStatus" );
_LIT8( KParamClientInfo, "ClientInfo" );
_LIT8( KParamCompartmentCount, "CompartmentCount" );
_LIT8( KParamConnectionState, "ConnectionState" );
_LIT8( KParamContent, "Content" );
_LIT8( KParamData, "data" );
_LIT8( KParamDataLength, "byteLen" );
_LIT8( KParamDestinationPath, "dest" );
_LIT8( KParamDialogState, "DialogState" );
_LIT8( KParamDigestRealm, "DigestRealm" );
_LIT8( KParamDigestUsername, "DigestUsername" );
_LIT8( KParamDigestPassword, "DigestPassword" );
_LIT8( KParamExpires, "Expires" );
_LIT8( KParamEventType, "EventType" );
_LIT8( KParamFile, "File" );
_LIT8( KParamMethod, "Method" );
_LIT8( KParamProcess, "Process" );
_LIT8( KParamReason, "Reason" );
_LIT8( KParamInterfaceImpUid, "InterfaceImplemetationUid" );
_LIT8( KParamRefresh, "Refresh" );
_LIT8( KParamRefreshState, "RefreshState" );
_LIT8( KParamRemoteURI, "RemoteURI" );
_LIT8( KParamSecurityMechanisms, "SecurityMechanisms" );
_LIT8( KParamSecurityParams, "SecurityParams" );
_LIT8( KParamStackError, "StackError" );
_LIT8( KParamStackErrorMsg, "StackErrorMsg" );
_LIT8( KParamStatusCode, "StatusCode" );
_LIT8( KParamTimeout, "Timeout" );
_LIT8( KParamTransactionState, "TransactionState" );
_LIT8( KParamTransactionType, "TransactionType" );
_LIT8( KParamProfileEnabled, "ProfileEnabled" );
_LIT8( KParamProfileType, "ProfileType" );
_LIT8( KParamProfileClass, "Class" );
_LIT8( KParamProfileName, "ProfileName" );
_LIT8( KParamProfileAORs, "ProfileAORList" );
_LIT8( KParamProfileAOR, "ProfileAOR" );
_LIT8( KParamProfileHeaders, "ProfileHeadersList" );
_LIT8( KParamProfileHeaderParams, "ProfileHeaderParamsList" );
_LIT8( KParamProfilePrivateIdentity, "PrivateIdentity" );
_LIT8( KParamProfileSigComp, "SigComp" );
_LIT8( KParamProfileAutoRegistration, "AutoRegistration" );
_LIT8( KParamProfileSecurityNegotiation, "SecurityNegotiation" );
_LIT8( KParamNegotiatedSecMechanism, "NegotiatedSecMechanism" );
_LIT8( KParamProfileServer, "Server" );
_LIT8( KParamProfileAddress, "Address" );
_LIT8( KParamProfileServerParameter, "ServerParameter" );
_LIT8( KParamProfileParameter, "Parameter" );
_LIT8( KParamProfileExtensionParameter, "ExtensionParameter" );
_LIT8( KParamProfileServerExtensionParameter, "ServerExtensionParameter" );
_LIT8( KParamProfileValue, "Value" );
_LIT8( KParamProfileDefault, "ProfileDefault" );
_LIT8( KParamProfileId, "ProfileId" );
_LIT8( KParamProfileErrorCode, "ProfileErrorCode" );
_LIT8( KParamProfileRegStatus, "ProfileRegStatus" );
_LIT8( KParamProfileState, "ProfileState" );
_LIT8( KParamProfileProfileIdist, "ProfileIdList" );
_LIT8( KParamRegistryId, "RegistryId" );
_LIT8( KParamSourcePath, "src" );
_LIT8( KParamTestHang, "TestHang" );
_LIT8( KParamTestPanic, "TestPanic" );
_LIT8( KParamProxy, "Proxy" );
_LIT8( KInternet, "Internet" );
_LIT8( KIms, "Ims" );
_LIT8( KIETF, "IETF" );
_LIT8( KOther, "Other" );
_LIT8( KRegistrar, "Registrar" );
_LIT8( KOutboundProxy, "OutboundProxy" );
_LIT8( KDigestRealm, "DigestRealm" );
_LIT8( KDigestUsername, "DigestUsername" );
_LIT8( KDigestPassword, "DigestPassword" );
_LIT8( KParamAuthenticationNonce, "AuthenticationNonce" );
_LIT8( KParamAuthenticationRes, "AuthenticationRes" );
_LIT8( KParamAuthenticationIk, "AuthenticationIk" );
_LIT8( KParamAuthenticationCk, "AuthenticationCk" );
_LIT8( KParamAuthenticationAuts, "AuthenticationAuts" );
_LIT8( KParamUNSAFBindingRequestInterval, "BindingRequestInterval" );
_LIT8( KParamUNSAFUDPKeepaliveInterval, "UDPKeepAliveInterval" );
_LIT8( KParamUNSAFTCPKeepaliveInterval, "TCPKeepAliveInterval" );
_LIT8( KParamUNSAFEnableCRLFRefresh, "EnableCRLFRefresh" );
_LIT8( KParamUNSAFSharedSecretNotSupported, "SharedSecretNotSupported" );
_LIT8( KParamUNSAFBindingRequestFailed, "BindingRequestFailed" );
_LIT8( KParamDomainName, "DomainName" );
_LIT8( KParamSTUNServerDomain, "STUNServerDomain" );
_LIT8( KParamSTUNServerPort, "STUNServerPort" );
_LIT8( KParamClientMatchType, "ClientMatchType" );
_LIT8( KParamClientUserName, "ClientUserName" );
_LIT8( KParamRepositoryUid, "RepositoryUid" );
_LIT8( KParamRepositoryParameterKey, "RepositoryParameterKey" );
_LIT8( KParamRepositoryParameterValue, "RepositoryParameterValue" );
_LIT8( KParamRepositoryIntValue, "RepositoryIntValue" );

/// CTRL return value types
_LIT8( KClientDiscoveryChannelResolved, "ChannelResolved" );
_LIT8( KClientDiscoveryClientNotFound, "ClientNotFound" );
_LIT8( KClientDiscoveryErrorOccured, "ErrorOccured" );
_LIT8( KConnectionId, "ConnectionId" );
_LIT8( KDialogId, "DialogId" );
_LIT8( KInviteDialogId, "InviteDialogId" );
_LIT8( KNotifyDialogId, "NotifyDialogId" );
_LIT8( KProfileId, "ProfileId" );
_LIT8( KReferDialogId, "ReferDialogId" );
_LIT8( KRefreshId, "RefreshId" );
_LIT8( KRegistrationId, "RegistrationId" );
_LIT8( KRegistryId, "RegistryId" );
_LIT8( KServerTransactionId, "ServerTransactionId" );
_LIT8( KSubscribeDialogId, "SubscribeDialogId" );
_LIT8( KTransactionId, "TransactionId" );
_LIT8( KProfilesIds, "ProfilesIDs" );
_LIT8( KProfileCreated, "ProfileCreated" );
_LIT8( KProfileUpdated, "ProfileUpdated" );
_LIT8( KProfileRegistrationStatusChanged, "ProfileRegistrationStatusChanged" );
_LIT8( KProfileDestroyed, "ProfileDestroyed" );
_LIT8( KProfileUnregistered, "ProfileUnregistered" );
_LIT8( KProfileRegistered, "ProfileRegistered" );
_LIT8( KProfileRegistryErrorOccurred, "ProfileRegistryErrorOccurred" );
_LIT8( KUnknown, "Unknown" );
_LIT8( KParamUsedMemory, "UsedMemory" );
_LIT8( KParamFreeMemory, "FreeMemory" );

/// Parameter names in a CTRL response
_LIT8( KResponseTestClientIds, "TestClientIds" );
_LIT8( KResponseCode, "ResponseCode" );
_LIT8( KResponseHeaders, "Headers" );
_LIT8( KResponseActionReturnCode, "ActionReturnCode" );

// Misc
_LIT8( KMethodRegister, "REGISTER" );
_LIT8( KMethodMessage, "MESSAGE" );
_LIT8( KNoHost, "NOHOST" );
_LIT( KInstallerPath, "c:\\" );

const TInt KSIPReasonOk( 200 );
const TInt KDefaultReceiveTimeout( 5 );	// 5 seconds
const TInt KSecondAsMicros( 1000000 );
const TInt KMaxSecurityParamLength( 256 );

const TInt KSIPMessageAtIndex( 0 );

const TInt KFreezeAfterTimer( 3 ); // 3 seconds

// - Error codes --------------------------------------------------------------

// SIP tester specific error codes
const TInt KSIPErrBase( -100 );
const TInt KSIPErrQueueEmpty( KSIPErrBase - 1 );
const TInt KSIPErrInviteCanceled( KSIPErrBase - 2 );
const TInt KSIPErrOOMInNotifier( KSIPErrBase - 3 );	// actually only used as a panic
const TInt KSIPErrMax( KSIPErrOOMInNotifier );

// SIP stack error descriptions (see TError in CSIP/sip.h)
_LIT8( KSIPMalformedMessageErrMsg, "SIP message was malformed" );
_LIT8( KSIPRegistrarRespondedWithDifferentAORErrMsg, "Registrar responded with a different AOR (in the To-header)" );
_LIT8( KSIPRegistrarRespondedWithDifferentSipAddressInContactErrMsg, "Registrar modified Contact-header" );
_LIT8( KSIPRegistrarRespondedWithoutContactErrMsg, "Registrar responded without contact header" );
_LIT8( KSIPRequestPendingErrMsg, "Request pending" );
_LIT8( KSIPResponseWithExpiresZeroErrMsg, "The other end responded with expires value zero" );
_LIT8( KSIPResponseWithDifferentRemoteTagErrMsg, "The other end responded with a different tag in To-header" );
_LIT8( KSIPPortAlreadyInUseErrMsg, "Transport port is already in use" );
_LIT8( KSIPInvalidTransactionStateErrMsg, "The action can't be performed in the current transaction state" );
_LIT8( KSIPInvalidDialogStateErrMsg, "Not allowed in dialogs current state" );
_LIT8( KSIPInvalidDialogCreationRequestErrMsg, "Dialogs creation request invalid" );
_LIT8( KSIPInvalidDialogCreationResponseErrMsg, "Dialogs creation response invalid" );
_LIT8( KSIPInvalidResponseInDialogsErrMsg, "Response in dialogs invalid" );
_LIT8( KSIPInvalidRequestInDialogsErrMsg, "Request in dialogs invalid" );
_LIT8( KSIPOverlappingDialogRequestPendingErrMsg, "Overlapping request pending in a dialog" );
_LIT8( KSIPTransportFailureErrMsg, "Sending a SIP message failed. For example ICMP error occured" );
_LIT8( KSIPNoAckReceivedErrMsg, "No ACK was received after sending a 2xx response" );
_LIT8( KSIPInvalidRegistrationStateErrMsg, "Not allowed in registration's current state" );
_LIT8( KSIPInvalidContactErrMsg, "The contact given did not contain user part" );
_LIT8( KSIPResourceNotAvailableErrMsg, "Object can't access a resource, since that has been deleted by user. User is expected to delete this object as it can no longer be used." );
_LIT8( KSIPResolvingFailureErrMsg, "DNS query for the remote address failed" );
_LIT8( KSIPForbiddenErrMsg, "Authentication with a server failed" );
_LIT8( KSIPMaxCompartmentsInUseErrMsg, "Maximum number of SigComp compartments specified in the ini-file was exceeded" );
_LIT8( KSIPTerminatedWithResponseErrMsg, "Refresh initiated client transaction was terminated with 3xx, 4xx, 5xx or 6xx response." );
_LIT8( KSIPOutboundProxyNotRespondingErrMsg, "Outbound proxy not responding" );
_LIT8( KSIPInviteCanceledErrMsg, "Invite canceled" );
_LIT8( KSIPUnrecognizedErrMsg, "<unrecognized or non-SIP error code>" );

#endif	// __SIPCONSTANTS_H__
