#!/bin/bash

export LC_ALL=C

# Pour Importer les noeuds du fichier .dat
#filename='pbc.dat'
#condfichier=$(echo "" |bc -l)
if [ "$#" -eq 0 ];
then
echo -e "\e[0;31mEntrer le fichier de travail .dat!\e[0;m"
exit
fi
filename="$1"
CoordLine=$(grep -nE "coordinates" $filename | awk -F ":" '{print $1}')
temp=$(echo "$CoordLine +1" |bc -l)
NbreNoeuds=$(awk -v nb="$temp" 'NR==nb{print $2}' $filename)
Inf=$(echo "$CoordLine+2" |bc -l)
Sup=$(echo "$Inf+$NbreNoeuds-1" |bc -l)
awk -v Inf=$Inf -v Sup=$Sup 'NR>=Inf && NR<=Sup{print $0}' $filename > coordinatesTemp.txt
#sed -e "s/^ \+//g" coordinatesTemp.txt > coordinatesTempTemp.txt
#sed -e "s/\(-.-\)/\1transi/g"  coordinatesTempTemp.txt| sed -e "s/\(-..-\)/\1transi/g" |sed -e "s/\(+.+\)/\1transi/g" |#sed -e "s/\(+..+\)/\1transi/g" | sed -e 's/-transi/ /g' | sed -e 's/+transi/ /g' >coordinates.txt

cut -c1-10 "coordinatesTemp.txt"> col1.txt
cut -c11-30 "coordinatesTemp.txt"> col2.txt
cut -c31-50 "coordinatesTemp.txt"> col3.txt
cut -c51-70 "coordinatesTemp.txt"> col4.txt
paste -d " " col1.txt col2.txt col3.txt col4.txt > coordinates1.txt
 sed -e "s/^ \+//g" coordinates1.txt>coordinates.txt

#Pour sortir les retained nodes (coins)
: > tmp.txt
sed -e "s/\([^ ]+\)\|\(^+\)\|\($+\)/e+/g" coordinates.txt |sed -e "s/\([^ ]-\)\|\(^-\)\|\($-\)/e-/g" >>tmp.txt

cat tmp.txt > coordinates.txt
rm tmp.txt
#nCoords=$(cat "coordinates.txt" |wc -l)
# Ceci pour numéroter les sommets. nCoords désigne le plus grand numéro de sommet
nCoords=$(sort -gr +1 coordinates.txt | awk 'NR==1{print $1}')

Xmin=$(sort -g +1 coordinates.txt | awk 'NR==1{print $2}')
#Xmax=$(sort -g +1 coordinates.txt | awk -v Sup=$CoordLine 'NR==Sup{print $2}')
Xmax=$(sort -gr +1 coordinates.txt | awk 'NR==1{print $2}')

Ymin=$(sort -g +2 coordinates.txt | awk 'NR==1{print $3}')
#Ymax=$(sort -g +2 coordinates.txt | awk -v Sup=$CoordLine 'NR==Sup{print $3}')
Ymax=$(sort -gr +2 coordinates.txt | awk 'NR==1{print $3}')

Zmin=$(sort -g +3 coordinates.txt | awk 'NR==1{print $4}')
#Zmax=$(sort -g +3 coordinates.txt | awk -v Sup=$CoordLine 'NR==Sup{print $4}')
Zmax=$(sort -gr +3 coordinates.txt | awk 'NR==1{print $4}')
:<<'end'
Xmin="-0.5"
Xmax="0.5"
Ymin="-0.5"
Ymax="0.5"
Zmin="-0.5"
Zmax="0.5"
end
                                 
 #-------------------------------------------------------   Construction des conditions de bord symétriques  --- ------------------------------------------------------------                                
: <<'end'                                                                   
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
                                       Transform QML Type

Retained nodes : A, B, D, A'
A(Xmin, Ymin, Zmin), B (Xmax, Ymin, Zmin), C(Xmax, Ymax, Zmin), D(Xmin, Ymax, Zmin), A'(Xmin, Ymin, Zmax), B'(Xmax, Ymin, Zmax), C'(Xmax, Ymax, Zmax), D'(Xmin, Ymax, Zmax)                                
end

#Vérification minimale de la symétrie du maillage

XminDonnees=$(awk -v Xmin=$Xmin '$2==Xmin{print $0}' coordinates.txt | wc -l)
XmaxDonnees=$(awk -v Xmax=$Xmax '$2==Xmax{print $0}' coordinates.txt | wc -l)

YminDonnees=$(awk -v Ymin=$Ymin '$3==Ymin{print $0}' coordinates.txt | wc -l)
YmaxDonnees=$(awk -v Ymax=$Ymax '$3==Ymax{print $0}' coordinates.txt | wc -l)

ZminDonnees=$(awk -v Zmin=$Zmin '$4==Zmin{print $0}' coordinates.txt | wc -l)
ZmaxDonnees=$(awk -v Zmax=$Zmax '$4==Zmax{print $0}' coordinates.txt | wc -l)

EdgeNumberX=$(awk -v Ymin=$Ymin -v Zmin=$Zmin '($3==Ymin && $4==Zmin){print $0}' coordinates.txt | wc -l)
EdgeNumberY=$(awk -v Xmin=$Xmin -v Zmin=$Zmin '($2==Xmin && $4==Zmin){print $0}' coordinates.txt | wc -l)
EdgeNumberZ=$(awk -v Xmin=$Xmin -v Ymin=$Ymin '($2==Xmin && $3==Ymin){print $0}' coordinates.txt | wc -l)

if (( ($XminDonnees==$XmaxDonnees) && ($YminDonnees==$YmaxDonnees) && ($ZminDonnees==$ZmaxDonnees) )); 
then 
echo -e "Vérification sommaire de la symétrie : \e[0;32mOK\e[0;m" 
else
echo "Maillage non symétrique"
read -p "Appuyez sur entrer pour sortir du programme"
exit
fi

