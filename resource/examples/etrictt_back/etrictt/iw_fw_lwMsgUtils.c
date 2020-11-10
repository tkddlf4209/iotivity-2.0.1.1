/**
 * @file iw_fw_lwMsgUtils.c
 * @brief �淮�� �޽��� ��ƿ��Ƽ ���� ����
 * @details �淮�� �޽��� �Ľ�, ����ȭ �� �ʼ� ��ƿ��Ƽ ���� ����
 */
#include <ctype.h>
#include "iw_fw_lwMsgUtils.h"


extern size_t _get_size_of_type(const enum ePARAM_TYPE type);

static iw_queue_t lwmsg_receive_queue = NULL;
static iw_queue_t lwmsg_send_queue = NULL;
static iw_task_t lwmsg_receive_task = (iw_task_t)NULL;
static iw_task_t lwmsg_send_task = (iw_task_t)NULL;
static BOOL network_enabled = FALSE;


/**
 * @brief �� �淮�� �޽��� ����ü ����
 * @details �� �淮�� �޽����� �����Ҵ� �ϰ� �� ����ü �����͸� ����
 * @return ������ �淮�� �޽��� ����ü ������
 */
PLWMessage new_lwm_empty_message()
{
	PLWMessage rtn = (PLWMessage)calloc(1, sizeof(struct stLightWeightMessage));

	if (!rtn) return rtn;

	iw_init_list(&rtn->header);
	iw_init_list(&rtn->body);

	return rtn;
}

/**
 * @brief �� Request �淮�� �޽��� ����ü ����
 * @details �� Request �淮�� �޽����� �����Ҵ� �ϰ� �ʱⰪ�� ������ �� �� ����ü �����͸� ����
 * @param __if Interface ID
 * @param __dri dri
 * @param __di di
 * @return ������ �淮�� �޽��� ����ü ������
 */
PLWMessage new_lwm_req_message(const uint32_t __if, const char *const __dri, const char *const __di)
{
	PLWMessage rtn = NEW_LWM_EMPTY_MESSAGE();

	if (!rtn) return rtn;

	PLWMObject _if = NEW_LWM_NUMBER_OBJECT();
	PLWMNumber num = _if->value;
	num->type = eP_UINT32;
	num->value = calloc(1, sizeof(__if));
	if (num->value)
		*(uint32_t *)num->value = __if;
	SET_LWM_HEADER_OBJECT(rtn, "if", _if);

	if (__dri) {
		PLWMObject _dri = NEW_LWM_STRING_OBJECT(__dri);
		SET_LWM_HEADER_OBJECT(rtn, "dri", _dri);
	}

	if (__di) {
		PLWMObject _di = NEW_LWM_STRING_OBJECT(__di);
		SET_LWM_HEADER_OBJECT(rtn, "di", _di);
	}

	return rtn;
}

/**
 * @brief �� Response �淮�� �޽��� ����ü ����
 * @details �� Response �淮�� �޽����� �����Ҵ� �ϰ� �ʱⰪ�� ������ �� �� ����ü �����͸� ����
 * @param __dri dri
 * @param __rsc Response Code
 * @return ������ �淮�� �޽��� ����ü ������
 */
PLWMessage new_lwm_resp_message(const char *const __dri, const uint32_t __rsc)
{
	PLWMessage rtn = NEW_LWM_EMPTY_MESSAGE();

	if (!rtn) return rtn;

	if (__dri) {
		PLWMObject _dri = NEW_LWM_STRING_OBJECT(__dri);
		SET_LWM_HEADER_OBJECT(rtn, "dri", _dri);
	}

	if (__rsc) {
		PLWMObject _rsc = NEW_LWM_NUMBER_OBJECT();
		PLWMNumber num = _rsc->value;
		num->type = eP_UINT32;
		num->value = calloc(1, sizeof(__rsc));
		if (num->value)
			*(uint32_t *)num->value = __rsc;
		SET_LWM_HEADER_OBJECT(rtn, "rsc", _rsc);
	}

	return rtn;
}

/**
 * @brief �淮�� �޽������� Interface ID ȹ��
 * @details �淮�� �޽������� Interface ID ȹ��
 * @param __msg �淮�� �޽��� ����ü ������
 * @return Interface ID, ���� �� 0
 */
uint32_t get_lwm_interface_id(PLWMessage __msg)
{
	uint32_t __if = 0;
	PLWMObject _if = get_lwm_object_from_dict(&__msg->header, "if");

	// get_lwm_number(_if, &__if);
	if (_if && IS_LWM_NUMBER(_if)) {
		PLWMNumber num = (PLWMNumber)_if->value;
		if (num && num->type == eP_UINT32) {
			__if = *(uint32_t *)(num->value);
		}
	}

	return __if;
}

/**
 * @brief �淮�� �޽������� dri ȹ��
 * @details �淮�� �޽������� dri ȹ��
 * @param __msg �淮�� �޽��� ����ü ������
 * @return dri, ���� �� NULL
 */
char *get_lwm_dri(PLWMessage __msg)
{
	char *rtn = NULL;
	PLWMObject _dri = get_lwm_object_from_dict(&__msg->header, "dri");

	if (_dri && IS_LWM_STRING(_dri)) {
		rtn = _dri->value;
	}

	return rtn;
}

