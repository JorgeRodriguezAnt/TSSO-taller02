#ifndef _CHECKARGS_H_
#define _CHECKARGS_H_

#include <unistd.h>

extern char *optarg;
extern int optind, opterr, optopt;

class checkArgs {
private:
        
        const std::string optString = "N:t:l:L:h";

        const std::string opciones = "-N <nro> -t <nro> -l <nro> -L <nro> [-h]";
        const std::string descripcion  = "Descripción:\n"
                                             "\t-N   Tamaño del Vector\n"
                                             "\t-t   Cantidad de threads a utilizar\n"
                                             "\t-l   Limite inferior rango aleatorio\n"
                                             "\t-L   Limite superior rango aleatorio\n"
                                             "\t-h   Muestra esta ayuda y termina\n";

        typedef struct args_t{
                uint32_t tamProblema;
                uint32_t numThreads;
                uint32_t limInferior;
                uint32_t limSuperior;
        } args_t;

	
        
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
        parametros.tamProblema = 0;
        parametros.numThreads  = 0;
        parametros.limInferior = 0;
        parametros.limSuperior = 0;

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
                                        parametros.tamProblema = atoi(optarg);
                                        break;
                        case 't':
                                        parametros.numThreads = atoi(optarg);
                                        break;

                        case 'l':
                                        parametros.limInferior = atoi(optarg);
                                        break;
                        case 'L':
                                        parametros.limSuperior = atoi(optarg);
                                        break;
                        case 'h':
                        default:
                                        printUsage();
                                        exit(EXIT_FAILURE);
                }
        }

	
	
	 if ( parametros.tamProblema == 0 ||
                 parametros.numThreads  == 0 ){
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
