// Path.h: interface for the CPath class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_PATH_H__21B793D8_036A_4F2C_8A28_2A766324FF3E__INCLUDED_)
#define AFX_PATH_H__21B793D8_036A_4F2C_8A28_2A766324FF3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include<iostream>
#include<vector>
#include"Node.h"
#include<algorithm>

using namespace std;

class CPath  
{
private:
	vector<NODE> OpenList,ClosedList;
	int obstruction[20][20];
	int start_x,start_y;
	int goal_x,goal_y;
	int diagonalCost = 14;
	int cost = 10;
	int next_x, next_y;
public:
	CPath();
	virtual ~CPath();

	void SetObstructionMatrix(unsigned int map[20][20],unsigned int ground,unsigned int start,unsigned int goal);
	void InsertIntoClosedList(NODE n);
	void InsertIntoOpenList(int x,int y);
	vector<NODE> DeleteElement(vector<NODE> nodesList,int x,int y);
	bool IsElementExits(int x,int y);
	NODE MinNode(vector<NODE>);
	void Create();
	void ClearAll();
	vector<NODE> BackTrack();
	NODE GetNodeAt(vector<NODE>,int x,int y);

};

#endif // !defined(AFX_PATH_H__21B793D8_036A_4F2C_8A28_2A766324FF3E__INCLUDED_)
