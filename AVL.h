/*	AVL Tree Library.
	Written by: G & F
	Date:       2/98
	
	Revised:    4/99 Converted to C++

	Brooks/Cole
	A division of Thomson Learning
	Copyright(c) 2001. All Rights Reserved
*/


//	==================== MACROS ====================
#define LH +1     // Left High 
#define EH  0     // Even High 
#define RH -1     // Right High 

using namespace std;

// 	NODE Definitions
template <class TYPE> 
class NODE 
	{
	private : 
	 TYPE    data;
	 shared_ptr<NODE<TYPE> >   left;
	 int     bal;
	 shared_ptr<NODE<TYPE> >   right;
	public : 
	shared_ptr<NODE<TYPE> > getLeftSharedPtr() const;
	shared_ptr<NODE<TYPE> > getRightSharedPtr() const;

	void setLeftSharedPtr(shared_ptr<NODE<TYPE> > leftPtr);
	void setRightSharedPtr(shared_ptr<NODE<TYPE> > rightPtr);

	int getBalance() const;
	void setBalance(int balance);

	TYPE getData() const;
	void setData(TYPE theData);
	} ; // NODE

// Class Declaration
template <class TYPE> 
class AvlTree
	{
	 private: 
	    int          count;
		shared_ptr<NODE<TYPE> > tree;

	    shared_ptr<NODE<TYPE> > _insert          (shared_ptr<NODE<TYPE> > root, 
                                      shared_ptr<NODE<TYPE> > newPtr, 
	                                  bool&       taller);

	    shared_ptr<NODE<TYPE> > leftBalance     (shared_ptr<NODE<TYPE> > root, 
	                                  bool&        taller);

	    shared_ptr<NODE<TYPE> > rotateLeft      (shared_ptr<NODE<TYPE> > root);
	    shared_ptr<NODE<TYPE> > rightBalance    (shared_ptr<NODE<TYPE> > root, 
	                                  bool&        taller);
	    shared_ptr<NODE<TYPE> > rotateRight     (shared_ptr<NODE<TYPE> > root);
	    shared_ptr<NODE<TYPE> > _delete          (shared_ptr<NODE<TYPE> > root, 
	                                  TYPE        dltKey,
	                                  bool&        shorter,
	                                  bool&        success);

	    shared_ptr<NODE<TYPE> > dltLeftBalance  (shared_ptr<NODE<TYPE> > root,
	                                  bool&        smaller);
	    shared_ptr<NODE<TYPE> > dltRightBalance (shared_ptr<NODE<TYPE> > root, 
	                                  bool&        shorter);
	    shared_ptr<NODE<TYPE> > _retrieve        (TYPE        key, 
	                                  shared_ptr<NODE<TYPE> > root);
	                                  
	    void  _traversal  (void (*process)(TYPE dataProc),
	                              shared_ptr<NODE<TYPE> > root); 

	    void  _destroyAVL (shared_ptr<NODE<TYPE> > root);

//  	The following function is used for debugging.
	    void  _print      (shared_ptr<NODE<TYPE> > root, int   level);

	 public:
	          AvlTree (void);
	         ~AvlTree  (void); 
	    bool  AVL_Insert   (TYPE   dataIn); 
	    bool  AVL_Delete   (TYPE  dltKey);
	    bool  AVL_Retrieve (TYPE  key, TYPE& dataOut);
	    void  AVL_Traverse (void (*process)(TYPE  dataProc));

	    bool  AVL_Empty    (void);
	    bool  AVL_Full     (void);
	    int   AVL_Count    (void);
	    
//      The following function is used for debugging.
	    void  AVL_Print    (void);
	} ; // class AvlTree
	
/*	=================== Constructor ===================	
	Initializes private data.
	   Pre     class is being defined
	   Post    private data initialized
*/

template <class TYPE>
AvlTree<TYPE> ::  AvlTree (void) 
{
//	Statements 
	tree    = NULL;
	count   = 0;
}	//  Constructor

