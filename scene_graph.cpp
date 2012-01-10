//http://scriptionary.com/2009/02/17/simple-scene-graph-in-c/
#include "node.hpp"
namespace graingert{
	#include
#include "Node.h"

	Node::Node(void)
	{
	}
	
	Node::~Node(void)
	{
		kill();
	}
	
	void Node::kill()
	{
		delete this;
	}
	
	void Node::addChild(Node* node)
	{
		childs.push_back(node);
	}
	
	void Node::removeChilds(Node* node)
	{
		for(std::list::iterator it = childs.begin(); it != childs.end(); it++)
		{
			(*it)->;kill();
			childs.clear();
		}
	}
	
	void Node::traverse()
	{
		for(std::list::iterator it = childs.begin(); it != childs.end(); it++)
		{
			(*it)->;traverse();
		}
	}

}
