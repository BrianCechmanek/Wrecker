#ifndef _MODEL_H
#define _MODEL_H


typedef struct ModelCell
{
	int x, y;
	unsigned fgColor, bgColor;
	unsigned characterCode;
	int layer;
} ModelCell;

#endif
