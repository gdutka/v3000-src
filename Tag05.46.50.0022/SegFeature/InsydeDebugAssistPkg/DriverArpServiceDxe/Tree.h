/** @file
//
//Brief description of file's purpose. Detailed description of file's purpose.
//
//;******************************************************************************
//;* Copyright (c) 2014-2014, Insyde Software Corporation. All Rights Reserved.
//;*
//;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
//;* transmit, broadcast, present, recite, release, license or otherwise exploit
//;* any part of this publication in any form, by any means, without the prior
//;* written permission of Insyde Software Corporation.
//;*
//;******************************************************************************
//
//
//other copy right statement
*/

#ifndef __UEFI_TREE_H__
#define __UEFI_TREE_H__

//#include <Base.h>

//#include <Protocol/LoadedImage.h>

#pragma pack(1)

typedef enum {
  RetrieveEqual     = 0,
  RetrieveLeftNode,
  RetrieveRightNode,
  RetrieveMax
} RETRIEVE_DIRECTION;

typedef struct _TREE_NODE_ {
  UINTN                                 NumberOfNodes;
  UINTN                                 Depth;
  UINTN                                 Operation;
  struct _TREE_NODE_                    *Parent;
  struct _TREE_NODE_                    *Left;
  struct _TREE_NODE_                    *Right;
} TREE_NODE;

typedef RETRIEVE_DIRECTION              COMPARE_NODE (TREE_NODE *Node1, TREE_NODE *Node2);
typedef EFI_STATUS                      DESTROY_CONTENTS (TREE_NODE *Ref);

#pragma pack()

/**

Brief-description of function.
Extended description of function.

@param[in]             para1             description
@param[in, out]        para2             description

@retval                EFI_SUCCESS       Function complete successfully.
@retval                EFI_SUCCESS       Function error.
@retval                NONE

**/
UINTN
EFIAPI
GetTreeDepth (
  IN  TREE_NODE                         *Ref
  );

/**

Brief-description of function.
Extended description of function.

@param[in]             para1             description
@param[in, out]        para2             description

@retval                EFI_SUCCESS       Function complete successfully.
@retval                EFI_SUCCESS       Function error.
@retval                NONE

**/
BOOLEAN
EFIAPI
IsBalanceTree (
  IN  TREE_NODE                         *Ref
  );

//
// ----------------------------------------------------------------------------
//

/**

Brief-description of function.
Extended description of function.

@param[in]             para1             description
@param[in, out]        para2             description

@retval                EFI_SUCCESS       Function complete successfully.
@retval                EFI_SUCCESS       Function error.
@retval                NONE

**/
EFI_STATUS
EFIAPI
DestroyContents (
  IN  TREE_NODE                         *Ref
  );

/**

Brief-description of function.
Extended description of function.

@param[in]             para1             description
@param[in, out]        para2             description

@retval                EFI_SUCCESS       Function complete successfully.
@retval                EFI_SUCCESS       Function error.
@retval                NONE

**/
RETRIEVE_DIRECTION
EFIAPI
CompareNode (
  IN  TREE_NODE                         *Node1,
  IN  TREE_NODE                         *Node2
  );

/**

Brief-description of function.
Extended description of function.

@param[in]             para1             description
@param[in, out]        para2             description

@retval                EFI_SUCCESS       Function complete successfully.
@retval                EFI_SUCCESS       Function error.
@retval                NONE

**/
VOID
EFIAPI
InitializeTreeFunctions (
  IN  COMPARE_NODE                      *CompareFun,
  IN  DESTROY_CONTENTS                  *DestroyFun
  );

//
// ----------------------------------------------------------------------------
//

///**
//
//Brief-description of function.
//Extended description of function.
//
//@param[in]             para1             description
//@param[in, out]        para2             description
//
//@retval                EFI_SUCCESS       Function complete successfully.
//@retval                EFI_SUCCESS       Function error.
//@retval                NONE
//
//**/
//TREE_NODE *
//EFIAPI
//AllocTreeNode (
//  VOID
//  );

/**

Brief-description of function.
Extended description of function.

@param[in]             para1             description
@param[in, out]        para2             description

@retval                EFI_SUCCESS       Function complete successfully.
@retval                EFI_SUCCESS       Function error.
@retval                NONE

**/
INTN
EFIAPI
GetTreeSkewFactor (
  IN  TREE_NODE                         *Ref
  );

/**

Brief-description of function.
Extended description of function.

@param[in]             para1             description
@param[in, out]        para2             description

@retval                EFI_SUCCESS       Function complete successfully.
@retval                EFI_SUCCESS       Function error.
@retval                NONE

**/
TREE_NODE *
EFIAPI
AvlTree (
  IN  TREE_NODE                         *Ref
  );