/*	==================== AVL_Insert ==================== 
	This function inserts new data into the tree.
	   Pre    dataIn contains data to be inserted
	   Post   data have been inserted or memory overflow 
	   Return success (true) or overflow (false)
*/

template <class TYPE>
bool   AvlTree<TYPE> :: AVL_Insert (TYPE dataIn) 
{
//	Local Definitions 
	shared_ptr<NODE<TYPE> > newPtr;
	bool         taller;

//	Statements 
	if (!(newPtr = make_shared<NODE<TYPE> >()))
	   return false;
	newPtr->setBalance(EH);
	newPtr->setRightSharedPtr(NULL);
	newPtr->setLeftSharedPtr(NULL);
	newPtr->setData(dataIn);
   
	tree = _insert(tree, newPtr, taller);
	count++;
	return true;
}	//  Avl_Insert   

/*	======================= _insert =======================
	This function uses recursion to insert the new data into 
	a leaf node in the AVL tree.
	   Pre    application has called AVL_Insert, which passes 
	          root and data pointers
	   Post   data have been inserted
	   Return pointer to [potentially] new root
*/

template <class TYPE>
shared_ptr<NODE<TYPE> > AvlTree<TYPE> 
         ::  _insert (shared_ptr<NODE<TYPE> > root, 
                      shared_ptr<NODE<TYPE> > newPtr, 
                      bool&        taller)
{
//	Statements   
	if (!root)
	   {
	    root    = newPtr;
	    taller  = true;
	    return  root;
	   } //  if NULL tree 
 
	if (newPtr->getData() < root->getData())
	   {
	    root->setLeftSharedPtr(_insert(root->getLeftSharedPtr(), 
	                         newPtr, 
	                         taller));
	    if (taller)
	       //  Left subtree is taller 
	       switch (root->getBalance())
	          {
	           case LH: // Was left high--rotate 
	                    root = leftBalance (root, taller);
	                    break;

	           case EH: // Was balanced--now LH 
	                    root->setBalance(LH);
	                    break;

	           case RH: // Was right high--now EH 
	                    root->setBalance(EH);
	                    taller    = false;
	                    break;
	          } // switch 
	   } //  new < node 
	else 
	   //  new data >= root data 
	   {
	    root->setRightSharedPtr(_insert (root->getRightSharedPtr(), 
	                           newPtr, 
	                           taller));
	    if (taller)
	       // Right subtree is taller
	       switch (root->getBalance())
	           {
	            case LH: // Was left high--now EH 
	                     root->setBalance(EH);
	                     taller    = false;
	                     break;
	            
	            case EH: // Was balanced--now RH
	                     root->setBalance(RH);
	                     break;
	
	            case RH: // Was right high--rotate 
	                     root = rightBalance (root, taller);
	                     break;
	           } //  switch 
	   } //  else new data >= root data 
	return root;
}	//  _insert 

/*	===================== leftBalance ===================== 
	The tree is out of balance to the left. This function 
	rotates the tree to the right.
	   Pre     the tree is left high 
	   Post    balance restored  
	   Returns potentially new root 
*/

template <class TYPE>
shared_ptr<NODE<TYPE> > AvlTree<TYPE> 
         :: leftBalance (shared_ptr<NODE<TYPE> > root, 
                         bool&        taller) 
{
// 	Local Definitions 
	shared_ptr<NODE<TYPE> > rightTree;
	shared_ptr<NODE<TYPE> > leftTree;

//	Statements 
	leftTree = root->getLeftSharedPtr();
	switch (leftTree->getBalance())
	   {
	    case LH: // Left High--Rotate Right 
	                root->setBalance(EH);
	                leftTree->setBalance(EH);
	             
	             // Rotate Right 
	                root     = rotateRight (root);
	                taller   = false;
	             break;
	    case EH: // This is an error 
	                cout <<"\n\a\aError in leftBalance\n";
	                exit (100); 
	    case RH: // Right High - Requires double rotation: 
	             // first left, then right 
	                rightTree = leftTree->getRightSharedPtr();
					switch (rightTree->getBalance())
	                   {
	                    case LH: root->setBalance(RH);
	                             leftTree->setBalance(EH);
	                             break;
	                    case EH: root->setBalance(EH);
	                             leftTree->setBalance(EH);
	                             break;
	                    case RH: root->setBalance(EH);
	                             leftTree->setBalance(LH);
	                             break;
	                   } //  switch rightTree 
	             
	                rightTree->setBalance(EH);
	                //  Rotate Left 
	                root->setLeftSharedPtr(rotateLeft (leftTree));
	             
	                // Rotate Right 
	                root    = rotateRight (root);
	                taller  = false;
	   } // switch leftTree
	return root;
}	// leftBalance 

