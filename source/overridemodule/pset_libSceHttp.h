#pragma once
#include "PsetLibraryCommon.h" 
int PSET_SYSV_ABI Pset_sceHttpAbortRequest(void);
int PSET_SYSV_ABI Pset_sceHttpAbortRequestForce(void);
int PSET_SYSV_ABI Pset_sceHttpAbortWaitRequest(void);
int PSET_SYSV_ABI Pset_sceHttpAddCookie(void);
int PSET_SYSV_ABI Pset_sceHttpAddQuery(void);
int PSET_SYSV_ABI Pset_sceHttpAddRequestHeader(void);
int PSET_SYSV_ABI Pset_sceHttpAddRequestHeaderRaw(void);
int PSET_SYSV_ABI Pset_sceHttpAuthCacheExport(void);
int PSET_SYSV_ABI Pset_sceHttpAuthCacheFlush(void);
int PSET_SYSV_ABI Pset_sceHttpAuthCacheImport(void);
int PSET_SYSV_ABI Pset_sceHttpCacheRedirectedConnectionEnabled(void);
int PSET_SYSV_ABI Pset_sceHttpCookieExport(void);
int PSET_SYSV_ABI Pset_sceHttpCookieFlush(void);
int PSET_SYSV_ABI Pset_sceHttpCookieImport(void);
int PSET_SYSV_ABI Pset_sceHttpCreateConnection(void);
int PSET_SYSV_ABI Pset_sceHttpCreateConnectionWithURL(void);
int PSET_SYSV_ABI Pset_sceHttpCreateEpoll(void);
int PSET_SYSV_ABI Pset_sceHttpCreateRequest(void);
int PSET_SYSV_ABI Pset_sceHttpCreateRequest2(void);
int PSET_SYSV_ABI Pset_sceHttpCreateRequestWithURL(void);
int PSET_SYSV_ABI Pset_sceHttpCreateRequestWithURL2(void);
int PSET_SYSV_ABI Pset_sceHttpCreateTemplate(void);
int PSET_SYSV_ABI Pset_sceHttpDbgEnableProfile(void);
int PSET_SYSV_ABI Pset_sceHttpDbgGetConnectionStat(void);
int PSET_SYSV_ABI Pset_sceHttpDbgGetRequestStat(void);
int PSET_SYSV_ABI Pset_sceHttpDbgSetPrintf(void);
int PSET_SYSV_ABI Pset_sceHttpDbgShowConnectionStat(void);
int PSET_SYSV_ABI Pset_sceHttpDbgShowMemoryPoolStat(void);
int PSET_SYSV_ABI Pset_sceHttpDbgShowRequestStat(void);
int PSET_SYSV_ABI Pset_sceHttpDbgShowStat(void);
int PSET_SYSV_ABI Pset_sceHttpDeleteConnection(void);
int PSET_SYSV_ABI Pset_sceHttpDeleteRequest(void);
int PSET_SYSV_ABI Pset_sceHttpDeleteTemplate(void);
int PSET_SYSV_ABI Pset_sceHttpDestroyEpoll(void);
int PSET_SYSV_ABI Pset_sceHttpGetAcceptEncodingGZIPEnabled(void);
int PSET_SYSV_ABI Pset_sceHttpGetAllResponseHeaders(void);
int PSET_SYSV_ABI Pset_sceHttpGetAuthEnabled(void);
int PSET_SYSV_ABI Pset_sceHttpGetAutoRedirect(void);
int PSET_SYSV_ABI Pset_sceHttpGetConnectionStat(void);
int PSET_SYSV_ABI Pset_sceHttpGetCookie(void);
int PSET_SYSV_ABI Pset_sceHttpGetCookieEnabled(void);
int PSET_SYSV_ABI Pset_sceHttpGetCookieStats(void);
int PSET_SYSV_ABI Pset_sceHttpGetEpoll(void);
int PSET_SYSV_ABI Pset_sceHttpGetEpollId(void);
int PSET_SYSV_ABI Pset_sceHttpGetLastErrno(void);
int PSET_SYSV_ABI Pset_sceHttpGetMemoryPoolStats(void);
int PSET_SYSV_ABI Pset_sceHttpGetNonblock(void);
int PSET_SYSV_ABI Pset_sceHttpGetRegisteredCtxIds(void);
int PSET_SYSV_ABI Pset_sceHttpGetResponseContentLength(void);
int PSET_SYSV_ABI Pset_sceHttpGetStatusCode(void);
int PSET_SYSV_ABI Pset_sceHttpInit(void);
int PSET_SYSV_ABI Pset_sceHttpParseResponseHeader(void);
int PSET_SYSV_ABI Pset_sceHttpParseStatusLine(void);
int PSET_SYSV_ABI Pset_sceHttpReadData(void);
int PSET_SYSV_ABI Pset_sceHttpRedirectCacheFlush(void);
int PSET_SYSV_ABI Pset_sceHttpRemoveRequestHeader(void);
int PSET_SYSV_ABI Pset_sceHttpRequestGetAllHeaders(void);
int PSET_SYSV_ABI Pset_sceHttpsDisableOption(void);
int PSET_SYSV_ABI Pset_sceHttpsDisableOptionPrivate(void);
int PSET_SYSV_ABI Pset_sceHttpsEnableOption(void);
int PSET_SYSV_ABI Pset_sceHttpsEnableOptionPrivate(void);
int PSET_SYSV_ABI Pset_sceHttpSendRequest(void);
int PSET_SYSV_ABI Pset_sceHttpSetAcceptEncodingGZIPEnabled(void);
int PSET_SYSV_ABI Pset_sceHttpSetAuthEnabled(void);
int PSET_SYSV_ABI Pset_sceHttpSetAuthInfoCallback(void);
int PSET_SYSV_ABI Pset_sceHttpSetAutoRedirect(void);
int PSET_SYSV_ABI Pset_sceHttpSetChunkedTransferEnabled(void);
int PSET_SYSV_ABI Pset_sceHttpSetConnectTimeOut(void);
int PSET_SYSV_ABI Pset_sceHttpSetCookieEnabled(void);
int PSET_SYSV_ABI Pset_sceHttpSetCookieMaxNum(void);
int PSET_SYSV_ABI Pset_sceHttpSetCookieMaxNumPerDomain(void);
int PSET_SYSV_ABI Pset_sceHttpSetCookieMaxSize(void);
int PSET_SYSV_ABI Pset_sceHttpSetCookieRecvCallback(void);
int PSET_SYSV_ABI Pset_sceHttpSetCookieSendCallback(void);
int PSET_SYSV_ABI Pset_sceHttpSetCookieTotalMaxSize(void);
int PSET_SYSV_ABI Pset_sceHttpSetDefaultAcceptEncodingGZIPEnabled(void);
int PSET_SYSV_ABI Pset_sceHttpSetDelayBuildRequestEnabled(void);
int PSET_SYSV_ABI Pset_sceHttpSetEpoll(void);
int PSET_SYSV_ABI Pset_sceHttpSetEpollId(void);
int PSET_SYSV_ABI Pset_sceHttpSetHttp09Enabled(void);
int PSET_SYSV_ABI Pset_sceHttpSetInflateGZIPEnabled(void);
int PSET_SYSV_ABI Pset_sceHttpSetNonblock(void);
int PSET_SYSV_ABI Pset_sceHttpSetPolicyOption(void);
int PSET_SYSV_ABI Pset_sceHttpSetPriorityOption(void);
int PSET_SYSV_ABI Pset_sceHttpSetProxy(void);
int PSET_SYSV_ABI Pset_sceHttpSetRecvBlockSize(void);
int PSET_SYSV_ABI Pset_sceHttpSetRecvTimeOut(void);
int PSET_SYSV_ABI Pset_sceHttpSetRedirectCallback(void);
int PSET_SYSV_ABI Pset_sceHttpSetRequestContentLength(void);
int PSET_SYSV_ABI Pset_sceHttpSetRequestStatusCallback(void);
int PSET_SYSV_ABI Pset_sceHttpSetResolveRetry(void);
int PSET_SYSV_ABI Pset_sceHttpSetResolveTimeOut(void);
int PSET_SYSV_ABI Pset_sceHttpSetResponseHeaderMaxSize(void);
int PSET_SYSV_ABI Pset_sceHttpSetSendTimeOut(void);
int PSET_SYSV_ABI Pset_sceHttpSetSocketCreationCallback(void);
int PSET_SYSV_ABI Pset_sceHttpsFreeCaList(void);
int PSET_SYSV_ABI Pset_sceHttpsGetCaList(void);
int PSET_SYSV_ABI Pset_sceHttpsGetSslError(void);
int PSET_SYSV_ABI Pset_sceHttpsLoadCert(void);
int PSET_SYSV_ABI Pset_sceHttpsSetMinSslVersion(void);
int PSET_SYSV_ABI Pset_sceHttpsSetSslCallback(void);
int PSET_SYSV_ABI Pset_sceHttpsSetSslVersion(void);
int PSET_SYSV_ABI Pset_sceHttpsUnloadCert(void);
int PSET_SYSV_ABI Pset_sceHttpTerm(void);
int PSET_SYSV_ABI Pset_sceHttpTryGetNonblock(void);
int PSET_SYSV_ABI Pset_sceHttpTrySetNonblock(void);
int PSET_SYSV_ABI Pset_sceHttpUnsetEpoll(void);
int PSET_SYSV_ABI Pset_sceHttpUriBuild(void);
int PSET_SYSV_ABI Pset_sceHttpUriCopy(void);
int PSET_SYSV_ABI Pset_sceHttpUriEscape(void);
int PSET_SYSV_ABI Pset_sceHttpUriMerge(void);
int PSET_SYSV_ABI Pset_sceHttpUriParse(void);
int PSET_SYSV_ABI Pset_sceHttpUriSweepPath(void);
int PSET_SYSV_ABI Pset_sceHttpUriUnescape(void);
int PSET_SYSV_ABI Pset_sceHttpWaitRequest(void);
