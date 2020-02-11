SemaphoreHandle_t Sem1;
SemaphoreHandle_t Sem2;
SemaphoreHandle_t Sem3;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(112500);
  delay(1000);
  Sem1 = xSemaphoreCreateBinary();
  Sem2 = xSemaphoreCreateBinary();
  Sem3 = xSemaphoreCreateBinary();
  xTaskCreate(task1, "Task1", 10000, NULL, 1, NULL);
  xTaskCreate(task2, "Task2", 10000, NULL, 2, NULL);
  xTaskCreate(task3, "Task3", 10000, NULL, 3, NULL);
  xSemaphoreGive( Sem1 );
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
}

void task1( void * parameter ){
  for(;;){
    xSemaphoreTake( Sem1, portMAX_DELAY );
    /* Take the mutex to acceed to shared ressource */
    Serial.println("Hello from task 1");
    delay(1000);
    xSemaphoreGive( Sem2 );
  }
}

void task2( void * parameter ){
  for(;;){
    xSemaphoreTake( Sem2, portMAX_DELAY );
    /* Take the mutex to acceed to shared ressource */
    Serial.println("Hello from task 2");
    delay(1000);
    xSemaphoreGive( Sem3 );
  }
}

void task3( void * parameter ){
  for(;;){
    xSemaphoreTake( Sem3, portMAX_DELAY );
    /* Take the mutex to acceed to shared ressource */
    Serial.println("Hello from task 3");
    delay(1000);
    xSemaphoreGive( Sem1 );
  }
}
