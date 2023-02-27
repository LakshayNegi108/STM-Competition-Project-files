#include "main.h"
#include "BML_DEF.h"
#include "rcc.h"
#include "gpio.h"
#include "uart.h"
#include "st7783.h"

#define	min(x, y)	x>y?y:x

void testFillScreen();
void testText();
void testLines(uint16_t color);
void testFastLines(uint16_t color1, uint16_t color2);
void testRects(uint16_t color);
void testFilledRects(uint16_t color1, uint16_t color2);
void testFilledCircles(uint8_t radius, uint16_t color);
void testCircles(uint8_t radius, uint16_t color);
void testTriangles();
void testFilledTriangles();
void testRoundRects();
void testFilledRoundRects();
void testCase1();
void testCase2();

void HMS_bgdisplay(void) {

	//===================== | Border |===============================//
	for (uint16_t i = 5; i > 0; i--) {
		LCD_DrawRect(i, i, TFTWIDTH - 2 * i, TFTHEIGHT - 2 * i, WHITE);
	}

	LCD_DrawHBorder(0, TFTHEIGHT / 3, TFTWIDTH, WHITE, 5);
	LCD_DrawHBorder(0, 2 * (TFTHEIGHT / 3), TFTWIDTH, WHITE, 5);
	//===============================================================//

	//===================== | Box 1 |===============================//

	LCD_DrawButton1(" ", 20, 20, TFTWIDTH - 40, (TFTHEIGHT / 3) - 40, 10, CYAN,
	WHITE, 0, 2, 2, 2);

	LCD_SetTextSize(2);
	LCD_SetTextColor(WHITE, CYAN);
	LCD_SetCursor(((TFTWIDTH - 20) / 3) + 15, 25);
	LCD_Printf("Health");
	LCD_SetCursor(((TFTWIDTH - 20) / 3) - 10, 45);
	LCD_Printf("Monitoring");
	LCD_SetCursor(((TFTWIDTH - 20) / 3) + 15, 65);
	LCD_Printf("System");

	//===============================================================//

	//===================== | Box 2 |===============================//

	LCD_FillRoundRect(15, (TFTHEIGHT / 3) + 15, TFTWIDTH - 35, 25, 5, RED);
	LCD_SetTextColor(WHITE, RED);
	LCD_SetCursor(20, (TFTHEIGHT / 3) + 20);
	LCD_SetTextSize(2);
	LCD_Printf("H.Rate: ");

	LCD_FillRoundRect(15, (TFTHEIGHT / 3) + 45, TFTWIDTH - 35, 25, 5, GREEN);
	LCD_SetTextColor(WHITE, GREEN);
	LCD_SetCursor(20, (TFTHEIGHT / 3) + 50);
	LCD_SetTextSize(2);
	LCD_Printf("SpO2: ");

	LCD_FillRoundRect(15, (TFTHEIGHT / 3) + 75, TFTWIDTH - 35, 25, 5, BLUE);
	LCD_SetTextColor(WHITE, BLUE);
	LCD_SetCursor(20, (TFTHEIGHT / 3) + 80);
	LCD_SetTextSize(2);
	LCD_Printf("Temp: ");

	//===============================================================//

	//========================= | Box 3 |============================//

	LCD_FillRect(20, (2 * (TFTHEIGHT / 3)) + 20, TFTWIDTH - 35, 70, WHITE);

	//===============================================================//

}

uint16_t temp2color(int degree, int lo, int hi) {
	uint8_t r, g, b;
	r = map(degree, lo, hi, 255, 0);
	g = 0;
	b = map(degree, lo, hi, 0, 255);
	return LCD_Color565(r, g, b);
}

void colorgradient(int x, int y, int w, int h, int percent) {
//	LCD_DrawRect(x, y, w, h, BLACK);
	for (int row = 1; row < h - 1; row++) {
		LCD_DrawFastHLine(x + 1, y + row, w - 2, temp2color(row, 0, h));
	}
}

