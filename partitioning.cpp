#include<iostream>
#include<stdio.h>
#include <cstdlib>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

using namespace std;
map <int, int > cellsToarea;
int total_area;
int max_cell_area;
map<int, bool> locked_cells; 


void printVector(vector<int> vec) {
	for (vector<int>:: iterator itr = vec.begin(); itr != vec.end(); ++itr) {
		cout << *itr << ' ';	
	} 
		cout << endl;

} 

vector <int> split(string s, char c) { 
	vector <int> vec;
	vector <string> str;	
	string number;
	char single_char ;
	int int_num;

	for (unsigned i = 0; i< s.length(); i++) // giving problem.
	{
		single_char = s.at(i);
		 if (single_char != ' ' && (i+1) != s.length()) {
			number = number + single_char;					
		} 
		else {
			number = number + '\0';
			int_num = atoi(number.c_str());
			str.push_back(number);
			vec.push_back(int_num);

			number.clear();					
		}
	}
	//for (vector<int>:: iterator itr = vec.begin(); itr != vec.end(); ++itr) {
	//cout << *itr << endl ;
	//}
	return vec;
}

map <int, vector<int> > readFile(string filename) {
	map <int, vector<int> > netsTocell;
	map <int, vector<int> > cellsTonet;
	int tot_net, tot_cell, count = 0, num_cell = 0;
	char delimiter = ' ';
	ifstream hyperfile;
	string nets;
	hyperfile.open(filename.c_str());
	if(!hyperfile.is_open()) {
		cout << "Error in opening the file" << endl;

	} 
	else{

	   while(hyperfile.good()) {

		
		getline(hyperfile,nets);
		vector <int> separate_net;
		
		separate_net = split(nets, delimiter) ;
		if (count == 0) {
			tot_net = separate_net[0];
			tot_cell = separate_net[1];
			cout <<  "total_net = " <<tot_net <<  " total_cell = " <<tot_cell << endl;		
		} else {

			if (count > tot_net) {
				num_cell++ ;
				//break;
				cellsToarea[num_cell] = separate_net[0];
				locked_cells[num_cell] = false;
				
				if (num_cell == tot_cell) {
					break;
				}
			}	
			else { 
				netsTocell[count] = separate_net;
			}
		
		} 
		 count++;
	   }
	}
	
	for (unsigned int i = 1; i<= netsTocell.size(); i++ ) {
		printVector(netsTocell[i]);
	}
	cout << "printing cell area" << endl;
	for(map<int, int>:: iterator itr = cellsToarea.begin(); itr != cellsToarea.end(); ++itr) { 
		
		cout << itr-> first << "--> " << itr-> second << endl;
		
	}	
	return netsTocell;
}

map<int, vector<int> > convert_cellsTonet(map<int, vector<int> > netsTocell ) {
	vector <int > separate_net;
	map<int, vector<int> > cellsTonet;
	for (unsigned int j = 1; j <= netsTocell.size(); j++) {
		separate_net = netsTocell[j];	
		for (unsigned int i = 0; i < separate_net.size(); i++) {
			cellsTonet[separate_net[i]].push_back(j);
		}
	}
	return cellsTonet; 
}

//int cell [] = {1,2,3,4,5,6,7,8};
//int part [] = {0,1,0,0,1,1,0,1};


map<int,int> initial_partition(map <int, vector<int> > cellsTonet) { 
	map<int,int> init_part;
	/*init_part[1] = 0;
	init_part[2] = 1;
	init_part[3] = 0;
	init_part[4] = 0;
	init_part[5] = 1;
	init_part[6] = 1;
	init_part[7] = 0;
	init_part[8] = 1;*/

	for (unsigned int i = 1; i<= cellsTonet.size() / 2; i++) { 
		init_part[i] = 0;
	}
	for (unsigned int i = (cellsTonet.size() / 2) + 1; i<= cellsTonet.size(); i++) { 
		init_part[i] = 1;
	}
		
		//init_part.insert(pair<int,int>(cell[i],part[i]));
	
	/*for(map<int,int>:: iterator itr= init_part.begin(); itr != init_part.end(); itr++) { 
		cout << itr-> first<< "--" << itr-> second << endl;
	}*/
	return init_part;
}

