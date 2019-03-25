/*
 * list.c
 *
 *  Created on: Mar 19, 2019
 *      Author: mdellavi
 */

/* Includes */
#include "list.h"
#include <stdlib.h>

/*-----------------------------------------------------------*/

void list_add(TaskList * list, TaskHandle_t TaskHandle, TickType_t Deadline)
{

	TaskList * temp = list;
	TaskList * node = (TaskList*)pvPortMalloc(sizeof(TaskList));
	node->Handle = TaskHandle;
	node->Deadline = Deadline;
	node->Name = pcTaskGetName(TaskHandle);

	// Case: empty
	if (temp->Handle == NULL)
	{
		vPortFree((void*)node);
		list->Deadline = Deadline;
		list->Handle = TaskHandle;
		list->Name = pcTaskGetName(TaskHandle);
		list->Next = NULL;
	}
	// Case: new item has earliest deadline
	else if (temp->Deadline > Deadline)
	{
		node->Next = list;
		list = node;
	}
	else while (temp)
	{
		//Case: General
		if (temp->Next && (temp->Next->Deadline > Deadline))
		{
			node->Next = temp->Next;
			temp->Next = node;
			temp = NULL;
			break;
		}
		// Case: new item goes on back of the list
		else if (temp->Next == NULL)
		{
			temp->Next = node;
			node->Next = NULL;
			temp = NULL;
			break;
		}
		// Increment otherwise
		temp = temp->Next;
	}
	return;
}
void list_remove(TaskList * list, TaskHandle_t TaskHandle)
{
	TaskList * temp = list;

	if(list == NULL)
	{
		while(1){;} //empty list access
	}
	//Case: remove first item
	if((temp->Handle == TaskHandle) && temp->Next)
	{
		TaskNode * temp2 = temp->Next;
		*list = *temp->Next;
		vPortFree((void*)temp2);

	}
	//Case: remove only item in the list, and reset thelist
	else if (temp->Handle == TaskHandle)
	{

		//vPortFree((void*)temp);
		list->Next = 0;
		list->Handle = 0;


	}
	// Case:  General
	else while (temp->Next)
	{
		if (temp->Next->Handle == TaskHandle)
		{
			TaskNode * temp2 = temp->Next;
			temp->Next = temp2->Next;
			vPortFree((void*)temp2);
		}
		temp = temp->Next;
	}
	return;
}

int list_size(TaskList * list)
{
	TaskList * temp = list;
	int size = 0;

	if (temp != NULL) size++;

	while (temp->Next)
	{
		size++;
		temp = temp->Next;
	}

	return size;
}
