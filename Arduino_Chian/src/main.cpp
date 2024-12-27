// #include "M5Unified.h"
// #include "Chain.h"

// Chain chain;
// device_list_t *devices_list;
// uint8_t dev_nums = 0;

// uint16_t tof_distance = 0;
// uint16_t device_nums  = 0;
// bool operation_status;
// uint8_t sta = 0;

// #define TXD_PIN GPIO_NUM_21  // 串口发送端口(Tx)
// #define RXD_PIN GPIO_NUM_22  // 串口接收端口(Rx)

// /// 定义屏幕尺寸
// #define SCREEN_WIDTH  320
// #define SCREEN_HEIGHT 240

// // 圆心坐标和半径
// #define CIRCLE_X      (SCREEN_WIDTH / 2)
// #define CIRCLE_Y      (SCREEN_HEIGHT / 2)
// #define CIRCLE_RADIUS 115

// // 小球半径
// #define BALL_RADIUS 10

// // 小球的初始位置
// int ballX = CIRCLE_X;
// int ballY = CIRCLE_Y;

// // 上一帧小球的位置
// int lastBallX = ballX;
// int lastBallY = ballY;

// const int startX         = 5;                         // 表格左上角 x 坐标
// const int startY         = 40;                        // 表格左上角 y 坐标
// const int endX           = 315;                       // 表格右下角 x 坐标
// const int endY           = 220;                       // 表格右下角 y 坐标
// const int rows           = 10;                        // 行数
// const int cols           = 6;                         // 列数
// const int colWidths[6]   = {30, 80, 60, 60, 40, 40};  // 列宽数组
// uint16_t headerColors[6] = {
//     M5.Display.color565(0, 0, 0),  // 第一列颜色（红色）
//     M5.Display.color565(0, 0, 0),  // 第二列颜色（绿色）
//     M5.Display.color565(0, 0, 0),  // 第三列颜色（蓝色）
//     M5.Display.color565(0, 0, 0),  // 第四列颜色（黄色）
//     M5.Display.color565(0, 0, 0),  // 第五列颜色（橙色）
//     M5.Display.color565(0, 0, 0)   // 第五列颜色（橙色）
// };
// uint16_t bgColor            = M5.Display.color565(0, 0, 0);  // 表格背景颜色（黑色）
// uint16_t defaultHeaderColor = M5.Display.color565(0, 0, 0);

// rgb_color_t rgb = {0x00, 0x00, 0x00};

// void viewMenu(void);
// void viewIndexUpadte(void);
// void viewEncoderTest(uint8_t id);
// void viewKeyTest(uint8_t id);
// void viewAngleTest(uint8_t id);
// void viewJoystickTest(uint8_t id);
// void viewTofTest(uint8_t id);
// void viewUartTest(uint8_t id);
// void setCellColor(int row, int col, uint16_t color);
// void drawTable(void);
// void addDataToCell(int row, int col, String data, uint16_t color);
// void addSwitchToCell(int row, int col, bool isOn);
// void clearTableCell(int row, int col);
// void fillTable(const device_list_t *devices);
// void printDeviceList(device_list_t *devices);
// long myMap(long x, long in_min, long in_max, long out_min, long out_max);
// void changeRGB(int16_t encodeIncValue, uint8_t rgb_change_status, rgb_color_t *rgb);
// void drawSwitch(int x, int y, uint8_t switchState);
// void drawCenteredText(const char *text, int x, int y, uint16_t color);
// void drawProgressBar(int adcValue, int x, int y, int width, int height, int radius, const char *text);
// void drawCircleAndBall();
// void updateBallPosition(int joyX, int joyY);
// void moveBallToCenter();

// void displayHeartBasedOnDistance(uint16_t distance);
// void drawFilledHeart(int x, int y, float size);

// bool checkConnection(void);

// uint8_t test_index = 0;
// chain_status_t operationStatus;

