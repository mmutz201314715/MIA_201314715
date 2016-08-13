#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define kb 1024
#define mb 1048576
#define mx 100

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
int mbr_tamano;//int Tamaño total del disco en bytes
//int eflag; //bandera utilizada para saber si existe una particion extendida
//int contp; //bandera utilizada para contar cuantas particiones se encuentran activas
char mbr_fecha_creacion[15]; //time Fecha y hora de creación del disco
int mbr_disk_signature; //int Número random, que identificará de forma única a cada disco
 partition mbr_partition_1; //partition Estructura con información de la partición 1
 partition mbr_partition_2; //partition Estructura con información de la partición 2
 partition mbr_partition_3; //partition Estructura con información de la partición 3
 partition mbr_partition_4; //partition Estructura con información de la partición 4
}MBR;

/*estructuras para montar*/

typedef struct part{
    partition info;
    int id;
    char iddisco;
    char * path;
    struct part * siguiente;
    struct part *anterior;

}montado;

typedef struct ListaId{
    montado *inicio, *fin;

}Lista;

/*****lista de funciones y procedimientos a utilizar*/

int crearDisco(char *cadena);
void CreateFolder(char * path);
int analizar(char * cad, int n, Lista * lista, int ward);
void insert_mbr(char *fname, int size);
int elimDisco(char * cadena);
int fDisk(char * cadena);
int crearParticion(char * path, char * name, int size, char tipo, char *fit);
void quitarPart(char *path, char *name, char * deldesic);
void aumentarPart(char *path, char *name, int add);
void mount(Lista * lista, char *cadena);
void inicializar(Lista * lista);
void montarPart(Lista * lista, char* path, char *name);
int buscarpart(Lista * lista, char* path, char *nombre);
char seekpart(Lista * lista, char* path);
void mostrarmount(Lista * lista);
void umount(Lista * lista, char * cadena);
void desmontador(Lista * lista, char * identificador);
void actetiqueta(Lista * lista, char iddisk, int id);
void exec(Lista *lista ,char *cadena);
void ejecutar(Lista * lista, char * path);
void rep(Lista *lista , char * cadena);
void repmbr(Lista *lista, char * identificador, char *pathrep);
void repdisk(Lista *lista, char * identificador, char * pathrep);
/****ANALIZADOR******/

int analizar(char * cad, int n, Lista *lista, int ward){
int estado  = 0;

    /***LECTURA DE CADENA**/
    int i, c;
if(ward == 1){
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
}

       char entrada[1000];
       entrada[0]='\0';


       strcpy(entrada, cad);


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
       }else if(strcasecmp("mount", spliteo)==0){
           estado = 4;
       }else if(strcasecmp("umount", spliteo)==0){
           estado = 5;
       }else if(strcasecmp("exec", spliteo)==0){
           estado = 6;
       }else if(strcasecmp("rep", spliteo)==0){
           estado = 7;
       }
       else{
           //errores de escritura

           estado = -1;
       }

       switch(estado){
       case 0:
           printf("comentario: %s\n", entrada);
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
       case 4:
           printf("comando mount\n");
           mount(lista, entrada);

           break;
       case 5:
           printf("comando mount\n");
           umount(lista, entrada);

           break;
       case 6:
           printf("comando exec\n");
           exec(lista, entrada);

           break;
       case 7:
           printf("comando rep\n");
           rep(lista , entrada);

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
    int size = -1;
    char unit='\0';
    char path[100];
    char type = '\0';
    char deldesic[10];
    char fit[5];
    char name[16];
    int add;
    int estado = -1;
    path[0] = '\0';
    deldesic[0] = '\0';
    fit[0]='\0';
    name[0] = '\0';
char * spliteo, *split2;
char aux1[100], aux2[100], buffer[100], aux3[16],aux4[16], buffer2[16];
int bandera = 0;
int bandera2= 0;
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
                //crear como primaria
                type = 'P';

            }else if(strcasecmp("E",spliteo)==0){
                //crear como extendida
                type  = 'E';
           } else if(strcasecmp("L",spliteo)==0){
                //crear como logica
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
            for(tam =0; spliteo[tam]!='\0';tam++){

            }
            if(tam<16){


            if(spliteo[tam-1]=='"'){
                //el paramtro no posee espacios

                strcpy(name,spliteo);
                estado = 0;
                bandera2 = 1;
            }else{
                strcpy(name,spliteo);
                estado = 18;
            }
            }else{
                printf("el nombre de la particion no de debe ser mayor a 15 caracteres\n");
                estado = 0;
            }

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

                                    if(spliteo[tam-5]=='.'&& spliteo[tam-4]=='d' && spliteo[tam-3]=='s'&& spliteo[tam-2]=='k'){

                                     bandera = 1;
                                     estado = 0;
                                    }
                                    }else{
                                        estado =17;
                                    }
            break;
        case 18:
            for(tam =0; spliteo[tam]!='\0';tam++){

            }
            tam++;
            for(tam = tam; name[tam]!='\0';tam++){

            }
            if(tam<16){
            strcpy(aux3, spliteo);
              snprintf(buffer2, sizeof(buffer2), "%s %s", name, aux3);
              strcpy(name, buffer2);

                for(tam =0; spliteo[tam]!='\0';tam++){

            }
              if(spliteo[tam-1]=='\"'){
                                    //el paramtro no posee espacios

                                    bandera2 = 1;
                                    estado = 0;

              }else{
                  estado = 18;
              }
            }else{
                printf("el nombre de la particion no de debe ser mayor a 15 caracteres\n");
                estado = 0;
            }
            break;
        default:

            break;
        }

        spliteo = strtok(NULL, " \n");
    }


    if(path[0]!='\0' && bandera == 1){
        strcpy(aux1 , path);
        split2 = strtok(aux1, "\"");
        strcpy(path,split2);

        //
        if(name[0]!='\0' && bandera2 == 1){
            strcpy(aux4, name);
            split2 = strtok(aux4, "\"");
            strcpy(name, split2);


            if(size>0){

                if(unit!='\0'){
                    if(unit=='b' && size>= (mb*2)){
                       int d= crearParticion(path, name, size,type,fit);
                       if(d== 0){
                           printf("el nombre ya existe\n");
                       }
                    }else if(unit == 'k' && size>= 2048){
                       int d = crearParticion(path, name, (size*kb),type,fit);
                       if(d== 0){
                           printf("el nombre ya existe\n");
                       }
                    }else if(unit == 'm' && size>=2){
                       int d = crearParticion(path, name, (size*mb),type,fit);
                       if(d== 0){
                           printf("el nombre ya existe\n");
                       }
                    }
                }else if(unit == '\0' && size>=2048){
                   int d = crearParticion(path, name, (size*kb),type,fit);
                    if(d== 0){
                        printf("el nombre ya existe\n");
                    }
                }

            }else if(size == -1){
                //aqui van los comandos para delete y add
                if(deldesic[0]!='\0'){
                    quitarPart(path, name, deldesic);
                }else if(deldesic[0]=='\0' && add!=0){
                    if(unit != '\0'){
                        if(unit == 'b'){
                        aumentarPart(path, name,add);
                        }else if(unit == 'k'){
                            aumentarPart(path, name,add*kb);
                        }else if(unit == 'm'){
                            aumentarPart(path, name,add*mb);
                        }else{
                            printf("parametro invalido en unit\n");
                        }
                    }else{
                        printf("unit debe de estar especificado para hacer uso de este subcomando\n");
                    }

                }else{
                    printf("solo puede haber un comando add o desic a la vez\n");
                }
            }else{
               printf("el tamanio de la particion no puede menor a 2 MB\n");
            }



        }else{
            printf("el nombre no se especifico de manera correcta\n");
        }


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
int pos;
    time_t t;
      struct tm *tm;
      char fechayhora[15];

      t=time(NULL);



      tm=localtime(&t);
      strftime(fechayhora, 15, "%d/%m/%Y", tm);


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
    int hora = time(NULL);
    srand(hora);

    nmbr.mbr_disk_signature = rand()% (1000000+1);
    FILE *fichero = fopen(fname , "rb+");

     if(fichero){
              fseek(fichero,0,SEEK_SET);
              pos  = ftell(fichero);

               fwrite(&nmbr, sizeof(MBR),1,fichero);


               //leer el archivo
               fseek(fichero,0,SEEK_SET);
               fread(&aux,sizeof(MBR),1,fichero);

               printf("la fecha del mbr es: %s\n", aux.mbr_fecha_creacion);
               fseek(fichero,0,SEEK_SET);
               fclose(fichero);
         }

}

