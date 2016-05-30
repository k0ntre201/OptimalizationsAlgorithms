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
	///Tutaj jest przyk�ad jak nale�y u�ywa� tego co wam da�em. W pliku function.h jest 21 fuckji specjalnych kt�re s� do�� zabawne.
	///Dodatkowo w tym pliku znajduje tym wiliczeniowy (enum) w kt�rym s� zawarte nazwy ka�dej z funckji. 
	///Dzi�ki temu za pomoc� funckji getFuncton(nazwa funkcji) wzracamy funckj�, i jej dziedin�,. Zwracamy tkzw, tupl� i w sk��d tej tupli wchodzi (finckja,dziedzina sk�adaj�ca si� z dw�ch wekt�r�w)
	///Poni�ej macie przyk�ad jak pobiera� dane z tej tupli i jak ko�ysta� z ka�dej klasy posiadaj�cej jaki� algorytm liczenia minim�w funkcji.
	///I robiie co� takiego. Tworzycie obiekt klasy dodaj�c do konstruktora to co jest poni�ej jak w przyk�adzie i wywo�ujecje funkcj� run.... i to wszystko i tak dzia�a ka�dy algorytm.
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

	///tutaj znajduje si� przyk�ad u�ycia algorytmu wyszukiwania najkr�tszej �cie�ki. Jak na razie jest tylko jeden, ale ka�dy b�dzie wygl�da� tak samo.
	///Chodzi mi o u�ycie tych algorytm�w. Czyli w konstuktorze podajemy nazw� pliku w kt�tym znajduje si� graf, a nast�pnie wywo�ywana jest finkcja run w kt�rej wszyskto si� wykonyje.
	///i tu wszystko co trzeba zrobi�.
	//RiverFormationDynamic riverFormationDynamics("./graph.txt");
	//riverFormationDynamics.run();
	//IntelligentWaterDrops inteligentWaterDrops("./graph.txt");
	//inteligentWaterDrops.run();

	///Liczby pse�dolosowe. To pierwsze wy�wietla nam 4 okna. Reszta wy�wietla po jednym.
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

	///TO s� rzeczy for fun czyli �ledzenie kursora myszki i flocking
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