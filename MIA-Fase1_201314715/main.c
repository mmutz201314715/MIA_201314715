#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
char mbr_fecha_creacion[15]; //time Fecha y hora de creación del disco
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
void insert_mbr(char *fname, int size);
int elimDisco(char * cadena);
int fDisk(char * cadena);
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
       }else if(strcasecmp("rmdisk", spliteo)==0){
           estado = 2;
       }else if(strcasecmp("fdisk", spliteo)==0){
           estado = 3;
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
       case 2:
           printf("comando rmdisk\n");
           elimDisco(entrada);
           break;
       case 3:
           printf("comando fdisk\n");
           fDisk(entrada);
           break;
       default:
          //error
           printf("error comando no valido\n");
           break;
       }

       return 1;

}

/***********CREACION DE PARTICIONES**********/
int fDisk(char * cadena){
    int tam;
    int size;
    char unit='\0';
    char path[100];
    char type = '\0';
    char deldesic[10];
    char fit[5];
    char name[100];
    int add;
    int estado = -1;
    path[0] = '\0';
    deldesic[0] = '\0';
    fit[0]='\0';
    name[0] = '\0';
char * spliteo, *split2;
char aux1[100], aux2[100], buffer[100];
int bandera = 0;
    spliteo = strtok(cadena, " ");

    while(spliteo!= NULL){

        if(strcasecmp("fdisk", spliteo) ==0){
                   estado = 0;
          }else if(strcasecmp("-size::", spliteo)==0){
            estado = 1;

        }else if(strcasecmp("+unit::", spliteo)==0){
            estado = 2;
        }else if(strcasecmp("-path::", spliteo)==0){
            estado = 3;
        }else if(strcasecmp("+type::", spliteo)==0){
            estado = 4;
        }else if(strcasecmp("+fit::", spliteo)==0){
            estado = 5;
        }else if(strcasecmp("+delete::", spliteo)==0){
            estado = 6;
        }else if(strcasecmp("-name::", spliteo)==0){
            estado = 7;
        }else if(strcasecmp("+add::", spliteo)==0){
            estado = 8;
        }

        switch (estado) {
        case 0:
           //no hace nada desecha;
            break;
        case 1:
            estado = 9;
            break;
        case 2:
            estado = 10;
            break;
        case 3:
            estado = 11;
            break;
        case 4:
            estado = 12;
            break;
        case 5:
            estado = 13;
            break;
        case 6:
            estado = 14;
            break;
        case 7:
            estado = 15;
            break;
        case 8:
            estado = 16;
            break;
        case 9:
            size = atoi(spliteo);
            if(size <0){
                printf("el tamanio de la particion debe de ser mayor a 2 MB\n");
            }
            estado = 0;
            break;
        case 10:
            if(strcasecmp("K",spliteo)==0){
                //crear e kilos
                unit = 'k';

            }else if(strcasecmp("M",spliteo)==0){
                //crear en megas
                unit  = 'm';
               } else if(strcasecmp("B",spliteo)==0){
                                //crear en megas
                                unit  = 'b';
            }else{
                //viene un caracter erroneo
                printf("caracter de unidades invalido\n");

            }
            estado = 0;
            break;
        case 11:

            for(tam =0; spliteo[tam]!='\0';tam++){

            }
            if(spliteo[tam-1]=='"'){
                //el paramtro no posee espacios
                printf("comilla\n");
                if(spliteo[tam-5]=='.'&& spliteo[tam-4]=='d' && spliteo[tam-3]=='s'&& spliteo[tam-2]=='k'){

                 strcpy(path,spliteo);
                 bandera = 1;
                 estado = 0;
                }
            }else{
                //viene un espacio
                strcpy(path,spliteo);
                estado =17;
            }
            break;
        case 12:
            if(strcasecmp("P",spliteo)==0){
                //crear e kilos
                type = 'P';

            }else if(strcasecmp("E",spliteo)==0){
                //crear en megas
                type  = 'E';
           } else if(strcasecmp("L",spliteo)==0){
                //crear en megas
                type  = 'L';
            }else{
                //viene un caracter erroneo
                printf("caracter de particion invalido\n");

            }
            break;
        case 13:
            if(strcasecmp("BF",spliteo)==0){
                //crear e kilos
                strcpy(fit,spliteo);


            }else if(strcasecmp("FF",spliteo)==0){
                //crear en megas
                strcpy(fit,spliteo);

               } else if(strcasecmp("WF",spliteo)==0){
                                //crear en megas
                strcpy(fit,spliteo);

            }else{
                //viene un caracter erroneo
                printf("caracteres de fit invalidos\n");

            }
            break;
        case 14:
            if(strcasecmp("Fast",spliteo)==0){
                //crear e kilos
                strcpy(deldesic,spliteo);


            }else if(strcasecmp("Full",spliteo)==0){
                //crear en megas
                strcpy(deldesic,spliteo);

            }else{
                //viene un caracter erroneo
                printf("caracteres de delete invalidos\n");

            }

            break;
        case 15:
            strcpy(path,spliteo);

             estado = 0;
            break;
        case 16:
            add = atoi(spliteo);
            break;
        case 17:

            strcpy(aux2, spliteo);
              snprintf(buffer, sizeof(buffer), "%s %s", path, aux2);
              strcpy(path, buffer);

              for(tam =0; spliteo[tam]!='\0';tam++){

              }
              if(spliteo[tam-1]=='\"'){
                                    //el paramtro no posee espacios
                                    printf("comilla\n");
                                    if(spliteo[tam-5]=='.'&& spliteo[tam-4]=='d' && spliteo[tam-3]=='s'&& spliteo[tam-2]=='k'){

                                     bandera = 1;
                                     estado = 0;
                                    }
                                    }else{
                                        estado =17;
                                    }
            break;
        default:
            break;
        }

        spliteo = strtok(NULL, " \n");
    }

    printf("%s\n",path);
    if(path[0]!='\0' && bandera == 1){
        strcpy(aux1 , path);
        split2 = strtok(aux1, "\"");
        strcpy(path,split2);

        printf("si entro \n");

    }else{
        printf("no se especifico el path del disco\n");
    }


return 1;
}


