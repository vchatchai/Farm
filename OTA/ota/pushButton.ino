#include <Bounce2.h>

Bounce debouncer = Bounce(); // สร้าง debouncer object
void pushButtonSetup() { 

  debouncer.attach(BUTTON_PIN, INPUT_PULLUP);
  debouncer.interval(25); // กำหนดเวลาการเปลี่ยนสถานะให้กับ debouncer object ที่ 25 มิลลิวินาที

}



void pushButtonLoop() {
  debouncer.update(); // อัปเดตสถานะให้กับ debouncer object

  // กำหนดเงื่อนไขให้โค้ดโปรแกรมในวงเล็บปีกกาทำงานเมื่อสถานะปุ่มกดเปลี่ยนจาก HIGH เป็น LOW โดยเช็คจากฟังก์ชั่น fell()
  // หากต้องการเช็คสถานะจาก LOW เป็น HIGH ให้แทนที่ฟังก์ชั่น fell() ด้วยฟังก์ชั่น rose()
  if ( debouncer.fell() ) {
    ledState = !ledState; // สลับสถานะติด/ดับของ LED
    mqtt_client.publish((valve_topic).c_str(), String(ledState).c_str(), true);
  }
}