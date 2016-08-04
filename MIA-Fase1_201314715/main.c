#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/****ANALIZADOR******/
int crearDisco(char *cadena);
void CreateFolder(char * path);
int analizar(char * cad, int n, int process){
int estado  = 0;

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
 /*estado = 0*/
       if(cad[0]=='#'){
           estado = 0;
       }else if(strcasecmp("mkdisk", cad)==0){
           estado = 1;
       }
       else{
           //errores de escritura

           estado = -1;
       }

       switch(estado){
       case 0:
           printf("comentario: %s\n", cad);
           estado = 0;
           break;
       case 1:
           printf("comando mkdisk\n");
           break;
       default:
          //error
           printf("error comando no valido\n");
           break;
       }

       return 1;

}

/*****CREAR DISCO******/
//normal
int crearDisco(char * cadena){
    int size=0;
    char unit='\0';
    char path[100];
    char name[100];
    int estado=0;
    int tam;
    char * spliteo;

        spliteo = strtok(cadena, " ");
        while (spliteo != NULL)
        {
            if(strcasecmp("mkdisk", spliteo)==0){
                //ingnorar y confirmar que si se recibio el comando
                estado = 0;
            }
            else if(strcasecmp("-size::", spliteo)==0){
                estado = 1;

            }
            else if(strcasecmp("+unit::", spliteo)==0){
                estado = 2;

            }
            else if(strcasecmp("-path::", spliteo)==0){
                estado = 3;
            }
            else if(strcasecmp("-name::", spliteo)==0){
                estado = 4;
            }else{
                estado = -1;
            }

            switch (estado) {
            case 0:
                //desechar mkdisk
                break;
            case 1:
                //entro size
                estado = 5;
                break;
            case 2:
                //entro unit
                estado = 6;
                break;
            case 3:
                //entro path
                estado = 7;
                break;
            case 4:
                //entro name
                estado = 8;
                break;
            case 5:
                //convertir entrada a int

                size= atoi(spliteo);
                break;
            case 6:
                if(strcasecmp("k",spliteo)){
                    //crear e kilos
                    unit = 'k';
                }else if(strcasecmp("m",spliteo)){
                    //crear en megas
                }else{
                    //viene un caracter erroneo

                }
                break;
            case 7:
                CreateFolder(spliteo);
                break;
            case 8:


                for(tam =0; spliteo[tam]!='\0';tam++){

                }
                if(tam>5){
                    if(strcasecmp('d',spliteo[tam-3])==0 && strcasecmp('s',spliteo[tam-2])==0 && strcasecmp('k',spliteo[tam-1])==0){
                        FILE* archivo = fopen("%s/%s",path,name, "r");

                        if (archivo) {
                        fclose(archivo);
                        printf("El archivo existe");
                        } else {
                        printf("El archivo no existe");
                        archivo = fopen("archivo.txt", "w");
                        fclose(archivo);
                        }
                    }
                }
                break;
            default:
                //error o fin
                break;
            }
        //toma el siguiente token
        spliteo = strtok(NULL, " \n");
        }
/*
        int tam;

        for(tam =0; cad[tam]!='\0';tam++){

        }

        printf("el espacio ocupado es: %d y el ultimo caracter es %c\n",tam, cad[tam-1]);
*/
return 1;
}
//desde script

void CreateFolder(char * path)
{
 char buffer2[50];
    snprintf(buffer2, sizeof(buffer2), "mkdir -p %s", path);
    system(buffer2);


}

int main(void)
{
    printf("Hello World!\n");
 char cadena[500];




  analizar(cadena , 500, 0);
CreateFolder("/home/mario/prueba");
CreateFolder("/home/mario/prueba2");
    return 0;
}

