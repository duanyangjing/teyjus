/*
 * This file should define all the data/code that are needed to communicate
 * between the abstract machine and the external library.
 */

// everything in error.h
#include "../../system/error.h"

// TODO: may need to include only the functions needed
// This is needed by io to print lambda terms
#include "../printterm.h"
// DJ: comp and eval does not needed stream
// io needs stream
#include "../../system/stream.h"
// DJ: only PERV_STDIN PERV_STDOUT PERV_STDERR are needed in
// BIIO_getStreamFromTerm
#include "../../tables/pervasives.h"
// DJ: needed for read/readterm/term_to_string/string_to_term
#include "../builtins/readterm.h"
#include "../../front/readterm_c.h"

// everything except for typedef DF_Tag, and Disagreement set definitions
//#include "../dataformats.h"
#ifndef DATAFORMATS_H
#define DATAFORMATS_H

/********************************************************************/
/*   DATA TAG FIELD IN TYPES AND TERMS                              */
/********************************************************************/

/* The tags of heap items */
typedef enum
{
    //type categories
    DF_TY_TAG_SORT = 0,       //sort
    DF_TY_TAG_REF,            //reference
    DF_TY_TAG_SKVAR,          //skeleton variable
    DF_TY_TAG_ARROW,          //type arrow
    DF_TY_TAG_STR,            //type structure
    DF_TY_TAG_FUNC,           //functor of type structure

    //term categories
    DF_TM_TAG_VAR  = 6,       // existential variables
    DF_TM_TAG_CONST,          // constants 
    DF_TM_TAG_INT,            // integers
    DF_TM_TAG_FLOAT,          // floats
    DF_TM_TAG_NIL,            // empty lists
    DF_TM_TAG_STR,            // strings
    DF_TM_TAG_STREAM,         // streams
    DF_TM_TAG_BVAR,           // lambda bound variables (de Bruijn index)
                              // -- atoms above
    DF_TM_TAG_REF,            // references
                              // -- complex terms below
    DF_TM_TAG_CONS,           // list constructors
    DF_TM_TAG_LAM,            // abstractions
    DF_TM_TAG_APP,            // applications
    DF_TM_TAG_SUSP,           // suspensions

    DF_TM_TAG_STRBODY = 19,   // string body

    //suspension environment items 
    DF_ENV_TAG_DUMMY = 20,    //dummy environment
    DF_ENV_TAG_PAIR,          //pair environment 
    
    //disagreement pair
    DF_DISPAIR = 22           
} DF_HeapDataCategory;

/********************************************************************/
/*                                                                  */
/*                         TYPE REPRESENTATION                      */
/*                                                                  */
/********************************************************************/
//generic type (head) for every category
typedef struct               
{
    DF_Tag          tag;     /* the common field for every type (head); can 
                                be any one of enum TypeCategory.
                                rely on struct alignment */ 
    Word            dummy;   /* a place holder which enforces the size of the 
                                generic term to be 2 words. */
} DF_Type;

typedef DF_Type *DF_TypePtr;      //type pointer

//sizes of different type items
#define DF_TY_ATOMIC_SIZE    2    //atomic size

//attributes of special type constructors
#define DF_TY_ARROW_ARITY    2    //arity of type arrow

/******************************************************************/
/*                      Interface functions                       */
/******************************************************************/

/* TYPE DEREFERENCE */
DF_TypePtr DF_typeDeref(DF_TypePtr);

/* TYPE RECOGNITION */
Boolean DF_isSortType(DF_TypePtr);     // is sort?
Boolean DF_isRefType(DF_TypePtr);      // is reference? (including free var)
Boolean DF_isFreeVarType(DF_TypePtr);  // is free var?
Boolean DF_isSkelVarType(DF_TypePtr);  // is skeleton var?
Boolean DF_isArrowType(DF_TypePtr);    // is type arrow?
Boolean DF_isStrType(DF_TypePtr);      // is type structure?

/* TYPE DECOMPOSITION */
int DF_typeTag(DF_TypePtr);                      //generic type
int DF_typeKindTabIndex(DF_TypePtr);             //sorts
int DF_typeSkelVarIndex(DF_TypePtr);             //skel var
DF_TypePtr DF_typeRefTarget(DF_TypePtr);         //reference
DF_TypePtr DF_typeArrowArgs(DF_TypePtr);         //arrows
DF_TypePtr DF_typeStrFuncAndArgs(DF_TypePtr);    //structures
int DF_typeStrFuncInd(DF_TypePtr);           
int DF_typeStrFuncArity(DF_TypePtr); 
DF_TypePtr DF_typeStrArgs(DF_TypePtr);

