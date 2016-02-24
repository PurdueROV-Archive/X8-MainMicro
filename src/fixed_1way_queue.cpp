#include "fixed_1way_queue.h"

// CLASS FUNCTIONS:

// TODO: REPLACE THIS WITH CONTINUAL INTEGRATING CLASS.

Fixed1WayQueue::Fixed1WayQueue(int size)
{
	maxsize = qsize
}

void Fixed1WayQueue::push(int value)
{
	int i = 0;
	if (currentsize < maxsize)
	{
		queue[currentsize] = value;
		currentsize++;
	}
	else
	{
		for (; i < maxsize - 1; i++)
			queue[i] = queue[i + 1];
		queue[maxsize - 1] = value;
	}
}

int Fixed1WayQueue::get(int index)
{
	if (index < currentsize)
		return queue[index];
	else
		return 0;
}

int Fixed1WayQueue::getCurrSize(void)
{
	return currentsize;
}

int Fixed1WayQueue::getMaxSize(void)
{
	return maxsize;
}