#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define kb 1024
#define mb 1048576

/*****estructuras o tablas *******/
//particion:  puede ser primaria o extendida
typedef struct particion{
   char part_status;// Char Indica si la partición está activa o no
   char part_type; //Char Indica el tipo de partición, primaria o extendida. Tendrá los valores P o E
   char part_fit; //Char Tipo de ajuste de la partición. Tendrá los valores BF (Best), FF (First) o WF (worst)
   int part_start ;//Int Indica en qué byte del disco inicia la partición
   int part_size;// Int Contiene el tamaño total de la partición en bytes.
   char part_name[16];// char[16] Nombre de la partición
}partition;

//MBR

typedef struct mbr{
int mbr_tamano;
//int Tamaño total del disco en bytes
char *mbr_fecha_creacion; //time Fecha y hora de creación del disco
int mbr_disk_signature; //int Número random, que identificará de forma única a cada disco
 partition mbr_partition_1; //partition Estructura con información de la partición 1
 partition mbr_partition_2; //partition Estructura con información de la partición 2
 partition mbr_partition_3; //partition Estructura con información de la partición 3
 partition mbr_partition_4; //partition Estructura con información de la partición 4
}MBR;



/*****lista de funciones y procedimientos a utilizar*/
int crearDisco(char *cadena);
void CreateFolder(char * path);
int analizar(char * cad, int n);


/****ANALIZADOR******/
int analizar(char * cad, int n){
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
       entrada[0]='\0';
       printf("%s\n",cad);

       strcpy(entrada, cad);
       printf("%s\n",entrada);

char * spliteo;
spliteo = strtok(cad, " ");


       /*INGRESO AL AUTOMATA*/
 /*estado = 0*/
       if(spliteo[0]=='#'){
           estado = 0;
       }else if(strcasecmp("mkdisk", spliteo)==0){
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
           crearDisco(entrada);
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
    path[0] = '\0';
    name[0]='\0';
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

                size = atoi(spliteo);
                if(size <0){
                    printf("el tamanio del disco debe de ser mayor a 0 kB\n");
                }
                break;
            case 6:
                if(strcasecmp("k",spliteo)==0){
                    //crear e kilos
                    unit = 'k';

                }else if(strcasecmp("m",spliteo)==0){
                    //crear en megas
                    unit  = 'm';
                }else{
                    //viene un caracter erroneo
                    printf("caracter de unidades invalido\n");

                }
                break;
            case 7:
               strcpy(path,spliteo);

                break;
            case 8:


                for(tam =0; spliteo[tam]!='\0';tam++){

                }
                if(tam>4){
                    if(spliteo[tam-4]=='.'&& spliteo[tam-3]=='d' && spliteo[tam-2]=='s'&& spliteo[tam-1]=='k'){

                        strcpy(name,spliteo);

                    }else{
                        printf("el nombre del disco no posee extension\n");
                    }
                }
                break;
            default:
                //error o fin
                printf("paramentro no valido\n");
                break;
            }
        //toma el siguiente token
        spliteo = strtok(NULL, " \n");
        }

        //verificar que si haya introducido un path
       if(path[0]!='\0'){

           if(name[0]!='\0'){
               if(size>0){
                   FILE* archivo;
                   char buffer2[200];
                   snprintf(buffer2, sizeof(buffer2), "%s/%s", path,name);
                    printf("%s",buffer2);
                   if(unit!='\0'){

                       if(unit=='k'){
                           //creacion KiB

                         CreateFolder(path);
                         archivo = fopen(buffer2,"wb+");
                         char relleno = '\0';
                         long i;
                         for(i = 0; i<(size*kb);i++){
                             fwrite(&relleno,sizeof(char),1,archivo);
                         }

                         fclose(archivo);

                       }else if(unit == 'm'){

                           CreateFolder(path);
                           archivo = fopen(buffer2,"wb+");
                           char relleno = '\0';
                           long i;
                           for(i = 0; i<(size*mb);i++){
                               fwrite(&relleno,sizeof(char),1,archivo);
                           }

                           fclose(archivo);
                       }

                   }else{
                       //creacion por default en MiB
                       CreateFolder(path);
                       archivo = fopen(buffer2,"wb+");
                       char relleno = '\0';
                       long i;
                       for(i = 0; i<(size*mb);i++){
                           fwrite(&relleno,sizeof(char),1,archivo);
                       }

                       fclose(archivo);
                   }

               }else{

                   printf("tamanio del archivo no esta especificado, size: %i\n",size);

               }
           }else{
               printf("nombre del disco no esta especificado\n");
           }

       }else{
           printf("path del disco no esta especificado\n");
       }

return 1;
}
//desde script

//procedimiento para crear folder
void CreateFolder(char * path)
{
 char buffer2[100];
    snprintf(buffer2, sizeof(buffer2), "mkdir -p '%s'", path);
    system(buffer2);


}

int main(void)
{
    printf("Hello World!\n");
 char cadena[500];




  analizar(cadena , 500);



    return 0;
}