// void setup()
// {
//     M5.begin();
//     Serial.begin(115200);
//     chain.begin(&Serial2, 115200, RXD_PIN, TXD_PIN);

//     if (chain.isDeviceConnected()) {
//         operation_status = chain.getDeviceNum(&device_nums);
//         if (operation_status == CHAIN_OK) {
//             devices_list          = (device_list_t *)malloc(sizeof(device_list_t));
//             devices_list->count   = device_nums;
//             devices_list->devices = (device_info_t *)malloc(sizeof(device_info_t) * device_nums);  // 分配内存
//             operation_status      = chain.getDeviceList(devices_list);
//             Serial.print(operation_status);
//             if (operation_status) {
//                 printDeviceList(devices_list);
//             }
//         } else {
//             Serial.printf("error status:%d \r\n", operation_status);
//             Serial.printf("数据读取失败。。。。\r\n");
//         }
//     }
//     // 关闭所有RGB值
//     for (uint8_t i = 0; i < devices_list->count; i++) {
//         chain.setRGBValue(devices_list->devices[i].id, {0x00, 0x00, 0x00}, &sta);
//     }
//     viewMenu();

//     //  M5.Display.setTextFont(&fonts::efontCN_24);
//     //  M5.Display.printf("  enum device test 10ms");
// }

// bool encodeButtonStatus   = false;
// bool keyButtonStatus      = false;
// bool joystickButtonStatus = false;

// int16_t encodeIncValue = 0;
// int16_t encodeValue    = 0;

// uint16_t angleAdc12 = 0;
// uint16_t adcTemp16  = 0;

// int16_t joyStickX16 = 0;
// int16_t joyStickY16 = 0;

// uint16_t tofDistance = 0;
// uint16_t tofTemp     = 0;

// int16_t temp  = 0;
// int16_t temp1 = 0;

// uint8_t buf[6]   = {0};
// uint16_t uartAdc = 0;

// uint32_t ans1 = 0;
// uint32_t ans2 = 0;
// uint8_t num   = 0;

// uint8_t update_nums = 0;
// void loop()
// {
//     M5.update();
//     if (chain.isDeviceConnected()) {
//         if (M5.BtnC.wasReleased() || M5.BtnC.pressedFor(500)) {
//             viewIndexUpadte();
//         }
//         for (uint8_t i = 0; i < devices_list->count; i++) {
//             if (devices_list->devices[i].device_type == chain.getToFTypeCode()) {
//                 operationStatus = chain.getTofDistance(devices_list->devices[i].id, &tofTemp);
//                 if (operationStatus == CHAIN_OK) {
//                     tofDistance = tofTemp;
//                     ans2++;
//                 } else {
//                     ans1++;
//                     Serial.printf("TOF ERROR\r\n");
//                 }
//                 // Serial.printf("distance : %d\n", tofDistance);
//             } else if (devices_list->devices[i].device_type == chain.getEncoderTypeCode()) {
//                 operationStatus = chain.getEncoderValue(devices_list->devices[i].id, &temp, 30);
//                 if (operationStatus == CHAIN_OK) {
//                     encodeValue = temp;
//                     ans2++;
//                 } else {
//                     Serial.printf("ENCODE ERROR\r\n");
//                     ans1++;
//                 }
//                 operationStatus = chain.getEncoderIncValue(devices_list->devices[i].id, &temp, 30);
//                 if (operationStatus == CHAIN_OK) {
//                     encodeIncValue = temp;
//                     ans2++;
//                 } else {
//                     Serial.printf("ENCODE INC ERROR\r\n");
//                     ans1++;
//                 }
//                 if (chain.getEncoderButtonPressStatus(devices_list->devices[i].id)) {
//                     encodeButtonStatus = 1 - encodeButtonStatus;
//                 }
//                 // Serial.printf("Incremental Value: %d\n", encodeIncValue);
//                 // Serial.printf("Encoder Value: %d\n", encodeValue);
//             } else if (devices_list->devices[i].device_type == chain.getAngleTypeCode()) {
//                 operationStatus = chain.getAngle12Adc(devices_list->devices[i].id, &adcTemp16, 30);
//                 if (operationStatus == CHAIN_OK) {
//                     angleAdc12 = adcTemp16;
//                     ans2++;
//                 } else {
//                     Serial.printf("ANGLE ERROR\r\n");
//                     ans1++;
//                 }
//                 // Serial.printf("Angle 12Value: %d\n", angleAdc12);
//             } else if (devices_list->devices[i].device_type == chain.getJoystickTypeCode()) {
//                 operationStatus = chain.getJoystickMappedInt16Value(devices_list->devices[i].id, &temp, &temp1, 30);
//                 if (operationStatus == CHAIN_OK) {
//                     joyStickX16 = temp;
//                     joyStickY16 = temp1;
//                     ans2++;
//                 } else {
//                     Serial.printf("JOYSTICK ERROR\r\n");
//                     ans1++;
//                 }
//                 if (chain.getJoystickButtonPressStatus(devices_list->devices[i].id)) {
//                     joystickButtonStatus = 1 - joystickButtonStatus;
//                 }
//                 // Serial.printf("JoyStick16 x: %d  JoyStick16 Y: %d\n", joyStickX16,
//                 //               joyStickY16);
//             } else if (devices_list->devices[i].device_type == chain.getKeyTypeCode()) {
//                 if (chain.getKeyButtonPressStatus(devices_list->devices[i].id)) {
//                     keyButtonStatus = 1 - keyButtonStatus;
//                 }
//             }
//         }

