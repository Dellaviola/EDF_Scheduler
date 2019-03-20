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
	TaskList * node = malloc(sizeof(TaskList));
	node->Handle = TaskHandle;
	node->Deadline = Deadline;

	// Case: empty
	if (temp->Handle == NULL) //TODO: fixme
	{
		list->Deadline = Deadline;
		list->Handle = TaskHandle;
		list->Next = NULL;
		list->TaskType = 0;

		free(node);
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
		if (temp->Next->Deadline > Deadline)
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
}
void list_remove(TaskList * list, TaskHandle_t TaskHandle)
{
	TaskList * temp = list;

	if(list == NULL)
	{
		while(1){;} //empty list access
	}
	//Case: remove first item
	if(temp->Handle == TaskHandle) //TODO: this doesnt reassign param->active...
	{
		TaskNode * temp2 = temp->Next;
		*list = *temp->Next;
		free(temp2);

	}
	// Case:  General
	else while (temp->Next)
	{
		if (temp->Next->Handle == TaskHandle)
		{
			TaskNode * temp2 = temp->Next;
			temp->Next = temp2->Next;
			free(temp2);
		}
	}
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