/* TYPE CONSTRUCTION */
void DF_copyAtomicType(DF_TypePtr src, MemPtr dest);
void DF_mkSortType(MemPtr loc, int ind);
void DF_mkRefType(MemPtr loc, DF_TypePtr target);
void DF_mkFreeVarType(MemPtr loc);
void DF_mkSkelVarType(MemPtr loc, int offset);
void DF_mkArrowType(MemPtr loc, DF_TypePtr args);
void DF_mkStrType(MemPtr loc, DF_TypePtr funcAndArgs);
void DF_mkStrFuncType(MemPtr loc, int ind, int n);


/********************************************************************/
/*                                                                  */
/*                         TERM REPRESENTATION                      */
/*                                                                  */
/********************************************************************/
typedef struct               
{
    DF_Tag       tag;        /* the common field for every term (head); can 
                                be any one of enum TermCategory.
                                rely on struct alignment */
    Word         dummy;      /* a place holder which enforces the size of the 
                                generic term to be 2 words. */
} DF_Term;

typedef DF_Term *DF_TermPtr; //term pointer

//sizes of different term items
#define DF_TM_ATOMIC_SIZE  2       // atomic size
#define DF_TM_TCONST_SIZE  3       // type associated constant (config set) 
#define DF_TM_APP_SIZE     3       // application head
#define DF_TM_LAM_SIZE     2       // abstraction
#define DF_TM_CONS_SIZE    2       // cons 
#define DF_TM_SUSP_SIZE    4       // suspension               (config set)

// attributes of some special constants 
#define  DF_CONS_ARITY     2       //arity of cons

// head of string body (a tag word should be followed by encoding of literals)
typedef union                     
{
    DF_Tag        tag;
    Word          dummy;
} DF_StrData;                 

typedef DF_StrData *DF_StrDataPtr;

//#define DF_STRDATA_HEAD_SIZE   (int)ceil((double)sizeof(DF_StrData)/WORD_SIZE)
#define DF_STRDATA_HEAD_SIZE  2     

//a generic environment item in suspension
typedef struct DF_env
{
    //Boolean          isDummy;
    DF_Tag           tag;
    TwoBytes         embedLevel;
    struct DF_env    *rest;        //the tail of the list
} DF_Env;

typedef DF_Env *DF_EnvPtr;

// empty environment list 
#define DF_EMPTY_ENV NULL

//sizes of different environment items
#define DF_ENV_DUMMY_SIZE  2       // dummy environment item
#define DF_ENV_PAIR_SIZE   3       // pair environment item

//limits (to be set by configuration)
#define DF_MAX_BV_IND      USHRT_MAX        //max db ind (embedding level)
#define DF_TM_MAX_ARITY    USHRT_MAX        //max arity 
#define DF_MAX_UNIVIND     USHRT_MAX        //max universe index


/******************************************************************/
/*                      Interface functions                       */
/******************************************************************/

/* DEREFERENCE      */
DF_TermPtr DF_termDeref(DF_TermPtr); // term dereference

/* TERM RECOGNITION */
Boolean DF_isAtomic(DF_TermPtr); //note ref is neither atomic nor complex
Boolean DF_isNAtomic(DF_TermPtr);                                    
Boolean DF_isFV(DF_TermPtr);     // is unbound variable?
Boolean DF_isConst(DF_TermPtr);  // is constant (typed and untyped)?
Boolean DF_isTConst(DF_TermPtr); // is a type associated constant?
                                 // Note we assume the arg is known to be const
Boolean DF_isInt(DF_TermPtr);    // is integer?
Boolean DF_isFloat(DF_TermPtr);  // is float?
Boolean DF_isNil(DF_TermPtr);    // is list nil?
Boolean DF_isStr(DF_TermPtr);    // is string?
Boolean DF_isBV(DF_TermPtr);     // is de Bruijn index?
Boolean DF_isStream(DF_TermPtr); // is stream?
Boolean DF_isRef(DF_TermPtr);    // is reference?
Boolean DF_isCons(DF_TermPtr);   // is list cons?
Boolean DF_isLam(DF_TermPtr);    // is abstraction?
Boolean DF_isApp(DF_TermPtr);    // is application?
Boolean DF_isSusp(DF_TermPtr);   // is suspension?