/*	===================== rotateLeft ====================== 
	This function exchanges pointers so as to rotate the  
	tree to the left.
	   Pre  root points to tree to be rotated 
	   Post NODE rotated and new root returned 
*/

template <class TYPE>
shared_ptr<NODE<TYPE> > AvlTree<TYPE> 
          :: rotateLeft (shared_ptr<NODE<TYPE> > root) 
{
//	Local Definitions 
	shared_ptr<NODE<TYPE> > tempPtr;

//	Statements 
	tempPtr        = root->getRightSharedPtr();
	root->setRightSharedPtr(tempPtr->getLeftSharedPtr());
	tempPtr->setLeftSharedPtr(root);

	return tempPtr;
}	//  rotateLeft 

/*	===================== rotateRight ===================== 
	This function exchanges pointers to rotate the tree
	to the right.
	   Pre   root points to tree to be rotated 
	   Post  NODE rotated and new root returned 
*/

template <class TYPE>
shared_ptr<NODE<TYPE> > AvlTree<TYPE> 
          :: rotateRight (shared_ptr<NODE<TYPE> > root) 
{
//	Local Definitions 
	shared_ptr<NODE<TYPE> > tempPtr;

//	Statements 
	tempPtr         = root->getLeftSharedPtr();
	root->setLeftSharedPtr(tempPtr->getRightSharedPtr());
	tempPtr->setRightSharedPtr(root);

	return tempPtr;
}	//  rotateRight  

/*	====================  rightBalance ===================
	The tree is out-of-balance to the right. This function 
	rotates the tree to the left.
	   Pre     The tree is right high 
	   Post    Balance restored 
	   Returns potentially new root 
*/

template <class TYPE>
shared_ptr<NODE<TYPE> > AvlTree<TYPE> 
         :: rightBalance (shared_ptr<NODE<TYPE> > root, bool& taller) 
{

//	Local Definitions 
	shared_ptr<NODE<TYPE> > rightTree;
	shared_ptr<NODE<TYPE> > leftTree;
 
//	Statements 
	rightTree = root->getRightSharedPtr();
	switch (rightTree->getBalance())
	   {
	    case LH: // Left High - Requires double rotation:
	             //             first right, then left 
	                leftTree = rightTree->getLeftSharedPtr();
  	 	 			
	                //  First Rotation - Left 
	                switch (leftTree->getBalance()) 
	                   {
	                    case RH: root->setBalance(LH);
	                             rightTree->setBalance(EH);
	                             break;

	                    case EH: root->setBalance(EH);
	                             rightTree->setBalance(EH);
	                             break;
	
	                    case LH: root->setBalance(EH);
	                             rightTree->setBalance(RH);
	                             break;
	                   } //  switch 
  	 	 			leftTree->setBalance(EH);
  	 	 			
	                root->setRightSharedPtr(rotateRight (rightTree));
	                root          = rotateLeft  (root);
	                taller        = false;
	                break;

	     case EH: // Deleting from balanced node 
				     root->setBalance(EH);
	                 taller    = false;
	                 break;
						 	 
   	 	 case RH: // Right High - Rotate Left 
	                 root->setBalance(EH);
	                 rightTree->setBalance(EH);
	                 root            = rotateLeft (root);
	                 taller          = false;
	                 break;
  	 	} // switch 
	return root;
}	//  rightBalance   

