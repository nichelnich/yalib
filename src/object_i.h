/*
 * Copyright (C), 2010-2013, ATCOM Tech. Co., Ltd.
 * File Name: object_i.h
 * Author: hyh
 * Email: hyf@atcom.com.cn 
 * Created: Tue 14 May 2013 02:29:10 PM CST
 */

#ifndef OBJECT_I_H
#define OBJECT_I_H

struct OBJECT
{
	const struct CLASS *class; /**< 对象的描述 */
};

struct CLASS
{
	const struct OBJECT _;     /**< 类的描述 */
	const char *name;          /**< 类的名称 */
	const struct CLASS *super; /**< 类的父类 */
	size_t size;               /**< 类的对象的大小 */
	void *(*ctor)(void *self, va_list *app);
	void *(*dtor)(void *self);
};

void *super_ctor(const void *class, void *self, va_list *app);
void *super_dtor(const void *class, void *self);

#endif /* #ifndef OBJECT_I_H */
