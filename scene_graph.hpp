//http://www.andresilaghi.com/?page_id=239

#ifndef __node_h_
#define __node_h_ 1
#include <list>

namespace graingert{
	 class Node
		{
		public:
			Node(void);
			~Node(void);
		
			Node* parent;
		
			void addChild(Node* node);
			void removeChilds(Node* node);
		
			virtual void traverse();
		
			void kill();
			
			std::list<Node*> childs;
		};
		
	class TransformationNode
		{
		public:
			static glm::mat4 s_transform;
			glm::mat4 transform;
		
		};
	
	class MeshNode
		{
			public:
				Mesh* mesh;
				static Renderer* renderer;
		};
#endif
}
