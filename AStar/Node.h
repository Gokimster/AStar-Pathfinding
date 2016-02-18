// Node.h: interface for the CNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODE_H__DB5E38D8_9964_4BF5_87D6_A209C9F5E3B8__INCLUDED_)
#define AFX_NODE_H__DB5E38D8_9964_4BF5_87D6_A209C9F5E3B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNode  
{

public:
	CNode();
	virtual ~CNode();

};

struct NODE
{
	int TotalCost;
	int X,Y;
	int StartCost;
	int ParentX,ParentY; //parent x,y

};


#endif // !defined(AFX_NODE_H__DB5E38D8_9964_4BF5_87D6_A209C9F5E3B8__INCLUDED_)