Boolean DF_isEmpEnv(DF_EnvPtr);  // is empty environment?
Boolean DF_isDummyEnv(DF_EnvPtr);// is dummy environment item?
                      
/* TERM DECOMPOSITION */
//generic term
int DF_termTag(DF_TermPtr);                  // term category tag
//unbound variable
int DF_fvUnivCount(DF_TermPtr);              // universe count
//constants (w/oc type associations)
int DF_constUnivCount(DF_TermPtr);           // universe index
int DF_constTabIndex(DF_TermPtr);            // symbol table index
//constants with type associations
DF_TypePtr DF_constType(DF_TermPtr);         // type environment
//integer
long DF_intValue(DF_TermPtr);                // integer value (long)
//float
float DF_floatValue(DF_TermPtr);             // float value
//string
MCSTR_Str DF_strValue(DF_TermPtr);           // string value
DF_StrDataPtr DF_strData(DF_TermPtr tmPtr);  // string data field
MCSTR_Str DF_strDataValue(DF_StrDataPtr tmPtr);  //acc str value from data fd
//stream
WordPtr DF_streamTabIndex(DF_TermPtr);       // stream table index 
//de Bruijn indices
int DF_bvIndex(DF_TermPtr);                  // de Bruijn index
//reference
DF_TermPtr DF_refTarget(DF_TermPtr);         // target
//list cons
DF_TermPtr DF_consArgs(DF_TermPtr);          // arg vector
//abstractions
int DF_lamNumAbs(DF_TermPtr);                // embedding level
DF_TermPtr DF_lamBody(DF_TermPtr);           // lambda body
//application
int DF_appArity(DF_TermPtr);                 // arity
DF_TermPtr DF_appFunc(DF_TermPtr);           // functor
DF_TermPtr DF_appArgs(DF_TermPtr);           // arg vector
//suspension
int DF_suspOL(DF_TermPtr);                   // ol
int DF_suspNL(DF_TermPtr);                   // nl
DF_TermPtr DF_suspTermSkel(DF_TermPtr);      // term skel
DF_EnvPtr  DF_suspEnv(DF_TermPtr);           // environment list

//environment item (dummy/pair)
DF_EnvPtr DF_envListRest(DF_EnvPtr);         // next env item
DF_EnvPtr DF_envListNth(DF_EnvPtr, int);     // the nth item 
int DF_envIndex(DF_EnvPtr);                  // l in @l or (t,l)
//pair environment item 
DF_TermPtr DF_envPairTerm(DF_EnvPtr);        // t in (t,l)
                        

/* TERM CONSTRUCTION */
void DF_copyAtomic(DF_TermPtr src, MemPtr dest);   //copy atomic 
void DF_copyApp(DF_TermPtr src, MemPtr dest);      //copy application
void DF_copySusp(DF_TermPtr src, MemPtr dest);     //copy suspension
void DF_mkVar(MemPtr loc, int uc);                 //unbound variable
void DF_mkBV(MemPtr loc, int ind);                 //de Bruijn index
void DF_mkConst(MemPtr loc, int uc, int ind);      //const 
void DF_mkTConst(MemPtr loc, int uc, int ind, DF_TypePtr typeEnv);
                                                  //const with type association
void DF_mkInt(MemPtr loc, long value);             //int
void DF_mkFloat(MemPtr loc, float value);          //float
void DF_mkStr(MemPtr loc, DF_StrDataPtr data);     //string
void DF_mkStrDataHead(MemPtr loc);                 //string data head
void DF_mkStream(MemPtr loc, WordPtr ind);         //stream 
void DF_setStreamInd(DF_TermPtr tm, WordPtr ind);  //update index of a stream
void DF_mkNil(MemPtr loc);                         //nil
void DF_mkRef(MemPtr loc, DF_TermPtr target);      //reference
void DF_mkCons(MemPtr loc, DF_TermPtr args);       //cons
void DF_mkLam(MemPtr loc, int n, DF_TermPtr body); //abstraction
void DF_mkApp(MemPtr loc, int n, DF_TermPtr func, DF_TermPtr args); 
                                                   //application
