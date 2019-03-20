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

void list_init()
{
	ActiveList = malloc(sizeof(TaskList *));
	OverdueList = malloc(sizeof(TaskList *));

	ActiveList->Next = NULL;
	OverdueList->Next = NULL;

}

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
	}
	// Case: new item has earliest deadline
	else if (temp->Deadline > Deadline)
	{
		node->Next = list;
		list = node;
	}
	else while (temp->Next)
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
	if(temp->Handle == TaskHandle)
	{
		list = temp->Next;
		temp->Next = NULL;
		free(temp);
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
