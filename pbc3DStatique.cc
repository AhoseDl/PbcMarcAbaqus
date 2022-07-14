#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include<bitset>
#include<algorithm>
#include<cmath>
#include<cassert>

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


template<typename T, size_t D>
class Point : public array<T,D>{

protected:
	T x[D];
	
public:

	template<typename U, size_t Di>
	friend ostream& operator <<(ostream& sortie, const Point<U,Di>& Pt);
	//template<typename U, size_t Di>
	//friend ostream& operator <<(ostream& sortie, Point<U,Di> Pt);

	template<typename U, size_t Di>
	friend istream& operator >>(istream& entree, Point<U,Di>& Pt);
	
	//T& operator=(const Point<T,D>& Pt){for (size_t i=0; i<x.size(); i++){x[i]=Pt.x[i];};
	Point(){}
	
	Point(const Point<T,D>& Pt){for(size_t i=0; i<D; i++) x[i]=Pt.x[i];}
	
	Point<T,D>& operator=(const Point<T,D>& Pt){for(size_t i=0; i<D; i++) x[i]=Pt.x[i];
	return *this;}
	
	//template<typename U, size_t Di>
	//friend istream& operator <<(istream& entree, Point<U,Di> Pt);	
	T getX() const {return x[0];}
	T getY() const {return x[1];}
	T getZ() const {return x[2];}	
};


// Pour l'affichage des points
	template<typename T, size_t D>
	ostream& operator <<(ostream& sortie, const Point<T,D>& Pt){
	
	for(size_t i=0; i<D-1; i++){
	sortie<<Pt.x[i]<<" ";}
	sortie<<Pt.x[D-1];
	return sortie;}

// Pour la lecture des points

	template<typename T, size_t D>
	istream& operator >>(istream& entree, Point<T,D>& Pt){
	
	for(size_t i=0; i<D; i++){
	entree >> Pt.x[i];}
	return entree;}
	

template<typename T, size_t D>
class Coordinates : public vector<Point<T,D>>{

protected:
size_t NPts, dim;
vector<Point<T,D> > coo;

public:
	Coordinates(){}
	
	Coordinates(const Coordinates<T,D>& co){NPts=co.NPts; dim=co.dim;
	coo.resize(NPts);
	for(size_t i=0; i<NPts; i++)
	coo[i]=co.getPoint(i);}
	
	template<typename U, size_t Di>
	friend ostream& operator <<(ostream& sortie, const Coordinates<U,Di>& Coo);
	template<typename U, size_t Di>
	friend istream& operator >>(istream& sortie, Coordinates<U,Di>& Coo);
	
	void affiche();
	void lire();
	const int getNPts(){return NPts;}
	const Point<T,D>& getPoint(size_t i) const {return coo[i];}

};

template<typename T, size_t D>
ostream& operator <<(ostream& sortie, const Coordinates<T,D>& Coo){
for (int i=0; i<Coo.size()-1; i++){sortie<<i+1<<" "<<Coo[i]<<"\n";}
sortie<<Coo.size()<<" "<<Coo[Coo.size()-1];
return sortie;
}

template<typename T, size_t D>
istream& operator >>(istream& entree, Coordinates<T,D>& Coo){
int NumPoint; int skipdata;
entree>> Coo.dim >> Coo.NPts >>skipdata>>skipdata;
Coo.resize(Coo.NPts);
for (int i=0; i<Coo.size(); i++){entree>>NumPoint>>Coo[i];}
return entree;
}

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}


