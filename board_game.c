#include <stdio.h>
// #include<graphics.h>
#define RED (0xF)
#define GREEN (0xF0)
#define BLUE (0xF00)
#define WHITE (0xFFF)
#define BLACK (0x0)
#define DELAY 1000000
#define r0 9
#define h0 40 // 砖高
#define l0 213
#define h 380
#define c (*(volatile unsigned int *)(0x101 * 4))
#define a0 (*(volatile unsigned int *)(0x100 * 4))
#define led ((volatile unsigned int(*)[3])(0x0))
#define READ_SW(idx) ((*(volatile unsigned int *)(0x300 * 4)) & (1 << (idx)))
#define VALID (*(volatile unsigned int *)(0x350 * 4))
#define ball_speed 2
#define board_speed 3
// int led[12][3];

int main()
{

	int x0, y0, s1, s2, score = 0, SW1 = 0, SW2 = 0, x1, y1, i, j; // （s1为1x增反之减是，s2用来代表y）下降l,r是木板边界,a0表示圆心,SW1表示左移，SW2表示右移
	// 初始板的位置
	int flag;
	int start;
	int l, r, b; // 定义在第380行 长为200的板子 b高16位左端点 低16位右端点
	while (1)
	{
		// 初始化
		// drawo(x0, y0,BLACK);
		score = 0;
		y0 = 380 - r0;
		x0 = 240;
		s1 = -ball_speed, s2 = -ball_speed;
		start = 1;
		flag = 1;
		a0 = (int)((y0 << 16) + x0); // ####2 a0高16位
		// printf("%d\n",a0);
		// drawo(x0, y0, WHITE);
		l = 200;
		r = 400;
		c = (int)((l << 16) + r);
		// printf("%d\n",c);
		led[0][0] = RED;
		led[0][1] = BLUE;
		led[0][2] = GREEN;
		led[1][0] = GREEN;
		led[1][1] = RED;
		led[1][2] = BLUE;
		led[2][0] = BLUE;
		led[2][1] = GREEN;
		led[2][2] = RED;
		for (i = 3; i <= 11; i++)
		{
			for (j = 0; j <= 2; j++)
				led[i][j] = BLACK;
		}
		while (1)
		{
			if (flag)
			{
				flag = 0;
				if (start)
				{
					if(READ_SW(9)){ start = 0; break;}
					SW1 = READ_SW(10);
					SW2 = READ_SW(11);
					// 移动木板
					if (SW1) // 右移
					{
						l += board_speed;
						r += board_speed;
						x0 += board_speed;
						if (r > 639)
						{
							r = 639;
							l = 439;
						} // 维护木板的边
					}
					else if (SW2)
					{
						l -= board_speed;
						r -= board_speed;
						x0 -= board_speed;
						if (l < 0)
						{
							r = 200;
							l = 0;
						}
					}
				}
				else
				{								  // 不发送
					if (x0 + r0 >= 639 && s1 > 0) // 到达右边界
					{
						s1 = 0 - s1;
					}

					if (x0 - r0 <= 0 && s1 < 0) // 到左边界
					{
						s1 = 0 - s1;
					}

					if (y0 + r0 >= 380 && x0 + r0 <= r && x0 - r0 >= l && s2 > 0) // 碰板
					{
						if (x0 > (r + l) / 2)
						{
							s1 = ball_speed;
							s2 = -ball_speed;
						}
						else if (x0 < (r + l) / 2)
						{
							s1 = -ball_speed;
							s2 = -ball_speed;
						}
						else if (x0 == (r + l) / 2)
						{
							s1 = 0;
							s2 = -(ball_speed + ball_speed);
						}
					}
					if (y0 - r0 <= 0 && s2 < 0) // 上边界
					{
						s2 = 0 - s2;
					}

					if (y0 > 380 + ball_speed && s2 > 0) // 下边界
					{
						break; // 失败1 简化处理一旦球超过板就回到初始位置
					}

					if (led[(y0 - r0) / h0][x0 / l0] != BLACK)
					{
						s2 = 0 - s2;
						led[(y0 - r0) / h0][x0 / l0] = BLACK;
					}
					else if (led[(y0 + r0) / h0][x0 / l0] != BLACK)
					{
						s2 = 0 - s2;
						led[(y0 + r0) / h0][x0 / l0] = BLACK;
					}
					else if (led[y0 / h0][(x0 + r0) / l0] != BLACK)
					{
						s1 = 0 - s1;
						led[y0 / h0][(x0 + r0) / l0] = BLACK;
					}
					else if (led[y0 / h0][(x0 - r0) / l0] != BLACK)
					{
						s1 = 0 - s1;
						led[y0 / h0][(x0 - r0) / l0] = BLACK;
					}
					// 球的新位置
					x0 = x0 + s1;
					y0 = y0 + s2;
					a0 = (int)((y0 << 16) + x0);
					// printf("%d\n",a0);
					SW1 = READ_SW(10);
					SW2 = READ_SW(11);
					// 移动木板
					if (SW1) // 右移
					{
						l += board_speed;

						r += board_speed;
						if (r > 639)
						{
							r = 639;
							l = 439;
						} // 维护木板的边界
					}
					else if (SW2)
					{
						l -= board_speed;
						r -= board_speed;
						if (l < 0)
						{
							r = 200;
							l = 0;
						}
					}
					// 木板的新位置
					c = (int)((l << 16) + r);
					// printf("%d\n",c);
				}
			}
			while (!VALID) flag = 1;
		}
	}
}