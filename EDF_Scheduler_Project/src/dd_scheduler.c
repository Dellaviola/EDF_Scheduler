/*
 * dd_scheduler.c
 *
 *  Created on: Mar 12, 2019
 *      Author: mdellavi
 */


#include "dd_scheduler.h"

/*-----------------------------------------------------------*/



/*-----------------------------------------------------------*/

TaskHandle_t dd_tcreate(Task_param_s param){
	/*
	 * open a queue
	 * create min priority task give task params
	 * create a task message and send to scheduler
	 * wait for reply at q
	 * take reply
	 * destroy q
	 * return null or task handle
	 */
	return NULL;
}

uint32_t dd_delete(TaskHandle_t handle){
	/*
	 * open a queue
	 * delete task handle
	 * create task delete message send to scheduler
	 * wait for reply
	 * take reply
	 * destroy q
	 * return
	 */
	return 0;
}

uint32_t dd_return_active_list(Task_list_s *list){
	/*
	 *	open a queue
	 *	create task list request message
	 *	wait for reply
	 *	take reply
	 *	destroy q
	 *	return
	 */
	return 0;
}

uint32_t dd_return_overdue_list(Task_list_s *list){
	/*
	 *	open a queue
	 *	create task list request message
	 *	wait for reply
	 *	take reply
	 *	destroy q
	 *	return
	 */
	return 0;
}

