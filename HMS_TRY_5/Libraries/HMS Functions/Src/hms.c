/*
 * hms.c
 *
 *  Created on: Mar 13, 2023
 *      Author: hp
 */
#include "hms.h"

#define graphRefreshRate_peak			2
#define graphRefreshRate_normal			5

const int graph_x0 = 30;
const int graph_y0 = (2 * (TFTHEIGHT / 3)) + 30;
const int graph_x1 = 30;
const int graph_y1 = graph_y0 + 50;
const int graph_x2 = graph_x0 + TFTWIDTH - 95;
const int graph_y2 = graph_y0;
const int graph_x3 = graph_x0 + TFTWIDTH - 95;
const int graph_y3 = graph_y0 + 50;
int graph_x = graph_x0;
int graph_y = 0;
int graph_y_prevVal = 0;
int graph_refresh_x = graph_x0;

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

	for(int i = 4; i > 0; i--)
		LCD_ECGAnimation();

	LCD_SetTextColor(BLACK, WHITE);
	LCD_SetCursor(180, 240);
	LCD_SetTextSize(2);
	LCD_Printf("ECG");

}

void ecgPeak() {
	LCD_DrawLine(graph_x, graph_y0 + 25, graph_x + graphRefreshRate_peak,
			graph_y0 + 25, RED);
	LCD_DrawLine(graph_x, graph_y0 + 26, graph_x + graphRefreshRate_peak,
			graph_y0 + 26, RED);
	graph_x += graphRefreshRate_peak;
	LCD_DrawLine(graph_x, graph_y0 + 25, graph_x + graphRefreshRate_peak,
			graph_y0 + 25, RED);
	LCD_DrawLine(graph_x, graph_y0 + 26, graph_x + graphRefreshRate_peak,
			graph_y0 + 26, RED);
	graph_x += graphRefreshRate_peak;
	LCD_DrawLine(graph_x, graph_y0 + 25, graph_x + graphRefreshRate_peak,
			graph_y0 + 25, RED);
	LCD_DrawLine(graph_x, graph_y0 + 26, graph_x + graphRefreshRate_peak,
			graph_y0 + 26, RED);
	graph_x += graphRefreshRate_peak;

	LCD_DrawLine(graph_x, graph_y0 + 25, graph_x + graphRefreshRate_peak,
			graph_y0 + 40, RED);
	LCD_DrawLine(graph_x, graph_y0 + 26, graph_x + graphRefreshRate_peak,
			graph_y0 + 41, RED);
	graph_x += graphRefreshRate_peak;
	LCD_DrawLine(graph_x, graph_y0 + 40, graph_x + graphRefreshRate_peak,
			graph_y0, RED);
	LCD_DrawLine(graph_x, graph_y0 + 41, graph_x + graphRefreshRate_peak,
			graph_y0 + 1, RED);
	graph_x += graphRefreshRate_peak;
	LCD_DrawLine(graph_x, graph_y0, graph_x + graphRefreshRate_peak,
			graph_y0 + 25, RED);
	LCD_DrawLine(graph_x, graph_y0 + 1, graph_x + graphRefreshRate_peak,
			graph_y0 + 26, RED);
	graph_x += graphRefreshRate_peak;

	LCD_DrawLine(graph_x, graph_y0 + 25, graph_x + graphRefreshRate_peak,
			graph_y0 + 25, RED);
	LCD_DrawLine(graph_x, graph_y0 + 26, graph_x + graphRefreshRate_peak,
			graph_y0 + 26, RED);
	graph_x += graphRefreshRate_peak;
	LCD_DrawLine(graph_x, graph_y0 + 25, graph_x + graphRefreshRate_peak,
			graph_y0 + 25, RED);
	LCD_DrawLine(graph_x, graph_y0 + 26, graph_x + graphRefreshRate_peak,
			graph_y0 + 26, RED);
	graph_x += graphRefreshRate_peak;
	LCD_DrawLine(graph_x, graph_y0 + 25, graph_x + graphRefreshRate_peak,
			graph_y0 + 25, RED);
	LCD_DrawLine(graph_x, graph_y0 + 26, graph_x + graphRefreshRate_peak,
			graph_y0 + 26, RED);
}

void ecgLine() {
	for (uint8_t i = 0; i < 3; i++) {
		LCD_DrawLine(graph_x, graph_y0 + 25, graph_x + graphRefreshRate_peak,
				graph_y0 + 25, RED);
		LCD_DrawLine(graph_x, graph_y0 + 26, graph_x + graphRefreshRate_peak,
				graph_y0 + 26, RED);
		graph_x += graphRefreshRate_peak;
		LCD_DrawLine(graph_x, graph_y0 + 25, graph_x + graphRefreshRate_peak,
				graph_y0 + 25, RED);
		LCD_DrawLine(graph_x, graph_y0 + 26, graph_x + graphRefreshRate_peak,
				graph_y0 + 26, RED);
		graph_x += graphRefreshRate_peak;
		LCD_DrawLine(graph_x, graph_y0 + 25, graph_x + graphRefreshRate_peak,
				graph_y0 + 25, RED);
		LCD_DrawLine(graph_x, graph_y0 + 26, graph_x + graphRefreshRate_peak,
				graph_y0 + 26, RED);
	}
}

void LCD_ECGAnimation() {
	ecgLine();

	int a = graph_x;
	if (a + graphRefreshRate_peak >= graph_x2 - 25) {
		LCD_FillRect(20, (2 * (TFTHEIGHT / 3)) + 20, TFTWIDTH - 80, 70, WHITE);
		graph_x = graph_x0;
	} else {
		graph_x += graphRefreshRate_peak;
	}

	ecgPeak();
}