int main(void) {
	RCC_CONFIG_84MHZ();
	uart_print_config(9600);
	print("Working\n\r");

//	LCD_Begin();
//	LCD_FillScreen(BLACK);
//
//	testFillScreen();
//	Delay(500);
//	testText();
//	Delay(500);
//	testLines(CYAN);
//	Delay(500);
//	testFastLines(RED, BLUE);
//	Delay(500);
//	testRects(GREEN);
//	Delay(500);
//	testFilledRects(YELLOW, MAGENTA);
//	Delay(500);
//	testFilledCircles(10, MAGENTA);
//	Delay(500);
//	testCircles(10, WHITE);
//	Delay(500);
//	testTriangles();
//	Delay(500);
//	testFilledTriangles();
//	Delay(500);
//	testRoundRects();
//	Delay(500);
//	testFilledRoundRects();
//	Delay(500);

	LCD_Begin();
	LCD_SetRotation(0);
	//	LCD_FillScreen(BLACK);
	colorgradient(0, 0, TFTWIDTH, TFTHEIGHT, 50);

	LCD_SetTextSize(3);

	HMS_bgdisplay();

	LCD_SetTextColor(BLACK, WHITE);
	LCD_SetCursor(190, 240);
	LCD_SetTextSize(2);
	LCD_Printf("HR");

	while (1) {
//		for (uint8_t rotation = 0; rotation < 4; rotation++) {
//			LCD_SetRotation(rotation);
//			testText();
//			Delay(2000);
//		}
	}
	return 0;
}

void testCase1() {
	testFillScreen();
	Delay(500);
	testText();
	Delay(500);
	testLines(CYAN);
	Delay(500);
	testFastLines(RED, BLUE);
	Delay(500);
	testRects(GREEN);
	Delay(500);
	testFilledRects(YELLOW, MAGENTA);
	Delay(500);
	testFilledCircles(10, MAGENTA);
	Delay(500);
	testCircles(10, WHITE);
	Delay(500);
	testTriangles();
	Delay(500);
	testFilledTriangles();
	Delay(500);
	testRoundRects();
	Delay(500);
	testFilledRoundRects();
	Delay(500);
}

void testCase2() {
	for (uint8_t rotation = 0; rotation < 4; rotation++) {
		LCD_SetRotation(rotation);
		testText();
		Delay(2000);
	}
}

void testFillScreen() {
	unsigned long start = HAL_GetTick();
	LCD_FillScreen(BLACK);
	LCD_FillScreen(RED);
	LCD_FillScreen(GREEN);
	LCD_FillScreen(BLUE);
	LCD_FillScreen(BLACK);

	print("testFillScreen: %dms \n\r", HAL_GetTick() - start);
}

void testText() {
	LCD_FillScreen(BLACK);
	unsigned long start = HAL_GetTick();
	LCD_SetCursor(0, 0);
	LCD_SetTextColor(WHITE, BLACK);
	LCD_SetTextSize(1);
	LCD_Printf("Hello World! \n\r");
	LCD_SetTextColor(YELLOW, BLACK);
	LCD_SetTextSize(2);
	float tempVar = 1234.56;
	LCD_Printf("%0.2f \n\r", tempVar);
	LCD_SetTextColor(RED, BLACK);
	LCD_SetTextSize(3);
	uint32_t tempVar1 = 0xDEADBEEF;
	LCD_Printf("%X \n\r", tempVar1);
	LCD_SetTextColor(GREEN, BLACK);
	LCD_SetTextSize(5);
	LCD_Printf("Group \n\r");
	LCD_SetTextSize(2);
	LCD_Printf("I implore thee, \n\r");
	LCD_SetTextSize(1);
	LCD_Printf("my foonting turlingdromes. \n\r");
	LCD_Printf("And hooptiously drangle me \n\r");
	LCD_Printf("with crinkly bindlewurdles, \n\r");
	LCD_Printf("Or I will rend thee \n\r");
	LCD_Printf("in the gobberwarts \n\r");
	LCD_Printf("with my blurglecruncheon, \n\r");
	LCD_Printf("see if I don't! \n\r");

	print("testText: %dms \n\r", HAL_GetTick() - start);
}

