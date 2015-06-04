#include <time.h>
#include <windows.h>
#include <stdio.h>
#include "Block.h"
#include "Util.h"
#include "Constant.h"

#define BLOCK_EXAMPLES_SIZE 7

const static Point blockExamples[BLOCK_EXAMPLES_SIZE][POSITIONS_SIZE][POSITIONS_SIZE] = {
	//��������
	{
		{ { 0, 5 }, { 0, 6 }, { 0, 7 }, { 0, 8 } },
		{ { -1, 6 }, { 0, 6 }, { 1, 6 }, { 2, 6 } },
		{ { 0, 5 }, { 0, 6 }, { 0, 7 }, { 0, 8 } },
		{ { -1, 6 }, { 0, 6 }, { 1, 6 }, { 2, 6 } }
	},
	//    ��
	//������
	{
		{ { 0, 8 }, { 1, 6 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 7 }, { 0, 7 }, { 1, 7 }, { 1, 8 } },
		{ { 0, 6 }, { 0, 7 }, { 0, 8 }, { 1, 6 } },
		{ { -1, 6 }, { -1, 7 }, { 0, 7 }, { 1, 7 } }
	},
	//  ����
	//����
	{
		{ { 0, 7 }, { 0, 8 }, { 1, 6 }, { 1, 7 } },
		{ { -1, 6 }, { 0, 6 }, { 0, 7 }, { 1, 7 } },
		{ { 0, 7 }, { 0, 8 }, { 1, 6 }, { 1, 7 } },
		{ { -1, 6 }, { 0, 6 }, { 0, 7 }, { 1, 7 } }
	},
	//����
	//  ����
	{
		{ { 0, 6 }, { 0, 7 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 8 }, { 0, 8 }, { 0, 7 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 8 }, { 0, 8 }, { 0, 7 }, { 1, 7 } }
	},
	//  ��
	//������
	{
		{ { 0, 7 }, { 1, 6 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 7 }, { 0, 7 }, { 0, 8 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 0, 8 }, { 1, 7 } },
		{ { -1, 7 }, { 0, 6 }, { 0, 7 }, { 1, 7 } }
	},
	//��
	//������
	{
		{ { 0, 6 }, { 1, 6 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 8 }, { -1, 7 }, { 0, 7 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 0, 8 }, { 1, 8 } },
		{ { -1, 7 }, { 0, 7 }, { 1, 7 }, { 1, 6 } }
	},
	//����
	//����
	{
		{ { 0, 6 }, { 0, 7 }, { 1, 6 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 1, 6 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 1, 6 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 1, 6 }, { 1, 7 } }
	}
};

static Block _Block_MoveToDown(Block block);
static Block _Block_MoveToLeft(Block block);
static Block _Block_MoveToRight(Block block);
static Block _Block_RotateRight(Block block);
static void _Block_PrintDefaultBlock(int blockNumber, int x, int* y);

Block Block_Make(int isFirst, Block block){
	int i;
	int j;
	srand((unsigned int)time(NULL));
	if (isFirst){
		block.current = rand() % BLOCK_EXAMPLES_SIZE;
		block.hold = -1;
	}
	else{
		block.current = block.next;
	}
	for (i = 0; i < POSITIONS_SIZE; i++){
		for (j = 0; j < POSITIONS_SIZE; j++){
			block.positions[i][j] = blockExamples[block.current][i][j];
		}
	}
	do{
		block.next = rand() % BLOCK_EXAMPLES_SIZE;
	} while (block.current == block.next);
	block.direction = UP;
	block.color = rand() % (FONT_COLOR_SIZE - 2) + 2;
	return block;
}

Block Block_Move(Block block, int direction){
	switch (direction){
	case LEFT:
		return _Block_MoveToLeft(block);
	case RIGHT:
		return _Block_MoveToRight(block);
	case DOWN:
		return _Block_MoveToDown(block);
	case UP:
		return _Block_RotateRight(block);
	}
	return _Block_MoveToDown(block);
}

Point* Block_GetPositions(Block block){
	return block.positions[block.direction];
}

int Block_IsHoldSet(Block block){
	return block.hold != -1;
}

void Block_ChangeCurrentForHold(Block* block){
	int i;
	int j;
	int temp = block->current;
	block->current = block->hold;
	block->hold = temp;
	if (block->current != -1){
		for (i = 0; i < POSITIONS_SIZE; i++){
			for (j = 0; j < POSITIONS_SIZE; j++){
				block->positions[i][j] = blockExamples[block->current][i][j];
			}
		}
	}
	else{
		*block = Block_Make(False, *block);
	}
}

void Block_PrintNext(Block block, int x, int y){
	CursorUtil_GotoXY(x, y++);
	printf("���� Next Block ����");
	CursorUtil_GotoXY(x, y++);
	_Block_PrintDefaultBlock(block.next, x, &y);
	CursorUtil_GotoXY(x, y++);
	printf("��������������������");
}

void Block_PrintHold(Block block, int x, int y){
	CursorUtil_GotoXY(x, y++);
	printf("���� Hold Block ����");
	CursorUtil_GotoXY(x, y++);
	_Block_PrintDefaultBlock(block.hold, x, &y);
	CursorUtil_GotoXY(x, y++);
	printf("��������������������");
}

static Block _Block_MoveToDown(Block block){
	int i;
	int j;
	for (i = 0; i < POSITIONS_SIZE; i++){
		for (j = 0; j < POSITIONS_SIZE; j++){
			block.positions[i][j].x++;
		}
	}
	return block;
}

static Block _Block_MoveToLeft(Block block){
	int i;
	int j;
	for (i = 0; i < POSITIONS_SIZE; i++){
		for (j = 0; j < POSITIONS_SIZE; j++){
			block.positions[i][j].y--;
		}
	}
	return block;
}

static Block _Block_MoveToRight(Block block){
	int i;
	int j;
	for (i = 0; i < POSITIONS_SIZE; i++){
		for (j = 0; j < POSITIONS_SIZE; j++){
			block.positions[i][j].y++;
		}
	}
	return block;
}

static Block _Block_RotateRight(Block block){
	block.direction = (block.direction + 1) % POSITIONS_SIZE;
	return block;
}

static void _Block_PrintDefaultBlock(int blockNumber, int x, int* y){
	switch (blockNumber){
	case -1:
		printf("��                ��");
		CursorUtil_GotoXY(x, (*y)++);
		printf("��                ��");
		break;
	case 0:
		printf("��    �����    ��");
		CursorUtil_GotoXY(x, (*y)++);
		printf("��                ��");
		break;
	case 1:
		printf("��          ��    ��");
		CursorUtil_GotoXY(x, (*y)++);
		printf("��      ����    ��");
		break;
	case 2:
		printf("��       ���     ��");
		CursorUtil_GotoXY(x, (*y)++);
		printf("��     ���       ��");
		break;
	case 3:
		printf("��     ���       ��");
		CursorUtil_GotoXY(x, (*y)++);
		printf("��       ���     ��");
		break;
	case 4:
		printf("��       ��       ��");
		CursorUtil_GotoXY(x, (*y)++);
		printf("��     ����     ��");
		break;
	case 5:
		printf("��     ��         ��");
		CursorUtil_GotoXY(x, (*y)++);
		printf("��     ����     ��");
		break;
	case 6:
		printf("��      ���      ��");
		CursorUtil_GotoXY(x, (*y)++);
		printf("��      ���      ��");
		break;
	}
}