void DF_mkSusp(MemPtr loc, int ol, int nl, DF_TermPtr tp, DF_EnvPtr env);
                                                   //suspension
void DF_mkDummyEnv(MemPtr loc, int l, DF_EnvPtr rest);    //@l env item
void DF_mkPairEnv(MemPtr loc, int l, DF_TermPtr t, DF_EnvPtr rest);
                                                          // (t, l) env item

/* TERM MODIFICATION */
void DF_modVarUC(DF_TermPtr vPtr, int uc);

/* (NON_TRIVIAL) TERM COMPARISON */
Boolean DF_sameConsts(DF_TermPtr const1, DF_TermPtr const2);     //same const?
Boolean DF_sameStrs(DF_TermPtr str1, DF_TermPtr str2);           //same string?
Boolean DF_sameStrData(DF_TermPtr tmPtr, DF_StrDataPtr strData); //same str?

#endif  //DATAFORMATS_H


// only MEM_FindCodeFnPtr from memory.h
//#include "../../system/memory.h"
/******************************************************************************
 *                From system/memory.h
 *                FIND CODE FUNCTION                                          *
/******************************************************************************/
//arguments: constInd, search table size, search table addr
typedef CSpacePtr (*MEM_FindCodeFnPtr)(int, int, MemPtr);



// everything except for RUN-TIME SYMBOL TABLES
//#include "../abstmachine.h"
#ifndef ABSTMACHINE_H
#define ABSTMACHINE_H
/***************************######********************************************
 *                          ERROR INFORMATION
 *********************************######**************************************/

#define SIM_NUM_ERROR_MESSAGES 13
enum
{
   SIM_ERROR = SIM_FIRST_ERR_INDEX,
   SIM_ERROR_TOO_MANY_ABSTRACTIONS,
   SIM_ERROR_TOO_MANY_ARGUMENTS,
   SIM_ERROR_TOO_MANY_UNIV_QUANTS,
   SIM_ERROR_HEAP_TOO_BIG,
   SIM_ERROR_HEAP_TOO_SMALL,
   SIM_ERROR_CANNOT_ALLOCATE_HEAP,
   SIM_ERROR_CANNOT_ALLOCATE_HEAP_MESSAGE,
   SIM_ERROR_CANNOT_ALLOCATE_HEAP_SUGGESTION,
   SIM_ERROR_TRAIL_OVERFL,
   SIM_ERROR_HEAP_OVERFL,
   SIM_ERROR_STACK_OVERFL,
   SIM_ERROR_PDL_OVERFL,
};

typedef union  //the type of data: (atomic) term or type 
{
    DF_Term term;
    DF_Type type;
} AM_DataType;

typedef AM_DataType *AM_DataTypePtr;

//#define AM_DATA_SIZE (int)ceil((double)sizeof(AM_DataType)/WORD_SIZE)
#define AM_DATA_SIZE  2

/****************************************************************************/
/*                ABSTRACT MACHINE REGISTERS (AND FLAGS)                    */
/****************************************************************************/

typedef enum {OFF = 0, ON = 1}     AM_FlagTypes;     //FLAG type
typedef Byte                       Flag;


/*There are 255 argument registers numbered 1 through 255; Reg_0 is never
  used. (agree with instruction format)*/
#define AM_NUM_OF_REG 256      
extern AM_DataType  AM_regs[AM_NUM_OF_REG];//argument regs/temp variables

//data register access: return the address of the ith register
AM_DataTypePtr      AM_reg(int i);               

extern MemPtr       AM_hreg;                //heap top
extern MemPtr       AM_hbreg;               //heap backtrack point
extern MemPtr       AM_ereg;                //current environment
extern MemPtr       AM_breg;                //last choice point
extern MemPtr       AM_b0reg;               //cut point
extern MemPtr       AM_ireg;                //impl pt reg, defining prog context
extern MemPtr       AM_cireg;               //impl pt for current clause   
extern MemPtr       AM_cereg;               //closure environment
extern MemPtr       AM_tosreg;              //top of stack impl or choice pt.
extern MemPtr       AM_trreg;               //trail top
extern MemPtr       AM_pdlTop;              //top of pdl
extern MemPtr       AM_pdlBot;              //(moving) bottom of pdl
extern MemPtr       AM_typespdlBot;         //(moving) bottom of types pdl

