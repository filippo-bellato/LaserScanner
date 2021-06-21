//Filippo Bellato 1224717 Assegnamento1
#include <iostream>
#include <vector>
#include <cmath>

class LaserScannerDriver {
private:
	static constexpr int BUFFER_DIM = 10; //dimensione del buffer costante. Invariante
	double** buffer; //buffer come array di puntatori a double
	double resolution; //risoluzione dello scanner. Invariante
	int index; //indice sempre una posizione avanti rispetto all'ultima scansione effettuata
	int size; //stato di riempimento del buffer
	int scan_dimension; //dimensione di oggni scansione. Invariante
	bool isEmpty() const; //verifica se il buffer è vuoto

public:
	LaserScannerDriver(double r) ; //costruttore con parametro la risolutione
	LaserScannerDriver(); //costruttore di default
	void new_scan(std::vector<double> &v);
	std::vector<double> get_scan();
	void clear_buffer(); 
	double get_distance(double grad) const;
	friend std::ostream& operator<<(std::ostream& os, const LaserScannerDriver obj); //overloading <<

	LaserScannerDriver(const LaserScannerDriver& other); //costruttore di copia
	LaserScannerDriver& operator=(const LaserScannerDriver& a); //operatore di assegnamento

	LaserScannerDriver(LaserScannerDriver&& a) noexcept; //move constructor
	LaserScannerDriver& operator=(LaserScannerDriver&& other) noexcept; //move assignment
	~LaserScannerDriver(); //distruttore
};