#include <iostream>
#include "functions.h"
#include "optimalizationAlgorithms.h"
#include "ParticleSwarmOptimalization.h"
#include "Flocking.h"
#include "GenerateFucntionTexture.h"
#include "SimulatedAneaning.h"
#include "PSO.h"
#include "FireflyAlgorithm.h"
#include "GlowwormSwarmAlgorith.h"
#include "ChargedSystemSearch.h"
#include "GrawitationalSearchAlgorithm.h"
#include "BatAlgorithm.h"
#include "RiverFormationDynamic.h"
#include <SDL\SDL_ttf.h>
#include "UniformRandomNumbers.h"
#include "CauchyRandomNumbers.h"
#include <thread>
#include "GaussRandomNumbers.h"
#include "LevyRandomNumbers.h"
#include "IntelligentWaterDrops.h"
#include "DrawFourGenerators.h"
#include "DifferentialEvolution.h"
#include "CuckooSearch.h"
#include "BlackHolesAlgorithm.h"

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	///Tutaj jest przyk³ad jak nale¿y u¿ywaæ tego co wam da³em. W pliku function.h jest 21 fuckji specjalnych które s¹ doœæ zabawne.
	///Dodatkowo w tym pliku znajduje tym wiliczeniowy (enum) w którym s¹ zawarte nazwy ka¿dej z funckji. 
	///Dziêki temu za pomoc¹ funckji getFuncton(nazwa funkcji) wzracamy funckjê, i jej dziedinê,. Zwracamy tkzw, tuplê i w sk³¹d tej tupli wchodzi (finckja,dziedzina sk³adaj¹ca siê z dwóch wektórów)
	///Poni¿ej macie przyk³ad jak pobieraæ dane z tej tupli i jak ko¿ystaæ z ka¿dej klasy posiadaj¹cej jakiœ algorytm liczenia minimów funkcji.
	///I robiie coœ takiego. Tworzycie obiekt klasy dodaj¹c do konstruktora to co jest poni¿ej jak w przyk³adzie i wywo³ujecje funkcjê run.... i to wszystko i tak dzia³a ka¿dy algorytm.
	int i = 0;
	std::cin >> i;
	auto fun = getFunction((Functions)i);

	GrawitationalSearchAlgorithm grawitationalSearchAlgorithm(std::get<0>(fun), std::get<1>(fun), std::get<2>(fun),40);
	grawitationalSearchAlgorithm.run(50);

	//BatAlgorithm batAlgorithm(std::get<0>(fun), std::get<1>(fun), std::get<2>(fun));
	//batAlgorithm.run(50);

	//DifferentialEvolution differentialEvolution(std::get<0>(fun), std::get<1>(fun), std::get<2>(fun), 10, 0.1);
	//differentialEvolution.run();

	//CuckooSearch cuckooSearch(std::get<0>(fun), std::get<1>(fun), std::get<2>(fun));
	//cuckooSearch.run(50);

	//BlackHolesAlgorithm bh(std::get<0>(fun), std::get<1>(fun), std::get<2>(fun));
	//bh.run(50);

	//Pso pso(std::get<0>(fun), std::get<1>(fun), std::get<2>(fun), 25);
	//pso.run(50);

	//FireflyAlgorithm firefly(std::get<0>(fun), std::get<1>(fun), std::get<2>(fun),20);
	//firefly.run(50);

	//GlowwarmSwarmAlgorithm glow(std::get<0>(fun), std::get<1>(fun), std::get<2>(fun),200);
	//glow.run(50);

	//ChargedSystemSearch chargetSystemSearch(std::get<0>(fun), std::get<1>(fun), std::get<2>(fun),40);
	//chargetSystemSearch.run(50);

	//SilumatedAneaning sim(std::get<0>(fun), std::get<1>(fun), std::get<2>(fun));
	//sim.run();

	///tutaj znajduje siê przyk³ad u¿ycia algorytmu wyszukiwania najkrótszej œcie¿ki. Jak na razie jest tylko jeden, ale ka¿dy bêdzie wygl¹da³ tak samo.
	///Chodzi mi o u¿ycie tych algorytmów. Czyli w konstuktorze podajemy nazwê pliku w któtym znajduje siê graf, a nastêpnie wywo³ywana jest finkcja run w której wszyskto siê wykonyje.
	///i tu wszystko co trzeba zrobiæ.
	//RiverFormationDynamic riverFormationDynamics("./graph.txt");
	//riverFormationDynamics.run();
	//IntelligentWaterDrops inteligentWaterDrops("./graph.txt");
	//inteligentWaterDrops.run();

	///Liczby pse³dolosowe. To pierwsze wyœwietla nam 4 okna. Reszta wyœwietla po jednym.
	//DrawFourGenerators dr;
	//dr.run();

	//CauchyRandomNumbers cauchyRandomNumbers;
	//cauchyRandomNumbers.run();

	//UniformRandomNumbers uniformRandomNumbers;
	//uniformRandomNumbers.run();
	
	//GaussRandomNumbers gaussRandomNumbers;
	//gaussRandomNumbers.run();

	//LevyRandomNumbers levyRandomNumbers;
	//levyRandomNumbers.run();

	///TO s¹ rzeczy for fun czyli œledzenie kursora myszki i flocking
	//ParticleSwarmOptimalization ps(glm::vec2(800), glm::vec2(-5, -5), glm::vec2(5, 5));
	//ps.run();

	//Flocking flocking(50, 600, 600);
	//flocking.flockingMouse();
	//flocking.freeFlocking();
	//flocking.wandererFlocking();
	//flocking.wandererMouseFlocking();

	TTF_Quit();
	SDL_Quit();
	std::cin.get();
	return 0;
}