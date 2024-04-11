# Multithreaded Queue

## Simple Push Pop Operation

__Queue.c__:
This is the heart of the operation. This file holds 4 files which create a synchronous bounded buffer. The functions are queue.new, queuedelete, queuepush, queuepop. 

- __queue_new__ is a function which creates a new queue. The objects for this function are defined in a struct.
-The objects defined are 3 integers named head, tail, size. 
-3 semaphores which define how empty and how full a buffer is as well as one that acts like a mutex respectiveley. 
-An array of void *
-queue_new initializes all the parts and I malloc sizeof(void*) * size of the buffer. 
- __queue_delete__: is a function which destroys all the semaphores that are used and frees the memory from the heap which was allocated to make the buffer array of void *. 
- __queue_push__: is a function which pushes the elements into the list/array. 
-if multithreaded, it will call on semempty which checks if the buffer is full, if it is full, it will wait until semempty is less than size of the buffer.
-it will call on the mutex and lock away all other threads from accessing the criticial section
-it will add the items onto the buffer like a list
-it will unlock and signal fullsem by telling signaling it has one more item in the buffer
-If an invalid pointer is passed, it will return false. It a NULL item is passed to be pushed onto the buffer, it will return True.
- __queue_pop__: is a function which pops elemnts from the queue. It acts exactly the same way as Push but in reverse. 
-it will call on fullsem which checks if the buffer is empty, if it is, it will wait until fullsem is not empty and has items in the buffer. 
-it will lock the item by calling in the mutex.
-it updates the critical section
-it unlocks the mutex
-it signals emptysem that there is one less item in the buffer. 
-If an invalid pointer is passed in, it will return False. If an invalid element is passed in via **elem, it will also return false and the buffer will not proceed and stay at the same index. 

__NOTES__: I used the same methods to test my own tests the way the tests were given to us in the resources folder. Using the same make file and making a testing directory which is deleted after the bashscripts run. All tests that I made should pass returning a 0.