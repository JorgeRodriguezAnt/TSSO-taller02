#include <global.h>
#include <checkArgs.hpp>

void sumaParcial(const std::vector<uint32_t> &v,
                  std::vector<uint32_t> &sumasParciales,
                  size_t pos,
                  size_t beginIndex,
                  size_t endIndex) {
    for (size_t i = beginIndex; i < endIndex; ++i){
        sumasParciales[pos] += v[i];
    }
}

int main(int argc, char** argv){

        uint32_t totalElementos;
        uint32_t numThreads;      //cantidad de hilos
        uint32_t limSuperior;
        uint32_t limInferior;
  uint32_t sumaSerial = 0;
        uint32_t sumaThreads = 0;
        std::vector<uint32_t> v;
        std::vector<std::thread*> threads;
        std::vector<uint32_t> sumasParciales;
        std::shared_ptr<checkArgs> argumentos = (std::shared_ptr<checkArgs>) new checkArgs(argc, argv);
        totalElementos = argumentos->getArgs().tamProblema;   //tamaño del vector
        numThreads     = argumentos->getArgs().numThreads;
        limSuperior    = argumentos->getArgs().limSuperior;
        limInferior    = argumentos->getArgs().limInferior;
        static thread_local std::random_device device;
        static thread_local std::mt19937 rng(device());
        std::uniform_int_distribution<uint32_t> nRandom(limInferior, limSuperior);

        for(size_t i=0; i < totalElementos; i++){
                uint32_t number = nRandom(rng);
                v.push_back( number );
                sumasParciales.push_back(0);
        }

      //SERIAL

  auto start = std::chrono::system_clock::now();
        for(auto& num : v){
                sumaSerial += num;
        }
  auto end = std::chrono::system_clock::now();
        std::chrono::duration<float,std::milli> duration = end - start;
        auto totalTimeSerial = duration.count();

        //HILOS

        for(size_t i = 0; i < numThreads; ++i){
                threads.push_back( new std::thread(sumaParcial,
                                                                std::ref(v), std::ref(sumasParciales),i,
                                                                i*(totalElementos)/numThreads,
                                                                (i+1)*(totalElementos)/numThreads));
        }
  start = std::chrono::system_clock::now();
        for(auto& th : threads){
                th->join();   //sincronizar hilos
        }

        // Reducción (Consolidación de resultados parciales)
        for(auto& sumaTh : sumasParciales){
                sumaThreads += sumaTh;
        }
  end = std::chrono::system_clock::now();
  std::chrono::duration<float,std::milli> duration2 = end - start;
  auto totalTimeThread = duration2.count();

        //DESEMPEÑO
  float speedUp = (totalTimeSerial/totalTimeThread);
  float eficiencia = 1/(1+(totalTimeThread/totalTimeSerial));
      
	//RESULTADOS
  std::cout << "SumaSerial:" << sumaSerial << std::endl;
  std::cout << "TiempoTotalHilos:" << totalTimeThread << std::endl;
  std::cout << "TiempoTotalSerial:" << totalTimeSerial << std::endl;
  std::cout << "Acceleracion:" <<speedUp << std::endl;
  std::cout << "Eficiencia:" <<eficiencia << std::endl;
        return(EXIT_SUCCESS);
}