//funcion para crear una particion en el disco
int crearParticion(char * path, char * name, int size, char tipo, char *fit){
MBR aux, mod, cp2;
partition auxp;
//int pos;
auxp.part_status = 'a';

strcpy(auxp.part_name, name);
auxp.part_size = size;


if(tipo!='\0'){
    auxp.part_type=tipo;
}else{
    auxp.part_type = 'P';
}
if(fit[0]!='\0'){
    auxp.part_fit = fit[0];
}else{
    auxp.part_fit = 'W';
}



FILE * archivo;

    archivo=fopen(path,"rb+");
    if(archivo){

                fseek(archivo,0,SEEK_SET);
                fread(&aux,sizeof(MBR),1,archivo);
           fclose(archivo);
    }


    int contp= 0;

    if(aux.mbr_partition_1.part_status =='i'){
        contp++;
    }
    if(aux.mbr_partition_2.part_status == 'i'){
        contp++;
    }
    if(aux.mbr_partition_3.part_status == 'i'){
        contp++;
    }
    if(aux.mbr_partition_4.part_status =='i'){
        contp++;
    }
    int p1i ,p1f, p2i, p2f, p3i, p3f, p4i, p4f; //si valen -1 quiere decir que se encuentra libre esa region
    if(contp>0){

        if(aux.mbr_partition_1.part_status=='i'){
            p1i = -1;
            p1f = -1;
        }else{
            if(strcasecmp(aux.mbr_partition_1.part_name,name)==0){
                return 0;
            }
            p1i = aux.mbr_partition_1.part_start;
            p1f = aux.mbr_partition_1.part_size+p1i;
        }
        if(aux.mbr_partition_2.part_status=='i'){
            p2i = -1;
            p2f = -1;
        }else{
            if(strcasecmp(aux.mbr_partition_2.part_name,name)==0){
                 return 0;
             }
            p2i = aux.mbr_partition_2.part_start;
            p2f = aux.mbr_partition_2.part_size+p2i;
        }
        if(aux.mbr_partition_3.part_status=='i'){
            p3i = -1;
            p3f = -1;
        }else{
            if(strcasecmp(aux.mbr_partition_3.part_name,name)==0){
                 return 0;
             }
            p3i = aux.mbr_partition_3.part_start;
            p3f = aux.mbr_partition_3.part_size+p3i;
        }
        if(aux.mbr_partition_4.part_status=='i'){
            p4i = -1;
            p4f = -1;
        }else{
            if(strcasecmp(aux.mbr_partition_4.part_name,name)==0){
                 return 0;
             }
            p4i = aux.mbr_partition_4.part_start;
            p4f = aux.mbr_partition_4.part_size+p4i;
        }

        if(contp == 4){
        if(auxp.part_size<(aux.mbr_tamano-136)){
            auxp.part_start = 137;
            archivo=fopen(path,"rb+");
            if(archivo){
            mod = aux;
            mod.mbr_partition_1 = auxp;
            printf("status de la particion:: %c\n",mod.mbr_partition_1.part_status);
            fseek(archivo,0,SEEK_SET);
             fwrite(&mod, sizeof(MBR),1,archivo);

             fseek(archivo,0,SEEK_SET);
             fread(&cp2,sizeof(MBR),1,archivo);

             fclose(archivo);
            }else{
                   printf("no fue posible insertar la particion, espacio insuficiente en el disco\n");
               }
            }
        }else if(contp==3){
            //si la ocupada es p1
            if(p1i != -1){

               if(auxp.part_size<(aux.mbr_tamano-p1f)){
                   auxp.part_start = p1f+1;
                   archivo=fopen(path,"rb+");
                   if(archivo){
                   mod = aux;
                   mod.mbr_partition_2 = auxp;
                   printf("status de la particion:: %c\n",mod.mbr_partition_2.part_status);
                   fseek(archivo,0,SEEK_SET);
                   fwrite(&mod, sizeof(MBR),1,archivo);
                    fclose(archivo);
                   }
               }else{
                   printf("no fue posible insertar la particion, espacio insuficiente en el disco\n");
               }

            }else if(p2i != -1){
                //si la ocupada es p2
                    int e3 = -1;

                   if(auxp.part_size<(aux.mbr_tamano-p2f)){
                        e3 = 2;
                    }
                   if(auxp.part_size<(p2i-136)){
                        e3=1;
                    }

                    switch (e3) {
                    case 1:
                        auxp.part_start = 137;
                        archivo=fopen(path,"rb+");
                        if(archivo){
                        mod = aux;
                        mod.mbr_partition_1 = auxp;
                        printf("status de la particion:: %c\n",mod.mbr_partition_1.part_status);
                        fseek(archivo,0,SEEK_SET);
                         fwrite(&mod, sizeof(MBR),1,archivo);

                         fclose(archivo);
                        }
                        break;
                    case 2:
                        auxp.part_start = p2f+1;
                        archivo=fopen(path,"rb+");
                        if(archivo){
                        mod = aux;
                        mod.mbr_partition_3 = auxp;
                        printf("status de la particion:: %c\n",mod.mbr_partition_3.part_status);
                        fseek(archivo,0,SEEK_SET);
                         fwrite(&mod, sizeof(MBR),1,archivo);

                         fclose(archivo);
                        }
                        break;
                    default:
                        printf("no fue posible insertar la particion, espacio insuficiente en el disco\n");
                        break;
                    }

            }else if(p3i != -1){
                //si la ocupada es p3
                int e3 = -1;

                if(auxp.part_size<(aux.mbr_tamano-p3f)){
                    e3 = 2;
                }
                if(auxp.part_size<(p3i-136)){
                    e3=1;
                }

                switch (e3) {
                case 1:
                    auxp.part_start = 137;
                    archivo=fopen(path,"rb+");
                    if(archivo){
                    mod = aux;
                    mod.mbr_partition_1 = auxp;
                    printf("status de la particion:: %c\n",mod.mbr_partition_1.part_status);
                    fseek(archivo,0,SEEK_SET);
                     fwrite(&mod, sizeof(MBR),1,archivo);

                     fclose(archivo);
                    }
                    break;
                case 2:
                    auxp.part_start = p3f+1;
                    archivo=fopen(path,"rb+");
                    if(archivo){
                    mod = aux;
                    mod.mbr_partition_4 = auxp;
                    printf("status de la particion:: %c\n",mod.mbr_partition_4.part_status);
                    fseek(archivo,0,SEEK_SET);
                     fwrite(&mod, sizeof(MBR),1,archivo);

                     fclose(archivo);
                    }
                    break;
                default:
                    printf("no fue posible insertar la particion, espacio insuficiente en el disco\n");
                    break;
                }
            }else if(p4i != -1){
                int e3 =-1;
                 partition auxpm;
                if(auxp.part_size<(aux.mbr_tamano-p4f)){
                    e3 = 2;
                }
                if(auxp.part_size<(p4i-136)){
                    e3 = 1;
                }
                switch (e3) {
                case 1:
                    auxp.part_start = 137;
                    archivo=fopen(path,"rb+");
                    if(archivo){
                    mod = aux;
                    mod.mbr_partition_1 = auxp;
                    printf("status de la particion:: %c\n",mod.mbr_partition_1.part_status);
                    fseek(archivo,0,SEEK_SET);
                    fwrite(&mod, sizeof(MBR),1,archivo);
                     fclose(archivo);
                    }
                    break;
                case 2:

                    auxpm = aux.mbr_partition_4;
                    aux.mbr_partition_3 = auxpm;
                    auxp.part_start = p4f+1;
                    mod = aux;
                    mod.mbr_partition_4 = auxp;
                    archivo=fopen(path,"rb+");
                    if(archivo){
                    printf("status de la particion:: %c\n",mod.mbr_partition_4.part_status);
                    fseek(archivo,0,SEEK_SET);
                    fwrite(&mod, sizeof(MBR),1,archivo);
                     fclose(archivo);
                    }
                    break;
                default:
                    printf("no fue posible insertar la particion, espacio insuficiente en el disco\n");
                    break;
                }


            }
            printf("ya tiene 1 particiones\n");

        }else if(contp == 2){

            if(p1i!= -1 && p2i!=-1){
                int e3 = -1;
                 partition auxpm;

                if(auxp.part_size<(aux.mbr_tamano-p2f)){
                     e3 = 2;
                 }
                if(auxp.part_size<(p2i-p1f)){
                    e3=1;
                }

                switch (e3) {
                case 1:
                    auxpm = aux.mbr_partition_2;
                    aux.mbr_partition_3 = auxpm;
                    auxp.part_start = p1f+1;
                    mod = aux;
                    mod.mbr_partition_2 = auxp;
                    archivo=fopen(path,"rb+");
                    if(archivo){
                    printf("status de la particion:: %c\n",mod.mbr_partition_2.part_status);
                    fseek(archivo,0,SEEK_SET);
                    fwrite(&mod, sizeof(MBR),1,archivo);
                     fclose(archivo);
                    }


                    break;
                case 2:

                    auxp.part_start = p2f+1;
                    archivo=fopen(path,"rb+");
                    if(archivo){
                    mod = aux;
                    mod.mbr_partition_3 = auxp;
                    printf("status de la particion:: %c\n",mod.mbr_partition_3.part_status);
                    fseek(archivo,0,SEEK_SET);
                    fwrite(&mod, sizeof(MBR),1,archivo);
                     fclose(archivo);
                    }
                    break;
                default:
                    printf("no fue posible insertar la particion, espacio insuficiente en el disco\n");
                    break;
                }

            }else if(p1i!=-1 && p3i!=-1){

                int e3 = -1;


                if(auxp.part_size<(aux.mbr_tamano-p3f)){
                     e3 = 2;
                 }
                if(auxp.part_size<(p3i-p1f)){
                    e3=1;
                }

                switch (e3) {
                case 1:
                    auxp.part_start = p1f+1;
                    mod = aux;
                    mod.mbr_partition_2 = auxp;
                    archivo=fopen(path,"rb+");
                    if(archivo){
                    printf("status de la particion:: %c\n",mod.mbr_partition_2.part_status);
                    fseek(archivo,0,SEEK_SET);
                    fwrite(&mod, sizeof(MBR),1,archivo);
                     fclose(archivo);
                    }


                    break;
                case 2:

                    auxp.part_start = p3f+1;
                    archivo=fopen(path,"rb+");
                    if(archivo){
                    mod = aux;
                    mod.mbr_partition_4 = auxp;
                    printf("status de la particion:: %c\n",mod.mbr_partition_4.part_status);
                    fseek(archivo,0,SEEK_SET);
                    fwrite(&mod, sizeof(MBR),1,archivo);
                     fclose(archivo);
                    }
                    break;
                default:
                    printf("no fue posible insertar la particion, espacio insuficiente en el disco\n");
                    break;
                }

            }else if(p1i!=-1 && p4i!= -1){

                int e3 = -1;
                 partition auxpm;

                if(auxp.part_size<(aux.mbr_tamano-p4f)){
                     e3 = 2;
                 }
                if(auxp.part_size<(p4i-p1f)){
                    e3=1;
                }

                switch (e3) {
                case 1:

                    auxp.part_start = p1f+1;
                    mod = aux;
                    mod.mbr_partition_2 = auxp;
                    archivo=fopen(path,"rb+");
                    if(archivo){
                    printf("status de la particion:: %c\n",mod.mbr_partition_2.part_status);
                    fseek(archivo,0,SEEK_SET);
                    fwrite(&mod, sizeof(MBR),1,archivo);
                     fclose(archivo);
                    }


                    break;
                case 2:
                    auxpm = aux.mbr_partition_4;
                    aux.mbr_partition_3 = auxpm;
                    auxp.part_start = p4f+1;
                    archivo=fopen(path,"rb+");
                    if(archivo){
                    mod = aux;
                    mod.mbr_partition_4 = auxp;
                    printf("status de la particion:: %c\n",mod.mbr_partition_4.part_status);
                    fseek(archivo,0,SEEK_SET);
                    fwrite(&mod, sizeof(MBR),1,archivo);
                     fclose(archivo);
                    }
                    break;
                default:
                    printf("no fue posible insertar la particion, espacio insuficiente en el disco\n");
                    break;
                }

            }else if(p2i!=-1 && p3i!= -1){

                int e3 = -1;
                 partition auxpm;
                 if(auxp.part_size<(aux.mbr_tamano-p3f)){
                     e3 = 3;
                 }

                 if(auxp.part_size<(p3i-p2f)){
                     e3 = 2;
                 }

                 if(auxp.part_size<(p2i-136)){
                   e3 = 1;
               }
                 switch (e3) {
                 case 1:
                     auxp.part_start = 136+1;
                     mod = aux;
                     mod.mbr_partition_1 = auxp;
                     archivo=fopen(path,"rb+");
                     if(archivo){
                     printf("status de la particion:: %c\n",mod.mbr_partition_1.part_status);
                     fseek(archivo,0,SEEK_SET);
                     fwrite(&mod, sizeof(MBR),1,archivo);
                      fclose(archivo);
                     }
                     break;
                 case 2:

                     auxpm = aux.mbr_partition_3;
                     aux.mbr_partition_4 = auxpm;
                     auxp.part_start = p2f+1;
                     archivo=fopen(path,"rb+");
                     if(archivo){
                     mod = aux;
                     mod.mbr_partition_3 = auxp;
                     printf("status de la particion:: %c\n",mod.mbr_partition_3.part_status);
                     fseek(archivo,0,SEEK_SET);
                     fwrite(&mod, sizeof(MBR),1,archivo);
                      fclose(archivo);
                     }

                     break;
                 case 3:

                     auxp.part_start = p2f+1;
                     mod = aux;
                     mod.mbr_partition_3 = auxp;
                     archivo=fopen(path,"rb+");
                     if(archivo){
                     printf("status de la particion:: %c\n",mod.mbr_partition_3.part_status);
                     fseek(archivo,0,SEEK_SET);
                     fwrite(&mod, sizeof(MBR),1,archivo);
                      fclose(archivo);
                     }
                     break;
                 default:
                     printf("no fue posible insertar la particion, espacio insuficiente en el disco\n");
                     break;
                 }

            }else if(p2i!=-1 && p4i!= -1){

                int e3 = -1;
                 partition auxpm;
                 if(auxp.part_size<(aux.mbr_tamano-p4f)){
                     e3 = 3;
                 }

                 if(auxp.part_size<(p4i-p2f)){
                     e3 = 2;
                 }

                 if(auxp.part_size<(p2i-136)){
                   e3 = 1;
               }
                 switch (e3) {
                 case 1:
                     auxp.part_start = 136+1;
                     mod = aux;
                     mod.mbr_partition_1 = auxp;
                     archivo=fopen(path,"rb+");
                     if(archivo){
                     printf("status de la particion:: %c\n",mod.mbr_partition_1.part_status);
                     fseek(archivo,0,SEEK_SET);
                     fwrite(&mod, sizeof(MBR),1,archivo);
                      fclose(archivo);
                     }
                     break;
                 case 2:

                     auxp.part_start = p2f+1;
                     mod = aux;
                     mod.mbr_partition_3 = auxp;
                     archivo=fopen(path,"rb+");
                     if(archivo){
                     printf("status de la particion:: %c\n",mod.mbr_partition_3.part_status);
                     fseek(archivo,0,SEEK_SET);
                     fwrite(&mod, sizeof(MBR),1,archivo);
                      fclose(archivo);
                     }


                     break;
                 case 3:
                     auxpm = aux.mbr_partition_4;
                     aux.mbr_partition_3 = auxpm;
                     auxp.part_start = p4f+1;
                     archivo=fopen(path,"rb+");
                     if(archivo){
                     mod = aux;
                     mod.mbr_partition_4 = auxp;
                     printf("status de la particion:: %c\n",mod.mbr_partition_4.part_status);
                     fseek(archivo,0,SEEK_SET);
                     fwrite(&mod, sizeof(MBR),1,archivo);
                      fclose(archivo);
                     }
                     break;
                 default:
                     printf("no fue posible insertar la particion, espacio insuficiente en el disco\n");
                     break;
                 }

            }else if(p3i!=-1 && p4i!= -1){

                int e3 = -1;
                 partition auxpm, auxpm2;
                 if(auxp.part_size<(aux.mbr_tamano-p4f)){
                     e3 = 3;
                 }

                 if(auxp.part_size<(p4i-p3f)){
                     e3 = 2;
                 }

                 if(auxp.part_size<(p3i-136)){
                   e3 = 1;
               }
                 switch (e3) {
                 case 1:
                     auxp.part_start = 136+1;
                     mod = aux;
                     mod.mbr_partition_1 = auxp;
                     archivo=fopen(path,"rb+");
                     if(archivo){
                     printf("status de la particion:: %c\n",mod.mbr_partition_1.part_status);
                     fseek(archivo,0,SEEK_SET);
                     fwrite(&mod, sizeof(MBR),1,archivo);
                      fclose(archivo);
                     }
                     break;
                 case 2:

                     auxpm = aux.mbr_partition_3;
                     aux.mbr_partition_2 = auxpm;
                     auxp.part_start = p3f+1;
                     archivo=fopen(path,"rb+");
                     if(archivo){
                     mod = aux;
                     mod.mbr_partition_3 = auxp;
                     printf("status de la particion:: %c\n",mod.mbr_partition_3.part_status);
                     fseek(archivo,0,SEEK_SET);
                     fwrite(&mod, sizeof(MBR),1,archivo);
                      fclose(archivo);
                     }

                     break;
                 case 3:
                     auxpm = aux.mbr_partition_3;
                     aux.mbr_partition_2 = auxpm;
                     auxpm2 = aux.mbr_partition_4;
                     aux.mbr_partition_3 = auxpm2;
                     auxp.part_start = p4f+1;
                     mod = aux;
                     mod.mbr_partition_4 = auxp;
                     archivo=fopen(path,"rb+");
                     if(archivo){
                     printf("status de la particion:: %c\n",mod.mbr_partition_4.part_status);
                     fseek(archivo,0,SEEK_SET);
                     fwrite(&mod, sizeof(MBR),1,archivo);
                      fclose(archivo);
                     }
                     break;
                 default:
                     printf("no fue posible insertar la particion, espacio insuficiente en el disco\n");
                     break;
                 }
            }
        }else if(contp == 1){
            if(p1i == -1){

                int e3 = -1;
                 partition auxpm, auxpm2, auxpm3;
                 if(auxp.part_size<(aux.mbr_tamano-p4f)){
                     e3 = 4;
                 }

                 if(auxp.part_size<(p4i-p3f)){
                     e3 = 3;
                 }
                 if(auxp.part_size<(p3i-p2f)){
                     e3 = 2;
                 }

                 if(auxp.part_size<(p2i-136)){
                   e3 = 1;
               }
                 switch (e3) {
                 case 1:
                     auxp.part_start = 136+1;
                     mod = aux;
                     mod.mbr_partition_1 = auxp;
                     archivo=fopen(path,"rb+");
                     if(archivo){
                     printf("status de la particion:: %c\n",mod.mbr_partition_1.part_status);
                     fseek(archivo,0,SEEK_SET);
                     fwrite(&mod, sizeof(MBR),1,archivo);
                      fclose(archivo);
                     }
                     break;
                 case 2:

                     auxpm = aux.mbr_partition_2;
                     aux.mbr_partition_1 = auxpm;
                     auxp.part_start = p2f+1;
                     archivo=fopen(path,"rb+");
                     if(archivo){
                     mod = aux;
                     mod.mbr_partition_2 = auxp;
                     printf("status de la particion:: %c\n",mod.mbr_partition_2.part_status);
                     fseek(archivo,0,SEEK_SET);
                     fwrite(&mod, sizeof(MBR),1,archivo);
                      fclose(archivo);
                     }

                     break;
                 case 3:
                     auxpm = aux.mbr_partition_2;
                     aux.mbr_partition_1 = auxpm;
                     auxpm2 = aux.mbr_partition_3;
                     aux.mbr_partition_2 = auxpm2;
                     auxp.part_start = p3f+1;
                     mod = aux;
                     mod.mbr_partition_3 = auxp;
                     archivo=fopen(path,"rb+");
                     if(archivo){
                     printf("status de la particion:: %c\n",mod.mbr_partition_3.part_status);
                     fseek(archivo,0,SEEK_SET);
                     fwrite(&mod, sizeof(MBR),1,archivo);
                      fclose(archivo);
                     }
                     break;
                 case 4:
                     auxpm = aux.mbr_partition_2;
                     aux.mbr_partition_1 = auxpm;
                     auxpm2 = aux.mbr_partition_3;
                     aux.mbr_partition_2 = auxpm2;
                     auxpm3 = aux.mbr_partition_4;
                     aux.mbr_partition_3 = auxpm3;
                     auxp.part_start = p4f+1;
                     mod = aux;
                     mod.mbr_partition_4 = auxp;
                     archivo=fopen(path,"rb+");
                     if(archivo){
                     printf("status de la particion:: %c\n",mod.mbr_partition_4.part_status);
                     fseek(archivo,0,SEEK_SET);
                     fwrite(&mod, sizeof(MBR),1,archivo);
                      fclose(archivo);
                     }
                     break;
                 default:
                     printf("no fue posible insertar la particion, espacio insuficiente en el disco\n");
                     break;
                 }

            }else if(p2i == -1){

                int e3 = -1;
                 partition auxpm, auxpm2;
                 if(auxp.part_size<(aux.mbr_tamano-p4f)){
                     e3 = 3;
                 }

                 if(auxp.part_size<(p4i-p3f)){
                     e3 = 2;
                 }

                 if(auxp.part_size<(p3i-p1f)){
                   e3 = 1;
               }
                 switch (e3) {
                 case 1:
                     auxp.part_start = p1f+1;
                     mod = aux;
                     mod.mbr_partition_2 = auxp;
                     archivo=fopen(path,"rb+");
                     if(archivo){
                     printf("status de la particion:: %c\n",mod.mbr_partition_2.part_status);
                     fseek(archivo,0,SEEK_SET);
                     fwrite(&mod, sizeof(MBR),1,archivo);
                      fclose(archivo);
                     }
                     break;
                 case 2:

                     auxpm = aux.mbr_partition_3;
                     aux.mbr_partition_2 = auxpm;
                     auxp.part_start = p3f+1;
                     archivo=fopen(path,"rb+");
                     if(archivo){
                     mod = aux;
                     mod.mbr_partition_3 = auxp;
                     printf("status de la particion:: %c\n",mod.mbr_partition_3.part_status);
                     fseek(archivo,0,SEEK_SET);
                     fwrite(&mod, sizeof(MBR),1,archivo);
                      fclose(archivo);
                     }

                     break;
                 case 3:
                     auxpm = aux.mbr_partition_3;
                     aux.mbr_partition_2 = auxpm;
                     auxpm2 = aux.mbr_partition_4;
                     aux.mbr_partition_3 = auxpm2;
                     auxp.part_start = p4f+1;
                     mod = aux;
                     mod.mbr_partition_4 = auxp;
                     archivo=fopen(path,"rb+");
                     if(archivo){
                     printf("status de la particion:: %c\n",mod.mbr_partition_4.part_status);
                     fseek(archivo,0,SEEK_SET);
                     fwrite(&mod, sizeof(MBR),1,archivo);
                      fclose(archivo);
                     }
                     break;
                 default:
                     printf("no fue posible insertar la particion, espacio insuficiente en el disco\n");
                     break;
                 }

            }else if(p3i == -1){

                int e3 = -1;
                 partition auxpm, auxpm2;
                 if(auxp.part_size<(aux.mbr_tamano-p4f)){
                     e3 = 3;
                 }

                 if(auxp.part_size<(p4i-p2f)){
                     e3 = 2;
                 }

                 if(auxp.part_size<(p2i-p1f)){
                   e3 = 1;
               }
                 switch (e3) {
                 case 1:
                     auxpm = aux.mbr_partition_2;
                     aux.mbr_partition_3 = auxpm;
                     auxp.part_start = p1f+1;
                     mod = aux;
                     mod.mbr_partition_2 = auxp;
                     archivo=fopen(path,"rb+");
                     if(archivo){
                     printf("status de la particion:: %c\n",mod.mbr_partition_2.part_status);
                     fseek(archivo,0,SEEK_SET);
                     fwrite(&mod, sizeof(MBR),1,archivo);
                      fclose(archivo);
                     }
                     break;
                 case 2:


                     auxp.part_start = p2f+1;
                     archivo=fopen(path,"rb+");
                     if(archivo){
                     mod = aux;
                     mod.mbr_partition_3 = auxp;
                     printf("status de la particion:: %c\n",mod.mbr_partition_3.part_status);
                     fseek(archivo,0,SEEK_SET);
                     fwrite(&mod, sizeof(MBR),1,archivo);
                      fclose(archivo);
                     }

                     break;
                 case 3:
                     auxpm2 = aux.mbr_partition_4;
                     aux.mbr_partition_3 = auxpm2;
                     auxp.part_start = p4f+1;
                     mod = aux;
                     mod.mbr_partition_4 = auxp;
                     archivo=fopen(path,"rb+");
                     if(archivo){
                     printf("status de la particion:: %c\n",mod.mbr_partition_4.part_status);
                     fseek(archivo,0,SEEK_SET);
                     fwrite(&mod, sizeof(MBR),1,archivo);
                      fclose(archivo);
                     }
                     break;
                 default:
                     printf("no fue posible insertar la particion, espacio insuficiente en el disco\n");
                     break;
                 }

            }else if(p4i == -1){
                int e3 = -1;
                 partition auxpm, auxpm2;
                 if(auxp.part_size<(aux.mbr_tamano-p3f)){
                     e3 = 3;
                 }

                 if(auxp.part_size<(p3i-p2f)){
                     e3 = 2;
                 }

                 if(auxp.part_size<(p2i-p1f)){
                   e3 = 1;
               }
                 switch (e3) {
                 case 1:
                     auxpm = aux.mbr_partition_3;
                     aux.mbr_partition_4 = auxpm;
                     auxpm2 = aux.mbr_partition_2;
                     aux.mbr_partition_3 = auxpm2;
                     auxp.part_start = p1f+1;
                     mod = aux;
                     mod.mbr_partition_2 = auxp;
                     archivo=fopen(path,"rb+");
                     if(archivo){
                     printf("status de la particion:: %c\n",mod.mbr_partition_2.part_status);
                     fseek(archivo,0,SEEK_SET);
                     fwrite(&mod, sizeof(MBR),1,archivo);
                      fclose(archivo);
                     }
                     break;
                 case 2:

                     auxpm = aux.mbr_partition_3;
                     aux.mbr_partition_4 = auxpm;
                     auxp.part_start = p2f+1;
                     archivo=fopen(path,"rb+");
                     if(archivo){
                     mod = aux;
                     mod.mbr_partition_3 = auxp;
                     printf("status de la particion:: %c\n",mod.mbr_partition_3.part_status);
                     fseek(archivo,0,SEEK_SET);
                     fwrite(&mod, sizeof(MBR),1,archivo);
                      fclose(archivo);
                     }

                     break;
                 case 3:

                     auxp.part_start = p3f+1;
                     mod = aux;
                     mod.mbr_partition_4 = auxp;
                     archivo=fopen(path,"rb+");
                     if(archivo){
                     printf("status de la particion:: %c\n",mod.mbr_partition_4.part_status);
                     fseek(archivo,0,SEEK_SET);
                     fwrite(&mod, sizeof(MBR),1,archivo);
                      fclose(archivo);
                     }
                     break;

                     break;
                 default:
                     printf("no fue posible insertar la particion, espacio insuficiente en el disco\n");
                     break;
                 }
            }
        }
    }else{
            //aqui debe de ir el manejo de las extendidas
                printf("no es posible insertar mas particiones, slots primarios llenos\n");
            }
printf("hay %d particiones inactivas\n", contp);
    return 1;
}
//funcion para quitar una particion en el disco
void quitarPart(char *path, char *name, char * deldesic){

    MBR aux, mod;
    partition auxp;

   printf("si entro a quitar particion\n");
    FILE * archivo;

        archivo=fopen(path,"rb+");
        if(archivo){

                    fseek(archivo,0,SEEK_SET);
                    fread(&aux,sizeof(MBR),1,archivo);
               fclose(archivo);
        }

        //verificar que exista la particion
        if(strcasecmp(aux.mbr_partition_1.part_name,name)==0){
            auxp = aux.mbr_partition_1;
            aux.mbr_partition_1.part_fit = '\0';
            aux.mbr_partition_1.part_name[0] = '\0';
            aux.mbr_partition_1.part_start = -1;
            aux.mbr_partition_1.part_status = 'i';
            aux.mbr_partition_1.part_type = '\0';
            aux.mbr_partition_1.part_size = -1;

            mod = aux;

            if(strcasecmp(deldesic,"Fast")==0){
                archivo=fopen(path,"rb+");
                if(archivo){
                printf("status de la particion:: %c\n",mod.mbr_partition_1.part_status);
                fseek(archivo,0,SEEK_SET);
                fwrite(&mod, sizeof(MBR),1,archivo);
                 fclose(archivo);
                }
            }else if(strcasecmp(deldesic, "Full")==0){
                archivo=fopen(path,"rb+");
                if(archivo){
                    fseek(archivo,auxp.part_start-1,SEEK_SET);
                    char relleno = '\0';
                    long i;
                    for(i = 0; i<auxp.part_size;i++){
                        fwrite(&relleno,sizeof(char),1,archivo);
                    }
                    fclose(archivo);
                }
                archivo=fopen(path,"rb+");
                if(archivo){
                printf("status de la particion:: %c\n",mod.mbr_partition_1.part_status);
                fseek(archivo,0,SEEK_SET);
                fwrite(&mod, sizeof(MBR),1,archivo);
                 fclose(archivo);
                }

            }

        }else if(strcasecmp(aux.mbr_partition_2.part_name,name)==0){

            auxp = aux.mbr_partition_2;
            aux.mbr_partition_2.part_fit = '\0';
            aux.mbr_partition_2.part_name[0] = '\0';
            aux.mbr_partition_2.part_start = -1;
            aux.mbr_partition_2.part_status = 'i';
            aux.mbr_partition_2.part_type = '\0';
            aux.mbr_partition_2.part_size = -1;

            mod = aux;

            if(strcasecmp(deldesic,"Fast")==0){
                archivo=fopen(path,"rb+");
                if(archivo){
                printf("status de la particion:: %c\n",mod.mbr_partition_2.part_status);
                fseek(archivo,0,SEEK_SET);
                fwrite(&mod, sizeof(MBR),1,archivo);
                 fclose(archivo);
                }
            }else if(strcasecmp(deldesic, "Full")==0){
                archivo=fopen(path,"rb+");
                if(archivo){
                    fseek(archivo,auxp.part_start-1,SEEK_SET);
                    char relleno = '\0';
                    long i;
                    for(i = 0; i<auxp.part_size;i++){
                        fwrite(&relleno,sizeof(char),1,archivo);
                    }
                    fclose(archivo);
                }
                archivo=fopen(path,"rb+");
                if(archivo){
                printf("status de la particion:: %c\n",mod.mbr_partition_2.part_status);
                fseek(archivo,0,SEEK_SET);
                fwrite(&mod, sizeof(MBR),1,archivo);
                 fclose(archivo);
                }

            }

        }else if(strcasecmp(aux.mbr_partition_3.part_name,name)==0){

            auxp = aux.mbr_partition_3;
            aux.mbr_partition_3.part_fit = '\0';
            aux.mbr_partition_3.part_name[0] = '\0';
            aux.mbr_partition_3.part_start = -1;
            aux.mbr_partition_3.part_status = 'i';
            aux.mbr_partition_3.part_type = '\0';
            aux.mbr_partition_3.part_size = -1;

            mod = aux;

            if(strcasecmp(deldesic,"Fast")==0){
                archivo=fopen(path,"rb+");
                if(archivo){
                printf("status de la particion:: %c\n",mod.mbr_partition_3.part_status);
                fseek(archivo,0,SEEK_SET);
                fwrite(&mod, sizeof(MBR),1,archivo);
                 fclose(archivo);
                }
            }else if(strcasecmp(deldesic, "Full")==0){
                archivo=fopen(path,"rb+");
                if(archivo){
                    fseek(archivo,auxp.part_start-1,SEEK_SET);
                    char relleno = '\0';
                    long i;
                    for(i = 0; i<auxp.part_size;i++){
                        fwrite(&relleno,sizeof(char),1,archivo);
                    }
                    fclose(archivo);
                }
                archivo=fopen(path,"rb+");
                if(archivo){
                printf("status de la particion:: %c\n",mod.mbr_partition_3.part_status);
                fseek(archivo,0,SEEK_SET);
                fwrite(&mod, sizeof(MBR),1,archivo);
                 fclose(archivo);
                }
            }

        }else if(strcasecmp(aux.mbr_partition_4.part_name,name)==0){

            auxp = aux.mbr_partition_4;
            aux.mbr_partition_4.part_fit = '\0';
            aux.mbr_partition_4.part_name[0] = '\0';
            aux.mbr_partition_4.part_start = -1;
            aux.mbr_partition_4.part_status = 'i';
            aux.mbr_partition_4.part_type = '\0';
            aux.mbr_partition_4.part_size = -1;

            mod = aux;

            if(strcasecmp(deldesic,"Fast")==0){
                archivo=fopen(path,"rb+");
                if(archivo){
                printf("status de la particion:: %c\n",mod.mbr_partition_4.part_status);
                fseek(archivo,0,SEEK_SET);
                fwrite(&mod, sizeof(MBR),1,archivo);
                 fclose(archivo);
                }
            }else if(strcasecmp(deldesic, "Full")==0){
                archivo=fopen(path,"rb+");
                if(archivo){
                    fseek(archivo,auxp.part_start-1,SEEK_SET);
                    char relleno = '\0';
                    long i;
                    for(i = 0; i<auxp.part_size;i++){
                        fwrite(&relleno,sizeof(char),1,archivo);
                    }
                    fclose(archivo);
                }
                archivo=fopen(path,"rb+");
                if(archivo){
                printf("status de la particion:: %c\n",mod.mbr_partition_4.part_status);
                fseek(archivo,0,SEEK_SET);
                fwrite(&mod, sizeof(MBR),1,archivo);
                 fclose(archivo);
                }
            }

        }
}
void aumentarPart(char *path, char *name,int add){

    MBR aux, mod;
    partition auxp;

   printf("si entro a quitar particion\n");
    FILE * archivo;

        archivo=fopen(path,"rb+");
        if(archivo){

                    fseek(archivo,0,SEEK_SET);
                    fread(&aux,sizeof(MBR),1,archivo);
               fclose(archivo);
        }
        int p1i ,p1f, p2i, p2f, p3i, p3f, p4i, p4f; //si valen -1 quiere decir que se encuentra libre esa region


            if(aux.mbr_partition_1.part_status=='i'){
                p1i = -1;
                p1f = -1;
            }else{

                p1i = aux.mbr_partition_1.part_start;
                p1f = aux.mbr_partition_1.part_size+p1i;
            }
            if(aux.mbr_partition_2.part_status=='i'){
                p2i = -1;
                p2f = -1;
            }else{
                p2i = aux.mbr_partition_2.part_start;
                p2f = aux.mbr_partition_2.part_size+p2i;
            }
            if(aux.mbr_partition_3.part_status=='i'){
                p3i = -1;
                p3f = -1;
            }else{
                p3i = aux.mbr_partition_3.part_start;
                p3f = aux.mbr_partition_3.part_size+p3i;
            }
            if(aux.mbr_partition_4.part_status=='i'){
                p4i = -1;
                p4f = -1;
            }else{
                p4i = aux.mbr_partition_4.part_start;
                p4f = aux.mbr_partition_4.part_size+p4i;
            }

         if(strcasecmp(aux.mbr_partition_1.part_name,name)==0){

             auxp = aux.mbr_partition_1;
             if(add<0){
                 //quitar memoria a la particion
                 if((aux.mbr_tamano-136+(add))<0){
                     printf("no es posible que la memoria quede en valores negativos \n");
                        return;
                 }
                 auxp = aux.mbr_partition_1;
                 if((auxp.part_size+(add))<(2*mb)){
                     printf("el tamanio final de la particion no puede ser menor a 2MB \n");
                     return;
                 }
                 auxp.part_size= auxp.part_size+(add);
                 aux.mbr_partition_1 = auxp;
                 mod = aux;
                 archivo=fopen(path,"rb+");
                 if(archivo){
                 printf("status de la particion:: %c\n",mod.mbr_partition_1.part_status);
                 fseek(archivo,0,SEEK_SET);
                 fwrite(&mod, sizeof(MBR),1,archivo);
                  fclose(archivo);
                 }



             }else{
                 //agregar memoria a la particion
                 if((auxp.part_size+add)>(aux.mbr_tamano-136)){
                     printf("no es posible que la particion supere al tamanio del disco \n");
                        return;
                 }
                 if((p1f+add)<p2i){
                     auxp.part_size= auxp.part_size+(add);
                     aux.mbr_partition_1 = auxp;
                     mod = aux;
                     archivo=fopen(path,"rb+");
                     if(archivo){
                     printf("status de la particion:: %c\n",mod.mbr_partition_1.part_status);
                     fseek(archivo,0,SEEK_SET);
                     fwrite(&mod, sizeof(MBR),1,archivo);
                      fclose(archivo);
                     }
                 }
             }

         }else if(strcasecmp(aux.mbr_partition_2.part_name,name)==0){

             auxp = aux.mbr_partition_2;
             if(add<0){
                 //quitar memoria a la particion
                 if((aux.mbr_tamano-136+(add))<0){
                     printf("no es posible que la memoria quede en valores negativos \n");
                        return;
                 }
                 auxp = aux.mbr_partition_2;
                 if((auxp.part_size+(add))<(2*mb)){
                     printf("el tamanio final de la particion no puede ser menor a 2MB \n");
                     return;
                 }
                 auxp.part_size= auxp.part_size+(add);
                 aux.mbr_partition_2 = auxp;
                 mod = aux;
                 archivo=fopen(path,"rb+");
                 if(archivo){
                 printf("status de la particion:: %c\n",mod.mbr_partition_2.part_status);
                 fseek(archivo,0,SEEK_SET);
                 fwrite(&mod, sizeof(MBR),1,archivo);
                  fclose(archivo);
                 }



             }else{
                 //agregar memoria a la particion
                 if((auxp.part_size+add)>(aux.mbr_tamano-136)){
                     printf("no es posible que la particion supere al tamanio del disco \n");
                        return;
                 }
                 if((p2f+add)<p3i){
                     auxp.part_size= auxp.part_size+(add);
                     aux.mbr_partition_2 = auxp;
                     mod = aux;
                     archivo=fopen(path,"rb+");
                     if(archivo){
                     printf("status de la particion:: %c\n",mod.mbr_partition_2.part_status);
                     fseek(archivo,0,SEEK_SET);
                     fwrite(&mod, sizeof(MBR),1,archivo);
                      fclose(archivo);
                     }
                 }
             }

         }else if(strcasecmp(aux.mbr_partition_3.part_name,name)==0){

             auxp = aux.mbr_partition_3;
             if(add<0){
                 //quitar memoria a la particion
                 if((aux.mbr_tamano-136+(add))<0){
                     printf("no es posible que la memoria quede en valores negativos \n");
                        return;
                 }
                 auxp = aux.mbr_partition_3;
                 if((auxp.part_size+(add))<(2*mb)){
                     printf("el tamanio final de la particion no puede ser menor a 2MB \n");
                     return;
                 }
                 auxp.part_size= auxp.part_size+(add);
                 aux.mbr_partition_3 = auxp;
                 mod = aux;
                 archivo=fopen(path,"rb+");
                 if(archivo){
                 printf("status de la particion:: %c\n",mod.mbr_partition_3.part_status);
                 fseek(archivo,0,SEEK_SET);
                 fwrite(&mod, sizeof(MBR),1,archivo);
                  fclose(archivo);
                 }



             }else{
                 //agregar memoria a la particion
                 if((auxp.part_size+add)>(aux.mbr_tamano-136)){
                     printf("no es posible que la particion supere al tamanio del disco \n");
                        return;
                 }
                 if((p3f+add)<p4i){
                     auxp.part_size= auxp.part_size+(add);
                     aux.mbr_partition_2 = auxp;
                     mod = aux;
                     archivo=fopen(path,"rb+");
                     if(archivo){
                     printf("status de la particion:: %c\n",mod.mbr_partition_2.part_status);
                     fseek(archivo,0,SEEK_SET);
                     fwrite(&mod, sizeof(MBR),1,archivo);
                      fclose(archivo);
                     }
                 }
             }

         }else if(strcasecmp(aux.mbr_partition_4.part_name,name)==0){

             auxp = aux.mbr_partition_4;
             if(add<0){
                 //quitar memoria a la particion
                 if((aux.mbr_tamano-136+(add))<0){
                     printf("no es posible que la memoria quede en valores negativos \n");
                        return;
                 }
                 auxp = aux.mbr_partition_4;
                 if((auxp.part_size+(add))<(2*mb)){
                     printf("el tamanio final de la particion no puede ser menor a 2MB \n");
                     return;
                 }
                 auxp.part_size= auxp.part_size+(add);
                 aux.mbr_partition_4 = auxp;
                 mod = aux;
                 archivo=fopen(path,"rb+");
                 if(archivo){
                 printf("status de la particion:: %c\n",mod.mbr_partition_4.part_status);
                 fseek(archivo,0,SEEK_SET);
                 fwrite(&mod, sizeof(MBR),1,archivo);
                  fclose(archivo);
                 }



             }else{
                 //agregar memoria a la particion
                 if((auxp.part_size+add)>(aux.mbr_tamano-136)){
                     printf("no es posible que la particion supere al tamanio del disco \n");
                        return;
                 }
                 if((p4f+add)<aux.mbr_tamano){
                     auxp.part_size= auxp.part_size+(add);
                     aux.mbr_partition_4 = auxp;
                     mod = aux;
                     archivo=fopen(path,"rb+");
                     if(archivo){
                     printf("status de la particion:: %c\n",mod.mbr_partition_4.part_status);
                     fseek(archivo,0,SEEK_SET);
                     fwrite(&mod, sizeof(MBR),1,archivo);
                      fclose(archivo);
                     }
                 }
             }

         }

}