#NbLinks=$(echo "3*(  4+3*(($EdgeNumberX-2)+($EdgeNumberY-2)+($EdgeNumberZ-2))+($EdgeNumberX-2)*($EdgeNumberY-2)+($EdgeNumberX-2)*($EdgeNumberZ-2)+($EdgeNumberY-2)*($EdgeNumberZ-2)  )" |bc -l)
#NbLinks=$(echo "3*(  4+3*(($EdgeNumberX-2)+($EdgeNumberY-2)+($EdgeNumberZ-2))+($XminDonnees-2*$EdgeNumberY-2*$EdgeNumberZ+4)+($YminDonnees-2*$EdgeNumberX-2*$EdgeNumberZ+4)+($ZminDonnees-2*$EdgeNumberY-2*$EdgeNumberX+4) )" |bc -l)
#echo -e "Nombre de servo link : \e[0;44m$NbLinks\e[0;m"
#: > servolink.txt
#echo "servo link, $NbLinks, 0, 1" > servolink.txt

# Corner nodes links
:<<'end'
awk -v Xmin=$Xmin -v Ymin=$Ymin -v Zmin=$Zmin '($2==Xmin && $3==Ymin && $4==Zmin){print $0}' coordinates.txt > corners.txt #Point A
awk -v Xmax=$Xmax -v Ymin=$Ymin -v Zmin=$Zmin '($2==Xmax && $3==Ymin && $4==Zmin){print $0}' coordinates.txt >> corners.txt #Point B
awk -v Xmax=$Xmax -v Ymax=$Ymax -v Zmin=$Zmin '($2==Xmax && $3==Ymax && $4==Zmin){print $0}' coordinates.txt >> corners.txt #Point C
awk -v Xmin=$Xmin -v Ymax=$Ymax -v Zmin=$Zmin '($2==Xmin && $3==Ymax && $4==Zmin){print $0}' coordinates.txt >> corners.txt #Point D

awk -v Xmin=$Xmin -v Ymin=$Ymin -v Zmax=$Zmax '($2==Xmin && $3==Ymin && $4==Zmax){print $0}' coordinates.txt >> corners.txt #Point A'
awk -v Xmax=$Xmax -v Ymin=$Ymin -v Zmax=$Zmax '($2==Xmax && $3==Ymin && $4==Zmax){print $0}' coordinates.txt >> corners.txt #Point B'
awk -v Xmax=$Xmax -v Ymax=$Ymax -v Zmax=$Zmax '($2==Xmax && $3==Ymax && $4==Zmax){print $0}' coordinates.txt >> corners.txt #Point C'
awk -v Xmin=$Xmin -v Ymax=$Ymax -v Zmax=$Zmax '($2==Xmin && $3==Ymax && $4==Zmax){print $0}' coordinates.txt >> corners.txt #Point D'
end

A=$(awk -v Xmin=$Xmin -v Ymin=$Ymin -v Zmin=$Zmin '($2==Xmin && $3==Ymin && $4==Zmin){print $0}' coordinates.txt) #Point A
if [[ -z "$A" ]];
then
nA=$(echo "$nCoords+1" |bc -l)
echo "$nA $Xmin $Ymin $Zmin" > corners.txt
echo "$nA $Xmin $Ymin $Zmin" >> coordinates.txt
echo "$nA, $Xmin, $Ymin, $Zmin"> servolink.txt
else
echo "$A" > corners.txt
fi

B=$(awk -v Xmax=$Xmax -v Ymin=$Ymin -v Zmin=$Zmin '($2==Xmax && $3==Ymin && $4==Zmin){print $0}' coordinates.txt) #Point B
if [[ -z "$B" ]];
then
nB=$(echo "$nCoords+2" |bc -l)
echo "$nB $Xmax $Ymin $Zmin" >> corners.txt
echo "$nB $Xmax $Ymin $Zmin" >> coordinates.txt
echo "$nB, $Xmax, $Ymin, $Zmin">> servolink.txt
else
echo "$B" >> corners.txt
fi

C=$(awk -v Xmax=$Xmax -v Ymax=$Ymax -v Zmin=$Zmin '($2==Xmax && $3==Ymax && $4==Zmin){print $0}' coordinates.txt) #Point C
if [[ -z "$C" ]];
then
nC=$(echo "$nCoords+3" |bc -l)
echo "$nC $Xmax $Ymax $Zmin" >> corners.txt
echo "$nC $Xmax $Ymax $Zmin" >> coordinates.txt
echo "$nC, $Xmax, $Ymax, $Zmin">> servolink.txt
else
echo "$C" >> corners.txt
fi

D=$(awk -v Xmin=$Xmin -v Ymax=$Ymax -v Zmin=$Zmin '($2==Xmin && $3==Ymax && $4==Zmin){print $0}' coordinates.txt) #Point D
if [[ -z "$D" ]];
then
nD=$(echo "$nCoords+4" |bc -l)
echo "$nD $Xmin $Ymax $Zmin" >> corners.txt
echo "$nD $Xmin $Ymax $Zmin" >> coordinates.txt
echo "$nD, $Xmin, $Ymax, $Zmin" >> servolink.txt
else
echo "$D" >> corners.txt
fi

Ap=$(awk -v Xmin=$Xmin -v Ymin=$Ymin -v Zmax=$Zmax '($2==Xmin && $3==Ymin && $4==Zmax){print $0}' coordinates.txt) #Point Ap
if [[ -z "$Ap" ]];
then
nAp=$(echo "$nCoords+5" |bc -l)
echo "$nAp $Xmin $Ymin $Zmax" >> corners.txt
echo "$nAp $Xmin $Ymin $Zmax" >> coordinates.txt
echo "$nAp, $Xmin, $Ymin, $Zmax" >> servolink.txt
else
echo "$Ap" >> corners.txt
fi