//         for (uint8_t i = 0; i < devices_list->count; i++) {
//             if (devices_list->devices[i].device_type == chain.getToFTypeCode()) {
//                 addDataToCell(devices_list->devices[i].id, 2, String(tofDistance), GREEN);
//                 addDataToCell(devices_list->devices[i].id, 3, "", GREEN);
//                 addDataToCell(devices_list->devices[i].id, 4, "", GREEN);
//             } else if (devices_list->devices[i].device_type == chain.getEncoderTypeCode()) {
//                 addDataToCell(devices_list->devices[i].id, 2, String(encodeValue), GREEN);
//                 addDataToCell(devices_list->devices[i].id, 3, String(encodeIncValue), GREEN);
//                 addSwitchToCell(devices_list->devices[i].id, 4, encodeButtonStatus);
//             } else if (devices_list->devices[i].device_type == chain.getAngleTypeCode()) {
//                 addDataToCell(devices_list->devices[i].id, 2, String(angleAdc12), GREEN);
//                 addDataToCell(devices_list->devices[i].id, 3, "", GREEN);
//                 addDataToCell(devices_list->devices[i].id, 4, "", GREEN);
//             } else if (devices_list->devices[i].device_type == chain.getJoystickTypeCode()) {
//                 addDataToCell(devices_list->devices[i].id, 2, String(joyStickX16), GREEN);
//                 addDataToCell(devices_list->devices[i].id, 3, String(joyStickY16), GREEN);
//                 addSwitchToCell(devices_list->devices[i].id, 4, joystickButtonStatus);
//             } else if (devices_list->devices[i].device_type == chain.getKeyTypeCode()) {
//                 addDataToCell(devices_list->devices[i].id, 2, "", GREEN);
//                 addDataToCell(devices_list->devices[i].id, 3, "", GREEN);
//                 addSwitchToCell(devices_list->devices[i].id, 4, keyButtonStatus);
//             }
//         }
//         addDataToCell(7, 1, String(ans2), GREEN);
//         addDataToCell(8, 1, String(ans1), RED);
//     }
//     delay(10);
// }

// // 设置单元格颜色
// void setCellColor(int row, int col, uint16_t color)
// {
//     if (row < 0 || row >= rows || col < 0 || col >= cols) return;  // 检查有效性

