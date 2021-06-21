//Filippo Bellato 1224717 Assegnamento1
#include "LaserScannerDriver.h"


int main() {

	LaserScannerDriver o(2);
	std::vector<double> test(181); //vettore di prova
	for (int i = 0; i < test.size(); i++) {
		test[i] = i;
	}

	o.new_scan(test);
	o.new_scan(test);
	o.new_scan(test);
	o.new_scan(test);
	o.new_scan(test);


	std::cout << o.get_distance(180) << "\n";
	std::vector<double> test1 = o.get_scan();
	std::cout << o;

	LaserScannerDriver copia = o;
	o.clear_buffer();
	
	std::cout << copia;

	
	o.get_distance(1);
	o.get_scan();

	return 0;
}