Bp=$(awk -v Xmax=$Xmax -v Ymin=$Ymin -v Zmax=$Zmax '($2==Xmax && $3==Ymin && $4==Zmax){print $0}' coordinates.txt) #Point Bp
if [[ -z "$Bp" ]];
then
nBp=$(echo "$nCoords+6" |bc -l)
echo "$nBp $Xmax $Ymin $Zmax" >> corners.txt
echo "$nBp $Xmax $Ymin $Zmax" >> coordinates.txt
echo "$nBp, $Xmax, $Ymin, $Zmax">> servolink.txt
else
echo "$Bp" >> corners.txt
fi

Cp=$(awk -v Xmax=$Xmax -v Ymax=$Ymax -v Zmax=$Zmax '($2==Xmax && $3==Ymax && $4==Zmax){print $0}' coordinates.txt) #Point Cp
if [[ -z "$Cp" ]];
then
nCp=$(echo "$nCoords+7" |bc -l)
echo "$nCp $Xmax $Ymax $Zmax" >> corners.txt
echo "$nCp $Xmax $Ymax $Zmax" >> coordinates.txt
echo "$nCp, $Xmax, $Ymax, $Zmax" >> servolink.txt
else
echo "$Cp" >> corners.txt
fi

Dp=$(awk -v Xmin=$Xmin -v Ymax=$Ymax -v Zmax=$Zmax '($2==Xmin && $3==Ymax && $4==Zmax){print $0}' coordinates.txt) #Point Dp
if [[ -z "$Dp" ]];
then
nDp=$(echo "$nCoords+8" |bc -l)
echo "$nDp $Xmin $Ymax $Zmax" >> corners.txt
echo "$nDp $Xmin $Ymax $Zmax" >> coordinates.txt
echo "$nDp, $Xmin, $Ymax, $Zmax" >> servolink.txt
else
echo "$Dp" >> corners.txt
fi


# format (td rd1 rd2)
nC=$(awk 'NR==3{print $1}' corners.txt)
nD=$(awk 'NR==4{print $1}' corners.txt)
nB=$(awk 'NR==2{print $1}' corners.txt)
echo "$nC $nD $nB" > connectivity.txt

nCp=$(awk 'NR==7{print $1}' corners.txt)
nDp=$(awk 'NR==8{print $1}' corners.txt)
nB=$(awk 'NR==2{print $1}' corners.txt)
echo "$nCp $nDp $nB" >> connectivity.txt

nBp=$(awk 'NR==6{print $1}' corners.txt)
nAp=$(awk 'NR==5{print $1}' corners.txt)
nB=$(awk 'NR==2{print $1}' corners.txt)
echo "$nBp $nAp $nB" >> connectivity.txt

nDp=$(awk 'NR==8{print $1}' corners.txt)
nAp=$(awk 'NR==5{print $1}' corners.txt)
nD=$(awk 'NR==4{print $1}' corners.txt)
echo "$nDp $nAp $nD" >> connectivity.txt
nA=$(awk 'NR==1{print $1}' corners.txt)

#To take into account cellule with the corner nodes not on the corners


#echo $nA, $nB, $nC, $nD, $nAp, $nBp, $nCp, $nDp
#Edges inner nodes

awk -v nA=$nA -v nB=$nB -v nC=$nC -v nD=$nD -v nAp=$nAp -v nBp=$nBp -v nCp=$nCp -v nDp=$nDp  '$1!=nA && $1!=nB && $1!=nC && $1!=nD && $1!=nAp && $1!=nBp && $1!=nCp && $1!=nDp{print $1, $2, $3, $4}' coordinates.txt > CoordinatesSansCoins.txt


#NodesString='-v nA='"$nA"" -v nB=$nB -v nC=$nC -v nD=$nD -v nAp=$nAp -v nBp=$nBp -v nCp=$nCp -v nDp=$nDp"
#NotNodesString="$1!=$nA && $1!=$nB && $1!=$nC && $1!=$nD && $1!=$nAp && $1!=$nBp && $1!=$nCp && $1!=$nDp"
#StringAwk="$NodesString "'-v Zmin=$Zmin -v Xmin=$Xmin''$2==Xmin && $4==Zmin{print $1, $2, $3, $4}' 

epsi="0"
#epsi="0"

