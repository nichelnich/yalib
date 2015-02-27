/*
 * Copyright (C), 2010-2013, ATCOM Tech. Co., Ltd.
 * File Name: object.c
 * Author: hyh
 * Email: hyf@atcom.com.cn 
 * Created: Sun 12 May 2013 08:29:58 AM CST
 */

/**
 * @file object.c
 * @brief OO的基础.
 *
 * OO的基础
 */

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "ya/object.h"
#include "object_i.h"

/*
 * object
 */

static void *object_ctor(void *_self, va_list *app)
{
	return _self;
}

static void *object_dtor(void *_self)
{
	return _self;
}

const void *class_of(const void *_self)
{
	const struct OBJECT *self = _self;

	assert(self && self->class);
	return self->class;
}

size_t size_of(const void *_self)
{
	const struct CLASS *class = class_of(_self);

	return class->size;
}

/*
 * class
 */

static void *class_ctor(void *_self, va_list *app)
{
	struct CLASS *self = _self;
	const size_t offset = offsetof(struct CLASS, ctor);

	self->name = va_arg(*app, char *);
	self->super = va_arg(*app, struct CLASS *);
	self->size = va_arg(*app, size_t);

	assert(self->super);

	memcpy((char *)self + offset, (char *)self->super + offset
			, size_of(self->super) - offset);

	/** 派生类继承基类的public接口 */
	{
		typedef void (*voidf)(); /**< 通用的函数指针 */
		voidf selector;
		va_list ap = *app;

		while((selector = va_arg(ap, voidf)))
		{
			voidf method = va_arg(ap, voidf);

			if(selector == (voidf)ctor)
			{
				*(voidf *) &self->ctor = method;
			}
			else if(selector == (voidf)dtor)
			{
				*(voidf *) &self->dtor = method;
			}
		}

		return self;
	}
}

static void *class_dtor(void *_self)
{
	struct CLASS *self = _self;

	fprintf(stderr, "%s, destroy class\n", self->name);
	return _self;
}

const void *super(const void *_self)
{
	const struct CLASS *self = _self;

	assert(self && self->super);
	return self->super;
}

/*
 * 初始化
 */

static const struct CLASS __object[] = 
{
	{
		{__object + 1},
		"OBJECT", 
		__object, 
		sizeof(struct OBJECT),
		object_ctor,
		object_dtor
	},
	{
		{__object + 1},
		"CLASS",
		__object,
		sizeof(struct CLASS),
		class_ctor,
		class_dtor
	}
};

const void *_object = __object;
const void *_class = __object + 1;

const void * const object(void)
{
	return _object;
}

const void * const class(void)
{
	return _class;
}

/*
 * 对象管理和选择器
 */


/**
 * @brief 根据元类或类的类型描述构造新的元类对象或类对象.
 *
 * 根据元类或类的类型描述构造新的元类对象或类对象.
 * @param [in]_class 元类的类型描述
 * @return 元类或类的对象
 */
void *new(const void *_class, ...)
{
	const struct CLASS *class = _class;
	struct OBJECT *object;
	va_list ap;

	assert(class && class->size);
	object = calloc(1, class->size);

	assert(object);
	object->class = class; /* 指向元类或类的类型描述 */

	
	va_start(ap, _class);
	object = ctor(object, &ap); 
	va_end(ap);

	return object;
}

/**
 * @brief 删除一个对象.
 *
 * 删除一个对象.
 * @param [in]_self 对象指针
 * @return 无
 */
void delete(void *_self)
{
	if(_self)
		free(dtor(_self));
}

/**
 * @brief 构造函数选择器.
 *
 * 构造函数选择器.
 * @param [in]_class 类类型描述指针
 * @param [in]_self 对象指针
 * @param [in]app 构造函数参数
 * @return 对象指针
 */
void *ctor(void *_self, va_list *app)
{
	const struct CLASS *class = class_of(_self);

	assert(class->ctor);
	return class->ctor(_self, app);
}

/**
 * @brief 基类构造函数选择器.
 *
 * 基类构造函数选择器.
 * @param [in]_class 类类型描述指针
 * @param [in]_self 对象指针
 * @param [in]app 构造函数参数
 * @return 对象指针
 */
void *super_ctor(const void *_class, void *_self, va_list *app)
{
	const struct CLASS *super_class = super(_class);

	assert(_self && super_class->ctor);
	return super_class->ctor(_self, app);
}

/**
 * @brief 析构函数选择器.
 *
 * 析构函数选择器.
 * @param [in]_self 对象指针
 * @return 对象指针
 */
void *dtor(void *_self)
{
	const struct CLASS *class = class_of(_self);

	assert(class->dtor);
	return class->dtor(_self);
}

/**
 * @brief 基类析构函数选择器.
 *
 * 基类析构函数选择器.
 * @param [in]_class 类类型描述指针
 * @param [in]_self 对象指针
 * @return 对象指针
 */
void *super_dtor(const void *_class, void *_self)
{
	const struct CLASS *super_class = super(_class);

	assert(_self && super_class->dtor);
	return super_class->dtor(_self);
}