/**
 * @brief Dictionary Object���� Object ȹ��
 * @details �淮�� �޽����� Dictionary Object���� Key�� ���� Object ȹ��
 * @param __dict �淮�� �޽��� Dictionary Object ������
 * @param __key Key
 * @return �淮�� �޽��� Object ������
 */
PLWMObject get_lwm_object_from_dict(LPCLWMDictionary __dict, const char *const __key)
{
	PLWMObject rtn = NULL;
	PLWMKVObject pkvo = NULL;

	if (__dict && __key) {
		LIST_FOREACH(__dict, pxLWMItem) {
			pkvo = IW_GET_LIST_NODE_DATA(pxLWMItem);
			if (strcmp(pkvo->key, __key) == 0) {
				rtn = pkvo->value;
				break;
			}
		}
	}

	return rtn;
}

/**
 * @brief Dictionary Object�� Object ����
 * @details �淮�� �޽����� Dictionary Object�� Key�� Object ����
 * @param __dict �淮�� �޽��� Dictionary Object ������
 * @param __key Key
 * @param __value ������ �淮�� �޽��� Object ������
 */
void set_lwm_object_to_dict(PLWMDictionary __dict, const char *const __key, PLWMObject __value)
{
	if (__dict && __key && __value) {
		_delete_lwm_kvo_by_key(__dict, __key);

		iw_list_node_t *lwm_item = calloc(1, sizeof(iw_list_node_t));
		if (lwm_item) {
			PLWMKVObject kvo = (PLWMKVObject)calloc(1, sizeof(LWMKVObject));
			if (!kvo) {
				free(lwm_item);
				return;
			}
			kvo->key = (char *)calloc(strlen(__key) + 1, sizeof(char));
			if (kvo->key)
				strcpy(kvo->key, __key);
			kvo->value = (PLWMObject)__value;

			_IW_SET_LIST_NODE_DATA(lwm_item, kvo);
			iw_add_list_node(__dict, lwm_item);
		}
	}
}

/**
 * @brief Array Object�� Object �߰�
 * @details �淮�� �޽��� Array Object�� �淮�� �޽��� Object �߰�
 * @param __array �淮�� �޽��� Array Object ������
 * @param __value ������ �淮�� �޽��� Object ������
 */
void add_lwm_object_to_array(PLWMArray __array, PLWMObject __value)
{
	if (__array && __value) {
		iw_list_node_t *lwm_item = calloc(1, sizeof(iw_list_node_t));
		if (lwm_item) {
			_IW_SET_LIST_NODE_DATA(lwm_item, __value);
			iw_add_list_node(__array, lwm_item);
		}
	}
}

/**
 * @brief �� �� �淮�� �޽��� Array / Dictionary Object ����
 * @details �� �� �淮�� �޽��� Array / Dictionary Object �����Ҵ����� ����
 * @return �淮�� �޽��� Array / Dictionary Object ������
 */
iw_list_t *new_lwm_empty_array_or_dict()
{
	iw_list_t * rtn = (iw_list_t *)calloc(1, sizeof(iw_list_t));
	if (rtn)
		iw_init_list(rtn);
	return rtn;
}

/**
 * @brief Ÿ�Կ� �´� �� �� �淮�� �޽��� Object ����
 * @details MicroService ������ Ÿ�Կ� ���� �˸��� �淮�� �޽��� Object�� �����Ҵ� ����
 * Array, Dictionary Ÿ���� ������, ���ڿ� Ȥ�� ���� ���¸� ����
 * @param __type MicroService ������ Ÿ��
 * @return �淮�� �޽��� Object ������
 */
PLWMObject _new_lwm_empty_object_for_ms(const enum ePARAM_TYPE __type)
{
	if (IS_DECIMAL(__type) || __type == eP_FLOAT || __type == eP_DOUBLE) {
		return new_lwm_empty_object(eLWM_Number);
	}
	else if (__type == eP_STRING) {
		return new_lwm_empty_object(eLWM_String);
	}

	return NULL;
}

/**
 * @brief Ÿ�Կ� �´� �� �� �淮�� �޽��� Object ����
 * @details �淮�� �޽��� ������ Ÿ�Կ� ���� �˸��� �淮�� �޽��� Object�� �����Ҵ� ����
 * @param __type �淮�� �޽��� ������ Ÿ��
 * @return �淮�� �޽��� Object ������
 */
PLWMObject new_lwm_empty_object(const LWMSG_DATA_TYPE __type)
{
	void *value = NULL;
	PLWMObject rtn = (PLWMObject)calloc(1, sizeof(LWMObject));

	if (!rtn)
		return rtn;

	switch (__type) {
		case eLWM_Number:
			value = calloc(1, sizeof(LWMNumber));
			break;
		case eLWM_String:
		default:
			break;
		case eLWM_Array:
			value = (PLWMArray)new_lwm_empty_array_or_dict();
			break;
		case eLWM_Dictionary:
			value = (PLWMDictionary)new_lwm_empty_array_or_dict();
			break;
	}

	rtn->type = __type;
	rtn->value = value;

	return rtn;
}