map<int, vector<int> > compute_gain(map <int, vector<int> > cellsTonet, map <int, vector<int> > netsTocell, map<int,int> init_part) {
	
	map<int, vector<int> > gainTocell;
	
	int cellForGain;
	vector <int> nets;
	for (unsigned int i = 1; i<= cellsTonet.size(); i++ ) {
		int Fs = 0;
		int Ts = 0;
		int Gs = 0;
		cellForGain= i;		
		nets = cellsTonet[i];
		vector <int> cells;
		for  (unsigned int j = 0; j< nets.size(); j++) { 
			cells = netsTocell[nets.at(j)]; 
			int prob_ts =0;
			int prob_fs = 0;
			for (unsigned int k = 0; k< cells.size(); k++ ) {
				if (init_part[cellForGain] == init_part[cells.at(k)]) { 
					prob_ts++;
				}
				else
					prob_fs++;
			}
			
			if (prob_ts == cells.size()) {
				Ts = Ts + 1;
			}
			else if (prob_fs == cells.size() - 1) 
				Fs = Fs + 1;
		} 
			
		Gs = Fs - Ts;
		gainTocell[Gs].push_back(cellForGain) ;

	}

	return gainTocell;
}

float calculate_r(map<int, int> init_part, map<int, int> cellsToarea) { 
	float r;
	int area_A = 0;
	int area_B = 0;
	for (unsigned int i = 1; i<= cellsToarea.size() ; i++) { 
		if (init_part[i] == 0) {
			area_A = area_A + cellsToarea[i] ; 
			}
			else
				area_B = area_B + cellsToarea[i];
	}
	r = (1.0 * area_A) / (area_A + area_B);
	cout<< "value of area = " << r <<endl;	
	return r; 
	}
	
int partition_area(map<int, int> init_part, map<int, int> cellsToarea) { 
	int area_A = 0;
	int area_B = 0;
	for (unsigned int i = 1; i<= cellsToarea.size(); i++) { 
		if (init_part[i] == 0) {
			area_A = area_A + cellsToarea[i] ; 
		}
		else
			area_B = area_B + cellsToarea[i];
	}
	return area_A;
}

void calculate_area( map<int, int> cellsToarea) {
	total_area = 0;
	max_cell_area = 0;
	for (unsigned int i = 1; i<= cellsToarea.size() ; i++) { 
		total_area = total_area + cellsToarea[i] ; 
		if (max_cell_area < cellsToarea[i]) { 
				max_cell_area = cellsToarea[i];
		}
	}
	cout << "Total area of cells = " << total_area << "\nMaximum cell area = " << max_cell_area << endl;
	}

bool area_constraint (float r,  int max_cell_area, int total_area, int area_A) { 
	if ((r*total_area - max_cell_area) <= area_A  && area_A <= (r* total_area + max_cell_area)) {
		return true;
		}
		else
			return false;
	
	}
	
bool all_cells_locked(map<int, bool> locked_cells) {
	int lc = 0;
	for (unsigned int i = 1; i<= locked_cells.size(); i++) {
		if (locked_cells[i] == true)
			lc++;
	}
	if (lc == locked_cells.size()) { 
		return true;
	}
	else
		return false;
}

void print_gains (map<int, vector<int> > gainTocell) { 
	cout << "printing gains of cell" << endl;
	
	for (map<int,vector<int> >:: iterator itr = gainTocell.begin(); itr != gainTocell.end(); itr++) {
		cout << itr-> first << "--> " ;
		printVector(itr-> second); 
	}
	
}

int calculate_cutsize(map <int, vector<int> > netsTocell, map<int,int> init_part) { 
	int cutsize = 0;
	vector<int> cells;
	for (unsigned int i = 1; i<= netsTocell.size(); i++) { 
		cells = netsTocell[i];
		bool cut_status = false;
		for (unsigned int j = 0; j < cells.size(); j++) {
			for (unsigned int k = 0; k < cells.size(); k++) {
				if(init_part[cells[j]] != init_part[cells[k]]) { 
						cutsize++;
						cut_status = true;
						break;
				}
			}
			if (cut_status == true) {
				break;
			}
		}
	}
		
	
	return cutsize;
	}
	
