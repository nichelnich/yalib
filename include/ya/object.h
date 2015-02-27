/*
 * Copyright (C), 2010-2013, ATCOM Tech. Co., Ltd.
 * File Name: object.h
 * Author: hyh
 * Email: hyf@atcom.com.cn 
 * Created: Sun 12 May 2013 07:32:57 AM CST
 */

/**
 * @file object.h
 * @brief OO的基础.
 *
 * 用C实现OO的基础
 */

#ifndef OBJECT_H
#define OBJECT_H

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * 获取object抽象数据类型\n
 *
 * new(object());
 */
const void *const object(void);

void *new(const void *class, ...);
void delete(void *self);

const void *class_of(const void *self);
size_t size_of(const void *self);

void *ctor(void *self, va_list *app);
void *dtor(void *self);

/**
 * 获取class抽象数据类型\n
 *
 * new(class(), "name", super, size, sel, method, ...0);
 */
const void *const class(void);

/** 获取class的基类 */
const void *super(const void *self);

#endif /* ifndef OBJECT_H */