void testLines(uint16_t color) {
	unsigned long start, t;
	int x1, y1, x2, y2, w = TFTWIDTH, h = TFTHEIGHT;

	LCD_FillScreen(BLACK);

	x1 = y1 = 0;
	y2 = h - 1;
	start = HAL_GetTick();
	for (x2 = 0; x2 < w; x2 += 6)
		LCD_DrawLine(x1, y1, x2, y2, color);
	x2 = w - 1;
	for (y2 = 0; y2 < h; y2 += 6)
		LCD_DrawLine(x1, y1, x2, y2, color);
	t = HAL_GetTick() - start; // fillScreen doesn't count against timing

	LCD_FillScreen(BLACK);

	x1 = w - 1;
	y1 = 0;
	y2 = h - 1;
	start = HAL_GetTick();
	for (x2 = 0; x2 < w; x2 += 6)
		LCD_DrawLine(x1, y1, x2, y2, color);
	x2 = 0;
	for (y2 = 0; y2 < h; y2 += 6)
		LCD_DrawLine(x1, y1, x2, y2, color);
	t += HAL_GetTick() - start;

	LCD_FillScreen(BLACK);

	x1 = 0;
	y1 = h - 1;
	y2 = 0;
	start = HAL_GetTick();
	for (x2 = 0; x2 < w; x2 += 6)
		LCD_DrawLine(x1, y1, x2, y2, color);
	x2 = w - 1;
	for (y2 = 0; y2 < h; y2 += 6)
		LCD_DrawLine(x1, y1, x2, y2, color);
	t += HAL_GetTick() - start;

	LCD_FillScreen(BLACK);

	x1 = w - 1;
	y1 = h - 1;
	y2 = 0;
	start = HAL_GetTick();
	for (x2 = 0; x2 < w; x2 += 6)
		LCD_DrawLine(x1, y1, x2, y2, color);
	x2 = 0;
	for (y2 = 0; y2 < h; y2 += 6)
		LCD_DrawLine(x1, y1, x2, y2, color);

	print("testLines: %dms \n\r", HAL_GetTick() - start);
}

void testFastLines(uint16_t color1, uint16_t color2) {
	unsigned long start;
	int x, y, w = TFTWIDTH, h = TFTHEIGHT;

	LCD_FillScreen(BLACK);
	start = HAL_GetTick();
	for (y = 0; y < h; y += 5)
		LCD_DrawFastHLine(0, y, w, color1);
	for (x = 0; x < w; x += 5)
		LCD_DrawFastVLine(x, 0, h, color2);

	print("testFastLines: %dms \n\r", HAL_GetTick() - start);
}

void testRects(uint16_t color) {
	unsigned long start;
	int n, i, i2, cx = TFTWIDTH / 2, cy = TFTHEIGHT / 2;

	LCD_FillScreen(BLACK);
	n = min(TFTWIDTH, TFTHEIGHT);
	start = HAL_GetTick();
	for (i = 2; i < n; i += 6) {
		i2 = i / 2;
		LCD_DrawRect(cx - i2, cy - i2, i, i, color);
	}

	print("testRects: %dms \n\r", HAL_GetTick() - start);
}

