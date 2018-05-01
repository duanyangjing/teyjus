//------------------DF_Term------------------
typedef struct 
{
    Byte   categoryTag;
    Byte   mark;                    //to be used in garbage collection
} DF_Tag;

// a generic term (head) for every category
typedef struct               
{
    DF_Tag       tag;        /* the common field for every term (head); can 
                                be any one of enum TermCategory.
                                rely on struct alignment */
    Word         dummy;      /* a place holder which enforces the size of the 
                                generic term to be 2 words. */
} DF_Term;

typedef DF_Term *DF_TermPtr; //term pointer


//------------------DF_Type------------------
typedef struct               
{
    DF_Tag          tag;     /* the common field for every type (head); can 
                                be any one of enum TypeCategory.
                                rely on struct alignment */ 
    Word            dummy;   /* a place holder which enforces the size of the 
                                generic term to be 2 words. */
} DF_Type;

typedef DF_Type *DF_TypePtr;      //type pointer

//------------------primitive types-----------
typedef struct                  //integers
{
    DF_Tag        tag;     
    long int      value;
} DF_IntTerm;

typedef struct                  //floats
{
    DF_Tag        tag;     
    float         value;
} DF_FloatTerm;







// ----------------------------------
// absmachine datatype, a term or a type
typedef union  //the type of data: (atomic) term or type 
{
    DF_Term term;
    DF_Type type;
} AM_DataType;

typedef AM_DataType *AM_DataTypePtr;

