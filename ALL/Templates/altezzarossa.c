#include <stdio.h>
#include <stdlib.h>

typedef struct _node
{
    int key;
    int col;
    struct _node *right;
    struct _node *left;
} node;

struct result
{
    int verifica;
    int altezzarossa;
};

void inserisci(node **t, int val, int col)
{
    if ( !(*t) )
    {
        node *foglia = (node*) malloc(sizeof(node));
        foglia->key = val;
        foglia->col = col;
        foglia->left = NULL;
        foglia->right = NULL;
        *t = foglia;
    }
    else
        if ( val <= (*t)->key )
            inserisci(&((*t)->left), val, col);
        else inserisci(&((*t)->right), val, col);
}

void svuota_albero(node **t)
{
    if ( *t )
    {
        svuota_albero(&((*t)->left));
        svuota_albero(&((*t)->right));
        free(*t);
    }
}

node *leggiABRC(size_t dim)
{
    node *tree = NULL;
    int k, c;
    for ( size_t i = 0; i < dim; i++ )
    {
        scanf("%d", &k);
        scanf("%d", &c);
        inserisci(&tree, k, c);
    }
    return tree;
}

struct result auxaltezza(const node *tree)
{
    if ( !tree )
    {
        struct result res = { 1, 0 };
        return res;
    }
    
    struct result resdx = auxaltezza(tree->right);
    struct result ressx = auxaltezza(tree->left);

    if ( !(tree->col) )
    {
        resdx.altezzarossa++;
        ressx.altezzarossa++;
    }

    if ( abs(resdx.altezzarossa - ressx.altezzarossa) > 1  || 
            (tree->left && !tree->right && ressx.altezzarossa > 1) ||
            (tree->right && !tree->left && resdx.altezzarossa > 1) )
    {
        struct result res = { 0, 0 };
        return res;
    }

    return resdx.altezzarossa > ressx.altezzarossa ? resdx : ressx;
}

int altezzarossa(const node* tree)
{
    return auxaltezza(tree).verifica;
}

/* int altezzarossa(node *t, int *ver)
{
    if ( *ver )
    {
        if ( t )
        {
            int risdx = 1, rissx = 1;
            int hdx = 0, hsx = 0;
            hdx = altezzarossa(t->right, &risdx);
            hsx = altezzarossa(t->left, &rissx);
            if ( !t->col )
            {
                hdx += 1;
                hsx += 1;
            }
            *ver = risdx && rissx && !( abs(hdx - hsx) > 1);
            return ( hdx > hsx ) ? (hdx) : (hsx);
        }
        else 
        {
            *ver = 1;
            return 0;
        }
    }
    else return 0;
} */

int main()
{
    size_t dim;
    scanf("%lu", &dim);
    if ( dim == 0 )
        goto vero;
    node *tree = leggiABRC(dim);
    if ( altezzarossa(tree) )
    {
        svuota_albero(&tree);
        goto vero;
    }
    else 
    {
        svuota_albero(&tree);
        goto falso;
    }

    vero: 
        printf("TRUE\n");
        return 0;
    falso:
        printf("FALSE\n");
        return 0;
}