//     int cellHeight = (endY - startY) / rows;  // 单元格高度
//     int xPos       = startX;

//     // 计算单元格的x坐标
//     for (int j = 0; j < col; j++) {
//         xPos += colWidths[j];
//     }

//     // 填充单元格颜色
//     M5.Display.fillRect(xPos + 1, startY + row * cellHeight + 1, colWidths[col] - 2, cellHeight - 2, color);
// }

// void drawTable(void)
// {
//     int cellHeight = (endY - startY) / rows;

//     // // 绘制表格背景
//     M5.Display.fillRect(startX, startY, endX - startX, endY - startY, bgColor);

//     // 绘制水平线
//     for (int i = 0; i <= rows; i++) {
//         M5.Display.drawLine(startX, startY + i * cellHeight, endX, startY + i * cellHeight, WHITE);
//     }

//     // 绘制垂直线
//     int currentX = startX;
//     for (int j = 0; j <= cols; j++) {
//         M5.Display.drawLine(currentX, startY, currentX, endY, WHITE);
//         currentX += (j < cols) ? colWidths[j] : 0;  // 更新x坐标
//     }

//     // 设置第一行颜色（可以自定义），其他行使用每列的颜色
//     currentX = startX;
//     for (int col = 0; col < cols; col++) {
//         M5.Display.fillRect(currentX + 1, startY + 1, colWidths[col] - 2, cellHeight - 2,
//                             defaultHeaderColor);  // 第一行的颜色
//         currentX += colWidths[col];
//     }

//     // 绘制其他行
//     for (int row = 1; row < rows; row++) {  // 从第二行开始
//         currentX = startX;
//         for (int col = 0; col < cols; col++) {
//             M5.Display.fillRect(currentX + 1, startY + row * cellHeight + 1, colWidths[col] - 2, cellHeight - 2,
//                                 headerColors[col]);  // 其他行的颜色
//             currentX += colWidths[col];
//         }
//     }
//     M5.Display.setTextFont(&fonts::efontCN_14);
//     addDataToCell(0, 0, "ID", (uint16_t)M5.Display.color565(0, 211, 211));
//     addDataToCell(0, 1, "   TYPE", (uint16_t)M5.Display.color565(0, 211, 211));
//     addDataToCell(0, 2, " VALUE", (uint16_t)M5.Display.color565(0, 211, 211));
//     addDataToCell(0, 3, " VALUE", (uint16_t)M5.Display.color565(0, 211, 211));
//     addDataToCell(0, 4, "BTON", (uint16_t)M5.Display.color565(0, 211, 211));
//     addDataToCell(0, 5, "TEST", (uint16_t)M5.Display.color565(0, 211, 211));
// }

// void addDataToCell(int row, int col, String data, uint16_t color)
// {
//     clearTableCell(row, col);
//     if (row < 0 || row >= rows || col < 0 || col >= cols) return;  // 检查有效性

//     int cellHeight = (endY - startY) / rows;  // 单元格高度

//     // 计算单元格的x坐标
//     int xPos = startX;
//     for (int j = 0; j < col; j++) {
//         xPos += colWidths[j];
//     }

//     // 在指定单元格中插入数据
//     M5.Display.setTextSize(1.1);
//     M5.Display.setCursor(xPos + 6, startY + row * cellHeight + 1);
//     M5.Display.setTextColor(color);  // 使用传入的颜色参数
//     M5.Display.print(data);
// }

// void addSwitchToCell(int row, int col, bool isOn)
// {
//     clearTableCell(row, col);
//     if (row < 0 || row >= rows || col < 0 || col >= cols) return;  // 检查有效性

//     int cellHeight = (endY - startY) / rows;  // 单元格高度

//     // 计算单元格的x坐标
//     int xPos = startX;
//     for (int j = 0; j < col; j++) {
//         xPos += colWidths[j];
//     }

