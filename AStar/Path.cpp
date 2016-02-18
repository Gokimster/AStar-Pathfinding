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
			cost_to_goal = (min(abs(x - goal_x), abs(y - goal_y)) * diagonalCost) + (abs(abs(x - goal_x)- abs(y - goal_y)) * cost);
			cost_to_start = ClosedList.back().StartCost;
			if (abs(ClosedList.back().X - x) + abs(ClosedList.back().Y - y) > 1)
			{
				cost_to_start += diagonalCost;
			}
			else {
				cost_to_start += cost;
			}

			tempNode.StartCost = cost_to_start;
			tempNode.TotalCost = base_cost + cost_to_start + cost_to_goal;
			tempNode.X = x;
			tempNode.Y = y;
			tempNode.ParentX = next_x;
			tempNode.ParentY = next_y;
			if(GetNodeAt(OpenList,x,y).TotalCost != -1 )
			{
				if(GetNodeAt(OpenList,x,y).TotalCost > tempNode.TotalCost)
				{
					DeleteElement(OpenList, x, y);
					OpenList.push_back(tempNode);
					printf("Insterted [%d, %d] with tota weight : %d \n", x, y, tempNode.TotalCost);
				}
			}else {
				if (!IsElementExits(x, y))
				{
					OpenList.push_back(tempNode);
					printf("Insterted [%d, %d] with tota weight : %d \n", x, y, tempNode.TotalCost);
				}
			}


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
void CPath::InsertIntoClosedList(NODE n)
{
	NODE tempNode;
		
	tempNode.StartCost=n.StartCost;
	tempNode.TotalCost=n.TotalCost;
	tempNode.X=n.X;
	tempNode.Y=n.Y;
	tempNode.ParentX=n.ParentX;
	tempNode.ParentY=n.ParentY;
	ClosedList.push_back(tempNode);
}
void CPath::Create()
{
	//Clear previous lists
	ClearAll();

	//add the start node to the closed list
	NODE start;
	start.X = start_x;
	start.Y = start_y;
	start.StartCost = 0;
	start.TotalCost = 0;
	start.ParentX = start_x;
	start.ParentY = start_y;
	InsertIntoClosedList(start);

	next_x=start_x;
	next_y=start_y;
	while(obstruction[next_x][next_y] != 9 && next_x <20 && next_x >=0 && next_y <20 && next_y >=0)
	{
		//take all the neighboring passable nodes and add them to the OpenList
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (!(i == 0 && j == 0))
				{
					if (next_x + i < 20 && next_x + i >= 0 && next_y + j < 20 && next_y + j >= 0)
					{
						InsertIntoOpenList(next_x + i, next_y + j);
					}
				}
			}
		}
		printf("did tile : [%d %d] \n", next_x, next_y);
		if (OpenList.size() == 0)
		{
			break;
		}
		NODE minNode = MinNode(OpenList);
		InsertIntoClosedList(minNode);
		OpenList = DeleteElement(OpenList, minNode.X, minNode.Y);
		next_x = minNode.X;
		next_y = minNode.Y;
		//find the min node
		//delete the minimum node from the open list
		 
	}
	printf("DONE \n");
}
vector<NODE> CPath::BackTrack()
{
	vector<NODE> pathL;
	int k;
	k=0;
	NODE tempNode = GetNodeAt(ClosedList, ClosedList[ClosedList.size() - 1].X, ClosedList[ClosedList.size() - 1].Y);
	while ((tempNode.TotalCost != -1) && (!((tempNode.X == start_x) && (tempNode.Y == start_y))))
	{
		tempNode = GetNodeAt(ClosedList, tempNode.ParentX, tempNode.ParentY);
		if (tempNode.TotalCost != -1)
			pathL.push_back(tempNode);
	}
	return pathL;
}
NODE CPath::GetNodeAt(vector<NODE> nodesList, int x,int y)
{
	NODE tempNode;
	tempNode.TotalCost=-1;
	for(int i=0;i<nodesList.size();i++)
	{
		if(nodesList[i].X==x && nodesList[i].Y==y)
		{
			tempNode=nodesList[i];
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