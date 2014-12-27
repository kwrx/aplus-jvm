#ifndef _JCLASS_H
#define _JCLASS_H

#include <stdint.h>

#include "jlist.h"


#define JCLASS_MAGIC			0xCAFEBABE

#define JCLASS_ATTR_UNKNOWN		(-1)
#define JCLASS_ATTR_CONSTVALUE	0
#define JCLASS_ATTR_SOURCEFILE	1
#define JCLASS_ATTR_CODE		2
#define JCLASS_ATTR_EXCEPTIONS	3
#define JCLASS_ATTR_INNERCLASS	4
#define JCLASS_ATTR_SYNTHETIC	5
#define JCLASS_ATTR_LINENUMBERS	6
#define JCLASS_ATTR_LOCALVARS	7
#define JCLASS_ATTR_DEPRECATED	8




#define ACC_PUBLIC				0x0001
#define ACC_PRIVATE				0x0002
#define ACC_PROTECTED			0x0004
#define ACC_STATIC				0x0008
#define ACC_FINAL				0x0010
#define ACC_SYNCHRONIZED		0x0020
#define ACC_VOLATILE			0x0040
#define ACC_TRANSIENT			0x0080
#define ACC_NATIVE				0x0100
#define ACC_INTERFACE			0x0200
#define ACC_ABSTRACT			0x0400
#define ACC_STRICT				0x0800



typedef struct cpinfo {
	char cp_tag;
	char cp_len;
	char* cp_name;
} cpinfo_t;

typedef struct cpvalue {
	uint8_t tag;
	uint64_t value;
	char* data;
} cpvalue_t;

typedef struct cpclass {
	uint8_t tag;
	uint16_t name_index;
} cpclass_t;

typedef struct cpfield {
	uint8_t tag;
	uint16_t class_index;
	uint16_t typename_index;
} cpfield_t;

typedef cpfield_t cpmethod_t;
typedef cpfield_t cpinterface_t;

typedef struct cpstring {
	uint8_t tag;
	uint16_t string_index;
} cpstring_t;

typedef struct cpint {
	uint8_t tag;
	uint32_t value;
} cpint_t;

typedef struct cplong {
	uint8_t tag;
	uint64_t value;
} cplong_t;

typedef struct cpfloat {
	uint8_t tag;
	float value;
} cpfloat_t;

typedef struct cpdouble {
	uint8_t tag;
	double value;
} cpdouble_t;

typedef struct cptypename {
	uint8_t tag;
	uint16_t name_index;
	uint16_t desc_index;
} cptypename_t;

typedef struct cputf8 {
	uint8_t tag;
	uint16_t length;
	char* value;
} cputf8_t;



typedef struct fieldinfo {
	uint16_t access;
	uint16_t name_index;
	uint16_t desc_index;
	uint16_t attr_count;

	list_t* attributes;
} fieldinfo_t;

typedef fieldinfo_t methodinfo_t;

typedef struct attrinfo {
	uint16_t name_index;
	uint32_t length;
	uint8_t* info;
} attrinfo_t;

typedef struct attr_cvalue {
	uint16_t name_index;
	uint32_t length;
	uint16_t cvalue_index;
} attr_cvalue_t;

typedef struct attr_sourcefile {
	uint16_t name_index;
	uint32_t length;
	uint16_t source_index;
} attr_sourcefile_t;


typedef struct innerclass {
	uint16_t info_index;
	uint16_t super_index;
	uint16_t name_index;
	uint16_t access;
} innerclass_t;

typedef struct attr_iclass {
	uint16_t name_index;
	uint32_t length;
	uint16_t classes_count;
	innerclass_t* classes;
} attr_iclass_t;

typedef struct localvars_table {
	uint16_t length;
	uint16_t name_index;
	uint16_t desc_index;
	uint16_t index;
} localvars_table_t;

typedef struct attr_localvars {
	uint16_t name_index;
	uint32_t length;
	uint16_t vars_count;
	localvars_table_t* table;
} attr_localvars_t;

typedef struct exception_table {
	uint16_t start_pc;
	uint16_t end_pc;
	uint16_t handler_pc;
	uint16_t catch_type;
} exception_table_t;

typedef struct attr_exception {
	uint16_t name_index;
	uint32_t length;
	uint16_t exceptions_count;
	uint16_t* exceptions;
} attr_exception_t;

typedef struct attr_code {
	uint16_t name_index;
	uint32_t length;
	uint16_t max_stack;
	uint16_t max_locals;
	uint32_t code_length;
	uint16_t exception_table_length;
	uint16_t attr_count;

	uint8_t* code;
	list_t* attributes;
	exception_table_t* exceptions;
} attr_code_t;


typedef struct linenumber_table {
	uint16_t pc;
	uint16_t line;
} linenumber_table_t;

typedef struct attr_linenumbers {
	uint16_t name_index;
	uint32_t length;
	uint16_t lines_count;
	linenumber_table_t* table;
} attr_linenumbers_t;

typedef struct jclass_header {
	uint32_t jc_magic;
	uint16_t jc_minor;
	uint16_t jc_major;
	uint16_t jc_access;
	uint16_t jc_this;
	uint16_t jc_super;
	uint16_t jc_interfaces_count;
	uint16_t jc_fields_count;
	uint16_t jc_methods_count;
	uint16_t jc_attributes_count;
	uint16_t jc_cp_count;

	uint16_t* jc_interfaces;
	list_t* jc_fields;
	list_t* jc_methods;
	list_t* jc_attributes;
	list_t* jc_cpinfo;
} jclass_header_t;


#define JCLASS_TAG_UTF8STRING 			1
#define JCLASS_TAG_UNKNOWN				2
#define JCLASS_TAG_INTEGER				3
#define JCLASS_TAG_FLOAT				4
#define JCLASS_TAG_LONG					5
#define JCLASS_TAG_DOUBLE				6
#define JCLASS_TAG_CLASS				7
#define JCLASS_TAG_STRING				8
#define JCLASS_TAG_FIELD				9
#define JCLASS_TAG_METHOD				10
#define JCLASS_TAG_INTERFACE			11
#define JCLASS_TAG_TYPENAME				12

#ifdef _WITH_CPINFO
static cpinfo_t cpinfo[] = {
	{ 0, 0, NULL },
	{ 1, 2, "UTF-8 String" },
	{ 2, 0, "Unknown" },
	{ 3, 4, "Integer" },
	{ 4, 4, "Float" },
	{ 5, 8, "Long" },
	{ 6, 8, "Double" },
	{ 7, 2, "Class Reference" },
	{ 8, 2, "String Reference" },
	{ 9, 4, "Field Reference" },
	{ 10, 4, "Method Reference" },
	{ 11, 4, "Interface Method Reference" },
	{ 12, 4, "TypeName Descriptor" }
};
#undef _WITH_CPINFO
#endif


#endif