//     // 设置开关的宽度和高度
//     int switchWidth  = colWidths[col] / 2;  // 开关宽度占单元格的一半
//     int switchHeight = cellHeight / 2;      // 开关高度占单元格的一半

//     // 开关的x和y坐标
//     int switchX = xPos + (colWidths[col] - switchWidth) / 2;
//     int switchY = startY + row * cellHeight + (cellHeight - switchHeight) / 2;

//     if (isOn) {
//         // 显示“开”状态的图案
//         M5.Display.fillRoundRect(switchX - 4, switchY - 2, switchWidth + 8, switchHeight + 5, 5,
//                                  GREEN);  // 绿色开关
//         M5.Display.setTextColor(WHITE);
//         M5.Display.setCursor(xPos + 11, startY + row * cellHeight);
//         M5.Display.setTextSize(1.1);
//         M5.Display.print("ON");
//     } else {
//         // 显示“关”状态的图案
//         M5.Display.fillRoundRect(switchX - 4, switchY - 2, switchWidth + 8, switchHeight + 5, 5, RED);  // 红色开关
//         M5.Display.setTextColor(WHITE);
//         M5.Display.setCursor(xPos + 9, startY + row * cellHeight);
//         M5.Display.setTextSize(1.1);
//         M5.Display.print("OFF");
//     }
// }
// void clearTableCell(int row, int col)
// {
//     if (row < 0 || row >= rows || col < 0 || col >= cols) return;  // 检查有效性

//     int cellHeight = (endY - startY) / rows;  // 单元格高度

//     // 计算单元格的x坐标
//     int xPos = startX;
//     for (int j = 0; j < col; j++) {
//         xPos += colWidths[j];
//     }

//     // 清除单元格
//     M5.Display.fillRect(xPos + 1, startY + row * cellHeight + 1, colWidths[col] - 2, cellHeight - 2, bgColor);
// }

// const char *deviceTypeToString(uint8_t type)
// {
//     switch (type) {
//         case 1:
//             return "ENCODER";
//         case 2:
//             return "ANGLE";
//         case 3:
//             return "KEY";
//         case 4:
//             return "JOYSTICK";
//         case 5:
//             return "TOF";
//         case 6:
//             return "UART";
//         default:
//             return "UNKNOWN";
//     }
// }

// void fillTable(const device_list_t *devices)
// {
//     M5.Display.setTextFont(&fonts::efontCN_14);
//     for (int i = 0; i < devices_list->count; i++) {
//         const device_info_t &dev = devices_list->devices[i];  // 引用简化访问
//         addDataToCell(i + 1, 0, String(dev.id), (uint16_t)M5.Display.color565(0, 211, 211));
//         addDataToCell(i + 1, 1, deviceTypeToString(dev.device_type),
//                       (uint16_t)M5.Display.color565(0, 211, 211));  // 使用转换函数
//     }
// }

// void printDeviceList(device_list_t *devices)
// {
//     if (devices == NULL) {
//         Serial.println("设备列表为空。");
//         return;
//     }

//     Serial.print("设备数量: ");
//     Serial.println(devices_list->count);

//     for (uint8_t i = 0; i < devices_list->count; i++) {
//         Serial.print("设备 ID: ");
//         Serial.println(devices_list->devices[i].id);
//         Serial.print("设备类型: ");
//         Serial.println(devices_list->devices[i].device_type);
//         Serial.println();
//     }
// }

// long myMap(long x, long in_min, long in_max, long out_min, long out_max)
// {
//     long result;

//     // Perform the mapping calculation
//     result = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

//     // Clamp the result to ensure it's within the output range
//     if (result < out_min) {
//         result = out_min;  // Set to out_min if below range
//     } else if (result > out_max) {
//         result = out_max;  // Set to out_max if above range
//     }

