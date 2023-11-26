#include "Type.h"

#if PLATFORM_OS & OS_WINDOWS
#include <sdkddkver.h>
#endif
#include <asio/asio.hpp>

BEG_META( XE::HttpMethodType )
type->Value( "GET", XE::HttpMethodType::HTTP_GET );
type->Value( "HEAD", XE::HttpMethodType::HTTP_HEAD );
type->Value( "POST", XE::HttpMethodType::HTTP_POST );
type->Value( "PUT", XE::HttpMethodType::HTTP_PUT );
type->Value( "DELETE", XE::HttpMethodType::HTTP_DELETE );
type->Value( "CONNECT", XE::HttpMethodType::HTTP_CONNECT );
type->Value( "OPTIONS", XE::HttpMethodType::HTTP_OPTIONS );
type->Value( "TRACE", XE::HttpMethodType::HTTP_TRACE );
END_META()

BEG_META( XE::HttpVersionType )
type->Value( "HTTP_1_0", XE::HttpVersionType::HTTP_1_0 );
type->Value( "HTTP_1_1", XE::HttpVersionType::HTTP_1_1 );
END_META()

BEG_META( XE::HttpHeaderFieldType )
type->Value("<unknown-field>", XE::HttpHeaderFieldType::UNKNOWN );
type->Value("A-IM", XE::HttpHeaderFieldType::HTTP_ALLOW );
type->Value("Accept", XE::HttpHeaderFieldType::HTTP_ACCEPT );
type->Value("Accept-Additions", XE::HttpHeaderFieldType::HTTP_ACCEPT_ADDITIONS );
type->Value("Accept-Charset", XE::HttpHeaderFieldType::HTTP_ACCEPT_CHARSET );
type->Value("Accept-Datetime", XE::HttpHeaderFieldType::HTTP_ACCEPT_DATETIME );
type->Value("Accept-Encoding", XE::HttpHeaderFieldType::HTTP_ACCEPT_ENCODING );
type->Value("Accept-Features", XE::HttpHeaderFieldType::HTTP_ACCEPT_FEATURES );
type->Value("Accept-Language", XE::HttpHeaderFieldType::HTTP_ACCEPT_LANGUAGE );
type->Value("Accept-Patch", XE::HttpHeaderFieldType::HTTP_ACCEPT_PATCH );
type->Value("Accept-Post", XE::HttpHeaderFieldType::HTTP_ACCEPT_POST );
type->Value("Accept-Ranges", XE::HttpHeaderFieldType::HTTP_ACCEPT_RANGES );
type->Value("Access-Control", XE::HttpHeaderFieldType::HTTP_ACCESS_CONTROL );
type->Value("Access-Control-Allow-Credentials", XE::HttpHeaderFieldType::HTTP_ACCESS_CONTROL_ALLOW_CREDENTIALS );
type->Value("Access-Control-Allow-Headers", XE::HttpHeaderFieldType::HTTP_ACCESS_CONTROL_ALLOW_HEADERS );
type->Value("Access-Control-Allow-Methods", XE::HttpHeaderFieldType::HTTP_ACCESS_CONTROL_ALLOW_METHODS );
type->Value("Access-Control-Allow-Origin", XE::HttpHeaderFieldType::HTTP_ACCESS_CONTROL_ALLOW_ORIGIN );
type->Value("Access-Control-Expose-Headers", XE::HttpHeaderFieldType::HTTP_ACCESS_CONTROL_EXPOSE_HEADERS );
type->Value("Access-Control-Max-Age", XE::HttpHeaderFieldType::HTTP_ACCESS_CONTROL_MAX_AGE );
type->Value("Access-Control-Request-Headers", XE::HttpHeaderFieldType::HTTP_ACCESS_CONTROL_REQUEST_HEADERS );
type->Value("Access-Control-Request-Method", XE::HttpHeaderFieldType::HTTP_ACCESS_CONTROL_REQUEST_METHOD );
type->Value("Age", XE::HttpHeaderFieldType::HTTP_AGE );
type->Value("Allow", XE::HttpHeaderFieldType::HTTP_ALLOW );
type->Value("ALPN", XE::HttpHeaderFieldType::HTTP_ALPN );
type->Value("Also-Control", XE::HttpHeaderFieldType::HTTP_ALSO_CONTROL );
type->Value("Alt-Svc", XE::HttpHeaderFieldType::HTTP_ALT_SVC );
type->Value("Alt-Used", XE::HttpHeaderFieldType::HTTP_ALT_USED );
type->Value("Alternate-Recipient", XE::HttpHeaderFieldType::HTTP_ALTERNATE_RECIPIENT );
type->Value("Alternates", XE::HttpHeaderFieldType::HTTP_ALTERNATES );
type->Value("Apparently-To", XE::HttpHeaderFieldType::HTTP_APPARENTLY_TO );
type->Value("Apply-To-Redirect-Ref", XE::HttpHeaderFieldType::HTTP_APPLY_TO_REDIRECT_REF );
type->Value("Approved", XE::HttpHeaderFieldType::HTTP_APPROVED );
type->Value("Archive", XE::HttpHeaderFieldType::HTTP_ARCHIVE );
type->Value("Archived-At", XE::HttpHeaderFieldType::HTTP_ARCHIVED_AT );
type->Value("Article-Names", XE::HttpHeaderFieldType::HTTP_ARTICLE_NAMES );
type->Value("Article-Updates", XE::HttpHeaderFieldType::HTTP_ARTICLE_UPDATES );
type->Value("Authentication-Control", XE::HttpHeaderFieldType::HTTP_AUTHENTICATION_CONTROL );
type->Value("Authentication-Info", XE::HttpHeaderFieldType::HTTP_AUTHENTICATION_INFO );
type->Value("Authentication-Results", XE::HttpHeaderFieldType::HTTP_AUTHENTICATION_RESULTS );
type->Value("Authorization", XE::HttpHeaderFieldType::HTTP_AUTHORIZATION );
type->Value("Auto-Submitted", XE::HttpHeaderFieldType::HTTP_AUTO_SUBMITTED );
type->Value("Autoforwarded", XE::HttpHeaderFieldType::HTTP_AUTOFORWARDED );
type->Value("Autosubmitted", XE::HttpHeaderFieldType::HTTP_AUTOSUBMITTED );
type->Value("Base", XE::HttpHeaderFieldType::HTTP_BASE );
type->Value("Bcc", XE::HttpHeaderFieldType::HTTP_BCC );
type->Value("Body", XE::HttpHeaderFieldType::HTTP_BODY );
type->Value("C-Ext", XE::HttpHeaderFieldType::HTTP_C_EXT );
type->Value("C-Man", XE::HttpHeaderFieldType::HTTP_C_MAN );
type->Value("C-Opt", XE::HttpHeaderFieldType::HTTP_C_OPT );
type->Value("C-PEP", XE::HttpHeaderFieldType::HTTP_C_PEP );
type->Value("C-PEP-Info", XE::HttpHeaderFieldType::HTTP_C_PEP_INFO );
type->Value("Cache-Control", XE::HttpHeaderFieldType::HTTP_CACHE_CONTROL );
type->Value("CalDAV-Timezones", XE::HttpHeaderFieldType::HTTP_CALDAV_TIMEZONES );
type->Value("Cancel-Key", XE::HttpHeaderFieldType::HTTP_CANCEL_KEY );
type->Value("Cancel-Lock", XE::HttpHeaderFieldType::HTTP_CANCEL_LOCK );
type->Value("Cc", XE::HttpHeaderFieldType::HTTP_CC );
type->Value("Close", XE::HttpHeaderFieldType::HTTP_CLOSE );
type->Value("Comments", XE::HttpHeaderFieldType::HTTP_COMMENTS );
type->Value("Compliance", XE::HttpHeaderFieldType::HTTP_COMPLIANCE );
type->Value("Connection", XE::HttpHeaderFieldType::HTTP_CONNECTION );
type->Value("Content-Alternative", XE::HttpHeaderFieldType::HTTP_CONTENT_ALTERNATIVE );
type->Value("Content-Base", XE::HttpHeaderFieldType::HTTP_CONTENT_BASE );
type->Value("Content-Description", XE::HttpHeaderFieldType::HTTP_CONTENT_DESCRIPTION );
type->Value("Content-Disposition", XE::HttpHeaderFieldType::HTTP_CONTENT_DISPOSITION );
type->Value("Content-Duration", XE::HttpHeaderFieldType::HTTP_CONTENT_DURATION );
type->Value("Content-Encoding", XE::HttpHeaderFieldType::HTTP_CONTENT_ENCODING );
type->Value("Content-features", XE::HttpHeaderFieldType::HTTP_CONTENT_FEATURES );
type->Value("Content-ID", XE::HttpHeaderFieldType::HTTP_CONTENT_ID );
type->Value("Content-Identifier", XE::HttpHeaderFieldType::HTTP_CONTENT_IDENTIFIER );
type->Value("Content-Language", XE::HttpHeaderFieldType::HTTP_CONTENT_LANGUAGE );
type->Value("Content-Length", XE::HttpHeaderFieldType::HTTP_CONTENT_LENGTH );
type->Value("Content-Location", XE::HttpHeaderFieldType::HTTP_CONTENT_LOCATION );
type->Value("Content-MD5", XE::HttpHeaderFieldType::HTTP_CONTENT_MD5 );
type->Value("Content-Range", XE::HttpHeaderFieldType::HTTP_CONTENT_RANGE );
type->Value("Content-Return", XE::HttpHeaderFieldType::HTTP_CONTENT_RETURN );
type->Value("Content-Script-Type", XE::HttpHeaderFieldType::HTTP_CONTENT_SCRIPT_TYPE );
type->Value("Content-Style-Type", XE::HttpHeaderFieldType::HTTP_CONTENT_STYLE_TYPE );
type->Value("Content-Transfer-Encoding", XE::HttpHeaderFieldType::HTTP_CONTENT_TRANSFER_ENCODING );
type->Value("Content-Type", XE::HttpHeaderFieldType::HTTP_CONTENT_TYPE );
type->Value("Content-Version", XE::HttpHeaderFieldType::HTTP_CONTENT_VERSION );
type->Value("Control", XE::HttpHeaderFieldType::HTTP_CONTROL );
type->Value("Conversion", XE::HttpHeaderFieldType::HTTP_CONVERSION );
type->Value("Conversion-With-Loss", XE::HttpHeaderFieldType::HTTP_CONVERSION_WITH_LOSS );
type->Value("Cookie", XE::HttpHeaderFieldType::HTTP_COOKIE );
type->Value("Cookie2", XE::HttpHeaderFieldType::HTTP_COOKIE2 );
type->Value("Cost", XE::HttpHeaderFieldType::HTTP_COST );
type->Value("DASL", XE::HttpHeaderFieldType::HTTP_DASL );
type->Value("Date", XE::HttpHeaderFieldType::HTTP_DATE );
type->Value("Date-Received", XE::HttpHeaderFieldType::HTTP_DATE_RECEIVED );
type->Value("DAV", XE::HttpHeaderFieldType::HTTP_DAV );
type->Value("Default-Style", XE::HttpHeaderFieldType::HTTP_DEFAULT_STYLE );
type->Value("Deferred-Delivery", XE::HttpHeaderFieldType::HTTP_DEFERRED_DELIVERY );
type->Value("Delivery-Date", XE::HttpHeaderFieldType::HTTP_DELIVERY_DATE );
type->Value("Delta-Base", XE::HttpHeaderFieldType::HTTP_DELTA_BASE );
type->Value("Depth", XE::HttpHeaderFieldType::HTTP_DEPTH );
type->Value("Derived-From", XE::HttpHeaderFieldType::HTTP_DERIVED_FROM );
type->Value("Destination", XE::HttpHeaderFieldType::HTTP_DESTINATION );
type->Value("Differential-ID", XE::HttpHeaderFieldType::HTTP_DIFFERENTIAL_ID );
type->Value("Digest", XE::HttpHeaderFieldType::HTTP_DIGEST );
type->Value("Discarded-X400-IPMS-Extensions", XE::HttpHeaderFieldType::HTTP_DISCARDED_X400_IPMS_EXTENSIONS );
type->Value("Discarded-X400-MTS-Extensions", XE::HttpHeaderFieldType::HTTP_DISCARDED_X400_MTS_EXTENSIONS );
type->Value("Disclose-Recipients", XE::HttpHeaderFieldType::HTTP_DISCLOSE_RECIPIENTS );
type->Value("Disposition-Notification-Options", XE::HttpHeaderFieldType::HTTP_DISPOSITION_NOTIFICATION_OPTIONS );
type->Value("Disposition-Notification-To", XE::HttpHeaderFieldType::HTTP_DISPOSITION_NOTIFICATION_TO );
type->Value("Distribution", XE::HttpHeaderFieldType::HTTP_DISTRIBUTION );
type->Value("DKIM-Signature", XE::HttpHeaderFieldType::HTTP_DKIM_SIGNATURE );
type->Value("DL-Expansion-History", XE::HttpHeaderFieldType::HTTP_DL_EXPANSION_HISTORY );
type->Value("Downgraded-Bcc", XE::HttpHeaderFieldType::HTTP_DOWNGRADED_BCC );
type->Value("Downgraded-Cc", XE::HttpHeaderFieldType::HTTP_DOWNGRADED_CC );
type->Value("Downgraded-Disposition-Notification-To", XE::HttpHeaderFieldType::HTTP_DOWNGRADED_DISPOSITION_NOTIFICATION_TO );
type->Value("Downgraded-Final-Recipient", XE::HttpHeaderFieldType::HTTP_DOWNGRADED_FINAL_RECIPIENT );
type->Value("Downgraded-From", XE::HttpHeaderFieldType::HTTP_DOWNGRADED_FROM );
type->Value("Downgraded-In-Reply-To", XE::HttpHeaderFieldType::HTTP_DOWNGRADED_IN_REPLY_TO );
type->Value("Downgraded-Mail-From", XE::HttpHeaderFieldType::HTTP_DOWNGRADED_MAIL_FROM );
type->Value("Downgraded-Message-Id", XE::HttpHeaderFieldType::HTTP_DOWNGRADED_MESSAGE_ID );
type->Value("Downgraded-Original-Recipient", XE::HttpHeaderFieldType::HTTP_DOWNGRADED_ORIGINAL_RECIPIENT );
type->Value("Downgraded-Rcpt-To", XE::HttpHeaderFieldType::HTTP_DOWNGRADED_RCPT_TO );
type->Value("Downgraded-References", XE::HttpHeaderFieldType::HTTP_DOWNGRADED_REFERENCES );
type->Value("Downgraded-Reply-To", XE::HttpHeaderFieldType::HTTP_DOWNGRADED_REPLY_TO );
type->Value("Downgraded-Resent-Bcc", XE::HttpHeaderFieldType::HTTP_DOWNGRADED_RESENT_BCC );
type->Value("Downgraded-Resent-Cc", XE::HttpHeaderFieldType::HTTP_DOWNGRADED_RESENT_CC );
type->Value("Downgraded-Resent-From", XE::HttpHeaderFieldType::HTTP_DOWNGRADED_RESENT_FROM );
type->Value("Downgraded-Resent-Reply-To", XE::HttpHeaderFieldType::HTTP_DOWNGRADED_RESENT_REPLY_TO );
type->Value("Downgraded-Resent-Sender", XE::HttpHeaderFieldType::HTTP_DOWNGRADED_RESENT_SENDER );
type->Value("Downgraded-Resent-To", XE::HttpHeaderFieldType::HTTP_DOWNGRADED_RESENT_TO );
type->Value("Downgraded-Return-Path", XE::HttpHeaderFieldType::HTTP_DOWNGRADED_RETURN_PATH );
type->Value("Downgraded-Sender", XE::HttpHeaderFieldType::HTTP_DOWNGRADED_SENDER );
type->Value("Downgraded-To", XE::HttpHeaderFieldType::HTTP_DOWNGRADED_TO );
type->Value("EDIINT-Features", XE::HttpHeaderFieldType::HTTP_EDIINT_FEATURES );
type->Value("Eesst-Version", XE::HttpHeaderFieldType::HTTP_EESST_VERSION );
type->Value("Encoding", XE::HttpHeaderFieldType::HTTP_ENCODING );
type->Value("Encrypted", XE::HttpHeaderFieldType::HTTP_ENCRYPTED );
type->Value("Errors-To", XE::HttpHeaderFieldType::HTTP_ERRORS_TO );
type->Value("ETag", XE::HttpHeaderFieldType::HTTP_ETAG );
type->Value("Expect", XE::HttpHeaderFieldType::HTTP_EXPECT );
type->Value("Expires", XE::HttpHeaderFieldType::HTTP_EXPIRES );
type->Value("Expiry-Date", XE::HttpHeaderFieldType::HTTP_EXPIRY_DATE );
type->Value("Ext", XE::HttpHeaderFieldType::HTTP_EXT );
type->Value("Followup-To", XE::HttpHeaderFieldType::HTTP_FOLLOWUP_TO );
type->Value("Forwarded", XE::HttpHeaderFieldType::HTTP_FORWARDED );
type->Value("From", XE::HttpHeaderFieldType::HTTP_FROM );
type->Value("Generate-Delivery-Report", XE::HttpHeaderFieldType::HTTP_GENERATE_DELIVERY_REPORT );
type->Value("GetProfile", XE::HttpHeaderFieldType::HTTP_GETPROFILE );
type->Value("Hobareg", XE::HttpHeaderFieldType::HTTP_HOBAREG );
type->Value("Host", XE::HttpHeaderFieldType::HTTP_HOST );
type->Value("HTTP2-Settings", XE::HttpHeaderFieldType::HTTP_HTTP2_SETTINGS );
type->Value("If", XE::HttpHeaderFieldType::HTTP_IF_ );
type->Value("If-Match", XE::HttpHeaderFieldType::HTTP_IF_MATCH );
type->Value("If-Modified-Since", XE::HttpHeaderFieldType::HTTP_IF_MODIFIED_SINCE );
type->Value("If-None-Match", XE::HttpHeaderFieldType::HTTP_IF_NONE_MATCH );
type->Value("If-Range", XE::HttpHeaderFieldType::HTTP_IF_RANGE );
type->Value("If-Schedule-Tag-Match", XE::HttpHeaderFieldType::HTTP_IF_SCHEDULE_TAG_MATCH );
type->Value("If-Unmodified-Since", XE::HttpHeaderFieldType::HTTP_IF_UNMODIFIED_SINCE );
type->Value("IM", XE::HttpHeaderFieldType::HTTP_IM );
type->Value("Importance", XE::HttpHeaderFieldType::HTTP_IMPORTANCE );
type->Value("In-Reply-To", XE::HttpHeaderFieldType::HTTP_IN_REPLY_TO );
type->Value("Incomplete-Copy", XE::HttpHeaderFieldType::HTTP_INCOMPLETE_COPY );
type->Value("Injection-Date", XE::HttpHeaderFieldType::HTTP_INJECTION_DATE );
type->Value("Injection-Info", XE::HttpHeaderFieldType::HTTP_INJECTION_INFO );
type->Value("Jabber-ID", XE::HttpHeaderFieldType::HTTP_JABBER_ID );
type->Value("Keep-Alive", XE::HttpHeaderFieldType::HTTP_KEEP_ALIVE );
type->Value("Keywords", XE::HttpHeaderFieldType::HTTP_KEYWORDS );
type->Value("Label", XE::HttpHeaderFieldType::HTTP_LABEL );
type->Value("Language", XE::HttpHeaderFieldType::HTTP_LANGUAGE );
type->Value("Last-Modified", XE::HttpHeaderFieldType::HTTP_LAST_MODIFIED );
type->Value("Latest-Delivery-Time", XE::HttpHeaderFieldType::HTTP_LATEST_DELIVERY_TIME );
type->Value("Lines", XE::HttpHeaderFieldType::HTTP_LINES );
type->Value("Link", XE::HttpHeaderFieldType::HTTP_LINK );
type->Value("List-Archive", XE::HttpHeaderFieldType::HTTP_LIST_ARCHIVE );
type->Value("List-Help", XE::HttpHeaderFieldType::HTTP_LIST_HELP );
type->Value("List-ID", XE::HttpHeaderFieldType::HTTP_LIST_ID );
type->Value("List-Owner", XE::HttpHeaderFieldType::HTTP_LIST_OWNER );
type->Value("List-Post", XE::HttpHeaderFieldType::HTTP_LIST_POST );
type->Value("List-Subscribe", XE::HttpHeaderFieldType::HTTP_LIST_SUBSCRIBE );
type->Value("List-Unsubscribe", XE::HttpHeaderFieldType::HTTP_LIST_UNSUBSCRIBE );
type->Value("List-Unsubscribe-Post", XE::HttpHeaderFieldType::HTTP_LIST_UNSUBSCRIBE_POST );
type->Value("Location", XE::HttpHeaderFieldType::HTTP_LOCATION );
type->Value("Lock-Token", XE::HttpHeaderFieldType::HTTP_LOCK_TOKEN );
type->Value("Man", XE::HttpHeaderFieldType::HTTP_MAN );
type->Value("Max-Forwards", XE::HttpHeaderFieldType::HTTP_MAX_FORWARDS );
type->Value("Memento-Datetime", XE::HttpHeaderFieldType::HTTP_MEMENTO_DATETIME );
type->Value("Message-Context", XE::HttpHeaderFieldType::HTTP_MESSAGE_CONTEXT );
type->Value("Message-ID", XE::HttpHeaderFieldType::HTTP_MESSAGE_ID );
type->Value("Message-Type", XE::HttpHeaderFieldType::HTTP_MESSAGE_TYPE );
type->Value("Meter", XE::HttpHeaderFieldType::HTTP_METER );
type->Value("Method-Check", XE::HttpHeaderFieldType::HTTP_METHOD_CHECK );
type->Value("Method-Check-Expires", XE::HttpHeaderFieldType::HTTP_METHOD_CHECK_EXPIRES );
type->Value("MIME-Version", XE::HttpHeaderFieldType::HTTP_MIME_VERSION );
type->Value("MMHS-Acp127-Message-Identifier", XE::HttpHeaderFieldType::HTTP_MMHS_ACP127_MESSAGE_IDENTIFIER );
type->Value("MMHS-Authorizing-Users", XE::HttpHeaderFieldType::HTTP_MMHS_AUTHORIZING_USERS );
type->Value("MMHS-Codress-Message-Indicator", XE::HttpHeaderFieldType::HTTP_MMHS_CODRESS_MESSAGE_INDICATOR );
type->Value("MMHS-Copy-Precedence", XE::HttpHeaderFieldType::HTTP_MMHS_COPY_PRECEDENCE );
type->Value("MMHS-Exempted-Address", XE::HttpHeaderFieldType::HTTP_MMHS_EXEMPTED_ADDRESS );
type->Value("MMHS-Extended-Authorisation-Info", XE::HttpHeaderFieldType::HTTP_MMHS_EXTENDED_AUTHORISATION_INFO );
type->Value("MMHS-Handling-Instructions", XE::HttpHeaderFieldType::HTTP_MMHS_HANDLING_INSTRUCTIONS );
type->Value("MMHS-Message-Instructions", XE::HttpHeaderFieldType::HTTP_MMHS_MESSAGE_INSTRUCTIONS );
type->Value("MMHS-Message-Type", XE::HttpHeaderFieldType::HTTP_MMHS_MESSAGE_TYPE );
type->Value("MMHS-Originator-PLAD", XE::HttpHeaderFieldType::HTTP_MMHS_ORIGINATOR_PLAD );
type->Value("MMHS-Originator-Reference", XE::HttpHeaderFieldType::HTTP_MMHS_ORIGINATOR_REFERENCE );
type->Value("MMHS-Other-Recipients-Indicator-CC", XE::HttpHeaderFieldType::HTTP_MMHS_OTHER_RECIPIENTS_INDICATOR_CC );
type->Value("MMHS-Other-Recipients-Indicator-To", XE::HttpHeaderFieldType::HTTP_MMHS_OTHER_RECIPIENTS_INDICATOR_TO );
type->Value("MMHS-Primary-Precedence", XE::HttpHeaderFieldType::HTTP_MMHS_PRIMARY_PRECEDENCE );
type->Value("MMHS-Subject-Indicator-Codes", XE::HttpHeaderFieldType::HTTP_MMHS_SUBJECT_INDICATOR_CODES );
type->Value("MT-Priority", XE::HttpHeaderFieldType::HTTP_MT_PRIORITY );
type->Value("Negotiate", XE::HttpHeaderFieldType::HTTP_NEGOTIATE );
type->Value("Newsgroups", XE::HttpHeaderFieldType::HTTP_NEWSGROUPS );
type->Value("NNTP-Posting-Date", XE::HttpHeaderFieldType::HTTP_NNTP_POSTING_DATE );
type->Value("NNTP-Posting-Host", XE::HttpHeaderFieldType::HTTP_NNTP_POSTING_HOST );
type->Value("Non-Compliance", XE::HttpHeaderFieldType::HTTP_NON_COMPLIANCE );
type->Value("Obsoletes", XE::HttpHeaderFieldType::HTTP_OBSOLETES );
type->Value("Opt", XE::HttpHeaderFieldType::HTTP_OPT );
type->Value("Optional", XE::HttpHeaderFieldType::HTTP_OPTIONAL );
type->Value("Optional-WWW-Authenticate", XE::HttpHeaderFieldType::HTTP_OPTIONAL_WWW_AUTHENTICATE );
type->Value("Ordering-Type", XE::HttpHeaderFieldType::HTTP_ORDERING_TYPE );
type->Value("Organization", XE::HttpHeaderFieldType::HTTP_ORGANIZATION );
type->Value("Origin", XE::HttpHeaderFieldType::HTTP_ORIGIN );
type->Value("Original-Encoded-Information-Types", XE::HttpHeaderFieldType::HTTP_ORIGINAL_ENCODED_INFORMATION_TYPES );
type->Value("Original-From", XE::HttpHeaderFieldType::HTTP_ORIGINAL_FROM );
type->Value("Original-Message-ID", XE::HttpHeaderFieldType::HTTP_ORIGINAL_MESSAGE_ID );
type->Value("Original-Recipient", XE::HttpHeaderFieldType::HTTP_ORIGINAL_RECIPIENT );
type->Value("Original-Sender", XE::HttpHeaderFieldType::HTTP_ORIGINAL_SENDER );
type->Value("Original-Subject", XE::HttpHeaderFieldType::HTTP_ORIGINAL_SUBJECT );
type->Value("Originator-Return-Address", XE::HttpHeaderFieldType::HTTP_ORIGINATOR_RETURN_ADDRESS );
type->Value("Overwrite", XE::HttpHeaderFieldType::HTTP_OVERWRITE );
type->Value("P3P", XE::HttpHeaderFieldType::HTTP_P3P );
type->Value("Path", XE::HttpHeaderFieldType::HTTP_PATH );
type->Value("PEP", XE::HttpHeaderFieldType::HTTP_PEP );
type->Value("Pep-Info", XE::HttpHeaderFieldType::HTTP_PEP_INFO );
type->Value("PICS-Label", XE::HttpHeaderFieldType::HTTP_PICS_LABEL );
type->Value("Position", XE::HttpHeaderFieldType::HTTP_POSITION );
type->Value("Posting-Version", XE::HttpHeaderFieldType::HTTP_POSTING_VERSION );
type->Value("Pragma", XE::HttpHeaderFieldType::HTTP_PRAGMA );
type->Value("Prefer", XE::HttpHeaderFieldType::HTTP_PREFER );
type->Value("Preference-Applied", XE::HttpHeaderFieldType::HTTP_PREFERENCE_APPLIED );
type->Value("Prevent-NonDelivery-Report", XE::HttpHeaderFieldType::HTTP_PREVENT_NONDELIVERY_REPORT );
type->Value("Priority", XE::HttpHeaderFieldType::HTTP_PRIORITY );
type->Value("Privicon", XE::HttpHeaderFieldType::HTTP_PRIVICON );
type->Value("ProfileObject", XE::HttpHeaderFieldType::HTTP_PROFILEOBJECT );
type->Value("Protocol", XE::HttpHeaderFieldType::HTTP_PROTOCOL );
type->Value("Protocol-Info", XE::HttpHeaderFieldType::HTTP_PROTOCOL_INFO );
type->Value("Protocol-Query", XE::HttpHeaderFieldType::HTTP_PROTOCOL_QUERY );
type->Value("Protocol-Request", XE::HttpHeaderFieldType::HTTP_PROTOCOL_REQUEST );
type->Value("Proxy-Authenticate", XE::HttpHeaderFieldType::HTTP_PROXY_AUTHENTICATE );
type->Value("Proxy-Authentication-Info", XE::HttpHeaderFieldType::HTTP_PROXY_AUTHENTICATION_INFO );
type->Value("Proxy-Authorization", XE::HttpHeaderFieldType::HTTP_PROXY_AUTHENTICATE );
type->Value("Proxy-Connection", XE::HttpHeaderFieldType::HTTP_PROXY_CONNECTION );
type->Value("Proxy-Features", XE::HttpHeaderFieldType::HTTP_PROXY_FEATURES );
type->Value("Proxy-Instruction", XE::HttpHeaderFieldType::HTTP_PROXY_INSTRUCTION );
type->Value("Public", XE::HttpHeaderFieldType::HTTP_PUBLIC_ );
type->Value("Public-Key-Pins", XE::HttpHeaderFieldType::HTTP_PUBLIC_KEY_PINS );
type->Value("Public-Key-Pins-Report-Only", XE::HttpHeaderFieldType::HTTP_PUBLIC_KEY_PINS_REPORT_ONLY );
type->Value("Range", XE::HttpHeaderFieldType::HTTP_RANGE );
type->Value("Received", XE::HttpHeaderFieldType::HTTP_RECEIVED );
type->Value("Received-SPF", XE::HttpHeaderFieldType::HTTP_RECEIVED_SPF );
type->Value("Redirect-Ref", XE::HttpHeaderFieldType::HTTP_REDIRECT_REF );
type->Value("References", XE::HttpHeaderFieldType::HTTP_REFERENCES );
type->Value("Referer", XE::HttpHeaderFieldType::HTTP_REFERER );
type->Value("Referer-Root", XE::HttpHeaderFieldType::HTTP_REFERER_ROOT );
type->Value("Relay-Version", XE::HttpHeaderFieldType::HTTP_RELAY_VERSION );
type->Value("Reply-By", XE::HttpHeaderFieldType::HTTP_REPLY_BY );
type->Value("Reply-To", XE::HttpHeaderFieldType::HTTP_REPLY_TO );
type->Value("Require-Recipient-Valid-Since", XE::HttpHeaderFieldType::HTTP_REQUIRE_RECIPIENT_VALID_SINCE );
type->Value("Resent-Bcc", XE::HttpHeaderFieldType::HTTP_RESENT_BCC );
type->Value("Resent-Cc", XE::HttpHeaderFieldType::HTTP_RESENT_CC );
type->Value("Resent-Date", XE::HttpHeaderFieldType::HTTP_RESENT_DATE );
type->Value("Resent-From", XE::HttpHeaderFieldType::HTTP_RESENT_FROM );
type->Value("Resent-Message-ID", XE::HttpHeaderFieldType::HTTP_RESENT_MESSAGE_ID );
type->Value("Resent-Reply-To", XE::HttpHeaderFieldType::HTTP_RESENT_REPLY_TO );
type->Value("Resent-Sender", XE::HttpHeaderFieldType::HTTP_RESENT_SENDER );
type->Value("Resent-To", XE::HttpHeaderFieldType::HTTP_RESENT_TO );
type->Value("Resolution-Hint", XE::HttpHeaderFieldType::HTTP_RESOLUTION_HINT );
type->Value("Resolver-Location", XE::HttpHeaderFieldType::HTTP_RESOLVER_LOCATION );
type->Value("Retry-After", XE::HttpHeaderFieldType::HTTP_RETRY_AFTER );
type->Value("Return-Path", XE::HttpHeaderFieldType::HTTP_RETURN_PATH );
type->Value("Safe", XE::HttpHeaderFieldType::HTTP_SAFE );
type->Value("Schedule-Reply", XE::HttpHeaderFieldType::HTTP_SCHEDULE_REPLY );
type->Value("Schedule-Tag", XE::HttpHeaderFieldType::HTTP_SCHEDULE_TAG );
type->Value("Sec-Fetch-Dest", XE::HttpHeaderFieldType::HTTP_SEC_FETCH_DEST );
type->Value("Sec-Fetch-Mode", XE::HttpHeaderFieldType::HTTP_SEC_FETCH_MODE );
type->Value("Sec-Fetch-Site", XE::HttpHeaderFieldType::HTTP_SEC_FETCH_SITE );
type->Value("Sec-Fetch-User", XE::HttpHeaderFieldType::HTTP_SEC_FETCH_USER );
type->Value("Sec-WebSocket-Accept", XE::HttpHeaderFieldType::HTTP_SEC_WEBSOCKET_ACCEPT );
type->Value("Sec-WebSocket-Extensions", XE::HttpHeaderFieldType::HTTP_SEC_WEBSOCKET_EXTENSIONS );
type->Value("Sec-WebSocket-Key", XE::HttpHeaderFieldType::HTTP_SEC_WEBSOCKET_KEY );
type->Value("Sec-WebSocket-Protocol", XE::HttpHeaderFieldType::HTTP_SEC_WEBSOCKET_PROTOCOL );
type->Value("Sec-WebSocket-Version", XE::HttpHeaderFieldType::HTTP_SEC_WEBSOCKET_VERSION );
type->Value("Security-Scheme", XE::HttpHeaderFieldType::HTTP_SECURITY_SCHEME );
type->Value("See-Also", XE::HttpHeaderFieldType::HTTP_SEE_ALSO );
type->Value("Sender", XE::HttpHeaderFieldType::HTTP_SENDER );
type->Value("Sensitivity", XE::HttpHeaderFieldType::HTTP_SENSITIVITY );
type->Value("Server", XE::HttpHeaderFieldType::HTTP_SERVER );
type->Value("Set-Cookie", XE::HttpHeaderFieldType::HTTP_SET_COOKIE );
type->Value("Set-Cookie2", XE::HttpHeaderFieldType::HTTP_SET_COOKIE2 );
type->Value("SetProfile", XE::HttpHeaderFieldType::HTTP_SETPROFILE );
type->Value("SIO-Label", XE::HttpHeaderFieldType::HTTP_SIO_LABEL );
type->Value("SIO-Label-History", XE::HttpHeaderFieldType::HTTP_SIO_LABEL_HISTORY );
type->Value("SLUG", XE::HttpHeaderFieldType::HTTP_SLUG );
type->Value("SoapAction", XE::HttpHeaderFieldType::HTTP_SOAPACTION );
type->Value("Solicitation", XE::HttpHeaderFieldType::HTTP_SOLICITATION );
type->Value("Status-URI", XE::HttpHeaderFieldType::HTTP_STATUS_URI );
type->Value("Strict-Transport-Security", XE::HttpHeaderFieldType::HTTP_STRICT_TRANSPORT_SECURITY );
type->Value("Subject", XE::HttpHeaderFieldType::HTTP_SUBJECT );
type->Value("SubOK", XE::HttpHeaderFieldType::HTTP_SUBOK );
type->Value("Subst", XE::HttpHeaderFieldType::HTTP_SUBST );
type->Value("Summary", XE::HttpHeaderFieldType::HTTP_SUMMARY );
type->Value("Supersedes", XE::HttpHeaderFieldType::HTTP_SUPERSEDES );
type->Value("Surrogate-Capability", XE::HttpHeaderFieldType::HTTP_SURROGATE_CAPABILITY );
type->Value("Surrogate-Control", XE::HttpHeaderFieldType::HTTP_SURROGATE_CONTROL );
type->Value("TCN", XE::HttpHeaderFieldType::HTTP_TCN );
type->Value("TE", XE::HttpHeaderFieldType::HTTP_TE );
type->Value("Timeout", XE::HttpHeaderFieldType::HTTP_TIMEOUT );
type->Value("Title", XE::HttpHeaderFieldType::HTTP_TITLE );
type->Value("To", XE::HttpHeaderFieldType::HTTP_TO );
type->Value("Topic", XE::HttpHeaderFieldType::HTTP_TOPIC );
type->Value("Trailer", XE::HttpHeaderFieldType::HTTP_TRAILER );
type->Value("Transfer-Encoding", XE::HttpHeaderFieldType::HTTP_TRANSFER_ENCODING );
type->Value("TTL", XE::HttpHeaderFieldType::HTTP_TTL );
type->Value("UA-Color", XE::HttpHeaderFieldType::HTTP_UA_COLOR );
type->Value("UA-Media", XE::HttpHeaderFieldType::HTTP_UA_MEDIA );
type->Value("UA-Pixels", XE::HttpHeaderFieldType::HTTP_UA_PIXELS );
type->Value("UA-Resolution", XE::HttpHeaderFieldType::HTTP_UA_RESOLUTION );
type->Value("UA-Windowpixels", XE::HttpHeaderFieldType::HTTP_UA_WINDOWPIXELS );
type->Value("Upgrade", XE::HttpHeaderFieldType::HTTP_UPGRADE );
type->Value("Urgency", XE::HttpHeaderFieldType::HTTP_URGENCY );
type->Value("URI", XE::HttpHeaderFieldType::HTTP_URI );
type->Value("User-Agent", XE::HttpHeaderFieldType::HTTP_USER_AGENT );
type->Value("Variant-Vary", XE::HttpHeaderFieldType::HTTP_VARIANT_VARY );
type->Value("Vary", XE::HttpHeaderFieldType::HTTP_VARY );
type->Value("VBR-Info", XE::HttpHeaderFieldType::HTTP_VBR_INFO );
type->Value("Version", XE::HttpHeaderFieldType::HTTP_VERSION );
type->Value("Via", XE::HttpHeaderFieldType::HTTP_VIA );
type->Value("Want-Digest", XE::HttpHeaderFieldType::HTTP_WANT_DIGEST );
type->Value("Warning", XE::HttpHeaderFieldType::HTTP_WARNING );
type->Value("WWW-Authenticate", XE::HttpHeaderFieldType::HTTP_WWW_AUTHENTICATE );
type->Value("X-Archived-At", XE::HttpHeaderFieldType::HTTP_X_ARCHIVED_AT );
type->Value("X-Device-Accept", XE::HttpHeaderFieldType::HTTP_X_DEVICE_ACCEPT );
type->Value("X-Device-Accept-Charset", XE::HttpHeaderFieldType::HTTP_X_DEVICE_ACCEPT_CHARSET );
type->Value("X-Device-Accept-Encoding", XE::HttpHeaderFieldType::HTTP_X_DEVICE_ACCEPT_ENCODING );
type->Value("X-Device-Accept-Language", XE::HttpHeaderFieldType::HTTP_X_DEVICE_ACCEPT_LANGUAGE );
type->Value("X-Device-User-Agent", XE::HttpHeaderFieldType::HTTP_X_DEVICE_USER_AGENT );
type->Value("X-Frame-Options", XE::HttpHeaderFieldType::HTTP_X_FRAME_OPTIONS );
type->Value("X-Mittente", XE::HttpHeaderFieldType::HTTP_X_MITTENTE );
type->Value("X-PGP-Sig", XE::HttpHeaderFieldType::HTTP_X_PGP_SIG );
type->Value("X-Ricevuta", XE::HttpHeaderFieldType::HTTP_X_RICEVUTA );
type->Value("X-Riferimento-Message-ID", XE::HttpHeaderFieldType::HTTP_X_RIFERIMENTO_MESSAGE_ID );
type->Value("X-TipoRicevuta", XE::HttpHeaderFieldType::HTTP_X_TIPORICEVUTA );
type->Value("X-Trasporto", XE::HttpHeaderFieldType::HTTP_X_TRASPORTO );
type->Value("X-VerificaSicurezza", XE::HttpHeaderFieldType::HTTP_X_VERIFICASICUREZZA );
type->Value("X400-Content-Identifier", XE::HttpHeaderFieldType::HTTP_X400_CONTENT_IDENTIFIER );
type->Value("X400-Content-Return", XE::HttpHeaderFieldType::HTTP_X400_CONTENT_RETURN );
type->Value("X400-Content-Type", XE::HttpHeaderFieldType::HTTP_X400_CONTENT_TYPE );
type->Value("X400-MTS-Identifier", XE::HttpHeaderFieldType::HTTP_X400_MTS_IDENTIFIER );
type->Value("X400-Originator", XE::HttpHeaderFieldType::HTTP_X400_ORIGINATOR );
type->Value("X400-Received", XE::HttpHeaderFieldType::HTTP_X400_RECEIVED );
type->Value("X400-Recipients", XE::HttpHeaderFieldType::HTTP_X400_RECIPIENTS );
type->Value("X400-Trace", XE::HttpHeaderFieldType::HTTP_X400_TRACE );
type->Value("Xref", XE::HttpHeaderFieldType::HTTP_XREF );
END_META()


