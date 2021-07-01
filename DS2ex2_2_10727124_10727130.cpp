// 10727130 ¶À·N³Ô 10727124 ¼B¦t§Ê

#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <time.h>
#include <vector>
#include <cmath>
#include <math.h>
#include <stack>
using namespace std ;

typedef char Char1000[1000] ;

struct College {
  int num ;
  string schoolname ;
  string departmentname ;
  string dayornight ;
  string level ;
  int graduaters ;
};

struct Record {     // record AVLtree ;
  vector<College> data ;
  int heights ;
  int nodes ;
};

vector<College> forAllList ;
Record record ;
int m3counts = 1 ;

class TwoThreeTree{

struct InTreeNode {
  vector<int> rSet ;
  string key ;
};

struct TreeNode {
  InTreeNode data[2] ;
  TreeNode *link[3] ;
  TreeNode *parent ;
};

struct TreeSearch {
  TreeNode *parentNode ;
  int parentIndex ;
};

struct Block {
  InTreeNode slot ;
  TreeNode *link ;
};

  TreeNode *root = NULL ;
  int nodes = 0 ;

public :

College setData( Char1000 cStr, int num ) {
  College temp ;
  string buffer, cut ;
  int tabN = 0 ;
  int indexOfTab = 0 ;
  int i = 0 ;

  temp.num = num ;

  buffer.assign( cStr ) ;     // cStr to string ;

  while ( tabN < 10 ) {
    indexOfTab = buffer.find_first_of( '\t', i ) ;

    if ( tabN == 1 ) {
      temp.schoolname = buffer.substr( i, indexOfTab-i ) ;
    }
    else if ( tabN == 3 ) {
      temp.departmentname = buffer.substr( i, indexOfTab-i ) ;
    }
    else if ( tabN == 4 ) {
      temp.dayornight = buffer.substr( i, indexOfTab-i ) ;
    }
    else if ( tabN == 5 ) {
      temp.level = buffer.substr( i, indexOfTab-i ) ;
    }
    else if ( tabN == 8 ) {
      cut = buffer.substr( i, indexOfTab-i ) ;
      temp.graduaters = atoi( cut.c_str() ) ;
    }

    i = indexOfTab+1 ;
    tabN++ ;
  }
  return temp ;
}

void read() {
  FILE *infile ;
  string filenum, filename ;

  cout << endl << "Input a file number: " ;
  cin >> filenum ;
  filename = "input" + filenum + ".txt" ;

  bool isSet = false ;
  while ( !isSet ) {
    infile = fopen( filename.c_str() , "r" ) ;

    if ( infile == NULL ) {
      cout << endl << filename << " does not exit. Try again!" << endl ;
      cout << endl << "Input a file number: " ;
      cin >> filenum ;
      filename = "input" + filenum + ".txt" ;
    }
    else {
      Char1000 cStr ;

      fscanf( infile, "%[^\n]%*c", &cStr ) ;     // three-line title ;
      fscanf( infile, "%[^\n]%*c", &cStr ) ;
      fscanf( infile, "%[^\n]%*c", &cStr ) ;

      for ( int num = 1 ; fscanf( infile, "%[^\n]%*c", &cStr ) != EOF ; num++ ) {
        forAllList.push_back( setData( cStr, num ) ) ;
      }

      fclose( infile ) ;
      isSet = true ;
    }
  }
}

void insert23Tree( string newKey, int newRid ) {
  InTreeNode newSlot ;
  newSlot.rSet.push_back( newRid ) ;
  newSlot.key = newKey ;

  if ( root == NULL ) {
    root = createNode( NULL, NULL, NULL, newSlot ) ;
  }
  else {
    stack<TreeSearch> aPath ;
    TreeSearch curP ;
    Block blockUp ;

    searchPath( newKey, aPath ) ;

    if ( !aPath.empty() ) {
      curP = aPath.top() ;

      if ( ( curP.parentNode->data[curP.parentIndex].rSet.size() != 0 ) &&
        ( newKey.compare( curP.parentNode->data[curP.parentIndex].key ) == 0 ) ) {
        curP.parentNode->data[curP.parentIndex].rSet.push_back( newRid ) ;
      }
      else if ( curP.parentNode->data[1].rSet.size() == 0 ) {
        insertLeaf( newSlot, curP ) ;
      }
      else {
        splitLeaf( newSlot, curP, blockUp ) ;

        if ( curP.parentNode->parent == NULL ) {     // root split ;
          root = createRoot( curP.parentNode, blockUp.link, blockUp.slot ) ;
        }
        else {
          while ( 1 ) {     // until the insertion is stopped ;
            aPath.pop() ;
            curP = aPath.top() ;

            if ( curP.parentNode->data[1].rSet.size() == 0 ) {
              insertNonLeaf( blockUp, curP ) ;
              break ;
            }
            else {
              splitNonLeaf( curP, blockUp ) ;

              if ( curP.parentNode->parent == NULL ) {
                root = createRoot( curP.parentNode, blockUp.link, blockUp.slot ) ;
                break ;
              }
            }
          }
        }
      }
    }
  }
}

TreeNode *createNode( TreeNode *left, TreeNode *right, TreeNode *parent, InTreeNode newSlot ) {
  TreeNode *newNode = NULL ;

  newNode = new TreeNode ;
  newNode->data[0].rSet = newSlot.rSet ;
  newNode->data[0].key = newSlot.key ;
  newNode->data[1].rSet.clear() ;
  newNode->data[1].key = "" ;
  newNode->parent = parent ;
  newNode->link[0] = left ;
  newNode->link[1] = right ;
  newNode->link[2] = NULL ;
  nodes++ ;

  return newNode ;
}

void searchPath( string newKey, stack<TreeSearch> &aPath ) {
  TreeSearch oneP ;
  int pos ;
  TreeNode *cur = root ;

  while ( cur != NULL ) {
    oneP.parentNode = cur ;

    for ( pos = 0 ; pos < 2 ; pos++ ) {
      if ( ( cur->data[pos].rSet.size() ) == 0 ||
        ( newKey.compare( cur->data[pos].key ) < 0 ) ) {
        break ;
      }
      else if ( newKey.compare( cur->data[pos].key ) == 0 ) {
        oneP.parentIndex = pos ;
        aPath.push( oneP ) ;
        return ;
      }
    }

    oneP.parentIndex = pos ;
    aPath.push( oneP ) ;
    cur = cur->link[pos] ;
  }
}

void insertLeaf( InTreeNode newSlot, TreeSearch curP ) {
  for ( int i = 1 ; i >= curP.parentIndex ; i-- ) {
    if ( i > curP.parentIndex ) {
      curP.parentNode->data[i].rSet = curP.parentNode->data[i-1].rSet ;
      curP.parentNode->data[i].key = curP.parentNode->data[i-1].key ;
    }
    else if ( i == curP.parentIndex ) {
      curP.parentNode->data[i].rSet = newSlot.rSet ;
      curP.parentNode->data[i].key = newSlot.key ;
    }
    else {
      break ;
    }
  }
}

void splitLeaf( InTreeNode newSlot, TreeSearch curP, Block &blockUp ) {
  InTreeNode buf[3] ;
  int index = 0 ;

  for ( int i = 0 ; i < 3 ; i++ ) {
    buf[i].rSet = ( i == curP.parentIndex ) ? newSlot.rSet : curP.parentNode->data[index].rSet ;
    buf[i].key = ( i == curP.parentIndex ) ? newSlot.key : curP.parentNode->data[index++].key ;
  }

  curP.parentNode->data[0].rSet = buf[0].rSet ;
  curP.parentNode->data[0].key = buf[0].key ;

  for ( int i = 1 ; i < 2 ; i++ ) {
    curP.parentNode->data[i].rSet.clear() ;
  }

  blockUp.link = createNode( NULL, NULL, curP.parentNode->parent, buf[2] ) ;
  blockUp.slot.rSet = buf[1].rSet ;
  blockUp.slot.key = buf[1].key ;
}

TreeNode *createRoot( TreeNode *left, TreeNode *right, InTreeNode oneSlot ) {
  TreeNode *newRoot = createNode( left, right, NULL, oneSlot ) ;

  left->parent = newRoot ;
  right->parent = newRoot ;

  return newRoot ;
}

void insertNonLeaf( Block blockUp, TreeSearch curP ) {
  for ( int i = 1 ; i >= curP.parentIndex ; i-- ) {
    if ( i > curP.parentIndex ) {
      curP.parentNode->data[i].rSet = curP.parentNode->data[i-1].rSet ;
      curP.parentNode->data[i].key = curP.parentNode->data[i-1].key ;
    }
    else if ( i == curP.parentIndex ) {
      curP.parentNode->data[i].rSet = blockUp.slot.rSet ;
      curP.parentNode->data[i].key = blockUp.slot.key ;

      if ( curP.parentNode->link[1]->data[0].key.compare( blockUp.link->data[0].key ) > 0 ) {
        curP.parentNode->link[2] = curP.parentNode->link[1] ;
        curP.parentNode->link[1] = blockUp.link ;
      }
      else {
        curP.parentNode->link[2] = blockUp.link ;
      }
    }
    else {
      break ;
    }
  }
}

void splitNonLeaf( TreeSearch curP, Block &blockUp ) {
  InTreeNode buf[3] ;
  TreeNode *ptr[4] ;
  int index = 0 ;

  for ( int i = 0 ; i < 3 ; i++ ) {
    buf[i].rSet = (  i == curP.parentIndex ) ? blockUp.slot.rSet : curP.parentNode->data[index].rSet ;
    buf[i].key = (  i == curP.parentIndex ) ? blockUp.slot.key : curP.parentNode->data[index++].key ;
  }

  ptr[0] = curP.parentNode->link[0] ;
  ptr[1] = curP.parentNode->link[1] ;
  ptr[2] = curP.parentNode->link[2] ;
  ptr[3] = blockUp.link ;

  if ( blockUp.link->data[0].key.compare( curP.parentNode->link[1]->data[0].key ) < 0 ) {     // sort *ptr[4] ;
    ptr[1] = blockUp.link ;
    ptr[2] = curP.parentNode->link[1] ;
    ptr[3] = curP.parentNode->link[2] ;

    curP.parentNode->link[1] = ptr[1] ;
    ptr[1]->parent = curP.parentNode ;
  }
  else if ( blockUp.link->data[0].key.compare( curP.parentNode->link[2]->data[0].key ) < 0 ) {
    ptr[2] = blockUp.link ;
    ptr[3] = curP.parentNode->link[2] ;
  }

  curP.parentNode->data[0].rSet = buf[0].rSet ;
  curP.parentNode->data[0].key = buf[0].key ;

  for ( int i = 1 ; i < 2 ; i++ ) {
    curP.parentNode->data[i].rSet.clear() ;
  }

  curP.parentNode->link[2] = NULL ;

  blockUp.link = createNode( ptr[2], ptr[3], curP.parentNode->parent, buf[2] ) ;
  blockUp.slot.rSet = buf[1].rSet ;
  blockUp.slot.key = buf[1].key ;
}


void free23Tree( TreeNode *root ) {
  if ( root == NULL ) {
   ;
  }
  else if ( root != NULL ) {
    free23Tree( root->link[0] ) ;

    for ( int i = 0 ; i < 2 ; i++ ) {
      if ( root->data[i].rSet.size() == 0 ) {
        break ;
      }
      free23Tree( root->link[i+1] ) ;
    }
    delete root ;
  }
}

int height() {
  int i = 0 ;
  TreeNode *walk = root ;

  while ( walk != NULL ) {
    i++ ;
    walk = walk->link[0] ;
  }
  return i ;
}

void run() {
  int i = 0 ;
  while ( i < forAllList.size() ) {
    insert23Tree( forAllList.at(i).schoolname, forAllList.at(i).num ) ;
    i++ ;
  }
}

void free() {
  free23Tree( root ) ;
}

void print() {
  cout << endl ;
  cout << "Tree heights = " << height() << endl ;
  cout << "Number of nodes = " << nodes << endl ;

  int k = 1 ;
  if ( root->data[1].rSet.size() == 0 ) {
    for ( int j = 0 ; j < root->data[0].rSet.size() ; j++ ) {
      for( int i = 1 ; i <= forAllList.size() ; i++  ) {
        if ( i == root->data[0].rSet.at(j) ) {
          cout << k << ": [" << forAllList.at(i-1).num << "] " ;
          cout <<  forAllList.at(i-1).schoolname << ", " ;
          cout <<  forAllList.at(i-1).departmentname << ", " ;
          cout <<  forAllList.at(i-1).dayornight << ", " ;
          cout <<  forAllList.at(i-1).level << ", " ;
          cout <<  forAllList.at(i-1).graduaters << endl ;
          k++ ;
          break ;
        }
      }
    }
  }
  else if ( root->data[0].rSet.at(0) < root->data[1].rSet.at(0) ) {
    for ( int j = 0 ; j < root->data[0].rSet.size() ; j++ ) {
      for( int i = 1 ; i <= forAllList.size() ; i++  ) {
        if ( i == root->data[0].rSet.at(j) ) {
          cout << k << ": [" << forAllList.at(i-1).num << "] " ;
          cout <<  forAllList.at(i-1).schoolname << ", " ;
          cout <<  forAllList.at(i-1).departmentname << ", " ;
          cout <<  forAllList.at(i-1).dayornight << ", " ;
          cout <<  forAllList.at(i-1).level << ", " ;
          cout <<  forAllList.at(i-1).graduaters << endl ;
          k++ ;
          break ;
        }
      }
    }

    for ( int j = 0 ; j < root->data[1].rSet.size() ; j++ ) {
      for( int i = 1 ; i <= forAllList.size() ; i++  ) {
        if ( i == root->data[1].rSet.at(j) ) {
          cout << k << ": [" << forAllList.at(i-1).num << "] " ;
          cout <<  forAllList.at(i-1).schoolname << ", " ;
          cout <<  forAllList.at(i-1).departmentname << ", " ;
          cout <<  forAllList.at(i-1).dayornight << ", " ;
          cout <<  forAllList.at(i-1).level << ", " ;
          cout <<  forAllList.at(i-1).graduaters << endl ;
          k++ ;
          break ;
        }
      }
    }
  }
  else {
    for ( int j = 0 ; j < root->data[1].rSet.size() ; j++ ) {
      for( int i = 1 ; i <= forAllList.size() ; i++  ) {
        if ( i == root->data[1].rSet.at(j) ) {
          cout << k << ": [" << forAllList.at(i-1).num << "] " ;
          cout <<  forAllList.at(i-1).schoolname << ", " ;
          cout <<  forAllList.at(i-1).departmentname << ", " ;
          cout <<  forAllList.at(i-1).dayornight << ", " ;
          cout <<  forAllList.at(i-1).level << ", " ;
          cout <<  forAllList.at(i-1).graduaters << endl ;
          k++ ;
          break ;
        }
      }
    }

    for ( int j = 0 ; j < root->data[0].rSet.size() ; j++ ) {
      for( int i = 1 ; i <= forAllList.size() ; i++  ) {
        if ( i == root->data[0].rSet.at(j) ) {
          cout << k << ": [" << forAllList.at(i-1).num << "] " ;
          cout <<  forAllList.at(i-1).schoolname << ", " ;
          cout <<  forAllList.at(i-1).departmentname << ", " ;
          cout <<  forAllList.at(i-1).dayornight << ", " ;
          cout <<  forAllList.at(i-1).level << ", " ;
          cout <<  forAllList.at(i-1).graduaters << endl ;
          k++ ;
          break ;
        }
      }
    }
  }
  cout << endl ;
}

};

