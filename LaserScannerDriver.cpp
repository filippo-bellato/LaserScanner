//Filippo Bellato 1224717 Assegnamento1
#include "LaserScannerDriver.h"

/*Costruttore di default con risoluzione = 1*/
LaserScannerDriver::LaserScannerDriver() {
	index = 0;
	size = 0;
	scan_dimension = 181;
	resolution = 1;
	buffer = new double* [BUFFER_DIM];
	for (int i = 0; i < BUFFER_DIM; i++) {
		buffer[i] = { nullptr };
	}
}

/*Costruttore con parametro la risoluzione. Inizializza tutte le variabili e tutti
gli elementi del buffer a nullptr.
Se la risoluzione non rientra nel range questa viene ripristinata a 1 o a 0.1
*/
LaserScannerDriver::LaserScannerDriver(double r) {
	if (r > 1)
		r = 1;
	else if (r < 0.1)
		r = 0.1;

	index = 0;
	size = 0;
	scan_dimension = (int) std::floor(180 / r) + 1;
	resolution = r;
	buffer = new double* [BUFFER_DIM];
	for (int i = 0; i < BUFFER_DIM; i++) {
		buffer[i] = { nullptr };
	}
}


/*Inserisce una nuova scansione nel buffer. Alloca nuova memoria solo se 
il buffer si sta riempiendo. Una volta pieno sovrascrive solo i dati*/
void LaserScannerDriver::new_scan(std::vector<double> &v) {
	if (buffer[index] == nullptr) { //verifica se quell'elemento è ancora vuoto
		buffer[index] = new double[scan_dimension];
	}

	for (int i = 0; i < v.size(); i++) {
		buffer[index][i] = v[i];
	}

	if (v.size() < scan_dimension) { //se il vettore è più corto dell'array aggiunge 0.0
		for (int i = v.size(); i < scan_dimension; i++) {
			buffer[index][i] = 0.0;
		}
	}

	if (index >= BUFFER_DIM - 1) //incrementa l'indice. Se è arrivato alla fine torna indietro visto che è un array circolare
		index = 0;
	else
		index++;

	if (size >= BUFFER_DIM) //aumenta la dimensione solo se non è già pieno
		size = BUFFER_DIM;
	else
		size++;
}

/*Ritorna un vector con la più vecchia scansione e la elimina. */
std::vector<double> LaserScannerDriver::get_scan() {
	if (isEmpty()) { //se il buffer è vuoto ritorna un vettore di dimensione 0
		std::cout << "Buffer vuoto\n";
		std::vector<double> no(0);
		return no;
	}

	std::vector<double> vettore(scan_dimension);
	 
	if (size <= index) { //in base alla posizione di index e size calcola dove si trova la scansione più vecchia
		for (int i = 0; i < scan_dimension; i++) {
			vettore[i] = buffer[index - size][i];
		}
		delete[] buffer[index - size];
		buffer[index - size] = { nullptr };
	}
	else
	{
		for (int i = 0; i < scan_dimension; i++) {
			vettore[i] = buffer[index + BUFFER_DIM - size][i];
		}
		delete[] buffer[index + BUFFER_DIM - size];
		buffer[index + BUFFER_DIM - size] = { nullptr };
	}

	size--; //dimensione diminuisce di 1
	return vettore;
}

/*Rimuove tutte le scansioni e ripristina tutti gli elementi a nullptr*/
void LaserScannerDriver::clear_buffer() {
	for (int i = 0; i < BUFFER_DIM; i++) {
		if (buffer[i] != nullptr) {
			delete[] buffer[i];
			buffer[i] = { nullptr };
		}
	}
	index = 0;
	size = 0;
}

/*Datogli un angolo restituisce la distanza corrispondente dell'ultima lettura*/
double LaserScannerDriver::get_distance(double angolo) const{
	if (angolo < 0 || angolo > 180) { //ritorna -1 se l'angolo non è valido
		std::cout << "Angolo fuori dall'intervallo(0 - 180)\n";
		return -1;
	}

	if (isEmpty()) { //ritorna -1 se il buffer è vuoto
		std::cout << "Buffer vuoto\n";
		return -1;
	}

	int indice = (int)(angolo / resolution) + 0.5; //calcola l'indice al quale si trova l'angolo richiesto

	if (index > 0) //in base a dove si trova l'indice restituisce la distanza
		return buffer[index - 1][indice];
	else
		return buffer[BUFFER_DIM - 1][indice];
}

/*Overloading dell'operatore <<*/
std::ostream& operator<<(std::ostream& os, const LaserScannerDriver obj) {
	if (obj.isEmpty()) {
		os << "Buffer vuoto\n";
		return os;
	}

	if (obj.index > 0) { //calcola l'indice al quale si trova la scansione più recente
		for (int i = 0; i < obj.scan_dimension; i++) {
			os << obj.buffer[obj.index - 1][i] << "\n";
		}
	}
	else
	{
		for (int i = 0; i < obj.scan_dimension; i++) {
			os << obj.buffer[obj.BUFFER_DIM - 1][i] << "\n";
		}
	}
	return os;
}

/*Costruttore di copia*/
LaserScannerDriver::LaserScannerDriver(const LaserScannerDriver& other) 
	: resolution{ other.resolution }, 
	size{ other.size }, 
	index{ other.index },
	scan_dimension{ other.scan_dimension }
{
	buffer = new double* [BUFFER_DIM];
	for (int i = 0; i < BUFFER_DIM; i++) {
		if (other.buffer[i] == nullptr) {
			buffer[i] = { nullptr };
		}
		else
		{
			buffer[i] = new double[scan_dimension];
			for (int j = 0; j < scan_dimension; j++) {
				buffer[i][j] = other.buffer[i][j];
			}
		}
	}
}

/*Overloading operatore di assegnamento per la copia*/
LaserScannerDriver& LaserScannerDriver::operator=(const LaserScannerDriver& other) {
	double** temp = new double* [other.BUFFER_DIM];
	for (int i = 0; i < other.BUFFER_DIM; i++) {
		if (other.buffer[i] == nullptr) {
			temp[i] = { nullptr };
		}
		else
		{
			temp[i] = new double[other.scan_dimension];
			for (int j = 0; j < other.scan_dimension; j++) {
				temp[i][j] = other.buffer[i][j];
			}
		}
	}
	size = other.size;
	index = other.index;
	scan_dimension = other.scan_dimension;
	resolution = other.resolution;
	clear_buffer();
	buffer = temp;
	return *this;
}


/*Costruttore di spostamento*/
LaserScannerDriver::LaserScannerDriver(LaserScannerDriver&& other) noexcept {
	resolution = other.resolution;
	index = other.resolution;
	size = other.size;
	scan_dimension = other.scan_dimension;
	buffer = other.buffer;

	other.size = 0;
	other.buffer = nullptr;
}

/*Operatore di assegnamento per lo spostamento*/
LaserScannerDriver& LaserScannerDriver::operator=(LaserScannerDriver&& other) noexcept {
	clear_buffer();
	buffer = other.buffer;
	resolution = other.resolution;
	index = other.resolution;
	size = other.size;
	scan_dimension = other.scan_dimension;
	other.buffer = nullptr;
	other.size = 0;
	return *this;
}

/*Distruttore*/
LaserScannerDriver::~LaserScannerDriver() {
	clear_buffer();
	delete[] buffer;
}

bool LaserScannerDriver::isEmpty() const{
	return size <= 0;
}