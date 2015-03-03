/*
 * brief
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "ya/list.h"
#include "ya/object.h"
#include "ya/util.h"
#include "ya/string.h"


void *new(const char *name, size_t size, ctor_type ctor, dtor_type dtor, ...)
{
	struct BASE_CLASS_OBJECT *obj = calloc(1, size);
	va_list ap;
	void *ret;

	if (!obj) {
		return NULL;
	}

	ya_list_init(obj);
	obj->dtor = dtor;
	ref_obj(obj);
	ya_strcpy(obj->class_name, name, sizeof(obj->class_name));

	va_start(ap, dtor);
	ret = ctor(obj, dtor, &ap);
	va_end(ap);

	if(!ret) {
		free(obj);
		return NULL;
	}

	return obj;
}

void *base_class_ctor(void *obj, ctor_type ctor, dtor_type dtor, ...)
{
	void *ret;
	va_list ap;

	va_start(ap, dtor);
	ret = ctor(obj, dtor, &ap);
	va_end(ap);

	return ret;
}

static void delete(void *obj)
{
	struct BASE_CLASS_OBJECT *class_obj = (struct BASE_CLASS_OBJECT *)obj;

	class_obj->dtor(class_obj);
	free(class_obj);
}

void ref_obj(void *obj)
{
	struct BASE_CLASS_OBJECT *class_obj = (struct BASE_CLASS_OBJECT *)obj;
	class_obj->ref ++;
}

void unref_obj(void *obj)
{
	struct BASE_CLASS_OBJECT *class_obj = (struct BASE_CLASS_OBJECT *)obj;
	class_obj->ref --;
	if(class_obj->ref <= 0) {
		delete(obj);
	}
}
