/*
 * brief
 */

#ifndef __OBJECT_H__
#define __OBJECT_H__


typedef void dtor_type(void *obj);
typedef void *ctor_type(void *obj, dtor_type *dtor, va_list *ap);

#define CLASS_MAX_NAME_LEN	64

struct BASE_CLASS_OBJECT {
	ya_list_t list_node;
	dtor_type *dtor;
	int ref;
	char class_name[CLASS_MAX_NAME_LEN];
};

#define CLASS_ATTR_DATE void *private; \
		dtor_type *dtor

void *new(const char *name, size_t size, ctor_type ctor, dtor_type dtor, ...);
void *base_class_ctor(void *obj, ctor_type ctor, dtor_type dtor, ...);
void ref_obj(void *obj);
void unref_obj(void *obj);
struct BASE_CLASS_OBJECT *class(void *obj);

#endif