void CreateWorkFile(string WorkFile){

/* 
The purpose of this function is in long term to create a templated function that can tranform geometric data file from different software like Abaqus, Marc MSC to data format usable by the software in creation

Right now, it is just usable with Marc MSC
*/
string GivenFile(WorkFile.begin()+4, WorkFile.end());
ofstream sortie(WorkFile, ios::out);
ifstream entree(GivenFile, ios::in);

int i, NumLigne=-1, NbreLignes, dim, skipdata1, skipdata2;
string ligne;
int NumPoint; double x, y, z;
int taille;

do{
NumLigne++;
//char* ligne;
//entree.getline(ligne, 70);
getline(entree, ligne, '\n');
taille=ligne.size();
i=ligne.find("coordinates");
//if (i<0 || i>taille){continue;} else exit;
}while(!entree.eof() && ((i<0 || i>70))); //(Au lieu de i>70, mettre i>11( longueur du mot coordinates) )

entree>>dim>>NbreLignes>>skipdata1>>skipdata2;

cout<<"dim : "<<dim<<endl;
cout<<"Number of vertices : "<<NbreLignes<<endl;

sortie<<dim<<" "<<NbreLignes<<" "<<skipdata1<<" "<<skipdata2<<"\n";
getline(entree, ligne, '\n');
for (int i=0; i<NbreLignes; i++){
getline(entree, ligne, '\n');
//cout<<entree.gcount();

istringstream InputLine(ligne);
string InputLineX, InputLineY, InputLineZ;
//InputLine>>setw(10)>>NumPoint>>setw(20)>>InputLineX>>setw(20)>>InputLineY>>setw(20)>>InputLineZ;
InputLine>>NumPoint;
InputLineX=ligne.substr(10,20);
InputLineY=ligne.substr(30,20);
InputLineZ=ligne.substr(50,20);

if(InputLineX.find('-',1)>0 && InputLineX.find('-',1)<InputLineX.size()) InputLineX.insert((InputLineX.begin()+InputLineX.find('-',1)),'e');
if(InputLineX.find('+',1)>0 && InputLineX.find('+',1) <InputLineX.size()) InputLineX.insert((InputLineX.begin()+InputLineX.find('+',1)),'e');

if(InputLineY.find('-',1)>0 && InputLineY.find('-',1)< InputLineY.size()) InputLineY.insert((InputLineY.begin()+InputLineY.find('-',1)),'e');
if(InputLineY.find('+',1)>0 && InputLineY.find('+',1)< InputLineY.size()) InputLineY.insert((InputLineY.begin()+InputLineY.find('+',1)),'e');

if(InputLineZ.find('-',1)>0 && InputLineZ.find('-',1)<InputLineZ.size()) InputLineZ.insert((InputLineZ.begin()+InputLineZ.find('-',1)),'e');
if(InputLineZ.find('+',1)>0 && InputLineZ.find('+',1)<InputLineZ.size()) InputLineZ.insert((InputLineZ.begin()+InputLineZ.find('+',1)),'e');

ligne=InputLineX + ' ' + InputLineY + ' ' + InputLineZ;

istringstream InputLineXYZ(ligne);

InputLineXYZ>>x>>y>>z;
//sortie<<setw(10)<<NumPoint<<" "<<scientific<<setw(21)<<x<<" "<<setw(22)<<y<<" "<<setw(22)<<z<<"\n";
sortie<<NumPoint<<" "<<scientific<<x<<" "<<y<<" "<<z<<"\n";
}
sortie.close();
entree.close();
}


void PointSelect(const vector<Point<double, 3>>& Input, string StructInf, bitset<6>bs, double epsi, vector<size_t>& Output){

/*
StuctInf="Xmin Xmax Ymin Ymax Zmin Zmax"
*/


double Xmin, Xmax, Ymin, Ymax, Zmin, Zmax;
double PtX, PtY, PtZ;
size_t InputSize=Input.size();
istringstream ss(StructInf);
ss>>Xmin>>Xmax>>Ymin>>Ymax>>Zmin>>Zmax;

Point<double,3> Pt;
bitset<6>bi;
string bPtString, bsString;

for(size_t i=0; i<InputSize; i++){
PtX=Input[i].getX();
PtY=Input[i].getY();
PtZ=Input[i].getZ();
bool bxmin, bxmax, bymin, bymax, bzmin, bzmax;

bxmin=(fabs(PtX-Xmin)<=epsi);
bxmax=(fabs(PtX-Xmax)<=epsi);
bymin=(fabs(PtY-Ymin)<=epsi);
bymax=(fabs(PtY-Ymax)<=epsi);
bzmin=(fabs(PtZ-Zmin)<=epsi);
bzmax=(fabs(PtZ-Zmax)<=epsi);

bPtString=to_string(bxmin)+to_string(bxmax)+to_string(bymin)+to_string(bymax)+to_string(bzmin)+to_string(bzmax);

bsString=bs.to_string();

//bi=bPtString;
if(bPtString==bsString) Output.push_back(i);

}

}