/*	====================== AVL_Delete ====================== 
	This function deletes a node from the tree and rebalances 
	it if necessary. 
	   Pre    dltKey contains key to be deleted
	   Post   the node is deleted and its space recycled
	          -or- an error code is returned 
	   Return success (true) or not found (false)
*/

template <class TYPE>
bool  AvlTree <TYPE> :: AVL_Delete (TYPE  dltKey)
{
//	Local Definitions 
	bool shorter;
	bool success;

	shared_ptr<NODE<TYPE> > newRoot;

//	Statements 
	newRoot = _delete (tree, dltKey, shorter, success);
	if (success)
	   {
	    tree = newRoot;
	    count--;
	   } // if 
	return success;
}	// AVL_Delete

/*	======================== _delete ======================= 
	This function deletes a node from the tree and rebalances 
	it if necessary. 
	   Pre    dltKey contains key of node to be deleted
	          shorter is Boolean indicating tree is shorter
	   Post   the node is deleted and its space recycled
	          -or- if key not found, tree is unchanged 
	   Return true if deleted, false if not found
	          pointer to root
*/

template <class TYPE>
shared_ptr<NODE<TYPE> >  AvlTree<TYPE> 
          :: _delete (shared_ptr<NODE<TYPE> > root, 
                      TYPE       dltKey,
                      bool&       shorter,
                      bool&       success) 
{
//  Local Definitions 
	shared_ptr<NODE<TYPE> > dltPtr;
	shared_ptr<NODE<TYPE> > exchPtr;
	shared_ptr<NODE<TYPE> > newRoot;
 
// 	Statements 
	if (!root)
	   {
	    shorter = false;
	    success = false;
	    return NULL;
	   } //  if -- base case 
	
	if (dltKey < root->getData())
	    {
	     root->setLeftSharedPtr(_delete (root->getLeftSharedPtr(), dltKey, 
	                           shorter,    success));
	     if (shorter)
	         root   = dltRightBalance (root, shorter);
	    } // if less 
	else if (dltKey > root->getData())
	    {
	     root->setRightSharedPtr(_delete (root->getRightSharedPtr(), dltKey,
	                            shorter,     success));
	     if (shorter)
	         root = dltLeftBalance (root, shorter);
	    } //  if greater 
	else
	    //  Found equal node 
	    {
	     dltPtr  = root;
	     if (!root->getRightSharedPtr())
	         // Only left subtree 
	         {
	          newRoot  = root->getLeftSharedPtr();
	          success  = true;
	          shorter  = true;
	          return newRoot;            //  base case 
	         } //  if true 
	     else
	         if (!root->getLeftSharedPtr())
	             //  Only right subtree 
	             {
	              newRoot  = root->getRightSharedPtr();
	              success  = true;
	              shorter  = true;
	              return newRoot;        // base case 
	            } //  if 
	         else
	             //  Delete NODE has two subtrees 
	             {
	              exchPtr = root->getLeftSharedPtr();
	              while (exchPtr->getRightSharedPtr())
	                  exchPtr = exchPtr->getRightSharedPtr();
	                  
	              root->setData(exchPtr->getData());
	              root->setLeftSharedPtr(_delete (root->getLeftSharedPtr(), 
	                                    exchPtr->getData(),
	                                    shorter, 
	                                    success)); 
	              if (shorter)
	                  root = dltRightBalance (root, shorter); 
	             } //  else 
	
	    } // equal node 
	return root; 
}	// _delete 

  // ================== dltLeftBalance ==================
/*	The tree is out-of-balance to the left (left high)--
	rotates the tree to the right.
	   Pre     The tree is left high 
	   Post    Balance has been restored 
	   Returns potentially new root 
*/