//     return result;  // Return the mapped value
// }
// void viewMenu(void)
// {
//     printDeviceList(devices_list);
//     M5.Display.fillScreen(BLACK);
//     M5.Display.setTextSize(1);
//     M5.Display.setTextFont(&fonts::efontCN_24);
//     // 显示设备表格
//     M5.Display.setCursor(26, 10);
//     M5.Display.setTextColor(WHITE);
//     M5.Display.printf("CHAIN DEVICE NUMS IS %d", dev_nums);
//     drawTable();  // 绘制表格
//     fillTable(devices_list);
//     addDataToCell(test_index + 1, 5, "<<<", RED);
//     M5.Display.setTextFont(&fonts::efontCN_16);
//     M5.Display.setCursor(20, 220);
//     M5.Display.setTextColor(WHITE);
//     M5.Display.printf("   back       comfirm      next");
// }

// void viewIndexUpadte(void)
// {
//     M5.Speaker.tone(990, 200);
//     clearTableCell(test_index + 1, 5);
//     test_index++;
//     test_index = test_index % devices_list->count;
//     addDataToCell(test_index + 1, 5, "<<<", RED);
// }

// // 函数：根据给定位置绘制开关
// void drawSwitch(int x, int y, uint8_t switchState)
// {
//     // 清除开关之前的图形
//     M5.Display.fillRect(x, y, 60, 30, BLACK);  // 先清空区域

//     // 根据开关状态绘制
//     if (switchState) {
//         // 开关打开状态
//         M5.Display.fillRoundRect(x, y, 60, 30, 15, GREEN);  // 绘制绿色的开关
//         drawCenteredText("ON", x + 30, y + 15, BLACK);      // 在开关中间绘制 "ON"
//     } else {
//         // 开关关闭状态
//         M5.Display.fillRoundRect(x, y, 60, 30, 15, RED);  // 绘制红色的开关
//         drawCenteredText("OFF", x + 30, y + 15, WHITE);   // 在开关中间绘制 "OFF"
//     }
// }

// // 函数：在指定位置绘制居中文本
// void drawCenteredText(const char *text, int x, int y, uint16_t color)
// {
//     M5.Display.setTextColor(color);
//     M5.Display.setTextFont(&fonts::efontCN_24);

//     int16_t textWidth  = M5.Display.textWidth(text);  // 获取文本宽度
//     int16_t textHeight = M5.Display.fontHeight();     // 获取文本高度

//     // 计算文本的中心位置
//     int16_t textX = x - (textWidth / 2);
//     int16_t textY = y - (textHeight / 2);

//     // 设置光标位置并绘制文本
//     M5.Display.setCursor(textX, textY);
//     M5.Display.print(text);
// }

// // 绘制带圆角的进度条函数，正中间显示字符串
// void drawProgressBar(int adcValue, int x, int y, int width, int height, int radius, const char *text)
// {
//     // 将 ADC 值映射到进度条宽度范围
//     int progressWidth = map(adcValue, 0, 4095, 0, width);

//     // 清除进度条区域
//     M5.Display.fillRoundRect(x, y, width, height, radius, BLACK);

//     // 绘制进度条边框（带圆角）
//     M5.Display.drawRoundRect(x, y, width, height, radius, WHITE);

//     // 绘制带圆角的进度条填充部分
//     M5.Display.fillRoundRect(x, y, progressWidth, height, radius, GREEN);

//     // 设置文本的颜色和大小
//     M5.Display.setTextColor(RED);

//     // 计算文本的位置，使其在进度条中间
//     int textWidth = M5.Display.textWidth(text);   // 获取文本宽度
//     int textX     = x + (width - textWidth) / 2;  // 文本水平居中
//     int textY     = y + (height - 16) / 2;        // 文本垂直居中（假设字符高度约为16像素）

//     // 绘制字符串
//     M5.Display.setCursor(textX, textY);
//     M5.Display.print(text);
// }