void CoorL(const vector<Point<double,3> >& coo, const vector<size_t>& Sommets, int ind, const vector<size_t>& v1, const vector<size_t>& v2, double epsi, vector<array<size_t,3> >& sortie){
// le paramètre indicator indique si les arrêtes sont orientés suivant x (ind=0), y(ind=1), z(ind=2)
// The function assumes that vectA, vectB, vectC, vectD, vectAp, vectBp, etc ... have been prior declared
array<size_t,3>PointEqui;
size_t tn, rt1, rt2;
size_t sizev1=v1.size(), sizev2=v2.size();

assert(sizev1==sizev2);
assert(ind==0 || ind==1 || ind==2);
double coordAv1, coordBv1, coordAv2, coordBv2;
double condA, condB;
for (size_t i=0; i<sizev1; i++){
/*if(v1[i]==Sommets[0] || v1[i]==Sommets[1] || v1[i]==Sommets[2] || v1[i]==Sommets[3] || v1[i]==Sommets[4] || v1[i]==Sommets[5] || v1[i]==Sommets[6] || v1[i]==Sommets[7]) continue; */ // To skip angle vertices
	for (size_t j=0; j<sizev1; j++){
		{
			if(ind==0){
			coordAv1=coo[ v1[i] ].getX();
			coordAv2=coo[ v2[j] ].getX();
			coordBv1=coo[ v1[i] ].getZ();
			coordBv2=coo[ v2[j] ].getZ();}
			else if(ind==1){
			coordAv1=coo[ v1[i] ].getY();
			coordAv2=coo[ v2[j] ].getY();
			coordBv1=coo[ v1[i] ].getZ();
			coordBv2=coo[ v2[j] ].getZ();}
			else if(ind==2){
			coordAv1=coo[ v1[i] ].getZ();
			coordAv2=coo[ v2[j] ].getZ();
			
			coordBv1=coo[ v1[i] ].getY();
			coordBv2=coo[ v2[j] ].getY();}
			
		condA=coordAv1-coordAv2;
		
		}
		if(condA<=epsi){
		tn=v1[i];
		rt1=v2[j];
		condB=coordBv1-coordBv2;
			if(condB<=epsi){
				if(ind==0) {rt2=Sommets[3];/*vectD[0] ;*/} else if (ind==1){rt2=Sommets[1];/*vectB[0] ;*/}else if(ind==2){rt2=Sommets[1];/* vectB[0];*/}
			}
			else 
		 	{
			if(ind==0) {rt2=Sommets[4];/*vectAp[0] ;*/} else if (ind==1){rt2=Sommets[4];/*vectAp[0] ;*/}else if(ind==2){rt2=Sommets[3];/*vectD[0] ;*/}
			}
		}
		}
PointEqui[0]=tn;
PointEqui[1]=rt1;
PointEqui[2]=rt2;
sortie.push_back(PointEqui);
}

}


void CoorF(const vector<Point<double,3> >& coo, const vector<size_t>& Sommets, int ind, const vector<size_t>& v1, const vector<size_t>& v2, double epsi, vector<array<size_t,3> >& sortie){

// le paramètre indicator indique si les arrêtes sont orientés suivant x (ind=0), y(ind=1), z(ind=2)
array<size_t,3>PointEqui;
size_t tn, rt1, rt2;
size_t sizev1=v1.size(), sizev2=v2.size();

assert(sizev1==sizev2);
assert(ind==0 || ind==1 || ind==2);
double coordAv1, coordBv1, coordAv2, coordBv2;
double condA, condB;
for (size_t i=0; i<sizev1; i++){
/*if(v1[i]==Sommets[0] || v1[i]==Sommets[1] || v1[i]==Sommets[2] || v1[i]==Sommets[3] || v1[i]==Sommets[4] || v1[i]==Sommets[5] || v1[i]==Sommets[6] || v1[i]==Sommets[7]) continue;*/ // To skip angle vertices
	for (size_t j=0; j<sizev1; j++){
	

			if(ind==0){
			coordAv1=coo[ v1[i] ].getY();
			coordAv2=coo[ v2[j] ].getY();	
			coordBv1=coo[ v1[i] ].getZ();
			coordBv2=coo[ v2[j] ].getZ();}
			else if(ind==1){
			coordAv1=coo[ v1[i] ].getX();
			coordAv2=coo[ v2[j] ].getX();	
			coordBv1=coo[ v1[i] ].getZ();
			coordBv2=coo[ v2[j] ].getZ();}			
			else if(ind==2){
			coordAv1=coo[ v1[i] ].getX();
			coordAv2=coo[ v2[j] ].getX();	
			coordBv1=coo[ v1[i] ].getY();
			coordBv2=coo[ v2[j] ].getY();}			
		
			
		condA=coordAv1-coordAv2;
		condB=coordBv1-coordBv2;
		if(condA<=epsi && condB<=epsi){
		tn=v1[i];
		rt1=v2[j];
		if(ind==0){rt2=Sommets[1];} else if(ind==1){Sommets[3];} else if(ind==2){Sommets[4];}
	
		}
	}
	PointEqui[0]=tn;
	PointEqui[1]=rt1;
	PointEqui[2]=rt2;
	sortie.push_back(PointEqui);
}
}	
	


