/*
 * list.c
 *
 *  Created on: Mar 19, 2019
 *      Author: mdellavi
 */

#include "list.h"
#include <stdlib.h>

void list_init()
{
	ActiveList = malloc(sizeof(TaskList *));
	OverdueList = malloc(sizeof(TaskList *));

	ActiveList->Next = NULL;
	OverdueList->Next = NULL;

}
//	TaskHandle_t TaskHandle;
//	TickType_t Deadline;

void list_add(TaskList * list, TaskHandle_t TaskHandle, TickType_t Deadline)
{

	TaskList * temp = list;
	TaskList * node = malloc(sizeof(TaskList));

	// Case: empty
	if (temp->Next == NULL)
	{
		list->Next = node;

		node->Deadline = Deadline;
		node->Handle = TaskHandle;
		node->Next = NULL;
		node->TaskType = 0;
	} // Case: new item has earliest deadline
	else if (temp->Deadline > Deadline)
	{
		node->Next = list;
		list = node;
	}
	else while (temp->Next)
	{
		if (temp->Next->Deadline > Deadline)
		{
			node->Next = temp->Next;
			temp->Next = node;
			break;
		}
		else
		{
			temp = temp->Next;
		}
	}
}
void list_remove(TaskNode * list)
{

}