template <class TYPE>
shared_ptr<NODE<TYPE> > AvlTree<TYPE> 
        ::  dltLeftBalance  (shared_ptr<NODE<TYPE> > root,
                             bool&        smaller) 
{
//	Local Definitions 
	shared_ptr<NODE<TYPE> > rightTree;
	shared_ptr<NODE<TYPE> > leftTree;
  	 
//	Statements 
	switch (root->getBalance())
	    {
	     case RH:  root->setBalance(EH);
	               break;
  	 	 			
	     case EH:  // Delete occurred on right 
		           root->setBalance(LH);
		           smaller    = false;
		           break;
						  	 	 
	     case LH:  leftTree = root->getLeftSharedPtr();
		           switch (leftTree->getBalance()) 
		              {
		               case LH:
		               case EH: // Rotate Single Left 
		                        if (leftTree->getBalance()  == EH)
		                           {
		                            root->setBalance(LH);
		                            leftTree->setBalance(RH);
		                            smaller       = false;
		                           } // if
		                        else
		                           {
		                            root->setBalance(EH);
		                            leftTree->setBalance(EH);
		                           } // else

		                        root = rotateRight (root);
		                        break;
  	 	 				 			
		               case RH:	//Double Rotation 
							   rightTree = leftTree->getRightSharedPtr();
		                        switch (rightTree->getBalance())
		                           {
		                            case LH: root->setBalance(RH);
		                                     leftTree->setBalance(EH);
		                                     break;
		                            case EH: root->setBalance(EH);
		                                     leftTree->setBalance(EH);
		                                     break;
		                            case RH: root->setBalance(EH);
		                                     leftTree->setBalance(LH);
		                                     break;
		                           } //  switch 
		                        rightTree->setBalance(EH);
		                        root->setLeftSharedPtr(rotateLeft (leftTree));
		                        root           = rotateRight (root);
		                        break;
		              } //  switch : leftTree->bal  
  	 	 				   	 	 			
 	 	} //  switch : root->bal 
	return root; 
}	// dltLeftBalance 

/*	=================== dltRightBalance ==================  
	The tree is shorter after a delete on the left. 
	Adjust the balance factors and rotate the tree 
	to the left if necessary.
	   Pre     the tree is shorter 
	   Post    balance factors adjusted and balance restored 
	   Returns potentially new root 
*/

template <class TYPE>
shared_ptr<NODE<TYPE> > AvlTree<TYPE> 
         ::  dltRightBalance (shared_ptr<NODE<TYPE> > root, 
                              bool&        shorter) 
{
//  Local Definitions 
	shared_ptr<NODE<TYPE> > rightTree;
	shared_ptr<NODE<TYPE> > leftTree;
	
//	Statements
	switch (root->getBalance())
	   {
	    case LH: //  Deleted Left--Now balanced 
	             root->setBalance(EH);
	             break;
	    case EH: //  Now Right high 
	             root->setBalance(RH);
	             shorter    = false;
	             break;
	    case RH: //  Right High - Rotate Left 
	             rightTree = root->getRightSharedPtr();
	             if (rightTree->getBalance() == LH)
	                 // Double rotation required 
	                 {
	                  leftTree  = rightTree->getLeftSharedPtr();
	               
	                  switch (leftTree->getBalance())
	                      {
	                       case LH: rightTree->setBalance(RH);
	                                root->setBalance(EH);
	                                break;
	                       case EH: root->setBalance(EH);
	                                rightTree->setBalance(EH);
	                                break;
	                       case RH: root->setBalance(LH);
	                                rightTree->setBalance(EH);
	                                break;
	                      } // switch 
	                   
	                  leftTree->setBalance(EH);
	               
	                  // Rotate Right then Left 
	                  root->setRightSharedPtr(rotateRight (rightTree));
	                  root        = rotateLeft  (root);
	                 } //  if rightTree->bal == LH 
	             else
	                {
	                 //  Single Rotation Only 
	                 switch (rightTree->getBalance())
	                    {
	                     case LH:
	                     case RH: root->setBalance(EH);
	                              rightTree->setBalance(EH);
	                              break;
	                     case EH: root->setBalance(RH);
	                              rightTree->setBalance(LH);
	                              shorter        = false;
	                              break;
	                    } // switch rightTree->bal 
	                 root = rotateLeft (root);
	                } // else 
	    } //  switch root bal 
	return root;
}	//  dltRightBalance 