int main(int argc, char** argv)
{

size_t NPts;
/* Generation of the working file */


string filename;
//cout<<"Donner le nom de fichier"<<endl;
//cin>>filename;
filename=argv[1];
cout<<"\e[0;31mNom du fichier traité \e[0;m:"<<filename<<endl;
string WorkFile="Work"+filename;
CreateWorkFile(WorkFile);

/* Generation of the retained node */

Coordinates<double, 3> coo;
ifstream WorkFileStream(WorkFile, ios::in);
WorkFileStream>>coo;
WorkFileStream.close();
//cout<<coo;

NPts=coo.getNPts();
vector<double> X, Y, Z;
//Point<double, 3> Temp;

/*     Détermination de la résolution de la géométrie */
for (size_t i=0; i<NPts; i++){X.push_back(coo[i].getX());}
for (size_t i=0; i<NPts; i++){Y.push_back(coo[i].getY());}
for (size_t i=0; i<NPts; i++){Z.push_back(coo[i].getZ());}


double Xmin, Xmax, Ymin, Ymax, Zmin, Zmax;
vector<double>XSort=X;
sort(XSort.begin(), XSort.end());
Xmin=XSort[0];
Xmax=XSort[X.size()-1];

vector<double>YSort=Y;
sort(YSort.begin(), YSort.end());
Ymin=YSort[0];
Ymax=YSort[Y.size()-1];

vector<double>ZSort=Z;
sort(ZSort.begin(), ZSort.end());
Zmin=ZSort[0];
Zmax=ZSort[Z.size()-1];

cout<<"L'ensemble des points est contenu dans le volume "<<"["<<Xmin<<", "<<Xmax<<"]x["<<Ymin<<", "<<Ymax<<"]x["<<Zmin<<", "<<Zmax<<"]"<<endl;

Point<double, 3> A, B, C, D, Ap, Bp, Cp, Dp;
string PointA=to_string(Xmin)+" "+to_string(Ymin)+" "+to_string(Zmin);
string PointB=to_string(Xmax)+" "+to_string(Ymin)+" "+to_string(Zmin);
string PointC=to_string(Xmax)+" "+to_string(Ymax)+" "+to_string(Zmin);
string PointD=to_string(Xmin)+" "+to_string(Ymax)+" "+to_string(Zmin);
string PointAp=to_string(Xmin)+" "+to_string(Ymin)+" "+to_string(Zmax);
string PointBp=to_string(Xmax)+" "+to_string(Ymin)+" "+to_string(Zmax);
string PointCp=to_string(Xmax)+" "+to_string(Ymax)+" "+to_string(Zmax);
string PointDp=to_string(Xmin)+" "+to_string(Ymax)+" "+to_string(Zmax);
//cout<<endl<<PointDp<<endl;

/* Définition d'une fonction qui trie les points en respectant un champ de bits données (XminXmaxYminYmaxZminZmax)*/

double epsi=0.0;
string StructInf=to_string(Xmin)+" "+to_string(Xmax)+" "+to_string(Ymin)+" "+to_string(Ymax)+" "+to_string(Zmin)+" "+to_string(Zmax);


/* Vérification si les points A, B, C, D, E, F, Ap, Bp, Cp, Dp font partir du maillage et le cas échéant donner leurs numéros. Dans la pratique, on ne vérifiera que l'existence du point A vu que ce code assume qu'on est en présence d'un maillage symétrique*/

vector<size_t> vectA, vectB, vectC, vectD, vectAp, vectBp, vectCp, vectDp;
bitset<6> bA("101010"), bB("011010"), bC("010110"), bD("100110"), bAp("101001"), bBp("011001"), bCp("010101"), bDp("100101");
PointSelect(coo, StructInf, bA, epsi, vectA);
//PointSelect(coo, StructInf, bB, epsi, vectB);

bool CornersVertices=(vectA.size() == 1); // This variable is used to generate the pbcfile. 
if(vectA.size()==1){

PointSelect(coo, StructInf, bB, epsi, vectB);
PointSelect(coo, StructInf, bC, epsi, vectC);
PointSelect(coo, StructInf, bD, epsi, vectD);
PointSelect(coo, StructInf, bAp, epsi, vectAp);
PointSelect(coo, StructInf, bBp, epsi, vectBp);
PointSelect(coo, StructInf, bCp, epsi, vectCp);
PointSelect(coo, StructInf, bDp, epsi, vectDp);

}else {
vectA.push_back(coo.size()-1+1);
vectB.push_back(coo.size()-1+2);
vectC.push_back(coo.size()-1+3);
vectD.push_back(coo.size()-1+4);
vectAp.push_back(coo.size()-1+5);
vectBp.push_back(coo.size()-1+6);
vectCp.push_back(coo.size()-1+7);
vectDp.push_back(coo.size()-1+8);
}

vector<size_t> Sommets, SommetsSort;//, SommetsSort contient les numéros ordonnés dans l'ordre croissant tandis que Sommets contient les numéros de sommets dans la séquence A, B, C, D, Ap, Bp, Cp, Dp
Sommets.push_back(vectA[0]); Sommets.push_back(vectB[0]); Sommets.push_back(vectC[0]); Sommets.push_back(vectD[0]);
Sommets.push_back(vectAp[0]); Sommets.push_back(vectBp[0]); Sommets.push_back(vectCp[0]); Sommets.push_back(vectDp[0]);

SommetsSort.push_back(vectA[0]); SommetsSort.push_back(vectB[0]); SommetsSort.push_back(vectC[0]); SommetsSort.push_back(vectD[0]);
SommetsSort.push_back(vectAp[0]); SommetsSort.push_back(vectBp[0]); SommetsSort.push_back(vectCp[0]); SommetsSort.push_back(vectDp[0]);
sort(SommetsSort.begin(), SommetsSort.end());
//for (auto i : SommetsSort) cout<<i<<" ";


vector<size_t> CoordinatesSansCoins;
for(size_t i=0; i<NPts; i++) CoordinatesSansCoins.push_back(i);
CoordinatesSansCoins.erase(CoordinatesSansCoins.begin()+ SommetsSort[7]);
CoordinatesSansCoins.erase(CoordinatesSansCoins.begin()+ SommetsSort[6]);
CoordinatesSansCoins.erase(CoordinatesSansCoins.begin()+ SommetsSort[5]);
CoordinatesSansCoins.erase(CoordinatesSansCoins.begin()+ SommetsSort[4]);
CoordinatesSansCoins.erase(CoordinatesSansCoins.begin()+ SommetsSort[3]);
CoordinatesSansCoins.erase(CoordinatesSansCoins.begin()+ SommetsSort[2]);
CoordinatesSansCoins.erase(CoordinatesSansCoins.begin()+ SommetsSort[1]);
CoordinatesSansCoins.erase(CoordinatesSansCoins.begin()+ SommetsSort[0]);


// Selection of edge nodes, Il faut remarquer ici que de part la définition de PointSelect, les sommets ne sont pas sélectionnés
vector<size_t> YmaxZmin, YminZmin, YmaxZmax, YminZmax, XmaxZmin, XminZmin, XmaxZmax, XminZmax, XmaxYmin, XminYmin, XmaxYmax, XminYmax;

bitset<6>bYmaxZmin("000110"), bYminZmin("001010"), bYmaxZmax("000101"), bYminZmax("001001"), bXmaxZmin("010010"), bXminZmin("100010"), bXmaxZmax("010001"), bXminZmax("100001"), bXmaxYmin("011000"), bXminYmin("101000"), bXmaxYmax("010100"), bXminYmax("100100");

PointSelect(coo, StructInf, bYmaxZmin, epsi, YmaxZmin);
PointSelect(coo, StructInf, bYminZmin, epsi, YminZmin);
PointSelect(coo, StructInf, bYmaxZmax, epsi, YmaxZmax);
PointSelect(coo, StructInf, bYminZmax, epsi, YminZmax);

PointSelect(coo, StructInf, bXmaxZmin, epsi, XmaxZmin);
PointSelect(coo, StructInf, bXminZmin, epsi, XminZmin);
PointSelect(coo, StructInf, bXmaxZmax, epsi, XmaxZmax);
PointSelect(coo, StructInf, bXminZmax, epsi, XminZmax);

PointSelect(coo, StructInf, bXmaxYmin, epsi, XmaxYmin);
PointSelect(coo, StructInf, bXminYmin, epsi, XminYmin);
PointSelect(coo, StructInf, bXmaxYmax, epsi, XmaxYmax);
PointSelect(coo, StructInf, bXminYmax, epsi, XminYmax);


//Selection of face nodes, Il faut remarquer ici que de part la définition de PointSelect, les sommets ne sont pas sélectionnés

vector<size_t> XminF, XmaxF, YminF, YmaxF, ZminF, ZmaxF;

bitset<6> bXminF("100000"), bXmaxF("010000"), bYminF("001000"), bYmaxF("000100"), bZminF("000010"), bZmaxF("000001");

PointSelect(coo, StructInf, bXminF, epsi, XminF);
PointSelect(coo, StructInf, bXmaxF, epsi, XmaxF);
PointSelect(coo, StructInf, bYminF, epsi, YminF);
PointSelect(coo, StructInf, bYmaxF, epsi, YmaxF);
PointSelect(coo, StructInf, bZminF, epsi, ZminF);
PointSelect(coo, StructInf, bZmaxF, epsi, ZmaxF);

cout<<"Vérification sommaire de la symétrie ..."<<endl;
if( XminF.size()==XmaxF.size() && YminF.size()==YmaxF.size() && ZminF.size()==ZmaxF.size() && XminZmin.size()==YminZmin.size() && YmaxZmax.size()==YminZmax.size() && XmaxZmin.size()==XminZmin.size() && XmaxZmax.size()==XminZmax.size() && XmaxYmin.size()==XminYmin.size() && XmaxYmax.size()==XminYmax.size()) {cout<<"Maillage probablement symétrique"<<endl;}else{abort;}
//cout<<endl<<ZmaxF.size()<<endl;

size_t NbLinks;
NbLinks=3*( 4+3*( YminZmin.size()+XminZmin.size()+XminYmin.size() )+ XminF.size()+YminF.size()+ZminF.size() );
cout<<"\e[0;44mNombre de servo link : \e[0;m"<<NbLinks<<endl;

//CoorL(const vector<Point<double,3> > coo, vector<size_t>Sommets, int ind, const vector<size_t>& v1, const vector<size_t>& v2, double epsi, vector<array<size_t,3> >& sortie);

array<size_t,3>Sommets1, Sommets2, Sommets3, Sommets4;

Sommets1={vectC[0], vectD[0], vectB[0]};
Sommets2={vectCp[0], vectDp[0], vectB[0]};
Sommets3={vectBp[0], vectAp[0], vectB[0]};
Sommets4={vectDp[0], vectAp[0], vectD[0]};
vector<array<size_t,3> >ConnectivitySommets;
ConnectivitySommets.push_back(Sommets1);
ConnectivitySommets.push_back(Sommets2);
ConnectivitySommets.push_back(Sommets3);
ConnectivitySommets.push_back(Sommets4);


vector<array<size_t,3> > Edge1, Edge2, Edge3, Edge4, Edge5, Edge6, Edge7, Edge8, Edge9;

CoorL(coo, Sommets, 0, YmaxZmin, YminZmin, epsi, Edge1);
CoorL(coo, Sommets, 0, YmaxZmax, YminZmax, epsi, Edge2);
CoorL(coo, Sommets, 0, YminZmax, YminZmin, epsi, Edge3);

CoorL(coo, Sommets, 1, XmaxZmin, XminZmin, epsi, Edge4);
CoorL(coo, Sommets, 1, XmaxZmax, XminZmax, epsi, Edge5);
CoorL(coo, Sommets, 1, XminZmax, XminZmin, epsi, Edge6);

CoorL(coo, Sommets, 2, XmaxYmin, XminYmin, epsi, Edge7);
CoorL(coo, Sommets, 2, XmaxYmax, XminYmax, epsi, Edge8);
CoorL(coo, Sommets, 2, XminYmax, XminYmin, epsi, Edge9);

vector<array<size_t,3> > FacesX, FacesY, FacesZ;

CoorF(coo, Sommets, 0, XmaxF, XminF, epsi, FacesX);
CoorF(coo, Sommets, 1, YmaxF, YminF, epsi, FacesY);
CoorF(coo, Sommets, 2, ZmaxF, ZminF, epsi, FacesZ);

vector<array<size_t,3> > Connectivity(ConnectivitySommets);
for (auto p : Edge1) Connectivity.push_back(p);
for (auto p : Edge2) Connectivity.push_back(p);
for (auto p : Edge3) Connectivity.push_back(p);
for (auto p : Edge4) Connectivity.push_back(p);
for (auto p : Edge5) Connectivity.push_back(p);
for (auto p : Edge6) Connectivity.push_back(p);
for (auto p : Edge7) Connectivity.push_back(p);
for (auto p : Edge8) Connectivity.push_back(p);
for (auto p : Edge9) Connectivity.push_back(p);
for (auto p : FacesX) Connectivity.push_back(p);
for (auto p : FacesY) Connectivity.push_back(p);
for (auto p : FacesZ) Connectivity.push_back(p);

/*Creating the pbc*.dat file 
If there is no vertices in the corners, the created corner vertices are incorporated in the pbc*.dat just before the "servolink word" as the servolinks are put just after the coordinates
*/

//string pbcfile="pbc"+filename;
string pbcTractionX("pbcTractionX.dat"), pbcTractionY("pbcTractionY.dat"), pbcTractionZ("pbcTractionZ.dat"), pbcCisXY("pbcCisXY.dat"), pbcCisXZ("pbcCisXZ.dat"), pbcCisYZ("pbcCisYZ.dat");
string StringCoins="";
if(CornersVertices){;
} else{
ostringstream Coins;
Coins<<to_string(vectA[0]+1)<<", "<<coo[ vectA[0] ].getX()<<", "<<coo[ vectA[0] ].getY()<<", "<<coo[ vectA[0] ].getZ()<<"\n"
     <<to_string(vectB[0]+1)<<", "<<coo[ vectB[0] ].getX()<<", "<<coo[ vectB[0] ].getY()<<", "<<coo[ vectB[0] ].getZ()<<"\n"
     <<to_string(vectC[0]+1)<<", "<<coo[ vectC[0] ].getX()<<", "<<coo[ vectC[0] ].getY()<<", "<<coo[ vectC[0] ].getZ()<<"\n"
     <<to_string(vectD[0]+1)<<", "<<coo[ vectD[0] ].getX()<<", "<<coo[ vectD[0] ].getY()<<", "<<coo[ vectD[0] ].getZ()<<"\n"
     <<to_string(vectAp[0]+1)<<", "<<coo[ vectAp[0] ].getX()<<", "<<coo[ vectAp[0] ].getY()<<", "<<coo[ vectAp[0] ].getZ()<<"\n"
     <<to_string(vectBp[0]+1)<<", "<<coo[ vectBp[0] ].getX()<<", "<<coo[ vectBp[0] ].getY()<<", "<<coo[ vectBp[0] ].getZ()<<"\n"
     <<to_string(vectCp[0]+1)<<", "<<coo[ vectCp[0] ].getX()<<", "<<coo[ vectCp[0] ].getY()<<", "<<coo[ vectCp[0] ].getZ()<<"\n"
     <<to_string(vectDp[0]+1)<<", "<<coo[ vectDp[0] ].getX()<<", "<<coo[ vectDp[0] ].getY()<<", "<<coo[ vectDp[0] ].getZ();
     
StringCoins=Coins.str();     
;}



//size_t CoordLine=NumLigne+1;
string SortieLine;


int i, NumLigne=-1, NbreLignes, dim, skipdata1, skipdata2;
//string ligne;
int NumPoint;

//ofstream pbcSortie(pbcfile, ios::out);
ofstream pbcSortieTractionX(pbcTractionX, ios::out);
ofstream pbcSortieTractionY(pbcTractionY, ios::out);
ofstream pbcSortieTractionZ(pbcTractionZ, ios::out);
ofstream pbcSortieCisXY(pbcCisXY, ios::out);
ofstream pbcSortieCisXZ(pbcCisXZ, ios::out);
ofstream pbcSortieCisYZ(pbcCisYZ, ios::out);
ifstream InputFile(filename, ios::in);

do{
NumLigne++;
getline(InputFile, SortieLine, '\n');
i=SortieLine.find("coordinates");
pbcSortieTractionX<<SortieLine<<endl;
pbcSortieTractionY<<SortieLine<<endl;
pbcSortieTractionZ<<SortieLine<<endl;
pbcSortieCisXY<<SortieLine<<endl;
pbcSortieCisXZ<<SortieLine<<endl;
pbcSortieCisYZ<<SortieLine<<endl;
}while(!InputFile.eof() && ((i<0 || i>11)));

getline(InputFile, SortieLine, '\n');
pbcSortieTractionX<<SortieLine<<endl;
pbcSortieTractionY<<SortieLine<<endl;
pbcSortieTractionZ<<SortieLine<<endl;
pbcSortieCisXY<<SortieLine<<endl;
pbcSortieCisXZ<<SortieLine<<endl;
pbcSortieCisYZ<<SortieLine<<endl;

istringstream DimNbreLignes(SortieLine);
DimNbreLignes>>dim>>NbreLignes>>skipdata1>>skipdata2;


//Reading coordinates
for(size_t i=0; i<NbreLignes; i++){
getline(InputFile, SortieLine, '\n');
pbcSortieTractionX<<SortieLine<<endl;
pbcSortieTractionY<<SortieLine<<endl;
pbcSortieTractionZ<<SortieLine<<endl;
pbcSortieCisXY<<SortieLine<<endl;
pbcSortieCisXZ<<SortieLine<<endl;
pbcSortieCisYZ<<SortieLine<<endl;
}

pbcSortieTractionX<<StringCoins;
pbcSortieTractionY<<StringCoins;
pbcSortieTractionZ<<StringCoins;
pbcSortieCisXY<<StringCoins;
pbcSortieCisXZ<<StringCoins;
pbcSortieCisYZ<<StringCoins;

pbcSortieTractionX<<"servo link, "<<NbLinks<<", 0, 1\n";
pbcSortieTractionY<<"servo link, "<<NbLinks<<", 0, 1\n";
pbcSortieTractionZ<<"servo link, "<<NbLinks<<", 0, 1\n";
pbcSortieCisXY<<"servo link, "<<NbLinks<<", 0, 1\n";
pbcSortieCisXZ<<"servo link, "<<NbLinks<<", 0, 1\n";
pbcSortieCisYZ<<"servo link, "<<NbLinks<<", 0, 1\n";

ostringstream ServoLinkStream;
for(size_t i=1; i<=3; i++){ // Pour les trois dimensions
	for(size_t j=0; j<Connectivity.size(); j++){
	ServoLinkStream<<"2, "<<to_string(i)<<", "<<Connectivity[j][0]<<", "<<to_string(i)<<", "<<Connectivity[j][1]<<", "<<to_string(i)<<", "<<Connectivity[j][2]<<"\n1, 1\n0\n";
	}
}

string ServoLinkString=ServoLinkStream.str();
pbcSortieTractionX<<ServoLinkString;
pbcSortieTractionY<<ServoLinkString;
pbcSortieTractionZ<<ServoLinkString;
pbcSortieCisXY<<ServoLinkString;
pbcSortieCisXZ<<ServoLinkString;
pbcSortieCisYZ<<ServoLinkString;

/*

do{
getline(InputFile, SortieLine, '\n');
pbcSortie<<SortieLine<<endl;
}while(!InputFile.eof());
*/



/* This section is used to put boundary conditions, Materials and for output control*/


string MaterialFile("material.dat");
string MaterialLine("");
string Material;

ifstream MaterialStream(MaterialFile, ios::in);
do{
getline(MaterialStream, MaterialLine, '\n');
Material += MaterialLine + "\n";
}while(!MaterialStream.eof());


ostringstream ModelConditions;

ModelConditions<<Material;

//Nodes results to print
ModelConditions<<"PRINT NODE\n,1,,\nTOTA, REAC\n"<<vectB[0]<<","<<vectD[0]<<","<<vectAp[0]<<endl;

// Sets definition, these nodes are used to put the necessary boundary conditions
ModelConditions<<"define, node, set, nA\n"<<vectA[0]<<endl
	       <<"define, node, set, nBx\n"<<vectB[0]<<endl
	       <<"define, node, set, nDy\n"<<vectD[0]<<endl
	       <<"define, node, set, nApz\n"<<vectAp[0]<<endl;
	       
string RampeTable = "table               Rampe\n         1         1         0         0         2\n         1         2         2         0         0         2         0         0         2\n         0         0         2\n 0.000000000000000+0 0.000000000000000+0\n 1.000000000000000+0 1.000000000000000+0\nfixed disp";
 
 ModelConditions<<endl<<RampeTable<<endl;

string BoundaryConditionsTractionX="./Boundary/TractionX.dat";
string BoundaryConditionsTractionY="./Boundary/TractionY.dat";
string BoundaryConditionsTractionZ="./Boundary/TractionZ.dat";
string BoundaryConditionsCisXY="./Boundary/CisXY.dat";
string BoundaryConditionsCisXZ="./Boundary/CisXZ.dat";
string BoundaryConditionsCisYZ="./Boundary/CisYZ.dat";
 
ifstream Xstream(BoundaryConditionsTractionX, ios::in);
ifstream Ystream(BoundaryConditionsTractionY, ios::in);
ifstream Zstream(BoundaryConditionsTractionZ, ios::in);
ifstream XYstream(BoundaryConditionsCisXY, ios::in);
ifstream XZstream(BoundaryConditionsCisXZ, ios::in);
ifstream YZstream(BoundaryConditionsCisYZ, ios::in);

pbcSortieTractionX<<ModelConditions.str();
pbcSortieTractionY<<ModelConditions.str();
pbcSortieTractionZ<<ModelConditions.str();
pbcSortieCisXY<<ModelConditions.str();
pbcSortieCisXZ<<ModelConditions.str();
pbcSortieCisYZ<<ModelConditions.str();


string BoundLine;

do{
getline(Xstream, BoundLine, '\n');
pbcSortieTractionX<<BoundLine<<endl;
}while(! Xstream.eof());

do{
getline(Ystream, BoundLine, '\n');
pbcSortieTractionY<<BoundLine<<endl;
}while(! Ystream.eof());

do{
getline(Zstream, BoundLine, '\n');
pbcSortieTractionZ<<BoundLine<<endl;
}while(! Zstream.eof());

do{
getline(XYstream, BoundLine, '\n');
pbcSortieCisXY<<BoundLine<<endl;
}while(! XYstream.eof());

do{
getline(XZstream, BoundLine, '\n');
pbcSortieCisXZ<<BoundLine<<endl;
}while(! XZstream.eof());

do{
getline(YZstream, BoundLine, '\n');
pbcSortieCisYZ<<BoundLine<<endl;
}while(! YZstream.eof());
 

pbcSortieTractionX.close();
pbcSortieTractionY.close();
pbcSortieTractionZ.close();
pbcSortieCisXY.close();
pbcSortieCisXZ.close();
pbcSortieCisYZ.close();
InputFile.close();

ofstream BDAp("NodesBDAp.dat", ios::out);
BDAp<<vectB[0]<<" "<<vectD[0]<<" "<<vectAp[0];

BDAp.close();

	return 0;
}