int main(int argc, char** argv)
{
	map <int, vector<int> > cellsTonet;
	map <int, vector<int> > netsTocell;
	map<int, vector<int> > gainTocell;
	map<int,int> init_part;
	float r;
	int cutsize, init_cutsize;	
	netsTocell = readFile(string(argv[1]));
	
	cellsTonet = convert_cellsTonet(netsTocell);
	init_part = initial_partition(cellsTonet);
	init_cutsize = calculate_cutsize(netsTocell, init_part);
	r = calculate_r(init_part, cellsToarea);
	calculate_area(cellsToarea);	
	gainTocell = compute_gain(cellsTonet, netsTocell, init_part);
	
	cout << "Size of gain to cell = " << gainTocell.size() << endl;
	cout << "Size of cells to net = " << cellsTonet.size() << endl;
	cout << "Initial cutsize = " << init_cutsize << endl;
	
	for (unsigned int i = 1; i<= cellsTonet.size(); i++ ) {
		printVector(cellsTonet[i]);
	}
	//print_gains(gainTocell);
	// runing the partitioning alorithm FM!!
	vector<int> max_gain_cells; 
	map<int,int> intermediate_part = init_part;	
	int area_A;
	bool is_constraint_satisfied;
	bool is_all_cells_locked; 
	int iteration = 0; 
	vector<int> cutsizes;
	while (1) { 
		cout << "Iteration = " << iteration << endl;
		iteration++;
		is_all_cells_locked = all_cells_locked(locked_cells); // make function
		if (is_all_cells_locked == true) {
			cout << "All cells are locked" << endl;
			break;
		}
		gainTocell = compute_gain(cellsTonet, netsTocell, intermediate_part);	
		bool moved_cell = false;
		for (map<int, vector<int> >:: reverse_iterator itr = gainTocell.rbegin(); itr != gainTocell.rend(); itr++) {
			max_gain_cells = itr->second ;
			
			/*map<int, vector<int> >:: reverse_iterator itr = gainTocell.rbegin();
			max_gain_cells  = itr->second;*/
			//print_gains(gainTocell);
			//printVector(max_gain_cells); 
			for(unsigned int i =0; i< max_gain_cells.size(); i++ ) {
				if (locked_cells[max_gain_cells[i]] == true) { 
					continue;
				}
				int cell_partition = intermediate_part[max_gain_cells[i]];
				if (cell_partition == 0) {
					intermediate_part[max_gain_cells[i]] = 1;
				}
				else { 
					intermediate_part[max_gain_cells[i]] = 0;
				}	
				area_A = partition_area(intermediate_part, cellsToarea);
				is_constraint_satisfied = area_constraint (r, max_cell_area, total_area, area_A);
				if (is_constraint_satisfied == true ) {
					locked_cells[max_gain_cells[i]] = true;
					moved_cell = true;
					/*cout << "Cell moved = " << max_gain_cells[i] << endl;
					print_gains(gainTocell);*/
					break;
				}
				else {
					//cout << "constraint not satisfied " << endl;
					intermediate_part[max_gain_cells[i]] = cell_partition;
				}
			}
			if(moved_cell == true) {
				break;
			
			}
		}
		cutsize = calculate_cutsize(netsTocell, intermediate_part);
		cutsizes.push_back(cutsize);
		cout << "Cutsize = " << cutsize << endl;
		cout << endl ; 
	}
	/*for (map<int, vector<int> >:: reverse_iterator itr = gainTocell.rbegin(); itr != gainTocell.rend(); itr++) {
		 max_gain_cells = itr->second ;
		 cout << itr->first << "--> " ;
		 printVector(max_gain_cells); 
	}*/
	int min_cutsize = init_cutsize; 
	int min_index = 0;
	for( unsigned int i = 0; i< cutsizes.size(); i++ ) { 
		if (min_cutsize > cutsizes[i]) {
			min_cutsize = cutsizes[i];
			min_index = i; 
		}
	}
	cout << " Final cut size = " << min_cutsize << " at iteration = " << min_index << endl;
	//printVector(netsTocell[2]);
	return 0;
}








