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



/*****lista de funciones y procedimientos a utilizar*/
int crearDisco(char *cadena);
void CreateFolder(char * path);
int analizar(char * cad, int n);
void insert_mbr(char *fname, int size);
int elimDisco(char * cadena);
int fDisk(char * cadena);
int crearParticion(char * path, char * name, int size, char tipo, char *fit);
void reordenarPart(char *path);
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
                printf("comilla\n");
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
                                    printf("comilla\n");
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


              if(spliteo[tam-1]=='\"'){
                                    //el paramtro no posee espacios
                                    printf("comilla\n");
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

    printf("%s\n",path);
    if(path[0]!='\0' && bandera == 1){
        strcpy(aux1 , path);
        split2 = strtok(aux1, "\"");
        strcpy(path,split2);
        printf("entro\n");
        //
        if(name[0]!='\0' && bandera2 == 1){
            strcpy(aux4, name);
            split2 = strtok(aux4, "\"");
            strcpy(name, split2);
            printf("entro\n");

            if(size>0){

                if(unit!='\0'){
                    if(unit=='b' && size>= (mb*2)){
                        crearParticion(path, name, size,type,fit);
                    }else if(unit == 'k' && size>= 2048){
                        crearParticion(path, name, (size*kb),type,fit);
                    }else if(unit == 'm' && size>=2){
                        crearParticion(path, name, (size*mb),type,fit);
                    }
                }else if(unit == '\0' && size>=2048){
                    crearParticion(path, name, (size*kb),type,fit);
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
int pos;
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
    //nmbr.contp=0;
   // nmbr.eflag = 0;
    nmbr.mbr_tamano = size;
    FILE *fichero = fopen(fname , "rb+");

     if(fichero){
              fseek(fichero,0,SEEK_SET);
              pos  = ftell(fichero);
              printf("pos puntero 1: %d\n",pos);
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
}
if(fit[0]!='\0'){
    auxp.part_fit = fit[0];
}


printf("si entro a crear particion\n");
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
             int pos  = ftell(archivo);
             int tamp = sizeof(MBR);
             printf("la fecha del mbr es: %s y la part 1 status %c pos puntero: %d\n", cp2.mbr_fecha_creacion, cp2.mbr_partition_1.part_status, pos);
             printf("tamanio de struct mbr: %d", tamp);
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

    /*
                mod = aux;
                mod.mbr_partition_1 = auxp;
                printf("status de la particion:: %c\n",mod.mbr_partition_1.part_status);
                fseek(archivo,0,SEEK_SET);
                 fwrite(&mod, sizeof(MBR),1,archivo);

                 fseek(archivo,0,SEEK_SET);
                 fread(&cp2,sizeof(MBR),1,archivo);
                 int pos  = ftell(archivo);
                 int tamp = sizeof(MBR);
                 printf("la fecha del mbr es: %s y la part 1 status %c pos puntero: %d\n", cp2.mbr_fecha_creacion, cp2.mbr_partition_1.part_status, pos);
                 printf("tamanio de struct mbr: %d", tamp);
                 fclose(archivo);
        */
printf("si entro a crear particion***22***\n");
    return 1;
}


int main(void)
{
    printf(":::::::::::::Bienvenido::::::::\n");
 char cadena[500];


int b =1;
while(b==1){
  analizar(cadena , 500);
}


    return 0;
}

