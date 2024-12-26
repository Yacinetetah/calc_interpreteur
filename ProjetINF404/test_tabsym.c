#include <stdio.h>
#include "table_symbole.h"

int main() {

    TAB_IDF tab_idf;
    int v;
    
    initTS(&tab_idf);
    
    afficheTS(&tab_idf);
    
    insererTS(&tab_idf, 'x', 88);
    
    afficheTS(&tab_idf);
    
    insererTS(&tab_idf, 'y', 22);
    
    afficheTS(&tab_idf);
    
    insererTS(&tab_idf, 'x', 44);
    
    afficheTS(&tab_idf);
    
    modifierTS(&tab_idf, 'z', 8);
    
    estPresentTS(&tab_idf, 'g', &v);
    
    estPresentTS(&tab_idf, 'x', &v);
    
    printf("%d\n", v);
    
    estPresentTS(&tab_idf, 'y', &v);
     
    printf("%d\n", v);
    
    afficheTS(&tab_idf);
    
    return 0;
    
}