extern DF_TermPtr   AM_sreg;                //"structure" pointer
extern DF_TypePtr   AM_tysreg;              //type structure pointer

extern CSpacePtr    AM_preg;                //program pointer
extern CSpacePtr    AM_cpreg;               //continuation pointer

extern DF_DisPairPtr AM_llreg;              //ptr to the live list

extern Flag         AM_bndFlag;             //does binding on fv (term) occur?
extern Flag         AM_writeFlag;           //in write mode?
extern Flag         AM_tyWriteFlag;         //in ty write mode? 
extern Flag         AM_ocFlag;              //occurs check? 

extern Flag         AM_consFlag;            //cons? 
extern Flag         AM_rigFlag;             //rigid?

extern TwoBytes     AM_numAbs;              //number of abstractions in hnf
extern TwoBytes     AM_numArgs;             //number of arguments in hnf

extern DF_TermPtr   AM_head;                //head of a hnf 
extern DF_TermPtr   AM_argVec;              //argument vector of a hnf 

extern DF_TermPtr   AM_vbbreg;              //variable being bound for occ
extern DF_TypePtr   AM_tyvbbreg;            //type var being bound for occ
extern TwoBytes     AM_adjreg;              //univ count of variable being bound

extern TwoBytes     AM_ucreg;               //universe count register

/****************************************************************************/
/*               STACK, HEAP, TRAIL AND PDL RELATED STUFF                   */
/****************************************************************************/
extern MemPtr    AM_heapBeg,                //beginning of the heap
                 AM_heapEnd,                //end of the heap
                 AM_stackBeg,               //beginning of the stack
                 AM_stackEnd,               //end of the stack 
                 AM_trailBeg,               //beginning of the trail
                 AM_trailEnd,               //end of the trail
                 AM_pdlBeg,                 //beginning of pdl
                 AM_pdlEnd,                 //end of pdl
                 AM_fstCP;                  //the first choice point


/****************************************************************************/
/*            CODE PLACED IN THE HEAP BY THE SYSTEM                         */
/****************************************************************************/
extern CSpacePtr  AM_failCode;
extern CSpacePtr  AM_andCode;
extern CSpacePtr  AM_orCode;
extern CSpacePtr  AM_allCode;
extern CSpacePtr  AM_solveCode;
extern CSpacePtr  AM_builtinCode;
extern CSpacePtr  AM_eqCode;
extern CSpacePtr  AM_stopCode;
extern CSpacePtr  AM_haltCode;
extern CSpacePtr  AM_notCode1;
extern CSpacePtr  AM_notCode2;
extern CSpacePtr  AM_proceedCode;


Boolean AM_isFailInstr(CSpacePtr cptr);
/****************************************************************************/
/*               VITUAL MACHINE MEMORY OPERATIONS                           */
/****************************************************************************/
Boolean AM_regAddr(MemPtr p);      //is the given addr referring to a register?
Boolean AM_stackAddr(MemPtr p);    //is the given addr on stack?
Boolean AM_nHeapAddr(MemPtr p);    //is the given addr on heap?

Boolean AM_botIP(MemPtr p);        //is the "first" impl/impt record?
Boolean AM_botCP();                //is the "first" choice point?
Boolean AM_noEnv();                //no env record left on the stack?

MemPtr  AM_findtos(int i);
MemPtr  AM_findtosEnv();
void    AM_settosreg();            //set AM_tosreg to the top imp or choice pt

/***************************************************************************/
/*               ENVIRONMENT RECORD OPERATIONS                             */
/***************************************************************************/
#define   AM_ENV_FIX_SIZE   4                //size of the fix part of env rec

//environment record creation function
MemPtr AM_mkEnv(MemPtr ep);                  //create the fixed part of env rec
MemPtr AM_mkEnvWOUC(MemPtr ep);              //ct fixed part of env without uc

//environment record access functions (current top env record)
AM_DataTypePtr AM_envVar(int n);             //the nth var fd
int            AM_envUC();                   //the env universe count 
CSpacePtr      AM_envCP();                   //the env continuation point
MemPtr         AM_envCE();                   //continuation point 
MemPtr         AM_envCI();                   //impl point 
Boolean        AM_inCurEnv(MemPtr p);        //is p an addr in the curr env?

