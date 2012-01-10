//http://scriptionary.com/2009/02/17/simple-scene-graph-in-c/

#ifndef __node_h_
#define __node_h_ 1
#include <vector>
 
class Node
{
public:
    Node(Node* Parent = NULL, const char* Name = NULL);
    virtual ~Node(void);
 
    virtual void Update(void);
 
    Node* GetParentNode(void) const;
    void SetParentNode(Node* NewParent);
 
    void AddChildNode(Node* ChildNode);
    void RemoveChildNode(Node* ChildNode);
 
    const char* GetNodeName(void) const;
    const size_t CountChildNodes(const bool& RecursiveCount = false) const;
    virtual const bool IsRootNode(void) const = 0;
 
    Node* GetChildNodeByName(const char* SearchName);
 
private:
    Node* m_Parent;
    const char* m_Name;
    std::vector<Node*> m_Children;
 
}; // class Node

class Leaf: public Node {
	public:
		Leaf(Node* Parent = NULL, const char* Name = NULL, Mesh& mesh);
		void Update(void);
		const bool IsRootNode(void) const = 0; 
	private:
		Mesh* mesh;
}  
#endif