/*FUNCIONES PARA MONTAJE Y DESMONTAJE DE PARTICIONES*/
 /*funcion mount*/

void mount(Lista * lista, char *cadena){
    int tam;
    char path[100];
    char name[16];
    int estado = -1;
    int contador = 0;
    path[0] = '\0';
    name[0] = '\0';
char * spliteo, *split2;
char aux1[100], aux2[100], buffer[100], aux3[16],aux4[16], buffer2[16];
int bandera = 0;
int bandera2= 0;
    spliteo = strtok(cadena, " ");

    while(spliteo!= NULL){

        if(strcasecmp("mount", spliteo) ==0){
                   estado = 0;
        }else if(strcasecmp("-path::", spliteo)==0){
            estado = 3;
        }else if(strcasecmp("-name::", spliteo)==0){
            estado = 7;
        }

        switch (estado) {
        case 0:
           //no hace nada desecha;
            break;
        case 3:
            estado = 11;
            break;
        case 7:
            estado = 15;
            break;
        case 11:

            for(tam =0; spliteo[tam]!='\0';tam++){

                        }
                        if(spliteo[tam-1]=='"'){
                            //el paramtro no posee espacios

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
        case 15:
            for(tam =0; spliteo[tam]!='\0';tam++){

            }
            if(tam<16){

                for(tam =0; spliteo[tam]!='\0';tam++){

            }
            if(spliteo[tam-1]=='"'){
                //el paramtro no posee espacios

                strcpy(name,spliteo);
                estado = 0;
                bandera2 = 1;
            }else{
                strcpy(name,spliteo);
                estado = 18;
            }
            }else{
                printf("el nombre de la particion no de debe ser mayor a 15 caracteres\n");
                estado = 0;
            }

            break;
        case 17:

            strcpy(aux2, spliteo);
              snprintf(buffer, sizeof(buffer), "%s %s", path, aux2);
              strcpy(path, buffer);

              for(tam =0; spliteo[tam]!='\0';tam++){

              }
              if(spliteo[tam-1]=='\"'){
                                    //el paramtro no posee espacios

                                    if(spliteo[tam-5]=='.'&& spliteo[tam-4]=='d' && spliteo[tam-3]=='s'&& spliteo[tam-2]=='k'){

                                     bandera = 1;
                                     estado = 0;
                                    }
                                    }else{
                                        estado =17;
                                    }
            break;
        case 18:
            for(tam =0; spliteo[tam]!='\0';tam++){

            }
            tam++;
            for(tam = tam; name[tam]!='\0';tam++){

            }
            if(tam<16){
            strcpy(aux3, spliteo);
              snprintf(buffer2, sizeof(buffer2), "%s %s", name, aux3);
              strcpy(name, buffer2);

for(tam =0; spliteo[tam]!='\0';tam++){

            }
              if(spliteo[tam-1]=='\"'){
                                    //el paramtro no posee espacios

                                    bandera2 = 1;
                                    estado = 0;

              }else{
                  estado = 18;
              }
            }else{
                printf("el nombre de la particion no de debe ser mayor a 15 caracteres\n");
                estado = 0;
            }
            break;
        default:

            break;
        }
        contador ++;
        spliteo = strtok(NULL, " \n");
    }


    if(contador == 1 && estado == 0){
        mostrarmount(lista);
        return;
    }

    if(path[0]!='\0' && bandera == 1){
        strcpy(aux1 , path);
        split2 = strtok(aux1, "\"");
        strcpy(path,split2);


        if(name[0]!='\0' && bandera2 == 1){
            strcpy(aux4, name);
            split2 = strtok(aux4, "\"");
            strcpy(name, split2);


            montarPart(lista, path, name);

        }else{
            printf("el nombre no se especifico de manera correcta\n");
        }


    }else{
        printf("no se especifico el path del disco\n");
    }


}

/*INICIALIZAR LISTA*/
void inicializar(Lista * lista){
    lista->inicio = NULL;
    lista->fin = NULL;

}

void montarPart(Lista * lista, char* path, char *name){
    MBR aux;
    partition auxp;

    montado *nuevo, *auxm;

    //obtener la particion del mbr, de no existir el disco o el mbr se retorna error
    FILE * archivo;

        archivo=fopen(path,"rb+");
        if(archivo){

                    fseek(archivo,0,SEEK_SET);
                    fread(&aux,sizeof(MBR),1,archivo);
               fclose(archivo);
        }else{
            printf("el disco no existe\n");
            return;
        }

        if(strcasecmp(aux.mbr_partition_1.part_name,name)==0){
            //si existe
            auxp = aux.mbr_partition_1;
        }else if(strcasecmp(aux.mbr_partition_2.part_name,name)==0){
            auxp = aux.mbr_partition_2;
        }else if(strcasecmp(aux.mbr_partition_3.part_name,name)==0){
            auxp = aux.mbr_partition_3;
        }else if(strcasecmp(aux.mbr_partition_4.part_name,name)==0){
            auxp = aux.mbr_partition_4;
        }else{
            printf("la particion no existe\n");
            return;
        }


        nuevo = (montado*)malloc(sizeof(montado));
    if(lista->inicio==NULL){
        nuevo->id = 1;
        nuevo->iddisco = 'a';
        nuevo->info = auxp;
        nuevo->path = (char*)malloc(mx * sizeof(char));
        strcpy(nuevo->path,path);
        nuevo->siguiente = NULL;
        nuevo->anterior = NULL;
        lista->inicio = nuevo;
        lista->fin = nuevo;

    }else{
        //insercion al final
        //recorrer la lista sin imprimir
        int sduplicate = buscarpart(lista, path, name);
        if(sduplicate!=-1){


                nuevo->id = sduplicate;
                nuevo->iddisco = seekpart(lista, path);
                nuevo->info = auxp;
                nuevo->path = (char*)malloc(mx * sizeof(char));
                strcpy(nuevo->path,path);
                nuevo->siguiente = NULL;
                nuevo->anterior = NULL;

                auxm = lista->fin;
                auxm->siguiente = nuevo;
                nuevo->anterior = auxm;
                lista->fin = nuevo;
            }else{

            printf("la particion ya se encuentra montada\n");

        }
    }

}
char seekpart(Lista * lista, char* path){
    montado *aux;
    aux = lista->inicio;
    char contadorp = 'a';

    while(aux!=NULL){
        if(strcasecmp(aux->path, path)==0){

            return contadorp;
        }
        aux = aux->siguiente;
        contadorp++;
    }
    return contadorp;
}

int buscarpart(Lista * lista, char* path, char *nombre){
    montado *aux;
    aux = lista->inicio;
    int contadorp = 1;

    while(aux!=NULL){
        if(strcasecmp(aux->path, path)==0){
            if(strcasecmp(aux->info.part_name,nombre)==0){
                //la particion ya esta montada
                return -1;
            }
            aux = aux->siguiente;
            contadorp++;
        }
    }
    return contadorp;
}

void mostrarmount(Lista * lista){
    montado *aux;
    aux = lista->inicio;

   if(lista->inicio == NULL){
       printf("no existen particiones montadas\n") ;
       return;
   }
   printf("PARTICIONES MONTADAS ACTUALMENTE\n");
    while(aux!=NULL){
        printf("id::vd%c%d -path::\"%s\" -name:: \"%s\"\n",aux->iddisco, aux->id, aux->path, aux->info.part_name);
aux = aux->siguiente;
    }

}

void umount(Lista * lista, char * cadena){
    int tam;

    char name[16];
    int estado = -1;
    int contador = 0;

    name[0] = '\0';
char * spliteo;
    spliteo = strtok(cadena, " ");

    while(spliteo!= NULL){
        for(tam =0; spliteo[tam]!='\0';tam++){

                    }

        if(strcasecmp("umount", spliteo) ==0){
                   estado = 0;
        }else if(strcasecmp("-id1::", spliteo)==0){
            estado = 1;
        }else if(contador >3 && tam > 5 && spliteo[0]=='-' && (spliteo[1]=='i' || spliteo[1]=='I') && (spliteo[2]=='d' || spliteo[2]=='D') && spliteo[tam-1]==':' && spliteo[tam-2]==':'){
            estado = 3;
        }

        switch (estado) {
        case 0:
           //no hace nada desecha;
            break;
        case 1:
            estado = 2;
            break;
        case 2:
            strcpy(name,spliteo);
            estado = 0;
            break;
        case 3:
            estado = 4;
            break;
        case 4:
            //desmonta varios
            strcpy(name,spliteo);
            desmontador(lista, name);
            //umount(lista, name);
            break;
        default:

            break;
        }
        contador ++;
        spliteo = strtok(NULL, " \n");
    }


    if(contador == 3 && estado == 0){
        //desmonta solo 1
        desmontador(lista, name);
        return;
    }

}

void desmontador(Lista * lista, char * identificador){

    if(lista->inicio==NULL){
        printf("la lista de particiones montadas esta vacia\n");
        return;
    }

    montado *elim , *aux, *aux2;
    int tam, idpart;
    char splitter1[5];
    char path[mx];
    for(tam =0; identificador[tam]!='\0';tam++){

                }

    char iddisk = identificador[2];


    for(tam =2; identificador[tam]!='\0';tam++){
        splitter1[tam] = identificador[tam];
    }
    idpart = atoi(splitter1);

    aux = lista->inicio;
    if(lista->inicio == lista->fin){
        //solo tiene un elemento

        if(aux->id == idpart && aux->iddisco == iddisk){
            elim = aux;
            lista->inicio = NULL;
            lista->fin = NULL;

            free(elim->path);
            free(elim);
        }else{
            //no es el unico y lo debe ubicar
            while(aux!=NULL){
                if(aux->id == idpart && aux->iddisco == iddisk){
                    break;
                }
                aux = aux->siguiente;
            }
            if(aux!=NULL){
                //lo encontro
                elim = aux;
                strcpy(path,elim->path);
                if(aux == lista->inicio){
                    aux = aux->siguiente;
                    aux->anterior = NULL;
                    lista->inicio = aux;

                    free(elim->path);
                    free(elim);
                    //se llama aqui al metodo que reajusta los valores de las etiquetas
                    actetiqueta(lista, iddisk, idpart);

                }else if(aux == lista->fin){
                    aux = aux->anterior;
                    aux->siguiente = NULL;
                    lista->fin = aux;

                    free(elim->path);
                    free(elim);
                    //
                    actetiqueta(lista, iddisk, idpart);
                }else{
                    //esta entre el primero y el ultimo
                    aux2 = aux->siguiente;
                    aux = aux->anterior;
                    aux->siguiente = aux2;
                    aux2->anterior= aux;

                    free(elim->path);
                    free(elim);
                    //
                    actetiqueta(lista, iddisk, idpart);
                }
            }else{
                printf("la particion que se desea desmontar no se encuentra montada actualmente\n");
            }

        }
    }

}
void actetiqueta(Lista * lista, char iddisk, int id){
    montado *aux;
    aux = lista->inicio;

    while(aux!=NULL){
        if(aux->iddisco == iddisk){
            if(id<aux->id){
                aux->id--;
            }
        }
        aux = aux->siguiente;
    }
}
void exec(Lista *lista , char *cadena){
    int bandera =0;
    char path[100];
    char aux1[100];
    char buffer[100];

    path[0] = '\0';
    int estado = 0;
    int tam;
    char * spliteo, *split2;

        spliteo = strtok(cadena, " ");
        while (spliteo != NULL){
           if(strcasecmp("exec", spliteo)==0){
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
               if(spliteo[tam-4]=='.' && spliteo[tam-3]=='s' && spliteo[tam-2]=='h'){
                   //el paramtro no posee espacios

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
                  if(spliteo[tam-3]=='.' && spliteo[tam-2]=='s' && spliteo[tam-1]=='h'){
                 //el paramtro no posee espacios

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


        //verificar que si haya introducido un path
       if(path[0]!='\0' && bandera == 1){
           strcpy(aux1 , path);
           split2 = strtok(aux1, "\"");
           strcpy(path,split2);


           ejecutar(lista, path);

       }else{
           printf("no se especifico el path del script a ejecutar\n");
       }



}


void ejecutar(Lista * lista, char * path){

    char buffer[1000], entrada[1000];
    int bcoment = 0;
    int tam;
    int estado = -1;
    char *split;
    FILE *file;
    char aux1[100];
   char linea[1000];

        file = fopen(path, "r");
        if (file == NULL) {
            printf("Error al abrir el script\n");
        }
        else {
            int contador = 0;
            while (!feof(file)) {
                fgets(linea, 1000, file);
                printf("linea %d: %s",contador, linea);
                for(tam =0; linea[tam]!='\0';tam++){

                }
                if(linea[tam-1]=='/' && linea[0]!='#'){
                    //se tiene que almacenar en el buffer
                    estado = 2;
                }else if(linea[0]=='\n'){
                    estado = 0;
                }else {
                    if(estado == 3 && linea[0]!='#'){
                        //ya termino en esta linea el comando
                        estado = 3;
                    }else if(estado == 3 && linea[0]=='#'){

                        estado = 3;
                        bcoment = 1;
                    }else{
                        //comando de una sola linea
                        estado =1;
                    }

                }

                switch (estado) {
                case 0:
                    //es un espacio no hace nada
                    break;
                case 1:
                    analizar(linea, 1000,lista,0);
                    estado = 0;
                    break;
                case 2:
                    split = strtok(linea, "/");
                    strcpy(entrada, split);
                     estado = 3;
                    break;
                case 3:
                    if(bcoment == 1){
                       analizar(linea, 1000,lista,0);
                       estado = 3;
                       bcoment = 0;
                    }else{
                        strcpy(aux1, linea);
                         snprintf(buffer, sizeof(buffer), "%s %s", buffer, aux1);
                         strcpy(entrada, buffer);
                         analizar(entrada, 1000, lista,0);
                         estado = 0;
                    }

                    break;
                default:
                    break;
                }
                contador ++;
            }
            fclose(file);
        }
}
/*********REPORTES**********/
void rep(Lista *lista , char * cadena){

    int tam;
    char path[100];
    char name[16];
    int estado = -1;

    path[0] = '\0';
    name[0] = '\0';
    char iddisk[10];
    iddisk[0] = '\0';
char * spliteo, *split2;
char aux1[100], aux2[100], buffer[100];
int bandera = 0;

    spliteo = strtok(cadena, " ");

    while(spliteo!= NULL){

        if(strcasecmp("rep", spliteo) ==0){
                   estado = 0;
        }else if(strcasecmp("-path::", spliteo)==0){
            estado = 3;
        }else if(strcasecmp("-name::", spliteo)==0){
            estado = 7;
        }else if(strcasecmp("-id::", spliteo)==0){
            estado = 1;
        }

        switch (estado) {
        case 0:
           //no hace nada desecha;
            break;
        case 1:
            estado = 2;
            break;
        case 2:
            strcpy(iddisk,spliteo);
            estado = 0;
            break;
        case 3:
            estado = 11;
            break;
        case 7:
            estado = 15;
            break;
        case 11:

            for(tam =0; spliteo[tam]!='\0';tam++){

                        }
                        if(spliteo[tam-1]=='"'){
                            //el paramtro no posee espacios

                            if(spliteo[tam-5]=='.'){

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
        case 15:

            strcpy(name, spliteo);
            estado = 0;
            break;
        case 17:

            strcpy(aux2, spliteo);
              snprintf(buffer, sizeof(buffer), "%s %s", path, aux2);
              strcpy(path, buffer);

              for(tam =0; spliteo[tam]!='\0';tam++){

              }
              if(spliteo[tam-1]=='\"'){
                                    //el paramtro no posee espacios

                                    if(spliteo[tam-5]=='.'){

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


    if(path[0]!='\0' && bandera == 1){
        strcpy(aux1 , path);
        split2 = strtok(aux1, "\"");
        strcpy(path,split2);

        //

        if(iddisk[0] !='\0'){

            strcpy(aux1 , name);
            split2 = strtok(aux1, "\"");
            strcpy(name,split2);
            if(strcasecmp(name,"mbr")==0){
                repmbr(lista, iddisk, path);

            }else if(strcasecmp(name,"disk")==0){
                  repdisk(lista, iddisk, path);
            }else{
                printf("nombre de reporte invalido\n");
            }

        }else{
            printf("el id de la particion no se especifico de manera correcta\n");
        }


    }else{
        printf("no se especifico el path del archivo\n");
    }


}
/****REP*MBR**/
void repmbr(Lista *lista, char * identificador, char * pathrep){
    if(lista->inicio==NULL){
        printf("no es posible crear el reporte, la lista de particiones montadas esta vacia\n");
        return;
    }
    MBR aux;

    montado *auxpart;
    int tam, idpart;
    char splitter1[5];


    for(tam =0; identificador[tam]!='\0';tam++){

      }

    char iddisk = identificador[2];


int i = 0;
    for(tam =3; identificador[tam]!='\0';tam++){
        splitter1[i] = identificador[tam];
        i++;
    }

    idpart = atoi(splitter1);

    auxpart = lista->inicio;
    while(auxpart!=NULL){
        if(auxpart->iddisco == iddisk && auxpart->id == idpart){
            break;
        }
        auxpart= auxpart->siguiente;
    }

    if(auxpart!=NULL){

        FILE * archivo;

            archivo=fopen(auxpart->path,"rb+");
            if(archivo){
                        fseek(archivo,0,SEEK_SET);
                        fread(&aux,sizeof(MBR),1,archivo);
                   fclose(archivo);
            }

            FILE* file;



                 file =fopen("mbr.dot", "w");

                 fprintf(file,"digraph {\nnode [shape = plaintext];\n");
                      // fprintf(file,"\tedge [color=\"blue\", dir=forward]\n");
                         // fprintf(file,"\trankdir=TB\n");
                          fprintf(file,"subgraph cluster0 {\n");
                          fprintf(file, "label = \"MBR %s\";\n", auxpart->path);
                         fprintf(file, "struct1 [label = <<table>");

                         fprintf(file,"<tr WIDTH=\"100\" HEIGHT=\"50\"><td><b>Nombre</b></td><td><b>Valor</b></td></tr>");
                         fprintf(file,"<tr WIDTH=\"100\" HEIGHT=\"50\"><td><b>mbr_tamano</b></td><td>%d</td></tr>",aux.mbr_tamano);
                         fprintf(file,"<tr WIDTH=\"100\" HEIGHT=\"50\"><td><b>mbr_fecha_creacion</b></td><td>%s</td></tr>",aux.mbr_fecha_creacion);
                          fprintf(file,"<tr WIDTH=\"100\" HEIGHT=\"50\"><td><b>mbr_disk_signature</b></td><td>%d</td></tr>",aux.mbr_disk_signature);
                         if(aux.mbr_partition_1.part_status == 'a'){

                          fprintf(file,"<tr WIDTH=\"100\" HEIGHT=\"50\"><td><b>part_status_1</b></td><td>%c</td></tr>",aux.mbr_partition_1.part_status);
                          fprintf(file,"<tr WIDTH=\"100\" HEIGHT=\"50\"><td><b>part_type_1</b></td><td>%c</td></tr>",aux.mbr_partition_1.part_type);
                          fprintf(file,"<tr WIDTH=\"100\" HEIGHT=\"50\"><td><b>part_fit_1</b></td><td>%c</td></tr>",aux.mbr_partition_1.part_fit);
                          fprintf(file,"<tr WIDTH=\"100\" HEIGHT=\"50\"><td><b>part_start_1</b></td><td>%d</td></tr>",aux.mbr_partition_1.part_start);
                          fprintf(file,"<tr WIDTH=\"100\" HEIGHT=\"50\"><td><b>part_size_1</b></td><td>%d</td></tr>",aux.mbr_partition_1.part_size);
                          fprintf(file,"<tr WIDTH=\"100\" HEIGHT=\"50\"><td><b>part_name_1</b></td><td>%s</td></tr>",aux.mbr_partition_1.part_name);

                         }

                         if(aux.mbr_partition_2.part_status == 'a'){

                             fprintf(file,"<tr WIDTH=\"100\" HEIGHT=\"50\"><td><b>part_status_2</b></td><td>%c</td></tr>",aux.mbr_partition_2.part_status);
                             fprintf(file,"<tr WIDTH=\"100\" HEIGHT=\"50\"><td><b>part_type_2</b></td><td>%c</td></tr>",aux.mbr_partition_2.part_type);
                             fprintf(file,"<tr WIDTH=\"100\" HEIGHT=\"50\"><td><b>part_fit_2</b></td><td>%c</td></tr>",aux.mbr_partition_2.part_fit);
                             fprintf(file,"<tr WIDTH=\"100\" HEIGHT=\"50\"><td><b>part_start_2</b></td><td>%d</td></tr>",aux.mbr_partition_2.part_start);
                             fprintf(file,"<tr WIDTH=\"100\" HEIGHT=\"50\"><td><b>part_size_2</b></td><td>%d</td></tr>",aux.mbr_partition_2.part_size);
                             fprintf(file,"<tr WIDTH=\"100\" HEIGHT=\"50\"><td><b>part_name_2</b></td><td>%s</td></tr>",aux.mbr_partition_2.part_name);

                         }

                         if(aux.mbr_partition_3.part_status == 'a'){

                             fprintf(file,"<tr WIDTH=\"100\" HEIGHT=\"50\"><td><b>part_status_</b></td><td>%c</td></tr>",aux.mbr_partition_3.part_status);
                             fprintf(file,"<tr WIDTH=\"100\" HEIGHT=\"50\"><td><b>part_type_3</b></td><td>%c</td></tr>",aux.mbr_partition_3.part_type);
                             fprintf(file,"<tr WIDTH=\"100\" HEIGHT=\"50\"><td><b>part_fit_3</b></td><td>%c</td></tr>",aux.mbr_partition_3.part_fit);
                             fprintf(file,"<tr WIDTH=\"100\" HEIGHT=\"50\"><td><b>part_start_3</b></td><td>%d</td></tr>",aux.mbr_partition_3.part_start);
                             fprintf(file,"<tr WIDTH=\"100\" HEIGHT=\"50\"><td><b>part_size_3</b></td><td>%d</td></tr>",aux.mbr_partition_3.part_size);
                             fprintf(file,"<tr WIDTH=\"100\" HEIGHT=\"50\"><td><b>part_name_3</b></td><td>%s</td></tr>",aux.mbr_partition_3.part_name);

                         }

                         if(aux.mbr_partition_4.part_status == 'a'){

                             fprintf(file,"<tr WIDTH=\"100\" HEIGHT=\"50\"><td><b>part_status_4</b></td><td>%c</td></tr>",aux.mbr_partition_4.part_status);
                             fprintf(file,"<tr WIDTH=\"100\" HEIGHT=\"50\"><td><b>part_type_4</b></td><td>%c</td></tr>",aux.mbr_partition_4.part_type);
                             fprintf(file,"<tr WIDTH=\"100\" HEIGHT=\"50\"><td><b>part_fit_4</b></td><td>%c</td></tr>",aux.mbr_partition_4.part_fit);
                             fprintf(file,"<tr WIDTH=\"100\" HEIGHT=\"50\"><td><b>part_start_4</b></td><td>%d</td></tr>",aux.mbr_partition_4.part_start);
                             fprintf(file,"<tr WIDTH=\"100\" HEIGHT=\"50\"><td><b>part_size_4</b></td><td>%d</td></tr>",aux.mbr_partition_4.part_size);
                             fprintf(file,"<tr WIDTH=\"100\" HEIGHT=\"50\"><td><b>part_name_4</b></td><td>%s</td></tr>",aux.mbr_partition_4.part_name);

                         }

                          fprintf(file,"</table>>];\n}");
                          fprintf(file , "\n}");


                fclose(file);
                char * pbuffer2;
                char pbuffer[100], buffer2[100], buffer3[200];
                pbuffer2 = strtok(pathrep,".");
                strcpy(pbuffer,pbuffer2);
                snprintf(buffer3,sizeof(buffer3), "dot -Tjpg mbr.dot -o %s.jpg -Gcharset=latin1", pbuffer);
                snprintf(buffer2, sizeof(buffer2), "xdg-open %s.jpg",pbuffer);
                system(buffer3);
                system(buffer2);
                //system("dot -Tpng sistema.dot -o sistema.png -Gcharset=latin1");
              //  system("xdg-open /home/mario/EDD/Practicav12016/sistema.png");

    }else{
        printf("la particion de la cual se desea reportar MBR no esta montada\n");
    }


}
void repdisk(Lista *lista, char * identificador, char * pathrep){

    if(lista->inicio==NULL){
        printf("no es posible crear el reporte, la lista de particiones montadas esta vacia\n");
        return;
    }
    MBR aux;

    montado *auxpart;
    int tam, idpart;
    char splitter1[5];


    for(tam =0; identificador[tam]!='\0';tam++){

      }

    char iddisk = identificador[2];


int i = 0;
    for(tam =3; identificador[tam]!='\0';tam++){
        splitter1[i] = identificador[tam];
        i++;
    }

    idpart = atoi(splitter1);

    auxpart = lista->inicio;
    while(auxpart!=NULL){
        if(auxpart->iddisco == iddisk && auxpart->id == idpart){
            break;
        }
        auxpart= auxpart->siguiente;
    }

    if(auxpart!=NULL){

        FILE * archivo;

            archivo=fopen(auxpart->path,"rb+");
            if(archivo){
                        fseek(archivo,0,SEEK_SET);
                        fread(&aux,sizeof(MBR),1,archivo);
                   fclose(archivo);
            }

            FILE* file;

                 // imprimir cabeceras de archivo

                 file =fopen("disk.dot", "w");
                 fprintf(file,"digraph structs {\n");
                 fprintf(file,"subgraph cluster0 {\n");
                 fprintf(file, "label = \"Disk %s\";\n", auxpart->path);

                 fprintf(file," struct3 [shape=record,label=\" MBR |");
                if(aux.mbr_partition_1.part_status == 'a'){
                    if(aux.mbr_partition_1.part_type== 'P'){
                        fprintf(file,"Primaria:: %s | ",aux.mbr_partition_1.part_name);
                    }else{
                        fprintf(file,"Extendida:: %s | ",aux.mbr_partition_1.part_name);
                    }

                }else{
                    fprintf(file," LIBRE |");
                }
                if(aux.mbr_partition_2.part_status == 'a'){
                    if(aux.mbr_partition_2.part_type== 'P'){
                        fprintf(file,"Primaria:: %s | ",aux.mbr_partition_2.part_name);
                    }else{
                        fprintf(file,"Extendida:: %s | ",aux.mbr_partition_2.part_name);
                    }

                }else{
                    fprintf(file," LIBRE |");
                }
                if(aux.mbr_partition_3.part_status == 'a'){
                    if(aux.mbr_partition_3.part_type== 'P'){
                        fprintf(file,"Primaria:: %s | ",aux.mbr_partition_3.part_name);
                    }else{
                        fprintf(file,"Extendida:: %s | ",aux.mbr_partition_3.part_name);
                    }

                }else{
                    fprintf(file," LIBRE |");
                }
                if(aux.mbr_partition_4.part_status == 'a'){
                    if(aux.mbr_partition_4.part_type== 'P'){
                        fprintf(file,"Primaria:: %s\" ]",aux.mbr_partition_4.part_name);
                    }else{
                        fprintf(file,"Extendida:: %s\" ]",aux.mbr_partition_4.part_name);
                    }

                }else{
                    fprintf(file," LIBRE\" ]");
                }


                          fprintf(file , "\n}");

                          fprintf(file , "\n}");


                fclose(file);
                char * pbuffer2;
                char pbuffer[100], buffer2[100], buffer3[200];
                pbuffer2 = strtok(pathrep,".");
                strcpy(pbuffer,pbuffer2);
                snprintf(buffer3,sizeof(buffer3), "dot -Tjpg disk.dot -o %s.jpg -Gcharset=latin1", pbuffer);
                snprintf(buffer2, sizeof(buffer2), "xdg-open %s.jpg",pbuffer);
                system(buffer3);
                system(buffer2);
                //system("dot -Tpng sistema.dot -o sistema.png -Gcharset=latin1");
              //  system("xdg-open /home/mario/EDD/Practicav12016/sistema.png");

    }else{
        printf("la particion de la cual se desea reportar Disk no esta montada\n");
    }
}

/****************************************/
Lista *listapart;
int main(void)
{
  listapart = (Lista*)malloc(sizeof(Lista));
inicializar(listapart);
    printf(":::::::::::::Bienvenido::::::::\n");
    printf("para salir presione dos veces enter\n");
 char cadena[1000];

int b =1;
while(b==1){
  analizar(cadena , 1000, listapart, 1);
}


    return 0;
}

