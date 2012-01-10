//http://scriptionary.com/2009/02/17/simple-scene-graph-in-c/

#ifndef __node_h_
#define __node_h_ 1
#include <vector>
using std;
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
			
		protected:
			std::list childs;
		};
		
	class TransformationNode
		{
		public:
			//get transform
			//set transform
			
			//apply transform
			//remove tranform
		
		private:
			glm:mat4 transform;
		};
	
	class MeshNode
		{
			//apply tranformation, draw
		};
#endif
}