using buffers_iterator = asio::buffers_iterator<asio::streambuf::const_buffers_type>;
template< typename T > buffers_iterator & iter( T & data )
{
	return *( buffers_iterator * ) ( data.data() );
}
XE::BufferIterator::reference XE::BufferIterator::operator*() const
{
	return *iter( _Ptr );
}

XE::BufferIterator & XE::BufferIterator::operator++()
{
	++iter( _Ptr );

	return *this;
}

XE::BufferIterator XE::BufferIterator::operator++( int )
{
	auto tmp = *this;

	++iter( _Ptr );

	return tmp;
}

XE::BufferIterator & XE::BufferIterator::operator--()
{
	--iter( _Ptr );

	return *this;
}

XE::BufferIterator XE::BufferIterator::operator--( int )
{
	auto tmp = *this;

	--iter( _Ptr );

	return tmp;
}

XE::BufferIterator & XE::BufferIterator::operator+=( const difference_type _Off )
{
	iter( _Ptr ) += _Off;

	return *this;
}

XE::BufferIterator XE::BufferIterator::operator+( const difference_type _Off ) const
{
	auto tmp = *this;

	tmp += _Off;

	return tmp;
}

XE::BufferIterator & XE::BufferIterator::operator-=( const difference_type _Off )
{
	iter( _Ptr ) -= _Off;

	return *this;
}

XE::BufferIterator XE::BufferIterator::operator-( const difference_type _Off ) const
{
	auto tmp = *this;

	tmp -= _Off;

	return tmp;
}

XE::BufferIterator::difference_type XE::BufferIterator::operator-( const BufferIterator & _Right ) const
{
	return iter( _Ptr ) - iter( _Right._Ptr );
}

bool XE::BufferIterator::operator==( const BufferIterator & _Right ) const
{
	return iter( _Ptr ) == iter( _Right._Ptr );
}