//access functions for clause environment
AM_DataTypePtr AM_cenvVar(int n);            //the nth var fd in clause env

/****************************************************************************/
/*                       CHOICE POINT OPERATIONS                            */
/****************************************************************************/
#define AM_CP_FIX_SIZE      11           //size of the fix part of choice point

//choice point creation functions
void AM_mkCP(MemPtr cp, CSpacePtr label, int n); //create a choice pt
void AM_saveStateCP(MemPtr cp, CSpacePtr label);   
void AM_setNClCP(CSpacePtr ncl);                 //set the ncl fd in top ch pt

//restore functions 
//restore all components of a choice point except the trail top and the 
//backtrack point registers 
void AM_restoreRegs(int n);           
//restore all components of a choice point except the trail top, the backtrack 
//point and the clause context registers
void AM_restoreRegsWoCI(int n);
//access functions
MemPtr    AM_cpH();
CSpacePtr AM_cpNCL();
MemPtr    AM_cpTR();
MemPtr    AM_cpB();
MemPtr    AM_cpCI();

AM_DataTypePtr AM_cpArg(MemPtr cp, int n); //addr of nth arg in a given cp

/***************************************************************************/
/*                IMPLICATION/IMPORT RECORD OPERATIONS                     */
/***************************************************************************/
#define AM_IMP_FIX_SIZE         6         //size of the fix part of impl/impt rec
#define AM_DUMMY_IMPT_REC_SIZE  2         //size of a dummy impt rec
#define AM_NCLT_ENTRY_SIZE      2         //size of each entry in next clause tab
#define AM_BCKV_ENTRY_SIZE      2         //size of ent. in back chained vector


//finding code for a predicate in the program context given by the value of
//the AM_ireg. 
void AM_findCode(int constInd, CSpacePtr *clPtr, MemPtr *iptr);

//creating the fixed part of a new implication record
void AM_mkImplRec(MemPtr ip,MemPtr sTab,int sTabSize, MEM_FindCodeFnPtr fnPtr);
//creating the fixed part of a new import record with local consts
void AM_mkImptRecWL(MemPtr ip, int npreds, MemPtr sTab, int sTabSize, 
                    MEM_FindCodeFnPtr fnPtr);
//creating the fixed part of a new import record without local consts
void AM_mkImptRecWOL(MemPtr ip, int npreds, MemPtr sTab, int sTabSize, 
                     MEM_FindCodeFnPtr fnPtr);
//creating a dummy import point 
void AM_mkDummyImptRec(MemPtr ip);

//initializing the next clause table in an implication/import record.
void AM_mkImpNCLTab(MemPtr ip, MemPtr linkTab, int size);
//initializing the backchained vector in an import record
void AM_initBCKVector(MemPtr ip, int nclTabSize, int noSegs);
//set back chained number in a given back chained field
void AM_setBCKNo(MemPtr bck, int n);
//set most recent cp in a given back chained field
void AM_setBCKMRCP(MemPtr bck, MemPtr cp);
//initializing the universe indices in the symbol table entries for constants
//local to a module
void AM_initLocs(int nlocs, MemPtr locTab);

//implication/import record access functions
MemPtr    AM_impNCL(MemPtr ip, int i); //the ith entry of next clause tab
CSpacePtr AM_impNCLCode(MemPtr ncl);   //code in a next clause field
MemPtr    AM_impNCLIP(MemPtr ncl);     //ip in a next clause field
MemPtr    AM_cimpBCK(int i);           //the ith entry of back chained vec in CI
int       AM_impBCKNo(MemPtr bck);     //back chain num in a bck field
MemPtr    AM_impBCKMRCP(MemPtr bck);   //most recent cp is a bck field   
MemPtr    AM_cimpCE();                 //clause env of impl rec in CI
int       AM_cimpNPreds();             //# preds of impt rec in CI
MemPtr    AM_impPST(MemPtr ip);        //search table field addr 
MEM_FindCodeFnPtr AM_impFC(MemPtr ip); //find code function field addr
MemPtr    AM_impPIP(MemPtr ip);        //PIP in given imp point
MemPtr    AM_curimpPIP();              //PIP in the current top imp point
int       AM_impPSTS(MemPtr ip);       //search table size field

