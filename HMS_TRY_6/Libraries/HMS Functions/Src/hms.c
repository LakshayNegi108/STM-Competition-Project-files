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

	LCD_SetTextColor(WHITE, RED);
	LCD_SetTextSize(2);
	LCD_SetCursor(38, 70);
	LCD_Printf("__");

	LCD_SetTextColor(WHITE, LCD_Color565(16, 220, 21));
	LCD_SetTextSize(2);
	LCD_SetCursor(160, 70);
	LCD_Printf("__");

	LCD_SetTextColor(WHITE, LCD_Color565(30, 35, 247));
	LCD_SetTextSize(2);
	LCD_SetCursor(30, 145);
	LCD_Printf("__");

	LCD_SetTextColor(BLACK, WHITE);
	LCD_SetTextSize(2);
	LCD_SetCursor(145, 130);
	LCD_Printf("__");

	LCD_SetTextColor(BLACK, WHITE);
	LCD_SetTextSize(2);
	LCD_SetCursor(145, 170);
	LCD_Printf("__");

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

	int a = graph_x;
	if (a + graphRefreshRate_peak >= graph_x2 - 25) {
		LCD_FillRect(20, (2 * (TFTHEIGHT / 3)) + 25, TFTWIDTH - 105, 55, WHITE);
		graph_x = graph_x0;
	} else {
		ecgPeak();
		ecgLine();
		graph_x += graphRefreshRate_peak;
	}
//	ecgLine();
}
