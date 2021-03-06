/*
 * Copyright (c) 2016 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef X86_EXCEPTION_H
#define X86_EXCEPTION_H

#ifndef _ASMLANGUAGE

#include <toolchain/common.h>

#define _EXCEPTION_INTLIST(vector) \
	".pushsection .gnu.linkonce.intList.exc_" #vector "\n\t" \
	".long 1f\n\t"				/* ISR_LIST.fnc */ \
	".long -1\n\t"				/* ISR_LIST.irq */ \
	".long -1\n\t"				/* ISR_LIST.priority */ \
	".long " STRINGIFY(vector) "\n\t"	/* ISR_LIST.vec */ \
	".long 0\n\t"				/* ISR_LIST.dpl */ \
	".popsection\n\t" \

/* Extra preprocessor indirection to ensure arguments get expanded before
 * concatenation takes place
 */
#define __EXCEPTION_STUB_NAME(handler, vec) \
	_ ## handler ## _vector_ ## vec ## _stub

#define _EXCEPTION_STUB_NAME(handler, vec) \
	__EXCEPTION_STUB_NAME(handler, vec) \

/* Unfortunately, GCC extended asm doesn't work at toplevel so we need
 * to stringify stuff.
 *
 * What we are doing here is generating entires in the .intList section
 * and also the assembly language stubs for the exception. We use
 * .gnu.linkonce section prefix so that the linker only includes the
 * first one of these it encounters for a particular vector. In this
 * way it's easy for applications or drivers to install custom exception
 * handlers without having to #ifdef out previous instances such as in
 * arch/x86/core/fatal.c
 */
#define __EXCEPTION_CONNECT(handler, vector, codepush) \
	__asm__ ( \
	_EXCEPTION_INTLIST(vector) \
	".pushsection .gnu.linkonce.t.exc_" STRINGIFY(vector) \
		  "_stub, \"ax\"\n\t" \
	".global " STRINGIFY(_EXCEPTION_STUB_NAME(handler, vector)) "\n\t" \
	STRINGIFY(_EXCEPTION_STUB_NAME(handler, vector)) ":\n\t" \
	"1:\n\t" \
	codepush \
	"push $" STRINGIFY(handler) "\n\t" \
	"jmp _exception_enter\n\t" \
	".popsection\n\t" \
	)


/**
 * @brief Connect an exception handler that doesn't expect error code
 *
 * Assign an exception handler to a particular vector in the IDT.
 *
 * @param handler A handler function of the prototype
 *                void handler(const NANO_ESF *esf)
 * @param vector Vector index in the IDT
 */
#define _EXCEPTION_CONNECT_NOCODE(handler, vector) \
	__EXCEPTION_CONNECT(handler, vector, "push $0\n\t")

/**
 * @brief Connect an exception handler that does expect error code
 *
 * Assign an exception handler to a particular vector in the IDT.
 * The error code will be accessible in esf->errorCode
 *
 * @param handler A handler function of the prototype
 *                void handler(const NANO_ESF *esf)
 * @param vector Vector index in the IDT
 */
#define _EXCEPTION_CONNECT_CODE(handler, vector) \
	__EXCEPTION_CONNECT(handler, vector, "")

#endif /* _ASMLANGUAGE */

#endif /* X86_EXCEPTION_H */