Boolean AM_isImptWL(MemPtr ip);        //is an imp rec a import rec w local
Boolean AM_isImptWOL(MemPtr ip);       //is an imp rec a import rec wo local
Boolean AM_isImpl(MemPtr ip);          //is an imp rec a implication rec
Boolean AM_isImpt(MemPtr ip);          //is an imp rec a import rec 

Boolean AM_isImplCI();                 //is rec referred to by CI impl?
Boolean AM_isCurImptWL();              //is rec referred to by I impt with loc?


/***************************************************************************/
/*                     LIVE LIST OPERATIONS                                */
/***************************************************************************/
Boolean AM_empLiveList();                   //live list is empty?
Boolean AM_nempLiveList();                  //live list not empty?

//add a dpair to the beginning of live list
void    AM_addDisPair(DF_TermPtr tPtr1, DF_TermPtr tPtr2);

/***************************************************************************/
/*                        PDL OPERATIONS                                   */
/***************************************************************************/ 
MemPtr   AM_popPDL();                       //pop (term/type) PDL
void     AM_pushPDL(MemPtr);                //push (term/type) PDL

Boolean  AM_emptyPDL();                     //is empty PDL?
Boolean  AM_nemptyPDL();                    //is not empty PDL?
void     AM_initPDL();                      //initialize PDL

Boolean  AM_emptyTypesPDL();                //is empty type PDL?
Boolean  AM_nemptyTypesPDL();               //is not empty type PDL?
void     AM_initTypesPDL();                 //initialize type PDL
void     AM_resetTypesPDL();                //reset PDL to that before ty unif

/****************************************************************************
 *                         OVERFLOW ERROR FUNCTIONS                         *
 ****************************************************************************/
void AM_heapError(MemPtr);                  //heap overflow
void AM_stackError(MemPtr);                 //stack overflow
void AM_pdlError(int);                      //pdl stack overflow for n cells
void AM_trailError(int);                    //trail overflow for n cells


#endif //ABSTMACHINE_H


// VM string related functions
#include "../mcstring.h"

#include "../mctypes.h"

// builtin number
//#include "../builtins/builtins.h"
#ifndef BUILTINS_H
#define BUILTINS_H

/***********************************************************************/
/*             Builtin Dispatch Table Index                            */
/***********************************************************************/
typedef enum
{
    BI_SOLVE          = 0,
    BI_EVAL           = 1,
    BI_NOT            = 2,
    BI_UNIFY          = 3,
    // comparison operations
    BI_INT_LT         = 4, 
    BI_INT_GT         = 5,
    BI_INT_LE         = 6,
    BI_INT_GE         = 7,
    BI_FLOAT_LT       = 8,
    BI_FLOAT_GT       = 9, 
    BI_FLOAT_LE       = 10,
    BI_FLOAT_GE       = 11,
    BI_STR_LT         = 12,
    BI_STR_GT         = 13,
    BI_STR_LE         = 14,
    BI_STR_GE         = 15,
    //IO
    BI_IO_OPEN_IN     = 16,
    BI_IO_OPEN_OUT    = 17,
    BI_IO_OPEN_APP    = 18,
    BI_IO_CLOSE_IN    = 19,
    BI_IO_CLOSE_OUT   = 20,
    BI_IO_OPEN_STR    = 21,
    BI_IO_INPUT       = 22,
    BI_IO_OUTPUT      = 23,
    BI_IO_INPUT_LINE  = 24,
    BI_IO_LOOKAHEAD   = 25,
    BI_IO_EOF         = 26,
    BI_IO_FLUSH       = 27,
    BI_IO_PRINT       = 28,
    BI_IO_READ        = 29,
    BI_IO_PRINTTERM   = 30,
    BI_IO_TERM_TO_STR = 31,
    BI_IO_STR_TO_TERM = 32,
    BI_IO_READTERM    = 33,
    BI_IO_GETENV      = 34,
    BI_IO_OPEN_SOCKET = 35,
    BI_UNIX_TIME      = 36,
    BI_SYSTEM         = 37
} BI_BuiltinTabIndex;
/* typedef enum */
/* { */
/*     // actual builtins */
/*     BI_SOLVE          = 0, */
/*     BI_NOT            = 1, */
/*     BI_UNIFY          = 2, */
/*     BI_IO_TERM_TO_STR = 3, */
/*     BI_IO_STR_TO_TERM = 4, */
/*     BI_IO_READ        = 5, */
/*     BI_IO_READTERM    = 6, */

