void setup() {
  // put your setup code here, to run once:
  Serial.begin(112500);
  delay(1000);
  // xTaskCreate(taskOne, "TaskOne", 10000, NULL, 1, NULL);
  // xTaskCreate(taskTwo, "TaskTwo", 10000, NULL, 1, NULL);
  // xTaskCreate(taskThree, "TaskThree", 10000, NULL, 2, NULL);
  xTaskCreate(task1, "Task1", 10000, NULL, 1, NULL);
  xTaskCreate(task2, "Task1", 10000, NULL, 4, NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
}

void taskOne( void * parameter )
{
  for( int i = 0;i<10;i++ ){
    Serial.println("Hello from task 1");
    delay(1000);
  }
  Serial.println("Ending task 1");
  vTaskDelete( NULL );
}

void taskTwo( void * parameter)
{
  for( int i = 0;i<10;i++ ){
    Serial.println("Hello from task 2");
    delay(1000);
  }
  Serial.println("Ending task 2");
  vTaskDelete( NULL );
}

void taskThree( void * parameter)
{
  for( int i = 0;i<10;i++ ){
    Serial.println("Hello from task 3");
    delay(1000);
  }
  Serial.println("Ending task 3");
  vTaskDelete( NULL );
}

void task1( void * parameter )
{
  /* loop forever */
  for(;;){
    Serial.println("task1 is running");
    Serial.println("task1 is ending");
  }
}

void task2( void * parameter )
{
  /* loop forever */
  for(;;){
    Serial.println("task2 is running");
    Serial.println("task2 is ending");
  }
}
