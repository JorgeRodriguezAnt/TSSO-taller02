#include <global.h>
#include <checkArgs.hpp>

void sumaParcial(const std::vector<uint32_t> &v,
                 uint32_t &suma,
                 size_t beginIndex,
                                 size_t endIndex) {
    suma = 0;
    for (size_t i = beginIndex; i < endIndex; ++i){
        suma += v[i];
    }
}

int main(int argc, char** argv){

        uint32_t tamArray;
        uint32_t numThreads;
        uint32_t limitinferior;
        uint32_t limSuperior;

        uint32_t sumaSerial = 0;
      
        uint32_t sumaThreads = 0;

        std::vector<uint32_t> v;

        std::shared_ptr<checkArgs> argumentos = (std::shared_ptr<checkArgs>) new checkArgs(argc, argv);

        tamArray       = argumentos->getArgs().tamArray;
        numThreads     = argumentos->getArgs().numThreads;
        limitinferior  = argumentos->getArgs().limitinf;
        limSuperior    = argumentos->getArgs().limitsup;

        std::cout << "Tamaño del arreglo: " << tamArray << std::endl;
        std::cout << "Threads  : " << numThreads     << std::endl;
        std::cout << "Limit inferior: " << limitinferior  << std::endl;
        std::cout << "Limit superior : " << limSuperior     << std::endl;

         static thread_local std::random_device device;
        static thread_local std::mt19937 rng(device());

        //Modulo llenado
        std::uniform_int_distribution<uint32_t> nRandom(limitinferior, limSuperior);
        for(size_t i=0; i < tamArray; i++){
                uint32_t number = nRandom(rng);
                v.push_back( number );
        }

        //======SERIAL======
        auto start = std::chrono::high_resolution_clock::now();
        for(auto& num : v){
                sumaSerial += num;
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        float totalTimeSerial = elapsed.count();

        //======THREADS======
        std::thread hilos[numThreads];
        uint32_t sumasParciales[numThreads];
        uint32_t tam = v.size() / numThreads;

        //Separación del trabajo

        start = std::chrono::high_resolution_clock::now();
        for(uint8_t x = 0; x<numThreads;x++){
                hilos[x] = std::thread(sumaParcial, std::ref(v), std::ref(sumasParciales[x]), tam*x, tam*(x+1));
        }
        for(uint8_t x = 0; x<numThreads;x++){
                hilos[x].join(); // se sincronizan los hilos
        }

        //Consolidación de resultados parciales
        for(uint8_t x = 0; x<numThreads;x++){
                sumaThreads+= sumasParciales[x];
        }

        end = std::chrono::high_resolution_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        float totalTimeThread = elapsed.count();

        //======Mostrar resultados=====
        float speedUp = totalTimeSerial/totalTimeThread;
        float eficiencia = speedUp/numThreads;
        std::cout << "====Serial====" << std::endl;
        std::cout << "sumaSerial  : " << sumaSerial << std::endl;
        std::cout << "Tiempo total serial   :"  << totalTimeSerial << std::endl;
        std::cout << std::endl;
        std::cout << "====Threads====" << std::endl;
        std::cout << "sumaThreads : " << sumaThreads << std::endl;
        std::cout << "Tiempo total threads :"  << totalTimeThread << std::endl;
        std::cout << std::endl;

        std::cout << "SpeedUp    :"<<speedUp << std::endl;
        std::cout << "Eficiencia :"<<eficiencia << std::endl;
        
        return(EXIT_SUCCESS);
}