/**
 * @brief �� �淮�� �޽��� ���ڿ� Object ����
 * @details �ʱⰪ�� ���� �� �淮�� �޽��� ���� Object�� �����Ҵ� �� ����
 * @param __str �ʱ� ���ڿ� ��, NULL ����
 * @return �淮�� �޽��� ���ڿ� Object ������
 */
PLWMObject new_lwm_string_object(const char *__str)
{
	PLWMObject rtn = new_lwm_empty_object(eLWM_String);

	if (__str) {
		rtn->value = calloc(strlen(__str) + 1, sizeof(char));
		if (rtn->value)
			strcpy(rtn->value, __str);
	}

	return rtn;
}

/**
 * @brief �淮�� �޽��� ���� Object�κ��� ���� �� ȹ��
 * @details �淮�� �޽��� ���� Object�κ��� ���� �� ȹ��
 * @param __obj �淮�� �޽��� Object ������
 * @param __number_buffer ���ڰ� ����� ����
 */
void get_lwm_number(PLWMObject __obj, void *__number_buffer)
{
	if (__obj && IS_LWM_NUMBER(__obj) && __obj->value) {
		PLWMNumber value = (PLWMNumber)__obj->value;
		size_t sz = _get_size_of_type(value->type);
		memcpy(__number_buffer, value->value, sz);
	}
}

/**
 * @brief �淮�� �޽��� ���ڿ� Object�� ���ڿ� ����
 * @details �淮�� �޽��� ���ڿ� Object�� ���ڿ� ����
 * @param __obj �淮�� �޽��� Object ������
 * @param __str ���ڿ�
 */
void set_lwm_string(PLWMObject __obj, const char *__str)
{
	if (__obj && IS_LWM_STRING(__obj) && __str) {
		if (__obj->value)
			free(__obj->value);
		__obj->value = calloc(strlen(__str) + 1, sizeof(char));
		if (__obj->value)
			strcpy(__obj->value, __str);
	}
}

PLWMNumber _get_lwm_number_from_obj(PLWMObject __obj)
{
	PLWMNumber num = NULL;
	if (__obj && IS_LWM_NUMBER(__obj)) {
		if (!__obj->value) {
			__obj->value = calloc(1, sizeof(LWMNumber));
		}
		if (__obj->value) {
			num = (PLWMNumber)__obj->value;
			if (num->value)
				free(num->value);
		}
	}
	return num;
}

/**
 * @brief �淮�� �޽��� ���� Object�� ���� �� ����
 * @details �淮�� �޽��� ���� Object�� ���� �� ����
 * @param __obj �淮�� �޽��� Object ������
 * @param __int ���� ��
 */
void set_lwm_int(PLWMObject __obj, const int32_t __int)
{
	PLWMNumber num = _get_lwm_number_from_obj(__obj);
	if (num) {
		num->value = calloc(1, sizeof(__int));
		num->type = eP_INT32;
		if (num->value)
			*(int32_t *)num->value = __int;
	}
}

/**
 * @brief �淮�� �޽��� ���� Object�� �Ǽ� �� ����
 * @details �淮�� �޽��� ���� Object�� �Ǽ� �� ����
 * @param __obj �淮�� �޽��� Object ������
 * @param __float �Ǽ� ��
 */
void set_lwm_float(PLWMObject __obj, const float __float)
{
	PLWMNumber num = _get_lwm_number_from_obj(__obj);
	if (num) {
		num->value = calloc(1, sizeof(__float));
		num->type = eP_FLOAT;
		if (num->value)
			*(float *)num->value = __float;
	}
}

/**
 * @brief �淮�� �޽��� Object ����
 * @details �淮�� �޽��� Object �����ϰ� ����
 * @param __obj ������ �淮�� �޽��� Object ������
 */
void delete_lwm_object(PLWMObject __obj)
{
	if (__obj) {
		if (__obj->value) {
			if (IS_LWM_NUMBER(__obj)) {
				PLWMNumber value = (PLWMNumber)__obj->value;
				if (value->value)
					free(value->value);
				free(__obj->value);
			}
			else if (IS_LWM_STRING(__obj)) {
				free(__obj->value);
			}
			else if (IS_LWM_ARRAY(__obj)) {
				_delete_lwm_array((PLWMArray)__obj->value);
			}
			else if (IS_LWM_DICT(__obj)) {
				_delete_lwm_dictionary((PLWMDictionary)__obj->value);
			}
			__obj->value = NULL;
			__obj->type = eLWM_Unknown;
		}

		free(__obj);
	}
}

/**
 * @brief �淮�� �޽��� ����
 * @details �淮�� �޽��� �����ϰ� ����
 * @param __msg ������ �淮�� �޽��� ����ü ������
 */
void delete_lwm_message(PLWMessage __msg)
{
	if (__msg) {
		_delete_lwm_kvo(&__msg->header);
		_delete_lwm_kvo(&__msg->body);
		free(__msg);
	}
}

/**
 * @brief �淮�� �޽��� KVO ���� Object ����
 * @details �淮�� �޽��� KVO ���� Object �����ϰ� ����
 * @param __obj ���� �� �淮�� �޽��� KVO ���� Object ������
 */
