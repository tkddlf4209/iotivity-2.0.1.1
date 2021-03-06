/* ****************************************************************
 *
 * Copyright 2014 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#ifndef CA_COMMON_UTIL_H_
#define CA_COMMON_UTIL_H_

#include "cacommon.h"
#include "experimental/logger.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * Macro to verify the validity of input argument.
 *
 * @param  expr         Expression to verify.
 * @param  log_tag      Log tag.
 * @param  log_message  Log message.
 */
#define VERIFY_TRUE(expr, log_tag, log_message) \
    VERIFY_TRUE_RET((expr), (log_tag), (log_message), CA_STATUS_INVALID_PARAM)

/**
 * Macro to verify the validity of input argument.
 *
 * @param  expr         Expression to verify.
 * @param  log_tag      Log tag.
 * @param  log_message  Log message.
 * @param  ret          Return value.
 */
#define VERIFY_TRUE_RET(expr, log_tag, log_message, ret) \
    if (!(expr)) \
    { \
        OIC_LOG_V(ERROR, (log_tag), "Invalid input: %s", (log_message)); \
        return (ret); \
    } \

/**
 * Macro to verify the validity of input argument.
 *
 * @param  arg  log level
 * @param  log_tag  log tag
 * @param  log_message  log message
 * @param  ret  return value
 */
#define VERIFY_NON_NULL_RET(arg, log_tag, log_message, ret) \
    VERIFY_TRUE_RET(NULL != (arg), (log_tag), (log_message), (ret))

/**
 * Macro to verify the validity of input argument.
 *
 * @param  arg  log level
 * @param  log_tag  log tag
 * @param  log_message  log message
 */
#define VERIFY_NON_NULL(arg, log_tag, log_message) \
    VERIFY_NON_NULL_RET((arg), (log_tag), (log_message), CA_STATUS_INVALID_PARAM)

/**
 * Macro to verify the validity of input argument.
 *
 * @param  arg  log level
 * @param  log_tag  log tag
 * @param  log_message  log message
 */
#define VERIFY_NON_NULL_VOID(arg, log_tag, log_message) \
    if (NULL == (arg)) \
    { \
        OIC_LOG_V(ERROR, (log_tag), "Invalid input:%s", (log_message)); \
        return; \
    } \

/**
 * Macro to verify an expression, goto exit: if not satisfied
 *
 * @param  log_tag    log tag
 * @param  expr       Expression to verify.
 * @param  msg        Message to log prior to exiting
 * @param  log_level  logging level
 *
 * @note Invoking function must define "exit:" label for goto functionality to work correctly.
 */
#define VERIFY_SUCCESS_OR_EXIT(log_tag, expr, msg, log_level) do{ if (!(expr)) \
    {OIC_LOG((log_level), (log_tag), (msg)); goto exit; } }while(0)


/**
 * Macro to verify an expression, or return
 *
 * @param  log_tag    log tag
 * @param  expr       Expression to verify.
 * @param  msg        Message to log prior to exiting
 * @param  log_level  logging level
 */
#define VERIFY_SUCCESS_OR_RETURN(log_tag, expr, msg, log_level) do{ if (!(expr)) \
    {OIC_LOG((log_level), (log_tag), (msg)); return; } }while(0)

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* CA_COMMON_UTIL_H_ */