CorrespondanceX(){
file1="$1"
file2="$2"
size1=$(cat "$file1" |wc -l)
size2=$(cat "$file2" |wc -l)
#Size2Temoins="$size2"
file2Temoin="${file2%.txt}Temoin.txt"
cat "$file2" > $file2Temoin
for ((ii=1; ii<=size1; ii++, size2--))
do
        for ((jj=1; jj<=size2; jj++))
        do
                a=$(awk -v ii=$ii 'NR==ii{print $0}' $file1)
                b=$(awk -v jj=$jj 'NR==jj{print $0}' $file2Temoin)
                a=$(echo "$a" |sed -e s/e/E/g)
                b=$(echo "$b" |sed -e s/e/E/g)
                ax=$(echo "$a"|awk '{print $2}')
                az=$(echo "$a"|awk '{print $4}')
                bx=$(echo "$b"|awk '{print $2}')
                bz=$(echo "$b"|awk '{print $4}')
                #condx1=$(echo "($ax==$bx)" |bc -l)
                #condx1=$(awk -v ax=$ax -v bx=$bx -v epsi=$epsi 'BEGIN{print ((ax-bx)<=epsi || (bx-ax)<=epsi )}')
                
                axbx=$(awk -v ax=$ax -v bx=$bx 'BEGIN{print (ax-bx)}')
                axbx=$(echo -e "$axbx" |sed -e s/^-//g )
                condx1=$(awk -v axbx=$axbx -v epsi=$epsi 'BEGIN{print ((axbx)<=epsi ) }')
                
   		 if (( $condx1 ));
                then
                tn=$(echo "$a"| awk '{print $1}')
                rt1=$(echo "$b"| awk '{print $1}')
                #condx2=$(echo "($az==$bz)" |bc -l)
                #condx2=$(awk -v az=$az -v bz=$bz -v epsi=$epsi 'BEGIN{print ((az-bz)<=epsi || (bz-az)<=epsi )}')            
                azbz=$(awk -v az=$az -v bz=$bz 'BEGIN{print (az-bz)}')
                azbz=$(echo -e "$azbz" |sed -e s/^-//g )
                condx2=$(awk -v azbz=$azbz -v epsi=$epsi 'BEGIN{print ((azbz)<=epsi ) }')    
                	if (( $condx2 ));
                	then
                	rt2="$nD"
                	else
                	rt2="$nAp"
                	fi
                echo "$tn $rt1 $rt2"
                #size2=$(echo "$size2-1"| bc -l)
                #continue 1
                break
                else
                continue
                fi         
        done
       sed "$jj d" $file2Temoin >${file2%.txt}Temp.txt && cat ${file2%.txt}Temp.txt > $file2Temoin && rm ${file2%.txt}Temp.txt
       #size2=$(cat $file2Temoin |wc -l)           
done
rm $file2Temoin
}

CorrespondanceY(){
file1="$1"
file2="$2"
size1=$(cat "$file1" |wc -l)
size2=$(cat "$file2" |wc -l)
file2Temoin="${file2%.txt}Temoin.txt"
cat "$file2" > $file2Temoin
for ((ii=1; ii<=size1; ii++, size2--))
do
        for ((jj=1; jj<=size2 ; jj++))
        do
                a=$(awk -v ii=$ii 'NR==ii{print $0}' $file1)
                b=$(awk -v jj=$jj 'NR==jj{print $0}' $file2Temoin)
                a=$(echo "$a" |sed -e s/e/E/g)
                b=$(echo "$b" |sed -e s/e/E/g)
                ay=$(echo "$a"|awk '{print $3}')
                az=$(echo "$a"|awk '{print $4}')
                by=$(echo "$b"|awk '{print $3}')
                bz=$(echo "$b"|awk '{print $4}')
                #condy1=$(echo "($ay==$by)" |bc -l)
                #condy1=$(awk -v ay=$ay -v by=$by -v epsi=$epsi 'BEGIN{print ((ay-by)<=epsi || (by-ay)<=epsi )}')
                              
                ayby=$(awk -v ay=$ay -v by=$by 'BEGIN{print (ay-by)}')
                ayby=$(echo -e "$ayby" |sed -e s/^-//g )
                condy1=$(awk -v ayby=$ayby -v epsi=$epsi 'BEGIN{print ((ayby)<=epsi ) }')   
                         
                if (( $condy1 ));
                then
                tn=$(echo "$a"| awk '{print $1}')
                rt1=$(echo "$b"| awk '{print $1}')
                #condy2=$(echo "($az==$bz)" |bc -l)
                #condy2=$(awk -v az=$az -v bz=$bz -v epsi=$epsi 'BEGIN{print ((az-bz)<=epsi || (bz-az)<=epsi )}')               
                azbz=$(awk -v az=$az -v bz=$bz 'BEGIN{print (az-bz)}')
                azbz=$(echo -e "$azbz" |sed -e s/^-//g )
                condy2=$(awk -v azbz=$azbz -v epsi=$epsi 'BEGIN{print ((azbz)<=epsi ) }')              
                if (( $condy2 ));
                then
                rt2="$nB"
                else
                rt2="$nAp"
                fi
                echo "$tn $rt1 $rt2"
                break
                else
                continue
                fi
       done
       sed "$jj d" $file2Temoin >${file2%.txt}Temp.txt && cat ${file2%.txt}Temp.txt > $file2Temoin && rm ${file2%.txt}Temp.txt        
done
rm $file2Temoin
}


CorrespondanceZ(){
file1="$1"
file2="$2"
size1=$(cat "$file1" |wc -l)
size2=$(cat "$file2" |wc -l)
file2Temoin="${file2%.txt}Temoin.txt"
cat "$file2" > $file2Temoin
for ((ii=1; ii<=size1; ii++, size2--))
do
        for ((jj=1; jj<=size2 ; jj++))
        do
                a=$(awk -v ii=$ii 'NR==ii{print $0}' $file1)
                b=$(awk -v jj=$jj 'NR==jj{print $0}' $file2Temoin)
                a=$(echo "$a" |sed -e s/e/E/g)
                b=$(echo "$b" |sed -e s/e/E/g)
                ay=$(echo "$a"|awk '{print $3}')
                az=$(echo "$a"|awk '{print $4}')
                by=$(echo "$b"|awk '{print $3}')
                bz=$(echo "$b"|awk '{print $4}')
                #condz1=$(echo "($az==$bz)" |bc -l)
                #condz1=$(awk -v az=$az -v bz=$bz -v epsi=$epsi 'BEGIN{print ((az-bz)<=epsi || (bz-az)<=epsi )}')     
                          
                azbz=$(awk -v az=$az -v bz=$bz 'BEGIN{print (az-bz)}')
                azbz=$(echo -e "$azbz" |sed -e s/^-//g )
                condz1=$(awk -v azbz=$azbz -v epsi=$epsi 'BEGIN{print ((azbz)<=epsi ) }')
                
                if (( $condz1 ));
                then
                tn=$(echo "$a"| awk '{print $1}')
                rt1=$(echo "$b"| awk '{print $1}')
                #condz2=$(echo "($ay==$by)" |bc -l)
                #condz2=$(awk -v ay=$ay -v by=$by -v epsi=$epsi 'BEGIN{print ((ay-by)<=epsi || (by-ay)<=epsi )}')                
                ayby=$(awk -v ay=$ay -v by=$by 'BEGIN{print (ay-by)}')
                ayby=$(echo -e "$ayby" |sed -e s/^-//g )
                condz2=$(awk -v ayby=$ayby -v epsi=$epsi 'BEGIN{print ((ayby)<=epsi ) }')                                   
                if (( $condz2 ));
                then
                rt2="$nB"
                else
                rt2="$nD"
                fi
                echo "$tn $rt1 $rt2"
                break
                else
                continue
                fi
        done
       sed "$jj d" $file2Temoin >${file2%.txt}Temp.txt && cat ${file2%.txt}Temp.txt > $file2Temoin && rm ${file2%.txt}Temp.txt        
done
rm $file2Temoin
}


CorrespondanceXF(){
file1="$1"
file2="$2"
size1=$(cat "$file1" |wc -l)
size2=$(cat "$file2" |wc -l)
file2Temoin="${file2%.txt}Temoin.txt"
cat "$file2" > $file2Temoin
for ((ii=1; ii<=size1; ii++, size2--))
do
        for ((jj=1; jj<=size2 ; jj++))

        do
                a=$(awk -v ii=$ii 'NR==ii{print $0}' $file1)
                b=$(awk -v jj=$jj 'NR==jj{print $0}' $file2Temoin)
                a=$(echo "$a" |sed -e s/e/E/g)
                b=$(echo "$b" |sed -e s/e/E/g)
                ay=$(echo "$a"|awk '{print $3}')
                az=$(echo "$a"|awk '{print $4}')
                by=$(echo "$b"|awk '{print $3}')
                bz=$(echo "$b"|awk '{print $4}')
                #condx=$(echo "($ay==$by)&&($az==$bz)" |bc -l)
                #condx=$(awk -v ay=$ay -v by=$by -v az=$az -v bz=$bz -v epsi=$epsi 'BEGIN{print (((ay-by)<=epsi || (by-ay)<=epsi ) && ((az-bz)<=epsi || (bz-az)<=epsi ))}')
                
                ayby=$(awk -v ay=$ay -v by=$by 'BEGIN{print (ay-by)}')
                ayby=$(echo -e "$ayby" |sed -e s/^-//g )
                azbz=$(awk -v az=$az -v bz=$bz 'BEGIN{print (az-bz)}')
                azbz=$(echo -e "$azbz" |sed -e s/^-//g )
                
                
                condx=$(awk -v ayby=$ayby -v azbz=$azbz -v epsi=$epsi 'BEGIN{print ( (ayby)<=epsi && (azbz)<=epsi ) }')   
                
                if (( $condx ));
                then
                tn=$(echo "$a"| awk '{print $1}')
                rt1=$(echo "$b"| awk '{print $1}')               
                rt2="$nB"
                echo "$tn $rt1 $rt2"
                sed "$jj d" $file2Temoin >${file2%.txt}Temp.txt && cat ${file2%.txt}Temp.txt > $file2Temoin && rm ${file2%.txt}Temp.txt  
                #break
                continue 2
                else
                continue 1
                fi
        done
      # sed "$jj d" $file2Temoin >${file2%.txt}Temp.txt && cat ${file2%.txt}Temp.txt > $file2Temoin && rm ${file2%.txt}Temp.txt         
done
rm $file2Temoin
}

CorrespondanceYF(){
file1="$1"
file2="$2"
size1=$(cat "$file1" |wc -l)
size2=$(cat "$file2" |wc -l)
file2Temoin="${file2%.txt}Temoin.txt"
cat "$file2" > $file2Temoin
for ((ii=1; ii<=size1; ii++, size2--))
do
        for ((jj=1; jj<=size2 ; jj++))
        do
                a=$(awk -v ii=$ii 'NR==ii{print $0}' $file1)
                b=$(awk -v jj=$jj 'NR==jj{print $0}' $file2Temoin)
                a=$(echo "$a" |sed -e s/e/E/g)
                b=$(echo "$b" |sed -e s/e/E/g)
                ax=$(echo "$a"|awk '{print $2}')
                az=$(echo "$a"|awk '{print $4}')
                bx=$(echo "$b"|awk '{print $2}')
                bz=$(echo "$b"|awk '{print $4}')
                #condy=$(echo "($ax==$bx)&&($az==$bz)" |bc -l)
                #condy=$(awk -v ax=$ax -v bx=$bx -v az=$az -v bz=$bz -v epsi=$epsi 'BEGIN{print (((ax-bx)<=epsi || (bx-ax)<=epsi ) && ((az-bz)<=epsi || (bz-az)<=epsi ))}')
                
                axbx=$(awk -v ax=$ax -v bx=$bx 'BEGIN{print (ax-bx)}')
                axbx=$(echo -e "$axbx" |sed -e s/^-//g )
                azbz=$(awk -v az=$az -v bz=$bz 'BEGIN{print (az-bz)}')
                azbz=$(echo -e "$azbz" |sed -e s/^-//g )
                
                condy=$(awk -v axbx=$axbx -v azbz=$azbz -v epsi=$epsi 'BEGIN{print ( (axbx)<=epsi && (azbz)<=epsi ) }')
                         
                if (( $condy ));
                then
                tn=$(echo "$a"| awk '{print $1}')
                rt1=$(echo "$b"| awk '{print $1}')               
                rt2="$nD"
                echo "$tn $rt1 $rt2"
                break
                else
                continue
                fi
        done
       sed "$jj d" $file2Temoin >${file2%.txt}Temp.txt && cat ${file2%.txt}Temp.txt > $file2Temoin && rm ${file2%.txt}Temp.txt     
done
rm $file2Temoin
}

CorrespondanceZF(){
file1="$1"
file2="$2"
size1=$(cat "$file1" |wc -l)
size2=$(cat "$file2" |wc -l)
file2Temoin="${file2%.txt}Temoin.txt"
cat "$file2" > $file2Temoin
for ((ii=1; ii<=size1; ii++, size2--))
do
        for ((jj=1; jj<=size2 ; jj++))
        do
                a=$(awk -v ii=$ii 'NR==ii{print $0}' $file1)
                b=$(awk -v jj=$jj 'NR==jj{print $0}' $file2Temoin)
                a=$(echo "$a" |sed -e s/e/E/g)
                b=$(echo "$b" |sed -e s/e/E/g)
                ax=$(echo "$a"|awk '{print $2}')
                ay=$(echo "$a"|awk '{print $3}')
                bx=$(echo "$b"|awk '{print $2}')
                by=$(echo "$b"|awk '{print $3}')
                #condz=$(echo "($ax==$bx)&&($ay==$by)" |bc -l)
                #condz=$(awk -v ax=$ax -v bx=$bx -v ay=$ay -v by=$by -v epsi=$epsi 'BEGIN{print (((ax-bx)<=epsi || (bx-ax)<=epsi ) && ((ay-by)<=epsi || (by-ay)<=epsi ))}')
                
                axbx=$(awk -v ax=$ax -v bx=$bx 'BEGIN{print (ax-bx)}')
                axbx=$(echo -e "$axbx" |sed -e s/^-//g )
                azbz=$(awk -v az=$az -v bz=$bz 'BEGIN{print (az-bz)}')
                azbz=$(echo -e "$azbz" |sed -e s/^-//g )
                
                condz=$(awk -v axbx=$axbx -v ayby=$ayby -v epsi=$epsi 'BEGIN{print ( (axbx)<=epsi && (ayby)<=epsi ) }')
                          
                if (( $condz ));
                then
                tn=$(echo "$a"| awk '{print $1}')
                rt1=$(echo "$b"| awk '{print $1}')               
                rt2="$nAp"
                echo "$tn $rt1 $rt2"
                break
                else
                continue
                fi
        done
       sed "$jj d" $file2Temoin >${file2%.txt}Temp.txt && cat ${file2%.txt}Temp.txt > $file2Temoin && rm ${file2%.txt}Temp.txt         
done
rm $file2Temoin
}
#Rotaion x-axis
awk -v nA=$nA -v nB=$nB -v nC=$nC -v nD=$nD -v nAp=$nAp -v nBp=$nBp -v nCp=$nCp -v nDp=$nDp -v Zmin=$Zmin -v Ymin=$Ymin '$3==Ymin && $4==Zmin{print $1, $2, $3, $4}' CoordinatesSansCoins.txt >YminZmin.txt
awk -v nA=$nA -v nB=$nB -v nC=$nC -v nD=$nD -v nAp=$nAp -v nBp=$nBp -v nCp=$nCp -v nDp=$nDp -v Zmin=$Zmin -v Ymax=$Ymax '$3==Ymax && $4==Zmin{print $1, $2, $3, $4}' CoordinatesSansCoins.txt >YmaxZmin.txt
awk -v nA=$nA -v nB=$nB -v nC=$nC -v nD=$nD -v nAp=$nAp -v nBp=$nBp -v nCp=$nCp -v nDp=$nDp -v Zmax=$Zmax -v Ymax=$Ymax '$3==Ymax && $4==Zmax{print $1, $2, $3, $4}' CoordinatesSansCoins.txt >YmaxZmax.txt
awk -v nA=$nA -v nB=$nB -v nC=$nC -v nD=$nD -v nAp=$nAp -v nBp=$nBp -v nCp=$nCp -v nDp=$nDp -v Zmax=$Zmax -v Ymin=$Ymin '$3==Ymin && $4==Zmax{print $1, $2, $3, $4}' CoordinatesSansCoins.txt >YminZmax.txt

#Rotation y-axix
awk -v nA=$nA -v nB=$nB -v nC=$nC -v nD=$nD -v nAp=$nAp -v nBp=$nBp -v nCp=$nCp -v nDp=$nDp -v Xmin=$Xmin -v Zmin=$Zmin '$2==Xmin && $4==Zmin{print $1, $2, $3, $4}' CoordinatesSansCoins.txt >XminZmin.txt
awk -v nA=$nA -v nB=$nB -v nC=$nC -v nD=$nD -v nAp=$nAp -v nBp=$nBp -v nCp=$nCp -v nDp=$nDp -v Xmax=$Xmax -v Zmin=$Zmin '$2==Xmax && $4==Zmin{print $1, $2, $3, $4}' CoordinatesSansCoins.txt >XmaxZmin.txt
awk -v nA=$nA -v nB=$nB -v nC=$nC -v nD=$nD -v nAp=$nAp -v nBp=$nBp -v nCp=$nCp -v nDp=$nDp -v Xmax=$Xmax -v Zmax=$Zmax '$2==Xmax && $4==Zmax{print $1, $2, $3, $4}' CoordinatesSansCoins.txt >XmaxZmax.txt
awk -v nA=$nA -v nB=$nB -v nC=$nC -v nD=$nD -v nAp=$nAp -v nBp=$nBp -v nCp=$nCp -v nDp=$nDp -v Xmin=$Xmin -v Zmax=$Zmax '$2==Xmin && $4==Zmax{print $1, $2, $3, $4}' CoordinatesSansCoins.txt >XminZmax.txt

#Rotation z-axix
awk -v nA=$nA -v nB=$nB -v nC=$nC -v nD=$nD -v nAp=$nAp -v nBp=$nBp -v nCp=$nCp -v nDp=$nDp -v Xmin=$Xmin -v Ymin=$Ymin '$2==Xmin && $3==Ymin{print $1, $2, $3, $4}' CoordinatesSansCoins.txt >XminYmin.txt
awk -v nA=$nA -v nB=$nB -v nC=$nC -v nD=$nD -v nAp=$nAp -v nBp=$nBp -v nCp=$nCp -v nDp=$nDp -v Xmax=$Xmax -v Ymin=$Ymin '$2==Xmax && $3==Ymin{print $1, $2, $3, $4}' CoordinatesSansCoins.txt >XmaxYmin.txt
awk -v nA=$nA -v nB=$nB -v nC=$nC -v nD=$nD -v nAp=$nAp -v nBp=$nBp -v nCp=$nCp -v nDp=$nDp -v Xmax=$Xmax -v Ymax=$Ymax '$2==Xmax && $3==Ymax{print $1, $2, $3, $4}' CoordinatesSansCoins.txt >XmaxYmax.txt
awk -v nA=$nA -v nB=$nB -v nC=$nC -v nD=$nD -v nAp=$nAp -v nBp=$nBp -v nCp=$nCp -v nDp=$nDp -v Xmin=$Xmin -v Ymax=$Ymax '$2==Xmin && $3==Ymax{print $1, $2, $3, $4}' CoordinatesSansCoins.txt >XminYmax.txt

# Noeuds internes faces X
awk -v nA=$nA -v nB=$nB -v nC=$nC -v nD=$nD -v nAp=$nAp -v nBp=$nBp -v nCp=$nCp -v nDp=$nDp -v Xmin=$Xmin -v Ymin=$Ymin -v Ymax=$Ymax -v Zmin=$Zmin -v Zmax=$Zmax '$2==Xmin && $3!=Ymax && $3!=Ymin && $4!=Zmin && $4!=Zmax {print $1, $2, $3, $4}' CoordinatesSansCoins.txt >XminF.txt
awk -v nA=$nA -v nB=$nB -v nC=$nC -v nD=$nD -v nAp=$nAp -v nBp=$nBp -v nCp=$nCp -v nDp=$nDp -v Xmax=$Xmax -v Ymin=$Ymin -v Ymax=$Ymax -v Zmin=$Zmin -v Zmax=$Zmax '$2==Xmax && $3!=Ymax && $3!=Ymin && $4!=Zmin && $4!=Zmax {print $1, $2, $3, $4}' CoordinatesSansCoins.txt >XmaxF.txt

# Noeuds internes faces Y
awk -v nA=$nA -v nB=$nB -v nC=$nC -v nD=$nD -v nAp=$nAp -v nBp=$nBp -v nCp=$nCp -v nDp=$nDp -v Xmin=$Xmin -v Xmax=$Xmax -v Ymin=$Ymin -v Ymax=$Ymax -v Zmin=$Zmin -v Zmax=$Zmax '$3==Ymin && $2!=Xmin && $2!=Xmax && $4!=Zmin && $4!=Zmax {print $1, $2, $3, $4}' CoordinatesSansCoins.txt >YminF.txt
awk -v nA=$nA -v nB=$nB -v nC=$nC -v nD=$nD -v nAp=$nAp -v nBp=$nBp -v nCp=$nCp -v nDp=$nDp -v Xmin=$Xmin -v Xmax=$Xmax -v Ymin=$Ymin -v Ymax=$Ymax -v Zmin=$Zmin -v Zmax=$Zmax '$3==Ymax && $2!=Xmin && $2!=Xmax && $4!=Zmin && $4!=Zmax {print $1, $2, $3, $4}' CoordinatesSansCoins.txt >YmaxF.txt

# Noeuds internes faces Z
awk -v nA=$nA -v nB=$nB -v nC=$nC -v nD=$nD -v nAp=$nAp -v nBp=$nBp -v nCp=$nCp -v nDp=$nDp -v Xmin=$Xmin -v Xmax=$Xmax -v Ymin=$Ymin -v Ymax=$Ymax -v Zmin=$Zmin -v Zmax=$Zmax '$4==Zmin && $2!=Xmin && $2!=Xmax && $3!=Ymin && $3!=Ymax {print $1, $2, $3, $4}' CoordinatesSansCoins.txt >ZminF.txt
awk -v nA=$nA -v nB=$nB -v nC=$nC -v nD=$nD -v nAp=$nAp -v nBp=$nBp -v nCp=$nCp -v nDp=$nDp -v Xmin=$Xmin -v Xmax=$Xmax -v Ymin=$Ymin -v Ymax=$Ymax -v Zmin=$Zmin -v Zmax=$Zmax '$4==Zmax && $2!=Xmin && $2!=Xmax && $3!=Ymin && $3!=Ymax {print $1, $2, $3, $4}' CoordinatesSansCoins.txt >ZmaxF.txt


XminDonnees=$(awk -v Xmin=$Xmin '$2==Xmin{print $0}' CoordinatesSansCoins.txt | wc -l)
XmaxDonnees=$(awk -v Xmax=$Xmax '$2==Xmax{print $0}' CoordinatesSansCoins.txt | wc -l)

YminDonnees=$(awk -v Ymin=$Ymin '$3==Ymin{print $0}' CoordinatesSansCoins.txt | wc -l)
YmaxDonnees=$(awk -v Ymax=$Ymax '$3==Ymax{print $0}' CoordinatesSansCoins.txt | wc -l)

ZminDonnees=$(awk -v Zmin=$Zmin '$4==Zmin{print $0}' CoordinatesSansCoins.txt | wc -l)
ZmaxDonnees=$(awk -v Zmax=$Zmax '$4==Zmax{print $0}' CoordinatesSansCoins.txt | wc -l)

EdgeNumberX=$(awk -v Ymin=$Ymin -v Zmin=$Zmin '($3==Ymin && $4==Zmin){print $0}' CoordinatesSansCoins.txt | wc -l)
EdgeNumberY=$(awk -v Xmin=$Xmin -v Zmin=$Zmin '($2==Xmin && $4==Zmin){print $0}' CoordinatesSansCoins.txt | wc -l)
EdgeNumberZ=$(awk -v Xmin=$Xmin -v Ymin=$Ymin '($2==Xmin && $3==Ymin){print $0}' CoordinatesSansCoins.txt | wc -l)

ll1=$(cat "YminZmin.txt" |wc -l)
ll2=$(cat "XminZmin.txt" |wc -l)
ll3=$(cat "XminYmin.txt" |wc -l)
ll4=$(cat "XminF.txt" |wc -l)
ll5=$(cat "YminF.txt" |wc -l)
ll6=$(cat "ZminF.txt" |wc -l)

ll=$(echo "3*(4+3*($ll1+$ll2+$ll3)+$ll4+$ll5+$ll6)" |bc -l)
NbLinks="$ll"
#echo $ll6
echo -e "Nombre de servo link : \e[0;44m$ll\e[0;m"

#echo $Xmin, $Xmax, $Ymin, $Ymax, $Zmin, $Zmax
echo "Domaine : [$Xmin : $Xmax]x[$Ymin : $Ymax]x[$Zmin : $Zmax]"
echo "Nombre de noeuds sur les faces : $XminDonnees $YminDonnees $ZminDonnees"
echo "Nombre de noeuds sur les segments de bords : $EdgeNumberX $EdgeNumberY $EdgeNumberZ"
#echo $XminDonnees, $XmaxDonnees, $YminDonnees, $YmaxDonnees, $ZminDonnees, $ZmaxDonnees
#echo $EdgeNumberX, $EdgeNumberY, $EdgeNumberZ

#Rotaion x-axis (edge notes in x direction)
dos2unix YminZmin.txt YmaxZmin.txt YminZmax.txt YmaxZmax.txt YminZmin.txt YminZmax.txt 2>/dev/null
CorrespondanceX YmaxZmin.txt YminZmin.txt>>connectivity.txt
CorrespondanceX YmaxZmax.txt YminZmax.txt>>connectivity.txt
CorrespondanceX YminZmax.txt YminZmin.txt>>connectivity.txt

#Rotation y-axix (edge notes in y direction)
dos2unix XminZmin.txt XmaxZmin.txt XmaxZmax.txt XminZmax.txt 2>/dev/null
CorrespondanceY XmaxZmin.txt XminZmin.txt>>connectivity.txt
CorrespondanceY XmaxZmax.txt XminZmax.txt>>connectivity.txt
CorrespondanceY XminZmax.txt XminZmin.txt>>connectivity.txt     

#Rotation z-axix (edge notes in x direction)
dos2unix XminYmin.txt XmaxYmin.txt XmaxYmax.txt XminYmax.txt 2>/dev/null
CorrespondanceZ XmaxYmin.txt XminYmin.txt>>connectivity.txt
CorrespondanceZ XmaxYmax.txt XminYmax.txt>>connectivity.txt
CorrespondanceZ XminYmax.txt XminYmin.txt>>connectivity.txt


# Noeuds internes faces X
dos2unix XminF.txt XmaxF.txt 2>/dev/null
CorrespondanceXF XmaxF.txt XminF.txt >> connectivity.txt
#CorrespondanceXF XmaxF.txt XminF.txt >>connectivity.txt
#cat bam.txt >> connectivity.txt


# Noeuds internes faces Y
dos2unix YminF.txt YmaxF.txt 2>/dev/null
CorrespondanceYF YmaxF.txt YminF.txt >>connectivity.txt

# Noeuds internes faces Z
dos2unix ZminF.txt ZmaxF.txt 2>/dev/null
CorrespondanceZF ZmaxF.txt ZminF.txt >>connectivity.txt

:<<'end'
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------      Génération des fichier servolink ---------------------------------------------------------
end

#NbLinks=$(echo "3*(  4+3*(($EdgeNumberX-2)+($EdgeNumberY-2)+($EdgeNumberZ-2))+($EdgeNumberX-2)*($EdgeNumberY-2)+($EdgeNumberX-2)*($EdgeNumberZ-2)+($EdgeNumberY-2)*($EdgeNumberZ-2)  )" |bc -l)

#NbLinks=$(echo "3*(  4+3*(($EdgeNumberX-2)+($EdgeNumberY-2)+($EdgeNumberZ-2))+($XminDonnees-2*$EdgeNumberY-2*$EdgeNumberZ+4)+($YminDonnees-2*$EdgeNumberX-2*$EdgeNumberZ+4)+($ZminDonnees-2*$EdgeNumberY-2*$EdgeNumberX+4) )" |bc -l)

#: > servolink.txt
echo "servo link, $NbLinks, 0, 1" >> servolink.txt

:>motif.txt
echo -e "2, Dir, tn, Dir, rd1, Dir, rd2\n1, 1\n0" > motif.txt
nl=$(cat connectivity.txt |wc -l)
for ((ii=1; ii<=3; ii++))
do
	for ((jj=1; jj<=nl; jj++))
	do
	Dir="$ii"
	tn=$(awk -v jj=$jj 'NR==jj{print $1}' connectivity.txt)
	rd1=$(awk -v jj=$jj 'NR==jj{print $2}' connectivity.txt)
	rd2=$(awk -v jj=$jj 'NR==jj{print $3}' connectivity.txt)
	sed -e s/Dir/$Dir/g motif.txt | sed -e s/tn/$tn/g |sed -e s/rd1/$rd1/g |sed -e s/rd2/$rd2/g >>servolink.txt
	done
done
awk -v Sup=$Sup 'NR<=Sup{print $0}' $filename >"${filename%.dat}pbc.dat"
cat servolink.txt >>"${filename%.dat}pbc.dat"
awk -v Sup=$Sup 'NR>Sup{print $0}' $filename>>"${filename%.dat}pbc.dat"
echo -e "Fichier de calcul Marc MSC avec les conditions de bords périodiques : \e[0;32m${filename%.dat}pbc.dat\e[0;m"          
echo -e "(Voir dans `pwd`)"
#rm coordinates.txt  connectivity.txt
rm XmaxF.txt XmaxYmax.txt XmaxYmin.txt XmaxZmax.txt XmaxZmin.txt XminF.txt XminYmax.txt XminYmin.txt XminZmax.txt XminZmin.txt YmaxF.txt YmaxZmax.txt YmaxZmin.txt YminF.txt YminZmax.txt YminZmin.txt ZmaxF.txt ZminF.txt
rm CoordinatesSansCoins.txt motif.txt servolink.txt col1.txt col2.txt col3.txt col4.txt coordinates1.txt coordinatesTemp.txt

#rm corners.txt 
