/*
  Example  8. Changing task priorities  更改任务优先级
  
  FreeRTOS.org V9.0.0 - Copyright (C) 2003-2017 Richard Barry.

  This file is part of the FreeRTOS.org distribution.

  FreeRTOS.org is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  FreeRTOS.org is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with FreeRTOS.org; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  A special exception to the GPL can be applied should you wish to distribute
  a combined work that includes FreeRTOS.org, without being obliged to provide
  the source code for any proprietary components.  See the licensing section
  of http://www.FreeRTOS.org for full details of how and when the exception
  can be applied.

*/

#include "FreeRTOS.h"

/* The two task functions. */
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );

/* Used to hold the handle of Task2. */
TaskHandle_t xTask2Handle;

/*-----------------------------------------------------------*/

void setup( void )
{
#ifdef EFM32GENERIC
/* setup alternate location default is 0# */  
  Serial.setRouteLoc(1); /*set to 1#. the serial port's Alternate LOCATION(see datasheet)*/
#endif
  Serial.begin(115200);  /*set param: 115200bps 8N1 (default 115200bps 8N1) */
  /* Create the first task at priority 2.  This time the task parameter is
  not used and is set to NULL.  The task handle is also not used so likewise
  is also set to NULL. */
  xTaskCreate( vTask1, "Task 1", 200, NULL, 2, NULL );
          /* The task is created at priority 2 ^. */

  /* Create the second task at priority 1 - which is lower than the priority
  given to Task1.  Again the task parameter is not used so is set to NULL -
  BUT this time we want to obtain a handle to the task so pass in the address
  of the xTask2Handle variable. */
  xTaskCreate( vTask2, "Task 2", 200, NULL, 1, &xTask2Handle );
         /* The task handle is the last parameter ^^^^^^^^^^^^^ */

  /* Start the scheduler so our tasks start executing. */
  vTaskStartScheduler();

  for( ;; );

}
/*-----------------------------------------------------------*/

void vTask1( void *pvParameters )
{
/* unsigned */ portBASE_TYPE uxPriority;

  /* This task will always run before Task2 as it has the higher priority.
  Neither Task1 nor Task2 ever block so both will always be in either the
  Running or the Ready state.

  Query the priority at which this task is running - passing in NULL means
  "return our own priority". */
  uxPriority = uxTaskPriorityGet( NULL );

  for( ;; )
  {
    /* Print out the name of this task. */
    vPrintString( "Task1 is running\r\n" );

    /* Setting the Task2 priority above the Task1 priority will cause
    Task2 to immediately start running (as then Task2 will have the higher
    priority of the    two created tasks). */
    vPrintString( "About to raise the Task2 priority\r\n" );
    vTaskPrioritySet( xTask2Handle, ( uxPriority + 1 ) );

    /* Task1 will only run when it has a priority higher than Task2.
    Therefore, for this task to reach this point Task2 must already have
    executed and set its priority back down to 0. */
  }
}

/*-----------------------------------------------------------*/

void vTask2( void *pvParameters )
{
/*unsigned*/ portBASE_TYPE uxPriority;

  /* Task1 will always run before this task as Task1 has the higher priority.
  Neither Task1 nor Task2 ever block so will always be in either the
  Running or the Ready state.

  Query the priority at which this task is running - passing in NULL means
  "return our own priority". */
  uxPriority = uxTaskPriorityGet( NULL );

  for( ;; )
  {
    /* For this task to reach this point Task1 must have already run and
    set the priority of this task higher than its own.

    Print out the name of this task. */
    vPrintString( "Task2 is running\r\n" );

    /* Set our priority back down to its original value.  Passing in NULL
    as the task handle means "change our own priority".  Setting the
    priority below that of Task1 will cause Task1 to immediately start
    running again. */
    vPrintString( "About to lower the Task2 priority\r\n" );
    vTaskPrioritySet( NULL, ( uxPriority - 2 ) );
  }
}

/****************  default idle hook callback if configUSE_IDLE_HOOK ***************************
 * 1  EFM32GENERIC loop() is call by default idle hook if enable(set configUSE_IDLE_HOOK to 1) *
 * 2  Idle loop has a very small stack (check or set configMINIMAL_STACK_SIZE)                 * 
 * 3  Loop must never block.                                                                   * 
 * 4  This default idle hook can be overload by vApplicationIdleHook()                         * 
 ***********************************************************************************************/
void loop() {
  for(;;){} //This example Not used.
}

