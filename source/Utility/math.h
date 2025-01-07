typedef enum
{
    FALSE   =   0,
    TRUE    =   1,
} bool;

#define BIT(N)                      (1UL<<(N))
// Bit manipulation using masks
#define BITSET(V,M)                 (V)|=(M)
#define BITCLEAR(V,M)               (V)&=~(M)
#define BITTOGGLE(V,M)              (V)^=(M)
// Bit manipulation using indexes
#define BITSETN(V,N)                 (V)|=BIT(N)
#define BITCLEARN(V,N)               (V)&=~BIT(N)
#define BITTOGGLEN(V,N)              (V)^=BIT(N)
// Bit fields
#define BITVALUE(V,N)               ((V)<<(N))
#define BITMASK(M,N)                ((BIT((M)-(N)+1)-1)<<(N))
// Bit fields using masks
#define BITFIELDGET(VAR,MASK)       ((VAR)&(MASK))
#define BITFIELDSET(VAR,MASK,VAL)   (VAR) = ((VAR)&~(MASK))|(VAL)
// Bit fields using indexes (M > N)
#define BITFIELDMNSET(VAR,M,N,VAL)  (VAR)=((VAR)&~(BITMASK((M),(N)))|BITVALUE((VAL),(N)))
#define BITFIELDMNGET(VAR,M,N)      ((VAR)&(BITMASK((M),(N)))>>(N))