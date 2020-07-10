#ifndef _CHECKARGS_H_
#define _CHECKARGS_H_

#include <unistd.h>

extern char *optarg;
extern int optind, opterr, optopt;

class checkArgs {
private:
        // 1) Modificar esta sección
        const std::string optString = "N:t:l:L:h";

        const std::string opciones = "-N <nro> -t <nro> -l <nro> -L <nro> [-h]";

        const std::string descripcion  = "Descripción:\n"
                                         "\t-N   Tamaño del arreglo\n"
                                         "\t-t   Cantidad de threads a utilizar\n"
                                         "\t-l   Limite inferior rango aleatorio\n"
                                         "\t-L   Límite superior rango aleatorio\n"
                                         "\t[-h]   Muestra esta ayuda y termina\n"
                                         ;

        typedef struct args_t{
                uint32_t tamArray;
                uint32_t numThreads;
                uint32_t limitinf;
                uint32_t limitsup;

        } args_t;

        // 2) Modificar constructor
        // 3) Modificar ciclo "getopt" en método checkArgs::getArgs()
        // 4) Recuerde que para compilar nuevamente, y por tratarse
        //    de un archivo header, debe hacer primero un make clean

        args_t  parametros;

        int argc;
        char **argv;


        public:
            checkArgs(int _argc , char **_argv);
            ~checkArgs();
            args_t getArgs();

        private:
            void printUsage();


        };
        checkArgs::checkArgs(int _argc , char **_argv){
        parametros.tamArray    = 0;
        parametros.numThreads  = 0;
        parametros.limitinf    = 0;
        parametros.limitsup    = 0;
        argc = _argc;
        argv = _argv;

}

checkArgs::~checkArgs(){

}

checkArgs::args_t checkArgs::getArgs(){
        int opcion;

        while ((opcion = getopt (argc, argv, optString.c_str())) != -1){
                switch (opcion) {
                        case 'N':
                                        parametros.tamArray = atoi(optarg);
                                        break;
                        case 't':
                                        parametros.numThreads = atoi(optarg);
                                        break;
                        case 'l':
                                        parametros.limitinf  = atoi(optarg);
                                        break;
                        case 'L':
                                        parametros.limitsup = atoi(optarg);
                                        break;
                        case 'h':
                        default:
                                        printUsage();
                                        exit(EXIT_FAILURE);
                }
        }


if ( parametros.tamArray == 0 ||
     parametros.numThreads  == 0 ||
     parametros.limitsup == 0  ||
     parametros.limitinf == 0){
     	printUsage();
        exit(EXIT_FAILURE);
        }

        return(parametros);
}

void checkArgs::printUsage(){
        std::cout << "Uso: " <<
                argv[0] << " " << opciones << " " << descripcion << std::endl;
}



#endif