class AVLtree {

struct InAVLTreeNode {
  vector<int> rSet ;
  int key ;
};

struct AVLTreeNode{
  InAVLTreeNode data ;
  int height ;
  AVLTreeNode *left ;
  AVLTreeNode *right ;
};

  AVLTreeNode *root = NULL ;
  int nodes = 0 ;

public :

int Max( int a, int b ) {
  if ( a > b ) {
    return a ;
  }
  else {
    return b ;
  }
}

int Height( AVLTreeNode *cur ) {
  if ( cur == NULL ) {
    return 0 ;
  }
  else {
    return cur->height ;
  }
}

AVLTreeNode *LLrotate( AVLTreeNode *k1 ) {
  AVLTreeNode *k2 ;

  k2 = k1->left ;
  k1->left = k2->right ;
  k2->right = k1 ;

  k1->height = Max( Height( k1->left ), Height( k1->right ) ) + 1 ;
  k2->height = Max( Height( k2->left ), k1->height) + 1 ;

  return k2 ;
}

AVLTreeNode *RRrotate( AVLTreeNode *k1 ) {
  AVLTreeNode *k2 ;

  k2 = k1->right ;
  k1->right = k2->left ;
  k2->left = k1 ;

  k1->height = Max( Height( k1->left ), Height( k1->right ) ) + 1 ;
  k2->height = Max( Height( k2->right ), k1->height ) + 1 ;

  return k2 ;
}

AVLTreeNode *LRrotate( AVLTreeNode *k1 ) {
  k1->left = RRrotate( k1->left ) ;

  return LLrotate( k1 ) ;
}

AVLTreeNode *RLrotate( AVLTreeNode *k1 ) {
  k1->right = LLrotate( k1->right ) ;

  return RRrotate( k1 ) ;
}

AVLTreeNode *insertAVLTree( AVLTreeNode *root, int gra, int id ) {
  if ( root == NULL ) {     // create root ;
    root = new AVLTreeNode() ;
    root->data.key = gra ;
    root->data.rSet.push_back( id ) ;
    root->left = NULL ;
    root->right = NULL ;
    nodes++ ;
  }
  else if ( gra < root->data.key ) {
    root->left = insertAVLTree( root->left, gra, id ) ;

    if ( Height( root->left ) - Height( root->right ) == 2 ) {
      if ( gra < root->left->data.key ) {
        root = LLrotate( root ) ;
      }
      else {
        root = LRrotate( root ) ;
      }
    }
  }
  else if ( gra > root->data.key ) {
    root->right = insertAVLTree( root->right, gra, id );

    if ( Height( root->right ) - Height( root->left ) == 2) {
      if ( gra > root->right->data.key ) {
        root = RRrotate( root ) ;
      }
      else {
        root = RLrotate( root ) ;
      }
    }
  }
  else {
    root->data.rSet.push_back( id ) ;
  }

  root->height = Max( Height( root->left ), Height( root->right ) ) + 1 ;

  return root ;

}

void run() {
  int i = 0 ;
  while ( i < forAllList.size() ) {
    root = insertAVLTree( root, forAllList.at(i).graduaters, forAllList.at(i).num ) ;
    i++ ;
  }
}

void print() {
  cout << endl ;
  record.heights = root->height ;
  record.nodes = nodes ;
  cout << "Tree heights = " << root->height << endl ;
  cout << "Number of nodes = " << nodes << endl ;

  int k = 1 ;
  for ( int j = 0 ; j < root->data.rSet.size() ; j++ ) {
    for( int i = 1 ; i <= forAllList.size() ; i++  ) {
      if ( i == root->data.rSet.at(j) ) {
        record.data.push_back( forAllList.at(i-1) ) ;

        cout << k << ": [" << forAllList.at(i-1).num << "] " ;
        cout <<  forAllList.at(i-1).schoolname << ", " ;
        cout <<  forAllList.at(i-1).departmentname << ", " ;
        cout <<  forAllList.at(i-1).dayornight << ", " ;
        cout <<  forAllList.at(i-1).level << ", " ;
        cout <<  forAllList.at(i-1).graduaters << endl ;
        k++ ;
        break ;
      }
    }
  }
  cout << endl ;
}

void freeAVLTree( AVLTreeNode *root ) {
  if ( root == NULL ) {
    ;
  }

  if ( root->left != NULL ) {
    freeAVLTree( root->left ) ;
  }

  if ( root->right != NULL ) {
    freeAVLTree( root->right ) ;
  }

  delete root ;
}

void free() {
  freeAVLTree( root ) ;
}

void m3() {
  int k ;
  int i = 0 ;
  bool check = false ;

  while ( !check ) {
  cout << endl ;
    cout << "Enter K in [1," << forAllList.size() << "] : " ;
    m3counts = 1 ;
    cin >> k ;

    if ( k <= forAllList.size() ) {
      check = true ;
    }
  }

  AVLTreeNode *walk = root ;
  m3order( walk, i, k ) ;
  cout << endl ;
}

void m3order( AVLTreeNode *root, int &i, int k ) {
  if ( root != NULL ) {
    m3order( root->right, i, k ) ;

    AVLTreeNode *temp = root ;
    if ( i < k && m3counts <= k ) {
      i++ ;
      m3print( temp ) ;
    }

    m3order( root->left, i, k ) ;
  }
}

void m3print( AVLTreeNode *temp ) {
  for( int j = 0 ; j < temp->data.rSet.size() ; j++ ) {
    cout << m3counts << ": [" << temp->data.rSet.at(j) << "] " ;
    m3printfromlist( temp->data.rSet.at(j) ) ;
    m3counts++ ;
  }
}

void m3printfromlist( int num ) {
  for( int i = 0 ; i < forAllList.size() ; i++  ) {
    if ( forAllList.at(i).num == num ) {
      cout <<  forAllList.at(i).schoolname << ", " ;
      cout <<  forAllList.at(i).departmentname << ", " ;
      cout <<  forAllList.at(i).dayornight << ", " ;
      cout <<  forAllList.at(i).level << ", " ;
      cout <<  forAllList.at(i).graduaters << endl ;
      break ;
    }
  }
}

};

