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

#include <Base.h>
#include <Library/DebugLib.h>

#include "Tree.h"

//#include <Library/MemoryAllocationLib.h>

COMPARE_NODE                            *mCompareNode = CompareNode;
DESTROY_CONTENTS                        *mDestroyContents = DestroyContents;

#if 0
struct node {
  void * element;
  struct node * next;
};

typedef struct node * STACKPTR;

STACKPTR getNode(void * element){
  STACKPTR temp= (STACKPTR)AllocateZeroPool(sizeof(struct node));
  
  temp->element =element;
  temp->next=NULL;
  return temp;
}

void pushStack(STACKPTR * topRef, void * info){
  
  STACKPTR temp = getNode(info);
  if(*topRef==NULL){    
    temp->next=NULL;
    *topRef=temp;
    return;
  }
  temp->next=*topRef;
  *topRef=temp;
}


bool isEmptyStack(STACKPTR top){
  if(top == NULL){
    return true;
  }
  else 
    return false;
}

void * popStack(STACKPTR * topRef){
  if(!isEmptyStack(*topRef)){
    STACKPTR temp = *topRef;
    *topRef = temp->next;
    temp->next=NULL;
    return temp->element;
  }
  assert(0);
}
#endif

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
UINTN
EFIAPI
GetTreeDepth (
  IN  TREE_NODE                         *Ref
  )
{
  UINTN                                 l_Depth, r_Depth;

  if (Ref == NULL) return 0;

  //
  l_Depth = 0;
  r_Depth = 0;

  if (Ref->Left != NULL) l_Depth = (GetTreeDepth (Ref->Left) + 1);
  if (Ref->Right != NULL) r_Depth = (GetTreeDepth (Ref->Right) + 1);

  return (l_Depth > r_Depth ? l_Depth : r_Depth);
}

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
  )
{
  INTN                                  l_Depth, r_Depth;

  if (Ref == NULL) return TRUE;

  //
  l_Depth = GetTreeDepth (Ref->Left);
  r_Depth = GetTreeDepth (Ref->Right);

  return ((BOOLEAN) (ABS (l_Depth - r_Depth) < 2));
}

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
  )
{
  return RETURN_SUCCESS;
}

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
  )
{
  return RetrieveMax;
}

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
  )
{
  if (CompareFun != NULL) {
    mCompareNode = CompareFun;
  }
  if (DestroyFun != NULL) {
    mDestroyContents = DestroyFun;
  }
  return;
}

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
STATIC
VOID
EFIAPI
BubbleUp (
  IN  TREE_NODE                         *Ref
  )
{
  TREE_NODE                             *Parent;
  UINTN                                 l_DataN, r_DataN;

  if (Ref == NULL) return;

  //
  Parent  = Ref->Parent;
//  l_DataN = 0;
//  r_DataN = 0;

  //
  l_DataN = (Ref->Left == NULL) ? 0 : Ref->Left->NumberOfNodes;
  r_DataN = (Ref->Right == NULL) ? 0 : Ref->Right->NumberOfNodes;
  Ref->NumberOfNodes = (l_DataN + r_DataN + 1);

  //
  l_DataN = (Ref->Left == NULL) ? 0 : (Ref->Left->Depth + 1);
  r_DataN = (Ref->Right == NULL) ? 0 : (Ref->Right->Depth + 1);
  Ref->Depth = (l_DataN > r_DataN) ? l_DataN : r_DataN;

  // Update parameters of parent node
  if (Parent != NULL) BubbleUp (Parent);

  return;
}

/**

Brief-description of function.
Extended description of function.

@param[in]             para1             description
@param[in, out]        para2             description

@retval                EFI_SUCCESS       Function complete successfully.
@retval                EFI_SUCCESS       Function error.
@retval                NONE

**/
STATIC
VOID
EFIAPI
SetOperation (
  IN  TREE_NODE                         *Ref
  )
{
  if (Ref == NULL) return;
  if (!Ref->Operation)
    Ref->Operation = 1;
  else
    ASSERT (FALSE);

  return;
}