/*	==================== AVL_Retrieve ===================  
	Retrieve node searches the tree for the node containing 
	the requested key and returns pointer to its data.
	   Pre     dataOut is variable to receive data
	   Post    tree searched and data returned
	   Return  true if found, false if not found
*/

template <class TYPE>
bool   AvlTree<TYPE> 
   ::  AVL_Retrieve  (TYPE   key, TYPE& dataOut)
{
//	Local Definitions
	shared_ptr<NODE<TYPE> > node;
	
//	Statements 
	if (!tree)
	   return false;
	   
	node    = _retrieve (key, tree);
	if (node)
	   {
	    dataOut = node->getData();
	    return true;
	   } // if found
	else
	   return false;
}	//  AVL_Retrieve 

/*	===================== _retrieve ===================== 
	Retrieve searches tree for node containing requested 
	key and returns its data to the calling function.
	   Pre     AVL_Retrieve called: passes key to be located 
	   Post    tree searched and data pointer returned 
	   Return  address of matching node returned 
	           if not found, NULL returned 
*/

template <class TYPE>
shared_ptr<NODE<TYPE> > AvlTree<TYPE> 
        ::  _retrieve (TYPE       key, 
                       shared_ptr<NODE<TYPE> > root)
{
//	Statements 
	if (root)
	    {
	     if (key < root->getData())
	         return _retrieve (key, root->getLeftSharedPtr());
	     else if (key > root->getData())
	         return _retrieve (key, root->getRightSharedPtr());
	     else
	         // Found equal key 
	         return (root);
	    } // if root 
	else
	    //Data not in tree 
	    return root;
}	//  _retrieve 

/*	==================== AVL_Traverse ==================== 
	Process tree using inorder traversal. 
	   Pre   process used to "visit" nodes during traversal 
	   Post  all nodes processed in LNR (inorder) sequence 
*/

template <class TYPE>
void  AvlTree<TYPE> 
  ::  AVL_Traverse (void (*process)(TYPE dataProc))
{
//	Statements 
	_traversal (process, tree);
	return;
}	// end AVL_Traverse 

/*	===================== _traversal ===================== 
	Traverse tree using inorder traversal. To process a
	node, we use the function passed when traversal is called.
	   Pre   tree has been created (may be null) 
	   Post  all nodes processed 
*/

template <class TYPE>
void  AvlTree<TYPE> 
  ::  _traversal (void(*process)(TYPE dataproc),
                  shared_ptr<NODE<TYPE> > root)
{
//	Statements 
	if (root)
	   {
	    _traversal  (process, root->getLeftSharedPtr());
	    process     (root->getData());
	    _traversal  (process, root->getRightSharedPtr());
	   } //  if 
	return;
}	//  _traversal 

/*	=================== AVL_Empty ==================	
	Returns true if tree is empty, false if any data.
	   Pre      tree has been created; may be null 
	   Returns  true if tree empty, false if any data 
*/

template <class TYPE>
bool   AvlTree<TYPE> ::  AVL_Empty (void)
{
//	Statements 
	return (count == 0);
}	//  AVL_Empty 

/*	=================== AVL_Full =================== 
	If there is no room for another node, returns true.
	   Pre      tree has been created
	   Returns  true if no room, false if room 
*/

template <class TYPE>
bool   AvlTree<TYPE> ::  AVL_Full (void)
{
//	Local Definitions 
	shared_ptr<NODE<TYPE> > newPtr;
	
//	Statements 
	newPtr = new NODE<TYPE>;
	if (newPtr)
	   {
	    delete  newPtr;
	    return false;
	   } // if
	else
	   return true;
}	//  AVL_Full 

/*	=================== AVL_Count ===================
	Returns number of nodes in tree.
	   Pre     tree has been created
	   Returns tree count 
*/

