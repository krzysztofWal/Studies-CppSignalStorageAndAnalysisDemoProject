//============================================================================
// Name        : DataProject.cpp
// Author      : Krzysztof Wal
//============================================================================

#include <iostream>
#include <fstream>
#include <memory>
#include <ctime>
#include <typeinfo>
#include "Packet.h"
#include "Sequence.h"
#include "TimeHistory.h"
#include "Spectrum.h"
#include "Alarm.h"
#include "Functions.h"
#include "Exceptions.h"


std::vector<std::ofstream> setFiles() {
	bool filesOpened = true;

	std::vector<std::ofstream> fileVector;
	fileVector.emplace_back(std::ofstream{ "channel1.txt", std::ios::ate });
	fileVector.emplace_back(std::ofstream{ "channel2.txt", std::ios::ate });
	fileVector.emplace_back(std::ofstream{ "channel3.txt", std::ios::ate });
	fileVector.emplace_back(std::ofstream{ "channel4.txt", std::ios::ate });
	fileVector.emplace_back(std::ofstream{ "channelSum.txt", std::ios::ate });
	fileVector.emplace_back(std::ofstream{ "channelQuotient.txt", std::ios::ate });


	for (size_t i{ 0 }; i < fileVector.size(); i++) {
		if (!fileVector.at(i))
			filesOpened = false;
	}

	if (filesOpened)
		return fileVector;
	else
		return std::vector<std::ofstream>{};
}

