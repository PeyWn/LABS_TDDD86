/**********************************************************************
 *  Mönsterigenkänning readme.txt
 **********************************************************************/

/**********************************************************************
 *  Empirisk    Fyll i tabellen nedan med riktiga körtider i sekunder
 *  analys      när det känns vettigt att vänta på hela beräkningen.
 *              Ge uppskattningar av körtiden i övriga fall.
 *
 **********************************************************************/
    
      N       brute 	      sortering
 ----------------------------------
    150		46ms		12ms
    200		116ms		20ms
    300		322ms		50ms
    400		750ms		72ms
    800		5876ms		272ms
   1600		47280ms		1119ms
   3200		375615ms	4819ms
   6400		-		20317ms
  12800		-		84965ms


/**********************************************************************
 *  Teoretisk   Ge ordo-uttryck för värstafallstiden för programmen som
 *  analys      en funktion av N. Ge en kort motivering.
 *
 **********************************************************************/

Brute: O(N⁴) Fyra nästlade for-loopar, som går igenom N punkter i värsta fall.

Sortering:O(N²log(N)) Loopar över alla punkter och för varje punkt sorterar (Nlog(N)). insättning av N noder i ett set ger också O(NlogN). Alltså totalt O(N²logN).
