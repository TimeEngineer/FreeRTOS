hw_timer_t * timer = NULL;
const int ledPin1 = 23;
const int ledPin2 = 22;
QueueHandle_t Q1;
int count;

int fib (int n) {
  if (n <2)
    return n;
  else
    return fib(n-1) + fib(n-2);
}

void IRAM_ATTR onTimer() {
  digitalWrite(ledPin1, HIGH);
  count++;
  xQueueSend(Q1, &count, portMAX_DELAY);
  digitalWrite(ledPin1, LOW);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  count = 0;
  Q1 = xQueueCreate(100, 4);
  xTaskCreate(task1, "task1", 1000, NULL, 1, NULL);
  xTaskCreate(task2, "task2", 10000, NULL, 1, NULL);
  
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 100000, true);
  timerAlarmEnable(timer);
  
}

void loop() {
  // put your main code here, to run repeatedly:

}

void task1( void * parameter ){
  for(;;) {
    digitalWrite(ledPin2, !digitalRead(ledPin2));
    vTaskDelay(200);
  }
}

void task2( void * parameter ){
  for(;;) {
     int count;
     xQueueReceive(Q1, &count, portMAX_DELAY);
     int res = fib (count%35);
     Serial.println(String(res));
  }
}