int main() {

	time_t secondsElapsed{};
	const time_t plannedTime{ 2 };

	try {
		_set_se_translator(ExceptionHandler);

		std::vector<std::ofstream> fileVector = setFiles();

		secondsElapsed = time(NULL);

		if (fileVector.empty())
			throw FilesNotOpened();

		//przydzielenie pamięci i inicjalizacja czterech kanałów
		std::vector<std::unique_ptr<TimeHistory<float, 400, 10>>> channelVector;

		channelVector.emplace_back(std::make_unique<TimeHistory<float, 400, 10>>(
			"urzadzenie 1", "pierwsza akwizycja", 07052020, 1, "volt", 0.02, 0.03
			));
		channelVector.emplace_back(std::make_unique<TimeHistory<float, 400, 10>>(
			"urzadzenie 2", "druga akwizycja", 07052020, 2, "volt", 0.02, 0.03
			));
		channelVector.emplace_back(std::make_unique<TimeHistory<float, 400, 10>>(
			"urzadzenie 3", "trzecia akwizycja", 07052020, 3, "volt", 0.02, 0.03
			));
		channelVector.emplace_back(std::make_unique<TimeHistory<float, 400, 10>>(
			"urzadzenie 4", "czwarta akwizycja", 07052020, 4, "volt", 0.02, 0.03
			));


		//zasymulowanie danych z białym szumem
		channelVector.at(0)->simulate(3.0, 0.3);
		channelVector.at(1)->simulate(6.0, 0.3);
		channelVector.at(2)->simulate(8.0, 0.1);
		channelVector.at(3)->simulate(3.0, 0.3);

		// zsumowanie kanału drugiego i trzeciego
		TimeHistory<float, 400, 10>* sum = new TimeHistory<float, 400, 10>;
		*sum = *channelVector.at(1) + *channelVector.at(2);

		//podzielenie kanału pierwszego przez czwarty
		TimeHistory<float, 400, 10>* quotient = new TimeHistory<float, 400, 10>;
		*quotient = *channelVector.at(0) / *channelVector.at(3);

		checkForTime(secondsElapsed, 1);

		//wyznaczanie widma amplitudowego
		std::vector<std::unique_ptr<SpectrumAbs<float, 400, 10>>> spectrumAbsVector;
		for (const auto& channel : channelVector) {
			spectrumAbsVector.emplace_back(std::make_unique<SpectrumAbs<float, 400, 10>>(*channel, logarythmic));
		}
		SpectrumAbs<float, 400, 10>* sumSpectrumAbs = new SpectrumAbs<float, 400, 10>{ *sum, logarythmic };
		SpectrumAbs<float, 400, 10>* quotientSpectrumAbs = new SpectrumAbs<float, 400, 10>{ *quotient, logarythmic };

		//wyznaczanie widma zespolonego
		std::vector<std::unique_ptr<SpectrumComplex<float, 400, 10>>> spectrumComplexVector;
		for (const auto& channel : channelVector) {
			spectrumComplexVector.emplace_back(std::make_unique<SpectrumComplex<float, 400, 10>>(*channel, linear));
		}
		SpectrumComplex<float, 400, 10>* sumSpectrumComplex = new SpectrumComplex<float, 400, 10>{ *sum, linear };
		SpectrumComplex<float, 400, 10>* quotientSpectrumComplex = new SpectrumComplex<float, 400, 10>{ *quotient, linear };

		checkForTime(secondsElapsed, 1);

		//zapisanie danych do plików
		for (size_t i{ 0 }; i < fileVector.size() - 2; i++) {
			fileVector.at(i) << "//czasowy przebieg sygnalu" << '\n';
			fileVector.at(i) << *channelVector.at(i);
			fileVector.at(i) << "//widmo amplitudowe sygnalu" << '\n';
			fileVector.at(i) << *spectrumAbsVector.at(i);
			fileVector.at(i) << "//widmo zespolone sygnalu" << '\n';
			fileVector.at(i) << *spectrumComplexVector.at(i);
		}
		fileVector.at(4) << "//czasowy przebieg sygnalu" << '\n';
		fileVector.at(4) << *sum;
		fileVector.at(4) << "//widmo amplitudowe sygnalu" << '\n';
		fileVector.at(4) << *sumSpectrumAbs;
		fileVector.at(4) << "//widmo zespolone sygnalu" << '\n';
		fileVector.at(4) << *sumSpectrumComplex;
		fileVector.at(5) << "//czasowy przebieg sygnalu" << '\n';
		fileVector.at(5) << *quotient;
		fileVector.at(5) << "//widmo amplitudowe sygnalu" << '\n';
		fileVector.at(5) << *quotientSpectrumAbs;
		fileVector.at(5) << "//widmo zespolone sygnalu" << '\n';
		fileVector.at(5) << *quotientSpectrumComplex;



		checkForTime(secondsElapsed, 2);

		//obliczenie Rms dla każdego kanału
		std::cout << "obliczone RMS: " << std::endl;
		for (const auto& channel : channelVector) {
			std::cout << "kanal " << channel->getChannelNr() << ": " << channel->calculateRms() << std::endl;
		}
		std::cout << "kanal 2+3: " << sum->calculateRms() << std::endl;
		std::cout << "kanal 1/4: " << quotient->calculateRms() << std::endl;

		checkForTime(secondsElapsed, 1);

		for (size_t i{ 0 }; i < fileVector.size(); i++) {
			fileVector.at(i).close();
		}

		delete sum;
		delete quotient;

	}
	catch (SystemException * e) {
		unsigned int exNum = e->seNumber();
		if (exNum == 3221225614 || exNum == 3221225620) {
			std::cout << "Proba podzielenia przez zero" << std::endl;
		}
		else if (exNum == 3221225477) {
			std::cout << "Naruszenie dostepu do pamieci" << std::endl;
		}
		else if (exNum == 3221225612) {
			std::cout << "Niepoprawny indeks tablicy" << std::endl;
		}
		else if (exNum == 3221225495) {
			std::cout << "Zabraklo pamieci" << std::endl;
		}
		else if (exNum == 3221225725) {
			std::cout << "Stack overflow" << std::endl;
		}
		else if (exNum == 3221225617 || exNum == 3221225621) {
			std::cout << "Nadmiar" << std::endl;
		}
		else if (exNum == 3221225619) {
			std::cout << "Niedomiar" << std::endl;
		}
		else {
			std::cout << "Nieokreslony wyjatek";
		}
	}
	catch (const TooMuchTime & ex) {
		std::cerr << ex.what() << std::endl;
	}
	catch (const FilesNotOpened & ex) {
		std::cerr << ex.what() << std::endl;
	}

	std::string choice{};

	while (choice != "q") {
		std::cout << "press q and enter to quit ";
		std::cin >> choice;
		std::cout << std::endl;
	}

	return 0;
}
