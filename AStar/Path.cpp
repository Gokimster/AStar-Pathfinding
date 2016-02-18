// Path.cpp: implementation of the CPath class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Path.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPath::CPath()
{

}

CPath::~CPath()
{

}
void CPath::SetObstructionMatrix(unsigned int map[20][20],unsigned int grnd,unsigned int strt,unsigned int gol)
{
	//we set the ground tiles to be zero
	//we also set the tree,bushes, stones tiles to be 1
	//the start is set to 8 and goal is set to 9
	for(int i=0;i<20;i++)
	{
		for(int j=0;j<20;j++)
		{
			if(map[i][j]==grnd)
				obstruction[i][j]=0;
			else if(map[i][j]==strt)
			{
				obstruction[i][j]=8;
				start_x=i;
				start_y=j;
			}
			else if(map[i][j]==gol)
			{
				obstruction[i][j]=9;
				goal_x=i;
				goal_y=j;
			}
			else
				obstruction[i][j]=1;
		}
	}
}
void CPath::InsertIntoOpenList(int x, int y)
{
	
	//node must be inside the boundary of the map
	if(x <20 && y<20 && ClosedList.size() > 0 && !IsElementExits(x,y))
	{
		NODE tempNode;
		//the tile must be ground tile, so it will be passable
		if(obstruction[x][y]==0)
		{
			int base_cost,cost_to_start,cost_to_goal;
	
			base_cost=obstruction[x][y];
		//This is where you need to work out where to go next
		}
		else if(obstruction[x][y]==9)
		{
			tempNode.StartCost=0;
			tempNode.TotalCost=0;
			tempNode.X=x;
			tempNode.Y=y;
			tempNode.ParentX=ClosedList.back().X;
			tempNode.ParentY=ClosedList.back().Y;

			OpenList.push_back(tempNode);
		}
		
	}
}
void CPath::InsertIntoClosedList(int total_cost,int start_cost,int x, int y)
{
	
	//node must be inside the boundary of the map
	if(x <20 && y<20 && !IsElementExits(x,y))
	{
		NODE tempNode;
		
		tempNode.StartCost=start_cost;
		tempNode.TotalCost=total_cost;
		tempNode.X=x;
		tempNode.Y=y;
		tempNode.ParentX=x;
		tempNode.ParentY=y;
		ClosedList.push_back(tempNode);		
	}
}
void CPath::Create()
{
	//Clear previous lists
	ClearAll();

	//add the start node to the closed list
	InsertIntoClosedList(0,0,start_x,start_y);

	int next_x=start_x;
	int next_y=start_y;
	while(obstruction[next_x][next_y] != 9 && next_x <20 && next_x >=0 && next_y <20 && next_y >=0)
	{
		//take all the neighboring passable nodes and add them to the OpenList
		

		//find the min node

		

		//delete the minimum node from the open list
		 
	}
}
vector<NODE> CPath::BackTrack()
{
	vector<NODE> pathL;
	int k;
	k=0;
	
	for(int i=ClosedList.size()-1;i> -1;i--)
	{
		NODE tempNode;
		tempNode=GetNodeAt(ClosedList[i].ParentX,ClosedList[i].ParentY);
		if(tempNode.TotalCost != -1)
			pathL.push_back(tempNode);
	}
	return pathL;
}
NODE CPath::GetNodeAt(int x,int y)
{
	NODE tempNode;
	tempNode.TotalCost=-1;
	for(int i=0;i<ClosedList.size();i++)
	{
		if(ClosedList[i].X==x && ClosedList[i].Y==y)
		{
			tempNode=ClosedList[i];
		}
	}
	return tempNode;
}
NODE CPath::MinNode(vector<NODE> nodesList)
{
	NODE minNode;
	if(nodesList.size()>0)
	{
		minNode=nodesList[0];
		for(int i=1;i<nodesList.size();i++)
		{
			if(nodesList[i].TotalCost < minNode.TotalCost)
				minNode=nodesList[i];
		}
		return minNode;
	}
	else
	{
		//return an error
		minNode.StartCost=-1;
		minNode.TotalCost=-1;
		minNode.X=0;
		minNode.Y=0;

		return minNode;
	}
}

vector<NODE> CPath::DeleteElement(vector<NODE> nodesList,int x,int y)
{
	vector<NODE> tempL=nodesList;

	for(int i=0;i<tempL.size();i++)
	{
		if(tempL[i].X==x && tempL[i].Y==y)
		{
			tempL.erase(tempL.begin()+i);
			break;
		}
	}
	return tempL;
}

bool CPath::IsElementExits(int x,int y)
{
	//check to see if the element is not exit in both lists or not
	for(int i=0;i<OpenList.size();i++)
	{
		if(OpenList[i].X==x && OpenList[i].Y==y)
		{
			return true;
		}
	}
	for(int i=0;i<ClosedList.size();i++)
	{
		if(ClosedList[i].X==x && ClosedList[i].Y==y)
		{
			return true;
		}
	}
	return false;
}

void CPath::ClearAll()
{
	OpenList.erase(OpenList.begin(),OpenList.begin()+OpenList.size());
	ClosedList.erase(ClosedList.begin(),ClosedList.begin()+ClosedList.size());
}