template <class TYPE>
int  AvlTree<TYPE> ::  AVL_Count (void)
{
// 	Statements 
	return (count);
}	//  AVL_Count 

/*	=================== Destructor =================== 
	Deletes all data in tree and recycles memory.
	The nodes are deleted by calling a recursive
	function to traverse the tree in inorder sequence.
	   Pre      tree is a pointer to a valid tree 
	   Post     all data have been deleted 
*/

template <class TYPE>
AvlTree<TYPE> :: ~AvlTree  (void) 
{
//	Statements 
	if (tree.get())
	   tree = nullptr;
}	// Destructor

/*	=================== _destroyAVL =================== 
	Deletes all data in tree and recycles memory.
	The nodes are deleted by calling a recursive
	function to traverse the tree in postorder sequence.   
	   Pre   tree is being destroyed 
	   Post  all data have been deleted 
*/

template <class TYPE>
void  AvlTree<TYPE> 
  ::  _destroyAVL (shared_ptr<NODE<TYPE> > root)
{
//	Statements 
	return;
}	//  _destroyAVL

/*  ============================= AVL_Print ============================= 
	This function prints the AVL tree by calling a recursive inorder 
	traversal. NOTE: THIS IS NOT AN APPLICATION ADT FUNCTION. IT IS 
	USED ONLY FOR DEBUGGING PURPOSES.
	
	To correctly visualize the tree when turned sideways, the actual 
	traversal is RNL.
	Pre	 Tree must be initialized. Null tree is OK.
		 Level is node level: root == 0
	Post Tree has been printed.
*/
template <class TYPE>
void  AvlTree<TYPE> :: AVL_Print (void) 
{
/*  statements */
    _print (tree, 0);
    return;
}   /* AVL_PRINT */

/*  ============================= _print ============================= 
	This function is not a part of the ADT. It is included to verify
	that the tree has been properly built by printing out the tree
	structure.
*/

/*  This function uses recursion to print the tree. At each level, the 
    level number is printed along with the node contents (an integer).
    Pre		root is the root of a tree or subtree
            level is the level of the tree: tree root is 0
    Post    Tree has been printed.
*/
template <class TYPE>
void  AvlTree<TYPE> ::  _print (shared_ptr<NODE<TYPE> > root,
                                       int         level) 
{
 /* Local Definitions */
 	int i;
 	
 /* Statements */
 	if (root)
 		{
		 _print ( root->right, level + 1 );
	 
	     cout << "bal "     << setw(3) << root->bal 
	          << ": Level " << setw(3) << level;

 		 for (i = 0; i <= level; i++ )
 		 	cout << "....";
  		 cout << setw(3) << root->data.key;
  		 if (root->bal == LH)
  		    cout << " (LH)\n";
  		 else if (root->bal == RH)
  		    cout << " (RH)\n";
  		 else
  		    cout << " (EH)\n";
  		    
 		 _print ( root->left, level + 1 );
 		} /* if */
 
 } /* AVL_Print */

template<class TYPE>
shared_ptr<NODE<TYPE> > NODE<TYPE>::getLeftSharedPtr() const
{
	return left;
}

template<class TYPE>
shared_ptr<NODE<TYPE> > NODE<TYPE>::getRightSharedPtr() const
{
	return right;
}

template<class TYPE>
void NODE<TYPE>::setLeftSharedPtr(shared_ptr<NODE<TYPE> > leftPtr)
{
	left = leftPtr;
}

template<class TYPE>
void NODE<TYPE>::setRightSharedPtr(shared_ptr<NODE<TYPE> > rightPtr)
{
	right = rightPtr;
}

template<class TYPE>
int NODE<TYPE>::getBalance() const
{
	return bal;
}

template<class TYPE>
void NODE<TYPE>::setBalance(int balance)
{
	bal = balance;
}

template<class TYPE>
TYPE NODE<TYPE>::getData() const
{
	return data;
}

template<class TYPE>
void NODE<TYPE>::setData(TYPE theData)
{
	data = theData;
}
