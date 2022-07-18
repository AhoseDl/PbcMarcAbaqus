#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
//#include<bitset>
#include<algorithm>
#include<cmath>
#include<cassert>
#include<boost/math/statistics/linear_regression.hpp>

using namespace std;

/*  

------------------------  Construction des conditions de bord symétriques -------------------------------                                                                                               
                                                                            Z-
                                             D'               C'             -                          
                                              ----------------               -  -Y
                                            - -             --               - -
                                          -   -           -  -               --     
                                      A' ---------------B'   -               -----------X
                                         -    -        -     -
                                         -    -        -     -                         
                                         -   D----------------C   
                                         -  -          -   -                   
                                         - -           -  -   
                                         ---------------                                     
                                       A                B
                                       
Retained nodes : A, B, D, A'
A(Xmin, Ymin, Zmin), B (Xmax, Ymin, Zmin), C(Xmax, Ymax, Zmin), D(Xmin, Ymax, Zmin), A'(Xmin, Ymin, Zmax), B'(Xmax, Ymin, Zmax), C'(Xmax, Ymax, Zmax), D'(Xmin, Ymax, Zmax)                                

*/

int main(int argc, char** argv){

double E11, nu12, nu13, E22, nu21, nu23, E33, nu31, nu32, G12, G13, G23;

size_t NodeB, NodeD, NodeAp;
ifstream NStream("NodesBDAp.dat", ios::in);  // Le fichier NodesBDAp contient dans l'ordre les numéros des noeuds B, D et Ap
NStream >> NodeB >> NodeD >> NodeAp;
NStream.close();


string filenameTable[6]={"pbcTractionX.out", "pbcTractionY.out", "pbcTractionZ.out", "pbcCisXY.out", "pbcCisXZ.out" , "pbcCisYZ.out"};


string filename;
for (size_t fileNum=0; fileNum<6; fileNum++){

filename = filenameTable[fileNum];
ifstream filenameStream(filename, ios::in);
string line; string line1, line2, line3;
int match; size_t Num; size_t Inc=0;

vector<array<double, 6> > NodeBResults, NodeDResults, NodeApResults; // Les arrays contiennent Ux Uy Uz Rx Ry Rz
array<double, 6> IncNodeBResults, IncNodeDResults, IncNodeApResults; 

istringstream Line1Stream, Line2Stream, Line3Stream;

do{
getline(filenameStream, line, '\n');
match = line.find("t o t a l   d i s p l a c e m e n t s");
if(match != std::string::npos){
getline(filenameStream, line, '\n');
getline(filenameStream, line, '\n'); // On se déplace de deux lignes pour se mettre au niveau des données à lire

getline(filenameStream, line1, '\n');
getline(filenameStream, line2, '\n');
getline(filenameStream, line3, '\n');

Line1Stream.str(line1);
Line2Stream.str(line2);
Line3Stream.str(line3);

Inc++;  bool ModeLecture;
	if(Inc==1){
	size_t NodeBTest, NodeDTest, NodeApTest;
	istringstream Stream1(line1);
	istringstream Stream2(line2);
	istringstream Stream3(line3);
	Stream1>>NodeBTest;
	Stream2>>NodeDTest;
	Stream3>>NodeApTest;
	
	ModeLecture=(NodeB==NodeBTest && NodeD==NodeDTest && NodeAp==NodeApTest);	
	}
	
	
	if(ModeLecture){
	Line1Stream >> Num >> IncNodeBResults[0] >> IncNodeBResults[1]>> IncNodeBResults[2];
	Line2Stream >> Num >> IncNodeDResults[0] >> IncNodeDResults[1]>> IncNodeDResults[2];
	Line3Stream >> Num >> IncNodeApResults[0] >> IncNodeApResults[1]>> IncNodeApResults[2];
	
	for(size_t i=0; i<6; i++) getline(filenameStream, line, '\n');
	getline(filenameStream, line1, '\n');
	getline(filenameStream, line2, '\n');
	getline(filenameStream, line3, '\n');
	
	Line1Stream.str(line1);
	Line2Stream.str(line2);
	Line3Stream.str(line3);
	Line1Stream >> Num >> IncNodeBResults[3] >> IncNodeBResults[4]>> IncNodeBResults[5];
	Line2Stream >> Num >> IncNodeDResults[3] >> IncNodeDResults[4]>> IncNodeDResults[5];
	Line3Stream >> Num >> IncNodeApResults[3] >> IncNodeApResults[4]>> IncNodeApResults[5];
	}else
	{
	Line1Stream >> Num >> IncNodeBResults[0] >> IncNodeBResults[1]>> IncNodeBResults[2];
	Line1Stream >> Num >> IncNodeDResults[0] >> IncNodeDResults[1]>> IncNodeDResults[2];
	Line2Stream >> Num >> IncNodeApResults[0] >> IncNodeApResults[1]>> IncNodeApResults[2];
	for(size_t i=0; i<5; i++) getline(filenameStream, line, '\n');
	getline(filenameStream, line1, '\n');
	getline(filenameStream, line2, '\n');
	getline(filenameStream, line3, '\n');
	
	Line1Stream.str(line1);
	Line2Stream.str(line2);
	Line3Stream.str(line3);
	Line1Stream >> Num >> IncNodeBResults[3] >> IncNodeBResults[4]>> IncNodeBResults[5];
	Line1Stream >> Num >> IncNodeDResults[3] >> IncNodeDResults[4]>> IncNodeDResults[5];
	Line2Stream >> Num >> IncNodeApResults[3] >> IncNodeApResults[4]>> IncNodeApResults[5];
	}

NodeBResults.push_back(IncNodeBResults);
NodeDResults.push_back(IncNodeDResults);
NodeApResults.push_back(IncNodeApResults);
	
}

}while(! filenameStream.eof() );


// On assume que L=l=h=1; Sinon il faudra tenir compte des surfaces initiales et des longueurs initiales dans le calcul de la matrice de rigidité


using boost::math::statistics::simple_ordinary_least_squares;

if(filename == "pbcTractionX.out"){

/*E11 = ( NodeBResults[3] )/( NodeBResults[0] );
nu12 = -1*( NodeDResults[1] )/( NodeBResults[0] );
nu13 = -1*( NodeApResults[2] )/( NodeBResults[0] );*/

vector<double> V11A, V11B, V12A, V12B, V13A, V13B;
for (size_t i=0; i < NodeBResults.size(); i++){V11A.push_back(NodeBResults[i][3]); V11B.push_back(NodeBResults[i][0]); V12A.push_back(NodeDResults[i][1]); V12B.push_back(NodeBResults[i][0]); V13A.push_back(NodeApResults[i][2]); V13B.push_back(NodeBResults[i][0]);}
auto [ep11, e11]=simple_ordinary_least_squares(V11B, V11A);
auto [Nup12, Nu12]=simple_ordinary_least_squares(V12B, V12A);
auto [Nup13, Nu13]=simple_ordinary_least_squares(V13B, V13A);

E11=e11; nu12=-1*Nu12; nu13=-1*Nu13;

}else if(filename == "pbcTractionY.out"){

/*E22 = ( NodeDResults[4] )/( NodeDResults[1] );
nu21 = -1*( NodeBResults[0] )/( NodeDResults[1] );
nu23 = -1*( NodeApResults[2] )/( NodeDResults[1] );*/

vector<double> V22A, V22B, V21A, V21B, V23A, V23B;
for (size_t i=0; i < NodeDResults.size(); i++){
V22A.push_back(NodeDResults[i][4]); V22B.push_back(NodeDResults[i][1]); V21A.push_back(NodeBResults[i][0]); V21B.push_back(NodeDResults[i][1]); V23A.push_back(NodeApResults[i][2]); V23B.push_back( NodeDResults[i][1]);}
auto [ep22, e22]=simple_ordinary_least_squares(V22B, V22A);
auto [Nup21, Nu21]=simple_ordinary_least_squares(V21B, V21A);
auto [Nup23, Nu23]=simple_ordinary_least_squares(V23B, V23A);

E22=e22; nu21=-1*Nu21; nu23=-1*Nu23;

}else if(filename == "pbcTractionZ.out"){

/*
E33 = ( NodeApResults[5] )/( NodeApResults[2] );
nu31 = -1*( NodeBResults[0] )/( NodeApResults[2] );
nu32 = -1*( NodeDResults[1] )/( NodeApResults[2] );*/

vector<double> V33A, V33B, V31A, V31B, V32A, V32B;
for (size_t i=0; i < NodeDResults.size(); i++){
V33A.push_back(NodeApResults[i][5] ); V33B.push_back( NodeApResults[i][2] ); V31A.push_back(NodeBResults[i][0] ); V31B.push_back(NodeApResults[i][2]); V32A.push_back(NodeDResults[i][1] ); V32B.push_back( NodeApResults[i][2] );}
auto [ep33, e33]=simple_ordinary_least_squares(V33B, V33A);
auto [Nup31, Nu31]=simple_ordinary_least_squares(V31B, V31A);
auto [Nup32, Nu32]=simple_ordinary_least_squares(V32B, V32A);

E33=e33 ; nu31=-1*Nu31; nu32=-1*Nu32;

}else if(filename == "pbcCisXY.out"){

/*G12 = ( NodeBResults[4] )/( NodeBResults[1] );*/

vector<double> V12A, V12B;
for (size_t i=0; i < NodeBResults.size(); i++){
V12A.push_back( NodeBResults[i][4] ); V12B.push_back(NodeBResults[i][1] );}
auto [g12p, g12]=simple_ordinary_least_squares(V12B, V12A);

G12=g12;

}else if(filename == "pbcCisXZ.out"){

/*G13 = ( NodeBResults[5] )/( NodeBResults[2] );*/


vector<double> V13A, V13B;
for (size_t i=0; i < NodeBResults.size(); i++){
V13A.push_back(NodeBResults[i][5]); V13B.push_back(NodeBResults[i][2]);}
auto [g13p, g13]=simple_ordinary_least_squares(V13B, V13A);

G13=g13;

}else if(filename == "pbcCisYZ.out"){

/*G23 = ( NodeDResults[5] )/( NodeDResults[2] );*/

vector<double> V23A, V23B;
for (size_t i=0; i < NodeBResults.size(); i++){
V23A.push_back(NodeDResults[i][5]); V23B.push_back(NodeDResults[i][2] );}
auto [g23p, g23]=simple_ordinary_least_squares(V23B, V23A);

G23=g23;

}
} // Fin du parcours des fichiers

cout<<endl<<"============================================================================================================="<<endl;
cout<<"E11      |nu12     |nu13     |E22      |nu21     |nu23     |E33      |nu31     |nu32     |G12      |G13      |G23"<<endl;

cout<<left<<setw(9)<<E11<<" "<<setw(9)<<nu12<<" "<<setw(9)<<nu13
<<" "<<setw(9)<<E22<<" "<<setw(9)<<nu21<<" "<<setw(9)<<nu23
<<" "<<setw(9)<<E33<<" "<<setw(9)<<nu31<<" "<<setw(9)<<nu32
<<" "<<setw(9)<<G12<<" "<<setw(9)<<G13<<" "<<setw(9)<<G23<<endl;


return 0;}