/******ELIMINAR DISCO**********/

int elimDisco(char * cadena){

    int bandera =0;
    char path[100];
    char aux1[100];
    char buffer[100];
    char desic = '\0';
    path[0] = '\0';
    int estado = 0;
    int tam;
    char * spliteo;
    char * split2;
        spliteo = strtok(cadena, " ");
        while (spliteo != NULL){
           if(strcasecmp("rmdisk", spliteo)==0){
               //confirmar que si se recibio el comando rmdisk
               estado = 0;
           }else if(strcasecmp("-path::", spliteo)==0){
               estado = 1;
           }

           switch (estado) {
           case 0:
               //no hacer nada
               break;
           case 1:
               estado = 2;
               break;
           case 2:
               for(tam =0; spliteo[tam]!='\0';tam++){

               }
               if(spliteo[tam-1]=='"'){
                   //el paramtro no posee espacios
                   printf("comilla\n");
                   if(spliteo[tam-5]=='.'&& spliteo[tam-4]=='d' && spliteo[tam-3]=='s'&& spliteo[tam-2]=='k'){

                    strcpy(path,spliteo);
                    bandera = 1;
                    estado = 0;
                   }
               }else{
                   //viene un espacio
                   strcpy(path,spliteo);
                   estado =3;
               }
                break;
           case 3:

                strcpy(aux1, spliteo);
                  snprintf(buffer, sizeof(buffer), "%s %s", path, aux1);
                  strcpy(path, buffer);

                  for(tam =0; spliteo[tam]!='\0';tam++){

                  }
                  if(spliteo[tam-1]=='\"'){
                                        //el paramtro no posee espacios
                                        printf("comilla\n");
                                        if(spliteo[tam-5]=='.'&& spliteo[tam-4]=='d' && spliteo[tam-3]=='s'&& spliteo[tam-2]=='k'){

                                         bandera = 1;
                                         estado = 0;
                                        }
                                        }else{
                                            estado =3;
                                        }




               break;
           default:
               break;
           }


        spliteo = strtok(NULL, " \n");
}

printf("%s\n", path);
        //verificar que si haya introducido un path
       if(path[0]!='\0' && bandera == 1){
           strcpy(aux1 , path);
           split2 = strtok(aux1, "\"");
           strcpy(path,split2);
           printf("Esta seguro de eliminar el disco : 1. Si / cualquier letra. No\n");
           scanf("%s",&desic);

           if(desic =='1'){
               int i =remove(path);
              if(i==0){
                  printf("Se ha eliminado el disco exitosamente\n");
              }else{
                  printf("ERROR, imposible borrar el archivo\n");
              }
           }else{
               printf("Se ha cancelado la operacion de eliminar un disco\n");
           }

       }else{
           printf("no se especifico el path a eliminar\n");
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
    char aux1[101];
    char aux2[101], aux3[100], aux4[100], buffer3[100], buffer[100];
    char *split2;
    char *split3;
    int bandera1 = 0;
    int bandera2 = 0;
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
                estado = 0;
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
                estado = 0;
                break;
            case 7:
                for(tam =0; spliteo[tam]!='\0';tam++){

                }
                if(spliteo[tam-1]=='"'){
                    //el paramtro no posee espacios
                    printf("comilla\n");
                    strcpy(path,spliteo);
                    estado = 0;
                    bandera1 = 1;
                }else{
                    strcpy(path,spliteo);
                    estado = 9;
                }



                break;
            case 8:


                for(tam =0; spliteo[tam]!='\0';tam++){

                }
                if(spliteo[tam-1]=='"'){
                if(tam>5){
                    if(spliteo[tam-5]=='.'&& spliteo[tam-4]=='d' && spliteo[tam-3]=='s'&& spliteo[tam-2]=='k'){

                        strcpy(name,spliteo);
                        bandera2 = 1;
                        estado = 0;

                    }else{
                        printf("el nombre del disco no posee extension\n");
                        estado = 0;
                    }
                }
                }else{
                    strcpy(name, spliteo);
                    estado = 10;
                }


                break;
            case 9:
                strcpy(aux3, spliteo);
                  snprintf(buffer, sizeof(buffer), "%s %s", path, aux3);
                  strcpy(path, buffer);

                  for(tam =0; spliteo[tam]!='\0';tam++){

                  }
                  if(spliteo[tam-1]=='\"'){
                                        //el paramtro no posee espacios
                                        printf("comilla\n");
                                        bandera1 = 1;
                                        estado = 0;

                  }else{
                      estado = 9;
                  }

                break;
            case 10:
                strcpy(aux4, spliteo);
                  snprintf(buffer3, sizeof(buffer3), "%s %s", name, aux4);
                  strcpy(name, buffer3);

                  for(tam =0; spliteo[tam]!='\0';tam++){

                  }
                  if(spliteo[tam-1]=='"'){
                  if(tam>5){
                      if(spliteo[tam-5]=='.'&& spliteo[tam-4]=='d' && spliteo[tam-3]=='s'&& spliteo[tam-2]=='k'){


                          bandera2 = 1;
                          estado = 0;

                      }else{
                          printf("el nombre del disco no posee extension\n");
                          estado = 0;
                      }
                  }
                  }else{

                      estado = 10;
                  }
                break;
            default:
                //error o fin
                printf("paramentro no valido\n");
                break;
            }
        //toma el siguiente token
        printf("split: %s\n",spliteo);
        spliteo = strtok(NULL, " \n");
        }

        //verificar que si haya introducido un path
       if(path[0]!='\0' && bandera1 == 1){
            strcpy(aux1 , path);
            split2 = strtok(aux1, "\"");
            strcpy(path,split2);
           if(name[0]!='\0' && bandera2 == 1){
               if(size>0){

                   strcpy(aux2,name);
                   split3 = strtok(aux2, "\"");
                   strcpy(name, split3);

                   FILE* archivo;
                   char buffer2[200];
                   snprintf(buffer2, sizeof(buffer2), "%s%s", path,name);
                    printf("%s\n",buffer2);
                   if(unit!='\0'){

                       if(unit=='k' && size >= 10240){
                           //creacion KiB

                         CreateFolder(path);
                         archivo = fopen(buffer2,"wb+");
                         char relleno = '\0';
                         long i;
                         for(i = 0; i<(size*kb);i++){
                             fwrite(&relleno,sizeof(char),1,archivo);
                         }

                         fclose(archivo);
                         insert_mbr(buffer2, size*kb);

                       }else if(unit == 'm' && size >=10){

                           CreateFolder(path);
                           archivo = fopen(buffer2,"wb+");
                           char relleno = '\0';
                           long i;
                           for(i = 0; i<(size*mb);i++){
                               fwrite(&relleno,sizeof(char),1,archivo);
                           }

                           fclose(archivo);
                           insert_mbr(buffer2, size*mb);
                       }

                   }else if(unit == '\0' && size>=10){
                       //creacion por default en MiB
                       CreateFolder(path);
                       archivo = fopen(buffer2,"wb+");
                       char relleno = '\0';
                       long i;
                       for(i = 0; i<(size*mb);i++){
                           fwrite(&relleno,sizeof(char),1,archivo);
                       }

                       fclose(archivo);
                       insert_mbr(buffer2, size*mb);
                   }else{

                       printf("tamanio de disco demasiado pequeno, debe ser mayor a 10 MB\n");
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

//procedimiento para insertar el mbr en el disco creado
void insert_mbr(char *fname, int size){
    MBR nmbr, aux;
    partition part1, part2 , part3, part4;

    time_t t;
      struct tm *tm;
      char fechayhora[15];

      t=time(NULL);
      tm=localtime(&t);
      strftime(fechayhora, 15, "%d/%m/%Y", tm);
      printf ("Hoy es: %s\n", fechayhora);

      part1.part_status = 'i';
      part2.part_status = 'i';
      part3.part_status = 'i';
      part4.part_status = 'i';

      nmbr.mbr_partition_1 = part1;
      nmbr.mbr_partition_2 = part2;
      nmbr.mbr_partition_3 = part3;
      nmbr.mbr_partition_4 = part4;

    strcpy(nmbr.mbr_fecha_creacion, fechayhora);
    nmbr.mbr_tamano = size;
    FILE *fichero = fopen(fname , "rb+");

     if(fichero){
              fseek(fichero,0,SEEK_SET);
               fwrite(&nmbr, sizeof(MBR),1,fichero);


               //leer el archivo
               fseek(fichero,0,SEEK_SET);
               fread(&aux,sizeof(MBR),1,fichero);

               printf("la fecha del mbr es: %s\n", aux.mbr_fecha_creacion);

               fclose(fichero);
         }

}


int main(void)
{
    printf(":::::::::::::Bienvenido::::::::\n");
 char cadena[500];




  analizar(cadena , 500);



    return 0;
}