/*     // comp.so */
/*     BI_EVAL           = 7, */
/*     BI_INT_LT         = 8, */
/*     BI_INT_GT         = 9, */
/*     BI_INT_LE         = 10, */
/*     BI_INT_GE         = 11, */
/*     BI_FLOAT_LT       = 12, */
/*     BI_FLOAT_GT       = 13, */
/*     BI_FLOAT_LE       = 14, */
/*     BI_FLOAT_GE       = 15, */
/*     BI_STR_LT         = 16, */
/*     BI_STR_GT         = 17, */
/*     BI_STR_LE         = 18, */
/*     BI_STR_GE         = 19, */
/*     // io.so */
/*     BI_IO_OPEN_IN     = 20, */
/*     BI_IO_OPEN_OUT    = 21, */
/*     BI_IO_OPEN_APP    = 22, */
/*     BI_IO_CLOSE_IN    = 23, */
/*     BI_IO_CLOSE_OUT   = 24, */
/*     BI_IO_OPEN_STR    = 25, */
/*     BI_IO_INPUT       = 26, */
/*     BI_IO_OUTPUT      = 27, */
/*     BI_IO_INPUT_LINE  = 28, */
/*     BI_IO_LOOKAHEAD   = 29, */
/*     BI_IO_EOF         = 30, */
/*     BI_IO_FLUSH       = 31, */
/*     BI_IO_PRINT       = 32, */
/*     BI_IO_PRINTTERM   = 33, */
/*     BI_IO_OPEN_SOCKET = 34, */
/*     // os.so */
/*     BI_IO_GETENV      = 35, */
/*     BI_UNIX_TIME      = 36, */
/*     BI_SYSTEM         = 37 */
/* } BI_BuiltinTabIndex; */
/*****************************************************************************/
/* Dispatching function for the builtin table                                */
/*****************************************************************************/ 
void BI_dispatch(int number);

/* builtin index "register"*/
extern BI_BuiltinTabIndex BI_number;

/***************************######********************************************
 *                          ERROR INFORMATION
 *********************************######**************************************/

#define BI_NUM_ERROR_MESSAGES 29
enum
{
   BI_ERROR = BI_FIRST_ERR_INDEX,
   BI_ERROR_TERM,
   BI_ERROR_NOT_IMPLEMENTED,
   BI_ERROR_FVAR_CAP,
   BI_ERROR_TYFVAR_CAP,
   BI_ERROR_DIV_BY_ZERO,
   BI_ERROR_NEG_SQRT,
   BI_ERROR_NEG_LOG,
   BI_ERROR_CONST_IND,
   BI_ERROR_FLEX_HEAD,		/* takes term */
   BI_ERROR_ILLEGAL_ARG,	/* takes term */
   BI_ERROR_EVAL_TYPE,
   BI_ERROR_ILLEGAL_STREAM,
   BI_ERROR_FLEX_GOAL,
   BI_ERROR_NON_VAR_TERM,	/* takes term */
   BI_ERROR_INDEX_OUT_OF_BOUNDS,
   BI_ERROR_NEGATIVE_VALUE,
   BI_ERROR_UNBOUND_VARIABLE,	/* takes string indicating desired arg. */
   BI_ERROR_NON_STREAM_TERM,	/* takes term */
   BI_ERROR_STREAM_ALREADY_CLOSED,
   BI_ERROR_CANNOT_OPEN_STREAM,	/* takes filename */
   BI_ERROR_STREAM,		/* takes term (stream) */
   BI_ERROR_READING_STREAM,	/* takes term (stream) */
   BI_ERROR_WRITING_STREAM,	/* takes term (stream) */
   BI_ERROR_FLUSHING_STREAM,	/* takes term (stream) */
   BI_ERROR_OPENING_STRING,	/* takes string */
   BI_ERROR_INTEGER_EXPECTED,	/* takes term */
   BI_ERROR_SUBSTRING,
   BI_ERROR_UNSET_ENV_VAR       /* takes string */
};

#endif //BUILTINS_H