/**

Brief-description of function.
Extended description of function.

@param[in]             para1             description
@param[in, out]        para2             description

@retval                EFI_SUCCESS       Function complete successfully.
@retval                EFI_SUCCESS       Function error.
@retval                NONE

**/
STATIC
VOID
EFIAPI
ClearOperation (
  IN  TREE_NODE                         *Ref
  )
{
  if (Ref == NULL) return;
  if (Ref->Operation)
    Ref->Operation ^= Ref->Operation;

  return;
}

/**

Brief-description of function.
Extended description of function.

@param[in]             para1             description
@param[in, out]        para2             description

@retval                EFI_SUCCESS       Function complete successfully.
@retval                EFI_SUCCESS       Function error.
@retval                NONE

**/
STATIC
BOOLEAN
EFIAPI
IsOperation (
  IN  TREE_NODE                         *Ref
  )
{
  if (Ref == NULL) return FALSE;

  return (Ref->Operation != 0);
}

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
RotateLeft (
  IN  TREE_NODE                         *Ref
  )
{
  TREE_NODE                             *temp;
  UINTN                                 tmp_Nodes, l_Depth, r_Depth;

  if (Ref == NULL)  return NULL;
  ASSERT (Ref->Right != NULL); // There must have sub-tree @ Ref->Right
  if (Ref->Right == NULL) return Ref;

  //
  temp                = Ref->Right;
  Ref->Right          = temp->Left;
  if (temp->Left != NULL)
    temp->Left->Parent= Ref;
  temp->Left          = Ref;

  //
  tmp_Nodes           = (temp->Right == NULL) ? 0 : temp->Right->NumberOfNodes;

  temp->NumberOfNodes = Ref->NumberOfNodes;
  temp->Parent        = Ref->Parent;

  Ref->NumberOfNodes  = (Ref->NumberOfNodes - tmp_Nodes - 1);
  Ref->Parent         = temp;

  l_Depth             = (Ref->Left == NULL) ? 0 : (Ref->Left->Depth + 1);
  r_Depth             = (Ref->Right == NULL) ? 0 : (Ref->Right->Depth + 1);
  Ref->Depth          = (l_Depth > r_Depth) ? l_Depth : r_Depth;

  l_Depth             = (temp->Left == NULL) ? 0 : (temp->Left->Depth + 1);
  r_Depth             = (temp->Right == NULL) ? 0 : (temp->Right->Depth + 1);
  temp->Depth         = (l_Depth > r_Depth) ? l_Depth : r_Depth;

  return temp;
}

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
RotateRight (
  IN  TREE_NODE                         *Ref
  )
{
  TREE_NODE                             *temp;
  UINTN                                 tmp_Nodes, l_Depth, r_Depth;

  if (Ref == NULL)  return NULL;
  ASSERT (Ref->Left != NULL); // There must have sub-tree @ Ref->Left
  if (Ref->Left == NULL) return Ref;

  //
  temp                = Ref->Left;
  Ref->Left           = temp->Right;

  if (temp->Right != NULL)
    temp->Right->Parent = Ref;

  //
  temp->Right         = Ref;

  //
  tmp_Nodes           = (temp->Left == NULL) ? 0 : temp->Left->NumberOfNodes;

  temp->NumberOfNodes = Ref->NumberOfNodes;
  temp->Parent        = Ref->Parent;

  Ref->NumberOfNodes  = (Ref->NumberOfNodes - tmp_Nodes - 1);
  Ref->Parent         = temp;

  l_Depth             = (Ref->Left == NULL) ? 0 : (Ref->Left->Depth + 1);
  r_Depth             = (Ref->Right == NULL) ? 0 : (Ref->Right->Depth + 1);
  Ref->Depth          = (l_Depth > r_Depth) ? l_Depth : r_Depth;

  l_Depth             = (temp->Left == NULL) ? 0 : (temp->Left->Depth + 1);
  r_Depth             = (temp->Right == NULL) ? 0 : (temp->Right->Depth + 1);
  temp->Depth         = (l_Depth > r_Depth) ? l_Depth : r_Depth;

  return temp;
}

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
  )
{
  TREE_NODE                             *l, *r;
  INTN                                  l_Depth, r_Depth;

  //
  if (Ref == NULL) return 0;

  //
  l = Ref->Left;
  r = Ref->Right;

  l_Depth = (l == NULL) ? 0 : (l->Depth + 1);
  r_Depth = (r == NULL) ? 0 : (r->Depth + 1);

  return (l_Depth - r_Depth);
}

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
  )
{
  INTN                                  SkewFactor;
  TREE_NODE                             *l, *r, *temp;

  //
  if (Ref == NULL) {
    return Ref;
  }

  //
  l     = Ref->Left;
  r     = Ref->Right;
  temp  = Ref;

  //
  SkewFactor = GetTreeSkewFactor (Ref);
  if (SkewFactor == 2) {

    ASSERT (l != NULL); // There must have sub-tree @ Ref->Left
    //
    if (GetTreeSkewFactor (l) == -1) { // "Left Right skew Case"
      temp->Left = RotateLeft (l);
    }
    // "Left Left skew Case"
    temp = RotateRight (Ref);
  }
  else if (SkewFactor == -2) {

    ASSERT (r != NULL); // There must have sub-tree @ Ref->Right
    //
    if (GetTreeSkewFactor (r) == 1) { // "Right Left skew Case"
      temp->Right = RotateRight (r);
    }
    // "Right Right skew Case"
    temp = RotateLeft (Ref);
  }

//[-start-190418-IB05820463-modify]//
  if (temp != NULL) {
  // Update parents' parameters
  if ((temp != Ref) && (temp->Parent != NULL)) BubbleUp (temp->Parent);
  }
//[-end-190418-IB05820463-modify]//
  
  return temp;
}

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
  IN CONST TREE_NODE                    *Child
  )
{
  //
  if ((Ref == NULL) || (Child == NULL)) return FALSE;

  if (Child->Parent == NULL)
    return FALSE;

  if (Child->Parent == Ref)
    return TRUE;
  else
    return IsChildNode (Ref, Child->Parent);
}

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
//  )
//{
//  TREE_NODE                             *temp = NULL;
//
//  temp = (TREE_NODE *) AllocateZeroPool (sizeof (TREE_NODE));
//  if (temp != NULL) {
//    temp->Left      = NULL;
//    temp->Right     = NULL;
//  }
//  return temp;
//}


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
  )
{
  EFI_STATUS                            Status = RETURN_SUCCESS;
  TREE_NODE                             *NewRefNode = NULL;
  RETRIEVE_DIRECTION                    AddDirection = RetrieveMax;

  //
  if (NewNode == NULL) {
    return RETURN_INVALID_PARAMETER;
  }
  if (IsChildNode ((*Ref), NewNode)) {
    return RETURN_SUCCESS;
  }

  //
  if (IsOperation (*Ref)) {
    return RETURN_NOT_READY;
  } else {
    SetOperation (*Ref);
  }

  //
  if (*Ref == NULL) {
    DEBUG ((DEBUG_INFO, "\n"));
    (*Ref) = NewNode;
  }
  //
  else {

    //
    AddDirection = (*mCompareNode) (*Ref, NewNode);
    if (AddDirection == RetrieveLeftNode) {
      //
//      DEBUG ((DEBUG_INFO, "<-, "));
      Status = AddTree (&(*Ref)->Left, NewNode);
      ASSERT_EFI_ERROR (Status);
    }
    //
    else if ((AddDirection == RetrieveRightNode) || (AddDirection == RetrieveEqual)) {
      //
//      DEBUG ((DEBUG_INFO, "->, "));
      Status = AddTree (&(*Ref)->Right, NewNode);
      ASSERT_EFI_ERROR (Status);
    }
    //
    else {
      Status = RETURN_NOT_READY;
    }
  }
  //
  if (!RETURN_ERROR (Status)) {
    //
    // Update numbers and depth of tree
    //
    if (NewNode->NumberOfNodes == 0)  NewNode->NumberOfNodes = 1;

    if ((NewNode == (*Ref)->Left) || (NewNode == (*Ref)->Right))
      NewNode->Parent = (*Ref);

    //
    if ((*Ref) != NewNode)
      (*Ref)->NumberOfNodes = ((*Ref)->NumberOfNodes + NewNode->NumberOfNodes);

    if (AddDirection == RetrieveLeftNode) {
      //
      if (((*Ref)->Left->Depth + 1) > (*Ref)->Depth)
        (*Ref)->Depth = ((*Ref)->Left->Depth + 1);
    }
    //
    else if ((AddDirection == RetrieveRightNode) || (AddDirection == RetrieveEqual)) {
      //
      if (((*Ref)->Right->Depth + 1) > (*Ref)->Depth)
        (*Ref)->Depth = ((*Ref)->Right->Depth + 1);
    }

    // Do AVL self-balancing tree
    NewRefNode = AvlTree (*Ref);
  }
  else {
    // Add DEBUG messages here...
  }

  //
  ClearOperation (*Ref);
  if ((*Ref) != NewRefNode)
    (*Ref) = NewRefNode;

  //
  return Status;
}

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
  )
{
  EFI_STATUS                            Status = RETURN_SUCCESS;
  TREE_NODE                             *Parent = NULL;

  //
  if ((Ref == NULL) || (*Ref == NULL) || (Target == NULL)) {
    return RETURN_INVALID_PARAMETER;
  }

  //
  if (IsChildNode ((*Ref), Target)) {

    // TODO:
//    if (IsOperation (*Ref)) {
//      return RETURN_NOT_READY;
//    } else {
//      SetOperation (*Ref);
//    }

    Parent = Target->Parent;
    Target->Parent  = NULL;

    if (Parent != NULL) {
      //
      if (Parent->Left == Target)
        Parent->Left  = NULL;
      //
      else if (Parent->Right == Target)
        Parent->Right = NULL;

      //
      BubbleUp (Parent);

      // TODO: Do AVL self-balancing tree
//      if (Parent->Parent != NULL) {
//        if (Parent->Parent->Left == Parent)
//          Parent->Parent->Left = AvlTree (Parent);
//        else
//          Parent->Parent->Right = AvlTree (Parent);
//      }
//      else {
//        (*Ref) = AvlTree (Parent);
//      }
    }

    // TODO:
//    ClearOperation (*Ref);
  } else {
    Status = RETURN_NOT_FOUND;
  }

  //
  return Status;
}

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
  )
{
  RETRIEVE_DIRECTION                    RetrieveDirection = RetrieveMax;
  TREE_NODE                             *Result = NULL;

  //
  if ((Ref == NULL) || (Target == NULL)) {
    return Result;
  }

  //
  if (IsOperation (Ref)) {
    return NULL;
  } else {
    SetOperation (Ref);
  }

  //
  RetrieveDirection = (*mCompareNode) (Ref, Target);
  if (RetrieveDirection == RetrieveEqual) {
    Result = Ref;
  } else if (RetrieveDirection == RetrieveLeftNode) {
    Result = RetrieveTree (Ref->Left, Target);
  } else if (RetrieveDirection == RetrieveRightNode) {
    Result = RetrieveTree (Ref->Right, Target);
  }

  //
  ClearOperation (Ref);
  return Result;
}

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
  )
{
  EFI_STATUS                            Status = RETURN_SUCCESS;
  TREE_NODE                             *Parent;

  //
  if (Ref == NULL) {
    return RETURN_INVALID_PARAMETER;
  }

  //
  if (Ref->Left != NULL) DestroyTree (Ref->Left);
  if (Ref->Right != NULL) DestroyTree (Ref->Right);

  Parent = Ref->Parent;
  if (Parent != NULL) {
    Status = DeleteTree (&Parent, Ref);
//    ASSERT_EFI_ERROR (Status);
  }

  Ref->NumberOfNodes  ^= Ref->NumberOfNodes;
  Ref->Depth          ^= Ref->Depth;
  Ref->Operation      ^= Ref->Operation;
  Ref->Parent         = NULL;
  Ref->Left           = NULL;
  Ref->Right          = NULL;

  (*mDestroyContents) (Ref);

  return Status;
}

#if 0
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
  )
{
  STACKPTR stackTop = NULL;
  TREE_NODE                             *OutPut = NULL;
  UINTN                                 Index;

  if ((Ref == NULL) || IsOperation (Ref)) return NULL;

  OutPut = (TREE_NODE *) AllocateZeroPool (Ref->NumberOfNodes * sizeof (TREE_NODE));
  if (OutPut != NULL) {
    
  }


  
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

  return OutPut;
}
#endif