/**

Brief-description of function.
Extended description of function.

@param[in]             para1             description
@param[in, out]        para2             description

@retval                EFI_SUCCESS       Function complete successfully.
@retval                EFI_SUCCESS       Function error.
@retval                NONE

**/
BOOLEAN
EFIAPI
IsChildNode (
  IN CONST TREE_NODE                    *Ref,
  IN CONST TREE_NODE                    *ChildNode
  );

/**

Brief-description of function.
Extended description of function.

@param[in]             para1             description
@param[in, out]        para2             description

@retval                EFI_SUCCESS       Function complete successfully.
@retval                EFI_SUCCESS       Function error.
@retval                NONE

**/
EFI_STATUS
EFIAPI
AddTree (
  IN  TREE_NODE                         **Ref,
  IN  TREE_NODE                         *NewNode
  );

/**

Brief-description of function.
Extended description of function.

@param[in]             para1             description
@param[in, out]        para2             description

@retval                EFI_SUCCESS       Function complete successfully.
@retval                EFI_SUCCESS       Function error.
@retval                NONE

**/
EFI_STATUS
EFIAPI
DeleteTree (
  IN  TREE_NODE                         **Ref,
  IN  TREE_NODE                         *Target
  );

/**

Brief-description of function.
Extended description of function.

@param[in]             para1             description
@param[in, out]        para2             description

@retval                EFI_SUCCESS       Function complete successfully.
@retval                EFI_SUCCESS       Function error.
@retval                NONE

**/
TREE_NODE *
EFIAPI
RetrieveTree (
  IN  TREE_NODE                         *Ref,
  IN  TREE_NODE                         *Target
  );

/**

Brief-description of function.
Extended description of function.

@param[in]             para1             description
@param[in, out]        para2             description

@retval                EFI_SUCCESS       Function complete successfully.
@retval                EFI_SUCCESS       Function error.
@retval                NONE

**/
EFI_STATUS
EFIAPI
DestroyTree (
  IN  TREE_NODE                         *Ref
  );

/**

Brief-description of function.
Extended description of function.

@param[in]             para1             description
@param[in, out]        para2             description

@retval                EFI_SUCCESS       Function complete successfully.
@retval                EFI_SUCCESS       Function error.
@retval                NONE

**/
TREE_NODE *
EFIAPI
GetPostOrderArray (
  IN  TREE_NODE                         *Ref
  );

/*
void inOrderRec(TREEPTR tree){
  if(tree!=NULL){    
    inOrderRec(tree->left);
    printf("\n %d",tree->info);
    inOrderRec(tree->right);
  }  
}

void preOrderRec(TREEPTR tree){
  if(tree!=NULL){
    printf("\n %d",tree->info);
    preOrderRec(tree->left);    
    preOrderRec(tree->right);
  }  
}

void postOrderRec(TREEPTR tree){
  if(tree!=NULL){
    postOrderRec(tree->left);    
    postOrderRec(tree->right);
    printf("\n %d",tree->info);
    
  }  
}

void inOrderIterative(TREEPTR tree){
  
  STACKPTR stackTop = NULL;
  
  TREEPTR current = tree;

  do{
    while(current!=NULL){
      pushStack(&stackTop,(void *)current);
      current=current->left;
    }
    
    if(!isEmptyStack(stackTop)){
      current = popStack(&stackTop);
      printf("\n%d", current->info);
      current=current->right;
    }
  }while(!isEmptyStack(stackTop) || current!=NULL);
}


void preOrderIterative(TREEPTR tree){
  
  STACKPTR stackTop = NULL;
  
  TREEPTR current = tree;

  do{
    while(current!=NULL){
      printf("\n%d", current->info);
      pushStack(&stackTop,(void *)current->right);
      current=current->left;
    }
    
    if(!isEmptyStack(stackTop)){
      current = popStack(&stackTop);    
    }
  }while(!isEmptyStack(stackTop) || current!=NULL);
}


void postOrderIterative(TREEPTR tree){
  
  STACKPTR stackTop = NULL;
  
  TREEPTR current = tree;
  
  do{
    while(current!=NULL){
      pushStack(&stackTop,current);
      current=current->left;
    }
    if(!isEmptyStack(stackTop)){
      current=popStack(&stackTop);
      if(current->right==NULL){
        printf("\n%d",current->info);
        current=current->right;
      }
      else{
        if(current->isVisited==true){
          printf("\n%d",current->info);
          current=NULL;
        }else{
          current->isVisited=true;
          pushStack(&stackTop,current);
          current=current->right;
        }
      }
    }
  }while(!isEmptyStack(stackTop) || current!=NULL);
    
}
*/

#endif // end of #define __UEFI_TREE_H__