void _delete_lwm_keyvalue_object(PLWMKVObject __pkvo)
{
	if (__pkvo) {
		delete_lwm_object(__pkvo->value);
		free(__pkvo->key);
		__pkvo->value = NULL;
		__pkvo->key = NULL;
		free(__pkvo);
	}
}

/**
 * @brief �淮�� �޽��� Dictionary Object���� Key/Value ����
 * @details �淮�� �޽��� Dictionary Object���� ������ Key�� �� Value �����ϰ� ����
 * Value�� �淮�� �޽��� KVO ���� Object
 * @param __dict �淮�� �޽��� Dictionary Object ������
 * @param __key ������ Ű
 */
void _delete_lwm_kvo_by_key(PLWMDictionary __dict, const char *const __key)
{
	if (__dict && __key) {
		LIST_FOREACH(__dict, pxLWMItem) {
			PLWMKVObject pkvo = IW_GET_LIST_NODE_DATA(pxLWMItem);
			if (pkvo && strcmp(pkvo->key, __key) == 0) {
				_delete_lwm_keyvalue_object(pkvo);
				pkvo = NULL;
				iw_remove_list_node(__dict, pxLWMItem);
				free(pxLWMItem);
				break;
			}
		}
	}
}

/**
 * @brief �淮�� �޽��� Dictionary Object ���� ����
 * @details �淮�� �޽��� Dictionary Object���� ���븸 �����ϰ� ��� ����
 * @param __dict �淮�� �޽��� Dictionary Object ������
 */
void _delete_lwm_kvo(PLWMDictionary __dict)
{
	if (__dict) {
		LIST_FOREACH_TO_DEL(__dict, pxLWMItem) {
			PLWMKVObject pkvo = IW_GET_LIST_NODE_DATA(pxLWMItem);
			_delete_lwm_keyvalue_object(pkvo);
			pkvo = NULL;
			iw_remove_list_node(__dict, pxLWMItem);
			free(pxLWMItem);
		}
	}
}

/**
 * @brief �淮�� �޽��� Dictionary Object ����
 * @details �淮�� �޽��� Dictionary Object �����ϰ� ����
 * @param __dict ���� �� �淮�� �޽��� Dictionary Object ������
 */
void _delete_lwm_dictionary(PLWMDictionary __dict)
{
	_delete_lwm_kvo(__dict);
	free(__dict);
}

/**
 * @brief �淮�� �޽��� Array Object ����
 * @details �淮�� �޽��� Array Object �����ϰ� ����
 * @param __dict ���� �� �淮�� �޽��� Array Object ������
 */
void _delete_lwm_array(PLWMArray __array)
{
	if (__array) {
		LIST_FOREACH_TO_DEL(__array, pxLWMItem) {
			PLWMObject obj = IW_GET_LIST_NODE_DATA(pxLWMItem);
			DEL_LWM_OBJECT(obj);
			obj = NULL;
			iw_remove_list_node(__array, pxLWMItem);
			free(pxLWMItem);
		}
		free(__array);
	}
}

/**
 * @brief �淮�� �޽��� Object ����ȭ
 * @details �淮�� �޽��� Object�� �����Ҵ��� ���ڿ��� ����ȭ
 * @param __obj �淮�� �޽��� Object ������
 * @return ����ȭ �� ���ڿ�, ���� �� NULL
 */
char *serialize_lwm_object(PLWMObject __obj)
{
	char *str = NULL;

	if (__obj && __obj->value) {
		if (__obj->type == eLWM_Number) {
			PLWMNumber num = __obj->value;
			if (num->type == eP_INT32) {
				// "-2147483648"
				str = calloc(12, sizeof(char));
				if (str)
					snprintf(str, 11, "%ld", *(int32_t *)num->value);
			}
			else if (num->type == eP_UINT32) {
				// "4294967295"
				str = calloc(11, sizeof(char));
				if (str)
					snprintf(str, 10, "%ld", *(uint32_t *)num->value);
			}
			else if (num->type == eP_INT16) {
				// "-32768"
				str = calloc(7, sizeof(char));
				if (str)
					snprintf(str, 6, "%d", *(int16_t *)num->value);
			}
			else if (num->type == eP_UINT16) {
				// "65535"
				str = calloc(6, sizeof(char));
				if (str)
					snprintf(str, 5, "%d", *(uint16_t *)num->value);
			}
			else if (num->type == eP_INT8) {
				// "-128"
				str = calloc(5, sizeof(char));
				if (str)
					snprintf(str, 4, "%d", *(int8_t *)num->value);
			}
			else if (num->type == eP_UINT8) {
				// "255"
				str = calloc(4, sizeof(char));
				if (str)
					snprintf(str, 3, "%d", *(uint8_t *)num->value);
			}
			else if (num->type == eP_FLOAT) {
				str = calloc(21, sizeof(char));
				if (str)
					snprintf(str, 20, "%.2f", *(float *)num->value);
			}
			else if (num->type == eP_DOUBLE) {
				str = calloc(21, sizeof(char));
				if (str)
					snprintf(str, 20, "%lf", *(double *)num->value);
			}
		}
		else if (__obj->type == eLWM_String) {
			size_t len = strlen(__obj->value);
			str = calloc(len + 1, sizeof(char));
			if (str)
				memcpy(str, __obj->value, len);
		}
		else if (__obj->type == eLWM_Array) {
			str = serialize_lwm_array(__obj->value);
		}
		else if (__obj->type == eLWM_Dictionary) {
			str = serialize_lwm_dictionary(__obj->value);
		}
	}

	return str;
}