void testFilledRects(uint16_t color1, uint16_t color2) {
	unsigned long start, t = 0;
	int n, i, i2, cx = TFTWIDTH / 2 - 1, cy = TFTHEIGHT / 2 - 1;

	LCD_FillScreen(BLACK);
	n = min(TFTWIDTH, TFTHEIGHT);
	for (i = n; i > 0; i -= 6) {
		i2 = i / 2;
		start = HAL_GetTick();
		LCD_FillRect(cx - i2, cy - i2, i, i, color1);
		t += HAL_GetTick() - start;
		// Outlines are not included in timing results
		LCD_DrawRect(cx - i2, cy - i2, i, i, color2);
	}
	print("testFilledRects: %dms \n\r", t);
}

void testFilledCircles(uint8_t radius, uint16_t color) {
	unsigned long start;
	int x, y, w = TFTWIDTH, h = TFTHEIGHT, r2 = radius * 2;

	LCD_FillScreen(BLACK);
	start = HAL_GetTick();
	for (x = radius; x < w; x += r2) {
		for (y = radius; y < h; y += r2) {
			LCD_FillCircle(x, y, radius, color);
		}
	}

	print("testFilledCircles: %dms \n\r", HAL_GetTick() - start);
}

void testCircles(uint8_t radius, uint16_t color) {
	unsigned long start;
	int x, y, r2 = radius * 2, w = TFTWIDTH + radius, h = TFTHEIGHT + radius;

	// Screen is not cleared for this one -- this is
	// intentional and does not affect the reported time.
	start = HAL_GetTick();
	for (x = 0; x < w; x += r2) {
		for (y = 0; y < h; y += r2) {
			LCD_DrawCircle(x, y, radius, color);
		}
	}

	print("testCircles: %dms \n\r", HAL_GetTick() - start);
}

void testTriangles() {
	unsigned long start;
	int n, i, cx = TFTWIDTH / 2 - 1, cy = TFTHEIGHT / 2 - 1;

	LCD_FillScreen(BLACK);
	n = min(cx, cy);
	start = HAL_GetTick();
	for (i = 0; i < n; i += 5) {
		LCD_DrawTriangle(cx, cy - i, // peak
		cx - i, cy + i, // bottom left
		cx + i, cy + i, // bottom right
		LCD_Color565(0, 0, i));
	}

	print("testTriangles: %dms \n\r", HAL_GetTick() - start);
}

void testFilledTriangles() {
	unsigned long start, t = 0;
	int i, cx = TFTWIDTH / 2 - 1, cy = TFTHEIGHT / 2 - 1;

	LCD_FillScreen(BLACK);
	start = HAL_GetTick();
	for (i = min(cx, cy); i > 10; i -= 5) {
		start = HAL_GetTick();
		LCD_FillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
				LCD_Color565(0, i, i));
		t += HAL_GetTick() - start;
		LCD_DrawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
				LCD_Color565(i, i, 0));
	}
	print("testFilledTriangles: %dms \n\r", t);
//  return t;
}

void testRoundRects() {
	unsigned long start;
	int w, i, i2, cx = TFTWIDTH / 2 - 1, cy = TFTHEIGHT / 2 - 1;

	LCD_FillScreen(BLACK);
	w = min(TFTWIDTH, TFTHEIGHT);
	start = HAL_GetTick();
	for (i = 0; i < w; i += 6) {
		i2 = i / 2;
		LCD_DrawRoundRect(cx - i2, cy - i2, i, i, i / 8, LCD_Color565(i, 0, 0));
	}

	print("testRoundRects: %dms \n\r", HAL_GetTick() - start);
}

void testFilledRoundRects() {
	unsigned long start;
	int i, i2, cx = TFTWIDTH / 2 - 1, cy = TFTHEIGHT / 2 - 1;

	LCD_FillScreen(BLACK);
	start = HAL_GetTick();
	for (i = min(TFTWIDTH, TFTHEIGHT); i > 20; i -= 6) {
		i2 = i / 2;
		LCD_FillRoundRect(cx - i2, cy - i2, i, i, i / 8, LCD_Color565(0, i, 0));
	}

	print("testFilledRoundRects: %dms \n\r", HAL_GetTick() - start);
}
