QueueHandle_t Q1;
QueueHandle_t Q2;
QueueHandle_t Q3;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(112500);
  delay(1000);
  Q1 = xQueueCreate(1, 4);
  Q2 = xQueueCreate(1, 4);
  Q3 = xQueueCreate(1, 4);
  xTaskCreate(task1, "Task1", 10000, NULL, 1, NULL);
  xTaskCreate(task2, "Task2", 10000, NULL, 2, NULL);
  xTaskCreate(task3, "Task3", 10000, NULL, 3, NULL);
  uint32_t count = 1;
  xQueueSend(Q1, &count, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
}

void task1( void * parameter ){
  for(;;){
    uint32_t count;
    xQueueReceive(Q1, &count, portMAX_DELAY);
    count++;
    Serial.println("Hello from task 1:");
    Serial.println(count);
    delay(1000);
    xQueueSend(Q2, &count, portMAX_DELAY);
  }
}

void task2( void * parameter ){
  for(;;){
    uint32_t count;
    xQueueReceive(Q2, &count, portMAX_DELAY);
    count++;
    Serial.println("Hello from task 2:");
    Serial.println(count);
    delay(1000);
    xQueueSend(Q3, &count, portMAX_DELAY);
  }
}

void task3( void * parameter ){
  for(;;){
    uint32_t count;
    xQueueReceive(Q3, &count, portMAX_DELAY);
    count++;
    Serial.println("Hello from task 3:");
    Serial.println(count);
    delay(1000);
    xQueueSend(Q1, &count, portMAX_DELAY);
  }
}