/**
 * @brief �淮�� �޽��� KVO ���� Object ����ȭ
 * @details �淮�� �޽��� KVO ���� Object�� �����Ҵ��� ���ڿ��� ����ȭ
 * @param __kvo �淮�� �޽��� KVO ���� Object ������
 * @return ����ȭ �� ���ڿ�, ���� �� NULL
 */
char *serialize_lwm_kvo(PLWMKVObject __kvo)
{
	char *str = NULL;

	if (__kvo && __kvo->key && __kvo->value) {
		char *str_value = serialize_lwm_object(__kvo->value);
		if (str_value) {
			str = calloc(strlen(__kvo->key) + strlen(str_value) + 2, sizeof(char));
			if (str) {
				strcpy(str, __kvo->key);
				strcat(str, "=");
				strcat(str, str_value);
			}
			free(str_value);
		}
	}

	return str;
}

/**
 * @brief �淮�� �޽��� Array Object ����ȭ
 * @details �淮�� �޽��� Array Object�� �����Ҵ��� ���ڿ��� ����ȭ
 * @param __array �淮�� �޽��� Array Object ������
 * @return ����ȭ �� ���ڿ�
 */
char *serialize_lwm_array(LPCLWMArray __array)
{
	char *str = calloc(3, sizeof(char));
	if (str)
		strcpy(str, "[");

	if (__array) {
		LIST_FOREACH(__array, pxLWMItem) {
			PLWMObject pobj = IW_GET_LIST_NODE_DATA(pxLWMItem);
			char *temp = serialize_lwm_object(pobj);
			if (temp) {
				str = realloc(str, strlen(str) + strlen(temp) + 3);
				if (str) {
					if (str[1] != '\0')
						strcat(str, ",");
					strcat(str, temp);
				}
				free(temp);
			}
		}
	}

	if (str)
		strcat(str, "]");
	return str;
}

/**
 * @brief �淮�� �޽��� Dictionary Object ����ȭ
 * @details �淮�� �޽��� Dictionary Object�� �����Ҵ��� ���ڿ��� ����ȭ
 * @param __dict �淮�� �޽��� Dictionary Object ������
 * @return ����ȭ �� ���ڿ�
 */
char *serialize_lwm_dictionary(LPCLWMDictionary __dict)
{
	char *str = calloc(3, sizeof(char));
	if (str)
		strcpy(str, "{");

	if (__dict) {
		LIST_FOREACH(__dict, pxLWMItem) {
			PLWMKVObject pkvo = IW_GET_LIST_NODE_DATA(pxLWMItem);
			char *temp = serialize_lwm_kvo(pkvo);
			if (temp) {
				str = realloc(str, strlen(str) + strlen(temp) + 3);
				if (str) {
					if (str[1] != '\0')
						strcat(str, ";");
					strcat(str, temp);
				}
				free(temp);
			}
		}
	}

	if (str)
		strcat(str, "}");
	return str;
}

/**
 * @brief �淮�� �޽��� ����ȭ
 * @details �淮�� �޽����� �����Ҵ��� ���ڿ��� ����ȭ
 * @param __msg �淮�� �޽��� ������
 * @return ����ȭ �� ���ڿ�, ���� �� NULL
 */
char *serialize_lwm_message(PLWMessage __msg)
{
	char *str = NULL;
	char *header = NULL, *body = NULL;

	if (__msg) {
		size_t len = 0;
		header = serialize_lwm_dictionary(&__msg->header);
		if (IW_GET_LIST_NODE_NUM(&__msg->body) > 0)
			body = serialize_lwm_dictionary(&__msg->body);
		if (header) len += strlen(header);
		if (body) len += strlen(body);

		str = calloc(len + 1, sizeof(char));
		if (str && header) {
			strcpy(str, header);
			free(header);
		}
		if (str && body) {
			strcat(str, body);
			free(body);
		}
	}

	return str;
}


// mode
// 0 : ���� ����
// 1 << 0 : dict
// 1 << 1 : array
// 1 << 2 : key:value �� key
// 1 << 3 : key:value �� value
enum eLWM_PARSING_MODE {
	ePM_Openable = 1 << 0,
	ePM_Dict = 1 << 1,
	ePM_Array = 1 << 2,
	ePM_KV_Key = 1 << 3,
	ePM_KV_Val = 1 << 4,
	ePM_Object = ePM_Dict | ePM_Array,
	ePM_Header = 1 << 6,
	ePM_Body = 1 << 7,
};

#define IS_LWM_OPENABLE(__mode) ((__mode & ePM_Openable) == ePM_Openable)
#define IS_LWM_HEADER_OPENABLE(__mode) (__mode == (ePM_Header | ePM_Openable))
#define IS_LWM_BODY_OPENABLE(__mode) (__mode == (ePM_Body | ePM_Openable))
#define SET_LWM_HEADER_OPENABLE(__mode) __mode = (ePM_Header | ePM_Openable)
#define SET_LWM_BODY_OPENABLE(__mode) __mode = (ePM_Body | ePM_Openable)
#define IS_LWM_OBJECT(__mode) ((__mode & ePM_Object) != 0)