// // 绘制圆和小球
// void drawCircleAndBall()
// {
//     M5.Lcd.fillCircle(CIRCLE_X, CIRCLE_Y, CIRCLE_RADIUS, DARKGREY);  // 圆形

//     // 清除上一个位置的小球
//     M5.Lcd.fillCircle(lastBallX, lastBallY, BALL_RADIUS, BLACK);

//     // 绘制新的小球位置
//     M5.Lcd.fillCircle(ballX, ballY, BALL_RADIUS, RED);

//     // 更新小球的前一帧位置
//     lastBallX = ballX;
//     lastBallY = ballY;
// }

// // 更新小球位置
// void updateBallPosition(int joyX, int joyY)
// {
//     // 将摇杆值映射到[-1, 1]区间
//     // 反转X轴，确保X为正时小球右移，X为负时小球左移
//     float mappedJoyX = -(float)joyX / 4095.0f;  // 反向映射X轴
//     float mappedJoyY = (float)joyY / 4095.0f;   // 保持Y轴正常映射

//     int moveSpeed = 5;  // 小球移动的速度

//     if (abs(joyX) > 100 || abs(joyY) > 100) {  // 摇杆有效输入
//         ballX += mappedJoyX * moveSpeed;
//         ballY += mappedJoyY * moveSpeed;
//     } else {  // 摇杆静止时，小球向中心靠拢
//         moveBallToCenter();
//     }

//     // 限制小球不能离开圆形范围
//     int dx = ballX - CIRCLE_X;
//     int dy = ballY - CIRCLE_Y;
//     if (dx * dx + dy * dy > CIRCLE_RADIUS * CIRCLE_RADIUS) {
//         // 计算小球与圆心的方向
//         float angle = atan2(dy, dx);
//         ballX       = CIRCLE_X + CIRCLE_RADIUS * cos(angle);
//         ballY       = CIRCLE_Y + CIRCLE_RADIUS * sin(angle);
//     }
// }

// // 小球向圆心靠拢
// void moveBallToCenter()
// {
//     int moveSpeed = 10;
//     if (ballX < CIRCLE_X) ballX += moveSpeed;
//     if (ballX > CIRCLE_X) ballX -= moveSpeed;
//     if (ballY < CIRCLE_Y) ballY += moveSpeed;
//     if (ballY > CIRCLE_Y) ballY -= moveSpeed;
// }

// // 函数定义：根据距离显示爱心图案
// void displayHeartBasedOnDistance(uint16_t distance)
// {
//     // 将距离映射到图案大小，使用浮点数进行映射
//     float maxSize = 6.0;  // 最大图案大小
//     float minSize = 1.0;  // 最小图案大小

//     float heartSize = map(distance, 0, 1000, minSize * 100, maxSize * 100) / 100.0;  // 根据距离调整图案大小
//     if (distance >= 1000) {
//         heartSize = 6;
//     }

//     // 清空屏幕
//     M5.Display.fillRect(0, 30, 320, 192, BLACK);

//     // 计算心形的中心位置
//     int centerX = M5.Display.width() / 2;
//     int centerY = M5.Display.height() / 2;

//     // 画一个实心爱心
//     drawFilledHeart(centerX, centerY, heartSize);
// }

// // 辅助函数：在给定位置绘制实心爱心图案
// void drawFilledHeart(int x, int y, float size)
// {
//     // 填充心形的上半部分和下半部分
//     for (float t = 0; t < 2 * PI; t += 0.01) {
//         int heartX = size * (16 * pow(sin(t), 3));
//         int heartY = size * (-13 * cos(t) + 5 * cos(2 * t) + 2 * cos(3 * t) + cos(4 * t));

//         // 确保坐标在屏幕范围内
//         if (x + heartX >= 0 && x + heartX < 320 && y + heartY >= 0 && y + heartY < 240) {
//             M5.Display.drawPixel(x + heartX, y + heartY, RED);  // 绘制心形的边界
//         }
//     }
// }