int main() {
  string choice ;

  cout << "***  Search Tree Utilities  ***" << endl ;
  cout << "* 0. Quit                     *" << endl ;
  cout << "* 1. Build 2-3 tree           *" << endl ;
  cout << "* 2. Build a AVL tree         *" << endl ;
  cout << "* 3. Top-K search on AVL tree *" << endl ;
  cout << "*******************************" << endl ;
  cout << "Input a choice(0, 1, 2, 3): " ;
  cin >> choice ;

  while ( choice != "0" ) {
    if ( choice == "1" ) {
      TwoThreeTree twothreeTree ;
      forAllList.clear() ;
      record.data.clear() ;
      twothreeTree.read() ;
      twothreeTree.run() ;
      twothreeTree.print() ;
      twothreeTree.free() ;
    }
    else if ( choice == "2" ) {
      if ( forAllList.size() == 0 ) {
        cout << endl << "### Choose 1 first. ###" << endl << endl ;
      }
      else if ( record.data.size() == 0 ) {
        AVLtree avlTree ;
        avlTree.run() ;
        avlTree.print() ;
        avlTree.free() ;
      }
      else {
        cout << endl << "### AVL tree has been built. ###" << endl << endl ;

        cout << "Tree heights = " << record.heights << endl ;
        cout << "Number of nodes = " << record.nodes << endl ;

        int k = 1 ;
        for ( int i = 0 ; i < record.data.size() ; i++ ) {
          cout << k << ": [" << record.data.at(i).num << "] " ;
          cout <<  record.data.at(i).schoolname << ", " ;
          cout <<  record.data.at(i).departmentname << ", " ;
          cout <<  record.data.at(i).dayornight << ", " ;
          cout <<  record.data.at(i).level << ", " ;
          cout <<  record.data.at(i).graduaters << endl ;
          k++ ;
        }
        cout << endl ;
      }
    }
    else if ( choice == "3" ) {
      if ( forAllList.size() == 0 ) {
        cout << endl << "### Choose 1 first. ###" << endl << endl ;
      }
      else if ( record.data.size() == 0 ) {
        cout << endl << "### Choose 2 first. ###" << endl << endl ;
      }
      else {
        AVLtree avlTree ;
        avlTree.run() ;
        avlTree.m3() ;
      }
    }
    else {
      cout << endl << "Command does not exit!"  << endl << endl ;
    }

    cout << "***  Search Tree Utilities  ***" << endl ;
    cout << "* 0. Quit                     *" << endl ;
    cout << "* 1. Build 2-3 tree           *" << endl ;
    cout << "* 2. Build a AVL tree         *" << endl ;
    cout << "* 3. Top-K search on AVL tree *" << endl ;
    cout << "*******************************" << endl ;
    cout << "Input a choice(0, 1, 2, 3): " ;
    cin >> choice ;
  }
}