static BOOL _lwm_is_token(const char ch, const char *tokens);
static PLWMObject _lwm_new_array_parsing(const char **p);
static PLWMObject _lwm_dict_parsing(const char **p, PLWMDictionary dict, BOOL no_return);
static PLWMObject _lwm_new_dict_parsing(const char **p);
static PLWMKVObject _lwm_kvo_parsing(const char **p);


/**
 * @brief ������ ��ū ���� ���� ȹ��
 * @details �־��� ���ڰ� �淮�� �޽��� ���ڿ������� ��ū �������� ���θ� ȹ��
 * ���ĺ�, ����, ���鿡 ���ԵǴ��� Ȥ�� ��� ������ �������� ���η� �Ǵ�
 * �Ǵ� 2��° ������ tokens�� �����ص� ��ū �������� �Ǵ�
 * @param __ch ��ū ���� ���� Ȯ���� ����
 * @param tokens �߰� ��ū üũ �� ����� ���ڵ�
 * @return ��ū �����ϸ� 1, �ƴϸ� 0
 */
static BOOL _lwm_is_token(const char ch, const char *tokens)
{
	const char *p = tokens;
	// ���������� NULL���ڴ� ������ ó�����ڷ� ����
	if (*p == '\0') return TRUE;

	for (; *p != '\0'; ++p) {
		if (ch == *p || !(isalnum(ch) || isspace(ch) || ispunct(ch)))
			return TRUE;
	}

	return FALSE;
}

/**
 * @brief �淮�� �޽��� Array Object�� �Ľ�
 * @details �־��� ���ڿ��κ��� �淮�� �޽��� Array Object�� �Ľ��� ȹ��
 * @param p ���ڿ� ������
 * @return �淮�� �޽��� Array Object ������
 */
static PLWMObject _lwm_new_array_parsing(const char **p)
{
	PLWMObject arr_obj = NEW_LWM_ARRAY_OBJECT();
	PLWMObject obj = NULL;
	enum eLWM_PARSING_MODE mode = ePM_Openable;
	BOOL bFlag = TRUE;
	const char *start = NULL;
	char *valstr = NULL;
	BOOL value_has_num = FALSE;
	BOOL value_has_word = FALSE;
	BOOL value_has_one_period = FALSE;
	BOOL value_has_many_period = FALSE;

	if (**p == '[')
		++(*p);
	if (**p == ']')
		return arr_obj;

	for (; p && *p && **p != '\0'; ++(*p)) {
		if (isspace(**p))
			continue;
		if (IS_LWM_OPENABLE(mode) && (**p == ',' || **p == ']')) {
			bFlag = FALSE;
			break;
		}

		if (IS_LWM_OPENABLE(mode) && **p == '{') {
			mode = ePM_Dict;
			obj = _lwm_new_dict_parsing(p);
			ADD_LWM_ARRAY_ITEM(arr_obj, obj);
			obj = NULL;
		}
		else if (IS_LWM_OPENABLE(mode) && **p == '[') {
			mode = ePM_Array;
			obj = _lwm_new_array_parsing(p);
			ADD_LWM_ARRAY_ITEM(arr_obj, obj);
			obj = NULL;
		}
		else if (!IS_LWM_OPENABLE(mode) && IS_LWM_OBJECT(mode)) {
			if (**p == ',') {
				mode = ePM_Openable | ePM_Object;
				continue;
			}
			else if (**p == ']') {
				break;
			}
		}
		else if (IS_LWM_OPENABLE(mode) && start == NULL)  {
			mode = ePM_KV_Val;
			start = *p;

			if (_lwm_is_token(**p, "};=")) {
				bFlag = FALSE;
				break;
			}
		}
		else if (mode == ePM_KV_Val && (**p == ',' || **p == ']')) {
			size_t len = *p - start + 1;
			valstr = (char *)malloc(*p - start + 1);
			ZeroMemory(valstr, len);
			strncpy(valstr, start, len - 1);

			obj = NEW_LWM_STRING_OBJECT(valstr);
			ADD_LWM_ARRAY_ITEM(arr_obj, obj);
			obj = NULL;

			if (valstr) free(valstr);
			valstr = NULL;
			start = NULL;
			mode = ePM_Openable;

			if (**p == ']')
				break;
		}
		else if (mode == ePM_KV_Val) {
			// do nothing
		}
		else {
			bFlag = FALSE;
			break;
		}

		if (mode == ePM_KV_Val) {
			if (**p != '.' && !isdigit(**p)) value_has_word = TRUE;
			if (isdigit(**p)) value_has_num = TRUE;
			if (!value_has_one_period && !value_has_many_period && **p == '.')
				value_has_one_period = TRUE;
			else if (value_has_one_period && !value_has_many_period && **p == '.')
				value_has_many_period = FALSE;
		}
	}

	if (!bFlag) {
		if (obj) DEL_LWM_OBJECT(obj);
		obj = NULL;
		if (arr_obj) DEL_LWM_OBJECT(arr_obj);
		arr_obj = NULL;
	}

	return arr_obj;
}