// void viewTofTest(uint8_t id)
// {
//     M5.Speaker.tone(990, 200);
//     M5.Display.fillScreen(BLACK);
//     uint16_t tofDistance = 0;
//     M5.Display.setTextFont(&fonts::efontCN_16);
//     M5.Display.setTextColor(WHITE);
//     M5.Display.setCursor(95, 5);
//     M5.Display.print("Distance: ");
//     M5.Display.print("    mm");
//     M5.Display.setTextFont(&fonts::efontCN_16);
//     M5.Display.setCursor(20, 220);
//     M5.Display.setTextColor(WHITE);
//     M5.Display.printf("   back       comfirm      next");
//     while (1) {
//         M5.update();
//         if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(500)) {
//             M5.Speaker.tone(990, 200);
//             viewMenu();
//             return;
//         }
//         while (!chain.getTofDistance(id, &tofDistance)) {
//             delay(1);
//         }
//         M5.Display.fillRect(170, 5, 35, 20, BLACK);
//         M5.Display.setTextColor(GREEN);
//         M5.Display.setCursor(170, 5);
//         M5.Display.print(tofDistance);

//         displayHeartBasedOnDistance(tofDistance);

//         delay(100);  // 控制帧率
//     }
// }

// void viewUartTest(uint8_t id)
// {
//     M5.Speaker.tone(990, 200);
//     uint8_t buf[6]   = {0};
//     uint16_t uartAdc = 0;
//     bool operationStatus;
//     M5.Display.fillScreen(BLACK);
//     M5.Display.setTextFont(&fonts::efontCN_24);
//     // 显示设备表格
//     M5.Display.setCursor(45, 10);
//     M5.Display.setTextColor(WHITE);
//     M5.Display.setTextSize(1);
//     M5.Display.printf("CHAIN UART ID IS %d", id);

//     M5.Display.setTextFont(&fonts::efontCN_16);

//     drawProgressBar(uartAdc, 20, 120, 280, 20, 10, "uart adc value");
//     M5.Display.setTextFont(&fonts::efontCN_16);
//     M5.Display.setCursor(20, 220);
//     M5.Display.setTextColor(WHITE);
//     M5.Display.printf("   back       comfirm      next");
//     while (1) {
//         M5.update();
//         uint16_t lastAdc12 = uartAdc;
//         if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(500)) {
//             M5.Speaker.tone(990, 200);
//             viewMenu();
//             return;
//         }
//         uint8_t status  = 0;
//         operationStatus = chain.chainUartAdcInit(id, 0x01, 0x00, &status);
//         if (operationStatus && status) {
//             uint8_t adcNums = 0;
//             operationStatus = chain.getAdcValue(id, &adcNums, buf, &status);
//             if (operationStatus && status) {
//                 uartAdc = (buf[1] << 8) | buf[2];
//                 M5.Display.fillRect(180, 40, 230, 60, BLACK);
//                 M5.Display.setTextFont(&fonts::efontCN_16);
//                 M5.Display.setTextColor(GREEN);
//                 M5.Display.setCursor(60, 60);
//                 M5.Display.printf("uart 12adc Value: %d\n", uartAdc);
//                 if (lastAdc12 > (uartAdc + 3) || lastAdc12 < (uartAdc - 3)) {
//                     drawProgressBar(uartAdc, 20, 120, 280, 20, 10, "uart adc value");
//                 }
//             }
//         }
//         delay(100);
//     }
// }

// bool checkConnection(void)
// {
//     int attempts = 0;      // 尝试次数计数器
//     bool result  = false;  // 存储检查结果

//     while (attempts < 3) {
//         result = chain.isDeviceConnected();  // 调用检查函数
//         if (result) {                        // 如果成功连接，返回 true
//             return true;
//         }
//         attempts++;  // 增加尝试次数
//     }

//     return false;  // 如果三次都没有成功，返回 false
// }