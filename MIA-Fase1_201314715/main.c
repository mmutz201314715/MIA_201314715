#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/****ANALIZADOR******/
int analizar(char * cad, int n, int estado){


    /***LECTURA DE CADENA**/
    int i, c;

       c=getchar();
       if (c == EOF) {
           cad[0] = '\0';
           return 0;
       }

       if (c == '\n')
           i = 0;
       else {
           cad[0] = c;
           i = 1;
       }

       for (; i < n-1 && (c=getchar())!=EOF && c!='\n'; i++)
          cad[i] = c;

       cad[i] = '\0';

       if (c != '\n' && c != EOF)
           while ((c = getchar()) != '\n' && c != EOF);


       char entrada[500];
       printf("%s\n",cad);

       strcpy(entrada, cad);
       printf("%s\n",entrada);

       /*INGRESO AL AUTOMATA*/

       if(cad[0]=='#'){
           printf("comentario\n");
           return 0;
       }else if(strcasecmp("mkdisk", cad)==0){
           estado = 1;
       }
       else{
           //aqui deben de ir el resto de valores
           printf("linea normal\n");
       }

       return 1;

}

int main(void)
{
    printf("Hello World!\n");
 char cadena[500];

 analizar(cadena , 500, 0);
    return 0;
}