/**
 * @brief �淮�� �޽��� Dictionary Object�� �Ľ�
 * @details �־��� ���ڿ��κ��� �淮�� �޽��� Dictionary Object�� �Ľ��� ȹ��
 * @param p ���ڿ� ������
 * @param dict �Ľ��� �����Ͱ� ����� �̸� ������� �淮�� �޽��� Dictionary Object
 * @param no_return ���� �� ���� ����
 * @return �淮�� �޽��� Dictionary Object ������, ���� Ȥ�� no_return == 1 �� NULL
 */
static PLWMObject _lwm_dict_parsing(const char **p, PLWMDictionary dict, BOOL no_return)
{
	PLWMObject obj = NULL;
	BOOL bFlag = TRUE;
	PLWMKVObject kvo = NULL;

	if (**p == '{')
		++(*p);

	for (; p && *p && **p != '\0'; ++(*p)) {
		if (isspace(**p))
			continue;
		if (**p == '}')
			break;
		if (**p == ';')
			continue;

		kvo = _lwm_kvo_parsing(p);
		if (kvo) {
			_delete_lwm_kvo_by_key(dict, kvo->key);

			iw_list_node_t *lwm_item = malloc(sizeof(iw_list_node_t));
			if (lwm_item) {
				ZeroMemory(lwm_item, sizeof(iw_list_node_t));
				_IW_SET_LIST_NODE_DATA(lwm_item, kvo);
				iw_add_list_node(dict, lwm_item);
			}

			if (**p == '}')
				break;
		}
		else {
			bFlag = FALSE;
			break;
		}
	}

	if (bFlag && dict && !no_return) {
		obj = NEW_LWM_DICT_OBJECT();
		obj->type = eLWM_Dictionary;
		obj->value = dict;
	}
	if (!bFlag && dict) {
		_delete_lwm_kvo(dict);
	}

	return obj;
}

/**
 * @brief �淮�� �޽��� Dictionary Object�� �Ľ�
 * @details �־��� ���ڿ��κ��� �淮�� �޽��� Dictionary Object�� �Ľ��� ȹ��
 * @param p ���ڿ� ������
 * @return �淮�� �޽��� Dictionary Object ������
 */
static PLWMObject _lwm_new_dict_parsing(const char **p)
{
	PLWMDictionary dict = (PLWMDictionary)new_lwm_empty_array_or_dict();
	PLWMObject obj = _lwm_dict_parsing(p, dict, FALSE);
	if (!obj) {
		free(dict);
	}
	return obj;
}

/**
 * @brief �淮�� �޽��� KVO ���� Object�� �Ľ�
 * @details �־��� ���ڿ��κ��� �淮�� �޽��� KVO ���� Object�� �Ľ��� ȹ��
 * @param p ���ڿ� ������
 * @return �淮�� �޽��� KVO ���� Object ������
 */
static PLWMKVObject _lwm_kvo_parsing(const char **p)
{
	PLWMKVObject kvo = NULL;
	BOOL bFlag = TRUE;
	enum eLWM_PARSING_MODE mode = ePM_KV_Key;
	char *key = NULL, *valstr = NULL;
	PLWMObject value = NULL;
	const char *start = NULL;
	BOOL value_has_num = FALSE;
	BOOL value_has_word = FALSE;
	BOOL value_has_one_period = FALSE;
	BOOL value_has_many_period = FALSE;

	for (; p && *p && **p != '\0'; ++(*p)) {
		if (isspace(**p))
			continue;

		if (mode == ePM_KV_Key) {
			// {[]};,
			if (_lwm_is_token(**p, "{[]};,")) {
				bFlag = FALSE;
				break;
			}
			// =
			if (**p == '=') {
				if (!start || start[0] == '\0') {
					bFlag = FALSE;
					break;
				}
				else {
					size_t len = *p - start + 1;
					key = (char *)malloc(*p - start + 1);
					ZeroMemory(key, len);
					strncpy(key, start, len - 1);

					mode = ePM_KV_Val;
					start = NULL;
					continue;
				}
			}

			if (start == NULL) start = *p;
		}
		else if (mode == ePM_KV_Val) {
			if (start == NULL)  {
				start = *p;

				if (**p == '{') {
					value = _lwm_new_dict_parsing(p);
					if (!value) bFlag = FALSE;
					break;
				}
				else if (**p == '[') {
					value = _lwm_new_array_parsing(p);
					if (!value) bFlag = FALSE;
					break;
				}
				else if (_lwm_is_token(**p, "]};=,")) {
					bFlag = FALSE;
					break;
				}
			}
			else if (_lwm_is_token(**p, "{[=,")) {
				bFlag = FALSE;
				break;
			}
			else if (_lwm_is_token(**p, "]};")) {
				size_t len = *p - start + 1;
				valstr = (char *)malloc(*p - start + 1);
				ZeroMemory(valstr, len);
				strncpy(valstr, start, len - 1);
				break;
			}

			if (**p != '.' && !isdigit(**p)) value_has_word = TRUE;
			if (isdigit(**p)) value_has_num = TRUE;
			if (!value_has_one_period && !value_has_many_period && **p == '.')
				value_has_one_period = TRUE;
			else if (value_has_one_period && !value_has_many_period && **p == '.')
				value_has_many_period = FALSE;
		}
		else {
			bFlag = FALSE;
			break;
		}
	}

	if (bFlag) {
		kvo = (PLWMKVObject)malloc(sizeof(LWMKVObject));
		ZeroMemory(kvo, sizeof(LWMKVObject));
		kvo->key = key;
		if (!value) {
			kvo->value = NEW_LWM_STRING_OBJECT(valstr);
		}
		else {
			kvo->value = (PLWMObject)value;
		}
	}
	else {
		if (key) free(key);
		key = NULL;
	}

	if (valstr) free(valstr);
	valstr = NULL;

	return kvo;
}

