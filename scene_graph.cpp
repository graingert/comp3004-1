//http://scriptionary.com/2009/02/17/simple-scene-graph-in-c/
#include "node.hpp"
namespace graingert{

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
			(*it)->kill();
			childs.clear();
		}
	}
	
	void Node::traverse()
	{
		for(std::list::iterator it = childs.begin(); it != childs.end(); it++)
		{
			(*it)->traverse();
		}
	}
	
	void TransformationNode::traverse()
	{
		glm::mat4 old_transform = TransformationNode::s_transform;
		TransformationNode::s_transform = TransformationNode::s_transform * this.transform;
		
		for(std::list::iterator it = childs.begin(); it != childs.end(); it++)
		{
			(*it)->traverse();
		}
		
		TransformationNode::s_transform = old_transform;
	}
	
	void MeshNode::traverse()
	{
		MeshNode::renderer->_mv_matrix = view * TransformationNode::s_transform;
		MeshNode::renderer->bind();
		mesh->draw();
		
		for(std::list::iterator it = childs.begin(); it != childs.end(); it++)
		{
			(*it)->traverse();
		}
	}

}
