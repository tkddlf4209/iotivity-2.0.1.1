//******************************************************************
//
// Copyright 2015 Intel Mobile Communications GmbH All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef IOTVT_SRM_UTILITY_H
#define IOTVT_SRM_UTILITY_H

#include "ocstack.h"
#include "experimental/securevirtualresourcetypes.h"
#ifdef __cplusplus
extern "C"
{
#endif
#include <coap/uri.h>
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#if defined (__WITH_TLS__) || defined(__WITH_DTLS__)
#include "mbedtls/error.h"
#endif
typedef struct OicParseQueryIter OicParseQueryIter_t;

/**
 * OicRestQueryIter data structure is used for book-keeping
 * sub-REST query's attribute's and value's, starting location &
 * length between calls to GetNextQuery(). This struct needs
 * to be first initialized with ParseQueryIterInit().
 *
 */
struct OicParseQueryIter
{
    unsigned char * attrPos;    /**< stating location of attribute. */
    size_t attrLen;             /**< length of the attribute. */
    unsigned char * valPos;     /**< starting location of value. */
    size_t valLen;              /**< length of the value. */
    coap_parse_iterator_t pi;   /**< coap struct for tokenizing the query.*/
};

/**
 * Macro to verify success of operation.
 * eg: VERIFY_SUCCESS(TAG, OC_STACK_OK == foo(), ERROR);
 * @note Invoking function must define "exit:" label for goto functionality to work correctly.
 */
#ifndef VERIFY_SUCCESS
#define VERIFY_SUCCESS(tag, op, logLevel) do{ if (!(op)) \
            {OIC_LOG((logLevel), tag, #op " failed!!"); goto exit; } }while(0)
#endif

/**
 * Macro to verify a conditional, if fails, log supplied message and goto exit
 * eg: VERIFY_SUCCESS_OR_LOG_AND_EXIT(TAG, OC_STACK_OK == foo(), ERROR);
 * @note Invoking function must define "exit:" label for goto functionality to work correctly.
 */
#define VERIFY_OR_LOG_AND_EXIT(tag, op, msg, logLevel) do{ if (!(op)) \
            {OIC_LOG((logLevel), tag, msg); goto exit; } }while(0)

/**
 * Macro to verify expression evaluates to bool true.
 * eg: VERIFY_TRUE_OR_EXIT(TAG, OC_STACK_OK == foo(), ERROR);
 * @note Invoking function must define "exit:" label for goto functionality to work correctly.
 */
#ifndef VERIFY_TRUE_OR_EXIT
#define VERIFY_TRUE_OR_EXIT(tag, op, logLevel) do{ if (!(op)) \
            {OIC_LOG_V((logLevel), tag, "%s:" #op "evaluates to false!",__func__); \
            goto exit; } }while(0)
#endif

/**
 * Macro to verify success of operation.
 * eg: VERIFY_SUCCESS_RETURN(TAG, OC_STACK_OK == foo(), ERROR, OC_STACK_ERROR);
 */
#ifndef VERIFY_SUCCESS_RETURN
#define VERIFY_SUCCESS_RETURN(tag, op, logLevel, retValue) do { if (!(op)) \
            {OIC_LOG((logLevel), tag, #op " failed!!"); return retValue;} } while(0)
#endif

/**
 * Macro to verify argument is not equal to NULL.
 * eg: VERIFY_NOT_NULL(TAG, ptrData, ERROR);
 * @note Invoking function must define "exit:" label for goto functionality to work correctly.
 */
#ifndef VERIFY_NOT_NULL
#define VERIFY_NOT_NULL(tag, arg, logLevel) do{ if (NULL == (arg)) \
            { OIC_LOG((logLevel), tag, #arg " is NULL"); goto exit; } }while(0)
#endif

/**
 * Macro to verify argument is not equal to NULL.
 * eg: VERIFY_NOT_NULL_RETURN(TAG, ptrData, ERROR, OC_STACK_ERROR);
 */
#ifndef VERIFY_NOT_NULL_RETURN
#define VERIFY_NOT_NULL_RETURN(tag, arg, logLevel, retValue) do { if (NULL == (arg)) \
            { OIC_LOG((logLevel), tag, #arg " is NULL"); return retValue; } } while(0)
#endif
#if defined (__WITH_TLS__) || defined(__WITH_DTLS__)
/**
 * Macro to log an mbedtls error
 * For mbedtls functions that return 0 as non-error
 * @note Invoker must provide message buffer, and must include "mbedtls/error.h"
 */
#define LOG_MBED_ERROR(tag, ret, buf, bufSize, logLevel) do{ if (0!=(ret)) { \
    mbedtls_strerror((ret), (buf), (bufSize));                               \
    OIC_LOG_V((logLevel), (tag), "mbedtls error:  %s", (buf)); } }while(0)

#define LOG_MBEDTLS_ERROR(ret) do{ \
    if (0!=(ret)) {\
    char *_buf = (char*)OICCalloc(2048, 1);\
    if (_buf)\
    {\
        mbedtls_strerror((ret), _buf, 2048);                           \
        OIC_LOG_V(ERROR, TAG, "mbedtls error:  %s", _buf); \
        OICFree(_buf);\
    }\
    } }while(0)

#define LOG_MBEDTLS_VERIFY_ERROR(flags) do{ \
    if (0!=(flags)) {                       \
    char *_buf = (char*)OICCalloc(2048, 1);  \
    if (_buf)                                \
    {                                       \
        mbedtls_x509_crt_verify_info(_buf, 2048, "", flags); \
        OIC_LOG_V(ERROR, TAG, "mbedtls verify error:  %s", _buf); \
        OICFree(_buf);                       \
    }                                       \
    } }while(0)


#endif //(__WITH_TLS__) || defined(__WITH_DTLS__)

/**
 * This method initializes the @ref OicParseQueryIter_t struct.
 *
 * @param query is the REST query, to be parsed.
 * @param parseIter is the @ref OicParseQueryIter_t struct, to be initialized based on the query.
 */
void ParseQueryIterInit(const unsigned char * query, OicParseQueryIter_t * parseIter);

/**
 * This method fills the @ref OicParseQueryIter_t struct with next REST query's
 * attribute's and value's information.
 *
 * @param parseIter is the @ref OicParseQueryIter_t struct, has next query's attribute's
 *  & value's info.
 *
 * @return reference to the @ref OicParseQueryIter_t if it has parsed query info, else
 * NULL if it has no query to parse.
 */
OicParseQueryIter_t * GetNextQuery(OicParseQueryIter_t * parseIter);

/**
 * Function to getting string of ownership transfer method
 *
 * @param oxmType ownership transfer method
 *
 * @return string value of ownership transfer method
 */
const char* GetOxmString(OicSecOxm_t oxmType);

/**
 * This method converts UUID to canonical format string.
 *
 * @param uuid Device UUID
 * @param strUuid converted UUID in canonical format
 * @return OC_STACK_OK for success.
 *
 * @note Caller needs to invoke OICFree after done using the return pointer
 */
OCStackResult ConvertUuidToStr(const OicUuid_t* uuid, char** strUuid);


/**
 * This method converts string UUID to OicUuid_t.
 *
 * @param strUuid Device UUID in string format
 * @param uuid converted UUID in OicUuid_t format
 * @return OC_STACK_OK for success.
 *
 */
OCStackResult OC_CALL ConvertStrToUuid(const char* strUuid, OicUuid_t* uuid);

/**
 * Is the URI for a Device Configuration Resource as defined
 * by Security Specification.
 *
 * @return true IFF the URI is for a DCR
 */
bool IsDeviceConfigurationResourceUri(const char *uri);

/**
 * Is the URI for a Non0Configuration Resource as defined
 * by Security Specification.
 *
 * @return true IFF the URI is for a NCR
 */
bool IsNonConfigurationResourceUri(const char *uri);

/**
 * Compares two OicUuid_t structs.
 *
 * @return true if the two OicUuid_t structs are equal, else false.
 */
bool UuidCmp(const OicUuid_t *firstId, const OicUuid_t *secondId);

extern const OicUuid_t THE_NIL_UUID;

/**
 * OicUuid_t to Nil UUID {.id={0000000000000000}}
 *
 * @return true if the OicUuid_t is the Nil UUID
 */
bool IsNilUuid(const OicUuid_t *uuid);

#if defined(__WITH_DTLS__) || defined (__WITH_TLS__)
/**
 * API to save the seed value to generate device UUID.
 * Seed value MUST be unique per device (e.g. MAC address)
 *
 * @param seed buffer of seed value.
 * @param seedSize byte length of seed
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value.
 */
OCStackResult OC_CALL SetDeviceIdSeed(const uint8_t* seed, size_t seedSize);
#endif

/**
 * cbor default init
 */
#ifndef OC_DEFAULT_CBOR_VALUE
#   if (__STDC_VERSION__ >= 199901L)
#       define OC_DEFAULT_CBOR_VALUE {.parser = NULL, .ptr = NULL, .remaining = 0, .extra = 0, .type = 0, .flags = 0}
#   else
#       define OC_DEFAULT_CBOR_VALUE {NULL, NULL, 0, 0, 0, 0}
#   endif
#endif

#ifndef OC_DEFAULT_CBOR_PARSER
#   if (__STDC_VERSION__ >= 199901L)
#       define OC_DEFAULT_CBOR_PARSER {.end = NULL, .flags = 0}
#   else
#       define OC_DEFAULT_CBOR_PARSER {NULL, 0}
#   endif
#endif

#ifndef OC_DEFAULT_OICPARSEQUWRYITER
#   if (__STDC_VERSION__ >= 199901L)
#       define OC_DEFAULT_OICPARSEQUWRYITER \
        { \
            .attrPos = NULL,\
            .attrLen = 0,\
            .valPos = NULL,\
            .valLen = 0,\
            .pi = {.n=0,.separator=NULL,.delim=NULL,.dlen=0,.pos=NULL,.segment_length=0}\
        }
#   else
#       define OC_DEFAULT_OICPARSEQUWRYITER { NULL, 0, NULL, 0, {0,NULL,NULL,0,NULL,0}}
#   endif
#endif

#ifndef OC_DEFAULT_OICUUID
#   if (__STDC_VERSION__ >= 199901L)
#   define OC_DEFAULT_OICUUID {.id= { 0 } }
#   else
#   define OC_DEFAULT_OICUUID {{ 0 }}
#   endif
#endif

/**
 * Is the URI for a Security Virtual Resource as defined
 * by Security Specification.
 *
 * @return true IFF the URI is for a SVR
 */
bool SRMIsSecurityResourceURI(const char* uri);

/**
 * Log OicUuid_t structs.
 */
void LogUuid(const OicUuid_t* uuid);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //IOTVT_SRM_UTILITY_H
