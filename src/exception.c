/**
 * @file exception.c
 * @brief Exception Handler
 * @ingroup exceptions
 */
#include "exception.h"
#include <string.h>

/**
 * @defgroup exceptions Exception Handler
 * @ingroup lowlevel
 * @{
 */

/** @brief Exception handler currently registered with exception system */
static void (*__exception_handler)(exception_t*) = NULL;
/** @brief Base register offset as defined by the interrupt controller */
extern const unsigned char* __baseRegAddr;

/**
 * @brief Register an exception handler to handle exceptions
 *
 * @param[in] cb
 *            Callback function to call when exceptions happen
 */
void register_exception_handler( void (*cb)(exception_t*))
{
	__exception_handler = cb;
}

/**
 * @brief Fetch the string name of the exception
 *
 * @todo Implement exceptionMap , then calculate the offset
 *
 * @param[in] etype
 *            Exception type
 *
 * @return String representation of the exception
 */
static const char* __get_exception_name(uint32_t etype)
{
	static const char* exceptionMap[] =
	{
		"Exception infomap not implemented",
		NULL,
	};

	etype = 0;

	return exceptionMap[(int32_t)etype];
}

/**
 * @brief Fetch relevant registers
 *
 * @param[out] e
 *             Pointer to structure describing the exception
 * @param[in]  type
 *             Exception type.  Either #EXCEPTION_TYPE_CRITICAL or 
 *             #EXCEPTION_TYPE_RESET
 */
static void __fetch_regs(exception_t* e,int32_t type)
{
	e->regs = (volatile const reg_block_t*) __baseRegAddr;
	e->type = type;
	e->info = __get_exception_name((uint32_t)e->regs->gpr[30]);
}

/**
 * @brief Respond to a critical exception
 */
void __onCriticalException()
{
	exception_t e;
	
	if(!__exception_handler) { return; }

	__fetch_regs(&e,EXCEPTION_TYPE_CRITICAL);
	__exception_handler(&e);
}

/**
 * @brief Respond to a reset exception
 */
void __onResetException()
{
	exception_t e;
	
	if(!__exception_handler) { return; }

	__fetch_regs(&e,EXCEPTION_TYPE_RESET);
	__exception_handler(&e);
}

/** @} */