/**
 * @brief �淮�� �޽��� �Ľ�
 * @details �־��� ���ڿ��κ��� �淮�� �޽����� �Ľ��� ȹ��
 * @param p ���ڿ� ������
 * @return �淮�� �޽��� ������
 */
PLWMessage parse_lwm_message_str(const char *__str)
{
	if (!__str)
		return NULL;

	size_t len = strlen(__str);
	if (len == 0)
		return NULL;

	PLWMessage pmsg = NEW_LWM_EMPTY_MESSAGE();
	const char *p = __str;
	enum eLWM_PARSING_MODE SET_LWM_HEADER_OPENABLE(mode);
	BOOL bFlag = FALSE;

	for (; *p != '\0'; ++p) {
		if (isspace(*p))
			continue;
		// []};=,
		if (_lwm_is_token(*p, "[]};=,")) {
			break;
		}

		if (*p == '{') {
			if (IS_LWM_HEADER_OPENABLE(mode)) {
				_lwm_dict_parsing(&p, &pmsg->header, TRUE);
				SET_LWM_BODY_OPENABLE(mode);
				bFlag = TRUE;
			}
			else if (IS_LWM_BODY_OPENABLE(mode)) {
				_lwm_dict_parsing(&p, &pmsg->body, TRUE);
				mode = ePM_Openable;
				bFlag = TRUE;
			}
			else {
				bFlag = FALSE;
				break;
			}
		}
		else {
			bFlag = FALSE;
			break;
		}
	}

	if (!bFlag) {
		DEL_LWM_MESSAGE(pmsg);
		pmsg = NULL;
	}

	return pmsg;
}

iw_queue_t get_lwmsg_receive_queue()
{
	return lwmsg_receive_queue;
}

void set_lwmsg_receive_queue(iw_queue_t __lwmsg_recv_q)
{
	if (__lwmsg_recv_q)
		lwmsg_receive_queue = __lwmsg_recv_q;
}

iw_queue_t get_lwmsg_send_queue()
{
	return lwmsg_send_queue;
}

void set_lwmsg_send_queue(iw_queue_t __lwmsg_send_q)
{
	if (!__lwmsg_send_q) return;

	if (lwmsg_send_queue) {
		PLWMessage lwmsg;
		while (recv_queue(lwmsg_send_queue, &lwmsg, FALSE) == IW_SUCCESS) {
			DEL_LWM_MESSAGE(lwmsg);
			sleep(10);
		}
		delete_queue(lwmsg_send_queue);
	}
	lwmsg_send_queue = __lwmsg_send_q;
}

iw_task_t get_lwmsg_receive_task()
{
	return lwmsg_receive_task;
}

void set_lwmsg_receive_task(iw_task_t __lwmsg_recv_t)
{
	lwmsg_receive_task = __lwmsg_recv_t;
}

iw_task_t get_lwmsg_send_task()
{
	return lwmsg_send_task;
}

void set_lwmsg_send_task(iw_task_t __lwmsg_send_t)
{
	lwmsg_send_task = __lwmsg_send_t;
}

/**
 * @brief Response �淮�� �޽��� �۽�
 * @details Response �淮�� �޽�����, ��Ʈ��ũ �������̽��� ���� �۽�
 * @param __msg �淮�� �޽��� ������
 * @return send_queue() ���� �� 0, ���� �� -1
 */
iw_error_t response_lwmsg(PLWMessage __msg)
{
	if (lwmsg_send_queue && __msg)
		return send_queue(lwmsg_send_queue, &__msg, 0, lwmsg_send_task);
	return IW_FAIL;
}

/**
 * @brief Request �淮�� �޽��� �۽�
 * @details Request �淮�� �޽�����, ��Ʈ��ũ �������̽��� ���� �۽�
 * @param __msg �淮�� �޽��� ������
 * @return send_queue() ���� �� 0, ���� �� -1
 */
iw_error_t request_lwmsg(PLWMessage __msg)
{
	if (lwmsg_send_queue && __msg)
		return send_queue(lwmsg_send_queue, &__msg, 0, lwmsg_send_task);
	return IW_FAIL;
}

BOOL get_lwmsg_network_enabled()
{
	return network_enabled;
}

void set_lwmsg_network_enabled(BOOL __enabled)
{
	network_enabled = __enabled;
}
